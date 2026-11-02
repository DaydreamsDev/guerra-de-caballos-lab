# archaeology-16 — Platform lock-in

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** platform lock-in and adapters · **Prerequisite:**
> archaeology-01 · **Estimated time:** 60 minutes
> **Reading:** ~25 minutes
> **Lesson role:** optional · safe to skip

## Why this lesson exists

The archive does not compile on Linux. It does not compile on macOS. It does not compile with a
modern Microsoft compiler either, because `iostream.h` was removed from the standard. The first
error is not subtle:

```text
fatal error: conio.h: No such file or directory
```

Six constructs carry the lock-in. In `variant-01` at tag `v1.0.0`, line 11 includes `conio.h`,
line 12 includes `iostream.h`, line 32 declares `void main()`, line 38 runs `system("color 1f")`,
and lines 42 and 43 call `getch()` and `system("cls")`. The same shape repeats in every 2008
variant. The catalog calls it `failure-015`, a robustness failure: **the program works only in one
environment**.

`archaeology-11` separated input and output *inside* the program. This lesson is about the device
underneath: color, clear and key. The bench is 34 lines across three files, and it builds here:

```text
adapter=portable
```

That sentence is the trap. "Portable" describes the build. It does not describe the behavior. The
caller compiles against three functions instead of `conio.h`, and that is a real improvement. It
does not mean the program looks, waits or clears the same way on another terminal. The lesson is
the gap between those two claims.

The adapter is small on purpose. It hides enough to make the caller portable, and it hides too
little to make the behavior identical. Learning to say which one you bought is the skill.

## What you will be able to do

By the end of this lesson you can:

- name the six archive constructs that bind the program to Windows and Visual C++ 6.0;
- separate compile-time lock-in from runtime lock-in, and say which one a header hides;
- explain what an adapter hides and what it merely relocates;
- distinguish portability of the interface from identity of behavior, with a measured example;
- prove a caller has no platform header by reading includes and object symbols, not a comment;
- swap one adapter for another and show the caller compiles unchanged;
- measure the bytes the adapter writes and explain why a pipe receives all of them;
- state why `std::getchar` is not `getch`, in echo, buffering and blocking;
- list the terminal capabilities the adapter assumes without asking;
- price the lock-in by build failure, not by taste;
- keep the archive read-only while you work.

The lesson is done when another
person can reproduce both builds and follow your reasons.

## The concept: a portable name is not a portable behavior

### Lock-in is a dependency on an environment

Code is **locked in** when a correct build or run depends on an environment that is not part of the
artifact's stated contract. The archive depends on three environments at once:

| Layer | Example | What it assumes |
|---|---|---|
| Language dialect | `#include<iostream.h>`, `void main()` | A pre-standard compiler. |
| Platform API | `#include<conio.h>`, `getch()` | A DOS/Windows console. |
| Terminal behavior | `system("color 1f")`, `system("cls")` | A Windows command shell and console buffer. |

The first two fail at build time. The third can survive compilation and fail at run time, silently.
That difference matters: deleting a header can make a program compile without making it work.

### The adapter in one picture

```text
main.cpp ──► platform.h ──► platform.cpp ──► terminal
             (port)         (adapter)
```

`platform.h` is the **port**: the shape of the conversation the caller is allowed to have with a
console. `platform.cpp` is one **adapter**: an implementation of that conversation for one
environment. `main.cpp` is the caller, and it does not know whether the adapter writes ANSI escapes,
calls a Win32 function, or does nothing at all.

The port is three functions:

```cpp
void set_console_color();
void clear_console();
int read_console_key();
```

That is the whole interface. Notice what it does not contain: no color code, no terminal handle, no
error result, no capability query. The interface is narrow, and the narrowness is both its strength
and its limit.

### What the adapter hides

The adapter hides **where the platform knowledge lives**, not the knowledge itself.

| Item | Where it went | What the caller gained |
|---|---|---|
| `#include<conio.h>` | deleted | The caller compiles on a POSIX system. |
| `#include<iostream.h>` | not addressed | Nothing; this is a dialect problem, not a console problem. |
| `system("color 1f")` | `set_console_color()` | The theme code is inside the adapter, not the caller. |
| `system("cls")` | `clear_console()` | Clear semantics are now the adapter's promise. |
| `getch()` | `read_console_key()` | Key semantics are now a name, not a guarantee. |
| `void main()` | `int main()` in the bench | A standard entry point, in the bench only. |

Read the second row again. The adapter does not touch `iostream.h`, because the bench is written in
modern C++. The archive still includes it. A console adapter is not a language migration, and this
lesson will not pretend otherwise.

The last row is the same warning. The bench uses `int main()` because that is what a standard
compiler accepts. The archive's `void main()` is a separate defect, diagnosed in `archaeology-04`.
The adapter cannot fix a function signature it never sees.

What the adapter really does is turn a **compile-time dependency** into a **runtime dependency**:

