#include "FIFOScheduler.h"

void FIFOScheduler::addTask(const Task& task) {
    taskQueue.push(task);
}

Task FIFOScheduler::getNextTask() {
    Task front = taskQueue.front();
    taskQueue.pop();
    return front;
}

bool FIFOScheduler::hasNext() const {
    return !taskQueue.empty();
}

std::string FIFOScheduler::name() const {
    return "FIFO";
}
