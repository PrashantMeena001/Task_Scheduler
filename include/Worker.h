#pragma once

// Worker.h
// Pulls tasks from a Scheduler one at a time and "executes" them sequentially.
// Single-threaded by design: simplicity here keeps the Strategy pattern and
// custom heap (the core OOP/DSA focus of this project) front and center.
// TODO: declare Worker class -- e.g. run(Scheduler&, Logger&)
