import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("vector_task_results.csv")

plt.figure(figsize=(10, 6))

for threads in sorted(data["Threads"].unique()):
    subset = data[data["Threads"] == threads]

    plt.plot(
        subset["InputSize"],
        subset["Speedup"],
        marker="o",
        linewidth=2,
        markersize=6,
        label=f"{threads} Threads"
    )

plt.xlabel("Input Size", fontsize=12)
plt.ylabel("Speedup", fontsize=12)

plt.title(
    "OpenMP Task-Level Parallelism\n"
    "Vector Addition, Subtraction and Multiplication",
    fontsize=14
)

plt.grid(True, linestyle="--", alpha=0.6)
plt.legend(title="Number of Threads")

plt.tight_layout()

plt.savefig(
    "vector_task_speedup.png",
    dpi=300,
    bbox_inches="tight"
)

plt.show()