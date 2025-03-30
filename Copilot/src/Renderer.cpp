#include "../include/Renderer.h"
#include <SFML/Graphics.hpp>
#include <iostream>

// Actual implementation of the Graphics class using SFML
class Graphics {
public:
    Graphics() : window(nullptr) {}
    ~Graphics() { cleanup(); }
    
    bool initialize(const std::string& title, int width, int height) {
        window = new sf::RenderWindow(sf::VideoMode(width, height), title);
        if (!window) {
            return false;
        }
        window->setFramerateLimit(60);
        return loadResources();
    }
    
    void cleanup() {
        if (window) {
            delete window;
            window = nullptr;
        }
    }
    
    void clear() {
        if (window) {
            window->clear(sf::Color(240, 240, 240)); // Light gray background
        }
    }
    
    void present() {
        if (window) {
            window->display();
        }
    }
    
    bool processEvents(sf::Event& event) {
        if (window && window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
                return false;
            }
            return true;
        }
        return false;
    }
    
    bool isOpen() const {
        return window && window->isOpen();
    }
    
    void drawBoard(int x, int y, int cellSize) {
        sf::RectangleShape board(sf::Vector2f(cellSize * 3, cellSize * 3));
        board.setPosition(x, y);
        board.setFillColor(sf::Color(200, 200, 200));
        board.setOutlineThickness(2);
        board.setOutlineColor(sf::Color::Black);
        window->draw(board);
        
        // Draw grid lines
        for (int i = 1; i < 3; ++i) {
            // Horizontal lines
            sf::RectangleShape hLine(sf::Vector2f(cellSize * 3, 2));
            hLine.setPosition(x, y + i * cellSize);
            hLine.setFillColor(sf::Color::Black);
            window->draw(hLine);
            
            // Vertical lines
            sf::RectangleShape vLine(sf::Vector2f(2, cellSize * 3));
            vLine.setPosition(x + i * cellSize, y);
            vLine.setFillColor(sf::Color::Black);
            window->draw(vLine);
        }
    }
    
    void drawPiece(int x, int y, int playerID, int size) {
        sf::CircleShape piece;
        
        // Size of the piece based on its size category
        float radius;
        switch (size) {
            case 0: radius = 15; break;  // Small
            case 1: radius = 25; break;  // Medium
            case 2: radius = 35; break;  // Large
            default: radius = 15; break;
        }
        
        piece.setRadius(radius);
        piece.setOrigin(radius, radius);
        piece.setPosition(x, y);
        
        // Color based on player ID
        if (playerID == 0) {
            piece.setFillColor(sf::Color(255, 50, 50));  // Red for player 1
        } else {
            piece.setFillColor(sf::Color(50, 50, 255));  // Blue for player 2
        }
        
        piece.setOutlineThickness(2);
        piece.setOutlineColor(sf::Color::Black);
        
        window->draw(piece);
    }
    
    void drawText(const std::string& text, int x, int y, int size, sf::Color color) {
        sf::Text textObj;
        textObj.setFont(font);
        textObj.setString(text);
        textObj.setCharacterSize(size);
        textObj.setFillColor(color);
        textObj.setPosition(x, y);
        window->draw(textObj);
    }
    
    sf::Vector2i getMousePosition() const {
        if (window) {
            return sf::Mouse::getPosition(*window);
        }
        return sf::Vector2i(0, 0);
    }
    
    bool isMouseButtonPressed() const {
        return sf::Mouse::isButtonPressed(sf::Mouse::Left);
    }
    
private:
    sf::RenderWindow* window;
    sf::Font font;
    
    bool loadResources() {
        if (!font.loadFromFile("assets/font.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
            return false;
        }
        return true;
    }
};

// Renderer implementation
Renderer::Renderer() 
    : m_graphics(std::make_unique<Graphics>()), 
      m_windowWidth(800), m_windowHeight(600),
      m_boardX(0), m_boardY(0), m_cellSize(0), m_boardSize(0)
{
}

Renderer::~Renderer()
{
    cleanup();
}

bool Renderer::initialize(const std::string& title, int width, int height)
{
    m_windowWidth = width;
    m_windowHeight = height;
    
    if (!m_graphics->initialize(title, width, height)) {
        return false;
    }
    
    calculateBoardMetrics();
    return true;
}

void Renderer::cleanup()
{
    m_graphics->cleanup();
}

void Renderer::clear()
{
    m_graphics->clear();
}

void Renderer::present()
{
    m_graphics->present();
}

void Renderer::renderGame(const Board& board, const std::array<std::unique_ptr<Player>, 2>& players, 
                          int currentPlayerIndex, bool gameOver, const std::string& message)
{
    clear();
    
    // Draw the board
    renderBoard(board);
    
    // Draw player pieces not on the board
    renderPlayerPieces(*players[0], 50, m_boardY + m_boardSize + 50);
    renderPlayerPieces(*players[1], m_windowWidth - 250, m_boardY + m_boardSize + 50);
    
    // Draw UI elements
    renderUI(currentPlayerIndex, gameOver, message);
    
    present();
}

