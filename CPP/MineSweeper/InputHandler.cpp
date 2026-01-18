#include "InputHandler.hpp"

namespace MineSweeper {

    InputHandler::ClickInfo InputHandler::HandleCellClick(int row, int col, MouseButton button) const
    {
        ClickInfo info;
        info.row = row;
        info.col = col;
        info.button = button;
        info.result = ClickResult::None;

        return info;
    }

    bool InputHandler::IsValidCoordinates(int row, int col, int gridSize)
    {
        return row >= 0 && row < gridSize && col >= 0 && col < gridSize;
    }
}