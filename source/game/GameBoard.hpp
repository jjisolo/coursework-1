#pragma once

#include <map>
#include <string>
#include <vector>
#include <random> 
#include <iterator> 
#include <algorithm> 

#include "../engine/Logger.hpp"
#include "../engine/ResourseManager.hpp"

namespace Game
{
  enum CardRank
  {
	  Diamonds     = 1,
	  Hearts       = 2,
	  Spades       = 3,
	  Clubs        = 4,
	  CardRankLast = 5,
  };

  enum CardSuit
  {
  	  Ace          = 1,
	  Six          = 2,
	  Seven        = 3,
	  Eight        = 4,
	  Nine         = 5,
	  Ten          = 6,
	  Jack         = 7,
	  Queen        = 8,
	  King         = 9,
	  CardSuitLast = 10,
  };

  enum CardOwner
  {
	CARD_OWNER_PLAYER1,
	CARD_OWNER_PLAYER2,
	CARD_OWNER_PLAYER3,
	CARD_OWNER_PLAYER4,
	CARD_OWNER_HEAP,
	CARD_OWNER_DECK,
	CARD_OWNER_BOARD,
  };

  struct Card
  {
	CardRank  cardRank;
	CardSuit  cardSuit;
	CardOwner cardOwner;

	std::string textureHandleMain;
	std::string textureHandleBack;
  };

  class Board
  {
  public:
	 Board();
	~Board();

  public:
	inline std::vector<Card>& getCards(void)
	{
	  return(m_Cards);
	}

	void assignCardsToThePlayers(void);

	void shuffleDeck(void);

	void generateDeck(void);

  private:
	std::string loadTextureForCard(int cardRank, int cardSuit);

  private:
	std::vector<Card> m_Cards;
  };

}
