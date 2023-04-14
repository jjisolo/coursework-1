#ifndef __ENGINE_HH__
#define __ENGINE_HH__

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "game.hh"

namespace Core {

class Engine
{
  public:
    Engine(Engine&)  = delete;
    Engine(Engine&&) = delete;
    Engine();
    ~Engine();

    void run(void);

  private:
    void initializeGraphics(void);

    void processEvents(void);
    void update(float elapsedTime, sf::Time deltaTime);

    void guiRenderDebug(sf::Clock& clock);
    void guiRenderOptions(void);
    void guiRenderMenu(void);

    void render(sf::Clock& clock);

  private:
    sf::RenderWindow m_RenderWindow;
    Game::Board      m_GameBoard;
    int              m_GameState;

    // +-----+-----------------+
    // | BIT | VALUE           |
    // +-----+-----------------+
    // | 0   | Debug Mode      |
    // +-----+-----------------+
    // | 1   | Open Cards Mode |
    // +-----+-----------------+
    // | 2   | Toggle Options  |
    // +-----+-----------------+
    // | 3   | Toggle Quit Menu|
    // +-----+-----------------+
    // | 4   | Reserved        |
    // +-----+-----------------+
    // |  5   | Reserved       |
    // +-----+-----------------+
    // |  6  | Reserved        |
    // +-----+-----------------+
    // | 7   | Reserved        |
    // +-----+-----------------+
    std::uint16_t  m_ApplicationAttributes = 0x0;
};

}

#endif //__ENGINE_HH
