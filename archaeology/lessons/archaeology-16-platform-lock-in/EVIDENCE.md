# archaeology-16 — Evidence: platform lock-in

> This is your record. Write it for your future self.
> Record what you observed, not what you expected the lesson to want.

## 1. Setup

- Date:
- Archive path:
- Tag checked out:
- Commit hash (`git rev-parse HEAD`):
- Compiler and version (`c++ --version`):
- C library (`ldd --version` first line, or `getconf GNU_LIBC_VERSION`):
- Shell and version:
- Kernel and distribution (`uname -sr`):
- Terminal and `TERM`:
- Working directory:
- Scratch directory used:
- Anything unexpected in the setup:

## 2. Prediction — write before inspecting the source

Do not edit this table after Step 2. The gap between prediction and observation is the learning
signal; repairing the prediction destroys it.

| # | Question | My prediction | How I would know I am wrong |
|---:|---|---|---|
| 1 | Does `main.cpp` include a platform header? | | |
| 2 | Does `./run.sh` output contain escape bytes? | | |
| 3 | Does a no-op adapter link the same caller? | | |
| 4 | Does `std::getchar()` behave like `getch()`? | | |
| 5 | How many bytes does a redirected run write? | | |

One falsifiable sentence about portability in this bench:

>

## 3. Inspection of the baseline

Files:

```text
code/platform.h
code/platform.cpp
code/main.cpp
run.sh
```

Line counts:

| File | Lines | Verified? |
|---|---:|---|
| `code/platform.h` | 5 | |
| `code/platform.cpp` | 18 | |
| `code/main.cpp` | 11 | |
| `run.sh` | 19 | |

The port:

| Function | Declared in | Implemented in | Called by `main.cpp`? |
|---|---|---|---|
| `set_console_color` | `platform.h:3` | `platform.cpp:5-8` | |
| `clear_console` | `platform.h:4` | `platform.cpp:10-13` | |
| `read_console_key` | `platform.h:5` | `platform.cpp:15-18` | |

Includes:

| File | Includes | Platform header? |
|---|---|---|
| `code/main.cpp` | | |
| `code/platform.h` | | |
| `code/platform.cpp` | | |

What `main.cpp` calls:

What `main.cpp` does not call:

How `run.sh` captures stdout:

What the substring check tolerates:

What the harness does not test:

## 4. Inspection of the archive

Anchors I verified in
`original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`:

| Item | Line or count | Verified? |
|---|---:|---|
| `#include<conio.h>` | 11 | |
| `#include<iostream.h>` | 12 | |
| `void  main()` | 32 | |
| `system("color 1f")` | 38 | |
| `getch()` in `main` | 42 | |
| `system("cls")` in `main` | 43 | |
| `getch()` in the turn loop | 781, 806, 835, 861 | |
| `system("cls")` in the turn loop | 784, 809, 838, 864 | |
| `getch` calls in the file | 5 | |
| `system("cls")` calls in the file | 5 | |
| `system("color")` calls in the file | 1 | |
| Lines in the file | 930 | |

The other variants:

| Variant | Entry point | `getch` | `system("cls")` | `system("color")` |
|---|---|---:|---:|---:|
| `variant-00-precursor-2007` | `int main()` at 359 | | | |
| `variant-01-baseline-2008` | `void main()` at 32 | 5 | 5 | 1 |
| `variant-02-records-2008` | `void main()` at 37 | 5 | 5 | 1 |
| `variant-03-pointers-dev-2008` | `int main()` at 47 | 5 | 5 | 1 |
| `variant-04-pointers-final-2008` | `void main()` at 52 | 5 | 5 | 1 |

What `variant-00` does better than `variant-01`:

What `variant-00` still shares with `variant-01`:

What the archive's `05-known-issues.md` says about the console and the headers:

>

Compile-time lock-in versus runtime lock-in:

| Construct | Fails at build or run? | What the failure looks like |
|---|---|---|
| `#include<conio.h>` | | |
| `#include<iostream.h>` | | |
| `void main()` | | |
| `getch()` | | |
| `system("color 1f")` | | |
| `system("cls")` | | |

The shell probe:

