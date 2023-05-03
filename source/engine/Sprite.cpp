#include "Sprite.hpp"
#include "Logger.hpp"
#include "ResourseManager.hpp"

namespace Engine::GFX
{
	void Sprite::render(std::shared_ptr<Engine::GFX::SpriteRenderer>& spriteRenderer) const noexcept
	{
		(*Engine::Core::ResourceManager::getShader("spriteShader")).useShader();

		auto textureOrError = Engine::Core::ResourceManager::getTexture(m_BindedTextureName);
		if (!textureOrError.has_value()) {
			Engine::Logger::m_GraphicsLogger->info("Unable to render texture {}", m_BindedTextureName);
			return;
		}

		spriteRenderer->renderSprite(*textureOrError, m_SpritePosition, m_SpriteSize, m_SpriteRotation, m_SpriteColor);
	}
}