#ifndef __CARD_HH__
#define __CARD_HH__

#include <cstdint>
#include <SFML/Graphics.hpp>

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

      void inline setInPlayerPocket(const bool& value) { m_InPlayerPocket = value; }
      void inline setInCardHeap    (const bool& value) { m_InCardHeap     = value; }

      void inline setOwner(const std::size_t& playerIndex) { m_Owner = playerIndex; }

      std::size_t inline getOwner() const { return m_Owner; }

      bool inline isInPlayerPocket() const { return m_InPlayerPocket; }
      bool inline isInCardHeap() const     { return m_InCardHeap; }

      sf::Sprite& getSpriteRef() { return m_Sprite; }

    private:
      sf::Sprite  m_Sprite;

      CardSuit    m_Suit;
      CardRank    m_Rank;

      std::size_t m_Owner;
      bool        m_InPlayerPocket;
      bool        m_InCardHeap;
  };
}

#endif // __CARD_HH__
