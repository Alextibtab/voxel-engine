#pragma once

#include <GLFW/glfw3.h>
#include <memory>

#include "ecs/entity_manager.h"
#include "ecs/system_manager.h"
#include "ecs/entity.h"
#include "shader.h"

class Game {
private:
  GLFWwindow *window_; // TODO: possible abstract into separate window class
  EntityManager entity_manager_;
  SystemManager system_manager_;
  Shader *shader_;
  std::shared_ptr<Entity> player_;
  bool paused_ = false;
  bool running_ = true;

public:
  Game();
  ~Game();

  void init(const unsigned int width, const unsigned int height);
  void run();
};
