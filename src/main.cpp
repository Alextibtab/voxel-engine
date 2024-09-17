#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <ctime>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "shader.hpp"
#include "player.hpp"
#include "voxel_world.hpp"

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

float delta_time = 0.0f;
float last_frame = 0.0f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

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

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 130");

  ImGui::GetStyle().ScaleAllSizes(2.0f);
  ImGui::GetIO().FontGlobalScale = 2.0f;

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

  unsigned int seed = static_cast<unsigned int>(time(nullptr));
  world.generate(seed);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
      ImGui::Begin("Debug Menu");

      ImGui::Text("Hello, World!");
      ImGui::SliderInt("Chunks to render", &world.chunks_to_render, 0, 100);
      ImGui::End();
    }

    // Calculate delta_time
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (!ImGui::GetIO().WantCaptureMouse && !player.is_paused()) {
      player.process_mouse(xpos, ypos);
    }

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

    ImGui::Render();
    world.render(shader);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
