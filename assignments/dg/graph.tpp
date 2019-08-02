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
    auto it = std::find_if(src->edges_.begin(), src->edges_.end(), [&dst, &w](std::shared_ptr<Edge> e) { return (dst == e->GetDest().lock() && w == e->GetValue()) ;});
    if (!(it == src->edges_.end())) return false;
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
                edge->SetSource(nodes_[newData]);
            } else if ((edge->GetDest().lock())->GetValue() == oldData) {
                edge->SetDest(nodes_[newData]);
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

#endif
