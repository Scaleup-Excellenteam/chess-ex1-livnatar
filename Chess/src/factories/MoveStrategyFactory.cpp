

#include "factories/MoveStrategyFactory.h"
#include <cctype>
#include "strategies/RookMoveStrategy.h"
#include "strategies/BishopMoveStrategy.h"
#include "strategies/KnightMoveStrategy.h"

//------------------------------------------------------------------------
// Initialize the static map
std::map<char, std::shared_ptr<MoveStrategy>> MoveStrategyFactory::m_strategies;

//------------------------------------------------------------------------

void MoveStrategyFactory::initialize() {
    
    m_strategies['R'] = std::make_shared<RookMoveStrategy>();
    m_strategies['r'] = m_strategies['R']; // Same strategy for lowercase

    m_strategies['B'] = std::make_shared<BishopMoveStrategy>();
    m_strategies['b'] = m_strategies['B'];

    m_strategies['N'] = std::make_shared<KnightMoveStrategy>();
    m_strategies['n'] = m_strategies['N'];

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