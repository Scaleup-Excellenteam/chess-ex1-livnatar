
#include "Bishop.h"
#include "ChessBoard.h"
#include "GameConstants.h"

//------------------------------------------------------------------------
Bishop::Bishop(bool isWhite, char pieceType, const std::pair<int, int>& pos)
    : ChessPiece(isWhite, pieceType, pos) {}

//------------------------------------------------------------------------

int Bishop::checkMovement(const ChessBoard& board, const std::pair<int, int>& newPos) const {

    int startRow = m_position.first;
    int startCol = m_position.second;
    int endRow = newPos.first;
    int endCol = newPos.second;
    
    // Check if the move is diagonal: difference between rows == difference between cols
    int rowDiff = abs(endRow - startRow);
    int colDiff = abs(endCol - startCol);
    
    if (rowDiff != colDiff) {
        return MOVE_INVALID_OR_BLOCKED;  // Bishops can only move diagonally
    }
   
    //Direction of movement
    int rowStep = (endRow > startRow) ? 1 : -1;
    int colStep = (endCol > startCol) ? 1 : -1;

    int row = startRow + rowStep;
    int col = startCol + colStep;
   
    //Check for clear path
    while (row != endRow) {  

        if (board.isOccupied(row, col)) {
            return MOVE_INVALID_OR_BLOCKED;
        }
        row += rowStep;
        col += colStep;
    }
    
    // Path is clear - valid move
    return MOVE_SUCCESS;
}