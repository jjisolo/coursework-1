#include "Sprite.hpp"
#include "Logger.hpp"
#include "ResourseManager.hpp"

namespace Engine::GFX
{
	void Sprite::render(std::shared_ptr<Engine::GFX::SpriteRenderer>& spriteRenderer) const noexcept
	{
		(*Engine::Core::ResourceManager::getShader("spriteShader")).useShader();
		spriteRenderer->renderSprite(m_BindedTextureName, m_SpritePosition, m_SpriteSize, m_SpriteRotation, m_SpriteColor);
	}
}