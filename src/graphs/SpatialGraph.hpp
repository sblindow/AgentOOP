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

struct WorldNode {
  const int nodeID;
  const int capacity;
  NodeType type;
  int currentLoad = 0; // default member initializer
};

class SpatialGraph {
  private:
    std::vector<std::vector<Edge>> adjacencyList; // adjacencyList: Nodes as vectors of Edges
    std::vector<WorldNode> nodeData; // this list stores each nodes location Data 
    
  public:
    // Graph creation functions: used for explicit set-up of world graph
    int addNode(NodeType type, int capacity); // returns the new node's ID
    void addEdge(int from, int to, double distance, EdgeType type);   

    // This function is for agents that percieve the surroundings of a node
    std::vector<Edge> getEdges(int nodeID);
    WorldNode& getNode(int nodeID);
     
    // testing and debugging
    void printGraph();
};
