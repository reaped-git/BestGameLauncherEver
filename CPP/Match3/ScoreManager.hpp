#pragma once

#include <array>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <any>
#include "GameAlgorithms.hpp"
#include "GameDataContainer.hpp"
#include "BonusStrategies.hpp"

#ifndef GAME_BONUS_MODE
#define GAME_BONUS_MODE NORMAL
#endif

namespace Match3 {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Collections::Generic;

    /// <summary>
    /// Класс для управления счетом игры
    /// Отвечает за хранение, обновление и отображение счета
    /// </summary>
    class ScoreManager
    {
    private:
        static const Int64 SCORE_PER_TILE = 10;
        std::array<std::pair<Int64, Int64>, 3> bonuses;
        Int64 currentScore;       // Текущее количество очков
        std::string playerName;
        std::shared_ptr<IBonusStrategy> bonusStrategy;

        // Хранилище для динамической конфигурации
        std::unordered_map<std::string, std::any> runtimeConfig;

        // Новые поля для хранения истории
        PlayerScoreContainer scoreHistory;
        std::vector<Int64> recentScores;

    public:
        /// <summary>
        /// Создает стратегию на основе compile-time макросов
        /// </summary>
        std::shared_ptr<IBonusStrategy> CreateStaticConfiguredStrategy() {
            // Используем макросы, определенные при компиляции
#if GAME_BONUS_MODE == AGGRESSIVE
            return std::make_shared<ConfigurableRuntimeStrategy>(2.0, 2, true, 2000);
#elif GAME_BONUS_MODE == CONSERVATIVE
            return std::make_shared<ConfigurableRuntimeStrategy>(1.2, 5, false, 500);
#elif GAME_BONUS_MODE == BALANCED
            return std::make_shared<ConfigurableRuntimeStrategy>(1.5, 3, false, 1000);
#else // NORMAL
            return std::make_shared<ClassicBonusStrategy>();
#endif
        }

        /// <summary>
        /// Использует шаблонную стратегию с compile-time параметрами
        /// </summary>
        template <size_t BufferSize = 1024, bool UseSIMD = true>
        void UseTemplateConfiguredStrategy() {
            auto strategy = std::make_shared<OptimizedBonusStrategy<BufferSize, UseSIMD>>();
            SetBonusStrategy(strategy, "template-configured");
        }

        /// <summary>
        /// Использует конфигурацию через шаблонные параметры
        /// </summary>
        template <int BaseScore = 10, int MinCombo = 3, bool Progressive = true>
        void UseConfigurableTemplateStrategy() {
            auto strategy = std::make_shared<ConfigurableBonusStrategy<BaseScore, MinCombo, Progressive>>();
            SetBonusStrategy(strategy, "configurable-template");
        }

        // ==================== ДИНАМИЧЕСКАЯ КОНФИГУРАЦИЯ ====================

        /// <summary>
        /// Конфигурирует стратегию из аргументов командной строки
        /// </summary>
        bool ConfigureFromCommandLine(int argc, char* argv[]) {
            std::string mode = "normal";
            int difficulty = 1;

            // Парсинг аргументов командной строки
            for (int i = 1; i < argc; ++i) {
                std::string arg = argv[i];
                if (arg == "--bonus-mode") {
                    if (i + 1 < argc) mode = argv[++i];
                }
                else if (arg == "--difficulty") {
                    if (i + 1 < argc) difficulty = std::stoi(argv[++i]);
                }
            }

            auto strategy = std::make_shared<CommandLineStrategy>(mode, difficulty);
            SetBonusStrategy(strategy, "cmdline-configured");
            return true;
        }

        /// <summary>
        /// Конфигурирует стратегию из JSON-файла (упрощенная версия)
        /// </summary>
        bool ConfigureFromJson(const std::string& jsonConfig) {
            // Парсинг упрощенного JSON
            double multiplier = 1.5;
            int minTiles = 3;
            bool exponential = false;
            int maxBonus = 1000;

            // Простой парсинг (в реальности использовать библиотеку JSON)
            if (jsonConfig.find("\"multiplier\":") != std::string::npos) {
                // ... парсинг значений ...
            }

            auto strategy = std::make_shared<ConfigurableRuntimeStrategy>(multiplier, minTiles, exponential, maxBonus);
            SetBonusStrategy(strategy, "json-configured");
            return true;
        }

        /// <summary>
        /// Конфигурирует стратегию на основе игровых условий
        /// </summary>
        void ConfigureAdaptiveStrategy(double initialMultiplier = 1.5) {
            auto strategy = std::make_shared<AdaptiveStrategy>(initialMultiplier);
            SetBonusStrategy(strategy, "adaptive");
        }

