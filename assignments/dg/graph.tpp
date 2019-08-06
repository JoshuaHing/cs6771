#ifndef GRAPH_TPP
#define GRAPH_TPP

#include <algorithm>


template <typename N, typename E>
gdwg::Graph<N,E>::Graph (typename std::vector<N>::const_iterator begin, typename std::vector<N>::const_iterator end) noexcept {
    for (auto it = begin; it != end; ++it ) {
        nodes_.insert(std::make_shared<N>(Node{*it}));
    }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph (typename std::vector<std::tuple<N, N, E>>::const_iterator tup_begin, typename std::vector<std::tuple<N, N, E>>::const_iterator tup_end) noexcept {

    for (auto it = tup_begin; it != tup_end; ++it) {
        std::tuple<N, N, E> tup = *it;
        InsertNode(std::get<0>(tup));
        InsertNode(std::get<1>(tup));
        InsertEdge(std::get<0>(tup), std::get<1>(tup), std::get<2>(tup));
    }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph (std::initializer_list<N> list) noexcept {
    for (const auto& node_val : list) {
        InsertNode(node_val);
    }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(const gdwg::Graph<N, E>& g) noexcept {
    for (const auto& node : g.nodes_) {
        this->nodes_.insert(node);
    }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(gdwg::Graph<N, E>&& g) noexcept {
    this->nodes_ = std::move(g.nodes_);
}

template <typename N, typename E>
gdwg::Graph<N, E>& gdwg::Graph<N, E>::operator=(const gdwg::Graph<N, E>& g) noexcept {
    this->nodes_.clear();
    for (const auto& node : g.nodes_) {
        this->nodes_.insert(node);
    }
    return *this;
}

template <typename N, typename E>
gdwg::Graph<N, E>& gdwg::Graph<N, E>::operator=(gdwg::Graph<N, E>&& g)  noexcept {
    this->nodes_.clear();
    this->nodes_ = std::move(g.nodes_);
    return *this;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertNode(const N& val) noexcept {
    if (!IsNode(val)) {
        nodes_.insert({val, std::make_shared<Node>(Node{val})});
        return true;
    }
    return false;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertEdge(const N& src, const N& dst, const E& w) {
    if (!IsNode(src) || !IsNode(dst)) {
        throw std::runtime_error("Cannot call Graph::InsertEdge when either src or dst node does not exist");
    }
    return (nodes_[src]->AddEdge(nodes_[src], nodes_[dst], w));

}

template <typename N, typename E>
bool gdwg::Graph<N, E>::Node::AddEdge(std::shared_ptr<Node> src, std::shared_ptr<Node> dst, const E& w) {
    auto it = std::find_if(src->edges_.begin(), src->edges_.end(), [&src, &dst, &w](std::shared_ptr<Edge> e) { return (src == e->GetSource().lock() && dst == e->GetDest().lock() && w == e->GetValue()) ;});
    if (!(it == src->edges_.end())) return false;
    src->edges_.insert(std::make_shared<Edge>(Edge{src, dst, w}));
    return true;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::DeleteNode(const N& val) noexcept {
    if (IsNode(val)) {
        nodes_.erase(val);
        return true;
    }
    return false;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::Replace(const N& oldData, const N& newData) {
    if (!IsNode(oldData)) {
        throw std::runtime_error("Cannot call Graph::Replace on a node that doesn't exist");
    }
    if (IsNode(newData)) {
        return false;
    }

    // First, insert the edge and copy over the data
    InsertNode(newData);
    nodes_[newData]->SetEdges(nodes_[oldData]->GetEdges());

    for (const auto& node : nodes_) {
        for (const auto& edge : node.second->GetEdges()) {

            // If there is any instance of the old data in any of the edges, update
            // them to point to the new node
            if ((edge->GetSource().lock())->GetValue() == oldData) {
                edge->SetSource(nodes_[newData]);
            } else if ((edge->GetDest().lock())->GetValue() == oldData) {
                edge->SetDest(nodes_[newData]);
            }
        }
    }

    // Now that there aren't any hanging pointers, we can safely delete the old node
    DeleteNode(oldData);

    return true;

}

template <typename N, typename E>
void gdwg::Graph<N, E>::MergeReplace(const N& oldData, const N& newData) {
    if (!IsNode(oldData) || !IsNode(newData)) {
        throw std::runtime_error("Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
    }

    // Copy over old edge data
    nodes_[newData]->AddEdges(nodes_[oldData]->GetEdges());

    for (const auto& node : nodes_) {
        for (const auto& edge : node.second->GetEdges()) {

            // Add any of the outgoing edges of the old node to the new node, but with updated values
            if ((edge->GetSource().lock())->GetValue() == oldData) {
                InsertEdge(newData, (edge->GetDest().lock())->GetValue(), edge->GetValue());
                nodes_[newData]->GetEdges().erase(edge);
            } else if ((edge->GetDest().lock())->GetValue() == oldData) {
                InsertEdge((edge->GetSource().lock())->GetValue(), newData, edge->GetValue());
                nodes_[(edge->GetSource().lock())->GetValue()]->GetEdges().erase(edge);
            }
        }
    }

    // Now that there aren't any hanging pointers, we can safely delete the old node
    DeleteNode(oldData);

}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsNode(const N& val) const noexcept {
    if (nodes_.count(val)) {
        return true;
    }
    return false;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsConnected(const N& src, const N& dst) const {

    if (!IsNode(src) || !IsNode(dst)) {
        throw std::runtime_error("Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
    }

    if (IsNode(src)) {
        for (const auto& edge : nodes_[src]->GetEdges()) {
            if ((edge->GetDest().lock())->GetValue() == dst) {
                return true;
            }
        }
    }
    return false;
}

template <typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetNodes() const noexcept {
    std::vector<N> v;
    for (const auto& node : nodes_) {
        v.push_back(node.second->GetValue());
    }
    return v;
}

template <typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetConnected(const N& src) const {

    if (!IsNode(src)) {
        throw std::out_of_range("Cannot call Graph::GetConnected if src doesn't exist in the graph");
    }

    std::vector<N> conn;
    for (const auto& edge : nodes_.at(src)->GetEdges()) {
        conn.push_back((edge->GetDest().lock())->GetValue());
    }

    // Remove elements
    conn.erase(std::unique(conn.begin(), conn.end()), conn.end());

    return conn;
}

template <typename N, typename E>
std::vector<E> gdwg::Graph<N, E>::GetWeights(const N& src, const N& dst) const {
    if (!IsNode(src) || !IsNode(dst)) {
        throw std::out_of_range("Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
    }
    std::vector<E> weights;
    for (const auto& edge : nodes_.at(src)->GetEdges()) {
        if ((edge->GetDest().lock())->GetValue() == dst) {
            weights.push_back(edge->GetValue());
        }
    }
    return weights;

}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::find(const N& src, const N& dst, const E& w) const noexcept {
    return std::find_if(begin(), end(), [&src, &dst, &w] (const std::tuple<N, N, E> curr) {
      return (std::get<0>(curr) == src && std::get<1>(curr) == dst && std::get<2>(curr) == w);
    });
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::erase(const N& src, const N& dst, const E& w) noexcept {
    if (!IsNode(src)) {
        return false;
    }
    for (const auto& edge : nodes_[src]->GetEdges()) {
        if ((edge->GetDest().lock())->GetValue() == dst) {
            if (edge->GetValue() == w) {
                nodes_[src]->GetEdges().erase(edge);
                return true;
            }
        }
    }
    return false;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::erase(gdwg::Graph<N, E>::const_iterator it) noexcept {

    auto res = std::find_if(begin(), end(), [&it](auto curr) { return *it == curr; });
    if (res == end()) {
        return res;
    }
    res++;
    erase(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it));

    return res;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator::reference gdwg::Graph<N, E>::const_iterator::operator*() const  {
    // First check if node exists..?
    const N& a = (*edge_it_)->GetSource().lock()->GetValue();
    const N& b = (*edge_it_)->GetDest().lock()->GetValue();
    const E& c = (*edge_it_)->GetValue();
    return {a, b, c};
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator& gdwg::Graph<N, E>::const_iterator::operator++() {
    ++edge_it_;
    if (edge_it_ == node_it_->second->GetEdges().end()) {
        do {
            ++node_it_;
        } while (node_it_ != sentinel_ && node_it_->second->GetEdges().begin() == node_it_->second->GetEdges().end());
        if (node_it_ != sentinel_) {
            edge_it_ = node_it_->second->GetEdges().begin();
        }
    }

    return *this;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator& gdwg::Graph<N, E>::const_iterator::operator--() {
    // decrementing from end() on bi-directional iterator should return last element
    if (node_it_ == sentinel_) {
        node_it_ = last_node_;
    }
    // Traverse to the first node that contains an edge
    while (node_it_->second->GetEdges().size() == 0) {
        node_it_ = std::prev(node_it_);
        edge_it_ = node_it_->second->GetEdges().end();
    }

    // If edge iterator points to first edge of a node, it wraps around to the last
    // edge of the previous node
    if (edge_it_ == node_it_->second->GetEdges().begin()) {
        node_it_ = std::prev(node_it_);

        // Traverse to the first node that contains an edge, update iterator as you go
        while (node_it_->second->GetEdges().size() == 0) {
            node_it_ = std::prev(node_it_);
            edge_it_ = node_it_->second->GetEdges().end();
        }
        edge_it_ = node_it_->second->GetEdges().end();
        --edge_it_;
    } else {
        --edge_it_;
    }

    return *this;
}


template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cbegin() {
    // If node doesn't exist, return end()
    for (const auto& node : nodes_) {
        if (!node.second->GetEdges().empty()) {
            return {nodes_.begin(), nodes_.end(), std::prev(nodes_.end()), nodes_.begin()->second->GetEdges().begin()};
        }
    }
    return end();
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cend() {
    return {nodes_.end(), nodes_.end(), std::prev(nodes_.end()), nodes_.begin()->second->GetEdges().end()};
}


template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::begin() {
    return cbegin();
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::end() {
    return cend();
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::crbegin() {
    return const_reverse_iterator{cend()};
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::crend() {
    return const_reverse_iterator{cbegin()};
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::rbegin() {
    return crbegin();
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::rend() {
    return crend();
}



#endif
