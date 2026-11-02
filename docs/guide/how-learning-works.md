# How learning works (pedagogía y didáctica)
> [English](how-learning-works.md) | [Español](how-learning-works-es.md)

> The learner is the agent of their own learning. The material provides experiences, tools and
> criteria. It does not grade, rank, or require a classroom.

The stance is explicit:

- **No grades. No exams. No quizzes. No points. No ranking.**
- **No group work in the method.** Collaboration, if any, is optional and outside it. The core path
  must be completable by one person, at their own pace.
- **Verification is engineering, not schooling:** build the artifact, run the acceptance criteria,
  observe the evidence, decide the next step.
- **The only question that matters:** *can I demonstrate the concept with a working artifact?*

The four-layer framework behind all of this is canonical in
[`conceptual-framework.md`](conceptual-framework.md); it is not repeated here.

---

## 1. Theoretical anchors (anclas teóricas)

| Framework | Core claim | What it changes here |
|---|---|---|
| **Constructivism** (constructivismo — Piaget) | Knowledge is built from experience, not transmitted | The learner starts from a running artifact and constructs the model by contrast |
| **Constructionism** (construccionismo — Papert) | Learning happens best when making shareable objects | Every artifact lesson produces a runnable artifact; the variants are “objects to think with” |
| **Heutagogy** (heutagogía — Hase & Kenyon; Blaschke) | Self-determined learning: the learner decides what, how and when | No fixed calendar, no teacher, no gatekeeper; the material is a resource, not an authority |
| **Self-regulated learning** (aprendizaje autorregulado — Zimmerman) | Forethought → performance → self-reflection | Each lesson has goal-setting, monitoring, and a written reflection |
| **Self-determination theory** (teoría de la autodeterminación — Deci & Ryan) | Motivation needs autonomy, competence and relatedness | Autonomy by design; competence through achievable criteria; relatedness optional, never required |
| **Flow** (flujo — Csikszentmihalyi) | Engagement peaks when challenge slightly exceeds skill | Each rung is one step above the current level; no deadlines, no arbitrary difficulty |
| **Authentic assessment** (evaluación auténtica — Wiggins) | Judge performance on real tasks, not on proxies | Acceptance criteria derived from the real engineering artifact |
| **Feedback literacy** (alfabetización de retroalimentación — Carless & Boud) | The learner must be able to judge quality and act on evidence | Self-review against criteria; feedback is information, not a verdict |
| **Reflective practice** (práctica reflexiva — Schön) | Professionals think in and on action | the reflection section of `EVIDENCE.md` after every project |

### Why not the usual school machinery

| Rejected | Why |
|---|---|
| **Exams and quizzes** | They measure recall under artificial constraints, create ranking and anxiety, and are not how engineering work is accepted. We keep *prediction* and *self-explanation* as engineering checks, not as tests. |
| **Grades and points** | They shift attention from learning to scoring, encourage working to the grade, and make comparison the goal. |
| **Rubrics with levels** | They become grades in disguise. Replaced by binary acceptance criteria. |
| **Forced group work** | The learner may be in a different timezone or simply prefer to work at their own pace. Collaboration is optional and outside the method. |
| **Deadlines** | They punish life. The material gives suggested time, never a due date. |
| **Coverage** | Finishing the catalog is not the goal; demonstrating a concept is. |

> A note on retrieval: the testing effect is real, but it does not require a quiz. Predicting the
> output before running, and explaining the result after, is retrieval with a purpose. We keep the
> mechanism, not the exam.

---

## 2. How concepts are formed

| Technique | What it says | How it appears |
|---|---|---|
| **Variation theory** (teoría de la variación — Marton) | To discern a critical aspect, vary it while everything else stays invariant | Same game, same rules, same traces; exactly one design decision changes per lesson |
| **Contrastive pairs** (pares de contraste) | Learning lives in the difference | Every **concept lesson** ships “with” and “without”, both runnable |
| **Threshold concepts** (conceptos umbral — Meyer & Land) | Some concepts transform understanding and are hard to unlearn | Indirection, abstraction, polymorphism and ownership are treated as thresholds |
| **Misconception-based teaching** | Wrong models resist explanation; they need conflict and a counterexample | Each lesson names the likely misconception and provides a runnable refutation |
| **Concrete before abstract** | Abstraction is earned from examples | Run it first; name it after the learner has felt the problem |

---

## 3. How lessons are structured

