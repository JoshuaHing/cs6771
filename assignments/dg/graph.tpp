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
    //auto src_it = std::find_if(nodes_.begin(), nodes_.end(), [&src](std::map<N, std::shared_ptr<Node>> n) { return (src == n.second->GetValue()); });
    if (!nodes_.count(src)) {
        throw std::runtime_error("Cannot call Graph::InsertEdge when either src or dst node does not exist");
    }
    if (!nodes_.count(dst)) {
        throw std::runtime_error("Cannot call Graph::InsertEdge when either src or dst node does not exist");
    }
    return (nodes_[src]->AddEdge(nodes_[src], nodes_[dst], w));

}

template <typename N, typename E>
bool gdwg::Graph<N, E>::Node::AddEdge(std::shared_ptr<Node> src, std::shared_ptr<Node> dst, const E& w) {
    auto it = std::find_if(src->edges_.begin(), src->edges_.end(), [&src, &dst, &w](std::shared_ptr<Edge> e) { return (src == e->GetSource().lock() && dst == e->GetDest().lock() && w == e->GetValue()) ;});
    if (!(it == src->edges_.end())) return false;
    //std::cout << "ADDING " << src->GetValue() << " - " << w << " - " << dst->GetValue() << "\n";
    src->edges_.insert(std::make_shared<Edge>(Edge{src, dst, w}));
    return true;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::DeleteNode(const N& val) noexcept {
    if (nodes_.count(val)) {
        std::cout <<  "Erasing node...\n";
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


    InsertNode(newData);
    nodes_[newData]->SetEdges(nodes_[oldData]->GetEdges());


    for (const auto& node : nodes_) {
        for (const auto& edge : node.second->GetEdges()) {
            if ((edge->GetSource().lock())->GetValue() == oldData) {
                edge->SetSource(nodes_[newData]);


            } else if ((edge->GetDest().lock())->GetValue() == oldData) {
                edge->SetDest(nodes_[newData]);

            }
        }
    }

    DeleteNode(oldData);

    return true;

}

template <typename N, typename E>
void gdwg::Graph<N, E>::MergeReplace(const N& oldData, const N& newData) {
    if (!IsNode(oldData) || !IsNode(newData)) {
        throw std::runtime_error("Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
    }

    nodes_[newData]->AddEdges(nodes_[oldData]->GetEdges());

    for (const auto& node : nodes_) {
        for (const auto& edge : node.second->GetEdges()) {
            if ((edge->GetSource().lock())->GetValue() == oldData) {
                //edge->SetSource(nodes_[newData]);
                //std::cout << "inserting " << newData << " - " << edge->GetValue() << " - " << (edge->GetDest().lock())->GetValue() << "\n";
                InsertEdge(newData, (edge->GetDest().lock())->GetValue(), edge->GetValue());
                nodes_[newData]->GetEdges().erase(edge);
            } else if ((edge->GetDest().lock())->GetValue() == oldData) {
                //edge->SetDest(nodes_[newData]);
                //std::cout << "inserting " << (edge->GetSource().lock())->GetValue()<< " - " << edge->GetValue() << " - " << newData << "\n";;
                //Insert edge with new data
                InsertEdge((edge->GetSource().lock())->GetValue(), newData, edge->GetValue());

                //Erase old edge
                nodes_[(edge->GetSource().lock())->GetValue()]->GetEdges().erase(edge);
            }
          
        }
        
    }
    DeleteNode(oldData);

}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsNode(const N& val) const noexcept {
    if (nodes_.count(val)) {
        if (nodes_.at(val)->GetValue() == val) {
            return true;
        }
    }
    return false;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsConnected(const N& src, const N& dst) const {

    if (nodes_.count(src)) {
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

    if (!nodes_.count(src)) {
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
    if (!nodes_.count(src) || !nodes_.count(dst)) {
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

/*
template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::find(const N& src, const N& dst, const E& w) const noexcept {
    std::tuple<N, N, E> to_find = {src, dst, w};
    for (const auto& it = begin(); it != end(); ++it) {
        if (to_find == *it) {
            return *it;
        }
    }
    //return this->end();

}
*/



template <typename N, typename E>
bool gdwg::Graph<N, E>::erase(const N& src, const N& dst, const E& w) noexcept {
    if (!IsNode(src)) {
        return false;
    }
    for (const auto& edge : nodes_[src]->GetEdges()) {
        if ((edge->GetDest().lock())->GetValue() == dst) {
            if (edge->GetValue() == w) {
                nodes_[src].erase(edge);
                return true;
            }
        }
    }
    return false;
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
    if (edge_it_ == node_it_->second->GetEdges().begin()) {
        --node_it_;
        edge_it_ = node_it_->second->GetEdges().end();
        std::advance(edge_it_, -1);
    } else {
        --edge_it_;
    }

    return *this;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::begin() {
    return const_iterator{nodes_.begin(), nodes_.end(), nodes_.begin()->second->GetEdges().begin()};
}


template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::begin() {
    return const_iterator{nodes_.begin(), nodes_.end(), nodes_.begin()->second->GetEdges().begin()};
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::end() {
    return const_iterator{nodes_.end(), nodes_.end(), {}};
}

#endif
