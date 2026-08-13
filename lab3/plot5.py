import pandas as pd
import matplotlib.pyplot as plt

# Read CSV file
data = pd.read_csv("schedule_results.csv")

schedules = ["Static", "Dynamic", "Guided", "Auto"]

# Create 2 x 2 layout
fig, axes = plt.subplots(
    2, 2,
    figsize=(14, 10)
)

# Flatten axes for easy iteration
axes = axes.flatten()

for index, schedule in enumerate(schedules):

    ax = axes[index]

    # Select scheduling strategy
    subset = data[data["Schedule"] == schedule]

    # Plot 1, 2, 3 and 4 threads
    for threads in sorted(subset["Threads"].unique()):

        thread_data = subset[
            subset["Threads"] == threads
        ]

        ax.plot(
            thread_data["Interval"],
            thread_data["Speedup"],
            marker="o",
            linewidth=2,
            markersize=5,
            label=f"{threads} Threads"
        )

    ax.set_title(
        f"{schedule} Scheduling",
        fontsize=13,
        fontweight="bold"
    )

    ax.set_xlabel("Input Interval")
    ax.set_ylabel("Speedup")

    ax.grid(
        True,
        linestyle="--",
        alpha=0.5
    )

    ax.legend(fontsize=8)

# Overall title
fig.suptitle(
    "OpenMP Integer Summation - Scheduling Strategies",
    fontsize=17,
    fontweight="bold"
)

# Adjust spacing
plt.tight_layout(
    rect=[0, 0, 1, 0.95]
)

# Save as ONE image
plt.savefig(
    "scheduling_speedup_2x2.png",
    dpi=300,
    bbox_inches="tight"
)

plt.show()