| Technique | What it says | How it appears |
|---|---|---|
| **Cognitive load theory** (carga cognitiva — Sweller) | Working memory is limited | One concept; segmenting; signaling; no split attention; no redundancy |
| **Worked examples + fading** (ejemplos resueltos y desvanecimiento — Renkl) | Novices learn from examples, then from completion, then from independence | Example → completion → independent |
| **Subgoal labeling** (etiquetado de subobjetivos) | Naming the parts of an example improves transfer | Every substantial block is labeled by subgoal, not by syntax |
| **PRIMM** (Predict, Run, Investigate, Modify, Make) | Code reading is a skill | The default rhythm of concept lessons |
| **Use–Modify–Create** (Usar–Modificar–Crear) | Start from a working artifact | Track A starts from a running non-structured baseline (*monolith*); Track B from the original |
| **Cognitive apprenticeship** (aprendizaje cognitivo — Collins, Brown & Newman) | Make expert thinking visible | Worked reasoning is written out; the learner articulates decisions |
| **Reflective practice** (práctica reflexiva — Schön) | Reflection is part of the work | Written reflection after every project |

**Minimal-guidance caveat:** pure discovery is a poor strategy for novices (Kirschner, Sweller &
Clark, 2006). Guidance is provided; independence is the destination, not the starting point.

---

## 4. Verification without grades (verificación sin notas)

The learner does not “take a test”. The learner **runs acceptance criteria** and reads the evidence.

### 4.1 Acceptance criteria (criterios de aceptación)

- Derived from the specification.
- Observable and binary: pass / not yet.
- Public: written before the work starts.
- No hidden criteria.
- One criterion per concept, not per line of code.

Examples:

| Lesson | Acceptance criterion |
|---|---|
| construction-02 | The transcript matches the golden trace and the control-flow graph has no `goto`. |
| construction-04 | Adding a field to the record compiles; the transcript is unchanged; the touched signatures are listed. |
| construction-09 | Swapping the board representation modifies only `board.cpp`; every caller compiles unchanged. |
| construction-12 | Adding a piece type requires zero edits in the game loop. |
| archaeology-04 | A test reproduces the off-by-one under a sanitizer; after the fix, the test passes and the traces are unchanged. |
| archaeology-18 | The refactor preserves every golden trace. |

### 4.2 Definition of done (definición de terminado)

A project is done, for the learner, when:

1. The artifact runs offline.
2. Every acceptance criterion passes, and the prediction/reality diff is recorded.
3. The learner can explain the concept **without using syntax words**.
4. The learner can apply the change test.
5. The reflection note is written.
6. The next step is known.

There is no grade for meeting this. There is only evidence.

### 4.3 Evidence portfolio (portafolio de evidencia)

The learner keeps, per lesson:

- the working artifact,
- the acceptance report (criterion → output),
- the prediction note and its comparison with the trace,
- the change-cost table (files / lines / concepts / tests),
- the reflection note.

This is not submitted to anyone. It is the learner's own record — and, later, a professional writing
sample.

### 4.4 Self-diagnosis (autodiagnóstico)

Four questions, no levels, no score:

1. **Can I explain it?** Without notes, without syntax words.
2. **Can I change it?** On demand, with a measured cost.
3. **Can I extend it?** To a requirement I have not seen.
4. **Did my prediction match?** If not, which part of my mental model was wrong?

If any answer is “not yet”, the next step is explicit: repeat the lesson, read the contrast pair
again, or move to the prerequisite.

### 4.5 The engineering check (comprobación de ingeniería)

The core habit of every lesson that produces or changes an artifact, and the closest thing to real
work:

> **Predict → Run → Compare → Explain → Record.**

| Step | What the learner does | Evidence |
|---|---|---|
| **Predict** | Writes what will happen, before running anything | Prediction note, with the expected trace or output |
| **Run** | Executes the artifact with a fixed seed | Command + output |
| **Compare** | Diffs prediction against observation | The exact lines that differ |
| **Explain** | Says *why* the gap exists; names the wrong assumption | One paragraph, without syntax words |
| **Record** | Saves the prediction, the diff and the explanation in `EVIDENCE.md` | Portfolio entry |

**It is not an exam.** It is open book, untimed, ungraded, and the artifact is the answer. There is
no score; the gap between prediction and observation **is** the feedback.

**Why it belongs to real engineering:** teams predict the effect of a change, deploy, observe
telemetry, and write a postmortem when the observation differs. The learner rehearses exactly that
loop on a small system, at their own pace.

**Calibration** (calibración): the learner's progress is visible in the shrinking gap between what
they expected and what happened. A learner who predicts correctly is not “good at tests”; they have
a working mental model. A learner who predicts wrongly has found the exact place to study next.

**Misuse to avoid:** writing the prediction after seeing the result, or copying the explanation. The
check is only worth something if the prediction is written **before** the run. The material cannot
enforce honesty; the learner's own progress is the stake.

---

## 5. Motivation and achievability (motivación y dificultad alcanzable)

| Need | Design response |
|---|---|
| **Autonomy** (autonomía) | Choose track, rung, optional lesson and pace; no deadlines; no mandatory order beyond prerequisites |
| **Competence** (competencia) | Small wins; binary criteria; visible progress; each rung one step above the current level |
| **Relatedness** (relación) | Optional and asynchronous; a community may exist, but no lesson requires another person |
| **Flow** | Challenge slightly above skill; no artificial time pressure; no ranking |
| **Visible progress** | Before/after artifacts, cost tables, portfolio, and the shrinking prediction–observation gap (calibration); the learner can see what they could not do before |

