#ifndef __GAME_HH__
#define __GAME_HH__

#include "card.hh"

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

namespace Game {

  enum State
  {
    STATE_UNASSIGNED,
    STATE_MAIN_MENU,
    STATE_OPTIONS,
    STATE_GAME_START,
  };

  class Board
  {
    public:
      Board(Board&)  = delete;
      Board(Board&&) = delete;
      Board();
      
      void shuffleCards();
      Card getCard(int unsigned cardIndex) const;

    private:
      sf::Texture m_CardTextureAtlas;
      std::unique_ptr< Card[] > m_Cards;
      std::unique_ptr< sf::Sprite[] > m_CardSprites;

  };
}

#endif
