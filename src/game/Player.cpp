#pragma once

#include "Player.hpp"
#include "core/ECSCoordinator.hpp"

namespace game {

  void Player::setPlayerID (core::EntityID id){
    Player::playerID = id;
    return;
  }

  core::EntityID Player::getPlayerID (){
    return Player::playerID;
  }

  void Player::createPlayer(core::ECSCoordinator ecsC){ 
    ecsC.createEntity();
  }
}
