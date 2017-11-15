LibGraph
========

This is an extensible graph library in C++ with dijkstra routing.
It can store any node and edge types that
are derived from the Node and Edge classes in this library.
They can be mixed in the graph, since it stores them as polymorphic pointers.
Just override the Edge::getWeight function in order to return the right
weights of your edges, for correct routing with the Dijkstra algorithm.

This work was created based on my C++ classes at the HTW Berlin,
where i worked as a lecturer for a few years.
Use it as it is without any warranty (MIT license).


How to build
------------

Just build your project with the Graph.cpp, Edge.cpp and Node.cpp and add
the corresponding header files. A Makefile to build the files as a static library
will be added soon.


Documentation
-------------

The functions are documented in the header files.


Example
-------------

Here is a simple example how the library could be used:

```cpp
#include "Graph.h"
#include "SimpleEdge.h"
#include <iostream>

int main()
{
  // You can subclass Node, in order to add functionallity to the nodes.
  Node& rMunich = g.makeNode(Node("Munich"));
  Node& rHamburg = g.makeNode<Node>("Hamburg");
  Node& rBerlin = g.makeNode<Node>("Berlin");
  Node& rFrankfurt = g.makeNode<Node>("Frankfurt");

  // SimpleEdge is useful for some cases, but you can also subclass Edge.
  g.makeEdge<SimpleEdge>(rBerlin, rHamburg, 450);
  g.makeEdge<SimpleEdge>(rHamburg, rBerlin, 450);
  // You can make the edges bidirectional, too:
  g.makeBiEdge<SimpleEdge>(rBerlin, rMunich, 650);
  g.makeBiEdge<SimpleEdge>(rBerlin, rFrankfurt, 590);

  // find the shortest path between any type of nodes, regarding the weight of your edges
  auto path = g.findShortestPathDijkstra(rHamburg, rMunich);
  for (Edge* pEdge : path) {
      // dynamic_cast to your Edge type is useful, if you have multiple different types of edges.
      SimpleEdge* pMyEdge = dynamic_cast<SimpleEdge*>(pEdge);
      if (pMyEdge != NULL) {
          std::cout << pEdge->toString() << std::endl;
      }
  }

  return 0;
}
```
