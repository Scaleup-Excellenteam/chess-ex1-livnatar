
#include "board/GameManager.h"
#include <iostream>
#include <stdexcept>
#include "factories/PieceFactory.h"

//------------------------------------------------------------------------
/**
 * Constructs a GameManager and initializes the board with a given state.
 * @param boardStr String representing the initial board layout.
 */
GameManager::GameManager(const std::string& boardStr) 
    : m_chessBoard(std::make_unique<ChessBoard>(boardStr)),
    m_isWhiteTurn(true),
    m_capturedWhitePieces(),
    m_capturedBlackPieces(),
    m_lastCaptured(nullptr) {}

//------------------------------------------------------------------------
/**
 * Converts a position from chess notation (e.g., "e2") to board coordinates.
 * @param pos The position in chess notation.
 * @return A pair of integers representing (row, column) or (-1, -1) if invalid.
 */
std::pair<int, int> GameManager::convertPosition(const std::string& pos) const {
   
    if (pos.size() != 2) return { -1, -1 };

    if( ( (('A' <= pos[0]) && (pos[0] <= 'H')) || (('a' <= pos[0]) && (pos[0] <= 'h'))) &&
        (('1' <= pos[1]) && (pos[1] <= '8')) ){
       
        // Map 'a' to 0, 'b' to 1, etc.
        int row = tolower(pos[0]) - 'a';

        // Map '1' to 0, '2' to 1, etc.
        int col = pos[1] - '1';

        return { row, col };    
    }

    return { -1, -1 };  // no need to get here - Chess class check for invalid input
}
//------------------------------------------------------------------------
/**
 * Validates and processes a move.
 * If the move is legal and doesn't result in self-check, it is executed.
 * @param move The move string in standard format (e.g., "e2e4").
 * @return Status code indicating the move result.
 */
int GameManager::checkMovement(const std::string& move) {
    
    //Invalid format - treat as code 11 - not need to happen cause we check in Chess class
    if (move.size() != 4) {
        return MOVE_NO_PIECE_IN_SOURCE; 
    }

    std::pair<int, int> from = convertPosition(move.substr(0, 2));
    std::pair<int, int> to = convertPosition(move.substr(2, 2));

    //Step 1: Call ChessBoard to validate the move
    int moveStatus = m_chessBoard->checkMovement(from, to, m_isWhiteTurn);

    //Step 2: If move is valid, perform it and check results
    if (moveStatus == MOVE_SUCCESS) {
        
        makeMove(from, to);

        // Step 3: Ensure the move does not put the player in check - code 31
        if (isCheck()) {
            undoLastMove(from, to);  
            return MOVE_CAUSES_CHECK;           
        }

        // Step 4: Check if this move puts the opponent in check - code 41
        int finalMoveStatus = checkOpponentInCheck();

        //If everything is valid, switch turn
        switchTurn();

        return finalMoveStatus;
    }

    return moveStatus;
}


//------------------------------------------------------------------------
/**
 * Switches the active player from white to black or vice versa.
 */
void GameManager::switchTurn() {

    m_isWhiteTurn = !m_isWhiteTurn;
}

//------------------------------------------------------------------------
/**
 * Checks if the current player's king is under attack.
 * @return True if the current player is in check, false otherwise.
 */
bool GameManager::isCheck() const{
   
    // Find the king of the current player
    std::pair<int, int> kingPos = findKingPosition(m_isWhiteTurn);

    // If king not found (shouldn't happen in a valid game), return false
    if (kingPos.first == -1 || kingPos.second == -1) {
        return false;
    }

    // Check if any opponent piece can attack the king
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            
            const ChessPiece* piece = m_chessBoard->getPieceAt(row, col);

            // Skip empty squares and friendly pieces
            if (!piece || piece->getColor() == m_isWhiteTurn) {
                continue;
            }

            // Check if this opponent piece can move to the king's position
            if ( (piece->checkMovement(*m_chessBoard, kingPos)) == MOVE_SUCCESS) {
                return true;  
            }
        }
    }

    return false;  
}

//------------------------------------------------------------------------
/**
 * Finds the current position of the specified color's king.
 * @param isWhiteKing True if searching for the white king, false for black.
 * @return A pair of integers representing the king's position or (-1, -1) if not found.
 */
