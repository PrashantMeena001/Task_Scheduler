#include "Logger.h"
#include <fstream>
#include <iostream>

Logger::Logger(const std::string& filepath)
    : filepath(filepath)
{
    // Write CSV header if file doesn't exist.
    std::ifstream checkFile(filepath);
    bool fileExists = checkFile.good() && checkFile.peek() != std::ifstream::traits_type::eof();
    checkFile.close();

    if (!fileExists) {
        std::ofstream outFile(filepath);
        if (outFile.is_open()) {
            outFile << "taskId,strategyName,arrivalTime,startTime,endTime,waitingTime,turnaroundTime\n";
            outFile.close();
        } else {
            std::cerr << "Warning: could not create log file: " << filepath << "\n";
        }
    }
}

void Logger::log(const Task& task, const std::string& strategyName,
                 int startTime, int endTime, int waitingTime, int turnaroundTime)
{
    // Open in append mode.
    std::ofstream outFile(filepath, std::ios::app);
    if (outFile.is_open()) {
        outFile << task.id << ","
                << strategyName << ","
                << task.arrivalTime << ","
                << startTime << ","
                << endTime << ","
                << waitingTime << ","
                << turnaroundTime << "\n";
        outFile.close();
    } else {
        std::cerr << "Warning: could not open log file for writing: " << filepath << "\n";
    }
}
