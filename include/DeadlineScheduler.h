#pragma once

#include "IScheduler.h"
#include "MinHeap.h"
#include "Task.h"

// EDF (Earliest Deadline First) scheduler.

// Comparator for EDF scheduling.
struct DeadlineComparator {
    bool operator()(const Task& a, const Task& b) const {
        return a.deadline < b.deadline;
    }
};

class DeadlineScheduler : public IScheduler {
public:
    void addTask(const Task& task) override;
    Task getNextTask() override;
    bool hasNext() const override;
    std::string name() const override;

private:
    MinHeap<Task, DeadlineComparator> heap;
};
