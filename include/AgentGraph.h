#pragma once

#include "SpatialGraph.h"

#include <vector>


struct AgentNode {
  const int globalID;
  const int localID;
  NodeType type;
  float familiarity;
};

class AgentGraph {
  private:
    std::vector<std::vector<Edge>> adjacencyList;
    std::vector<AgentNode> nodeData;
    
    int addWorldNode(WorldNode& worldNode);
    void addEdge(int from, int to, double distance, EdgeType type);
    float familiarity(WorldNode& currentLocation);
    int findLocalID(int worldID);

  public:
    int mapSurroundings (WorldNode& location, SpatialGraph& world, int perceptionEffort);
    NodeType getLocalType(int localNodeID);

    //
    std::vector<int> shortestPath (int start, int goal) const;       
};
