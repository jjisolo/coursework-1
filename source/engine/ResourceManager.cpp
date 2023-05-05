// This file implements the `ResourceManager` class
#pragma once

#include "ResourseManager.hpp"
#include "Logger.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image.h"

// Initialize the static std::unordered_map's that was declared in the `ResourceManager` class.
std::unordered_map<std::string, Engine::GFX::Core::ShaderWrapper>  Engine::ResourceManager::m_Shaders;
std::unordered_map<std::string, Engine::GFX::Core::TextureWrapper> Engine::ResourceManager::m_Textures;

namespace Engine
{
	ResourceManager::ShaderOrError ResourceManager::loadShader(const char* vertShaderFilename, const char* fragShaderFilename, const char* geomShaderFilename, const std::string& name) noexcept
	{
		// If the shader in-class container already holds some data by the provided 
		// descriptor, safely destroy this shader and place the new one in place of
		// a previous.
		if (m_Shaders.contains(name))
		{
			Logger::m_ResourceLogger->warn("Reassigning shader {}", name);
		
			glDeleteProgram(m_Shaders[name].getShaderID());
		}

		Logger::m_ResourceLogger->info("Loading shader {}", name);

		// Try to load the shader from the file, retrieve std::expected container that contains either the ::ShaderWrapper or the ::Error.
		auto shaderLoadingResultOrError = loadShaderFromFile(vertShaderFilename, fragShaderFilename, geomShaderFilename);
		
		// If we succesfully loaded the shader assign it to the map, if not raise an ::InitializationError.
		if (shaderLoadingResultOrError.has_value())
		{
			m_Shaders[name] = *shaderLoadingResultOrError;
		}
		else
		{
			Logger::m_ResourceLogger->warn("Shader {} is not assigned due to an error", name);
		
			return std::unexpected(Engine::Error::InitializationError);
		}

		return(m_Shaders[name]);
	}

	ResourceManager::ShaderOrError ResourceManager::getShader(const std::string& name) noexcept
	{
		// If the shader in-class container does not holds the descriptor associated with some loaded shader,
		// raise an ::InitializationError.
		if (!m_Shaders.contains(name))
		{
			Logger::m_ResourceLogger->warn("Attempted to get the shader that does not exists in the map({})", name);
		
			return(std::unexpected(Engine::Error::InitializationError));
		}

		// If not just return the shader associated with passed descriptor.
		return(m_Shaders[name]);
	}

	ResourceManager::TextureOrError ResourceManager::loadTexture(const char* textureFileName, GLboolean alphaChannel, const std::string& name) noexcept
	{
		// If the texture in-class container already holds some data by the provided 
		// descriptor, safely destroy this texture and place the new one in place of
		// a previous.
		if (m_Textures.contains(name))
		{
			Logger::m_ResourceLogger->warn("Reassigning texture {}", name);

			const auto textureID = m_Textures[name].getTextureID();
			glDeleteTextures(1, &textureID);
		}

		Logger::m_ResourceLogger->info("Loading texture {}", name);

		// Try to load the shader from the file, retrieve std::expected container that contains either the ::TextureWrapper or
		// the ::Error.
		auto textureLoadingResultOrError = loadTextureFromFile(textureFileName, alphaChannel);
		
		// If we succesfully loaded the texture assign it to the map, if not raise an ::InitializationError.
		if (textureLoadingResultOrError.has_value())
		{
			m_Textures[name] = *textureLoadingResultOrError;
		}
		else 
		{
			Logger::m_ResourceLogger->warn("Texture {} is not loaded due to an error", name);
			return(std::unexpected(Engine::Error::InitializationError));
		}

		return(m_Textures[name]);
	}
	
	ResourceManager::TextureOrError ResourceManager::getTexture(const std::string& name) noexcept
	{
		// If the shader in-class container does not holds the descriptor associated with some loaded texture,
		// raise an ::InitializationError.
		if (!m_Textures.contains(name))
		{
			Logger::m_ResourceLogger->warn("Attempted to get the texture that does not exists in the map({})", name);
		
			return(std::unexpected(Engine::Error::InitializationError));
		}

		// If not just return the texture associated with passed descriptor.
		return(m_Textures[name]);
	}