```bash
sh -c 'cls'; printf 'exit=%s\n' "$?"
```

Observed output:

- Exit code:
- What the archive does with that exit code:

## 5. Baseline run

Command:

```bash
cd archaeology/lessons/archaeology-16-platform-lock-in
./run.sh
```

Observed output:

```text
adapter=portable
```

- Exit code:
- Compiler used:
- Flags observed in `run.sh`:
- Did the build pass with `-Werror`? If not, what diagnostic appeared?

The bytes:

```bash
./build/platform-adapter | cat -v
./build/platform-adapter > build/out.txt
od -An -tx1c build/out.txt
wc -c < build/out.txt
```

- `cat -v` output:
- `od` output:
- Total bytes:
- Bytes that are terminal control:
- Bytes that are the message:

| Part | Bytes | What it is |
|---|---:|---|
| `\033[1;37;44m` | 10 | |
| `\033[2J` | 4 | |
| `\033[H` | 3 | |
| `portable=yes\n` | 13 | |
| **Total** | **30** | |

- Did `run.sh` show the escapes? Why or why not?
- What a reader of `run.sh` output would wrongly conclude:

## 6. The second adapter

Scratch path:

```text
build/scratch/
```

Files copied:

The plain adapter I wrote:

```cpp

```

Compile command:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 \
    build/scratch/platform-plain.cpp build/scratch/main.cpp \
    -o build/scratch/app-plain
```

- Build result:
- Observed output:
- Exit code:
- Bytes in the output:
- Did `main.cpp` change? (It must be no.)

The comparison:

```bash
diff <(./build/platform-adapter) <(./build/scratch/app-plain)
```

- `diff` output:
- `diff` exit code:
- What changed:
- What did not change:
- Which claim this proves: interface portability or behavior identity?

Optional widened port:

- Did I add a color parameter? Why or why not?
- Files touched:
- What the narrow port hides about the theme:

What I did not do:

- The archive still does not compile because:
- I did not write a Windows adapter because:
- I did not test a real terminal because:
- I did not fix the encoding because:

## 7. Symbol and include proof

Includes:

```bash
grep -n '#include' code/main.cpp code/platform.h code/platform.cpp
```

Observed:

Object symbols:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 -c code/platform.cpp -o build/platform.o
nm -C -u build/platform.o
```

Observed at `-O2`:

Observed at `-O0`:

Why the names differ:

The filter:

```bash
nm -C -u build/platform.o | grep -E 'conio|getch|system|clrscr'
```

- Output:
- Exit code:
- What it proves:

The unexercised port:

```bash
grep -rn 'read_console_key' code/
```

- Hits:
- Calls:
- What the acceptance run therefore does not prove:

## 8. Terminal capability matrix

Fill this from your environment and from what the source assumes. Mark `measured` when you ran it,
`assumed` when you inferred it, and `unknown` when you did not check.

| Environment | Escape | Color | Clear | Key | How I know |
|---|---|---|---|---|---|
| My terminal | | | | | |
| Pipe or file | | | | | |
| `TERM=dumb` | | | | | |
| CI log | | | | | |
| tmux or screen | | | | | |
| Windows 10+ terminal | | | | | |
| Legacy `cmd.exe` | | | | | |

The `TERM=dumb` probe:

```bash
TERM=dumb ./build/platform-adapter | cat -v
```

- Output:
- Did the bytes change?
- Did the adapter read `TERM`?
- Did the adapter call `isatty`?

The key contract:

| Property | `getch()` | `std::getchar()` |
|---|---|---|
| Echo | | |
| Waiting | | |
| Buffering | | |
| End of input | | |
| Redirection | | |

The encoding probe (optional):

- Bytes printed:
- What the terminal showed:
- Why the adapter does not fix it:

## 9. Interface portability versus behavior identity

| Claim | Evidence I have | Supported? |
|---|---|---|
| The same caller compiles against two adapters. | | |
| The two adapters produce the same bytes. | | |
| The two adapters clear the screen the same way. | | |
| The two adapters read a key the same way. | | |
| The caller is free of platform headers. | | |
| The archive is portable now. | | |

