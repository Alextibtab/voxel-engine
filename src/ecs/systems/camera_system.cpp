#include "ecs/systems/camera_system.h"
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>

CameraSystem::CameraSystem() {}
CameraSystem::~CameraSystem() {}

void CameraSystem::update(EntityVector &entities) {
  for (auto entity : entities) {
    if (entity->has<Camera>() && entity->has<Transform>()) {
      auto camera = entity->get<Camera>();
      auto transform = entity->get<Transform>();

      glm::vec3 updated_front_;
      updated_front_.x =
          cos(glm::radians(camera.yaw_)) * cos(glm::radians(camera.pitch_));
      updated_front_.y = sin(glm::radians(camera.pitch_));
      updated_front_.z =
          sin(glm::radians(camera.yaw_)) * cos(glm::radians(camera.pitch_));

      camera.front_ = glm::normalize(updated_front_);
      camera.right_ =
          glm::normalize(glm::cross(camera.front_, camera.world_up_));
      camera.up_ = glm::normalize(glm::cross(camera.right_, camera.front_));

      camera.view_matrix_ = glm::lookAt(
          transform.position_, transform.position_ + camera.front_, camera.up_);
    }
  }
}