void Renderer::renderBoard(const Board& board)
{
    // Draw the board grid
    m_graphics->drawBoard(m_boardX, m_boardY, m_cellSize);
    
    // Draw pieces on the board
    for (int row = 0; row < Board::BOARD_SIZE; ++row) {
        for (int col = 0; col < Board::BOARD_SIZE; ++col) {
            const auto& cell = board.getCell(row, col);
            if (!cell.empty()) {
                const Piece* piece = cell.back().get();
                if (piece) {
                    int x = m_boardX + col * m_cellSize + m_cellSize / 2;
                    int y = m_boardY + row * m_cellSize + m_cellSize / 2;
                    m_graphics->drawPiece(x, y, piece->getPlayerID(), static_cast<int>(piece->getSize()));
                }
            }
        }
    }
}

void Renderer::renderPlayerPieces(const Player& player, int x, int y)
{
    const auto& availablePieces = player.getAvailablePieces();
    
    // Draw player name
    std::string name = player.getName();
    m_graphics->drawText(name, x, y - 30, 24, sf::Color::Black);
    
    // Draw available pieces
    for (size_t sizeIdx = 0; sizeIdx < availablePieces.size(); ++sizeIdx) {
        const auto& pieces = availablePieces[sizeIdx];
        
        // Display count of each piece size
        std::string countText = "x" + std::to_string(pieces.size());
        m_graphics->drawText(countText, x + 80, y + sizeIdx * 60, 20, sf::Color::Black);
        
        // Display an example piece
        if (!pieces.empty()) {
            m_graphics->drawPiece(x + 30, y + sizeIdx * 60 + 20, player.getID(), static_cast<int>(sizeIdx));
        }
    }
}

void Renderer::renderUI(int currentPlayerIndex, bool gameOver, const std::string& message)
{
    // Display current player or game over message
    std::string statusMessage;
    
    if (gameOver) {
        statusMessage = message;
        m_graphics->drawText(statusMessage, m_windowWidth / 2 - 150, 30, 36, sf::Color::Red);
    } else {
        statusMessage = "Player " + std::to_string(currentPlayerIndex + 1) + "'s Turn";
        sf::Color playerColor = (currentPlayerIndex == 0) ? sf::Color(255, 50, 50) : sf::Color(50, 50, 255);
        m_graphics->drawText(statusMessage, m_windowWidth / 2 - 100, 30, 28, playerColor);
    }
}

bool Renderer::isBoardCellClicked(int mouseX, int mouseY, int& outRow, int& outCol) const
{
    // Check if the click is within board boundaries
    if (mouseX >= m_boardX && mouseX < m_boardX + m_boardSize &&
        mouseY >= m_boardY && mouseY < m_boardY + m_boardSize) {
        
        // Calculate the row and column
        outCol = (mouseX - m_boardX) / m_cellSize;
        outRow = (mouseY - m_boardY) / m_cellSize;
        
        return true;
    }
    
    return false;
}

bool Renderer::isPlayerPieceClicked(int mouseX, int mouseY, int playerIndex, Piece::Size& outSize) const
{
    int baseX = (playerIndex == 0) ? 50 : (m_windowWidth - 250);
    int baseY = m_boardY + m_boardSize + 50;
    
    // Check for clicks in the piece selection area
    for (int sizeIdx = 0; sizeIdx < 3; ++sizeIdx) {
        int pieceX = baseX + 30;
        int pieceY = baseY + sizeIdx * 60 + 20;
        
        // Simple circular hitbox
        int radius;
        switch (sizeIdx) {
            case 0: radius = 15; break;  // Small
            case 1: radius = 25; break;  // Medium
            case 2: radius = 35; break;  // Large
            default: radius = 15; break;
        }
        
        int dx = mouseX - pieceX;
        int dy = mouseY - pieceY;
        if (dx * dx + dy * dy <= radius * radius) {
            outSize = static_cast<Piece::Size>(sizeIdx);
            return true;
        }
    }
    
    return false;
}

bool Renderer::processEvents()
{
    sf::Event event;
    if (m_graphics->processEvents(event)) {
        return true;
    }
    
    return m_graphics->isOpen();
}

// Implementare pentru getMousePosition
Renderer::MousePosition Renderer::getMousePosition() const
{
    sf::Vector2i mousePos = m_graphics->getMousePosition();
    return {mousePos.x, mousePos.y};
}

// Implementare pentru isMouseButtonPressed
bool Renderer::isMouseButtonPressed() const
{
    return m_graphics->isMouseButtonPressed();
}

void Renderer::calculateBoardMetrics()
{
    // Calculate board size and position
    m_cellSize = 100;
    m_boardSize = m_cellSize * 3;
    
    // Center the board horizontally
    m_boardX = (m_windowWidth - m_boardSize) / 2;
    
    // Position board with some margin from the top
    m_boardY = 100;
}