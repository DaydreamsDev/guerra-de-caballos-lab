# archaeology-09 — Missing abstract data type

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** abstract data type · **Prerequisite:** archaeology-08 ·
> **Estimated time:** 90 minutes
> **Reading:** ~19 minutes

## Why this lesson exists

`archaeology-08` ended with a header. A caller could no longer name `visited[0]`, and the linker
joined two translation units. But the header still contained this line:

```cpp
bool visited[cellCount];
```

The representation was still in the caller's build. Changing it to a 64-bit mask forced every
caller to recompile, because the object's size and layout changed. The boundary bought access
control; it did not buy representation independence.

The archive tried to fix that with a type. In `variant-03`:

```text
34:  struct stablero {
35:
36:      int  mvectab;
37:  };
...
42:  struct stablero vectab[64];
44:  struct stablero *pvectab,*pvectab1,*pvectab2;
```

`variant-04` does the same with a different name:

```text
44:  struct  tab {
45:      int  vectab;
46:
47:  };
48:  struct tab tablero[64];
49:  struct tab *ptablero,*ptablero1,*ptablero2;
```

That is a struct with one integer inside. The compiler accepts it as a type. The design does not:
the struct has no operations, no invariant and no hidden representation. A caller still writes
`pvectab->mvectab = 0;` and still knows that a cell is an `int`. In `variant-03`, 102 lines mention
`mvectab`. The wrapper renamed the type and changed nothing about the dependency.

The mechanical translation is visible in the code. `vectab[i]=0;` became `pvectab->mvectab=0;`
(variant-03, line 86; variant-04, line 92). The pointer moved; the coupling stayed.

An **abstract data type** (ADT) is a set of values with a set of operations, where the
representation is not part of the contract. The only honest test is a swap: replace the
representation, keep the interface, and run the same caller. The 2008 code cannot pass that test,
because there is no interface to keep. The field is the interface.

This lesson builds the interface. The shipped module is the reference: a 12-line header that
declares an incomplete type and six operations, two implementations of roughly 60 lines each, and
one 15-line caller that includes only the header.

The lesson is also honest about the price. The C-style `create`/`destroy` API buys representation
independence and pays with manual lifetime. That trade is the second half of the lesson.

## What you will be able to do

By the end of this lesson you can:

- explain the difference between a type and its representation;
- read a forward declaration and say what the compiler knows about an incomplete type;
- write an opaque `Board` whose definition lives in one implementation file;
- show that the same caller and the same header compile against two representations;
- capture the compiler refusing to size, allocate or inspect an incomplete type;
- explain why a struct with one field is not an abstract data type;
- list the lifetime failures a create/destroy API leaves to the caller;
- compare the opaque free-function API with a C++ class and a pimpl, with costs;
- state what the swap proves and what it does not prove;
- keep the archive read-only while you work.

The type is done when a caller can be compiled against the
header alone and linked against either implementation without an edit.

## The concept: abstract data type

### A type is operations, not layout

A type answers two questions: which values exist, and what can be done with them. A
representation answers a third question: how those values are stored. An ADT keeps the third
answer out of the contract.

| Question | Who answers it | Where it belongs |
|---|---|---|
| Which values exist? | The type | The header, in prose or in the operations. |
| What can I do with a value? | The operations | The header. |
| How is the value stored? | The implementation | The `.cpp`. |
| What must always be true? | The invariant | The operations, and later the class. |

In the archive, the only way to touch a board cell is to name `mvectab`. The field name is the
interface, so the representation is the contract. That is the defect, and it is a design defect:
the program works.

### The opaque type

C++ lets a header name a type without defining it:

```cpp
struct Board;
```

This is a **forward declaration**. It creates an **incomplete type**: the compiler knows the name,
that it is a struct, and that it lives in some translation unit. It does not know the size, the
members or the layout.

The definition goes in exactly one implementation file:

```cpp
struct Board
{
    bool visited[64];
};
```

The caller includes only the header. The result is a type the caller can hold, pass and return by
pointer, and never inspect.

| The caller can | The caller cannot | What the compiler says |
|---|---|---|
| Declare `Board *` | `sizeof(Board)` | `invalid application of 'sizeof' to incomplete type 'Board'` |
| Pass `Board *` to an operation | Declare `Board board;` | `aggregate 'Board board' has incomplete type and cannot be defined` |
| Compare two `Board *` values | Read `board->visited` | `invalid use of incomplete type 'struct Board'` |
| Store the pointer | `delete board;` | `-Wdelete-incomplete` warning; the destructor is not called |

The fourth row is the one that surprises people. `delete` on an incomplete type is a warning, not
an error. With `-Werror` it stops the build; without `-Werror` it compiles and skips the
destructor. That is undefined behavior with a polite message.

### The swap test

