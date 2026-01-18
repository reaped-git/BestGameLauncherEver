#include "Application.hpp"

using namespace System;
using namespace System::Windows::Forms;

// Точка входа в приложение
/// <summary>
/// Главная функция - точка входа в приложение
/// Инициализирует окружение WinForms и запускает главную форму
/// </summary>
/// <param name="args">Аргументы командной строки (не используются)</param>
/// <returns>Код завершения приложения</returns>
int main(array<String^>^) {
    // Настройка совместимости текстового рендеринга для современных версий .NET
    Application::SetCompatibleTextRenderingDefault(false);

    // Включение визуальных стилей для современных элементов интерфейса Windows
    Application::EnableVisualStyles();

    // Создание и запуск главной формы лаунчера
    // Используем stack semantics для автоматического управления памятью
    GameLauncher::LauncherForm form;
    Application::Run(% form);

    return 0;
}
