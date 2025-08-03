#include "Agent.h"
#include "SpatialGraph.h"

Agent::Agent(SpatialGraph& world, std::chrono::hours timeOfDay, int iD, std::string name, int home)
  : agentID {iD}
  , agentName {name}
  , worldGraph {world}
  , worldPosition {home}
  , ticksAwake {}
  , hunger {}
  , energy {}
  , graph {}
{}

void Agent::evaluateNeeds(){
  
}

void Agent::percieve(){
  graph.addNode(worldGraph )
}

void Agent::process(){
  
}

void Agent::move(){
  
}

void Agent::work(){
  
}

void Agent::buy(){
  
}

void Agent::eat(){
  
}

void Agent::sleep(){
  
}
