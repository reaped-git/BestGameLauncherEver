#include "MineSweeperGame.hpp"

namespace MineSweeper {

    MineSweeperForm::MineSweeperForm()
        : mineField(nullptr), gameTimer(nullptr), difficultyManager(nullptr),
        inputHandler(nullptr), gameState(nullptr), cellButtons(nullptr),
        updatingDifficulty(false)
    {
        try {
            InitializeComponent();
            InitializeGame();
        }
        catch (Exception^ ex) {
            MessageBox::Show(
                String::Format(L"Ошибка при создании формы:\n{0}", ex->Message),
                L"Ошибка",
                MessageBoxButtons::OK,
                MessageBoxIcon::Error);
            throw;
        }
        catch (const std::exception& ex) {
            String^ message = ToSystemString("Ошибка C++ при создании формы: " + std::string(ex.what()));
            MessageBox::Show(message, L"Ошибка C++", MessageBoxButtons::OK, MessageBoxIcon::Error);
            throw gcnew Exception(message);
        }
        catch (...) {
            MessageBox::Show(
                L"Неизвестная ошибка при создании формы",
                L"Ошибка",
                MessageBoxButtons::OK,
                MessageBoxIcon::Error);
            throw gcnew Exception(L"Неизвестная ошибка при создании формы");
        }
    }

    MineSweeperForm::~MineSweeperForm()
    {
        if (components)
        {
            delete components;
        }

        // Очистка чистых C++ объектов
        delete mineField;
        delete gameTimer;
        delete difficultyManager;
        delete inputHandler;
        delete gameState;
    }

    System::String^ MineSweeperForm::ToSystemString(const std::string& str)
    {
        return gcnew System::String(str.c_str());
    }

