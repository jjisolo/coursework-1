#include "GameBoard.hpp"

using namespace std;
using namespace Engine;

namespace Game
{
  Game::Board::Board()
  {
	  m_RandomGenerator = mt19937(m_RandomDevice());
	  m_GameStep        = 0;
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
    Engine::Logger::m_GameLogger->info("Assigning starting card to the players!");

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
	
	// Determine card deliverer
	uniform_int_distribution<mt19937::result_type> distribution(0, 3);
	m_Deliverer = CardPlayerOwners[distribution(m_RandomGenerator)];

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

  void Board::step(void)
  {
	  // Save the card state snapshot
	  //copy(m_Cards.begin(), m_Cards.end(), back_inserter(m_CardsSnapshot));
	  m_CardsSnapshot = m_Cards;
	  switch(m_GameStep)
	  {
	  case 0: {
		  // First move: Shuffle the deck
		  shuffleDeck();
	  } break;

	  case 1: {
		  // Second move: Assign the cards to the players
		  assignCardsToThePlayers();
	  } break;

	  case 2: {
		  // Third move: Make deliverer move
		  //getCardRefByOwner(CARD_OWNER_PLAYER1, true).cardOwner = CARD_OWNER_PLAYER2;
		  assignNextDeliverer();
	  } break;

	  default: {

	  } break;

	  }

	  assignNextDeliverer();

	  Engine::Logger::m_GameLogger->info("Current step game is {}", m_GameStep);

	  m_GameStep++;
  }

  void Board::assignNextDeliverer(void)
  {
	  switch (m_Deliverer)
	  {
	  case CARD_OWNER_PLAYER1:
		  m_Deliverer = CARD_OWNER_PLAYER2;
	  case CARD_OWNER_PLAYER2:
		  m_Deliverer = CARD_OWNER_PLAYER3;
	  case CARD_OWNER_PLAYER3:
		  m_Deliverer = CARD_OWNER_PLAYER4;
	  case CARD_OWNER_PLAYER4:
		  m_Deliverer = CARD_OWNER_PLAYER1;
	  default:
		  m_Deliverer = CARD_OWNER_PLAYER1;
	  }
  }

  void Board::shuffleDeck(void)
  {
	Engine::Logger::m_GameLogger->info("Shuffling game board");
	
	shuffle(m_Cards.begin(), m_Cards.end(), m_RandomGenerator);
  }

  Card& Board::getCardRef(CardSuit cardSuit, CardRank cardRank, bool rewind)
  {
    for(auto& card: !rewind ? m_Cards : m_CardsSnapshot)
      if(card.cardRank == cardRank && card.cardSuit == cardSuit)
        return(card);
  }

  Card& Board::getCardRefByOwner(CardOwner cardOwner, bool reverse)
  {
	  if (reverse)
	  {
		  vector<Card>::reverse_iterator iterator = m_Cards.rbegin();
		  
		  while (iterator != m_Cards.rend())
		  {
			  if ((*iterator).cardOwner == cardOwner)
				  return *iterator;

			  iterator++;
		  }
	  }
	  else
	  {
		  vector<Card>::iterator iterator = m_Cards.begin();

		  while (iterator != m_Cards.end())
		  {
			  if ((*iterator).cardOwner == cardOwner)
				  return *iterator;

			  iterator++;
		  }
	  }
  }

}
