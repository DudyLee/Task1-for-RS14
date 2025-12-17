# Sorting Experiment Utilities

This project provides a simple experimental harness around a basic comparison-based sort implemented in [`sort.c`](./sort.c). The program reads integers from an input file, sorts them, and records statistics and timing information.

## Algorithms Included
- Naive comparison-based sorting (nested-loop swap). No additional algorithms are implemented.

## Usage Setup
1. Ensure a C compiler (e.g., `gcc`) is available.
2. Place a configuration file named `config.txt` and the referenced data file in the project root (see format details below).

## Compilation Instructions
```bash
gcc -O2 -o sort sort.c
```
This produces the executable `sort` in the project root.

## Execution Instructions
Run the compiled binary from the project root so it can locate `config.txt` and the data file specified inside it:
```bash
./sort
```
The program reports progress to stdout and writes results to output files.

## Input Files
- `config.txt`: Defines the data source and runtime parameters.
- `data.txt` (or another filename referenced by `config.txt`): Contains the integers to sort.

## Output Files
- `output.txt`: Sorted integers written in ascending order on one line with spaces.
- `stat.txt`: Single integer indicating how many swaps occurred during sorting.
- `time.txt`: Four lines of user and system time (seconds) for reading, sorting, writing, and overall execution. (Only populated on non-Windows systems.)

## Appendix

### Format of Each Input File
- **`config.txt`** (five lines):
  1. Data filename (e.g., `data.txt`).
  2. Number of integers to read from the data file.
  3. Integer parameter #2 (unused by `sort.c`, kept for compatibility with the original experiment setup).
  4. Integer parameter #3 (unused by `sort.c`).
  5. Floating-point parameter #4 (unused by `sort.c`).

- **Data file** (single line):
  - Space-separated integers. The program reads exactly the count specified in `config.txt`.

### Format of Each Output File
- **`output.txt`**: One line of space-separated integers sorted in ascending order, followed by a trailing newline.
- **`stat.txt`**: One line containing the number of swaps performed during sorting.
- **`time.txt`** (non-Windows only): Four lines, each with two floating-point numbers (`user_time system_time`):
  1. Reading time.
  2. Sorting time.
  3. Writing time.
  4. Overall execution time.
