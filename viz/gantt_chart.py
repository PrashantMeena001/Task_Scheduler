#!/usr/bin/env python3
"""
Gantt Chart Visualization for the Task Scheduler project.

Usage:
    1. Run the C++ scheduler first (./build/task_scheduler) to generate data/execution_log.csv
    2. Then run: python viz/gantt_chart.py
    3. Output is saved to viz/gantt_output.png and displayed on screen.

Requirements: pandas, matplotlib
    pip install pandas matplotlib
"""

import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

# Read the execution log produced by the C++ scheduler
df = pd.read_csv("data/execution_log.csv")

# Get unique strategies in the order they appear in the log
strategies = df["strategyName"].unique()

# Color palette — one distinct color per task ID
colors = plt.cm.Set3.colors  # 12 qualitative colors
task_ids = sorted(df["taskId"].unique())
color_map = {tid: colors[i % len(colors)] for i, tid in enumerate(task_ids)}

# Create vertically stacked subplots, one per strategy
fig, axes = plt.subplots(len(strategies), 1, figsize=(12, 3.5 * len(strategies)),
                         sharex=True)

# Handle the case where there's only one strategy (axes is not a list)
if len(strategies) == 1:
    axes = [axes]

for ax, strat in zip(axes, strategies):
    subset = df[df["strategyName"] == strat]

    # Draw one horizontal bar per task
    for _, row in subset.iterrows():
        duration = row["endTime"] - row["startTime"]
        ax.barh(
            y=f"Task {row['taskId']}",
            width=duration,
            left=row["startTime"],
            color=color_map[row["taskId"]],
            edgecolor="black",
            linewidth=0.8,
            height=0.6
        )

    ax.set_title(strat, fontsize=13, fontweight="bold")
    ax.set_ylabel("Tasks")
    ax.invert_yaxis()  # Task 1 at the top
    ax.grid(axis="x", linestyle="--", alpha=0.5)

# Shared x-axis label
axes[-1].set_xlabel("Simulated Time Units")

# Legend
patches = [mpatches.Patch(color=color_map[tid], label=f"Task {tid}") for tid in task_ids]
fig.legend(handles=patches, loc="upper right", fontsize=9, title="Tasks")

plt.tight_layout(rect=[0, 0, 0.88, 1])  # leave room for legend
plt.savefig("viz/gantt_output.png", dpi=150, bbox_inches="tight")
print("Gantt chart saved to viz/gantt_output.png")
plt.show()
