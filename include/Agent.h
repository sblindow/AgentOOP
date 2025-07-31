#pragma once

#include <string>

class Agent{
private:
  int actorId;
  double wealth;
  
public:
  std::string name {};

  enum class AgentType{
    YOUTH,
    WORKING,
    UNEMPLOYED
  };

  AgentType type;
    
};
