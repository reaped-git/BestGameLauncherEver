#pragma once

#include "Enums.hpp"

namespace Blackjack {

    class GameState {
    private:
        GameStatus currentStatus;
        int playerScore;
        int dealerScore;
        bool isPlayerTurn;
        int playerBalance;
        int currentBet;

    public:
        GameState(int initialBalance = 1000);

        void startNewGame();
        void updateScores(int player, int dealer);
        void switchTurn();
        void endGame();
        void reset();

        // Getters
        GameStatus getCurrentStatus() const;
        int getPlayerScore() const;
        int getDealerScore() const;
        bool isPlayerTurnActive() const;
        int getPlayerBalance() const;
        int getCurrentBet() const;

        // Betting methods
        bool canPlaceBet(int amount) const;
        void placeBet(int amount);
        void winBet();
        void loseBet();
        void pushBet();
        void blackjackWin();

        // State checks
        bool isGameOver() const;
        bool isBettingPhase() const;
        bool isPlayerTurnPhase() const;
        bool isDealerTurnPhase() const;
    };
}