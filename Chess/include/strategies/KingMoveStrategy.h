#pragma once
#include "strategies/MoveStrategy.h"

class KingMoveStrategy : public MoveStrategy {
public:
    int checkMovement(const ChessBoard& board,
                      const std::pair<int, int>& from,
                      const std::pair<int, int>& to) const override;

private:

    // Note: Function declarations for future Castling implementations
    // bool isCastlingMove(const ChessBoard& board, bool isWhite,
    //                     const std::pair<int, int>& from, 
    //                     const std::pair<int, int>& to) const;
    // bool canCastle(const ChessBoard& board, bool isWhite,
    //                const std::pair<int, int>& from, 
    //                const std::pair<int, int>& to) const;
};