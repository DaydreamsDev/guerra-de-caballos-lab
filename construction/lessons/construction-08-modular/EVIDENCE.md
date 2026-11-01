# construction-08 — Evidence
> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record compares one translation unit with four, and measures one change to the board's active
size. Keep both sides. A tie in line count is still a result if the recompiled units differ.

## 1. Run setup

- Date:
- Seed: `1`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- Modular baseline read on (date):
- Pointers baseline read on (date):
- Construction-07 record at hand? (yes / no)
- The number I carried from `construction-07` (lines / traces / translation units):

## 2. Interface inventory (before the run)

### 2.1 Headers and declarations

Fill this from the source, with line numbers, not from memory.

| Module | Header | Header lines | Declarations | Definitions in | Data hidden |
|---|---|---|---|---|---|
| | | | | | |

Counts:

- Headers:
- Translation units:
- Public declarations:
- Function definitions, total:
- Internal helpers (`game.cpp`):
- Internal helpers (`main.cpp`):
- Program entry points:

### 2.2 Include graph

Paste the output of `g++ -std=c++20 -Icode -MM` for each `.cpp`, or fill the table.

| Object | Direct includes |
|---|---|
| `main.o` | |
| `game.o` | |
| `board.o` | |
| `random_sequence.o` | |

- Which translation unit includes the most project headers:
- Which project headers does `game.cpp` include:
- Which project header does `main.cpp` include:

### 2.3 Hidden names

Mark `yes` or `no` from the source, before compiling.

| Name | Declared in a header? | Defined in | Usable from `main.cpp`? |
|---|---|---|---|
| `run_game` | | | |
| `board_column_count` | | | |
| `BoardCell` | | | |
| `boardCells` | | | |
| `Knight` | | | |
| `randomState` | | | |
| `moveOrderMaximum` | | | |
| `parseArguments` | | | |

- The row where the boundary is clearest, and why:
- The name I expected to be visible and was not:

### 2.4 The single `8`

`game.cpp` contains one literal `8`. Its line and name:

- Line:
- Identifier:
- Is it a board dimension? (yes / no)
- Why:

## 3. Prediction (written before measuring)

### 3.1 The traces

The change: none. Both baselines implement the same game.

| Question | My prediction | Confidence | Reason |
|---|---|---|---|
| Modular vs `seed-0001.txt` | | | |
| Pointers vs `seed-0001.txt` | | | |
| Modular vs pointers, seed 1 | | | |
| Seeds that match, of 20 | | | |
| First line that could differ | | | |

- The result I expect to miss, and why:

### 3.2 The change surface

The change: `activeBoardRowCount` and `activeBoardColumnCount` in `board.cpp` from 8 to 10.

| Question | My prediction |
|---|---|
| Source files edited | |
| Lines edited | |
| Headers edited | |
| `.cpp` files edited outside `board.cpp` | |
| Objects recompiled, of 4 | |
| Does it compile without other edits? | |
| Seeds that match the 8×8 traces, of 20 | |
| Does the game still reach an ending? | |

- The number I expect to miss, and why:

### 3.3 Header visibility

Copy your prediction from §2.3 here, or write it here first and compare later.

| Name | `main.cpp` | `game.cpp` | `board.cpp` |
|---|---|---|---|
| `run_game` | | | |
| `board_column_count` | | | |
| `BoardCell` | | | |
| `boardCells` | | | |
| `Knight` | | | |
| `randomState` | | | |
| `moveOrderMaximum` | | | |

- The unit that knows the most names, and why:
- The unit that knows the fewest, and why:

## 4. Run — both baselines

- Modular build command:
- Pointers build command:
- Exit statuses:
- Diff target: `../../../common/traces/seed-0001.txt`

Record `empty` or `diff` in each result cell.

| Check | Command | Result |
|---|---|---|
| Modular vs golden | | |
| Pointers vs golden | | |
| Modular vs pointers | | |

```text
<diff output, or "no differences">
```

Seeds that matched, of 20:

If any diff is not empty, stop here and fill §11.

## 5. Map each header to its implementation

### 5.1 Objects and external symbols

- Object build command:
- `nm` command:
- Platform note (any symbol the command could not show):

| Header | Declarations | Object | External symbols | Hidden names |
|---|---|---|---|---|
| `board.h` | 5 | `board.o` | | |
| `game.h` | 1 | `game.o` | | |
| `random_sequence.h` | 3 | `random_sequence.o` | | |

- What the game object exports, in full:
- What the board object exports, in full:
- What the random object exports, in full:
- Where the 17 game helpers went at `-O2`:
- The sentence I will keep about a header and an object file:

### 5.2 Include graph

- Paste the `g++ -MM` output for the four units:

```text
<output>
```

- Which object depends on all three project headers:
- Which object depends on only `game.h`:
- Which header change would recompile `main.o`:

