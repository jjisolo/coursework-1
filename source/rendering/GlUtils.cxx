#include "GlUtils.hxx"

namespace GL {
namespace Utils {
  int unsigned createVertexBufferArray()
  {
    unsigned int vertexBufferArray;

    glGenVertexArrays(1, &vertexBufferArray);
    glBindVertexArray(vertexBufferArray);
  }

  int unsigned createVertexBufferObjects(float *verticies, unsigned int verticiesSizeBytes)
  {
    unsigned int vertexBufferObjects;

    glGenBuffers(1, &vertexBufferObjects);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects);
    glBufferData(GL_ARRAY_BUFFER, verticiesSizeBytes, verticies, GL_STATIC_DRAW);
  }
}// namespace Utils
}// namespace GL