#pragma once

#include <string>
#include <memory>

namespace Blackjack {

    class Card {
    private:
        std::string suit;
        std::string rank;
        int value;

    public:
        Card(const std::string& suit = "", const std::string& rank = "", int value = 0);

        // Конструктор копирования
        Card(const Card& other);

        // Перегрузка операторов
        Card& operator=(const Card& other);
        bool operator==(const Card& other) const;
        bool operator<(const Card& other) const;

        // Преобразование в строку
        operator std::string() const;

        std::string getSuit() const;
        std::string getRank() const;
        int getValue() const;

        std::string toString() const;

    private:
        std::string normalizeSuit(const std::string& suit) const;
    };
}