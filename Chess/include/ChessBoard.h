#pragma once
#include <vector>
#include <memory>
#include <string>
#include "ChessPiece.h"
#include "GameConstants.h" 


class ChessBoard {
public:
    // Constructor that initializes the board from a string
    ChessBoard(const std::string& boardStr);
    ~ChessBoard() = default;

    // Checks if a square is occupied
    bool isOccupied(int row, int col) const;

    // Returns a pointer to the piece at a given position (const to prevent modification)
    const ChessPiece* getPieceAt(int row, int col) const;

    // Check the movement of a piece and returns a status code
    int checkMovement(std::pair<int, int> from, std::pair<int, int> to);

    // Prints the board (for debugging)
    void printBoard() const;

private:
    std::vector<std::vector<std::unique_ptr<ChessPiece>>> m_board;

    // Initializes the board from a given string
    void setupBoard(const std::string& boardStr);
};
