import sys, os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

def ci95(series):
    x = np.array(series, dtype=float)
    if len(x) <= 1:
        return 0.0
    return 1.96 * x.std(ddof=1) / np.sqrt(len(x))

def plot_case(csv_path):
    df = pd.read_csv(csv_path, comment="#")
    # group by impl,threads
    groups = df.groupby(['impl','threads'])['elapsed_ms']
    stats = groups.agg(['mean','std','count']).reset_index()
    stats['ci95'] = groups.apply(ci95).values

    threads = sorted(stats['threads'].unique())
    impls = ['serial','mutex','rwlock']

    plt.figure()
    for impl in impls:
        sub = stats[stats['impl']==impl].sort_values('threads')
        if sub.empty: continue
        plt.errorbar(sub['threads'], sub['mean'], yerr=sub['ci95'], marker='o', label=impl)

    plt.title(f"Average Execution Time vs Threads\n{os.path.basename(csv_path)}")
    plt.xlabel("Threads")
    plt.ylabel("Time (ms)")
    plt.grid(True, alpha=0.3)
    plt.legend()
    out_png = csv_path.replace('.csv','.png')
    plt.savefig(out_png, bbox_inches='tight', dpi=150)
    print("Saved plot:", out_png)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python tools/plot.py results/case1.csv [results/case2.csv ...]")
        sys.exit(1)
    for path in sys.argv[1:]:
        plot_case(path)
