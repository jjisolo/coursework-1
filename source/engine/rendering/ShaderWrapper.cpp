// This class implements the `ShaderWrapper` class.
#include "ShaderWrapper.hpp"
#include "../Logger.hpp"

namespace Engine::GFX::Core
{
	Error ShaderWrapper::compileShader(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
	{
		GLuint geomertyShader;

		Engine::Logger::m_GraphicsLogger->info("Compiling vertex shader");

		// Create the vertex shader, from vertex shader source code.
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		
		// Bind the shader source code to this shader descriptior, and the compile it.
		glShaderSource (vertexShader, 1, &vertexSource, nullptr);
		glCompileShader(vertexShader);

		if (checkCompilationErrors(vertexShader, false) != Error::Ok)
		{
			Engine::Logger::m_GraphicsLogger->error("Compilation stopped because of an error");
				
			return(Error::ValidationError);
		}

		Engine::Logger::m_GraphicsLogger->info("Compiling fragment shader");

		// Create the fragment shader from the fragment shader source code.
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		
		// Bind the shader source code to this shader descriptior, and the compile it.
		glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
		glCompileShader(fragmentShader);
		
		if (checkCompilationErrors(fragmentShader, false) != Error::Ok)
		{
			// Delete the created shader to avoid memory leaks.
			glDeleteShader(vertexShader);
			
			Engine::Logger::m_GraphicsLogger->error("Compilation stopped because of an error");
		
			return(Error::ValidationError);
		}

		if (geometrySource != nullptr)
		{
			Engine::Logger::m_GraphicsLogger->info("Compiling geometry shader");
			
			// Create the geometry shader.
			geomertyShader = glCreateShader(GL_GEOMETRY_SHADER);
			
			// Bind the shader source code to this shader descriptior, and the compile it.
			glShaderSource (geomertyShader, 1, &geometrySource, nullptr);
			glCompileShader(geomertyShader);
			
			if (checkCompilationErrors(geomertyShader, false) != Error::Ok)
			{
				Engine::Logger::m_GraphicsLogger->warn("Compilation stopped because of an error");

				// Delete the created shaders to avoid memory leaks.
				glDeleteShader(vertexShader);
				glDeleteShader(fragmentShader);
			
				return(Error::ValidationError);
			}
		}

		Engine::Logger::m_GraphicsLogger->info("Linking shaders");

		// Attach the shaders to the shader program
		m_ShaderID = glCreateProgram();
		glAttachShader(m_ShaderID, vertexShader);
		glAttachShader(m_ShaderID, fragmentShader);

		// Link the geometry shader also if the code for it was provided.
		if (geometrySource != nullptr) 
		{
			glAttachShader(m_ShaderID, geomertyShader);
		}	
		
		// Link the program.
		glLinkProgram(m_ShaderID);

		// Delete the shaders because we don't need them more.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// Also delete the geometry shader if it is existing.
		if (geometrySource != nullptr)
		{
			glDeleteShader(geomertyShader);
		}

		if (checkCompilationErrors(m_ShaderID, true) != Error::Ok) 
		{
			Engine::Logger::m_GraphicsLogger->warn("Compilation stopped because of an error");

			return(Error::ValidationError);
		}

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

		// If we're checking for the errors at the shader compilation stage.
		if (!isProgram) 
		{
			// Get compilation status of the program.
			glGetShaderiv(object, GL_COMPILE_STATUS, &success);
			
			if (!success) 
			{
				// Allocate the space for the shader error message.
				GLchar infoBuffer[2048];

				// Get the shader compilation error message.
				glGetShaderInfoLog(object, 2048, nullptr, infoBuffer);
				
				// And pipe it into the graphics+global logging sink.
				Engine::Logger::m_GraphicsLogger->error("Compile-Time error(program): {}", infoBuffer);
			
				return(Error::ValidationError);
			}
		}
		// Or if we're checking for the errors at the program link stage.
		else 
		{
			// Get the program linking status.
			glGetProgramiv(object, GL_LINK_STATUS, &success);
			
			if (!success)
			{
				// Allocate the space for the program link error message.
				GLchar infoBuffer[1024];

				// Get program link error message;
				glGetProgramInfoLog(object, 1024, NULL, infoBuffer);

				// Pipe it into the graphics+global logging sink.
				Engine::Logger::m_GraphicsLogger->error("Compile-Time error(shader): {}", infoBuffer);

				return(Error::ValidationError);
			}
		}

		return(Error::Ok);
	}
}