| Construct | Fails when | Failure mode |
|---|---|---|
| `#include<conio.h>` | POSIX, macOS, modern MSVC | Compile: file not found. |
| `#include<iostream.h>` | Modern standard library | Compile: file not found. |
| `void main()` | Standard C++ | Compile warning, or error under `-Werror`. |
| `getch()` | Non-Windows | Compile: undeclared identifier. |
| `system("color 1f")` | Non-Windows shell | Runtime: command not found; status ignored. |
| `system("cls")` | Non-Windows shell | Runtime: command not found; status ignored. |

The first four are visible the moment you build. The last two are not. `system()` is standard C++;
the *string* is the platform dependency. On a POSIX shell, `system("cls")` starts a shell, fails to
find a command named `cls`, prints an error to stderr, and the shell exits 127; `system()` hands
back a wait status, and the archive ignores it. The program continues as if the screen had been
cleared.

You can see the failure without the archive:

```bash
sh -c 'cls'; printf 'exit=%s\n' "$?"
```

```text
sh: 1: cls: not found
exit=127
```

That is the sharpest argument for the adapter. It does not just make the caller compile; it moves
the platform-specific *command string* into one file where a reviewer can find it.

### What the adapter does not guarantee

The adapter guarantees that the caller compiles. It does not guarantee that the terminal behaves.
Here are the assumptions it makes without asking.

#### 1. Escape support

`platform.cpp` writes ANSI escape sequences:

```cpp
std::printf("\033[1;37;44m");   // set_console_color
std::printf("\033[2J\033[H");   // clear_console
```

ANSI works on most POSIX terminals. On Windows it depends on the console and on virtual terminal
processing being enabled. A legacy `cmd.exe` window that does not process escapes displays the bytes
as text: `←[1;37;44m←[2J←[H`. The adapter has no branch for that case, and no way to detect it.

#### 2. Color capability

`\033[1;37;44m` is an SGR sequence: bold, foreground 37 (white), background 44 (blue). That is a
faithful rendering of the archive's `color 1f` (bright white on blue) on a terminal that supports
16 colors. A monochrome terminal ignores it or approximates it. A terminal with a different palette
shows different blue. The adapter does not ask how many colors exist.

#### 3. Clear semantics

`\033[2J\033[H` clears the visible screen and homes the cursor. `cls` on Windows clears the console
buffer. They are not the same operation: scrollback behavior differs, and some terminals clear only
the viewport. If the program depends on what remains above the fold, the two adapters disagree.

#### 4. Key semantics

This is the largest gap, and the one a name hides best.

| Property | `getch()` (conio) | `std::getchar()` (this adapter) |
|---|---|---|
| Header | `conio.h` | `<cstdio>` |
| Echo | No | Yes, the terminal echoes the key. |
| Waiting | Returns on one key | Waits for Enter in canonical mode. |
| Buffering | Unbuffered console read | Standard I/O buffer. |
| End of input | Not applicable to a console | Returns `EOF`. |
| Redirection | Reads the console | Reads whatever `stdin` is. |

`read_console_key()` is a portable **name**. It is not a portable **interaction**. A caller that
expects "press any key and continue" gets "type a line and press Enter" on a POSIX terminal. The
bench never calls the function, so this difference is invisible in the acceptance run.

#### 5. Redirection

The adapter writes escapes to `stdout` unconditionally. Redirect the program to a file and the file
contains 17 control bytes before the marker:

```text
1b 5b 31 3b 33 37 3b 34 34 6d   ESC [ 1 ; 3 7 ; 4 4 m
1b 5b 32 4a                     ESC [ 2 J
1b 5b 48                        ESC [ H
70 6f 72 74 61 62 6c 65 3d 79 65 73 0a   portable=yes\n
```

A pipe, a log file or a CI transcript receives them too. The program never asks whether `stdout` is
a terminal. `system("cls")` on Windows writes to the console, and under redirection it may do
nothing or fail; the behaviors differ in the other direction as well.

#### 6. Encoding

The archive draws the board with CP437 bytes: `simcab1=21`, `simcab2=156`, `sep=219`. A modern
terminal usually interprets bytes as UTF-8. Byte 219 alone is not valid UTF-8, so the board can
render as replacement characters or mojibake. The adapter does not touch encoding. Swapping
`conio.h` for ANSI does not turn a CP437 program into a UTF-8 program.

#### 7. Output ownership

`main.cpp` still calls `std::printf("portable=yes\n")`. The adapter owns color, clear and key; it
does not own the transcript. That is a deliberate boundary in this bench, and it is a gap if you
want a test adapter to capture output. `archaeology-11` is the lesson for that port.

#### 8. The language dialect

`iostream.h` and `void main` are not console problems. They are compiler problems. The adapter does
not fix them, and a learner who says "the archive is portable now" after adding this adapter has
overclaimed.

### Interface portability versus identity of behavior

These are two different claims, and they are routinely confused.

| Claim | Meaning | How you prove it |
|---|---|---|
| **Interface portability** | The same caller source compiles and links against an implementation on each target. | Build the same `main.cpp` with two adapters. |
| **Behavior identity** | The same observable interaction: bytes, timing, echo, blocking, screen state. | Measure both builds and compare. |

