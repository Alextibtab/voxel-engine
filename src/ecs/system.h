#pragma once

#include "ecs/entity_manager.h"

class System {
public:
  bool exists_ = false;

  System();
  ~System();

  virtual void update(EntityVector &entities);
};
