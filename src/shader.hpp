#pragma once

#include <gtc/type_ptr.hpp>
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
private:
  void m_check_compile_errors(unsigned int shader, std::string type) const;

public:
  unsigned int id;

  Shader(const char *vertex_path, const char *fragment_path);

  void use() const;
  void set_bool(const std::string &name, bool value) const;
  void set_int(const std::string &name, int value) const;
  void set_float(const std::string &name, float value) const;
  void set_vec3f(const std::string &name, glm::vec3 vec) const;
  void set_mat4f(const std::string &name, glm::mat4 matrix) const;
};
