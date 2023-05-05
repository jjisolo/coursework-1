#include "Program.hpp"

#include "../engine/Sprite.hpp"

namespace Game
{
	Engine::GFX::Sprite cardSprite;
	Engine::Error GameProgram::onUserInitialize()
	{
		Engine::ResourceManager::loadTexture("data/cards.png", true, "cardSprite");

		cardSprite.setSpriteColor({ 1.0f, 1.0f, 1.0f });
		cardSprite.setSpritePosition({ 200.0f, 200.0f });
		cardSprite.setSpriteSize({ 600.0f, 400.0f });
		cardSprite.setSpriteRotation(15.0f);
		cardSprite.bindTexture("cardSprite");

		return(Engine::Error::Ok);
	}

	Engine::Error GameProgram::onUserRelease()
	{
		
		return(Engine::Error::Ok);
	}

	Engine::Error GameProgram::onUserUpdate(GLfloat elapsedTime)
	{
		ClearScreen(0.1f, 0.1f, 0.1f);

		cardSprite.render(m_SpriteRenderer);

		ImguiCreateNewFrameKHR();
		ImGui::NewFrame();
		ImGui::Begin("A custom window");
		ImGui::Text("Test");
		ImGui::End();
		ImGui::EndFrame();
		ImGui::Render();

		return(Engine::Error::Ok);
	}
}