#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "../../../vendor/stb_image.h"
#include "../../../vendor/glad.h"

namespace GL::Utils {

class TextureManager
{
public:
  static unsigned int loadTexture2D(const std::string &texturePath, bool presentAlpha = true)
  {
    unsigned int textureDescriptor;

    glGenTextures(1, &textureDescriptor);
    glBindTexture(GL_TEXTURE_2D, textureDescriptor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int textureWidth, textureHeight, nrChannels;

    unsigned char *textureData = stbi_load(
      texturePath.c_str(),
      &textureWidth,
      &textureHeight,
      &nrChannels,
      0);

    if (textureData) {
      bool textureFormat = (presentAlpha) ? GL_RGBA : GL_RGB;

      glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      // TODO: logging
    }

    stbi_image_free(textureData);
    return textureDescriptor;
  }
};

}// namespace GL::Utils