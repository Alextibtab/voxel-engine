#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

enum class Camera_Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
};

class Camera {
public:
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -45.0f,
         float pitch = -45.0f);

  void move(Camera_Movement, float velocity);
  void rotate(float xoffset, float yoffset, float sensitivity,
              bool constrain_pitch = true);
  void zoom(float yoffset);

  glm::mat4 get_view_matrix() const;

  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 right;
  glm::vec3 world_up;
  glm::vec3 up;

  float zoom_value;
  float yaw;
  float pitch;

  void debug_output() const;

private:
  void m_update_camera_vectors();
};
