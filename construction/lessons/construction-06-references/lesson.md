# References (referencias)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** aliasing without addresses (alias sin direcciones) · **Time:** about 45 minutes ·
**Status:** drafted
**Reading:** ~15 minutes

## Why this lesson exists

`construction-05` selected the active knight with a local alias and moved it with two assignments
in the middle of `playTurn`. Both writes named the same record. The selection and the write were
correct, but the write had no name of its own, and a call to any helper could not say whether the
record would change.

The references baseline replaces the two local aliases with two functions that return aliases, and
the two assignments with one function that writes through a reference parameter. The behavior does
not change: all 20 traces stay byte-identical. What changes is where the alias is created and what
its type permits.

The sentence to keep:

> A reference is a second name for an existing record; a write through either name is a write to
> the same record.

Two measurements follow. The first is behavioral: remove one `&` from `moveKnight` and all 20 traces
diverge, because the function writes to a copy. The second is structural: the file has 17 `Knight`
parameters, 9 of them mutable aliases, 8 read-only aliases, and none by value. The first number
shows what a reference does; the second shows how often this file relies on it.

One honest note before the comparison: references are not new in this file. `construction-05`
already had `initializePositions(Knight &, Knight &)`, `tryMove(..., int &destinationRow, ...)` and
the two local aliases. This lesson does not introduce the syntax. It makes the alias a named
boundary, splits it into mutable and read-only, and gives the write its own function. If you expect
a new keyword, you will be disappointed; the lesson is in the types.

## What you will be able to do

By the end of this lesson you can explain aliasing without addresses and apply the lesson's change
test.

## The concept: one record, two names

Five forms of name do the work in this comparison:

```text
value parameter      void moveKnight(Knight knight, ...)      the callee owns a copy
reference parameter  void moveKnight(Knight &knight, ...)     the callee names the caller's record
const reference      const Knight &otherKnight                the callee may read, not write
reference return     Knight &selectActiveKnight(...)          the caller receives the chosen record
local alias          Knight &activeKnight = ...               a second name inside the body
```

Side by side, the three sites that name the knight:

| Decision | `construction-05` | `construction-06` |
|---|---|---|
| Active knight | local `Knight &` from a ternary | `Knight &` returned by `selectActiveKnight` |
| Other knight | local mutable `Knight &` from a ternary | `const Knight &` returned by `selectOtherKnight` |
| Move | two assignments in `playTurn` | `moveKnight(Knight &, ...)` |

The two baselines differ in one decision: how the selected knight is named. The diff is 21 lines
added and 4 removed, net 17. Three functions appear, 19 definitions become 22, and 63 parameters
become 72. No existing signature changes type, and no existing call site changes an argument. The
transcript does not move.

Side by side, the difference is what a parameter promises:

| Question | `Knight` by value | `Knight &` |
|---|---|---|
| What the parameter holds | a copy | a name for the caller's record |
| A write inside the function | changes the copy | changes the caller's record |
| What the caller sees after the call | nothing | the new field values |
| What the call site says about mutation | nothing | nothing |
| Can the parameter be null | no | no; it must bind to a record |
| Can it be reseated later | not applicable | no; assignment copies the record |
| What the call costs | eight bytes copied | a name passed |
| What the compiler checks | a written copy that is never read (warning) | binding and `const` violations |

At the language level, a reference does not expose an address and does not add one to the source.
The implementation may use an address; the code does not. That is why this rung comes before
pointers: you learn the alias without the address, then `construction-07` gives you the address and
the bill.

What a reference is **not**, in this lesson:

- **Not a pointer.** There is no `&` at the call, no `*` to read, no arithmetic, and no null value.
- **Not a copy.** `moveKnight(activeKnight, ...)` writes to the record the caller passed, not to a
  duplicate that is thrown away.
- **Not reseatable.** `activeKnight = machineKnight;` copies the machine record into the player
  record. It does not make `activeKnight` name the machine from then on.
- **Not an owner.** The reference does not extend the record's lifetime. A reference that outlives
  its record dangles, and the compiler does not stop you.
