#pragma once

#include <memory>
#include "Match3/Match3Game.hpp"
#include "MineSweeper/MineSweeperGame.hpp"
#include "Battleship/BattleshipGame.hpp"

namespace GameLauncher {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    /// <summary>
    /// Базовый класс для всех игровых кнопок
    /// </summary>
    public ref class GameButtonBase abstract : public Button
    {
    public:
        GameButtonBase() : Button()
        {
            // Общие настройки для всех игровых кнопок
            this->BackColor = Color::White;
            this->FlatAppearance->BorderColor = Color::Black;
            this->FlatAppearance->BorderSize = 0;
            this->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->Font = gcnew Drawing::Font(L"Segoe Print", 20.25F, FontStyle::Bold);
            this->ForeColor = Color::Black;
            this->TextAlign = ContentAlignment::BottomCenter;
            this->Padding = System::Windows::Forms::Padding(0, 0, 0, 10);
            this->Size = Drawing::Size(232, 155);

            // Обработчики событий для эффектов
            this->MouseEnter += gcnew EventHandler(this, &GameButtonBase::OnMouseEnter);
            this->MouseLeave += gcnew EventHandler(this, &GameButtonBase::OnMouseLeave);
            this->Click += gcnew EventHandler(this, &GameButtonBase::OnClick);
        }

        /// <summary>
        /// Абстрактный метод для запуска игры
        /// </summary>
        virtual void LaunchGame() abstract;

        /// <summary>
        /// Абстрактный метод для получения названия игры
        /// </summary>
        virtual String^ GetGameText() {
            return this->Text;
        }

        /// <summary>
        /// Абстрактный метод для получения идентификатора игры
        /// </summary>
        virtual String^ GetGameName() {
            return this->Name;
        }

    protected:
        /// <summary>
        /// Событие при наведении мыши
        /// </summary>
        virtual void OnMouseEnter(Object^ sender, EventArgs^ e)
        {
            this->BackColor = Color::LightGray;
        }

        /// <summary>
        /// Событие при уходе мыши
        /// </summary>
        virtual void OnMouseLeave(Object^ sender, EventArgs^ e)
        {
            this->BackColor = Color::White;
        }

        /// <summary>
        /// Событие при клике
        /// </summary>
        virtual void OnClick(Object^ sender, EventArgs^ e)
        {
            LaunchGame();
        }
    };

    /// <summary>
    /// Кнопка для запуска standalone .exe игр
    /// </summary>
    public ref class StandAloneGameButton sealed : public GameButtonBase
    {
    private:
        String^ exePath;
        String^ gameTitle;

    public:
        StandAloneGameButton(String^ title, String^ path) : GameButtonBase()
        {
            this->gameTitle = title;
            this->exePath = path;
            this->Text = title;
            this->Name = L"standAlone_" + title->Replace(L" ", L"_");
            this->Font = gcnew Drawing::Font(L"Segoe Print", 20.25F, FontStyle::Bold);
        }

        virtual void LaunchGame() override
        {
            try
            {
                System::Diagnostics::Process^ process = gcnew System::Diagnostics::Process();

                // Настраиваем параметры запуска
                process->StartInfo->FileName = exePath;
                process->StartInfo->UseShellExecute = true;
                process->StartInfo->WorkingDirectory = System::IO::Path::GetDirectoryName(exePath);

                // Запускаем процесс
                process->Start();

                // Отсоединяем процесс, чтобы он мог работать независимо
                process->Close();
            }
            catch (Exception^ ex)
            {
                MessageBox::Show(String::Format(L"Не удалось запустить игру '{0}': {1}",
                    gameTitle, ex->Message),
                    L"Ошибка запуска", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }
    };

    /// <summary>
    /// Кнопка для игры Match3
    /// </summary>
    public ref class Match3GameButton sealed : public GameButtonBase
    {
    public:
        Match3GameButton() : GameButtonBase()
        {
            this->Text = L"Match Three";
            this->Name = L"buttonGame1";
        }

        virtual void LaunchGame() override
        {
            // Создаем и показываем окно игры Match3
            Match3::Match3^ gameForm = gcnew Match3::Match3();
            gameForm->Show();

        }
    };

    /// <summary>
    /// Кнопка для второй игры (пример)
    /// </summary>
    public ref class MineSweeperGameButton sealed : public GameButtonBase
    {
    public:
        MineSweeperGameButton() : GameButtonBase()
        {
            this->Text = L"MineSweeper";
            this->Name = L"buttonGame2";
            this->Font = gcnew Drawing::Font(L"Segoe Print", 20.25F, FontStyle::Bold);
        }

        virtual void LaunchGame() override
        {
            // Создаем и показываем окно игры Match3
            MineSweeper::MineSweeperForm^ gameForm = gcnew MineSweeper::MineSweeperForm();
            gameForm->Show();

        }
    };

    /// <summary>
    /// Кнопка для третьей игры (пример)
    /// </summary>
    public ref class BattleshipGameButton sealed : public GameButtonBase
    {
    public:
        BattleshipGameButton() : GameButtonBase()
        {
            this->Text = L"Battleship";
            this->Name = L"buttonGame3";
            this->Font = gcnew Drawing::Font(L"Segoe Print", 20.25F, FontStyle::Bold);
        }

        virtual void LaunchGame() override
        {

        }
    };

    /// <summary>
    /// Кнопка для третьей игры (пример)
    /// </summary>
    public ref class Game4Button sealed : public GameButtonBase
    {
    public:
        Game4Button() : GameButtonBase()
        {
            this->Text = L"Game Four";
            this->Name = L"buttonGame3";
            this->Font = gcnew Drawing::Font(L"Segoe Print", 20.25F, FontStyle::Bold);
        }

        virtual void LaunchGame() override
        {

        }

    };

    /// <summary>
    /// Кнопка для заблокированных игр
    /// </summary>
    public ref class LockedGameButton sealed : public GameButtonBase
    {
    private:
        String^ gameTitle;

    public:
        LockedGameButton(String^ title) : GameButtonBase()
        {
            this->gameTitle = title;
            this->Text = L"Soon...";
            this->Font = gcnew Drawing::Font(L"Segoe Print", 20.25F, FontStyle::Regular);
            this->Enabled = false;
            this->BackColor = Color::LightGray;
        }

        virtual void LaunchGame() override
        {
            MessageBox::Show(String::Format(L"Game '{0}' in development!", gameTitle),
                L"Soon...", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    };
}