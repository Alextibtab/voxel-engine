#pragma once

#include <memory>
#include <vector>
#include <string>

#include "component.h"

class Entity {
private:
  const size_t id_ = 0;
  const std::string tag_ = "None";
  bool alive_ = true;
  std::vector<std::unique_ptr<Component>> components_;

  Entity(const std::string &tag, size_t id);

public:
  void destroy();
  const std::string &tag();

  friend class EntityManager;
};
