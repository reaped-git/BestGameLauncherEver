#pragma once

#include <string>
#include <vector>

namespace MineSweeper {

    /// <summary>
    /// Класс для управления уровнями сложности
    /// </summary>
    class DifficultyManager
    {
    public:
        /// <summary>
        /// Структура настроек сложности
        /// </summary>
        struct DifficultySettings
        {
            std::string name;
            int gridSize;
            int minesCount;
            int timeLimit;
        };

    private:
        std::vector<DifficultySettings> difficulties;
        int currentDifficulty;

    public:
        /// <summary>
        /// Конструктор менеджера сложности
        /// </summary>
        DifficultyManager();

        /// <summary>
        /// Деструктор
        /// </summary>
        ~DifficultyManager() = default;

        /// <summary>
        /// Устанавливает текущий уровень сложности
        /// </summary>
        void SetDifficulty(int index);

        /// <summary>
        /// Возвращает текущие настройки сложности
        /// </summary>
        const DifficultySettings& GetCurrentSettings() const;

        /// <summary>
        /// Возвращает массив названий уровней сложности
        /// </summary>
        std::vector<std::string> GetDifficultyNames() const;

        /// <summary>
        /// Возвращает количество уровней сложности
        /// </summary>
        int GetDifficultyCount() const { return (int)difficulties.size(); }

        /// <summary>
        /// Возвращает текущий индекс сложности
        /// </summary>
        int GetCurrentDifficulty() const { return currentDifficulty; }
    };
}