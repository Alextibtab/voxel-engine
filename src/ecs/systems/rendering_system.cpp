#include <glad/glad.h>
#include <imgui.h>

#include "ecs/components/cubemesh.h"
#include "ecs/entity_manager.h"

#include "ecs/systems/rendering_system.h"

RenderingSystem::RenderingSystem() {
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);

  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);

  glBufferData(GL_ARRAY_BUFFER, sizeof(CubeMesh::vertices_),
               CubeMesh::vertices_, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

RenderingSystem::~RenderingSystem() {
  glDeleteVertexArrays(1, &vao_);
  glDeleteBuffers(1, &vbo_);
}

void RenderingSystem::update(EntityVector &entities) {
  glBindVertexArray(vao_);

  for (const auto &entity : entities) {
    if (entity->has<CubeMesh>()) {
      glDrawArrays(GL_TRIANGLE_FAN, 0, CubeMesh::num_vertices_);
    }
  }

  glBindVertexArray(0);
  glUseProgram(0);
}