The acceptance criterion is a change, not a shape:

1. keep `board.h` byte-for-byte identical;
2. keep `main.cpp` byte-for-byte identical;
3. link `main.cpp` against `board_array.cpp`, run it, save the output;
4. link the same `main.cpp` against `board_bitmask.cpp`, run it, compare.

If the outputs match, the caller depends on the operations and not on the representation. That is
what an ADT buys, and it is the only claim the lesson makes.

| What the swap changes | What it does not change |
|---|---|
| The implementation file on the link line | The header |
| The object's size and layout | The caller's source |
| The algorithm inside each operation | The caller's output |
| The number of bytes allocated | The operations' signatures |

### What the archive's wrapper is missing

A `struct` with one field is a **transparent wrapper**. It is a type in the compiler's eyes and a
rename in the design. Three tests separate it from an ADT:

| Test | `struct stablero { int mvectab; }` | An opaque `Board` |
|---|---|---|
| Can a caller name the storage? | Yes: `pvectab->mvectab` | No: `board_mark(board, 1, 1)` |
| Can the representation change without touching callers? | No: 102 lines mention `mvectab` | Yes: edit one `.cpp` |
| Does the type have operations? | No, only a field | Yes, six functions |

`struct svecjugadas { int mjugadas; }` fails the same three tests for the move list. The archive has
two wrappers and zero abstractions.

### Three ways to hide a representation in C++

| Shape | Representation hidden? | Lifetime | Main cost |
|---|---|---|---|
| Free functions over an opaque struct (this lesson) | Yes | Manual `create` / `destroy` | Leaks and double frees are the caller's job |
| Class with private members (`archaeology-08`) | No; layout is in the header | Automatic, by value | Caller recompiles when layout changes |
| Class with a pimpl | Yes | Automatic, RAII | Heap storage for `Impl`, indirection, out-of-line destructor |

The first row is the C-style handle. The second is what lesson 08 built. The third is the usual
C++ answer, and it is the comparison this lesson owes you.

### The cost of the create/destroy API

Six functions, no hidden state, a stable ABI, usable from C. That is real value. The price is that
every rule about lifetime moves from the compiler to the caller:

| Failure | What happens | Who catches it |
|---|---|---|
| Forget `board_destroy` | The allocation leaks | Nobody, unless a sanitizer or profiler runs |
| Call `board_destroy` twice | Double free; undefined behavior | Nobody |
| Copy the handle: `Board *b2 = b;` | Two owners, one allocation | Nobody |
| Return early or throw between create and destroy | Leak on that path | Nobody |
| Pass `nullptr` to `board_mark` | Dereference of null | Nobody; the API does not say |
| Use the handle after destroy | Use after free | Nobody |
| Compare two handles | Identity, not board equality | Nobody; equality is not in the contract |

Only `board_destroy(nullptr)` is accidentally safe, because `delete nullptr` does nothing. Nothing
in the header says so. The header also does not say whether `board_create` can fail. It cannot
return null, because `new` throws `std::bad_alloc`; but the caller has to know that from the
implementation, not from the contract.

There is a second cost that is easy to miss: **no value semantics**. A caller cannot return a
`Board` by value, put one in a `std::vector` without a wrapper, compare two boards, or copy one.
The handle is an identity, not a value. The board is a value in the problem domain.

### The pimpl tradeoff

The C++ answer to the same problem is a class whose state is a pointer to an incomplete
implementation:

```cpp
// board.h
#pragma once
#include <cstddef>
#include <memory>

class Board
{
public:
    Board();
    ~Board();
    Board(const Board &other);
    Board &operator=(const Board &other);
    Board(Board &&other) noexcept;
    Board &operator=(Board &&other) noexcept;

    void reset();
    void mark(int row, int column);
    bool is_visited(int row, int column) const;
    std::size_t visited_count() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
```

The destructor, the copy operations and the move operations must be declared and defined **out of
line**, in the `.cpp`, where `Impl` is complete. That is the rule of five arriving as a bill.

| Criterion | Opaque handle | Class + pimpl |
|---|---|---|
| Representation hidden from the caller | Yes | Yes |
| Caller recompiles when the representation changes | No | No |
| Lifetime is automatic | No | Yes |
| Copyable as a value | No | Yes, if you write the copy |
| Heap allocations per board | One | One for `Impl`; the `Board` can be a value |
| Indirection per operation | One | Two: the call and the `Impl` access |
| Header size | 12 lines | 25 lines, five of them special members |
| C interoperability | Yes | No |
| ABI stability | Operations only | Operations only |

Neither option wins. The handle is smaller and C-compatible; the pimpl is safer and more expensive.
Choose by what the caller must be able to do, not by what looks modern.

