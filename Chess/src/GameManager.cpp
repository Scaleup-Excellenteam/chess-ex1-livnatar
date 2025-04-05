
#include "GameManager.h"
#include <iostream>
#include <stdexcept>

//------------------------------------------------------------------------
// Constructor initializes the chessboard from a string

GameManager::GameManager(const std::string& boardStr) 
    : m_chessBoard(std::make_unique<ChessBoard>(boardStr)), m_isWhiteTurn(true){
    m_chessBoard->printBoard();
}

//------------------------------------------------------------------------
// Converts chess notation (e.g., "a7") to board indices (row, col)

std::pair<int, int> GameManager::convertPosition(const std::string& pos) const {
   
    if (pos.size() != 2) return { -1, -1 };

    char loc = tolower(pos[0]); // Convert 'A'-'H' to 'a'-'h'
    if (loc < 'a' || loc > 'h' || pos[1] < '1' || pos[1] > '8') {
        return { -1, -1 };
    }

    // Map 'a' to 0, 'b' to 1, etc.
    int col = loc - 'a';

    // Map '1' to 0, '2' to 1, etc.
    int row = pos[1] - '1';

    return { col, row };
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

    //Step 2: If move is valid, perform it & check results
    if (moveStatus == MOVE_SUCCESS) {
        
        m_chessBoard->movePiece(from, to);  // Now update board

        // Step 3: Ensure the move does not put the player in check (Code 31)
        if (isCheck()) {
            m_chessBoard->movePiece(to, from);  // Undo the move before returning
            return MOVE_CAUSES_CHECK;           // Code 31
        }

        // Step 4: Check if this move puts the opponent in check - code 41
        int finalMoveStatus = checkOpponentInCheck();

        //If everything is valid, switch turn
        switchTurn();

        return finalMoveStatus;
    }

    /*
    //Step 2: If move is valid, perform it & switch turn
    if (moveStatus == MOVE_SUCCESS || moveStatus == MOVE_SUCCESS_CHECK) {
        m_chessBoard->movePiece(from, to);  // Now update board

        // Step 3: Ensure the move does not put the player in check (Code 31)
        if (isCheck()) {
            m_chessBoard->movePiece(to, from);  // Undo the move before returning
            return MOVE_CAUSES_CHECK;           // Code 31
        }

        //If everything is valid, switch turn
        switchTurn();
    }
    */

    return moveStatus;
}


//------------------------------------------------------------------------
//Switches the turn between white and black 

void GameManager::switchTurn() {

    m_isWhiteTurn = !m_isWhiteTurn;
}

//------------------------------------------------------------------------
//Checks if the current player is in check - code 31

bool GameManager::isCheck() const{
   
    // need to change this function 
    return false;

    /*
* bool GameManager::isCheck(bool isWhite) const {
    std::pair<int, int> kingPos = m_chessBoard->findKing(isWhite);
    if (kingPos.first == -1) {
        return false;  // King not found (should not happen in a valid game)
    }

    // Check if any opponent piece can attack the king
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            const ChessPiece* piece = m_chessBoard->getPieceAt(row, col);
            if (piece && piece->getColor() != isWhite) {
                if (piece->checkMovement(*m_chessBoard, kingPos) == MOVE_SUCCESS) {
                    return true;  // King is under attack
                }
            }
        }
    }
    return false; // No threat to the king
}

*/
}

//------------------------------------------------------------------------
// Helper method to check if a move puts the opponent in check - code 41
int GameManager::checkOpponentInCheck() {
   
    /*    
    switchTurn(); // Temporarily switch turn to check from opponent's perspective
    bool opponentInCheck = isCheck();
    switchTurn(); // Switch back to current player

    // Return appropriate success code
    if (opponentInCheck) {
        return MOVE_SUCCESS_CHECK; // Code 41 - valid move that puts opponent in check
    }
    else {
        return MOVE_SUCCESS;       // Code 42 - valid move
    }*/
    return MOVE_SUCCESS;
}