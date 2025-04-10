

#include "factories/MoveStrategyFactory.h"
#include <cctype>

//------------------------------------------------------------------------
// Initialize the static map
std::map<char, std::shared_ptr<MoveStrategy>> MoveStrategyFactory::m_strategies;

//------------------------------------------------------------------------

void MoveStrategyFactory::initialize() {
    
    // This is just the skeleton implementation
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