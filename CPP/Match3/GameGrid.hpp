#pragma once

namespace Match3 {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    /// <summary>
    /// Класс для управления игровой сеткой плиток
    /// Создает, размещает и управляет состоянием плиток на игровом поле
    /// </summary>
    public ref class GameGrid
    {

    public:
        static const Int64 DEFAULT_GRID_SIZE = 16;
        static const Int64 DEFAULT_TILE_SIZE = 34;
        static array<Color>^ colorPalette;

    private:
        array<Button^, 2>^ grid;        // Двумерный массив кнопок-плиток
        Int64 gridSize;                 // Текущий размер сетки
        Int64 tileSize;                 // Текущий размер плитки
        Random^ random;                 // Генератор случайных чисел
        Panel^ backgroundPanel;         // Родительская панель для размещения плиток

    public:
        /// <summary>
        /// Конструктор игровой сетки
        /// </summary>
        /// <param name="background">Панель для размещения плиток</param>
        /// <param name="size">Размер сетки</param>
        /// <param name="tileSize">Размер одной плитки</param>
        GameGrid(Panel^ background, Int64 size, Int64 tileSize);

        /// <summary>
        /// Деструктор
        /// </summary>
        ~GameGrid() {};

        /// <summary>
        /// Возвращает двумерный массив плиток
        /// </summary>
        array<Button^, 2>^ GetGrid() { return grid; }

        /// <summary>
        /// Возвращает размер сетки
        /// </summary>
        Int64 GetSize() { return gridSize; }

        /// <summary>
        /// Инициализирует сетку - создает и размещает все плитки
        /// </summary>
        Void InitializeGrid();

        /// <summary>
        /// Генерирует случайный цвет для плитки из предопределенной палитры
        /// </summary>
        Color GetRandomColor();

        /// <summary>
        /// Меняет цвета двух плиток местами
        /// </summary>
        Void SwapTiles(Button^ a, Button^ b);

        /// <summary>
        /// Заполняет пустые (прозрачные) плитки случайными цветами
        /// </summary>
        Void FillEmptyTile(Button^ btn);

        /// <summary>
        /// Устанавливает обработчик клика для всех плиток
        /// </summary>
        Void SetTileClickHandler(EventHandler^ handler);

        /// <summary>
        /// Сбрасывает выделение всех плиток
        /// </summary>
        Void ResetTileSelection(Button^ btn);

        Void ForEachTile(Action<Button^>^ action);

    private:
        /// <summary>
        /// Создает отдельную плитку в указанной позиции сетки
        /// </summary>
        /// <param name="i">Строка в сетке</param>
        /// <param name="j">Столбец в сетке</param>
        Void CreateTile(Int64 i, Int64 j);

    };
}