#pragma once

#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <type_traits>
#include "GameStatistics.hpp"

namespace MineSweeper {

    /// <summary>
    /// Абстрактный интерфейс для игровых объектов с именем
    /// </summary>
    class INamedObject {
    public:
        virtual ~INamedObject() = default;
        virtual std::string GetName() const = 0;
        virtual int GetScore() const = 0;
    };

    // Простые адаптеры в MineSweeper namespace
    class StatisticsAdapter : public BaseStatistics, public INamedObject {
    public:
        StatisticsAdapter(const std::string& name = "Player") : BaseStatistics(name) {}

        std::string GetName() const override {
            return GetPlayerName();
        }

        int GetScore() const override {
            return GetGamesPlayed();
        }
    };

    class ExtendedStatisticsAdapter : public ExtendedStatistics, public INamedObject {
    public:
        ExtendedStatisticsAdapter(const std::string& name = "Player") : ExtendedStatistics(name) {}

        std::string GetName() const override {
            return GetPlayerName();
        }

        int GetScore() const override {
            return GetGamesPlayed() * 100 + GetBestTime();
        }
    };

    class TimedStatisticsAdapter : public TimedStatistics, public INamedObject {
    public:
        TimedStatisticsAdapter(const std::string& name = "Player") : TimedStatistics(name) {}

        std::string GetName() const override {
            return GetPlayerName();
        }

        int GetScore() const override {
            return GetGamesPlayed() * 1000 + static_cast<int>(GetWinRate() * 100);
        }
    };

    /// <summary>
    /// Шаблонный класс для управления коллекцией игровых объектов
    /// </summary>
    template<typename T>
    class GameCollection {
    private:
        std::vector<std::unique_ptr<T>> items;
        std::string collectionName;

    public:
        // Конструктор
        explicit GameCollection(const std::string& name = "Collection")
            : collectionName(name) {
        }

        // Нешаблонные методы
        std::string GetName() const { return collectionName; }
        void SetName(const std::string& name) { collectionName = name; }

        size_t Size() const { return items.size(); }
        bool Empty() const { return items.empty(); }

        // Методы для добавления
        void Add(std::unique_ptr<T> item) {
            items.push_back(std::move(item));
        }

        // Сортировка по score
        void SortByScore() {
            std::sort(items.begin(), items.end(),
                [](const std::unique_ptr<T>& a, const std::unique_ptr<T>& b) {
                    return a->GetScore() > b->GetScore();
                });
        }

        // Сортировка по имени
        void SortByName() {
            std::sort(items.begin(), items.end(),
                [](const std::unique_ptr<T>& a, const std::unique_ptr<T>& b) {
                    return a->GetName() < b->GetName();
                });
        }

        // Поиск по имени
        T* FindByName(const std::string& name) {
            SortByName();

            auto it = std::lower_bound(items.begin(), items.end(), name,
                [](const std::unique_ptr<T>& item, const std::string& name) {
                    return item->GetName() < name;
                });

            if (it != items.end() && (*it)->GetName() == name) {
                return it->get();
            }
            return nullptr;
        }

        // Получение элемента
        T* Get(size_t index) {
            if (index < items.size()) {
                return items[index].get();
            }
            return nullptr;
        }

        const T* Get(size_t index) const {
            if (index < items.size()) {
                return items[index].get();
            }
            return nullptr;
        }

        // Очистка
        void Clear() {
            items.clear();
        }

        // Итераторы
        auto begin() { return items.begin(); }
        auto end() { return items.end(); }
        auto begin() const { return items.begin(); }
        auto end() const { return items.end(); }
    };

    // Специализация для интерфейса
    using PlayerCollection = GameCollection<INamedObject>;

} 