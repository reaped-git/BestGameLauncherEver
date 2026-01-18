#pragma once

#include "Enums.hpp"

namespace Blackjack {

    class InputHandler {
    private:
        char lastKeyPressed;
        ActionType lastAction;

    public:
        InputHandler();

        ActionType processKey(char key);
        ActionType getLastAction() const;

        static bool isValidKey(char key);
        static char getActionChar(ActionType action);
    };
}