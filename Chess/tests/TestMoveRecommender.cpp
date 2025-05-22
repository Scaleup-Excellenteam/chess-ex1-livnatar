
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
    // Test 1: Basic capture evaluation
    // White queen can capture black queen (high value)
    runTest("test1",
        "########"
        "########"
        "########"
        "####Q###"
        "###q####"
        "########"
        "########"
        "########",
        true, 2);  // Depth 0: Just evaluate immediate position

    // Test 2: Depth 1 evaluation - Capturing with potential loss
    // White queen can capture black queen, but might be recaptured by pawn
    runTest("test2",
        "########"
        "########"
        "########"
        "####Q###"
        "###q####"
        "##pp####"
        "########"
        "########",
        true, 2);  // Depth 1: Look at opponent's response

    // Test 3: Depth 2 evaluation - Strategic positioning
    // Knight fork potential (can threaten king and queen in subsequent move)
    runTest("test3",
        "######k#"
        "#####q##"
        "########"
        "########"
        "####N###"
        "########"
        "########"
        "######K#",
        true, 2);  // Depth 2: Look at opponent's response and your next move

    // Test 4: Center control bonus test (comparing different moves)
    runTest("test4",
        "########"
        "########"
        "########"
        "########"
        "#N######"
        "########"
        "########"
        "########",
        true, 2);  // Depth 0: Just evaluate immediate position with center bonus

    // Test 5: Test piece safety and board control
    // Both sides have multiple pieces for board control calculation
    runTest("test5",
        "##b#####"
        "#P######"
        "########"
        "###QN###"
        "####r###"
        "#####P##"
        "########"
        "########",
        true, 2);  // Depth 1 evaluation with board control

    // Test 6: Threat evaluation test
    // Black knight can threaten white queen or rook
    runTest("test6",
        "########"
        "########"
        "########"
        "###n####"
        "#####Q##"
        "######R#"
        "########"
        "########",
        false, 2);  // Depth 0 for pure threat evaluation

    // Test 7: Defending a threatened piece
    // White pawn is threatened by black bishop, white can defend or move it
    runTest("test7",
        "########"
        "########"
        "###b####"
        "########"
        "#####P##"
        "########"
        "######P#"
        "#######R",
        true, 2);  // Depth 1 to see potential defenses

    // Test 8: Piece trading evaluation
    // Should the knight capture the bishop and risk being recaptured by pawn?
    runTest("test8",
        "########"
        "########"
        "####b###"
        "###N####"
        "#####p##"
        "########"
        "########"
        "########",
        true, 2);  // Depth 2 to evaluate the full exchange

    // Test 9: Opening moves evaluation
    // Standard opening position, check if center pawns are properly valued
    runTest("test9",
        "RNBQKBNR"
        "PPPPPPPP"
        "########"
        "########"
        "########"
        "########"
        "pppppppp"
        "rnbqkbnr",
        true, 2);  // Depth 1 for opening move evaluation

    runTest("test10",
        "RNBQKBNR"
        "PPP#PPPP"
        "########"
        "###P####"
        "########"
        "########"
        "pppppppp"
        "rnbqkbnr",
        false, 2);  

    // Test 10: Multiple capture options with different values
    // White queen can capture different black pieces - should prefer highest value
    runTest("test11",
        "########"
        "########"
        "###r####"
        "##n#####"
        "####Q###"
        "#####p##"
        "########"
        "########",
        true, 2);  // Depth 1 to see value and safety of captures

    runTest("test12",
        "####K###"
        "######R#"
        "########"
        "########"
        "###Q##bk"
        "########"
        "####r###"
        "########",
        true, 2);
    return 0;
}

/*
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
    // - Columns are labeled 1-8 from left to right

// 1. Prefer capturing higher-value piece (queen vs pawn)
// Black queen can capture white pawn or move elsewhere. Should prefer capture.
    runTest("Prefer Capturing Higher-Value Piece",
        "########"
        "########"
        "########"
        "####Q###"
        "###q#p##"
        "########"
        "########"
        "########",
        true, 2);

    // 2. Penalize moving into danger (knight can capture pawn but is then captured by bishop)
    // White knight can capture black pawn, but black bishop will recapture. Should avoid if possible.
    runTest("Penalize Moving Into Danger",
        "########"
        "########"
        "###B####"
        "########"
        "#####P##"
        "########"
        "######n#"
        "########",
        false, 2);

    // 3. Bonus for threatening a higher-value piece (knight moves to threaten rook)
    // White knight can move to threaten black rook. Should get a threat bonus.
    runTest("Bonus for Threatening Higher-Value Piece",
        "########"
        "########"
        "########"
        "###n####"
        "########"
        "#####R##"
        "########"
        "########",
        false, 0);

    runTest("Checkmate Bonus",
        "#######k"
        "######Q#"
        "########"
        "########"
        "########"
        "########"
        "########"
        "#######K",
        true, 1);

    runTest("Pawn",
        "########"
        "########"
        "###P####"
        "########"
        "####p###"
        "########"
        "########"
        "#######K",
        true, 0);
  
    runTest("Pawn",
        "RNBQKBNR"
        "PPP#PPPP"
        "########"
        "###P####"
        "########"
        "########"
        "pppppppp"
        "rnbqkbnr",
        false, 0);

    runTest("Pawn",
        "RNBQKBNR"
        "PPP#PPPP"
        "########"
        "###P####"
        "########"
        "########"
        "pppppppp"
        "rnbqkbnr",
        false, 1);

   
    return 0;
}
*/