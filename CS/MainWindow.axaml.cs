using Avalonia;
using Avalonia.Controls;
using Avalonia.Input;
using Avalonia.Interactivity;
using Avalonia.Markup.Xaml;
using Avalonia.Media;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;

namespace GameLauncherCS
{
    public partial class MainWindow : Window
    {
        // Словарь для отслеживания открытых игровых окон
        private Dictionary<string, Window> gameWindows = new();

        // Переменные для перетаскивания окна
        private bool isDragging;
        private Point startPoint;

        public MainWindow()
        {
            InitializeComponent();
            InitializeGameButtons();
            SetupEventHandlers();
        }

        private void InitializeComponent()
        {
            AvaloniaXamlLoader.Load(this);

            // Получаем ссылки на элементы
            ToolBar = this.FindControl<Border>("ToolBar");
            GamesPanel = this.FindControl<WrapPanel>("GamesPanel");
            ButtonSettings = this.FindControl<Button>("ButtonSettings");
            ButtonClose = this.FindControl<Button>("ButtonClose");
        }

        private void SetupEventHandlers()
        {
            // Обработчики для перетаскивания окна
            ToolBar.PointerPressed += ToolBar_PointerPressed;
            ToolBar.PointerMoved += ToolBar_PointerMoved;
            ToolBar.PointerReleased += ToolBar_PointerReleased;

            // Эффекты при наведении на кнопку закрытия
            ButtonClose.PointerEntered += ButtonClose_PointerEntered;
            ButtonClose.PointerExited += ButtonClose_PointerExited;
        }

        /// <summary>
        /// Инициализация игровых кнопок с использованием полиморфизма
        /// </summary>
        private void InitializeGameButtons()
        {
            // Создаем и добавляем кнопки
            var buttons = new GameButtonBase[]
            {
                new StandAloneGameButton("Match Three", "Match3GameCS.exe"),
                new StandAloneGameButton("MineSweeper", "MineSweeperCS.exe"),
                new StandAloneGameButton("Battleship", "BattleshipCS.exe"),
                new StandAloneGameButton("Blackjack", "BlackjackCS.exe"),
                new LockedGameButton("Game 5"),
                new LockedGameButton("Game 6")
            };

            // Добавляем кнопки на панель
            foreach (var button in buttons)
            {
                GamesPanel.Children.Add(button);
            }
        }

        #region Обработчики событий

        private void ButtonClose_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void ButtonClose_PointerEntered(object sender, PointerEventArgs e)
        {
            ButtonClose.Background = new SolidColorBrush(Colors.LightCoral);
        }

        private void ButtonClose_PointerExited(object sender, PointerEventArgs e)
        {
            ButtonClose.Background = new SolidColorBrush(Colors.White);
        }

        private void ButtonMinimize_Click(object sender, RoutedEventArgs e)
        {
            WindowState = WindowState.Minimized;
        }

        private void ButtonSettings_Click(object sender, RoutedEventArgs e)
        {

        }

        private void ToolBar_PointerPressed(object sender, PointerPressedEventArgs e)
        {
            if (e.GetCurrentPoint(this).Properties.IsLeftButtonPressed)
            {
                isDragging = true;
                startPoint = e.GetPosition(this);
            }
        }

        private void ToolBar_PointerMoved(object sender, PointerEventArgs e)
        {
            if (isDragging && e.GetCurrentPoint(this).Properties.IsLeftButtonPressed)
            {
                var currentPosition = e.GetPosition(Parent as Window);
                Position = new PixelPoint(
                    Position.X + (int)(currentPosition.X - startPoint.X),
                    Position.Y + (int)(currentPosition.Y - startPoint.Y));
            }
        }

        private void ToolBar_PointerReleased(object sender, PointerReleasedEventArgs e)
        {
            isDragging = false;
        }

        #endregion
    }

    #region Классы игровых кнопок

    /// <summary>
    /// Базовый класс для всех игровых кнопок
    /// </summary>
    public abstract class GameButtonBase : Button
    {
        public GameButtonBase()
        {
            Width = 232;
            Height = 155;
            Background = new SolidColorBrush(Colors.White);
            Foreground = new SolidColorBrush(Colors.Black);
            FontSize = 20;
            FontWeight = FontWeight.Bold;
            FontFamily = new FontFamily("Segoe Print");
            BorderThickness = new Thickness(0);
            HorizontalContentAlignment = Avalonia.Layout.HorizontalAlignment.Center;
            VerticalContentAlignment = Avalonia.Layout.VerticalAlignment.Bottom;
            Padding = new Thickness(0, 0, 0, 10);

            // Обработчики событий для эффектов
            PointerEntered += OnMouseEnter;
            PointerExited += OnMouseLeave;
            Click += OnClick;
        }

        /// <summary>
        /// Абстрактный метод для запуска игры
        /// </summary>
        protected abstract void LaunchGame();

        protected virtual void OnMouseEnter(object sender, PointerEventArgs e)
        {
            Background = new SolidColorBrush(Colors.LightGray);
        }

        protected virtual void OnMouseLeave(object sender, PointerEventArgs e)
        {
            Background = new SolidColorBrush(Colors.White);
        }

        protected virtual void OnClick(object sender, RoutedEventArgs e)
        {
            LaunchGame();
        }
    }

    /// <summary>
    /// Кнопка для запуска standalone .exe игр
    /// </summary>
    public class StandAloneGameButton : GameButtonBase
    {
        private readonly string exePath;
        private readonly string gameTitle;

        public StandAloneGameButton(string title, string path)
        {
            gameTitle = title;
            exePath = path;
            Content = title;
        }

        protected override void LaunchGame()
        {
            try
            {
                var process = new Process();
                process.StartInfo.FileName = exePath;
                process.StartInfo.UseShellExecute = true;
                process.StartInfo.WorkingDirectory = Path.GetDirectoryName(exePath);
                process.Start();
            }
            catch (Exception ex)
            {
            
            }
        }
    }

    /// <summary>
    /// Кнопка для игры Match3 (заглушка)
    /// </summary>
    public class Match3GameButton : GameButtonBase
    {
        public Match3GameButton()
        {
            Content = "Match Three";
        }

        protected override void LaunchGame()
        {
        
        }
    }

    /// <summary>
    /// Кнопка для игры MineSweeper (заглушка)
    /// </summary>
    public class MineSweeperGameButton : GameButtonBase
    {
        public MineSweeperGameButton()
        {
            Content = "MineSweeper";
        }

        protected override void LaunchGame()
        {
        
        }
    }

    /// <summary>
    /// Кнопка для заблокированных игр
    /// </summary>
    public class LockedGameButton : GameButtonBase
    {
        private readonly string gameTitle;

        public LockedGameButton(string title)
        {
            gameTitle = title;
            Content = "Soon...";
            FontWeight = FontWeight.Normal;
            IsEnabled = false;
            Background = new SolidColorBrush(Colors.LightGray);
        }

        protected override void LaunchGame()
        {
        
        }
    }

    #endregion
}