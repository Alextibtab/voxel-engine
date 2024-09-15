#include "voxel.hpp"

Voxel::Voxel() : m_active(false), m_color(glm::vec3(1.0f)) {}

Voxel::Voxel(bool active, const glm::vec3 &color)
    : m_active(active), m_color(color) {}

bool Voxel::is_active() const { return m_active; }

void Voxel::set_active(bool is_active) { m_active = is_active; }

glm::vec3 Voxel::get_color() const { return m_color; }

void Voxel::set_color(const glm::vec3 &new_color) { m_color = new_color; }
