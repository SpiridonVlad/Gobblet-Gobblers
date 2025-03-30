#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Piece.h"

class Player {
public:
    static constexpr int PIECES_PER_SIZE = 2;

    Player(int id, const std::string& name);
    ~Player() = default;

    // Get player information
    int getID() const { return m_id; }
    const std::string& getName() const { return m_name; }
    
    // Piece management
    std::unique_ptr<Piece> selectPiece(Piece::Size size);
    bool hasPieceAvailable(Piece::Size size) const;
    void returnPiece(std::unique_ptr<Piece> piece);
    
    // Get number of available pieces of a specific size
    int getAvailablePieces(Piece::Size size) const;
    
    // Get all available pieces for rendering
    const std::vector<std::vector<std::unique_ptr<Piece>>>& getAvailablePieces() const;

private:
    int m_id;  // Player ID (0 or 1)
    std::string m_name;  // Player name
    
    // Available pieces organized by size
    std::vector<std::vector<std::unique_ptr<Piece>>> m_availablePieces;
};