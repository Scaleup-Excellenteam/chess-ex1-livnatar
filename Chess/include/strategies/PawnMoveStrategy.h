
#pragma once
#include "strategies/MoveStrategy.h"

class PawnMoveStrategy : public MoveStrategy {

public:
    int checkMovement(const IBoardState& board,
                      const std::pair<int, int>& from,
                      const std::pair<int, int>& to) const override;

    std::vector<Move> generateMoves(const IBoardState& board,
        const std::pair<int, int>& from,
        bool isWhite,
        char pieceType) const override;

private:
    bool canMoveForward(const IBoardState& board,
                        bool isWhite,
                        const std::pair<int, int>& from,
                        const std::pair<int, int>& to) const;

    bool canMoveTwoSquares(const IBoardState& board,
                           bool isWhite,
                           const std::pair<int, int>& from,
                           const std::pair<int, int>& to) const;

    bool canCaptureDiagonally(const IBoardState& board,
                              bool isWhite,
                              const std::pair<int, int>& from,
                              const std::pair<int, int>& to) const;

    void addForwardMoves(const IBoardState& board,
                         std::vector<Move>& validMoves,
                         const std::pair<int, int>& from,
                         int row, int col, int direction,
                         bool isWhite, char pieceType) const;
    void addCaptureMoves(const IBoardState& board,
                         std::vector<Move>& validMoves,
                         const std::pair<int, int>& from,
                         int row, int col, int direction,
                         bool isWhite, char pieceType) const;

    /* Note: Function declarations for future EnPassant and Promotion implementations
     bool canCaptureEnPassant(const ChessBoard& board, bool isWhite,
                              const std::pair<int, int>& from, 
                              const std::pair<int, int>& to) const;
     bool isPromotionMove(bool isWhite, const std::pair<int, int>& to) const;*/
};