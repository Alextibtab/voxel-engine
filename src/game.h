#pragma once

#include <GLFW/glfw3.h>

#include "ecs/entity_manager.h"
#include "ecs/system_manager.h"
#include "ecs/entity.h"
#include "shader.h"

class Game {
private:
  GLFWwindow *window_;
  EntityManager entity_manager_;
  SystemManager system_manager_;
  Shader *shader_;
  // Entity &player_;
  bool paused_ = false;
  bool running_ = true;

public:
  Game();
  ~Game();

  void init(const unsigned int width, const unsigned int height);
  void run();
};
