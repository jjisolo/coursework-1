// This file implements the `Sprite` class.
#include "Sprite.hpp"
#include "Logger.hpp"
#include "ResourseManager.hpp"

using namespace std;

namespace Engine::GFX
{
	void Sprite::render(shared_ptr<Engine::GFX::SpriteRenderer>& spriteRenderer) const noexcept
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

    // Bind the ::TextureWrapper descriptor to this sprite.
    void Sprite::bindTexture(const string& textureName) noexcept
	{
		m_BindedTextureName = textureName;
	}

    bool Sprite::isHovered(vec2 mousePosition, vec2 range) const
    {
      bool crossX = mousePosition.x >= m_SpritePosition.x - range.x && mousePosition.x <= m_SpritePosition.x + m_SpriteSize.x + range.x;
      bool crossY = mousePosition.y >= m_SpritePosition.y - range.x && mousePosition.y <= m_SpritePosition.y + m_SpriteSize.y + range.y;
      
      if(crossX && crossY)
        printf("Comparing mouse(%f, %f) with coords(%f, %f) and range(%f, %f)\n",
             mousePosition.x, mousePosition.y, m_SpritePosition.x, m_SpritePosition.y, range.x, range.y);
      return (crossX && crossY);
    }
}
