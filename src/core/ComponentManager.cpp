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
      
      size_t size = 0; // size variable of the components vector and the indexToEntity map.
      
    public:
      
      void addComponent(EntityID entity, T component){ // add a new component. put at the end of the component vector

        if (entityToIndex.find(entity) != entityToIndex.end()) {return;}
        
        size_t index  = components.size();
        components.push_back(component);
        entityToIndex.insert({entity, index});
        indexToEntity.insert({index, entity});
      }
      
      T& getComponent(EntityID entity){
        return components[entityToIndex[entity]]; 
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

  private:
    // here we can initialize the map without knowing the types by using std::unique_ptr<IComponentArray>
    std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> componentArrays;   
   
    ComponentTypeIndex nextTypeId {0};

    // helper function
    template<typename T>
    ComponentArray<T>* getArray(){ // gets component array for registered component type

      auto it = componentArrays.find(std::type_index(typeid(T)));
      
      assert(it != componentArrays.end() && "Component type not registered!"); // error

      return static_cast<ComponentArray<T>*>(it->second.get()); // cast from base pointer to specific type
      
     }

  public:
    // register component type (create storage)
    template<typename T>
    void registerComponent(){
      
      componentArrays.insert({std::type_index(typeid(T)), std::make_unique<ComponentArray<T>>()});
                    
    }

    // adding components of registered type
    template<typename T>
    void addComponent(EntityID entity, T component){
      
      ComponentArray<T>* array = getArray<T>(); // find the right wrapper for passed component
      array->addComponent(entity, component); // call wrapper function addComponent and pass arguments
      
    }

    // getting component for entity
    template<typename T>
    T& getComponent(EntityID entity){
      ComponentArray<T>* array = getArray<T>();
      // call wrapper function getComponent
      return array->getComponent(entity);
    }

    void entityDestroyed(EntityID entity) {
      for (auto& pair : componentArrays) {
        pair.second->removeEntity(entity);
      }
    }

    template<typename T>
    void removeComponentFromEntity (EntityID entity){
      
      ComponentArray<T>* array = getArray<T>();
      array->removeEntity(entity);
      
    }
};
