#pragma once

#include <list>
#include <iostream>
#include <string>
#include "exception/QueueExceptions.h"

//------------------------------------------------------------------------
/**
 * Template struct for a generic comparator.
 * Default implementation assumes T supports operator-.
 */
template <typename T>
struct MyComparator {
    int operator()(const T& a, const T& b) const {
        // Default implementation for numeric types
        return a - b;
    }
};

//------------------------------------------------------------------------
/**
 * Forward declaration of PriorityQueue class.
 */
template <typename T, typename Comparator>
class PriorityQueue;

//------------------------------------------------------------------------
/**
 * Forward declaration of stream insertion operator for PriorityQueue.
 */
template <typename T, typename Comparator>
std::ostream& operator<<(std::ostream& os, const PriorityQueue<T, Comparator>& queue);


//------------------------------------------------------------------------
/**
 * Template class representing a fixed-size priority queue.
 * Elements are ordered based on a provided comparator.
 */
template <typename T, typename Comparator = MyComparator<T>>
class PriorityQueue {
public:

    PriorityQueue(size_t maxSize = 5);

    void push(const T& element);
    T poll();
    bool isEmpty() const;
    size_t size() const;
    const std::list<T>& getElements() const;
    friend std::ostream& operator<<(std::ostream& os, const PriorityQueue<T, Comparator>& queue);

private:
    std::list<T> m_elements;
    Comparator m_comparator;
    size_t m_maxSize;
};


// Implementation of PriorityQueue methods
//------------------------------------------------------------------------
/**
* Constructs a PriorityQueue with an optional maximum size.
*
* @param maxSize The maximum number of elements the queue can hold (default 5).
*/
template <typename T, typename Comparator>
PriorityQueue<T, Comparator>::PriorityQueue(size_t maxSize) : m_maxSize(maxSize) {};

//------------------------------------------------------------------------
/**
* Pushes an element into the queue based on priority.
* Throws QueueFullException if the element cannot be added.
*
* @param element The element to insert.
*/

template <typename T, typename Comparator>
void PriorityQueue<T, Comparator>::push(const T& element) {
    
    // If the queue is full and the new element has lower priority than all current elements, reject it
    if (m_elements.size() >= m_maxSize) {
        
        // If the queue is not empty and the new element has lower priority than all current elements
        if (!m_elements.empty() && m_comparator(element, m_elements.back()) <= 0) {
            
            throw QueueFullException();
        }

        // If we have space or the new element has higher priority than some element
        // We'll remove the lowest priority one and then add it
        if (m_elements.size() == m_maxSize) {
            m_elements.pop_back(); // Remove the lowest priority element
        }
    }

    // Find the correct position to insert the element
    auto it = m_elements.begin();
    while (it != m_elements.end() && m_comparator(element, *it) <= 0) {
        ++it;
    }
    m_elements.insert(it, element);
}

//------------------------------------------------------------------------
/**
* Retrieves and removes the highest-priority element from the queue.
* Throws QueueEmptyException if the queue is empty.
*
* @return The highest-priority element.
*/

template <typename T, typename Comparator>
T PriorityQueue<T, Comparator>::poll() {

    if (m_elements.empty()) {
        
        throw QueueEmptyException();
    }

    T top = m_elements.front();
    m_elements.pop_front();
    return top;
}

//------------------------------------------------------------------------
/**
* Checks if the queue is empty.
*
* @return true if the queue has no elements, false otherwise.
*/

template <typename T, typename Comparator>
bool PriorityQueue<T, Comparator>::isEmpty() const {
    return m_elements.empty();
}

//------------------------------------------------------------------------
/**
* Returns the number of elements currently in the queue.
*
* @return The size of the queue.
*/

template <typename T, typename Comparator>
size_t PriorityQueue<T, Comparator>::size() const {
    return m_elements.size();
}

//------------------------------------------------------------------------
/**
* Provides access to all elements in the queue.
*
* @return A const reference to the internal list of elements.
*/

template <typename T, typename Comparator>
const std::list<T>& PriorityQueue<T, Comparator>::getElements() const {
    return m_elements;
}

//------------------------------------------------------------------------
/**
* Overloads the << operator to print the contents of the 3 top elements in the queue.
*/

template <typename T, typename Comparator>
std::ostream& operator<<(std::ostream& os, const PriorityQueue<T, Comparator>& queue) {
   
    os << "Priority Queue Contents:\n";
    int count = 1;
    for (const auto& element : queue.m_elements) {
        os << count << ". " << element << "\n";
        count++;

        // Break after printing 3 top elements
        if (count > 3) {
            break;
        }
    }
    return os;
}