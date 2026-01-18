#pragma once

#include <string>

namespace MineSweeper {

    /// <summary>
    /// Класс для управления игровым таймером
    /// </summary>
    class GameTimer
    {
    private:
        int timeLeft;
        int initialTime;
        bool isRunning;

    public:
        /// <summary>
        /// Конструктор таймера
        /// </summary>
        GameTimer(int initialTimeSeconds);

        /// <summary>
        /// Деструктор
        /// </summary>
        ~GameTimer() = default;

        /// <summary>
        /// Обновляет состояние таймера (вызывается каждый кадр/тик)
        /// </summary>
        void Update();

        /// <summary>
        /// Запускает таймер
        /// </summary>
        void Start();

        /// <summary>
        /// Останавливает таймер
        /// </summary>
        void Stop();

        /// <summary>
        /// Сбрасывает таймер
        /// </summary>
        void Reset();

        /// <summary>
        /// Возвращает оставшееся время
        /// </summary>
        int GetTimeLeft() const { return timeLeft; }

        /// <summary>
        /// Проверяет, истекло ли время
        /// </summary>
        bool IsTimeUp() const { return timeLeft <= 0; }

        /// <summary>
        /// Проверяет, запущен ли таймер
        /// </summary>
        bool IsRunning() const { return isRunning; }

        /// <summary>
        /// Форматирует время в строку MM:SS
        /// </summary>
        static std::string FormatTime(int seconds);
    };
}