#include <memory>

#include "system_manager.h"

SystemManager::SystemManager(EntityManager &entity_manager)
    : entity_manager_(entity_manager) {}

void SystemManager::update() {
  for (auto system : systems_) {
    system->update(entity_manager_.get_entities());
  }
}

std::shared_ptr<System> SystemManager::add_system() {
  auto system = std::shared_ptr<System>(new System());
  systems_.push_back(system);
  return system;
}
