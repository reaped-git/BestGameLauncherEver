#include "BlackjackGame.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>
#include <limits>
#include <stdexcept>
#include <vector>
#include <memory>

namespace Blackjack {

    BlackjackGame::BlackjackGame(int initialBalance)
        : gameRunning(false) {

        // Использование умных указателей
        deck = std::make_unique<Deck>();
        dealer = std::make_shared<Dealer>("Dealer", true);
        player = std::make_shared<Player>("Player", initialBalance);
        gameState = std::make_unique<GameState>(initialBalance);
        inputHandler = std::make_unique<InputHandler>();
        consoleUI = std::make_unique<ConsoleUI>(this);

        deck->shuffle();
    }

    void BlackjackGame::initializeGame() {
        gameRunning = true;
        gameState->startNewGame();
        consoleUI->showWelcomeMessage();
    }

    void BlackjackGame::startRound() {
        if (gameState->isBettingPhase()) {
            consoleUI->showGameState(*player->getHand(), *dealer->getHand(),
                gameState->getCurrentStatus(),
                gameState->getPlayerBalance(),
                gameState->getCurrentBet());
            consoleUI->showBettingPrompt();
        }
        else {
            resetForNewRound();
            dealInitialCards();
            checkForImmediateBlackjack();
        }
    }

    void BlackjackGame::checkForImmediateBlackjack() {
        if (player->getHand()->hasBlackjack()) {
            if (dealer->getHand()->hasBlackjack()) {
                gameState->pushBet();
                gameState->endGame();
                consoleUI->showGameState(*player->getHand(), *dealer->getHand(),
                    gameState->getCurrentStatus(),
                    gameState->getPlayerBalance(),
                    gameState->getCurrentBet());
                std::cout << "Both have Blackjack! Push.\n";
            }
            else {
                gameState->blackjackWin();
                gameState->endGame();
                consoleUI->showGameState(*player->getHand(), *dealer->getHand(),
                    gameState->getCurrentStatus(),
                    gameState->getPlayerBalance(),
                    gameState->getCurrentBet());
                std::cout << "BLACKJACK! You win 3:2!\n";
            }
        }
        else if (dealer->getHand()->hasBlackjack()) {
            gameState->loseBet();
            gameState->endGame();
            consoleUI->showGameState(*player->getHand(), *dealer->getHand(),
                gameState->getCurrentStatus(),
                gameState->getPlayerBalance(),
                gameState->getCurrentBet());
            std::cout << "Dealer has Blackjack! You lose.\n";
        }
    }

    void BlackjackGame::run() {
        initializeGame();

        while (gameRunning) {
            showGameState();

            if (gameState->isBettingPhase()) {
                handleBettingPhase();
            }
            else if (!gameState->isGameOver()) {
                handleGamePhase();
            }
            else {
                handleGameOverPhase();
            }
        }
    }

    void BlackjackGame::handleBettingPhase() {
        try {
            std::string input;
            std::getline(std::cin, input);

            // Проверка на пустой ввод
            if (input.empty()) {
                consoleUI->showInvalidInput();
                return;
            }

            std::stringstream ss(input);
            int bet;
            if (ss >> bet && ss.eof()) {
                safeBetting(bet);
                startRound();
            }
            else {
                consoleUI->showInvalidInput();
            }
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
            consoleUI->showInvalidInput();
        }
        catch (...) {
            consoleUI->showInvalidInput();
        }
    }

    void BlackjackGame::handleGamePhase() {
        char input;
        std::cin.get(input);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        processInput(input);
        checkWinner();
    }

    void BlackjackGame::handleGameOverPhase() {
        std::cout << "\nGame over! Press [D] to deal again or [Q] to quit.\n";
        char input;
        std::cin.get(input);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        processInput(input);
    }

    void BlackjackGame::showGameState() {
        consoleUI->showGameState(*player->getHand(), *dealer->getHand(),
            gameState->getCurrentStatus(),
            gameState->getPlayerBalance(),
            gameState->getCurrentBet());

        if (!gameState->isBettingPhase() && !gameState->isGameOver()) {
            consoleUI->showAvailableActions();
        }
    }

