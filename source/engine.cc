#include "engine.hh"

static constexpr const char* WINDOW_TITLE = "101";
static constexpr const char* BACKGROUND_TEXTURE_PATH  = "data/background.jpg";

Core::Engine::Engine() {
  sf::Vector2f screenResolution;
  screenResolution.x = sf::VideoMode::getDesktopMode().width;
  screenResolution.y = sf::VideoMode::getDesktopMode().height;

  m_RenderWindow.create(sf::VideoMode(screenResolution.x, screenResolution.y),
                        WINDOW_TITLE, sf::Style::Fullscreen);
  
  //m_BackgroundTexture.loadFromFile(BACKGROUND_TEXTURE_PATH);
  //m_BackgroundSrite.setTexture(m_BackgroundTexture);
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
  m_RenderWindow.clear(sf::Color::Green);

  m_RenderWindow.display();
}



