#include <memory>

#include "ecs/system.h"

class World {
private:
  std::vector<Entity *> entities_;
  std::vector<std::unique_ptr<System>> systems_;
  ChunkManager chunk_manager_;

public:
  void update();
  void addEntity();
  void removeEntity();
};
