#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <iostream>

namespace Match3 {

    // 2.1. Абстрактный базовый класс для игровых данных
    class IGameData {
    public:
        virtual ~IGameData() = default;

        virtual std::string GetName() const = 0;
        virtual int GetValue() const = 0;
        virtual void Display() const = 0;
        virtual std::string ToString() const = 0;

        // Оператор для сортировки
        virtual bool operator<(const IGameData& other) const {
            return GetValue() < other.GetValue();
        }
    };

    // 2.2. Класс для хранения данных о счете игрока (производный)
    class PlayerScoreData : public IGameData {
    private:
        std::string playerName;
        int score;
        int level;

    public:
        PlayerScoreData(const std::string& name, int score, int level = 1)
            : playerName(name), score(score), level(level) {
        }

        std::string GetName() const override { return playerName; }
        int GetValue() const override { return score; }
        int GetLevel() const { return level; }

        void SetScore(int newScore) { score = newScore; }
        void SetLevel(int newLevel) { level = newLevel; }

        void Display() const override {
            std::cout << "Player: " << playerName
                << ", Score: " << score
                << ", Level: " << level << std::endl;
        }

        std::string ToString() const override {
            return "Player: " + playerName + " | Score: " +
                std::to_string(score) + " | Level: " +
                std::to_string(level);
        }
    };

    // 2.3. Класс для хранения данных о времени игры (производный)
    class GameTimeData : public IGameData {
    private:
        std::string gameSessionId;
        int playTimeSeconds;
        std::string date;

    public:
        GameTimeData(const std::string& sessionId, int seconds,
            const std::string& dateStr)
            : gameSessionId(sessionId), playTimeSeconds(seconds), date(dateStr) {
        }

        std::string GetName() const override { return gameSessionId; }
        int GetValue() const override { return playTimeSeconds; }
        std::string GetDate() const { return date; }

        void Display() const override {
            std::cout << "Session: " << gameSessionId
                << ", Time: " << playTimeSeconds << "s"
                << ", Date: " << date << std::endl;
        }

        std::string ToString() const override {
            return "Session: " + gameSessionId + " | Time: " +
                std::to_string(playTimeSeconds) + "s | Date: " + date;
        }
    };

    // 2.4. Шаблонный класс-контейнер для хранения игровых данных
    template<typename T>
    class GameDataContainer {
        static_assert(std::is_base_of<IGameData, T>::value,
            "T must be derived from IGameData");

    private:
        std::vector<std::shared_ptr<T>> dataItems;
        std::string containerName;

    public:
        // Нешаблонные методы
        GameDataContainer(const std::string& name = "GameDataContainer")
            : containerName(name) {
        }

        ~GameDataContainer() {
            Clear();
        }

        // Нешаблонные методы доступа
        std::string GetName() const { return containerName; }
        void SetName(const std::string& name) { containerName = name; }

        size_t Size() const { return dataItems.size(); }
        bool IsEmpty() const { return dataItems.empty(); }

        // Шаблонные методы работы с данными
        void AddItem(std::shared_ptr<T> item) {
            dataItems.push_back(item);
        }

        template<typename... Args>
        void AddItem(Args&&... args) {
            dataItems.push_back(std::make_shared<T>(std::forward<Args>(args)...));
        }

        std::shared_ptr<T> GetItem(size_t index) {
            if (index < dataItems.size()) {
                return dataItems[index];
            }
            return nullptr;
        }

        void RemoveItem(size_t index) {
            if (index < dataItems.size()) {
                dataItems.erase(dataItems.begin() + index);
            }
        }

        void Clear() {
            dataItems.clear();
        }

        // Метод сортировки данных по значению
        void SortByValue(bool ascending = true) {
            if (ascending) {
                std::sort(dataItems.begin(), dataItems.end(),
                    [](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
                        return a->GetValue() < b->GetValue();
                    });
            }
            else {
                std::sort(dataItems.begin(), dataItems.end(),
                    [](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
                        return a->GetValue() > b->GetValue();
                    });
            }
        }

        // Метод сортировки данных по имени
        void SortByName() {
            std::sort(dataItems.begin(), dataItems.end(),
                [](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
                    return a->GetName() < b->GetName();
                });
        }

        // Метод поиска по имени (бинарный поиск - требует сортировки)
        std::shared_ptr<T> FindByName(const std::string& name) {
            // Сначала сортируем по имени, если не отсортировано
            SortByName();

            auto it = std::lower_bound(dataItems.begin(), dataItems.end(), name,
                [](const std::shared_ptr<T>& item, const std::string& name) {
                    return item->GetName() < name;
                });

            if (it != dataItems.end() && (*it)->GetName() == name) {
                return *it;
            }

            // Линейный поиск на случай, если контейнер не отсортирован
            for (const auto& item : dataItems) {
                if (item->GetName() == name) {
                    return item;
                }
            }

            return nullptr;
        }

        // Метод поиска по значению (линейный поиск)
        std::shared_ptr<T> FindByValue(int value) {
            for (const auto& item : dataItems) {
                if (item->GetValue() == value) {
                    return item;
                }
            }
            return nullptr;
        }

        // Метод для получения всех элементов выше порога
        std::vector<std::shared_ptr<T>> GetAboveThreshold(int threshold) {
            std::vector<std::shared_ptr<T>> result;
            for (const auto& item : dataItems) {
                if (item->GetValue() > threshold) {
                    result.push_back(item);
                }
            }
            return result;
        }

        // Метод для вычисления статистики
        struct Statistics {
            double average;
            int min;
            int max;
            int total;
        };

        Statistics CalculateStatistics() const {
            Statistics stats{ 0.0, 0, 0, 0 };

            if (dataItems.empty()) {
                return stats;
            }

            stats.min = dataItems[0]->GetValue();
            stats.max = dataItems[0]->GetValue();

            for (const auto& item : dataItems) {
                int value = item->GetValue();
                stats.total += value;

                if (value < stats.min) stats.min = value;
                if (value > stats.max) stats.max = value;
            }

            stats.average = static_cast<double>(stats.total) / dataItems.size();
            return stats;
        }

        // Метод для отображения всех элементов
        void DisplayAll() const {
            std::cout << "=== " << containerName << " ===" << std::endl;
            for (size_t i = 0; i < dataItems.size(); ++i) {
                std::cout << i + 1 << ". ";
                dataItems[i]->Display();
            }
        }

        // Итератор для foreach
        auto begin() { return dataItems.begin(); }
        auto end() { return dataItems.end(); }
        auto begin() const { return dataItems.begin(); }
        auto end() const { return dataItems.end(); }
    };

    // 2.5. Конкретные типы контейнеров
    using PlayerScoreContainer = GameDataContainer<PlayerScoreData>;
    using GameTimeContainer = GameDataContainer<GameTimeData>;

    // 2.6. Шаблонная функция для объединения двух контейнеров
    template<typename T>
    GameDataContainer<T> MergeContainers(const GameDataContainer<T>& container1,
        const GameDataContainer<T>& container2,
        const std::string& mergedName = "MergedContainer") {
        GameDataContainer<T> result(mergedName);

        for (const auto& item : container1) {
            result.AddItem(item);
        }

        for (const auto& item : container2) {
            result.AddItem(item);
        }

        return result;
    }

}