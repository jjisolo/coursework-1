#include "GameBoard.hpp"

using namespace std;
using namespace Engine;

namespace Game
{
	Game::Board::Board()
	{
		m_RandomGenerator = mt19937(m_RandomDevice());
		m_GameStep = 0;
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

		if (cardRank == Clubs)
			return(loadTextureA("clubs"));

		if (cardRank == Spades)
			return(loadTextureA("spades"));

		if (cardRank == Hearts)
			return(loadTextureA("hearts"));

		if (cardRank == Diamonds)
			return(loadTextureA("diamonds"));
	}

	void Board::assignCardsToThePlayers(void)
	{
		Engine::Logger::m_GameLogger->info("Assigning starting card to the players!");

		// Player 1
		for (size_t i = 0; i < 5; ++i)
			m_Cards[i].cardOwner = CARD_OWNER_PLAYER1;

		// Player 2
		for (size_t i = 5; i < 10; ++i)
			m_Cards[i].cardOwner = CARD_OWNER_PLAYER2;

		// Player 3
		for (size_t i = 10; i < 15; ++i)
			m_Cards[i].cardOwner = CARD_OWNER_PLAYER3;

		// Player 4
		for (size_t i = 15; i < 20; ++i)
			m_Cards[i].cardOwner = CARD_OWNER_PLAYER4;
	}

	void Board::generateDeck(void)
	{
		Engine::Logger::m_GameLogger->info("Generating card deck");

		// Determine card deliverer
		uniform_int_distribution<mt19937::result_type> distribution(0, 3);
		m_Deliverer = CardPlayerOwners[distribution(m_RandomGenerator)];

		// Assign proper texture to each card in the deck.
		for (int cardRank = Diamonds; cardRank != CardRankLast; ++cardRank) {
			for (int cardSuit = Ace; cardSuit != CardSuitLast; ++cardSuit) {
				Card dummyCard;
				dummyCard.cardRank = static_cast<CardRank>(cardRank);
				dummyCard.cardSuit = static_cast<CardSuit>(cardSuit);
				dummyCard.cardOwner = CARD_OWNER_DECK;
				dummyCard.textureHandleMain = loadTextureForCard(cardRank, cardSuit);
				dummyCard.textureHandleBack = "card-back-green";

				m_Cards.push_back(dummyCard);
			}
		}
	}

	void Board::moveCardAI(CardOwner cardOwner)
	{
		// Get all cards of this owner
		vector<Card> playerCards;

		for (auto card : m_Cards)
		{
			if (card.cardOwner == cardOwner && moveIsValid(card))
			{
				move(card);
				return;
			}
		}

		// Get the card if we found none
		getDeckCard(cardOwner);
	}

	void Board::getDeckCard(CardOwner cardOwner)
	{
		for (auto card : m_Cards)
		{
			if (card.cardOwner == CARD_OWNER_DECK) 
			{
				getCardRef(card).cardOwner = cardOwner;
				return;
			}
		}
	}

	void Board::step(void)
	{
		// Save the card state snapshot
		//copy(m_Cards.begin(), m_Cards.end(), back_inserter(m_CardsSnapshot));
		m_CardsSnapshot = m_Cards;
		switch (m_GameStep)
		{
		case 0: {
			// First move: Shuffle the deck
			shuffleDeck();
			m_GameStep++;
			return;
		} break;

		case 1: {
			// Second move: Assign the cards to the players
			assignCardsToThePlayers();
			m_GameStep++;
			return;
		} break;

		case 2: {
			// Third move: Make deliverer move
			move(getCardRefByOwner(m_Deliverer, true));
			return;
		} break;

		}

		// If the main player is not the deliverer make the AI)))))))))))) move the card
		if (m_Deliverer != CARD_OWNER_PLAYER1 && m_GameStep > 2)
		{
			moveCardAI(m_Deliverer);
		}

		// Check if the main player has no valid moves, if so give him the card
		if (m_Deliverer == CARD_OWNER_PLAYER1)
		{
			bool found = false;

			for (auto card : m_Cards)
			{
				if (card.cardOwner == CARD_OWNER_PLAYER1)
				{
					if (moveIsValid(card))
					{
						found = true;
					}
				}
			}

			if (!found)
			{
				getDeckCard(CARD_OWNER_PLAYER1);

				assignNextDeliverer();
				m_GameStep++;

				moveCardAI(m_Deliverer);
			}
		}
	}

	void Board::assignNextDeliverer(void)
	{
		switch (m_Deliverer)
		{
		case CARD_OWNER_PLAYER1:
			m_Deliverer = CARD_OWNER_PLAYER4;
			break;
		case CARD_OWNER_PLAYER2:
			m_Deliverer = CARD_OWNER_PLAYER1;
			break;
		case CARD_OWNER_PLAYER3:
			m_Deliverer = CARD_OWNER_PLAYER2;
			break;
		case CARD_OWNER_PLAYER4:
			m_Deliverer = CARD_OWNER_PLAYER3;
			break;
		default:
			m_Deliverer = CARD_OWNER_PLAYER1;
			break;
		}
	}

	bool Board::moveIsValid(Card& card)
	{
		if (m_Deck.size() > 0)
		{
			if (card.cardSuit == m_Deck.back().cardSuit || card.cardRank == m_Deck.back().cardRank)
			{
				return true;
			}

			return false;
		}

		return true;
	}

  Card& Board::getCardRef(Card card)
  {
	for (auto& it : m_Cards)
	{
		if (it.cardSuit == card.cardSuit && it.cardRank == card.cardRank)
		{
			return it;
		}
	}
  }

  Card Board::getCard(Card card)
  {
	return getCardRef(card);
  }

  void Board::move(Card& card)
  {
	  if (moveIsValid(card))
	  {
		  Engine::Logger::m_GameLogger->info("Move card");
			  
		  m_Deck.push_back(getCard(card));
		  getCardRef(card).cardOwner = CARD_OWNER_BOARD;

		  assignNextDeliverer();
		  m_GameStep++;

		  if (m_Deliverer == CARD_OWNER_PLAYER1)
		  {
			  m_PendingAutoMove = false;
		  }
		  else
		  {
			  m_PendingAutoMove = true;
		  }
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

  Card Board::getCardByOwner(CardOwner cardOwner, bool reverse)
  {
	  return getCardRefByOwner(cardOwner, reverse);
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
