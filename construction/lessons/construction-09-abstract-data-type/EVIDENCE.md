# construction-09 — Evidence
> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record compares a board behind five free functions with a board behind seven operations on an
opaque handle, and measures one representation swap. Keep both sides. A tie in file count is still a
result if the things that become possible differ.

## 1. Run setup

- Date:
- Seed: `1`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- ADT baseline read on (date):
- Modular baseline read on (date):
- Construction-08 record at hand? (yes / no)
- The number I carried from `construction-08` (lines / traces / translation units):

## 2. The incomplete type (before the run)

### 2.1 Headers and declarations

Fill this from the source, with line numbers, not from memory.

| Header | Lines | Declarations | Incomplete type | Definition in |
|---|---|---|---|---|
| | | | | |

Counts:

- Headers:
- Translation units:
- Public declarations:
- Incomplete types:
- Function definitions, total:
- Internal helpers (`game.cpp`):
- Internal helpers (`main.cpp`):
- Program entry points:

### 2.2 Where the definition lives

- Line of the forward declaration in `board.h`:
- Line of the definition in `board.cpp`:
- What is inside `struct Board`:
- The one `#include` that is missing from `game.cpp`, and what it would reveal:

### 2.3 Include graph

Paste the output of `g++ -std=c++20 -Icode -MM` for each `.cpp`, or fill the table.

| Object | Direct includes |
|---|---|
| `main.o` | |
| `game.o` | |
| `board.o` | |
| `random_sequence.o` | |

- Which translation unit includes the most project headers:
- Which project header does `game.cpp` include for the board:
- Which project header does `main.cpp` include:

### 2.4 What the header cannot express

Mark `yes` or `no` from the source, before compiling.

| Expression | Legal in `game.cpp`? | Why |
|---|---|---|
| `Board *board;` | | |
| `Board board;` | | |
| `sizeof(Board)` | | |
| `board->visited` | | |
| `delete board;` | | |
| `Board *copy = board;` | | |

- The operation that surprised me, and why:
- The name I expected to be visible and was not:

## 3. Prediction (written before measuring)

### 3.1 The traces

The change: none. Both baselines implement the same game.

| Question | My prediction | Confidence | Reason |
|---|---|---|---|
| ADT vs `seed-0001.txt` | | | |
| Modular vs `seed-0001.txt` | | | |
| ADT vs modular, seed 1 | | | |
| Seeds that match, of 20, on each baseline | | | |
| First line that could differ | | | |

- The result I expect to miss, and why:

### 3.2 The swap surface

The change: replace the fixed array in `board.cpp` with a flat vector and an index function.

| Question | My prediction |
|---|---|
| Source files edited | |
| Lines edited | |
| Headers edited | |
| `.cpp` files edited outside `board.cpp` | |
| Objects recompiled, of 4 | |
| Does it compile without other edits? | |
| Seeds that match, of 20 | |
| Does the object file grow, shrink or stay the same? | |

- The number I expect to miss, and why:

### 3.3 Header visibility

Copy your prediction from §2.4 here, or write it here first and compare later.

| Expression | `main.cpp` | `game.cpp` | `board.cpp` |
|---|---|---|---|
| `Board *board = board_create();` | | | |
| `sizeof(Board)` | | | |
| `board->visited` | | | |
| `Board board;` | | | |
| `delete board;` | | | |
| `board_was_visited(board, 1, 1)` | | | |
| `Board *copy = board;` | | | |

- The unit that knows the most names, and why:
- The row that is `no` for a different reason than the others, and why:

## 4. Run — both baselines

- ADT build command:
- Modular build command:
- Exit statuses:
- Diff target: `../../../common/traces/seed-0001.txt`

Record `empty` or `diff` in each result cell.

| Check | Command | Result |
|---|---|---|
| ADT vs golden | | |
| Modular vs golden | | |
| ADT vs modular | | |

```text
<diff output, or "no differences">
```

Seeds that matched, of 20, on the ADT baseline:

Seeds that matched, of 20, on the modular baseline:

If any diff is not empty, stop here and fill §11.

## 5. Prove the type is incomplete

### 5.1 The probe

- Probe source path (scratch only):
- Compile command:
- Compile exit status:
- Number of diagnostics:

| Forbidden operation | Diagnostic class | Compiler's note about the declaration |
|---|---|---|
| `sizeof(Board)` | | |
| `board->visited` | | |
| `Board board;` | | |
| `delete board;` | | |

- What the compiler refused to compute, in one sentence:
- Why the refusal is the abstraction, and not a limitation:

### 5.2 Object symbols

- Object build command:
- `nm` command:
- Platform note (any symbol the command could not show):

| Header | Declarations | Object | External symbols | Layout in the symbol name? |
|---|---|---|---|---|
| `board.h` | 7 | `board.o` | | |
| `game.h` | 1 | `game.o` | | |
| `random_sequence.h` | 3 | `random_sequence.o` | | |

- The seven board symbols, in full:
- What `P5Board` encodes:
- What `K` encodes:
- What `v` encoded in the `construction-08` symbols:

### 5.3 Symbol names against layout

- Do the mangled names change when the representation changes? (yes / no)
- Why that matters for the linker:
- The sentence I will keep about a name and a size:

## 6. Swap the representation

- Scratch copy (flat):
- Scratch copy (nested):
- Files edited:
- Lines edited, flat:
- Lines edited, nested:
- Headers edited:
- I did not edit any tracked code file: (yes / no)

### 6.1 Flat vector with an index function

- Struct definition:
- Index function:
- What `assign` replaced:
- Build command:
- Exit status:
- Seeds that matched, of 20:

