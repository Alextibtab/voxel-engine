#pragma once

#include <memory>
#include <map>
#include <string>

#include "entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVector;
typedef std::map<std::string, EntityVector> EntityMap;

class EntityManager {
private:
  EntityVector entities_;
  EntityVector entities_to_add_;
  EntityMap entities_map_;
  size_t total_entities_ = 0;

public:
  EntityManager();

  void update();
  std::shared_ptr<Entity> add_entity(const std::string &tag);
  EntityVector &get_entities();
  EntityVector &get_entities(const std::string &tag);
};
