#include "DifficultyManager.hpp"
#include <stdexcept>

namespace MineSweeper {

    DifficultyManager::DifficultyManager() : currentDifficulty(0)
    {
        difficulties = {
            { "Легкий", 10, 10, 600 },
            { "Средний", 12, 20, 480 },
            { "Сложный", 15, 40, 360 }
        };
    }

    void DifficultyManager::SetDifficulty(int index)
    {
        if (index < 0 || index >= static_cast<int>(difficulties.size())) {
            throw std::out_of_range("Invalid difficulty index");
        }
        currentDifficulty = index;
    }

    const DifficultyManager::DifficultySettings& DifficultyManager::GetCurrentSettings() const
    {
        return difficulties[currentDifficulty];
    }

    std::vector<std::string> DifficultyManager::GetDifficultyNames() const
    {
        std::vector<std::string> names;
        names.reserve(difficulties.size());

        for (const auto& settings : difficulties) {
            names.push_back(settings.name);
        }

        return names;
    }
}