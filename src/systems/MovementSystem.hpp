#pragma once

#include "core/System.hpp"

#include "components/Position.hpp"
#include "components/Velocity.hpp"

class MovementSystem : public core::ISystem {
  public:
    MovementSystem(core::ComponentManager& cm) : ISystem(cm) {} // reicht nur durch

    int getTickRate() const override {return 60; }

    core::Signature getSignature() const override {
      core::Signature sig;
      sig.set(components.getComponentTypeID<Position>());
      sig.set(components.getComponentTypeID<Velocity>());
      return sig;
    }

    void update(float deltaTime) override {
      for (core::EntityID e : assignedEntities) {
        auto& pos = components.getComponent<Position>(e);
        auto& vel = components.getComponent<Velocity>(e);
        // ... eigentliche Logik
      }
    }
};
