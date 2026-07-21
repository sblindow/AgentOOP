#pragma once

#include <core/CoreTypes.hpp>
#include <vector>
#include <unordered_map>
#include <cassert>

namespace core {

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
        std::unordered_map<EntityID, size_t> entityToIndex; // maps an entity ID to a component index
        std::unordered_map<size_t, EntityID> indexToEntity; // maps the index of a component to a specific entity ID

      public:

        void addComponent(EntityID entity, T component){
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
        
        void removeEntity(EntityID entity) override{
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
        template <typename T>
        static ComponentTypeIndex getIndex(){
          static ComponentTypeIndex id = counter++;
          return id;
        }
    };

    private:
      // here we can initialize the map without knowing the types by using std::unique_ptr<IComponentArray>
      std::vector<std::unique_ptr<IComponentArray>> componentArraysVector;

      // helper function
      template <typename T>
      ComponentArray<T>* getArray(){
        ComponentTypeIndex index = getComponentTypeID<T>();
        assert(index < componentArraysVector.size() && componentArraysVector[index] != nullptr
               && "Component type not registered!");
        return static_cast<ComponentArray<T>*>(componentArraysVector[index].get());
      }

    public:
      // helper function
      template <typename T>
      ComponentTypeIndex getComponentTypeID(){
        return ComponentTypeIndexGenerator::getIndex<T>();
      }

      template <typename T>
      ComponentTypeIndex registerComponent(){
        ComponentTypeIndex id = getComponentTypeID<T>(); // ID zuerst holen/erzeugen

        if (id >= componentArraysVector.size()){
          componentArraysVector.resize(id + 1); // Lücke bis zur ID auffüllen
        }
        
        assert(componentArraysVector[id] == nullptr && "Component type already registered!");
        componentArraysVector[id] = std::make_unique<ComponentArray<T>>(); // Zuweisung auf angelegten Platz
        return id;
      }

      template <typename T>
      void addComponent(EntityID entity, T component){
        getArray<T>()->addComponent(entity, component); // call wrapper function addComponent and pass arguments
      }

      template <typename T>
      T& getComponent(EntityID entity){
        return getArray<T>()->getComponent(entity);
      }

      template <typename T>
      void removeComponentFromEntity(EntityID entity){
        getArray<T>()->removeEntity(entity);
      }

      void entityDestroyed(EntityID entity){
        for (auto& component : componentArraysVector){
          component->removeEntity(entity);
        }
      }
  };
}
