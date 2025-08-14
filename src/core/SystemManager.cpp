#pragma once

#include "core/System.hpp"

#include <typeinfo>

class SystemManager {
  private:
    struct SystemEntry {
      std::unique_ptr<ISystem> system;
      float lastRunTime = 0.0f;
      float targetInterval;
    };

    std::unordered_map<const char*, std::unique_ptr<SystemEntry>> systems;
    float currentTime = 0.0f;

    float getCurrentTime(){
      // simple time tracking - could use chrono later for a real time implementation
      return currentTime;
    }
    
  public:
    
    template<typename T>
    void registerSystem(){
      const char* typeName = typeid(T).name();

      auto system = std::make_unique<T>();
      float interval = 1.0f / system->getTickRate();

      auto entry = std::make_unique<SystemEntry>();
      entry->system = std::move(system);
      entry->targetInterval = interval;

      systems[typeName] = std::move(entry);      
    }


    template<typename T>
    T* getSystem(){
      const char* typeName = typeid(T).name();
      return static_cast<T*>(systems[typeName]->system.get());
    }


    void entitySignatureChanged(EntityID entity, Signature newSignature){
      // Check each system to see if entity should be added/removede
      for (auto& [name, entry] : systems) {
        Signature systemSig = entry->system->getSignature();
        auto& entityList = entry->system->assignedEntities;

        bool shouldHaveEntity = (newSignature & systemSig) == systemSig;
        bool hasEntity = std::find(entityList.begin(), entityList.end(), entity) != entityList.end();

        if (shouldHaveEntity && !hasEntity) {
          entityList.push_back(entity);
        } else if (!shouldHaveEntity && hasEntity){
          entityList.erase(std::remove(entityList.begin(), entityList.end(), entity), entityList.end());
        }
      }
    }

    void update(float frameTime, ComponentManager& components){
      currentTime += frameTime;

      for (auto& [name, entry] : systems) {
        float timeSinceLastRun = currentTime - entry->lastRunTime;

        if (timeSinceLastRun >= entry->targetInterval) {
          entry->system->update(timeSinceLastRun, components);
          entry->lastRunTime = currentTime;
        }
      }
    }
};
