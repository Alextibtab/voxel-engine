#pragma once

#include "entity.h"

class System {
public:
  virtual void update(std::vector<Entity *> &entities);
};
