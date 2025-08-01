#pragma once

enum class AgentType{
  YOUTH,
  WORKING,
  UNEMPLOYED
};

class Agent{
private:
  AgentType type;
  
  
  
  
public:
  int agentID;

  void agentPercieve(); // updates internal data based on position in world graph
  void agentAct(); // agent processes internal data and decides on action
};
