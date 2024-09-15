#include "voxel_world.hpp"
#include "player.hpp"

VoxelWorld::VoxelWorld(int chunk_size, int world_size_x, int world_size_y,
                       int world_size_z)
    : chunks_to_render(0), m_chunk_size(chunk_size),
      m_world_size_x(world_size_x), m_world_size_y(world_size_y),
      m_world_size_z(world_size_z) {
  for (int x = 0; x < m_world_size_x; x++) {
    for (int y = 0; y < m_world_size_y; y++) {
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
  for (size_t i = 0; i < chunks_to_render && i < m_chunks.size(); ++i) {
    m_chunks[i]->render(shader);
  }
}

std::optional<glm::ivec3> VoxelWorld::raycast(float max_distance) const {
  if (!m_player) {
    return std::nullopt;
  }

  glm::vec3 start = m_player->get_position();
  glm::vec3 direction = m_player->get_front();

  for (float distance = 0.0f; distance <= max_distance; distance += 0.1f) {
    glm::vec3 check_position = start + direction * distance;
    glm::ivec3 block_position = glm::floor(check_position);

    int chunk_x = block_position.x / m_chunk_size;
    int chunk_y = block_position.y / m_chunk_size;
    int chunk_z = block_position.z / m_chunk_size;

    Chunk *chunk =
        m_get_chunk(block_position.x, block_position.y, block_position.z);
    if (chunk) {
      glm::ivec3 local_position = m_world_to_local_coordinates(block_position);
      if (chunk->get_voxel(local_position.x, local_position.y, local_position.z)
              .is_active()) {
        return block_position;
      }
    }
  }

  return std::nullopt;
}

Voxel VoxelWorld::get_voxel(int x, int y, int z) const {
  Chunk *chunk = m_get_chunk(x, y, z);
  if (chunk) {
    glm::ivec3 local_position = m_world_to_local_coordinates({x, y, z});
    return chunk->get_voxel(local_position.x, local_position.y,
                            local_position.z);
  }
  return Voxel();
}

void VoxelWorld::set_voxel(int x, int y, int z, const Voxel &voxel) {
  Chunk *chunk = m_get_chunk(x, y, z);
  if (chunk) {
    glm::ivec3 local_position = m_world_to_local_coordinates({x, y, z});
    chunk->set_voxel(local_position.x, local_position.y, local_position.z,
                     voxel);
  }
}

Player *VoxelWorld::get_player() const { return m_player; }

void VoxelWorld::set_player(Player *player) { m_player = player; }

Chunk *VoxelWorld::m_get_chunk(int x, int y, int z) const {
  int chunk_x = x / m_chunk_size;
  int chunk_y = y / m_chunk_size;
  int chunk_z = z / m_chunk_size;

  if (chunk_x < 0 || chunk_x >= m_world_size_x || chunk_y < 0 ||
      chunk_y >= m_world_size_y || chunk_z < 0 || chunk_z >= m_world_size_z) {
    return nullptr;
  }

  int index = chunk_x + chunk_y * m_world_size_x +
              chunk_z * m_world_size_x * m_world_size_y;

  return m_chunks[index].get();
}

glm::ivec3 VoxelWorld::m_world_to_local_coordinates(
    const glm::ivec3 &world_position) const {
  return {world_position.x % m_chunk_size, world_position.y % m_chunk_size,
          world_position.z % m_chunk_size};
}
