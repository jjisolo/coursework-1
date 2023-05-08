// This file declares the `ResourceManager` class
#pragma once

#include "_EngineIncludes.hpp"

#include "Window.hpp"

#include "rendering/ShaderWrapper.hpp"
#include "rendering/TextureWrapper.hpp"

using namespace std;

// This namespace is polluted with code for the game engine
namespace Engine
{
	// The resource manager is responsible for managing resources that are used in the game(shaders and
	// textures in that case). Basically this class is the safe wrapper around to unordered_maps that are
	// containing either shader or texture and its descriptor.
	class ResourceManager
	{
		// Typedef the std::expected containers for better readability.
		using ShaderOrError  = expected< GFX::Core::ShaderWrapper,  Engine::Error>;
		using TextureOrError = expected< GFX::Core::TextureWrapper, Engine::Error>;

	public:
		// Load the shader and get either an error or a shader packed into the ::ShaderWrapper class.
		static ShaderOrError loadShader(const char* vertShaderFilename, const char* fragShaderFilename, const char* geomShaderFilename, const string& name) noexcept;

		// Retrieve the loaded shader and get either an error or a shader packed into the ::ShaderWrapper class.
		static ShaderOrError getShader(const string& shaderName) noexcept;

		// Load the texture and get either an error or a shader packed into the ::TextureWrapper class.
		static TextureOrError loadTexture(const char* textureFileName, GLboolean alphaChannel, const string& name) noexcept;

		// Retrieve the loaded texture and get either an error or a shader packed into the ::TextureWrapper class.
		static TextureOrError getTexture(const string& name) noexcept;

		// Destroy all the loaded shaders and textures.
		static void release() noexcept;

	private:
		// Load the shaders from the file.
		static ShaderOrError  loadShaderFromFile(const char* vertShaderFilename, const char* fragShaderFilename, const char* geomShaderFilename = nullptr) noexcept;

		// Load the texture from the file.
		static TextureOrError loadTextureFromFile(const char* textureFilename, bool alpaChannel) noexcept;
		
	private:
		static std::unordered_map<string, GFX::Core::ShaderWrapper>  m_Shaders;
		static std::unordered_map<string, GFX::Core::TextureWrapper> m_Textures;
	};
}
