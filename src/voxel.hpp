#pragma once

#include <glm/glm.hpp>

class Voxel {
private:
  bool m_active;
  glm::vec3 m_color;

public:
  Voxel();
  Voxel(bool active, const glm::vec3 &color);

  bool is_active() const;
  void set_active(bool is_active);

  glm::vec3 get_color() const;
  void set_color(const glm::vec3 &color);
};
