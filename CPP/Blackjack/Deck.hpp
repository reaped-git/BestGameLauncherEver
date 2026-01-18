#pragma once

#include "Card.hpp"

namespace Blackjack {

    class Deck {
    private:
        static const int DECK_SIZE = 52;
        Card cards[DECK_SIZE];
        int currentIndex;

        void initialize();

    public:
        Deck();
        void shuffle();
        Card dealCard();
        void reset();
        bool isEmpty() const;
        int getCardsLeft() const;
    };
}