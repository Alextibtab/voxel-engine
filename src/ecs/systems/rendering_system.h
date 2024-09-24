#pragma once

#include "ecs/entity_manager.h"

#include "ecs/system.h"
#include "shader.h"

class RenderingSystem : public System {
private:
  unsigned int vao_, vbo_;

public:
  RenderingSystem();
  ~RenderingSystem();

  void update(EntityVector &entities, Shader *shader);
};
