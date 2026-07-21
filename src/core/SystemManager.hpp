#pragma once

#include <core/System.hpp>
#include <core/CoreTypes.hpp>

#include <vector>
#include <memory>
#include <algorithm>

namespace core {
  class SystemManager {
    private:
      struct SystemEntry {
        std::unique_ptr<ISystem> system;
        float lastRunTime = 0.0f;
        float targetInterval;
        uint32_t sequencePos;
      };

      std::vector<std::unique_ptr<SystemEntry>> systems;
      float currentTime = 0.0f;

      float getCurrentTime(){
        return currentTime;
      }

    public:
      template <typename T>
      void registerSystem(ComponentManager& components){
        auto system = std::make_unique<T>(components); // Referenz wird durchgereicht
  
        float interval = 1.0f / system->getTickRate();

        std::unique_ptr<SystemEntry> entry = std::make_unique<SystemEntry>();
        entry->system = std::move(system);
        entry->targetInterval = interval;
        entry->sequencePos = systems.size(); // 0 indexed

        systems.push_back(std::move(entry));
      }
      
      void entitySignatureChanged(EntityID entity, Signature newSignature){
        // Check each system to see if entity should be added/removede
        for (auto& entry : systems) {
    
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
      
      void updateSystems(float frameTime){
        currentTime += frameTime;

        for (auto& entry : systems) {
          float timeSinceLastRun = currentTime - entry->lastRunTime;

          if (timeSinceLastRun >= entry->targetInterval) {
            entry->system->update(timeSinceLastRun);
            entry->lastRunTime = currentTime;
          }
        }
      }
  };
}
