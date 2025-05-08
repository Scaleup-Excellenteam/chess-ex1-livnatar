#pragma once
#include <exception>


class NoMovesAvailableException : public std::exception {
public:
    const char* what() const noexcept override {
        return "No valid moves available for the current player";
    }
};