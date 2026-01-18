#include "GameStatistics.hpp"
#include <sstream>
#include <algorithm>
#include <numeric>
#include <iostream>

namespace MineSweeper {

    // ==================== BaseStatistics ====================

    BaseStatistics::BaseStatistics(const std::string& name)
        : playerName(name), gamesPlayed(0)
    {
        std::cout << "BaseStatistics constructor for " << name << "\n";
    }

    // Конструктор копирования для BaseStatistics
    BaseStatistics::BaseStatistics(const BaseStatistics& other)
        : playerName(other.playerName + " (Copied)"), gamesPlayed(other.gamesPlayed)
    {
        std::cout << "BaseStatistics copy constructor\n";
    }

    BaseStatistics::~BaseStatistics()
    {
        std::cout << "BaseStatistics destructor for " << playerName << "\n";
    }

    BaseStatistics& BaseStatistics::operator=(const BaseStatistics& other)
    {
        if (this != &other) {
            playerName = other.playerName + " (Assigned)";
            gamesPlayed = other.gamesPlayed;
        }
        return *this;
    }

    void BaseStatistics::AddGameResult(bool won)
    {
        gamesPlayed++;
        std::cout << "BaseStatistics: Game added. Total: " << gamesPlayed << "\n";
    }

    std::string BaseStatistics::GetSummary() const
    {
        std::ostringstream oss;
        oss << "Player: " << playerName
            << ", Games: " << gamesPlayed;
        return oss.str();
    }

    BaseStatistics* BaseStatistics::Clone() const
    {
        // Поверхностное клонирование
        return new BaseStatistics(*this);
    }

    BaseStatistics& BaseStatistics::operator+=(const BaseStatistics& other)
    {
        gamesPlayed += other.gamesPlayed;
        return *this;
    }

    bool BaseStatistics::operator>(const BaseStatistics& other) const
    {
        return gamesPlayed > other.gamesPlayed;
    }

    // ==================== ExtendedStatistics ====================

    ExtendedStatistics::ExtendedStatistics(const std::string& name)
        : BaseStatistics(name), gamesWon(0), bestTime(0), gameResults(new std::vector<bool>())
    {
        std::cout << "ExtendedStatistics constructor for " << name << "\n";
    }

    ExtendedStatistics::ExtendedStatistics(const BaseStatistics& base)
        : BaseStatistics(base), // Вызов конструктора базового класса с параметрами
        gamesWon(0), bestTime(0), gameResults(new std::vector<bool>())
    {
        playerName = playerName + " (Extended)";
        std::cout << "ExtendedStatistics constructor from BaseStatistics\n";
    }

    ExtendedStatistics::ExtendedStatistics(const ExtendedStatistics& other)
        : BaseStatistics(other), // Вызов конструктора копирования базового класса
        gamesWon(other.gamesWon),
        bestTime(other.bestTime),
        gameTimes(other.gameTimes),
        gameResults(new std::vector<bool>(*other.gameResults)) // Глубокое копирование
    {
        std::cout << "ExtendedStatistics deep copy constructor\n";
    }

    ExtendedStatistics::~ExtendedStatistics()
    {
        delete gameResults;
        std::cout << "ExtendedStatistics destructor for " << playerName << "\n";
    }

    ExtendedStatistics& ExtendedStatistics::operator=(const BaseStatistics& base)
    {
        // Вызов оператора присваивания базового класса
        BaseStatistics::operator=(base);
        gamesWon = 0;
        bestTime = 0;
        gameTimes.clear();
        if (gameResults) {
            gameResults->clear();
        }
        return *this;
    }

    ExtendedStatistics& ExtendedStatistics::operator=(const ExtendedStatistics& other)
    {
        if (this != &other) {
            BaseStatistics::operator=(other);
            gamesWon = other.gamesWon;
            bestTime = other.bestTime;
            gameTimes = other.gameTimes;
            if (gameResults) {
                delete gameResults;
            }
            gameResults = new std::vector<bool>(*other.gameResults); // Глубокое копирование
        }
        return *this;
    }

    // Перегрузка без вызова базового метода
    void ExtendedStatistics::AddGameResult(bool won)
    {
        gamesPlayed++;
        if (won) {
            gamesWon++;
        }
        if (gameResults) {
            gameResults->push_back(won);
        }
        std::cout << "ExtendedStatistics: Game added with win=" << won
            << ". Win rate: " << GetWinRate() * 100 << "%\n";
    }

