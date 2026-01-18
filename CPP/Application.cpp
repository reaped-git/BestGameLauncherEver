#include "Application.hpp"

namespace GameLauncher {

    /// <summary>
    /// Конструктор формы лаунчера
    /// </summary>
    LauncherForm::LauncherForm()
        : dragging(false)
        , start_point(Point(0, 0))
    {
        InitializeComponent();
        InitializeGameButtons();
    }

    /// <summary>
    /// Деструктор формы лаунчера
    /// </summary>
    LauncherForm::~LauncherForm()
    {
        if (components)
        {
            delete components;
        }
    }
    /// <summary>
    /// Инициализация игровых кнопок с использованием полиморфизма
    /// </summary>
    void LauncherForm::InitializeGameButtons()
    {
        // Создаем кнопки с использованием конкретных классов
        buttonGame1 = gcnew Match3GameButton();
        buttonGame2 = gcnew MineSweeperGameButton();
        buttonGame3 = gcnew StandAloneGameButton(L"Battleship", L"Battleship.exe");
        buttonGame4 = gcnew StandAloneGameButton(L"Blackjack", L"Blackjack.exe");

        // Заблокированные игры
        buttonGame5 = gcnew LockedGameButton(L"Игра 5");
        buttonGame6 = gcnew LockedGameButton(L"Игра 6");

        // Устанавливаем позиции
        buttonGame1->Location = Drawing::Point(1, 34);
        buttonGame2->Location = Drawing::Point(234, 34);
        buttonGame3->Location = Drawing::Point(467, 34);
        buttonGame4->Location = Drawing::Point(1, 190);
        buttonGame5->Location = Drawing::Point(234, 190);
        buttonGame6->Location = Drawing::Point(467, 190);

        // Добавляем кнопки на форму
        this->Controls->Add(buttonGame1);
        this->Controls->Add(buttonGame2);
        this->Controls->Add(buttonGame3);
        this->Controls->Add(buttonGame4);
        this->Controls->Add(buttonGame5);
        this->Controls->Add(buttonGame6);
    }


    /// <summary>
    /// Обработчик закрытия приложения
    /// </summary>
    void LauncherForm::buttonClose_Click(System::Object^ sender, System::EventArgs^ e)
    {
        Application::Exit();
    }

    void LauncherForm::buttonClose_MouseEnter(System::Object^ sender, System::EventArgs^ e)
    {
        this->buttonClose->BackColor = Color::LightCoral;
    }

    void LauncherForm::buttonClose_MouseLeave(System::Object^ sender, System::EventArgs^ e)
    {
        this->buttonClose->BackColor = Color::White;
    }

    /// <summary>
    /// Обработчик сворачивания окна
    /// </summary>
    void LauncherForm::buttonMinimize_Click(System::Object^ sender, System::EventArgs^ e)
    {
        this->WindowState = System::Windows::Forms::FormWindowState::Minimized;
    }

    /// <summary>
    /// Обработчики перетаскивания окна
    /// </summary>
    void LauncherForm::toolBar_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
    {
        dragging = true;
        start_point = Point(e->X, e->Y);
    }

    void LauncherForm::toolBar_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
    {
        if (dragging) {
            Point p = PointToScreen(e->Location);
            Location = Point(p.X - this->start_point.X, p.Y - this->start_point.Y);
        }
    }

    void LauncherForm::toolBar_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
    {
        dragging = false;
    }

    /// <summary>
    /// Обработчик клика по игровой кнопке
    /// </summary>
    void LauncherForm::buttonGame_Click(System::Object^ sender, System::EventArgs^ e)
    {
        Button^ btn = safe_cast<Button^>(sender);
        String^ gameKey = btn->Name;

    }

    /// <summary>
    /// Обработчик кнопки настроек
    /// </summary>
    void LauncherForm::buttonSettings_Click(System::Object^ sender, System::EventArgs^ e)
    {
        // SettingsManager^ form = gcnew SettingsManager();
        // form->ShowDialog();
        MessageBox::Show(L"Настройки еще не реализованы!");
    }

    /// <summary>
    /// Инициализация компонентов формы лаунчера
    /// </summary>
    void LauncherForm::InitializeComponent(void)
    {
        System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(LauncherForm::typeid));
        this->toolBar = (gcnew System::Windows::Forms::Panel());
        this->scoreLabel = (gcnew System::Windows::Forms::Label());
        this->buttonSettings = (gcnew System::Windows::Forms::Button());
        this->buttonClose = (gcnew System::Windows::Forms::Button());
        this->buttonMinimize = (gcnew System::Windows::Forms::Button());
        this->buttonGame1 = nullptr;
        this->buttonGame2 = nullptr;
        this->buttonGame3 = nullptr;
        this->buttonGame4 = nullptr;
        this->buttonGame5 = nullptr;
        this->buttonGame6 = nullptr;
        this->toolBar->SuspendLayout();
        this->SuspendLayout();

