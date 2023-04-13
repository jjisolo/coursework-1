#ifndef __GAME_HH__
#define __GAME_HH__

#include "card.hh"

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

namespace Game {
  enum GameState
  {
    GAME_STATE_MAIN_MENU,
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
      GameState m_GameState;
      
      sf::Texture m_CardTextureAtlas;
      std::unique_ptr< Card[] > m_Cards;
      std::unique_ptr< sf::Sprite[] > m_CardSprites;

  };
}

#endif