	void ResourceManager::release() noexcept
	{
		Logger::m_ResourceLogger->info("Releasing shaders");

		// For each shader delete its program.
		for (auto& shader : m_Shaders)
		{
			const GLuint shaderProgramID = shader.second.getShaderID();

			glDeleteProgram(shaderProgramID);
		}

		Logger::m_ResourceLogger->info("Releasing textures");

		// For each texture delete it is.
		for (auto& texture : m_Textures)
		{
			const GLuint textureID = texture.second.getTextureID();
		
			glDeleteTextures(1, &textureID);
		}
	}

	ResourceManager::ShaderOrError ResourceManager::loadShaderFromFile(const char* vertShaderFilename, const char* fragShaderFilename, const char* geomShaderFilename) noexcept
	{
		// This variables containing the code of the shaders source files.
		std::string vertexSourceCode;
		std::string fragmentSourceCode;
		std::string geometrySourceCode;

		try {
			// The file handle associated with shaders.
			std::ifstream vertexShaderFile(vertShaderFilename);
			std::ifstream fragmentShaderFile(fragShaderFilename);

			std::stringstream vertexShaderStream;
			std::stringstream fragmentShaderStream;

			// Read the code for the vertex and fragment shaders.
			vertexShaderStream   << vertexShaderFile.rdbuf();
			fragmentShaderStream << fragmentShaderFile.rdbuf();

			// Convert the stream to the the string to pass it to the compilation functions.
			vertexSourceCode   = vertexShaderStream.str();
			fragmentSourceCode = fragmentShaderStream.str();

			// Close shader file handles.
			fragmentShaderFile.close();
			fragmentShaderFile.close();

			// Do the same for the geometry shader(if it is provided).
			if (geomShaderFilename != nullptr) {
				std::ifstream     geometryShaderFile(fragShaderFilename);
				std::stringstream geometryShaderStream;

				geometryShaderStream << geometryShaderFile.rdbuf();
				geometrySourceCode = geometryShaderStream.str();

				geometryShaderFile.close();
			}
		}
		catch (std::exception& exp)
		{
			Logger::m_ResourceLogger->error("Unable to read shader files");
		
			return std::unexpected(Engine::Error::InitializationError);
		}

		// Convert it to the `const char *` for the compilation function.
		const char* vertexSource   = vertexSourceCode.c_str();
		const char* fragmentSource = fragmentSourceCode.c_str();
		const char* geometrySource = geometrySourceCode.c_str();

		GFX::Core::ShaderWrapper shaderWrapper;

		Logger::m_ResourceLogger->info("Compiling shader({}, {}, {})", vertShaderFilename, fragShaderFilename, geomShaderFilename);

		// Compile, link the shaders and store the shader program in the ::ShaderWrapper class so it is 
		// accessed with the std::string descriptor.
		if (shaderWrapper.compileShader(vertexSource, fragmentSource, geomShaderFilename != nullptr ? geometrySource : nullptr) != Engine::Error::Ok)
		{
			Logger::m_ResourceLogger->error("Unable to compile shader({}, {}, {})", vertShaderFilename, fragShaderFilename, geomShaderFilename);
		
			return std::unexpected(Engine::Error::InitializationError);
		}
		
		return(shaderWrapper);
	}

	ResourceManager::TextureOrError ResourceManager::loadTextureFromFile(const char* textureFilename, bool alpaChannel) noexcept
	{
		GFX::Core::TextureWrapper textureWrapper;
		
		// Enable the transparency effect for the texture.
		if (alpaChannel) 
		{
			textureWrapper.setTexFormat(GL_RGBA);
			textureWrapper.setImgFormat(GL_RGBA);
		}

		// Load the texture using the stbi_image library created by Sean Barrett.
		GLint    imageWidth, imageHeight, imageChannels;
		GLubyte* imageData = stbi_load(textureFilename, &imageWidth, &imageHeight, &imageChannels, 0);

		// Assert that the image is loaded correctly and there is no errors.
		if (imageData == nullptr)
		{
			Logger::m_ResourceLogger->error("Unable to load texture {}", textureFilename);
		
			return(std::unexpected(Engine::Error::InitializationError));
		}

		// Create the texture wrapper from the data retrieved from the image.
		textureWrapper.make(imageWidth, imageHeight, imageData);

		// Free the image data as it is already binded to the ::TextureWrapper
		stbi_image_free(imageData);

		// Return the created image.
		return(textureWrapper);
	}
}
