// the component manager handles:
// registering new component types
// storage for each component type
// add component to entity index mapping
// get component from entity
// remove component

# pragma once

# include <cstdint>
# include <vector>
# include <unordered_map>
# include <typeinfo>

// typedefs for Entity and Component ID
using EntityID = uint32_t;
using ComponentTypeID = uint32_t;


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
      std::unordered_map<EntityID, size_t> entityToIndex;
      std::unordered_map<size_t, EntityID> indexToEntity;
      size_t size = 0;
      
    public:
      void addComponent(EntityID entity, T component){
        // put a new component at the end of the vector
        
        if (size >= components.size()){ // resizes the vector, so it can take the new component
          components.resize(size+1);
        }
        
        size_t newIndex = size;
        entityToIndex[entity] = newIndex;
        indexToEntity[newIndex] = entity;
        components[newIndex] = component;
        size++;
      }
      
      T& getComponent(EntityID entity){
        return components[entityToIndex[entity]]; 
      }
      
      void removeEntity(EntityID entity) override { // overrides the pure virtual from the interface
          if(entityToIndex.find(entity) == entityToIndex.end()) return; // if the entity doesn't exist here, exit
          
          size_t rmIndex = entityToIndex[entity]; // get Index
          size_t lastIndex = size - 1; // Index of last component
          
          // move last element to removed position to maintain density
          components[rmIndex] = components[lastIndex];

          // update the moved entity
          EntityID movedEntity = indexToEntity[lastIndex]; // take ID from last Index
          entityToIndex[movedEntity] = rmIndex;
          indexToEntity[rmIndex] = movedEntity;

          // Clean up removed entity
          entityToIndex.erase(entity);
          indexToEntity.erase(lastIndex);
          size--; // cleanup of components vector by decreasing size counter
      }
  };

  private:
    // here we can initialize the map without knowing the types by using IComponentArray*
    std::unordered_map<ComponentTypeID, std::unique_ptr<IComponentArray>> arrays;
    std::unordered_map<const char*, ComponentTypeID> typeToID;
    ComponentTypeID nextTypeID {0};

    // helper function
    template<typename T>
    ComponentArray<T>* getArray(){ // gets component array for registered component type
      const char* typeName = typeid(T).name();
      ComponentTypeID id = typeToID[typeName];
      // cast from base pointer to specific type
      return static_cast<ComponentArray<T>*>(arrays[id].get());
     }

  public:
    // register component type (create storage)
    template<typename T>
    void registerComponent(){
      // get unique string from type T 
      const char* typeName = typeid(T).name(); // using typeinfo lib
      ComponentTypeID newID = nextTypeID++; // initializes with 0 and interates nextTypeID _afterwards_
      typeToID[typeName] = newID;
      // create wrapper for type T
      arrays[newID] = std::make_unique<ComponentArray<T>>();
    }

    // adding components of registered type
    template<typename T>
    void addComponent(EntityID entity, T component){
      // find the right wrapper for passed component
      ComponentArray<T>* array = getArray<T>();
      // call wrapper function addComponent
      array->addComponent(entity, component);
    }

    // getting component for entity
    template<typename T>
    T& getComponent(EntityID entity){
      ComponentArray<T>* array = getArray<T>();
      // call wrapper function getComponent
      return array->getComponent(entity);
    }

    void entityDestroyed(EntityID entity) {
      for (auto& pair : arrays) {
        pair.second->removeEntity(entity);
      }
    }
};
