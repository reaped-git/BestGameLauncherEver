#pragma once

#include "GameButtonBase.hpp" 

namespace GameLauncher {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Collections::Generic;

    /// <summary>
    /// Главная форма лаунчера
    /// </summary>
    ref class LauncherForm : public System::Windows::Forms::Form
    {
    public:
        LauncherForm();
        ~LauncherForm();

    private:
        // Переменные для управления окном
        System::Boolean dragging;
        Point start_point;

        // Словарь для отслеживания открытых игровых окон
        Dictionary<String^, Form^>^ gameForms;

        // Элементы пользовательского интерфейса
        System::Windows::Forms::Panel^ toolBar;
        System::Windows::Forms::Button^ buttonClose;
        System::Windows::Forms::Button^ buttonMinimize;
        GameButtonBase^ buttonGame1;
        GameButtonBase^ buttonGame2;
        GameButtonBase^ buttonGame3;
        GameButtonBase^ buttonGame4;
        GameButtonBase^ buttonGame5;
        GameButtonBase^ buttonGame6;
        System::Windows::Forms::Button^ buttonSettings;
        System::Windows::Forms::Label^ scoreLabel;
        System::ComponentModel::Container^ components;

        /// <summary>
        /// Инициализация компонентов формы
        /// </summary>
        void InitializeComponent(void);

        /// <summary>
        /// Инициализация игровых кнопок с использованием полиморфизма
        /// </summary>
        void InitializeGameButtons();

        // Обработчики событий
        void buttonClose_Click(System::Object^ sender, System::EventArgs^ e);
        void buttonClose_MouseLeave(System::Object^ sender, System::EventArgs^ e);
        void buttonClose_MouseEnter(System::Object^ sender, System::EventArgs^ e);
        void buttonMinimize_Click(System::Object^ sender, System::EventArgs^ e);
        void toolBar_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
        void toolBar_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
        void toolBar_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
        void buttonGame_Click(System::Object^ sender, System::EventArgs^ e);
        void buttonSettings_Click(System::Object^ sender, System::EventArgs^ e);
    };
}