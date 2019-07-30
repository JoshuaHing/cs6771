#ifndef GRAPH_TPP
#define GRAPH_TPP

#include <algorithm>

template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertNode(const N& val) {
    if (!IsNode(val)) {
        nodes_.insert(std::make_shared<Node>(Node{val}));
        return true;
    }
    return false;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertEdge(const N& src, const N& dst, const E& w) {
    auto src_it = std::find_if(nodes_.begin(), nodes_.end(), [&src](std::shared_ptr<Node> n) { return (src == n->GetValue()); });
    if (src_it == nodes_.end()) throw std::runtime_error("Cannot call Graph::InsertEdge when either src or dst node does not exist");

    auto dst_it = std::find_if(nodes_.begin(), nodes_.end(), [&dst](std::shared_ptr<Node> n) { return (dst == n->GetValue()); });
    if (dst_it == nodes_.end()) throw std::runtime_error("Cannot call Graph::InsertEdge when either src or dst node does not exist");

    auto src_node_ptr = *src_it;
    return (src_node_ptr->AddEdge(*src_it, *dst_it, w));

}

template <typename N, typename E>
bool gdwg::Graph<N, E>::Node::AddEdge(std::shared_ptr<Node> src, std::shared_ptr<Node> dst, const E& w) {
    Node src_node = *src;
    auto it = std::find_if(src_node.edges_.begin(), src_node.edges_.end(), [&dst, &w](std::shared_ptr<Edge> e) { return (dst == e->GetDest().lock() && w == e->GetWeight()) ;});
    if (!(it == src_node.edges_.end())) return false;
    src_node.edges_.insert(std::make_shared<Edge>(Edge{src, dst, w}));
    return true;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsNode(const N& val) const {
    for (const auto& node : nodes_) {
        if (node->GetValue() == val) return true;
    }
    return false;
}

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

#endif
