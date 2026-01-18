#pragma once

#include <string>
#include <memory>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "GameTimer.hpp"
#include "DifficultyManager.hpp"

namespace MineSweeper {

    /// <summary>
    /// Класс для управления состоянием игры "Сапёр"
    /// </summary>
    class GameState
    {
    public:
        enum class GameStatus
        {
            Playing = 0,
            Won = 1,
            Lost = 2,
            TimeUp = 3
        };

    private:
        GameStatus currentStatus;
        std::string gameName;
        std::vector<std::string> gameHistory;
        std::shared_ptr<GameTimer> gameTimer;
        std::unique_ptr<DifficultyManager> difficultyManager;


    public:
        GameState();
        GameState(const std::string& name);
        GameState(const GameState& other); // Конструктор копирования

        ~GameState() = default;

        // Перегрузка операторов
        GameState& operator=(const GameState& other);
        bool operator==(const GameStatus& status) const;
        operator std::string() const;

        std::vector<int> GetRecentGameTimes(int count);
        double CalculateAverageGameTime() const;

        // Остальные методы
        void StartNewGame();
        void ProcessCellOpen(bool wasMine);
        void CheckGameCompletion(bool hasWon, bool isTimeUp);
        void EndGame(GameStatus status);
        void Reset();

        GameStatus GetCurrentStatus() const { return currentStatus; }
        bool IsGameActive() const { return currentStatus == GameStatus::Playing; }

        // Работа со строками
        void AddToHistory(const std::string& event);
        std::string GetFullHistory() const;
        void SetGameName(const std::string& name);
        std::string GetGameName() const { return gameName; }

        static std::string GetStatusDescription(GameStatus status);
        int GetCurrentDifficulty();
        int GetTimeLeft();

    private:
        std::string FormatHistoryEntry(const std::string& event) const;
        std::string GetCurrentTimeString() const;
    };
}