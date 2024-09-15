#include "camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : position(position), front(glm::vec3(0.0f, 0.0f, -1.0f)),
      right(glm::vec3(0.0f)), world_up(up), up(glm::vec3(0.0f)),
      zoom_value(45.0f), yaw(yaw), pitch(pitch) {
  m_update_camera_vectors();
}
glm::mat4 Camera::get_view_matrix() const {
  return glm::lookAt(position, position + front, up);
}

void Camera::move(Camera_Movement direction, float velocity) {
  if (direction == Camera_Movement::FORWARD)
    position += front * velocity;
  if (direction == Camera_Movement::BACKWARD)
    position -= front * velocity;
  if (direction == Camera_Movement::LEFT)
    position -= right * velocity;
  if (direction == Camera_Movement::RIGHT)
    position += right * velocity;
}

void Camera::rotate(float xoffset, float yoffset, float sensitivity,
                    bool constrain_pitch) {
  yaw += xoffset * sensitivity;
  pitch += -yoffset * sensitivity;

  if (constrain_pitch) {
    if (pitch > 89.0f)
      pitch = 89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;
  }

  m_update_camera_vectors();
}

void Camera::zoom(float yoffset) {
  zoom_value -= yoffset;
  if (zoom_value < 1.0f)
    zoom_value = 1.0f;
  if (zoom_value > 45.0f)
    zoom_value = 45.0f;
}

void Camera::debug_output() const {
  std::cout << "Camera Debug:" << std::endl;
  std::cout << "  Position: " << position.x << ", " << position.y << ", "
            << position.z << std::endl;
  std::cout << "  Front: " << front.x << ", " << front.y << ", " << front.z
            << std::endl;
  std::cout << "  Up: " << up.x << ", " << up.y << ", " << up.z << std::endl;
  std::cout << "  Right: " << right.x << ", " << right.y << ", " << right.z
            << std::endl;
  std::cout << "  Yaw: " << yaw << ", Pitch: " << pitch << std::endl;
  std::cout << "  Zoom: " << zoom_value << std::endl;
}

void Camera::m_update_camera_vectors() {
  glm::vec3 new_front;
  new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  new_front.y = sin(glm::radians(pitch));
  new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  front = glm::normalize(new_front);

  right = glm::normalize(glm::cross(front, world_up));
  up = glm::normalize(glm::cross(right, front));
}