        /// <summary>
        /// Устанавливает параметры конфигурации во время выполнения
        /// </summary>
        template<typename T>
        void SetRuntimeConfig(const std::string& key, const T& value) {
            runtimeConfig[key] = value;
            ApplyRuntimeConfig();
        }

        /// <summary>
        /// Получает параметр конфигурации
        /// </summary>
        template<typename T>
        T GetRuntimeConfig(const std::string& key, const T& defaultValue) const {
            auto it = runtimeConfig.find(key);
            if (it != runtimeConfig.end()) {
                try {
                    return std::any_cast<T>(it->second);
                }
                catch (...) {
                    return defaultValue;
                }
            }
            return defaultValue;
        }

        /// <summary>
        /// Применяет накопленную runtime конфигурацию
        /// </summary>
        void ApplyRuntimeConfig() {
            double multiplier = GetRuntimeConfig<double>("bonus_multiplier", 1.5);
            int minTiles = GetRuntimeConfig<int>("min_tiles", 3);
            bool exponential = GetRuntimeConfig<bool>("exponential_growth", false);
            int maxBonus = GetRuntimeConfig<int>("max_bonus", 1000);

            auto strategy = std::make_shared<ConfigurableRuntimeStrategy>(
                multiplier, minTiles, exponential, maxBonus);
            SetBonusStrategy(strategy, "runtime-configured");
        }

        /// <summary>
        /// Конструктор менеджера счета
        /// </summary>
        /// <param name="scoreLabel">Метка для отображения счета</param>
        ScoreManager();
        ScoreManager(const std::string& name);
        ScoreManager(const ScoreManager& other);

        // Оператор присваивания
        ScoreManager& operator=(const ScoreManager& other);

        // Перегрузка операторов
        ScoreManager& operator+=(Int64 points);
        ScoreManager operator+(Int64 points) const;
        bool operator>(const ScoreManager& other) const;
        bool operator<(const ScoreManager& other) const;

        /// <summary>
        /// Деструктор
        /// </summary>
        ~ScoreManager() = default;

        // Управление стратегией бонусов
        void SetBonusStrategy(std::shared_ptr<IBonusStrategy> strategy);
        void SetBonusStrategy(std::shared_ptr<IBonusStrategy> strategy, std::string str);

        std::shared_ptr<IBonusStrategy> GetBonusStrategy() const;
       
        std::string GetBonusStrategyDescription() const;

        /// <summary>
        /// Рассчитывает бонусные очки
        /// </summary>
        /// <param name="tilesRemoved">Количество удаленных плиток</param>
        Int64 CalculateBonus(Int64 tilesRemoved);

        /// <summary>
        /// Добавляет очки на основе количества удаленных плиток
        /// </summary>
        /// <param name="tilesRemoved">Количество удаленных плиток</param>
        String^ AddScoreForTiles(Int64 tilesRemoved);

        /// <summary>
        /// Добавляет очки к текущему счету
        /// </summary>
        /// <param name="points">Количество очков для добавления</param>
        String^ AddScore(Int64 points);

        /// <summary>
        /// Сбрасывает счет до нуля
        /// </summary>
        Void ResetScore();

        /// <summary>
        /// Возвращает текущий счет
        /// </summary>
        Int64 GetCurrentScore();

        /// <summary>
        /// Обновляет отображение счета на форме
        /// </summary>
        String^ FormatScore(Int64 currentScore);

        // Работа с std::string
        void SetPlayerName(const std::string& name);
        std::string GetPlayerName() const;
        std::string GetFullScoreInfo() const;

        // Оператор вывода в строку
        operator std::string() const;

        /// <summary>
        /// Добавляет счет в историю
        /// </summary>
        void AddScoreToHistory(const std::string& playerName, Int64 score, Int64 level = 1);

        /// <summary>
        /// Сохраняет текущий счет в историю
        /// </summary>
        void SaveScoreToHistory(Int64 score);

        /// <summary>
        /// Возвращает топ N результатов
        /// </summary>
        std::vector<std::shared_ptr<PlayerScoreData>> GetTopScores(Int64 count = 10);

        /// <summary>
        /// Отображает историю счетов
        /// </summary>
        void DisplayScoreHistory() const;

        /// <summary>
        /// Возвращает средний счет из последних игр
        /// </summary>
        double GetAverageRecentScore() const;

        /// <summary>
        /// Возвращает максимальный счет из последних игр
        /// </summary>
        Int64 GetMaxRecentScore() const;

        /// <summary>
        /// Вычисляет и отображает статистику
        /// </summary>
        void CalculateAndDisplayStatistics() const;
    };

}