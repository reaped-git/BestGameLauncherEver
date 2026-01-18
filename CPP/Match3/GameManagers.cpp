#include "GameManagers.hpp"
#include <sstream>

namespace Match3 {

    // Базовый класс
    BaseGameManager::BaseGameManager(const std::string& version, Int64 id)
        : gameVersion(version), gameId(id)
    {
    }

    std::string BaseGameManager::GetGameVersion() const
    {
        return gameVersion;
    }

    Int64 BaseGameManager::GetGameId() const
    {
        return gameId;
    }

    // Дочерний класс
    ExtendedGameManager::ExtendedGameManager(const std::string& version, Int64 id,
        const std::string& mode)
        : BaseGameManager(version, id), // Вызов конструктора базового класса
        gameMode(mode)
    {
        scoreManager = std::make_shared<ScoreManager>();
        gameLogic = std::make_shared<GameLogic>(scoreManager);
    }

    ExtendedGameManager::ExtendedGameManager(const std::string& version, Int64 id)
        : BaseGameManager(version, id), // Вызов конструктора базового класса
        gameMode("Classic")
    {
        scoreManager = std::make_shared<ScoreManager>();
        gameLogic = std::make_shared<GameLogic>(scoreManager);
    }

    // Конструктор копирования
    ExtendedGameManager::ExtendedGameManager(const ExtendedGameManager& other)
        : BaseGameManager(other), // Копирование базовой части
        gameMode(other.gameMode)
    {
        scoreManager = std::make_shared<ScoreManager>(*other.scoreManager);
        gameLogic = std::make_shared<GameLogic>(*other.gameLogic);
    }

    void ExtendedGameManager::Initialize()
    {
        // Инициализация игры
        scoreManager->ResetScore();
    }

    void ExtendedGameManager::Shutdown()
    {
        // Очистка ресурсов
        gameLogic.reset();
        scoreManager.reset();
    }

    std::shared_ptr<GameLogic> ExtendedGameManager::GetGameLogic() const
    {
        return gameLogic;
    }

    std::shared_ptr<ScoreManager> ExtendedGameManager::GetScoreManager() const
    {
        return scoreManager;
    }

    // Дружественная функция - перегрузка оператора вывода
    std::ostream& operator<<(std::ostream& os, const ExtendedGameManager& manager)
    {
        os << "Game Manager [Version: " << manager.gameVersion
            << ", ID: " << manager.gameId
            << ", Mode: " << manager.gameMode << "]";
        return os;
    }

}