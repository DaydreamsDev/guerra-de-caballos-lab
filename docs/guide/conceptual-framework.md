# Conceptual framework (marco conceptual)
> [English](conceptual-framework.md) | [Español](conceptual-framework-es.md)

> **Canonical source.** If another document paraphrases this, this document wins. Do not redefine
> the four layers anywhere else; link here.

This is the differentiator of the whole project. It separates four things that most courses,
tutorials and debates collapse into one.

---

## 1. The two maxims (las dos máximas)

### 1.1 The epistemic maxim (máxima epistémica)

> **The code is the evidence, the concept is the decision, the paradigm is the map, the syntax is
> the dialect.**

> **El código es la evidencia, el concepto es la decisión, el paradigma es el mapa, la sintaxis es
> el dialecto.**

It answers: *what is each layer?*

### 1.2 The pedagogical maxim (máxima pedagógica)

> **The way of thinking is the goal, the paradigm is the map, the syntax is the gym — and none works
> in isolation.**

> **La forma de pensar es el objetivo, el paradigma es el mapa, la sintaxis es el gimnasio — y
> ninguno funciona aislado.**

It answers: *what is each layer for?*

The two are complementary, not redundant: the first is descriptive, the second is instructional.

---

## 2. The four layers (las cuatro capas)

| Layer | What it is | Example | Lifetime | Identity test | Failure when confused |
|---|---|---|---|---|---|
| **Concept** (Concepto) | A decision plus its consequence. Atomic, portable, language-independent | “Indirection: access through an address so the layout can change” | **Permanent** — survives language changes | Can you explain it without naming a language? | **Syntax as concept:** “the pointers chapter” teaches `*`, not indirection |
| **Paradigm** (Paradigma) | A bundle of concepts with a preferred order and a value system | Object-oriented programming (POO) prefers delegation and dynamic dispatch; modular prefers information hiding | **Decades** — survives technology cycles | Can you name what it optimizes for, and what it sacrifices? | **Paradigm tribalism:** “Object-oriented programming is better” without naming the concepts or the tradeoff |
| **Syntax** (Sintaxis) | The dialect that forces you to confront the concept | `->`, `virtual`, `#include`, `template` | **Per language** — survives a version, not a translation | Does it disappear when you change language? | **Cargo cult:** `private:` and the belief that object-oriented programming happened |
| **Implementation** (Implementación) | The runnable artifact that embodies one combination | `construction-09-abstract-data-type/` | **Per project / commit** — survives a commit, not a redesign | Does it pass the golden traces? | **Code as design:** “it works” used as the whole argument |

**Lifetime** is the key column: it tells you what survives a change. A concept survives a language
change; a paradigm survives a decade; a syntax survives a compiler version; an implementation
survives a commit. If your argument dies when the syntax changes, it was never about the concept.

---

## 3. How they relate (cómo se relacionan)

```
Concept  (what decision?)
   └── Paradigm  (what family and values?)
          └── Syntax  (what notation forces it?)
                 └── Implementation  (what evidence?)
```

Each layer answers a different question:

| Layer | Question | Output |
|---|---|---|
| Concept | What decision, and what does it buy? | A sentence without a language |
| Paradigm | What family, and what does it optimize? | A tradeoff |
| Syntax | What notation makes the decision unavoidable? | A feature |
| Implementation | What evidence proves it? | A running artifact + traces |

**Learn top-down; verify bottom-up.** The learner starts from the concept and descends to the
implementation. The reviewer starts from the evidence and ascends to the concept. Same four layers,
opposite direction.

### None works in isolation (ninguno funciona aislado)

| Missing layer | What remains | Name |
|---|---|---|
| Concept without paradigm | A good idea with no family, no order, no tradeoff | Philosophy |
| Paradigm without syntax | An ideology with no forcing function | Sect |
| Syntax without concept | A dialect with no decision behind it | Cargo cult |
| Implementation without concept | An accident that happens to compile | Coincidence |

The sentence “none works in isolation” is not rhetoric: each layer without the others produces a
recognizable failure mode.

---

## 4. The differentiator (el diferenciador)

Most teaching collapses the layers:

- “This is the pointers chapter” → **syntax presented as concept.**
- “Object-oriented programming vs functional programming” → **paradigm debate without naming the
  concepts.**
- “Clean code” → **syntax prescriptions without a decision.**
- “It works on my machine” → **implementation used as design evidence.**

This framework separates them, so the learner can:

