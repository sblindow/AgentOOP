#pragma once

#include <cstdint>

#include "raylib.h"

struct InputState {
  Vector2 mouseDelta;
  int8_t sideway;
  int8_t forward;
  bool jumpPressed;
  bool crouchHeld;
};
