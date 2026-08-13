import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("selection_sort_results.csv")

for threads in sorted(data["Threads"].unique()):
    subset = data[data["Threads"] == threads]

    plt.plot(
        subset["InputSize"],
        subset["Speedup"],
        marker="o",
        linewidth=2,
        label=f"{threads} Threads"
    )

plt.xlabel("Input Size")
plt.ylabel("Speedup")
plt.title("OpenMP Selection Sort - Speedup vs Input Size")
plt.grid(True, linestyle="--", alpha=0.6)
plt.legend()
plt.tight_layout()

plt.savefig("selection_sort_speedup.png", dpi=300)
plt.show()