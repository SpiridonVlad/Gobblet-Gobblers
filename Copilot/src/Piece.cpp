#include "../include/Piece.h"

Piece::Piece(int playerID, Size size)
    : m_playerID(playerID), m_size(size)
{
}

bool Piece::canCover(const Piece& other) const
{
    // A piece can cover another if:
    // 1. It's a different player's piece OR it's the same player's piece (allowing moving your own)
    // 2. It's larger than the other piece
    return static_cast<int>(m_size) > static_cast<int>(other.getSize());
}