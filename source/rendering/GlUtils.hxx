#pragma once

#include "../../vendor/glad.h"

namespace GL {
    namespace Utils {
        int unsigned createVertexBufferArray();

        int unsigned createVertexBufferObjects(float *verticies, int unsigned verticiesSizeBytes);
    }
}