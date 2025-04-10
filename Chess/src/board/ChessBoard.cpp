#include "board/ChessBoard.h"
#include <iostream>
#include <stdexcept>
#include "factories/PieceFactory.h"

//#include "Rook.h" 
//#include "King.h"
//#include "Bishop.h" 
//#include "Queen.h" // Include other pieces when added


//------------------------------------------------------------------------
// Constructor that initializes the board using a string representation

ChessBoard::ChessBoard(const std::string& boardStr) {
    
    // Initialize the factory
    PieceFactory::initialize();

    // Resize the outer vector (rows)
    m_board.resize(8);

    // Resize each inner vector (columns) and initialize them as nullptr
    for (int i = 0; i < 8; ++i) {
        m_board[i].resize(8);
    }

    if (boardStr.size() != 64) {
        std::cout << "Board string must be exactly 64 characters long." << std::endl;
    }

    setupBoard(boardStr);
}


//------------------------------------------------------------------------
// Checks if a given square is occupied

bool ChessBoard::isOccupied(int row, int col) const {
    return m_board[row][col] != nullptr;
}
//------------------------------------------------------------------------
// Returns a pointer to the piece at a given position

const ChessPiece* ChessBoard::getPieceAt(int row, int col) const {
    return m_board[row][col].get();
}
//------------------------------------------------------------------------
// Check the movement of a piece and returns a status code

int ChessBoard::checkMovement(const std::pair<int, int>& from, const std::pair<int, int>& to, bool isWhiteTurn) const {

    //If conversion failed - treat as "no piece at source" - not need to happen cause we check in Chess class
    if (from.first == -1 || to.first == -1) {

        return MOVE_NO_PIECE_IN_SOURCE;  // Code 11
    }

    //Step 1: Check if source square has a piece
    if (!isOccupied(from.first, from.second)) {
        return MOVE_NO_PIECE_IN_SOURCE; // Code 11
    }

    //Step 2: Check if source piece belongs to the current player
    const ChessPiece* sourcePiece = getPieceAt(from.first, from.second);
    if (!sourcePiece || sourcePiece->getColor() != isWhiteTurn) {
        return MOVE_OPPONENT_PIECE_IN_SOURCE; // Code 12
    }

    //Step 3: Check if the destination piece belongs to the current player
    const ChessPiece* destPiece = getPieceAt(to.first, to.second);
    if (destPiece && destPiece->getColor() == isWhiteTurn) {
        return MOVE_FRIENDLY_PIECE_IN_DEST; // Code 13
    }

    //Step 4: Call ChessPiece to validate the move   
    int moveStatus = sourcePiece->checkMovement(*this, to);

    return moveStatus;
}
//------------------------------------------------------------------------
// Sets up the board using a given string

void ChessBoard::setupBoard(const std::string& boardStr) {
   
    for (int i = 0; i < 8; ++i) {
       
        for (int j = 0; j < 8; ++j) {
            
            char pieceChar = boardStr[i * 8 + j];
            std::pair<int, int> pos = { i, j };

            m_board[i][j] = PieceFactory::createPiece(pieceChar, pos);
        }
    }
}
//------------------------------------------------------------------------
void ChessBoard::movePiece(const std::pair<int, int>& from, const std::pair<int, int>& to) {
    
    m_board[to.first][to.second] = std::move(m_board[from.first][from.second]);
    m_board[to.first][to.second]->setPosition(to);
    m_board[from.first][from.second] = nullptr; // Clear the source square
}
//------------------------------------------------------------------------

void ChessBoard::setupPieceAt(char pieceChar, const std::pair<int, int>& pos) {
    
    m_board[pos.first][pos.second] = PieceFactory::createPiece(pieceChar, pos);
}
//------------------------------------------------------------------------
// Prints the board for debugging purposes

void ChessBoard::printBoard() const {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (m_board[i][j])
                std::cout << m_board[i][j]->getPieceType() << " ";
            else
                std::cout << ". ";
        }
        std::cout << "\n";
    }
}