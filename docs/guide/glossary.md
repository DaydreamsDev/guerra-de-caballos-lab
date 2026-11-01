# Glossary (glosario)
> [English](glossary.md) | [Español](glossary-es.md)

Single source of truth for terminology and translations. For the four-layer framework, the canonical
source is [`conceptual-framework.md`](conceptual-framework.md); the entries below are
summaries, not redefinitions.

Format: **English** — *Español* — definition — common misconception (if any).

---

## Foundations (fundamentos)

| English | Español | Definition | Misconception |
|---|---|---|---|
| Concept | Concepto | A decision plus its consequence; atomic, portable, language-independent | “A concept is a keyword.” |
| Conceptual framework | Marco conceptual | The four-layer model: concept, paradigm, syntax, implementation | “It is just a glossary.” |
| Layer | Capa | One level of the framework; each has its own lifetime and test | “A layer is a section.” |
| Paradigm | Paradigma | A bundle of concepts with a preferred order and a value system | “A paradigm is a syntax style.” |
| Syntax | Sintaxis | The dialect that forces you to confront the concept | “Syntax is the concept.” |
| Implementation | Implementación | The runnable artifact that embodies one combination | “The code is the design.” |
| Abstraction | Abstracción | Hiding detail behind a stable interface | “Abstraction means more layers.” |
| Complexity | Complejidad | The cost of understanding and changing a system | “Complexity is lines of code.” |

## Paradigms (paradigmas)

| English | Español | Definition | Misconception |
|---|---|---|---|
| Non-structured programming | Programación no estructurada | Unrestricted control flow (e.g. `goto`), single global scope; also called monolithic programming (*monolítico*) | “It just means no functions.” |
| Procedural programming | Programación procedimental | Decomposition into procedures/functions with parameters | “It is the same as structured.” |
| Structured programming | Programación estructurada | Only sequence, selection, iteration; single entry/exit | “It means using functions.” |
| Modular programming | Programación modular | Modules with interface/implementation split and information hiding | “It means splitting files.” |
| Abstract data type (ADT) | Tipo abstracto de datos (TAD; also TDA) | A specification of operations independent of representation | “An ADT is a struct.” |
| Object-oriented programming (OOP) | Programación orientada a objetos (POO) | Objects with identity, state, behavior and dynamic dispatch | “OOP means inheritance.” |
| Generic programming | Programación genérica | Algorithms parameterized over types | “Generic means `void*`.” |
| Functional programming | Programación funcional | Computation as pure functions and immutable values | “Functional means using lambdas.” |
| Data-oriented design | Diseño orientado a datos | Layout and access patterns drive the design | “It is just optimization.” |

## Data representation (representación de datos)

| English | Español | Definition | Misconception |
|---|---|---|---|
| Record | Registro | A composite value with named fields (`struct`) | “A record is just a bag of variables.” |
| Array of records (AoS) | Arreglo de estructuras | A collection stored record by record | “AoS is always slower.” |
| Struct of arrays (SoA) | Estructura de arreglos | A collection stored field by field | “SoA is always faster.” |
| Parallel arrays | Arreglos paralelos | Several arrays indexed by the same position | “It is simpler than a record.” |
| Indirection | Indirección | Accessing a value through a reference or address | “Indirection is a pointer.” |
| Reference | Referencia | An alias to an existing object | “A reference is a pointer.” |
| Pointer | Puntero | A typed value holding a memory address | “A pointer is an integer.” |
| Ownership | Propiedad | Responsibility for an object's lifetime | “Ownership is garbage collection.” |
| Lifetime | Ciclo de vida | The interval during which an object is valid | “Lifetime is scope.” |
| RAII | RAII (adquisición de recursos en la inicialización) | Resource lifetime tied to object lifetime | “RAII is only for memory.” |

## Design (diseño)

