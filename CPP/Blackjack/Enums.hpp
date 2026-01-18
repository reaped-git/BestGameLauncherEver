#pragma once

namespace Blackjack {

    enum class ActionType {
        NONE,
        HIT,
        STAND,
        DEAL,
        QUIT,
        INVALID
    };

    enum class GameStatus {
        BETTING,
        PLAYER_TURN,
        DEALER_TURN,
        GAME_OVER
    };
}