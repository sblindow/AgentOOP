#pragma once

#include "SpatialGraph.h"

#include <string>
#include <chrono>


class Agent{
public:
  Agent(SpatialGraph& worldGraph, std::chrono::hours timeOfDay, int iD, std::string name, int home);
  
  const int agentID;
  const std::string agentName;

  SpatialGraph& worldGraph;
  int worldPosition;
  
private:
  
  void evaluateNeeds();
  void percieve();
  void process();
  void move();
  void work();
  void buy();
  void eat();
  void sleep();

  int ticksAwake;
  int hunger;
  int energy;
  int homeLocation;
  
  SpatialGraph graph; // idiosyncratic knowledge about the world graph
};
