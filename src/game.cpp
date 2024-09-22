#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "game.h"
#include "ecs/components/transform.h"
#include "ecs/components/camera.h"
#include "ecs/components/cubemesh.h"
#include "ecs/systems/camera_system.h"
#include "ecs/systems/rendering_system.h"

Game::Game()
    : window_(nullptr), entity_manager_(), system_manager_(), shader_(nullptr),
      player_(entity_manager_.add_entity("PLAYER")) {}

Game::~Game() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
}

void Game::init(const unsigned int width, const unsigned int height) {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(width, height, "voxel-engine", NULL, NULL);

  if (window_ == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    running_ = false;
  }

  glfwMakeContextCurrent(window_);
  glfwSetFramebufferSizeCallback(window_,
                                 [](GLFWwindow *window, int width, int height) {
                                   glViewport(0, 0, width, height);
                                 });

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window_, true);
  ImGui_ImplOpenGL3_Init("#version 130");

  ImGui::GetStyle().ScaleAllSizes(2.0f);
  ImGui::GetIO().FontGlobalScale = 2.0f;

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    running_ = false;
  }

  glEnable(GL_DEPTH_TEST);

  shader_ = new Shader("../src/vertex.glsl", "../src/fragment.glsl");
}

void Game::run() {
  if (!running_) {
    glfwSetWindowShouldClose(window_, true);
  }

  float delta_time = 0.0f;
  float last_frame = 0.0f;

  player_->add<Transform>(glm::vec3(20.0f, 30.0f, 30.0f));
  player_->add<Camera>();

  auto cube = entity_manager_.add_entity("CUBE");
  cube->add<CubeMesh>();
  cube->add<Transform>(glm::vec3(20.0f, 30.0f, 20.0f));

  auto &renderer = system_manager_.add<RenderingSystem>();
  auto &camera_system = system_manager_.add<CameraSystem>();

  while (!glfwWindowShouldClose(window_)) {
    glfwPollEvents();
    entity_manager_.update();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
      ImGui::Begin("Test Menu");
      ImGui::End();
    }

    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_->use();
    camera_system.update(entity_manager_.get_entities());

    shader_->set_mat4f("model", glm::mat4(1.0f));
    shader_->set_mat4f("view", player_->get<Camera>().view_matrix_);
    shader_->set_mat4f("projection", player_->get<Camera>().projection_matrix_);

    renderer.update(entity_manager_.get_entities());

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window_);
  }
}
