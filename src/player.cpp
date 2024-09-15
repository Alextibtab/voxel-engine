#include "player.hpp"
#include "GLFW/glfw3.h"
#include "ext/matrix_clip_space.hpp"

Player::Player(float aspect_ratio)
    : m_camera(std::make_unique<Camera>(glm::vec3(48.0f, 16.0f, 16.0f))),
      m_first_mouse(true), m_last_x(1920.0f / 2.0f), m_last_y(1080.0f / 2.0f),
      m_move_speed(15.0f), m_mouse_sensitivity(0.1f),
      m_aspect_ratio(aspect_ratio) {}

void Player::process_input(GLFWwindow *window, float delta_time) {
  float velocity = m_move_speed * delta_time;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    m_camera->move(Camera_Movement::FORWARD, velocity);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    m_camera->move(Camera_Movement::BACKWARD, velocity);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    m_camera->move(Camera_Movement::LEFT, velocity);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    m_camera->move(Camera_Movement::RIGHT, velocity);
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    m_camera->position += m_camera->up * velocity;
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    m_camera->position -= m_camera->up * velocity;
}

void Player::mouse_callback(double xpos, double ypos) {
  if (m_first_mouse) {
    m_last_x = xpos;
    m_last_y = ypos;
    m_first_mouse = false;
  }

  float xoffset = xpos - m_last_x;
  float yoffset = ypos - m_last_y;
  m_last_x = xpos;
  m_last_y = ypos;

  m_camera->rotate(xoffset, yoffset, m_mouse_sensitivity);
}

void Player::scroll_callback(double xoffset, double yoffset) {
  m_camera->zoom(yoffset);
}

void Player::update(float delta_time) {}

glm::mat4 Player::get_view_matrix() const {
  return m_camera->get_view_matrix();
}

glm::mat4 Player::get_projection_matrix() const {
  return glm::perspective(glm::radians(m_camera->zoom_value), m_aspect_ratio,
                          0.1f, 1000.0f);
}

glm::vec3 Player::get_position() const { return m_camera->position; }

Camera Player::get_camera() const { return *m_camera; }

void Player::set_position(glm::vec3 position) { m_camera->position = position; }
