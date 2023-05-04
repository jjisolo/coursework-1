#include "engine/Application.hpp"
#include "engine/Logger.hpp"

#include "game/Program.hpp"

int main(int argc, char** argv[])
{
	UnreferencedParameter(argc);
	UnreferencedParameter(argv);

	Game::GameProgram gameProgram;
	gameProgram.execute();

	return(0);
}