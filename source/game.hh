#ifndef __GAME_HH__
#define __GAME_HH__

#include "card.hh"

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

namespace Game {
  
  class Board
  {
    public:
      Board(Board&)  = delete;
      Board(Board&&) = delete;
      Board();
      
      void shuffleCards();

      inline sf::Sprite* getCardSprites() {
        return m_CardSprites.get();
      }

    private:
      std::unique_ptr< Card[] > m_Cards;
      std::unique_ptr< sf::Sprite[] > m_CardSprites;
  };
}

#endif
