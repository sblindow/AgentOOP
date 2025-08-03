#include "AgentGraph.h"

// #include <iostream>
// #include <cstdlib>
// #include <ctime>
# include <algorithm>
#include <new>

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

void AgentGraph::mapSurroundings(WorldNode& currentLocation, SpatialGraph& world, int perceptionEffort){
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
}

