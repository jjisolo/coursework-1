#include "Sprite.hpp"
#include "Logger.hpp"
#include "ResourseManager.hpp"

namespace Engine::GFX
{
	void Sprite::render(std::shared_ptr<Engine::GFX::SpriteRenderer>& spriteRenderer) const noexcept
	{
		auto spriteShaderIdOrError = Engine::Core::ResourceManager::getShader("spriteShader");
		if (spriteShaderIdOrError.has_value()) {
			(*spriteShaderIdOrError).useShader();
		}
		else {
			Logger::m_GraphicsLogger->error("Cannot render sprite {}, because ResourceManager returned error", m_SpriteTexturePath);
			return;
		}

		if (!Engine::Core::ResourceManager::getTexture(m_SpriteTextureName).has_value()) {
			Engine::Core::ResourceManager::loadTexture(m_SpriteTexturePath.c_str(), true, m_SpriteTextureName);
		}

		auto textureOrError = Engine::Core::ResourceManager::getTexture(m_SpriteTextureName);
		if (textureOrError.has_value()) {
			spriteRenderer->renderSprite(*textureOrError, m_SpritePosition, m_SpriteSize, m_SpriteRotation, m_SpriteColor);
		}
	}
}