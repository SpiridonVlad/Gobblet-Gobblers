#pragma once

#include <memory>
#include <string>
#include "Board.h"
#include "Player.h"

// Forward declarations for graphics library components
class Graphics; // Replace with actual graphics library components

class Renderer {
public:
    // Mouse position structure
    struct MousePosition {
        int x;
        int y;
    };

    Renderer();
    ~Renderer();

    // Initialize rendering system
    bool initialize(const std::string& title, int width, int height);
    
    // Clean up resources
    void cleanup();
    
    // Clear the screen
    void clear();
    
    // Present the rendered frame
    void present();
    
    // Draw the entire game state
    void renderGame(const Board& board, const std::array<std::unique_ptr<Player>, 2>& players, 
                    int currentPlayerIndex, bool gameOver, const std::string& message);
                    
    // Draw the board
    void renderBoard(const Board& board);
    
    // Draw player pieces not on the board
    void renderPlayerPieces(const Player& player, int x, int y);
    
    // Draw UI elements
    void renderUI(int currentPlayerIndex, bool gameOver, const std::string& message);
    
    // Check if the user clicked on a board cell
    bool isBoardCellClicked(int mouseX, int mouseY, int& outRow, int& outCol) const;
    
    // Check if the user clicked on a player piece
    bool isPlayerPieceClicked(int mouseX, int mouseY, int playerIndex, Piece::Size& outSize) const;
    
    // Handle window events
    bool processEvents();

    // Get current mouse position
    MousePosition getMousePosition() const;

    // Check if mouse button is pressed
    bool isMouseButtonPressed() const;

private:
    std::unique_ptr<Graphics> m_graphics;
    int m_windowWidth;
    int m_windowHeight;
    
    // Calculate board dimensions and positions
    void calculateBoardMetrics();
    
    // Board rendering metrics
    int m_boardX;
    int m_boardY;
    int m_cellSize;
    int m_boardSize;
};