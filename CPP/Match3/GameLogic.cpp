#include "GameLogic.hpp"

namespace Match3 {

    /// <summary>
    /// Конструктор игровой логики
    /// </summary>
    GameLogic::GameLogic()
        : initializing(true), currentState(GameState::eInitializing),
        scoreManager(std::make_shared<ScoreManager>())
    {
    }

    GameLogic::GameLogic(std::shared_ptr<ScoreManager> scoreMgr)
        : initializing(true), currentState(GameState::eInitializing),
        scoreManager(scoreMgr)
    {
    }

    // Конструктор копирования
    GameLogic::GameLogic(const GameLogic& other)
        : initializing(other.initializing),
        currentState(other.currentState),
        scoreManager(other.scoreManager) // shared_ptr копируется
    {
    }

    // Оператор присваивания
    GameLogic& GameLogic::operator=(const GameLogic& other)
    {
        if (this != &other)
        {
            initializing = other.initializing;
            currentState = other.currentState;
            scoreManager = other.scoreManager;
        }
        return *this;
    }

    // Перегрузка операторов
    bool GameLogic::operator==(const GameLogic& other) const
    {
        return initializing == other.initializing &&
            currentState == other.currentState &&
            scoreManager == other.scoreManager;
    }

    bool GameLogic::operator!=(const GameLogic& other) const
    {
        return !(*this == other);
    }

    GameLogic GameLogic::operator+(const GameLogic& other) const
    {
        GameLogic result(*this);
        // Логика "сложения" состояний (для демонстрации)
        if (other.currentState == GameState::eProcessing)
            result.currentState = GameState::eProcessing;
        return result;
    }


    template<typename Func>
    void GameLogic::ForEachMatched(Int64 gridSize,
        const std::vector<std::vector<bool>>& matched, Func func) {
        for (Int64 i = 0; i < gridSize; i++) {
            for (Int64 j = 0; j < gridSize; j++) {
                func(i, j, matched[i][j]);
            }
        }
    }

    template<typename Func>
    void GameLogic::ForEachGrid(array<Button^, 2>^ grid, Int64 gridSize, Func func) {
        for (Int64 i = 0; i < gridSize; i++) {
            for (Int64 j = 0; j < gridSize; j++) {
                func(i, j, grid);
            }
        }
    }

    /// <summary>
    /// Проверяет и удаляет совпадения (3+ в ряд по горизонтали или вертикали)
    /// </summary>
    Boolean GameLogic::CheckMatches(array<Button^, 2>^ grid, Int64 gridSize, std::vector<std::vector<bool>>& matchedTiles)
    {
        Boolean found = false;

        // Создаем матрицу для отметки совпавших плиток
        std::vector<std::vector<bool>> matched(gridSize, std::vector<bool>(gridSize, false));

        CheckHorizontalMatches(grid, gridSize, matched);
        CheckVerticalMatches(grid, gridSize, matched);
        found = HasMatches(gridSize, matched);

        if (found)
        {
            matchedTiles = matched; // Возвращаем информацию о совпавших плитках
            RemoveMatchedTiles(grid, gridSize, matched);
            DropTiles(grid, gridSize);
        }

        return found;
    }

    /// <summary>
    /// Проверяет горизонтальные совпадения
    /// </summary>
    Void GameLogic::CheckHorizontalMatches(array<Button^, 2>^ grid,
        Int64 gridSize, std::vector<std::vector<bool>>& matched)
    {
        ForEachGrid(grid, gridSize, [&](Int64 i, Int64 j, array<Button^, 2>^ gridRef) {
            if (j < gridSize - 2) {
                Color c = gridRef[i, j]->BackColor;
                if (c == Color::Transparent) return;
                if (c != gridRef[i, j + 1]->BackColor || c != gridRef[i, j + 2]->BackColor) return;

                matched[i][j] = true;
                matched[i][j + 1] = true;
                matched[i][j + 2] = true;
            }
            });
    }

    /// <summary>
    /// Проверяет вертикальные совпадения
    /// </summary>
    Void GameLogic::CheckVerticalMatches(array<Button^, 2>^ grid, Int64 gridSize, std::vector<std::vector<bool>>& matched)
    {
        ForEachGrid(grid, gridSize, [&](Int64 i, Int64 j, array<Button^, 2>^ gridRef) {
            if (i < gridSize - 2) {
                Color c = gridRef[i, j]->BackColor;
                if (c == Color::Transparent) return;
                if (c != gridRef[i + 1, j]->BackColor || c != gridRef[i + 2, j]->BackColor) return;

                matched[i][j] = true;
                matched[i + 1][j] = true;
                matched[i + 2][j] = true;
            }
            });
    }

    /// <summary>
    /// Проверяет, есть ли в матрице отмеченные совпадения
    /// </summary>
    Boolean GameLogic::HasMatches(Int64 gridSize, const std::vector<std::vector<bool>>& matched)
    {
        Boolean found = false;
        ForEachMatched(gridSize, matched, [&found](Int64 i, Int64 j, bool isMatched) {
            if (isMatched) found = true;
            });
        return found;
    }