The material must be **achievable**, not impressive. A rung that cannot be completed at your own
pace is a design bug.

---

## 6. The canonical lesson template (plantilla de lección)

A 60–90 minute concept lesson. Shorter formats are cuts of the same spine.

| # | Stage | Time | What happens | Evidence |
|---|---|---|---|---|
| 1 | **Hook & predict** (gancho y predicción) | 10 min | Read the code; predict behavior before running | Written prediction |
| 2 | **Run & observe** (ejecutar y observar) | 5 min | Run the variant; compare with the prediction | Prediction/reality diff |
| 3 | **Investigate** (investigar) | 15 min | Guided questions: where is the state? who mutates it? what breaks? | Answers with `file:line` |
| 4 | **Explain** (explicar) | 10 min | The concept, with subgoal labels and the historical anchor | Annotated code |
| 5 | **Modify** (modificar) | 20 min | The change test | Working modification |
| 6 | **Extend** (extender) | 20 min | Apply the concept to a new requirement | New small artifact |
| 7 | **Self-verify** (autoverificar) | 10 min | Run the acceptance criteria; compare with the prediction; record evidence | Acceptance report + prediction diff |
| 8 | **Reflect** (reflexionar) | 10 min | What did I learn? What can I not explain yet? What is next? | Reflection note |

**No stage requires another person.** Stage 8 can be a written note, a voice memo, or a recorded
explanation to camera; no audience is needed.

---

## 7. Cognitive load rules (reglas de carga cognitiva)

1. One concept per lesson.
2. Segment into 5–15 minute steps with visible outcomes.
3. Signal the changed lines; never make the learner hunt the diff.
4. Worked example first.
5. Fade: example → completion → independent.
6. No split attention: explanation next to the code it explains.
7. No redundancy: do not say the same thing in prose, comment and caption.
8. Concrete before abstract: run it, then name it.

---

## 8. Misconceptions (concepciones erróneas)

| Misconception | Diagnostic question | Refutation | Check |
|---|---|---|---|
| “A pointer is an integer.” | What does `p + 1` mean? | A pointer is a typed address; arithmetic scales by type | `sizeof(*p)` vs `sizeof(p)` |
| “A class is a struct with methods.” | What can a class enforce that a struct cannot? | Invariants and access control, not syntax | Make an invalid state unrepresentable |
| “Object-oriented programming means inheritance.” | Can you have object-oriented programming without inheritance? | Yes: encapsulation + dynamic dispatch | Add a type without editing the loop |
| “An abstract data type is a struct.” | Can you swap representation without touching callers? | An abstract data type is a specification, not a layout | Replace array with list |
| “Modular means splitting files.” | Can another module touch the internals? | Modularity is information hiding, not file count | Change a private helper; no caller breaks |
| “Generic means `void*`.” | What do you lose with `void*`? | Type safety and intent | A wrong type fails at compile time |
| “Globals are faster.” | Faster for whom, at what cost? | Locality wins micro-benchmarks; coupling loses systems | Change one global; count touched files |
| “It compiles, so it is correct.” | What does the compiler not know? | Behavior, invariants, intent | Golden traces |
| “Comments document the code.” | What can a comment not fix? | A bad name or a hidden coupling | Delete the comment; is the code clear? |
| “More abstraction is always better.” | What does it cost? | Indirection, files, concepts, debugging | The change test |

---

## 9. Self-paced by design (diseñado para una persona)

- Every lesson is completable **at your own pace**, offline, without an account.
- No group work, no pair programming, no peer review requirement, no classroom.
- A teacher may use the material, but the material does not require one.
- Community (issues, discussions) is **optional** and outside the method.
- The learner can start at any rung whose prerequisites are met.
- No deadlines; suggested time only.

**Accessibility:** multiple representations (prose, diagram, runnable code, transcript), no
color-only information, plain language, glossary at first mention, screen-reader-friendly headings
and tables.

---

## 10. What this pedagogy deliberately rejects

- Graded exams and quizzes.
- Points, curves, ranking, badges that compare learners.
- Rubrics converted into grades.
- Forced collaboration.
- Hidden criteria.
- Arbitrary deadlines.
- “Coverage” as a proxy for learning.

---

## 11. Why this is closer to real work

In engineering, nobody gives you a quiz. You get a requirement, a set of acceptance criteria, and a
running system. You either meet the criteria or you don't. The feedback is the behavior of the
artifact, not a letter.

That is the whole evaluation model of this project: **build, run, observe, decide**. The learner is
the engineer and the reviewer; the material is the bench, the criteria and the mirror.
