# archaeology-15 — Static analysis and compiler warnings

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** static analysis · **Prerequisite:** archaeology-04 ·
> **Estimated time:** 60 minutes
> **Reading:** ~22 minutes

## Why this lesson exists

A compiler warning is the cheapest piece of evidence a codebase can give you. It arrives without a
test, without a debugger and without a user. It names a file, a line, a column and the analysis that
produced it. It costs one build.

It is also easy to ignore. A warning is not an error. The program still runs and the build still
succeeds. That is exactly why warnings rot: each one is small enough to postpone, and together they
become background noise that hides the one that mattered.

The bench is deliberately small. `code/warnings.cpp` is nine lines and produces exactly one
diagnostic: an unused local variable. `code/fixed.cpp` is the same program with that declaration
removed. `run.sh` builds both, counts the warning and compares the output:

```text
warnings=1
fixed=ok
match=yes
```

Three lines, one warning, one deletion. The lesson is not the deletion. The lesson is the reasoning
that makes the deletion safe, and the evidence that proves it.

## What you will be able to do

By the end of this lesson you can:

- explain what static analysis is and where compiler warnings sit among its tools;
- read a diagnostic as four facts: location, severity, message and flag;
- reproduce a warning outside the harness, with and without `-Werror`;
- distinguish a real defect, dead code, a false positive and an intentional exception;
- triage every warning in a translation unit with a written decision per diagnostic;
- verify a warning's claim instead of trusting or dismissing it;
- choose between deletion, a real use, `[[maybe_unused]]` and suppression, and say why;
- fix a warning without changing observable behavior, and prove the behavior is unchanged;
- explain what a clean `-Werror` build proves and what it does not prove;
- read a harness critically: `match=yes` is not automatically byte equality;
- write a warning record another person can reproduce.

## The concept: a warning is a claim about code, not a verdict

### Static analysis in one sentence

**Static analysis** is reasoning about a program without executing it. The compiler is the static
analyzer you already run. Every build parses the source, resolves names, checks types and applies a
set of rules; some of those rules emit diagnostics.

| Technique | Runs the program? | Finds | Misses |
|---|---|---|---|
| Compiler warnings | No | Patterns the compiler can prove from the source. | Anything that needs runtime values. |
| Type checker | No | Type mismatches, missing conversions. | Logic errors that type-check. |
| `clang-tidy`, `cppcheck` | No | Style, bug patterns, local dataflow. | Deep interprocedural facts; some false positives. |
| GCC `-fanalyzer` | No | Paths, leaks, null dereferences. | Scale, precision, and code it cannot model. |
| Tests | Yes | Behaviors someone thought to assert. | Everything unasserted. |
| Sanitizers | Yes | Memory errors and undefined behavior at run time. | Code paths that never execute. |
| Fuzzing | Yes | Crashes on generated inputs. | Semantic wrongness that does not crash. |

The table is the point: no single technique covers the space. Warnings are cheap, repeatable and
available on every build, so they belong first. They are not sufficient.

### What the compiler actually did

The build pipeline is roughly: preprocess, parse, semantic analysis, optimize, generate code. An
unused-variable warning is produced in the compiler's front end, before optimization. You can verify
that claim: the warning appears at `-O0` and at `-O2`, unchanged. Optimization cannot remove a
diagnostic that the front end already emitted.

The compiler is not guessing. It has a precise model of the language: it knows that `unusedValue`
is a local object, that its initializer is an integer literal, that no expression reads it and that
its address is never taken. Within that model, "unused" is a fact.

The limit is the model, not the compiler's competence. A name can be read only by an external tool,
by inline assembly, or through a macro the compiler does not expand in this configuration. In those
cases the warning is either a false positive or a true finding about an intentional exception. That
is why triage exists.

### Anatomy of a diagnostic

Here is the diagnostic from this bench, produced by GCC 16.2.1:

```text
code/warnings.cpp:5:9: warning: unused variable ‘unusedValue’ [-Wunused-variable]
    5 |     int unusedValue = 0;
      |         ^~~~~~~~~~~
```

Read it in four parts:

| Part | In this diagnostic | What it tells you |
|---|---|---|
| Location | `code/warnings.cpp:5:9` | File, line and column. Column 9 is the name, not the type. |
| Severity | `warning` | The build continues unless `-Werror` is active. |
| Message | `unused variable ‘unusedValue’` | The claim, in the compiler's words. |
| Flag | `[-Wunused-variable]` | The rule that fired. This is your handle. |

The source line and the caret are context. The caret points at the declaration because that is the
thing the compiler considers unused. It does not point at a missing use, because there is no use to
point at.

The flag is the most useful part. It names the analysis, it is stable enough to search in the
compiler manual, and it is the exact token you would use to suppress the diagnostic. Suppression is
a last resort, but you cannot decide against it until you know its name.

### The warning is not the defect

"Unused variable" means the value is never read. It does not say why. The same message can describe
four different situations with four different costs:

