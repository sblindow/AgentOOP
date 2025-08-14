#pragma once

#include "graphs/SpatialGraph.hpp"

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

    // Pathfinding via dijkstras algortihm
    std::vector<int> shortestPath (int start, int goal) const;

    // getter functions for agent decision making
    int getGlobalID(int localID) const;
    int getLocalID(int globalID) const;
    std::vector<AgentNode> getKnownNodes() const;
    size_t getNodeCount() const;
};
