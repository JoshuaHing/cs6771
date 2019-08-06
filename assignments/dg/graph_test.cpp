/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

*/
/*
 * Every function in this assignment is tested to ensure full coverage.
 * (The only exception to this is the destructor, which should not be called manually.)
 * To ensure that each function is tested thoroughly, both regular cases
 * and edge cases were tested for those where it was deemed necessary.
 *
 * The tests were not designed to be overly complicated (in the trek sense
 * e.g. by using complicated floats like 3.523523454). This is to avoid any incorrect
 * tests. Additionally, when the testing of function A depended on function B,
 * I avoided testing function B with only function A. This will eliminate any
 * cyclic errors that could be undetected. Also, I made sure to check for the number
 * of nodes, their names and the number of connections they had before checking specific connections.
 * This is to make sure there aren't any random nodes or connections that shouldn't be there.
 *
 * Note how no private members or unscoped (hidden in the implementation) functions
 * were accessed in these tests. This ensures our tests are not brittle if the
 * underlying implementations change so long as the provided functions do as intended.
 *
 * Furthermore, tests are nesting withing clear SCENARIO -> WHEN -> THEN structures
 * which are designed to test one thing at a time. It will thus become clear what went
 * wrong if a test has failed.
 *
*/
#include <sstream>
#include <string>
#include <tuple>

#include "assignments/dg/graph.h"
#include "catch.h"

/*
  **************************************************************************************************
                                 2.1 Constructors & Destructors
  **************************************************************************************************
*/

// Default Constructor
SCENARIO("Default Constructor") {
  WHEN("Creating graph with Default Constructor") {
    gdwg::Graph<int, int> g;
    THEN("Nothing should be there") {
      REQUIRE(g.GetNodes().size() == 0);
    }
  }
}

//Node Iterator Constructor
SCENARIO("Node Iterator Constructor") {
  WHEN("Creating graph with Node Iterator Constructor N2") {
    std::vector<std::string> v{"Hello", "there"};
    gdwg::Graph<std::string, double> g{v.begin(),v.end()};
    THEN("2 nodes should be there") {
      REQUIRE(g.GetNodes().size() == 2);
      REQUIRE(g.IsNode("Hello") == true);
      REQUIRE(g.IsNode("there") == true);
      REQUIRE(g.GetConnected("Hello").size() == 0);
    }
  }
  WHEN("Creating graph with Node Iterator Constructor N0") {
    std::vector<std::string> v{};
    gdwg::Graph<std::string, double> g{v.begin(),v.end()};
    THEN("Nothing should be there") {
      REQUIRE(g.GetNodes().size() == 0);
    }
  }
}


// Node-Node-Weight Iterator Constructor
SCENARIO("Node-Node-Weight Iterator Constructor") {
  WHEN("Creating graph with Node-Node-Weight Iterator Constructor N3 E2") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"are"};
    auto e1 = std::make_tuple(s1, s2, 5.4);
    auto e2 = std::make_tuple(s2, s3, 7.6);
    auto e = std::vector<std::tuple<std::string, std::string, double>>{e1, e2};
    gdwg::Graph<std::string, double> g{e.begin(), e.end()};
    THEN("4 nodes exist with 2 edges") {
      //Test nodes
      REQUIRE(g.GetNodes().size() == 3);
      REQUIRE(g.IsNode("Hello") == true);
      REQUIRE(g.IsNode("how") == true);
      REQUIRE(g.IsNode("are") == true);
      //Test edges
      REQUIRE(g.GetConnected("Hello").size() == 1);
      REQUIRE(g.GetConnected("how").size() == 1);
      REQUIRE(g.GetConnected("are").size() == 0);
      REQUIRE(g.GetWeights("Hello","how").size() == 1);
      REQUIRE(g.GetWeights("Hello","how")[0] == 5.4);
      REQUIRE(g.GetWeights("how","are").size() == 1);
      REQUIRE(g.GetWeights("how","are")[0] == 7.6);


    }
  }
  WHEN("Creating graph with Node-Node-Weight Iterator Constructor N0 E0") {
    auto e = std::vector<std::tuple<std::string, std::string, double>>{};
    gdwg::Graph<std::string, double> g{e.begin(), e.end()};
    THEN("Nothing should be there") {
      REQUIRE(g.GetNodes().size() == 0);
    }
  }
}

//List of Nodes Constructor
SCENARIO("List of Nodes Constructor") {
  WHEN("Creating graph with List of Nodes Constructor N4") {
    gdwg::Graph<char, std::string> g{'a', 'b', 'x', 'y'};
    THEN("4 nodes exist") {
      REQUIRE(g.GetNodes().size() == 4);
      REQUIRE(g.IsNode('a') == true);
      REQUIRE(g.IsNode('b') == true);
      REQUIRE(g.IsNode('x') == true);
      REQUIRE(g.IsNode('y') == true);
      REQUIRE(g.GetConnected('a').size() == 0);
      REQUIRE(g.GetConnected('b').size() == 0);
      REQUIRE(g.GetConnected('x').size() == 0);
      REQUIRE(g.GetConnected('y').size() == 0);
    }
  }
  WHEN("Creating graph with List of Nodes Constructor N0") {
    gdwg::Graph<char, std::string> g{};
    THEN("Nothing should be there") {
      REQUIRE(g.GetNodes().size() == 0);
    }
  }
}



