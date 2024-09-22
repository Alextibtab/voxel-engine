#include <memory>

#include "entity_manager.h"

EntityManager::EntityManager() {
  entities_.reserve(300);
  entities_to_add_.reserve(30);
}

void EntityManager::update() {
  for (auto entity : entities_to_add_) {
    entities_.push_back(entity);
    entities_map_[entity->tag()].push_back(entity);
  }

  entities_to_add_.clear();
}

std::shared_ptr<Entity> EntityManager::add_entity(const std::string &tag) {
  auto entity = std::shared_ptr<Entity>(new Entity(tag, total_entities_++));
  entities_to_add_.push_back(entity);
  return entity;
}

EntityVector &EntityManager::get_entities() { return entities_; }
EntityVector &EntityManager::get_entities(const std::string &tag) {
  return entities_;
};
