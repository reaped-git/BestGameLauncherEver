#pragma once

#include "Card.hpp"
#include <vector>
#include <string>

namespace Blackjack {

    class Hand {
    private:
        std::vector<Card> cards;
        int totalValue;
        int aceCount;

    public:
        Hand();
        void addCard(const Card& card);
        void clear();
        int getTotalValue() const;
        bool isBusted() const;
        bool hasBlackjack() const;
        bool hasAce() const;
        int getCardCount() const;
        std::vector<Card> getCards() const;
        std::string toString(bool hideFirstCard = false) const;
    };
}
