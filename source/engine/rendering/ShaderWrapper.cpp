#include "ShaderWrapper.hpp"
#include "../Logger.hpp"

namespace Engine::GFX::Core
{
	Error ShaderWrapper::compileShader(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
	{
		GLuint geomertyShader;

		Engine::Logger::m_GraphicsLogger->info("Compiling vertex shader...");
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource (vertexShader, 1, &vertexSource, nullptr);
		glCompileShader(vertexShader);
		if (checkCompilationErrors(vertexShader, false) != Error::Ok) {
			Engine::Logger::m_GraphicsLogger->error("Compilation stopped because of an error");
			return(Error::ValidationError);
		}

		Engine::Logger::m_GraphicsLogger->info("Compiling fragment shader");
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
		glCompileShader(fragmentShader);
		if (checkCompilationErrors(fragmentShader, false) != Error::Ok) {
			glDeleteShader(vertexShader);
			Engine::Logger::m_GraphicsLogger->error("Compilation stopped because of an error");
			return(Error::ValidationError);
		}

		if (geometrySource != nullptr) {
			Engine::Logger::m_GraphicsLogger->info("Compiling geometry shader");
			geomertyShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource (geomertyShader, 1, &geometrySource, nullptr);
			glCompileShader(geomertyShader);
			if (checkCompilationErrors(geomertyShader, false) != Error::Ok) {
				glDeleteShader(vertexShader);
				glDeleteShader(fragmentShader);
				Engine::Logger::m_GraphicsLogger->warn("Compilation stopped because of an error");
				return(Error::ValidationError);
			}
		}

		Engine::Logger::m_GraphicsLogger->info("Linking shaders");
		m_ShaderID = glCreateProgram();
		glAttachShader(m_ShaderID, vertexShader);
		glAttachShader(m_ShaderID, fragmentShader);
		if (geometrySource != nullptr)
			glAttachShader(m_ShaderID, geomertyShader);
		
		glLinkProgram(m_ShaderID);
		if (checkCompilationErrors(m_ShaderID, true) != Error::Ok) {
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			if (geometrySource != nullptr) 
				glDeleteShader(geomertyShader);

			Engine::Logger::m_GraphicsLogger->warn("Compilation stopped because of an error");
			return(Error::ValidationError);
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if(geometrySource != nullptr)
			glDeleteShader(geomertyShader);

		return(Error::Ok);
	}

	void ShaderWrapper::setFloat(const char* name, GLfloat value, GLboolean useShader)
	{
		if (useShader) this->useShader();
		glUniform1f(glGetUniformLocation(m_ShaderID, name), value);
	}

	void ShaderWrapper::setInteger(const char* name, int value, GLboolean useShader)
	{
		if (useShader) this->useShader();
		glUniform1i(glGetUniformLocation(m_ShaderID, name), value);
	}

	void ShaderWrapper::setVector2f(const char* name, GLfloat x, GLfloat y, GLboolean useShader)
	{
		if (useShader) this->useShader();
		glUniform2f(glGetUniformLocation(m_ShaderID, name), x, y);
	}

	void ShaderWrapper::setVector2f(const char* name, const glm::vec2& value, GLboolean useShader)
	{
		if (useShader) this->useShader();
		glUniform2f(glGetUniformLocation(m_ShaderID, name), value.x, value.y);
	}

	void ShaderWrapper::setVector3f(const char* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
	{
		if (useShader) this->useShader();
		glUniform3f(glGetUniformLocation(m_ShaderID, name), x, y, z);
	}

	void ShaderWrapper::setVector3f(const char* name, const glm::vec3& value, GLboolean useShader)
	{
		if (useShader) this->useShader();
		glUniform3f(glGetUniformLocation(m_ShaderID, name), value.x, value.y, value.z);
	}

	void ShaderWrapper::setVector4f(const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
	{
		if (useShader) this->useShader();
		glUniform4f(glGetUniformLocation(m_ShaderID, name), x, y, z, w);
	}

	void ShaderWrapper::setVector4f(const char* name, const glm::vec4& value, GLboolean useShader)
	{
		if (useShader) this->useShader();
		glUniform4f(glGetUniformLocation(m_ShaderID, name), value.x, value.y, value.z, value.w);
	}

	void ShaderWrapper::setMatrix4(const char* name, const glm::mat4& matrix, GLboolean useShader)
	{
		if (useShader) this->useShader();
		glUniformMatrix4fv(glGetUniformLocation(m_ShaderID, name), 1, false, glm::value_ptr(matrix));
	}

	Error ShaderWrapper::checkCompilationErrors(GLuint object, GLboolean isProgram)
	{
		GLint  success;
		GLchar infoBuffer[1024];

		if (!isProgram) {
			glGetShaderiv(object, GL_COMPILE_STATUS, &success);
			
			if (!success) {
				glGetShaderInfoLog(object, 1024, nullptr, infoBuffer);
				Engine::Logger::m_GraphicsLogger->error("Compile-Time error(program): {}", infoBuffer);
				return(Error::ValidationError);
			}
		}
		else {
			glGetProgramiv(object, GL_LINK_STATUS, &success);
			
			if (!success) {
				glGetProgramInfoLog(object, 1024, NULL, infoBuffer);
				Engine::Logger::m_GraphicsLogger->error("Compile-Time error(shader): {}", infoBuffer);
				return(Error::ValidationError);
			}
		}

		return(Error::Ok);
	}
}