- **Not visible at the call site.** `moveKnight(activeKnight, destinationRow, destinationColumn)`
  looks exactly like a read. The signature is the only place that says otherwise.
- **Not a safety feature.** A mutable reference gives the callee every field. It does not protect
  an invariant. `construction-10` and `construction-11` are where that starts.

Three consequences follow:

1. **The selection has one home.** `selectActiveKnight` returns the alias, so `playTurn` does not
   repeat the ternary that chooses between the two records.
2. **The write has one home.** `moveKnight` writes both fields, so the two assignments that had to
   stay together cannot drift apart.
3. **The read-only side is enforced.** `selectOtherKnight` returns `const Knight &`, so a call that
   tries to move the other knight does not compile.

The cost is the fourth thing: the call site says nothing. The reader must know the signature, and
the compiler is the only one who checks it.

## Before you start

- [`construction-05`](../construction-05-array-of-records/lesson.md) — you have its board of
  records, its change test on `moveNumber`, and its distinction between what a record localizes
  and what it does not.
- A C++20 compiler (`g++` or `clang++`). `cmake` is optional.

You do not need to know C++ well. You need to read a parameter list, a return type, and a call. The
only new mark is `&`, and the lesson is about what that mark permits, not about its spelling.

### What you need

- The references baseline: [`code/references-baseline.cpp`](code/references-baseline.cpp).
- The array-of-records baseline:
  [`board-cells-baseline.cpp`](../construction-05-array-of-records/code/board-cells-baseline.cpp).
- Build scripts and notes: [`run.sh`](run.sh) and [`README.md`](README.md).
- The rules: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §12 fixes the
  order of random draws.
- The transcript format:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- One golden trace: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- This learner template: [`EVIDENCE.md`](EVIDENCE.md).

Check that the artifacts are present before you begin, from this lesson directory:

```bash
test -f code/references-baseline.cpp &&
test -f ../construction-05-array-of-records/code/board-cells-baseline.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

If either baseline is missing, the comparison has no second side. Record the missing file in
`EVIDENCE.md` §11 and return when it is available. A count against a file you have not read is not
evidence.

Both baselines reproduce all 20 traces in `common/traces/` byte for byte, before you change
anything. Same game, two ways of naming the knight. That is what makes the comparison fair.

## Step 1 — Inventory the aliases (~8 min)

Read both files once. Then let the text tell you where a knight is named by reference:

```bash
grep -n 'Knight &' code/references-baseline.cpp
grep -n 'const Knight &' code/references-baseline.cpp
grep -n 'selectActiveKnight\|selectOtherKnight\|moveKnight' code/references-baseline.cpp
```

The first command also matches the `const Knight &` lines; the second prints the read-only subset.
The third prints the three new functions and every call to them.

Then read the whole change in one view:

```bash
diff -u ../construction-05-array-of-records/code/board-cells-baseline.cpp \
  code/references-baseline.cpp
