#pragma once

#include "../engine/Application.hpp"

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
	};
}