#include "game.hh"

#include <map>
#include <cstdio>
#include <memory>
#include <random>
#include <algorithm>

#include <spdlog/spdlog.h>

static const constexpr int PLAYERS_NUM = 4;
static const constexpr int CARDS_NUM = 36;
static const constexpr int CARDS_ON_START = 5;
static const constexpr int CARDS_PLAYERS_ON_START = PLAYERS_NUM * CARDS_ON_START;
static const constexpr int CARD_ATLAS_CARD_WIDTH = 80;
static const constexpr int CARD_ATLAS_CARD_HEIGHT = 123;
static constexpr const char *CARDS_ATLAS_FILENAME = "data/cards.png";

// Getting some safety just in case starting number of cards is going
// to change somehow.
static_assert(PLAYERS_NUM * CARDS_ON_START <= CARDS_NUM);

// Translate Game::CardSuit enumaration to the string
static std::map<Game::CardSuit, std::string> cardSuit2String = {
  { Game::Diamonds, "CardSuit::Diamonds" },
  { Game::Hearts, "CardSuit::Hearts" },
  { Game::Clubs, "CardSuit::Clubs" },
  { Game::Spades, "CardSuit::Spades" },
};

// Translate Game::CardRank enumaration to the string
static std::map<Game::CardRank, std::string> cardRank2String = {
  { Game::Ace, "CardRank::Ace" },
  { Game::Jack, "CardRank::Jack" },
  { Game::Queen, "CardRank::Queen" },
  { Game::King, "CardRank::King" },
  { Game::Tens, "CardRank::Tenth" },
  { Game::Ninth, "CardRank::Ninth" },
  { Game::Eights, "CardRank::Eights" },
  { Game::Seventh, "CardRank::Seventh" },
  { Game::Six, "CardRank::Six" },
};

Game::Board::Board()
{
  // Initialize card texture and its corresponding spirte
  if (!m_CardTextureAtlas.loadFromFile(CARDS_ATLAS_FILENAME)) {
    spdlog::error("Unable to load cards texture atlas");
    exit(-1);
  }

  // For each card Suit and each Rank generate a card class and then push
  // it to the global cards array(to the deck).
  m_Cards.reserve(CARDS_NUM);

  for (int cardSuit = Diamonds; cardSuit != Spades; ++cardSuit) {
    for (int cardRank = Ace; cardRank != Six; ++cardRank) {
      Game::Card dummyCard(static_cast<Game::CardSuit>(cardSuit), static_cast<Game::CardRank>(cardRank));
      dummyCard.getSpriteRef().setTextureRect({ 0, 0, CARD_ATLAS_CARD_WIDTH, CARD_ATLAS_CARD_HEIGHT });
      dummyCard.getSpriteRef().setTexture(m_CardTextureAtlas);
      m_Cards.push_back(dummyCard);
    }
  }

  // The all initialization is done, and its time to assign the 5 cards
  // to all the players.
  shuffleCards();

  // And finally log the changes
  spdlog::info("Game board has been created!");
}

void Game::Board::prepare() {

  // Assign the cards from the card deck to the each player, actually,
  // we're bringing the last card from the deck, but as soon as the deck
  // shuffled no one cares.
  //
  // And by the way create the players
  m_Players.reserve(PLAYERS_NUM);

  for (std::size_t playerIndex{ 0 }; playerIndex < PLAYERS_NUM; playerIndex++) {
    m_Players.emplace_back(Game::Player());

    // Give player CARDS_ON_START cards from the top of the deck
    for (std::size_t cardIndex{ 0 }; cardIndex < CARDS_ON_START; cardIndex++)
      giveCardFromDeck(playerIndex);
  }
}

void Game::Board::giveCardFromDeck(std::size_t playerIndex)
{
  if (playerIndex > m_Players.size() || playerIndex < 0) {
    spdlog::error("Requested inappropriate player index(out of bounds): " + std::to_string(playerIndex));
    throw std::invalid_argument("The index is out of bounds");
  }

  Game::Card card = m_Cards.back();

  // Insert the card from to the player hand, remove it from the
  // cards deck
  m_Players.at(playerIndex).addCard(m_Cards.back());
  m_Cards.pop_back();

  spdlog::debug("Player #" + std::to_string(playerIndex) + " recieved card with props: { " + cardSuit2String[card.getSuit()] + ", " + cardRank2String[card.getRank()] + "} ");
}

Game::Card Game::Board::getCard(std::size_t cardIndex) const
{
  if (cardIndex > m_Cards.size() || cardIndex < 0) {
    spdlog::error("Requested inappropriate card index array(out of bounds): " + std::to_string(cardIndex));
    throw std::invalid_argument("The index is out of bounds");
  }

  return m_Cards.at(cardIndex);
}

void Game::Board::shuffleCards()
{
  std::random_device randomDevice;
  std::mt19937 randomGenerator(randomDevice());

  spdlog::info("Cards in the deck has been shuffled");
}

void Game::Board::turn()
{
}
