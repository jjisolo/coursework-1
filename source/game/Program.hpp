#pragma once

#include "../engine/Application.hpp"
#include "../engine/Sprite.hpp"

#include "GameInfo.hpp"
#include "GameBoard.hpp"

using namespace std;
using namespace glm;
using namespace Engine;
using namespace Engine::GFX;

namespace Game
{
	class GameProgram: public One::Application
	{
	public:
		GameProgram() {};
		~GameProgram() {};
		
	public:
		virtual Error onUserInitialize() override;

		virtual Error onUserRelease() override;

		virtual Error onUserUpdate(GLfloat elapsedTime) override;
	  
	private:
	    void updateGameBoardCardSprites(ivec2& windowDimensions);

	    void renderGameBoardUI(ivec2& windowDimensions);
	    
	    void renderMainMenuUI(ivec2& windowDimensions);

        void renderPlayerStatUI(CardOwner owner);

        bool spriteCollided(Sprite& sprite, vec2 point) const;
	private:
	    Game::Board m_gameBoard;
	    Game::Info  m_gameInfo;

	    bool m_showSettingsWindow    = false;
		bool m_showQuitApproveWindow = false;
		bool m_showDebugWindow       = false;

        bool m_ShowPlayer2Stats = false;
        bool m_ShowPlayer3Stats = false;
        bool m_ShowPlayer4Stats = false;

		bool m_openCardsMode = false;

		vector<Sprite> m_mainMenuSprites;
	    vector<Sprite> m_gameBoardGeneral;

        vector<Sprite> m_gameBoardCards;
        vector<Card>   m_gameBoardCardsRef;

        Card m_hoveredCardCopy;
	};
}
