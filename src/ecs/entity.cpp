#include "entity.h"

Entity::Entity(const std::string &tag, size_t id) : id_(id), tag_(tag) {}

void Entity::destroy() { alive_ = false; }
const std::string &Entity::tag() { return tag_; }
