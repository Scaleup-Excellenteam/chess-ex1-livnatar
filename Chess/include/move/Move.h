#pragma once
#include <string>
#include <utility>
#include <iostream>
#include "exception/MoveExceptions.h"

class Move {
public:
    Move(const std::pair<int, int>& from, const std::pair<int, int>& to,
         int score = 0, char pieceType = ' ');

    std::pair<int, int> getFrom() const;
    std::pair<int, int> getTo() const;
    int getScore() const;
    char getPieceType() const;
    void setScore(int score);
    void setPieceType(char pieceType);
    std::string fromNotation() const;
    std::string toNotation() const;
    std::string getNotation() const;
   
    friend std::ostream& operator<<(std::ostream& os, const Move& move);

private:
    std::pair<int, int> m_from;
    std::pair<int, int> m_to;
    int m_score;
    char m_pieceType;
};

//------------------------------------------------------------------------
// Specialization for MyComparator - forward declaration
template <typename T>
struct MyComparator;

//------------------------------------------------------------------------
// Specialization of MyComparator for Move
/**
 * A comparator for Move objects, used for sorting moves by score.
 *
 * @tparam T The type to be compared (in this case, Move).
 */
template <>
struct MyComparator<Move> {
    
    /**
     * Compares two Move objects based on their score.
     *
     * @param a The first Move object.
     * @param b The second Move object.
     * @return A negative value if a has a lower score than b,
     *         zero if they are equal,
     *         and a positive value if a has a higher score than b.
     */
    int operator()(const Move& a, const Move& b) const {
        
        return a.getScore() - b.getScore();
    }
};