std::pair<int, int> GameManager::findKingPosition(bool isWhiteKing) const {
    
    for (int row = 0; row < 8; ++row) {
        
        for (int col = 0; col < 8; ++col) {
            
            const ChessPiece* piece = m_chessBoard->getPieceAt(row, col);

            // Check if this square has a king of the specified color
            if (piece && piece->getColor() == isWhiteKing &&
                (piece->getPieceType() == 'K' || piece->getPieceType() == 'k')) {
                
                return { row, col };
            }
        }
    }

    // King not found (shouldn't happen in a valid game)
    return{ -1,-1 };
}

//------------------------------------------------------------------------
/**
 * Checks if the opponent is in check after the current move.
 * @return MOVE_SUCCESS_CHECK if opponent is in check, MOVE_SUCCESS otherwise.
 */
int GameManager::checkOpponentInCheck() {
   
    // Temporarily switch turn to check if opponent in check
    switchTurn(); 
    bool opponentInCheck = isCheck();
    
    switchTurn(); // Switch back to current player

    // Return correct success code
    if (opponentInCheck) {
        return MOVE_SUCCESS_CHECK; // Code 41 - valid move that puts opponent in check
    }
    else {
        return MOVE_SUCCESS;       // Code 42 - valid move
    }
   
    return MOVE_SUCCESS;
}

//------------------------------------------------------------------------
/**
 * Executes a valid move, updates the board, and handles any captured piece.
 * @param from The source coordinates of the moving piece.
 * @param to The destination coordinates of the moving piece.
 */
void GameManager::makeMove(const std::pair<int, int>& from, const std::pair<int, int>& to) {
   
    // Save piece that might be captured
    const ChessPiece* destPiece = m_chessBoard->getPieceAt(to.first, to.second);

    if (destPiece) {
       
        // Copy of the piece data before it gets deleted
        char pieceType = destPiece->getPieceType();
        bool pieceColor = destPiece->getColor();
        std::pair<int, int> piecePos = destPiece->getPosition();

        // Copy for the captured pieces collection
        std::unique_ptr<ChessPiece> capturedPiece = PieceFactory::createPiece(pieceType, piecePos);

        // Store in the appropriate collection
        if (pieceColor) { 
            m_capturedWhitePieces.push_back(std::move(capturedPiece));
        }
        else {
            m_capturedBlackPieces.push_back(std::move(capturedPiece));
        }

        // Create another copy for undo 
        m_lastCaptured = PieceFactory::createPiece(pieceType, piecePos);
    }
    else {
        m_lastCaptured = nullptr;
    }

    // Now move the piece in the board
    m_chessBoard->movePiece(from, to);
}
//------------------------------------------------------------------------
/**
 * Reverts the last move, restoring both piece position and any captured piece.
 * @param from The original source of the move.
 * @param to The destination the piece moved to before being undone.
 */
void GameManager::undoLastMove(const std::pair<int, int>& from, const std::pair<int, int>& to) {
    
    // Move the piece back to its original position on the board
    m_chessBoard->movePiece(to, from);

    // If a piece was captured, restore it
    if (m_lastCaptured) {
        
        // Extract the piece data
        std::pair<int, int> pos = m_lastCaptured->getPosition();
        char pieceType = m_lastCaptured->getPieceType();
        bool pieceColor = m_lastCaptured->getColor();

        // Create a new piece on the board at the captured position
        m_chessBoard->setupPieceAt(pieceType, pos);

        // Remove it from the captured pieces collection
        auto& capturedPieces = pieceColor ? m_capturedWhitePieces : m_capturedBlackPieces;
        if (!capturedPieces.empty()) {
            capturedPieces.pop_back(); 
        }

        // Clear the last captured 
        m_lastCaptured = nullptr;
    }
}

/*
* for future:

const std::vector<std::unique_ptr<ChessPiece>>& GameManager::getCapturedWhitePieces() const {
    return m_capturedWhitePieces;
}

const std::vector<std::unique_ptr<ChessPiece>>& GameManager::getCapturedBlackPieces() const {
    return m_capturedBlackPieces;
}
*/