#pragma once

#include "ecs/entity_manager.h"

#include "ecs/system.h"

class RenderingSystem : public System {
private:
  unsigned int vao_, vbo_;

public:
  RenderingSystem();
  ~RenderingSystem();

  void update(EntityVector &entities) override;
};
