#include "../include/Player.h"
#include <algorithm>

Player::Player(int id, const std::string& name)
    : m_id(id), m_name(name)
{
    // Initialize the player's pieces
    // We need 3 sizes (small, medium, large), with PIECES_PER_SIZE pieces of each
    m_availablePieces.resize(3); // 3 sizes: small, medium, large
    
    // For each size, create the pieces
    for (int sizeIdx = 0; sizeIdx < 3; ++sizeIdx) {
        auto size = static_cast<Piece::Size>(sizeIdx);
        for (int i = 0; i < PIECES_PER_SIZE; ++i) {
            m_availablePieces[sizeIdx].push_back(std::make_unique<Piece>(m_id, size));
        }
    }
}

std::unique_ptr<Piece> Player::selectPiece(Piece::Size size)
{
    int sizeIdx = static_cast<int>(size);
    
    // If there are no pieces of this size available, return nullptr
    if (m_availablePieces[sizeIdx].empty()) {
        return nullptr;
    }
    
    // Take the last piece from the vector (for efficiency)
    std::unique_ptr<Piece> piece = std::move(m_availablePieces[sizeIdx].back());
    m_availablePieces[sizeIdx].pop_back();
    
    return piece;
}

bool Player::hasPieceAvailable(Piece::Size size) const
{
    int sizeIdx = static_cast<int>(size);
    return !m_availablePieces[sizeIdx].empty();
}

void Player::returnPiece(std::unique_ptr<Piece> piece)
{
    if (!piece) {
        return;
    }
    
    int sizeIdx = static_cast<int>(piece->getSize());
    m_availablePieces[sizeIdx].push_back(std::move(piece));
}

int Player::getAvailablePieces(Piece::Size size) const
{
    int sizeIdx = static_cast<int>(size);
    return static_cast<int>(m_availablePieces[sizeIdx].size());
}

const std::vector<std::vector<std::unique_ptr<Piece>>>& Player::getAvailablePieces() const
{
    return m_availablePieces;
}