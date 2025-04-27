
#pragma once
#include <exception>

// Custom exceptions that inherit from std::exception
class QueueEmptyException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Attempted to poll from an empty priority queue";
    }
};

class QueueFullException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Attempted to push into a full priority queue";
    }
};