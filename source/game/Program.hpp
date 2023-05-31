#pragma once

#include "../engine/Application.hpp"
#include "../engine/Sprite.hpp"
#include "../engine/AnimatedSprite.hpp"

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

		void calculateRenderAreas();

		void arrangePlayerSprite(CardOwner cardOwner);

		void arrangeDeckSprites();

		void arrangeBoardSprites();

		vector<Card> searchCard(CardOwner owner, bool rewind=true);

	private:
		pair<vec2, vec2> getRenderAreaBasedOnCardOwner(CardOwner cardOwner);

		void renderSpriteGroup(vector<AnimatedSprite>& spriteGroup);

	    void renderGameBoardUI(ivec2& windowDimensions);
	    
	    void renderMainMenuUI(ivec2& windowDimensions);

		void renderQuitApproveUI();

		void renderSettingsUI();

		void renderPlayerStatUI(CardOwner owner);

	private:
	    Game::Board m_gameBoard;
	    Game::Info  m_gameInfo;

	    bool m_showSettingsWindow    = false;
		bool m_showQuitApproveWindow = false;
		bool m_showDebugWindow       = false;
		bool m_showBoardMenuWindow   = false;

        bool m_ShowPlayer2Stats = false;
        bool m_ShowPlayer3Stats = false;
        bool m_ShowPlayer4Stats = false;

		bool m_openCardsMode    = false;

		bool m_gameBoardPendingUpdate = false;

		vec2 m_playerRenderAreaStart1;
		vec2 m_playerRenderAreaStart2;
		vec2 m_playerRenderAreaStart3;
		vec2 m_playerRenderAreaStart4;

		vec2 m_playerRenderAreaEnd1;
		vec2 m_playerRenderAreaEnd2;
		vec2 m_playerRenderAreaEnd3;
		vec2 m_playerRenderAreaEnd4;

		vec2 m_boardPosition;
		vec2 m_deckPosition;

        vector<Sprite> m_mainMenuSprites;
	    vector<Sprite> m_gameBoardGeneral;

        vector<AnimatedSprite> m_gameBoardCards;
        vector<Card>           m_gameBoardCardsRef;

        Card m_hoveredCardCopy;
		Card m_lastCardCopy;
	};
}
