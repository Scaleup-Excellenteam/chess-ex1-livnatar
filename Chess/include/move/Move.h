#pragma once
#include <string>
#include <utility>
#include <iostream>

class Move {
public:
    // Constructor
    Move(const std::pair<int, int>& from, const std::pair<int, int>& to,
        int score = 0, char pieceType = ' ');

    // Getters
    std::pair<int, int> getFrom() const;
    std::pair<int, int> getTo() const;
    int getScore() const;
    char getPieceType() const;

    // Setters
    void setScore(int score);
    void setPieceType(char pieceType);

    // Convert positions to chess notation
    std::string fromNotation() const;
    std::string toNotation() const;
    std::string getNotation() const;

private:
    std::pair<int, int> m_from;
    std::pair<int, int> m_to;
    int m_score;
    char m_pieceType;
};

//------------------------------------------------------------------------
// Overload the << operator for Move to display it nicely
std::ostream& operator<<(std::ostream& os, const Move& move);

//------------------------------------------------------------------------
// Specialization for MyComparator - forward declaration
template <typename T>
struct MyComparator;

//------------------------------------------------------------------------
// Specialization of MyComparator for Move
template <>
struct MyComparator<Move> {
    int operator()(const Move& a, const Move& b) const {
        // Compare by score (default behavior for Move)
        return a.getScore() - b.getScore();
    }
};

