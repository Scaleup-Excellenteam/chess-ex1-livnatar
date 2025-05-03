
#include "move/MoveRecommender.h"
#include <algorithm>
#include <iostream>
#include <climits> // For INT_MIN and INT_MAX

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

    try{
        // Clear previous recommendations
        m_recommendations = PriorityQueue<Move>(5);

        // Generate all possible moves
        std::vector<Move> allMoves = generateAllMoves(isWhiteTurn, m_board);

        
        // If no moves are available, throw an exception
        if (allMoves.empty()) {
            throw NoMovesAvailableException();
        }
        
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
    catch (const std::exception& e) {
        std::cerr << "Error in getRecommendations: " << e.what() << std::endl;
        return PriorityQueue<Move>(5); // Return empty queue on error
    }
}
//------------------------------------------------------------------------
/**
* Generate all valid moves for a player
*
* @param isWhiteTurn True if generating moves for white player
* @param board The chess board to evaluate
* @return Vector of all valid moves for the player
*/
std::vector<Move> MoveRecommender::generateAllMoves(bool isWhiteTurn, const ChessBoard& board) const {

    std::vector<Move> allMoves;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
           
            const ChessPiece* piece = board.getPieceAt(row, col);

            if (piece && piece->getColor() == isWhiteTurn) {
               
                std::pair<int, int> pos = { row, col };

                // Get all valid moves for this piece
                //std::vector<Move> pieceMoves = generateMovesForPiece(pos, isWhiteTurn,board);

                // Use the new optimized method to get valid moves directly from the piece
                std::vector<Move> pieceMoves = piece->generateValidMoves(board);

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
* @param board The chess board to evaluate
* @return Vector of all valid moves for the piece
*/
std::vector<Move> MoveRecommender::generateMovesForPiece(const std::pair<int, int>& pos, bool isWhiteTurn, const ChessBoard& board) const {

    std::vector<Move> moves;
    const ChessPiece* piece = board.getPieceAt(pos.first, pos.second);

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
            int moveStatus = board.checkMovement(pos, destPos, isWhiteTurn);

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

    // Calculate immediate score for position after a move
    int score = evaluatePosition(move, isWhiteTurn, boardCopy);

    // Base case: if we've reached maximum depth, return the score
    if (depth <= 0) {
        return score;
    }

    // Look at opponent's responses
    bool opponentTurn = !isWhiteTurn;
    std::vector<Move> opponentMoves = generateAllMoves(opponentTurn, boardCopy);

    // If opponent has no moves, this is good for us
    if (opponentMoves.empty()) {
        return score + 50; // Bonus for limiting opponent options
    }

    // Find opponent's best move (from their perspective)
    int bestOpponentScore = INT_MIN;
    
    for (const auto& opponentMove : opponentMoves) {
        
        // Create another board copy for opponent's move
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
int MoveRecommender::evaluatePosition(const Move& move, bool isWhiteTurn, ChessBoard& boardCopy) const {

    int score = 0;
    const std::pair<int, int>& from = move.getFrom();
    const std::pair<int, int>& to = move.getTo();

    // Capture target before move
    const ChessPiece* capturedPiece = boardCopy.getPieceAt(to.first, to.second);

    // Make the move on our board copy
    boardCopy.movePiece(from, to);

    const ChessPiece* movedPiece = boardCopy.getPieceAt(to.first, to.second);

    if (!movedPiece) {
        return -1000; // Something went wrong with the simulation
    }

    // 1. Capture value
    if (capturedPiece && capturedPiece->getColor() != isWhiteTurn) {
        score += getPieceValue(capturedPiece->getPieceType());
    }

    // 2. Check if our piece is in danger after the move
    if (isPieceInDanger(to, isWhiteTurn, boardCopy)) {
        int movedPieceValue = getPieceValue(movedPiece->getPieceType());
        score -= movedPieceValue; // Penalty for putting piece in danger
    }

    // 3. Bonus for threatening opponent pieces after the move
    score += evaluateThreats(to, isWhiteTurn, boardCopy);

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

    const ChessPiece* targetPiece = board.getPieceAt(pos.first, pos.second);
    if (!targetPiece) {
        return false;
    }

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
                /*
                // Check if this opponent piece can attack our piece
                if (board.checkMovement(opponentPos, pos, !isWhitePiece) == MOVE_SUCCESS) {
                    // Per assignment requirements - any threat is considered danger
                    return true;
                }
                */
            }
        }
    }
    return false;
}
//------------------------------------------------------------------------
/**
* Evaluate threats made by the piece that just moved
*
* @param to Destination position of the moved piece
* @param isWhiteTurn True if it's white's turn
* @param boardCopy Reference to the board after the move
* @return Score bonus for threats made
*/
int MoveRecommender::evaluateThreats(const std::pair<int, int>& to, bool isWhiteTurn, const ChessBoard& boardCopy) const {
    
    int threatBonus = 0;
    const ChessPiece* movedPiece = boardCopy.getPieceAt(to.first, to.second);

    if (!movedPiece) {
        return 0;
    }

    int movedPieceValue = getPieceValue(movedPiece->getPieceType());

    // Check all board positions for opponent pieces we might threaten
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {

            if (row == to.first && col == to.second) continue;  // Skip our own position

            const ChessPiece* targetPiece = boardCopy.getPieceAt(row, col);
            if (targetPiece && targetPiece->getColor() != isWhiteTurn) {

                std::pair<int, int> targetPos = { row, col };

                // Check if our moved piece can attack this opponent piece
                if (boardCopy.checkMovement(to, targetPos, isWhiteTurn) == MOVE_SUCCESS) {

                    int targetValue = getPieceValue(targetPiece->getPieceType());

                    if (targetValue > movedPieceValue) {
                        // Threatening a stronger piece
                        threatBonus += 3;
                    }
                    else if (targetValue == movedPieceValue) {
                        // Threatening an equal piece
                        threatBonus += 2;
                    }
                    else {
                        // Threatening a weaker piece
                        threatBonus += 1;
                    }
                }
            }
        }
    }

    return threatBonus;
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
            for (int row = 0; row < 8 && !(controlledByUs && controlledByOpponent); ++row) {
                for (int col = 0; col < 8 && !(controlledByUs && controlledByOpponent); ++col) {
                    
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

            // Add to control counts - each square counts once per side
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