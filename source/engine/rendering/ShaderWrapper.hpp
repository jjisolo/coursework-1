// This file declares the `ShaderWrapper` class.
#pragma once

#include "../_EngineIncludes.hpp"

// This namespace is polluted with the Internals components of the graphics API.
namespace Engine::GFX::Core
{
	
	// This class represents a wrapper around OpenGL Shaders linked into the program.
	// it is gathering useful functions(such as setters for the shader uniform variables
	// that are basically a bridge between GLM and OpenGl).
	//
	// The purpose of this class is to simplify the working process with the shaders.
	class ShaderWrapper
	{
	public:
		 ShaderWrapper() = default;
		~ShaderWrapper() = default;

		// Use the shader.
		inline ShaderWrapper& useShader()
		{
			glUseProgram(m_ShaderID);
		
			return(*this);
		}

		// Getter for the shader ID.
		inline GLuint getShaderID()
		{
			return(m_ShaderID);
		}

		// Compile the shader using 3 or 2 shader sources, if the source for the geometry shader is not specified,
		// the compiled program will be linked using only vertex shader and the geometry shader.
		Error compileShader(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

		// Uniform shader variables setters.
		void    setFloat   (const char* name, GLfloat value, GLboolean useShader = false);
		void    setInteger (const char* name, int value, GLboolean useShader = false);
		void    setVector2f(const char* name, GLfloat x, GLfloat y, GLboolean useShader = false);
		void    setVector2f(const char* name, const glm::vec2& value, GLboolean useShader = false);
		void    setVector3f(const char* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
		void    setVector3f(const char* name, const glm::vec3& value, GLboolean useShader = false);
		void    setVector4f(const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
		void    setVector4f(const char* name, const glm::vec4& value, GLboolean useShader = false);
		void    setMatrix4 (const char* name, const glm::mat4& matrix, GLboolean useShader = false);
		 
	private:
		// Check the compilations errors, pipe them into the graphics+global logging sink.
		Error checkCompilationErrors(GLuint object, GLboolean isProgram);

	private:
		GLuint m_ShaderID;
	};
}