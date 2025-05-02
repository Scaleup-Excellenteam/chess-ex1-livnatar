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
