#pragma once

#include <vector>
#include <random>
#include <string>
#include <memory>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <functional>

namespace MineSweeper {

    /// <summary>
    /// Базовый класс для игрового поля
    /// </summary>
    class BaseField
    {
    protected:
        int gridSize;
        int minesCount;

    public:
        BaseField(int size, int mines) : gridSize(size), minesCount(mines) {}
        virtual ~BaseField() = default;

        virtual void Initialize() = 0;
        virtual void Clear() = 0;

        int GetGridSize() const { return gridSize; }
        int GetMinesCount() const { return minesCount; }

        // Дружественная функция для вывода информации
        friend std::ostream& operator<<(std::ostream& os, const BaseField& field);
    };

    /// <summary>
    /// Класс для управления игровым полем с минами
    /// </summary>
    class MineField : public BaseField
    {
    private:
        std::vector<std::vector<bool>> mines;         // Расположение мин
        std::vector<std::vector<bool>> opened;        // Открытые клетки
        std::vector<std::vector<bool>> flagged;       // Клетки с флагами
        std::mt19937 random;         // Генератор случайных чисел
        std::string fieldName;       // Имя поля

    public:
        /// <summary>
        /// Конструктор игрового поля
        /// </summary>
        MineField(int size, int mines);

        /// <summary>
        /// Конструктор с именем
        /// </summary>
        MineField(int size, int mines, const std::string& name);

        /// <summary>
        /// Конструктор копирования
        /// </summary>
        MineField(const MineField& other);

        /// <summary>
        /// Конструктор из базового класса
        /// </summary>
        MineField(const BaseField& base, const std::string& name = "");

        ~MineField() override = default;

        // Остальные методы остаются, но добавляем:

        /// <summary>
        /// Перегрузка оператора присваивания
        /// </summary>
        MineField& operator=(const MineField& other);

        /// <summary>
        /// Перегрузка оператора сравнения
        /// </summary>
        bool operator==(const MineField& other) const;

        /// <summary>
        /// Перегрузка оператора индексации
        /// </summary>
        std::vector<bool>& operator[](int row);
        const std::vector<bool>& operator[](int row) const;

        /// <summary>
        /// Перегрузка оператора преобразования в строку
        /// </summary>
        operator std::string() const;

        /// <summary>
        /// Получает имя поля
        /// </summary>
        std::string GetName() const { return fieldName; }

        /// <summary>
        /// Устанавливает имя поля
        /// </summary>
        void SetName(const std::string& name);

        // Остальные методы остаются без изменений
        void Initialize() override;
        void Clear() override;
        bool OpenCell(int row, int col);
        void ToggleFlag(int row, int col);
        int CountMinesAround(int row, int col) const;
        bool CheckWin() const;
        bool IsMine(int row, int col) const { return mines[row][col]; }
        bool IsOpened(int row, int col) const { return opened[row][col]; }
        bool IsFlagged(int row, int col) const { return flagged[row][col]; }

        struct CellState {
            bool isMine;
            bool isOpened;
            bool isFlagged;
            int minesAround;
        };

        CellState GetCellState(int row, int col) const;

        // Дружественная функция для обмена полями
        friend void SwapFields(MineField& first, MineField& second);

    private:
        void PlaceMines();
        void OpenEmptyCells(int row, int col);
        bool IsValidCoordinate(int row, int col) const;
    };

    // Дружественная функция для BaseField
    std::ostream& operator<<(std::ostream& os, const BaseField& field);

    // Дружественная функция для MineField
    void SwapFields(MineField& first, MineField& second);
}