#include "GameState.hpp"
#include <stdexcept>

namespace Blackjack {

    GameState::GameState(int initialBalance)
        : currentStatus(GameStatus::BETTING), playerScore(0), dealerScore(0),
        isPlayerTurn(true), playerBalance(initialBalance), currentBet(0) {
    }

    void GameState::startNewGame() {
        playerScore = 0;
        dealerScore = 0;
        isPlayerTurn = true;
        currentStatus = GameStatus::BETTING;
    }

    void GameState::updateScores(int player, int dealer) {
        playerScore = player;
        dealerScore = dealer;
    }

    void GameState::switchTurn() {
        if (currentStatus == GameStatus::PLAYER_TURN) {
            currentStatus = GameStatus::DEALER_TURN;
            isPlayerTurn = false;
        }
        else if (currentStatus == GameStatus::DEALER_TURN) {
            currentStatus = GameStatus::GAME_OVER;
            isPlayerTurn = false;
        }
    }

    void GameState::endGame() {
        currentStatus = GameStatus::GAME_OVER;
        isPlayerTurn = false;
    }

    void GameState::reset() {
        playerScore = 0;
        dealerScore = 0;
        currentBet = 0;
        currentStatus = GameStatus::BETTING;
        isPlayerTurn = true;
    }

    GameStatus GameState::getCurrentStatus() const {
        return currentStatus;
    }

    int GameState::getPlayerScore() const {
        return playerScore;
    }

    int GameState::getDealerScore() const {
        return dealerScore;
    }

    bool GameState::isPlayerTurnActive() const {
        return isPlayerTurn;
    }

    int GameState::getPlayerBalance() const {
        return playerBalance;
    }

    int GameState::getCurrentBet() const {
        return currentBet;
    }

    bool GameState::canPlaceBet(int amount) const {
        return amount > 0 && amount <= playerBalance;
    }

    void GameState::placeBet(int amount) {
        if (!canPlaceBet(amount)) {
            throw std::invalid_argument("Invalid bet amount");
        }
        currentBet = amount;
        playerBalance -= amount;
        currentStatus = GameStatus::PLAYER_TURN;
    }

    void GameState::winBet() {
        playerBalance += currentBet * 2;
        currentBet = 0;
    }

    void GameState::loseBet() {
        currentBet = 0;
    }

    void GameState::pushBet() {
        playerBalance += currentBet;
        currentBet = 0;
    }

    void GameState::blackjackWin() {
        playerBalance += static_cast<int>(currentBet * 2.5); // Обычно 3:2
        currentBet = 0;
    }

    bool GameState::isGameOver() const {
        return currentStatus == GameStatus::GAME_OVER;
    }

    bool GameState::isBettingPhase() const {
        return currentStatus == GameStatus::BETTING;
    }

    bool GameState::isPlayerTurnPhase() const {
        return currentStatus == GameStatus::PLAYER_TURN;
    }

    bool GameState::isDealerTurnPhase() const {
        return currentStatus == GameStatus::DEALER_TURN;
    }
}