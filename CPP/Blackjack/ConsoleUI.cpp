#include "ConsoleUI.hpp"
#include "BlackjackGame.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace Blackjack {

    ConsoleUI::ConsoleUI(BlackjackGame* game) : game(game) {}

    void ConsoleUI::showWelcomeMessage() {
        std::cout << "\033[2J\033[1;1H"; // Clear screen
        std::cout << "=================================\n";
        std::cout << "       BLACKJACK GAME\n";
        std::cout << "=================================\n";
        std::cout << "Welcome to Blackjack!\n";
        std::cout << "\nControls:\n";
        std::cout << "  [H] - Hit\n";
        std::cout << "  [S] - Stand\n";
        std::cout << "  [D] - Deal (new round)\n";
        std::cout << "  [Q] - Quit\n";
        std::cout << "\nPress Enter to continue...\n";
        std::cin.ignore();
    }

    void ConsoleUI::clearScreen() {
        std::cout << "\033[2J\033[1;1H"; // Clear screen
    }

    void ConsoleUI::showGameState(const Hand& playerHand, const Hand& dealerHand,
        GameStatus status, int balance, int bet) {
        clearScreen();
        std::cout << "=================================\n";
        std::cout << "       BLACKJACK TABLE\n";
        std::cout << "=================================\n";
        std::cout << "Balance: $" << balance << " | Current Bet: $" << bet << "\n";

        std::string statusStr;
        switch (status) {
        case GameStatus::BETTING: statusStr = "BETTING"; break;
        case GameStatus::PLAYER_TURN: statusStr = "PLAYER'S TURN"; break;
        case GameStatus::DEALER_TURN: statusStr = "DEALER'S TURN"; break;
        case GameStatus::GAME_OVER: statusStr = "GAME OVER"; break;
        }
        std::cout << "Status: " << statusStr << "\n\n";

        // Всегда показываем полную руку дилера, если игра завершена
        bool hideDealerCard = (status != GameStatus::GAME_OVER &&
            status != GameStatus::DEALER_TURN);

        std::cout << "Dealer's hand: " << dealerHand.toString(hideDealerCard) << "\n";
        std::cout << "Your hand: " << playerHand.toString() << "\n\n";
    }

    void ConsoleUI::showBettingPrompt() {
        std::cout << "Enter your bet amount: ";
    }

    void ConsoleUI::showAvailableActions() {
        std::cout << "\nAvailable actions:\n";
        std::cout << "  [H] - Hit (take another card)\n";
        std::cout << "  [S] - Stand (keep your hand)\n";
        std::cout << "  [Q] - Quit game\n";
    }

    void ConsoleUI::showWinner(int playerScore, int dealerScore) {
        std::cout << "\n=== FINAL RESULT ===\n";
        std::cout << "Your score: " << playerScore << "\n";
        std::cout << "Dealer's score: " << dealerScore << "\n";
        std::cout << "====================\n";
    }

    void ConsoleUI::showBalance(int balance) {
        std::cout << "\nYour current balance: $" << balance << "\n";
    }

    void ConsoleUI::showInvalidInput() {
        std::cout << "Invalid input! Please try again.\n";
    }
}