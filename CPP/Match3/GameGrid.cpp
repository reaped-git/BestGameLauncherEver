#include "GameGrid.hpp"

namespace Match3 {

    /// <summary>
    /// Конструктор игровой сетки
    /// </summary>
    /// <param name="background">Панель для размещения плиток</param>
    /// <param name="size">Размер сетки</param>
    /// <param name="tileSize">Размер одной плитки</param>
    GameGrid::GameGrid(Panel^ background, Int64 size, Int64 tileSize)
    {
        backgroundPanel = background;
        gridSize = size;
        this->tileSize = tileSize;
        random = gcnew Random();
        grid = gcnew array<Button^, 2>(gridSize, gridSize);

        // Инициализация статической цветовой палитры по умолчанию
        colorPalette = gcnew array<Color>{
                Color::FromArgb(158, 25, 66),    // Темно-красный
                Color::FromArgb(244, 109, 67),   // Оранжевый
                Color::FromArgb(254, 224, 139),  // Светло-желтый
                Color::FromArgb(147, 219, 135),  // Светло-зеленый
                Color::FromArgb(50, 136, 189),   // Синий
                Color::FromArgb(94, 79, 162)     // Фиолетовый
        };


        InitializeGrid();
    }

    /// <summary>
    /// Инициализирует сетку - создает и размещает все плитки
    /// </summary>
    Void GameGrid::InitializeGrid()
    {
        for (Int64 i = 0; i < gridSize; i++)
        {
            for (Int64 j = 0; j < gridSize; j++)
            {
                CreateTile(i, j);
            }
        }
    }

    /// <summary>
    /// Генерирует случайный цвет для плитки из предопределенной палитры
    /// </summary>
    Color GameGrid::GetRandomColor()
    {
        return colorPalette[random->Next(colorPalette->Length)];
    }

    /// <summary>
    /// Меняет цвета двух плиток местами
    /// </summary>
    Void GameGrid::SwapTiles(Button^ a, Button^ b)
    {
        Color temp = a->BackColor;
        a->BackColor = b->BackColor;
        b->BackColor = temp;
    }

    /// <summary>
    /// Заполняет пустые (прозрачные) плитки случайными цветами
    /// </summary>
    Void GameGrid::FillEmptyTile(Button^ btn)
    {
        if (btn->BackColor == Color::Transparent)
        {
            btn->BackColor = GetRandomColor();
        }
    }

    /// <summary>
    /// Создает отдельную плитку в указанной позиции сетки
    /// </summary>
    /// <param name="i">Строка в сетке</param>
    /// <param name="j">Столбец в сетке</param>
    Void GameGrid::CreateTile(Int64 i, Int64 j)
    {
        Button^ btn = gcnew Button();
        btn->Size = Drawing::Size(tileSize, tileSize);
        btn->Location = Drawing::Point(j * tileSize, i * tileSize);
        btn->FlatStyle = FlatStyle::Flat;
        btn->Tag = Point(i, j);
        btn->FlatAppearance->BorderColor = Color::Black;
        btn->FlatAppearance->BorderSize = 1;
        btn->BackColor = GetRandomColor();
        btn->Text = "";
        btn->Name = "Tile_" + i + "_" + j;

        backgroundPanel->Controls->Add(btn);
        grid[i, j] = btn;
    }

    /// <summary>
    /// Устанавливает обработчик клика для всех плиток
    /// </summary>
    Void GameGrid::SetTileClickHandler(EventHandler^ handler)
    {
        for each (Button ^ tile in grid)
        {
            if (tile == nullptr) continue;
            tile->Click += handler;
        }
    }

    /// <summary>
    /// Сбрасывает выделение всех плиток
    /// </summary>
    Void GameGrid::ResetTileSelection(Button^ btn)
    {
        if (btn != nullptr)
        {
            btn->FlatAppearance->BorderSize = 1;
        }
    }

    Void GameGrid::ForEachTile(Action<Button^>^ action)
    {
        for each (Button ^ tile in grid)
        {
            action(tile);
        }
    }


}
