#pragma once

#include <cstdint>

// Class representing a game piece
class Piece {
public:
    // Piece sizes from smallest to largest
    enum class Size : uint8_t {
        SMALL = 0,
        MEDIUM = 1,
        LARGE = 2
    };

    Piece(int playerID, Size size);
    ~Piece() = default;

    // Getters
    int getPlayerID() const { return m_playerID; }
    Size getSize() const { return m_size; }
    
    // Check if this piece can cover another piece
    bool canCover(const Piece& other) const;

private:
    int m_playerID;  // Which player owns this piece (0 or 1)
    Size m_size;     // Size of the piece
};