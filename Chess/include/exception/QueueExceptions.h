
#pragma once
#include <exception>

// Custom exceptions that inherit from std::exception

//------------------------------------------------------------------------
/**
 * Exception thrown when attempting to poll from an empty priority queue.
 */

class QueueEmptyException : public std::exception {
public:
    /**
     * Returns a C-style character string describing the general cause of the current error.
     * @return Error message.
     */
    const char* what() const noexcept override {
        return "Attempted to poll from an empty priority queue";
    }
};

//------------------------------------------------------------------------
/**
 * Exception thrown when attempting to push into a full priority queue.
 */
class QueueFullException : public std::exception {
public:
    /**
     * Returns a C-style character string describing the general cause of the current error.
     * @return Error message.
     */
    const char* what() const noexcept override {
        return "Attempted to push into a full priority queue";
    }
};



/*

 //Exception thrown when attempting to promote a pawn.
 //This is a special exception that can be used to handle pawn promotion interactively.

class PawnPromotionException : public std::exception {
private:
    std::string m_message;
    std::pair<int, int> m_position;

public:
    
     // Constructor for PawnPromotionException.
     // @param row Row position of the pawn to be promoted
     // @param col Column position of the pawn to be promoted
     
    PawnPromotionException(int row, int col)
        : m_position(row, col) {
        m_message = "Pawn at position (" + std::to_string(row) + ", " +
            std::to_string(col) + ") can be promoted. Choose a piece type (Q, R, B, N):";
    }

    
     // Returns a message describing the exception.
     // @return Description of the exception
     
    const char* what() const noexcept override {
        return m_message.c_str();
    }

    
     // Gets the position of the pawn to be promoted.
     // @return Pair containing row and column of the pawn
     
    std::pair<int, int> getPosition() const {
        return m_position;
    }
};

*/