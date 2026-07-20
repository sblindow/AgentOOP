#include "core/ESCCoordinator.hpp"

namespace core {
  EntityID ECSCoordinator::createEntity(){
    EntityID entity = entityManager.registerEntity();
    entitySignatures.insert({entity, Signature{}}); // leere Signature, explizit angelegt
    return entity;
  }

  void ECSCoordinator::destroyEntity(EntityID entity){
    entityManager.deleteEntity(entity);
    componentManager.entityDestroyed(entity);
    systemManager.entitySignatureChanged(entity, Signature{}); // wirft Entity aus allen Systemen
    entitySignatures.erase(entity);
  }

  template <typename T>
  void ECSCoordinator::addComponent(EntityID entity, T component){
    componentManager.addComponent<T>(entity, component);
    
    Signature sig = entitySignatures[entity];
    sig.set(componentManager.getComponentTypeID<T>());
    entitySignatures[entity] = sig;

    systemManager.entitySignatureChanged(entity, sig);
  }

  template <typename T>
  void ECSCoordinator::removeComponent(EntityID entity){
    componentManager.removeComponentFromEntity<T>(entity);

    Signature sig = entitySignatures[entity];
    sig.reset(componentManager.getComponentTypeID<T>());
    entitySignatures[entity] = sig;

    systemManager.entitySignatureChanged(entity, sig);
  }

  template <typename T>
  T& ECSCoordinator::getComponent(EntityID entity){
    return componentManager.getComponent<T>(entity);
  }

  template <typename T>
  ComponentTypeIndex ECSCoordinator::registerComponent(){
    return componentManager.registerComponent<T>();
  }

  template <typename T>
  void ECSCoordinator::registerSystem(){
    systemManager.registerSystem<T>(componentManager);
  }

  void ECSCoordinator::update(float frameTime){
    systemManager.updateSystems(frameTime);
  }
}



