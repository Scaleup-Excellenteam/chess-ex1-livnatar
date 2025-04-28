#include "move\Move.h"

//------------------------------------------------------------------------

Move::Move(const std::pair<int, int>& from, const std::pair<int, int>& to,
           int score, char pieceType)
    : m_from(from), m_to(to), m_score(score), m_pieceType(pieceType) {}

//------------------------------------------------------------------------
std::pair<int, int> Move::getFrom() const {
    return m_from;
}
//------------------------------------------------------------------------
std::pair<int, int> Move::getTo() const {
    return m_to; 
}
//------------------------------------------------------------------------
int Move::getScore() const {
    return m_score; 
}
//------------------------------------------------------------------------
char Move::getPieceType() const { 
    return m_pieceType; 
}
//------------------------------------------------------------------------
void Move::setScore(int score) { 
    m_score = score; 
}
//------------------------------------------------------------------------
void Move::setPieceType(char pieceType) {
    m_pieceType = pieceType; 
}
//------------------------------------------------------------------------
std::string Move::fromNotation() const {
    return std::string(1, 'a' + m_from.first) + std::string(1, '1' + m_from.second);
}
//------------------------------------------------------------------------
std::string Move::toNotation() const {
    return std::string(1, 'a' + m_to.first) + std::string(1, '1' + m_to.second);
}
//------------------------------------------------------------------------
std::string Move::getNotation() const {
    return fromNotation() + toNotation();
}
//------------------------------------------------------------------------
// Stream operator implementation
std::ostream& operator<<(std::ostream& os, const Move& move) {
    os << move.fromNotation() << " -> " << move.toNotation()
        << " (Score: " << move.getScore() << ", Piece: " << move.getPieceType() << ")";
    return os;
}