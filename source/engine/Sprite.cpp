#include "Sprite.hpp"
#icnlude "Logger.hpp"
#include "ResourceManager.hpp"

namespace Engine::GFX
{
	void Sprite::render(void) const noexcept
	{
		auto spriteShaderIdOrError = ResourceManager::getShader("spriteShader");
		if (spriteShaderIdOrError.has_value()) {
			Logger::m_GraphicsLogger->info("Binding shader spriteShader");
			(*spriteShaderIdOrError).useShader();
		} 
		else {
			Logger::m_GraphicsLogger->error("Cannot render sprite {}, because ResourceManager returned error", m_SpriteTexturePath);
			return;
		}

		if (!ResourceManager::getTexture(m_SpriteTextureName).has_value()) {
			Engine::Core::ResourceManager::loadTexture(m_SpriteTexturePath, true, m_SpriteTextureName);
		}

		auto textureOrError = Engine::Core::ResourceManager::getTexture("cards");
		if (textureOrError.has_value()) {
			m_SpriteRenderer->renderSprite(*textureOrError, m_SpritePostiion, m_SpiteSize, m_SpriteRotation, m_SpriteColor);
		}
	}
}