    /// <summary>
    /// Удаляет плитки, отмеченные как совпавшие
    /// </summary>
    Void GameLogic::RemoveMatchedTiles(array<Button^, 2>^ grid, Int64 gridSize, const std::vector<std::vector<bool>>& matched)
    {
        ForEachGrid(grid, gridSize, [&](Int64 i, Int64 j, array<Button^, 2>^ gridRef) {
            if (matched[i][j]) {
                gridRef[i, j]->BackColor = Color::Transparent;
            }
            });
    }

    /// <summary>
    /// Осуществляет падение плиток после удаления совпадений
    /// </summary>
    Void GameLogic::DropTiles(array<Button^, 2>^ grid, Int64 gridSize)
    {
        for (Int64 j = 0; j < gridSize; j++)
        {
            Int64 emptyRow = gridSize - 1;
            for (Int64 i = gridSize - 1; i >= 0; i--)
            {
                if (grid[i, j]->BackColor != Color::Transparent)
                {
                    if (i != emptyRow) {
                        grid[emptyRow, j]->BackColor = grid[i, j]->BackColor;
                        grid[i, j]->BackColor = Color::Transparent;
                    }
                    emptyRow--;
                }
            }
        }

        if (initializing) return;
        Application::DoEvents();
        Thread::Sleep(200);
    }

    /// <summary>
    /// Обрабатывает цепную реакцию совпадений до их полного отсутствия
    /// </summary>
    Int64 GameLogic::ProcessMatches(array<Button^, 2>^ grid, Int64 gridSize, GameGrid^ gameGrid)
    {
        currentState = GameState::eProcessing; // Блокирует ввод во время обработки

        Int64 totalRemoved = 0;
        Boolean changed;
        do
        {
            std::vector<std::vector<bool>> matchedTiles;
            gameGrid->ForEachTile(gcnew Action<Button^>(gameGrid, &GameGrid::FillEmptyTile));
            changed = CheckMatches(grid, gridSize, matchedTiles);   // Проверяет и удаляет совпадения

            if (changed)
            {
                // Считаем плитки, удаленные на этом шаге цепной реакции
                Int64 stepRemoved = CountRemovedTiles(matchedTiles, gridSize);
                totalRemoved += stepRemoved;

                // Добавляет задержку для визуального эффекта
                if (initializing) continue;

                Application::DoEvents();
                Thread::Sleep(200);
            }

        } while (changed);  // Продолжает пока есть изменения

        currentState = GameState::ePlaying;
        initializing = false;   // Снимает флаг инициализации

        return totalRemoved;
    }

    /// <summary>
    /// Проверяет, являются ли две плитки соседними
    /// </summary>
    Boolean GameLogic::AreAdjacent(Button^ a, Button^ b)
    {
        // Получает координаты из Tag плиток
        Point p1 = safe_cast<Point>(a->Tag);
        Point p2 = safe_cast<Point>(b->Tag);

        // Вычисляет разницу по координатам
        Int64 dx = Math::Abs(p1.X - p2.X);
        Int64 dy = Math::Abs(p1.Y - p2.Y);

        return (dx + dy) == 1;
    }

    /// <summary>
    /// Обрабатывает попытку обмена двух плиток
    /// </summary>
    Int64 GameLogic::HandleTileSwap(Button^ tile1, Button^ tile2, array<Button^, 2>^ grid, Int64 gridSize, GameGrid^ gameGrid)
    {
        if (AreAdjacent(tile1, tile2))
        {
            // Пробует обменять плитки
            gameGrid->SwapTiles(tile1, tile2);

            // Добавляет задержку для визуального эффекта
            if (!initializing)
            {
                Application::DoEvents();
                Thread::Sleep(200);
            }

            // Проверяем совпадения после обмена
            std::vector<std::vector<bool>> matchedTiles;
            if (CheckMatches(grid, gridSize, matchedTiles))
            {
                // Если есть совпадения - обрабатываем цепную реакцию и возвращаем общее количество удаленных плиток
                Int64 removedCount = CountRemovedTiles(matchedTiles, gridSize);

                // Обрабатываем цепную реакцию и суммируем все удаленные плитки
                removedCount += ProcessMatches(grid, gridSize, gameGrid);

                return removedCount;
            }
            else
            {
                // Если нет совпадений - откатываем обмен
                gameGrid->SwapTiles(tile1, tile2);
                return 0;
            }
        }
        return 0;
    }

    /// <summary>
    /// Подсчитывает количество удаленных плиток
    /// </summary>
    Int64 GameLogic::CountRemovedTiles(const std::vector<std::vector<bool>>& matched, Int64 gridSize)
    {
        Int64 count = 0;
        ForEachMatched(gridSize, matched, [&count](Int64 i, Int64 j, bool isMatched) {
            if (isMatched) count++;
            });
        return count;
    }



    std::string GetGameStateString(const Match3::GameLogic& gameLogic)
    {
        switch (gameLogic.currentState)
        {
        case GameLogic::GameState::eInitializing:
            return "Initializing";
        case GameLogic::GameState::ePlaying:
            return "Playing";
        case GameLogic::GameState::eProcessing:
            return "Processing";
        default:
            return "Unknown";
        }
        return "Unknown";
    }
}