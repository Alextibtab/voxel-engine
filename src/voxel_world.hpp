#pragma once

#include <vector>
#include <memory>

#include "chunk.hpp"
#include "noise_generator.hpp"
#include "shader.hpp"

class VoxelWorld {
private:
  std::vector<std::unique_ptr<Chunk>> m_chunks;
  std::unique_ptr<NoiseGenerator> m_noise_gen;

  int m_chunk_size;
  int m_world_size_x, m_world_size_y, m_world_size_z;

  Chunk *get_chunk(int x, int y, int z);
  const Chunk *get_chunk(int x, int y, int z) const;

public:
  VoxelWorld(int chunk_size, int size_x, int size_y, int size_z);

  void generate(unsigned int seed);
  void render(const Shader &shader);
};
