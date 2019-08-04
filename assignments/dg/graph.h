#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <initializer_list>
#include <map>
#include <memory>
#include <set>
#include <tuple>
#include <vector>
#include <typeinfo>

namespace gdwg {

template<typename T>
// requires Relation<Less<T>, T>()
struct CompareByValue {
  bool operator()(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs) const {
    //return *lhs < *rhs;
    /*
    static_cast<void>(lhs);
    static_cast<void>(rhs);
    return true;
    */
   if (((*(lhs->GetDest().lock())).GetValue() < (*(rhs->GetDest().lock())).GetValue())) {
       return true;
   }

  if (((*(lhs->GetDest().lock())).GetValue() <= (*(rhs->GetDest().lock())).GetValue())&& lhs->GetValue() < rhs->GetValue()) {
      return true;
  }
  return false;


   
  }
};


template<typename N, typename E>
class Graph {
 public:

    Graph() = default;
    Graph(typename std::vector<N>::const_iterator begin, typename std::vector<N>::const_iterator end) noexcept;

    Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator tup_begin, typename std::vector<std::tuple<N, N, E>>::const_iterator tup_end) noexcept;
    Graph(std::initializer_list<N> list) noexcept;

    // move and copy constructors
    Graph(const Graph<N, E>& g) noexcept;
    Graph(Graph<N, E>&& g) noexcept;

    Graph& operator=(const Graph& g) noexcept;
    Graph& operator=(Graph && g) noexcept;


    bool InsertNode(const N& val) noexcept;
    bool InsertEdge(const N& src, const N& dst, const E& w);
    bool DeleteNode(const N&) noexcept; 
    bool Replace(const N& oldData, const N& newData);
    void MergeReplace(const N& oldData, const N& newData);

    bool IsNode(const N& val) const noexcept ;
    bool IsConnected(const N& src, const N& dst) const;


    std::vector<N> GetNodes() const noexcept;
    std::vector<N> GetConnected(const N& src) const;
    std::vector<E> GetWeights(const N& src, const N& dst) const;
    const_iterator find(const N&, const N&, const E&);

    bool erase(const N& src, const N& dst, const E& w) noexcept;


    void PrintGraph() {
        std::cout << "printing graph...\n";


        for (const auto& node : nodes_) {
            //set of shared pointers
            for (const auto& ptr_set : node.second->GetEdges()) {
                //for (const auto &ptr_set : ptr_map.second) {
                std::cout << ptr_set->GetSource().lock()->GetValue() << " - " << ptr_set->GetValue()
                          << " - "
                          << ptr_set->GetDest().lock()->GetValue() << "\n";
                //}

            }
        }

    }


    ~Graph() = default;

    const_iterator begin();
    const_iterator end();
 private:


  class Node;
  class Edge;
  class const_iterator;


  std::map<N, std::shared_ptr<Node>> nodes_;



  class Node {
   public:

    Node() = default;
    Node(N value): value_{value} {}

    N GetValue() const { return value_; }
    void SetValue(const N& val) { value_ = val; }

    bool AddEdge(std::shared_ptr<Node> src, std::shared_ptr<Node> dst, const E& w);

    std::set<std::shared_ptr<Edge>, CompareByValue<Edge>> GetEdges() { return edges_; }


    void ClearEdges() { edges_.clear(); }
    void SetEdges(std::set<std::shared_ptr<Edge>, CompareByValue<Edge>> e) { edges_ = e; }
    void AddEdges(std::set<std::shared_ptr<Edge>, CompareByValue<Edge>> e) {
        for (const auto& edge : e) {
            edges_.insert(edge);
        }
    }

    ~Node() = default;



   private:
    N value_;
    std::set<std::shared_ptr<Edge>, CompareByValue<Edge>> edges_;

    bool operator <(const Node &compare) const {
        return value_ < compare.value;
    }
    
  };


  class Edge {
   public:

    Edge() = delete;
    Edge(std::weak_ptr<Node> source, std::weak_ptr<Node> dest, E weight): source_{source}, dest_{dest}, weight_{weight} {}

    E GetValue() const { return weight_; }
    void SetSource(std::weak_ptr<Node> new_source) { source_ = new_source; }
    void SetDest(std::weak_ptr<Node> new_dest) { dest_ = new_dest; }
    std::weak_ptr<Node> GetSource() { return source_; }
    std::weak_ptr<Node> GetDest() { return dest_; }

    ~Edge() = default;

   private:
    std::weak_ptr<Node> source_;
    std::weak_ptr<Node> dest_;
    E weight_;

    /*
    bool operator <(const Edge &compare) const {
        std::cout << "called\n";
        return (compare.GetDest() <= dest_ && compare.GetWeight() < weight_);
    }
    */

  };

  class const_iterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::tuple<N, N, E>;
    using reference = std::tuple<const N&, const N&, const E&>;
    using pointer = std::tuple<const N*, const N*, const E*>;
    using difference_type = std::ptrdiff_t;

    reference operator*() const;
    /*
    const_iterator& operator++();
    const_iterator operator++(int) {
        auto copy{*this};
        ++(*this);
        return copy;
    }
     */

   private:
    typename std::map<N, std::shared_ptr<Node>>::iterator node_it_;

    const typename std::map<N, std::shared_ptr<Node>>::iterator sentinel_;
    typename std::set<std::shared_ptr<Edge>, CompareByValue<Edge>>::iterator edge_it_;

    friend class Graph;

    const_iterator(const decltype(node_it_)& outer, const decltype(sentinel_)& sentinel, const decltype(edge_it_)& inner): node_it_{outer}, sentinel_{sentinel}, edge_it_{inner} {}

  };

};

// Define your graph_iterator here
// Define your graph_const_iterator here


}

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_