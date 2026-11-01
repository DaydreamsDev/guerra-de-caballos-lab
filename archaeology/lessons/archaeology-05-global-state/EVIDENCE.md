# archaeology-05 — Evidence: global state and hidden coupling

> This is your record. Write it for your future self.
> Record what you observed, not what you expected the lesson to want.

## 1. Setup

- Date:
- Archive path:
- Tag checked out:
- Commit hash (`git rev-parse HEAD`):
- Shell and version:
- Kernel and distribution (`uname -sr`):
- Working directory:
- Anything unexpected in the setup:

## 2. Prediction — write before reading the functions

Do not edit this table after Step 3. The gap between prediction and observation is the learning
signal; repairing the prediction destroys it.

| # | Question | My prediction | How I would know I am wrong |
|---:|---|---|---|
| 1 | Global written by the most functions | | |
| 2 | Global read but not written after setup | | |
| 3 | Global carrying more than one meaning | | |
| 4 | Function hardest to test | | |

One falsifiable sentence about the set:

>

## 3. Scanner run

Command:

```bash
./archaeology/lessons/archaeology-05-global-state/scan_globals.sh /path/to/archive
```

Observed mention counts:

```text
vectab=
vecjugadas=
cab1=
cab2=
jugada=
orden=
ganador=
cf1=
cc1=
cf2=
cc2=
cab=
simcab1=
simcab2=
sep=
```

- Exit code:
- Archive path used:
- Function headers the scanner recognized:
- Why the header count is not the function count:

The mention count is a starting point, not the map. A mention is not a mutation.

## 4. Global map — mutations per global

Add rows if you find a global the template does not list.

| Global | Type | Declared at | Written by | Read by | Side effects | Mutation path | Confidence |
|---|---|---|---|---|---|---|---|
| `vectab` | | | | | | | |
| `vecjugadas` | | | | | | | |
| `cab1` | | | | | | | |
| `cab2` | | | | | | | |
| `jugada` | | | | | | | |
| `orden` | | | | | | | |
| `ganador` | | | | | | | |
| `cf1` | | | | | | | |
| `cc1` | | | | | | | |
| `cf2` | | | | | | | |
| `cc2` | | | | | | | |
| `cab` | | | | | | | |

Read-only file-scope symbols excluded from the map, with the reason:

- `simcab1`:
- `simcab2`:
- `sep`:

## 5. Function map

| Function | Reads | Writes | Prints | Hidden coupling |
|---|---|---|---|---|
| `main` | | | | |
| `inicializacion` | | | | |
| `cabubicacion` | | | | |
| `impresion` | | | | |
| `conversionvm` | | | | |
| `conversionmv` | | | | |
| `vecjug` | | | | |
| `imprevecjug` | | | | |
| `movcab` | | | | |
| `ordenjuego` | | | | |
| `impresionorden` | | | | |
| `impresion0` | | | | |

Indirect writers I marked as indirect:

-

## 6. Mutation paths

Write each path as a call chain ending at the value that changes.

- Path 1:
- Path 2:
- Path 3:
- Hardest path to test:
- Why it is the hardest:

## 7. Evidence boundary

- Verified from source:
- Inferred from call sites:
- Unknown:

## 8. Fixes

- Minimal fix (one seam):
- Structural fix:
- Track A link:
- Test that would catch a regression:
- What the minimal fix does not fix:
- What the structural fix does not fix:

## 9. What the map proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| A global is written at the cited line. | | |
| A function depends on a global it does not receive. | | |
| A function prints as a side effect. | | |
| The mutation order at runtime is the order in the map. | | |
| The design failure causes a wrong result. | | |
| The minimal fix removes the coupling. | | |
| The structural fix removes the class. | | |

## 10. Severity and cost

One paragraph. Name the cost in real work, not the ugliness of the code. Say whether the cost is
medium or high in your context and why. Do not propose a rewrite in this paragraph.

>

## 11. Archive integrity

- `git status` in the archive:
- Did I edit `original/`? (It must be no.)
- Did I apply any patch inside the archive? (It must be no.)
- Where did I keep my scratch copy, if I made one?

## 12. Next step

- Continue to `archaeology-06` — duplication and the eight-case switch.
- Re-read the global you ranked last. Was the rank about mutation count or about consequence? Write
  one sentence.
- If a global had no writer in your map, check whether it is written through a callee before
  concluding that it is constant.
