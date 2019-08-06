#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <initializer_list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <sstream>
#include <iostream>
#include <tuple>
#include <vector>
#include <typeinfo>

namespace gdwg {

template<typename T>
// requires Relation<Less<T>, T>()
struct CompareByValue {
  bool operator()(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs) const {
      if (lhs->GetSource().lock()->GetValue() < rhs->GetSource().lock()->GetValue()) {
          return true;
      }

      if (
          lhs->GetSource().lock()->GetValue() <= rhs->GetSource().lock()->GetValue() &&
          lhs->GetDest().lock()->GetValue() < rhs->GetDest().lock()->GetValue()) {
          return true;
      }

      if (lhs->GetSource().lock()->GetValue() <= rhs->GetSource().lock()->GetValue() &&
      lhs->GetDest().lock()->GetValue() <= rhs->GetDest().lock()->GetValue()
      && lhs->GetValue() < rhs->GetValue()) {
          return true;
      }
      return false;
  }
};

class const_iterator;

template<typename N, typename E>
class Graph {
 public:
  class Node;
  class Edge;
  class const_iterator;

  using const_reverse_iterator = std::reverse_iterator<const_iterator>;


  class Node {
   public:
    // Constructors / Destructors
    Node() = delete;
    explicit Node(N value): value_{value} {}
    ~Node() = default;

    // Getters / Setters
    std::set<std::shared_ptr<Edge>, CompareByValue<Edge>>& GetEdges()  { return edges_; }
    N& GetValue() { return value_; }
    void SetValue(const N& val) { value_ = val; }
    void SetEdges(std::set<std::shared_ptr<Edge>, CompareByValue<Edge>> e) { edges_ = e; }

    // Helper methods
    bool AddEdge(std::shared_ptr<Node> src, std::shared_ptr<Node> dst, const E& w);
    void ClearEdges() { edges_.clear(); }
    void AddEdges(std::set<std::shared_ptr<Edge>, CompareByValue<Edge>> e) {
        for (const auto& edge : e) {
            edges_.insert(edge);
        }
    }

   private:
    N value_;
    std::set<std::shared_ptr<Edge>, CompareByValue<Edge>> edges_;

    bool operator <(const Node &compare) const {
        return value_ < compare.value;
    }

  };


  class Edge {
   public:
    // Constructors / Destructors
    Edge() = delete;
    Edge(std::weak_ptr<Node> source, std::weak_ptr<Node> dest, E weight): source_{source}, dest_{dest}, weight_{weight} {}
    ~Edge() = default;

    // Getters / Setters
    E& GetValue() { return weight_; }
    std::weak_ptr<Node> GetSource() { return source_; }
    std::weak_ptr<Node> GetDest() { return dest_; }
    void SetSource(std::weak_ptr<Node> new_source) { source_ = new_source; }
    void SetDest(std::weak_ptr<Node> new_dest) { dest_ = new_dest; }

   private:
    std::weak_ptr<Node> source_;
    std::weak_ptr<Node> dest_;
    E weight_;
  };


  class const_iterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::tuple<N, N, E>;
    using reference = std::tuple<const N&, const N&, const E&>;
    using pointer = std::tuple<const N*, const N*, const E*>;
    using difference_type = std::ptrdiff_t;

    // Constructors / Destructors
    const_iterator() = delete;
    ~const_iterator() = default;

    // Operators
    reference operator*() const;
    const_iterator& operator++();
    const_iterator operator++(int) {
        auto copy{*this};
        ++(*this);
        return copy;
    }
    const_iterator& operator--();
    const_iterator operator--(int) {
        auto copy{*this};
        --(*this);
        return copy;
    }

    friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) {
        return lhs.node_it_ == rhs.node_it_ && (lhs.node_it_ == lhs.sentinel_ || lhs.edge_it_ == rhs.edge_it_);
    }

    friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) {
        return !(lhs == rhs);
    }


   private:
    // Outer node iterator
    typename std::map<N, std::shared_ptr<Node>>::iterator node_it_;

    // Keep track of end() of node iterator
    const typename std::map<N, std::shared_ptr<Node>>::iterator sentinel_;

    // Keep track of last value for reverse iterator
    const typename std::map<N, std::shared_ptr<Node>>::iterator last_node_;

    // Inner edge iterator
    typename std::set<std::shared_ptr<Edge>, CompareByValue<Edge>>::iterator edge_it_;

    friend class Graph;
    const_iterator(
        const decltype(node_it_)& node_it,
        const decltype(sentinel_)& sentinel,
        const decltype(last_node_)& last_node,
        const decltype(edge_it_)& edge_it
        ):
        node_it_{node_it},
        sentinel_{sentinel},
        last_node_{last_node},
        edge_it_{edge_it} {}

  };

  // Constructors / Destructors
  Graph() = default;
  Graph(typename std::vector<N>::const_iterator begin, typename std::vector<N>::const_iterator end) noexcept;
  Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator tup_begin, typename std::vector<std::tuple<N, N, E>>::const_iterator tup_end) noexcept;
  Graph(std::initializer_list<N> list) noexcept;
  ~Graph() = default;

  // Gettters / Setters
  std::vector<N> GetNodes() const noexcept;
  std::vector<N> GetConnected(const N& src) const;
  std::vector<E> GetWeights(const N& src, const N& dst) const;

  Graph(const Graph<N, E>& g) noexcept;
  Graph(Graph<N, E>&& g) noexcept;

  Graph& operator=(const Graph& g) noexcept;
  Graph& operator=(Graph && g) noexcept;


  // Helper methods
  bool InsertNode(const N& val) noexcept;
  bool InsertEdge(const N& src, const N& dst, const E& w);
  bool DeleteNode(const N&) noexcept;
  bool Replace(const N& oldData, const N& newData);
  void MergeReplace(const N& oldData, const N& newData);
  void Clear() noexcept;
  bool IsNode(const N& val) const noexcept ;
  bool IsConnected(const N& src, const N& dst);
  const_iterator find(const N& src, const N& dst, const E& val) noexcept;
  bool erase(const N& src, const N& dst, const E& w) noexcept;
  const_iterator erase(const_iterator it) noexcept;


  const_iterator cbegin();
  const_iterator cend();
  const_iterator begin();
  const_iterator end();

  const_reverse_iterator crbegin();
  const_reverse_iterator crend();
  const_reverse_iterator rbegin();
  const_reverse_iterator rend();

  friend bool operator==(
      const gdwg::Graph<N, E>& a,
      const gdwg::Graph<N, E>& b) noexcept {
      // test number of nodes
      if (!a.nodes_.size() == b.nodes_.size()) {
          return false;
      }
      // test if same nodes
      for (const auto& node : a.nodes_) {
          if (!b.nodes_.count(node.first)) {
              return false;
          }
      }
      // compare with << print
      std::stringstream streamA;
      streamA << a;
      std::stringstream streamB;
      streamB << b;
      if(streamA.str() == streamB.str()){
          return true;
      } else {
          return false;
      }
  }

  friend bool operator!=(
      const gdwg::Graph<N, E>& a,
      const gdwg::Graph<N, E>& b) noexcept {
      return !(a == b);
  }

  friend std::ostream& operator<<(
      std::ostream& os,
      const gdwg::Graph<N, E>& g) noexcept {
      for (const auto& node : g.nodes_) {
          os << node.first << " (" << "\n";
          for (const auto& ptr_set : node.second->GetEdges()) {
              os << "  " << ptr_set->GetDest().lock()->GetValue() << " | " << ptr_set->GetValue() << "\n";
          }
          os << ")\n";
      }
      return os;
  }
 private:
  std::map<N, std::shared_ptr<Node>> nodes_;
};

}

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
