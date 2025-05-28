# Chess Move Recommender

## Overview

This project implements a chess move recommendation system that analyzes the board state and suggests the best moves for the current player. The system uses a minimax-like algorithm to evaluate moves by looking ahead several turns, considering both the immediate benefits of a move and potential future consequences.

## Algorithm Description

The move recommender uses a depth-limited minimax approach to evaluate chess moves:

### Move Generation

For each piece belonging to the current player, all valid moves are generated.

### Move Evaluation

Each move is evaluated using a scoring system that considers:

- **Capture Value**: Points for capturing opponent pieces (weighted by piece value)
- **Piece Safety**: Penalties for putting pieces in danger, especially if threatened by lower-value pieces
- **Threats Created**: Bonuses for threatening opponent pieces. Threatening stronger pieces gives higher bonuses (up to half their value), while threatening weaker pieces gives minimal bonuses.
- **Center Control**: +2 points for central 4 squares, +1 point for extended center.
- **Board Control**: The difference between the number of squares controlled by each player, scaled down to reduce overall weight in the final score.

### Look-ahead Evaluation

The algorithm recursively evaluates:

- **Depth 0**: Immediate position evaluation
- **Depth 1**: Opponent's best response (which minimizes our score)
- **Depth 2**: Our best counter-response
- And so on up to the specified depth

All scores are computed from the maximizing player's perspective, meaning opponent moves subtract value and our responses add value accordingly.

### Priority Queue

The top 5 highest-scoring moves are maintained in a priority queue and the top 3 recommended to the player.

## Optimization Techniques

The implementation uses several optimizations to improve performance:

### Board State Management

Instead of creating multiple copies of the chess board during recursive evaluation, the implementation:

- Creates a single copy of the board
- Applies moves to this copy
- Records sufficient information to undo the moves
- Restores the board after evaluation

This significantly reduces memory usage and improves performance.

### Move Data Storage

The `MoveData` structure stores information about moves, including captured pieces, to enable proper move undoing.

## Complexity Analysis

### Time Complexity

The time complexity of the algorithm is **O(b^d)**, where:

- `b` is the branching factor (average number of valid moves at each position, typically ~35 in chess)
- `d` is the search depth (as specified in the assignment, `d = 2`)

This corresponds to:

- **Depth 0**: Current player's move
- **Depth 1**: Opponent’s response
- **Depth 2**: Current player's counter-response

This results in evaluating up to **35^3 = 42,875** positions in the worst case (3 layers of move evaluation: root, opponent, response).

### Space Complexity

The optimized implementation has a space complexity of **O(b*d)**, where:

- `b` represents the branching factor
- `d` is the search depth

This is achieved by using move/undo operations rather than creating new board copies at each level.

## Bonus Features

### Center Control

The algorithm gives bonus points for moves that control central squares:

- **+2 points** for controlling the 4 center squares (`d4`, `d5`, `e4`, `e5`)
- **+1 point** for controlling the extended center (12 squares surrounding the center)

### Board Coverage Analysis

The implementation calculates how many squares each player controls and awards points based on the difference. This encourages moves that:

- Maximize the player's board control
- Minimize the opponent's control options

## Exception Handling

This project defines five custom exceptions that inherit from `std::exception`, used to handle invalid states during move recommendation and priority queue operations:

### `InvalidTurnCountException`
Thrown when an invalid number of turns (depth < 0) is requested. Ensures the algorithm has a valid lookahead depth.

**Example message**:  
`Invalid number of turns requested: 0. Must be 1 or more.`

---

### `NoMovesAvailableException`
Thrown when the current player has no valid moves (e.g., in stalemate or checkmate).

**Message**:  
`No valid moves available for the current player`

---

### `QueueEmptyException`
Thrown when trying to remove a move from an empty priority queue.

**Message**:  
`Attempted to poll from an empty priority queue`

---

### `QueueFullException`
Thrown when trying to insert a lower-priority move into a full priority queue.

**Message**:  
`Attempted to push into a full priority queue with lower priority element`

---

### `InvalidPositionException`
Thrown when a move string is not in valid chess notation or refers to an out-of-bounds position.  
This ensures that all move inputs are validated before processing.

**Example message**:  
`Invalid chess position: "e9"`