#pragma once
#include <stdexcept>
#include <string>

class InvalidPositionException : public std::exception {
public:
    explicit InvalidPositionException(const std::string& pos)
        : m_message("Invalid chess position: \"" + pos + "\"") {
    }

    const char* what() const noexcept override {
        return m_message.c_str();
    }
private:
    std::string m_message;
};