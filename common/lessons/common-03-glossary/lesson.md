# common-03 — Glossary (glosario)

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** shared vocabulary (vocabulario compartido) ·
> **Prerequisites:** none · **Estimated time:** 20 minutes
> **Reading:** ~13 minutes
> **Lesson role:** core

## Why this lesson exists

A specification is written in words before it is written in code. If two people read the same word
and understand two different decisions, every later comparison is built on sand. The transcript can
be byte-identical while the discussion around it is not: one reader thinks a *seed* is a secret, the
other thinks it is an ordinary input; one reader thinks a *contract* is documentation, the other
thinks it is behavior that can fail.

This lesson is about that layer. You select ten terms from the project glossary, write one line that
fixes each meaning, name one misconception that the line rules out, and then check the same term
against the specification. The check is not decoration: it is how you find out whether the
vocabulary you are using is the vocabulary the project actually uses.

The lesson changes no code and no canonical document. Its product is a table you can defend: ten
terms, ten definitions, ten misconceptions, ten verdicts with line references.

## What you will be able to do

By the end of this lesson you can:

- select ten terms that matter to the game, not ten words that sound technical;
- write a one-line definition that names a category and a difference;
- name one plausible misconception per term, not a typo or a trivial mistake;
- search [`common/spec/game-rules.md`](../../spec/game-rules.md) for each term and read the line in
  context;
- classify the result as consistent, different sense, or absent, with a line reference;
- explain why a shared term is a control on the conversation, not a synonym;
- record the prediction, the commands, the table and the explanation in `EVIDENCE.md`.

## The concept: shared vocabulary

A shared vocabulary is an agreement about which distinctions matter. It is not a list of synonyms.
Two words can point at the same object and still encode different decisions; one word can point at
two objects and hide a disagreement. The glossary exists to make those distinctions explicit and
translatable.

Five ideas carry this lesson:

| Idea | Meaning in this lesson |
|---|---|
| **Term** | A word that names a distinction the project depends on. |
| **One-line definition** | A sentence that places the term in a category and names what makes it different. |
| **Misconception** | A stable wrong model that predicts a wrong action, not a spelling error. |
| **Consistency** | The same term carries the same sense in the glossary and in the specification. |
| **Translation pair** | The English and Spanish names preserve the same distinction. |

### What a one-line definition must do

A useful definition has two parts: the category the term belongs to, and the difference that
separates it from its neighbours. Compare:

- Weak: *“A seed is something random.”* It does not say what kind of thing a seed is, and it is
  wrong: the seed is the input that makes a random sequence repeatable.
- Strong: *“A seed is the value that initializes a deterministic pseudo-random sequence.”* Category:
  a value. Difference: it initializes the sequence, and it makes the sequence repeatable.

The strong version can be checked against a line in the specification. The weak version cannot:
almost any random value would satisfy it.

A definition is not a slogan. Keep it to one sentence, but make the sentence earn its place.

### What a misconception must do

A misconception is a wrong model that survives a correct explanation. It is plausible, it is stable,
and it leads to a wrong action. “The seed is a password” is a misconception: it is plausible because
both are short values you pass to a program, and it leads to a wrong action, such as hiding the seed
or treating it as a secret.

“The seed is written with four letters” is not a misconception. It is a spelling observation. It
does not predict a wrong decision.

For each term, write the misconception as a sentence in quotation marks, then add one clause that
says why it is wrong. The clause is what turns an opinion into a check.

## Before you start

Read these documents in full. They are short, and the lesson depends on their exact wording:

- [`docs/guide/glossary.md`](../../../docs/guide/glossary.md) — the terminology source. Read every
  section, including the rejected schooling terms and the maintenance rule.
- [`common/spec/game-rules.md`](../../spec/game-rules.md) — the fourteen sections of the game
  specification. You will search it, but search is not a substitute for reading it once.
- [`docs/guide/how-learning-works.md`](../../../docs/guide/how-learning-works.md) — §4.5 defines the
  engineering check used at the end of this lesson.

You also need:

- a terminal with `grep` (or an equivalent search tool);
- a plain-text file for your working table;
- [`docs/guide/glossary-es.md`](../../../docs/guide/glossary-es.md) if you are working in Spanish;
- [`common/spec/game-rules-es.md`](../../spec/game-rules-es.md) for the Spanish consistency check.

The English specification is the authority for behavior. The Spanish specification is its
translation; if the two disagree, record the disagreement instead of choosing the version you
prefer.

## The 20-minute path

