#include "move\Move.h"

//------------------------------------------------------------------------
/**
 * Constructor for the Move class.
 *
 * @param from      The starting position of the move.
 * @param to        The target position of the move.
 * @param score     The score associated with the move (used for evaluation).
 * @param pieceType The type of piece being moved (e.g., 'P' for pawn, 'K' for king).
 */
Move::Move(const std::pair<int, int>& from, const std::pair<int, int>& to,
           int score, char pieceType)
    : m_from(from), m_to(to), m_score(score), m_pieceType(pieceType) {}

//------------------------------------------------------------------------
/**
 * Gets the starting position of the move.
 *
 * @return A pair representing the coordinates of the starting position.
 */
std::pair<int, int> Move::getFrom() const {
    return m_from;
}
//------------------------------------------------------------------------
/**
 * Gets the target position of the move.
 *
 * @return A pair representing the coordinates of the target position.
 */
std::pair<int, int> Move::getTo() const {
    return m_to; 
}
//------------------------------------------------------------------------
/**
 * Gets the score associated with the move.
 *
 * @return The score (an integer) that represents the evaluation of the move.
 */
int Move::getScore() const {
    return m_score; 
}
//------------------------------------------------------------------------
/**
 * Gets the type of piece being moved.
 *
 * @return A character representing the type of piece (e.g., 'P' for pawn, 'K' for king).
 */
char Move::getPieceType() const { 
    return m_pieceType; 
}
//------------------------------------------------------------------------
/**
 * Sets the score for the move.
 *
 * @param score The new score to be assigned to the move.
 */
void Move::setScore(int score) { 
    m_score = score; 
}
//------------------------------------------------------------------------
/**
 * Sets the type of piece being moved.
 *
 * @param pieceType A character representing the type of piece (e.g., 'P' for pawn).
 */
void Move::setPieceType(char pieceType) {
    m_pieceType = pieceType; 
}
//------------------------------------------------------------------------
/**
 * Converts the starting position of the move into standard algebraic notation (e.g., 'a1').
 *
 * @return A string representing the starting position in algebraic notation.
 */
std::string Move::fromNotation() const {
    return std::string(1, 'a' + m_from.first) + std::to_string(m_from.second + 1);
}
//------------------------------------------------------------------------
/**
 * Converts the target position of the move into standard algebraic notation (e.g., 'a1').
 *
 * @return A string representing the target position in algebraic notation.
 */
std::string Move::toNotation() const {
    return std::string(1, 'a' + m_to.first) + std::to_string(m_to.second + 1);
}
//------------------------------------------------------------------------
/**
 * Combines the starting and target positions into a complete algebraic notation string.
 *
 * @return A string representing the full move in algebraic notation (e.g., "a2a3").
 */
std::string Move::getNotation() const {
    return fromNotation() + toNotation();
}
//------------------------------------------------------------------------
/**
 * Stream operator for printing a Move object.
 *
 * @param os The output stream to print to.
 * @param move The Move object to be printed.
 * @return The output stream with the move details.
 */
std::ostream& operator<<(std::ostream& os, const Move& move) {

    os << move.fromNotation() << " -> " << move.toNotation()
        << " (Score: " << move.getScore() << ", Piece: " << move.getPieceType() << ")";
    return os;
}