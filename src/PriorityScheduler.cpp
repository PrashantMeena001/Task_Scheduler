#include "PriorityScheduler.h"

void PriorityScheduler::addTask(const Task& task) {
    heap.insert(task);
}

Task PriorityScheduler::getNextTask() {
    return heap.extractMin();
}

bool PriorityScheduler::hasNext() const {
    return !heap.isEmpty();
}

std::string PriorityScheduler::name() const {
    return "Priority";
}
