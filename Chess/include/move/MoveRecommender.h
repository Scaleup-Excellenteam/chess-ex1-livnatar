#pragma once
#include "board/ChessBoard.h"
#include "move/Move.h"
#include "template/PriorityQueue.h"     
#include <vector>   
#include <climits> // For INT_MIN and INT_MAX
#include "GameConstants.h"

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

    std::vector<Move> generateAllMoves(bool isWhiteTurn, const ChessBoard& board) const;
    int evaluateMove(const Move& move, bool isWhiteTurn, int depth, ChessBoard& boardCopy);
    int evaluatePosition(const Move& move, bool isWhiteTurn, ChessBoard& boardCopy, const ChessPiece* capturedPiece = nullptr) const;
    int getPieceValue(char pieceType) const;
    bool isPieceInDanger(const std::pair<int, int>& pos, bool isWhitePiece, const ChessBoard& board) const;
    int evaluateThreats(const std::pair<int, int>& to, bool isWhiteTurn, const ChessBoard& boardCopy) const;
    int centerControlBonus(const std::pair<int, int>& pos) const;
    int calculateBoardControl(bool isWhiteTurn, const ChessBoard& board) const;

    //int evaluateMove(const Move& move, bool isWhiteTurn, int depth,
     //ChessBoard& boardCopy, int alpha = INT_MIN, int beta = INT_MAX);
    // Original evaluateMove - kept for compatibility
};