#pragma once

#include <memory>

#include "system.h"

typedef std::vector<std::shared_ptr<System>> SystemVector;

class SystemManager {
private:
  EntityManager &entity_manager_;
  SystemVector systems_;
  size_t total_systems_;

public:
  SystemManager(EntityManager &entity_manager_);

  void update();
  std::shared_ptr<System> add_system();
  SystemVector &get_systems();
};
