#include "Task.h"
#include <sstream>

Task::Task()
    : id(0), priority(0), deadline(0), burstTime(0), arrivalTime(0),
      status(Status::PENDING)
{
}

Task::Task(int id, int priority, int deadline, int burstTime, int arrivalTime)
    : id(id), priority(priority), deadline(deadline), burstTime(burstTime),
      arrivalTime(arrivalTime), status(Status::PENDING)
{
}

std::string Task::toString() const {
    std::ostringstream oss;
    oss << "Task " << id
        << " [pri=" << priority
        << " dead=" << deadline
        << " burst=" << burstTime
        << " arr=" << arrivalTime
        << " status=";

    switch (status) {
        case Status::PENDING:   oss << "PENDING";   break;
        case Status::RUNNING:   oss << "RUNNING";   break;
        case Status::COMPLETED: oss << "COMPLETED"; break;
    }

    oss << "]";
    return oss.str();
}
