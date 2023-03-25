#pragma once
#include <string>

namespace GL::Utils {

unsigned int loadTexture2D(const std::string &texturePath);

unsigned int releaseTexture2D(unsigned int *textureDescriptor);

}// namespace GL::Utils