    System::Void MineSweeperForm::InitializeComponent()
    {
        this->components = (gcnew System::ComponentModel::Container());

        this->buttonRestart = (gcnew System::Windows::Forms::Button());
        this->comboBoxDifficulty = (gcnew System::Windows::Forms::ComboBox());
        this->labelTimer = (gcnew System::Windows::Forms::Label());
        this->gamePanel = (gcnew System::Windows::Forms::Panel());
        this->updateTimer = (gcnew System::Windows::Forms::Timer(this->components));

        this->SuspendLayout();

        // buttonRestart
        this->buttonRestart->Location = System::Drawing::Point(12, 12);
        this->buttonRestart->Name = L"buttonRestart";
        this->buttonRestart->Size = System::Drawing::Size(80, 30);
        this->buttonRestart->TabIndex = 0;
        this->buttonRestart->Text = L"Рестарт";
        this->buttonRestart->UseVisualStyleBackColor = true;
        this->buttonRestart->Click += gcnew System::EventHandler(this, &MineSweeperForm::buttonRestart_Click);

        // comboBoxDifficulty
        this->comboBoxDifficulty->FormattingEnabled = true;
        this->comboBoxDifficulty->Location = System::Drawing::Point(98, 15);
        this->comboBoxDifficulty->Name = L"comboBoxDifficulty";
        this->comboBoxDifficulty->Size = System::Drawing::Size(120, 21);
        this->comboBoxDifficulty->TabIndex = 1;
        this->comboBoxDifficulty->SelectedIndexChanged += gcnew System::EventHandler(this, &MineSweeperForm::comboBoxDifficulty_SelectedIndexChanged);

        // labelTimer
        this->labelTimer->Font = (gcnew System::Drawing::Font(L"Arial Narrow", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->labelTimer->Location = System::Drawing::Point(224, 12);
        this->labelTimer->Name = L"labelTimer";
        this->labelTimer->Size = System::Drawing::Size(120, 30);
        this->labelTimer->TabIndex = 2;
        this->labelTimer->Text = L"Время: 10:00";
        this->labelTimer->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;

        // gamePanel
        this->gamePanel->Location = System::Drawing::Point(12, 48);
        this->gamePanel->Name = L"gamePanel";
        this->gamePanel->Size = System::Drawing::Size(516, 516);
        this->gamePanel->TabIndex = 3;
        this->gamePanel->BackColor = System::Drawing::Color::LightGray;

        // updateTimer
        this->updateTimer->Interval = 1000;
        this->updateTimer->Tick += gcnew System::EventHandler(this, &MineSweeperForm::updateTimer_Tick);

        // MineSweeperForm
        this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
        this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
        this->ClientSize = System::Drawing::Size(540, 576);
        this->Controls->Add(this->gamePanel);
        this->Controls->Add(this->labelTimer);
        this->Controls->Add(this->comboBoxDifficulty);
        this->Controls->Add(this->buttonRestart);
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
        this->MaximizeBox = false;
        this->Name = L"MineSweeperForm";
        this->Text = L"Сапёр";
        this->ResumeLayout(false);
    }

    System::Void MineSweeperForm::InitializeGame()
    {
        try {
            if (!difficultyManager) {
                difficultyManager = new DifficultyManager();
            }

            // Заполнение комбобокса сложностями
            auto difficultyNames = difficultyManager->GetDifficultyNames();

            // Временно отключаем обработчик события
            updatingDifficulty = true;
            comboBoxDifficulty->Items->Clear();
            for (const auto& name : difficultyNames) {
                comboBoxDifficulty->Items->Add(ToSystemString(name));
            }
            // Используем текущую сложность из менеджера, а не сбрасываем
            comboBoxDifficulty->SelectedIndex = difficultyManager->GetCurrentDifficulty();
            updatingDifficulty = false;

            // Получение текущих настроек
            auto settings = difficultyManager->GetCurrentSettings();

            // Инициализация игровых компонентов
            // Удаляем старое поле перед созданием нового
            delete mineField;
            mineField = new MineField(settings.gridSize, settings.minesCount);

            // Пересоздаем таймер с новыми настройками
            delete gameTimer;
            gameTimer = new GameTimer(settings.timeLimit);

            // Пересоздаем остальные компоненты
            delete inputHandler;
            inputHandler = new InputHandler();

            delete gameState;
            gameState = new GameState();

            // Создание клеток интерфейса
            CreateCellButtons();

            // Начало игры
            gameState->StartNewGame();
            gameTimer->Start();
            updateTimer->Start();

            // Обновление дисплея
            UpdateGameDisplay();
            UpdateTimerDisplay();
        }
        catch (const std::exception& ex) {
            updatingDifficulty = false;
            String^ message = ToSystemString("Ошибка при инициализации игры: " + std::string(ex.what()));
            MessageBox::Show(message, L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            throw;
        }
        catch (...) {
            updatingDifficulty = false;
            throw;
        }
    }

    System::Void MineSweeperForm::CreateCellButtons()
    {
        if (cellButtons != nullptr)
        {
            for each (Button ^ btn in cellButtons)
            {
                if (btn != nullptr)
                {
                    gamePanel->Controls->Remove(btn);
                    delete btn;
                }
            }
        }

        if (mineField == nullptr) return;

        int gridSize = mineField->GetGridSize();
        cellButtons = gcnew array<Button^, 2>(gridSize, gridSize);
        int cellSize = gamePanel->Width / gridSize;

        for (int r = 0; r < gridSize; r++)
        {
            for (int c = 0; c < gridSize; c++)
            {
                Button^ btn = gcnew Button();
                btn->Size = Drawing::Size(cellSize, cellSize);
                btn->Location = Drawing::Point(cellSize * c, cellSize * r);
                btn->Tag = Point(r, c);
                btn->Font = gcnew Drawing::Font("Arial", 10, Drawing::FontStyle::Bold);
                btn->MouseUp += gcnew MouseEventHandler(this, &MineSweeperForm::cellButton_MouseUp); // Изменили на MouseUp
                btn->MouseDown += gcnew MouseEventHandler(this, &MineSweeperForm::cellButton_MouseDown); // Добавили MouseDown

                gamePanel->Controls->Add(btn);
                cellButtons[r, c] = btn;
            }
        }
    }

    System::Void MineSweeperForm::cellButton_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
    {
        // Блокируем стандартное поведение правой кнопки на кнопках
        if (e->Button == System::Windows::Forms::MouseButtons::Right)
        {
            // Не делаем ничего, чтобы предотвратить стандартное поведение
        }
    }

    System::Void MineSweeperForm::cellButton_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
    {
        try {
            if (gameState == nullptr || !gameState->IsGameActive() || mineField == nullptr)
                return;

            Button^ btn = safe_cast<Button^>(sender);
            Point point = safe_cast<Point>(btn->Tag);
            int row = point.X;
            int col = point.Y;

            // Используем MouseUp вместо MouseClick
            if (e->Button == System::Windows::Forms::MouseButtons::Right)
            {
                // Правая кнопка - установка/снятие флага
                mineField->ToggleFlag(row, col);
                UpdateGameDisplay();
            }
            else if (e->Button == System::Windows::Forms::MouseButtons::Left)
            {
                // Левая кнопка - открытие клетки
                if (!mineField->IsFlagged(row, col))
                {
                    bool success = mineField->OpenCell(row, col);
                    if (!success) // Наступили на мину
                    {
                        gameState->ProcessCellOpen(true);
                        HandleGameOver(GameState::GameStatus::Lost);
                    }
                    else
                    {
                        // Проверяем победу после успешного открытия
                        if (mineField->CheckWin())
                        {
                            gameState->CheckGameCompletion(true, false);
                            HandleGameOver(GameState::GameStatus::Won);
                        }
                    }
                    UpdateGameDisplay();
                }
            }
        }
        catch (const std::exception& ex) {
            String^ message = ToSystemString("Ошибка при клике по клетке: " + std::string(ex.what()));
            MessageBox::Show(message, L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
        catch (Exception^ ex) {
            MessageBox::Show("Ошибка при клике по клетке: " + ex->Message, L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }

    System::Void MineSweeperForm::UpdateGameDisplay()
    {
        if (mineField == nullptr || cellButtons == nullptr) return;

        int gridSize = mineField->GetGridSize();

        for (int r = 0; r < gridSize; r++)
        {
            for (int c = 0; c < gridSize; c++)
            {
                Button^ btn = cellButtons[r, c];
                auto cellState = mineField->GetCellState(r, c);

                if (cellState.isOpened)
                {
                    btn->Enabled = false;
                    btn->BackColor = Color::LightGray;

                    if (cellState.isMine)
                    {
                        btn->Text = L"💣";
                        btn->BackColor = Color::Red;
                    }
                    else if (cellState.minesAround > 0)
                    {
                        btn->Text = cellState.minesAround.ToString();
                        // Установка цвета в зависимости от количества мин
                        array<Color>^ colors = gcnew array<Color> {
                            Color::Blue, Color::Green, Color::Red,
                                Color::DarkBlue, Color::DarkRed, Color::Teal,
                                Color::Black, Color::Gray
                        };

                        if (cellState.minesAround <= colors->Length)
                        {
                            btn->ForeColor = colors[cellState.minesAround - 1];
                        }
                    }
                }
                else
                {
                    btn->Enabled = (gameState != nullptr && gameState->IsGameActive());
                    btn->BackColor = Color::White;
                    btn->Text = cellState.isFlagged ? L"🚩" : L"";
                }
            }
        }
    }

    System::Void MineSweeperForm::UpdateTimerDisplay()
    {
        if (gameTimer == nullptr || labelTimer == nullptr) return;

        std::string timeStr = GameTimer::FormatTime(gameTimer->GetTimeLeft());
        labelTimer->Text = ToSystemString("Время: " + timeStr);
    }

    System::Void MineSweeperForm::HandleGameOver(GameState::GameStatus status)
    {
        if (updateTimer != nullptr) updateTimer->Stop();
        if (gameTimer != nullptr) gameTimer->Stop();

        // Показываем все мины
        if (mineField != nullptr && cellButtons != nullptr)
        {
            for (int r = 0; r < mineField->GetGridSize(); r++)
            {
                for (int c = 0; c < mineField->GetGridSize(); c++)
                {
                    if (mineField->IsMine(r, c))
                    {
                        cellButtons[r, c]->Text = L"💣";
                        cellButtons[r, c]->BackColor = mineField->IsOpened(r, c) ? Color::Red : Color::LightGray;
                    }
                }
            }
        }

        std::string message = GameState::GetStatusDescription(status);
        MessageBox::Show(ToSystemString(message), L"Игра окончена", MessageBoxButtons::OK, MessageBoxIcon::Information);
    }

    System::Void MineSweeperForm::buttonRestart_Click(System::Object^ sender, System::EventArgs^ e)
    {
        try {
            // Останавливаем таймеры
            if (updateTimer != nullptr) updateTimer->Stop();
            if (gameTimer != nullptr) gameTimer->Stop();

            // Удаляем старые объекты (НО НЕ УДАЛЯЕМ difficultyManager!)
            delete mineField;
            delete gameTimer;
            // delete difficultyManager; // ЭТУ СТРОКУ КОММЕНТИРУЕМ ИЛИ УДАЛЯЕМ
            delete inputHandler;
            delete gameState;

            // Сбрасываем указатели
            mineField = nullptr;
            gameTimer = nullptr;
            // difficultyManager = nullptr; // ЭТУ СТРОКУ КОММЕНТИРУЕМ ИЛИ УДАЛЯЕМ
            inputHandler = nullptr;
            gameState = nullptr;

            // Инициализируем заново (с сохраненной сложностью)
            InitializeGame();
        }
        catch (const std::exception& ex) {
            String^ message = ToSystemString("Ошибка при перезапуске игры: " + std::string(ex.what()));
            MessageBox::Show(message, L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
        catch (Exception^ ex) {
            MessageBox::Show("Ошибка при перезапуске игры: " + ex->Message, L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }

    System::Void MineSweeperForm::comboBoxDifficulty_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
    {
        // Игнорируем события, если мы сами обновляем комбобокс
        if (updatingDifficulty) return;

        try {
            if (difficultyManager != nullptr && comboBoxDifficulty->SelectedIndex >= 0)
            {
                difficultyManager->SetDifficulty(comboBoxDifficulty->SelectedIndex);
                // Просто перезапускаем игру, не меняя комбобокс
                buttonRestart_Click(sender, e);
            }
        }
        catch (const std::exception& ex) {
            String^ message = ToSystemString("Ошибка при изменении сложности: " + std::string(ex.what()));
            MessageBox::Show(message, L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
        catch (Exception^ ex) {
            MessageBox::Show("Ошибка при изменении сложности: " + ex->Message, L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }

    System::Void MineSweeperForm::updateTimer_Tick(System::Object^ sender, System::EventArgs^ e)
    {
        try {
            if (gameTimer == nullptr || gameState == nullptr || mineField == nullptr) return;

            gameTimer->Update();
            UpdateTimerDisplay();

            // Проверяем победу
            if (gameState->IsGameActive() && mineField->CheckWin())
            {
                gameState->CheckGameCompletion(true, false);
                HandleGameOver(GameState::GameStatus::Won);
            }

            // Проверяем окончание времени
            if (gameState->IsGameActive() && gameTimer->IsTimeUp())
            {
                gameState->CheckGameCompletion(false, true);
                HandleGameOver(GameState::GameStatus::TimeUp);
            }
        }
        catch (const std::exception& ex) {
            String^ message = ToSystemString("Ошибка в таймере: " + std::string(ex.what()));
            MessageBox::Show(message, L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            if (updateTimer != nullptr) updateTimer->Stop();
        }
        catch (Exception^ ex) {
            MessageBox::Show("Ошибка в таймере: " + ex->Message, L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            if (updateTimer != nullptr) updateTimer->Stop();
        }
    }

    System::Void MineSweeperForm::cellButton_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
    {
        try {
            if (gameState == nullptr || !gameState->IsGameActive() || mineField == nullptr)
                return;

            Button^ btn = safe_cast<Button^>(sender);
            Point point = safe_cast<Point>(btn->Tag);
            int row = point.X;
            int col = point.Y;

            InputHandler::MouseButton button = (e->Button == System::Windows::Forms::MouseButtons::Right) ?
                InputHandler::MouseButton::Right : InputHandler::MouseButton::Left;

            if (button == InputHandler::MouseButton::Right)
            {
                mineField->ToggleFlag(row, col);
            }
            else
            {
                if (!mineField->IsFlagged(row, col))
                {
                    bool success = mineField->OpenCell(row, col);
                    if (!success) // Наступили на мину
                    {
                        gameState->ProcessCellOpen(true);
                        HandleGameOver(GameState::GameStatus::Lost);
                    }
                }
            }

            UpdateGameDisplay();
        }
        catch (const std::exception& ex) {
            String^ message = ToSystemString("Ошибка при клике по клетке: " + std::string(ex.what()));
            MessageBox::Show(message, L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
        catch (Exception^ ex) {
            MessageBox::Show("Ошибка при клике по клетке: " + ex->Message, L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }
}