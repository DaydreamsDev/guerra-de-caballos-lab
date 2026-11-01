# failure-013 — Board size, width, glyphs and draw cap are unnamed literals

> [English](failure-013-magic-numbers.md) | [Español](failure-013-magic-numbers-es.md)

- **Class:** design
- **Severity:** low
- **Concept violated:** a domain decision deserves a name
- **Archive:** tag `v1.0.0`,
  `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`

## Evidence

- **Variant:** `variant-01-baseline-2008`.
- **Lines:** 16, 17, 61, 81–82, 106–109 and 229.
- **Source:**

```cpp
const char simcab1=21,simcab2=156,sep=219;  // line 16
int  vectab[64],vecjugadas[8];              // line 17

for (i=0;i<=64;i++)                         // line 61
cab1= rand() % 64;                          // line 81
cab2= rand() % 64;                          // line 82

cf1=cab1/8;  cc1=cab1%8;                    // lines 106–107
cf2=cab2/8;  cc2=cab2%8;                    // lines 108–109
ccab=cf * 8 +cc;                            // line 229
```

`64` is the cell count and the placement modulus. `8` is the board width, the move count and
the row divisor. `21`, `156` and `219` are CP437 code points for the two knights and the wall;
they are written as decimal integers with no code page named.

`200` is not in the archive. It is the draw cap in the modern specification and the harness
default: `common/spec/game-rules.md` §11 and `common/harness/README.md` (`--max-moves`,
default `200`). The 2008 source has no draw clause at all; `archaeology-13` records that
absence as an inferred rule. The card includes `200` because it is the same class in the
reference implementation, not because the archive contains it.

## Symptom

The program works at 8×8. The meaning of each number lives in the reader's head. A board resize
requires finding every `64`, every `8` and every conversion, and deciding which `8` is the
width, which is the move count and which is the row divisor.

The glyphs are worse: `21`, `156` and `219` look like arbitrary integers. Nothing says they
are CP437 bytes, so a UTF-8 rewrite can silently change the board.

## Root cause

The code encodes domain decisions as literals at the point of use. The same digit carries
several meanings, so a change cannot be made by editing one name. There is no `boardWidth`,
no `boardCells`, no `moveCount`, no `knightGlyph` and no `maxMoves`.

The draw cap is a separate case: the archive has no draw rule, so `200` exists only in the
modern spec and harness. That is a policy literal outside the archive, and it has the same
problem: a default buried in a tool.

## Consequence

Changes are error-prone. A 10×10 board touches the array size, the placement modulus, four
conversion lines and every bounds check. A missed `8` keeps the old width in one path. A code
page change breaks the glyphs without a compiler error.

The program is correct for 8×8 and CP437. The cost is paid by the first resize or encoding
change.

## Test that catches it

No lesson replaces these literals. The available behavior check is
`archaeology/lessons/archaeology-14-tests-determinism/run.sh`, which freezes a transcript and
compares a rebuilt program against it:

```text
seed=1
move=1 position=2
move=2 position=5
move=3 position=0
winner=player
moves=3
golden=yes
```

The concrete change check is: replace one literal with a named constant, rebuild, and compare
the output with the frozen trace. Repeat for `64`, `8`, `21`, `156`, `219` and the spec's
`200`. A `grep -n` before and after shows whether a stray literal remains.

`archaeology-12` explicitly leaves the magic `8` in place: renaming a variable is a different
change from naming a constant. Do not claim that lesson fixes this card.

## Minimal fix

Name the decisions:

```cpp
constexpr int boardWidth = 8;
constexpr int boardCells = boardWidth * boardWidth;
constexpr int moveCount = 8;

constexpr char knightOneGlyph = 0x15;   // CP437, not UTF-8
constexpr char knightTwoGlyph = 0x9C;
constexpr char wallGlyph = 0xDB;

constexpr int maxMoves = 200;           // spec and harness, not the archive
```

Use the names in the declarations, the loops, the conversions and the bounds checks. The
values do not change; the meaning becomes visible.

## Structural fix

Give the board a type that owns its width and cell count, and give the rules a type that owns
the draw cap:

```cpp
class Board
{
public:
    static constexpr int width = 8;
    static constexpr int cellCount = width * width;
    // ...
};
```

The glyphs become an enum or a small `Glyphs` record with a comment naming CP437. The draw cap
becomes a `GameRules` field, not a CLI default duplicated in the harness.

Track A rungs: [`construction-04-records`][c04] for named fields and
[`construction-17-stl`][c17] for `std::array`, whose size comes from the type.

[c04]: ../../construction/lessons/construction-04-records/lesson.md
[c17]: ../../construction/lessons/construction-17-stl/lesson.md

## Confidence

High on the archive literals and their lines. High that `200` is absent from the archive and
present in the spec and harness. Medium on the exact number of `8` occurrences that are board
width versus move count: the same digit serves both, which is the defect.

## Note on the archive

This card is evidence, not a repair of the archive. The archive stays read-only; the named
constants belong in a scratch copy or in `reference/`.