| Situation | Class | Typical cost | How to tell |
|---|---|---|---|
| A local left over from an experiment. | Dead code | Low: noise, no behavior change. | No comment, no reference, trivial initializer. |
| A result that was supposed to be read. | Real defect | High: the program is incomplete. | The name suggests a result; nearby code expects it. |
| A parameter required by an interface. | Intentional | None, but undocumented. | The signature is fixed by a callback or base class. |
| A use the compiler cannot see. | False positive | None, but it needs evidence. | The name is read by asm, a macro or an external tool. |

The bench is the first row. `unusedValue` is initialized with `0`, never read, never passed
anywhere and never has its address taken. Deleting it removes dead code. It does not repair a live
bug, because there is no live bug in this line.

Say that precisely in your record. "The warning was a defect" is false here. "The warning was true
and the code was dead" is accurate. Precision about severity is part of the skill.

### Triage: four questions per warning

**Triage** is the decision process between reading a diagnostic and changing code. For every
warning, answer four questions in writing:

1. **Is the claim true?** Verify it. For an unused local, grep the name and check its scope. For a
   possible null dereference, follow the path.
2. **What kind of finding is it?** Real defect, dead code, false positive, or intentional exception.
3. **What is the cheapest correct fix?** Delete, use, mark, restructure, or suppress. Cheapest is
   not the same as shortest: a suppression is short and expensive later.
4. **How will I prove behavior did not change?** Capture output and exit status before and after,
   diff the source, and run the strict build.

A triage row is one warning. If a build emits forty warnings, there are forty rows, even if thirty
of them share a cause. Grouping is allowed after the rows exist; skipping rows is not.

### The bench warning, triaged

| Field | Value |
|---|---|
| Diagnostic | `unused variable ‘unusedValue’` |
| Flag | `-Wunused-variable` |
| Location | `code/warnings.cpp:5:9` |
| Claim verified? | Yes: the name appears once, on its own declaration. |
| Scope | Local to `main`; no other translation unit can read it. |
| Class | Dead code. |
| Severity | Low: no observable behavior depends on it. |
| Decision | Delete the declaration. |
| Behavior risk | None: the initializer is a literal and the object has no side effects. |

That last row deserves care. Deleting a declaration is not always behavior-preserving. If the
declaration were `std::string scratch;`, the constructor and destructor could allocate or throw; a
custom type could also log. If it were `Lock guard(mutex);`, deletion would remove a lock. The
safety argument is not "it was unused"; it is "it was unused and its construction and destruction
have no observable effect". For `int unusedValue = 0;` that argument is easy. For a class type, you
must check.

### The fix is not the only option

Five responses are possible. Only one is right for this bench, and the reasons matter more than the
choice.

| Response | What it does | Cost | Verdict here |
|---|---|---|---|
| Delete the declaration. | Removes the dead code and the warning. | None in this case. | **Chosen.** |
| `(void)unusedValue;` | Creates a fake read that silences the compiler. | Keeps dead code; hides intent. | Rejected. |
| `[[maybe_unused]]` | Documents that the object is intentionally unused. | None, but it must be true. | Rejected: nothing requires it. |
| `-Wno-unused-variable` | Disables the rule for the whole build. | Hides future real findings. | Rejected. |
| Print the value. | Makes the variable used. | Changes output; fails acceptance. | Rejected. |

`[[maybe_unused]]` is not a dirty trick. It is the correct tool when the declaration must exist: a
parameter fixed by an interface, a variable read only by a debugger, a structured binding with one
unused name. The attribute is documentation for a real constraint. Using it on dead code turns a
finding into a permanent lie.

Suppression has the same shape at a larger scale. A line-level pragma, a file-level flag or a
project-wide `-Wno-...` all trade evidence for quiet. Sometimes that trade is correct, for example
when a third-party header emits warnings you cannot fix. Then the suppression needs three things: a
reason, the narrowest possible scope, and a note that says when it can be removed. "It was noisy"
is not a reason.

### What a clean build proves

A clean build with `-Wall -Wextra -Werror` proves a narrow, useful fact:

> With this compiler, this version, these flags and these translation units, no enabled diagnostic
> fired.

It does not prove that the program is correct. It does not prove that no bug exists. It does not
prove that the tests are good, that the code is readable, or that a future compiler will stay
silent. `code/fixed.cpp` is clean and still contains a magic number, no tests and no validation.

The claim is still worth making, because it is reproducible and falsifiable. The mistake is
inflating it. Write the scope next to the result.

### Warnings are a budget, not a score

A codebase has a warning budget. At zero, every new warning is visible immediately. At three
hundred, a new one is invisible, and the team learns to scroll past the build output.

`-Werror` is a gate, not a cleanup tool. Turning it on in a tree with three hundred warnings does
not fix them; it blocks every build and teaches people to bypass the gate. The order is:

