#include "ChessBoard.h"
#include <iostream>
#include <stdexcept>
#include "Rook.h" // Include other pieces when added


//------------------------------------------------------------------------
// Constructor that initializes the board using a string representation
ChessBoard::ChessBoard(const std::string& boardStr) 
    : board(8, std::vector<std::unique_ptr<ChessPiece>>(8, nullptr)) {
    if (boardStr.size() != 64) {
        throw std::invalid_argument("Board string must be exactly 64 characters long.");
    }
    setupBoard(boardStr);
}

//------------------------------------------------------------------------
// Checks if a given square is occupied
bool ChessBoard::isOccupied(int row, int col) const {
    return board[row][col] != nullptr;
}

//------------------------------------------------------------------------
// Returns a pointer to the piece at a given position
const ChessPiece* ChessBoard::getPieceAt(int row, int col) const {
    return board[row][col].get();
}

//------------------------------------------------------------------------
// Moves a piece and returns a status code
int ChessBoard::movePiece(std::pair<int, int> from, std::pair<int, int> to) {
    if (!isOccupied(from.first, from.second)) {
        std::cout << "No piece at the source square!\n";
        return NO_PIECE;
    }

    ChessPiece* piece = board[from.first][from.second].get();
    int moveStatus = piece->isMoveValid(*this, to);
    if (moveStatus == MOVE_SUCCESS) {
        board[to.first][to.second] = std::move(board[from.first][from.second]);
        board[to.first][to.second]->setPosition(to);
        return MOVE_SUCCESS;
    }

    std::cout << "Invalid move!\n";
    return INVALID_MOVE;
}

//------------------------------------------------------------------------
// Sets up the board using a given string
void ChessBoard::setupBoard(const std::string& boardStr) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            char pieceChar = boardStr[i * 8 + j];
            std::pair<int, int> pos = { i, j };

            switch (pieceChar) {
            case 'R': board[i][j] = std::make_unique<Rook>(true, 'R', pos); break;
            case 'r': board[i][j] = std::make_unique<Rook>(false, 'r', pos); break;
            case '#': board[i][j] = nullptr; break; // Empty square
                // More pieces will be added later
            default: break;
            }
        }
    }
}

//------------------------------------------------------------------------
// Prints the board for debugging purposes
void ChessBoard::printBoard() const {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j])
                std::cout << board[i][j]->getPieceType() << " ";
            else
                std::cout << ". ";
        }
        std::cout << "\n";
    }
}