| English | Español | Definition | Misconception |
|---|---|---|---|
| Encapsulation | Encapsulación | Protecting an invariant behind an interface | “Encapsulation is `private`.” |
| Information hiding | Ocultamiento de información | Clients cannot depend on internal representation | “Hiding is the same as encapsulating.” |
| Inheritance | Herencia | Deriving a type from another | “Inheritance is reuse.” |
| Polymorphism | Polimorfismo | One interface, many implementations | “Polymorphism is overloading.” |
| Dynamic dispatch | Despacho dinámico | Choosing the implementation at run time | “Dispatch is a `switch`.” |
| Composition | Composición | Building behavior by combining objects | “Composition is inheritance with extra steps.” |
| Coupling | Acoplamiento | Degree of dependence between parts | “Coupling is always bad.” |
| Cohesion | Cohesión | Degree to which a part has one responsibility | “Cohesion is the same as coupling.” |
| Invariant | Invariante | A property that must always hold | “An invariant is a comment.” |
| Contract | Contrato | Preconditions, postconditions, invariants | “A contract is documentation.” |
| Specification | Especificación | The behavior a program must exhibit | “The code is the spec.” |

## Quality and legacy (calidad y legado)

| English | Español | Definition | Misconception |
|---|---|---|---|
| Legacy code | Código heredado | Code that is hard to change and has no tests | “Legacy means old.” |
| Symptom | Síntoma | The visible effect of a failure | “The symptom is the cause.” |
| Root cause | Causa raíz | The condition that produces a class of failures | “Every bug has one cause.” |
| Technical debt | Deuda técnica | A design choice that makes future changes more expensive | “Debt is always bad.” |
| Self-review | Autopsia / autorrevisión | A review of one's own past work, focused on systems, not blame | “Self-review is self-criticism.” |
| Failure card | Ficha de fallo | A structured record: evidence, class, severity, root cause, minimal and structural fix, test, concept violated | “A card is a complaint.” |
| Refactoring | Refactorización | A behavior-preserving change to structure | “Refactoring is rewriting.” |
| Minimal fix | Arreglo mínimo | The smallest change that removes the harm | “A minimal fix is a patch.” |
| Structural fix | Arreglo estructural | A change that prevents the class of failure | “A structural fix is a rewrite.” |
| Code review | Revisión de código | Evidence-based review of a change | “Review is style policing.” |

## Verification (verificación)

| English | Español | Definition | Misconception |
|---|---|---|---|
| Acceptance criteria | Criterios de aceptación | Observable, binary conditions that define whether the artifact meets its goal | “Acceptance criteria are a rubric.” |
| Definition of done | Definición de terminado | The checklist that makes a lesson complete for the learner | “Done means it compiles.” |
| Engineering check | Comprobación de ingeniería | Predict → run → compare → explain → record; an untimed, ungraded check against reality | “It is a quiz.” |
| Prediction note | Nota de predicción | The expected behavior written before running the artifact | “Prediction is guessing.” |
| Calibration | Calibración | The gap between what you expected and what happened; a progress signal | “Calibration is a grade.” |
| Evidence | Evidencia | A runnable artifact, output, trace or report | “Evidence is an opinion.” |
| Determinism | Determinismo | Same input and seed produce the same output | “Determinism means no randomness.” |
| Seed | Semilla | The value that initializes a pseudo-random sequence | “The seed is a password.” |
| Harness | Arnés (banco de pruebas) | A runner that executes a variant and captures its behavior | “A harness is a test framework.” |
| Golden trace | Traza de referencia | A recorded expected output used for comparison | “A trace is a log.” |
| Change test | Prueba de cambio | A requirement used to measure the cost of a design | “A change test is a unit test.” |
| Contrast pair | Par de contraste | Two runnable versions differing in one concept | “A pair is two examples.” |
| Evidence portfolio | Portafolio de evidencia | The learner's private record of artifacts, criteria and reflections | “A portfolio is a submission.” |

## Reproducibility and harness (reproducibilidad y arnés)

| English | Español | Definition | Misconception |
|---|---|---|---|
| Transcript | Transcripción | The complete record of one run, one line per record | “A transcript is a log.” |
| Trace | Traza | A recorded transcript used as evidence | “A trace is a test.” |
| Digest | Resumen criptográfico | A fixed-length fingerprint of a byte sequence | “A digest is encryption.” |
| Build step | Paso de compilación | A command that turns source into a runnable artifact | “Every project needs one.” |
| Interpreter | Intérprete | A program that executes source directly | “An interpreter is a compiler.” |
| Entry point | Punto de entrada | The file or command that starts a program | “An entry point is a function.” |
| Working directory | Directorio de trabajo | The directory a command runs from | “The working directory is the repository root.” |
| Location independence | Independencia de ubicación | A command works from any directory | “Location independence means no paths.” |
| Build reproducibility | Reproducibilidad de la compilación | Same source and tools produce the same behavior | “It means no build.” |
| Reference implementation | Implementación de referencia | A complete implementation used as the standard | “It is the only implementation.” |