```

Three hunks: the three new functions, the two local aliases that became calls, and the two
assignments that became one call.

Fill this table into `EVIDENCE.md` §2.1 while you read. Line numbers are from the references
baseline.

| Function | Line | Knight parameters | Form | Reads | Writes |
|---|---|---|---|---|---|
| `initializePositions` | 125 | 2 | `Knight &` | — | `row`, `column` |
| `printGameHeader` | 149 | 2 | `const Knight &` | `row`, `column` | — |
| `printTurnHeader` | 171 | 1 | `const Knight &` | `row`, `column` | — |
| `printMove` | 206 | 1 | `const Knight &` | `row`, `column` | — |
| `selectActiveKnight` | 218 | 2 | `Knight &`, returns `Knight &` | — | — (grants write access) |
| `selectOtherKnight` | 223 | 2 | `const Knight &`, returns `const Knight &` | — | — |
| `moveKnight` | 229 | 1 | `Knight &` | — | `row`, `column` |
| `tryMove` | 235 | 2 | `const Knight &` | `row`, `column` | — |
| `playTurn` | 258 | 2 | `Knight &` | through helpers | through `moveKnight` |
| `playGame` | 327 | 2 | `Knight &` | — | forwards |
| `main` | 345 | 0 | declares two records | — | initializes |

Counts: 9 mutable knight parameters, 8 read-only ones, 0 by value, and 2 reference returns.

Two rows are worth a minute.

- `playGame` receives both knights as mutable references and writes neither. It forwards them to
  `playTurn`. The mutable alias is a pass-through, exactly as the board parameter was in
  `construction-05`. The type does not say "I write"; it says "I may".
- `printGameHeader`, `printTurnHeader` and `printMove` receive `const Knight &` and only read. They
  could take `Knight` by value and still be correct. The copy would cost eight bytes per call for
  no benefit, but correctness is not the reason the reference is there; access is.

Then answer in `EVIDENCE.md` §2:

1. How many functions receive a knight by mutable reference? By `const` reference? By value?
2. Which two functions return a knight alias, and what does each return type permit?
3. Which function is the only one that writes a knight's fields through a parameter?
4. Which three functions could take a knight by value without changing behavior, and why is by
   value not the right default there?
5. Which two lines from `construction-05` disappeared, and what replaced them?
6. Why are `selectOtherKnight`'s parameters `const` as well as its return type?
7. What would `Knight &otherKnight` allow that `const Knight &otherKnight` prevents?
8. Which of these aliases existed in `construction-05` before this lesson? List them by line.

Question 4 is the one that separates "by value is wrong" from "by value is wrong where a write must
escape". Question 8 is the honesty check: the syntax was already there.

## Step 2 — Predict before you run (~8 min)

### 2.1 The flip

The change under test is one token: delete the `&` after `Knight` in `moveKnight`'s signature. Do
not touch the body, the call, or any other function.

```cpp
void moveKnight(Knight knight, int destinationRow, int destinationColumn)
```

The function is still valid C++. It assigns `knight.row` and `knight.column`, then returns. The
assignment is legal because `knight` is a local object of the function.

Write these predictions before building:

| Question | My prediction |
|---|---|
| Does it build with `-Wall -Wextra -Werror`? | |
| If not, which diagnostic class appears? | |
| First line of `seed-0001.txt` that differs (number and text) | |
| Does the `move=` line still appear? | |
| What does the next `turn=` header for that knight say? | |
| Moves in the by-value run of seed 1 (golden: 21) | |
| Seeds that differ, of 20 | |
| Signature changes / call sites edited / characters removed | |
| Does the by-value game terminate? | |
| Lines added or removed in the body | |

The argument you are testing: the move is printed before `moveKnight` is called, from
`destinationRow` and `destinationColumn` computed in `playTurn`. The call is the only thing that
writes the new position into the record. If the call writes to a copy, the print survives and the
position does not.

### 2.2 The selector return types

Three one-line changes, each on a scratch copy. Answer before compiling in `EVIDENCE.md` §3.2.

| Change | Compiles? | What the compiler says | What the program would do |
|---|---|---|---|
| `selectActiveKnight` returns `Knight` | | | |
| `selectActiveKnight` returns `const Knight &` | | | |
| `selectOtherKnight` returns `Knight &` | | | |

The line that receives the result is
`Knight &activeKnight = selectActiveKnight(playerIsActive, playerKnight, machineKnight);`. Use it.
A non-const reference cannot bind to a temporary, and it cannot bind to a `const` object.

### 2.3 The limits

Answer these now, without testing:

- Can a reference be null, so that "no knight" is representable?
- Can `activeKnight` be made to name the other record after it is initialized?
- Does `moveKnight(activeKnight, ...)` tell a reader that the argument may change?
- What happens if a reference outlives the record it names?
- Which of the four does the compiler catch?

Write the answers in `EVIDENCE.md` §3.3. Step 4 and Step 5 check them.

## Step 3 — Run both baselines (~5 min)

Create the scratch directory first; every command below writes into it.

```bash
mkdir -p /tmp/reference-test
./run.sh --seed 1 --name Learner > /tmp/reference-test/references-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/reference-test/references-0001.txt
```

Then the previous baseline:

```bash
cd ../construction-05-array-of-records
./run.sh --seed 1 --name Learner > /tmp/reference-test/board-cells-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/reference-test/board-cells-0001.txt
```

Then the two against each other:

```bash
diff -u /tmp/reference-test/board-cells-0001.txt /tmp/reference-test/references-0001.txt
```

Three empty diffs. Return to this lesson directory and confirm all 20 seeds, not only the first:

```bash
cd ../construction-06-references
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

