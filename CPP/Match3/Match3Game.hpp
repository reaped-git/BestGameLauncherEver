#pragma once

#include "GameGrid.hpp"
#include "GameLogic.hpp"
#include "InputHandler.hpp"
#include "ScoreManager.hpp"
#include "GameManagers.hpp" 

namespace Match3 {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    /// <summary>
    /// Главная форма игры "Три в ряд"
    /// Содержит игровое поле, управляет взаимодействием всех компонентов игры
    /// </summary>
    public ref class Match3 : public System::Windows::Forms::Form
    {
    public:
        /// <summary>
        /// Конструктор главной формы
        /// </summary>
        Match3();

        /// <summary>
        /// Деструктор
        /// </summary>
        ~Match3();

    private:
        // Игровые компоненты
        GameGrid^ gameGrid;           // Управление игровой сеткой
        GameLogic* gameLogic;         // Логика игры
        InputHandler^ inputHandler;   // Обработка ввода
        ScoreManager* scoreManager;   // Управление очками

        // Элементы интерфейса
        Panel^ toolBar;               // Верхняя панель с кнопками управления
        Button^ buttonClose;          // Кнопка закрытия приложения
        Button^ buttonMinimize;       // Кнопка сворачивания окна
        Label^ scoreLabel;            // Метка для отображения счета
        Panel^ background;            // Панель игрового поля

        // Переменные для перетаскивания окна
        Boolean dragging;             // Флаг перетаскивания окна
        Point start_point;            // Начальная точка перетаскивания

        /// <summary>
        /// Обязательная переменная конструктора - контейнер компонентов
        /// </summary>
        System::ComponentModel::Container^ components;

        /// <summary>
        /// Инициализация компонентов формы (автогенерируемый код)
        /// </summary>
        System::Void InitializeComponent();

        /// <summary>
        /// Настраивает обработчики событий для всех плиток
        /// </summary>
        System::Void SetupTileEventHandlers();

        /// <summary>
        /// Обработчик клика по плитке
        /// </summary>
        System::Void OnTileClicked(System::Object^ sender, System::EventArgs^ e);

        /// <summary>
        /// Обновляет отображение счета на форме
        /// </summary>
        Void UpdateScoreDisplay(String^ score);

        // Обработчики событий интерфейса
        System::Void buttonClose_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void buttonMinimize_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void buttonClose_MouseEnter(System::Object^ sender, System::EventArgs^ e);
        System::Void buttonClose_MouseLeave(System::Object^ sender, System::EventArgs^ e);
        System::Void buttonMinimize_MouseEnter(System::Object^ sender, System::EventArgs^ e);
        System::Void buttonMinimize_MouseLeave(System::Object^ sender, System::EventArgs^ e);
        System::Void toolBar_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
        System::Void toolBar_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
        System::Void toolBar_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
    };

}
