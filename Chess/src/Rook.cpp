
#include "Rook.h"
#include "ChessBoard.h"
#include "GameConstants.h"

//------------------------------------------------------------------------
Rook::Rook(bool isWhite, char pieceType, const std::pair<int, int>& pos)
    : ChessPiece(isWhite, pieceType, pos) {}

//------------------------------------------------------------------------
int Rook::checkMovement(const ChessBoard& board, const std::pair<int, int>& newPos) const {
    
    // For simplicity, let's assume the rook can move horizontally or vertically, and the path is clear.
    if (m_position.first != newPos.first && m_position.second != newPos.second) {
        return MOVE_INVALID_OR_BLOCKED; // Can't move diagonally
    }

    // Check if the path is clear
    int startRow = m_position.first;
    int startCol = m_position.second;
    int endRow = newPos.first;
    int endCol = newPos.second;

    // Check if moving vertically
    if (startCol == endCol) {
        int step = (endRow > startRow) ? 1 : -1;
        for (int row = startRow + step; row != endRow; row += step) {
            if (board.isOccupied(row, startCol)) {
                return MOVE_INVALID_OR_BLOCKED;
            }
        }
    }
    // Check if moving horizontally
    else {
        int step = (endCol > startCol) ? 1 : -1;
        for (int col = startCol + step; col != endCol; col += step) {
            if (board.isOccupied(startRow, col)) {
                return MOVE_INVALID_OR_BLOCKED;
            }
        }
    }

    return MOVE_SUCCESS; // Path is clear
}