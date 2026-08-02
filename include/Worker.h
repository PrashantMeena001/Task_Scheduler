#pragma once

#include "Scheduler.h"
#include "Logger.h"

// Worker simulating a single CPU processing tasks.

class Worker {
public:
    // Process all tasks in the scheduler.
    void run(Scheduler& scheduler, Logger& logger);
};
