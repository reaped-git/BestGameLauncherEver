#include "InputHandler.hpp"
#include <cctype>

namespace Blackjack {

    InputHandler::InputHandler() : lastKeyPressed('\0'), lastAction(ActionType::NONE) {}

    ActionType InputHandler::processKey(char key) {
        lastKeyPressed = key;
        char upperKey = std::toupper(key);

        switch (upperKey) {
        case 'H':
            lastAction = ActionType::HIT;
            break;
        case 'S':
            lastAction = ActionType::STAND;
            break;
        case 'D':
            lastAction = ActionType::DEAL;
            break;
        case 'Q':
            lastAction = ActionType::QUIT;
            break;
        default:
            lastAction = ActionType::INVALID;
            break;
        }

        return lastAction;
    }

    ActionType InputHandler::getLastAction() const {
        return lastAction;
    }

    bool InputHandler::isValidKey(char key) {
        char upperKey = std::toupper(key);
        return upperKey == 'H' || upperKey == 'S' || upperKey == 'D' || upperKey == 'Q';
    }

    char InputHandler::getActionChar(ActionType action) {
        switch (action) {
        case ActionType::HIT: return 'H';
        case ActionType::STAND: return 'S';
        case ActionType::DEAL: return 'D';
        case ActionType::QUIT: return 'Q';
        default: return '?';
        }
    }
}