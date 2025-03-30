#include "../include/Game.h"
#include <iostream>
#include <algorithm>

Game::Game()
    : m_board(std::make_unique<Board>()),
      m_renderer(std::make_unique<Renderer>()),
      m_currentPlayerIndex(0),
      m_gameOver(false)
{
    // Create players
    m_players[0] = std::make_unique<Player>(0, "Player 1");
    m_players[1] = std::make_unique<Player>(1, "Player 2");
}

void Game::initialize()
{
    // Initialize the renderer
    if (!m_renderer->initialize("Gobblet Gobblers", 800, 600)) {
        std::cerr << "Failed to initialize renderer!" << std::endl;
        return;
    }
    
    // Reset the board
    m_board->reset();
    
    // Reset game state
    m_currentPlayerIndex = 0;
    m_gameOver = false;
    m_winnerMessage = "";
}

void Game::run()
{
    // Main game loop
    while (!m_gameOver && m_renderer->processEvents()) {
        update();
        render();
    }
    
    // Game over screen
    while (m_gameOver && m_renderer->processEvents()) {
        render();
    }
}

void Game::update()
{
    handleEvents();
    checkGameState();
}

void Game::render()
{
    m_renderer->renderGame(*m_board, m_players, m_currentPlayerIndex, m_gameOver, m_winnerMessage);
}

void Game::handleEvents()
{
    static bool mousePressed = false;
    static bool pieceSelected = false;
    static Piece::Size selectedPieceSize;
    
    // Get mouse position
    auto mousePos = m_renderer->getMousePosition();
    int mouseX = mousePos.x;
    int mouseY = mousePos.y;
    
    // Check if mouse is pressed
    bool isMouseDown = m_renderer->isMouseButtonPressed();
    
    // Mouse press and release detection
    if (isMouseDown && !mousePressed) {
        mousePressed = true;
        
        // Process the mouse press
        if (!pieceSelected) {
            // Try to select a piece
            if (m_renderer->isPlayerPieceClicked(mouseX, mouseY, m_currentPlayerIndex, selectedPieceSize)) {
                if (m_players[m_currentPlayerIndex]->hasPieceAvailable(selectedPieceSize)) {
                    pieceSelected = true;
                }
            }
        } else {
            // Try to place the selected piece
            int row, col;
            if (m_renderer->isBoardCellClicked(mouseX, mouseY, row, col)) {
                auto piece = m_players[m_currentPlayerIndex]->selectPiece(selectedPieceSize);
                
                if (piece && m_board->placePiece(row, col, std::move(piece))) {
                    // Piece was placed successfully
                    pieceSelected = false;
                    
                    // Switch to the next player
                    m_currentPlayerIndex = (m_currentPlayerIndex + 1) % 2;
                }
            } else {
                // Clicked elsewhere, cancel selection
                pieceSelected = false;
            }
        }
    } else if (!isMouseDown && mousePressed) {
        mousePressed = false;
    }
}

void Game::checkGameState()
{
    // Check if either player has won
    int winType, winIndex;
    
    // Check for player 1 win
    if (m_board->checkWin(0, winType, winIndex)) {
        m_gameOver = true;
        m_winnerMessage = "Player 1 Wins!";
        return;
    }
    
    // Check for player 2 win
    if (m_board->checkWin(1, winType, winIndex)) {
        m_gameOver = true;
        m_winnerMessage = "Player 2 Wins!";
        return;
    }
    
    // Check for draw (if all pieces have been used)
    bool allPiecesUsed = true;
    for (const auto& player : m_players) {
        for (int sizeIdx = 0; sizeIdx < 3; ++sizeIdx) {
            if (player->hasPieceAvailable(static_cast<Piece::Size>(sizeIdx))) {
                allPiecesUsed = false;
                break;
            }
        }
        if (!allPiecesUsed) break;
    }
    
    if (allPiecesUsed) {
        m_gameOver = true;
        m_winnerMessage = "Game Draw!";
    }
}

bool Game::isGameOver() const
{
    return m_gameOver;
}