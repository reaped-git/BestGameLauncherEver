#include "GameState.hpp"
#include <ctime>
#include <sstream>
#include <iomanip>

namespace MineSweeper {

    GameState::GameState()
        : currentStatus(GameStatus::Playing), gameName("Default Game")
    {
        difficultyManager = std::make_unique<DifficultyManager>();
        auto settings = difficultyManager->GetCurrentSettings();
        gameTimer = std::make_shared<GameTimer>(settings.timeLimit);
        AddToHistory("Game created");
    }

    GameState::GameState(const std::string& name)
        : currentStatus(GameStatus::Playing), gameName(name)
    {
        AddToHistory("Game created with name: " + name);
    }

    GameState::GameState(const GameState& other)
        : currentStatus(other.currentStatus),
        gameName(other.gameName + " (Copy)"),
        gameHistory(other.gameHistory)
    {
        AddToHistory("Game copied from: " + other.gameName);
    }

    GameState& GameState::operator=(const GameState& other)
    {
        if (this != &other) {
            currentStatus = other.currentStatus;
            gameName = other.gameName + " (Assigned)";
            gameHistory = other.gameHistory;
            AddToHistory("Game assigned from: " + other.gameName);
        }
        return *this;
    }

    bool GameState::operator==(const GameStatus& status) const
    {
        return currentStatus == status;
    }

    GameState::operator std::string() const
    {
        std::ostringstream oss;
        oss << "GameState: " << gameName
            << ", Status: " << GetStatusDescription(currentStatus)
            << ", History entries: " << gameHistory.size();
        return oss.str();
    }

    std::string GameState::GetCurrentTimeString() const
    {
        std::time_t now = std::time(nullptr);
        std::tm* localTime = std::localtime(&now);

        std::ostringstream oss;
        oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    int GameState::GetCurrentDifficulty()
    {
        return difficultyManager->GetCurrentDifficulty();
    }

    int GameState::GetTimeLeft()
    {
        return gameTimer->GetTimeLeft();
    }

    void GameState::AddToHistory(const std::string& event)
    {
        std::string timeStr = GetCurrentTimeString();
        std::string entry = timeStr + " - " + event;

        gameHistory.push_back(entry);

        // Ограничиваем историю 100 записями
        if (gameHistory.size() > 100) {
            gameHistory.erase(gameHistory.begin());
        }
    }

    std::string GameState::GetFullHistory() const
    {
        std::ostringstream oss;
        oss << "Game History for: " << gameName << "\n";
        oss << std::string(40, '=') << "\n";

        for (const auto& entry : gameHistory) {
            oss << entry << "\n";
        }

        return oss.str();
    }

    std::string GameState::FormatHistoryEntry(const std::string& event) const
    {
        // Конкатенация строк
        std::string prefix = "[HISTORY] ";
        return prefix + event;
    }

    void GameState::SetGameName(const std::string& name)
    {
        if (!name.empty() && name != gameName) {
            std::string oldName = gameName;
            gameName = name;
            AddToHistory("Game renamed from '" + oldName + "' to '" + name + "'");
        }
    }

    void GameState::StartNewGame()
    {
        currentStatus = GameStatus::Playing;
        AddToHistory("New game started");
    }

    void GameState::ProcessCellOpen(bool wasMine)
    {
        if (currentStatus != GameStatus::Playing)
            return;

        if (wasMine)
        {
            AddToHistory("Player stepped on a mine!");
            EndGame(GameStatus::Lost);
        }
        else
        {
            AddToHistory("Cell opened successfully");
        }
    }

    void GameState::CheckGameCompletion(bool hasWon, bool isTimeUp)
    {
        if (currentStatus != GameStatus::Playing)
            return;

        if (isTimeUp)
        {
            EndGame(GameStatus::TimeUp);
        }
        else if (hasWon)
        {
            EndGame(GameStatus::Won);
        }
    }

    void GameState::EndGame(GameStatus status)
    {
        currentStatus = status;
        std::string statusStr = GetStatusDescription(status);
        AddToHistory("Game ended: " + statusStr);
    }

    void GameState::Reset()
    {
        currentStatus = GameStatus::Playing;
        gameHistory.clear();
        AddToHistory("Game reset");
    }

    std::string GameState::GetStatusDescription(GameStatus status)
    {
        switch (status)
        {
        case GameStatus::Won:
            return "Поздравляем! Вы выиграли!";
        case GameStatus::Lost:
            return "Вы наступили на мину! Игра окончена.";
        case GameStatus::TimeUp:
            return "Время вышло! Вы проиграли.";
        case GameStatus::Playing:
            return "Игра в процессе...";
        default:
            return "Неизвестное состояние игры";
        }
    }

    //// Пример использования шаблонной функции в контексте игры
    //std::vector<int> GameState::GetRecentGameTimes(int count) const {
    //    // Возвращает последние N значений времени из истории
    //    std::vector<int> times;
    //    // Реализация получения времен из истории...
    //    return times;
    //}

    //double GameState::CalculateAverageGameTime() const {
    //    auto times = GetRecentGameTimes(10);
    //    if (times.empty()) return 0.0;

    //    auto stats = CalculateStatistics(times);
    //    return stats.average;
    //}
}