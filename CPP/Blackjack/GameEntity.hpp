#pragma once

#include <string>
#include <memory>
#include <vector>

namespace Blackjack {

    class Card;

    // Абстрактный класс
    class GameEntity {
    protected:
        std::string name;
        int id;
        static int nextId; // Статическое поле

    public:
        GameEntity(const std::string& entityName);
        virtual ~GameEntity(); // Виртуальный деструктор

        // Виртуальные методы
        virtual void displayInfo() const = 0; // Чисто виртуальная функция
        virtual std::string getDescription() const;

        // Статический метод
        static int getNextId();

        // Перегрузка операторов
        virtual bool operator==(const GameEntity& other) const;
        virtual bool operator!=(const GameEntity& other) const;

        // Геттеры
        std::string getName() const;
        int getId() const;

        virtual std::unique_ptr<GameEntity> clone() const = 0;

        // Запрещаем конструктор копирования по умолчанию
        GameEntity(const GameEntity&) = delete;
        GameEntity& operator=(const GameEntity&) = delete;
    };
}
