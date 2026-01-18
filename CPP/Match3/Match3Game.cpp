#include "Match3Game.hpp"

namespace Match3 {

    /// <summary>
    /// Конструктор главной формы игры
    /// </summary>
    Match3::Match3()
        : dragging(false)
        , start_point(Point(0, 0))
    {
        InitializeComponent();

        auto gameManager = std::make_shared<ExtendedGameManager>("1.0", 1, "Classic");

        // Инициализация игровых компонентов
        this->gameGrid = gcnew GameGrid(background, GameGrid::DEFAULT_GRID_SIZE, GameGrid::DEFAULT_TILE_SIZE);
        this->scoreManager = new ScoreManager();
        this->gameLogic = new GameLogic();
        this->inputHandler = gcnew InputHandler();

        // Настройка обработчиков событий для плиток
        SetupTileEventHandlers();

        // Начало игры
        gameLogic->ProcessMatches(gameGrid->GetGrid(), gameGrid->GetSize(), gameGrid);
    }

    /// <summary>
    /// Деструктор
    /// </summary>
    Match3::~Match3()
    {
        if (components)
        {
            delete components;
        }
    }

    /// <summary>
    /// Обновляет отображение счета на форме
    /// </summary>
    Void Match3::UpdateScoreDisplay(String^ score)
    {
        scoreLabel->Text = score;
    }

    /// <summary>
    /// Настраивает обработчики событий для всех плиток игрового поля
    /// </summary>
    System::Void Match3::SetupTileEventHandlers()
    {
        // Устанавливаем обработчик клика для всех плиток
        gameGrid->SetTileClickHandler(gcnew EventHandler(this, &Match3::OnTileClicked));
    }

    /// <summary>
    /// Обработчик клика по плитке
    /// </summary>
    System::Void Match3::OnTileClicked(System::Object^ sender, System::EventArgs^ e)
    {
        InputHandler::TileClickResult result = inputHandler->HandleTileClick(sender, e, gameLogic);

        switch (result)
        {
        case InputHandler::TileClickResult::FirstSelected:
            // Выделяем первую плитку
            gameGrid->ForEachTile(gcnew Action<Button^>(gameGrid, &GameGrid::ResetTileSelection));
            inputHandler->GetFirstSelectedTile()->FlatAppearance->BorderSize = 2;
            inputHandler->GetFirstSelectedTile()->FlatAppearance->BorderColor = Color::Black;
            break;

        case InputHandler::TileClickResult::SecondSelected:
            // Пытаемся обменять две выбранные плитки
        {
            Button^ tile1 = inputHandler->GetFirstSelectedTile();
            Button^ tile2 = inputHandler->GetSecondSelectedTile();

            if (tile1 != nullptr && tile2 != nullptr)
            {
                Int64 removedTiles = gameLogic->HandleTileSwap(tile1, tile2,
                    gameGrid->GetGrid(), gameGrid->GetSize(), gameGrid);

                if (removedTiles > 0)
                {
                    // Успешный обмен - добавляем очки на основе количества удаленных плиток
                    String^ score = scoreManager->AddScoreForTiles(removedTiles);
                    UpdateScoreDisplay(score);
                }
            }

            inputHandler->ResetSelection();
            gameGrid->ForEachTile(gcnew Action<Button^>(gameGrid, &GameGrid::ResetTileSelection));
        }
        break;

        case InputHandler::TileClickResult::Deselected:
            // Снимаем выделение
            gameGrid->ForEachTile(gcnew Action<Button^>(gameGrid, &GameGrid::ResetTileSelection));
            break;

        case InputHandler::TileClickResult::None:
        default:
            // Ничего не делаем
            break;
        }
    }