1. triage and fix the existing warnings, or record a baseline count;
2. enable `-Werror` on the cleaned scope;
3. keep the gate, and treat a new warning as a build failure;
4. revisit the flag set when the compiler or the standard changes.

This bench has one warning, so the gate is honest immediately. That is a luxury, and the lesson is
to notice it.

## Before you start

Read these in full:

- [`README.md`](README.md) — the lesson card and the acceptance criterion.
- [archaeology-04](../archaeology-04-correctness-failures/lesson.md) — the prerequisite: symptom,
  root cause and the discipline of writing a finding down.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — the
  failure classes and the severity scale this lesson uses.

You also need:

- a C++20 compiler (`c++`, `g++` or `clang++`);
- `bash`, `grep`, `diff`, `cmp` and `od`;
- a scratch directory under `build/`.

> **Rule:** do not edit `code/`. Copy the bench into `build/scratch/` before you change anything.
> `code/` is the baseline the next learner will run.

## The 60-minute path

| Min | Step | What you do | Evidence |
|---:|---|---|---|
| 0–6 | Predict | Commit to answers about the count, the location and the output | Prediction table |
| 6–16 | Compile | Run the harness, then reproduce the diagnostic by hand | Exact diagnostic, exit codes |
| 16–26 | Read | Read the bench and the harness line by line | Line counts, source diff |
| 26–40 | Triage | Verify the claim, classify it and choose a fix | One triage row, alternatives |
| 40–50 | Fix | Delete the dead declaration in a scratch copy | Scratch diff, clean build |
| 50–56 | Verify | Compare bytes, exit status and strict builds | `cmp`, `-Werror`, negative control |
| 56–60 | Record | Fill `EVIDENCE.md` | Portfolio entry |

If the harness boundary takes ten minutes to demonstrate, that investigation is the lesson.

### The engineering check in this lesson

The six verbs are **compile → read → triage → fix → verify → record**. Step 1 adds a prediction
before the first verb; it is a habit, not a seventh verb.

| Verb | Where it happens | What it produces |
|---|---|---|
| **Compile** | Step 2 | The real diagnostic and the exit codes. |
| **Read** | Step 3 | The line counts, the source diff and the harness boundary. |
| **Triage** | Step 4 | One written decision per warning. |
| **Fix** | Step 5 | A minimal scratch change with a stated reason. |
| **Verify** | Step 6 | Identical bytes, identical status, clean gate. |
| **Record** | Step 7 | `EVIDENCE.md` with commands and outputs. |

## Step 1 — Predict (6 min)

Before you compile, answer these five questions in writing. The point is to commit to answers you
can be wrong about.

1. How many diagnostics does `-Wall -Wextra` produce on `code/warnings.cpp`?
2. Which line and column does the caret point to: the declaration or a use?
3. Does the warning survive `-O0`? Does it survive `-O2`?
4. Does `-Wall` alone produce it? Does `-Wextra` alone?
5. If you delete the declaration, does stdout change? Does the exit code change?

A useful prediction is falsifiable. "Warnings are bad" is not. "One warning at line 5, column 9,
pointing at the declaration; it appears at both optimization levels; `-Wall` alone is enough;
stdout stays `value=10` and the exit code stays 0" is.

### A worked prediction

Before running anything, I would write:

- one warning, because only one declaration has no reader;
- line 5, column 9, under `unusedValue`;
- it survives `-O0` and `-O2`, because it is a semantic finding;
- `-Wall` alone produces it and `-Wextra` alone does not, because it belongs to `-Wunused-variable`,
  which `-Wall` enables;
- stdout stays `value=10\n` and the exit code stays 0, because the deleted object is never read.

I am less sure about the exact wording under `-Werror`. My guess is that the severity becomes
`error`, the flag becomes `[-Werror=unused-variable]`, and the build exits 1. Step 2 checks that
guess.

## Step 2 — Compile (10 min)

### Run the harness first

```bash
cd archaeology/lessons/archaeology-15-static-analysis
./run.sh
printf 'exit=%s\n' "$?"
```

Expected:

```text
warnings=1
fixed=ok
match=yes
exit=0
```

The harness checks the mechanical part of acceptance, not the lesson. It tells you that the baseline
has one warning, that the fixed bench builds under `-Werror`, and that the two programs print the
same text. It does not show you the diagnostic. For that, compile by hand.

### Reproduce the diagnostic

```bash
mkdir -p build
c++ -std=c++20 -Wall -Wextra -O2 code/warnings.cpp -o build/warnings
printf 'exit=%s\n' "$?"
```

Expected, on GCC 16.2.1:

```text
code/warnings.cpp: In function ‘int main()’:
code/warnings.cpp:5:9: warning: unused variable ‘unusedValue’ [-Wunused-variable]
    5 |     int unusedValue = 0;
      |         ^~~~~~~~~~~
exit=0
```

