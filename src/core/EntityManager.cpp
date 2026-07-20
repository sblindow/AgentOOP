#include "core/EntityManager.hpp"

uint32_t EntityManager::registerEntity (){
  uint32_t Id = 0;

  if (freeIds.empty()) {
    Id = idState.size();
    idState.resize(Id + 1);
  } else {
    Id = freeIds.back();
    freeIds.pop_back();
  }
    
  idState[Id] = true;
  return Id;
  }

void EntityManager::deleteEntity (uint32_t deleteId){
  if (deleteId < idState.size() && idState[deleteId]){
    idState[deleteId] = false;
    freeIds.push_back(deleteId);
  }
}

bool EntityManager::isAlive(uint32_t id) const{
  return id < idState.size() && idState[id];
}

