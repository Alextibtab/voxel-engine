#pragma once

#include "ecs/entity_manager.h"

class System {
private:
  System();

public:
  virtual void init();
  virtual void update(EntityVector &entities);

  friend class SystemManager;
};
