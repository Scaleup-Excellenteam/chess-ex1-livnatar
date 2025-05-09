#pragma once
#include <stdexcept>
#include <string>

class InvalidTurnCountException : public std::exception {
public:
    explicit InvalidTurnCountException(int turns)
        : m_message("Invalid number of turns requested: " + std::to_string(turns) + ". Must be 1 or more.") {}

    const char* what() const noexcept override {
        return m_message.c_str();
    }

private:
    std::string m_message;
};
