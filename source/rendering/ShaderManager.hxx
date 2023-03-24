#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../vendor/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace GL {

class ShaderManager 
{
public:
  ShaderManager(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);

public:
  inline GLuint getProgramID() const { return m_ProgramID; }

  void execute() const;

  void setBoolean(const std::string &uniformTag, GLboolean value) const;

  void setInteger(const std::string &uniformTag, GLint value) const;

  void setFloat(const std::string &uniformTag, GLfloat value) const;

  void setVec2(const std::string &uniformTag, const glm::vec2 &value) const;

  void setVec3(const std::string &uniformTag, const glm::vec3 &value) const;

  void setVec4(const std::string &uniformTag, const glm::vec4 &value) const;

  void setMat2(const std::string &uniformTag, const glm::mat2 &value) const;

  void setMat3(const std::string &uniformTag, const glm::mat3 &value) const;

  void setMat4(const std::string &uniformTag, const glm::mat4 &value) const;

private:
  void checkCompilationErrors(GLuint shader, std::string type);

private:
  GLuint m_ProgramID;
};
}// namespace GL