//Copy Constructor
SCENARIO("Copy Constructor") {
  WHEN("Creating graph with Copy Constructor N0") {
    gdwg::Graph<std::string, int> g;
    gdwg::Graph<std::string, int> gCopy{g};
    THEN("gCopy should be same as original") {
      REQUIRE(g.GetNodes().size() == 0);
      REQUIRE(gCopy.GetNodes().size() == 0);
    }
  }
  WHEN("Creating graph with Copy Constructor N3 E2") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"are"};
    auto e1 = std::make_tuple(s1, s2, 5.4);
    auto e2 = std::make_tuple(s2, s3, 7.6);
    auto e = std::vector<std::tuple<std::string, std::string, double>>{e1, e2};
    gdwg::Graph<std::string, double> g{e.begin(), e.end()};
    gdwg::Graph<std::string, double> gCopy{g};
    THEN("gCopy should be same as original") {
      //Test nodes
      REQUIRE(g.GetNodes().size() == 3);
      REQUIRE(g.IsNode("Hello") == true);
      REQUIRE(g.IsNode("how") == true);
      REQUIRE(g.IsNode("are") == true);
      //Test edges
      REQUIRE(g.GetConnected("Hello").size() == 1);
      REQUIRE(g.GetConnected("how").size() == 1);
      REQUIRE(g.GetConnected("are").size() == 0);
      REQUIRE(g.GetWeights("Hello","how").size() == 1);
      REQUIRE(g.GetWeights("Hello","how")[0] == 5.4);
      REQUIRE(g.GetWeights("how","are").size() == 1);
      REQUIRE(g.GetWeights("how","are")[0] == 7.6);

      //Test nodes
      REQUIRE(gCopy.GetNodes().size() == 3);
      REQUIRE(gCopy.IsNode("Hello") == true);
      REQUIRE(gCopy.IsNode("how") == true);
      REQUIRE(gCopy.IsNode("are") == true);
      //Test edges
      REQUIRE(gCopy.GetConnected("Hello").size() == 1);
      REQUIRE(gCopy.GetConnected("how").size() == 1);
      REQUIRE(gCopy.GetConnected("are").size() == 0);
      REQUIRE(gCopy.GetWeights("Hello","how").size() == 1);
      REQUIRE(gCopy.GetWeights("Hello","how")[0] == 5.4);
      REQUIRE(gCopy.GetWeights("how","are").size() == 1);
      REQUIRE(gCopy.GetWeights("how","are")[0] == 7.6);
    }
  }
}

//Move Constructor
SCENARIO("Move Constructor") {
  WHEN("Creating graph with Move Constructor N0") {
    gdwg::Graph<std::string, int> g;
    gdwg::Graph<std::string, int> gMove{std::move(g)};
    THEN("gMove should be same as original, g should be nothing") {
      REQUIRE(g.GetNodes().size() == 0);
      REQUIRE(gMove.GetNodes().size() == 0);
    }
  }
  WHEN("Creating graph with Move Constructor N3 E2") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"are"};
    auto e1 = std::make_tuple(s1, s2, 5.4);
    auto e2 = std::make_tuple(s2, s3, 7.6);
    auto e = std::vector<std::tuple<std::string, std::string, double>>{e1, e2};
    gdwg::Graph<std::string, double> g{e.begin(), e.end()};
    gdwg::Graph<std::string, double> gMove{std::move(g)};
    THEN("gMove should be same as original, g should be nothing") {
      REQUIRE(g.GetNodes().size() == 0);

      //Test nodes
      REQUIRE(gMove.GetNodes().size() == 3);
      REQUIRE(gMove.IsNode("Hello") == true);
      REQUIRE(gMove.IsNode("how") == true);
      REQUIRE(gMove.IsNode("are") == true);
      //Test edges
      REQUIRE(gMove.GetConnected("Hello").size() == 1);
      REQUIRE(gMove.GetConnected("how").size() == 1);
      REQUIRE(gMove.GetConnected("are").size() == 0);
      REQUIRE(gMove.GetWeights("Hello","how").size() == 1);
      REQUIRE(gMove.GetWeights("Hello","how")[0] == 5.4);
      REQUIRE(gMove.GetWeights("how","are").size() == 1);
      REQUIRE(gMove.GetWeights("how","are")[0] == 7.6);
    }
  }
}

/*
  **************************************************************************************************
                                       2.2. Operations
  **************************************************************************************************
*/

