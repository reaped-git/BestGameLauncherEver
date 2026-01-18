#pragma once

#include "GameLogic.hpp"

namespace Match3 {

    using namespace System;
    using namespace System::Windows::Forms;

    /// <summary>
    /// Класс обработки ввода пользователя - управляет выбором и обменом плиток
    /// </summary>
    public ref class InputHandler
    {
    private:
        Button^ selectedTile1;    // Первая выбранная плитка
        Button^ selectedTile2;    // Вторая выбранная плитка

    public:

        // Состояния плитки
        enum class TileClickResult 
        {
            None = 0,           // Ничего не произошло
            FirstSelected = 1,   // Выбрана первая плитка
            SecondSelected = 2,  // Выбрана вторая плитка
            Deselected = 3      // Снято выделение

        };

        /// <summary>
        /// Конструктор обработчика ввода
        /// </summary>
        InputHandler();

        /// <summary>
        /// Деструктор
        /// </summary>
        ~InputHandler() { };

        /// <summary>
        /// Основной обработчик кликов по плиткам
        /// </summary>
        TileClickResult HandleTileClick(Object^ sender, EventArgs^ e, GameLogic* gameLogic);

        /// <summary>
        /// Сбрасывает текущий выбор плиток
        /// </summary>
        Void ResetSelection();

        /// <summary>
        /// Возвращает первую выбранную плитку
        /// </summary>
        Button^ GetFirstSelectedTile();

        /// <summary>
        /// Возвращает вторую выбранную плитку
        /// </summary>
        Button^ GetSecondSelectedTile();

    private:
        /// <summary>
        /// Обрабатывает логику выбора плитки (первая, вторая, отмена)
        /// </summary>
        TileClickResult ProcessTileSelection(Button^ clickedTile);
    };

}
