

#include "factories/MoveStrategyFactory.h"
#include <cctype>
#include "strategies/RookMoveStrategy.h"
#include "strategies/BishopMoveStrategy.h"
#include "strategies/KnightMoveStrategy.h"
#include "strategies/KingMoveStrategy.h"
#include "strategies/QueenMoveStrategy.h"

//------------------------------------------------------------------------
// Initialize the static map
std::map<char, std::shared_ptr<MoveStrategy>> MoveStrategyFactory::m_strategies;

//------------------------------------------------------------------------

void MoveStrategyFactory::initialize() {
  
    // Create strategies
    auto rookStrategy = std::make_shared<RookMoveStrategy>();
    auto bishopStrategy = std::make_shared<BishopMoveStrategy>();
    auto knightStrategy = std::make_shared<KnightMoveStrategy>();
    auto kingStrategy = std::make_shared<KingMoveStrategy>();
    auto queenStrategy = std::make_shared<QueenMoveStrategy>(rookStrategy, bishopStrategy);

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
}
//------------------------------------------------------------------------

std::shared_ptr<MoveStrategy> MoveStrategyFactory::createMoveStrategy(char pieceType) {
    
    auto it = m_strategies.find(pieceType);
    
    if (it != m_strategies.end()) {
        return it->second;
    }

    // No strategy found
    return nullptr; 
}