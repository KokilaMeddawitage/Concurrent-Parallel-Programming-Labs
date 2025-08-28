CS4532 – Concurrent Programming (Take-Home Lab 1)
=================================================

Project: Sorted singly linked list with Member / Insert / Delete under three implementations:
1) Serial (baseline)
2) Pthreads with a single global mutex
3) Pthreads with a single global read–write lock

Only the m-operations section is timed.

-------------------------------------------------
1) Tools & Environment
-------------------------------------------------
- OS: Windows 11 Home 64-bit
- CPU: AMD Ryzen 5 5500U (6 physical cores / 12 threads), 2.10 GHz
- Memory: 16 GB
- Editor/IDE: Visual Studio Code
- Compiler/Toolchain: MinGW-w64 (GCC) via MSYS2
- Build system: make (Makefile)
- Libraries / headers: POSIX Threads (<pthread.h>), timing via <time.h> / <sys/time.h>
- Terminal used: MSYS / MSYS2 shell (works), MinGW x64 shell recommended

Tip: If your path contains spaces or an '&', quote it exactly like this:
  cd "/c/Users/Dell/Documents/Sem 7/Parallel&Concurrent/Labs/Concurrent-Parallel-Programming-Labs/lab1_210382H_210449V"

-------------------------------------------------
2) Build
-------------------------------------------------
From the project root (folder containing Makefile):
  make

Expected binaries (Windows):
  serial.exe
  mutex.exe
  rwlock.exe

Clean build artifacts:
  make clean

-------------------------------------------------
3) Usage
-------------------------------------------------
Syntax
  Serial:
      ./serial  n m p_member p_insert p_delete

  Mutex:
      ./mutex   n m p_member p_insert p_delete threads

  RW-Lock:
      ./rwlock  n m p_member p_insert p_delete threads

Arguments
  n         – initial unique elements in list (e.g., 1000)
  m         – total number of operations (e.g., 10000)
  p_member  – fraction of Member ops (0–1)
  p_insert  – fraction of Insert ops (0–1)
  p_delete  – fraction of Delete ops (0–1)
  threads   – (pthread versions) number of worker threads (e.g., 1, 2, 4, 8)

Output
  Each run prints a single line:
    elapsed_ms,<value>
  which is the time (milliseconds) for the m-operations only (initial population and setup are excluded).

-------------------------------------------------
4) Example runs (match assignment cases)
-------------------------------------------------
Case 1
  n=1000, m=10000, p_member=0.99, p_insert=0.005, p_delete=0.005
    ./serial 1000 10000 0.99 0.005 0.005
    ./mutex  1000 10000 0.99 0.005 0.005 4
    ./rwlock 1000 10000 0.99 0.005 0.005 8

Case 2
  n=1000, m=10000, p_member=0.90, p_insert=0.05, p_delete=0.05

Case 3
  n=1000, m=10000, p_member=0.50, p_insert=0.25, p_delete=0.25

-------------------------------------------------
5) Automated experiments (CSV + plots)
-------------------------------------------------
A helper script is provided to run all three cases across thread counts and collect CSVs.

  chmod +x scripts/run_matrix.sh
  ./scripts/run_matrix.sh

This produces:
  results/case1.csv
  results/case2.csv
  results/case3.csv

CSV columns:
  impl,threads,elapsed_ms

Plotting (optional)
If you have Python 3 with pandas and matplotlib, you can generate “Average time vs Threads” plots with 95% CI bars:
  python3 tools/plot.py results/case1.csv results/case2.csv results/case3.csv

Outputs:
  results/case1.png
  results/case2.png
  results/case3.png

-------------------------------------------------
6) Accuracy requirement (±5% at 95% CI)
-------------------------------------------------
For each (implementation, threads) cell, gather multiple trials until:
  1.96 * STDEV.S(samples) / SQRT(COUNT(samples))  ≤  0.05 * AVERAGE(samples)

You can:
- Re-run scripts/run_matrix.sh to append more rows, or
- Manually loop runs to add more samples to each CSV.

-------------------------------------------------
7) Notes & assumptions
-------------------------------------------------
- The linked list is sorted and stores unique keys in [0, 65535].
- Insert only inserts if the key doesn’t already exist; Delete only deletes if present.
- Member uses shared (read) access under RW-lock; Insert/Delete use exclusive (write) access.
- Only the m-ops section is timed; initialization, thread creation, and I/O are excluded.

-------------------------------------------------
8) Troubleshooting
-------------------------------------------------
- make: gcc: No such file or directory
  Use MSYS2 MinGW x64 terminal or ensure MinGW toolchain is installed and on PATH.

- Script line-ending issue ('bash\r')
  If you see '/usr/bin/env: ‘bash\r’: No such file...', fix CRLF:
    sed -i 's/\r$//' scripts/run_matrix.sh

- Quoting paths with spaces or '&'
  Always wrap the path in double quotes:
    cd "/c/.../Sem 7/Parallel&Concurrent/.../lab1_210382H_210449V"

-------------------------------------------------
9) File list (key items)
-------------------------------------------------
Makefile
src/
  serial_list.c
  pthread_mutex_list.c
  pthread_rwlock_list.c
  util.c
  common.h
scripts/
  run_matrix.sh
tools/
  plot.py
results/
  machine_specs.txt
  case1.csv / case2.csv / case3.csv  (after running the script)
README.txt  (this file)
