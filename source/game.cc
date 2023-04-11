#include "game.hh"

#include <cstdio>
#include <memory>
#include <random>
#include <algorithm>

static const constexpr std::size_t CARDS_NUM = 36;
static constexpr const char* CARDS_ASSET_FILENAME = "data/cards.png";

Game::Board::Board()
{
  // Allocate the memory for the card array and corresponding cards sprites
  m_CardSprites = std::unique_ptr< sf::Sprite[] >(new sf::Sprite[CARDS_NUM]);
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

  // Initialize the cards texture array
  sf::Texture cardTexture;
  for(int cardIndex = 0; cardIndex < CARDS_NUM; ++cardIndex) {
    cardTexture.loadFromFile(CARDS_ASSET_FILENAME);
    m_CardSprites[cardIndex].setTexture(cardTexture);
  }
}

void Game::Board::shuffleCards() {
  std::random_device randomDevice;
  std::mt19937 randomGenerator(randomDevice());
  std::shuffle(m_Cards.get(), m_Cards.get() + CARDS_NUM, randomGenerator);  
}


