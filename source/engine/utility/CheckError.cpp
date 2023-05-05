#include "CheckError.hpp"

#ifdef DEBUG
GLenum Engine::Core::__checkOpenGLErrors(const GLchar* file, GLint line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		spdlog::info("Checking for OpenGL errors...");

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
		default: error = "NO ERROR"; break;
		}
		spdlog::info("OpenGL error: %s | %s (line %d)", error.c_str(), file, line);
	}
	return errorCode;
}
#endif