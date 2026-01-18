#include "Player.hpp"
#include <stdexcept>
#include <sstream>
#include <iostream>

namespace Blackjack {

    Player::Player(const std::string& playerName, int initialBalance)
        : GameEntity(playerName), balance(initialBalance), currentBet(0), gamesPlayed(0) {
        hand = std::make_unique<Hand>();
    }

    // Конструктор копирования с глубоким клонированием
    Player::Player(const Player& other)
        : GameEntity(other.name + "_copy"),
        balance(other.balance),
        currentBet(other.currentBet),
        gamesPlayed(other.gamesPlayed) {

        // Глубокое копирование руки
        if (other.hand) {
            hand = std::make_unique<Hand>(*other.hand);
        }
        else {
            hand = std::make_unique<Hand>();
        }

        this->id = other.id; 
    }

    Player::~Player() {
        // unique_ptr автоматически удалит Hand
    }

    void Player::displayInfo() const {
        std::cout << "Player: " << this->name
            << " | Balance: $" << this->balance
            << " | Games: " << this->gamesPlayed << "\n";
    }

    std::string Player::getDescription() const {
        // Вызов метода базового класса
        std::string baseDesc = GameEntity::getDescription();
        return baseDesc + " | Type: Player | Balance: $" + std::to_string(balance);
    }

    Player& Player::operator=(const Player& other) {
        if (this != &other) {
            // Глубокое копирование
            this->name = other.name + "_assigned";
            this->balance = other.balance;
            this->currentBet = other.currentBet;
            this->gamesPlayed = other.gamesPlayed;

            if (other.hand) {
                this->hand = std::make_unique<Hand>(*other.hand);
            }
        }
        return *this;
    }

    // Присваивание объекта базового класса
    Player& Player::operator=(const GameEntity& entity) {
        this->name = entity.getName() + "_from_base";
        return *this;
    }

    bool Player::operator>(const Player& other) const {
        return this->balance > other.balance;
    }

    bool Player::canDoubleDown() const {
        return this->balance >= this->currentBet * 2;
    }

    bool Player::canSplit() const {
        // Базовая реализация - в производном классе перегрузим
        return false;
    }

    void Player::placeBet(int amount) {
        // Использование this
        this->validateBet(amount);
        this->currentBet = amount;
        this->balance -= amount;
        this->gamesPlayed++;
    }

    void Player::winBet(int multiplier) {
        int winnings = this->currentBet * multiplier;
        this->balance += winnings;
        this->currentBet = 0;

        // Конкатенация строк
        std::string message = "Player " + this->name + " won $" +
            std::to_string(winnings) +
            " | New balance: $" + std::to_string(this->balance);
        std::cout << message << "\n";
    }

    Hand* Player::getHand() const {
        return hand.get();
    }

    int Player::getBalance() const {
        return this->balance;
    }

    int Player::getCurrentBet() const {
        return this->currentBet;
    }

    int Player::getGamesPlayed() const {
        return this->gamesPlayed;
    }

    void Player::validateBet(int amount) const {
        if (amount <= 0) {
            throw std::invalid_argument("Bet amount must be positive");
        }

        if (amount > this->balance) {
            std::string errorMsg = "Insufficient funds. Balance: $" +
                std::to_string(this->balance) +
                ", Bet: $" + std::to_string(amount);
            throw std::runtime_error(errorMsg);
        }
    }

    std::unique_ptr<GameEntity> Player::clone() const {
        return std::make_unique<Player>(*this);
    }

    // Дружественная функция
    std::string getPlayerStatus(const Player& player) {
        std::stringstream ss;
        ss << "Player Status - Name: " << player.name
            << ", ID: " << player.id
            << ", Balance: $" << player.balance
            << ", Active Bet: $" << player.currentBet;
        return ss.str();
    }
}