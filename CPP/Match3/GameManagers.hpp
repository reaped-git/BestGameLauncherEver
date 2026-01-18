#pragma once

#include "GameLogic.hpp"
#include "ScoreManager.hpp"
#include "GameAlgorithms.hpp"
#include <memory>
#include <string>

namespace Match3 {

    // Базовый класс для управления игрой
    class BaseGameManager
    {
    protected:
        std::string gameVersion;
        Int64 gameId;

    private:
        GameTimeContainer gameSessions;

    public:
        BaseGameManager(const std::string& version, Int64 id);
        virtual ~BaseGameManager() = default;

        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;

        // Методы для работы с сессиями
        void AddGameSession(const std::string& sessionId, Int64 playTime,
            const std::string& date) {
            gameSessions.AddItem(sessionId, playTime, date);
        }

        void DisplayAllSessions() {
            gameSessions.DisplayAll();
        }

        void SortSessionsByTime(bool ascending = true) {
            gameSessions.SortByValue(ascending);
        }

        std::shared_ptr<GameTimeData> FindSession(const std::string& sessionId) {
            return gameSessions.FindByName(sessionId);
        }

        // Использование шаблонной функции
        double GetAverageSessionTime() const {
            std::vector<Int64> times;
            for (const auto& session : gameSessions) {
                times.push_back(session->GetValue());
            }
            return CalculateAverage<std::vector<Int64>>(times);
        }

        std::string GetGameVersion() const;
        Int64 GetGameId() const;
    };

    // Дочерний класс - расширенный менеджер игры
    class ExtendedGameManager : public BaseGameManager
    {
    private:
        std::shared_ptr<GameLogic> gameLogic;
        std::shared_ptr<ScoreManager> scoreManager;
        std::string gameMode;

    public:
        ExtendedGameManager(const std::string& version, Int64 id,
            const std::string& mode);

        // Конструктор копирования
        ExtendedGameManager(const ExtendedGameManager& other);

        // Вызов конструктора базового класса
        ExtendedGameManager(const std::string& version, Int64 id);

        ~ExtendedGameManager() = default;

        void Initialize() override;
        void Shutdown() override;

        std::shared_ptr<GameLogic> GetGameLogic() const;
        std::shared_ptr<ScoreManager> GetScoreManager() const;

        // Перегрузка оператора вывода
        friend std::ostream& operator<<(std::ostream& os, const ExtendedGameManager& manager);
    };

}