#pragma once

#include "core/EntityManager.hpp"
#include "core/SystemManager.hpp"
//#include "core/ComponentManager.hpp" // already included through SystemManager.hpp -> System.hpp

#include <unordered_map>

namespace core {
  class ECSCoordinator {
    private:
      EntityManager entityManager;
      ComponentManager componentManager;
      SystemManager systemManager;

      std::unordered_map<EntityID, Signature> entitySignatures;

    public:
      EntityID createEntity();
      void destroyEntity(EntityID entity);

      template <typename T>
      void addComponent(EntityID entity, T component);

      template <typename T>
      void removeComponent(EntityID entity);

      template <typename T>
      T& getComponent(EntityID entity);

      template <typename T>
      ComponentTypeIndex registerComponent();

      template <typename T>
      void registerSystem();

      void update(float frameTime);
  };
}




