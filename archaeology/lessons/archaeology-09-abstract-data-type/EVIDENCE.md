# archaeology-09 — Evidence: missing abstract data type

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
| 1 | Lines in `variant-03` that mention `mvectab` | | |
| 2 | Lines of `main.cpp` that must change when `Board` becomes opaque | | |
| 3 | What the compiler rejects that the class version allowed | | |
| 4 | Whether the two representations print the same line | | |
| 5 | What the create/destroy API makes the caller responsible for | | |

One falsifiable sentence about the set:

>

## 3. Inspection of the original

Commands:

```bash
grep -c "mvectab" original/variant-03-pointers-dev-2008/source/*.cpp
grep -c "tablero" original/variant-04-pointers-final-2008/source/*.cpp
grep -n "struct" original/variant-02-records-2008/source/*.cpp
grep -n "mvectab=0" original/variant-03-pointers-dev-2008/source/*.cpp
```

Observed:

- Lines that mention `mvectab` in `variant-03`:
- Lines that mention `tablero` in `variant-04`:
- The only struct in `variant-02`:
- The two wrapper declarations (file and line):
- The two write sites that replace `vectab[i]=0;`:
- Header files in the archive:

The wrappers, side by side:

| Variant | Type declaration | Field | Array declaration |
|---|---|---|---|
| `variant-03` | | | |
| `variant-04` | | | |

The test I applied: rename the field, then change only its type. I counted the lines that stop
compiling in each case.

- What I changed in a scratch copy:
- How many lines stopped compiling:
- What that proves about the wrapper:

## 4. The opaque type I wrote

- Header path:
- Implementation path:
- Header lines:
- Implementation lines:
- `main.cpp` edits: (must be zero)
- Where the definition of `struct Board` lives:
- Where the helpers live:
- Are the helpers in an anonymous namespace? (yes / no)
- `nm` output for the helpers:

Public surface I chose:

| Declaration | Why a caller needs it |
|---|---|
| | |
| | |
| | |
| | |
| | |
| | |

## 5. Compile evidence — the incomplete type

For each forbidden operation, the line I added and the diagnostic I got.

### Size

```cpp
```

```text
```

### Stack allocation

```cpp
```

```text
```

### Member access

```cpp
```

```text
```

### Delete

```cpp
```

```text
```

- Which of the four were errors and which was a warning:
- What `-Werror` did to the warning:
- What the warning means when it is not promoted to an error:

## 6. The swap

### Two builds, one caller

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 main.cpp board_array.cpp -o board-array
c++ -std=c++20 -Wall -Wextra -Werror -O2 main.cpp board_bitmask.cpp -o board-bitmask
./board-array
./board-bitmask
```

- Array output:
- Bitmask output:
- `run.sh` result:
- `main.cpp` hash before and after:
- `board.h` hash before and after:

### The swap is exclusive

Command:

```bash
c++ -std=c++20 -O2 main.cpp board_array.cpp board_bitmask.cpp -o both
```

The error, verbatim:

```text
```

- What this proves about the choice of representation:

### Measured sizes

```cpp
// probe.cpp — a probe, not a caller
#include "board_array.cpp"
#include <cstdio>
int main() { std::printf("array_sizeof=%zu\n", sizeof(Board)); }
```

- Array size:
- Bitmask size:
- How I measured the second one:
- Why the caller cannot print either number:

### What "no recompile" meant in my build

- Build shape I used (static binary / shared library):
- Files that recompiled:
- Files that relinked:
- Files that did not change:
- The caller's source edits:

## 7. Caller knowledge

| Question | Caller knows? | How the caller learns it |
|---|---|---|
| Which operations exist | | |
| Parameter and return types | | |
| Which operations mutate | | |
| How big a board is | | |
| How a board is stored | | |
| Whether `board_create` can fail | | |
| Who owns the handle | | |
| That rows and columns are 1-based | | |
| That out-of-range input is ignored | | |
| That `board_reset` is redundant after create | | |

The row I found most surprising, and why:

>

## 8. Change test and cost

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

Lifetime failures I would have to prevent by hand:

| Failure | What happens | Who catches it |
|---|---|---|
| | | |
| | | |
| | | |

The pimpl comparison, in my own words:

- What the pimpl adds:
- What the pimpl costs:
- Which one I would choose for this caller, and why:

## 9. Evidence boundary

- Verified from source:
- Verified by running the compiler:
- Verified by running the program:
- Inferred from the header:
- Unknown:

## 10. What the swap proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| The caller cannot size or allocate a `Board`. | | |
| The caller cannot name a member. | | |
| The same caller compiles against two representations. | | |
| The output is identical for the scenario. | | |
| The caller's source survives a representation change. | | |
| The caller's object survives a representation change. | | |
| Both implementations agree for every input. | | |
| The handle is leak-free. | | |
| The board rejects invalid coordinates. | | |
| The type documents its contract. | | |

## 11. Severity and cost

One paragraph. Name the cost in real work, not the ugliness of the code. Say whether the cost is
medium or high in your context and why. Do not propose a rewrite in this paragraph.

>

## 12. Archive integrity

- `git status` in the archive:
- Did I edit `original/`? (It must be no.)
- Did I apply any patch inside the archive? (It must be no.)
- Where did I keep my scratch copy?
- Did any command write into the archive by accident? (Check, do not assume.)

## 13. Next step

- Continue to `archaeology-10` — missing polymorphism. The `switch` on move type is still there,
  and an opaque type does not remove it.
- Re-read the caller-knowledge row that the header could not answer. Write the contract sentence it
  is missing.
- If your header grew past 12 lines, list each declaration and the caller that needs it. Anything
  with no caller is a leak.
- If you want the safer version, write the pimpl and compare the line count. Then decide which cost
  your caller can pay.
- If the two outputs matched too easily, add a third scenario: mark the same cell twice, mark a
  corner, mark out of range. Both implementations must still agree.
