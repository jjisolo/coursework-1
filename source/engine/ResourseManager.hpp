// C++ `101` game for 
//
//

#pragma once

#include "unordered_map"
#include "expected"
#include "sstream"
#include "fstream"
#include "memory"

#include "utility/Error.hpp"
#include "rendering/ShaderWrapper.hpp"
#include "rendering/TextureWrapper.hpp"
#include "Window.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Engine
{
	static class ResourceManager
	{
		using ShaderOrError  = std::expected< GFX::Core::ShaderWrapper,  Engine::Error>;
		using TextureOrError = std::expected< GFX::Core::TextureWrapper, Engine::Error>;

	public:
		static ShaderOrError loadShader(const char* vertShaderFilename, const char* fragShaderFilename, const char* geomShaderFilename, const std::string& name) noexcept;

		static ShaderOrError getShader(const std::string& shaderName) noexcept;

		static TextureOrError loadTexture(const char* textureFileName, GLboolean alphaChannel, const std::string& name) noexcept;

		static TextureOrError getTexture(const std::string& name) noexcept;

		static void release() noexcept;

	private:
		static ShaderOrError  loadShaderFromFile(const char* vertShaderFilename, const char* fragShaderFilename, const char* geomShaderFilename = nullptr) noexcept;

		static TextureOrError loadTextureFromFile(const char* textureFilename, bool alpaChannel) noexcept;
		
	private:
		static std::unordered_map<std::string, GFX::Core::ShaderWrapper>  m_Shaders;
		static std::unordered_map<std::string, GFX::Core::TextureWrapper> m_Textures;
	};
}