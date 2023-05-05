#include "Program.hpp"

#include "../engine/Sprite.hpp"

static constexpr auto GAME_DESCRIPTION =
R"(The goal of the game is to score the least number of points. In order to play, you need a 
deck of 36 cards and from 2 to 4 players.The first card dealer in the game is determined by
lot, in  the following games, players deal cards in turn. The deck is carefully shuffled, 
removed and 5 cards are dealt to each player.

The deliverer deals himself 4 cards, and the fifth card is put on the line.The remaining
deck is placed in the center of the table in a closed form.The player to the left of the
deliverer continues the game. He must put a card of the same suit or a card of the same 
value on the fifth card of the deliverer, if there is no such card, then the player takes
one card from the deck and if this card does not fit, then the player skips the turn. Some
cards have their own features in the game, namely: aces, queens, king of spades, nines,
sevens and sixes.
)";

static constexpr auto GAME_SPECIFICATION =
R"(The game is baked using GLFW(platform layer) OpenGL library, GLAD OpenGL functions
loader library. GLM for the mathematical calculations(such as vector manipulations,
matricies calculus etc.), and the SPDLOG for logging into the files, and finally,
immediate-mode UI library Dear ImGui.
)";

static constexpr auto GAME_CREDITS =
R"(This game was create as a subject of my university coursework.

The source code is publicly available under the MIT license, and
hosted on https://github.com/jjisolo/coursework-1.
)";

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
		auto windowDimensions = getWindowDimensions();

		if (m_GameState == GameState::Main_Menu)
		{
			for (auto& sprite : m_mainMenuSprites)
				sprite.render(m_SpriteRenderer);

			ImguiCreateNewFrameKHR();
			ImGui::NewFrame();

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoResize;
			window_flags |= ImGuiWindowFlags_NoCollapse;

			ImGui::SetNextWindowPos(
				ImVec2(windowDimensions.x/3-100, windowDimensions.y/2-100),
				ImGuiCond_FirstUseEver);
			ImGui::Begin("Main Menu", NULL, window_flags);
			
			if (ImGui::Button("\t\t\tNew Game\t\t\t"))
				(void)0;
			
			ImGui::SameLine();

			if (ImGui::Button("\t\t\tContinue\t\t\t"))
				(void)0;

			ImGui::SameLine();

			if (ImGui::Button("\t\t\tSettings\t\t\t"))
				m_showSettingsWindow = !m_showSettingsWindow;

			ImGui::SameLine();

			if (ImGui::Button("\t\t\tQuit\t\t\t"))
				m_showQuitApproveWindow = !m_showQuitApproveWindow;
			
			ImGui::Separator();
			ImGui::Text(GAME_DESCRIPTION);
			ImGui::End();
			
			if (m_showQuitApproveWindow)
			{
				window_flags  = 0;
				window_flags |= ImGuiWindowFlags_NoResize;
				window_flags |= ImGuiWindowFlags_NoCollapse;

				ImGui::SetNextWindowPos(ImVec2(windowDimensions.x / 2.5, windowDimensions.y / 3.5), ImGuiCond_FirstUseEver);

				if (!ImGui::Begin("Do you really want to quit?", &m_showQuitApproveWindow, window_flags))
				{			
					ImGui::End();
				}
				else
				{
					if (ImGui::Button("\t\t\tYes\t\t\t"))
					{
						glfwSetWindowShouldClose(getWindowPointer(), true);
					}

					ImGui::SameLine();

					if (ImGui::Button("\t\tNo\t\t\t"))
					{
						m_showQuitApproveWindow = false;
					}

					ImGui::End();
				}
			}

			if (m_showSettingsWindow)
			{
				window_flags = 0;
				window_flags |= ImGuiWindowFlags_NoCollapse;

				ImGui::SetNextWindowPos(ImVec2(windowDimensions.x / 4, windowDimensions.y / 4), ImGuiCond_FirstUseEver);

				if (!ImGui::Begin("Settings", &m_showSettingsWindow, window_flags))
				{
					ImGui::End();
				}
				else
				{
					if (ImGui::CollapsingHeader("Game Rules"))
					{
						ImGui::Text(GAME_DESCRIPTION);
					}

					if (ImGui::CollapsingHeader("Game specification"))
					{
						ImGui::Text(GAME_SPECIFICATION);
					}

					if (ImGui::CollapsingHeader("ImGui User Manual"))
					{
						ImGui::ShowUserGuide();
					}

					if (ImGui::CollapsingHeader("Author Credits"))
					{
						ImGui::Text(GAME_CREDITS);
					}

					ImGui::Separator();

					if (ImGui::BeginMenu("Developer Tools"))
					{
						ImGui::MenuItem("Show Debug Window",     NULL, &m_showDebugWindow);
						ImGui::MenuItem("Show Enemy Card Faces", NULL, &m_openCardsMode);

						ImGui::EndMenu();
					}

					ImGui::End();
				}
			}

			ImGui::EndFrame();
			ImGui::Render();
		}
		
		return(Engine::Error::Ok);
	}
}