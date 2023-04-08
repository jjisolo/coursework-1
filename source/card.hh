#ifndef __CARD_HH__
#define __CARD_HH__

#include <cstdint>

namespace Game {
  
  enum CardSuit
  {
    Diamonds, Hearts, Clubs, Spades,
  };
  
  enum CardRank
  {
    Ace, Jack, Queen, King, Tens, Ninth, Eights, Seventh, Six
  };

  class Card
  {
    public:
      inline CardSuit getSuit() { return m_Suit; }
      inline CardRank getRank() { return m_Rank; }
      
      void setSuit(CardSuit newSuit) { m_Suit = newSuit; }
      void setRank(CardRank newRank) { m_Rank = newRank; }

    private:
     CardSuit m_Suit;
     CardRank m_Rank;
  };
}

#endif // __CARD_HH__
