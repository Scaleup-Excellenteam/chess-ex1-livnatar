

#include "factories/MoveStrategyFactory.h"
#include <cctype>
#include "strategies/RookMoveStrategy.h"
#include "strategies/BishopMoveStrategy.h"
#include "strategies/KnightMoveStrategy.h"
#include "strategies/KingMoveStrategy.h"
#include "strategies/QueenMoveStrategy.h"
#include "strategies/PawnMoveStrategy.h"

//------------------------------------------------------------------------
/**
 * Static map that holds the move strategies for each piece type.
 * Key: Piece character ('R', 'b', etc.)
 * Value: Corresponding shared MoveStrategy instance.
 */
std::map<char, std::shared_ptr<MoveStrategy>> MoveStrategyFactory::m_strategies;

//------------------------------------------------------------------------
/**
 * Initializes the move strategies for each piece type and registers them
 * in the strategy map.
 */
void MoveStrategyFactory::initialize() {
  
    // Create strategies
    auto rookStrategy = std::make_shared<RookMoveStrategy>();
    auto bishopStrategy = std::make_shared<BishopMoveStrategy>();
    auto knightStrategy = std::make_shared<KnightMoveStrategy>();
    auto kingStrategy = std::make_shared<KingMoveStrategy>();
    auto queenStrategy = std::make_shared<QueenMoveStrategy>(rookStrategy, bishopStrategy);
    auto pawnStrategy = std::make_shared<PawnMoveStrategy>();

    // Register all piece strategies
    m_strategies['R'] = rookStrategy;
    m_strategies['r'] = rookStrategy;

    m_strategies['B'] = bishopStrategy;
    m_strategies['b'] = bishopStrategy;

    m_strategies['N'] = knightStrategy;
    m_strategies['n'] = knightStrategy;

    m_strategies['Q'] = queenStrategy;
    m_strategies['q'] = queenStrategy;

    m_strategies['K'] = kingStrategy;
    m_strategies['k'] = kingStrategy;

    m_strategies['P'] = pawnStrategy;
    m_strategies['p'] = pawnStrategy;
}
//------------------------------------------------------------------------
/**
 * Retrieves the move strategy for the given piece character.
 * @param pieceType Character representing the piece type.
 * @return Shared pointer to the corresponding MoveStrategy, or nullptr if not found.
 */
std::shared_ptr<MoveStrategy> MoveStrategyFactory::createMoveStrategy(char pieceType) {
    
    auto it = m_strategies.find(pieceType);
    
    if (it != m_strategies.end()) {
        return it->second;
    }

    // No strategy found
    return nullptr; 
}