#include "GameBoard.hpp"

using namespace std;
using namespace Engine;

namespace Game
{
  Game::Board::Board()
  {
  }

  Game::Board::~Board()
  {

  }

  std::string Board::loadTextureForCard(int cardRank, int cardSuit)
  {
	auto createTexturePath = [&](const string& sCardRank) -> string 
    {
	  string result = "data/assets/";
	  result += sCardRank + "/";
	  result += "card-" + sCardRank + "-";
	  result += to_string((int)cardSuit);
	  result += ".png";
	  return(result);
	};

	auto loadTextureA = [&](const string& cardRank) -> string
	{
	  string texturePath = createTexturePath(cardRank);
	  Engine::ResourceManager::loadTexture(texturePath.c_str(), true, texturePath);

	  return(texturePath);
	}; 

	if(cardRank == Clubs)
	  return(loadTextureA("clubs"));

	if(cardRank == Spades)
	  return(loadTextureA("spades"));

	if(cardRank == Hearts)
	  return(loadTextureA("hearts"));

	if(cardRank == Diamonds)
	  return(loadTextureA("diamonds"));
  }

  void Board::assignCardsToThePlayers(void)
  {
	// Player 1
	for(size_t i=0; i < 5; ++i)
	  m_Cards[i].cardOwner = CARD_OWNER_PLAYER1;

	// Player 2
	for(size_t i=5; i < 10; ++i)
	  m_Cards[i].cardOwner = CARD_OWNER_PLAYER2;

	// Player 3
	for(size_t i=10; i < 15; ++i)
	  m_Cards[i].cardOwner = CARD_OWNER_PLAYER3;

	// Player 4
	for(size_t i=15; i < 20; ++i)
	  m_Cards[i].cardOwner = CARD_OWNER_PLAYER4;
  }

  void Board::generateDeck(void)
  {
	Engine::Logger::m_GameLogger->info("Generating card deck");
	
	// Assign proper texture to each card in the deck.
	for(int cardRank = Diamonds; cardRank != CardRankLast; ++cardRank) {
	  for(int cardSuit = Ace; cardSuit != CardSuitLast; ++cardSuit) {
		Card dummyCard;
		dummyCard.cardRank          = static_cast<CardRank>(cardRank);
		dummyCard.cardSuit          = static_cast<CardSuit>(cardSuit);
		dummyCard.cardOwner         = CARD_OWNER_DECK;
		dummyCard.textureHandleMain = loadTextureForCard(cardRank, cardSuit);
		dummyCard.textureHandleBack = "card-back-green";
	    
		m_Cards.push_back(dummyCard);
	  }
	}
  }

  void Board::shuffleDeck(void)
  {
	Engine::Logger::m_GameLogger->info("Shuffling game board");

	random_device randomDevice;
	mt19937       randomGenerator(randomDevice());
	
	shuffle(m_Cards.begin(), m_Cards.end(), randomGenerator);
  }

  Card& Board::getCardRef(CardSuit cardSuit, CardRank cardRank)
  {
    for(auto& card: m_Cards)
      if(card.cardRank == cardRank && card.cardSuit == cardSuit)
        return(card);
  }
}
