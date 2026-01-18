#pragma once

#include "IBonusStrategy.hpp"
#include <vector>
#include <memory>
#include <array>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <numbers>

// ==================== СТАТИЧЕСКИЕ КОНФИГУРАЦИИ (COMPILE-TIME) ====================

// Конфигурация через макросы (определяется в CMake/настройках проекта)
#ifdef GAME_BONUS_AGGRESSIVE
#define BONUS_MULTIPLIER_BASE 2.0
#define MIN_TILES_FOR_BONUS 2
#elif defined(GAME_BONUS_CONSERVATIVE)
#define BONUS_MULTIPLIER_BASE 1.2
#define MIN_TILES_FOR_BONUS 5
#else
    // Конфигурация по умолчанию
#define BONUS_MULTIPLIER_BASE 1.5
#define MIN_TILES_FOR_BONUS 3
#endif

// Конфигурация через шаблоны с разными параметрами
template <int BaseScore, int MinCombo, bool EnableProgressive>
class ConfigurableBonusStrategy : public Match3::IBonusStrategy {
public:
    int CalculateBonus(int tilesRemoved, int baseScorePerTile) const override {
        if (tilesRemoved < MinCombo) return 0;

        int base = tilesRemoved * baseScorePerTile;

        if constexpr (EnableProgressive) {
            // Прогрессивный множитель
            double multiplier = 1.0 + (tilesRemoved - MinCombo) * 0.3;
            return static_cast<int>(base * (multiplier - 1.0));
        }
        else {
            // Фиксированный множитель
            return base * (BaseScore / 10);
        }
    }

    std::string GetDescription() const override {
        return "Configurable Strategy [Base=" + std::to_string(BaseScore) +
            ", MinCombo=" + std::to_string(MinCombo) +
            ", Progressive=" + std::to_string(EnableProgressive) + "]";
    }
};

// Шаблонная стратегия с compile-time вычислениями
template <size_t BufferSize, bool UseSIMD>
class OptimizedBonusStrategy : public Match3::IBonusStrategy {
private:
    // Compile-time вычисление оптимальных параметров
    static constexpr size_t CalculateOptimalThreshold() {
        if constexpr (BufferSize > 1024) {
            return 10;  // Большие буферы позволяют более агрессивные бонусы
        }
        else {
            return 5;   // Маленькие буферы - консервативные бонусы
        }
    }

    static constexpr double CalculateMultiplier() {
        double base = 1.5;
        if constexpr (UseSIMD) {
            base *= 1.2;  // SIMD дает дополнительный множитель
        }
        if constexpr (BufferSize > 2048) {
            base *= 1.1;  // Большие буферы дают дополнительный бонус
        }
        return base;
    }

    static constexpr size_t OPTIMAL_THRESHOLD = CalculateOptimalThreshold();
    static constexpr double MULTIPLIER = CalculateMultiplier();

public:
    int CalculateBonus(int tilesRemoved, int baseScorePerTile) const override {
        if (tilesRemoved < static_cast<int>(OPTIMAL_THRESHOLD)) return 0;

        int extraTiles = tilesRemoved - OPTIMAL_THRESHOLD;
        return static_cast<int>(extraTiles * baseScorePerTile * MULTIPLIER);
    }

    std::string GetDescription() const override {
        return "Optimized Strategy [Buffer=" + std::to_string(BufferSize) +
            "KB, SIMD=" + std::to_string(UseSIMD) +
            ", Threshold=" + std::to_string(OPTIMAL_THRESHOLD) + "]";
    }
};

namespace Match3 {
    // ==================== ДИНАМИЧЕСКИЕ КОНФИГУРАЦИИ (RUNTIME) ====================

    /// <summary>
    /// Стратегия, конфигурируемая через JSON/файл конфигурации
    /// </summary>
    class ConfigurableRuntimeStrategy : public IBonusStrategy {
    private:
        struct Config {
            double multiplier;
            int minTiles;
            bool exponential;
            int maxBonus;
        } config;

