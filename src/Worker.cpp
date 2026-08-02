#include "Worker.h"
#include <iostream>

void Worker::run(Scheduler& scheduler, Logger& logger) {
    int simClock = 0;  // simulated time starts at 0

    while (scheduler.hasNext()) {
        Task task = scheduler.getNextTask();
        task.status = Task::Status::RUNNING;

        int startTime = simClock;

        // Jump clock forward if CPU was idle.
        if (task.arrivalTime > simClock) {
            simClock = task.arrivalTime;
            startTime = simClock;
        }

        int endTime = startTime + task.burstTime;
        simClock = endTime;
        task.status = Task::Status::COMPLETED;

        // Calculate metrics.
        int waitingTime = startTime - task.arrivalTime;
        int turnaroundTime = endTime - task.arrivalTime;

        // Log execution.
        logger.log(task, scheduler.currentStrategy(), startTime, endTime,
                   waitingTime, turnaroundTime);

        // Print live update.
        std::cout << "  [" << scheduler.currentStrategy() << "] "
                  << "Task " << task.id
                  << " | start=" << startTime
                  << " end=" << endTime
                  << " wait=" << waitingTime << "\n";
    }
}
