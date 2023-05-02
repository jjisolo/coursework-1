#include "TextureWrapper.hpp"

namespace Engine::GFX::Core
{
	void TextureWrapper::make(GLuint imageWidth, GLuint imageHeight, GLubyte* imageData)
	{
		glGenTextures(1, &m_TextureID);
		m_TextureWidth  = imageWidth;
		m_TextureHeight = imageHeight;

		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexImage2D (GL_TEXTURE_2D, GL_ZERO, m_TextureFormat, imageWidth, imageHeight, 0, m_ImageFormat, GL_UNSIGNED_BYTE, imageData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_WrapSMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_WrapTMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_FilterMin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_FilterMax);

		glBindTexture(GL_TEXTURE_2D, GL_ZERO);
	}
}