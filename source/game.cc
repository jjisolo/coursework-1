#include "game.hh"

#include <cstdio>
#include <memory>
#include <random>
#include <algorithm>

static const constexpr std::size_t CARDS_NUM = 36;

Game::Board::Board()
{
  // Allocate the memory for the card array
  m_Cards = std::unique_ptr< Game::Card[] >(new Game::Card[CARDS_NUM]);

  // Fill the cards array with unique values
  std::uint16_t cardCounter = 0u;
  for(int cardSuit = Diamonds; cardSuit != Spades; ++cardSuit) {
    for(int cardRank = Ace; cardRank != Six; ++cardRank) {
      m_Cards[cardCounter].setSuit(static_cast< Game::CardSuit >(cardSuit));
      m_Cards[cardCounter].setRank(static_cast< Game::CardRank >(cardRank));
      cardCounter++;
    }
  }
}

void Game::Board::shuffleCards() {
  std::random_device randomDevice;
  std::mt19937 randomGenerator(randomDevice());
  std::shuffle(m_Cards.get(), m_Cards.get() + CARDS_NUM, randomGenerator);  
}


