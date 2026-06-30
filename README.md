# Task Scheduler / Job Queue System (C++)

A C++17 task scheduling simulator built to demonstrate core Object-Oriented Programming
concepts — encapsulation, inheritance, polymorphism, and the **Strategy design pattern**
— layered on top of a hand-rolled **binary min-heap** data structure.

## What this project demonstrates

- **Strategy Pattern** — `Scheduler` is a context class that holds a pointer to an
  `IScheduler` interface. Three interchangeable strategies (`FIFOScheduler`,
  `PriorityScheduler`, `DeadlineScheduler`) can be swapped into the same `Scheduler`
  object at runtime, producing different task execution orders from identical input.
- **Custom Data Structure** — `MinHeap<T>` is a hand-written, array-backed binary heap
  (not `std::priority_queue`) with `O(log n)` insert/extract, used by the priority- and
  deadline-based strategies.
- **Single Responsibility** — `Logger` is solely responsible for writing execution
  records to disk; `Worker` is solely responsible for executing tasks pulled from the
  scheduler; no class does more than one job.

The system is intentionally **single-threaded**: a `Worker` pulls and processes one task
at a time, in the order the active strategy produces. This keeps the project's real
focus -- the Strategy pattern and the custom heap -- clear and fully explainable, rather
than diluting it with concurrency that isn't the point being demonstrated.

## Folder structure

```
task-scheduler/
├── include/              # Header files (class declarations)
│   ├── Task.h
│   ├── MinHeap.h
│   ├── IScheduler.h
│   ├── FIFOScheduler.h
│   ├── PriorityScheduler.h
│   ├── DeadlineScheduler.h
│   ├── Scheduler.h
│   ├── Worker.h
│   └── Logger.h
├── src/                  # Implementation files
│   ├── Task.cpp
│   ├── FIFOScheduler.cpp
│   ├── PriorityScheduler.cpp
│   ├── DeadlineScheduler.cpp
│   ├── Scheduler.cpp
│   ├── Worker.cpp
│   ├── Logger.cpp
│   └── main.cpp
├── data/                 # Runtime-generated execution_log.csv lands here
├── viz/                  # Optional, decoupled Python visualization
│   └── gantt_chart.py
├── CMakeLists.txt
├── .gitignore
└── README.md
```

## Build instructions

```bash
cmake -B build
cmake --build build
./build/task_scheduler
```

Requires a C++17-capable compiler (g++ 9+ / clang 10+) and CMake 3.10+. No external
dependencies beyond the standard library.

## Class responsibilities (interview cheat sheet)

| Class | Responsibility |
|---|---|
| `Task` | Plain data holder: id, priority, deadline, burst time, arrival time, status. No scheduling logic. |
| `MinHeap<T>` | Generic array-backed binary min-heap with `insert`/`extractMin`. Reused by priority and deadline strategies via a comparator. |
| `IScheduler` | Abstract interface defining the scheduling contract (`addTask`, `getNextTask`, `hasNext`). |
| `FIFOScheduler` | Concrete strategy — simple queue, first-come-first-served. |
| `PriorityScheduler` | Concrete strategy — `MinHeap` ordered by task priority. |
| `DeadlineScheduler` | Concrete strategy — `MinHeap` ordered by deadline (Earliest Deadline First). |
| `Scheduler` | Strategy *context* — holds the active `IScheduler`, delegates calls to it, swappable at runtime. |
| `Worker` | Pulls one task at a time from the `Scheduler` and processes it sequentially. |
| `Logger` | Writes execution records (task id, strategy, timings) to `data/execution_log.csv`. |

## Why the hand-rolled heap, not `std::priority_queue`

The point of this project is to show I understand the structure underneath, not just
the STL container. `MinHeap<T>` implements `siftUp`/`siftDown` manually — `O(log n)`
insert and extract, `O(1)` peek, `O(n)` heapify on construction.

## Optional visualization (`viz/gantt_chart.py`)

A small, fully decoupled Python script (pandas + matplotlib) that reads
`data/execution_log.csv` after a run and renders a Gantt-style chart comparing
strategies. It does not affect, call into, or get called by the C++ program — it's a
reporting layer only, used purely for the demo, not a requirement to understand or run
the core system.

