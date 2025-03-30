#include "../include/Game.h"
#include <iostream>
#include <stdexcept>

int main() {
    try {
        // Create the game
        Game game;
        
        // Initialize the game
        game.initialize();
        
        // Run the game loop
        game.run();
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 2;
    }
}