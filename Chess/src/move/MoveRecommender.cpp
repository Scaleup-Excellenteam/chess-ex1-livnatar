
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
           // int score = evaluateMove(move, isWhiteTurn, m_depth, boardCopy, INT_MIN, INT_MAX);
            move.setScore(score);

            try {
                m_recommendations.push(move);
            }
            catch (const QueueFullException& e) {
                // Queue is full and this move isn't good enough
                // Just ignore it
            }
            catch (const std::exception& e) {
                std::cerr << "Error in getRecommendations: " << e.what() << std::endl;
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

                // Get all valid moves directly from the piece
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
* Calculate score for a potential move with recursive depth
*
* @param move The move to evaluate
* @param isWhiteTurn True if it's white's turn
* @param depth How many turns to look ahead
* @param boardCopy Copy of the chess board for simulation
* @return Score indicating the move's quality
*/

int MoveRecommender::evaluateMove(const Move& move, bool isWhiteTurn, int depth, ChessBoard& boardCopy) {


    // Store move coordinates
    const std::pair<int, int>& from = move.getFrom();
    const std::pair<int, int>& to = move.getTo();

    // Check what piece (if any) is being captured
    const ChessPiece* capturedPiece = boardCopy.getPieceAt(to.first, to.second);

    // Apply the move on our simulation board
    boardCopy.movePiece(from, to);

    // Get immediate evaluation of this position
    int score = evaluatePosition(move, isWhiteTurn, boardCopy, capturedPiece);

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
* Calculate score for a potential move with recursive depth using Alpha-Beta pruning
*
* @param move The move to evaluate
* @param isWhiteTurn True if it's white's turn (maximizing player)
* @param depth How many turns to look ahead
* @param boardCopy Copy of the chess board for simulation
* @param alpha Alpha value for pruning
* @param beta Beta value for pruning
* @return Score indicating the move's quality
*/
/*
int MoveRecommender::evaluateMove(const Move& move, bool isWhiteTurn, int depth, ChessBoard& boardCopy, int alpha, int beta) {
    // Store move coordinates
    const std::pair<int, int>& from = move.getFrom();
    const std::pair<int, int>& to = move.getTo();

    // Check what piece (if any) is being captured
    const ChessPiece* capturedPiece = boardCopy.getPieceAt(to.first, to.second);

    // Apply the move on our simulation board
    boardCopy.movePiece(from, to);

    // Get immediate evaluation of this position
    int score = evaluatePosition(move, isWhiteTurn, boardCopy, capturedPiece);

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

    // For opponent's turn, we're looking for their best move (minimizing our score)
    if (opponentTurn) {
        int minScore = INT_MAX;

        for (const auto& opponentMove : opponentMoves) {
            // Create another board copy for opponent's move
            ChessBoard deeperCopy(boardCopy);

            // Evaluate opponent's move recursively
            int currentScore = evaluateMove(opponentMove, opponentTurn, depth - 1, deeperCopy, alpha, beta);

            // Update min score
            minScore = std::min(minScore, currentScore);

            // Update beta for pruning
            beta = std::min(beta, minScore);

            // Alpha-beta pruning
            if (alpha >= beta) {
                break; // Prune the rest of this branch
            }
        }

        return minScore;
    }
    // For our turn, we're looking for our best move (maximizing our score)
    else {
        int maxScore = INT_MIN;

        for (const auto& ourMove : opponentMoves) {
            // Create another board copy for our move
            ChessBoard deeperCopy(boardCopy);

            // Evaluate our move recursively
            int currentScore = evaluateMove(ourMove, opponentTurn, depth - 1, deeperCopy, alpha, beta);

            // Update max score
            maxScore = std::max(maxScore, currentScore);

            // Update alpha for pruning
            alpha = std::max(alpha, maxScore);

            // Alpha-beta pruning
            if (alpha >= beta) {
                break; // Prune the rest of this branch
            }
        }

        return maxScore;
    }
}*/
//------------------------------------------------------------------------
/**
* Calculate score for a potential move with recursive depth using Alpha-Beta pruning
*
* @param move The move to evaluate
* @param isWhiteTurn True if it's white's turn (maximizing player)
* @param depth How many turns to look ahead
* @param boardCopy Copy of the chess board for simulation
* @param alpha Alpha value for pruning
* @param beta Beta value for pruning
* @return Score indicating the move's quality
*/
/*
int MoveRecommender::evaluateMove(const Move& move, bool isWhiteTurn, int depth,
                                  ChessBoard& boardCopy, int alpha, int beta) {

    // Store move coordinates
    const std::pair<int, int>& from = move.getFrom();
    const std::pair<int, int>& to = move.getTo();

    // Check what piece (if any) is being captured
    const ChessPiece* capturedPiece = boardCopy.getPieceAt(to.first, to.second);

    // Apply the move on our simulation board
    boardCopy.movePiece(from, to);

    // Get immediate evaluation of this position
    int score = evaluatePosition(move, isWhiteTurn, boardCopy, capturedPiece);

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
        int opponentScore = evaluateMove(opponentMove, opponentTurn, depth - 1, deeperCopy, alpha, beta);

        // Update best score (from opponent's perspective)
        bestOpponentScore = std::max(bestOpponentScore, opponentScore);

        // Beta cutoff
        if (bestOpponentScore >= beta) {
            break;  // No need to check other moves
        }

        // Update alpha
        alpha = std::max(alpha, bestOpponentScore);
    }

    // From our perspective, opponent's gain is our loss
    score -= bestOpponentScore;

    return score;
}
*/
//------------------------------------------------------------------------
/**
* Calculate the immediate score for a position after a move
*
* @param move The move to evaluate
* @param isWhiteTurn True if it's white's turn
* @param boardCopy Reference to the board after the move
* @param capturedPiece The piece that was captured, if any
* @return Score for the position after the move
*/
int MoveRecommender::evaluatePosition(const Move& move, bool isWhiteTurn,
                                      ChessBoard& boardCopy, const ChessPiece* capturedPiece) const {

    int score = 0;
    const std::pair<int, int>& to = move.getTo();

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
    int targetValue = getPieceValue(targetPiece->getPieceType());


    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {

            const ChessPiece* piece = board.getPieceAt(row, col);
            if (piece && piece->getColor() != isWhitePiece) {
               
                std::pair<int, int> opponentPos = { row, col };

                // Check if this opponent piece can attack our piece
                if (piece->checkMovement(board, pos) == MOVE_SUCCESS) {
                    
                    int attackerValue = getPieceValue(piece->getPieceType());

                    // We're especially concerned when a lower-value piece threatens our higher-value piece
                    if (attackerValue < targetValue) {
                        return true;
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

    // Get all valid moves for the moved piece
    std::vector<Move> validMoves = movedPiece->generateValidMoves(boardCopy);

    // Check each valid move to see if it threatens an opponent piece
    for (const auto& move : validMoves) {
        const std::pair<int, int>& targetPos = move.getTo();
        const ChessPiece* targetPiece = boardCopy.getPieceAt(targetPos.first, targetPos.second);

        if (targetPiece && targetPiece->getColor() != isWhiteTurn) {
            int targetValue = getPieceValue(targetPiece->getPieceType());

            if (targetValue > movedPieceValue) {
                // Threatening a stronger piece
                threatBonus += 5;
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

    // Initialize control maps (true = controlled by player, false = not controlled)
    bool ourControlMap[8][8] = { {false} };
    bool opponentControlMap[8][8] = { {false} };

    // For each piece, calculate all squares it controls
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            
            const ChessPiece* piece = board.getPieceAt(row, col);
            if (!piece) continue;  // Empty square

            // Get all valid moves for this piece
            std::vector<Move> validMoves = piece->generateValidMoves(board);

            // Mark all squares this piece controls
            for (const auto& move : validMoves) {
                const std::pair<int, int>& controlPos = move.getTo();

                // Mark as controlled by the appropriate side
                if (piece->getColor() == isWhiteTurn) {
                    ourControlMap[controlPos.first][controlPos.second] = true;
                }
                else {
                    opponentControlMap[controlPos.first][controlPos.second] = true;
                }
            }
        }
    }

    // Count controlled squares
    int ourControl = 0;
    int opponentControl = 0;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (ourControlMap[row][col]) ourControl++;
            if (opponentControlMap[row][col]) opponentControl++;
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
