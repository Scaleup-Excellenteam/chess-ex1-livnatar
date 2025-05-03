

/*
#include "move/MoveRecommender.h"
#include "board/ChessBoard.h"
#include "exception/MoveExceptions.h"
#include "exception/QueueExceptions.h"
#include <iostream>


void testBasicRecommendations() {
    std::cout << "==== Testing Basic Recommendations ====" << std::endl;

    // Starting position
    std::string boardStr = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
    ChessBoard board(boardStr);
    MoveRecommender recommender(board, 2);

    try {
        // Get recommendations for white
        std::cout << "White's recommendations:" << std::endl;
        PriorityQueue<Move> whiteRecs = recommender.getRecommendations(true);
        std::cout << whiteRecs << std::endl;

        // Get recommendations for black
        std::cout << "Black's recommendations:" << std::endl;
        PriorityQueue<Move> blackRecs = recommender.getRecommendations(false);
        std::cout << blackRecs << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void testDifferentDepths() {
    std::cout << "==== Testing Different Depths ====" << std::endl;

    // Mid-game position with pieces in action
    std::string boardStr = "R#B#K##R#PPP#PPP###N#Q##########P#####p####n###ppp#ppp#r#b#k##r";
    ChessBoard board(boardStr);

    for (int depth = 1; depth <= 3; depth++) {
        std::cout << "Depth = " << depth << ":" << std::endl;
        MoveRecommender recommender(board, depth);

        try {
            PriorityQueue<Move> recs = recommender.getRecommendations(true);
            std::cout << recs << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

void testCaptureScenario() {
    std::cout << "==== Testing Capture Scenario ====" << std::endl;

    // Position with immediate capture available
    std::string boardStr = "R#BQKBNR#P#PPPPP#######P########p#####P########pppppp#r#bqkbnr";
    ChessBoard board(boardStr);
    MoveRecommender recommender(board, 2);

    try {
        PriorityQueue<Move> recs = recommender.getRecommendations(true);
        std::cout << recs << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void testExceptionHandling() {
    std::cout << "==== Testing Exception Handling ====" << std::endl;

    // Stalemate position (no valid moves)
    std::string boardStr = "#######K###############k##############Q########";
    ChessBoard board(boardStr);
    MoveRecommender recommender(board, 2);

    try {
        PriorityQueue<Move> recs = recommender.getRecommendations(true);
        std::cout << recs << std::endl;
    }
    catch (const NoMovesAvailableException& e) {
        std::cerr << "Expected exception caught: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected exception: " << e.what() << std::endl;
    }
}

int main() {
    testBasicRecommendations();
    testDifferentDepths();
    testCaptureScenario();
    testExceptionHandling();

    return 0;
}
*/


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
