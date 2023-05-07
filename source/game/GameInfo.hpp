#pragma once

namespace Game
{
	enum class GameState
	{
		Main_Menu,
		Game_Board,
	};

	struct GameInfo
	{
		GameState gameState;
	};
}