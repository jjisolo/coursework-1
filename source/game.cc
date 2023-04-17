#include "game.hh"

#include <cstdio>
#include <memory>
#include <random>
#include <algorithm>

#include <spdlog/spdlog.h>

static const constexpr int PLAYERS_NUM = 5;
static const constexpr int CARDS_NUM = 36;
static const constexpr int CARDS_ON_START = 5;
static const constexpr int CARDS_PLAYERS_ON_START = PLAYERS_NUM*CARDS_ON_START;
static const constexpr int CARD_ATLAS_CARD_WIDTH  = 120;
static const constexpr int CARD_ATLAS_CARD_HEIGHT = 80;
static constexpr const char* CARDS_ATLAS_FILENAME = "data/cards.png";

static_assert(PLAYERS_NUM*CARDS_ON_START <= CARDS_NUM);

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
      std::size_t cardIndex = static_cast< std::size_t >(cardCounter);

      m_Cards[cardIndex].setSuit(static_cast< Game::CardSuit >(cardSuit));
      m_Cards[cardIndex].setRank(static_cast< Game::CardRank >(cardRank));

      m_Cards[cardIndex].getSpriteRef().setTexture(m_CardTextureAtlas);
      m_Cards[cardIndex].getSpriteRef().setTextureRect(
        sf::IntRect(0, 0, CARD_ATLAS_CARD_WIDTH, CARD_ATLAS_CARD_HEIGHT)
      );

      cardCounter++;
    }
  }

  spdlog::info("Game board has been created!");
}

Game::Card Game::Board::getCard(std::size_t cardIndex) const {
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

void Game::Board::turn() {
    if(m_TurnNumber == 0) {
        shuffleCards();

        for(std::size_t cardIndex{0}, playerIndex{0};
            cardIndex < CARDS_PLAYERS_ON_START;
            ++cardIndex)
        {
            if((cardIndex+1 % 5 == 0) == 0) ++playerIndex;

            auto cardRank = m_Cards[cardIndex].getRank();
            auto cardSuit  = m_Cards[cardIndex].getSuit();
            m_Players.at(playerIndex).addCard(cardSuit, cardRank);

            m_Cards[cardIndex].setInPlayerPocket(true);
            m_Cards[cardIndex].setOwner(playerIndex);
        }
    }
}


