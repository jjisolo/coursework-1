// This file implements the `Sprite` class.
#include "Sprite.hpp"
#include "Logger.hpp"
#include "ResourseManager.hpp"

namespace Engine::GFX
{
	void Sprite::render(std::shared_ptr<Engine::GFX::SpriteRenderer>& spriteRenderer) const noexcept
	{
		// Use the sprite shader that is compiled only for rendering sprites.
		(*Engine::ResourceManager::getShader("spriteShader")).useShader();

		// And finally, render the sprite!
		spriteRenderer->renderSprite(m_BindedTextureName, m_SpritePosition, m_SpriteSize, m_SpriteRotation, m_SpriteColor);
	}
}