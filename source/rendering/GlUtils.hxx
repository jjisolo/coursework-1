#pragma once

namespace gl {
    namespace util {
        static int unsigned createVertexBufferArray() const {
            unsigned int vertexBufferArray;

            glGenVertexArrays(1, &vertexBufferArray);
            glBindVertexArray(vertexBufferArray);
        }

        static int unsigned createVertexBufferObjects(float *verticies, size_t verticiesSizeBytes) const {
            unsigned int vertexBufferObjects;

            glGenBuffers(1, &vertexBufferObjects);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects);
            glBufferData(GL_ARRAY_BUFFER, verticiesSizeBytes, verticies, GL_STATIC_DRAW);
        }
    }
  }
}