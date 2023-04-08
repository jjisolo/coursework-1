#include "game.hh"

#include <cstdio>
#include <memory>

static const constexpr std::size_t CARDS_NUM = 36;

Game::Board::Board()
{
  // Allocate the memory for the card array
  m_Cards = std::unique_ptr< Game::Card[] >(new Game::Card[CARDS_NUM]);

  // Fill the cards array with unique values
  std::uint16_t cardCounter = 0u;
  for(int cardSuit = Game::CardSuit::Diamonds; cardSuit != Game::CardSuit::Spades; ++cardSuit) {
    for(int cardRank = Game::CardRank::Ace; cardRank != Game::CardRank::Six; ++cardRank) {
      m_Cards[cardCounter].setSuit(static_cast< Game::CardSuit >(cardSuit));
      m_Cards[cardCounter].setRank(static_cast< Game::CardRank >(cardRank));
      cardCounter++;
    }
  }

}


