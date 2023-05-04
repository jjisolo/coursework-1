// This file defines the `SpriteRenderer` class.
#pragma once

#include "../_EngineIncludes.hpp"

#include "ShaderWrapper.hpp"
#include "TextureWrapper.hpp"

// This namespace is polluted with the Internals components of the graphics API.
namespace Engine::GFX
{
	// This class represents an object which is generating sprites to the screen, taking
	// the sprite data and the texture as an input.
	//
	// The purpose of this class is to simplify the rendering routine.
	class SpriteRenderer
	{
	public:
		SpriteRenderer(Core::ShaderWrapper& shaderWrapper);
		~SpriteRenderer();

	public:
		// Render the sprite on the screen.
		void renderSprite(const std::string& textureName, glm::vec2 spritePosition, glm::vec2 spriteSize = glm::vec2(10.0f, 10.0f), GLfloat spriteRotation = 0.0f, glm::vec3 spriteColor = glm::vec3(1.0f)) noexcept;

	private:
		// Initialize rendering-related data structures(VAO, VBO), setup vertex
		// attributes etc.
		void initializeRenderPipeline() noexcept;

	private:
		Core::ShaderWrapper m_ShaderWrapper;
		GLuint              m_QuadVertexArray;
	};
}