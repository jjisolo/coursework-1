#pragma once

#include "../../vendor/glad.h"

namespace GL::Utils {

int unsigned createVertexBufferArray();

int unsigned createVertexBufferObjects(float *verticies, int unsigned verticiesSizeBytes);

}// namespace GL::Utils