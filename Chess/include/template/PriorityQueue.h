#pragma once

#include <list>
#include <iostream>
#include <string>
#include "exception/QueueExceptions.h"

// Template class for a generic comparator
template <typename T>
struct MyComparator {
    int operator()(const T& a, const T& b) const {
        // Default implementation for numeric types
        return a - b;
    }
};

// Forward declaration for the friend operator overload (to access private members)
template <typename T, typename Comparator>
class PriorityQueue;

template <typename T, typename Comparator>
std::ostream& operator<<(std::ostream& os, const PriorityQueue<T, Comparator>& queue);


// Template class for PriorityQueue
template <typename T, typename Comparator = MyComparator<T>>
class PriorityQueue {
public:
    // Constructor with optional max size
    PriorityQueue(size_t maxSize = 5);

    // Push an element into the queue based on priority
    void push(const T& element);

    // Get and remove the highest priority element
    T poll();

    // Check if the queue is empty
    bool isEmpty() const;

    // Get the size of the queue
    size_t size() const;

    // Get all elements (for iteration)
    const std::list<T>& getElements() const;

    // Friend declaration for operator overload
    friend std::ostream& operator<<(std::ostream& os, const PriorityQueue<T, Comparator>& queue);

private:
    std::list<T> m_elements;
    Comparator m_comparator;
    size_t m_maxSize;
};

//------------------------------------------------------------------------
// Implementation of PriorityQueue methods
// Constructor
template <typename T, typename Comparator>
PriorityQueue<T, Comparator>::PriorityQueue(size_t maxSize) : m_maxSize(maxSize) {};

//------------------------------------------------------------------------
// Push method

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
// Poll method

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
// IsEmpty method

template <typename T, typename Comparator>
bool PriorityQueue<T, Comparator>::isEmpty() const {
    return m_elements.empty();
}

//------------------------------------------------------------------------
// Size method

template <typename T, typename Comparator>
size_t PriorityQueue<T, Comparator>::size() const {
    return m_elements.size();
}

//------------------------------------------------------------------------
// GetElements method
template <typename T, typename Comparator>
const std::list<T>& PriorityQueue<T, Comparator>::getElements() const {
    return m_elements;
}

//------------------------------------------------------------------------
// Overload the << operator for PriorityQueue

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