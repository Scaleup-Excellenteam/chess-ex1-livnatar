
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

    runTest("test1",
        "#B#####R"
        "########"
        "########"
        "####Q###"
        "###q####"
        "########"
        "#####p##"
        "########",
        true, 2);  

    runTest("test2",
        "########"
        "########"
        "########"
        "####Q###"
        "###q####"
        "##pp####"
        "########"
        "########",
        true, 2); 

    runTest("test3",
        "######k#"
        "#####q##"
        "########"
        "########"
        "####N###"
        "########"
        "########"
        "######K#",
        true, 2); 

    runTest("test4",
        "######K#"
        "#####Q##"
        "B#######"
        "########"
        "####r###"
        "########"
        "##n#####"
        "######k#",
        true, 2);

    runTest("test5",
        "##b#####"
        "#P######"
        "########"
        "###QN###"
        "####r###"
        "#####P##"
        "########"
        "########",
        true, 2);  

    runTest("test6",
        "########"
        "########"
        "########"
        "###n####"
        "#####Q##"
        "######R#"
        "########"
        "########",
        false, 2);  

    runTest("test7",
        "########"
        "########"
        "###b####"
        "########"
        "#####P##"
        "########"
        "######P#"
        "#######R",
        true, 2);  

    runTest("test8",
        "########"
        "########"
        "####b###"
        "###N####"
        "#####p##"
        "########"
        "########"
        "########",
        true, 2); 

    runTest("test9",
        "RNBQKBNR"
        "PPPPPPPP"
        "########"
        "########"
        "########"
        "########"
        "pppppppp"
        "rnbqkbnr",
        true, 2);  

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

    runTest("test11",
        "########"
        "########"
        "###r####"
        "##n#####"
        "####Q###"
        "#####p##"
        "########"
        "########",
        true, 2); 

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
   
    runTest("test13",
        "######K#"
        "#####Q##"
        "B#######"
        "########"
        "####r###"
        "########"
        "##n#####"
        "######k#",
        false, 2);

    runTest("test14",
        "##b#####"
        "#P######"
        "########"
        "###QN###"
        "####r###"
        "#####P##"
        "########"
        "########",
        false, 2);

    return 0;
}