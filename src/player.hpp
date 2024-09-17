#pragma once

#include <memory>

#include "camera.hpp"
#include "voxel_world.hpp"

class GLFWwindow;

class Player {
private:
  VoxelWorld *m_world;
  std::unique_ptr<Camera> m_camera;

  bool m_paused;
  bool m_first_mouse;
  float m_last_x, m_last_y;
  float m_move_speed;
  float m_mouse_sensitivity;
  float m_aspect_ratio;

public:
  Player(float aspect_ratio);

  void set_world(VoxelWorld *world);

  void process_input(GLFWwindow *window, float delta_time);
  void process_mouse(double xpos, double ypos);
  void process_scroll(double xoffset, double yoffset);

  void update(float delta_time);

  bool is_paused() const;

  glm::mat4 get_view_matrix() const;
  glm::mat4 get_projection_matrix() const;
  glm::vec3 get_position() const;
  glm::vec3 get_front() const;
  Camera get_camera() const;

  void set_position(glm::vec3 position);

  void place_block();
  void remove_block();
};
