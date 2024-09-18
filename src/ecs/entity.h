#pragma once

#include <memory>
#include <vector>

#include "component.h"

class Entity {
private:
  std::vector<std::unique_ptr<Component>> components_;

public:
  template <typename T> T *addComponent();

  template <typename T> T *getComponent();
};
