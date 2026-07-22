#pragma once

#include "core/System.hpp"

#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "components/Grounded.hpp"
#include "components/MoveDir.hpp"
#include "components/InputState.hpp"
#include "components/LookRotation.hpp"

#include <raylib.h>
#include <raymath.h>
//#include <math.h>


// Movement Constants

#define GRAVITY 32.0f
#define MAX_SPEED 20.0f
#define CROUCH_SPEED 5.0f
#define JUMP_FORCE 12.0f
#define MAX_ACCEL 150.0f

// Grounded drag
#define FRICTION 0.86f

// Increasing air drag, increases strafe speed
#define AIR_DRAG 0.98f


// Responsiveness for turning movement direction to looked direction
#define CONTROL 15.0f

class MovementSystem : public core::ISystem {
  public:
    
    MovementSystem(core::ComponentManager& cm) : ISystem(cm) {} // reicht nur durch

    int getTickRate() const override {return 60; }

    core::Signature getSignature() const override {
      core::Signature sig;
      sig.set(components.getComponentTypeID<Position>());
      sig.set(components.getComponentTypeID<Velocity>());
      sig.set(components.getComponentTypeID<MoveDir>());
      sig.set(components.getComponentTypeID<Grounded>());
      sig.set(components.getComponentTypeID<InputState>());
      sig.set(components.getComponentTypeID<LookRotation>());
      return sig;
    }

    void update(float deltaTime) override {
      
      for (core::EntityID e : assignedEntities) {
        
        auto& pos = components.getComponent<Position>(e).value;
        auto& vel = components.getComponent<Velocity>(e).value;
        auto& grd = components.getComponent<Grounded>(e).isGrounded;
        auto& dir = components.getComponent<MoveDir>(e).dir;
        auto& inp = components.getComponent<InputState>(e);
        auto& rot = components.getComponent<LookRotation>(e).rotation.x;
        
        // ... eigentliche Logik

        Vector2 input = (Vector2){(float)inp.sideway, (float)-inp.forward};

        #if defined(NORMALIZE_INPUT)
          // slow down diagonal movement
          if ((inp.sideway != 0) && (inp.forward != 0))
            input = Vector2Normalize(input);
        #endif

        float delta = GetFrameTime();

        if (!grd){
          vel.y -= GRAVITY * delta;
        }

        if (grd && inp.jumpPressed){
          vel.y = JUMP_FORCE;
          grd = false;
          // sound can be played at this moment
        }

        Vector3 front = (Vector3){sinf(rot), 0.f, cosf(rot)};
        Vector3 right = (Vector3){cosf(-rot), 0.f, sinf(-rot)};

        Vector3 desiredDir = (Vector3){
          input.x * right.x + input.y * front.x, //x
          0.0f,                                  //y
          input.x * right.z + input.y * front.z //z
        };

        dir = Vector3Lerp(dir, desiredDir, CONTROL * delta);

        float decel = (grd ? FRICTION : AIR_DRAG);

        Vector3 hvel = (Vector3){
          vel.x * decel,
          0.0f,
          vel.z * decel
        };

        float hvelLength = Vector3Length(hvel); // Magnitude

        if (hvelLength < (MAX_SPEED * 0.01f)){
          hvel = (Vector3){0};
        }

        // this is what creates strafing
        float speed = Vector3DotProduct(hvel, dir);

        
        // Whenever the amount of acceleration to add is clamped by the maximum
        // acceleration constant, a Player can make the speed faster by bringing the
        // direction closer to horizontal velocity angle More info here:
        // https://youtu.be/v3zT3Z5apaM?t=165

        float maxSpeed = (inp.crouchHeld ? CROUCH_SPEED : MAX_SPEED);

        float accel = Clamp(maxSpeed - speed, 0.f, MAX_ACCEL * delta);

        hvel.x += dir.x * accel;
        hvel.z += dir.z * accel;

        vel.x = hvel.x;
        vel.z = hvel.z;

        pos.x += vel.x * delta;
        pos.y += vel.y * delta;
        pos.z += vel.z * delta;

        // simple collision system with floor. Should be upgraded to collision with collision map later.
        if (pos.y <= 0.0f){
          pos.y = 0.0f;
          vel.y = 0.0f;
          grd = true; // Enable jumping
        }
      }
    }
};
