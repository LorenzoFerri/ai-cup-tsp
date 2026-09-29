# AI Cup — Traveling Salesperson Solver

C++ entry for the annual AI Cup at Università della Svizzera italiana (USI).
Developed in late 2017; awarded **1st Place at the 12th AI Cup in 2018**.

The solver combines **2-opt local search** and **simulated annealing** to find
short tours through the cities in a TSPLIB coordinate file.

## Build

```sh
g++ -std=c++11 -O2 tsp.cpp -o tsp
g++ -std=c++11 -O2 seed.cpp -o seed
```

## Run

```sh
./tsp eil76.tsp 42 > eil76.tsp.tour
```

The seed (`42`) is optional. The tour is written to standard output; its length,
CPU time and random seed are written to standard error.

To search repeatedly with different seeds:

```sh
./seed eil76.tsp
```

Run from the repository directory using the supplied filenames. Stop with
**Ctrl+C**. The helper saves the latest tour in `.tour`, the latest run statistics
in `.tour.last`, and the best statistics in `.tour.best`.

## Files

- `tsp.cpp`: TSP solver.
- `seed.cpp`: repeated-run helper for seed search.
- `*.tsp`: the ten TSPLIB benchmark instances used with the project.
- `examples/square.tsp`: a small example with optimal tour length 40.

Imported from the original files in 2026, with minor build and seed-helper fixes.
