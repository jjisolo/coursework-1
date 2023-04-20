#include "engine.hh"
#include "game.hh"
#include "player.hh"

#include "spdlog/spdlog.h"
#include "imgui-SFML.h"
#include "imgui.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
  // Set up logging
  spdlog::info("Welcome to spdlog!");
  spdlog::set_level(spdlog::level::debug);

  Core::Engine applicationEngine;

  applicationEngine.run();
  return 0;
}
