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

	GFX::Core::ShaderWrapper ResourceManager::getShader(const std::string& name)
	{
		return(m_Shaders[name]);
	}

	GFX::Core::TextureWrapper ResourceManager::loadTexture(const char* textureFileName, GLboolean alphaChannel, const std::string& name)
	{
		m_Textures[name] = loadTextureFromFile(textureFileName, alphaChannel);
		return(m_Textures[name]);
	}
	
	GFX::Core::TextureWrapper ResourceManager::getTexture(const std::string& name)
	{
		return(m_Textures[name]);
	}

	void  ResourceManager::release()
	{
		for (auto& shader : m_Shaders)
			const GLuint shaderProgramID = shader.second.getShaderID();
			glDeleteProgram(shaderProgramID);
		for (auto& texture : m_Textures) {
			const GLuint textureID = texture.second.getTextureID();
			glDeleteTextures(1, &textureID);
		}
	}
}