//Copy Assignment
SCENARIO("Copy Assignment") {
  WHEN("Using Copy Assignment N0") {
    const gdwg::Graph<std::string, int> g;
    gdwg::Graph<std::string, int> g2;
    g2 = g;
    THEN("g2 should be same as original") {
      REQUIRE(g.GetNodes().size() == 0);
      REQUIRE(g2.GetNodes().size() == 0);
    }
  }
  WHEN("Using Copy Assignment N3 E2") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"are"};
    auto e1 = std::make_tuple(s1, s2, 5.4);
    auto e2 = std::make_tuple(s2, s3, 7.6);
    auto e = std::vector<std::tuple<std::string, std::string, double>>{e1, e2};
    const gdwg::Graph<std::string, double> g{e.begin(), e.end()};
    gdwg::Graph<std::string, double> g2;
    g2 = g;
    THEN("g2 should be same as original") {
      //Test nodes
      REQUIRE(g.GetNodes().size() == 3);
      REQUIRE(g.IsNode("Hello") == true);
      REQUIRE(g.IsNode("how") == true);
      REQUIRE(g.IsNode("are") == true);
      //Test edges
      REQUIRE(g.GetConnected("Hello").size() == 1);
      REQUIRE(g.GetConnected("how").size() == 1);
      REQUIRE(g.GetConnected("are").size() == 0);
      REQUIRE(g.GetWeights("Hello","how").size() == 1);
      REQUIRE(g.GetWeights("Hello","how")[0] == 5.4);
      REQUIRE(g.GetWeights("how","are").size() == 1);
      REQUIRE(g.GetWeights("how","are")[0] == 7.6);

      //Test nodes
      REQUIRE(g2.GetNodes().size() == 3);
      REQUIRE(g2.IsNode("Hello") == true);
      REQUIRE(g2.IsNode("how") == true);
      REQUIRE(g2.IsNode("are") == true);
      //Test edges
      REQUIRE(g2.GetConnected("Hello").size() == 1);
      REQUIRE(g2.GetConnected("how").size() == 1);
      REQUIRE(g2.GetConnected("are").size() == 0);
      REQUIRE(g2.GetWeights("Hello","how").size() == 1);
      REQUIRE(g2.GetWeights("Hello","how")[0] == 5.4);
      REQUIRE(g2.GetWeights("how","are").size() == 1);
      REQUIRE(g2.GetWeights("how","are")[0] == 7.6);
    }
  }
}

//Move Assignment
SCENARIO("Move Assignment") {
  WHEN("Using Move Assignment N0") {
    gdwg::Graph<std::string, int> g;
    gdwg::Graph<std::string, int> g2;
    g2 = std::move(g);
    THEN("g2 should be same as original, g should be nothing") {
      REQUIRE(g.GetNodes().size() == 0);
      REQUIRE(g2.GetNodes().size() == 0);
    }
  }
  WHEN("Using Move Assignment N3 E2") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"are"};
    auto e1 = std::make_tuple(s1, s2, 5.4);
    auto e2 = std::make_tuple(s2, s3, 7.6);
    auto e = std::vector<std::tuple<std::string, std::string, double>>{e1, e2};
    gdwg::Graph<std::string, double> g{e.begin(), e.end()};
    gdwg::Graph<std::string, double> g2;
    g2 = std::move(g);
    THEN("g2 should be same as original, g should be nothing") {
      //Test nodes
      REQUIRE(g.GetNodes().size() == 0);

      //Test nodes
      REQUIRE(g2.GetNodes().size() == 3);
      REQUIRE(g2.IsNode("Hello") == true);
      REQUIRE(g2.IsNode("how") == true);
      REQUIRE(g2.IsNode("are") == true);
      //Test edges
      REQUIRE(g2.GetConnected("Hello").size() == 1);
      REQUIRE(g2.GetConnected("how").size() == 1);
      REQUIRE(g2.GetConnected("are").size() == 0);
      REQUIRE(g2.GetWeights("Hello","how").size() == 1);
      REQUIRE(g2.GetWeights("Hello","how")[0] == 5.4);
      REQUIRE(g2.GetWeights("how","are").size() == 1);
      REQUIRE(g2.GetWeights("how","are")[0] == 7.6);
    }
  }
}

/*
  **************************************************************************************************
                                          2.3. Methods
  **************************************************************************************************
*/

//InsertNode Method
SCENARIO("InsertNode Method") {
  WHEN("Inserting 2 Nodes into empty graph") {
    gdwg::Graph<std::string, int> g;
    std::string s1{"Hello"};
    std::string s2{"there"};
    std::string s3{"there"};
    REQUIRE(g.InsertNode(s1) == true);
    REQUIRE(g.InsertNode(s2) == true);
    REQUIRE(g.InsertNode(s3) == false);
    THEN("2 nodes there") {
      REQUIRE(g.GetNodes().size() == 2);
      REQUIRE(g.IsNode("Hello") == true);
      REQUIRE(g.IsNode("There") == true);
      REQUIRE(g.GetConnected("Hello").size() == 0);
    }
  }
}

