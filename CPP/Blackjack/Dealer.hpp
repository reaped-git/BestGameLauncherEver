#pragma once

#include "GameEntity.hpp"
#include "Hand.hpp"
#include <memory>

namespace Blackjack {

    class Dealer : public GameEntity {
    private:
        std::unique_ptr<Hand> hand;
        bool mustHitSoft17;

    public:
        Dealer(const std::string& dealerName, bool hitSoft17 = true);
        Dealer(const Dealer& other); // Конструктор копирования
        ~Dealer() override;

        // Перегрузка методов (без вызова метода базового класса)
        void displayInfo() const override;

        // Перегрузка виртуальных методов
        bool canDoubleDown() const; 
        virtual bool mustHit() const;

        // Геттеры
        Hand* getHand() const;

        // Перегрузка операторов
        Dealer& operator++(); // Префиксный инкремент
        Dealer operator++(int); // Постфиксный инкремент

        // Для клонирования
        std::unique_ptr<GameEntity> clone() const override;

    private:
        void resetHand();
    };
}