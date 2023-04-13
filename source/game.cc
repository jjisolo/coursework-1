#include "game.hh"

#include <cstdio>
#include <memory>
#include <random>
#include <algorithm>

#include <spdlog/spdlog.h>

static const constexpr std::size_t CARDS_NUM = 36;
static const constexpr std::size_t CARD_ATLAS_CARD_WIDTH  = 120;
static const constexpr std::size_t CARD_ATLAS_CARD_HEIGHT = 80;
static constexpr const char* CARDS_ATLAS_FILENAME = "data/cards.png";

Game::Board::Board()
{
  // Allocate the memory for the card array and corresponding cards sprites
  m_Cards = std::unique_ptr< Game::Card[] >(new Game::Card[CARDS_NUM]);

  // Initialize the cards texture array
  if(!m_CardTextureAtlas.loadFromFile(CARDS_ATLAS_FILENAME)) {
    spdlog::error("Unable to load cards texture atlas");
    exit(-1);
  }

  for(int cardCounter= 0, cardSuit = Diamonds; cardSuit != Spades; ++cardSuit) {
    for(int cardRank = Ace; cardRank != Six; ++cardRank) {
      m_Cards[cardCounter].setSuit(static_cast< Game::CardSuit >(cardSuit));
      m_Cards[cardCounter].setRank(static_cast< Game::CardRank >(cardRank));

      m_Cards[cardCounter].getSpriteRef().setTexture(m_CardTextureAtlas);
      m_Cards[cardCounter].getSpriteRef().setTextureRect(
        sf::IntRect(0, 0, CARD_ATLAS_CARD_WIDTH, CARD_ATLAS_CARD_HEIGHT)
      );

      cardCounter++;
    }
  }

  spdlog::info("Game board has been created!");
}

Game::Card Game::Board::getCard(int unsigned cardIndex) const {
  if(cardIndex > CARDS_NUM || cardIndex < 0) {
    spdlog::error("Requested inappropriate card index array(out of bounds): " + std::to_string(cardIndex));
  }

  return m_Cards[cardIndex];
}

void Game::Board::shuffleCards() {
  std::random_device randomDevice;
  std::mt19937 randomGenerator(randomDevice());
  std::shuffle(m_Cards.get(), m_Cards.get() + CARDS_NUM, randomGenerator);

  spdlog::info("board is shuffled");
}


