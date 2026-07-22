#include "Player.hpp"

#include "core/ECSCoordinator.hpp"

#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "components/MoveDir.hpp"
#include "components/Grounded.hpp"
#include "components/InputState.hpp"
#include "components/LookRotation.hpp"

namespace game {

  void Player::setPlayerID (core::EntityID id){
    Player::playerID = id;
    return;
  }

  core::EntityID Player::getPlayerID (){
    return Player::playerID;
  }

  void Player::createPlayer(core::ECSCoordinator& ecsC){
    core::EntityID id = ecsC.createEntity();
    setPlayerID(id);

    ecsC.addComponent<Position>(id, Position{0});
    ecsC.addComponent<Velocity>(id, Velocity{0});
    ecsC.addComponent<MoveDir>(id, MoveDir{0});
    ecsC.addComponent<Grounded>(id, Grounded{true});
    ecsC.addComponent<InputState>(id, InputState{});
    ecsC.addComponent<LookRotation>(id, LookRotation{});
  }
}
