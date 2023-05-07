#include "GameBoard.hpp"

using namespace Engine;
#define loadTexture ResourceManager::loadTexture;

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
	auto createTexturePath = [&](const std::string& sCardRank) -> std::string 
    {
	  std::string result = "data/assets/";
	  result += sCardRank + "/";
	  result += "card-" + sCardRank + "-";
	  result += std::to_string(cardSuit);
	  result += ".png";
	  return(result);
	};

	auto loadTextureA = [&](const std::string& cardRank) -> std::string
	{
	  std::string texturePath = createTexturePath(cardRank);
	  loadTexture(texturePath.c_str(), true, texturePath);

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

  void Board::generateDeck(void)
  {
	Engine::Logger::m_GameLogger->info("Generating card deck");
	
	// Load the card reverse side textures.
	Engine::ResourceManager::loadTexture("data/assets/card-back1.png", true, "card-back-blue");
    Engine::ResourceManager::loadTexture("data/assets/card-back2.png", true, "card-back-red");
    Engine::ResourceManager::loadTexture("data/assets/card-back3.png", true, "card-back-green");
	Engine::ResourceManager::loadTexture("data/assets/card-back4.png", true, "card-back-yellow");
	
	// Assign proper texture to each card in the deck.
	for(int cardRank = Diamonds; cardRank != Hearts; ++cardRank) {
	  for(int cardSuit = Ace; cardSuit != Six; ++cardSuit) {
		Card dummyCard;
		dummyCard.cardRank          = static_cast<CardRank>(cardRank);
		dummyCard.cardSuit          = static_cast<CardSuit>(cardSuit);
		dummyCard.textureHandleMain = loadTextureForCard(cardRank, cardSuit);
		dummyCard.textureHandleBack = "card-back-green";
	    
		m_Cards.push_back(dummyCard);
	  }
	}
  }

  void Board::shuffleDeck(void)
  {
	Engine::Logger::m_GameLogger->info("Shuffling game board");

	std::random_device randomDevice;
	std::mt19937       randomGenerator(randomDevice());
	
	std::shuffle(m_Cards.begin(), m_Cards.end(), randomGenerator);
  }
}
