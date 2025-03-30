#pragma once

#include <array>
#include <vector>
#include <optional>
#include <memory>
#include "Piece.h"

// Board class to manage the 3x3 game grid
class Board {
public:
    static constexpr int BOARD_SIZE = 3;
    
    Board();
    ~Board() = default;

    // Check if a move is valid
    bool isValidMove(int row, int col, const Piece& piece) const;
    
    // Place a piece on the board
    bool placePiece(int row, int col, std::unique_ptr<Piece> piece);
    
    // Get the top piece at a position (if any)
    const Piece* getTopPiece(int row, int col) const;
    
    // Check if there's a winning combination
    bool checkWin(int playerID, int& winType, int& winIndex) const;
    
    // Reset the board
    void reset();

    // Get a specific cell for rendering
    const std::vector<std::unique_ptr<Piece>>& getCell(int row, int col) const;

private:
    // 3x3 grid where each cell can contain a stack of pieces
    std::array<std::array<std::vector<std::unique_ptr<Piece>>, BOARD_SIZE>, BOARD_SIZE> m_grid;
    
    // Helper methods to check different winning conditions
    bool checkRows(int playerID, int& rowIndex) const;
    bool checkColumns(int playerID, int& colIndex) const;
    bool checkDiagonals(int playerID, int& diagIndex) const;
};