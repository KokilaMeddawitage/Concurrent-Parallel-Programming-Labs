#!/usr/bin/env bash
set -euo pipefail

# Runs all cases for serial/mutex/rwlock and writes CSVs with multiple trials until you stop.
# You can post-process with tools/plot.py which expects CSV rows with elapsed_ms.

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

CASES=("1 1000 10000 0.99 0.005 0.005" "2 1000 10000 0.90 0.05 0.05" "3 1000 10000 0.50 0.25 0.25")
THREADS=(1 2 4 8)

echo "Building..."
make -s

trial_ts=$(date +%Y%m%d_%H%M%S)
for case in "${CASES[@]}"; do
  set -- $case
  cid=$1; n=$2; m=$3; pm=$4; pi=$5; pd=$6
  out="results/case${cid}.csv"
  echo "# case=${cid}, n=${n}, m=${m}, p_member=${pm}, p_insert=${pi}, p_delete=${pd}, trial=${trial_ts}" >> "$out"
  echo "impl,threads,elapsed_ms" >> "$out"
  echo "Running Case ${cid}..."

  # serial
  for t in 1; do
    for rep in {1..5}; do
      line=$(./serial "$n" "$m" "$pm" "$pi" "$pd")
      echo "serial,$t,${line#elapsed_ms,}" >> "$out"
    done
  done

  # mutex & rwlock
  for t in "${THREADS[@]}"; do
    for rep in {1..5}; do
      line=$(./mutex "$n" "$m" "$pm" "$pi" "$pd" "$t")
      echo "mutex,$t,${line#elapsed_ms,}" >> "$out"
      line=$(./rwlock "$n" "$m" "$pm" "$pi" "$pd" "$t")
      echo "rwlock,$t,${line#elapsed_ms,}" >> "$out"
    done
  done
  echo "Saved -> $out"
done

echo "Done."
