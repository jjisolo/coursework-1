#ifndef __ENGINE_HH__
#define __ENGINE_HH__

#include <SFML/Graphics.hpp>
#include "game.hh"

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
    Game::Board      m_GameBoard;
};

}

#endif //__ENGINE_HH
