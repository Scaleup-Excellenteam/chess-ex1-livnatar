
#pragma once
#include <memory>
#include <map>
#include <functional>
#include "ChessPiece.h"

class PieceFactory {

public:
    static void initialize();
    static std::unique_ptr<ChessPiece> createPiece(char pieceChar, const std::pair<int, int>& pos);


private:
    using PieceCreator = std::function<std::unique_ptr<ChessPiece>(bool, char, const std::pair<int, int>&)>;
    static std::map<char, PieceCreator> m_pieceCreators;

};