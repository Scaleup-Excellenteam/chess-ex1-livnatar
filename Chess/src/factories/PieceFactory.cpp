
#include "factories/PieceFactory.h"
#include "factories/MoveStrategyFactory.h"
#include <iostream>
#include "pieces/Rook.h" 
#include "pieces/King.h"
#include "pieces/Bishop.h" 
#include "pieces/Queen.h" 
#include "pieces/Knight.h" // Include other pieces when added


//------------------------------------------------------------------------
/**
 * Static map that holds piece creation lambdas.
 * Key: Character representing the piece.
 * Value: Lambda that returns a unique_ptr to the corresponding piece.
 */
std::map<char, PieceFactory::PieceCreator> PieceFactory::m_pieceCreators;

//------------------------------------------------------------------------
/**
 * Initializes the factory by registering all piece creators.
 * Also ensures the move strategies are initialized first.
 */
void PieceFactory::initialize() {

    // Initialize the move strategy factory 
    MoveStrategyFactory::initialize();
   
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

    m_pieceCreators['N'] = [](bool isWhite, char symbol, const std::pair<int, int>& pos) {
        return std::make_unique<Knight>(true, 'N', pos);
        };
    m_pieceCreators['n'] = [](bool isWhite, char symbol, const std::pair<int, int>& pos) {
        return std::make_unique<Knight>(false, 'n', pos);
        };

    // Add other pieces 

}

//------------------------------------------------------------------------
/**
 * Creates a piece based on its character representation.
 * @param pieceChar Character representing the piece type ('R', 'k', etc.)
 * @param pos Position on the board where the piece will be placed.
 * @return A unique_ptr to the created ChessPiece or nullptr if empty or unknown.
 */
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