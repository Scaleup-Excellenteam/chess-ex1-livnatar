#include "board/ChessBoard.h"
#include <iostream>
#include <stdexcept>
#include "factories/PieceFactory.h"

//------------------------------------------------------------------------
/**
 * Constructor that initializes the board using a string representation.
 * @param boardStr A 64-character string representing the initial board state.
 */
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
/**
 * Checks if a square on the board is occupied.
 * @param row The row index of the square.
 * @param col The column index of the square.
 * @return True if the square is occupied, false otherwise.
 */
bool ChessBoard::isOccupied(int row, int col) const {
    return m_board[row][col] != nullptr;
}
//------------------------------------------------------------------------
/**
 * Gets the piece at a specific board position.
 * @param row The row index.
 * @param col The column index.
 * @return Pointer to the piece at the position, or nullptr if empty.
 */
const ChessPiece* ChessBoard::getPieceAt(int row, int col) const {
    return m_board[row][col].get();
}
//------------------------------------------------------------------------
/**
 * Validates a piece's movement and returns a status code.
 * @param from The source position of the piece.
 * @param to The destination position.
 * @param isWhiteTurn Indicates whether it's white's turn.
 * @return Status code indicating the result of the move.
 */
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
/**
 * Sets up the board pieces using a string representation.
 * @param boardStr A 64-character string representing the board layout.
 */
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
/**
 * Moves a piece from one square to another.
 * @param from The source position.
 * @param to The destination position.
 */
void ChessBoard::movePiece(const std::pair<int, int>& from, const std::pair<int, int>& to) {
    
    m_board[to.first][to.second] = std::move(m_board[from.first][from.second]);
    m_board[to.first][to.second]->setPosition(to);
    m_board[from.first][from.second] = nullptr; // Clear the source square
}
//------------------------------------------------------------------------
/**
 * Creates and places a specific piece at a given position.
 * @param pieceChar Character representing the piece type.
 * @param pos The position to place the piece at.
 */
void ChessBoard::setupPieceAt(char pieceChar, const std::pair<int, int>& pos) {
    
    m_board[pos.first][pos.second] = PieceFactory::createPiece(pieceChar, pos);
}
//------------------------------------------------------------------------
/**
 * Prints the current board state to the console (for debugging).
 */
/*
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
*/