A silent loop is 20 matches. If `run.sh` cannot build, compile once by hand:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o build/references-baseline code/references-baseline.cpp
```

If a diff is not empty, stop. Record it in `EVIDENCE.md` §11. Do not adjust a trace to match the
code.

## Step 4 — Break the boundary (~10 min)

Copy the baseline. The tracked file stays untouched.

```bash
cp code/references-baseline.cpp /tmp/reference-test/by-value.cpp
```

Remove the `&`:

```cpp
void moveKnight(Knight knight, int destinationRow, int destinationColumn)
```

Build with the project flags:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/reference-test/by-value /tmp/reference-test/by-value.cpp
```

### 4.1 Why the compiler complains

With GCC, the build stops:

```text
error: parameter 'knight' set but not used
```

The relevant option is `-Werror=unused-but-set-parameter`. Your compiler may print the message in
another language; the diagnostic class is `unused-but-set-parameter`. Read what it says: the
parameter is written and never read. A by-value parameter that is written and never read is a
local dead end. The compiler does not know that you meant to change the caller's record. It only
sees a write that no one will observe.

That is the first evidence, and it arrives before the program runs. The mistake is legal C++, so
the compiler cannot reject it on principle; it can only warn about the dead store.

### 4.2 What the transcript shows

If the build stopped, demote that one warning from error to warning, so the program can be built
and observed:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -Wno-error=unused-but-set-parameter -O2 \
  -o /tmp/reference-test/by-value /tmp/reference-test/by-value.cpp
/tmp/reference-test/by-value --seed 1 --name Learner \
  > /tmp/reference-test/by-value-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/reference-test/by-value-0001.txt | head -20
```

If your compiler does not know that option, drop `-Werror` for this scratch build instead.

The first useful lines of the diff:

```text
 move=1 machine 4,4->3,6
-turn=3 who=machine at=3,6 order=3,4,5,6,7,8,1,2
+turn=3 who=machine at=4,4 order=3,4,5,6,7,8,1,2
```

The `move=` line is still there, and it still says `4,4->3,6`. The next machine turn is the proof:
the knight is still at `4,4`. The move was printed before the call, from values computed in
`playTurn`; the call wrote to a copy that died at the closing brace.

So the transcript does not stop showing the move. It stops showing the move's **effect**, and it
contradicts itself one line later. That is a stronger observation than a missing line: a transcript
can print an intention and still disprove it.

Now all 20 seeds:

```bash
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/reference-test/by-value --seed "${seedNumber}" --name Learner \
    > "/tmp/reference-test/by-value-${traceName}.txt"
  if cmp -s "${trace}" "/tmp/reference-test/by-value-${traceName}.txt"; then
    echo "seed ${seedNumber} match"
  else
    echo "seed ${seedNumber} differs"
  fi
