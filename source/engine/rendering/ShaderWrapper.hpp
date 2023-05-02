#pragma once

#include "string"
#include "memory"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "../utility/Error.hpp"

namespace Engine::GFX::Core
{
	class ShaderWrapper
	{
	public:
		ShaderWrapper() = default;
		~ShaderWrapper() = default;

		inline ShaderWrapper& useShader() {
			glUseProgram(m_ShaderID);
			return(*this);
		}

		inline GLuint getShaderID() {
			return(m_ShaderID);
		}

		Error compileShader(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

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
		Error checkCompilationErrors(GLuint object, GLboolean isProgram);

		void setupLogging(const char* vertexShaderSource);

	private:
		GLuint m_ShaderID;

		std::shared_ptr<spdlog::logger> m_ShaderLogger;
	};
}