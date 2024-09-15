#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <ctime>

#include "shader.hpp"
#include "player.hpp"
#include "voxel_world.hpp"

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

float delta_time = 0.0f;
float last_frame = 0.0f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void process_input(GLFWwindow *window);

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "voxel-engine", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Set callbacks
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glEnable(GL_DEPTH_TEST);

  Shader shader("../src/vertex.glsl", "../src/fragment.glsl");
  VoxelWorld world(16, 6, 2, 6);
  Player player((float)SCR_WIDTH / (float)SCR_HEIGHT);

  world.set_player(&player);
  player.set_world(&world);

  glfwSetWindowUserPointer(window, &player);

  glfwSetCursorPosCallback(
      window, [](GLFWwindow *window, double xpos, double ypos) {
        static_cast<Player *>(glfwGetWindowUserPointer(window))
            ->mouse_callback(xpos, ypos);
      });

  glfwSetScrollCallback(
      window, [](GLFWwindow *window, double xoffset, double yoffset) {
        static_cast<Player *>(glfwGetWindowUserPointer(window))
            ->scroll_callback(xoffset, yoffset);
      });

  unsigned int seed = static_cast<unsigned int>(time(nullptr));
  world.generate(seed);

  while (!glfwWindowShouldClose(window)) {
    // Calculate delta_time
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    player.process_input(window, delta_time);
    player.update(delta_time);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    shader.set_mat4f("model", glm::mat4(1.0f));

    shader.set_mat4f("view", player.get_view_matrix());
    shader.set_mat4f("projection", player.get_projection_matrix());

    shader.set_vec3f("light_pos", player.get_position());
    float time = glfwGetTime();
    glm::vec3 light_color((sin(time * 2.0f) + 1.0f) / 2.0f,
                          (sin(time * 0.7f) + 1.0f) / 2.0f,
                          (sin(time * 1.3f) + 1.0f) / 2.0f);
    shader.set_vec3f("light_color", light_color);
    shader.set_vec3f("view_position", player.get_position());

    world.render(shader);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
