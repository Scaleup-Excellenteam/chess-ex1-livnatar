
#include "GameManager.h"
#include <iostream>
#include <stdexcept>

//------------------------------------------------------------------------
// Constructor initializes the chessboard from a string

GameManager::GameManager(const std::string& boardStr) 
    : m_chessBoard(std::make_unique<ChessBoard>(boardStr)), m_isWhiteTurn(true){}

//------------------------------------------------------------------------
// Converts chess notation (e.g., "a7") to board indices (row, col)

std::pair<int, int> GameManager::convertPosition(const std::string& pos) const 
{
    //maybe change to pos[0] > 'H' , if the player enter C7, H3 ?
    if (pos.size() != 2 || pos[0] < 'a' || pos[0] > 'h' || pos[1] < '1' || pos[1] > '8') {
        return { -1, -1 };  // Indicate invalid input
    }
    int col = pos[0] - 'a';
    int row = 8 - (pos[1] - '0');
    return { row, col };
}

//------------------------------------------------------------------------
// Processes a move in chess notation and returns a status code

int GameManager::checkMovement(const std::string& move) {
    
    if (move.size() != 4) {
        return MOVE_NO_PIECE_IN_SOURCE;  // Invalid format -> Code 11
    }

    std::pair<int, int> from = convertPosition(move.substr(0, 2));
    std::pair<int, int> to = convertPosition(move.substr(2, 2));

    //Step 1: Call ChessBoard to validate the move (does NOT modify board yet)
    int moveStatus = m_chessBoard->checkMovement(from, to, m_isWhiteTurn);

    //Step 2: If move is valid, perform it & switch turn
    if (moveStatus == MOVE_SUCCESS || moveStatus == MOVE_SUCCESS_CHECK) {
        m_chessBoard->movePiece(from, to);  // Now update board
        switchTurn();
    }

    return moveStatus;
}


//------------------------------------------------------------------------
//Switches the turn between white and black 

void GameManager::switchTurn() {

    m_isWhiteTurn = !m_isWhiteTurn;
}

//------------------------------------------------------------------------
//Checks if the current player is in check

bool GameManager::isCheck() const{

    // need to change this function 
    return false;
}