
#include "GameManager.h"
#include <iostream>
#include <stdexcept>

//------------------------------------------------------------------------
// Constructor initializes the chessboard from a string
GameManager::GameManager(const std::string& boardStr) : m_chessBoard(boardStr),m_isWhiteTurn(true){}

//------------------------------------------------------------------------
// Converts chess notation (e.g., "a7") to board indices (row, col)
std::pair<int, int> GameManager::convertPosition(const std::string& pos) const {

    if (pos.size() != 2 || pos[0] < 'a' || pos[0] > 'h' || pos[1] < '1' || pos[1] > '8') {
        throw std::invalid_argument("Invalid chess position: " + pos);
    }
    int col = pos[0] - 'a';  // Convert 'a'-'h' to 0-7
    int row = 8 - (pos[1] - '0');  // Convert '1'-'8' to 7-0
    return { row, col };
}

//------------------------------------------------------------------------
// Processes a move in chess notation and returns a status code
int GameManager::checkMovement(const std::string& move) {

    if (move.size() != 4) {
        std::cerr << "Invalid move format: " << move << std::endl;
        return INVALID_MOVE;
    }

    std::pair<int, int> from, to;
    try {
        from = convertPosition(move.substr(0, 2));  // e.g., "a7"
        to = convertPosition(move.substr(2, 2));    // e.g., "c6"
    }
    catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
        return INVALID_MOVE;
    }

    // Call ChessBoard::movePiece and return the status code
    return m_chessBoard.movePiece(from, to);
}

//------------------------------------------------------------------------
//Switches the turn between white and black 
void GameManager::switchTurn() {

}

//------------------------------------------------------------------------
//Checks if the current player is in check
bool GameManager::isCheck() {

}