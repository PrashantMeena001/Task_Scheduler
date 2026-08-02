#include "Task.h"
#include "Scheduler.h"
#include "Worker.h"
#include "Logger.h"
#include "FIFOScheduler.h"
#include "PriorityScheduler.h"
#include "DeadlineScheduler.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <memory>
#include <cstdio>

// Create hardcoded tasks for demo.
static std::vector<Task> createTasks() {
    return {
        Task(1, 3, 10, 3, 0),
        Task(2, 1, 20, 5, 1),
        Task(3, 4,  5, 2, 2),
        Task(4, 2, 15, 4, 3),
        Task(5, 5, 25, 1, 4),
        Task(6, 1,  8, 3, 5),
        Task(7, 3, 12, 6, 6),
        Task(8, 2, 18, 2, 7)
    };
}

// Print the task table.
static void printTaskTable(const std::vector<Task>& tasks) {
    std::cout << "\nAvailable Tasks:\n";
    std::cout << "  " << std::left
              << std::setw(6)  << "ID"
              << std::setw(10) << "Priority"
              << std::setw(10) << "Deadline"
              << std::setw(12) << "BurstTime"
              << std::setw(12) << "ArrivalTime"
              << "\n";

    for (const auto& t : tasks) {
        std::cout << "  " << std::left
                  << std::setw(6)  << t.id
                  << std::setw(10) << t.priority
                  << std::setw(10) << t.deadline
                  << std::setw(12) << t.burstTime
                  << std::setw(12) << t.arrivalTime
                  << "\n";
    }
    std::cout << "\n";
}

// Execution data for Gantt chart and metrics.
struct ExecutionRecord {
    int taskId;
    int startTime;
    int endTime;
    int waitingTime;
    int turnaroundTime;
};

// Run a single strategy and collect execution records.
static std::vector<ExecutionRecord> runStrategy(
    Scheduler& scheduler,
    const std::vector<Task>& tasks,
    Logger& logger)
{
    // Load all tasks into the scheduler
    for (const auto& task : tasks) {
        scheduler.addTask(task);
    }

    std::string stratName = scheduler.currentStrategy();

    std::cout << "----------------------------------------\n";
    std::cout << "Running: " << stratName << "\n";
    std::cout << "----------------------------------------\n";

    // Simulate execution and collect records
    std::vector<ExecutionRecord> records;
    int simClock = 0;

    while (scheduler.hasNext()) {
        Task task = scheduler.getNextTask();
        task.status = Task::Status::RUNNING;

        int startTime = simClock;
        if (task.arrivalTime > simClock) {
            simClock = task.arrivalTime;
            startTime = simClock;
        }

        int endTime = startTime + task.burstTime;
        simClock = endTime;
        task.status = Task::Status::COMPLETED;

        int waitingTime = startTime - task.arrivalTime;
        int turnaroundTime = endTime - task.arrivalTime;

        // Log to CSV
        logger.log(task, stratName, startTime, endTime, waitingTime, turnaroundTime);

        // Print live console line
        std::cout << "  [" << stratName << "] "
                  << "Task " << task.id
                  << " | start=" << startTime
                  << " end=" << endTime
                  << " wait=" << waitingTime << "\n";

        records.push_back({task.id, startTime, endTime, waitingTime, turnaroundTime});
    }

    std::cout << "\n";

    // --- ASCII Gantt Chart ---
    int maxTime = 0;
    for (const auto& r : records) {
        if (r.endTime > maxTime) maxTime = r.endTime;
    }

    // Round up to next multiple of 5 for neat axis labels
    int axisEnd = ((maxTime / 5) + 1) * 5;

    std::cout << "Gantt Chart: " << stratName << "\n";

    // Time axis labels
    std::cout << "Time: ";
    for (int t = 0; t <= axisEnd; t += 5) {
        std::cout << std::left << std::setw(5) << t;
    }
    std::cout << "\n";

    // Tick marks
    std::cout << "      ";
    for (int t = 0; t <= axisEnd; t += 5) {
        std::cout << "|    ";
    }
    std::cout << "\n";

    // One row per task showing its execution bar
    for (const auto& r : records) {
        std::cout << "T" << std::left << std::setw(5) << r.taskId;

        // Spaces up to startTime, then [===] for the burst duration
        for (int t = 0; t < r.startTime; t++) {
            std::cout << " ";
        }
        std::cout << "[";
        for (int t = r.startTime; t < r.endTime; t++) {
            std::cout << "=";
        }
        std::cout << "]";
        std::cout << "\n";
    }

    std::cout << "\n";

    // --- Average metrics ---
    double totalWait = 0.0;
    double totalTurnaround = 0.0;
    for (const auto& r : records) {
        totalWait += r.waitingTime;
        totalTurnaround += r.turnaroundTime;
    }
    double avgWait = totalWait / static_cast<double>(records.size());
    double avgTurnaround = totalTurnaround / static_cast<double>(records.size());

    std::cout << "Results:\n";
    std::cout << "  Avg Waiting Time:     " << std::fixed << std::setprecision(1) << avgWait << "\n";
    std::cout << "  Avg Turnaround Time:  " << std::fixed << std::setprecision(1) << avgTurnaround << "\n";
    std::cout << "\n";

    return records;
}

