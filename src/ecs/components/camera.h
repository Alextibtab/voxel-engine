#pragma once

#include <glm/glm.hpp>

#include "ecs/component.h"

class Camera : public Component {
public:
  glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 front = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 right = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 up = glm::vec3(0.0f, 0.0f, 0.0f);

  float zoom_value = 0;
  float yaw = 0;
  float pitch = 0;

  Camera() {}
  ~Camera() {}
};
