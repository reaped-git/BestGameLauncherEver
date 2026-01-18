#include "GameEntity.hpp"
#include <iostream>

namespace Blackjack {

    // Инициализация статического поля
    int GameEntity::nextId = 1;

    GameEntity::GameEntity(const std::string& entityName)
        : name(entityName), id(nextId++) {
        std::cout << "GameEntity created: " << name << " (ID: " << id << ")\n";
    }

    GameEntity::~GameEntity() {
        std::cout << "GameEntity destroyed: " << name << "\n";
    }

    std::string GameEntity::getDescription() const {
        return "Game Entity: " + name;
    }

    int GameEntity::getNextId() {
        return nextId;
    }

    bool GameEntity::operator==(const GameEntity& other) const {
        return this->id == other.id && this->name == other.name;
    }

    bool GameEntity::operator!=(const GameEntity& other) const {
        return !(*this == other);
    }

    std::string GameEntity::getName() const {
        return name;
    }

    int GameEntity::getId() const {
        return id;
    }
}