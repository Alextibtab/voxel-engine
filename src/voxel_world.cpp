#include "voxel_world.hpp"

VoxelWorld::VoxelWorld(int chunk_size, int world_size_x, int world_size_y,
                       int world_size_z)
    : m_chunk_size(chunk_size), m_world_size_x(world_size_x),
      m_world_size_y(world_size_y), m_world_size_z(world_size_z) {
  for (int x = 0; x < m_world_size_x; x++) {
    for (int y = 0; y < m_world_size_z; y++) {
      for (int z = 0; z < m_world_size_z; z++) {
        m_chunks.push_back(std::make_unique<Chunk>(
            m_chunk_size, m_chunk_size, m_chunk_size, x * m_chunk_size,
            y * m_chunk_size, z * m_chunk_size));
      }
    }
  }
}

void VoxelWorld::generate(unsigned int seed) {
  m_noise_gen = std::make_unique<NoiseGenerator>(seed);
  std::cout << "Generating world with seed: " << seed << std::endl;
  int active_voxels = 0;
  for (auto &chunk : m_chunks) {
    chunk->generate(*m_noise_gen);
    active_voxels += chunk->get_active_voxels();
  }
  std::cout << "World generation complete. Active voxels: " << active_voxels
            << std::endl;
}

void VoxelWorld::render(const Shader &shader) {
  for (auto &chunk : m_chunks) {
    chunk->render(shader);
  }
}
