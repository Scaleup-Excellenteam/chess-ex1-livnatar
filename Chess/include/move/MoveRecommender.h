#pragma once
#include "board/ChessBoard.h"
#include "move/Move.h"
#include "template/PriorityQueue.h"     
#include <vector>   
#include <climits> // For INT_MIN and INT_MAX
#include "GameConstants.h"


/**
 * Minimal move representation for making/undoing moves during search.
 * Lighter than Move class (no score/notation), optimized for memory and speed.
 */
struct MoveData {
    std::pair<int, int> from;
    std::pair<int, int> to;
    char capturedPieceType;     // '#' if no piece was captured
    bool capturedPieceColor;    // true for white, false for black
    // bool wasFirstMove;          // TODO: For tracking special moves like castling
    
    // Constructor to initialize the members
    MoveData()
        : from(0, 0), to(0, 0), capturedPieceType('#'), capturedPieceColor(false) {}
};

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
    int getPieceValue(char pieceType) const;
    bool isPieceInDanger(const std::pair<int, int>& pos, bool isWhitePiece, const ChessBoard& board) const;
    int evaluateThreats(const std::pair<int, int>& to, bool isWhiteTurn, const ChessBoard& boardCopy) const;
    int centerControlBonus(const std::pair<int, int>& pos) const;
    int calculateBoardControl(bool isWhiteTurn, const ChessBoard& board) const;
    MoveData makeMoveAndGetData(const Move& move, ChessBoard& board);
    void undoMove(const MoveData& data, ChessBoard& board);
    int evaluatePosition(const Move& move, bool isWhiteTurn, ChessBoard& board,
                         char capturedPieceType, bool capturedPieceColor) const;

    //int evaluateMove(const Move& move, bool isWhiteTurn, int depth,
    //ChessBoard& boardCopy, int alpha = INT_MIN, int beta = INT_MAX);
    // Original evaluateMove - kept for compatibility

    int evaluateMove(const Move& move, bool isWhiteTurn, int depth, ChessBoard& board, bool isMaximizingPlayer = true);
};