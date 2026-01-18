#include "GameTimer.hpp"
#include <sstream>
#include <iomanip>

namespace MineSweeper {

    GameTimer::GameTimer(int initialTimeSeconds)
        : initialTime(initialTimeSeconds), timeLeft(initialTimeSeconds), isRunning(false)
    {
    }

    void GameTimer::Update()
    {
        if (isRunning && timeLeft > 0)
        {
            timeLeft--;
        }
    }

    void GameTimer::Start()
    {
        isRunning = true;
    }

    void GameTimer::Stop()
    {
        isRunning = false;
    }

    void GameTimer::Reset()
    {
        Stop();
        timeLeft = initialTime;
    }

    std::string GameTimer::FormatTime(int seconds)
    {
        int minutes = seconds / 60;
        int secs = seconds % 60;

        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << minutes << ":"
            << std::setw(2) << std::setfill('0') << secs;
        return oss.str();
    }
}