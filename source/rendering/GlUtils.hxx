#pragma once

#include "../../vendor/glad.h"

namespace GL::Utils {

namespace ColorCode {
  struct __RGBA
  {
    float r, g, b, a;
  };

  static const constexpr __RGBA Black = { 0.0f, 0.0f, 0.0f, 1.0f };
  static const constexpr __RGBA White = { 1.0f, 1.0f, 1.0f, 1.0f };
}// namespace ColorCode

inline void ClearScreen(ColorCode::__RGBA color)
{
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

int unsigned createVertexBufferArray();

int unsigned createVertexBufferObjects(float *verticies, int unsigned verticiesSizeBytes);

}// namespace GL::Utils
