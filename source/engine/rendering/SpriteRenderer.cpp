#include "SpriteRenderer.hpp"

namespace Engine::GFX
{
    SpriteRenderer::SpriteRenderer(Core::ShaderWrapper& shaderWrapper)
	{
		m_ShaderWrapper = shaderWrapper;
		initializeRenderPipeline();
	}

	SpriteRenderer::~SpriteRenderer()
	{
		glDeleteVertexArrays(1, &m_QuadVertexArray);
	}

	void SpriteRenderer::initializeRenderPipeline() noexcept
	{
		GLuint  vertexBuffer{};
		GLfloat verticies[] = {
			// Position   // Texture
			0.0f, 1.0f,   0.0f, 1.0f,
			1.0f, 0.0f,   1.0f, 0.0f,
			0.0f, 0.0f,   0.0f, 0.0f,

			0.0f, 1.0f,   0.0f, 1.0f,
			1.0f, 1.0f,   1.0f, 1.0f,
			1.0f, 0.0f,   1.0f, 0.0f
		};

		glGenVertexArrays(1, &m_QuadVertexArray);
		glGenBuffers     (1, &vertexBuffer);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

		glBindVertexArray        (m_QuadVertexArray);
		glEnableVertexAttribArray(GL_ZERO);
		glVertexAttribPointer    (GL_ZERO, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)GL_ZERO);
		glBindBuffer             (GL_ARRAY_BUFFER, GL_ZERO);
		glBindVertexArray        (GL_ZERO);
	}

	void SpriteRenderer::renderSprite(Core::TextureWrapper& textureWrapper, glm::vec2 spritePosition, glm::vec2 spriteSize, GLfloat spriteRotation, glm::vec3 spriteColor) noexcept
	{
		m_ShaderWrapper.useShader();

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(spritePosition, 1.0f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f * spriteSize.x, 0.5f * spriteSize.y, 0.0f));
		modelMatrix = glm::rotate   (modelMatrix, glm::radians(spriteRotation), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f * spriteSize.x, -0.5f * spriteSize.y, 0.0f));
		modelMatrix = glm::scale    (modelMatrix, glm::vec3(spriteSize, 1.0f));

		m_ShaderWrapper.setMatrix4 ("modelMatrix", modelMatrix);
		m_ShaderWrapper.setVector3f("spriteColor", spriteColor);

		glActiveTexture(GL_TEXTURE0);
		textureWrapper.bind();

		glBindVertexArray(m_QuadVertexArray);
		glDrawArrays     (GL_TRIANGLES, GL_ZERO, 6);
		glBindVertexArray(GL_ZERO);
	}
}