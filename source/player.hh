#ifndef __PLAYER_HH__
#define __PLAYER_HH__

#include "card.hh"
#include <algorithm>

namespace Game {

class Player
{
public:
  // Player(Player&&) = delete;
  // Player(Player&)  = delete;
  Player() = default;

  inline const std::vector<Game::Card> &getCardsRef() const { return m_Cards; }

  inline void addCard(Card card) const
  {
    if (findCard(card.getSuit(), card.getRank()))
      return;

    m_Cards.push_back(card);
  }

  inline void addCard(CardSuit cardSuit, CardRank cardRank) const
  {
    if (findCard(cardSuit, cardRank))
      return;

    m_Cards.push_back(Card(cardSuit, cardRank));
  }

  inline bool findCard(CardSuit cardSuit, CardRank cardRank) const
  {
    for (auto &card : m_Cards)
      if (card.getSuit() == cardSuit && card.getRank() == cardRank)
        return true;
    return false;
  }

  inline void removeCard(CardSuit cardSuit, CardRank cardRank)
  {
    std::size_t cardArraySize = m_Cards.size();
    std::size_t cardIndex = 0;

    // Find the requested card index
    while (cardIndex < cardArraySize) {
      if (m_Cards.at(cardIndex).getSuit() == cardSuit && m_Cards.at(cardIndex).getRank() == cardRank)
        break;
    }

    // If we did not find one, just do nothing
    if (cardIndex >= cardArraySize) {
      auto difference = static_cast<std::__wrap_iter<Game::Card *>::difference_type>(cardIndex);
      m_Cards.erase(std::vector<Card>::iterator(m_Cards.begin() + difference));
    }
  }

private:
  mutable std::vector<Card> m_Cards;
};

}// namespace Game

#endif// __PLAYER_HH__
