#include "SpatialGraph.h"

#include <iostream>


int SpatialGraph::addNode(NodeType type){
  int newNodeID = adjacencyList.size();

  Node newNode{type, newNodeID};
  
  adjacencyList.push_back(std::vector<Edge>()); // adds empty vector
  nodeData.push_back(newNode);
  
  return newNodeID;
}

void SpatialGraph::addEdge(int newFrom, int newTo, double newDistance, EdgeType type){
  // Create Edge from -> to
  Edge FromTo {newTo, newDistance, type};
  adjacencyList[newFrom].push_back(FromTo);

  // Create Edge back to -> from
  Edge ToFrom {newFrom, newDistance, type};
  adjacencyList[newTo].push_back(ToFrom);
}

void SpatialGraph::printGraph(){
  // Loop through each node (outer loop)
  for(int i = 0; i < adjacencyList.size(); i++){
    std::cout << "Node [" << i << "] ";
    // For each node, loop through its edges (inner loop)
    for(int j = 0; j < adjacencyList[i].size(); j++){
      // Print the connection information
      int toNode = adjacencyList[i][j].targetNode;
      double dist = adjacencyList[i][j].distance;
      std::cout << "--> Node[" << toNode << "] (distance:" << dist << "), ";
    }
    std::cout << std::endl;
  }
}
