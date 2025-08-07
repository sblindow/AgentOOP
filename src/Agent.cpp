#include "Agent.h"
#include "SpatialGraph.h"

#include <iostream>


namespace sleepConst {
  constexpr std::chrono::hours wakeUp {6};
  constexpr std::chrono::hours sleep {22};
}

namespace simulationConst {
  constexpr int ticksPerHour {60}; // I will need to run multiple ticks per hour, because most actions don't take a full 60 Minutes
  constexpr int energyLoss {2};
}

// constructor
Agent::Agent(SpatialGraph& world, std::chrono::hours timeOfDay, int iD, std::string name, int home)
  : agentID {iD}
  , agentName {name}
  , worldGraph {world}
  , worldPosition {home}
  , localPosition {}
  , ticksAwake {0}
  , hoursAwake {static_cast<int>(
      (timeOfDay >= sleepConst::wakeUp && timeOfDay < sleepConst::sleep)
      ? (timeOfDay - sleepConst::wakeUp).count()
      : std::chrono::hours {0}.count())
    } // Agents get up at 6AM by default
  , hunger {20} // start with some hunger
  , energy {static_cast<double>(100 - hoursAwake * simulationConst::energyLoss)} // energy depends on hours awake and hourly energy loss
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
  localPosition = graph.mapSurroundings(startNode, worldGraph, 3); // map surroundings with initial perception effort of 3
}

void Agent::evaluateNeeds(){ // Agent evaluatesNeeds and sets a Goal based on a need hierarchy
  // basic needs hierarchy: critical -> secondary -> comfort

  // critical needs: immediate action required
  if(hunger > 80){
    currentGoal = Goal::EAT;
    return;
  }

  if(energy < 20){
    currentGoal = Goal::SLEEP;
    return;
  }

  // secondary needs
  if(hunger > 50 && foodCarrying+foodFridge == 0){
    currentGoal = Goal::BUY_FOOD;
    return;
  }

  if(money < 20){
    currentGoal = Goal::WORK;
    return;
  }

  // comfort & routine needs
  if(energy < 60 && hoursAwake > 16){
    currentGoal = Goal::SLEEP;
    return;
  }

  if(hunger > 30){
    currentGoal = (foodCarrying+foodFridge > 0)? Goal::EAT : Goal::BUY_FOOD;
    return;
  }
}

void Agent::percieve(){ // Agents perceive their surroundings and update their mental map
  WorldNode& currentNode = worldGraph.getNode(worldPosition);

  // perception effort depends on familiarity with the node and energy
  int perceptionEffort = std::max(1, static_cast<int>(energy / 20));

  // map surroundings to agents personal graph
  graph.mapSurroundings(currentNode, worldGraph, perceptionEffort);

  // update load information of current node
  currentNode.currentLoad++; // Agent occupies space on the node
}

