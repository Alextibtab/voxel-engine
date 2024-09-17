#pragma once

#include <optional>
#include <vector>
#include <memory>

#include "chunk.hpp"
#include "noise_generator.hpp"
#include "voxel.hpp"

class Player;
class Shader;

class VoxelWorld {
private:
  std::vector<std::unique_ptr<Chunk>> m_chunks;
  std::unique_ptr<NoiseGenerator> m_noise_gen;
  Player *m_player;

  int m_chunk_size;
  int m_world_size_x, m_world_size_y, m_world_size_z;

  Chunk *m_get_chunk(int x, int y, int z) const;

  glm::ivec3 m_world_to_local_coordinates(const glm::ivec3 &world_pos) const;

public:
  VoxelWorld(int chunk_size, int size_x, int size_y, int size_z);

  int chunks_to_render;

  void generate(unsigned int seed);
  void render(const Shader &shader);

  std::optional<glm::ivec3> raycast(float max_distance = 5.0f) const;

  Voxel get_voxel(int x, int y, int z) const;
  void set_voxel(int x, int y, int z, const Voxel &voxel);

  Player *get_player() const;
  void set_player(Player *p);
};
