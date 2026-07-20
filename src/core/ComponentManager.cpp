#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <cassert>

// typedefs for Entity and Component ID
using EntityID = uint32_t;
using ComponentTypeIndex = uint32_t;


// ComponentManager class using type erasure on component types
class ComponentManager {

  // Interface Class: pure virtual function
  class IComponentArray {
    public:
      virtual ~IComponentArray() = default; // virtual destructor
      virtual void removeEntity(EntityID entity) = 0;
  };
  
  // Wrapper Class using template invocation
  template <typename T>
  
  class ComponentArray : public IComponentArray{ // inherits from the Interface Class
    private:
      std::vector<T> components; // creates a vector with the passed type
      
      std::unordered_map<EntityID, size_t> entityToIndex; // maps an entity Id to a component index
      
      std::unordered_map<size_t, EntityID> indexToEntity; // maps the index of a component to a specific entity Id
      
      //size_t size = 0; // size variable of the components vector and the indexToEntity map.
      
    public:
      
      void addComponent(EntityID entity, T component){ // add a new component. put at the end of the component vector

        if (entityToIndex.find(entity) != entityToIndex.end()) {return;}
        
        size_t index  = components.size();
        components.push_back(component);
        entityToIndex.insert({entity, index});
        indexToEntity.insert({index, entity});
      }
      
      T& getComponent(EntityID entity){
        auto it = entityToIndex.find(entity);
        assert(it != entityToIndex.end() && "Entity has no such component!");
        return components[it->second];
      }
      
      void removeEntity(EntityID entity) override { // overrides the pure virtual from the interface
          if(entityToIndex.find(entity) == entityToIndex.end()){
             return; // if the entity doesn't exist here, exit. Iterators match.
          } else {
            size_t rmIndex = entityToIndex[entity]; // get Index of component to be removed
            size_t lastIndex = components.size()-1; // get Index of last component

            components[rmIndex] = components[lastIndex]; // overwrite components to be removed with last component
            EntityID lastEntity = indexToEntity[lastIndex]; // update maps
            entityToIndex[lastEntity] = rmIndex;
            indexToEntity[rmIndex] = lastEntity;

            entityToIndex.erase(entity); // clean up maps
            indexToEntity.erase(lastIndex);
            components.pop_back(); // remove last element in vector
          }       
      }
  };

  class ComponentTypeIndexGenerator {
    inline static ComponentTypeIndex counter = 0;

    public:
      template<typename T>
      static ComponentTypeIndex getIndex(){
        static ComponentTypeIndex id = counter++; // pro T nur einmal initialisiert
        return id;
      }
  };

  private:
    // here we can initialize the map without knowing the types by using std::unique_ptr<IComponentArray>
    std::vector<std::unique_ptr<IComponentArray>> componentArraysVector;   



    // helper function
    template<typename T>
    ComponentArray<T>* getArray(){ // gets component array for registered component type

      ComponentTypeIndex index = getComponentTypeId<T>();

      assert(index < componentArraysVector.size() && componentArraysVector[index] != nullptr
             && "Component type not registered!"); // bounds check, falls getComponentTypeId vor registerComponent aufgerufen wird
            
      return static_cast<ComponentArray<T>*>(componentArraysVector[index].get()); // cast from base pointer to specific type
      
    }
  
  public:

    // helper function
    template<typename T>
    ComponentTypeIndex getComponentTypeId(){
      return ComponentTypeIndexGenerator::getIndex<T>();
    }
    
    // register component type (create storage) and return Index
    template<typename T>
    ComponentTypeIndex registerComponent(){

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
    void addComponent(EntityID entity, T component){
      
      getArray<T>()->addComponent(entity, component); // call wrapper function addComponent and pass arguments
      
    }

    // getting component for entity
    template<typename T>
    T& getComponent(EntityID entity){
      ComponentArray<T>* array = getArray<T>();
      // call wrapper function getComponent
      return array->getComponent(entity);
    }

    void entityDestroyed(EntityID entity) {
      for (auto& component : componentArraysVector) {
        component->removeEntity(entity);
      }
    }

    template<typename T>
    void removeComponentFromEntity (EntityID entity){
      
      ComponentArray<T>* array = getArray<T>();
      array->removeEntity(entity);
      
    }
};
