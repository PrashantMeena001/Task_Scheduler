#include "DeadlineScheduler.h"

void DeadlineScheduler::addTask(const Task& task) {
    heap.insert(task);
}

Task DeadlineScheduler::getNextTask() {
    return heap.extractMin();
}

bool DeadlineScheduler::hasNext() const {
    return !heap.isEmpty();
}

std::string DeadlineScheduler::name() const {
    return "Deadline (EDF)";
}
