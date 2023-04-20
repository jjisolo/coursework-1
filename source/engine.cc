#include "SFML/Graphics/Rect.hpp"
#include "card.hh"
#include "unordered_map"
#include "engine.hh"
#include "spdlog/spdlog.h"

static constexpr const char* WINDOW_TITLE = "101";
//static constexpr const char* BACKGROUND_TEXTURE_PATH  = "data/background.jpg";

static constexpr auto HELP_TEXT_IN_COLLAPSING_HEADER =
R"('101' card game help text:

Controls:
    --- Toggle debug mode: press key `D` on your
        keyboard, or find the corresponding checkbox
        in the settings menu.

Features:
    --- Debug mode: When the debug mode is enabled,
        the debug window with all debug-necessary
        information pops up.
    --- Open Cards mode: You can see the cards faces
        even if theirs holder is your enemy >.<
)";

[[maybe_unused]] static constexpr auto GAME_TUTORIAL =
R"('101' Card Game Tutorial

)";

static std::unordered_map<Game::State, std::string> StateToString = {
    { Game::STATE_UNASSIGNED, "GAME_STATE_UNASSIGNED"},
    { Game::STATE_MAIN_MENU,  "GAME_STATE_MAIN_MENU" },
};

void Core::Engine::initializeGraphics() {
    sf::VideoMode targetVideoMode = sf::VideoMode::getDesktopMode();
    m_RenderWindow.create(targetVideoMode, WINDOW_TITLE, sf::Style::Fullscreen);
    m_RenderWindow.setFramerateLimit(75);
    m_RenderWindow.setKeyRepeatEnabled(false);

    if(!ImGui::SFML::Init(m_RenderWindow)) {
        spdlog::error("Cannot initialize Dear ImGui!");
        exit(1);
    }

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
          case sf::Event::Resized:
             calculateSpriteAnchors();
             break;
          case sf::Event::Closed:
             m_RenderWindow.close();
             break;
          default:
              break;
      }
  }
}

void Core::Engine::render(sf::Clock& clock) {
  m_RenderWindow.clear(sf::Color(38, 80, 14, 255));

  switch(m_GameState) {
      case Game::STATE_MAIN_MENU: {
          guiRenderMenu();
      } break;

      case Game::STATE_GAME_START: {
          guiRenderTutorial();
          renderGameBoard();
      } break;

      case Game::STATE_UNASSIGNED: {

      } break;

      default: {

      } break;
  }

  if(((m_ApplicationAttributes >> 2) & 1u)) guiRenderOptions();
  if(((m_ApplicationAttributes >> 0) & 1u)) guiRenderDebug(clock);

  ImGui::SFML::Render(m_RenderWindow);
  m_RenderWindow.display();
}

void Core::Engine::renderGameBoard() {
    auto players    = m_GameBoard.getPlayersRef();
    auto windowSize = m_RenderWindow.getSize();
    
    // Render player cards
    for(auto& player: players)
      for(auto& card: player.getCardsRef())
          m_RenderWindow.draw(card.getSpriteRef());
}

static bool DebugModeOptionsCheckBox = false;
static bool OpenCardsOptionsCheckBox = false;
void Core::Engine::guiRenderOptions(void) {
    ImGui::Begin("Options", nullptr, ImGuiWindowFlags_NoCollapse);

    if(ImGui::CollapsingHeader("Help")) {
        ImGui::Text(HELP_TEXT_IN_COLLAPSING_HEADER);
    }

    if(ImGui::CollapsingHeader("Settings")) {
        ImGui::Checkbox("Debug Mode", &DebugModeOptionsCheckBox);
        ImGui::Checkbox("Open Cards Mode", &OpenCardsOptionsCheckBox);
    }

    ImGui::End();
}

void Core::Engine::guiRenderTutorial(void) {
    ImGui::Begin("101 Tutorial");
    ImGui::Text(HELP_TEXT_IN_COLLAPSING_HEADER);
    ImGui::End();
}

void Core::Engine::update([[maybe_unused]] float elapsedTime, sf::Time deltaTime) {
    DebugModeOptionsCheckBox ? m_ApplicationAttributes |= 1ul << 0 : m_ApplicationAttributes &= ~(1ul << 0);
    OpenCardsOptionsCheckBox ? m_ApplicationAttributes |= 1ul << 1 : m_ApplicationAttributes &= ~(1ul << 1);

    ImGui::SFML::Update(m_RenderWindow, deltaTime);
}

void Core::Engine::calculateSpriteAnchors() {
    const auto windowSize = m_RenderWindow.getSize();

    if(m_GameState == Game::STATE_GAME_START) {
        m_MainPlayerCardRenderArea.left   = windowSize.x*0.35f;
        m_MainPlayerCardRenderArea.top    = windowSize.y*0.75f;
        m_MainPlayerCardRenderArea.width  = windowSize.x*0.70f - m_MainPlayerCardRenderArea.left;
        m_MainPlayerCardRenderArea.height = windowSize.y*0.80f - m_MainPlayerCardRenderArea.top;
        
        // ...
        auto adjustPlayerSpritePosition = [&](const sf::FloatRect& renderArea, const std::vector< Game::Card >& cards) -> void {
            if(cards.empty()) return;

            const auto cardAngle   = 90 / cards.size();
            const auto cardOffsetX = renderArea.width  / cards.size();
            const auto cardOffsetY = renderArea.height / cards.size();

            for(std::size_t cardIndex{0}; cardIndex < cards.size(); cardIndex++) {
                auto cardPositionX = renderArea.left + cardOffsetX*cardIndex;
                auto cardPositionY = renderArea.top + cardOffsetY*cardIndex;
                
                cards[cardIndex].getSpriteRef().setPosition(cardPositionX, cardPositionY);
                cards[cardIndex].getSpriteRef().setRotation(cardAngle*cardIndex);
            }
        };

        const auto& players = m_GameBoard.getPlayersRef();
        adjustPlayerSpritePosition(m_MainPlayerCardRenderArea, players.at(0).getCardsRef());
    }
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
    spdlog::debug("Quit menu attribute has been toggled");
    m_ApplicationAttributes ^= 1ul << 3;
    m_RenderWindow.close();
  } ImGui::SameLine();

  ImGui::End();
}

void Core::Engine::guiRenderDebug(sf::Clock& clock) {
    ImGui::Begin("Debug window");
    ImGui::Text("Frame Time(seconds): %f\n", clock.restart().asSeconds());
    ImGui::Text("Game State: %s", StateToString[m_GameState].c_str());
    ImGui::End();
}

Core::Engine::~Engine() {
    ImGui::SFML::Shutdown();

    spdlog::info("SFML engine + ImGui has been shut down");
}

