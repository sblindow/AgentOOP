#pragma once

#include <vector>

enum class EdgeType {
  FOOTPATH,
  ELEVATOR,
  STAIRWAY
};

enum class NodeType{
  INTERSECTION,
  HOME,
  SHOP,
  WORKPLACE
};

struct Edge {
  int targetNode;
  double distance;
  EdgeType type;
  int currentLoad = 0; // default member initializer
};

struct Node {
  const int nodeID;
  NodeType type;
  int currentLoad;
  const int capacity;
};


class SpatialGraph {
  private:
    std::vector<std::vector<Edge>> adjacencyList; // adjacencyList: Nodes as vectors of Edges
    std::vector<Node> nodeData; // this list stores each nodes location Data 
    
  public:
    int addNode(NodeType type); // returns the new node's ID
    void addEdge(int from, int to, double distance, EdgeType type);
    // void traverseEdge(int fromNodeID, int toNodeID);
    // void visitNode(int nodeID);
    void printGraph();
};
