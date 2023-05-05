// This file implements the `Sprite` class.
#include "Sprite.hpp"
#include "Logger.hpp"
#include "ResourseManager.hpp"

namespace Engine::GFX
{
	void Sprite::render(std::shared_ptr<Engine::GFX::SpriteRenderer>& spriteRenderer) const noexcept
	{
		// Use the sprite shader that is compiled only for rendering sprites.
		auto shaderOrError = Engine::ResourceManager::getShader("spriteShader");

		if (shaderOrError.has_value())
		{
			shaderOrError.value().useShader();
		}
		else
		{
			// Well, actually the shader cannot be destroyed, but defensive coding thing, whatever
			Engine::Logger::m_GraphicsLogger->error("Attempted to render the sprite without its shader");

			return;
		}

		// And finally, render the sprite!
		spriteRenderer->renderSprite(m_BindedTextureName, m_SpritePosition, m_SpriteSize, m_SpriteRotation, m_SpriteColor);
	}
}