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
      EntityID createEntity(){
        EntityID entity = entityManager.registerEntity();
        entitySignatures.insert({entity, Signature{}}); // leere Signature, explizit angelegt
        return entity;
      }
      void destroyEntity(EntityID entity){
        entityManager.deleteEntity(entity);
        componentManager.entityDestroyed(entity);
        systemManager.entitySignatureChanged(entity, Signature{}); // wirft Entity aus allen Systemen
        entitySignatures.erase(entity);
      }

      template <typename T>
      void addComponent(EntityID entity, T component){
        componentManager.addComponent<T>(entity, component);
    
        Signature sig = entitySignatures[entity];
        sig.set(componentManager.getComponentTypeID<T>());
        entitySignatures[entity] = sig;

        systemManager.entitySignatureChanged(entity, sig);
      }

      template <typename T>
      void removeComponent(EntityID entity){
        componentManager.removeComponentFromEntity<T>(entity);

        Signature sig = entitySignatures[entity];
        sig.reset(componentManager.getComponentTypeID<T>());
        entitySignatures[entity] = sig;

        systemManager.entitySignatureChanged(entity, sig);
      }

      template <typename T>
      T& getComponent(EntityID entity){
        return componentManager.getComponent<T>(entity);
      }

      template <typename T>
      ComponentTypeIndex registerComponent(){
        return componentManager.registerComponent<T>();
      }

      template <typename T>
      void registerSystem(){
        systemManager.registerSystem<T>(componentManager);
      }

      void update(float frameTime){
        systemManager.updateSystems(frameTime);
      }
  };
}




