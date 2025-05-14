#pragma once

class ChessPiece;

/**
 * Interface for accessing chess board state
 *
 * This interface defines the contract for classes that provide read-only access to
 * a chess board's state. It allows pieces and movement strategies to query the board
 * without having access to or knowledge of the complete board implementation.
 * This follows the Interface Segregation Principle by providing only the methods
 * necessary for checking piece movements and positions.
 */
class IBoardState {
public:
    virtual bool isOccupied(int row, int col) const = 0;
    virtual const ChessPiece* getPieceAt(int row, int col) const = 0;
    virtual bool isValidPosition(int row, int col) const = 0;
    virtual ~IBoardState() = default;
};