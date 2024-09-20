#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>

#include "ecs/entity_manager.h"
#include "ecs/system_manager.h"
#include "ecs/entity.h"

class Game {
private:
  GLFWwindow *window_;
  EntityManager entity_manager_;
  SystemManager system_manager_;
  // Entity &player_;
  bool paused_ = false;
  bool running_ = true;

public:
  Game();
  ~Game();

  void init(const unsigned int width, const unsigned int height);
  void run();
};
