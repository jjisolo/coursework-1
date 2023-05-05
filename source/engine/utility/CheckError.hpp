#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

#include "string"

#define DEBUG

namespace Engine::Core
{
	#ifdef DEBUG
	// This is used only when some logging error/warning is triggered
	// and should not persist in the final build, that is why it is
	// guarded with the #ifdef
	GLenum __checkOpenGLErrors(const GLchar* file, GLint line);

	#define checkOpenGLErrors() Engine::Core::__checkOpenGLErrors(__FILE__, __LINE__);
	#else
	#define checkOpenGLErrors()
	#endif
}	