#pragma once
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace MineSweeper {

    /// <summary>
    /// Базовый класс для статистики
    /// </summary>
    class BaseStatistics
    {
    protected:
        std::string playerName;
        int gamesPlayed;

    public:
        BaseStatistics(const std::string& name = "Player");
        BaseStatistics(const BaseStatistics& other); // Восстанавливаем конструктор копирования
        virtual ~BaseStatistics(); // Виртуальный деструктор

        BaseStatistics& operator=(const BaseStatistics& other);

        // Виртуальные методы
        virtual void AddGameResult(bool won);
        virtual std::string GetSummary() const;
        virtual BaseStatistics* Clone() const; // Виртуальный метод для клонирования

        // Перегрузка операторов
        BaseStatistics& operator+=(const BaseStatistics& other);
        bool operator>(const BaseStatistics& other) const;

        std::string GetPlayerName() const { return playerName; }
        int GetGamesPlayed() const { return gamesPlayed; }

        // Невиртуальная функция, которая вызывает виртуальную
        void DisplayStatistics() const {
            std::cout << "Displaying statistics:\n";
            std::cout << GetSummary() << "\n"; // Вызов виртуальной функции
        }
    };

    /// <summary>
    /// Дочерний класс для расширенной статистики
    /// </summary>
    class ExtendedStatistics : public BaseStatistics
    {
    protected:
        int gamesWon;
        int bestTime;
        std::vector<int> gameTimes;
        std::vector<bool>* gameResults; // Для демонстрации глубокого клонирования

    public:
        ExtendedStatistics(const std::string& name = "Player");
        ExtendedStatistics(const BaseStatistics& base); // Конструктор с вызовом базового
        ExtendedStatistics(const ExtendedStatistics& other); // Конструктор глубокого копирования
        ~ExtendedStatistics() override;

        ExtendedStatistics& operator=(const BaseStatistics& base); // Перегрузка оператора присваивания
        ExtendedStatistics& operator=(const ExtendedStatistics& other);

        // Перегрузка виртуальных методов (с вызовом базового и без)
        void AddGameResult(bool won) override; // Без вызова базового
        void AddGameResult(bool won, int time); // С вызовом базового
        std::string GetSummary() const override;
        ExtendedStatistics* Clone() const override; // Перегрузка метода клонирования

        // Перегрузка оператора преобразования
        operator std::string() const;

        double GetWinRate() const;
        int GetBestTime() const { return bestTime; }

        // Виртуальная функция для демонстрации
        virtual std::string GetDetailedReport() const;

    private:
        void UpdateBestTime(int time);
    };

    /// <summary>
    /// Производный класс для статистики с временными метками
    /// </summary>
    class TimedStatistics : public ExtendedStatistics
    {
    private:
        std::vector<std::string> gameDates;
        double averageTime;

    public:
        TimedStatistics(const std::string& name = "Player");
        TimedStatistics(const ExtendedStatistics& base);
        TimedStatistics(const TimedStatistics& other); // Конструктор копирования

        void AddGameResult(bool won, int time, const std::string& date);
        std::string GetSummary() const override; // Перегрузка без вызова базового
        std::string GetDetailedReport() const override; // Перегрузка с вызовом базового

        // Новый виртуальный метод
        virtual std::string GetTimeline() const;

    private:
        void CalculateAverageTime();
    };

    /// <summary>
    /// Абстрактный интерфейс для игровых объектов с именем
    /// </summary>
    class INamedObject {
    public:
        virtual ~INamedObject() = default;
        virtual std::string GetName() const = 0;
        virtual int GetScore() const = 0; // Виртуальный метод для сортировки
    };

    /// <summary>
    /// Адаптер для BaseStatistics для реализации интерфейса INamedObject
    /// </summary>
    class StatisticsAdapter : public BaseStatistics, public INamedObject {
    public:
        StatisticsAdapter(const std::string& name = "Player") : BaseStatistics(name) {}

        std::string GetName() const override {
            return GetPlayerName();
        }

        int GetScore() const override {
            return GetGamesPlayed();
        }
    };

    /// <summary>
    /// Адаптер для ExtendedStatistics
    /// </summary>
    class ExtendedStatisticsAdapter : public ExtendedStatistics, public INamedObject {
    public:
        ExtendedStatisticsAdapter(const std::string& name = "Player") : ExtendedStatistics(name) {}

        std::string GetName() const override {
            return GetPlayerName();
        }

        int GetScore() const override {
            return GetGamesPlayed() * 100 + GetBestTime(); // Композитный скоринг
        }
    };

    /// <summary>
    /// Адаптер для TimedStatistics
    /// </summary>
    class TimedStatisticsAdapter : public TimedStatistics, public INamedObject {
    public:
        TimedStatisticsAdapter(const std::string& name = "Player") : TimedStatistics(name) {}

        std::string GetName() const override {
            return GetPlayerName();
        }

        int GetScore() const override {
            return GetGamesPlayed() * 1000 + static_cast<int>(GetWinRate() * 100);
        }
    };
}