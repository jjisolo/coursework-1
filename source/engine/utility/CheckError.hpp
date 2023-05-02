#pragma once

#include "spdlog/spdlog.h"

namespace Engine::Core
{
	// This is used only when some logging error/warning is triggered
	// and should not persist in the final build, that is why it is
	// guarded with the #ifdef
	#ifdef DEBUG
	GLenum __checkOpenGLErrors(const GLchar* file, GLint line)
	{
		GLenum errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			std::string error;
			switch (errorCode)
			{
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
			}
			spdlog::error("OpenGL error: %s | %s (line %d)", error.c_str(), file, line);
		}
		return errorCode;
	}

	#define checkOpenGLErrors() __checkOpenGLErrors(__FILE__, __LINE__);
	#else
	#define checkOpenGLErrors()
	#endif
}