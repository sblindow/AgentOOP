#pragma once

#include "../include/InputHandling.hpp"

namespace game {
  void readInput(InputState& inputState){
    
    inputState.forward = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));
    inputState.sideway = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
    inputState.crouchHeld = IsKeyDown(KEY_C);
    inputState.jumpPressed = IsKeyPressed(KEY_SPACE);

    return;
    
  }
}
