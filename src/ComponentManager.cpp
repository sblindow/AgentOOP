// the component manager handles:
// registering new component types
// storage for each component type
// add component to entity
// get component from entity
// remove component
//
# pragma once

# include <vector>

struct EntityID {
  uint32_t id;
};

// implement componentManager class using type erasure on component types
class ComponentManager {

  // the interface
  class IComponentArray {
    public:
      virtual void removeEntity(EntityID entity) = 0;
    
  };
  // the derived type
  template <typename T> //Wrapper T
  
  class ComponentArray : public IComponentArray{
    private:
      std::vector<T> data;
    public:
      void addComponent(EntityID entity, T componetn);
      T& getComponent(EntityID entity);
  };

  
}