## Learning design (diseño de aprendizaje)

| English | Español | Definition | Misconception |
|---|---|---|---|
| Constructivism | Constructivismo | Knowledge is built from experience | “Constructivism means no guidance.” |
| Constructionism | Construccionismo | Learning by making shareable artifacts | “Constructionism is just projects.” |
| Heutagogy | Heutagogía | Self-determined learning: the learner decides what, how and when | “Heutagogy is just self-study.” |
| Self-regulated learning | Aprendizaje autorregulado | Forethought, performance and self-reflection | “Self-regulation is willpower.” |
| Self-determination | Autodeterminación | Motivation through autonomy, competence and relatedness | “Motivation is a reward.” |
| Flow | Flujo | Engagement when challenge slightly exceeds skill | “Flow is relaxation.” |
| Authentic assessment | Evaluación auténtica | Judging performance on real tasks | “Authentic means realistic-looking.” |
| Feedback literacy | Alfabetización de retroalimentación | The capacity to judge quality and act on evidence | “Feedback is a grade.” |
| Ungrading | Sin notas | Removing grades and replacing them with feedback and evidence | “Ungrading means no standards.” |
| Criterion-referenced | Referenciado a criterios | Judging against explicit criteria, not against other learners | “Criterion means norm.” |
| Ipsative | Ipsativo | Comparing with one's own previous work | “Ipsative means subjective.” |
| Self-diagnosis | Autodiagnóstico | Four questions: can I explain it, change it, extend it, and did my prediction match? | “Self-diagnosis is self-grading.” |
| Worked example | Ejemplo resuelto | A fully solved case studied before solving | “Examples are for beginners only.” |
| Fading | Desvanecimiento | Gradually removing support | “Fading is deleting the solution.” |
| Subgoal label | Etiqueta de subobjetivo | A name for a meaningful part of an example | “Labels are comments.” |
| Cognitive load | Carga cognitiva | Working-memory demand of a task | “Load is difficulty.” |
| Mental model | Modelo mental | The internal explanation a learner uses to predict behavior | “A mental model is an opinion.” |
| Threshold concept | Concepto umbral | A concept that transforms understanding and is hard to unlearn | “Threshold means advanced.” |
| Misconception | Concepción errónea | A stable wrong model that resists explanation | “A misconception is a mistake.” |
| Formative feedback | Retroalimentación formativa | Information used to decide the next step; never graded | “Formative means ungraded.” |

## Rejected schooling terms (términos escolares rechazados)

| English | Español | Why it is rejected |
|---|---|---|
| Exam | Examen | Timed recall under artificial constraints; not how engineering work is accepted. |
| Quiz | Cuestionario | Same; prediction against a trace keeps the learning mechanism without the ceremony. |
| Grade | Nota | Shifts attention to scoring and comparison; undermines intrinsic motivation. |
| Rubric with levels | Rúbrica con niveles | Becomes a grade in disguise; replaced by binary acceptance criteria. |
| Checkpoint gate | Punto de control | A gate implies permission; replaced by acceptance criteria the learner runs. |
| Ranking | Ranking / clasificación | Compares learners instead of evidence. |
| Forced group work | Trabajo grupal obligatorio | The learner may study independently; collaboration is optional and outside the method. |

## Project vocabulary (vocabulario del proyecto)

| English | Español | Definition | Misconception |
|---|---|---|---|
| Invariant (project sense) | Invariante (del proyecto) | The game behavior that never changes across variants | “The code is the invariant.” |
| Rung | Peldaño | One step of the concept ladder | “A rung is a folder.” |
| Lens | Lente | An orthogonal way of thinking applied to the same game | “A lens is an advanced rung.” |
| Track | Pista | A learning route: construction or archaeology | “A track is a difficulty level.” |
| Provenance | Procedencia | The origin, authorship and license of an artifact | “Provenance is a comment.” |

---

## Maintenance rule

- Add a term only when a lesson needs it.
- One definition per term; no synonyms in the definition field.
- If two terms are often confused, add a row in the “Misconception” column, not a second term.
- Spanish translations are checked against this glossary before the `es` edition ships.