Two facts to notice. The build exits 0: a warning is not a failure by default. And the diagnostic
goes to stderr, which is why `run.sh` redirects it to `build/warnings.txt` before counting.

### Make the warning fatal

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 code/warnings.cpp -o build/warnings-strict
printf 'exit=%s\n' "$?"
```

Expected:

```text
code/warnings.cpp: In function ‘int main()’:
code/warnings.cpp:5:9: error: unused variable ‘unusedValue’ [-Werror=unused-variable]
    5 |     int unusedValue = 0;
      |         ^~~~~~~~~~~
cc1plus: all warnings being treated as errors
exit=1
```

The location and the message are identical. The severity, the flag tag and the exit code changed.
That is all `-Werror` does: it promotes a diagnostic to a build failure. It does not analyze
anything new.

The last line is GCC-specific. Another compiler will phrase it differently, which is one reason to
record the compiler and version in your evidence.

### Ask which flag set produced it

Run the same source with different flags. Capture the count, not the full text.

```bash
for flags in "-Wall" "-Wextra" "-Wall -Wextra" "-Wunused-variable" \
             "-Wall -Wextra -Wno-unused-variable"; do
    count="$(c++ -std=c++20 ${flags} -O2 code/warnings.cpp -o build/probe 2>&1 \
             | grep -c 'warning:' || true)"
    printf '%-40s warnings=%s\n' "${flags}" "${count}"
done
```

Expected on GCC 16.2.1:

| Flags | Warnings | What it shows |
|---|---:|---|
| `-Wall` | 1 | The rule is in `-Wall`. |
| `-Wextra` | 0 | The rule is not in `-Wextra`. |
| `-Wall -Wextra` | 1 | The union, not a sum. |
| `-Wunused-variable` | 1 | The specific rule is enough. |
| `-Wall -Wextra -Wno-unused-variable` | 0 | A later `-Wno-` wins. |

`-Wall` is a misnomer. It is a curated set, not every warning the compiler knows. `-Wextra` adds
another curated set. Neither is "all", and neither is a quality guarantee. The specific flag is the
one that names the analysis, and the order of flags matters because the last one wins.

That last row is a demonstration, not a recommendation. You just proved that suppression works. You
did not prove that it is a good idea.

### Look at a deeper analyzer

GCC can run a path-sensitive analysis with `-fanalyzer`:

```bash
c++ -std=c++20 -Wall -Wextra -fanalyzer -O2 code/warnings.cpp -o build/analyzer
```

On this bench it reports the same unused variable and nothing else. That is expected: there is no
path, allocation or pointer to analyze. The command is here to place the tool, not to sell it.
`-fanalyzer` is a GCC option; if you use another compiler, skip this command and say so in the
record. It is slower, it has its own false positives, and it still cannot see runtime values.

### Record the setup

Write the compiler and version now, while you have them:

```bash
c++ --version | head -1
```

You will need that line in `EVIDENCE.md`. A warning record without a compiler version is not
reproducible.

## Step 3 — Read (10 min)

### Read the three files

```bash
wc -l code/warnings.cpp code/fixed.cpp run.sh
nl -ba code/warnings.cpp
nl -ba code/fixed.cpp
nl -ba run.sh
```

The counts:

| File | Lines | What it is |
|---|---:|---|
| `code/warnings.cpp` | 9 | The bench with one dead local. |
| `code/fixed.cpp` | 8 | The same bench without that local. |
| `run.sh` | 27 | Build, count, gate, compare, print. |

`warnings.cpp` is small enough to read completely:

```cpp
#include <cstdio>

int main()
{
    int unusedValue = 0;
    int value = 10;
    std::printf("value=%d\n", value);
    return 0;
}
```

Line 5 declares and initializes `unusedValue`. Line 6 declares `value`. Line 7 reads `value` and
writes it to stdout. Line 8 returns 0. Nothing reads `unusedValue`, nothing takes its address, and
nothing passes it to a function. The compiler's claim is true.

`fixed.cpp` is identical except for the missing line 5. Do not take that on faith:

```bash
diff -u code/warnings.cpp code/fixed.cpp
```

Expected:

```text
--- code/warnings.cpp
+++ code/fixed.cpp
@@ -2,7 +2,6 @@

 int main()
 {
-    int unusedValue = 0;
     int value = 10;
     std::printf("value=%d\n", value);
     return 0;
 }