done
```

All 20 differ. Seed 1 ends after 9 moves instead of 21. The stale knight changes every legal-move
decision, so the whole game diverges, not only one header.

### 4.3 Restore the reference

Restore the `&` in the scratch copy, or copy the baseline again:

```bash
cp code/references-baseline.cpp /tmp/reference-test/by-value.cpp
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/reference-test/by-value /tmp/reference-test/by-value.cpp
```

The build is clean again. Run the 20-seed loop from Step 3 again and you get 20 matches.

Four observations, each one part of the answer:

**One — one character changed the semantics.** The signature changed; the body did not; no call
site changed. The call `moveKnight(activeKnight, destinationRow, destinationColumn)` reads the same
in both versions. A caller cannot see the difference. That is the price of an alias that is not
marked at the call.

**Two — the write happened, in a copy.** `knight.row = destinationRow;` executed. The record in
`playTurn` did not change. Both statements are true at once because `knight` and `activeKnight` are
two different objects.

**Three — the next header is the evidence.** The `move=` line is printed from local values before
the call; it proves the intention, not the mutation. The `turn=` header of the same knight on its
next turn reads the record. That is where the copy is exposed.

**Four — the compiler saw the dead write.** `-Wunused-but-set-parameter` is not a style warning.
It says the parameter is assigned and never read, which is exactly what a by-value mutation is. The
warning is easy to silence and easy to ignore; the 20 diffs are not.

Write `EVIDENCE.md` §5: the exact commands, the diff, the 20 results, which predictions held, and
which missed. The usual miss is the build: learners predict that by value compiles cleanly. It is
valid C++, but with GCC the project flags turn the dead-store warning into an error, so the first
observation is a failed build, not a wrong transcript.

## Step 5 — Const is a one-way street (~7 min)

Copy the baseline to a second scratch file:

```bash
cp code/references-baseline.cpp /tmp/reference-test/const-check.cpp
```

Add one line after the two aliases in `playTurn`:

```cpp
otherKnight.row = 0;
```

Build it. The write is rejected:

```text
error: assignment of member 'Knight::row' in read-only object
```

The message text depends on the compiler and its language. The cause does not: `otherKnight` is a
`const Knight &`, so every use of that name is read-only.

Replace the line with a call to the mutating function:

```cpp
moveKnight(otherKnight, 1, 1);
```

Build again. It is rejected for the same reason:

```text
error: binding reference of type 'Knight&' to 'const Knight' discards qualifiers
```

Now put both lines back:

```cpp
otherKnight.row = 0;
moveKnight(otherKnight, 1, 1);
```

Remove `const` from `selectOtherKnight`, both from its return type and from its two parameters:

```cpp
Knight &selectOtherKnight(bool playerIsActive, Knight &playerKnight, Knight &machineKnight)
```

Build. Both lines compile. Do not run the result: it would move the other knight and change the
game. Restore the copy before you continue.

What the pair proves:

- `const` is checked at every use of the name, not only where the alias is declared.
- Passing a `const` alias to a `Knight &` parameter fails, so the compiler prevents the wrong
  knight from being moved. That is a type-level guard, not a convention.
- The guard is only as strong as the aliases. The player and machine records are still mutable in
  `main`; a non-const alias anywhere can write them.

Keep the count: two lines that fail to compile with `const`, and one function whose `const` was
removed to make them compile. A run cannot prove what a type prevents; the compiler is the
evidence.

## Step 6 — Compare and explain (~5 min)

Write `EVIDENCE.md` §8. Two parts.

**First, one sentence.** Say what a reference decides about a record, without using the words
*pointer*, *address*, *better*, *worse*, *clean*, *cleaner*, *nice*, *elegant*, *ugly* or
*modular*. If your sentence needs one of those, you have described your reaction to the file
rather than the type.

**Second, one paragraph.** Answer:

- What a parameter type decides: whether the callee owns a copy or names the caller's record.
- What a return type decides: which record the caller's alias names, chosen at run time.
- What `const` decides, and where the compiler checks it.
- What the call site says about mutation, and what it hides.
- What the references localize: the selection, the write, and the read-only side.
- What they do **not** localize: nullability, reseating, lifetime, ownership, and call-site
  visibility.
- What the compiler can prove, and what only a run can prove.
- Which later lesson fixes each remaining gap: `construction-07` for addresses, null and
  reseating; `construction-09` for bounds and representation; `construction-10` and
  `construction-11` for invariants and access; `construction-16` for ownership and lifetime.

A strong answer keeps three things apart: **what the signature permits**, **what the call site
shows**, and **what the compiler enforces**. A weak answer says references are cleaner, which hides
all three.

## Step 7 — Acceptance check (~2 min)

The acceptance criterion for this lesson is: **mutate a record through a reference parameter, show
that a by-value parameter cannot change the caller's record, and keep all 20 traces passing with
the reference version.**

Check your record against it:

- [ ] I ran both unmodified baselines and got three empty diffs, plus 20 matching seeds.
- [ ] I inventoried the 17 knight parameters: 9 mutable, 8 read-only, 0 by value.
- [ ] I identified `selectActiveKnight` returning `Knight &` and `selectOtherKnight` returning
      `const Knight &`.
- [ ] I identified `moveKnight(Knight &, ...)` as the function that mutates through a parameter.
- [ ] I predicted the by-value result before building, including the build outcome.
- [ ] I changed `moveKnight` to take `Knight` by value and saw the caller's record stop changing.
- [ ] I explained why the `move=` line still appears while the next `turn=` header is stale.
- [ ] I ran the by-value version on all 20 seeds and saw 20 differences.
- [ ] I restored the reference and got 20 matches again.
- [ ] I showed that `const` rejects both a direct write and a call to `moveKnight`.
- [ ] I showed that removing `const` makes both writes compile.
- [ ] I explained what a reference makes explicit and what it still hides at the call site.
- [ ] My one-sentence explanation avoids the two syntax words and the eight verdict words.
- [ ] I did not edit either tracked baseline.
- [ ] I completed `EVIDENCE.md` §1–§10 and the self-check in §12.

The criterion is not "references are better than values". It is "you can say what a parameter type
permits, what the caller can see, and which check would notice if the permission were wrong".

## Self-check (binary)

- [ ] I read both baselines and inventoried every knight parameter and alias.
- [ ] I predicted the by-value build, transcript and seed counts before measuring.
- [ ] I built both baselines and reproduced all 20 traces.
- [ ] I flipped `moveKnight` to by value, observed the stale position, and counted 20 differences.
- [ ] I restored the reference and reproduced all 20 traces.
- [ ] I used `const` to reject a wrong write and a wrong call.
- [ ] I compared the prediction with the observation and named the misses.
- [ ] I explained what a reference localizes and what it does not.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §10.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "A reference is a pointer with nicer syntax." | There is no address in the source, no arithmetic, no null and no reseating. Those are pointer features, and they arrive in `construction-07`. | Read the signature; count what it permits. |
| "Passing by reference is always faster." | A two-int record is cheap to copy. The reason here is mutation, not speed. | Ask whether a write must survive the return. |
| "The call site shows that the record may change." | `moveKnight(activeKnight, ...)` has no marker. The signature is the only place the decision lives. | Read the declaration, not the call. |
| "A reference can be null." | It cannot; it must bind to a record. "No knight" is not representable, which is a cost, not a feature. | Use a pointer when absence is a real state. |
| "You can make a reference name another record later." | `activeKnight = machineKnight;` copies the machine values into the player record; it does not rebind the name. | Rebind with a pointer, or select again. |
| "By-value parameters are always wrong." | The three print helpers read only; by value is correct there, just unnecessary copying. | Ask where the write must land. |
| "The trace proves the mutation." | The `move=` line is printed before the call. The next `turn=` header proves the position. | Compare the header, not the intention line. |
| "The compiler catches the by-value mistake." | It warns about a written parameter that is never read. Without `-Werror` the program builds and runs wrong. | Treat the warning as evidence, and run the traces. |
| "`const` prevents all mutation." | It prevents writes through that name. Another non-const alias can still write the record. | Count the aliases, not the `const` keywords. |
| "The reference makes the code safe." | Any mutable alias can write any field. Invariants and access control come later. | That is `construction-10` and `construction-11`. |

## Next step

`construction-07` — pointers. A reference is an alias that must bind, cannot be null and cannot be
reseated. A pointer is an address you can hold, compare, null and change. The call site shows the
`&`, so the mutation becomes visible again; the bill is null checks and lifetime.

After pointers, `construction-08` puts the board behind a module boundary, and `construction-09`
gives the board its own bounds so the `1..8` stops being a fact the caller must remember.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If either baseline was missing, do not start `construction-07`. The next contrast needs both
sides.
