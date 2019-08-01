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
    bool IsNode(const N& val) const noexcept ;

    void PrintGraph() {
        std::cout << "printing graph...\n";
        
        
        for (const auto& node : nodes_) {
            //set of shared pointers
            for (const auto &ptr_map : node->GetEdges()) {
                for (const auto &ptr_set : ptr_map.second) {
                    std::cout << node->GetValue() << " - " << ptr_set->GetValue() << " - "
                              << ptr_set->GetDest().lock()->GetValue() << "\n";
                }

            }
        }
        
        
    }



    ~Graph() = default;
 private:


  class Node;
  class Edge;


  std::set<std::shared_ptr<Node>> nodes_;



  class Node {
   public:

    Node() = default;
    Node(N value): value_{value} {}

    N GetValue() { return value_; }

    bool AddEdge(std::shared_ptr<Node> src, std::shared_ptr<Node> dst, const E& w);

    std::map<N, std::set<std::shared_ptr<Edge>, CompareByValue<Edge>>> GetEdges() { return edges_; }
    void ClearEdges() { edges_.clear(); }

    ~Node() = default;


   private:
    N value_;
    //std::set<std::shared_ptr<Edge>> edges_;
    std::map<N, std::set<std::shared_ptr<Edge>, CompareByValue<Edge>>> edges_;


    
    bool operator <(const Node &compare) const {
        return value_ < compare.value;
    }
    
  };


  class Edge {
   public:

    Edge() = delete;
    Edge(std::weak_ptr<Node> source, std::weak_ptr<Node> dest, E weight): source_{source}, dest_{dest}, weight_{weight} {}

    E GetValue() { return weight_; }
    void SetSource(std::shared_ptr<Node> new_source) { source_ = new_source; }
    void SetDest(std::shared_ptr<Node> new_dest) { dest_ = new_dest; }
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

};

// Define your graph_iterator here
// Define your graph_const_iterator here

}

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_