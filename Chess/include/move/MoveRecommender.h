#pragma once

#include <vector>
#include "board/ChessBoard.h"
#include "move/Move.h"
#include "template/PriorityQueue.h"


/**
 * Class responsible for analyzing the chess board and recommending
 * the best moves for a player.
 */
class MoveRecommender {
public:
    MoveRecommender(const ChessBoard& board, int depth = 2);

    PriorityQueue<Move> getRecommendations(bool isWhiteTurn);
    int getDepth() const;
    void setDepth(int depth);

private:
    const ChessBoard& m_board;
    int m_depth;
    PriorityQueue<Move> m_recommendations;


    std::vector<Move> generateAllMoves(bool isWhiteTurn) const;
    std::vector<Move> generateMovesForPiece(const std::pair<int, int>& pos, bool isWhiteTurn) const;
    int evaluateMove(const Move& move, bool isWhiteTurn, int depth, ChessBoard& boardCopy);
    int evaluatePosition(const Move& move, bool isWhiteTurn, const ChessBoard& boardCopy) const;
    int getPieceValue(char pieceType) const;
    bool isPieceInDanger(const std::pair<int, int>& pos, bool isWhitePiece, const ChessBoard& board) const;
    int centerControlBonus(const std::pair<int, int>& pos) const;
    int calculateBoardControl(bool isWhiteTurn, const ChessBoard& board) const;
};