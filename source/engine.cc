#include "unordered_map"
#include "engine.hh"
#include "spdlog/spdlog.h"

static constexpr const char* WINDOW_TITLE = "101";
static constexpr const char* BACKGROUND_TEXTURE_PATH  = "data/background.jpg";

static std::unordered_map<Game::State, std::string> StateToString = {
    { Game::STATE_UNASSIGNED, "Unassigned"},
    { Game::STATE_MAIN_MENU,  "Main menu" },
    { Game::STATE_OPTIONS,    "Options"   },
};

void Core::Engine::initializeGraphics() {
    sf::Vector2f screenResolution;
    screenResolution.x = sf::VideoMode::getDesktopMode().width;
    screenResolution.y = sf::VideoMode::getDesktopMode().height;

    m_RenderWindow.create(sf::VideoMode(screenResolution.x, screenResolution.y),
                          WINDOW_TITLE, sf::Style::Fullscreen);
    m_RenderWindow.setFramerateLimit(75);
    m_RenderWindow.setKeyRepeatEnabled(false);

    ImGui::SFML::Init(m_RenderWindow);

    constexpr auto scaleFactor = 2.0;
    ImGui::GetStyle().ScaleAllSizes(scaleFactor);
    ImGui::GetIO().FontGlobalScale = scaleFactor;
}

Core::Engine::Engine() {
  initializeGraphics();

  m_GameState = Game::STATE_MAIN_MENU;
  spdlog::info("SFML window+ImGui has been initialized!");
}

void Core::Engine::run(void) {
  sf::Clock clock;
  sf::Time  deltaTime;
  float     elapsedTime;

  while(m_RenderWindow.isOpen()) {
    deltaTime = clock.restart();
    elapsedTime = deltaTime.asSeconds();

    processEvents();
    update(elapsedTime, deltaTime);
    render(clock);
  }
}

void Core::Engine::processEvents(void) {
  for(sf::Event event; m_RenderWindow.pollEvent(event);) {
      ImGui::SFML::ProcessEvent(event);
      switch(event.type) {
          case sf::Event::KeyReleased:
              switch(event.key.code) {
                  case sf::Keyboard::D: // Set the debug mode
                      m_ApplicationAttributes ^= 1ul << 0;
                      spdlog::info("Debug mode has been toggled");
                      break;
                  case sf::Keyboard::Escape:
                      spdlog::info("Requested to close the window");
                      m_RenderWindow.close();
                      break;
                  default:
                      break;
              }
              break;
         case sf::Event::Closed:
            m_RenderWindow.close();
            break;
      }
  }
}

void Core::Engine::update(float elapsedTime, sf::Time deltaTime) {
    ImGui::SFML::Update(m_RenderWindow, deltaTime);
}

void Core::Engine::render(sf::Clock& clock) {
  m_RenderWindow.clear(sf::Color(38, 80, 14, 255));

  for(int cardIndex = 0; cardIndex < 36; ++cardIndex)
    m_RenderWindow.draw(m_GameBoard.getCard(cardIndex).getSpriteRef());

  if(m_GameState == Game::STATE_MAIN_MENU) guiRenderMenu();

  if(((m_ApplicationAttributes >> 0) & 1u)) guiRenderDebug(clock);
  if(((m_ApplicationAttributes >> 2) & 1u)) guiRenderOptions();

  ImGui::SFML::Render(m_RenderWindow);
  m_RenderWindow.display();
}

void Core::Engine::guiRenderOptions(void) {
    ImGui::Begin("Options", nullptr, ImGuiWindowFlags_NoCollapse);

    ImGui::End();
}

void Core::Engine::guiRenderMenu(void) {
  ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

  if(ImGui::Button("Start")) {
      spdlog::debug("Game state is now STATE_GAME_START");
      m_GameState = Game::STATE_GAME_START;
  } ImGui::SameLine();

  if(ImGui::Button("Options")) {
      spdlog::debug("Options attribute has been toggled");
      m_ApplicationAttributes ^= 1ul << 2;
  } ImGui::SameLine();

  if(ImGui::Button("Exit")) {
    spdlog::debug("Qiut menu attribute has been toggled");
    m_ApplicationAttributes ^= 1ul << 3;
    m_RenderWindow.close();
  } ImGui::SameLine();

  ImGui::End();
}

void Core::Engine::guiRenderDebug(sf::Clock& clock) {
    ImGui::Begin("Debug window");
    ImGui::Text("Frame Time(seconds): %f", clock.restart().asSeconds());
    ImGui::End();
}

Core::Engine::~Engine() {
    ImGui::SFML::Shutdown();

    spdlog::info("SFML engine + ImGui has been shut down");
}

