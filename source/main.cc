#include "engine.hh"
#include "game.hh"
#include "player.hh"

#include "spdlog/spdlog.h"

int main(int argc, char* argv[]) {
  spdlog::info("Welcome to spdlog!");

  Core::Engine applicationEngine;
  Game::Board  applicationBoard;

  applicationEngine.run();
  return 0;
}