The adapter buys the first by construction. It does not buy the second, and no amount of naming
makes it. A plug adapter lets a plug fit; it does not guarantee the voltage. Here, the shape is the
three function declarations, and the voltage is the terminal.

The measured example is in Step 3. The ANSI adapter writes 30 bytes. A plain adapter writes 13. The
caller is byte-identical in both builds. Same interface, different behavior. That is the lesson in
one comparison.

This is not an argument against adapters. It is an argument for stating the contract precisely:

- "The caller compiles without `conio.h`" is true and useful.
- "The console behavior is portable" is false unless you measured it on each target.

Write the first sentence in the README. Write the second only after the matrix in Step 5.

### The legacy constraint

The code targeted one compiler, one operating system and one console. `conio.h` was the available
key reader, `system("color 1f")` the shortest color path, and `system("cls")` the screen
management the target offered. Those were local choices for a local program.

The requirement changed when the artifact became public. A published artifact is built on other
platforms, and a build that only works on one machine is not portable. The defect is that the
artifact never named its platform, so the boundary was missing until the code left the machine.

The same thing happens today when a prototype becomes a product.

## Before you start

Read these in full:

- [`README.md`](README.md) — the lesson card, the build command and the acceptance criterion.
- [archaeology-01](../archaeology-01-build-today/lesson.md) — the prerequisite: what it takes to
  build legacy code today, and what a documented blocker looks like.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §5, the severity scale; §9, the Track A mapping.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive, tag and commit.

You also need:

- a local clone of the archive at tag `v1.0.0` (for the anchors; the bench runs without it);
- a C++20 compiler (`c++`, `g++` or `clang++`);
- `bash`, `grep`, `od` and `cat -v`;
- a scratch directory outside `original/`.

> **Rule:** the archive is read-only. Copy any file you want to change into a scratch directory;
> never edit `original/`. The bench in this lesson is also evidence: copy it before you edit.

## The 60-minute path

| Min | Step | What you do | Evidence |
|---:|---|---|---|
| 0–8 | Predict | Commit to answers about headers, bytes and key behavior | Prediction table |
| 8–20 | Inspect | Read the three files and the archive anchors; run the baseline | Line counts and anchor table |
| 20–32 | Isolate | Copy the bench, name the seam, write a plain adapter | `platform-plain.cpp` |
| 32–42 | Compile | Build both callers, inspect bytes and symbols | 30 vs 13 bytes, `nm` output |
| 42–55 | Explain | Fill the capability matrix and price the lock-in | Matrix and cost note |
| 55–60 | Record | Fill `EVIDENCE.md`; confirm the archive is clean | Portfolio entry |

If the `getchar` versus `getch` difference takes fifteen minutes to
demonstrate, that investigation is the lesson.

### The engineering check in this lesson

Track B uses the same habit as Track A, with a different verb: the behavior already exists, so you
**freeze** what it does before you move it.

| Step | Where it happens | What it produces |
|---|---|---|
| **Predict** | Step 1 | A falsifiable guess about the build, the bytes and the key. |
| **Inspect** | Step 2 | The three files, the archive anchors and the baseline run. |
| **Isolate** | Step 3 | The seam named, and a second adapter behind the same header. |
| **Compile** | Step 4 | Two builds, one caller, measured bytes and symbols. |
| **Explain** | Step 5 | The capability matrix, the two claims and the cost. |
| **Record** | Step 6 | `EVIDENCE.md` with commands, bytes and boundaries. |

## Step 1 — Predict (8 min)

Before you open the source, answer these five questions in writing. The point is to commit to
answers you can be wrong about.

1. Does `code/main.cpp` include any platform header? Which includes do you expect?
2. Does the output of `./run.sh` contain escape bytes? Does the output of the program itself?
3. If you replace `platform.cpp` with an adapter that does nothing, does `main.cpp` still compile?
4. Does `std::getchar()` behave like `getch()`? Name one difference.
5. If you redirect the program to a file, how many bytes does the file contain?

A useful prediction is falsifiable. "The adapter is portable" is not. "`main.cpp` includes only
`platform.h` and `<cstdio>`, the binary writes 17 escape bytes before the marker, and the harness
captures them but prints only the marker" is.

### A worked prediction

Before reading, I would write:

- `main.cpp` includes `platform.h` and `<cstdio>`; no `conio.h`, no `windows.h`;
- `./run.sh` prints one clean line, because it captures stdout and prints a marker; the program
  itself writes the escapes, so the capture contains them;
- a no-op adapter links, because the caller uses only the interface;
- `getchar` echoes and waits for Enter; `getch` does neither;
- the file contains 30 bytes: 17 escape bytes, `portable=yes` and a newline.

I am less sure about `TERM=dumb`. My guess is that the adapter ignores it, because the source never
reads `TERM` and never calls `isatty`. The next steps check that guess.

Every claim can be checked in the next four steps. If your prediction says "adapters are good", you
have made a judgment, not an estimate. Write the mechanism you expect, then test it.

## Step 2 — Inspect (12 min)

### Read the three files

