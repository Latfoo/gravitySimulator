import pandas as pd
import matplotlib.pyplot as plt

csv_path = "docs/energy_errors.csv"
out_path = "docs/energy_errors.png"

df = pd.read_csv(csv_path)

fig, ax = plt.subplots(figsize=(10, 6))

# Replace exact zeros with NaN so they don't break the log scale
ax.plot(df["time_s"], df["explicit_euler"].replace(0, float("nan")),      label="Explicit Euler",      color="tab:red",    linewidth=1.5)
ax.plot(df["time_s"], df["semi_implicit_euler"].replace(0, float("nan")), label="Semi-implicit Euler", color="tab:orange", linewidth=1.5)
ax.plot(df["time_s"], df["leapfrog"].replace(0, float("nan")),            label="Leapfrog",            color="tab:blue",   linewidth=1.5)
ax.plot(df["time_s"], df["rk4"].replace(0, float("nan")),                 label="RK4",                 color="tab:green",  linewidth=1.5)

ax.set_yscale("log")
ax.set_xlabel("Simulated Time (s)")
ax.set_ylabel("Relative Energy Error |dE/E0|")
ax.set_title("Energy Conservation by Integrator")
ax.legend()
ax.grid(True, which="both", linestyle="--", alpha=0.4)

plt.tight_layout()
plt.savefig(out_path, dpi=150)
print(f"Saved {out_path}")
plt.show()
