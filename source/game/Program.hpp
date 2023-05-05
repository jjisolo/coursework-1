#pragma once

#include "../engine/Application.hpp"
#include "../engine/Sprite.hpp"

namespace Game
{
	enum class GameState
	{
		Main_Menu,
		Game_Board,
	};

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
		GameState m_GameState;

		bool m_showSettingsWindow    = false;
		bool m_showQuitApproveWindow = false;
		bool m_showDebugWindow       = false;

		bool m_openCardsMode = false;

		std::vector<Engine::GFX::Sprite> m_mainMenuSprites;
	};
}