    // Перегрузка с вызовом базового метода
    void ExtendedStatistics::AddGameResult(bool won, int time)
    {
        BaseStatistics::AddGameResult(won); // Вызов метода базового класса
        if (won) {
            gamesWon++;
            if (time > 0) {
                gameTimes.push_back(time);
                UpdateBestTime(time);
            }
        }
        if (gameResults) {
            gameResults->push_back(won);
        }
    }

    std::string ExtendedStatistics::GetSummary() const
    {
        std::string baseSummary = BaseStatistics::GetSummary(); // Вызов базового метода
        std::ostringstream oss;
        oss << baseSummary
            << ", Won: " << gamesWon
            << ", Win Rate: " << GetWinRate() * 100 << "%"
            << ", Best Time: " << bestTime << "s";
        return oss.str();
    }

    ExtendedStatistics* ExtendedStatistics::Clone() const
    {
        // Глубокое клонирование
        return new ExtendedStatistics(*this);
    }

    ExtendedStatistics::operator std::string() const
    {
        return GetSummary();
    }

    double ExtendedStatistics::GetWinRate() const
    {
        return gamesPlayed > 0 ? static_cast<double>(gamesWon) / gamesPlayed : 0.0;
    }

    std::string ExtendedStatistics::GetDetailedReport() const
    {
        std::ostringstream oss;
        oss << "=== Detailed Report ===\n"
            << GetSummary() << "\n"
            << "Total games: " << gamesPlayed << "\n"
            << "Games won: " << gamesWon << "\n";
        if (gameResults) {
            oss << "Game results recorded: " << gameResults->size() << "\n";
        }
        return oss.str();
    }

    void ExtendedStatistics::UpdateBestTime(int time)
    {
        if (bestTime == 0 || time < bestTime) {
            bestTime = time;
        }
    }

    // ==================== TimedStatistics ====================

    TimedStatistics::TimedStatistics(const std::string& name)
        : ExtendedStatistics(name), averageTime(0.0)
    {
        std::cout << "TimedStatistics constructor for " << name << "\n";
    }

    TimedStatistics::TimedStatistics(const ExtendedStatistics& base)
        : ExtendedStatistics(base), averageTime(0.0) // Вызов конструктора базового класса
    {
        playerName = playerName + " (Timed)";
        std::cout << "TimedStatistics constructor from ExtendedStatistics\n";
    }

    TimedStatistics::TimedStatistics(const TimedStatistics& other)
        : ExtendedStatistics(other), // Вызов конструктора копирования базового класса
        gameDates(other.gameDates),
        averageTime(other.averageTime)
    {
        std::cout << "TimedStatistics copy constructor\n";
    }

    void TimedStatistics::AddGameResult(bool won, int time, const std::string& date)
    {
        // Вызов метода базового класса
        ExtendedStatistics::AddGameResult(won, time);
        gameDates.push_back(date);
        CalculateAverageTime();
    }

    // Перегрузка без вызова базового метода
    std::string TimedStatistics::GetSummary() const
    {
        std::ostringstream oss;
        oss << "Timed Statistics for " << playerName << "\n"
            << "Games: " << gamesPlayed
            << ", Won: " << gamesWon
            << ", Avg Time: " << averageTime << "s";
        return oss.str();
    }

    // Перегрузка с вызовом базового метода
    std::string TimedStatistics::GetDetailedReport() const
    {
        // Вызов метода базового класса
        std::string baseReport = ExtendedStatistics::GetDetailedReport();
        std::ostringstream oss;
        oss << baseReport
            << "\n=== Time Analysis ===\n"
            << "Average time: " << averageTime << "s\n"
            << "Game dates recorded: " << gameDates.size() << "\n"
            << "Timeline available: " << (!gameDates.empty() ? "Yes" : "No");
        return oss.str();
    }

    std::string TimedStatistics::GetTimeline() const
    {
        std::ostringstream oss;
        oss << "=== Game Timeline ===\n";
        for (size_t i = 0; i < gameDates.size() && i < 5; ++i) {
            oss << "Game " << (i + 1) << ": " << gameDates[i] << "\n";
        }
        if (gameDates.size() > 5) {
            oss << "... and " << (gameDates.size() - 5) << " more games\n";
        }
        return oss.str();
    }

    void TimedStatistics::CalculateAverageTime()
    {
        if (!gameTimes.empty()) {
            double sum = std::accumulate(gameTimes.begin(), gameTimes.end(), 0.0);
            averageTime = sum / gameTimes.size();
        }
    }
}