```

One deletion, zero additions. That is the entire fix. If your diff has anything else, you changed
more than the warning asked for.

### Read the harness

`run.sh` is 27 lines, and every line matters for the evidence boundary.

| Lines | What they do |
|---:|---|
| 2 | `set -euo pipefail`: stop on error, unset variable or failed pipe. |
| 4–7 | Resolve the script directory, choose the compiler, create `build/`. |
| 9–11 | Compile `warnings.cpp` with `-Wall -Wextra -O2`; send stderr to `warnings.txt`. |
| 13 | Count lines containing `warning:` in that file. |
| 15–16 | Compile `fixed.cpp` with `-Werror`; a warning aborts the script. |
| 18–19 | Capture both programs' stdout into shell variables. |
| 21 | Print the warning count. |
| 22–25 | Compare the two strings; print `match=no` and exit 1 on difference. |
| 26–27 | Print `fixed=ok` and `match=yes`. |

Three properties follow from that reading:

1. **The count is a line count.** `grep -c 'warning:'` counts matching lines, not diagnostics. On
   this bench the two are the same. If the word ever appears on more than one line, or in a
   filename, the count drifts. Know what you are counting.
2. **The gate is real.** Because of `set -e`, a warning in `fixed.cpp` under `-Werror` stops the
   script before any success line is printed. `fixed=ok` therefore means the strict build passed.
3. **The comparison is text equality, not byte equality.** `$(...)` strips trailing newlines from
   both outputs. Two programs that differ only by trailing newlines compare equal. `match=yes` is
   strong evidence, not proof of identical bytes.

That third property is not a defect in the harness; it is a documented limit you must know before
you cite it. Step 6 closes the gap with `cmp`.

### Check your prediction

- One warning, at line 5, column 9, pointing at the declaration.
- It survives `-O0` and `-O2`.
- `-Wall` alone produces it; `-Wextra` alone does not.
- Under `-Werror` the severity becomes `error`, the tag becomes `-Werror=unused-variable`, and the
  exit code is 1.
- Deleting the declaration leaves stdout at `value=10\n` and the exit code at 0.

If you predicted that `-Wextra` alone would catch it, you assumed the flag sets overlap more than
they do. If you predicted that optimization would remove the warning, you placed the analysis after
the optimizer. Both gaps are worth writing down.

## Step 4 — Triage (14 min)

### Verify the claim before you decide

```bash
grep -n 'unusedValue' code/warnings.cpp
```

Expected:

```text
5:    int unusedValue = 0;
```

One hit, the declaration. That is the evidence for "no use in this translation unit". Then make the
scope argument explicit: `unusedValue` is declared inside `main`, has automatic storage, is not
`static`, is not captured by a lambda and its address is never taken. No other translation unit can
name it. The absence of a read is therefore complete, not merely local to this file.

For a global or a class member, the same grep would not be enough. You would have to search the
whole program, including code generated by macros and code that takes the address. The scope of the
name determines the scope of the search.

### Fill one triage row

Use this shape in `EVIDENCE.md`:

| # | Diagnostic | Flag | Location | Claim true? | Class | Decision | Behavior risk |
|---:|---|---|---|---|---|---|---|
| 1 | `unused variable ‘unusedValue’` | `-Wunused-variable` | `warnings.cpp:5:9` | Yes: one hit, the declaration | Dead code | Delete | None: literal init, no side effects |

Every column is a claim you can defend. "Class: dead code" is not a feeling; it follows from the
verification and the scope. "Behavior risk: none" is not optimism; it follows from the initializer
and the absence of a destructor with effects.

### Consider the alternatives in writing

You already saw the table in the concept section. Apply it:

- **Delete.** Removes the warning and the dead code. The bench has no other reference. Chosen.
- **`(void)unusedValue;`** Adds a read that exists only to silence the compiler. The variable is
  still dead; the next reader now has to understand a fake use. Rejected.
- **`[[maybe_unused]]`** Documents an intentional exception. There is no interface, no debugger
  contract and no comment asking for the variable. Using it here would document a constraint that
  does not exist. Rejected.
- **Suppress the flag.** Hides the whole class of finding for every line in the build. One dead
  variable does not justify that. Rejected.
- **Use the value in the output.** Changes stdout, which the acceptance criterion forbids. Rejected.

The decision is not "delete because the compiler said so". It is "delete because the claim is true,
the code is dead, deletion has no observable effect, and every alternative costs more".

### When the warning is a false positive

A false positive is a diagnostic whose claim is not true for this program. It is rarer than a true
finding about code that exists for a reason the compiler cannot see. Before calling either one,
prove the mechanism: a name read by inline assembly, a variable inspected only by a debugger, or a
declaration kept for a debug build. If the use is real, the warning is not false; it is a limit of
the model or an intentional exception.

The procedure is the same:

1. prove the use exists, with the mechanism named;
2. if the use is real but invisible to the compiler, mark it (`[[maybe_unused]]`) or restructure so
   the use is visible;
3. if the analyzer is wrong about a fact it should know, record the compiler version and the minimal
   reproduction;
4. suppress only with a reason and the narrowest scope.

Do not start at step 4. A suppression that outlives its reason is worse than the warning it hid.

### What triage is not

Triage is not a vote. One person can do it. It is not a severity contest, and it is not a reason to
rewrite the program. The output is a decision per diagnostic, with evidence. For this bench the
whole triage is one row. That is fine. A one-row table that is correct beats a ten-row table that
was copied from a blog post.

## Step 5 — Fix (10 min)

### Copy, do not edit

```bash
mkdir -p build/scratch
cp code/warnings.cpp build/scratch/warnings.cpp
```

From here on, edit only `build/scratch/warnings.cpp`. `code/warnings.cpp` stays as the baseline.

### Delete the declaration

Remove line 5. Any editor works; the result must be exactly:

```cpp
#include <cstdio>

