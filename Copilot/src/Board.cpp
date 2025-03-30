#include "../include/Board.h"

Board::Board() 
{
    reset();
}

bool Board::isValidMove(int row, int col, const Piece& piece) const 
{
    // Check if coordinates are valid
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return false;
    }

    // If cell is empty, move is valid
    if (m_grid[row][col].empty()) {
        return true;
    }

    // If cell has a piece, check if the new piece can cover it
    const Piece* topPiece = m_grid[row][col].back().get();
    return piece.canCover(*topPiece);
}

bool Board::placePiece(int row, int col, std::unique_ptr<Piece> piece) 
{
    if (!piece || !isValidMove(row, col, *piece)) {
        return false;
    }

    // Place the piece at the specified position
    m_grid[row][col].push_back(std::move(piece));
    return true;
}

const Piece* Board::getTopPiece(int row, int col) const 
{
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || m_grid[row][col].empty()) {
        return nullptr;
    }
    return m_grid[row][col].back().get();
}

bool Board::checkWin(int playerID, int& winType, int& winIndex) const 
{
    // Check rows
    if (checkRows(playerID, winIndex)) {
        winType = 0; // Row win
        return true;
    }
    
    // Check columns
    if (checkColumns(playerID, winIndex)) {
        winType = 1; // Column win
        return true;
    }
    
    // Check diagonals
    if (checkDiagonals(playerID, winIndex)) {
        winType = 2; // Diagonal win
        return true;
    }
    
    return false;
}

void Board::reset() 
{
    // Clear all cells
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            m_grid[row][col].clear();
        }
    }
}

const std::vector<std::unique_ptr<Piece>>& Board::getCell(int row, int col) const 
{
    return m_grid[row][col];
}

bool Board::checkRows(int playerID, int& rowIndex) const 
{
    for (int row = 0; row < BOARD_SIZE; ++row) {
        bool rowWin = true;
        
        for (int col = 0; col < BOARD_SIZE; ++col) {
            const Piece* piece = getTopPiece(row, col);
            if (!piece || piece->getPlayerID() != playerID) {
                rowWin = false;
                break;
            }
        }
        
        if (rowWin) {
            rowIndex = row;
            return true;
        }
    }
    
    return false;
}

bool Board::checkColumns(int playerID, int& colIndex) const 
{
    for (int col = 0; col < BOARD_SIZE; ++col) {
        bool colWin = true;
        
        for (int row = 0; row < BOARD_SIZE; ++row) {
            const Piece* piece = getTopPiece(row, col);
            if (!piece || piece->getPlayerID() != playerID) {
                colWin = false;
                break;
            }
        }
        
        if (colWin) {
            colIndex = col;
            return true;
        }
    }
    
    return false;
}

bool Board::checkDiagonals(int playerID, int& diagIndex) const 
{
    // Check main diagonal (top-left to bottom-right)
    bool mainDiagWin = true;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        const Piece* piece = getTopPiece(i, i);
        if (!piece || piece->getPlayerID() != playerID) {
            mainDiagWin = false;
            break;
        }
    }
    
    if (mainDiagWin) {
        diagIndex = 0; // Main diagonal
        return true;
    }
    
    // Check anti-diagonal (top-right to bottom-left)
    bool antiDiagWin = true;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        const Piece* piece = getTopPiece(i, BOARD_SIZE - 1 - i);
        if (!piece || piece->getPlayerID() != playerID) {
            antiDiagWin = false;
            break;
        }
    }
    
    if (antiDiagWin) {
        diagIndex = 1; // Anti-diagonal
        return true;
    }
    
    return false;
}