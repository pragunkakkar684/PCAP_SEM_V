import pandas as pd
import matplotlib.pyplot as plt

# Read results
data = pd.read_csv("sequential_search_results.csv")

# Plot speedup for each thread count
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
plt.title("OpenMP Sequential Search - Speedup vs Input Size")

plt.grid(True, linestyle="--", alpha=0.6)
plt.legend()

plt.tight_layout()

# Save graph
plt.savefig(
    "sequential_search_speedup.png",
    dpi=300
)

plt.show()