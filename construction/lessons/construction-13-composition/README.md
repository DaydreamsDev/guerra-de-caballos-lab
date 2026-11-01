# construction-13 — Composition baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** composition over inheritance · **Prerequisite:**
> construction-12 · **Estimated time:** 60 minutes

This directory holds the **after** artifact for `construction-13`. It has the same observable
behavior as `construction-12`, but the knight is composed of a `Position` object instead of
inheriting from a `Piece` interface.

## What the program implements

- The exact 64-bit linear congruential generator from `common/spec/game-rules.md`.
- The 8×8 board, both knights, starting positions and starting rolls.
- Move priority, legal and illegal moves, visited cells, capture, trap and the 200-move draw.
- The transcript format from `common/spec/transcript-format.md`, byte for byte.

The program accepts `--seed INTEGER --name TEXT` in either order and writes the transcript to
standard output.

## Files

| File | Purpose |
|---|---|
| `code/main.cpp` | Parses the command line and calls `run_game`. |
| `code/game.h` | Public game interface. |
| `code/game.cpp` | Game rules, turn loop and transcript. |
| `code/knight.h` | `Knight` with a `Position` member and delegated operations. |
| `code/knight.cpp` | Delegation to `Position` and role name. |
| `code/position.h` | `Position` value type. |
| `code/position.cpp` | Row and column state. |
| `code/board.h` | `Board` class with private invariant. |
| `code/board.cpp` | Board representation and operations. |
| `code/random_sequence.h` | Public random sequence interface. |
| `code/random_sequence.cpp` | Hidden generator state. |
| `CMakeLists.txt` | C++20 build with `-Wall -Wextra -Werror`. |
| `run.sh` | Compiles when needed and runs the binary. |

## The composition

`Knight` contains a `Position`:

```cpp
class Knight
{
private:
    Position position;
    std::string roleName;
};
```

`Knight::row()` delegates to `position.row()`, `Knight::column()` to `position.column()`, and
`Knight::set_position()` to `position.set()`. There is no `Piece` base class and no virtual
function.

## Build and run

```bash
./run.sh --seed 17 --name Learner
```

`run.sh` uses CMake when it is installed. When CMake is not available it compiles all translation
units directly with the compiler named by `CXX`, or `c++` by default.

## Verify the golden traces

From this directory:

```bash
for trace in ../../../common/traces/seed-*.txt; do
    traceName="$(basename "${trace}" .txt)"
    seedText="${traceName#seed-}"
    seedNumber="$((10#${seedText}))"
    ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

All 20 traces from `seed-0001.txt` through `seed-0020.txt` match byte for byte.

## Change test

In a scratch copy, replace the `Piece` inheritance from `construction-12` with a `Knight` that owns
a `Position`. Keep the game behavior. Count the inheritance-specific code removed and the delegation
methods added. All 20 traces must still pass.

## Acceptance criterion

The learner can replace inheritance with delegation and show that the game behavior and all 20
traces remain unchanged.
