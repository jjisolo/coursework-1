#pragma once

#include "ResourseManager.hpp"

static constexpr const char* LOG_FILE_DEF_PATH = "logs/application_logs.txt";
static constexpr const char* LOG_LOGGER_NAME   = "ResourceManager";

std::unordered_map<std::string, Engine::GFX::Core::ShaderWrapper>  Engine::Core::ResourceManager::m_Shaders;
std::unordered_map<std::string, Engine::GFX::Core::TextureWrapper> Engine::Core::ResourceManager::m_Textures;

std::shared_ptr<spdlog::logger> Engine::Core::ResourceManager::m_ResourceLogger;

namespace Engine::Core
{
	void ResourceManager::setupLogging(void) noexcept
	{
		m_ResourceLogger = spdlog::basic_logger_mt(LOG_LOGGER_NAME, LOG_FILE_DEF_PATH, true);
		m_ResourceLogger->set_level(spdlog::level::err);
		m_ResourceLogger->debug("Logging subsystem engaged");
	}

	GFX::Core::ShaderWrapper ResourceManager::loadShader(const char* vertShaderFilename, const char* fragShaderFilename, const char* geomShaderFilename,
		const std::string& name) noexcept
	{
		m_Shaders[name] = loadShaderFromFile(vertShaderFilename, fragShaderFilename, geomShaderFilename);
		return(m_Shaders[name]);
	}

	GFX::Core::ShaderWrapper ResourceManager::getShader(const std::string& name) noexcept
	{
		return(m_Shaders[name]);
	}

	GFX::Core::TextureWrapper ResourceManager::loadTexture(const char* textureFileName, GLboolean alphaChannel, const std::string& name) noexcept
	{
		m_Textures[name] = loadTextureFromFile(textureFileName, alphaChannel);
		return(m_Textures[name]);
	}
	
	GFX::Core::TextureWrapper ResourceManager::getTexture(const std::string& name) noexcept
	{
		return(m_Textures[name]);
	}

	void ResourceManager::release() noexcept
	{
		for (auto& shader : m_Shaders) {
			const GLuint shaderProgramID = shader.second.getShaderID();
			glDeleteProgram(shaderProgramID);
		}

		for (auto& texture : m_Textures) {
			const GLuint textureID = texture.second.getTextureID();
			glDeleteTextures(1, &textureID);
		}
	}

	GFX::Core::ShaderWrapper ResourceManager::loadShaderFromFile(const char* vertShaderFilename, const char* fragShaderFilename, const char* geomShaderFilename) noexcept
	{
		std::string vertexSourceCode;
		std::string fragmentSourceCode;
		std::string geometrySourceCode;

		try {
			std::ifstream vertexShaderFile(vertShaderFilename);
			std::ifstream fragmentShaderFile(fragShaderFilename);

			std::stringstream vertexShaderStream;
			std::stringstream fragmentShaderStream;

			vertexShaderStream   << vertexShaderFile.rdbuf();
			fragmentShaderStream << fragmentShaderFile.rdbuf();

			vertexSourceCode = vertexShaderStream.str();
			fragmentSourceCode = fragmentShaderStream.str();

			fragmentShaderFile.close();
			fragmentShaderFile.close();

			if (geomShaderFilename != nullptr) {
				std::ifstream     geometryShaderFile(fragShaderFilename);
				std::stringstream geometryShaderStream;

				geometryShaderStream << geometryShaderFile.rdbuf();
				geometrySourceCode = geometryShaderStream.str();

				geometryShaderFile.close();
			}
		}
		catch (std::exception& exp) {
			m_ResourceLogger->error("Unable to read shader files");
		}

		const char* vertexSource   = vertexSourceCode.c_str();
		const char* fragmentSource = fragmentSourceCode.c_str();
		const char* geometrySource = geometrySourceCode.c_str();

		// TODO: std::expected<ShaderWrapper, Engine::error>
		GFX::Core::ShaderWrapper shaderWrapper;
		if (shaderWrapper.compileShader(vertexSource, fragmentSource, geometrySource) != Engine::Error::Ok) {
			m_ResourceLogger->error("Unable to compile shader(%s, %s, %s)", vertShaderFilename, fragShaderFilename, geomShaderFilename);
		}
		
		return(shaderWrapper);
	}

	GFX::Core::TextureWrapper ResourceManager::loadTextureFromFile(const char* textureFilename, bool alpaChannel) noexcept
	{
		// TODO: std::expected<TextureWrapper, Engine::error>
		GFX::Core::TextureWrapper textureWrapper;
		if (alpaChannel) {
			textureWrapper.setTexFormat(GL_RGBA);
			textureWrapper.setImgFormat(GL_RGBA);
		}

		GLint imageWidth, imageHeight, imageChannels;
		GLubyte* imageData = stbi_load(textureFilename, &imageWidth, &imageHeight, &imageChannels, 0);
		if (imageData == nullptr) {
			m_ResourceLogger->error("Unable to load texture %s", textureFilename);
			// return
		}

		textureWrapper.make(imageWidth, imageHeight, imageData);
		stbi_image_free(imageData);

		return(textureWrapper);
	}
}