#pragma once

#include <stdint.h>
#include <vector>


class EntityManager {
  private:
    uint32_t nextID = 0;
    std::vector<bool> state;

  public:
    uint32_t addID (){
      uint32_t ID = nextID;
      nextID++;

      // resize state vector to fit ID
      if (ID >= state.size()){
        state.resize(ID + 1);
      }
      
      state[ID] = true;
      return ID;
    }

    void deleteID (uint32_t dlt){
      if (dlt < state.size()){
        state[dlt] = 0;
      }
    }

    bool isAlive(uint32_t id){
      return id < state.size() && state[id];
    }
};

