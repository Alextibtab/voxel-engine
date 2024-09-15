#pragma once

#include <memory>
#include <GLFW/glfw3.h>
#include "camera.hpp"

class Player {
private:
  std::unique_ptr<Camera> m_camera;

  bool m_first_mouse;
  float m_last_x, m_last_y;
  float m_move_speed;
  float m_mouse_sensitivity;
  float m_aspect_ratio;

public:
  Player(float aspect_ratio);

  void process_input(GLFWwindow *window, float delta_time);
  void mouse_callback(double xpos, double ypos);
  void scroll_callback(double xoffset, double yoffset);

  void update(float delta_time);
  glm::mat4 get_view_matrix() const;
  glm::mat4 get_projection_matrix() const;
  glm::vec3 get_position() const;
  Camera get_camera() const;

  void set_position(glm::vec3 position);
};
