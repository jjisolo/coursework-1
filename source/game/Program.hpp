#pragma once

#include "../engine/Application.hpp"
#include "../engine/Sprite.hpp"

#include "GameInfo.hpp"
#include "GameBoard.hpp"

using namespace std;

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
	  
     	virtual Engine::Error onMouseMove(double positionX, double positionY) override;

	    virtual Engine::Error onMousePress(int button, int action);

	private:
	    void updateGameBoardSprites(glm::ivec2& windowDimensions);

	    void renderGameBoardUI(glm::ivec2& windowDimensions);
	    
	    void renderMainMenuUI(glm::ivec2& windowDimensions);

	private:
	    Game::Board m_gameBoard;
	    Game::Info  m_gameInfo;

	    glm::vec2   m_cursorPosition;
        	   
		bool m_showSettingsWindow    = false;
		bool m_showQuitApproveWindow = false;
		bool m_showDebugWindow       = false;

		bool m_openCardsMode = false;

		vector<Engine::GFX::Sprite> m_mainMenuSprites;
	    vector<Engine::GFX::Sprite> m_gameBoardSprites;
	};
}
