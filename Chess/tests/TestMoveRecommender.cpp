/*
#include "move/MoveRecommender.h"
#include "board/ChessBoard.h"
#include "exception/MoveExceptions.h"
#include <iostream>

int main() {
    
    std::string boardStr = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";

    ChessBoard board(boardStr);
    MoveRecommender recommender(board, 2);

    try {
        PriorityQueue<Move> recs = recommender.getRecommendations(true);
        std::cout << recs << std::endl;
    }
    catch (const NoMovesAvailableException& e) {
        std::cerr << "No moves: " << e.what() << std::endl;
    }
}
*/

#include "move/MoveRecommender.h"
#include "board/ChessBoard.h"
#include "exception/MoveExceptions.h"
#include <iostream>
#include <string>

// Function to print the board
void printBoard(const ChessBoard& board) {
    std::cout << "  ";
    for (int col = 1; col < 9; col++) {
        std::cout << col << " ";
    }
    std::cout << "\n";

    for (int row = 0; row < 8; row++) {
        std::cout << static_cast<char>('a' + row) << " ";
        for (int col = 0; col < 8; col++) {
            const ChessPiece* piece = board.getPieceAt(row, col);
            if (piece) {
                std::cout << piece->getPieceType() << " ";
            }
            else {
                std::cout << "- ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

// Helper function to run tests
void runTest(const std::string& testName, const std::string& boardStr, bool isWhiteTurn, int depth) {
    std::cout << "=======================================\n";
    std::cout << "TEST: " << testName << "\n";
    std::cout << "=======================================\n";

    ChessBoard board(boardStr);
    printBoard(board);

    MoveRecommender recommender(board, depth);
    std::cout << "Getting recommendations for " << (isWhiteTurn ? "WHITE" : "BLACK") << " (depth=" << depth << ")\n";

    try {
        PriorityQueue<Move> recs = recommender.getRecommendations(isWhiteTurn);
        std::cout << recs << std::endl;
    }
    catch (const NoMovesAvailableException& e) {
        std::cerr << "No moves: " << e.what() << std::endl;
    }
    std::cout << "\n";
}

int main() {
    // All board strings must be exactly 64 characters long

    // Note: With your board representation:
    // - Uppercase = White pieces
    // - Lowercase = Black pieces 
    // - Board is represented as a 64-character string read from top-left to bottom-right
    // - Rows are labeled a-h from top to bottom
    // - Columns are labeled 0-7 from left to right

    // Test 1: Initial position
    runTest("Initial Position",
        "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr",
        true, 2);

    // Test 2: Mid-game position with potential captures
    runTest("Mid-game Position",
        "R#BQKBNRP#PPPPPP#P####P#########n#######P#####Ppppp#ppp#rnbqkb#r",
        //"R#BQK##R#PPPPPPP########n######################p###pppp#r#b#kb#r",
        true, 2);

    // Test 3: Position with check threat
    runTest("Check Threat Position",
        "R#BQKBNRPPPPPPPP############q###################pppp#ppprnb#kb#r",
        false, 2);

    // Test 4: Testing different depths
    runTest("Depth Testing - Depth 1",
        "R#BQKBNRPPPPPPPP############q###################pppp#ppprnb#kb#r",
        false, 1);

    runTest("Depth Testing - Depth 2",
        "R#BQKBNRPPPPPPPP############q###################pppp#ppprnb#kb#r",
        false, 2);
/*
    runTest("Depth Testing - Depth 3",
        "R#BQKBNRPPPPPPPP############q###################pppp#ppprnb#kb#r",
        false, 3);

    // Test 5: Simple endgame position
    runTest("Simple Endgame Position",
        "####K###############################################R#k#########",
        true, 2);

    // Test 6: Material advantage position
    runTest("Material Advantage Position",
        "R#BQK##R#PPPPPPP########n######################p###pppp#r#b#kb#r",
        true, 2);
        */

    return 0;
}