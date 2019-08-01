#include <iostream>
#include <string>

#include "assignments/dg/graph.h"

// Note: At the moment, there is no client.sampleout. Please do your own testing

int main() {
  gdwg::Graph<std::string, int> g;
  g.InsertNode("a");
  g.InsertNode("b");
  g.InsertNode("c");
  g.InsertNode("d");

  g.InsertEdge("a", "b", 5);
  g.InsertEdge("a", "c", 8);
  g.InsertEdge("a", "c", 2);

  g.InsertEdge("b", "d", 1);
  g.InsertEdge("b", "a", 4);

  g.InsertEdge("c", "d", 3);

  g.Replace("a", "e");

  g.PrintGraph();
/*

  std::cout << g << '\n';

  gdwg::Graph<std::string, int> g2{g};

  std::cout << g2 << "\n";

  // This is a structured binding.
  // https://en.cppreference.com/w/cpp/language/structured_binding
  // It allows you to unpack your tuple.
  for (const auto& [from, to, weight] : g) {
    std::cout << from << " -> " << to << " (weight " << weight << ")\n";
  }
   */
}
