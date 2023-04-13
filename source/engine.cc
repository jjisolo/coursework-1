#include "engine.hh"
#include "spdlog/spdlog.h"

static constexpr const char* WINDOW_TITLE = "101";
static constexpr const char* BACKGROUND_TEXTURE_PATH  = "data/background.jpg";

Core::Engine::Engine() {
  sf::Vector2f screenResolution;
  screenResolution.x = sf::VideoMode::getDesktopMode().width;
  screenResolution.y = sf::VideoMode::getDesktopMode().height;

  m_RenderWindow.create(sf::VideoMode(screenResolution.x, screenResolution.y),
                        WINDOW_TITLE, sf::Style::Fullscreen);

  spdlog::info("Render window is initialized!");
}

void Core::Engine::run(void) {
  sf::Clock clock;
  float     elapsedTime;

  while(m_RenderWindow.isOpen()) {
    sf::Time deltaTime = clock.restart();
    elapsedTime = deltaTime.asSeconds();
    
    processInput();
    processEvents();
    update(elapsedTime);
    render();
  }
}

void Core::Engine::processEvents(void) {
  for(sf::Event event; m_RenderWindow.pollEvent(event);) {

  }
}

void Core::Engine::processInput(void) {
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    m_RenderWindow.close();
}

void Core::Engine::update(float elapsedTime) {

}

void Core::Engine::render(void) {
  m_RenderWindow.clear(sf::Color(38, 80, 14, 255));

  for(int cardIndex = 0; cardIndex < 36; ++cardIndex)
    m_RenderWindow.draw(m_GameBoard.getCard(cardIndex).getSpriteRef());

  m_RenderWindow.display();
}



