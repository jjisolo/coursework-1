#ifndef __PLAYER_HH__
#define __PLAYER_HH__

#include "card.hh"
#include <algorithm>

namespace Game
{

class Player 
{
  public:
    Player(Player&&) = delete;
    Player(Player&)  = delete;
    Player()         = default;

    inline void addCard(CardSuit cardSuit, CardRank cardRank) {
      if(findCard( cardSuit, cardRank ))
        return;

      Card dummyCard;
      dummyCard.setSuit(cardSuit);
      dummyCard.setRank(cardRank);
      m_Cards.push_back(dummyCard);
    }
  
    inline bool findCard(CardSuit cardSuit, CardRank cardRank) {
      for(auto& card: m_Cards)
        if(card.getSuit() == cardSuit && card.getRank() == cardRank)
          return true;
      return false;
    }

    inline void removeCard(CardSuit cardSuit, CardRank cardRank) {
      int unsigned cardArraySize = m_Cards.size();
      int unsigned cardIndex = 0u;

      // Find the requested card index
      while( cardIndex < cardArraySize ) {
        if( m_Cards.at(cardIndex).getSuit() == cardSuit && m_Cards.at(cardIndex).getRank() == cardRank )
          break;    
      }
      
      // If we did not find one, just do nothing
      if( cardIndex >= cardArraySize )
        m_Cards.erase(std::vector< Card >::iterator( m_Cards.begin() + cardIndex ));
    }

  private:
      std::vector< Card > m_Cards;
};

}

#endif // __PLAYER_HH__
