#include "chunk.hpp"

Chunk::Chunk(int size_x, int size_y, int size_z, int offset_x, int offset_y,
             int offset_z)
    : m_size_x(size_x), m_size_y(size_y), m_size_z(size_z),
      m_offset_x(offset_x), m_offset_y(offset_y), m_offset_z(offset_z),
      m_voxels(size_x * size_y * size_z), m_is_dirty(true) {
  m_setup_mesh();
}

Chunk::~Chunk() {
  glDeleteVertexArrays(1, &mVAO);
  glDeleteBuffers(1, &mVBO);
}

void Chunk::generate(const NoiseGenerator &noise_gen) {
  // Create a random number generator seeded with the chunk offset
  std::mt19937 rng(std::hash<int>{}(m_offset_x ^ m_offset_y ^ m_offset_z));
  std::uniform_real_distribution<float> dist(0.0f, 1.0f);

  for (int x = 0; x < m_size_x; x++) {
    for (int y = 0; y < m_size_y; y++) {
      for (int z = 0; z < m_size_z; z++) {
        float noise_value =
            noise_gen.noise((x + m_offset_x) * 0.1f, (y + m_offset_y) * 0.4f,
                            (z + m_offset_z) * 0.2f);
        bool is_active = noise_value > 0.5f;

        // Generate a random color based on the chunk's offset
        glm::vec3 color(dist(rng), dist(rng), dist(rng));

        set_voxel(x, y, z, Voxel(is_active, color));
      }
    }
  }
  m_is_dirty = true;
}

void Chunk::render(const Shader &shader) {
  if (m_is_dirty) {
    m_update_mesh();
  }

  if (m_vertices.empty()) {
    return;
  }

  shader.set_vec3f("chunk_offset",
                   glm::vec3(m_offset_x, m_offset_y, m_offset_z));

  glBindVertexArray(mVAO);
  glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 9);
  glBindVertexArray(0);
}

Voxel Chunk::get_voxel(int x, int y, int z) const {
  if (x < 0 || x >= m_size_x || y < 0 || y >= m_size_y || z < 0 ||
      z >= m_size_z) {
    return Voxel();
  }
  return m_voxels[x + y * m_size_x + z * m_size_x * m_size_y];
}

void Chunk::set_voxel(int x, int y, int z, const Voxel &voxel) {
  if (x < 0 || x >= m_size_x || y < 0 || y >= m_size_y || z < 0 ||
      z >= m_size_z) {
    return;
  }
  m_voxels[x + y * m_size_x + z * m_size_x * m_size_y] = voxel;
  m_is_dirty = true;
}

int Chunk::get_active_voxels() const {
  return std::count_if(m_voxels.begin(), m_voxels.end(),
                       [](const Voxel &v) { return v.is_active(); });
}

void Chunk::m_setup_mesh() {
  glGenVertexArrays(1, &mVAO);
  glGenBuffers(1, &mVBO);
}

void Chunk::m_update_mesh() {
  m_vertices.clear();

  for (int x = 0; x < m_size_x; x++) {
    for (int y = 0; y < m_size_y; y++) {
      for (int z = 0; z < m_size_z; z++) {
        if (get_voxel(x, y, z).is_active()) {
          m_add_cube_faces(x, y, z, get_voxel(x, y, z).get_color());
        }
      }
    }
  }

  glBindVertexArray(mVAO);
  glBindBuffer(GL_ARRAY_BUFFER, mVBO);
  glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float),
               m_vertices.data(), GL_STATIC_DRAW);

  // Atrributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  m_is_dirty = false;
}

void Chunk::m_add_cube_faces(int x, int y, int z, const glm::vec3 &color) {
  if (!get_voxel(x, y, z + 1).is_active())
    m_add_face(x, y, z, 0, 0, 1, color); // Front
  if (!get_voxel(x, y, z - 1).is_active())
    m_add_face(x, y, z, 0, 0, -1, color); // Back
  if (!get_voxel(x + 1, y, z).is_active())
    m_add_face(x, y, z, 1, 0, 0, color); // Right
  if (!get_voxel(x - 1, y, z).is_active())
    m_add_face(x, y, z, -1, 0, 0, color); // Left
  if (!get_voxel(x, y + 1, z).is_active())
    m_add_face(x, y, z, 0, 1, 0, color); // Top
  if (!get_voxel(x, y - 1, z).is_active())
    m_add_face(x, y, z, 0, -1, 0, color); // Bottom
}

void Chunk::m_add_face(int x, int y, int z, float nx, float ny, float nz,
                       const glm::vec3 &color) {
  float vertices[54];

  if (nx != 0) {
    // Right/Left face
    float x_offset = (nx > 0) ? 1.0f : 0.0f;
    for (int i = 0; i < 6; ++i) {
      vertices[i * 9] = x + x_offset;
      vertices[i * 9 + 1] = y + (i == 2 || i == 4 || i == 5 ? 1.0f : 0.0f);
      vertices[i * 9 + 2] = z + (i == 1 || i == 2 || i == 4 ? 1.0f : 0.0f);
    }
  } else if (ny != 0) {
    // Top/Bottom face
    float y_offset = (ny > 0) ? 1.0f : 0.0f;
    for (int i = 0; i < 6; ++i) {
      vertices[i * 9] = x + (i == 1 || i == 2 || i == 4 ? 1.0f : 0.0f);
      vertices[i * 9 + 1] = y + y_offset;
      vertices[i * 9 + 2] = z + (i == 2 || i == 4 || i == 5 ? 1.0f : 0.0f);
    }
  } else {
    // Front/Back face
    float z_offset = (nz > 0) ? 1.0f : 0.0f;
    for (int i = 0; i < 6; ++i) {
      vertices[i * 9] = x + (i == 1 || i == 2 || i == 4 ? 1.0f : 0.0f);
      vertices[i * 9 + 1] = y + (i == 2 || i == 4 || i == 5 ? 1.0f : 0.0f);
      vertices[i * 9 + 2] = z + z_offset;
    }
  }

  // Set normal and color for all vertices
  for (int i = 0; i < 6; ++i) {
    vertices[i * 9 + 3] = nx;
    vertices[i * 9 + 4] = ny;
    vertices[i * 9 + 5] = nz;
    vertices[i * 9 + 6] = color.r;
    vertices[i * 9 + 7] = color.g;
    vertices[i * 9 + 8] = color.b;
  }

  this->m_vertices.insert(this->m_vertices.end(), std::begin(vertices),
                          std::end(vertices));
}
