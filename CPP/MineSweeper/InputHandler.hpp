#pragma once

namespace MineSweeper {

    /// <summary>
    /// Класс обработки ввода пользователя в игре "Сапёр"
    /// </summary>
    class InputHandler
    {
    public:
        /// <summary>
        /// Типы кнопок мыши
        /// </summary>
        enum class MouseButton
        {
            Left = 0,
            Right = 1
        };

        /// <summary>
        /// Результат обработки клика
        /// </summary>
        enum class ClickResult
        {
            None = 0,
            CellOpened = 1,
            FlagToggled = 2,
            Invalid = 3
        };

        /// <summary>
        /// Структура с информацией о клике
        /// </summary>
        struct ClickInfo
        {
            int row;
            int col;
            MouseButton button;
            ClickResult result;
        };

    public:
        /// <summary>
        /// Конструктор обработчика ввода
        /// </summary>
        InputHandler() = default;

        /// <summary>
        /// Деструктор
        /// </summary>
        ~InputHandler() = default;

        /// <summary>
        /// Обрабатывает клик по клетке
        /// </summary>
        ClickInfo HandleCellClick(int row, int col, MouseButton button) const;

        /// <summary>
        /// Проверяет валидность координат
        /// </summary>
        static bool IsValidCoordinates(int row, int col, int gridSize);
    };
}