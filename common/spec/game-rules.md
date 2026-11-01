# Game rules — the invariant

> [English](game-rules.md) | [Español](game-rules-es.md)

This document is the single source of truth for the game behavior. Every implementation, trace and
lesson must match it.

The historical archive is evidence. This specification defines the deterministic reference used by
the laboratory.

## 1. Board

- The board is an 8×8 grid.
- Rows are numbered 1 to 8 from top to bottom.
- Columns are numbered 1 to 8 from left to right.
- A cell is written `row,column`, for example `3,5`.

## 2. Pieces

There are two knights:

- `player` — the learner's knight.
- `machine` — the machine's knight.

Each knight occupies exactly one cell.

## 3. Starting positions

Given a seed:

1. Draw `player_start` uniformly from 0 to 63.
2. Draw `machine_start` uniformly from 0 to 63.
3. If the positions are equal, redraw `machine_start` until they differ.
4. Convert a number `n` to `row = n // 8 + 1` and `column = n % 8 + 1`.

The starting cells are occupied but are not marked as visited.

## 4. Who starts

1. Draw `roll_player` uniformly from 1 to 50.
2. Draw `roll_machine` uniformly from 1 to 50.
3. If the rolls are equal, redraw `roll_machine` until they differ.
4. The knight with the higher roll starts.

## 5. Turn order

On each turn, the active knight is the one whose turn it is. After a successful move, the turn
passes to the other knight.

A capture or a trapped knight ends the game immediately.

## 6. Move priority

On each turn, before trying moves:

1. Draw `start` uniformly from 1 to 8.
2. The move order is `start, start + 1, …, 8, 1, …, start - 1`.

The active knight tries the moves in that order and takes the first legal move.

## 7. Knight moves

| Move | Row change | Column change | Description |
|---|---|---|---|
| 1 | -1 | +2 | One row up, two columns right |
| 2 | -2 | +1 | Two rows up, one column right |
| 3 | -2 | -1 | Two rows up, one column left |
| 4 | -1 | -2 | One row up, two columns left |
| 5 | +1 | -2 | One row down, two columns left |
| 6 | +2 | -1 | Two rows down, one column left |
| 7 | +2 | +1 | Two rows down, one column right |
| 8 | +1 | +2 | One row down, two columns right |

A move is legal when:

- the destination is inside the board;
- the destination is not a cell already visited by either knight.

The one exception is the other knight's current cell: entering it is a capture and is legal.

## 8. Visited cells

- A cell becomes visited when a knight enters it.
- Starting cells are not visited until a knight enters them later.
- A knight may enter its own starting cell if it is not otherwise visited.
- A knight may not enter any visited cell except to capture the other knight.

## 9. Capture

If the active knight enters the other knight's current cell, the active knight wins immediately.

## 10. Trap

If the active knight has no legal move, the active knight loses and the other knight wins.

## 11. Draw

If 200 successful moves have been played and no knight has won, the game is a draw.

## 12. Randomness

The reference uses a 64-bit linear congruential generator. The algorithm is small enough to
implement in any language.

State:

```text
state = seed mod 2^64
```

Constants:

```text
multiplier = 6364136223846793005
increment  = 1442695040888963407
modulus    = 2^64
```

Operations:

```text
next_value():
    state = (state * multiplier + increment) mod 2^64
    return state

random_below(bound):
    return next_value() mod bound

random_integer(minimum, maximum):
    return minimum + random_below(maximum - minimum + 1)
```

Call order:

1. `player_start = random_below(64)`
2. `machine_start = random_below(64)`, redrawn until different from `player_start`
3. `roll_player = random_integer(1, 50)`
4. `roll_machine = random_integer(1, 50)`, redrawn until different from `roll_player`
5. For each turn: `start = random_integer(1, 8)`

No other random call is allowed between these steps. The modulo operation is part of the
specification; implementations must not add rejection sampling.

## 13. Transcript

Every run produces a transcript in the format defined by
[`transcript-format.md`](transcript-format.md).

The transcript is the behavioral contract. Two implementations agree when they produce the same
transcript for the same seed and player name.

## 14. Reference implementation

The reference implementation is:

```text
common/harness/reference.py
```

If the implementation and this specification disagree, the specification wins and the implementation
is corrected.
