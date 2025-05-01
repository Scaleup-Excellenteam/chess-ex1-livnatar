
#include "move/MoveRecommender.h"
#include <algorithm>
#include <iostream>


//------------------------------------------------------------------------
/**
 * Constructor for MoveRecommender
 *
 * @param board Reference to the chess board
 * @param depth How many turns to look ahead (default 2)
 */
MoveRecommender::MoveRecommender(const ChessBoard& board, int depth)
    : m_board(board), m_depth(depth), m_recommendations(5) {};

//------------------------------------------------------------------------
/**
* Generate and return recommendations for the current player
*
* @param isWhiteTurn True if generating recommendations for white
* @return PriorityQueue containing the best moves
*/
PriorityQueue<Move> MoveRecommender::getRecommendations(bool isWhiteTurn) {

    // Clear previous recommendations
    m_recommendations = PriorityQueue<Move>(5);

    // Generate all possible moves
    std::vector<Move> allMoves = generateAllMoves(isWhiteTurn);

    // For each move, create a copy of the board and evaluate
    for (auto& move : allMoves) {
        // Create a copy of the board for simulation
        ChessBoard boardCopy(m_board); // Using copy constructor

        // Evaluate the move
        int score = evaluateMove(move, isWhiteTurn, m_depth, boardCopy);
        move.setScore(score);

        try {
            m_recommendations.push(move);
        }
        catch (const QueueFullException& e) {
            // Queue is full and this move isn't good enough
            // Just ignore it
        }
    }

    return m_recommendations;
}
//------------------------------------------------------------------------
/**
* Generate all valid moves for a player
*
* @param isWhiteTurn True if generating moves for white player
* @return Vector of all valid moves for the player
*/
std::vector<Move> MoveRecommender::generateAllMoves(bool isWhiteTurn) const {

    std::vector<Move> allMoves;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
           
            const ChessPiece* piece = m_board.getPieceAt(row, col);

            if (piece && piece->getColor() == isWhiteTurn) {
               
                std::pair<int, int> pos = { row, col };

                // Get all valid moves for this piece
                std::vector<Move> pieceMoves = generateMovesForPiece(pos, isWhiteTurn);

                // Add them to our collection
                allMoves.insert(allMoves.end(), pieceMoves.begin(), pieceMoves.end());
            }
        }
    }

    return allMoves;
}
//------------------------------------------------------------------------
/**
* Generate all valid moves for a specific piece
*
* @param pos Position of the piece
* @param isWhiteTurn True if generating moves for white pieces
* @return Vector of all valid moves for the piece
*/
std::vector<Move> MoveRecommender::generateMovesForPiece(const std::pair<int, int>& pos, bool isWhiteTurn) const {

    std::vector<Move> moves;
    const ChessPiece* piece = m_board.getPieceAt(pos.first, pos.second);

    if (!piece || piece->getColor() != isWhiteTurn) {
        return moves;
    }

    char pieceType = piece->getPieceType();

    // Check all possible destinations on the board
    for (int destRow = 0; destRow < 8; ++destRow) {
        for (int destCol = 0; destCol < 8; ++destCol) {

            std::pair<int, int> destPos = { destRow, destCol };

            // Skip the source position
            if (destRow == pos.first && destCol == pos.second) {
                continue;
            }

            // Check if move is valid according to piece rules and the board
            int moveStatus = m_board.checkMovement(pos, destPos, isWhiteTurn);

            if (moveStatus == MOVE_SUCCESS) {
                // Valid move
                Move move(pos, destPos, 0, pieceType);
                moves.push_back(move);
            }
        }
    }

    return moves;
}
//------------------------------------------------------------------------
/**
* Calculate score for a potential move with recursive depth
*
* @param move The move to evaluate
* @param isWhiteTurn True if it's white's turn
* @param depth How many turns to look ahead
* @param boardCopy Copy of the chess board for simulation
* @return Score indicating the move's quality
*/
int MoveRecommender::evaluateMove(const Move& move, bool isWhiteTurn, int depth, ChessBoard& boardCopy) {

    const std::pair<int, int>& from = move.getFrom();
    const std::pair<int, int>& to = move.getTo();

    // Make the move on our board copy
    boardCopy.movePiece(from, to);

    // Calculate immediate score for this position
    int score = evaluatePosition(move, isWhiteTurn, boardCopy);

    // Base case: if we've reached maximum depth, return the score
    if (depth <= 0) {
        return score;
    }

    // Look at opponent's responses
    bool opponentTurn = !isWhiteTurn;
    std::vector<Move> opponentMoves = generateAllMoves(opponentTurn);

    // If opponent has no moves, this is good for us
    if (opponentMoves.empty()) {
        return score + 50; // Bonus for limiting opponent options
    }

    // Find opponent's best move
    int bestOpponentScore = INT_MIN;
    for (const auto& opponentMove : opponentMoves) {
        
        // Create another board copy for deeper simulation
        ChessBoard deeperCopy(boardCopy);

        // Evaluate opponent's move (with depth - 1)
        int opponentScore = evaluateMove(opponentMove, opponentTurn, depth - 1, deeperCopy);

        // Update best score (from opponent's perspective)
        bestOpponentScore = std::max(bestOpponentScore, opponentScore);
    }

    // From our perspective, opponent's gain is our loss
    score -= bestOpponentScore;

    return score;
}
//------------------------------------------------------------------------
/**
* Calculate the immediate score for a position after a move
*
* @param move The move to evaluate
* @param isWhiteTurn True if it's white's turn
* @param boardCopy Reference to the board after the move
* @return Score for the position after the move
*/
int MoveRecommender::evaluatePosition(const Move& move, bool isWhiteTurn, const ChessBoard& boardCopy) const {

    int score = 0;
    const std::pair<int, int>& from = move.getFrom();
    const std::pair<int, int>& to = move.getTo();

    const ChessPiece* movedPiece = boardCopy.getPieceAt(to.first, to.second);

    if (!movedPiece) {
        return -1000; // Something went wrong with the simulation
    }

    // 1. Capture value (we already captured in the simulation)
    // If this was a capture, the piece is no longer at the destination
    // We can detect captures by comparing board states, but for simplicity,
    // we'll check if the destination had an opponent piece in the original board
    const ChessPiece* originalDestPiece = m_board.getPieceAt(to.first, to.second);
    if (originalDestPiece && originalDestPiece->getColor() != isWhiteTurn) {
        score += getPieceValue(originalDestPiece->getPieceType());
    }

    // 2. Check if our piece is in danger after the move
    if (isPieceInDanger(to, isWhiteTurn, boardCopy)) {
        int movedPieceValue = getPieceValue(movedPiece->getPieceType());
        score -= movedPieceValue; // Penalty for putting piece in danger
    }

    // 3. Bonus for threatening opponent pieces after the move
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            
            if (row == to.first && col == to.second) continue;    // Skip the destination square

            const ChessPiece* targetPiece = boardCopy.getPieceAt(row, col);
            if (targetPiece && targetPiece->getColor() != isWhiteTurn) {
                
                std::pair<int, int> targetPos = { row, col };

                // If our piece can attack this opponent piece
                if (movedPiece->checkMovement(boardCopy, targetPos) == MOVE_SUCCESS) {
                    
                    int targetValue = getPieceValue(targetPiece->getPieceType());
                    int movedPieceValue = getPieceValue(movedPiece->getPieceType());

                    if (targetValue > movedPieceValue) {
                        // Threatening a stronger piece
                        score += 3;
                    }
                    else if (targetValue == movedPieceValue) {
                        // Threatening an equal piece
                        score += 2;
                    }
                    else {
                        // Threatening a weaker piece
                        score += 1;
                    }
                }
            }
        }
    }

    // 4. Center control bonus
    score += centerControlBonus(to);

    // 5. Board control difference
    score += calculateBoardControl(isWhiteTurn, boardCopy);

    return score;
}
//------------------------------------------------------------------------
/**
* Calculate the material value of a chess piece
*
* @param pieceType Character representing the piece
* @return Numerical value of the piece
*/
int MoveRecommender::getPieceValue(char pieceType) const {

    switch (std::toupper(pieceType)) {
    case 'P': return 1;  // Pawn
    case 'N': return 3;  // Knight
    case 'B': return 3;  // Bishop
    case 'R': return 5;  // Rook
    case 'Q': return 9;  // Queen
    case 'K': return 100; // King - very high value
    default: return 0;
    }
}
//------------------------------------------------------------------------
/**
* Check if a piece would be in danger at the given position
*
* @param pos Position to check
* @param isWhitePiece True if checking for a white piece
* @param board The board state to check on
* @return True if the piece would be threatened
*/
bool MoveRecommender::isPieceInDanger(const std::pair<int, int>& pos, bool isWhitePiece, const ChessBoard& board) const {

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {

            const ChessPiece* piece = board.getPieceAt(row, col);
            if (piece && piece->getColor() != isWhitePiece) {
               
                std::pair<int, int> opponentPos = { row, col };

                // Check if this opponent piece can attack our piece
                if (piece->checkMovement(board, pos) == MOVE_SUCCESS) {
                    
                    int attackerValue = getPieceValue(piece->getPieceType());
                    const ChessPiece* targetPiece = board.getPieceAt(pos.first, pos.second);

                    if (targetPiece) {
                        int targetValue = getPieceValue(targetPiece->getPieceType());

                        // We're especially concerned when a lower-value piece threatens our higher-value piece
                        if (attackerValue < targetValue) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}
//------------------------------------------------------------------------
/**
* Calculate a bonus for controlling center squares
*
* @param pos Position to evaluate
* @return Bonus score for center control
*/
int MoveRecommender::centerControlBonus(const std::pair<int, int>& pos) const {

    // Center squares are rows 3-4, columns 3-4 (0-indexed)
    if ((pos.first >= 3 && pos.first <= 4) && (pos.second >= 3 && pos.second <= 4)) {
        return 3; // Higher bonus for direct center control
    }
    // Extended center (rows 2-5, columns 2-5)
    else if ((pos.first >= 2 && pos.first <= 5) && (pos.second >= 2 && pos.second <= 5)) {
        return 1; // Smaller bonus for extended center
    }
    return 0;
}
//------------------------------------------------------------------------
/**
* Calculates how many squares are controlled by each player
*
* @param isWhiteTurn True if calculating for white player
* @param board The board state to check on
* @return Difference between our control and opponent's control (positive is good for us)
*/
int MoveRecommender::calculateBoardControl(bool isWhiteTurn, const ChessBoard& board) const {

    int ourControl = 0;
    int opponentControl = 0;

    // Count controlled squares for each side
    for (int destRow = 0; destRow < 8; ++destRow) {
        for (int destCol = 0; destCol < 8; ++destCol) {

            std::pair<int, int> destPos = { destRow, destCol };
            bool controlledByUs = false;
            bool controlledByOpponent = false;

            // Check which pieces can move to this square
            for (int row = 0; row < 8; ++row) {
                for (int col = 0; col < 8; ++col) {
                    
                    const ChessPiece* piece = board.getPieceAt(row, col);
                    if (!piece) continue;  // empty square

                    std::pair<int, int> pos = { row, col };

                    // Skip checking if the square has a piece of the same color
                    const ChessPiece* destPiece = board.getPieceAt(destRow, destCol);
                    if (destPiece && destPiece->getColor() == piece->getColor()) {
                        continue;
                    }

                    // Check if this piece controls the destination square
                    if (piece->checkMovement(board, destPos) == MOVE_SUCCESS) {
                        if (piece->getColor() == isWhiteTurn) {
                            controlledByUs = true;
                        }
                        else {
                            controlledByOpponent = true;
                        }
                    }
                }
            }

            // Add to control counts
            if (controlledByUs) {
                ourControl++;
            }
            if (controlledByOpponent) {
                opponentControl++;
            }
        }
    }

    // Return the difference (positive is good for us)
    return ourControl - opponentControl;
}

//------------------------------------------------------------------------
/**
* Get the current search depth
*
* @return Current depth value
*/
int MoveRecommender::getDepth() const {
    return m_depth;
}

//------------------------------------------------------------------------
/**
* Set the search depth for move evaluation
*
* @param depth New depth value
*/
void MoveRecommender::setDepth(int depth) {
    m_depth = depth;
}