### 5.3 The linker check

- Scratch copy:
- Declaration kept:
- Definition removed:
- Compile exit statuses:
- Link exit status:
- Diagnostic class:
- Message:

```text
<linker output>
```

- What the compiler checked:
- What the linker checked:
- What neither checked:

## 6. Change the board to 10×10

- Scratch copy:
- Constant changed, before / after:
- Second constant changed, before / after:
- Files edited:
- Lines edited:
- Headers edited:
- I did not edit any tracked code file: (yes / no)

### 6.1 Build and recompile

- First build: objects and timestamps:
- After the edit, objects recompiled:
- Command that decided what was stale:
- Link command:
- If the build recompiled all four, why:
- What the build log proves about `game.cpp` and `main.cpp`:

### 6.2 Behavior

- Run command:
- Exit status:
- Seeds that matched the 8×8 traces, of 20:
- The first line that differs:
- Why the starting positions change:
- Why the rolls and every later draw come from a different state:
- Why 0 of 20 is expected and not a regression:
- Did the game reach `winner=` and `moves=`? (yes / no)

### 6.3 File-by-file

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

### 6.4 Header change (the contrast)

- Second scratch copy:
- Header touched:
- Objects recompiled:
- Objects that stayed:
- Why `main.o` stayed:
- Why `game.o` did not:
- The rule I will keep about implementation changes and interface changes:

## 7. Compare the change surface

| Metric | Pointers baseline | Modular baseline |
|---|---|---|
| Files touched | | |
| Lines changed | | |
| Signatures whose parameter type changes | | |
| New concepts required | | |
| Translation units recompiled | | |
| 8×8 traces broken | | |

- Which numbers tie:
- Which numbers separate the two designs:
- What is inside the recompiled unit in each case:
- What modularity bought:
- What modularity cost:
- Which later change would separate the two designs more clearly, and why:

## 8. Explain

One sentence, without the words *header*, *file*, *module*, *compile*, *better*, *worse*, *clean*,
*cleaner*, *nice*, *elegant*, *ugly* or *modular*:

>

One paragraph:

- What a public declaration decides:
- What the implementation file hides:
- What the compiler checks:
- What the linker checks:
- What the build graph shows:
- What the interface still exposes:
- What it does not enforce:
- What only a run proves:
- What modularity cost:
- Which later lesson makes the representation swap explicit:
- Which later lessons add invariants:
- Which later lesson owns ownership:
- Which later lesson owns ports and adapters:

The four things I kept apart:

## 9. Acceptance check

- I ran both unmodified baselines: (yes / no)
- I got three empty diffs and 20 matching seeds: (yes / no)
- I inventoried 9 public declarations, 28 definitions and 18 internal helpers: (yes / no)
- I mapped each header to its object with `nm`: (yes / no)
- I showed a missing definition compiling and failing at link: (yes / no)
- I predicted the traces and the change surface before measuring: (yes / no)
- I changed two constants in `board.cpp` to 10 and 10: (yes / no)
- `game.cpp` and `main.cpp` required no edits: (yes / no)
- Only `board.o` recompiled, then the link: (yes / no)
- The 10×10 build reached a winner or a draw: (yes / no)
- I got 20 of 20 matches before and 0 of 20 after: (yes / no)
- I explained why 0 of 20 is expected: (yes / no)
- A header change recompiled its includers, a `.cpp` change did not: (yes / no)
- I measured the change-surface cost on both baselines: (yes / no)
- I explained what the headers hide and what they expose: (yes / no)
- My sentence avoids the four layout words and the eight verdict words: (yes / no)
- I did not edit any tracked code file: (yes / no)

If I edited a tracked code file, what and why:

## 10. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-09`:
- The check I trust most: the trace, the object symbols, or the build log:
- The check that surprised me:
- What the header hid that the single file showed:
- What the single file kept together that the modular baseline separates:
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
- [ ] I wrote the include graph from the compiler, not from memory.
- [ ] I predicted the traces, the edit surface and the recompilation surface before measuring.
- [ ] I built both baselines and got three empty diffs, plus 20 matching seeds.
- [ ] I compiled four objects and listed their external symbols.
- [ ] I showed the compile-versus-link split with a missing definition.
- [ ] I changed the active board to 10×10 without editing any other source file.
- [ ] I counted one edited file, two lines, one recompiled object and a relink.
- [ ] I saw 0 of 20 traces match and explained why that is expected.
- [ ] I contrasted a `.cpp` change with a header change and their blast radii.
- [ ] I compared the change-surface cost on both baselines.
- [ ] I separated what the interface permits, what the implementation hides, what the toolchain
      checks and what a build log or a run shows.
- [ ] My one-sentence explanation avoids the four layout words and the eight verdict words.
- [ ] I did not edit any tracked code file.

No score. If a box is unchecked, write the next smallest action in §10.
