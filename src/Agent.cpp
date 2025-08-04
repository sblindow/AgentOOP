#include "Agent.h"
#include "SpatialGraph.h"

namespace sleepConst {
  constexpr std::chrono::hours wakeUp {6};
  constexpr std::chrono::hours sleep {22};
}

namespace simulationConst {
  constexpr int hoursPerTick {1};
  constexpr int energyLoss {2};
}

// constructor
Agent::Agent(SpatialGraph& world, std::chrono::hours timeOfDay, int iD, std::string name, int home)
  : agentID {iD}
  , agentName {name}
  , worldGraph {world}
  , worldPosition {home}
  , ticksAwake {0}
  , hoursAwake {static_cast<int>(
      (timeOfDay >= sleepConst::wakeUp && timeOfDay < sleepConst::sleep)
      ? (timeOfDay - sleepConst::wakeUp).count()
      : std::chrono::hours {0}.count())
    } // Agents get up at 6AM by default
  , hunger {20} // start with some hunger
  , energy {100 - hoursAwake * simulationConst::energyLoss} // energy depends on hours awake and hourly energy loss
  , currentGoal {Goal::NONE}
  , targetLocation {-1}
  , pathToTarget {}
  , currentActivity {Activity::IDLE}
  , activityTimer {0}
  , money {100} // starting balance
  , homeLocation {home}
  , foodCarrying {0}
  , foodFridge {0}
  , graph {}
{
  // initialize Agents perception of starting location
  WorldNode& startNode = worldGraph.getNode(worldPosition); // get reference to start position world node
  graph.mapSurroundings(startNode, worldGraph, 3); // map surroundings with initial perception effort of 3
}

void Agent::evaluateNeeds(){
  
}

void Agent::percieve(){

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
