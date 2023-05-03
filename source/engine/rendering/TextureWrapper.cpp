// This class implements the `TextureWrapper` class.
#include "TextureWrapper.hpp"

namespace Engine::GFX::Core
{
	void TextureWrapper::make(GLuint imageWidth, GLuint imageHeight, GLubyte* imageData) noexcept
	{
		// Generate the texture
		glGenTextures(1, &m_TextureID);
		m_TextureWidth  = imageWidth;
		m_TextureHeight = imageHeight;

		// Bind the texture and populate it with the given data.
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexImage2D (GL_TEXTURE_2D, GL_ZERO, m_TextureFormat, imageWidth, imageHeight, 0, m_ImageFormat, GL_UNSIGNED_BYTE, imageData);

		// Set the texture parameter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_WrapSMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_WrapTMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_FilterMin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_FilterMax);

		// Unbind the texture.
		glBindTexture(GL_TEXTURE_2D, GL_ZERO);
	}
}