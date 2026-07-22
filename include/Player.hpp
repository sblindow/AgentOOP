#pragma once

#include "core/CoreTypes.hpp"
#include "core/ECSCoordinator.hpp"
#include "raylib.h"

namespace game {
  class Player {
    private:
      core::EntityID playerID;

    public:
      void setPlayerID (core::EntityID id);
      core::EntityID getPlayerID();
      void createPlayer(core::ECSCoordinator ecsC);   
  };
}
