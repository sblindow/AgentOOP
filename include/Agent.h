#pragma once

#include "SpatialGraph.h"

#include <string>


class Agent{
public:
  Agent(int timeOfDay);
  
  const int agentID;
  const std::string agentName;
  
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
  
  SpatialGraph graph; // idiosyncratic knowledge about the world graph
};
