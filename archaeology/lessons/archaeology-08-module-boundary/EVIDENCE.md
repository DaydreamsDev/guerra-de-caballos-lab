# archaeology-08 — Evidence: missing module boundary

> This is your record. Write it for your future self.
> Record what you observed, not what you expected the lesson to want.

## 1. Setup

- Date:
- Archive path:
- Tag checked out:
- Commit hash (`git rev-parse HEAD`):
- Compiler and version (`c++ --version`):
- Shell and version:
- Kernel and distribution (`uname -sr`):
- Scratch directory:
- Anything unexpected in the setup:

## 2. Prediction — write before reading `code/board.h`

Do not edit this table after Step 2. The gap between prediction and observation is the learning
signal; repairing the prediction destroys it.

| # | Question | My prediction | How I would know I am wrong |
|---:|---|---|---|
| 1 | Lines in the archive that mention `vectab` | | |
| 2 | Archive headers (`.h` / `.hpp`) | | |
| 3 | Functions that must change when the board moves behind a header | | |
| 4 | What the compiler rejects in `main.cpp` | | |
| 5 | Whether the output changes | | |
| 6 | What the header still reveals | | |

One falsifiable sentence about the set:

>

## 3. Inspection of the original

Commands:

```bash
grep -c "vectab" original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
find original -name "*.h" -o -name "*.hpp" | wc -l
wc -l original/*/source/*.cpp
```

Observed:

- `vectab` mentions:
- Header files in the archive:
- Lines in `variant-01`:
- Functions in `variant-01` (excluding `main`):
- Prototype lines:
- Where the board is declared (file and line):

Regions that reach the array directly:

| Region | Lines | Reads | Writes |
|---|---|---|---|
| `main` | | | |
| `inicializacion` | | | |
| `impresion0` | | | |
| `impresion` | | | |
| `movcab` | | | |
| `ordenjuego` | | | |

## 4. The module I wrote

- Header path:
- Implementation path:
- Header lines:
- Implementation lines:
- `main.cpp` edits: (must be zero)
- Helpers moved out of the class: (yes / no)
- Where the index formula lives now:

Public surface I chose:

| Declaration | Why a caller needs it |
|---|---|
| | |
| | |
| | |
| | |

## 5. Compile evidence

### The compiler's half

Command:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 board.cpp main.cpp -o board-split
```

The line I added to `main.cpp`:

```cpp
```

The error, verbatim:

```text
```

- Error code or identifier:
- Line the compiler pointed at:
- Line the note pointed at:

### The linker's half

The declaration I left without a definition:

The error, verbatim:

```text
```

- What this proves that the compiler error does not:
- Why the linker reported only the function the caller used:

### The boundary test

```bash
c++ -std=c++20 -Wall -Wextra -Werror -c board.cpp -o board.o
c++ -std=c++20 -Wall -Wextra -Werror -c main.cpp -o main.o
c++ board.o main.o -o board-split
grep -n "visited\[" main.cpp && exit 1
```

- Separate compilation result:
- `grep` result:
- In the archive, the same `grep` would have matched how many functions:

## 6. Caller knowledge

| Question | Caller knows? | How the caller learns it |
|---|---|---|
| Which operations exist | | |
| Parameter and return types | | |
| Which operations mutate | | |
| That rows and columns are 1-based | | |
| That out-of-range input is ignored | | |
| That the board is 8×8 | | |
| That the board is an array of 64 `bool` | | |
| The index formula | | |
| That `reset` is redundant after construction | | |

The row I found most surprising, and why:

>

## 7. Change test

Changes that stay inside the module:

| Change | Caller source | Caller recompile | Output |
|---|---|---|---|
| | | | |
| | | | |
| | | | |

Changes that reach the caller:

| Change | Why it reaches the caller |
|---|---|
| | |
| | |
| | |

- What I verified by running, and what I only reasoned about:
- The recompile-versus-rewrite distinction, in one sentence:

>

## 8. Evidence boundary

- Verified from source:
- Verified by running the compiler:
- Inferred from the header:
- Unknown:

## 9. What the boundary proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| A caller cannot name the visited array. | | |
| Every declared function has a body. | | |
| The output is unchanged by the split. | | |
| The caller no longer needs the index formula. | | |
| The representation is hidden from the compiler. | | |
| Callers survive a representation change without recompiling. | | |
| The module documents its coordinate system. | | |
| The module rejects invalid input. | | |

## 10. Severity and cost

One paragraph. Name the cost in real work, not the ugliness of the code. Say whether the cost is
medium or high in your context and why. Do not propose a rewrite in this paragraph.

>

## 11. Archive integrity

- `git status` in the archive:
- Did I edit `original/`? (It must be no.)
- Did I apply any patch inside the archive? (It must be no.)
- Where did I keep my scratch copy?
- Did any command write into the archive by accident? (Check, do not assume.)

## 12. Next step

- Continue to `archaeology-09` — missing abstract data type. The header you wrote still shows the
  representation; that lesson removes it.
- Re-read the row of the caller-knowledge table that the header could not answer. Write the contract
  sentence it is missing.
- If your header grew past 20 lines, list each declaration and the caller that needs it. Anything
  with no caller is a leak.
- If you could not reproduce the compiler error, check that you compiled the modified `main.cpp` and
  not a stale object file.
