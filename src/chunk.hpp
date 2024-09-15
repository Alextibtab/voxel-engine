#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "noise_generator.hpp"
#include "shader.hpp"
#include "voxel.hpp"

class Chunk {
private:
  int m_size_x, m_size_y, m_size_z;
  int m_offset_x, m_offset_y, m_offset_z;
  std::vector<Voxel> m_voxels;
  bool m_is_dirty;

  unsigned int mVAO, mVBO;
  std::vector<float> m_vertices;

  void m_setup_mesh();
  void m_update_mesh();
  void m_add_cube_faces(int x, int y, int z, const glm::vec3 &color);
  void m_add_face(int x, int y, int z, float nx, float ny, float nz,
                  const glm::vec3 &color);

public:
  Chunk(int size_x, int size_y, int size_z, int offset_x, int offset_y,
        int offset_z);
  ~Chunk();

  void generate(const NoiseGenerator &noise_gen);
  void render(const Shader &shader);

  Voxel get_voxel(int x, int y, int z) const;
  void set_voxel(int x, int y, int z, const Voxel &voxel);

  int get_active_voxels() const;
};
