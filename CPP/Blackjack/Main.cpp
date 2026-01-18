#include "BlackjackGame.hpp"
#include <iostream>

using namespace Blackjack;

int main() {
    try {
        BlackjackGame game(1000);
        game.run();
    }
    catch (const std::exception& ex) {
        std::cout << "An error occurred: " << ex.what() << "\n";
        std::cout << "Press any key to exit...\n";
        std::cin.get();
    }
    catch (...) {
        std::cout << "An unknown error occurred.\n";
        std::cout << "Press any key to exit...\n";
        std::cin.get();
    }

    return 0;
}