| Min | Step | What you do | Evidence |
|---|---|---|---|
| 0–3 | Select | Read the glossary; choose ten terms and record why. | Selection list |
| 3–6 | Predict | For each term, write the expected section, sense and verdict. | Prediction table |
| 6–10 | Run | Search the specification for each term. | Commands and hits |
| 10–15 | Compare | Read each hit; assign consistent, different sense, or absent. | Ten-row table |
| 15–18 | Explain | Write why shared vocabulary is a control. | One paragraph |
| 18–20 | Record | Fill `EVIDENCE.md` with the table and the explanation. | Portfolio entry |

If one term takes longer, that is information about the
term, not about you.

### The engineering check in this lesson

| Step | Where it happens | What it produces |
|---|---|---|
| **Predict** | Step 2 | The expected section, sense and verdict for each term. |
| **Run** | Step 3 | The search commands and their hits. |
| **Compare** | Step 4 | The ten-row table with a verdict per term. |
| **Explain** | Step 5 | One paragraph on why shared vocabulary is a control. |
| **Record** | Step 6 | A completed `EVIDENCE.md`. |

The artifact is the table.

## Step 1 — Select ten terms (3 min)

You are choosing ten terms that the game depends on. Use these rules:

1. Include at least one term from each of these glossary sections: Foundations, Design,
   Verification, and Project vocabulary.
2. Include one term you can already define without looking, and one you cannot.
3. Do not select terms from the rejected schooling section; the glossary rejects them on purpose.
4. Every term must be usable in a sentence about the game, the harness, or the evidence.
5. Record one clause that says why the term is in your set.

A suggested set is below. You may replace any term, but keep the distribution and record the reason
for every replacement.

| English | Español | Section |
|---|---|---|
| Specification | Especificación | Design |
| Invariant (project sense) | Invariante (del proyecto) | Project vocabulary |
| Determinism | Determinismo | Verification |
| Seed | Semilla | Verification |
| Contract | Contrato | Design |
| Evidence | Evidencia | Verification |
| Implementation | Implementación | Foundations |
| Reference | Referencia | Data representation |
| Golden trace | Traza de referencia | Verification |
| Harness | Arnés (banco de pruebas) | Verification |

The set is deliberately mixed. Three terms will not behave the way a beginner expects: *reference*
has a different sense in the specification, *golden trace* is called simply *trace* there, and
*harness* appears only inside a path. That is not a trick. Finding those gaps is part of the skill.

If you prefer a different set, choose it now, before you search. Changing the set after seeing the
hits destroys the prediction step.

## Step 2 — Predict before you search (3 min)

For each of your ten terms, write three things in your working table:

- the section of the specification where you expect to find it, or “absent”;
- the sense you expect the specification to use, in one line;
- the verdict you expect: consistent, different sense, or absent.

Then write one falsifiable sentence about the whole set. For example:

> I expect eight of my ten terms to be used in the same sense as the glossary, one to carry a
> different sense, and one to be absent from the specification.

A prediction is not a guess you may repair later. The gap between prediction and observation is the
learning signal; editing the prediction after the search destroys it.

### A worked prediction

Take *Seed*. Before searching, write:

- expected sections: §3, *Starting positions*, and §12, *Randomness*;
- expected sense: the value that starts the deterministic sequence;
- expected verdict: consistent.

That prediction is specific enough to fail. If the specification used *seed* to mean a player
password, the prediction would be wrong, and the wrongness would be useful.

## Step 3 — Run the consistency search (4 min)

Search is the “run” of this lesson. It is read-only: you are looking for evidence, not changing the
repository.

Set the repository path once, then search the English specification:

```bash
REPO="/path/to/38-guerra-de-caballos-lab"
cd "$REPO"

for term in specification invariant determinism seed contract evidence implementation \
  reference trace harness; do
  printf '\n== %s ==\n' "$term"
  grep -n -i "$term" common/spec/game-rules.md || echo "(not found)"
done
```

Replace the path with your checkout. If you replaced a term, replace the word in the list.

Two details matter:

- `-n` prints the line number, so the hit can be cited later.
- `-i` ignores case, but it does not handle word forms. *Determinism* is the glossary noun; the
  specification uses the adjective *deterministic*. Search both forms when the noun is absent:

```bash
grep -n -i -E "determinism|deterministic" common/spec/game-rules.md
```

For the Spanish edition, run the same loop against the Spanish specification:

```bash
for term in especificación invariante semilla contrato evidencia implementación \
  referencia traza arnés; do
  printf '\n== %s ==\n' "$term"
  grep -n -i "$term" common/spec/game-rules-es.md || echo "(not found)"
done
```

A hit is a pointer, not a verdict. `grep` can tell you that a word occurs on line 102; it cannot
tell you whether the line uses the word in the glossary sense. Read every hit in context, including
the sentence before and after it.

