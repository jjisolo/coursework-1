#include "engine.hh"
#include "game.hh"
#include "player.hh"

#include "spdlog/spdlog.h"
#include "imgui-SFML.h"
#include "imgui.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
  spdlog::info("Welcome to spdlog!");

  Core::Engine applicationEngine;

  applicationEngine.run();
  return 0;
}