```bash
cd archaeology/lessons/archaeology-16-platform-lock-in
wc -l code/platform.h code/platform.cpp code/main.cpp run.sh
nl -ba code/platform.h
nl -ba code/platform.cpp
nl -ba code/main.cpp
nl -ba run.sh
```

The bench is 34 lines of C++ plus a 19-line harness:

| File | Lines | What it is |
|---|---:|---|
| `code/platform.h` | 5 | The port: three declarations. |
| `code/platform.cpp` | 18 | The adapter: ANSI escapes and `std::getchar`. |
| `code/main.cpp` | 11 | The caller: two calls and one marker. |
| `run.sh` | 19 | Build, capture, substring check, exit code. |

Read `platform.h` first. It is five lines, and the important fact is what is missing: no color code,
no handle, no capability query, no error return. The port says "set a color", not "set color 1f".

Read `platform.cpp`. It includes `platform.h` and `<cstdio>`, and nothing else. `set_console_color`
writes one SGR sequence, `clear_console` writes two escape sequences, and `read_console_key` returns
`std::getchar()`. There is no `#ifdef`, no `conio.h`, no `system`.

Read `main.cpp`. It includes `platform.h` and `<cstdio>`, calls `set_console_color()` and
`clear_console()`, prints `portable=yes`, and returns 0. It does not call `read_console_key`.

That last observation is a fact about the bench, not an accident. The acceptance run exercises two
of the three functions. Keep it for the evidence boundary.

Read `run.sh`. It compiles both sources with:

```text
-std=c++20 -Wall -Wextra -Werror -O2
```

Then it captures stdout into `output` and checks a substring:

```bash
output="$("${binaryPath}")"
if [[ "${output}" != *"portable=yes"* ]]; then
    printf 'adapter=no\n'
    exit 1
fi
printf 'adapter=portable\n'
```

The substring check is deliberate. The capture begins with escape bytes, so an equality check
against `portable=yes` would fail. The harness tolerates the escapes and reports a clean marker.
That is a reasonable harness for a build check, and it is not a terminal test.

### The archive anchors

In the pinned archive at tag `v1.0.0`:

```bash
grep -nE 'conio|iostream\.h|void +main|system\("color|system\("cls"|getch\(\)' \
    original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

The output, with the counts that matter:

| Location | What is there |
|---:|---|
| 11 | `#include<conio.h>` |
| 12 | `#include<iostream.h>` |
| 32 | `void  main()` |
| 38 | `system("color 1f")` |
| 42 | `getch()` |
| 43 | `system("cls")` |
| 781, 806, 835, 861 | `getch()` in the turn loop |
| 784, 809, 838, 864 | `system("cls")` in the turn loop |

Count them yourself:

```bash
grep -c 'getch' original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
grep -c 'system("cls")' original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
grep -c 'system("color' original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

The answers are 5, 5 and 1. One `conio.h`, one `iostream.h`, one `void main`, one color command,
five key reads and five clears. That is the whole platform surface of a 930-line program.

The other variants carry the same shape, with one instructive exception:

| Variant | Entry point | `getch` | `system("cls")` | `system("color")` |
|---|---|---:|---:|---:|
| `variant-00-precursor-2007` | `int main()` at 359 | 0 | 2 | 1 |
| `variant-01-baseline-2008` | `void main()` at 32 | 5 | 5 | 1 |
| `variant-02-records-2008` | `void main()` at 37 | 5 | 5 | 1 |
| `variant-03-pointers-dev-2008` | `int main()` at 47 | 5 | 5 | 1 |
| `variant-04-pointers-final-2008` | `void main()` at 52 | 5 | 5 | 1 |

The precursor reads `cin>>caracter` at line 370 instead of `getch()`. That is *more* portable in the
input dimension, and it is still locked in color and clear. A partial fix is not a port. It also
shows the difference between the two failures: `cin` compiles everywhere and waits for Enter;
`getch` does not compile off Windows.

### What the archive says about itself

`05-known-issues.md` records both rows in its own words:

> | Windows console command | `variant-00:363`, `variant-01:38`, ... | Platform lock-in. |
> | Pre-standard headers | All variants, lines 11–12 | `iostream.h` and `conio.h` are not modern
> standard headers. |

The lab is not inventing the diagnosis. It is quoting the archive and then asking what an adapter
can and cannot do about it.

### Check your prediction

- `main.cpp` includes `platform.h` and `<cstdio>`; no platform header.
- `./run.sh` prints one clean line; the program writes 17 escape bytes before the marker.
- A no-op adapter links, because the caller uses only the interface.
- `getchar` echoes and waits for Enter; `getch` does neither.
- The file contains 30 bytes, not 13.
- `TERM=dumb` changes nothing, because the adapter never reads it.

If your prediction said the harness output would contain escapes, you missed the capture. If it said
the adapter detects the terminal, you assumed a capability check that the source does not have. Both
gaps are worth writing down.

## Step 3 — Isolate (12 min)

### Copy, do not edit

Create a scratch directory outside `original/`. `build/` is ignored by this repository:

```bash
mkdir -p build/scratch
cp code/platform.h code/platform.cpp code/main.cpp build/scratch/
```

From here on, edit the files in `build/scratch/`. Do not edit `code/`; it is the baseline the next
learner will run.

### Name the seam

The seam is `platform.h`. Everything above it is portable C++ that knows nothing about a terminal.
Everything below it is terminal knowledge. Count the surface:

| Side of the seam | Lines | What it knows |
|---|---:|---|
| `main.cpp` | 11 | Two calls and one marker. |
| `platform.h` | 5 | Three names, no mechanism. |
| `platform.cpp` | 18 | ANSI bytes and `stdin`. |

The seam is three functions wide. That is small enough to swap in one step, which is the point.

### Write a second adapter

In `build/scratch/platform-plain.cpp`:

```cpp
#include "platform.h"

