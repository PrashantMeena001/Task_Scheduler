#pragma once

#include "IScheduler.h"
#include "Task.h"
#include <queue>

// FIFO scheduling strategy using a standard queue.

class FIFOScheduler : public IScheduler {
public:
    void addTask(const Task& task) override;
    Task getNextTask() override;
    bool hasNext() const override;
    std::string name() const override;

private:
    std::queue<Task> taskQueue;
};
