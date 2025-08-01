#pragma once

#include <vector>

enum class EdgeType {
  WALKWAY,
  BUS_LINE,
  STAIRWAY,
  STREET
};

struct Edge {
  int targetNode;
  double distance;
  EdgeType type;
  int currentLoad = 0; // default member initializer
};

class SpatialGraph {
  private:
    std::vector<std::vector<Edge>> adjacencyList; // adjacencyList: Nodes as vectors of Edges
    // location data storage - we'll add this later
  public:
    int addNode(); // returns the new node's ID
    void addEdge(int fromNodeID, int toNodeID, double distance, EdgeType type);
    std::vector<Edge>& getNeighbors(int NodeID);
    int getNodeCount();
};
