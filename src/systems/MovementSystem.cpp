#pragma once

#include "./core/System.hpp"

class MovementSystem : public ISystem {
  public:
    MovementSystem(ComponentManager& cm) : ISystem(cm) {} // reicht nur durch

    int getTickRate() const override {return 60; }

    Signature getSignature() const override {
      Signature sig;
      sig.set(components.getComponentTypeId<Position>());
      sig.set(components.getComponentTypeId<Velocity>());
      return sig;
    }

    void update(float deltaTime) override {
      for (EntityID e : assignedEntities) {
        auto& pos = components.getComponent<Position>(e);
        auto& vel = components.getComponent<Velocity>(e);
        // ... eigentliche Logik
      }
    }
};
