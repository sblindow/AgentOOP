#include "AgentGraph.h"

// #include <iostream>
// #include <cstdlib>
// #include <ctime>

# include <algorithm>
# include <queue>
# include <limits>

int AgentGraph::addWorldNode(WorldNode& worldNode){
  int newNodeID = adjacencyList.size();
  AgentNode newAgentNode {worldNode.nodeID, newNodeID, worldNode.type};
  nodeData.push_back(newAgentNode); // nodeData vector takes AgentNode struct
  // pushing the default constructor of std::vector<> which creates an empty vector:
  adjacencyList.push_back(std::vector<Edge>());
  return newNodeID;
}

void AgentGraph::addEdge(int from, int to, double distance, EdgeType type){
  Edge fromTo {to, distance, type};
  adjacencyList[from].push_back(fromTo);
  Edge toFrom {from, distance, type};
  adjacencyList[to].push_back(toFrom);
  
  return;  
}

float AgentGraph::familiarity(WorldNode& currentLocation){
  // search for currentLocation in Agent nodeData
  // can optimize/ refactor for better performance 
  auto iterator = std::find_if(
    nodeData.begin(), nodeData.end(),
    [&](const AgentNode& a) {return a.globalID == currentLocation.nodeID;}
  );

  if(iterator != nodeData.end()){
    // found --> determine which element has been found
    std::size_t index = std::distance(nodeData.begin(), iterator);
    // index is the zero-based position of the matching element
    return nodeData[index].familiarity;
  }

  return 0;
    
}

int AgentGraph::findLocalID(int worldID){ // returns -1 when node is not found
  // can optimize/ refactor for better performance 
  auto iterator = std::find_if(
    nodeData.begin(), nodeData.end(),
    [&](const AgentNode& a) {return a.globalID == worldID;}
  );
  
  if(iterator != nodeData.end()){
    std::size_t index = std::distance(nodeData.begin(), iterator);
    return nodeData[index].localID;
  }

  return -1;
}

int AgentGraph::mapSurroundings(WorldNode& currentLocation, SpatialGraph& world, int perceptionEffort){
  int currentLocalID;
  
  // Handle current location
  if(familiarity(currentLocation) == 0){ // if the current node is unknown to the Agent
    currentLocalID = addWorldNode(currentLocation); // add it to the agent graph
  } else { // if the current node is known, we still need to know the localID to add new connections
    currentLocalID = findLocalID(currentLocation.nodeID);
  }

  // find unknown Neighbors
  std::vector<Edge> neighbors = world.getEdges(currentLocation.nodeID);
  
  for(int i = 0; i < neighbors.size(); i++){
    const Edge& edge = neighbors[i];
    WorldNode& targetNode = world.getNode(edge.targetNode);
    if(familiarity(targetNode) == 0){
      int targetLocalID = addWorldNode(targetNode);
      addEdge(currentLocalID, targetLocalID, edge.distance, edge.type);
    }
  }
  return currentLocalID;
}

NodeType AgentGraph::getLocalType(int localNodeID){
  return nodeData[localNodeID].type;
}

std::vector<int> AgentGraph::shortestPath(int start, int goal) const {
  size_t n = adjacencyList.size();
  std::vector<double> dist (n, std::numeric_limits<double>::infinity());
  std::vector<int> prev (n, -1);

  dist[start] = 0.0;
  // Min-heap: (distance so far, nodeID)
  using Pair = std::pair<double, int>;
  std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;
  pq.emplace(0.0, start);

  while(!pq.empty()) {
    auto [d, u] = pq.top(); pq.pop();
    if (u == goal) break; // early exit
    if (d > dist[u]) continue; // distance validation

    for (const Edge& edge : adjacencyList[u]) {
      int v = edge.targetNode;
      double weight = edge.distance;
      if(dist[v] > dist[u] + weight){
        dist[v] = dist[u] + weight;
        prev[v] = u;
        pq.emplace(dist[v], v);
      }
    }
  }

  // reconstruct path
  std::vector<int> path;
  if (dist[goal] == std::numeric_limits<double>::infinity()) return path; // no path

  for (int i = goal; i != -1; i = prev[i]){
    path.push_back(i);
  }
  std::reverse(path.begin(), path.end());
  return path;
}
