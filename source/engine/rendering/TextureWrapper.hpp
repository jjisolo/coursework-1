#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "glm/glm.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "../utility/GetSetMacro.hpp"

namespace Engine::GFX::Core
{

	class TextureWrapper
	{
	public:
		TextureWrapper() :
			m_TextureWidth (0),                m_TextureHeight(0),
			m_TextureFormat(GL_RGB),           m_ImageFormat(GL_RGB),
			m_WrapSMode    (GL_CLAMP_TO_EDGE), m_WrapTMode(GL_CLAMP_TO_EDGE),
			m_FilterMin    (GL_LINEAR),        m_FilterMax(GL_LINEAR)
		{
		}

		void inline bind() const { glBindTexture(GL_TEXTURE_2D, m_TextureID); }

		void make(GLuint imageWidth, GLuint imageHeight, GLubyte* imageData) noexcept;

		#define __gettersettertype GLuint
		makeGetterAndSetter(m_TextureID,     TextureID);

		makeGetterAndSetter(m_TextureWidth,  Width);
		makeGetterAndSetter(m_TextureHeight, Height);

		makeGetterAndSetter(m_TextureFormat, TexFormat);
		makeGetterAndSetter(m_ImageFormat,   ImgFormat);

		makeGetterAndSetter(m_WrapSMode, WrapSMode);
		makeGetterAndSetter(m_WrapTMode, WrapTMode);
		makeGetterAndSetter(m_FilterMin, FilterMin);
		makeGetterAndSetter(m_FilterMax, FilterMax);
		#undef __gettersettertype

	private:
		GLuint m_TextureID;
		GLuint m_TextureWidth;
		GLuint m_TextureHeight;

		GLuint m_TextureFormat;
		GLuint m_ImageFormat;

		GLuint m_WrapSMode;
		GLuint m_WrapTMode;
		GLuint m_FilterMin;
		GLuint m_FilterMax;
	};
}