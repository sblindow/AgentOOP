#pragma once

#include <core/System.hpp>
#include <core/CoreTypes.hpp>

#include <vector>

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

      float getCurrentTime();

    public:
      template <typename T>
      void registerSystem(ComponentManager& components);
      void entitySignatureChanged(EntityID entity, Signature newSignature);
      void updateSystems(float frameTime);
  };
}
