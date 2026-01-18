#include "Card.hpp"
#include <algorithm>
#include <cctype>

namespace Blackjack {

    Card::Card(const std::string& suit, const std::string& rank, int value)
        : suit(normalizeSuit(suit)), rank(rank), value(value) {
    }

    // Конструктор копирования
    Card::Card(const Card& other)
        : suit(other.suit), rank(other.rank), value(other.value) {
    }

    // Перегрузка оператора присваивания
    Card& Card::operator=(const Card& other) {
        if (this != &other) {
            suit = other.suit;
            rank = other.rank;
            value = other.value;
        }
        return *this;
    }

    bool Card::operator==(const Card& other) const {
        return (suit == other.suit) && (rank == other.rank);
    }

    bool Card::operator<(const Card& other) const {
        // Сортировка по масти, затем по значению
        if (suit != other.suit) {
            return suit < other.suit;
        }
        return value < other.value;
    }

    // Оператор преобразования
    Card::operator std::string() const {
        return toString();
    }

    std::string Card::getSuit() const {
        return suit;
    }

    std::string Card::getRank() const {
        return rank;
    }

    int Card::getValue() const {
        return value;
    }

    std::string Card::toString() const {
        // Конкатенация строк с использованием std::string
        std::string result = rank;
        result += " of ";
        result += suit;

        // Поиск подстроки
        if (result.find("Ace") != std::string::npos) {
            result += " (value: 1 or 11)";
        }

        return result;
    }

    std::string Card::normalizeSuit(const std::string& suit) const {
        std::string normalized = suit;
        std::transform(normalized.begin(), normalized.end(), normalized.begin(),
            [](unsigned char c) { return std::tolower(c); });

        // Замена сокращений
        if (normalized == "h") return "Hearts";
        if (normalized == "d") return "Diamonds";
        if (normalized == "c") return "Clubs";
        if (normalized == "s") return "Spades";

        return suit;
    }
}