### What counts as a hit

| Observation | What it means |
|---|---|
| The exact term appears and the sentence matches the glossary sense. | Candidate for consistent. |
| A different word form appears, such as *deterministic* for *determinism*. | Check the sense; form alone is not a mismatch. |
| The same word appears with a different sense. | Candidate for different sense; record both senses. |
| The word appears only as part of a path or a file name. | The concept is absent; record the path and the nearest concept. |
| No hit at all. | Absent; record “absent” and the nearest term the specification does use. |

## Step 4 — Compare glossary and specification (5 min)

For each term, put the glossary definition, the specification line and your verdict side by side.
The verdict has exactly three values. Do not invent a fourth value to avoid a decision.

| Verdict | Test | What to record |
|---|---|---|
| **Consistent** | The glossary sense fits the specification line without rewriting either. | The exact line number and the quoted phrase. |
| **Different sense** | The same word names a different concept in the two documents. | Both senses and the line that shows the difference. |
| **Absent** | The term, as a concept, does not appear in the specification. | “absent”, the nearest term, and the line where that term appears. |

### Worked example 1 — Seed (consistent)

- Glossary: *“The value that initializes a pseudo-random sequence.”*
- Misconception: *“The seed is a password.”* It is wrong because the seed is not secret; it selects
  a sequence, and anyone who knows it can reproduce the run.
- Specification: line 29 (“Given a seed:”), line 102 (“The reference uses a 64-bit linear
  congruential generator”), line 150 (“the same seed and player name”).
- Verdict: consistent. The specification treats the seed as an input that selects the sequence. It
  never treats it as a secret.

This example shows the shape of a complete row. The definition is one sentence, the misconception is
a wrong model with a reason, and the verdict cites real lines.

### Worked example 2 — Reference (different sense)

- Glossary: *“An alias to an existing object.”*
- Misconception: *“A reference is a pointer.”* It is wrong because a reference is an alias with
  different guarantees, not an address you can do arithmetic on.
- Specification: line 8 (“the deterministic reference”), line 120 (“Reference implementation”).
- Verdict: different sense. The glossary entry is about indirection in code; the specification is
  talking about the canonical implementation of the game.

Record both senses. Do not pretend they are the same, and do not silently drop the term. A word that
carries two senses is a vocabulary risk: a reader can agree with the glossary and still misread the
specification.

### Worked example 3 — Harness (absent as a concept)

- Glossary: *“A runner that executes a variant and captures its behavior.”*
- Misconception: *“A harness is a test framework.”* It is wrong because a harness runs a variant and
  captures behavior; it does not have to assert, mock, or report like a test framework.
- Specification: the string `harness` appears only inside the path `common/harness/reference.py`
  (line 125). There is no definition and no use of *harness* as a concept.
- Verdict: absent as a term. The specification calls the same role “reference implementation” in
  §14.

This is a finding about scope, not a failure of your work. The specification is allowed to name the
artifact by its path and its role instead of by the glossary word. Your evidence should say so
plainly: *“absent as a concept; nearest term: reference implementation, §14.”*

### The ten-row table

Your evidence table has one row per term and these columns:

| English | Español | One-line definition | One misconception | Spec line(s) | Verdict |
|---|---|---|---|---|---|

Write the definition in your own words. Copying the glossary is allowed only if you can explain
every word; the point is to make the term yours, not to transcribe it.

Use these rules when the row is difficult:

- If a term has two senses, put both in the definition cell, separated by a semicolon, and explain
  the difference in the verdict cell.
- If a term is absent, write “absent” in the verdict cell and name the nearest term in the spec.
- If the Spanish term preserves the sense but the English does not, record a translation gap instead
  of averaging the two.
- If the specification contradicts the glossary, the specification wins for behavior, but the
  glossary still needs a correction. Record the contradiction; do not edit either file in this
  lesson.

### The consistency rules

1. Consistency is about sense, not spelling. *Determinism* and *deterministic* can be consistent.
2. A term can be consistent in one section and not in another. Cite the section you checked.
3. A translation must preserve the distinction, not the word. Check the Spanish term against the
   Spanish specification as well.
4. Absence is a finding. It tells you which words the specification chose instead.
5. A verdict without a line reference is an opinion. Add the line or mark the row incomplete.

## Step 5 — Explain (3 min)

Write one paragraph, without syntax words. Answer:

- Why is a shared term a control on a conversation, rather than a convenience?
- What can two people no longer claim when they use the same word for different senses?
- Which of your ten terms was hardest to define, and what did the difficulty reveal?
- Which term would you add to the glossary after this lesson, and why?

