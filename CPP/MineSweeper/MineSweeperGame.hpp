#pragma once

#include "MineField.hpp"
#include "GameTimer.hpp"
#include "DifficultyManager.hpp"
#include "InputHandler.hpp"
#include "GameState.hpp"

namespace MineSweeper {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    /// <summary>
    /// Главная форма игры "Сапёр"
    /// Управляет всеми компонентами игры и интерфейсом
    /// </summary>
    public ref class MineSweeperForm : public System::Windows::Forms::Form
    {
    public:
        /// <summary>
        /// Конструктор главной формы
        /// </summary>
        MineSweeperForm();

        /// <summary>
        /// Деструктор
        /// </summary>
        ~MineSweeperForm();

    private:
        // Игровые компоненты (чистый C++)
        MineField* mineField;
        GameTimer* gameTimer;
        DifficultyManager* difficultyManager;
        InputHandler* inputHandler;
        GameState* gameState;

        // Элементы интерфейса (WinForms)
        Button^ buttonRestart;
        ComboBox^ comboBoxDifficulty;
        Label^ labelTimer;
        Panel^ gamePanel;
        array<Button^, 2>^ cellButtons;

        /// <summary>
        /// Флаг для предотвращения рекурсии при изменении сложности
        /// </summary>
        bool updatingDifficulty;

        /// <summary>
        /// Обязательная переменная конструктора - контейнер компонентов
        /// </summary>
        System::ComponentModel::Container^ components;

        /// <summary>
        /// Таймер обновления игры
        /// </summary>
        System::Windows::Forms::Timer^ updateTimer;

        /// <summary>
        /// Инициализация компонентов формы
        /// </summary>
        System::Void InitializeComponent();

        /// <summary>
        /// Инициализация игры
        /// </summary>
        System::Void InitializeGame();

        /// <summary>
        /// Создает клетки на игровом поле
        /// </summary>
        System::Void CreateCellButtons();
        System::Void cellButton_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
        System::Void cellButton_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);

        /// <summary>
        /// Обновляет отображение игрового поля
        /// </summary>
        System::Void UpdateGameDisplay();

        // Обработчики событий интерфейса
        System::Void buttonRestart_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void comboBoxDifficulty_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
        System::Void updateTimer_Tick(System::Object^ sender, System::EventArgs^ e);
        System::Void cellButton_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);

        /// <summary>
        /// Обновляет отображение таймера
        /// </summary>
        System::Void UpdateTimerDisplay();

        /// <summary>
        /// Обрабатывает завершение игры
        /// </summary>
        System::Void HandleGameOver(GameState::GameStatus status);

        /// <summary>
        /// Преобразует std::string в System::String^
        /// </summary>
        static System::String^ ToSystemString(const std::string& str);
    };
}