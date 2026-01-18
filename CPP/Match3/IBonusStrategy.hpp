#pragma once

#include <cstdint>
#include <iostream>

namespace Match3 {

    /// <summary>
    /// Интерфейс стратегии расчета бонусных очков
    /// </summary>
    class IBonusStrategy {
    public:
        virtual ~IBonusStrategy() = default;

        /// <summary>
        /// Рассчитывает бонусные очки за удаленные плитки
        /// </summary>
        /// <param name="tilesRemoved">Количество удаленных плиток</param>
        /// <param name="baseScorePerTile">Базовые очки за одну плитку</param>
        /// <returns>Сумма бонусных очков</returns>
        virtual int CalculateBonus(int tilesRemoved, int baseScorePerTile) const = 0;

        /// <summary>
        /// Возвращает описание стратегии
        /// </summary>
        virtual std::string GetDescription() const = 0;
    };
}