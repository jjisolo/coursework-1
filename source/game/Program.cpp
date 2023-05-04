#include "Program.hpp"

#include "../engine/Sprite.hpp"

namespace Game
{
	Engine::Error GameProgram::onUserInitialize()
	{

		return(Engine::Error::Ok);
	}

	Engine::Error GameProgram::onUserRelease()
	{
		
		return(Engine::Error::Ok);
	}

	Engine::Error GameProgram::onUserUpdate(GLfloat elapsedTime)
	{
		Engine::ResourceManager::loadTexture("data/cards.png", true, "cardSprite");

		Engine::GFX::Sprite cardSprite;
		cardSprite.setSpriteColor({ 1.0f, 1.0f, 1.0f });
		cardSprite.setSpritePosition({ 200.0f, 200.0f });
		cardSprite.setSpriteSize({ 400.0f, 300.0f });
		cardSprite.setSpriteRotation(15.0f);
		cardSprite.bindTexture("cardSprite");

		cardSprite.render(m_SpriteRenderer);

		return(Engine::Error::Ok);
	}
}