
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
* Generates and ranks the best 5 moves using `evaluateMove`.
* Throws `NoMovesAvailableException` if no moves are possible.
*
* @param isWhiteTurn True if it's white's turn
* @return A priority queue of the best 5 move recommendations
* @throws NoMovesAvailableException If no moves are available
*/
PriorityQueue<Move> MoveRecommender::getRecommendations(bool isWhiteTurn) {

    try {
        // Clear previous recommendations 
        m_recommendations = PriorityQueue<Move>(5);

        // Generate all possible moves
        std::vector<Move> allMoves = generateAllMoves(isWhiteTurn, m_board);

        // If no moves are available, throw an exception
        if (allMoves.empty()) {
            throw NoMovesAvailableException();
        }

        // Use a single board for all evaluations - Only create one copy
        ChessBoard boardCopy(m_board);

        // For each move, evaluate on our copy and restore the board after
        for (auto& move : allMoves) {

            // Past: int score = evaluateMove(move, isWhiteTurn, m_depth, boardCopy,true);  

            int score = evaluateMove(move, isWhiteTurn, m_depth, boardCopy, isWhiteTurn);

            move.setScore(score);

            try {
                m_recommendations.push(move);
            }
            catch (const QueueFullException& e) {
                // Queue is full and this move isn't good enough - Just ignore it             
            }
            catch (const std::exception& e) {
                std::cerr << "Error in getRecommendations: " << e.what() << std::endl;
            }
        }

        return m_recommendations;
    }
    catch (const NoMovesAvailableException& ex) {
        throw;  // Re-throw to be handled by caller
    }
    catch (const std::exception& ex) {
        std::cerr << "Error in getRecommendations: " << ex.what() << std::endl;
        return PriorityQueue<Move>(5);  // Return empty queue on error
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
* Recursively evaluates a move using minimax, simulating future moves up to a given depth.
*
* @param move Move to evaluate
* @param isWhiteTurn True if white's turn
* @param depth Remaining search depth
* @param board Board state (modified during evaluation)
* @param isMaximizingForWhite True if maximizing for white
* @return Evaluation score from the maximizing player's perspective
*/

int MoveRecommender::evaluateMove(const Move& move, bool isWhiteTurn, int depth,
    ChessBoard& board, bool isMaximizingForWhite) {

    // Make the move and save data for undoing
    MoveData moveData = makeMoveAndGetData(move, board);

    // Get the moved piece after the move
    const ChessPiece* movedPiece = board.getPieceAt(moveData.to.first, moveData.to.second);
    if (!movedPiece) {
        undoMove(moveData, board);
        return MOVE_ERROR;
    }

    // Calculate immediate score for this position
    int positionScore = evaluatePosition(move, isWhiteTurn, board, moveData.capturedPieceType, moveData.capturedPieceColor);

    // Base case: if depth is 0, return immediate position score from correct perspective
    if (depth <= 0) {
        undoMove(moveData, board);

        // If the current player is the one we're maximizing for, return positive score
        // If the current player is the opponent, return negative score
        return (isWhiteTurn == isMaximizingForWhite) ? positionScore : -positionScore;
    }

    // Generate next player's possible moves
    bool nextPlayerTurn = !isWhiteTurn;
    std::vector<Move> nextPlayerMoves = generateAllMoves(nextPlayerTurn, board);

    // Handle no moves case (checkmate/stalemate)
    if (nextPlayerMoves.empty()) {
        undoMove(moveData, board);

        // Adjust position score based on perspective
        int adjustedPositionScore = (isWhiteTurn == isMaximizingForWhite) ? positionScore : -positionScore;

        // Add game ending bonus - if we caused the game to end, it's good for us
        int gameEndBonus = (isWhiteTurn == isMaximizingForWhite) ? 500 : -500;
        return adjustedPositionScore + gameEndBonus;
    }

    // Determine if next level should maximize or minimize
    bool nextLevelMaximizing = (nextPlayerTurn == isMaximizingForWhite);

    int bestScore;

    if (nextLevelMaximizing) {
        // Next player is the one we're optimizing for - they want to maximize
        bestScore = INT_MIN;
        for (const auto& nextMove : nextPlayerMoves) {
            int nextMoveScore = evaluateMove(nextMove, nextPlayerTurn, depth - 1, board, isMaximizingForWhite);
            bestScore = std::max(bestScore, nextMoveScore);
        }
    }
    else {
        // Next player is the opponent - they want to minimize our score
        bestScore = INT_MAX;
        for (const auto& nextMove : nextPlayerMoves) {
            int nextMoveScore = evaluateMove(nextMove, nextPlayerTurn, depth - 1, board, isMaximizingForWhite);
            bestScore = std::min(bestScore, nextMoveScore);
        }
    }

    undoMove(moveData, board);

    // Combine immediate score with future best score
    int adjustedPositionScore = (isWhiteTurn == isMaximizingForWhite) ? positionScore : -positionScore;

    return adjustedPositionScore + bestScore;
}


//------------------------------------------------------------------------
/**
* Evaluates the position after a move based on factors like captured piece value, piece danger,
* threat to opponent pieces, center control, and overall board control.
*
* @param move The move made
* @param isWhiteTurn True if it's white's turn
* @param board Current board state
* @param capturedPieceType Type of captured piece (if any)
* @param capturedPieceColor Color of captured piece (if any)
* @return The position score
*/
int MoveRecommender::evaluatePosition(const Move& move, bool isWhiteTurn, ChessBoard& board,
    char capturedPieceType, bool capturedPieceColor) const {

    int score = 0;
    const std::pair<int, int>& to = move.getTo();

    const ChessPiece* movedPiece = board.getPieceAt(to.first, to.second);

    if (!movedPiece) {
        return MOVE_ERROR;// Something went wrong with the simulation
    }

    // 1. Capture value
    if ((capturedPieceType != '#') && (capturedPieceColor != isWhiteTurn)) {
        score += getPieceValue(capturedPieceType);
    }

    // 2. Check if our piece is in danger after the move
    if (isPieceInDanger(to, isWhiteTurn, board)) {
        int movedPieceValue = getPieceValue(movedPiece->getPieceType());
        //score -= movedPieceValue; // Penalty for putting piece in danger 
        score -= movedPieceValue;// / 2;
    }

    // 3. Bonus for threatening opponent pieces after the move
    score += evaluateThreats(to, isWhiteTurn, board);

    // 4. Center control bonus
    score += centerControlBonus(to);

    // 5. Board control difference
    score += calculateBoardControl(isWhiteTurn, board);

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
    case 'P': return PAWN_VALUE; 
    case 'N': return KNIGHT_VALUE; 
    case 'B': return BISHOP_VALUE; 
    case 'R': return ROOK_VALUE; 
    case 'Q': return QUEEN_VALUE; 
    case 'K': return KING_VALUE; 
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
                    
                    /*
                    // We're especially concerned when a lower-value piece threatens our higher-value piece
                    int attackerValue = getPieceValue(piece->getPieceType());

                    if (attackerValue < targetValue) {
                        return true;
                    }
                    */

                    // Check if this opponent piece can attack our piece
                    return true;
                    
                }
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
                //threatBonus += targetValue;
                threatBonus += targetValue / 2;  // About 0.5% of piece value
               
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
       // return 3; // Higher bonus for direct center control
        return 2; 
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
    //return ourControl - opponentControl;
    return (ourControl - opponentControl) / 4;
}
//------------------------------------------------------------------------
/**
* Executes a move and returns data about the move, including captured piece info.
*
* @param move The move to be made
* @param board The current board state
* @return MoveData containing information about the move
*/
MoveData MoveRecommender::makeMoveAndGetData(const Move& move, ChessBoard& board) {

    MoveData data;
    data.from = move.getFrom();
    data.to = move.getTo();
    data.capturedPieceType = '#';  // Default: no piece captured

    // Store information about the piece that will be captured (if any)
    const ChessPiece* targetPiece = board.getPieceAt(data.to.first, data.to.second);
    if (targetPiece) {
        data.capturedPieceType = targetPiece->getPieceType();
        data.capturedPieceColor = targetPiece->getColor();
    }

    //TODO: Store first-move status for special move handling

    // Move the piece on the board
    board.movePiece(data.from, data.to);

    return data;
}

//------------------------------------------------------------------------
/**
* Reverts a move by undoing the changes made to the board.
*
* @param data Data related to the move to undo
* @param board The board to revert
*/
void MoveRecommender::undoMove(const MoveData& data, ChessBoard& board) {

    // First move the piece back to its original position
    board.movePiece(data.to, data.from);

    // If a piece was captured, restore it
    if (data.capturedPieceType != '#') {
        board.setupPieceAt(data.capturedPieceType, data.to);
    }

    //TODO: Reset first-move flag if needed
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



//------------------------------------------------------------------------



/**
* Calculate score for a potential move with recursive minimax algorithm
*
* @param move The move to evaluate
* @param isWhiteTurn True if it's white's turn (color of player making this move)
* @param depth Search depth (remaining moves to evaluate)
* @param board The current board state
* @param isMaximizingPlayer True if evaluating from maximizing player's perspective (our side)
* @return The evaluation score for the move
*/

/*
int MoveRecommender::evaluateMove(const Move& move, bool isWhiteTurn, int depth, ChessBoard& board, bool isMaximizingPlayer) {

    // Make the move and save data for undoing
    MoveData moveData = makeMoveAndGetData(move, board);

    // Get the moved piece after the move
    const ChessPiece* movedPiece = board.getPieceAt(moveData.to.first, moveData.to.second);
    if (!movedPiece) {
        // Undo the move if something went wrong
        undoMove(moveData, board);
        return MOVE_ERROR;
    }

    // Calculate immediate score for this position
    int positionScore = evaluatePosition(move, isWhiteTurn, board, moveData.capturedPieceType, moveData.capturedPieceColor);

    // Base case: if depth is 0, return immediate position score
    if (depth <= 0) {
        undoMove(moveData, board);
        return isMaximizingPlayer ? positionScore : -positionScore; // Negate if opponent's perspective
    }

    // Generate next player's possible moves
    bool nextPlayerTurn = !isWhiteTurn;
    std::vector<Move> nextPlayerMoves = generateAllMoves(nextPlayerTurn, board);

    // Handle no moves case
    if (nextPlayerMoves.empty()) {
        undoMove(moveData, board);
        return isMaximizingPlayer ? positionScore + 50 : -positionScore - 50;
    }

    int bestScore;

    if (isMaximizingPlayer) {
        // Our turn, we want to maximize score
        bestScore = INT_MIN;

        for (const auto& nextMove : nextPlayerMoves) {
            int nextMoveScore = evaluateMove(nextMove, nextPlayerTurn, depth - 1, board, false);
            bestScore = std::max(bestScore, nextMoveScore);
        }
    }
    else {
        // Opponent's turn, they want to minimize our score
        bestScore = INT_MAX;

        for (const auto& nextMove : nextPlayerMoves) {
            int nextMoveScore = evaluateMove(nextMove, nextPlayerTurn, depth - 1, board, true);
            bestScore = std::min(bestScore, nextMoveScore);
        }
    }

    // Undo the move before returning
    undoMove(moveData, board);

    // Return position score plus best continuation
    return isMaximizingPlayer ? positionScore + bestScore : -positionScore + bestScore;
}

*/