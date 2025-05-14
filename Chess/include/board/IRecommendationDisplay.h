
/**
 * Interface for chess move recommendation display
 *
 * This interface defines the contract for classes that can display move recommendations
 * in a chess game. It allows for loose coupling between the Chess UI and the recommendation
 * logic, following the Interface Segregation Principle.
 */
class IRecommendationDisplay {
public:
    virtual void showRecommendations() const = 0;
    virtual ~IRecommendationDisplay() = default;
};