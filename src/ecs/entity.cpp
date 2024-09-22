#include "entity.h"

Entity::Entity(const std::string &tag, size_t id)
    : id_(id), tag_(tag), components_() {}

void Entity::destroy() { alive_ = false; }

size_t Entity::id() const { return id_; }

bool Entity::isAlive() const { return alive_; }

const std::string &Entity::tag() const { return tag_; }