//InsertEdge Method
SCENARIO("InsertEdge Method") {
  WHEN("Inserting 2 Edges into empty graph") {
    gdwg::Graph<std::string, int> g{"a", "b"};
    REQUIRE(g.InsertEdge("a", "b", 3) == true);
    REQUIRE(g.InsertEdge("a", "b", 3) == false);
    REQUIRE(g.InsertEdge("a", "b", 4) == true);
    REQUIRE_THROWS_WITH(g.InsertEdge("a", "c", 4), "Cannot call Graph::InsertEdge when either src or dst node does not exist");
    REQUIRE_THROWS_WITH(g.InsertEdge("c", "a", 4), "Cannot call Graph::InsertEdge when either src or dst node does not exist");
    THEN("2 Edges there") {
      REQUIRE(g.GetNodes().size() == 2);
      REQUIRE(g.IsNode("a") == true);
      REQUIRE(g.IsNode("b") == true);
      REQUIRE(g.GetConnected("a").size() == 2);
      REQUIRE(g.GetConnected("b").size() == 0);
      REQUIRE(g.GetWeights("a","b").size() == 2);
      REQUIRE(g.GetWeights("a","b")[0] == 3);
      REQUIRE(g.GetWeights("a","b")[1] == 4);
    }
  }
}

//DeleteNode Method
SCENARIO("DeleteNode Method") {
  WHEN("Deleting a node") {
    gdwg::Graph<std::string, int> g{"a", "b"};
    REQUIRE(g.InsertEdge("a", "b", 3) == true);
    REQUIRE(g.InsertEdge("b", "a", 3) == true);
    REQUIRE(g.GetNodes().size() == 2);

    REQUIRE(g.DeleteNode("b") == true);
    REQUIRE(g.DeleteNode("c") == false);
    THEN("1 node left") {
      REQUIRE(g.GetNodes().size() == 1);
      REQUIRE(g.IsNode("a") == true);
      REQUIRE(g.GetConnected("a").size() == 0);
    }
  }
}

//Replace Method
SCENARIO("Replace Method") {
  WHEN("Replacing a node") {
    gdwg::Graph<std::string, int> g{"a", "b"};
    REQUIRE(g.InsertEdge("a", "b", 3) == true);
    REQUIRE(g.InsertEdge("b", "a", 4) == true);
    REQUIRE(g.IsNode("a") == true);

    REQUIRE(g.Replace("a", "b") == false);
    REQUIRE(g.Replace("a", "c") == true);
    REQUIRE_THROWS_WITH(g.Replace("a", "d"), "Cannot call Graph::Replace on a node that doesn't exist");
    THEN("c node left") {
      REQUIRE(g.GetNodes().size() == 2);
      REQUIRE(g.IsNode("c") == true);
      REQUIRE(g.IsNode("b") == true);
      REQUIRE(g.IsNode("a") == false);
      REQUIRE(g.GetConnected("c").size() == 1);
      REQUIRE(g.GetConnected("b").size() == 1);
      REQUIRE(g.GetWeights("c","b")[0] == 3);
      REQUIRE(g.GetWeights("b","c")[0] == 4);
    }
  }
}