        // toolBar
        this->toolBar->BackColor = System::Drawing::Color::White;
        this->toolBar->Controls->Add(this->scoreLabel);
        this->toolBar->Controls->Add(this->buttonSettings);
        this->toolBar->Controls->Add(this->buttonClose);
        this->toolBar->Controls->Add(this->buttonMinimize);
        this->toolBar->Location = System::Drawing::Point(1, 1);
        this->toolBar->Name = L"toolBar";
        this->toolBar->Size = System::Drawing::Size(698, 32);
        this->toolBar->TabIndex = 9;
        this->toolBar->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &LauncherForm::toolBar_MouseDown);
        this->toolBar->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &LauncherForm::toolBar_MouseMove);
        this->toolBar->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &LauncherForm::toolBar_MouseUp);

        // scoreLabel
        this->scoreLabel->Font = (gcnew System::Drawing::Font(L"Impact", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->scoreLabel->ForeColor = System::Drawing::Color::Black;
        this->scoreLabel->Location = System::Drawing::Point(0, 1);
        this->scoreLabel->Margin = System::Windows::Forms::Padding(0);
        this->scoreLabel->Name = L"scoreLabel";
        this->scoreLabel->Size = System::Drawing::Size(200, 28);
        this->scoreLabel->TabIndex = 15;
        this->scoreLabel->Text = L"Games";
        this->scoreLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;

        // buttonSettings
        this->buttonSettings->BackColor = System::Drawing::Color::White;
        this->buttonSettings->FlatAppearance->BorderColor = System::Drawing::Color::Black;
        this->buttonSettings->FlatAppearance->BorderSize = 0;
        this->buttonSettings->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
        this->buttonSettings->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->buttonSettings->ForeColor = System::Drawing::Color::Black;
        this->buttonSettings->Location = System::Drawing::Point(588, 0);
        this->buttonSettings->Margin = System::Windows::Forms::Padding(0);
        this->buttonSettings->Name = L"buttonSettings";
        this->buttonSettings->Size = System::Drawing::Size(36, 32);
        this->buttonSettings->TabIndex = 6;
        this->buttonSettings->Text = L"*";
        this->buttonSettings->UseVisualStyleBackColor = false;
        this->buttonSettings->Click += gcnew System::EventHandler(this, &LauncherForm::buttonSettings_Click);

        // buttonClose
        this->buttonClose->BackColor = System::Drawing::Color::White;
        this->buttonClose->FlatAppearance->BorderColor = System::Drawing::Color::Black;
        this->buttonClose->FlatAppearance->BorderSize = 0;
        this->buttonClose->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
        this->buttonClose->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Bold));
        this->buttonClose->ForeColor = System::Drawing::Color::Black;
        this->buttonClose->Location = System::Drawing::Point(662, 0);
        this->buttonClose->Margin = System::Windows::Forms::Padding(0);
        this->buttonClose->Name = L"buttonClose";
        this->buttonClose->Size = System::Drawing::Size(36, 32);
        this->buttonClose->TabIndex = 4;
        this->buttonClose->Text = L"x";
        this->buttonClose->UseVisualStyleBackColor = false;
        this->buttonClose->Click += gcnew System::EventHandler(this, &LauncherForm::buttonClose_Click);
        this->buttonClose->MouseEnter += gcnew System::EventHandler(this, &LauncherForm::buttonClose_MouseEnter);
        this->buttonClose->MouseLeave += gcnew System::EventHandler(this, &LauncherForm::buttonClose_MouseLeave);

        // buttonMinimize
        this->buttonMinimize->BackColor = System::Drawing::Color::White;
        this->buttonMinimize->FlatAppearance->BorderColor = System::Drawing::Color::Black;
        this->buttonMinimize->FlatAppearance->BorderSize = 0;
        this->buttonMinimize->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
        this->buttonMinimize->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->buttonMinimize->ForeColor = System::Drawing::Color::Black;
        this->buttonMinimize->Location = System::Drawing::Point(625, 0);
        this->buttonMinimize->Margin = System::Windows::Forms::Padding(0);
        this->buttonMinimize->Name = L"buttonMinimize";
        this->buttonMinimize->Size = System::Drawing::Size(36, 32);
        this->buttonMinimize->TabIndex = 5;
        this->buttonMinimize->Text = L"_";
        this->buttonMinimize->UseVisualStyleBackColor = false;
        this->buttonMinimize->Click += gcnew System::EventHandler(this, &LauncherForm::buttonMinimize_Click);

        // LauncherForm
        this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
        this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
        this->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
        this->ClientSize = System::Drawing::Size(700, 346);
        this->Controls->Add(this->buttonGame6);
        this->Controls->Add(this->buttonGame5);
        this->Controls->Add(this->buttonGame4);
        this->Controls->Add(this->buttonGame3);
        this->Controls->Add(this->buttonGame2);
        this->Controls->Add(this->buttonGame1);
        this->Controls->Add(this->toolBar);
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
        this->Name = L"LauncherForm";
        this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
        this->Text = L"Games Launcher";
        this->toolBar->ResumeLayout(false);
        this->ResumeLayout(false);
    }

}