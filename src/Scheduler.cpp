#include "Scheduler.h"

void Scheduler::setStrategy(std::unique_ptr<IScheduler> newStrategy) {
    // Transfer ownership of the strategy.
    strategy = std::move(newStrategy);
}

void Scheduler::addTask(const Task& task) {
    strategy->addTask(task);
}

Task Scheduler::getNextTask() {
    return strategy->getNextTask();
}

bool Scheduler::hasNext() const {
    return strategy->hasNext();
}

std::string Scheduler::currentStrategy() const {
    return strategy->name();
}