void Agent::process(){ // Decision making: decide on actions and plan paths based on Goals
  evaluateNeeds();

  switch(currentGoal){
    // NONE
    case Goal::NONE:
      currentActivity = Activity::IDLE;
      break;
      
    // EAT  
    case Goal::EAT: // can eat at the shop or at home, when carrying food or something is in the fridge
      // if there is no food at home or on the person, the agent needs to buy some
      if(foodCarrying == 0 && foodFridge == 0){
        currentGoal = Goal::BUY_FOOD;
      } else if (foodCarrying > 0 && graph.getLocalType(localPosition) == NodeType::SHOP){
        // just bought food, agent is at the shop, they can eat it right there. It's a mall type situation. Ramen noodles
        currentActivity = Activity::EATINGOUT;
        activityTimer = 1; // agent spends one hour at the shop enjoing their fresh meal
      } else if (foodFridge > 0 && graph.getLocalType(localPosition) == NodeType::HOME){
        currentActivity = Activity::EATINGIN;
        activityTimer = 2; // eating in takes more time for preparation, but is more efficient with ressources
      } else { // agent has food at home but is out and about
        if(!pathToTarget.empty()){
          pathToTarget = graph.shortestPath(localPosition, homeLocation);
        }
        move();
      }     
      break;

    // SLEEP
    case Goal::SLEEP:
      if(graph.getLocalType(localPosition) == NodeType::HOME){
        currentActivity = Activity::SLEEPING;
        activityTimer = 8; // sleep for 8 hours
      }else{
        if(!pathToTarget.empty()){
          pathToTarget = graph.shortestPath(localPosition, homeLocation);
        }
        move();
      }
      break;

    // BUY_FOOD
    case Goal::BUY_FOOD:
      {
        if(graph.getLocalType(localPosition) == NodeType::SHOP){
          currentActivity = Activity::SHOPPING;
          activityTimer = 1; // It takes an hour to shop for something
        }else{
          if(!pathToTarget.empty()){
            pathToTarget = pathToNearestNodeType(NodeType::SHOP);
          }
          move();
        }
      }
      break;

    // WORK
    case Goal::WORK:
      {
        if(graph.getLocalType(localPosition) == NodeType::WORKPLACE){
          currentActivity = Activity::WORKING;
          activityTimer = 2; // The agent works 2 hours at a time
        }else{
          if(!pathToTarget.empty()){
            pathToTarget = pathToNearestNodeType(NodeType::WORKPLACE);
          }
          move();
        }
      }
      break;
      
  }
}


std::vector<int> Agent::pathToNearestNodeType(NodeType targetType){
  // search through agent's known nodes for nearest target type
  int bestDistance = std::numeric_limits<int>::max();
  std::vector<int> bestPath {};

  // Get all knwn nodes from agent's graph
  std::vector<AgentNode> knownNodes = graph.getKnownNodes();

  // Check all known nodes for targetType
  for(const auto& node : knownNodes){
    if(node.type == targetType){
      // calculate path length to this node
      std::vector<int> path = graph.shortestPath(localPosition, node.localID);
      if(!path.empty() && static_cast<int>(path.size()) < bestDistance){
        bestDistance = path.size();
        bestPath = path;
      }
    }
  }
  return bestPath; //returs quickest path to nearest node Type 
}


void Agent::move(){
  // movement along planned path
  if(! pathToTarget.empty() && currentActivity == Activity::MOVING){
    int nextNode = pathToTarget.front();
    pathToTarget.erase(pathToTarget.begin());

    // check if the target node has capacity
    WorldNode& targetNode = worldGraph.getNode(nextNode);
    if(targetNode.currentLoad < targetNode.capacity){
      // update occupancy
      worldGraph.getNode(worldPosition).currentLoad--;
      worldPosition = nextNode;
      targetNode.currentLoad++;

      // movement costs Energy
      energy = std::max(0, static_cast<int>(energy-2));

      // debugging & test
      std::cout << agentName << " moved to node " << worldPosition << std::endl;
      } else {
        // Target is full, wait or find alternative
        std::cout << agentName << " waiting - target location full" << std::endl;
      }
    if(pathToTarget.empty()){
      currentActivity = Activity::IDLE;
    }
  }
}

void Agent::work(){
   // how much energy is spent and how much hunger is accrued depends on the tick-rate
   double ticks = static_cast<double>(simulationConst::ticksPerHour);
   
   // per hour: 5 energy
   energy = energy - (5.0/ticks);
   // per hour: 5 hunger
   hunger = hunger + (5.0/ticks);

   // per hour: 12 money
   money = money + (5.0/ticks);

   // logic for activity timer:
   // The agent should adhere to a minium time per activity
   // but they should also keep evaluating their needs
   // if critical needs come up, the agent should abandon their activity
   // There should also be a system that lets agents spend longer working, if all needs are met and they are inclinded to do so   
}

void Agent::buy(){
  
}

void Agent::eat(){
  
}

void Agent::sleep(){
  
}

void Agent::explore(){
  
}

void Agent::updateTimers(){
  
}

void Agent::tick(){
  
}
