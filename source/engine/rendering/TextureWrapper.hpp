// This file defines the `TextureWrapper` class.
#pragma once

#include "../_EngineIncludes.hpp"

// This namespace is polluted with the Internals components of the graphics API.
namespace Engine::GFX::Core
{

	// This class represents a wrapper around OpenGL textures.
	//
	// The purpose of this class is to simplify the working process
	// with the textures(loading them from the file, rendering, binding).
	class TextureWrapper
	{
	public:

		// Default class constructor that's zero-initialized all texture attributes.
		TextureWrapper() :
			m_TextureWidth (GL_ZERO),          m_TextureHeight(GL_ZERO),
			m_TextureFormat(GL_RGB),           m_ImageFormat(GL_RGB),
			m_WrapSMode    (GL_CLAMP_TO_EDGE), m_WrapTMode(GL_CLAMP_TO_EDGE),
			m_FilterMin    (GL_LINEAR),        m_FilterMax(GL_LINEAR)
		{
		}

		// Bind the texture.
		void inline bind() const 
		{ 
			glBindTexture(GL_TEXTURE_2D, m_TextureID);
		}

		// Create the texture(load it from the file), store the loaded information in
		// the class members.
		void make(GLuint imageWidth, GLuint imageHeight, GLubyte* imageData) noexcept;

		// Getters and setters for the class members.
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