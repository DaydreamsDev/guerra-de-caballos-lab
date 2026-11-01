# failure-003 — `srand(time(NULL))` inside the placement retry loop

> [English](failure-003-seed-in-loop.md) | [Español](failure-003-seed-in-loop-es.md)

- **Class:** correctness
- **Severity:** high
- **Concept violated:** randomness has one entry point and one seed
- **Archive:** tag `v1.0.0`,
  `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`

## Evidence

- **Variant and line:** `variant-01:76`, `variant-02:87`, `variant-03:99`, `variant-04:105`.
- **Source:**

```cpp
int i = 1;
while (i == 1)
{
    srand(time(NULL));          // line 76
    tirocab1 = rand() % 51;
    tirocab2 = rand() % 51;
    cab1 = rand() % 64;
    cab2 = rand() % 64;
    if (cab1 != cab2 && tirocab1 != tirocab2)
    {
        i++;
    }
}
```

The seed is set inside the `while` that retries a collision, not once before it.

## Symptom

Two runs at different clock seconds place the knights differently. A run with a fixed seed patch can
repeat the same collision forever: the loop reseeds to the same value on every attempt, draws the
same four numbers, fails the same condition and tries again.

## Root cause

The seed is coupled to the clock and to the retry path. The original code wanted a different
sequence for each attempt, so the call was placed where the attempt begins. That reinitializes the
generator instead of advancing it. The loop can only make progress if the clock changes between
iterations, which turns a collision into a wait of up to one second.

## Consequence

Non-reproducible placement. Two runs cannot be compared, a golden trace cannot be captured without
patching, and a test that depends on placement is flaky. The retry can spin for a clock tick per
collision; with a fixed seed it can hang. `archaeology-03` records seed 9999 as `run_status=124`:
the patched build never leaves this loop.

## Test that catches it

`seed_in_loop_test()` in `tests/failure_tests.cpp`. It uses a small linear congruential generator,
seeds it with `1`, records the first value, then re-seeds with `1` three times and checks whether
the next value repeats. It returns `true` when the sequence restarts, which is exactly the behavior
that makes the placement loop repeat. `tests/run_failure_tests.sh` prints `failure-003=detected`.

The test detects repeated seeding. It does not call `rand()`, and it does not reproduce the hang.

## Minimal fix

Move the seed call before the loop, or remove it from the retry path:

```cpp
srand(time(NULL));
while (i == 1)
{
    // draw and retry
}
```

Better: take the seed as a parameter so the caller decides whether the run is reproducible.

## Structural fix

Inject the random source instead of calling the global generator from inside the placement logic. A
small `RandomSource` interface with a seed and a `next()` method lets production use the clock and
tests use a fixed sequence.

Track A rung: [`construction-03-functions`][c03] (parameters instead of hidden state) and
[`construction-20-hexagonal`][c20] (the generator as an adapter at the boundary).

[c03]: ../../construction/lessons/construction-03-functions/lesson.md
[c20]: ../../construction/lessons/construction-20-hexagonal/lesson.md

## Confidence

High for the code placement and the non-reproducibility. High for the repeated seeding. Medium for
the exact wait time, which depends on the clock resolution and the number of collisions.

## Note on the test

The test is an isolated reproduction of the failure pattern. It is not a test of the original
program. The LCG is a stand-in for the platform generator, chosen because its restart behavior is
easy to observe.