> **A trap worth knowing.** A factory that returns `std::unique_ptr<Board>` over an incomplete
> `Board` does not compile in the caller. The `unique_ptr` destructor needs `sizeof(Board)`, and
> the caller does not have it. The reference error is
> `invalid application of 'sizeof' to incomplete type 'Board'` inside `std::default_delete`.
> The fix is a custom deleter or the pimpl above, not a cast.

## Before you start

Read these in full:

- [`README.md`](README.md) — the lesson card and the acceptance criterion.
- [archaeology-08](../archaeology-08-module-boundary/lesson.md) — the prerequisite: the boundary
  this lesson makes opaque.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §8, the lesson sequence; §9, the Track A mapping.
- [`../../../docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the
  `archaeology-09` row and its acceptance criterion.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive, tag and commit.

You also need:

- a local clone of the archive at tag `v1.0.0`;
- a C++20 compiler (`c++`, `g++` or `clang++`);
- a scratch directory outside the archive and outside this repository's tracked files.

> **Rule:** the archive is read-only. Copy what you need into scratch; never edit `original/`.

## The 90-minute path

| Min | Step | What you do | Evidence |
|---:|---|---|---|
| 0–15 | Predict | Predict what an opaque type fixes and what it costs | Prediction table |
| 15–35 | Inspect | Read the archive's wrappers and the shipped module | Counts and run output |
| 35–55 | Hide | Rebuild the header as a forward declaration plus operations | Your `board.h` |
| 55–75 | Swap | Build the same caller against two implementations | Two runs, one output |
| 75–85 | Explain | Price the C API and compare it with a pimpl | Tradeoff table |
| 85–90 | Record | Fill `EVIDENCE.md`; confirm the archive is clean | Portfolio entry |

If the swap takes ten minutes to link, that is information about your
toolchain, not a failure.

### The engineering check in this lesson

Track B uses the same habit as Track A with a different verb: the reference module exists, so you
**rebuild** it and then **swap** it.

| Step | Where it happens | What it produces |
|---|---|---|
| **Predict** | Step 1 | A falsifiable claim about what an opaque type fixes. |
| **Inspect** | Step 2 | The archive's wrapper counts and the shipped surface. |
| **Hide** | Step 3 | Your own opaque header and one implementation. |
| **Swap** | Step 4 | Two link lines, one caller, identical output. |
| **Explain** | Step 5 | The caller-knowledge table and the cost table. |
| **Record** | Step 6 | The ADT note and `EVIDENCE.md`. |

## Step 1 — Predict (15 min)

Before you read `code/board.h`, write five sentences. Each one must be falsifiable: you must be
able to say what observation would prove it wrong.

1. In `variant-03`, how many lines mention the board field `mvectab`? Give one number.
2. If `Board` becomes an opaque type, how many lines of `main.cpp` must change?
3. What will the compiler reject that the class version in `archaeology-08` allowed?
4. Will the array and bitmask implementations print the same line?
5. What does the create/destroy API make the caller responsible for that a class would not?

### A worked prediction

> "`mvectab` is mentioned in about a hundred lines, because every case body reads or writes it.
> Making `Board` opaque changes no line of `main.cpp`, because the caller already uses only
> functions. The compiler will reject `sizeof(Board)` and `Board board;`, which the class version
> allowed. Both implementations will print `visited=2 first=1 second=1`, because the scenario
> marks two cells. The create/destroy API will make the caller responsible for calling
> `board_destroy` exactly once on every path, including the paths that return early."

That prediction is useful even where it is wrong. If the caller had to change, the header was
still leaking something. If the outputs differed, the two implementations did not implement the
same type.

## Step 2 — Inspect (20 min)

### Count the archive's coupling

From the archive root:

```bash
grep -c "mvectab" original/variant-03-pointers-dev-2008/source/*.cpp
grep -c "tablero" original/variant-04-pointers-final-2008/source/*.cpp
grep -n "struct" original/variant-02-records-2008/source/*.cpp
```

A reference run gives:

```text
102
146
31:struct movimiento {
```

The exact numbers do not depend on my machine; they are facts about the pinned file. What matters
is the order of magnitude: one field, reached from about a hundred lines, in each pointer variant.

| Variant | Declaration | Field | Lines that mention the board storage |
|---|---|---|---|
| `variant-01` | `int vectab[64]` (line 17) | none; it is an array | 80 lines mention `vectab` |
| `variant-02` | `int vectab[64]` (line 17) | none; it is an array | 102 lines mention `vectab` |
| `variant-03` | `struct stablero vectab[64]` (line 42) | `int mvectab` (line 36) | 102 lines mention `mvectab` |
| `variant-04` | `struct tab tablero[64]` (line 48) | `int vectab` (line 45) | 146 lines mention `tablero` |

`variant-02` is worth a second look. It is the "records" variant, and its only struct is
`struct movimiento { char nombre[250]; }` — a name, not a board. The variant that was supposed to
demonstrate records never gave the board one.

### The write that proves the wrapper is transparent

```text
variant-03, line 86:   pvectab->mvectab=0;
variant-04, line 92:   ptablero->vectab=0;
```

Both lines are the mechanical translation of `vectab[i]=0;`. The pointer was introduced, the field
was renamed, and the caller still writes the storage. Rename `mvectab` to `visited` and 102 lines
stop compiling. Change only the type and most of them still compile, because C++ converts integers
to `bool`; that is worse, because the coupling survives the type change. That is the test the
wrapper fails.

### Read the shipped module

The lesson ships the opaque version already done. Read the four files in this order, and read
`main.cpp` first:

1. [`code/main.cpp`](code/main.cpp) — 15 lines. The caller's entire view.
2. [`code/board.h`](code/board.h) — 12 lines. The contract.
3. [`code/board_array.cpp`](code/board_array.cpp) — 65 lines. One representation.
4. [`code/board_bitmask.cpp`](code/board_bitmask.cpp) — 59 lines. The other representation.

Then run it:

```bash
./run.sh
```

Expected output:

```text
array=visited=2 first=1 second=1
bitmask=visited=2 first=1 second=1
match=yes
```

The `match=yes` line is produced by `run.sh` only when the two output strings are equal.

### The public surface

| Declaration | Header line | What the signature promises |
|---|---|---|
| `struct Board;` | 5 | A type exists; its layout does not. |
| `Board *board_create()` | 7 | A new board exists. |
| `void board_destroy(Board *)` | 8 | The board ceases to exist. |
| `void board_reset(Board *)` | 9 | Every cell becomes unvisited. |
| `void board_mark(Board *, int, int)` | 10 | The cell becomes visited. |
| `bool board_is_visited(const Board *, int, int)` | 11 | Answers whether the cell is visited. |
| `std::size_t board_visited_count(const Board *)` | 12 | Counts visited cells. |

Seven lines, six operations, one incomplete type. Compare that with the archive: a struct with one
field, no operations, and a hundred call sites that know the field's name.

> **The gap the header does not close.** `board_mark(nullptr, 1, 1)` compiles. So does
> `board_mark(board, -5, 900)`. The header says nothing about null, coordinates or out-of-range
> input. The body ignores out-of-range input and dereferences null. The behavior is real,
> undocumented, and therefore not part of a contract a reviewer can check. Record it;
> `archaeology-13` is where it gets written down.

### A leak the header does not control

`board_array.cpp` defines two helpers:

```cpp
bool board_is_inside(int row, int column);
int board_cell_index(int row, int column);
```

They are not in the header, but they are not `static` and they are not in an anonymous namespace.
The linker exports them:

```bash
nm -C build/array-representation | grep " T board_"
```

```text
00000000004005c0 T board_mark(Board*, int, int)
0000000000400570 T board_reset(Board*)
0000000000400520 T board_create()
0000000000400550 T board_destroy(Board*)
0000000000400590 T board_is_inside(int, int)
00000000004005b0 T board_cell_index(int, int)
00000000004005e0 T board_is_visited(Board const*, int, int)
0000000000400610 T board_visited_count(Board const*)
```

Two implementation details are part of the binary's symbol table. The header hid them from the
compiler; the linker still advertises them. That is why Step 3 puts them in an anonymous
namespace. The addresses will differ on your machine; the `T` and the names are the evidence.

## Step 3 — Hide (20 min)

This is the change test. You will rebuild the type so that the shipped `main.cpp` compiles and
runs **without being edited**.

### Set up the scratch copy

```bash
mkdir -p /tmp/board-adt
cp archaeology/lessons/archaeology-09-abstract-data-type/code/main.cpp /tmp/board-adt/
cd /tmp/board-adt
```

`main.cpp` is now your only fixed point. Do not touch it. Everything else is yours to write.

### Write the header

Three rules decide what goes in:

1. **Declare the type, not its layout.** One line: `struct Board;`. No members, no size, no
   `#include` that reveals them.
2. **Declare operations, not helpers.** Six functions. `board_is_inside` and `board_cell_index`
   belong in the `.cpp`, in an anonymous namespace.
3. **Do not declare the allocation strategy.** The header says `Board *`, not `new`, not
   `malloc`, not a pool. That is the implementation's business.

The reference header is 12 lines. If yours is longer, each extra line needs a caller that uses it.

### Write the implementation

Move the definitions, keep the behavior. The body you write must satisfy the six promises above,
including the two the header does not state:

- rows and columns are **1-based**, `1..8`;
- out-of-range input is **ignored** by `board_mark` and **false** by `board_is_visited`.

Put the helpers in an anonymous namespace:

```cpp
namespace
{
    bool board_is_inside(int row, int column);
    int board_cell_index(int row, int column);
}
```

Compile and run:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 board_array.cpp main.cpp -o board-array
./board-array
```

| Check | Expected |
|---|---|
| Compile | No diagnostics; `-Werror` turns any warning into a failure. |
| Output | `visited=2 first=1 second=1` |
| `main.cpp` edits | Zero. |
| `sizeof(Board)` in the caller | Does not compile. |
| Helpers in `nm` | Absent. |

### Break the type on purpose

This is not vandalism; it is the only way to see what opacity buys. Add each line to a scratch
copy of `main.cpp`, one at a time.

**Size:**

```cpp
std::printf("%zu\n", sizeof(Board));
```

```text
main.cpp:3:35: error: invalid application of 'sizeof' to incomplete type 'Board'
    3 | int main() { std::printf("%zu\n", sizeof(Board)); }
      |                                   ^~~~~~~~~~~~~
```

**Stack allocation:**

```cpp
Board board;
```

```text
main.cpp:2:20: error: aggregate 'Board board' has incomplete type and cannot be defined
    2 | int main() { Board board; (void)board; }
      |                    ^~~~~
```

**Member access:**

```cpp
Board *b = board_create();
b->visited = 0;
```

```text
main.cpp:2:42: error: invalid use of incomplete type 'struct Board'
    2 | int main() { Board *b = board_create(); b->visited = 0; board_destroy(b); }
      |                                          ^~
In file included from main.cpp:1:
board.h:5:8: note: forward declaration of 'struct Board'
    5 | struct Board;
      |        ^~~~~
```

**Delete:**

```cpp
delete b;
```

```text
main.cpp:2:41: warning: possible problem detected in invocation of 'operator delete'
              [-Wdelete-incomplete]
main.cpp:2:21: warning: 'b' has incomplete type
board.h:5:8: note: forward declaration of 'struct Board'
main.cpp:2:41: note: neither the destructor nor the class-specific 'operator delete'
              will be called, even if they are declared when the class is defined
```

The last message is wrapped for width; the compiler prints it on one line. The first three
diagnostics are errors. The fourth is a warning, and `-Werror` turns it into a failure. Without
`-Werror` it compiles and skips the destructor: undefined behavior with a polite message. That is
why the build flags in `run.sh` are part of the lesson, not decoration.

### What the compiler now enforces

| Question | Enforced by | Failure mode |
|---|---|---|
| May a caller size or allocate the type? | Compiler | `incomplete type` |
| May a caller name a member? | Compiler | `invalid use of incomplete type` |
| Does every operation exist? | Linker | `undefined reference to ...` |
| Does the operation do what its name says? | Nobody | Silent wrong behavior |
| Is the handle still alive? | Nobody | Use after free |

The last two rows are the ones the ADT does not fix. Keep them visible.

## Step 4 — Swap (20 min)

### Build the same caller twice

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 main.cpp board_array.cpp -o board-array
c++ -std=c++20 -Wall -Wextra -Werror -O2 main.cpp board_bitmask.cpp -o board-bitmask
./board-array
./board-bitmask
```

Both print:

```text
visited=2 first=1 second=1
```

### Prove that nothing else changed

```bash
sha256sum main.cpp board.h > before.txt
# build and run both
sha256sum main.cpp board.h > after.txt
diff before.txt after.txt && echo "caller and header unchanged"
```

The only thing that changed between the two builds is the `.cpp` on the link line. That is the
whole acceptance criterion: opaque type, representation swap, same caller output.

### The swap is exclusive

Link both implementations at once and the linker refuses:

```bash
c++ -std=c++20 -O2 main.cpp board_array.cpp board_bitmask.cpp -o both
```

```text
/usr/bin/ld: board_bitmask.cpp:(.text+0x0): multiple definition of `board_create()';
board_array.cpp:(.text+0x0): first defined here
```

The error repeats for every operation. Two definitions of one type cannot coexist in one program.
That is not a limitation to work around; it is the definition of a representation choice.

### Measure the representation from inside

The caller cannot print `sizeof(Board)`. The implementation can. A probe that includes the
implementation is allowed, because it is inside the module:

```cpp
// probe.cpp — a probe, not a caller
#include "board_array.cpp"
#include <cstdio>
int main() { std::printf("array_sizeof=%zu\n", sizeof(Board)); }
```

```text
array_sizeof=64
```

The same probe over `board_bitmask.cpp` prints `bitmask_sizeof=8` on the reference toolchain. One
board is 64 bytes, the other is 8. `sizeof(bool)` is implementation-defined, so measure it instead
of assuming it is 1.

### What "no recompile" means

Be precise, because the phrase is easy to overclaim:

| Build shape | What changes when the representation changes |
|---|---|
| Static link, one binary | The `.cpp` recompiles; the caller object is reused; the program relinks. |
| Shared library | The `.so` is replaced; the caller is neither recompiled nor relinked. |
| Header-only | Not applicable: there is no separate representation to swap. |

The caller's **source** never changes. The caller's **object file** is rebuilt only if the header
changed, and it did not. That is the difference from `archaeology-08`, where `bool visited[64]` in
the header forced a rebuild.

### What the swap does not prove

| Claim | Proven? | Why |
|---|---|---|
| Both implementations agree for every sequence of marks. | No | One scenario, two marks. |
| The two implementations have the same complexity. | No | No measurement was made. |
| The board's behavior matches the archive. | No | The baseline is not the archive. |
| `board_mark` rejects out-of-range coordinates. | No | It ignores them. |
| The handle cannot leak. | No | `destroy` is manual. |
| The ABI is stable if an operation's signature changes. | No | The header changes, and callers rebuild. |

## Step 5 — Explain (10 min)

### What the caller knows now

Fill this from your own module, not from the shipped one.

| Question | Caller knows? | How the caller learns it |
|---|---|---|
| Which operations exist | Yes | The header. |
| Parameter and return types | Yes | The header. |
| Which operations mutate | Yes | `const` on the queries. |
| How big a board is | No | The implementation. |
| How a board is stored | No | The implementation. |
| Whether `board_create` can fail | No | The implementation: `new` throws. |
| Who owns the handle | Partly | The name `destroy` implies it; nothing states it. |
| That rows and columns are 1-based | No | The implementation or the tests. |
| That out-of-range input is ignored | No | The implementation. |
| That `board_reset` is redundant after create | No | The implementation: `new Board{}` zeroes. |

Two rows deserve a second look. "Who owns the handle" is the price of the C API: the name suggests
ownership, the compiler does not enforce it. "`board_reset` is redundant" is the same contract
ambiguity as `archaeology-08`: the factory already produces an empty board, so a caller cannot
tell whether `reset` is required before use.

### The change test

A design lesson is judged by a change, not by a shape. Two lists, both taken from the module you
built.

**Changes that stay inside the module:**

| Change | Caller source | Caller recompile | Output |
|---|---|---|---|
| `bool visited[64]` → one 64-bit mask | Unchanged | No | Identical. |
| `bool[64]` → `std::array<bool, 64>` | Unchanged | No | Identical. |
| Index formula `(row-1)*8+(col-1)` → `(row-1)*8+col-1` | Unchanged | No | Identical. |
| `visited_count` loop → `std::popcount` | Unchanged | No | Identical. |
| Helpers moved into an anonymous namespace | Unchanged | No | Identical. |
| Add a cached count inside the implementation | Unchanged | No | Identical. |

**Changes that reach the caller:**

| Change | Why it reaches the caller |
|---|---|
| Rename `board_mark` to `board_set_visited` | The name is in the header; every call site edits. |
| Change `board_mark` to take a `Cell` struct | The signature changes; every call site edits. |
| Switch to 0-based coordinates | Compiles, runs, and silently means something else. |
| Make out-of-range `board_mark` throw | Compiles, runs, and changes behavior at the edge. |
| Remove `board_reset` | Any caller that uses it stops compiling. |
| Change the return type of `board_visited_count` | The declaration changes; callers rebuild. |

Compare the two tables. The first table is "edit one file, rebuild one file". In lesson 08 the
same table was "edit nothing, rebuild everything". That difference is the product.

### The price, in one paragraph

Write this in your own words in the record. The reference:

> The opaque handle gives the caller representation independence and a stable set of operations. It
> takes away the compiler's ability to check lifetime. Every `board_create` must be matched by
> exactly one `board_destroy` on every path, including the paths that return early or throw. The
> compiler cannot see the match, the linker cannot see it, and the tests only see the paths they
> execute. A pimpl keeps the independence and returns the lifetime check to the compiler, at the
> cost of an indirection per operation and five out-of-line special members.

### Two fixes, two sizes

**Minimal fix.** The opaque handle you just wrote. It is minimal in the sense that matters here:
`main.cpp` is untouched, behavior is identical, and the representation can be replaced without
touching the caller. That is enough for the acceptance criterion.

**Structural fix.** A class with a pimpl, or a factory that returns a value type with an
out-of-line destructor. Then the caller gets RAII, copy and move, and the representation stays
hidden. Track A links: `construction-09` (abstract data type) for the swap, `construction-10`
(invariants) for validity, `construction-11` (encapsulation) for the class, `construction-16`
(ownership) for the lifetime. This lesson stops at the handle on purpose; `construction-16` and
`archaeology-10` take the next steps.

### What the ADT does not fix

- It does not make the board correct. `archaeology-04` owns the off-by-one.
- It does not give the board an owner in the caller. `archaeology-05` owns the ownership map.
- It does not state the contract. Coordinates and out-of-range policy are still undocumented;
  that is `archaeology-13`.
- It does not validate. `board_mark(board, -5, 900)` still compiles and does nothing.
- It does not choose between two representations. It only makes the choice cheap.
- It does not replace polymorphism. A `switch` on move type is `archaeology-10`.

## Step 6 — Record (5 min)

Fill [`EVIDENCE.md`](EVIDENCE.md):

- setup and archive pin;
- the prediction table, written before Step 2;
- the archive counts and the two wrapper declarations;
- the opaque header you wrote, with line counts;
- the exact compiler errors for size, stack and member access;
- the exact link error for two implementations;
- the two run outputs and the hash comparison;
- the measured sizes;
- the caller-knowledge table;
- the two change lists;
- the lifetime cost table;
- the pimpl comparison;
- the evidence boundary: verified, inferred, unknown;
- the archive integrity check.

A sentence such as "the type is opaque" is an opinion. A compiler error on `sizeof(Board)` is
evidence.

## The original in detail

Facts you will cite repeatedly, with line numbers from the pinned archive:

| Location | What is there |
|---|---|
| `variant-01`, line 17 | `int vectab[64],vecjugadas[8];` — no type at all. |
| `variant-02`, line 17 | The same array; the only struct holds a name. |
| `variant-03`, line 34 | `struct stablero {` — the wrapper opens. |
| `variant-03`, line 36 | `int mvectab;` — one integer, public. |
| `variant-03`, line 42 | `struct stablero vectab[64];` — the array of wrappers. |
| `variant-03`, line 44 | `struct stablero *pvectab,*pvectab1,*pvectab2;` — three cursors. |
| `variant-03`, line 86 | `pvectab->mvectab=0;` — the translation of `vectab[i]=0;`. |
| `variant-04`, line 44 | `struct tab {` — the same idea, another name. |
| `variant-04`, line 45 | `int vectab;` — one integer, public. |
| `variant-04`, line 48 | `struct tab tablero[64];` — the array of wrappers. |
| `variant-04`, line 49 | `struct tab *ptablero,*ptablero1,*ptablero2;` — three cursors. |
| `variant-04`, line 92 | `ptablero->vectab=0;` — the same translation. |

Counts, from `grep -c` on the pinned files:

| File | Lines | Lines that mention the board storage |
|---|---:|---:|
| `variant-01` baseline | 930 | 80 mention `vectab` |
| `variant-02` records | 1021 | 102 mention `vectab` |
| `variant-03` pointers dev | 1058 | 102 mention `mvectab` |
| `variant-04` pointers final | 1079 | 146 mention `tablero` |

The archive contains **zero** `.h` or `.hpp` files across all variants. That is a fact about the
artifact, not an inference.

> **A naming correction.** The track guide says `struct stablero` appears in "variant-03 and
> variant-04". The archive shows `stablero` in variant-03 and `struct tab` in variant-04. The
> failure class is identical; only the name differs. Cite the file you read.

## What the opaque type proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| The caller cannot size or allocate a `Board`. | Yes | The compiler rejects it; you captured the message. |
| The caller cannot name a member. | Yes | `invalid use of incomplete type`. |
| The same caller compiles against two representations. | Yes | Two builds, one `main.cpp`. |
| The output is identical for the scenario. | Yes | Two runs, same line. |
| The caller's source survives a representation change. | Yes | No edit was needed. |
| The caller's object survives a representation change. | Yes, if the header did not change | The header is the dependency. |
| Both implementations agree for every input. | No | One scenario. |
| The handle is leak-free. | No | `destroy` is manual. |
| The board rejects invalid coordinates. | No | It ignores them. |
| The type documents its contract. | No | The header has no prose. |
| The 2008 program would compile today unchanged. | No | `iostream.h`, `conio.h`, `void main`; see `archaeology-16`. |

This table is the honesty check. The strongest claim in this lesson is a compiler message; the
weakest is "the design is better". Keep them apart.

## Self-check (binary)

- [ ] I wrote five falsifiable predictions before reading `board.h`.
- [ ] I counted the board-storage mentions in `variant-03` and `variant-04`.
- [ ] I read `main.cpp` before `board.h`.
- [ ] I ran the shipped module and got `match=yes`.
- [ ] I rebuilt `board.h` and one implementation in scratch without editing `main.cpp`.
- [ ] My header declares `struct Board;` and six operations, and nothing else.
- [ ] My helpers are in an anonymous namespace and absent from `nm`.
- [ ] I captured the compiler error for `sizeof(Board)`.
- [ ] I captured the compiler error for a stack `Board`.
- [ ] I captured the compiler error for `board->visited`.
- [ ] I captured the `-Wdelete-incomplete` warning.
- [ ] I built the same caller against both implementations and compared the output.
- [ ] I captured the link error for linking both implementations.
- [ ] I measured 64 bytes and 8 bytes from inside the module.
- [ ] I filled the caller-knowledge table, including the rows the header does not answer.
- [ ] I listed three changes that stay inside the module and three that reach the caller.
- [ ] I listed the lifetime failures the create/destroy API leaves to the caller.
- [ ] I compared the handle with a pimpl and named the costs of each.
- [ ] I stated what the swap proves and what it does not.
- [ ] The archive is unmodified (`git status` in the archive is clean).

If a box is not ticked, the unticked box names the next step. There is no penalty for repeating
one.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| "A struct is an abstract data type." | A struct with public fields has no operations and no hidden representation. | Declare operations; hide the fields. |
| "Wrapping an int in a struct makes it a type." | The caller still writes `pvectab->mvectab`. | Change the call sites, not the spelling. |
| "A class is an ADT." | A class with public members leaks the representation the same way. | Hide the state and expose operations. |
| "The header hides the type." | It hides the layout; the definition still lives in one `.cpp`. | Say "incomplete type", not "secret". |
| "Opaque types are always better." | They cost a manual lifetime and an allocation. | Choose by what the caller must do. |
| "`delete` on an incomplete type is an error." | It is a warning; without `-Werror` it compiles and skips the destructor. | Build with `-Werror` and destroy through the API. |
| "The caller does not recompile, so nothing rebuilds." | The implementation object rebuilds and the program relinks. | Say "no caller edit, no caller recompile". |
| "The swap proves the implementations are equivalent." | It proves one scenario, one line. | Add a differential test over many sequences. |
| "The ADT validates the input." | It ignores out-of-range coordinates. | Add an invariant; that is `construction-10`. |
| "The ADT removes the need for tests." | It removes a class of coupling, not wrong behavior. | Keep the golden trace. |
| "This is the same lesson as the module boundary." | The boundary hides names; the ADT hides representation. | Compare the two change tables. |
| "A pimpl is free." | It costs an indirection per operation and five out-of-line special members. | Price it before choosing it. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Abstract data type | A set of values and operations whose representation is not in the contract. |
| Representation | How the state is stored, as opposed to what it means. |
| Representation independence | The ability to change the storage without editing callers. |
| Opaque type | A type whose definition the caller never sees. |
| Forward declaration | A declaration that names a type without defining it. |
| Incomplete type | A type the compiler knows by name but not by size or layout. |
| Transparent wrapper | A struct whose only content is one field the caller still names. |
| Handle | A value that identifies an object the caller cannot inspect. |
| Swap test | Replace the representation and rerun the same caller. |
| RAII | Tying a resource's lifetime to an object's lifetime. |
| pimpl | A class that holds a pointer to an incomplete implementation. |
| Rule of five | Destructor, copy constructor, copy assignment, move constructor, move assignment. |
| Value semantics | Copying a value copies its state, not a reference to it. |
| Differential test | Running the same inputs through two implementations and comparing. |

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- Continue to `archaeology-10` — missing polymorphism. The `switch` on move type is still there,
  and an opaque type does not remove it. Dispatch does.
- If your header grew past 12 lines, list each declaration and the caller that needs it. Anything
  with no caller is a leak.
- If the `delete` warning surprised you, re-read the difference between an error and a warning
  under `-Werror`. A warning that compiles is a bug with a receipt.
- If you want the safer version, write the pimpl and compare the line count. Then decide which
  cost your caller can pay.
- If the two outputs matched too easily, add a third scenario: mark the same cell twice, mark a
  corner, mark out of range. Both implementations must still agree.

## References

- [`README.md`](README.md) — the lesson card and the acceptance criterion.
- [`code/board.h`](code/board.h) — the opaque interface.
- [`code/board_array.cpp`](code/board_array.cpp) — the array representation.
- [`code/board_bitmask.cpp`](code/board_bitmask.cpp) — the bitmask representation.
- [`code/main.cpp`](code/main.cpp) — the caller that knows only six names.
- [`run.sh`](run.sh) — compiles both representations and compares the output.
- [`EVIDENCE.md`](EVIDENCE.md) — the portfolio record you fill in.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §8, the lesson sequence; §9, the Track A mapping.
- [`../../../docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the
  `archaeology-09` acceptance criterion.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive, tag and commit.
- [archaeology-08](../archaeology-08-module-boundary/lesson.md) — the prerequisite: the boundary
  this lesson makes opaque.
- Archive: <https://github.com/DaydreamsDev/DaydreamsDev-guerra-de-caballos-archive>, tag `v1.0.0`,
  commit `284e144e41403afbf0cc44422876425cacfb2bd5`.