    public:
        ConfigurableRuntimeStrategy(double mult = 1.5, int min = 3, bool exp = false, int max = 1000)
            : config{ mult, min, exp, max } {
        }

        void SetConfig(double mult, int min, bool exp, int max) {
            config = { mult, min, exp, max };
        }

        int CalculateBonus(int tilesRemoved, int baseScorePerTile) const override {
            if (tilesRemoved < config.minTiles) return 0;

            int bonus;
            if (config.exponential) {
                // Экспоненциальный рост
                double exp = std::pow(config.multiplier, tilesRemoved - config.minTiles);
                bonus = static_cast<int>(tilesRemoved * baseScorePerTile * exp);
            }
            else {
                // Линейный рост
                bonus = static_cast<int>(tilesRemoved * baseScorePerTile * config.multiplier);
            }

            // Ограничение максимального бонуса
            return (bonus > config.maxBonus) ? config.maxBonus : bonus;
        }

        std::string GetDescription() const override {
            return "Runtime Configurable [Mult=" + std::to_string(config.multiplier) +
                ", Min=" + std::to_string(config.minTiles) +
                ", Exp=" + std::to_string(config.exponential) +
                ", Max=" + std::to_string(config.maxBonus) + "]";
        }
    };

    /// <summary>
    /// Стратегия, загружающая конфигурацию из аргументов командной строки
    /// </summary>
    class CommandLineStrategy : public IBonusStrategy {
    private:
        std::string mode;
        int difficulty;

    public:
        CommandLineStrategy(const std::string& mode = "normal", int difficulty = 1)
            : mode(mode), difficulty(difficulty) {
        }

        int CalculateBonus(int tilesRemoved, int baseScorePerTile) const override {
            // Конфигурация на основе режима и сложности
            double multiplier = 1.0;

            if (mode == "aggressive") {
                multiplier = 2.0 + (difficulty * 0.5);
            }
            else if (mode == "conservative") {
                multiplier = 1.0 + (difficulty * 0.2);
            }
            else { // normal
                multiplier = 1.5 + (difficulty * 0.3);
            }

            int minTiles = (mode == "aggressive") ? 2 : 3;

            if (tilesRemoved < minTiles) return 0;
            return static_cast<int>(tilesRemoved * baseScorePerTile * multiplier);
        }

        std::string GetDescription() const override {
            return "Command Line Strategy [Mode=" + mode +
                ", Difficulty=" + std::to_string(difficulty) + "]";
        }
    };

    /// <summary>
    /// Адаптивная стратегия, меняющая параметры во время выполнения
    /// </summary>
    class AdaptiveStrategy : public IBonusStrategy {
    private:
        mutable double currentMultiplier;
        mutable int successCounter;
        int maxMultiplier;

    public:
        AdaptiveStrategy(double initialMultiplier = 1.5, int maxMult = 5)
            : currentMultiplier(initialMultiplier), successCounter(0), maxMultiplier(maxMult) {
        }

        int CalculateBonus(int tilesRemoved, int baseScorePerTile) const override {
            // Адаптируем множитель на основе успешности предыдущих расчетов
            if (tilesRemoved >= 10) {
                // Успешный большой комбо - увеличиваем множитель
                currentMultiplier = std::min(currentMultiplier * 1.1, static_cast<double>(maxMultiplier));
                successCounter++;
            }
            else if (tilesRemoved < 3) {
                // Неудачный ход - уменьшаем множитель
                currentMultiplier = std::max(currentMultiplier * 0.9, 1.0);
            }

            return static_cast<int>(tilesRemoved * baseScorePerTile * currentMultiplier);
        }

        std::string GetDescription() const override {
            return "Adaptive Strategy [CurrentMult=" + std::to_string(currentMultiplier) +
                ", Successes=" + std::to_string(successCounter) + "]";
        }
    };
}