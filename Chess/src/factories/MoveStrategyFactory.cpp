

#include "factories/MoveStrategyFactory.h"
#include <cctype>
#include "strategies/RookMoveStrategy.h"

//------------------------------------------------------------------------
// Initialize the static map
std::map<char, std::shared_ptr<MoveStrategy>> MoveStrategyFactory::m_strategies;

//------------------------------------------------------------------------

void MoveStrategyFactory::initialize() {
    
    m_strategies['R'] = std::make_shared<RookMoveStrategy>();
    m_strategies['r'] = m_strategies['R']; // Same strategy for lowercase
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