#include <cstdio>

void set_console_color()
{
}

void clear_console()
{
}

int read_console_key()
{
    return std::getchar();
}
```

Compile it with the *same* caller:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 \
    build/scratch/platform-plain.cpp build/scratch/main.cpp \
    -o build/scratch/app-plain
./build/scratch/app-plain
```

Expected:

```text
portable=yes
```

The caller did not change. Not one byte. That is **interface portability**, demonstrated by a build.

Now measure the difference:

```bash
./build/scratch/app-plain > build/scratch/plain.txt
wc -c < build/scratch/plain.txt
```

The file is 13 bytes: `portable=yes` and a newline. The ANSI build wrote 30. Same caller, different
behavior. That is the whole lesson in one comparison.

### Optional: widen the port

If the theme must be selectable, the port needs a parameter. Add:

```cpp
void set_console_color(int code);
```

Then update both adapters and the caller. Count the touched files: four — the header, two adapters
and `main.cpp`. The current port hides `system("color 1f")` by baking the argument into the adapter.
That is a **policy leak**: the choice of theme moved into the implementation, where the caller
cannot see it. It is not a portability win, and it is worth naming in the record.

If you do not widen the port, say why. "One theme is enough for this bench" is a valid scope
statement. An unstated assumption is not.

### What you did not do

You did not make the archive compile. You did not write a Windows adapter. You did not test a real
terminal. You did not fix `iostream.h`, `void main` or the CP437 glyphs. Write those four sentences
in the evidence. A port that claims more than it did is worse than a port that names its scope.

## Step 4 — Compile (10 min)

### Build the baseline

```bash
./run.sh
```

Expected:

```text
adapter=portable
```

The exit code is 0. Read the flags in `run.sh` before you trust the result:

- `-std=c++20` fixes the dialect;
- `-Wall -Wextra` asks for the warnings;
- `-Werror` makes a warning a build failure;
- `-O2` optimizes, which matters for the symbol check below.

`-Werror` matters. A bench that only builds with warnings suppressed is not a portable build; it is
a build that has not been read.

### Look at the bytes

```bash
./build/platform-adapter | cat -v
```

Expected:

```text
^[[1;37;44m^[[2J^[[Hportable=yes
```

`cat -v` makes the escapes visible. Without it, the terminal interprets them and you see a blue
screen with one line, which is exactly the illusion the adapter sells.

Now write the bytes to a file:

```bash
./build/platform-adapter > build/out.txt
od -An -tx1c build/out.txt
wc -c < build/out.txt
```

Decoded by hand from the `od` output:

| Part | Bytes | What it is |
|---|---:|---|
| `\033[1;37;44m` | 10 | SGR: bold, white on blue. |
| `\033[2J` | 4 | Erase the display. |
| `\033[H` | 3 | Home the cursor. |
| `portable=yes\n` | 13 | The marker and a newline. |
| **Total** | **30** | |

Seventeen of the thirty bytes are terminal control. The program's actual message is thirteen. If you
pipe the output to `grep`, `wc` or a log file, all thirty arrive. The adapter never asks whether the
destination is a terminal.

### Prove the caller has no platform header

```bash
grep -n '#include' code/main.cpp code/platform.h code/platform.cpp
```

Expected:

| File | Includes |
|---|---|
| `code/main.cpp` | `"platform.h"`, `<cstdio>` |
| `code/platform.h` | none |
| `code/platform.cpp` | `"platform.h"`, `<cstdio>` |

No `conio.h`. No `windows.h`. No `system`. No `getch`. The caller's only dependency is the port and
the standard library. That is the mechanical claim the lesson can defend.

### Prove it at the object level

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 -c code/platform.cpp -o build/platform.o
nm -C -u build/platform.o
```

On this platform, at `-O2`:

```text
                 U getc
                 U printf
                 U stdin
