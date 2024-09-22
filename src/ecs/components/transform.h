#pragma once

#include <glm/glm.hpp>

#include "ecs/component.h"

class Transform : public Component {
public:
  glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);

  Transform() {}
  Transform(glm::vec3 position) : position_(position) {}
  ~Transform() {}
};
