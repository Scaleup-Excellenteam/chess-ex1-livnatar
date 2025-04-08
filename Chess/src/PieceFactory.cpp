
#include "PieceFactory.h"
#include <iostream>
#include "Rook.h" 
#include "King.h"
#include "Bishop.h" 
#include "Queen.h" // Include other pieces when added


//------------------------------------------------------------------------

std::map<char, PieceFactory::PieceCreator> PieceFactory::m_pieceCreators;

void PieceFactory::initialize() {
   
    // Register piece creators
    m_pieceCreators['R'] = [](bool isWhite, char symbol, const std::pair<int, int>& pos) {
        return std::make_unique<Rook>(true, 'R', pos);
        };

    m_pieceCreators['r'] = [](bool isWhite, char symbol, const std::pair<int, int>& pos) {
        return std::make_unique<Rook>(false, 'r', pos);
        };

    m_pieceCreators['K'] = [](bool isWhite, char symbol, const std::pair<int, int>& pos) {
        return std::make_unique<King>(true, 'K', pos);
        };

    m_pieceCreators['k'] = [](bool isWhite, char symbol, const std::pair<int, int>& pos) {
        return std::make_unique<King>(false, 'k', pos);
        };

    m_pieceCreators['B'] = [](bool isWhite, char symbol, const std::pair<int, int>& pos) {
        return std::make_unique<Bishop>(true, 'B', pos);
        };

    m_pieceCreators['b'] = [](bool isWhite, char symbol, const std::pair<int, int>& pos) {
        return std::make_unique<Bishop>(false, 'b', pos);
        };

    m_pieceCreators['Q'] = [](bool isWhite, char symbol, const std::pair<int, int>& pos) {
        return std::make_unique<Queen>(true, 'Q', pos);
        };

    m_pieceCreators['q'] = [](bool isWhite, char symbol, const std::pair<int, int>& pos) {
        return std::make_unique<Queen>(false, 'q', pos);
        };

    // Add other pieces 

}

//------------------------------------------------------------------------

std::unique_ptr<ChessPiece> PieceFactory::createPiece(char pieceChar, const std::pair<int, int>& pos) {
    
    
    // Handle empty square
    if (pieceChar == '#') {
        return nullptr;
    }

    // Check if we have a creator for this piece
    auto it = m_pieceCreators.find(pieceChar);
    if (it != m_pieceCreators.end()) {
        
        bool isWhite = std::isupper(pieceChar) != 0;
        return it->second(isWhite, pieceChar, pos);
    }

    // Handle unknown piece
    std::cout << "Warning: Unknown piece character '" << pieceChar
        << "' at position (" << pos.first << ", " << pos.second << ")." << std::endl;

    return nullptr;
}