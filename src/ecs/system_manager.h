#pragma once

#include "ecs/system.h"

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include <vector>

using SystemVector = std::vector<std::unique_ptr<System>>;
using SystemMap = std::unordered_map<std::type_index, System *>;

class SystemManager {
private:
  SystemVector systems_;
  SystemMap systems_map_;

public:
  SystemManager() = default;

  template <typename T, typename... Args> T &add(Args &&...args) {
    static_assert(std::is_base_of<System, T>::value,
                  "T must inherit from System");

    auto system = std::make_unique<T>(std::forward<Args>(args)...);
    T &system_ref = *system;

    systems_.push_back(std::move(system));
    systems_map_[std::type_index(typeid(T))] = &system_ref;

    return system_ref;
  }

  template <typename T> T &get() {
    auto it = systems_map_.find(std::type_index(typeid(T)));
    if (it != systems_map_.end()) {
      return *static_cast<T *>(it->second);
    }
    throw std::runtime_error("System not found");
  }

  template <typename T> bool has() const {
    return systems_map_.find(std::type_index(typeid(T))) != systems_map_.end();
  }

  template <typename T> void remove() {
    auto it =
        std::find_if(systems_.begin(), systems_.end(),
                     [](const auto &sys) { return typeid(*sys) == typeid(T); });

    if (it != systems_.end()) {
      systems_map_.erase(std::type_index(typeid(T)));
      systems_.erase(it);
    }
  }

  void updateAll(EntityVector &entities) {
    for (auto &system : systems_) {
      system->update(entities);
    }
  }
};
