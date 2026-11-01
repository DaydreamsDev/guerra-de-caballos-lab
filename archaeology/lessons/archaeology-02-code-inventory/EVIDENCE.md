# archaeology-02 — Evidence: code inventory and call graph

> This is your record. Write it for your future self.
> Record what you counted, not what you expected the lesson to want.

## 1. Setup

- Source path:
- Working copy path:
- Line count (`wc -l`):
- Checksum of the file inspected (`sha256sum`):
- `grep --version` first line:
- Shell and version:
- Working directory:
- Anything unexpected in the setup:

## 2. Inspection commands

Paste the command and the part of its output you used. One row per command.

| # | Command | Output I used |
|---|---|---|
| 1 | `wc -l` | |
| 2 | `grep -n '^#include'` | |
| 3 | definition grep | |
| 4 | declaration grep | |

- How many lines matched the definition grep:
- How many were prototypes:
- How many were definitions:
- How I separated them:
- How many global names I counted:
- On which lines they are declared:

## 3. File inventory

| File | Lines | Role | Included by |
|---|---:|---|---|
| | | | |

- Files in the source directory that are not source:
- Files the source includes that are not in the directory:

## 4. Function inventory

Rule I used for the span: definition line to closing brace, blanks included.

| Function | Lines | Inputs | Output | Side effects | Calls |
|---|---:|---|---|---|---|
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |

- Sum of the spans:
- Lines not inside any function:
- Does the sum plus the remainder equal `wc -l`? yes / no
- If no, which gap did I miss:

### 4.1 Counting rules I used

- A definition is:
- A prototype is:
- Library calls go in:
- A `void` function's work is recorded as:

## 5. Globals and data

| Name | Type | Declared at | Mutated by | Read by |
|---|---|---|---|---|
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |

### 5.1 Write counts

| Name | Writes | Writing functions |
|---|---:|---:|
| | | |
| | | |
| | | |
| | | |
| | | |

- Pattern I used to count a write:
- Pattern I used to exclude a comparison:
- Names I almost missed, and why:
- Globals mutated through a reference parameter, and which parameter:

## 6. Call graph

```text

```

- Roots:
- Leaves:
- Cycles:
- Functions with no caller:
- Functions with no callee:
- Longest path, in edges:
- Functions reachable from the root:
- Functions not reachable from the root:

### 6.1 Edges versus call sites

| Caller | Distinct callees | Call sites |
|---|---:|---:|
| | | |
| | | |
| | | |
| | | |
| | | |
| **Total** | | |

- Distinct edges:
- Total call sites:
- Why the two numbers differ here:

### 6.2 Degree

| Node | In | Out | Degree |
|---|---:|---:|---:|
| | | | |
| | | | |
| | | | |
| | | | |
| | | | |

- Most connected node, by degree:
- Most called node, by in-degree:
- The definition of "connected" I am using:
- If I had used the other definition, the answer would be:

## 7. Entry points

- Program entry:
- Library entry:
- Test entry:
- How I know there is no library entry:
- How I know there is no test entry:

## 8. Findings

Every finding needs a claim, a `file:line` and a confidence label. The label is exactly one of
`verified`, `inferred` or `unknown`.

| # | Finding | Evidence (file:line) | Confidence | Consequence or open question |
|---|---|---|---|---|
| 1 | | | | |
| 2 | | | | |
| 3 | | | | |
| 4 | | | | |
| 5 | | | | |
| 6 | | | | |
| 7 | | | | |
| 8 | | | | |

### 8.1 The three required findings

- Largest function, with span and percentage of the file:
- Most connected function, with the degree and the definition used:
- Most mutated global, with the write count and the writing functions:

### 8.2 Dead or unreachable code

| What | Evidence | Why it is unreachable | Confidence |
|---|---|---|---|
| | | | |
| | | | |

- Functions with no incoming path from a root:
- Code that looks dead but is reachable, and why:

### 8.3 What the shape predicts

Two predictions, each written as a count that could be checked.

- Prediction 1:
- Prediction 2:
- Which functions the prediction touches:
- What result would prove the prediction wrong:

## 9. What I could not classify

| Item | Why I could not classify it | What evidence would decide it |
|---|---|---|
| | | |
| | | |
| | | |

## 10. Shape paragraph

One paragraph, without value judgments: where the work lives, where the state lives, what the graph
says about change.

>

## 11. Self-check

- [ ] I ran the four inspection commands and recorded their output.
- [ ] I separated prototypes from definitions with the brace rule.
- [ ] I counted global names, not declaration lines.
- [ ] My function spans plus the remainder equal the line count.
- [ ] I counted writes as assignment, increment or decrement only.
- [ ] I recorded globals mutated through reference parameters.
- [ ] I drew the graph and named one root and every leaf.
- [ ] I reported edges and call sites as separate numbers.
- [ ] I defined degree before naming the most connected node.
- [ ] I recorded dead or unreachable code with its reason.
- [ ] Every finding has a line number and a confidence label.
- [ ] I wrote the shape paragraph and two checkable predictions.
- [ ] The original source is unmodified.

## 12. Next step

- What I can explain now:
- What I cannot explain yet:
- Next lesson or repeat:
- The defect I noticed and deliberately did not chase:

## 13. Optional note

- Anything else:

---

> Reminder: this file is evidence for you.
