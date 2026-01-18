using Avalonia;
using GameLauncherCS;
using System;

namespace GameLauncher
{
    class Program
    {
        // Инициализация. Не удаляйте и не изменяйте метод AvaloniaXamlLoader.Load :)
        [STAThread]
        public static void Main(string[] args) => BuildAvaloniaApp()
            .StartWithClassicDesktopLifetime(args);

        // Конфигурация Avalonia, не удаляйте этот метод
        public static AppBuilder BuildAvaloniaApp()
            => AppBuilder.Configure<App>()
                .UsePlatformDetect()
                .LogToTrace();
    }
}