    void BlackjackGame::checkWinner() {
        if (gameState->isGameOver() && !player->getHand()->hasBlackjack() && !dealer->getHand()->hasBlackjack()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(800));
            determineWinner();
        }
    }

    void BlackjackGame::quitGame() {
        gameRunning = false;
        std::cout << "\nThanks for playing!\n";
    }

    void BlackjackGame::processAction(ActionType action) {
        switch (action) {
        case ActionType::HIT:
            if (gameState->isPlayerTurnPhase()) {
                player->getHand()->addCard(deck->dealCard());
                gameState->updateScores(player->getHand()->getTotalValue(),
                    dealer->getHand()->getTotalValue());

                if (player->getHand()->isBusted()) {
                    gameState->loseBet();
                    gameState->endGame();
                }
            }
            break;

        case ActionType::STAND:
            if (gameState->isPlayerTurnPhase()) {
                gameState->switchTurn();
                handleDealerTurn();
            }
            break;

        case ActionType::DEAL:
            if (gameState->isGameOver() || gameState->isBettingPhase()) {
                if (gameState->getPlayerBalance() <= 0) {
                    std::cout << "You're out of money! Game over.\n";
                    quitGame();
                    return;
                }

                gameState->reset();
                resetForNewRound();
                startRound();
            }
            break;

        case ActionType::QUIT:
            quitGame();
            break;

        case ActionType::INVALID:
            consoleUI->showInvalidInput();
            break;

        default:
            break;
        }
    }

    void BlackjackGame::determineWinner() {
        int playerScore = player->getHand()->getTotalValue();
        int dealerScore = dealer->getHand()->getTotalValue();

        consoleUI->clearScreen();
        consoleUI->showGameState(*player->getHand(), *dealer->getHand(),
            gameState->getCurrentStatus(),
            gameState->getPlayerBalance(),
            gameState->getCurrentBet());

        std::cout << "\n=== FINAL RESULT ===\n";
        std::cout << "Your score: " << playerScore << "\n";
        std::cout << "Dealer's score: " << dealerScore << "\n";
        std::cout << "====================\n\n";

        if (!player->getHand()->hasBlackjack() && !dealer->getHand()->hasBlackjack()) {
            if (playerScore > 21) {
                gameState->loseBet();
                std::cout << "You busted. Bet lost.\n";
            }
            else if (dealerScore > 21) {
                gameState->winBet();
                std::cout << "Dealer busted. You win!\n";
            }
            else if (playerScore > dealerScore) {
                gameState->winBet();
                std::cout << "You win!\n";
            }
            else if (playerScore < dealerScore) {
                gameState->loseBet();
                std::cout << "Dealer wins.\n";
            }
            else {
                gameState->pushBet();
                std::cout << "Push. Bet returned.\n";
            }
        }

        consoleUI->showBalance(gameState->getPlayerBalance());
    }

    void BlackjackGame::handleDealerTurn() {
        while (dealer->getHand()->getTotalValue() < 17 && !dealer->getHand()->isBusted()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(800));
            dealer->getHand()->addCard(deck->dealCard());
            gameState->updateScores(player->getHand()->getTotalValue(),
                dealer->getHand()->getTotalValue());

            consoleUI->clearScreen();
            consoleUI->showGameState(*player->getHand(), *dealer->getHand(),
                gameState->getCurrentStatus(),
                gameState->getPlayerBalance(),
                gameState->getCurrentBet());
            std::cout << "Dealer takes a card...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(800));
        }

        gameState->endGame();
    }

    void BlackjackGame::processInput(char key) {
        ActionType action = inputHandler->processKey(key);
        processAction(action);
    }

    bool BlackjackGame::isRunning() const {
        return gameRunning;
    }

    GameState* BlackjackGame::getGameState() {
        return gameState.get();
    }

    Hand* BlackjackGame::getPlayerHand() {
        return player->getHand();
    }

    Hand* BlackjackGame::getDealerHand() {
        return dealer->getHand();
    }

    void BlackjackGame::dealInitialCards() {
        player->getHand()->clear();
        dealer->getHand()->clear();

        player->getHand()->addCard(deck->dealCard());
        dealer->getHand()->addCard(deck->dealCard());
        player->getHand()->addCard(deck->dealCard());
        dealer->getHand()->addCard(deck->dealCard());

        gameState->updateScores(player->getHand()->getTotalValue(),
            dealer->getHand()->getTotalValue());
    }

    void BlackjackGame::resetForNewRound() {
        player->getHand()->clear();
        dealer->getHand()->clear();

        if (deck->getCardsLeft() < 15) {
            deck->reset();
            std::cout << "Deck reshuffled.\n";
        }
    }

    void BlackjackGame::safeBetting(int amount) {
        try {
            // Защищенный блок
            if (amount <= 0) {
                throw std::invalid_argument("Bet amount must be positive");
            }

            if (amount > player->getBalance()) {
                std::string errorMsg = "Cannot bet $" + std::to_string(amount) +
                    ". Balance: $" + std::to_string(player->getBalance());
                throw std::runtime_error(errorMsg);
            }

            player->placeBet(amount);
            gameState->placeBet(amount);
            std::cout << "Bet placed successfully: $" << amount << "\n";

        }
        catch (const std::invalid_argument& e) {
            std::cout << "Invalid argument: " << e.what() << "\n";
            throw; // Пробрасываем исключение дальше
        }
        catch (const std::runtime_error& e) {
            std::cout << "Runtime error: " << e.what() << "\n";
            consoleUI->showInvalidInput();
        }
        catch (const std::exception& e) {
            std::cout << "Exception occurred: " << e.what() << "\n";
            handleException(e);
        }
        catch (...) {
            std::cout << "Unknown error occurred during betting\n";
        }
    }


    void BlackjackGame::handleException(const std::exception& e) {
        // Обработка исключений с логированием
        std::string errorLog = "Exception handled: " + std::string(e.what()) +
            " | Game state: " + std::to_string(gameRunning);
        std::cout << errorLog << "\n";
    }
}