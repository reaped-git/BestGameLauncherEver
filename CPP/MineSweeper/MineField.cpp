#include "MineField.hpp"
#include <random>
#include <stdexcept>

namespace MineSweeper {

    MineField::MineField(int size, int mines)
        : BaseField(size, mines), random(std::random_device{}()), fieldName("Default Field")
    {
        if (minesCount >= gridSize * gridSize) {
            throw std::invalid_argument("Too many mines for the grid size");
        }
        Initialize();
    }

    MineField::MineField(int size, int mines, const std::string& name)
        : BaseField(size, mines), random(std::random_device{}()), fieldName(name)
    {
        if (minesCount >= gridSize * gridSize) {
            throw std::invalid_argument("Too many mines for the grid size");
        }
        Initialize();
    }

    MineField::MineField(const MineField& other)
        : BaseField(other.gridSize, other.minesCount),
        mines(other.mines),
        opened(other.opened),
        flagged(other.flagged),
        random(std::random_device{}()),
        fieldName(other.fieldName + " (Copy)")
    {
        // Копируем состояние
    }

    MineField::MineField(const BaseField& base, const std::string& name)
        : BaseField(base), random(std::random_device{}()), fieldName(name.empty() ? "Derived Field" : name)
    {
        Initialize();
    }

    MineField& MineField::operator=(const MineField& other)
    {
        if (this != &other) {
            gridSize = other.gridSize;
            minesCount = other.minesCount;
            mines = other.mines;
            opened = other.opened;
            flagged = other.flagged;
            fieldName = other.fieldName + " (Assigned)";
        }
        return *this;
    }

    bool MineField::operator==(const MineField& other) const
    {
        return gridSize == other.gridSize &&
            minesCount == other.minesCount &&
            fieldName == other.fieldName;
    }

    std::vector<bool>& MineField::operator[](int row)
    {
        if (row < 0 || row >= gridSize) {
            throw std::out_of_range("Row index out of range");
        }
        return mines[row];
    }

    const std::vector<bool>& MineField::operator[](int row) const
    {
        if (row < 0 || row >= gridSize) {
            throw std::out_of_range("Row index out of range");
        }
        return mines[row];
    }

    MineField::operator std::string() const
    {
        std::ostringstream oss;
        oss << "MineField: " << fieldName
            << ", Size: " << gridSize << "x" << gridSize
            << ", Mines: " << minesCount
            << ", Opened cells: " << [this]() {
            int count = 0;
            for (const auto& row : opened) {
                count += std::count(row.begin(), row.end(), true);
            }
            return count;
            }();
        return oss.str();
    }

    void MineField::SetName(const std::string& name)
    {
        fieldName = name;
    }

    // Дружественная функция для обмена
    void SwapFields(MineField& first, MineField& second)
    {
        using std::swap;
        swap(first.gridSize, second.gridSize);
        swap(first.minesCount, second.minesCount);
        swap(first.mines, second.mines);
        swap(first.opened, second.opened);
        swap(first.flagged, second.flagged);
        swap(first.fieldName, second.fieldName);
    }

    // Дружественная функция для BaseField
    std::ostream& operator<<(std::ostream& os, const BaseField& field)
    {
        os << "BaseField: Size=" << field.GetGridSize()
            << ", Mines=" << field.GetMinesCount();
        return os;
    }

    // Остальные методы остаются аналогичными, но с использованием std::string
    void MineField::Initialize()
    {
        Clear();
        mines = std::vector<std::vector<bool>>(gridSize, std::vector<bool>(gridSize, false));
        opened = std::vector<std::vector<bool>>(gridSize, std::vector<bool>(gridSize, false));
        flagged = std::vector<std::vector<bool>>(gridSize, std::vector<bool>(gridSize, false));
        PlaceMines();
    }

    void MineField::Clear()
    {
        // Векторы очищаются автоматически при присваивании
    }

    void MineField::PlaceMines()
    {
        std::uniform_int_distribution<int> dist(0, gridSize - 1);
        int placed = 0;

        while (placed < minesCount)
        {
            int r = dist(random);
            int c = dist(random);

            if (!mines[r][c])
            {
                mines[r][c] = true;
                placed++;
            }
        }
    }

    bool MineField::OpenCell(int row, int col)
    {
        if (!IsValidCoordinate(row, col))
            return false;

        if (opened[row][col] || flagged[row][col])
            return false;

        opened[row][col] = true;

        if (mines[row][col])
        {
            return false; // Игрок наступил на мину
        }

        int minesAround = CountMinesAround(row, col);
        if (minesAround == 0)
        {
            OpenEmptyCells(row, col);
        }

        return true;
    }

    void MineField::OpenEmptyCells(int row, int col)
    {
        for (int dr = -1; dr <= 1; dr++)
        {
            for (int dc = -1; dc <= 1; dc++)
            {
                if (dr != 0 || dc != 0)
                {
                    int newRow = row + dr;
                    int newCol = col + dc;

                    if (IsValidCoordinate(newRow, newCol) &&
                        !opened[newRow][newCol] &&
                        !flagged[newRow][newCol] &&
                        !mines[newRow][newCol])
                    {
                        opened[newRow][newCol] = true;

                        if (CountMinesAround(newRow, newCol) == 0)
                        {
                            OpenEmptyCells(newRow, newCol);
                        }
                    }
                }
            }
        }
    }

    void MineField::ToggleFlag(int row, int col)
    {
        if (IsValidCoordinate(row, col) && !opened[row][col])
        {
            flagged[row][col] = !flagged[row][col];
        }
    }

    int MineField::CountMinesAround(int row, int col) const
    {
        if (!IsValidCoordinate(row, col))
            return 0;

        int count = 0;
        for (int dr = -1; dr <= 1; dr++)
        {
            for (int dc = -1; dc <= 1; dc++)
            {
                int nr = row + dr;
                int nc = col + dc;
                if (IsValidCoordinate(nr, nc) && mines[nr][nc])
                {
                    count++;
                }
            }
        }
        return count;
    }

    bool MineField::CheckWin() const
    {
        for (int r = 0; r < gridSize; r++)
        {
            for (int c = 0; c < gridSize; c++)
            {
                if (!mines[r][c] && !opened[r][c])
                    return false;
            }
        }
        return true;
    }

    MineField::CellState MineField::GetCellState(int row, int col) const
    {
        CellState state;
        state.isMine = mines[row][col];
        state.isOpened = opened[row][col];
        state.isFlagged = flagged[row][col];
        state.minesAround = CountMinesAround(row, col);
        return state;
    }

    bool MineField::IsValidCoordinate(int row, int col) const
    {
        return row >= 0 && row < gridSize && col >= 0 && col < gridSize;
    }
}