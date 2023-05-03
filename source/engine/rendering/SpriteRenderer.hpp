#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "ShaderWrapper.hpp"
#include "TextureWrapper.hpp"

namespace Engine::GFX
{
	class SpriteRenderer
	{
	public:
		SpriteRenderer(Core::ShaderWrapper& shaderWrapper);
		~SpriteRenderer();

	public:
		void renderSprite(Core::TextureWrapper& textureWrapper, glm::vec2 spritePosition, glm::vec2 spriteSize = glm::vec2(10.0f, 10.0f), GLfloat spriteRotation = 0.0f, glm::vec3 spriteColor = glm::vec3(1.0f)) noexcept;

	private:
		void initializeRenderPipeline() noexcept;

	private:
		Core::ShaderWrapper m_ShaderWrapper;
		GLuint              m_QuadVertexArray;
	};
}