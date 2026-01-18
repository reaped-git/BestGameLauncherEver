#include "Dealer.hpp"
#include <iostream>

namespace Blackjack {

    Dealer::Dealer(const std::string& dealerName, bool hitSoft17)
        : GameEntity(dealerName), mustHitSoft17(hitSoft17) {
        hand = std::make_unique<Hand>();
    }

    // Конструктор копирования
    Dealer::Dealer(const Dealer& other)
        : GameEntity(other.name + "_copy"), mustHitSoft17(other.mustHitSoft17) {

        if (other.hand) {
            hand = std::make_unique<Hand>(*other.hand);
        }
        else {
            hand = std::make_unique<Hand>();
        }
    }

    Dealer::~Dealer() {
        // unique_ptr автоматически удалит Hand
    }

    void Dealer::displayInfo() const {
        // Не вызываем метод базового класса
        std::cout << "=== DEALER ===\n";
        std::cout << "Name: " << this->name << "\n";
        std::cout << "Hit Soft 17: " << (mustHitSoft17 ? "Yes" : "No") << "\n";

        if (hand && hand->getCardCount() > 0) {
            std::cout << "Current Hand: " << hand->toString() << "\n";
        }
    }

    bool Dealer::canDoubleDown() const {
        // У дилера нет такой возможности
        return false;
    }

    bool Dealer::mustHit() const {
        if (!hand) return true;

        int value = hand->getTotalValue();

        if (value < 17) return true;
        if (value == 17 && mustHitSoft17 && hand->hasAce()) {
            return true;
        }
        return false;
    }

    Hand* Dealer::getHand() const {
        return hand.get();
    }

    Dealer& Dealer::operator++() {
        std::cout << "Dealer " << name << " completed another game\n";
        return *this;
    }

    Dealer Dealer::operator++(int) {
        Dealer temp = *this;
        ++(*this);
        return temp;
    }

    std::unique_ptr<GameEntity> Dealer::clone() const {
        return std::make_unique<Dealer>(*this);
    }

    void Dealer::resetHand() {
        if (hand) {
            hand->clear();
        }
    }
}