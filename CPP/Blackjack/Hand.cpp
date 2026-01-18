#include "Hand.hpp"
#include <sstream>

namespace Blackjack {

    Hand::Hand() : totalValue(0), aceCount(0) {}

    void Hand::addCard(const Card& card) {
        cards.push_back(card);
        totalValue += card.getValue();

        if (card.getRank() == "Ace") {
            aceCount++;
        }

        while (totalValue > 21 && aceCount > 0) {
            totalValue -= 10;
            aceCount--;
        }
    }

    void Hand::clear() {
        cards.clear();
        totalValue = 0;
        aceCount = 0;
    }

    int Hand::getTotalValue() const {
        return totalValue;
    }

    bool Hand::isBusted() const {
        return totalValue > 21;
    }

    bool Hand::hasBlackjack() const {
        return cards.size() == 2 && totalValue == 21;
    }

    bool Hand::hasAce() const {
        for (const auto& card : cards) {
            if (card.getRank() == "Ace") {
                return true;
            }
        }
        return false;
    }

    int Hand::getCardCount() const {
        return static_cast<int>(cards.size());
    }

    std::vector<Card> Hand::getCards() const {
        return cards;
    }

    std::string Hand::toString(bool hideFirstCard) const {
        if (cards.empty()) {
            return "Empty hand";
        }

        std::stringstream ss;

        if (hideFirstCard) {
            // Скрываем первую карту
            ss << "[Hidden Card]";
            if (cards.size() > 1) {
                ss << ", ";
                for (size_t i = 1; i < cards.size(); i++) {
                    ss << cards[i].toString();
                    if (i < cards.size() - 1) {
                        ss << ", ";
                    }
                }
            }
        }
        else {
            // Показываем все карты
            for (size_t i = 0; i < cards.size(); i++) {
                ss << cards[i].toString();
                if (i < cards.size() - 1) {
                    ss << ", ";
                }
            }
            ss << " (Total: " << totalValue << ")";
        }

        return ss.str();
    }
}