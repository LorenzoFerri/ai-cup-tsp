# AI Cup — Traveling Salesperson Solver

My C++ entry for the annual AI Cup at Università della Svizzera italiana (USI).
The competition began in late 2017 and the award ceremony took place in 2018,
when the project received **1st Place in the 12th Artificial Intelligence Cup**.

This is an archival release of university work, published in 2026.

## Approach

The solver constructs a random tour, improves it with 2-opt local search, then
uses simulated annealing with segment reversals and further 2-opt searches.
Distances are precomputed using rounded Euclidean distances. The annealing loop
uses a cooling schedule and a roughly 179-second CPU-time cutoff; this is not a
strict wall-clock limit for the whole program.

## Build and run

Requires a C++11-capable compiler, such as GCC.

```sh
g++ -std=c++11 -O2 tsp.cpp -o tsp
./tsp examples/square.tsp 42
```

The optional second argument is the random seed; otherwise the current time is
used. The program prints city IDs in tour order to standard output, and the seed,
CPU time, and total tour length to standard error. The return edge to the first
city is included in the length. The supplied square has an optimal length of 40.

To save the tour:

```sh
./tsp examples/square.tsp 42 > square.tour
```

Input uses the TSPLIB-style coordinate format shown in the example, with
consecutive city IDs starting at 1 and a `NODE_COORD_SECTION` line. The original
parser expects valid input with Unix line endings; this is not a general-purpose
TSPLIB parser. Distance calculation implements rounded Euclidean 2D distances.
Random sequences may differ between C library implementations.

## Historical provenance

The first commit preserves `tsp.cpp` byte for byte as recovered. Its author date
is the preserved file modification timestamp, **2017-11-07 21:18:11 +01:00**.
Its committer date records the actual import in 2026. There was no Git repository
for the project during the competition, so this history does not reconstruct
intermediate development commits.

The subsequent publication commit adds explicit standard-library includes
needed by current GCC, this README, `.gitignore`, and a synthetic example.
The optimization algorithm is unchanged. Old binaries, generated tour results,
the seed-search helper, and competition spreadsheets are not needed to run the
solver and are omitted. Original benchmark datasets are not redistributed.
