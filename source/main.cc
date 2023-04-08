#include "engine.hh"
#include "game.hh"
#include "player.hh"

int main(int argc, char* argv[]) {
  Core::Engine applicationEngine;
  Game::Board  applicationBoard;

  applicationEngine.run();
  return 0;
}