int main()
{
    int value = 10;
    std::printf("value=%d\n", value);
    return 0;
}
```

Prove that the change is minimal:

```bash
diff -u code/warnings.cpp build/scratch/warnings.cpp
```

Expected: one removed line, the declaration, and no other hunk.

If your diff shows reordered lines, a changed format string or a new comment, you did more than the
warning required. Revert and do it again. The smallest change that removes the cause is the easiest
one to review and the easiest one to prove safe.

### Build with the gate

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 \
    build/scratch/warnings.cpp -o build/scratch/warnings
printf 'exit=%s\n' "$?"
```

Expected: no output, `exit=0`.

### Compare with the reference fix

```bash
diff -u code/fixed.cpp build/scratch/warnings.cpp
```

Expected: no output. Your fix and the reference fix are the same file.

If they differ, read the difference before you "fix" it. `fixed.cpp` is the answer key, but the
exercise is the reasoning. Copying `fixed.cpp` over your scratch file at the start would produce a
clean build and teach you nothing. Copying it at the end as a cross-check is fine.

### What not to do

- Do not add `(void)unusedValue;` and call it a fix. You silenced a finding and kept the dead code.
- Do not add `[[maybe_unused]]` without a constraint that requires the variable.
- Do not add `-Wno-unused-variable` to `run.sh`. The harness is evidence; weakening it weakens the
  lesson.
- Do not change the format string, the value or the return statement. None of them is part of the
  finding.

## Step 6 — Verify (6 min)

### Compare behavior, not intention

Build the original and the fixed scratch copy, capture stdout and exit status separately, and
compare bytes:

```bash
c++ -std=c++20 -Wall -Wextra -O2 code/warnings.cpp -o build/warnings
./build/warnings > build/before.txt
printf '%s\n' "$?" > build/before.exit

./build/scratch/warnings > build/after.txt
printf '%s\n' "$?" > build/after.exit

cmp build/before.txt build/after.txt && printf 'stdout: same bytes\n'
cmp build/before.exit build/after.exit && printf 'status: same\n'
wc -c < build/before.txt
od -c build/before.txt
```

Expected:

```text
stdout: same bytes
status: same
9
0000000   v   a   l   u   e   =   1   0  \n
0000011
```

Nine bytes: eight characters of `value=10` and one newline. Both programs write exactly those bytes
and exit 0.

Why `cmp` and not `diff`? `diff` reports line differences and is happy to ignore a missing final
newline depending on how you call it. `cmp` compares bytes. For a behavior claim, bytes are the
honest unit.

### Check the strict build again

```bash
c++ -std=c++20 -Wall -Wextra -Werror -Wpedantic -O2 \
    build/scratch/warnings.cpp -o build/scratch/warnings-pedantic
printf 'exit=%s\n' "$?"
```

Expected: no output, `exit=0`. `-Wpedantic` adds conformance diagnostics; there are none to add
here. If it complains, read the message before assuming the bench is wrong.

### Check both optimization levels

```bash
for level in -O0 -O2; do
    c++ -std=c++20 -Wall -Wextra -Werror "${level}" \
        build/scratch/warnings.cpp -o "build/scratch/warnings${level}"
    "build/scratch/warnings${level}" > "build/scratch/out${level}.txt"
done
cmp build/scratch/out-O0.txt build/scratch/out-O2.txt && printf 'levels: same bytes\n'
```

Expected: `levels: same bytes`. The warning was a front-end finding, and the output does not depend
on optimization. Both facts are worth recording.

### Run the negative control

A test you have never seen fail is not evidence. Copy the fixed bench once more, change the printed
value, and confirm the harness notices. Do this in scratch, never in `code/`.

```bash
cp build/scratch/warnings.cpp build/scratch/broken.cpp
sed -i 's/int value = 10;/int value = 11;/' build/scratch/broken.cpp
c++ -std=c++20 -Wall -Wextra -Werror -O2 \
    build/scratch/broken.cpp -o build/scratch/broken
```

Now compare its output with the original:

```bash
./build/warnings > build/original.txt
./build/scratch/broken > build/broken.txt
cmp build/original.txt build/broken.txt
printf 'cmp exit=%s\n' "$?"
```

Expected: `cmp` exits 1 and reports that the files differ. That is the harness's `match=no` path in
miniature. You have now seen the check reject a behavior change, which is what makes its acceptance
meaningful.

### Re-run the harness

```bash
./run.sh
```

Expected, unchanged:

```text
warnings=1
fixed=ok
match=yes
```

