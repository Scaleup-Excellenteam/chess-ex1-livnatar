# Chess Move Recommendation System

This project extends a previous chess game implementation by introducing a **templated priority queue** and a **move recommendation engine** that evaluates potential moves based on a scoring algorithm and suggests the best moves to the player.

---

## Objectives

- Use of templates (C++) for creating a generic priority queue.
- Practice exception handling with custom exceptions.
- Develop an intelligent algorithm to evaluate and recommend chess moves.
- Explore recursive decision-making (lookahead) in a simplified chess AI.

---

## Features Implemented

### Priority Queue

- `PriorityQueue<T, Comparator>` class implemented with a custom comparator and fixed capacity.
- Backed by `std::list` with manual sorted insertion.
- Push: O(n) — sorted insert with comparator.
- Poll: O(1) — front retrieval and removal.
- Overloaded `<<` operator to show top 3 moves.

### Move Class

- Stores a move’s origin, destination, type (piece type), and score.
- Overloads `<<` for display.

### Move Recommender

- Evaluates all possible legal moves for a player and recursively checks future consequences up to a defined depth.
- Calculates score based on:
  - **Material gain/loss** (captures and threats)
  - **Board control**
  - **Center control**
  - **Safety of the moving piece**
- Top 5 best moves are tracked using the `PriorityQueue`.

### Exception Handling

Custom exceptions implemented:

- `QueueFullException`: when trying to insert into a full queue with a weaker move.
- `QueueEmptyException`: when polling from an empty queue.
- `NoMovesAvailableException`: when no legal moves exist for the current player.

---

## Recommendation Algorithm (Overview)

### Step 1: Generate All Legal Moves
- Scans the board for pieces of the current player and gathers all valid moves.

### Step 2: Evaluate Moves
Each move is scored based on:
- Capturing an enemy piece (+value)
- Threatening stronger pieces (+)
- Being in danger (-)
- Center control bonus
- Coverage control bonus

### Step 3: Recursive Lookahead
- Each move is simulated by copying the board and playing the move.
- Then, the opponent's best response is recursively evaluated and subtracted from our score.
- Optional depth up to 2 or 3 plies.

### Step 4: Keep Top 5 Moves
- Insert moves into a `PriorityQueue` based on score.
- If the queue is full, ignore moves that don't beat the current weakest.

