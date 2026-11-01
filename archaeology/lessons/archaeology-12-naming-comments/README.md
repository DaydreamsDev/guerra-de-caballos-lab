# archaeology-12 — Naming and comments

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** naming and comments · **Prerequisite:** archaeology-04 ·
> **Estimated time:** 60 minutes

This lesson rewrites names and comments with reasons. The before program uses an abbreviation and
single-letter parameters; the after program uses descriptive names and no comment that repeats the
code.

## What the learner does

- Reads the before and after programs.
- Runs both and compares the output.
- Fills the rename table with old name, new name and reason.
- Rewrites or deletes every comment and records why.

## The baseline

```text
code/before.cpp
code/after.cpp
```

`before.cpp` uses `calc`, `f`, `c` and a comment that narrates the obvious. `after.cpp` uses
`board_cell_index`, `row`, `column` and no comment. Both print `index=19`.

## Build and run

```bash
./run.sh
```

Expected output:

```text
before=index=19
after=index=19
match=yes
```

## Files

| File | Purpose |
|---|---|
| `README.md` | This file. |
| `README-es.md` | Spanish edition. |
| `lesson.md` | The guided lesson. |
| `lesson-es.md` | Spanish edition of the guided lesson. |
| `RENAME-TEMPLATE.md` | Rename and comment table. |
| `RENAME-TEMPLATE-es.md` | Spanish edition of the table. |
| `code/before.cpp` | The before program. |
| `code/after.cpp` | The after program. |
| `run.sh` | Compiles both and compares the output. |
| `meta.yml` | Lesson metadata. |
| `EVIDENCE.md` | English evidence record. |
| `EVIDENCE-es.md` | Spanish evidence record. |

## Acceptance criterion

The lesson's **Self-check (binary)** is the authoritative checklist. `meta.yml` mirrors it for
tooling.
