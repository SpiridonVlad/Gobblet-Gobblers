#pragma once

#include <memory>
#include <vector>
#include <array>
#include <string>
#include "Board.h"
#include "Player.h"
#include "Renderer.h"

class Game {
public:
    Game();
    ~Game() = default;
    
    // Deleted copy constructor and assignment operator to prevent copying
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void initialize();
    void run();
    void update();
    void render();
    void handleEvents();
    void checkGameState();
    bool isGameOver() const;

private:
    std::unique_ptr<Board> m_board;
    std::array<std::unique_ptr<Player>, 2> m_players;
    std::unique_ptr<Renderer> m_renderer;
    int m_currentPlayerIndex;
    bool m_gameOver;
    std::string m_winnerMessage;
};