#pragma once

#include "SpatialGraph.h"
#include "AgentGraph.h"

#include <string>
#include <chrono>

enum class Goal {
  NONE,
  EAT,
  SLEEP,
  BUY_FOOD,
  WORK  
};

enum class Activity {
  IDLE,
  MOVING,
  EATING,
  SLEEPING,
  SHOPPING,
  WORKING
};



// Agent Class
class Agent{
public:
  Agent(SpatialGraph& worldGraph, std::chrono::hours timeOfDay, int iD, std::string name, int home);
  
  const int agentID;
  const std::string agentName;

  SpatialGraph& worldGraph;
  int worldPosition;

  void tick(); // this is the simulation method that defines what an agent does every tick
  
private:
  // core behavior functions
  void evaluateNeeds();
  void percieve();
  void process();
  void move();
  void work();
  void buy();
  void eat();
  void sleep();

  // helper functions
  void planPathTo(int targetNode);
  int findNearestNodeType(NodeType targetType);
  void updateTimers();

  // basic state
  int ticksAwake;
  int hoursAwake;
  int hunger;
  int energy;

  // Goal and Activity tracking
  Goal currentGoal;
  int targetLocation;
  std::vector<int> pathToTarget;
  Activity currentActivity;
  int activityTimer; // countdonw for current Activity

  // property
  int money;
  int homeLocation;
  int foodCarrying;
  int foodFridge;
  
  // knowledge
  AgentGraph graph; // Agents idiosyncratic knowledge about the world
};
