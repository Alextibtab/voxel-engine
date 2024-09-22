#pragma once

#include <tuple>
#include <string>

#include "ecs/components/transform.h"
#include "ecs/components/lifespan.h"
#include "ecs/components/input.h"
#include "ecs/components/camera.h"

using ComponentTuple = std::tuple<Transform, Camera, Lifespan, Input>;

class Entity {
private:
  const size_t id_ = 0;
  const std::string tag_ = "None";
  bool alive_ = true;
  ComponentTuple components_;

  Entity(const std::string &tag, size_t id);

public:
  template <typename T, typename... Args> T &add(Args &&...args) {
    auto &component = get<T>();
    component = T(std::forward<Args>(args)...);
    component.exists_ = true;
    return component;
  };
  template <typename T> T &get() { return std::get<T>(components_); }
  template <typename T> const T &get() const {
    return std::get<T>(components_);
  }
  template <typename T> bool has() const { return get<T>().exists_; }
  template <typename T> void remove() { get<T>() = T(); }

  void destroy();
  size_t id() const;
  bool isAlive() const;
  const std::string &tag() const;

  friend class EntityManager;
};
