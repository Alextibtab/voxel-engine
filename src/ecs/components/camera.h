#pragma once

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "ecs/component.h"

class Camera : public Component {
public:
  glm::vec3 front_ = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 right_ = glm::vec3(1.0f, 0.0f, 0.0f);
  glm::vec3 world_up_ = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 up_ = glm::vec3(0.0f, 1.0f, 0.0f);

  float zoom_value_ = 45.0f;
  float yaw_ = 0;
  float pitch_ = 0;
  float aspect_ratio_ = 0;

  glm::mat4x4 view_matrix_ = glm::lookAt(
      glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) + front_, up_);
  glm::mat4x4 projection_matrix_ =
      glm::perspective(glm::radians(zoom_value_), aspect_ratio_, 0.1f, 100.0f);

  Camera() {}
  Camera(float aspect_ratio) : aspect_ratio_(aspect_ratio) {};
  ~Camera() {}
};