```

At `-O0`, the same source reports `getchar` and `printf`. The compiler is allowed to rewrite
`getchar` to `getc` and a constant `printf` to `puts`. Do not grep the object file for the source
name and conclude that the call disappeared. The source is the contract; `nm` is corroboration.

Filter for the platform symbols:

```bash
nm -C -u build/platform.o | grep -E 'conio|getch|system|clrscr'
```

The command prints nothing and exits 1. That is the result you want: the object file has no
undefined symbol from the Windows console API. `getc` and `stdin` are standard C; they are not a
platform lock.

### Exercise the unexercised port

```bash
grep -rn 'read_console_key' code/
```

Expected: two hits, the declaration in `platform.h` and the definition in `platform.cpp`. Zero
calls. The portable build never reads a key.

So the acceptance criterion "portable build" proves that the caller compiles and that two functions
run. It does not prove that `read_console_key` works, that it waits the way `getch` waited, or that
the terminal echoes or does not echo. Say that in the record. It is not a defect in the bench; it is
the edge of the evidence.

### Ask the environment

```bash
TERM=dumb ./build/platform-adapter | cat -v
```

The bytes are identical. The adapter never reads `TERM`, never calls `isatty`, and never asks
whether the terminal supports color. That is the difference between "runs here" and "adapts here". A
real adapter for a hostile environment needs a capability check or a terminal library; this one has
neither.

If you want the sharp version, compare the two builds:

```bash
diff <(./build/platform-adapter) <(./build/scratch/app-plain)
```

`diff` exits 1 and shows the escape bytes on one side and the marker on the other. Same interface,
different behavior. Keep that output.

## Step 5 — Explain (13 min)

### The bill in 2008

| Cost | Where | Consequence |
|---|---|---|
| `#include<conio.h>` | `variant-01:11` | The build fails on every non-Windows target. |
| `#include<iostream.h>` | `variant-01:12` | The build fails on every modern standard library. |
| `void main()` | `variant-01:32` | Not standard; accepted by some compilers, rejected under `-Werror`. |
| `system("color 1f")` | `variant-01:38` | Runtime command; fails silently where `color` does not exist. |
| `getch()` | `variant-01:42`, 781, 806, 835, 861 | Compile-time lock; key timing differs elsewhere. |
| `system("cls")` | `variant-01:43`, 784, 809, 838, 864 | Runtime command; ignored exit status. |

The first three rows stop the build. The last three let the build succeed and change the behavior.
That is why "it compiles now" is not the end of the diagnosis.

The cost is paid by the audience the archive was published for. A reader who clones the repository
on Linux cannot build the artifact at all. For a published archive, that is a high-severity defect:
not a wrong pixel, a failed build. For a one-machine demo, the same code is defensible. Severity is
about consequence in a context, and the context is the one you write down.

### Interface portability, measured

| Question | Interface portability | Behavior identity |
|---|---|---|
| Does the same `main.cpp` compile? | Yes, by construction. | Not implied. |
| Are the output bytes the same? | No. | Yes, if you measure both. |
| Is the key timing the same? | No. | No: Enter versus one key. |
| Is the screen state the same? | No. | Depends on the terminal. |
| Is the color the same? | No. | Depends on the palette. |
| Is the exit code the same? | Yes, for this bench. | Yes, for this bench. |

The table is the answer to the lesson's central question. The adapter converts a build dependency
into a runtime dependency. That is progress: a build failure is total, and a runtime difference can
be detected, documented and, if necessary, adapted. It is not identity.

### Two fixes, two sizes

**Minimal fix — three functions in one translation unit.** Replace the console calls in the caller
with `set_console_color`, `clear_console` and `read_console_key`; put the platform code in
`platform.cpp`; keep the caller free of `conio.h`. That is the bench. It removes the compile-time
lock and gives a reviewer one file to audit.

**Structural fix — a console port with a contract.** State the capabilities the port assumes, add a
no-console adapter for tests and pipes, and let the caller choose the adapter. If the program needs
real capability negotiation, use a terminal library (`ncurses`, `PDCurses`) instead of hand-written
escapes. Keep the theme and the pause policy in the caller or a presentation layer, not inside the
adapter. Then a test can link the no-op adapter and assert the caller's behavior without a terminal.

Track A links: `construction-08` (module boundary) is the prerequisite; `construction-20`
(hexagonal, ports and adapters) is the direct rung; `common-05` (golden traces) is how you would
prove that the console output did not change when you swapped the adapter.

### What the adapter does not fix

- **The language dialect.** `iostream.h` and `void main` remain in the archive.
- **The encoding.** The CP437 bytes 21, 156 and 219 are still not UTF-8.
- **The output port.** `main.cpp` still prints the marker itself; there is no test adapter for text.
- **The key contract.** `std::getchar` is not `getch`; the name does not make it so.
- **Capability detection.** The adapter never asks what the terminal can do.
- **The archive.** None of this touches `original/`.
- **The tests and the seed.** `archaeology-14` owns that gap.
- **Validation and error handling.** The port returns no error, and the caller checks nothing.

### The tradeoff, stated fairly

An adapter costs a header, a translation unit and one indirection. A one-target console demo does
not need it: `system("color 1f")` is shorter, and there is no second target to justify the file. The
cost appears at the second target — a CI runner, a macOS reader, a test harness, a pipe. At that
moment the direct call charges the full price: the build fails, and the failure is in the caller,
mixed with the game.

Choose with the change test, not with taste: add one target, count the files that must change. If
the answer is "one adapter", the port paid for itself. If the answer is "every file that prints",
it did not.

