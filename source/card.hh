#ifndef __CARD_HH__
#define __CARD_HH__

#include <cstdint>
#include <SFML/Graphics.hpp>

namespace Game {

enum CardSuit {
  Diamonds,
  Hearts,
  Clubs,
  Spades,
};

enum CardRank {
  Ace,
  Jack,
  Queen,
  King,
  Tens,
  Ninth,
  Eights,
  Seventh,
  Six
};

class Card
{
public:
  explicit Card(CardSuit cardSuit, CardRank cardRank) : m_Suit(cardSuit), m_Rank(cardRank)
  {
  }

  Card() = default;

  inline CardSuit getSuit() const { return m_Suit; }
  inline CardRank getRank() const { return m_Rank; }

  void setSuit(CardSuit newSuit) { m_Suit = newSuit; }
  void setRank(CardRank newRank) { m_Rank = newRank; }

  void inline setInPlayerPocket(const bool &value) { m_InPlayerPocket = value; }
  void inline setInCardHeap(const bool &value) { m_InCardHeap = value; }

  void inline setOwner(const std::size_t &playerIndex) { m_Owner = playerIndex; }

  std::size_t inline getOwner() const { return m_Owner; }

  bool inline isInPlayerPocket() const { return m_InPlayerPocket; }
  bool inline isInCardHeap() const { return m_InCardHeap; }

  // SFML sprite interfaces
  void setSpritePosition(const float &positionX, const float &positionY) const
  {
    m_Sprite.setPosition(positionX, positionY);
  }

  void setSpriteRotation(const float &rotationAngle) const
  {
    m_Sprite.setRotation(rotationAngle);
  }

  sf::Sprite &getSpriteRef() const { return static_cast<sf::Sprite &>(m_Sprite); }

private:
  mutable sf::Sprite m_Sprite;

  CardSuit m_Suit;
  CardRank m_Rank;

  std::size_t m_Owner;
  bool m_InPlayerPocket;
  bool m_InCardHeap;
};
}// namespace Game

#endif// __CARD_HH__
