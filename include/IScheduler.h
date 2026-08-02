#pragma once

#include "Task.h"
#include <string>

// IScheduler — Strategy interface for scheduling algorithms.

class IScheduler {
public:
    virtual ~IScheduler() = default;

    // Add a task to this scheduler's internal data structure
    virtual void addTask(const Task& task) = 0;

    // Remove and return the next task according to this strategy's ordering
    virtual Task getNextTask() = 0;

    // Returns true if there are still tasks waiting to be processed
    virtual bool hasNext() const = 0;

    // Returns a human-readable name for this strategy (e.g., "FIFO", "Priority")
    virtual std::string name() const = 0;
};
