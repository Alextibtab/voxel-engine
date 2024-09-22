#pragma once

#include "ecs/component.h"

class CubeMesh : public Component {
public:
  CubeMesh();
  ~CubeMesh();
  static constexpr float vertices_[] = {// Front face
                                        -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,
                                        0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
                                        // Back face
                                        -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
                                        0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
                                        // Left face
                                        -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,
                                        -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f,
                                        // Right face
                                        0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f,
                                        0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f,
                                        // Top face
                                        -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
                                        0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
                                        // Bottom face
                                        -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,
                                        0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
                                        -0.5f};

  static constexpr unsigned int num_vertices_ =
      sizeof(vertices_) / (3 * sizeof(float));
};
