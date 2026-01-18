#pragma once

#include "Deck.hpp"
#include "Hand.hpp"
#include "GameState.hpp"
#include "InputHandler.hpp"
#include "ConsoleUI.hpp"
#include "Player.hpp"
#include "Dealer.hpp"
#include <memory>
#include <string>

namespace Blackjack {

    class BlackjackGame {
    private:
        std::unique_ptr<Deck> deck;
        std::shared_ptr<Dealer> dealer;
        std::shared_ptr<Player> player;
        std::unique_ptr<GameState> gameState;
        std::unique_ptr<InputHandler> inputHandler;
        std::unique_ptr<ConsoleUI> consoleUI;
        bool gameRunning;

        void checkForImmediateBlackjack();
        void handleBettingPhase();
        void handleGamePhase();
        void handleGameOverPhase();
        void processAction(ActionType action);
        void determineWinner();
        void handleDealerTurn();
        void dealInitialCards();
        void resetForNewRound();

    public:
        BlackjackGame(int initialBalance = 1000);

        // Запрещаем копирование
        BlackjackGame(const BlackjackGame&) = delete;
        BlackjackGame& operator=(const BlackjackGame&) = delete;

        void initializeGame();
        void startRound();
        void run();
        void showGameState();
        void checkWinner();
        void quitGame();
        void processInput(char key);

        bool isRunning() const;
        GameState* getGameState();
        Hand* getPlayerHand();
        Hand* getDealerHand();

        void safeBetting(int amount);

    private:
        void initializeEntities();
        void handleException(const std::exception& e);
    };
}