    /// <summary>
    /// Инициализирует компоненты формы (автогенерируемый код WinForms)
    /// </summary>
    System::Void Match3::InitializeComponent()
    {
        this->toolBar = (gcnew System::Windows::Forms::Panel());
        this->buttonClose = (gcnew System::Windows::Forms::Button());
        this->buttonMinimize = (gcnew System::Windows::Forms::Button());
        this->scoreLabel = (gcnew System::Windows::Forms::Label());
        this->background = (gcnew System::Windows::Forms::Panel());
        this->toolBar->SuspendLayout();
        this->SuspendLayout();

        // toolBar
        this->toolBar->BackColor = System::Drawing::Color::White;
        this->toolBar->Controls->Add(this->buttonClose);
        this->toolBar->Controls->Add(this->buttonMinimize);
        this->toolBar->Controls->Add(this->scoreLabel);
        this->toolBar->Location = System::Drawing::Point(1, 1);
        this->toolBar->Name = L"toolBar";
        this->toolBar->Size = System::Drawing::Size(544, 32);
        this->toolBar->TabIndex = 8;
        this->toolBar->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Match3::toolBar_MouseDown);
        this->toolBar->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Match3::toolBar_MouseMove);
        this->toolBar->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Match3::toolBar_MouseUp);

        // buttonClose
        this->buttonClose->BackColor = System::Drawing::Color::White;
        this->buttonClose->FlatAppearance->BorderColor = System::Drawing::Color::Black;
        this->buttonClose->FlatAppearance->BorderSize = 0;
        this->buttonClose->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
        this->buttonClose->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Bold));
        this->buttonClose->ForeColor = System::Drawing::Color::Black;
        this->buttonClose->Location = System::Drawing::Point(508, 0);
        this->buttonClose->Margin = System::Windows::Forms::Padding(0);
        this->buttonClose->Name = L"buttonClose";
        this->buttonClose->Size = System::Drawing::Size(36, 32);
        this->buttonClose->TabIndex = 4;
        this->buttonClose->Text = L"x";
        this->buttonClose->UseVisualStyleBackColor = false;
        this->buttonClose->Click += gcnew System::EventHandler(this, &Match3::buttonClose_Click);
        this->buttonClose->MouseEnter += gcnew System::EventHandler(this, &Match3::buttonClose_MouseEnter);
        this->buttonClose->MouseLeave += gcnew System::EventHandler(this, &Match3::buttonClose_MouseLeave);

        // buttonMinimize
        this->buttonMinimize->BackColor = System::Drawing::Color::White;
        this->buttonMinimize->FlatAppearance->BorderColor = System::Drawing::Color::Black;
        this->buttonMinimize->FlatAppearance->BorderSize = 0;
        this->buttonMinimize->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
        this->buttonMinimize->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->buttonMinimize->ForeColor = System::Drawing::Color::Black;
        this->buttonMinimize->Location = System::Drawing::Point(471, 0);
        this->buttonMinimize->Margin = System::Windows::Forms::Padding(0);
        this->buttonMinimize->Name = L"buttonMinimize";
        this->buttonMinimize->Size = System::Drawing::Size(36, 32);
        this->buttonMinimize->TabIndex = 5;
        this->buttonMinimize->Text = L"_";
        this->buttonMinimize->UseVisualStyleBackColor = false;
        this->buttonMinimize->Click += gcnew System::EventHandler(this, &Match3::buttonMinimize_Click);
        this->buttonMinimize->MouseEnter += gcnew System::EventHandler(this, &Match3::buttonMinimize_MouseEnter);
        this->buttonMinimize->MouseLeave += gcnew System::EventHandler(this, &Match3::buttonMinimize_MouseLeave);

        // scoreLabel
        this->scoreLabel->Font = (gcnew System::Drawing::Font(L"Impact", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->scoreLabel->ForeColor = System::Drawing::Color::Black;
        this->scoreLabel->Location = System::Drawing::Point(2, 2);
        this->scoreLabel->Margin = System::Windows::Forms::Padding(0);
        this->scoreLabel->Name = L"scoreLabel";
        this->scoreLabel->Size = System::Drawing::Size(200, 28);
        this->scoreLabel->TabIndex = 0;
        this->scoreLabel->Text = L"Score: 0";
        this->scoreLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        this->scoreLabel->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Match3::toolBar_MouseDown);
        this->scoreLabel->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Match3::toolBar_MouseMove);
        this->scoreLabel->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Match3::toolBar_MouseUp);

        // background
        this->background->BackColor = System::Drawing::Color::Black;
        this->background->ForeColor = System::Drawing::Color::Black;
        this->background->Location = System::Drawing::Point(1, 35);
        this->background->Name = L"background";
        this->background->Size = System::Drawing::Size(544, 544);
        this->background->TabIndex = 10;

        // Match3
        this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
        this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
        this->BackColor = System::Drawing::Color::Black;
        this->ClientSize = System::Drawing::Size(546, 580);
        this->Controls->Add(this->background);
        this->Controls->Add(this->toolBar);
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
        this->Name = L"Match3";
        this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
        this->Text = L"Match3";
        this->toolBar->ResumeLayout(false);
        this->ResumeLayout(false);
    }

    /// <summary>
    /// Обработчик нажатия кнопки закрытия приложения
    /// </summary>
    System::Void Match3::buttonClose_Click(System::Object^ sender, System::EventArgs^ e)
    {
        this->Close();
    }

    /// <summary>
    /// Обработчик нажатия кнопки сворачивания окна
    /// </summary>
    System::Void Match3::buttonMinimize_Click(System::Object^ sender, System::EventArgs^ e)
    {
        this->WindowState = FormWindowState::Minimized;
    }

    /// <summary>
    /// Обработчик наведения курсора на кнопку закрытия
    /// </summary>
    System::Void Match3::buttonClose_MouseEnter(System::Object^ sender, System::EventArgs^ e)
    {
        this->buttonClose->BackColor = Color::LightCoral;
    }

    /// <summary>
    /// Обработчик ухода курсора с кнопки закрытия
    /// </summary>
    System::Void Match3::buttonClose_MouseLeave(System::Object^ sender, System::EventArgs^ e)
    {
        this->buttonClose->BackColor = Color::White;
    }

    /// <summary>
    /// Обработчик наведения курсора на кнопку закрытия
    /// </summary>
    System::Void Match3::buttonMinimize_MouseEnter(System::Object^ sender, System::EventArgs^ e)
    {
        this->buttonMinimize->BackColor = Color::LightGray;
    }

    /// <summary>
    /// Обработчик ухода курсора с кнопки закрытия
    /// </summary>
    System::Void Match3::buttonMinimize_MouseLeave(System::Object^ sender, System::EventArgs^ e)
    {
        this->buttonMinimize->BackColor = Color::White;
    }

    /// <summary>
    /// Обработчик начала перетаскивания окна
    /// </summary>
    System::Void Match3::toolBar_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
    {
        this->dragging = true;
        this->start_point = Point(e->X, e->Y);
    }

    /// <summary>
    /// Обработчик перемещения окна при перетаскивании
    /// </summary>
    System::Void Match3::toolBar_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
    {
        if (this->dragging)
        {
            Point p = PointToScreen(Point(e->X, e->Y));
            this->Location = Point(p.X - this->start_point.X, p.Y - this->start_point.Y);
        }
    }

    /// <summary>
    /// Обработчик окончания перетаскивания окна
    /// </summary>
    System::Void Match3::toolBar_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
    {
        this->dragging = false;
    }

}
