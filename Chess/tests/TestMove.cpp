#include <iostream>
#include <cassert>
#include <string>
#include <sstream>
#include "move/Move.h"
#include "template/PriorityQueue.h"

// Utility function to check if assertions passed
bool testAssertion(bool condition, const std::string& testName) {
    if (!condition) {
        std::cerr << "FAILED: " << testName << std::endl;
        return false;
    }
    std::cout << "PASSED: " << testName << std::endl;
    return true;
}

int main() {
    bool allTestsPassed = true;

    std::cout << "===== Testing Move Class =====" << std::endl;

    // Test 1: Basic constructor and getters
    {
        std::cout << "\nTest 1: Basic constructor and getters" << std::endl;

        std::pair<int, int> from(1, 2);  // row 1, column 2 (corresponds to 'c2' in notation)
        std::pair<int, int> to(3, 2);    // row 3, column 2 (corresponds to 'c4' in notation)
        int score = 5;
        char pieceType = 'P';

        Move move(from, to, score, pieceType);

        allTestsPassed &= testAssertion(move.getFrom() == from, "getFrom() should return the correct from position");
        allTestsPassed &= testAssertion(move.getTo() == to, "getTo() should return the correct to position");
        allTestsPassed &= testAssertion(move.getScore() == score, "getScore() should return the correct score");
        allTestsPassed &= testAssertion(move.getPieceType() == pieceType, "getPieceType() should return the correct piece type");

        std::cout << "Move created: " << move << std::endl;
    }

    // Test 2: Setters
    {
        std::cout << "\nTest 2: Setters" << std::endl;

        std::pair<int, int> from(0, 0);  // row 0, column 0 (corresponds to 'a1' in notation)
        std::pair<int, int> to(0, 7);    // row 0, column 7 (corresponds to 'h1' in notation)

        Move move(from, to);

        // Initial values with default constructor
        allTestsPassed &= testAssertion(move.getScore() == 0, "Initial score should be 0");
        allTestsPassed &= testAssertion(move.getPieceType() == ' ', "Initial piece type should be space");

        // Update values
        move.setScore(10);
        move.setPieceType('R');

        // Check updated values
        allTestsPassed &= testAssertion(move.getScore() == 10, "setScore() should update the score");
        allTestsPassed &= testAssertion(move.getPieceType() == 'R', "setPieceType() should update the piece type");

        // Test negative score
        move.setScore(-5);
        allTestsPassed &= testAssertion(move.getScore() == -5, "setScore() should handle negative scores");

        std::cout << "Move after updates: " << move << std::endl;
    }

    // Test 3: Notation methods
    {
        std::cout << "\nTest 3: Notation methods" << std::endl;

        // Test notation for various board positions
        struct NotationTestCase {
            std::pair<int, int> from;
            std::pair<int, int> to;
            std::string expectedFromNotation;
            std::string expectedToNotation;
        };

        NotationTestCase testCases[] = {
            {{0, 0}, {7, 7}, "a1", "h8"},  // bottom-left to top-right
            {{7, 0}, {0, 7}, "a8", "h1"},  // top-left to bottom-right
            {{3, 3}, {4, 4}, "d4", "e5"},  // center move
            {{0, 3}, {7, 3}, "d1", "d8"},  // vertical move
            {{3, 0}, {3, 7}, "a4", "h4"},  // horizontal move
            {{6, 4}, {4, 4}, "e7", "e5"}   // pawn move
        };

        for (const auto& testCase : testCases) {
            Move move(testCase.from, testCase.to);

            std::string fromNotation = move.fromNotation();
            std::string toNotation = move.toNotation();
            std::string fullNotation = move.getNotation();

            bool fromCorrect = (fromNotation == testCase.expectedFromNotation);
            bool toCorrect = (toNotation == testCase.expectedToNotation);
            bool fullCorrect = (fullNotation == testCase.expectedFromNotation + testCase.expectedToNotation);

            allTestsPassed &= testAssertion(fromCorrect,
                "fromNotation() for (" + std::to_string(testCase.from.first) + "," +
                std::to_string(testCase.from.second) + ") should be " + testCase.expectedFromNotation);

            allTestsPassed &= testAssertion(toCorrect,
                "toNotation() for (" + std::to_string(testCase.to.first) + "," +
                std::to_string(testCase.to.second) + ") should be " + testCase.expectedToNotation);

            allTestsPassed &= testAssertion(fullCorrect,
                "getNotation() should be " + testCase.expectedFromNotation + testCase.expectedToNotation);

            std::cout << "Move: " << fromNotation << " -> " << toNotation << " (" << fullNotation << ")" << std::endl;
        }
    }

    // Test 4: MyComparator for Move
    {
        std::cout << "\nTest 4: MyComparator for Move" << std::endl;

        // Create moves with different scores
        Move moveA({ 0, 0 }, { 1, 0 }, 5, 'P');
        Move moveB({ 0, 1 }, { 1, 1 }, 10, 'P');
        Move moveC({ 0, 2 }, { 1, 2 }, 3, 'P');
        Move moveD({ 0, 3 }, { 1, 3 }, 5, 'N');  // Same score as moveA but different piece

        MyComparator<Move> comparator;

        // Test comparisons
        allTestsPassed &= testAssertion(comparator(moveA, moveB) < 0,
            "Lower score move should compare less than higher score move");
        allTestsPassed &= testAssertion(comparator(moveB, moveA) > 0,
            "Higher score move should compare greater than lower score move");
        allTestsPassed &= testAssertion(comparator(moveC, moveA) < 0,
            "Lower score move should compare less than higher score move");
        allTestsPassed &= testAssertion(comparator(moveA, moveD) == 0,
            "Equal score moves should compare equal regardless of piece type");

        std::cout << "Move A: " << moveA << std::endl;
        std::cout << "Move B: " << moveB << std::endl;
        std::cout << "Move C: " << moveC << std::endl;
        std::cout << "Move D: " << moveD << std::endl;
        std::cout << "Compare A to B: " << comparator(moveA, moveB) << std::endl;
        std::cout << "Compare B to A: " << comparator(moveB, moveA) << std::endl;
        std::cout << "Compare C to A: " << comparator(moveC, moveA) << std::endl;
        std::cout << "Compare A to D: " << comparator(moveA, moveD) << std::endl;
    }

    // Test 5: Integration with PriorityQueue
    {
        std::cout << "\nTest 5: Integration with PriorityQueue" << std::endl;

        try {
            PriorityQueue<Move> moveQueue(3);

            // Create moves with different scores
            Move move1({ 0, 0 }, { 1, 0 }, 5, 'P');
            Move move2({ 0, 1 }, { 1, 1 }, 10, 'P');
            Move move3({ 0, 2 }, { 1, 2 }, 3, 'P');
            Move move4({ 0, 3 }, { 1, 3 }, 7, 'N');

            // Push moves in random order
            moveQueue.push(move1);  // Score 5
            moveQueue.push(move3);  // Score 3
            moveQueue.push(move2);  // Score 10

            // Check queue ordering
            std::cout << "Move priority queue after first 3 pushes:" << std::endl;
            std::cout << moveQueue << std::endl;

            // Verify highest priority move is at the front
            Move topMove = moveQueue.poll();
            allTestsPassed &= testAssertion(topMove.getScore() == 10,
                "Top move score should be 10");

            // Push another move after polling
            moveQueue.push(move4);  // Score 7

            std::cout << "Move priority queue after poll and push:" << std::endl;
            std::cout << moveQueue << std::endl;

            // Verify new ordering
            topMove = moveQueue.poll();
            allTestsPassed &= testAssertion(topMove.getScore() == 7,
                "New top move score should be 7");

            // Test queue size and emptiness
            topMove = moveQueue.poll();
            allTestsPassed &= testAssertion(moveQueue.size() == 1,
                "Queue size should be 1 after polling twice");
            allTestsPassed &= testAssertion(!moveQueue.isEmpty(),
                "Queue should not be empty");

            // Poll remaining moves
            moveQueue.poll();
            allTestsPassed &= testAssertion(moveQueue.isEmpty(),
                "Queue should be empty after polling all moves");

            // Test QueueEmptyException
            try {
                moveQueue.poll();
                allTestsPassed &= testAssertion(false,
                    "Polling from empty queue should throw exception");
            }
            catch (const QueueEmptyException&) {
                allTestsPassed &= testAssertion(true,
                    "Correctly caught QueueEmptyException");
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Unexpected exception in priority queue test: " << e.what() << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 6: Output stream operator
    {
        std::cout << "\nTest 6: Output stream operator" << std::endl;

        Move move({ 2, 1 }, { 4, 3 }, 15, 'Q');

        std::stringstream ss;
        ss << move;
        std::string output = ss.str();

        std::string expected = "b3 -> d5 (Score: 15, Piece: Q)";
        allTestsPassed &= testAssertion(output == expected,
            "Stream output should match expected format");

        std::cout << "Move toString: " << output << std::endl;
    }

    // Test 7: Edge cases and boundary conditions
    {
        std::cout << "\nTest 7: Edge cases and boundary conditions" << std::endl;

        // Test board boundaries
        Move move1({ 0, 0 }, { 7, 7 }, 0, 'K');  // a1 -> h8
        Move move2({ 7, 7 }, { 0, 0 }, 0, 'K');  // h8 -> a1
        Move move3({ 0, 7 }, { 7, 0 }, 0, 'Q');  // h1 -> a8
        Move move4({ 7, 0 }, { 0, 7 }, 0, 'Q');  // a8 -> h1

        allTestsPassed &= testAssertion(move1.fromNotation() == "a1" && move1.toNotation() == "h8",
            "Boundary move bottom-left to top-right");
        allTestsPassed &= testAssertion(move2.fromNotation() == "h8" && move2.toNotation() == "a1",
            "Boundary move top-right to bottom-left");
        allTestsPassed &= testAssertion(move3.fromNotation() == "h1" && move3.toNotation() == "a8",
            "Boundary move bottom-right to top-left");
        allTestsPassed &= testAssertion(move4.fromNotation() == "a8" && move4.toNotation() == "h1",
            "Boundary move top-left to bottom-right");

        // Test extreme score values
        Move move5({ 3, 3 }, { 4, 4 }, INT_MAX, 'P');
        Move move6({ 3, 3 }, { 4, 4 }, INT_MIN, 'P');

        allTestsPassed &= testAssertion(move5.getScore() == INT_MAX,
            "Move should handle maximum integer score");
        allTestsPassed &= testAssertion(move6.getScore() == INT_MIN,
            "Move should handle minimum integer score");

        std::cout << "Extreme score moves:" << std::endl;
        std::cout << "MAX: " << move5 << std::endl;
        std::cout << "MIN: " << move6 << std::endl;
    }

    // Final results
    if (allTestsPassed) {
        std::cout << "\n[SUCCESS] All Move tests passed!" << std::endl;
        return 0;
    }
    else {
        std::cout << "\n[FAILURE] Some tests failed!" << std::endl;
        return 1;
    }
}

/*
#include <iostream>
#include <cassert>
#include <string>
#include "move/Move.h"
#include "template/PriorityQueue.h"

// Utility function to check if assertions passed
bool testAssertion(bool condition, const std::string& testName) {
    if (!condition) {
        std::cerr << "Test failed: " << testName << std::endl;
        return false;
    }
    return true;
}

int main() {
    bool allTestsPassed = true;

    std::cout << "===== Testing Move Class =====" << std::endl;

    // Test 1: Basic constructor and getters
    {
        std::cout << "\nTest 1: Basic constructor and getters" << std::endl;

        std::pair<int, int> from(1, 2);  // row 1, column 2 (corresponds to 'c2' in notation)
        std::pair<int, int> to(3, 2);    // row 3, column 2 (corresponds to 'c4' in notation)
        int score = 5;
        char pieceType = 'P';

        Move move(from, to, score, pieceType);

        allTestsPassed &= testAssertion(move.getFrom() == from, "getFrom()");
        allTestsPassed &= testAssertion(move.getTo() == to, "getTo()");
        allTestsPassed &= testAssertion(move.getScore() == score, "getScore()");
        allTestsPassed &= testAssertion(move.getPieceType() == pieceType, "getPieceType()");

        std::cout << "Move created: " << move << std::endl;
    }

    // Test 2: Setters
    {
        std::cout << "\nTest 2: Setters" << std::endl;

        std::pair<int, int> from(0, 0);  // row 0, column 0 (corresponds to 'a1' in notation)
        std::pair<int, int> to(0, 7);    // row 0, column 7 (corresponds to 'h1' in notation)

        Move move(from, to);

        // Initial values
        allTestsPassed &= testAssertion(move.getScore() == 0, "Initial score");
        allTestsPassed &= testAssertion(move.getPieceType() == ' ', "Initial piece type");

        // Update values
        move.setScore(10);
        move.setPieceType('R');

        // Check updated values
        allTestsPassed &= testAssertion(move.getScore() == 10, "setScore()");
        allTestsPassed &= testAssertion(move.getPieceType() == 'R', "setPieceType()");

        std::cout << "Move after updates: " << move << std::endl;
    }

    // Test 3: Notation methods
    {
        std::cout << "\nTest 3: Notation methods" << std::endl;

        // Based on your notation where letters are rows and numbers are columns
        std::pair<int, int> from(6, 4);  // row 6, column 4 (corresponds to 'e7' in notation)
        std::pair<int, int> to(4, 4);    // row 4, column 4 (corresponds to 'e5' in notation)

        Move move(from, to, 0, 'P');

        std::string expectedFromNotation = "e7";
        std::string expectedToNotation = "e5";
        std::string expectedFullNotation = "e7e5";

        allTestsPassed &= testAssertion(move.fromNotation() == expectedFromNotation,
            "fromNotation(): " + move.fromNotation() + " != " + expectedFromNotation);
        allTestsPassed &= testAssertion(move.toNotation() == expectedToNotation,
            "toNotation(): " + move.toNotation() + " != " + expectedToNotation);
        allTestsPassed &= testAssertion(move.getNotation() == expectedFullNotation,
            "getNotation(): " + move.getNotation() + " != " + expectedFullNotation);

        std::cout << "From notation: " << move.fromNotation() << std::endl;
        std::cout << "To notation: " << move.toNotation() << std::endl;
        std::cout << "Full notation: " << move.getNotation() << std::endl;
    }

    // Test 4: MyComparator for Move
    {
        std::cout << "\nTest 4: MyComparator for Move" << std::endl;

        Move moveA({ 0, 0 }, { 1, 0 }, 5, 'P');
        Move moveB({ 0, 1 }, { 1, 1 }, 10, 'P');
        Move moveC({ 0, 2 }, { 1, 2 }, 3, 'P');

        MyComparator<Move> comparator;

        allTestsPassed &= testAssertion(comparator(moveA, moveB) < 0,
            "comparator(moveA, moveB) < 0");
        allTestsPassed &= testAssertion(comparator(moveB, moveA) > 0,
            "comparator(moveB, moveA) > 0");
        allTestsPassed &= testAssertion(comparator(moveC, moveA) < 0,
            "comparator(moveC, moveA) < 0");

        std::cout << "Move A: " << moveA << std::endl;
        std::cout << "Move B: " << moveB << std::endl;
        std::cout << "Move C: " << moveC << std::endl;
        std::cout << "Compare A to B: " << comparator(moveA, moveB) << std::endl;
        std::cout << "Compare B to A: " << comparator(moveB, moveA) << std::endl;
        std::cout << "Compare C to A: " << comparator(moveC, moveA) << std::endl;
    }

    // Test 5: Integration with PriorityQueue
    {
        std::cout << "\nTest 5: Integration with PriorityQueue" << std::endl;

        try {
#include "template/PriorityQueue.h"

            PriorityQueue<Move, MyComparator<Move>> moveQueue(3);

            Move move1({ 0, 0 }, { 1, 0 }, 5, 'P');
            Move move2({ 0, 1 }, { 1, 1 }, 10, 'P');
            Move move3({ 0, 2 }, { 1, 2 }, 3, 'P');

            moveQueue.push(move1);
            moveQueue.push(move2);
            moveQueue.push(move3);

            std::cout << "Move priority queue:" << std::endl;
            std::cout << moveQueue << std::endl;

            Move topMove = moveQueue.poll();
            allTestsPassed &= testAssertion(topMove.getScore() == 10,
                "Top move score should be 10, got " + std::to_string(topMove.getScore()));

            std::cout << "Top move: " << topMove << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Exception in priority queue test: " << e.what() << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 6: Edge cases
    {
        std::cout << "\nTest 6: Edge cases" << std::endl;

        // Test with maximum board positions
        std::pair<int, int> from(7, 7);  // row 7, column 7 (corresponds to 'h8' in notation)
        std::pair<int, int> to(0, 0);    // row 0, column 0 (corresponds to 'a1' in notation)

        Move move(from, to, 100, 'Q');

        allTestsPassed &= testAssertion(move.fromNotation() == "h8", "Edge case fromNotation()");
        allTestsPassed &= testAssertion(move.toNotation() == "a1", "Edge case toNotation()");

        std::cout << "Edge case move: " << move << std::endl;
    }

    // Final results
    if (allTestsPassed) {
        std::cout << "\nAll Move tests passed!" << std::endl;
        return 0;
    }
    else {
        std::cout << "\nSome tests failed!" << std::endl;
        return 1;
    }
}
*/