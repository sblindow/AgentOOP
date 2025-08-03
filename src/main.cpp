#include "SpatialGraph.h"

#include <iostream>

int main(void){
  // Create a graph object
  SpatialGraph graph;

  // Add three nodes (locations)
  int room1 = graph.addNode(NodeType::LOCATION);
  int room2 = graph.addNode(NodeType::LOCATION);
  int room3 = graph.addNode(NodeType::INTERSECTION);

  // Connect the locations in a triangle pattern
  graph.addEdge(room1, room2, 5.0, EdgeType::FOOTPATH);
  graph.addEdge(room2, room3, 3.0, EdgeType::FOOTPATH);
  graph.addEdge(room3, room1, 4.0, EdgeType::ELEVATOR);

  // Print the graph that results from this to test the implementation
  std::cout << "Graph Test:" << std::endl;
  graph.printGraph();

  
  return 0;
}