`warnings=1` is correct and is not a regression. The harness deliberately compiles the original
`warnings.cpp` to report the baseline warning count; on this bench it is 1. Your fixed build is a
separate artifact under `build/scratch/`, and it is clean.

### State the evidence boundary

You have proved:

- the original bench emits one diagnostic, and its claim is true;
- the fixed bench emits none under the shipped flag set plus `-Wpedantic`;
- the source change is one deletion;
- stdout and exit status are byte-identical;
- the comparison can fail when behavior changes.

You have not proved that the program is correct, that no other bug exists, or that the compiler will
stay silent on another version. Say so in the record.

## Step 7 — Record (4 min)

Fill [`EVIDENCE.md`](EVIDENCE.md). At minimum:

- compiler, version, standard and exact flags;
- the five predictions, written before the first build;
- the harness output and exit code;
- the verbatim diagnostic, with file, line, column, message and flag;
- the exit codes with and without `-Werror`;
- the warning counts for `-Wall`, `-Wextra` and the specific flag;
- the line counts: 9, 8 and 27;
- the source diff: one deletion, zero additions;
- the triage row and the alternatives you rejected, with reasons;
- the scratch diff and the strict build result;
- the `cmp` results for stdout and exit status, and the 9-byte size;
- the negative control and its result;
- the harness boundary: text equality, trailing newlines stripped, `cmp` closes the gap;
- one sentence stating what the clean build proves and what it does not.

A sentence such as "the warning is fixed" is a status. "GCC 16.2.1 with `-Wall -Wextra -Werror`
accepts the scratch file; the diff against the baseline removes one declaration and adds nothing;
`cmp` reports identical 9-byte stdout and identical exit status; the negative control changes one
digit and `cmp` fails" is a record.

## The bench in detail

| Location | What is there |
|---:|---|
| `warnings.cpp:1` | `#include <cstdio>`, for `std::printf`. |
| `warnings.cpp:3` | `int main()`, the standard entry point. |
| `warnings.cpp:5` | `int unusedValue = 0;`, the dead declaration. |
| `warnings.cpp:6` | `int value = 10;`, the live value. |
| `warnings.cpp:7` | `std::printf("value=%d\n", value);`, the only output. |
| `warnings.cpp:8` | `return 0;`, the only exit path. |
| `fixed.cpp:5` | `int value = 10;`; the dead declaration is gone. |
| `run.sh:9–11` | First build, warnings to stderr, captured to a file. |
| `run.sh:13` | `grep -c 'warning:'`, the count. |
| `run.sh:15–16` | Second build with `-Werror`, the gate. |
| `run.sh:18–19` | Both outputs captured into variables. |
| `run.sh:22–25` | Text comparison and the failure path. |
| `run.sh:21,26–27` | The three printed lines. |

The two source files differ in exactly one line. The harness differs from a strict byte comparison
in three ways: command substitution strips trailing newlines, it compares only stdout, and it does
not compare the two exit statuses; it relies on `set -e` to stop on a nonzero exit. Both facts are
small enough to verify by hand, which is the point of a bench.

## What this bench proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| The baseline emits exactly one warning. | Yes | `run.sh` prints `warnings=1`; the manual build shows one diagnostic. |
| The warning's claim is true. | Yes | `grep` finds one occurrence, the declaration; the name is local. |
| The finding is dead code, not a live defect. | Yes | Nothing reads the value; the initializer has no side effects. |
| The fix is behavior-preserving. | Yes | `cmp` reports identical stdout and status; the diff is one deletion. |
| The fixed bench is clean under `-Werror`. | Yes | The strict build exits 0 with no output. |
| The byte comparison detects a behavior change. | Yes | The negative control makes `cmp` fail. |
| The program is correct. | No | One clean build says nothing about logic or requirements. |
| There are no other bugs. | No | Only enabled diagnostics were checked, on one translation unit. |
| `-Werror` will stay green forever. | No | A compiler upgrade can add or change diagnostics. |
| `match=yes` proves byte equality. | No | Command substitution strips trailing newlines; use `cmp`. |
| Compiler warnings replace tests. | No | They see source patterns, not runtime behavior. |
| Suppression is a fix. | No | It removes the message and keeps the cause. |

This table is the honesty check. A record that says "zero warnings, therefore clean" without naming
the compiler, the flags and the scope is overreaching.

## Self-check (binary)

