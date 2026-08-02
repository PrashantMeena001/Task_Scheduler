#pragma once

#include <string>

// Data class representing a single task.

class Task {
public:
    // Possible lifecycle states of a task
    enum class Status { PENDING, RUNNING, COMPLETED };

    int id;
    int priority;      // lower number = higher priority (1 is the most urgent)
    int deadline;       // the simulated time by which this task should finish
    int burstTime;      // how many time units this task needs on the CPU
    int arrivalTime;    // the simulated time at which the task enters the system

    Status status;

    // Default constructor.
    Task();

    // Parameterized constructor.
    Task(int id, int priority, int deadline, int burstTime, int arrivalTime);

    // Returns a human-readable one-line summary, useful for debugging and CLI output
    std::string toString() const;
};
