#pragma once

#include <vector>
#include <functional>
#include <stdexcept>

// MinHeap<T, Comparator> — A hand-rolled, array-backed binary min-heap.

template <typename T, typename Comparator = std::less<T>>
class MinHeap {
public:
    // Construct an empty heap with the given comparator.
    explicit MinHeap(Comparator comp = Comparator())
        : comp(comp)
    {
    }

    // Insert a new item into the heap.
    void insert(const T& item) {
        heap.push_back(item);
        siftUp(static_cast<int>(heap.size()) - 1);
    }

    // Remove and return the minimum element (the root).
    T extractMin() {
        if (heap.empty()) {
            throw std::runtime_error("extractMin() called on an empty heap");
        }

        T minItem = heap[0];

        // Move the last element to the root position
        heap[0] = heap.back();
        heap.pop_back();

        // Restore heap property by sifting the new root down
        if (!heap.empty()) {
            siftDown(0);
        }

        return minItem;
    }

    // Return the minimum element without removing it.
    const T& peek() const {
        if (heap.empty()) {
            throw std::runtime_error("peek() called on an empty heap");
        }
        return heap[0];
    }

    // Returns true if the heap contains no elements.
    bool isEmpty() const {
        return heap.empty();
    }

    // Returns the number of elements currently in the heap.
    int size() const {
        return static_cast<int>(heap.size());
    }

private:
    std::vector<T> heap;  // The flat array that stores the complete binary tree
    Comparator comp;      // Determines the ordering — what counts as "minimum"

    // Parent of node at index i is at (i-1)/2.
    // For the root (i=0), this returns 0, but siftUp's loop guard (index > 0)
    // prevents us from ever using that value incorrectly.
    int parent(int i) const {
        return (i - 1) / 2;
    }

    // Left child of node at index i.
    int left(int i) const {
        return 2 * i + 1;
    }

    // Right child of node at index i.
    int right(int i) const {
        return 2 * i + 2;
    }

    // Swap two elements in the heap array.
    void swap(int i, int j) {
        T temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;
    }

    // siftUp: restore heap property by moving element upward.
    void siftUp(int index) {
        while (index > 0 && comp(heap[index], heap[parent(index)])) {
            swap(index, parent(index));
            index = parent(index);
        }
    }

    // siftDown: restore heap property by moving element downward.
    void siftDown(int index) {
        int n = static_cast<int>(heap.size());

        while (true) {
            int best = index;     // assume current is the best
            int l = left(index);
            int r = right(index);

            // Check if the left child is "better" (smaller per comparator)
            if (l < n && comp(heap[l], heap[best])) {
                best = l;
            }

            // Check if the right child is even "better"
            if (r < n && comp(heap[r], heap[best])) {
                best = r;
            }

            // If neither child was better, heap property is satisfied
            if (best == index) {
                break;
            }

            swap(index, best);
            index = best;
        }
    }
};
