#pragma once

#include "Task.h"
#include <string>

// Logs execution records to a CSV file.

class Logger {
public:
    // Construct a logger that writes to the given file path.
    // If the file does not exist, it will be created with a CSV header row.
    explicit Logger(const std::string& filepath);

    // Append one row to the CSV log capturing a completed task's execution details.
    void log(const Task& task, const std::string& strategyName,
             int startTime, int endTime, int waitingTime, int turnaroundTime);

private:
    std::string filepath;
};
