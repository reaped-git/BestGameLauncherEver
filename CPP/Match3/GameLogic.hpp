#pragma once

#include "GameGrid.hpp"
#include "ScoreManager.hpp"
#include <vector>
#include <functional>
#include <memory>
#include <string>

namespace Match3 {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    using namespace System::Threading;

    /// <summary>
    /// Класс игровой логики - отвечает за проверку совпадений, обработку ходов и управление состоянием игры
    /// </summary>
    class GameLogic
    {
    public:
        /// <summary>
        /// Состояния игры
        /// </summary>
        enum class GameState
        {
            eInitializing = 0,      // Игра инициализируется
            ePlaying = 1,           // Игрок может делать ходы
            eProcessing = 2,        // Игра обрабатывает совпадения (блокирует ввод)
        };


    private:
        GameState currentState;     // Текущее состояние игры
        Boolean initializing;       // Флаг инициализации
        std::shared_ptr<ScoreManager> scoreManager;

    public:
        /// <summary>
        /// Конструктор игровой логики
        /// </summary>
        GameLogic();
        GameLogic(std::shared_ptr<ScoreManager> scoreMgr);
        GameLogic(const GameLogic& other);

        GameLogic& operator=(const GameLogic& other);

        bool operator==(const GameLogic& other) const;
        bool operator!=(const GameLogic& other) const;
        GameLogic operator+(const GameLogic& other) const;

        /// <summary>
        /// Деструктор
        /// </summary>
        ~GameLogic() = default;

        // Дружественная функция
        friend std::string GetGameStateString(const GameLogic& gameLogic);

        /// <summary>
        /// Проверяет и удаляет совпадения (3+ в ряд по горизонтали или вертикали)
        /// </summary>
        /// <param name="matchedTiles">Возвращает информацию о совпавших плитках</param>
        /// <returns>true если найдены совпадения, иначе false</returns>
        Boolean CheckMatches(array<Button^, 2>^ grid, Int64 gridSize, 
            std::vector<std::vector<bool>>& matchedTiles);

        /// <summary>
        /// Обрабатывает цепную реакцию совпадений до их полного отсутствия
        /// </summary>
        Int64 ProcessMatches(array<Button^, 2>^ grid, Int64 gridSize, GameGrid^ gameGrid);

        /// <summary>
        /// Проверяет, являются ли две плитки соседними
        /// </summary>
        Boolean AreAdjacent(Button^ a, Button^ b);

        /// <summary>
        /// Обрабатывает попытку обмена двух плиток
        /// </summary>
        Int64 HandleTileSwap(Button^ tile1, Button^ tile2, array<Button^, 2>^ grid, 
            Int64 gridSize, GameGrid^ gameGrid);

        /// <summary>
        /// Возвращает текущее состояние игры
        /// </summary>
        GameState GetCurrentState() { return currentState; }

        /// <summary>
        /// Устанавливает состояние игры
        /// </summary>
        Void SetCurrentState(GameState state) { currentState = state; }

        /// <summary>
        /// Проверяет, есть ли совпадения в матрице
        /// </summary>
        Boolean HasMatches(Int64 gridSize, const std::vector<std::vector<bool>>& matched);

        /// <summary>
        /// Удаляет совпавшие плитки
        /// </summary>
        Void RemoveMatchedTiles(array<Button^, 2>^ grid, Int64 gridSize, 
            const std::vector<std::vector<bool>>& matched);

        /// <summary>
        /// Подсчитывает количество удаленных плиток
        /// </summary>
        Int64 CountRemovedTiles(const std::vector<std::vector<bool>>& matched, Int64 gridSize);

    private:
        /// <summary>
        /// Проверяет горизонтальные совпадения
        /// </summary>
        Void CheckHorizontalMatches(array<Button^, 2>^ grid, Int64 gridSize, 
            std::vector<std::vector<bool>>& matched);

        /// <summary>
        /// Проверяет вертикальные совпадения
        /// </summary>
        Void CheckVerticalMatches(array<Button^, 2>^ grid, Int64 gridSize, 
            std::vector<std::vector<bool>>& matched);

        /// <summary>
        /// Осуществляет падение плиток после удаления совпадений
        /// </summary>
        Void DropTiles(array<Button^, 2>^ grid, Int64 gridSize);

        /// <summary>
        /// Итерация с matched
        /// </summary>
        /// <typeparam name="Func"></typeparam>
        /// <param name="gridSize"></param>
        /// <param name="matched"></param>
        /// <param name="func"></param>
        template<typename Func>
        void ForEachMatched(Int64 gridSize, 
            const std::vector<std::vector<bool>>& matched, Func func);

        /// <summary>
        /// Итерация с Grid
        /// </summary>
        /// <typeparam name="Func"></typeparam>
        /// <param name="grid"></param>
        /// <param name="gridSize"></param>
        /// <param name="func"></param>
        template<typename Func>
        void ForEachGrid(array<Button^, 2>^ grid, Int64 gridSize, Func func);

    };
}