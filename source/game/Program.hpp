#pragma once

#include "../engine/Application.hpp"
#include "../engine/Sprite.hpp"

#include "GameInfo.hpp"
#include "GameBoard.hpp"

namespace Game
{
	class GameProgram: public One::Application
	{
	public:
		GameProgram() {};
		~GameProgram() {};
		
	public:
		virtual Engine::Error onUserInitialize() override;

		virtual Engine::Error onUserRelease() override;

		virtual Engine::Error onUserUpdate(GLfloat elapsedTime) override;


	private:
	    void updateGameBoardSprites(glm::ivec2& windowDimensions);

	    void renderGameBoardUI(glm::ivec2& windowDimensions);
	    
	    void renderMainMenuUI(glm::ivec2& windowDimensions);

	private:
	    Game::Board m_gameBoard;
	    Game::Info  m_gameInfo;

		bool m_showSettingsWindow    = false;
		bool m_showQuitApproveWindow = false;
		bool m_showDebugWindow       = false;

		bool m_openCardsMode = false;

		std::vector<Engine::GFX::Sprite> m_mainMenuSprites;
	    std::vector<Engine::GFX::Sprite> m_gameBoardSprites;
	};
}
