# Task Scheduler — Job Queue System

A **C++17 task scheduling simulator** that demonstrates core Object-Oriented Programming concepts including the **Strategy design pattern**, **polymorphism**, and a **hand-rolled MinHeap** data structure.

The system simulates a single CPU processing a batch of tasks under three different scheduling algorithms (FIFO, Priority, EDF), compares their performance, and produces both an ASCII Gantt chart in the terminal and a CSV log for visualization.

---

## What This Demonstrates

| Concept | Where It Appears |
|---|---|
| **Strategy Pattern** | `IScheduler` interface + `FIFOScheduler`, `PriorityScheduler`, `DeadlineScheduler` implementations, swapped at runtime via `Scheduler::setStrategy()` |
| **Custom MinHeap (DSA)** | `MinHeap<T>` — hand-rolled array-backed binary heap with `siftUp`/`siftDown`, used by Priority and Deadline schedulers |
| **Single Responsibility** | Each class does exactly one thing: `Task` holds data, `Logger` writes CSV, `Worker` simulates execution, `Scheduler` delegates |
| **Polymorphism** | `Scheduler` holds a `std::unique_ptr<IScheduler>` and calls virtual methods — the concrete type is determined at runtime |
| **Encapsulation** | Internal data structures (queue, heap) are private; external code interacts only through the `IScheduler` interface |

---

## Folder Structure

```
task-scheduler/
├── include/
│   ├── Task.h                  # Task data class
│   ├── MinHeap.h               # Template min-heap (header-only)
│   ├── IScheduler.h            # Strategy interface (pure virtual)
│   ├── FIFOScheduler.h         # FIFO strategy header
│   ├── PriorityScheduler.h     # Priority strategy header
│   ├── DeadlineScheduler.h     # EDF strategy header
│   ├── Scheduler.h             # Strategy context class
│   ├── Worker.h                # Simulated CPU
│   └── Logger.h                # CSV logger
├── src/
│   ├── Task.cpp
│   ├── FIFOScheduler.cpp
│   ├── PriorityScheduler.cpp
│   ├── DeadlineScheduler.cpp
│   ├── Scheduler.cpp
│   ├── Worker.cpp
│   ├── Logger.cpp
│   └── main.cpp                # CLI entry point
├── data/
│   └── .gitkeep                # Output CSV goes here
├── viz/
│   └── gantt_chart.py          # Python matplotlib visualization
├── CMakeLists.txt
├── .gitignore
└── README.md
```

---

## Build Instructions

**Prerequisites:** A C++17 compiler (g++ 7+, clang++ 5+, or MSVC 19.14+) and CMake 3.10+.

```bash
# 1. Configure the build
cmake -B build

# 2. Compile
cmake --build build

# 3. Run the scheduler
./build/task_scheduler
```

The program will display a menu — select a scheduling strategy (1–4) and watch the simulation run.

---

## Visualization 

After running the scheduler (which creates `data/execution_log.csv`):

```bash
# Install dependencies (if not already installed)
pip install pandas matplotlib

# Generate the Gantt chart
python viz/gantt_chart.py
```

This produces `viz/gantt_output.png` — a horizontal bar chart showing task execution timelines for each strategy.

---

## Class Responsibilities

| Class | Responsibility |
|---|---|
| `Task` | Holds all data about a single job (id, priority, deadline, burst time, arrival time, status) |
| `MinHeap<T>` | Array-backed binary min-heap with manual siftUp/siftDown — provides O(log n) insert and extract |
| `IScheduler` | Abstract interface defining the contract all scheduling strategies must follow |
| `FIFOScheduler` | Processes tasks in arrival order using a std::queue |
| `PriorityScheduler` | Processes the highest-priority task first using MinHeap with a priority comparator |
| `DeadlineScheduler` | Processes the earliest-deadline task first (EDF) using MinHeap with a deadline comparator |
| `Scheduler` | Strategy context — holds an IScheduler pointer and delegates all calls to it |
| `Worker` | Simulates a single CPU: pulls tasks, advances a simulated clock, computes metrics |
| `Logger` | Writes execution results to a CSV file for analysis and visualization |

