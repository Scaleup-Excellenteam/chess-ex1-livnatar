

#include "pieces/Queen.h"
//#include "board/ChessBoard.h"
//#include "GameConstants.h"

//------------------------------------------------------------------------
Queen::Queen(bool isWhite, char pieceType, const std::pair<int, int>& pos)
    : ChessPiece(isWhite, pieceType, pos) {}

/*
//------------------------------------------------------------------------
int Queen::checkMovement(const ChessBoard& board, const std::pair<int, int>& newPos) const {
   
    int startRow = m_position.first;
    int startCol = m_position.second;
    int endRow = newPos.first;
    int endCol = newPos.second;

    // Check if move is horizontal, vertical, or diagonal
    bool isHorizontalMove = (startRow == endRow);
    bool isVerticalMove = (startCol == endCol);

    int rowDiff = abs(endRow - startRow);
    int colDiff = abs(endCol - startCol);
    bool isDiagonalMove = (rowDiff == colDiff);

    // If move is not horizontal, vertical, or diagonal, it's invalid
    if (!isHorizontalMove && !isVerticalMove && !isDiagonalMove) {
        return MOVE_INVALID_OR_BLOCKED;
    }

    // Check for clear path based on move type
    if (isHorizontalMove) {
        if (!isPathClearHorizontal(board, startRow, startCol, endCol)) {
            return MOVE_INVALID_OR_BLOCKED;
        }
    }
    else if (isVerticalMove) {
        if (!isPathClearVertical(board, startCol, startRow, endRow)) {
            return MOVE_INVALID_OR_BLOCKED;
        }
    }
    else if (isDiagonalMove) {
        if (!isPathClearDiagonal(board, startRow, startCol, endRow, endCol)) {
            return MOVE_INVALID_OR_BLOCKED;
        }
    }

    // Path is clear - valid move
    return MOVE_SUCCESS;
}
//------------------------------------------------------------------------
bool Queen::isPathClearHorizontal(const ChessBoard& board, int startRow, int startCol, int endCol) const {
    
    int step = (endCol > startCol) ? 1 : -1;
    for (int col = startCol + step; col != endCol; col += step) {
        if (board.isOccupied(startRow, col)) {
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------
bool Queen::isPathClearVertical(const ChessBoard& board, int startCol, int startRow, int endRow) const {
    
    int step = (endRow > startRow) ? 1 : -1;
    for (int row = startRow + step; row != endRow; row += step) {
        if (board.isOccupied(row, startCol)) {
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------
bool Queen::isPathClearDiagonal(const ChessBoard& board, int startRow, int startCol, int endRow, int endCol) const {
    
    int rowStep = (endRow > startRow) ? 1 : -1;
    int colStep = (endCol > startCol) ? 1 : -1;

    int row = startRow + rowStep;
    int col = startCol + colStep;

    while (row != endRow) {
        if (board.isOccupied(row, col)) {
            return false;
        }
        row += rowStep;
        col += colStep;
    }
    return true;
}
*/