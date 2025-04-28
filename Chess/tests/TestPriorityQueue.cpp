
#include <iostream>
#include <string>
#include "template/PriorityQueue.h"
#include "exception/QueueExceptions.h"

// Custom comparator for strings (by length)
struct StringLengthComparator {
    int operator()(const std::string& a, const std::string& b) const {
        return a.length() - b.length(); // Longer strings have higher priority
    }
};

// Custom class for testing
class Task {
public:
    Task(int priority, std::string name) : priority(priority), name(name) {}

    friend std::ostream& operator<<(std::ostream& os, const Task& task) {
        os << task.name << " (priority: " << task.priority << ")";
        return os;
    }

    int getPriority() const { return priority; }

private:
    int priority;
    std::string name;
};

// Custom comparator for Task objects
struct TaskComparator {
    int operator()(const Task& a, const Task& b) const {
        return a.getPriority() - b.getPriority();
    }
};

int main() {
    bool allPassed = true;

    // Test 1: Basic integer queue (original test)
    std::cout << "===== Test 1: Basic Integer Queue =====" << std::endl;
    try {
        PriorityQueue<int, MyComparator<int>> pq(3);
        pq.push(10);
        pq.push(5);
        pq.push(20);

        std::cout << "Queue after pushes: " << std::endl;
        std::cout << pq << std::endl;

        // Test poll order
        int top = pq.poll();
        if (top != 20) {
            std::cerr << "Test failed: expected 20, got " << top << std::endl;
            allPassed = false;
        }

        top = pq.poll();
        if (top != 10) {
            std::cerr << "Test failed: expected 10, got " << top << std::endl;
            allPassed = false;
        }

        top = pq.poll();
        if (top != 5) {
            std::cerr << "Test failed: expected 5, got " << top << std::endl;
            allPassed = false;
        }

        // Test empty queue exception
        try {
            pq.poll();
            std::cerr << "Test failed: expected exception when polling empty queue" << std::endl;
            allPassed = false;
        }
        catch (const QueueEmptyException& e) {
            std::cout << "Caught expected exception: " << e.what() << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected exception: " << e.what() << std::endl;
        allPassed = false;
    }

    // Test 2: Full Queue Exception
    std::cout << "\n===== Test 2: Full Queue Exception =====" << std::endl;
    try {
        PriorityQueue<int, MyComparator<int>> pq(2);
        pq.push(10);
        pq.push(20);

        std::cout << "Full queue: " << std::endl;
        std::cout << pq << std::endl;

        // Try to add a lower priority element to a full queue
        try {
            pq.push(5);
            std::cerr << "Test failed: expected exception when pushing to full queue" << std::endl;
            allPassed = false;
        }
        catch (const QueueFullException& e) {
            std::cout << "Caught expected exception: " << e.what() << std::endl;
        }

        // Add a higher priority element to a full queue (should replace lowest)
        pq.push(30);

        // Check that 30 replaced 10 (lowest priority)
        std::cout << "Queue after pushing higher priority to full queue: " << std::endl;
        std::cout << pq << std::endl;

        if (pq.poll() != 30 || pq.poll() != 20) {
            std::cerr << "Test failed: queue replacement policy not working correctly" << std::endl;
            allPassed = false;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected exception: " << e.what() << std::endl;
        allPassed = false;
    }

    // Test 3: Custom String Comparator
    std::cout << "\n===== Test 3: Custom String Comparator =====" << std::endl;
    try {
        PriorityQueue<std::string, StringLengthComparator> pq(3);
        pq.push("a");
        pq.push("abc");
        pq.push("ab");

        std::cout << "String queue by length: " << std::endl;
        std::cout << pq << std::endl;

        // Check ordering by string length
        if (pq.poll() != "abc" || pq.poll() != "ab" || pq.poll() != "a") {
            std::cerr << "Test failed: string comparator not working correctly" << std::endl;
            allPassed = false;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected exception: " << e.what() << std::endl;
        allPassed = false;
    }

    // Test 4: Custom Class with Custom Comparator
    std::cout << "\n===== Test 4: Custom Class with Custom Comparator =====" << std::endl;
    try {
        PriorityQueue<Task, TaskComparator> pq(3);
        pq.push(Task(5, "Low priority task"));
        pq.push(Task(10, "High priority task"));
        pq.push(Task(7, "Medium priority task"));

        std::cout << "Task queue: " << std::endl;
        std::cout << pq << std::endl;

        // Check ordering by task priority
        Task task = pq.poll();
        if (task.getPriority() != 10) {
            std::cerr << "Test failed: expected highest priority task" << std::endl;
            allPassed = false;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected exception: " << e.what() << std::endl;
        allPassed = false;
    }

    // Test 5: Equal Priority Elements
    std::cout << "\n===== Test 5: Equal Priority Elements =====" << std::endl;
    try {
        PriorityQueue<int, MyComparator<int>> pq(3);
        pq.push(10);
        pq.push(10); // Same priority
        pq.push(10); // Same priority

        std::cout << "Queue with equal priorities: " << std::endl;
        std::cout << pq << std::endl;

        // Just make sure no errors occur when polling
        pq.poll();
        pq.poll();
        pq.poll();
        std::cout << "Successfully handled equal priority elements" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected exception with equal priorities: " << e.what() << std::endl;
        allPassed = false;
    }

    // Final result
    if (allPassed) {
        std::cout << "\nAll PriorityQueue tests passed!" << std::endl;
    }
    else {
        std::cout << "\nSome tests failed!" << std::endl;
    }

    return allPassed ? 0 : 1;
}

/*
#include <iostream>
#include "template/PriorityQueue.h"
#include "exception/QueueExceptions.h"

int main() {
    PriorityQueue<int, MyComparator<int>> pq(3);

    try {
        pq.push(10);
        pq.push(5);
        pq.push(20);

        // Printing the queue -> tests operator<<
        std::cout << "Queue after pushes: " << std::endl;
        std::cout << pq << std::endl;

        // Now testing poll
        int top = pq.poll();
        if (top != 20) {
            std::cerr << "Test failed: expected 20, got " << top << std::endl;
            return 1;
        }

        top = pq.poll();
        if (top != 10) {
            std::cerr << "Test failed: expected 10, got " << top << std::endl;
            return 1;
        }

        pq.poll(); // Last one

        // Now should throw
        try {
            pq.poll();
            std::cerr << "Test failed: expected exception when polling empty queue" << std::endl;
            return 1;
        }
        catch (const QueueEmptyException& e) {
            std::cout << "Caught expected exception: " << e.what() << std::endl;
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected exception: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "All PriorityQueue tests passed!" << std::endl;
    return 0;
}
*/