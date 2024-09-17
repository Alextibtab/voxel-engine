#include "player.hpp"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"

Player::Player(float aspect_ratio)
    : m_camera(std::make_unique<Camera>(glm::vec3(40.0f, 20.0f, -30.0f))),
      m_paused(false), m_first_mouse(true), m_last_x(1920.0f / 2.0f),
      m_last_y(1080.0f / 2.0f), m_move_speed(15.0f), m_mouse_sensitivity(0.1f),
      m_aspect_ratio(aspect_ratio) {}

void Player::process_input(GLFWwindow *window, float delta_time) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    static double last_press_time = 0.0;
    double current_time = glfwGetTime();
    if (current_time - last_press_time > 0.2) { // Debounce
      m_paused = !m_paused;
      if (m_paused) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      }
      last_press_time = current_time;
    }
  }

  if (!m_paused) {
    float velocity = m_move_speed * delta_time;
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

    // Handle block placement and removal
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
      place_block();
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
      remove_block();
  }
}

void Player::process_mouse(double xpos, double ypos) {
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

void Player::process_scroll(double xoffset, double yoffset) {
  m_camera->zoom(yoffset);
}

void Player::update(float delta_time) {}

bool Player::is_paused() const { return m_paused; }

glm::mat4 Player::get_view_matrix() const {
  return m_camera->get_view_matrix();
}

glm::mat4 Player::get_projection_matrix() const {
  return glm::perspective(glm::radians(m_camera->zoom_value), m_aspect_ratio,
                          0.1f, 1000.0f);
}

glm::vec3 Player::get_position() const { return m_camera->position; }

glm::vec3 Player::get_front() const { return m_camera->front; }

Camera Player::get_camera() const { return *m_camera; }

void Player::set_position(glm::vec3 position) { m_camera->position = position; }

void Player::set_world(VoxelWorld *world) { m_world = world; }

void Player::place_block() {
  if (m_world) {
    auto hit_info = m_world->raycast();
    if (hit_info.has_value()) {
      m_world->set_voxel(hit_info.value().x, hit_info.value().y,
                         hit_info.value().z,
                         Voxel(true, glm::vec3(1.0f, 1.0f, 1.0f)));
    }
  }
}

void Player::remove_block() {
  if (m_world) {
    auto hit_info = m_world->raycast();
    if (hit_info.has_value()) {
      std::cout << "Removing block at: " << hit_info.value().x << ", "
                << hit_info.value().y << ", " << hit_info.value().z
                << std::endl;
      m_world->set_voxel(hit_info.value().x, hit_info.value().y,
                         hit_info.value().z, Voxel());
    }
  }
}
