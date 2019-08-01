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
    auto it = std::find_if(src->edges_[src->GetValue()].begin(), src->edges_[src->GetValue()].end(), [&dst, &w](std::shared_ptr<Edge> e) { return (dst == e->GetDest().lock() && w == e->GetValue()) ;});
    if (!(it == src->edges_[src->GetValue()].end())) return false;
    //src->edges_.insert({src->GetValue(), std::make_shared<Edge>(Edge{src, dst, w})});
    src->edges_[src->GetValue()].insert(std::make_shared<Edge>(Edge{src, dst, w}));
    return true;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::DeleteNode(const N& val) noexcept {
    for (const auto& node : nodes_) {
        if (node->GetValue() == val) {
            std::cout <<  "Erasing node...\n";
            nodes_.erase(node);
            return true;
        }

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

    Node new_node = Node{newData};

    for (const auto& node : nodes_) {

        if (node->GetEdges().count(oldData) == 1) {
            std::cout << "hello\n";
            if (node->GetValue() == oldData) {

                // Copy over old node data to the new node
                for (const auto& map_ptr : node->GetEdges()) {
                    for (const auto& edge_ptr : map_ptr.second) {
                        //static_cast<void>(map_ptr);
                        //static_cast<void>(edge_ptr);
                        //edge_ptr->SetSource(std::make_shared<Node>(new_node));
                        new_node.AddEdge(std::make_shared<Node>(new_node), edge_ptr->GetDest().lock(), edge_ptr->GetValue());
                    }
                }

                // Clear old node data
                //node->ClearEdges();
                //nodes_.erase(node);
            }
        } else {
            /*
            for (const auto& edge_ptr : (node->GetEdges())[oldData]) {
                edge_ptr->SetDest(std::make_shared<Node>(*node));
            }
             */
        }

    }
    std::cout << "size = " << nodes_.size() << "\n";
    nodes_.insert(std::make_shared<Node>(new_node));
    return true;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsNode(const N& val) const noexcept {
    for (const auto& node : nodes_) {
        if (node->GetValue() == val) {
            return true;
        }
    }
    return false;
}


#endif
