#include "core/ComponentManager.hpp"

namespace core {
      

  template<typename T>
  ComponentManager::ComponentTypeIndex ComponentManager::ComponentTypeIndexGenerator::getIndex(){
    static ComponentTypeIndex id = counter++; // pro T nur einmal initialisier
    return id;
  }

  template <typename T>
  ComponentManager::ComponentArray<T>* ComponentManager::getArray(){ // gets component array for registered component type
    ComponentTypeIndex index = getComponentTypeId<T>();
    assert(index < componentArraysVector.size() && componentArraysVector[index] != nullptr
           && "Component type not registered!"); // bounds check, falls getComponentTypeId vor registerComponent aufgerufen wird
    return static_cast<ComponentArray<T>*>(componentArraysVector[index].get()); // cast from base pointer to specific type
  }
  
  template<typename T>
  ComponentManager::ComponentTypeIndex ComponentManager::getComponentTypeID(){
    return ComponentTypeIndexGenerator::getIndex<T>();
  }
    
  // register component type (create storage) and return Index
  template<typename T>
  ComponentManager::ComponentTypeIndex ComponentManager::registerComponent(){
    ComponentTypeIndex id = getComponentTypeId<T>(); // ID zuerst holen/erzeugen
    if (id >= componentArraysVector.size()){
      componentArraysVector.resize(id + 1); // Lücke bis zur ID auffüllen
    }
    assert(componentArraysVector[id] == nullptr && "Component type already registered!");
    componentArraysVector[id] = std::make_unique<ComponentArray<T>>(); // Zuweisung auf angelegten Platz
    return id;
  }

  // adding components of registered type
  template<typename T>
  void ComponentManager::addComponent(EntityID entity, T component){
    
    getArray<T>()->addComponent(entity, component); // call wrapper function addComponent and pass arguments
    
  }

  // getting component for entity
  template<typename T>
  T& ComponentManager::getComponent(EntityID entity){
    ComponentArray<T>* array = getArray<T>();
    // call wrapper function getComponent
    return array->getComponent(entity);
  }

  void ComponentManager::entityDestroyed(EntityID entity) {
    for (auto& component : componentArraysVector) {
      component->removeEntity(entity);
    }
  }

  template<typename T>
  void ComponentManager::removeComponentFromEntity (EntityID entity){
    
    ComponentArray<T>* array = getArray<T>();
    array->removeEntity(entity);
  }
}