// Main entry point.
int main() {
    std::cout << "========================================\n";
    std::cout << "  Task Scheduler -- Job Queue System\n";
    std::cout << "  OOP Demo | IIT Ropar\n";
    std::cout << "========================================\n";

    std::vector<Task> tasks = createTasks();
    printTaskTable(tasks);

    std::cout << "Select scheduling strategy:\n";
    std::cout << "  1. FIFO (First In, First Out)\n";
    std::cout << "  2. Priority-Based Scheduling\n";
    std::cout << "  3. Deadline-Based Scheduling (EDF)\n";
    std::cout << "  4. Run all three and compare\n";
    std::cout << "> ";

    int choice = 0;
    std::cin >> choice;
    std::cout << "\n";

    // Delete the old CSV log so each run starts fresh
    std::remove("data/execution_log.csv");

    Logger logger("data/execution_log.csv");
    Scheduler scheduler;

    if (choice == 1) {
        scheduler.setStrategy(std::make_unique<FIFOScheduler>());
        runStrategy(scheduler, tasks, logger);

    } else if (choice == 2) {
        scheduler.setStrategy(std::make_unique<PriorityScheduler>());
        runStrategy(scheduler, tasks, logger);

    } else if (choice == 3) {
        scheduler.setStrategy(std::make_unique<DeadlineScheduler>());
        runStrategy(scheduler, tasks, logger);

    } else if (choice == 4) {
        // Run all three strategies with fresh copies of the task list

        // --- FIFO ---
        scheduler.setStrategy(std::make_unique<FIFOScheduler>());
        auto fifoRecords = runStrategy(scheduler, tasks, logger);

        // --- Priority ---
        scheduler.setStrategy(std::make_unique<PriorityScheduler>());
        auto prioRecords = runStrategy(scheduler, tasks, logger);

        // --- Deadline (EDF) ---
        scheduler.setStrategy(std::make_unique<DeadlineScheduler>());
        auto edfRecords = runStrategy(scheduler, tasks, logger);

        // Compute averages for the comparison table
        auto computeAvg = [](const std::vector<ExecutionRecord>& recs,
                             double& avgWait, double& avgTurn) {
            double tw = 0, tt = 0;
            for (const auto& r : recs) {
                tw += r.waitingTime;
                tt += r.turnaroundTime;
            }
            avgWait = tw / static_cast<double>(recs.size());
            avgTurn = tt / static_cast<double>(recs.size());
        };

        double fifoW, fifoT, prioW, prioT, edfW, edfT;
        computeAvg(fifoRecords, fifoW, fifoT);
        computeAvg(prioRecords, prioW, prioT);
        computeAvg(edfRecords,  edfW,  edfT);

        std::cout << "========================================\n";
        std::cout << "Strategy Comparison\n";
        std::cout << "========================================\n";
        std::cout << std::left << std::setw(20) << "Strategy"
                  << "| " << std::setw(9) << "Avg Wait"
                  << "| " << std::setw(14) << "Avg Turnaround" << "\n";
        std::cout << "--------------------|---------|--------------\n";

        std::cout << std::left << std::setw(20) << "FIFO"
                  << "| " << std::setw(9) << std::fixed << std::setprecision(1) << fifoW
                  << "| " << std::setw(14) << fifoT << "\n";

        std::cout << std::left << std::setw(20) << "Priority"
                  << "| " << std::setw(9) << std::fixed << std::setprecision(1) << prioW
                  << "| " << std::setw(14) << prioT << "\n";

        std::cout << std::left << std::setw(20) << "Deadline (EDF)"
                  << "| " << std::setw(9) << std::fixed << std::setprecision(1) << edfW
                  << "| " << std::setw(14) << edfT << "\n";

        std::cout << "\n";

    } else {
        std::cout << "Invalid choice. Please run again and select 1-4.\n";
        return 1;
    }

    std::cout << "Execution log saved to: data/execution_log.csv\n";
    std::cout << "Run 'python viz/gantt_chart.py' to generate a visual Gantt chart.\n";

    return 0;
}