## Step 6 — Record (5 min)

Fill [`EVIDENCE.md`](EVIDENCE.md). At minimum:

- setup: compiler, libc, kernel, archive tag and commit;
- the five predictions, written before inspection;
- the line counts and the three-function port;
- the archive anchors: lines 11–12, 32, 38, 42–43, and the 5 `getch` / 5 `cls` counts;
- the baseline run, its exit code and the 30 bytes with the 17 escape bytes named;
- the plain-adapter build: same caller, 13 bytes, no escapes;
- the object-symbol check and the `getc` / `getchar` optimization note;
- the `read_console_key` count: two hits, zero calls;
- the `TERM=dumb` probe and what it does not change;
- the capability matrix: escape, color, clear, key, redirection, encoding;
- the distinction between interface portability and behavior identity, with the 30/13 measurement;
- the evidence boundary: the marker proves the build and the call path, not the terminal;
- the archive integrity check.

A sentence such as "the adapter makes the console portable" is an opinion. "The same `main.cpp`
links against `platform.cpp` and `platform-plain.cpp`; the first writes 30 bytes, the second 13; the
caller source is unchanged; `nm -C -u platform.o` has no `conio`, `getch` or `system`" is a record.

The acceptance criterion for this lesson is two facts:

```text
adapter=portable
```

and a second build where the same caller links a different adapter. If you cannot produce both, the
unticked line names the next step. There is no penalty for repeating one.

## The original in detail

Facts you can cite, with line numbers from `variant-01` at tag `v1.0.0`:

| Location | What is there |
|---:|---|
| 11 | `#include<conio.h>` — the console API. |
| 12 | `#include<iostream.h>` — the pre-standard iostream. |
| 32 | `void  main()` — no standard return type. |
| 38 | `system("color 1f")` — the theme, via a shell command. |
| 42 | `getch()` — the first pause. |
| 43 | `system("cls")` — the first clear. |
| 781, 806, 835, 861 | `getch()` — each turn waits for a key. |
| 784, 809, 838, 864 | `system("cls")` — each turn clears the screen. |
| 930 | End of file; no build file, no test, no platform note. |

The other variants repeat the pattern. `variant-00` is the interesting contrast: it uses
`int main()` at 359 and `cin>>caracter` at 370, so it is more portable in the entry point and the
input, and it still calls `system("color 1f")` at 363 and `system("cls")` at 373. The pointer
variants changed the container; they did not change the console. That is worth noticing: a syntax
change is not a platform port.

The archive's `05-known-issues.md` records the two rows this lesson quotes: the Windows console
command at `variant-00:363`, `variant-01:38`, `variant-02:43`, `variant-03:53` and `variant-04:59`,
and the pre-standard headers at lines 11–12 in every variant.

## What this bench proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| The caller compiles without `conio.h`. | Yes | It includes only `platform.h` and `<cstdio>`. |
| The caller links against two different adapters. | Yes | `platform.cpp` and `platform-plain.cpp`. |
| The adapter object has no console-API symbol. | Yes | `nm -C -u` finds no `conio`, `getch` or `system`. |
| The output bytes are identical across adapters. | No | 30 versus 13 bytes. |
| `read_console_key` works like `getch`. | No | It is never called, and `getchar` differs. |
| The adapter detects terminal capabilities. | No | It never reads `TERM` or calls `isatty`. |
| The archive builds now. | No | The bench is a separate 34-line program. |
| The archive's encoding is fixed. | No | The CP437 bytes are untouched. |
| The archive's `void main` is fixed. | No | The bench uses `int main`; the archive does not. |
| The harness tests the terminal. | No | It checks a substring in captured stdout. |
| The adapter is free. | No | It costs a header, a unit and an indirection. |

This table is the honesty check. A record that says "the console is portable" without naming the
terminal, the adapter and the bytes is overreaching. Say "the caller compiles against a console
port; the ANSI adapter writes 30 bytes and the plain adapter writes 13; key behavior is unverified"
when that is what you ran.

## Self-check (binary)

- [ ] I wrote the five predictions before opening the source.
- [ ] I read `platform.h`, `platform.cpp`, `main.cpp` and `run.sh`.
- [ ] I recorded the line counts: 5, 18, 11 and 19.
- [ ] I ran `./run.sh` and saw `adapter=portable` with exit code 0.
- [ ] I found the six archive constructs: `conio.h`, `iostream.h`, `void main`, `system("color")`,
  `system("cls")` and `getch()`.
- [ ] I counted 5 `getch` and 5 `system("cls")` calls in `variant-01`.
- [ ] I compared `variant-00` and named what it does better and what it still shares.
- [ ] I copied the bench into `build/scratch/` before editing.
- [ ] I wrote `platform-plain.cpp` and linked the unchanged `main.cpp` against it.
- [ ] I measured 30 bytes for the ANSI adapter and 13 for the plain adapter.
- [ ] I used `cat -v` or `od` to show the 17 escape bytes.
- [ ] I proved the caller has no platform header with `grep` and `nm`.
- [ ] I explained the `getc` / `getchar` optimization and did not trust a name grep.
- [ ] I found that `read_console_key` has two hits and zero calls.
- [ ] I ran `TERM=dumb` and recorded that the bytes did not change.
- [ ] I distinguished interface portability from behavior identity, with the 30/13 example.
- [ ] I filled the capability matrix: escape, color, clear, key, redirection, encoding.
- [ ] I named what the adapter does not fix: dialect, encoding, output, key, detection, archive.
- [ ] I priced the lock-in by build failure and stated my context.
- [ ] I linked the structural fix to `construction-08`, `construction-20` and `common-05`.
- [ ] The archive is unmodified (`git status` in the archive is clean).

