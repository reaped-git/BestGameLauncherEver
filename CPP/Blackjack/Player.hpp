#pragma once

#include "GameEntity.hpp"
#include "Hand.hpp"
#include <memory>
#include <vector>

namespace Blackjack {

    class Player : public GameEntity {
    private:
        std::unique_ptr<Hand> hand;
        int balance;
        int currentBet;

    protected:
        int gamesPlayed;

    public:
        Player(const std::string& playerName, int initialBalance);
        Player(const Player& other); // Конструктор копирования
        ~Player() override;

        // Перегрузка методов базового класса
        void displayInfo() const override;
        std::string getDescription() const override;

        // Перегрузка операторов
        Player& operator=(const Player& other);
        Player& operator=(const GameEntity& entity); // Присваивание объекта базового класса
        bool operator>(const Player& other) const;

        // Виртуальные функции
        virtual bool canDoubleDown() const;
        virtual bool canSplit() const;

        void placeBet(int amount);
        void winBet(int multiplier = 2);

        // Геттеры/Сеттеры
        Hand* getHand() const;
        int getBalance() const;
        int getCurrentBet() const;
        int getGamesPlayed() const;

        // Для клонирования
        std::unique_ptr<GameEntity> clone() const override;

        // Дружественная функция
        friend std::string getPlayerStatus(const Player& player);

    private:
        void validateBet(int amount) const;
    };
}