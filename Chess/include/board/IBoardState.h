#pragma once

class ChessPiece;

class IBoardState {
public:
    virtual bool isOccupied(int row, int col) const = 0;
    virtual const ChessPiece* getPieceAt(int row, int col) const = 0;
    virtual bool isValidPosition(int row, int col) const = 0;
    virtual ~IBoardState() = default;
};