- Interface portability, in one sentence:
- Behavior identity, in one sentence:
- The measurement that separates them:
- Which one the adapter buys:
- Which one it does not:

## 10. Explanation

The 2008 cost, in one paragraph. Name the headers, the entry point, the color command and the key
calls. Say what each one prevents, and for whom.

>

Two fixes, two sizes:

- Minimal fix:
- Structural fix:
- Track A links:
- What `construction-08` adds to the question:
- What `construction-20` adds to the question:
- What `common-05` adds to the question:

What the adapter does not fix:

- The language dialect:
- The encoding:
- The output port:
- The key contract:
- Capability detection:
- The archive:
- The tests and the seed:
- Validation and error handling:

The change test:

| Change | Direct calls | Adapter |
|---|---|---|
| Add a second target | | |
| Add a no-console build | | |
| Change the theme | | |
| Add a test that captures output | | |
| Run under a pipe | | |

## 11. Evidence boundary

- Verified by reading the source:
- Verified by compiling:
- Verified by running the harness:
- Verified by measuring bytes:
- Verified by `nm`:
- Inferred:
- Not measured:
- Unknown:

## 12. What the bench proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| The caller compiles without `conio.h`. | | |
| The caller links against two adapters. | | |
| The adapter object has no console-API symbol. | | |
| The output bytes are identical across adapters. | | |
| `read_console_key` works like `getch`. | | |
| The adapter detects terminal capabilities. | | |
| The archive builds now. | | |
| The archive's encoding is fixed. | | |
| The archive's `void main` is fixed. | | |
| The harness tests the terminal. | | |
| The adapter is free. | | |

## 13. Severity and cost

One paragraph. Name the cost in real work, not the ugliness of the code. Say whether the cost is
high or critical in your context and why. Do not propose a rewrite in this paragraph.

>

## 14. Archive integrity

- `git status` in the archive:
- Did I edit `original/`? (It must be no.)
- Did I apply any patch inside the archive? (It must be no.)
- Where did I keep my scratch copy?
- Did I edit `code/`? (It must be no.)

## 15. Acceptance checklist

- [ ] I wrote the five predictions before inspecting the source.
- [ ] I read the three files and `run.sh`, and recorded the 5/18/11/19 line counts.
- [ ] I compiled with the shipped flags and recorded the compiler.
- [ ] I ran `./run.sh` and saw `adapter=portable` with exit code 0.
- [ ] I proved the caller includes only `platform.h` and `<cstdio>`.
- [ ] I copied the bench to `build/scratch/` before editing.
- [ ] I wrote `platform-plain.cpp` and linked the unchanged `main.cpp` against it.
- [ ] I measured 30 bytes for the ANSI adapter and 13 for the plain adapter.
- [ ] I used `cat -v` or `od` to show the 17 escape bytes.
- [ ] I inspected `nm -C -u` and found no `conio`, `getch` or `system`.
- [ ] I explained the `getc` / `getchar` optimization.
- [ ] I recorded that `read_console_key` has two hits and zero calls.
- [ ] I ran `TERM=dumb` and recorded that the bytes did not change.
- [ ] I filled the terminal capability matrix.
- [ ] I stated the `getch` versus `getchar` differences in echo, buffering and blocking.
- [ ] I distinguished interface portability from behavior identity, with the 30/13 example.
- [ ] I recorded the archive anchors: lines 11–12, 32, 38, 42–43, 5 `getch`, 5 `cls`.
- [ ] I named what the adapter does not fix.
- [ ] I priced the defect by build failure and stated my context.
- [ ] I linked the structural fix to `construction-08`, `construction-20` and `common-05`.
- [ ] The archive is unmodified.

## 16. Next step

- Continue to `archaeology-17` — code review of a legacy function.
- If the key contract surprised you, run `getchar` with and without a pipe and record when it
  returns `EOF`.
- If you want the Windows side, add a `platform-win32.cpp` behind `#ifdef _WIN32` in a scratch copy
  and count the touched files.
- If the encoding interests you, print bytes 21, 156 and 219 as UTF-8 and compare the glyphs.
- If you disagree with the severity, write the context that changes it.