A useful sentence to finish: *“A definition is not a nicer word for the thing; it is the line that
tells us when we are still talking about the same thing.”*

Do not name a programming language, a data type, or a syntax element. If you need one to explain the
idea, the idea is not yet separated from the implementation.

### Calibration

Compare your prediction table with the ten verdicts:

- If the verdicts match, your mental model of the project vocabulary is working. Say which term was
  easiest to predict and why.
- If one verdict differs, the gap names the term to study next. Do not average the result or call it
  “close enough”.
- If every verdict matches but the definitions were hard to write, the gap is in articulation, not
  in recognition. Rewrite the three weakest definitions.


## Step 6 — Record (2 min)

Fill [`EVIDENCE.md`](EVIDENCE.md). It is your portfolio entry. Include:

- the ten terms with English and Spanish names;
- the prediction table, written before the search;
- the commands you ran and their exit statuses;
- the ten-row table with definitions, misconceptions, line references and verdicts;
- the explanation paragraph;
- the next step.

## Self-check (binary)

- [ ] I read the glossary in full, including the rejected terms and the maintenance rule.
- [ ] I read the specification once before searching it.
- [ ] I selected ten terms and recorded why each one is in the set.
- [ ] I wrote the prediction before running the search.
- [ ] I searched the specification and recorded the exact commands.
- [ ] I read every hit in context, not only the matching word.
- [ ] Each of the ten terms has an English name and a Spanish name.
- [ ] Each term has a one-line definition in my own words.
- [ ] Each term has one plausible misconception and a reason it is wrong.
- [ ] Each term has a verdict with a line reference, or “absent” with the nearest term.
- [ ] I can explain why shared vocabulary is a control, without syntax words.
- [ ] I filled `EVIDENCE.md` and know my next step.

If a box is not ticked, the lesson is not yet done **for you**. The unticked box names the next
step. There is no penalty for repeating a step.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| “A glossary is a list of synonyms.” | A definition fixes a decision; a synonym only gives another word. | Write the category and the difference. |
| “If the word appears in the spec, the term is consistent.” | The same spelling can carry a different sense. | Read the line and compare senses. |
| “A different word form means a different concept.” | *Determinism* and *deterministic* can name the same idea. | Check the sense before calling a mismatch. |
| “Translation is cosmetic.” | A translation that changes the distinction breaks the shared vocabulary. | Check the Spanish term against the Spanish spec. |
| “A misconception is any mistake.” | A typo does not predict a wrong decision. | Name a stable wrong model and the action it causes. |
| “A one-line definition can be vague.” | Vagueness cannot be checked against a line of the spec. | Name the category and the difference. |
| “Ten terms means coverage.” | The goal is ten terms you can use, not a completed checklist. | Choose terms the game depends on. |
| “If a term is absent, the glossary is wrong.” | Absence may be a deliberate choice of the specification. | Record the nearest term and the gap. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Shared vocabulary | An agreement about which distinctions the project depends on. |
| Term | A word that names one of those distinctions. |
| One-line definition | A sentence with a category and a difference. |
| Misconception | A stable wrong model that leads to a wrong action. |
| Consistency | The same sense in the glossary and in the specification. |
| Translation pair | English and Spanish names that preserve the same distinction. |
| Verdict | Consistent, different sense, or absent, with a line reference. |
| Calibration | The gap between the predicted and the observed verdict. |

The glossary defines *misconception* and *calibration*; the other terms in this table are local to
this lesson.

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- If you have not run the harness yet, continue to `common-02` — deterministic harness — and use the
  vocabulary you just fixed to read its guarantees.
- If you want the build side first, go to `common-04` — build and run.
- If you are starting the archaeology track, `archaeology-00` uses this lesson as a prerequisite.
- If a term was absent or carried a different sense, record the gap and propose the smallest
  correction in your evidence. Do not edit the glossary or the specification in this lesson.
- If your definitions were hard to write, repeat the exercise with ten different terms. The second
  set is usually faster, and the speed is the evidence that the vocabulary is becoming yours.

## References

- [`docs/guide/glossary.md`](../../../docs/guide/glossary.md) — the terminology source.
- [`docs/guide/glossary-es.md`](../../../docs/guide/glossary-es.md) — the Spanish terminology
  source.
- [`common/spec/game-rules.md`](../../spec/game-rules.md) — the behavior specification to check.
- [`common/spec/game-rules-es.md`](../../spec/game-rules-es.md) — the Spanish specification.
- [`docs/guide/how-learning-works.md`](../../../docs/guide/how-learning-works.md) — §4.5, the
  engineering check.
- [common-02 lesson](../common-02-deterministic-harness/lesson.md) — the next common-core
  lesson.
