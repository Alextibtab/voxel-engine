#include "shader.hpp"

Shader::Shader(const char *vertex_path, const char *fragment_path) : id{0} {
  std::string vertex_code;
  std::string fragment_code;
  std::ifstream v_shader_file;
  std::ifstream f_shader_file;

  v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    v_shader_file.open(vertex_path);
    f_shader_file.open(fragment_path);
    std::stringstream v_shader_stream, f_shader_stream;

    v_shader_stream << v_shader_file.rdbuf();
    f_shader_stream << f_shader_file.rdbuf();

    v_shader_file.close();
    f_shader_file.close();

    vertex_code = v_shader_stream.str();
    fragment_code = f_shader_stream.str();
  } catch (std::ifstream::failure &e) {
    std::cout << "ERROR::SHADER::FILE::NOT_SUCCESSFULLY_READ: " << e.what()
              << std::endl;
  }

  const char *v_shader_code = vertex_code.c_str();
  const char *f_shader_code = fragment_code.c_str();

  unsigned int vertex, fragment;

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &v_shader_code, NULL);
  glCompileShader(vertex);
  m_check_compile_errors(vertex, "VERTEX");

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &f_shader_code, NULL);
  glCompileShader(fragment);
  m_check_compile_errors(fragment, "FRAGMENT");

  id = glCreateProgram();
  glAttachShader(id, vertex);
  glAttachShader(id, fragment);
  glLinkProgram(id);
  m_check_compile_errors(id, "PROGRAM");

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use() const { glUseProgram(id); }

void Shader::set_vec3f(const std::string &name, glm::vec3 vec) const {
  glUniform3f(glGetUniformLocation(id, name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::set_mat4f(const std::string &name, glm::mat4 matrix) const {
  glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(matrix));
}

void Shader::m_check_compile_errors(unsigned int shader,
                                    std::string type) const {
  int success;
  char info_log[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, info_log);
      std::cout << "ERROR::SHADER::COMPILATION_ERROR of type: " << type << "\n"
                << info_log << std::endl;
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, info_log);
      std::cout << "ERROR::PROGRAM_LINKING ERROR of type: " << type << "\n"
                << info_log << std::endl;
    }
  }
}
