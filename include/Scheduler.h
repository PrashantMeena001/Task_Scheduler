#pragma once

#include "IScheduler.h"
#include <memory>
#include <string>

// Scheduler Context class for the Strategy pattern.

class Scheduler {
public:
    // Replace the current scheduling strategy.
    void setStrategy(std::unique_ptr<IScheduler> newStrategy);

    // Delegate methods to the active strategy.
    void addTask(const Task& task);
    Task getNextTask();
    bool hasNext() const;

    // Returns the name of the currently active strategy (e.g., "FIFO")
    std::string currentStrategy() const;

private:
    std::unique_ptr<IScheduler> strategy;
};
