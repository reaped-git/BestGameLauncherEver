#include "InputHandler.hpp"

namespace Match3 {

    /// <summary>
    /// Конструктор обработчика ввода
    /// </summary>
    InputHandler::InputHandler()
    {
        // Изначально ни одна плитка не выбрана
        selectedTile1 = nullptr;
        selectedTile2 = nullptr;
    }

    /// <summary>
    /// Основной обработчик кликов по плиткам
    /// </summary>
    InputHandler::TileClickResult InputHandler::HandleTileClick(Object^ sender, EventArgs^ e, GameLogic* gameLogic)
    {
        Button^ clicked = safe_cast<Button^>(sender);

        // Проверяет, можно ли сейчас делать ходы (только в состоянии Playing)
        if (gameLogic->GetCurrentState() != GameLogic::GameState::ePlaying)
            return TileClickResult::None;

        return ProcessTileSelection(clicked);
    }

    /// <summary>
    /// Обрабатывает логику выбора плитки (первая, вторая, отмена)
    /// </summary>
    InputHandler::TileClickResult InputHandler::ProcessTileSelection(Button^ clickedTile)
    {
        // Первая плитка не выбрана - выбирает текущую
        if (selectedTile1 == nullptr)
        {
            selectedTile1 = clickedTile;
            return TileClickResult::FirstSelected;
        }

        // Клик на уже выбранную плитку - отмена выбора
        else if (selectedTile1 == clickedTile)
        {
            ResetSelection();
            return TileClickResult::Deselected;
        }

        // Выбрана вторая плитка - возвращаем обе для обмена
        else
        {
            selectedTile2 = clickedTile;
            return TileClickResult::SecondSelected;
        }
    }

    /// <summary>
    /// Сбрасывает текущий выбор плиток
    /// </summary>
    Void InputHandler::ResetSelection()
    {
        selectedTile1 = nullptr;
        selectedTile2 = nullptr;
    }

    /// <summary>
    /// Возвращает первую выбранную плитку
    /// </summary>
    Button^ InputHandler::GetFirstSelectedTile()
    {
        return selectedTile1;
    }

    /// <summary>
    /// Возвращает вторую выбранную плитку
    /// </summary>
    Button^ InputHandler::GetSecondSelectedTile()
    {
        return selectedTile2;
    }

}
