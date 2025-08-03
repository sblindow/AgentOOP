#include "AgentGraph.h"

// #include <iostream>
// #include <cstdlib>
// #include <ctime>
# include <algorithm>

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

void AgentGraph::mapSurroundings(WorldNode& currentLocation, SpatialGraph& world, int perceptionEffort){
  std::vector<int> newNodes; // create empty vector that will contain the indices of new nodes.
   
  newNodes.push_back(addWorldNode(currentLocation)); // adds world node of current location to the agent graph

  std::vector<Edge> newEdges = world.getEdges(currentLocation.nodeID); // vector of all nodes connected to world location node
  
  for(int i = 0; i < newEdges.size(); i++){ // for each Edge connected to the current world node
    WorldNode iTargetNode = world.getNode(newEdges[i].targetNode);  
    newNodes.push_back(addWorldNode(iTargetNode));
  }

  int nodesAdded = newNodes.size();

  // add Edges between all new nodes with correct AgentGraph indices
  for(int i = 1; i < nodesAdded; i++){
    Edge iWorldEdge = newEdges[i-1];
    addEdge(newNodes[0],newNodes[i],iWorldEdge.distance, iWorldEdge.type);
  }

  // set Node familiarities
  for(int i = 0; i < nodesAdded; i++){
    nodeData[newNodes[i]].familiarity = world.getEdges(currentLocation.nodeID).size() / adjacencyList[newNodes[i]].size();
  }

  return;
}

