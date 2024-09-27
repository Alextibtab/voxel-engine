#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/fwd.hpp>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ecs/components/cubemesh.h"
#include "ecs/entity_manager.h"
#include "shader.h"

#include "ecs/systems/rendering_system.h"

RenderingSystem::RenderingSystem() {
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);

  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);

  glBufferData(GL_ARRAY_BUFFER, sizeof(CubeMesh::vertices_),
               CubeMesh::vertices_, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(sizeof(float) * 3));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

RenderingSystem::~RenderingSystem() {
  glDeleteVertexArrays(1, &vao_);
  glDeleteBuffers(1, &vbo_);
}

void RenderingSystem::update(EntityVector &entities, Shader *shader) {
  glBindVertexArray(vao_);

  for (const auto &entity : entities) {
    if (entity->has<CubeMesh>()) {
      glm::mat4 position =
          glm::translate(glm::mat4(1.0f), entity->get<Transform>().position_);
      glm::mat4 rotation =
          glm::rotate(position, glm::radians((float)sin(glfwGetTime()) * 180),
                      glm::vec3(1.0f, 0.5f, 0.0f));
      shader->set_mat4f("model", rotation);
      glDrawArrays(GL_TRIANGLES, 0, CubeMesh::num_vertices_);
    }
  }

  glBindVertexArray(0);
  glUseProgram(0);
}
