#include "Program.hpp"

#include "../engine/Sprite.hpp"

namespace Game
{
	Engine::Error GameProgram::onUserInitialize()
	{
		m_GameState = GameState::Main_Menu;
	
		auto windowDimensions  = getWindowDimensions();
		
		Engine::ResourceManager::loadTexture("data/assets/background.jpg", false, "background");

		Engine::GFX::Sprite backgroundSprite;
		backgroundSprite.setSpriteSize    ({ windowDimensions.x, windowDimensions.y });
		backgroundSprite.bindTexture      ("background");

		m_mainMenuSprites.push_back(backgroundSprite);

		return(Engine::Error::Ok);
	}

	Engine::Error GameProgram::onUserRelease()
	{
		
		return(Engine::Error::Ok);
	}

	Engine::Error GameProgram::onUserUpdate(GLfloat elapsedTime)
	{
		ClearScreen(0.1f, 0.1f, 0.1f);

		if (m_GameState == GameState::Main_Menu)
		{
			for (auto& sprite : m_mainMenuSprites)
				sprite.render(m_SpriteRenderer);

			ImguiCreateNewFrameKHR();
			ImGui::NewFrame();
			ImGui::Begin("A custom window");
			ImGui::Text("Test");
			ImGui::End();
			ImGui::EndFrame();
			ImGui::Render();
		}
		
		return(Engine::Error::Ok);
	}
}