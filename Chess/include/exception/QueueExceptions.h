
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