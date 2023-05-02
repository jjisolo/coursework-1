#pragma once

#include "unordered_map"
#include "expected"
#include "memory"

#include "utility/Error.hpp"
#include "rendering/ShaderWrapper.hpp"
#include "rendering/TextureWrapper.hpp"
#include "Window.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Engine::Core
{
	class ResourceManager
	{
	public:
		static GFX::Core::ShaderWrapper loadShader(const char* vertShaderFilename, const char* fragShaderFilename, const char* geomShaderFilename, const std::string& name) noexcept;

		static GFX::Core::ShaderWrapper getShader(const std::string& shaderName) noexcept;

		static GFX::Core::TextureWrapper loadTexture(const char* textureFileName, GLboolean alphaChannel, const std::string& name);

		static GFX::Core::TextureWrapper getTexture(const std::string& name);

		static void release();

	private:
		static GFX::Core::ShaderWrapper  loadShaderFromFile(const char* vertShaderFilename, const char* fragShaderFilename, const char* geomShaderFilename = nullptr);

		static GFX::Core::TextureWrapper loadTextureFromFile(const char* textureFilename, bool alpaChannel);

		static void setupLogging(void) noexcept;
		
	private:
		static std::unordered_map<std::string, GFX::Core::ShaderWrapper>  m_Shaders;
		static std::unordered_map<std::string, GFX::Core::TextureWrapper> m_Textures;

		static mutable std::shared_ptr<spdlog::logger> m_ResourceLogger;
	};
}