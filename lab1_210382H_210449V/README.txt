Lab 1 – Concurrent Programming (CS4532)

This package provides three programs implementing a sorted, singly linked list with operations
Member/Insert/Delete under three models: Serial, Single Mutex, and Read–Write Lock.

Build
-----
    make

Usage
-----
Serial:
    ./serial  n m p_member p_insert p_delete

Mutex:
    ./mutex   n m p_member p_insert p_delete threads

RW-Lock:
    ./rwlock  n m p_member p_insert p_delete threads

Arguments
---------
n           Initial unique elements in the list (e.g., 1000)
m           Total operations (e.g., 10000)
p_member    Fraction of Member operations (0.0–1.0)
p_insert    Fraction of Insert operations (0.0–1.0)
p_delete    Fraction of Delete operations (0.0–1.0)
threads     (pthread versions) number of worker threads (e.g., 1,2,4,8)

Notes
-----
- Only the m-operations section is timed (milliseconds printed as CSV row "elapsed_ms,<value>").
- Initial population uses unique keys in [0, 65535].
- Insert/Delete operations are safe under the chosen synchronization model.
- For reproducibility, you may set your own seed by modifying the source (default uses time + address mix).

Examples (match assignment cases)
---------------------------------
Case 1: n=1000, m=10000, p_member=0.99, p_insert=0.005, p_delete=0.005
    ./serial 1000 10000 0.99 0.005 0.005
    ./mutex  1000 10000 0.99 0.005 0.005 4
    ./rwlock 1000 10000 0.99 0.005 0.005 8

Case 2: n=1000, m=10000, p_member=0.90, p_insert=0.05, p_delete=0.05
Case 3: n=1000, m=10000, p_member=0.50, p_insert=0.25, p_delete=0.25

Automation (optional)
---------------------
See scripts/run_matrix.sh to loop cases × {1,2,4,8}. CSV results are written to results/.
Then use tools/plot.py to generate “Average time vs Threads” plots with 95% CI bars.

Machine Spec
------------
Please add your CPU model, core/thread count, RAM, OS, and any frequency governor/turbo notes
to results/machine_specs.txt as required by the lab sheet.
