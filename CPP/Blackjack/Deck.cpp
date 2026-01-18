#include "Deck.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

namespace Blackjack {

    Deck::Deck() : currentIndex(0) {
        std::srand(static_cast<unsigned>(time(0)));
        initialize();
    }

    void Deck::initialize() {
        std::string suits[] = { "Hearts", "Diamonds", "Clubs", "Spades" };
        std::string ranks[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10",
                              "Jack", "Queen", "King", "Ace" };

        int index = 0;
        for (const auto& suit : suits) {
            for (const auto& rank : ranks) {
                int value = 0;
                if (rank == "Jack" || rank == "Queen" || rank == "King") {
                    value = 10;
                }
                else if (rank == "Ace") {
                    value = 11;
                }
                else {
                    value = std::stoi(rank);
                }

                cards[index] = Card(suit, rank, value);
                index++;
            }
        }
        currentIndex = 0;
    }

    void Deck::shuffle() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(std::begin(cards), std::end(cards), g);
        currentIndex = 0;
    }

    Card Deck::dealCard() {
        if (currentIndex >= DECK_SIZE) {
            reset();
        }
        return cards[currentIndex++];
    }

    void Deck::reset() {
        currentIndex = 0;
        shuffle();
    }

    bool Deck::isEmpty() const {
        return currentIndex >= DECK_SIZE;
    }

    int Deck::getCardsLeft() const {
        return DECK_SIZE - currentIndex;
    }
}