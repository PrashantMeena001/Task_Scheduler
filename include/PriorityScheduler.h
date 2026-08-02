#pragma once

#include "IScheduler.h"
#include "MinHeap.h"
#include "Task.h"

// Priority scheduling using a MinHeap.
// Lower priority value means higher urgency.

// Comparator for priority scheduling.
struct PriorityComparator {
    bool operator()(const Task& a, const Task& b) const {
        return a.priority < b.priority;
    }
};

class PriorityScheduler : public IScheduler {
public:
    void addTask(const Task& task) override;
    Task getNextTask() override;
    bool hasNext() const override;
    std::string name() const override;

private:
    MinHeap<Task, PriorityComparator> heap;
};