1. **Transfer** a concept to another language and see what survives.
2. **Choose** a paradigm by tradeoff, not by fashion.
3. **Read** syntax as evidence of a decision, not as the decision.
4. **Judge** an implementation by behavior, not by familiarity.

### The four-question test (la prueba de las cuatro preguntas)

Any claim about code should survive these four questions:

1. **Concept:** what decision does this make, and what does it buy?
2. **Paradigm:** what family does it belong to, and what does that family optimize?
3. **Syntax:** what notation forces the decision?
4. **Implementation:** what evidence shows it works?

If a claim cannot answer all four, it is incomplete. If it answers only the third, it is a syntax
opinion.

---

## 5. Worked example: the board (ejemplo: el tablero)

| Layer | In *Guerra de Caballos* | Concrete form |
|---|---|---|
| **Concept** | Representation independence: the game logic should not know how the board is stored | “A move updates a cell; whether the board is an array, a list or a bitboard is not the game's business.” |
| **Paradigm** | Abstract data type / modular: hide representation behind operations | Abstract data type (tipo abstracto de datos, TAD) |
| **Syntax** | Opaque type + free functions | `struct Tablero;` + `crear`, `marcar`, `vecinas` |
| **Implementation** | `construction-09-abstract-data-type/` | Passes the traces; swapping the representation changes only `board.cpp` |

Second example, polymorphism:

| Layer | In *Guerra de Caballos* | Concrete form |
|---|---|---|
| **Concept** | Late binding: choose behavior at run time | “Add a piece without editing the loop.” |
| **Paradigm** | Object-oriented programming (POO) | Interface + dynamic dispatch (despacho dinámico) |
| **Syntax** | `virtual` / `override` | the `Piece` interface |
| **Implementation** | `construction-12-polymorphism/` | New piece type, zero edits in `game` |

The same concept can appear under different paradigms and syntaxes. That is the point: **the concept
is the portable part.**

---

## 6. Canonical usage rules (reglas de uso)

1. **Quote the maxims verbatim.** Do not paraphrase them in lessons; link here.
2. **Every design lesson card declares all four layers:** `concept`, `paradigm`, `syntax`,
   `artifact`.
3. **Every full failure card names the concept violated** and the paradigm that prevents it — not
   only the syntax that looks wrong.
4. **No redefinition elsewhere.** Other documents may summarize, but they link here.
5. **Change propagation is one-way:** edit this document first, then propagate.
6. **Translations are fixed.** The Spanish versions above are canonical too; do not improvise new
   ones.

---

## 7. Where it appears (dónde aparece)

| Document | How it uses the framework |
|---|---|
| `README.md` | Principles 2–3: concept before syntax; evidence over opinion |
| `how-learning-works.md` | Why the lesson is built on contrast, not exposition |
| `construction-track.md` | Each rung: concept, paradigm, forcing syntax, change test |
| `archaeology-track.md` | Each full failure card: concept violated, paradigm that prevents it |
| `lesson-catalog.md` | The design lesson card declares the four layers |
| `glossary.md` | The vocabulary of the four layers |

---

## 8. Common confusions (confusiones frecuentes)

| This is not that | Clarification |
|---|---|
| Concept ≠ paradigm | A concept is atomic; a paradigm is a bundle with values and an order. |
| Paradigm ≠ syntax | A paradigm can be expressed in many syntaxes; a syntax can serve many paradigms. |
| Syntax ≠ implementation | Syntax is notation; implementation is a running artifact. |
| Implementation ≠ design | “It works” is evidence of behavior, not proof of a decision. |
| Abstraction ≠ indirection | Abstraction hides detail; indirection accesses through a reference or address. They often cooperate, but they are different concepts. |
| Modular ≠ multiple files | Modularity is information hiding; file count is a packaging detail. |
| Abstract data type ≠ struct | An abstract data type is a specification; a struct is a possible representation. |
| Object-oriented programming ≠ inheritance | Object-oriented programming is identity, state, behavior and dynamic dispatch; inheritance is one reuse mechanism. |
| Generic ≠ `void*` | Generics preserve type safety and intent; `void*` erases both. |
| Familiar syntax ≠ understood concept | Recognition is not transfer. The test is another language and a new requirement. |

---

## 9. The one-line test (la prueba de una línea)

> **Name the decision, the family, the dialect and the evidence — or you are only talking about
> code.**

> **Nombra la decisión, la familia, el dialecto y la evidencia — o solo estás hablando de código.**