---

## Why a Hand-Rolled Heap?

The C++ standard library provides `std::priority_queue` and heap utilities (`std::make_heap`, `std::push_heap`, `std::pop_heap`). So why write our own?

**Learning purpose:** This project is specifically designed to demonstrate understanding of the heap data structure at the implementation level.

### How it works:

A **binary heap** is a complete binary tree stored in a flat array. For a node at index `i`:
- **Parent:** `(i - 1) / 2`
- **Left child:** `2 * i + 1`
- **Right child:** `2 * i + 2`

Two key operations maintain the heap property (parent ≤ children for a min-heap):

1. **siftUp** — after inserting a new element at the end, compare it with its parent. If it's smaller, swap and repeat upward. This takes **O(log n)** because the tree height is log(n).

2. **siftDown** — after removing the root (minimum), move the last element to the root. Compare with children, swap with the smaller child if needed, repeat downward. Also **O(log n)**.

| Operation | Time Complexity |
|---|---|
| `insert()` | O(log n) |
| `extractMin()` | O(log n) |
| `peek()` | O(1) |
| `isEmpty()` | O(1) |
| `size()` | O(1) |

The `Comparator` template parameter lets us reuse the same heap code for different orderings (by priority, by deadline, etc.) without duplicating the siftUp/siftDown logic.

---

## Strategy Pattern Explained

Imagine you're at a restaurant. The kitchen (Worker) prepares dishes. But the order in which dishes are prepared depends on the ordering system:
- **FIFO:** First order placed = first order cooked (fair, but a VIP might wait)
- **Priority:** VIP orders go first, regardless of when they arrived
- **Deadline:** Orders closest to their pickup time go first

The **Strategy pattern** lets us swap the ordering system without changing the kitchen or anything else:

1. **IScheduler** (the interface) says: "Any ordering system must be able to add a task, get the next task, and tell us if there are more tasks."
2. **FIFOScheduler**, **PriorityScheduler**, **DeadlineScheduler** each implement that interface differently.
3. **Scheduler** (the context) holds a pointer to *any* IScheduler. It doesn't know or care which one — it just calls `getNextTask()`.
4. At runtime, calling `setStrategy()` swaps the algorithm. The Worker, Logger, and all other code continue working unchanged.

This is **polymorphism in action**: the same function call (`getNextTask()`) does completely different things depending on which object is behind the pointer. The decision is made at runtime, not compile time.

In an interview, you can explain: *"The Strategy pattern decouples the algorithm from the code that uses it. I can add a new scheduling strategy by creating one new class — I never touch Scheduler, Worker, or main."*

---

## What I Would Add Next

### Multithreading via WorkerPool

The current system uses a single `Worker` that processes tasks sequentially. The natural next step would be a `WorkerPool` class that manages multiple workers running concurrently:

- Each worker would be a `std::thread` pulling tasks from a shared `Scheduler`
- A `std::mutex` would protect the shared task queue from race conditions
- A `std::condition_variable` would let workers sleep when the queue is empty and wake up when new tasks arrive

**Why it was excluded:** Multithreading introduces concurrency concepts (mutexes, race conditions, deadlocks) that are beyond the scope of a second-year OOP course. The single-threaded design keeps the focus on OOP fundamentals (Strategy pattern, polymorphism, encapsulation) and DSA (the custom MinHeap).

### Other potential additions:
- **Task dependencies** — task B can't start until task A finishes (DAG scheduling)
- **Preemptive scheduling** — interrupt a running task when a higher-priority one arrives
- **Round Robin** — each task gets a fixed time slice, then goes to the back of the queue
- **Dynamic task arrival** — tasks arriving at different times during simulation rather than all loaded upfront

---

