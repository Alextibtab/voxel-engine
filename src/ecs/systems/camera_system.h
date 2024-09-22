#pragma once

#include "ecs/system.h"

class CameraSystem : public System {
public:
  CameraSystem();
  ~CameraSystem();

  void update(EntityVector &entities) override;
};
