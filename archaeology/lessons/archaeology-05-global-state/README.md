# archaeology-05 — Global state and hidden coupling

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** global state and hidden coupling · **Prerequisite:**
> archaeology-04 · **Estimated time:** 60 minutes

This lesson maps the mutable global state of the archived `variant-01` source and the hidden
coupling it creates. The output is a table of mutations per global and a function-level map of
reads, writes and printing side effects.

## What the learner does

- Runs `scan_globals.sh` to list the declarations and mention counts.
- Reads the functions and records which globals each one reads, writes and prints.
- Fills `GLOBAL-STATE-MAP-TEMPLATE.md`.
- Names the hardest mutation path to test and the smallest change that makes it testable.

## The source

```text
<archive>/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

The source declares board, move order, knight positions, turn, order and winner as file-scope
variables. Functions such as `cabubicacion`, `vecjug`, `movcab` and `ordenjuego` read and write them
and print from inside the same functions.

## Run the scanner

From the lab root:

```bash
./archaeology/lessons/archaeology-05-global-state/scan_globals.sh /path/to/archive
```

The script prints the global declarations, the function headers it can recognize, and a mention
count for each global. The mention count is a starting point, not the map.

## Files

| File | Purpose |
|---|---|
| `README.md` | This file. |
| `README-es.md` | Spanish edition. |
| `lesson.md` | The guided lesson. |
| `lesson-es.md` | Spanish edition of the guided lesson. |
| `GLOBAL-STATE-MAP-TEMPLATE.md` | The map the learner fills in. |
| `GLOBAL-STATE-MAP-TEMPLATE-es.md` | Spanish edition of the template. |
| `scan_globals.sh` | Lists declarations and global mention counts. |
| `meta.yml` | Lesson metadata. |
| `EVIDENCE.md` | English evidence record. |
| `EVIDENCE-es.md` | Spanish evidence record. |

## Acceptance criterion

The lesson's **Self-check (binary)** is the authoritative checklist. `meta.yml` mirrors it for
tooling.