If a box is not ticked, the unticked box names the next step. There is no penalty for repeating one.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| "It compiles on Linux, so it is portable." | The build is portable; the behavior may not be. | Measure the bytes and the key interaction. |
| "The adapter removed the platform dependency." | It moved the dependency behind a name. | Say where the platform code lives now. |
| "ANSI escapes work everywhere." | Legacy Windows consoles and `TERM=dumb` show the bytes as text. | Detect the terminal or use a library. |
| "`getchar` is a portable `getch`." | Echo, buffering and blocking differ. | State the key contract; test it. |
| "Escapes are harmless in a pipe." | A file receives 17 control bytes. | Ask whether `stdout` is a terminal. |
| "`system("cls")` and `ESC[2J` are the same." | Console buffer, viewport and scrollback differ. | Define the clear contract. |
| "If `run.sh` passes, the console works." | It checks a substring and never reads a key. | Test the port, not only the build. |
| "The adapter fixes `void main` and `iostream.h`." | It is a console port, not a language migration. | Fix the dialect separately. |
| "The adapter fixes the CP437 glyphs." | Encoding is a separate layer. | Convert the glyphs explicitly. |
| "A port is a rewrite." | This port is three functions wide. | Start with the seam, not the program. |
| "Portability is free." | It costs a header, a unit and an indirection. | Price it with the change test. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Platform lock-in | A correct build or run depends on one environment. |
| Port | The shape of a conversation the caller is allowed to have. |
| Adapter | One implementation of a port for one environment. |
| Interface portability | The same caller source compiles against each adapter. |
| Behavior identity | The same observable interaction on each target. |
| Capability | What a terminal can actually do: color, clear, key timing. |
| ANSI escape | A control sequence beginning with `ESC` (`0x1b`). |
| SGR | Select Graphic Rendition: the `ESC[...m` color sequence. |
| Canonical mode | Line-buffered input that waits for Enter. |
| Echo | The terminal printing the key you pressed. |
| Redirection | Sending `stdout` to a file or pipe instead of a terminal. |
| CP437 | The 1981 IBM code page used by the archive's glyph bytes. |
| Virtual terminal | Windows support for ANSI escape processing. |
| No-op adapter | An implementation that satisfies the port and does nothing. |
| Evidence boundary | The line between what the run proves and what you infer. |

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- Continue to `archaeology-17` — code review of a legacy function. The
  archive's own labels are part of the evidence, and a platform note is a label too.
- If the key contract surprised you, write a three-line program that calls `getchar` with and
  without a pipe, and record when it returns `EOF`.
- If you want the Windows side, add a `platform-win32.cpp` behind `#ifdef _WIN32` in a scratch copy.
  Count the touched files. If the answer is one, the port was honest.
- If the encoding interests you, print bytes 21, 156 and 219 as UTF-8 and compare the glyphs.
- If you disagree with the severity, write the context that changes it. A one-machine demo and a
  published archive do not price the same lock-in.

## References

- [`README.md`](README.md) — the lesson card, the build command and the acceptance criterion.
- [`code/platform.h`](code/platform.h) — the port: three declarations.
- [`code/platform.cpp`](code/platform.cpp) — the ANSI adapter.
- [`code/main.cpp`](code/main.cpp) — the caller.
- [`run.sh`](run.sh) — compiles and checks the marker with an exit code.
- [`EVIDENCE.md`](EVIDENCE.md) — the portfolio record you fill in.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §5, the severity scale; §9, the Track A mapping.
- [`../../../docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the lesson
  sequence and the `archaeology-16` acceptance criterion.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive, tag and commit.
- [archaeology-01](../archaeology-01-build-today/lesson.md) — the prerequisite: building the archive
  today.
- [archaeology-04](../archaeology-04-correctness-failures/lesson.md) — `void main` as a correctness
  defect.
- [archaeology-11](../archaeology-11-input-output/lesson.md) — the input/output port this lesson
  complements.
- [construction-08](../../../construction/lessons/construction-08-modular/lesson.md) — the module
  boundary.
- [construction-20](../../../construction/lessons/construction-20-hexagonal/lesson.md) — ports and
  adapters.
- [common-05](../../../common/lessons/common-05-golden-traces/lesson.md) — golden traces.
- Archive: <https://github.com/DaydreamsDev/DaydreamsDev-guerra-de-caballos-archive>, tag `v1.0.0`,
  commit `284e144e41403afbf0cc44422876425cacfb2bd5`.
