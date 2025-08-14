#pragma once

#include "core/ComponentManager.cpp"

#include <bitset>

// constants
constexpr size_t MAX_COMPONENTS = 64; // maximum number of types supported 

// define more basic types for system structure
using SystemTypeID = uint32_t;
using Signature = std::bitset<MAX_COMPONENTS>;


// Base System Interface
class ISystem {
  public:
    virtual ~ISystem() = default;
    virtual void update(float deltaTime, ComponentManager& components) = 0;
    virtual int getTickRate() const = 0;
    virtual Signature getSignature() const = 0;

    std::vector<EntityID> assignedEntities; // Entities the system processes
};