- [ ] I wrote the five predictions before compiling.
- [ ] I ran `./run.sh` and saw `warnings=1`, `fixed=ok`, `match=yes`, exit 0.
- [ ] I reproduced the diagnostic by hand and copied it verbatim.
- [ ] I recorded the compiler and version.
- [ ] I saw the exit code change from 0 to 1 when I added `-Werror`.
- [ ] I measured the counts for `-Wall`, `-Wextra`, the specific flag and the suppression.
- [ ] I read `warnings.cpp`, `fixed.cpp` and `run.sh` line by line.
- [ ] I recorded the line counts: 9, 8 and 27.
- [ ] I verified the warning's claim with `grep` and stated the scope argument.
- [ ] I classified the finding as dead code, not a live defect.
- [ ] I filled one triage row with evidence in every column.
- [ ] I rejected `(void)`, `[[maybe_unused]]`, suppression and a behavior change, with reasons.
- [ ] I copied the bench to `build/scratch/` before editing.
- [ ] My diff removes one declaration and adds nothing.
- [ ] The strict build exits 0 with no output.
- [ ] My scratch file matches `fixed.cpp` exactly.
- [ ] `cmp` reports identical stdout and identical exit status.
- [ ] I recorded the 9-byte output.
- [ ] I ran the negative control and saw the comparison fail.
- [ ] I stated the harness boundary: text equality, not byte equality.
- [ ] I stated what the clean build proves and what it does not.
- [ ] I did not edit `code/`.

If a box is not ticked, the unticked box names the next step. There is no penalty for repeating one.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| "Warnings are noise." | Noise is what hides the one warning that matters. | Triage each one; keep the budget at zero. |
| "Zero warnings means no bugs." | A clean build checks source patterns, not requirements or runtime. | Pair the gate with tests and review. |
| "`-Wall` enables all warnings." | It is a curated set; `-Wextra` adds more, and both omit rules. | Read the flag list; enable what you need. |
| "`-Werror` fixes the code." | It only promotes diagnostics to build failures. | Fix or triage the finding first. |
| "Deleting an unused variable is always safe." | A constructor or destructor can have side effects. | Check the type and the initializer before deleting. |
| "Suppressing a warning is the same as fixing it." | The message disappears; the cause remains. | Fix the cause, or document a narrow exception. |
| "The compiler is always right." | Its model can miss uses in asm, macros or external tools. | Verify the claim; mark real exceptions. |
| "The compiler is never right about unused code." | It usually is; unused means unread within its model. | Prove the use exists before dismissing it. |
| "`match=yes` proves the output is identical." | Shell capture strips trailing newlines. | Compare files with `cmp`. |
| "A warning that survives `-O2` is an optimizer bug." | This one comes from semantic analysis, before optimization. | Place the finding in the pipeline. |
| "Warnings are compiler-specific, so ignore them." | The differences are real, but the findings are still evidence. | Record the compiler and reproduce on another if it matters. |
| "The harness is the lesson." | The harness checks three lines; the lesson is the reasoning. | Use the harness as evidence, not as a substitute. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Static analysis | Reasoning about a program without running it. |
| Diagnostic | A message from a tool about the code: error, warning or note. |
| Warning flag | The named rule that produced a diagnostic, such as `-Wunused-variable`. |
| `-Wall` | A curated set of common warnings, not literally all of them. |
| `-Wextra` | An additional curated set with rules that `-Wall` does not enable. |
| `-Werror` | Treat every enabled warning as an error. |
| Front end | The parse and semantic-analysis stages, before optimization. |
| Translation unit | One source file after preprocessing, compiled as a unit. |
| Dead code | Code whose result cannot affect observable behavior. |
| Triage | Deciding what a finding is and what to do about it, with evidence. |
| False positive | A diagnostic whose claim is not true for this program. |
| Suppression | Disabling a diagnostic without changing the cause. |
| `[[maybe_unused]]` | A C++17 attribute documenting an intentional non-use. |
| Behavior-preserving | The observable output and status do not change. |
| Negative control | A deliberate change that must make a check fail. |
| Warning budget | The tolerated number of warnings; zero is the useful default. |

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- Continue to `archaeology-16` — platform lock-in. A warning is about the code; a platform
  dependency is about the environment. The two diagnoses use different evidence.
- If another compiler is installed, build the same bench with it and compare the wording, the flag
  names and the exit codes. Record the differences instead of assuming they are errors.
- Write a small program with a real leak and run it under `-fanalyzer`. Compare what the analyzer
  finds with what a sanitizer finds at run time. The gap between them is the gap between static and
  dynamic analysis.
- Add `-Wall -Wextra -Werror` to one of your own projects. Count the warnings first. If the count is
  not zero, fix or baseline before you enable the gate, or the gate will teach people to bypass it.

## References

- [`README.md`](README.md) — the lesson card and the acceptance criterion.
- [`code/warnings.cpp`](code/warnings.cpp) — the bench with one dead declaration.
- [`code/fixed.cpp`](code/fixed.cpp) — the reference fix.
- [`run.sh`](run.sh) — build, count, gate and compare.
- [`EVIDENCE.md`](EVIDENCE.md) — the record you fill in.
- [archaeology-04](../archaeology-04-correctness-failures/lesson.md) — symptom, root cause and
  written findings.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — failure
  classes and severity.
- GCC warning options: <https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html>.
- Clang diagnostics reference: <https://clang.llvm.org/docs/DiagnosticsReference.html>.
