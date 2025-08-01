#pragma once

#include <vector>

// enum class EdgeType {
//   FOOTPATH,
//   ELEVATOR
// };

// enum class NodeType{
//   INTERSECTION,
//   LOCATION
// };

struct Edge {
  int targetNode;
  double distance;
  // EdgeType type;
  // int currentLoad = 0; // default member initializer
};

// struct Node {
//   NodeType type;
//   int nodeID;
// };


class SpatialGraph {
  private:
    std::vector<std::vector<Edge>> adjacencyList; // adjacencyList: Nodes as vectors of Edges
    // std::vector<Node> nodeData; // this list stores each nodes location Data 
    
  public:
    int addNode(); // returns the new node's ID
    void addEdge(int from, int to, double distance);
    // void traverseEdge(int fromNodeID, int toNodeID);
    // void visitNode(int nodeID);
    void printGraph();
};