//MergeReplace Method
SCENARIO("MergeReplace Method") {
  WHEN("MergeReplacing a node e.g.1") {
    gdwg::Graph<std::string, int> g{"a", "b", "c", "d"};
    REQUIRE(g.InsertEdge("a", "b", 1) == true);
    REQUIRE(g.InsertEdge("a", "c", 2) == true);
    REQUIRE(g.InsertEdge("a", "d", 3) == true);

    REQUIRE_THROWS_WITH(g.MergeReplace("a", "e"), "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
    REQUIRE_THROWS_WITH(g.MergeReplace("e", "a"), "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
    g.MergeReplace("a", "b");

    THEN("checking graph after") {
      REQUIRE(g.GetNodes().size() == 3);
      REQUIRE(g.IsNode("b") == true);
      REQUIRE(g.IsNode("c") == true);
      REQUIRE(g.IsNode("d") == true);
      REQUIRE(g.GetConnected("b").size() == 3);
      REQUIRE(g.GetConnected("c").size() == 0);
      REQUIRE(g.GetConnected("d").size() == 0);
      REQUIRE(g.GetWeights("b","b")[0] == 1);
      REQUIRE(g.GetWeights("b","c")[0] == 2);
      REQUIRE(g.GetWeights("b","d")[0] == 3);
    }
  }
  WHEN("MergeReplacing a node e.g.2") {
    gdwg::Graph<std::string, int> g{"a", "b", "c", "d"};
    REQUIRE(g.InsertEdge("a", "b", 1) == true);
    REQUIRE(g.InsertEdge("a", "c", 2) == true);
    REQUIRE(g.InsertEdge("a", "d", 3) == true);
    REQUIRE(g.InsertEdge("b", "b", 1) == true);

    g.MergeReplace("a", "b");

    THEN("checking graph after") {
      REQUIRE(g.GetNodes().size() == 3);
      REQUIRE(g.IsNode("b") == true);
      REQUIRE(g.IsNode("c") == true);
      REQUIRE(g.IsNode("d") == true);
      REQUIRE(g.GetConnected("b").size() == 3);
      REQUIRE(g.GetConnected("c").size() == 0);
      REQUIRE(g.GetConnected("d").size() == 0);
      REQUIRE(g.GetWeights("b","b")[0] == 1);
      REQUIRE(g.GetWeights("b","c")[0] == 2);
      REQUIRE(g.GetWeights("b","d")[0] == 3);
    }
  }
  WHEN("MergeReplacing a node e.g.3.1") {
    gdwg::Graph<std::string, int> g{"a", "b", "c", "d"};
    REQUIRE(g.InsertEdge("a", "b", 3) == true);
    REQUIRE(g.InsertEdge("c", "b", 2) == true);
    REQUIRE(g.InsertEdge("d", "b", 4) == true);

    g.MergeReplace("b", "a");

    THEN("checking graph after") {
      REQUIRE(g.GetNodes().size() == 3);
      REQUIRE(g.IsNode("a") == true);
      REQUIRE(g.IsNode("c") == true);
      REQUIRE(g.IsNode("d") == true);
      REQUIRE(g.GetConnected("a").size() == 1);
      REQUIRE(g.GetConnected("c").size() == 1);
      REQUIRE(g.GetConnected("d").size() == 1);
      REQUIRE(g.GetWeights("a","a")[0] == 3);
      REQUIRE(g.GetWeights("c","a")[0] == 2);
      REQUIRE(g.GetWeights("d","a")[0] == 4);
    }
  }
  WHEN("MergeReplacing a node e.g.3.2") {
    gdwg::Graph<std::string, int> g{"a", "b", "c", "d"};
    REQUIRE(g.InsertEdge("b", "a", 3) == true);
    REQUIRE(g.InsertEdge("b", "c", 2) == true);
    REQUIRE(g.InsertEdge("b", "d", 4) == true);

    g.MergeReplace("b", "a");

    THEN("checking graph after") {
      REQUIRE(g.GetNodes().size() == 3);
      REQUIRE(g.IsNode("a") == true);
      REQUIRE(g.IsNode("c") == true);
      REQUIRE(g.IsNode("d") == true);
      REQUIRE(g.GetConnected("a").size() == 3);
      REQUIRE(g.GetConnected("c").size() == 0);
      REQUIRE(g.GetConnected("d").size() == 0);
      REQUIRE(g.GetWeights("a","a")[0] == 3);
      REQUIRE(g.GetWeights("a","c")[0] == 2);
      REQUIRE(g.GetWeights("a","d")[0] == 4);
    }
  }
}

//Clear Method
SCENARIO("Clear Method") {
  WHEN("Clearing Graph first, then adding a node after") {
    gdwg::Graph<std::string, int> g{"a", "b"};
    REQUIRE(g.InsertEdge("a", "b", 3) == true);
    REQUIRE(g.InsertEdge("b", "a", 4) == true);
    g.Clear();
    THEN("check nothing left at first, then one node after") {
      REQUIRE(g.GetNodes().size() == 0);
      std::string s{"a"};
      REQUIRE(g.InsertNode(s) == true);
      REQUIRE(g.GetNodes().size() == 1);
      REQUIRE(g.IsNode("a") == true);
    }
  }
  WHEN("Clearing nothing") {
    gdwg::Graph<std::string, int> g;
    g.Clear();
    THEN("check nothing still") {
      REQUIRE(g.GetNodes().size() == 0);
    }
  }
}

//IsNode Method
SCENARIO("IsNode Method") {
  WHEN("Checking for node") {
    gdwg::Graph<std::string, int> g{"a", "b"};
    THEN("nodes better be there, and others not") {
      REQUIRE(g.GetNodes().size() == 3);
      REQUIRE(g.IsNode("a") == true);
      REQUIRE(g.IsNode("b") == true);
      REQUIRE(g.IsNode("c") == false);
      REQUIRE(g.IsNode("z") == false);
      REQUIRE(g.IsNode("1") == false);
      REQUIRE(g.IsNode("#") == false);
    }
  }
}

//IsConnected Method
SCENARIO("IsConnected Method") {
  WHEN("Checking for connectedness") {
    gdwg::Graph<std::string, int> g{"a", "b", "c"};
    REQUIRE(g.InsertEdge("a", "b", 3) == true);
    REQUIRE(g.InsertEdge("b", "a", 3) == true);
    REQUIRE(g.InsertEdge("b", "c", 3) == true);
    THEN("connected nodes a<->b and b->c only") {
      REQUIRE(g.IsConnected("a", "b") == true);
      REQUIRE(g.IsConnected("b", "a") == true);
      REQUIRE(g.IsConnected("b", "c") == true);
      REQUIRE(g.IsConnected("c", "b") == false);
      REQUIRE(g.IsConnected("a", "c") == false);
      REQUIRE(g.IsConnected("c", "a") == false);
      REQUIRE_THROWS_WITH(g.MergeReplace("a", "e"), "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
      REQUIRE_THROWS_WITH(g.MergeReplace("e", "a"), "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
      REQUIRE_THROWS_WITH(g.MergeReplace("f", "g"), "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
    }
  }
}

//GetNodes Method
SCENARIO("GetNodes Method") {
  WHEN("Grabbing nodes from graph") {
    gdwg::Graph<std::string, int> g1{"c", "a", "b"};
    gdwg::Graph<int, int> g2{"3", "2", "1"};
    gdwg::Graph<std::string, int> g0{};
    THEN("nodes are returned in order") {
      REQUIRE(g1.GetNodes().size() == 3);
      REQUIRE(g1.GetNodes()[0] == "a");
      REQUIRE(g1.GetNodes()[1] == "b");
      REQUIRE(g1.GetNodes()[2] == "c");

      REQUIRE(g2.GetNodes().size() == 3);
      REQUIRE(g2.GetNodes()[0] == "1");
      REQUIRE(g2.GetNodes()[1] == "2");
      REQUIRE(g2.GetNodes()[2] == "3");

      REQUIRE(g0.GetNodes().size() == 0);
    }
  }
}

//GetConnected Method
SCENARIO("GetConnected Method") {
  WHEN("Grabbing connections from nodes") {
    gdwg::Graph<std::string, int> g{"a", "b", "c"};
    REQUIRE(g.InsertEdge("a", "b", 3) == true);
    REQUIRE(g.InsertEdge("a", "c", 3) == true);
    REQUIRE(g.InsertEdge("b", "c", 3) == true);
    THEN("nodes are returned in order") {
      REQUIRE(g.GetConnected("a").size() == 2);
      REQUIRE(g.GetConnected("a")[0] == "b");
      REQUIRE(g.GetConnected("a")[1] == "c");

      REQUIRE(g.GetConnected("b").size() == 1);
      REQUIRE(g.GetConnected("b")[0] == "c");

      REQUIRE(g.GetConnected("c").size() == 0);

      REQUIRE_THROWS_WITH(g.GetConnected("d"), "Cannot call Graph::GetConnected if src doesn't exist in the graph");
    }
  }
}

//GetWeights Method
SCENARIO("GetWeights Method") {
  WHEN("Grabbing weights of edges") {
    gdwg::Graph<std::string, int> g{"a", "b", "c"};
    REQUIRE(g.InsertEdge("a", "b", 5) == true);
    REQUIRE(g.InsertEdge("a", "b", 4) == true);
    REQUIRE(g.InsertEdge("a", "c", 3) == true);
    THEN("edges are returned in order") {
      REQUIRE(g.GetWeights("a","b").size() == 2);
      REQUIRE(g.GetWeights("a","b")[0] == 4);
      REQUIRE(g.GetWeights("a","b")[1] == 5);

      REQUIRE(g.GetWeights("a","c").size() == 1);
      REQUIRE(g.GetWeights("a","c")[0] == 3);

      REQUIRE(g.GetWeights("b","c").size() == 0);

      REQUIRE_THROWS_WITH(g.GetWeights("a","d"), "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
      REQUIRE_THROWS_WITH(g.GetWeights("d","a"), "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
    }
  }
}

//Find Method
SCENARIO("Find Method") {
  WHEN("Trying to find the first edge") {
    gdwg::Graph<std::string, int> g{"a", "b", "c"};
    REQUIRE(g.InsertEdge("a", "b", 5) == true);
    REQUIRE(g.InsertEdge("a", "c", 4) == true);
    std::string a1{"a"};
    std::string a2{"b"};
    int e = 5;
    int f = 6;
    auto it = g.find(a1, a2, e);
    auto it2 = g.find(a1, a2, f);

    auto begin = std::make_tuple (a1, a2, e);
    auto middle = std::make_tuple (a1, a2, e);

    THEN("it should be begin, it2 should be end") {
      REQUIRE(*(++it) == middle);
      REQUIRE(--it == g.begin());
      REQUIRE(++(++it) == g.end());
      REQUIRE(it2 == g.end());
    }
  }
}

//Erase Method bool
SCENARIO("Erase Method bool") {
  WHEN("Erasing an edge, and trying to erase nothing") {
    gdwg::Graph<std::string, int> g{"a", "b", "c"};
    REQUIRE(g.InsertEdge("a", "b", 5) == true);
    REQUIRE(g.InsertEdge("a", "b", 4) == true);
    REQUIRE(g.erase("a", "b", 5) == true);
    REQUIRE(g.erase("a", "b", 5) == false);
    REQUIRE(g.erase("a", "b", 0) == false);
    THEN("one edge left") {
      REQUIRE(g.GetConnected("a").size() == 1);
      REQUIRE(g.GetWeights("a","b")[0] == 4);
    }
  }
}

//Erase Method iterator
SCENARIO("Erase Method iterator") {
  WHEN("Erasing an edge, and trying to erase nothing") {
    gdwg::Graph<std::string, int> g{"a", "b", "c"};
    REQUIRE(g.InsertEdge("a", "b", 5) == true);
    REQUIRE(g.InsertEdge("a", "c", 4) == true);
    std::string a1{"a"};
    std::string a2{"b"};
    int e = 5;
    auto it = g.find(a1, a2, e);
    auto it2 = it;
    if (it != g.end()) {
      it2 = g.erase(it);
    }
    e = 4;
    auto begin = std::make_tuple (a1, a2, e);

    //checking nothing found case
    e = 6;
    REQUIRE(g.find(a1, a2, e) == g.end);
    THEN("one edge left, it2 points to that one") {
      REQUIRE(g.GetConnected("a").size() == 1);
      REQUIRE(g.GetWeights("a","b")[0] == 4);
      REQUIRE(it2 == g.begin());
      REQUIRE(++it2 == g.end());
    }
  }
}

//begin and end Methods
SCENARIO("begin and end Methods") {
  WHEN("Using begin and end on a two edge graph") {
    gdwg::Graph<std::string, int> g{"a", "b", "c"};
    REQUIRE(g.InsertEdge("a", "b", 5) == true);
    REQUIRE(g.InsertEdge("b", "c", 5) == true);

    THEN("begin and end should differ by 2, 3 after inserting") {
      REQUIRE(++(++g.begin()) == g.end());
      REQUIRE(++(++g.cbegin()) == g.cend());
      REQUIRE(g.begin() == g.find("a", "b", 5));
      REQUIRE(g.cbegin() == g.find("a", "b", 5));

      REQUIRE(g.InsertEdge("a", "b", 4) == true);

      REQUIRE(++(++(++g.begin())) == g.end());
      REQUIRE(++(++(++g.cbegin())) == g.cend());
      REQUIRE(g.begin() == g.find("a", "b", 4));
      REQUIRE(g.cbegin() == g.find("a", "b", 4));
    }
  }
}

//rbegin and rend Methods
SCENARIO("rbegin and rend Methods") {
  WHEN("Using rbegin and rend on a two edge graph") {
    gdwg::Graph<std::string, int> g{"a", "b", "c"};
    REQUIRE(g.InsertEdge("a", "b", 5) == true);
    REQUIRE(g.InsertEdge("b", "c", 5) == true);

    THEN("rbegin and rend should differ by 2, 3 after inserting") {
      REQUIRE(++(++g.rbegin()) == g.rend());
      REQUIRE(++(++g.crbegin()) == g.crend());
      REQUIRE(g.rbegin() == g.find("b", "c", 5));
      REQUIRE(g.crbegin() == g.find("b", "c", 5));

      REQUIRE(g.InsertEdge("b", "c", 6) == true);

      REQUIRE(++(++(++g.rbegin())) == g.rend());
      REQUIRE(++(++(++g.crbegin())) == g.crend());
      REQUIRE(g.rbegin() == g.find("b", "c", 6));
      REQUIRE(g.crbegin() == g.find("b", "c", 6));
    }
  }
}

//Thorough testing on iterator *,++,--,==,!=
SCENARIO("iterator *,++,--,==,!=") {
  WHEN("Setting up graph and using iterator *,++,--,==,!=") {
    gdwg::Graph<std::string, int> g{"a", "b", "c"};

    std::string a1{"a"};
    std::string a2{"b"};
    std::string a3{"c"};

    int e1 = 4;
    int e2 = 5;
    int e3 = 6;

    REQUIRE(g.InsertEdge(a1, a2, e1) == true);
    REQUIRE(g.InsertEdge(a1, a3, e2) == true);
    REQUIRE(g.InsertEdge(a2, a3, e3) == true);

    auto begin = std::make_tuple (a1, a2, e1);
    auto middle = std::make_tuple (a1, a3, e2);
    auto end = std::make_tuple (a2, a3, e3);

    THEN("interators should behave as iterators should") {

      //testing pre ++
      auto r1 = g.begin();
      auto r2 = ++g.begin();
      auto r3 = ++(++g.begin());
      auto r4 = ++(++(++g.begin()));

      //testing begin and end, ==, !=
      REQUIRE(r1 == g.begin());
      REQUIRE(r4 == g.end());
      REQUIRE(r3 != g.end());

      //testing pre --
      REQUIRE(--r4 == r3);
      REQUIRE(--r3 == r2);
      REQUIRE(--r2 == r1);

      //testing post ++
      r4++;
      r3++;
      r2++;

      REQUIRE(r2 == ++g.begin());
      REQUIRE(r3 == ++(++g.begin()));
      REQUIRE(r4 == ++(++(++g.begin())));

      //testing post --

      r4--;
      r2--;
      REQUIRE(r4 == r3);
      REQUIRE(r2 == r1);
      r4++;
      r2++;

      //testing *
      REQUIRE(*r1 == begin);
      REQUIRE(*r2++ == middle);
      REQUIRE(*r3 == end);
      REQUIRE(*r2-- == end);
      REQUIRE(*r2 == middle);
    }
  }
}


/*
  **************************************************************************************************
                                          2.4. Friends
  **************************************************************************************************
*/

//Equal Friend
SCENARIO("Equal Friend") {
  WHEN("Making two identical graphs") {
    gdwg::Graph<std::string, int> g{"a", "b", "c"};
    REQUIRE(g.InsertEdge("a", "b", 5) == true);
    REQUIRE(g.InsertEdge("b", "c", 5) == true);
    gdwg::Graph<std::string, int> h{"a", "b", "c"};
    REQUIRE(h.InsertEdge("a", "b", 5) == true);
    REQUIRE(h.InsertEdge("b", "c", 5) == true);

    THEN("they should be equal") {
      REQUIRE(g == h);
    }
  }
}

//Not Equal Friend
SCENARIO("Not Equal Friend") {
  WHEN("Making different graphs") {
    gdwg::Graph<std::string, int> g{"a", "b", "c"};
    REQUIRE(g.InsertEdge("a", "b", 5) == true);
    REQUIRE(g.InsertEdge("b", "c", 5) == true);
    gdwg::Graph<std::string, int> h{"a", "b", "c", "d"};
    REQUIRE(h.InsertEdge("a", "b", 5) == true);
    REQUIRE(h.InsertEdge("b", "c", 5) == true);
    gdwg::Graph<std::string, int> k{"a", "b", "c"};
    REQUIRE(k.InsertEdge("a", "b", 5) == true);
    REQUIRE(k.InsertEdge("c", "b", 5) == true);
    gdwg::Graph<std::string, int> l{"a", "b", "c"};
    REQUIRE(l.InsertEdge("a", "b", 6) == true);
    REQUIRE(l.InsertEdge("b", "c", 5) == true);

    THEN("they should not be equal") {
      REQUIRE(g != h);
      REQUIRE(g != k);
      REQUIRE(g != l);
      REQUIRE(h != k);
      REQUIRE(h != l);
      REQUIRE(k != l);
    }
  }
}

//Print Friend
SCENARIO("Print Friend") {
  WHEN("Setting up and printing out graphs"){
    gdwg::Graph<std::string, int> g1{"a", "b", "c"};
    REQUIRE(g1.InsertEdge("a", "b", 1) == true);
    REQUIRE(g1.InsertEdge("b", "c", 3) == true);
    REQUIRE(g1.InsertEdge("b", "c", 2) == true);
    std::stringstream stream1;
    stream1 << g1;

    gdwg::Graph<int, int> g2;
    std::stringstream stream2;
    stream2 << g2;

    gdwg::Graph<std::string, int> g3{"a", "b", "z"};
    REQUIRE(g3.InsertEdge("a", "b", 1) == true);
    REQUIRE(g3.InsertEdge("a", "c", 0) == true);
    REQUIRE(g3.InsertEdge("b", "c", 2) == true);
    REQUIRE(g3.InsertEdge("b", "c", 3) == true);
    REQUIRE(g3.InsertEdge("z", "b", 999) == true);
    REQUIRE(g3.InsertEdge("z", "z", -1) == true);
    std::stringstream stream3;
    stream3 << g3;

    gdwg::Graph<int, int> g4{"a", "b", "c"};
    std::stringstream stream4;
    stream4 << g4;

    THEN("They should look right"){
      REQUIRE(stream1.str() ==  "a (\n"
                                "  b | 1\n"
                                ")\n"
                                "b (\n"
                                "  c | 2\n"
                                "  c | 3\n"
                                ")\n"
                                "c (\n"
                                ")\n"
                                );
      REQUIRE(stream2.str() ==  ""
                                );
      REQUIRE(stream3.str() ==  "a (\n"
                                "  b | 1\n"
                                "  c | 0\n"
                                ")\n"
                                "b (\n"
                                "  c | 2\n"
                                "  c | 3\n"
                                ")\n"
                                "z (\n"
                                "  b | 999\n"
                                "  z | -1\n"
                                ")\n"
                                );
      REQUIRE(stream4.str() ==  "a (\n"
                                ")\n"
                                "b (\n"
                                ")\n"
                                "c (\n"
                                ")\n"
                                );
    }
  }
}
