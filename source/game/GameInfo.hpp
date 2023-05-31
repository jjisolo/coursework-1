#pragma once

namespace Game
{
	enum class GameState
	{
		Main_Menu,
		Game_Board,
		Game_Ended,
	};

	struct Info
	{
		GameState gameState;
	};
}
