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
	  Diamonds,
	  Hearts,
	  Spades,
	  Clubs,
  };

  enum CardSuit
  {
	Ace    = 1,
    Six    = 6,
	Seven  = 7,
	Eight  = 8,
    Nine   = 9,
    Ten    = 10,
	Jack   = 11,
    Queen  = 12,
    King   = 13,
  };

  struct Card
  {
	CardRank cardRank;
	CardSuit cardSuit;

	std::string textureHandleMain;
	std::string textureHandleBack;
  };

  class Board
  {
  public:
	 Board();
	~Board();

  public:
	void shuffleDeck(void);

	void generateDeck(void);

  private:
	std::string loadTextureForCard(int cardRank, int cardSuit);

  private:
	std::vector<Card> m_Cards;
  };

}
