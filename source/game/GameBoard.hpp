#pragma once

#include <map>
#include <string>
#include <vector>
#include <random> 
#include <iterator> 
#include <algorithm> 

#include "../engine/Logger.hpp"
#include "../engine/ResourseManager.hpp"

#include "GameInfo.hpp"

using namespace std;

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

  static const CardOwner CardPlayerOwners[] = {
	  CARD_OWNER_PLAYER1, CARD_OWNER_PLAYER2,
	  CARD_OWNER_PLAYER3, CARD_OWNER_PLAYER4,
  };

  struct Card
  {
	CardRank  cardRank;
	CardSuit  cardSuit;
	CardOwner cardOwner;

	string textureHandleMain;
	string textureHandleBack;
  };

  struct SaveData
  {
	  GameState gameState;

	  vector<Card> cards;
  };

  class Board
  {
  public:
	 Board();

	~Board();

  public:
	inline bool isPendingAutoMove()
	{
		  return m_PendingAutoMove;
	}

	inline vector<Card>& getCards(void)
	{
	    return(m_Cards);
	}

	inline vector<Card>& getCardsRewind(void)
	{
		return(m_CardsSnapshot);
	}

	inline vector<Card>& getDeck(void)
	{
		return m_Deck;
	}

	inline long long getCurrentStep() const
	{
		return m_GameStep;
	}

	inline bool isPrepared() const
	{
		return (m_GameStep <= 3) ? false : true;
	}

	CardOwner getDeliverer() const
	{
		return m_Deliverer;
	}
	
	Card& getCardRef(CardSuit cardSuit, CardRank cardRank, bool rewind = false);


	void assignCardsToThePlayers(void);


	void shuffleDeck(void);

	void generateDeck(void);


	void step(void);


	bool moveIsValid(Card& card);

	void move(Card& card);

  private:
	void moveCardAI(CardOwner cardOwner);
	  
	void getDeckCard(CardOwner cardOwner);

	Card& getCardRefByOwner(CardOwner cardOwner, bool reverse = false);
	Card  getCardByOwner   (CardOwner cardOwner, bool reverse = false);
	Card& getCardRef       (Card card);
	Card  getCard          (Card card);

	string loadTextureForCard(int cardRank, int cardSuit);

	void assignNextDeliverer(void);

  private:
	bool          m_PendingAutoMove;

	vector<Card>  m_Cards;
	vector<Card>  m_CardsSnapshot; // the cards state on previous move
	vector<Card>  m_Deck;

	CardOwner     m_Deliverer;
	random_device m_RandomDevice;
	mt19937       m_RandomGenerator;
	long long     m_GameStep;
  };

}	
