#ifndef __GAME_HH__
#define __GAME_HH__

#include "card.hh"
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

    private:
      std::unique_ptr< Card[] > m_Cards;
  };
}

#endif
