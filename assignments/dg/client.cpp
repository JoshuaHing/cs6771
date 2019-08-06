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
    g.InsertEdge("a", "b", 6);
    g.InsertEdge("a", "b", 4);
    g.InsertEdge("a", "c", 8);
    g.InsertEdge("a", "c", 2);



  g.InsertEdge("b", "d", 1);
  g.InsertEdge("b", "a", 4);

  g.InsertEdge("c", "d", 3);


  /*
  auto it = g.erase(g.begin());
    std::cout << "value = " << std::get<0>(*it) << "\n";
    std::cout << "value = " << std::get<1>(*it) << "\n";
    std::cout << "value = " << std::get<2>(*it) << "\n";
    */
  //g.Replace("a", "e");
  //g.MergeReplace("a", "b");


  /*
   * b - 5 - b
   * b - 6 - b
   * b - 4 - b
   * b - 8 - c
   * b - 2 - c
   * b - 1 - d
   * c - 3 - d
   */


    /*
    auto it = g.end();
    --it;
    std::cout << "value = " << std::get<0>(*it) << "\n";
    std::cout << "value = " << std::get<1>(*it) << "\n";
    std::cout << "value = " << std::get<2>(*it) << "\n";
     */


  /*
    for (int i = 0; i < 8; ++i) {
        std::cout << "value = " << std::get<0>(*it) << "\n";
        std::cout << "value = " << std::get<1>(*it) << "\n";
        std::cout << "value = " << std::get<2>(*it) << "\n";
        it++;
        std::cout << "\n";
    }
    */

    std::vector<std::string> v{"hello", "how"};
    gdwg::Graph<std::string, double> b{v.begin(), v.end()};
    static_cast<void>(b);



    //static_cast1 <void>(it);



    //g.Replace("a", "e");
    /*
    g.InsertEdge("a", "b", 1);
    g.InsertEdge("a", "c", 2);
    g.InsertEdge("a", "d", 3);
    g.InsertEdge("b", "b", 1);
     */


    /*
    g.InsertEdge("b", "a", 3);
    g.InsertEdge("b", "c", 2);
    g.InsertEdge("b", "d", 4);
     */

    /*

    auto vec = g.GetWeights("a", "b");
    for (const auto& item : vec) {
        std::cout << item << "\n";

    }
     */


  //std::cout << g.IsConnected("a", "d") << "\n";

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
