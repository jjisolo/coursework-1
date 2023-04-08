#ifndef __ENGINE_HH__
#define __ENGINE_HH__

#include <SFML/Graphics.hpp>

namespace Core {

class Engine
{
  public:
    Engine(Engine&)  = delete;
    Engine(Engine&&) = delete;
    Engine();

    void run(void);

  private:
    void processInput(void);
    void processEvents(void);
    void update(float elapsedTime);
    void render(void);

  private:
    sf::RenderWindow m_RenderWindow;
    sf::Sprite       m_BackgroundSrite;
    sf::Texture      m_BackgroundTexture;
};

}

#endif //__ENGINE_HH