### 6.2 Vector of vectors

- Struct definition:
- What changed in `board_reset`:
- Build command:
- Exit status:
- Seeds that matched, of 20:

### 6.3 Behavior and cost

- The first line that differs, if any:
- Why no line should differ:
- Text size, array:
- Text size, flat vector:
- Text size, vector of vectors:
- Are the exported symbols identical across the three? (yes / no)
- What the abstraction costs at run time:
- What the abstraction does not hide from the program:

### 6.4 File-by-file

| File | Edited? | Recompiled? |
|---|---|---|
| `code/board.cpp` | | |
| `code/board.h` | | |
| `code/game.cpp` | | |
| `code/game.h` | | |
| `code/main.cpp` | | |
| `code/random_sequence.cpp` | | |
| `code/random_sequence.h` | | |

- The count of edited source files:
- The count of recompiled objects:
- The count of files that needed no edit:
- Why `game.cpp` needed no edit:

### 6.5 The initialization the traces cannot check

- What I removed from `board_create`:
- Seeds that still matched, of 20:
- The call order in `run_game` that hides it:
- What the array version did differently:
- Why this is undefined behavior and not a failed trace:
- Which later lesson closes this gap:

## 7. Compare the change surface

| Metric | Modular baseline | ADT baseline |
|---|---|---|
| Files touched | | |
| Lines changed | | |
| Signatures changed | | |
| New concepts required | | |
| Translation units recompiled | | |
| 8×8 traces broken | | |
| Boards alive at once | | |
| Board lifetime | | |
| Operations that take a board | | |
| A test can build a fresh board | | |

- Which rows tie:
- Which rows separate the two designs:
- What the modular baseline could already do, and what it could not:
- What the ADT bought:
- What the ADT cost:
- The line count of the migration itself (`diff ../construction-08-modular/code/game.cpp`):
- Which later change would separate the two designs more clearly, and why:

## 8. Explain

One sentence, without the words *header*, *file*, *module*, *compile*, *pointer*, *struct*,
*class*, *opaque*, *encapsulation*, *better*, *worse*, *clean*, *cleaner*, *nice*, *elegant*,
*ugly* or *modular*:

>

One paragraph:

- What the declaration fixes:
- What it withholds:
- What the compiler enforces:
- What the linker sees:
- What a run proves:
- What a run does not prove:
- What multiplicity adds:
- What it costs:
- Which later lesson adds invariants:
- Which later lesson adds encapsulation:
- Which later lesson owns lifetime:
- Which later lesson owns ports and adapters:

The four things I kept apart:

## 9. Acceptance check

- I ran both unmodified baselines: (yes / no)
- I got three empty diffs and 20 matching seeds on each: (yes / no)
- I inventoried 11 public declarations and found the one incomplete type: (yes / no)
- I compiled a probe that fails on `sizeof`, member access and `delete`: (yes / no)
- I listed the seven board symbols and read their mangled names: (yes / no)
- I explained why no symbol name encodes the layout: (yes / no)
- I predicted the traces and the swap surface before measuring: (yes / no)
- I replaced the array with a flat vector: (yes / no)
- I replaced it again with a vector of vectors: (yes / no)
- `board.h`, `game.cpp` and `main.cpp` required no edits: (yes / no)
- Only `board.o` recompiled, then the link: (yes / no)
- I got 20 of 20 before the swap and 20 of 20 after: (yes / no)
- I recorded the object size for all three representations: (yes / no)
- I removed the initialization from `board_create` and explained the result: (yes / no)
- I measured the change-surface cost on both baselines: (yes / no)
- I named the rows that tie and the rows that separate: (yes / no)
- I explained what the declaration permits and what it hides: (yes / no)
- My sentence avoids the six syntax words and the eight verdict words: (yes / no)
- I did not edit any tracked code file: (yes / no)

If I edited a tracked code file, what and why:

## 10. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-10`:
- The check I trust most: the trace, the object symbols, or the probe diagnostics:
- The check that surprised me:
- What the header hid that `construction-08` still showed:
- What the modular baseline kept together that the ADT separates:
- What the swap proved, and what it did not:
- The change I would make next to test the boundary again:
- Next step:

## 11. If a baseline is missing or unfaithful

Fill this only if a file was absent, a build failed for an environmental reason, or a diff was not
empty.

- Missing file, build error, or diff:
- Exact command output:
- What I did instead of inventing a count:
- When I will retry:

## 12. Self-check

- [ ] I inventoried every header and every definition with its file.
- [ ] I located the forward declaration and the definition of `Board` in different files.
- [ ] I wrote the include graph from the compiler, not from memory.
- [ ] I predicted the traces, the swap surface and the visibility table before measuring.
- [ ] I built both baselines and got three empty diffs, plus 20 matching seeds on each.
- [ ] I compiled a probe that fails on the four forbidden operations.
- [ ] I listed the seven exported symbols and read their mangled names.
- [ ] I swapped the representation twice, inside `board.cpp` only.
- [ ] I recorded one edited file, one recompiled object and a relink.
- [ ] I saw 20 of 20 traces match after both swaps.
- [ ] I recorded the object size for the array and both vectors.
- [ ] I removed the initialization from `board_create` and explained why the traces still pass.
- [ ] I compared the change surface and named the rows that tie.
- [ ] I separated what the declaration permits, what the definition hides, what the toolchain
      checks and what a run or a size measurement shows.
- [ ] My one-sentence explanation avoids the six syntax words and the eight verdict words.
- [ ] I did not edit any tracked code file.

No score. If a box is unchecked, write the next smallest action in §10.
