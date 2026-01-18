#pragma once

#include "Hand.hpp"
#include "Enums.hpp"
#include <string>

namespace Blackjack {

    // Forward declaration
    class BlackjackGame;

    class ConsoleUI {
    private:
        BlackjackGame* game;

    public:
        ConsoleUI(BlackjackGame* game);

        void showWelcomeMessage();
        void clearScreen();
        void showGameState(const Hand& playerHand, const Hand& dealerHand,
            GameStatus status, int balance, int bet);
        void showBettingPrompt();
        void showAvailableActions();
        void showWinner(int playerScore, int dealerScore);
        void showBalance(int balance);
        void showInvalidInput();
    };
}