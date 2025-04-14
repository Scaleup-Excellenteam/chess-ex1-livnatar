
#pragma once
#include "strategies/MoveStrategy.h"

class PawnMoveStrategy : public MoveStrategy {

public:
    int checkMovement(const ChessBoard& board,
                      const std::pair<int, int>& from,
                      const std::pair<int, int>& to) const override;

private:
    bool canMoveForward(const ChessBoard& board,
                        bool isWhite,
                        const std::pair<int, int>& from,
                        const std::pair<int, int>& to) const;

    bool canMoveTwoSquares(const ChessBoard& board,
                           bool isWhite,
                           const std::pair<int, int>& from,
                           const std::pair<int, int>& to) const;

    bool canCaptureDiagonally(const ChessBoard& board,
                              bool isWhite,
                              const std::pair<int, int>& from,
                              const std::pair<int, int>& to) const;

    /* Note: Function declarations for future EnPassant and Promotion implementations
     bool canCaptureEnPassant(const ChessBoard& board, bool isWhite,
                              const std::pair<int, int>& from, 
                              const std::pair<int, int>& to) const;
     bool isPromotionMove(bool isWhite, const std::pair<int, int>& to) const;*/
};