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

	ResourceManager::ShaderOrError ResourceManager::loadShader(const char* vertShaderFilename, const char* fragShaderFilename, const char* geomShaderFilename, const std::string& name) noexcept
	{
		if (m_Shaders.contains(name)) {
			m_ResourceLogger->warn("Reassigning shader %s", name);
			glDeleteProgram(m_Shaders[name].getShaderID());
		}

		m_ResourceLogger->info("Loading shader %s", name);
		auto shaderLoadingResultOrError = loadShaderFromFile(vertShaderFilename, fragShaderFilename, geomShaderFilename);
		if (shaderLoadingResultOrError.has_value()) {
			m_Shaders[name] = *shaderLoadingResultOrError;
		}
		else {
			m_ResourceLogger->warn("Shader %s is not assigned due to an error", name);
			return std::unexpected(Engine::Error::InitializationError);
		}

		return(m_Shaders[name]);
	}

	ResourceManager::ShaderOrError ResourceManager::getShader(const std::string& name) noexcept
	{
		if (!m_Shaders.contains(name)) {
			m_ResourceLogger->warn("Attempted to get the shader that does not exists in the map(%s)", name);
			return(std::unexpected(Engine::Error::InitializationError));
		}

		return(m_Shaders[name]);
	}

	ResourceManager::TextureOrError ResourceManager::loadTexture(const char* textureFileName, GLboolean alphaChannel, const std::string& name) noexcept
	{
		if (m_Textures.contains(name)) {
			m_ResourceLogger->warn("Reassigning texture %s", name);
			glDeleteProgram(m_Textures[name].getTextureID());
		}

		m_ResourceLogger->info("Loading texture %s", name);
		auto textureLoadingResultOrError = loadTextureFromFile(textureFileName, alphaChannel);
		if (textureLoadingResultOrError.has_value()) {
			m_Textures[name] = *textureLoadingResultOrError;
		}
		else {
			m_ResourceLogger->warn("Texture %s is not loaded due to an error", name);
			return(std::unexpected(Engine::Error::InitializationError));
		}

		return(m_Textures[name]);
	}
	
	ResourceManager::TextureOrError ResourceManager::getTexture(const std::string& name) noexcept
	{
		if (!m_Textures.contains(name)) {
			m_ResourceLogger->warn("Attempted to get the texture that does not exists in the map(%s)", name);
			return(std::unexpected(Engine::Error::InitializationError));
		}

		return(m_Textures[name]);
	}

	void ResourceManager::release() noexcept
	{
		m_ResourceLogger->info("Releasing shaders..");
		for (auto& shader : m_Shaders) {
			const GLuint shaderProgramID = shader.second.getShaderID();
			glDeleteProgram(shaderProgramID);
		}

		m_ResourceLogger->info("Releasing textures..");
		for (auto& texture : m_Textures) {
			const GLuint textureID = texture.second.getTextureID();
			glDeleteTextures(1, &textureID);
		}
	}

	ResourceManager::ShaderOrError ResourceManager::loadShaderFromFile(const char* vertShaderFilename, const char* fragShaderFilename, const char* geomShaderFilename) noexcept
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
			return std::unexpected(Engine::Error::InitializationError);
		}

		const char* vertexSource   = vertexSourceCode.c_str();
		const char* fragmentSource = fragmentSourceCode.c_str();
		const char* geometrySource = geometrySourceCode.c_str();

		GFX::Core::ShaderWrapper shaderWrapper;

		m_ResourceLogger->info("Compiling shader(%s, %s, %s)", vertShaderFilename, fragShaderFilename, geomShaderFilename);
		if (shaderWrapper.compileShader(vertexSource, fragmentSource, geometrySource) != Engine::Error::Ok) {
			m_ResourceLogger->error("Unable to compile shader(%s, %s, %s)", vertShaderFilename, fragShaderFilename, geomShaderFilename);
			return std::unexpected(Engine::Error::InitializationError);
		}
		
		return(shaderWrapper);
	}

	ResourceManager::TextureOrError ResourceManager::loadTextureFromFile(const char* textureFilename, bool alpaChannel) noexcept
	{
		GFX::Core::TextureWrapper textureWrapper;
		if (alpaChannel) {
			textureWrapper.setTexFormat(GL_RGBA);
			textureWrapper.setImgFormat(GL_RGBA);
		}

		GLint    imageWidth, imageHeight, imageChannels;
		GLubyte* imageData = stbi_load(textureFilename, &imageWidth, &imageHeight, &imageChannels, 0);
		if (imageData == nullptr) {
			m_ResourceLogger->error("Unable to load texture %s", textureFilename);
			return std::unexpected(Engine::Error::InitializationError);
		}
		textureWrapper.make(imageWidth, imageHeight, imageData);

		stbi_image_free(imageData);
		return(textureWrapper);
	}
}