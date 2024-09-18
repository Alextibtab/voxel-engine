#include "game.h"
#include "ecs/entity_manager.h"

void Game::init() {
  auto entity_manager = new EntityManager();
  auto system_manager = new SystemManager(&entity_manager_);
}
