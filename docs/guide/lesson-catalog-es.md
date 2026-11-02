# Catálogo de lecciones (lesson catalog)

> [English](lesson-catalog.md) | [Español](lesson-catalog-es.md)

Este es el **catálogo**, no la ruta. Una persona que aprende sigue una pista; un docente puede
elegir de toda la tabla. Cada entrada se puede **completar de forma independiente**.

Cada entrada tiene un ID estable, un concepto, prerrequisitos, un tiempo estimado, un tiempo de
lectura y una **prueba de cambio** que define el éxito. El catálogo es la fuente de los archivos
`meta.yml`. El tiempo estimado es el de los pasos activos; el tiempo de lectura está en `meta.yml` y
en la cabecera de la lección.

Leyenda de estados: `planned` · `drafted` · `in review` · `stable`.

### Política de estados

El estado describe evidencia, no esfuerzo.

| Estado | Significado | Evidencia requerida |
|---|---|---|
| `planned` | Existen el ID y el alcance; todavía no hay lección para quien aprende. | Solo la entrada del catálogo. |
| `drafted` | Existen ambas ediciones y la lección está completa para ejecutarse. | `lesson.md`, `lesson-es.md`, `meta.yml`, enlaces válidos y criterios de aceptación. |
| `in review` | La lección está en revisión: la QA automatizada pasó y la revisión pedagógica independiente está abierta. | Evidencia de borrador, lista de QA y hallazgos de la revisión. |
| `stable` | La lección pasó la revisión, una persona externa la completó y los hallazgos están cerrados. | Evidencia de revisión, evidencia de quien aprende y lista de hallazgos cerrada. |

La promoción no es automática. Una lección sigue en `drafted` hasta que una persona revisora
registra la evidencia. El catálogo y `meta.yml` deben coincidir. Los tokens de máquina siguen en
inglés; la prosa en español puede mostrar `borrador` para `drafted`.

---

## 0. Núcleo común (common core)

| ID | Título | Concepto | Prerrequisito | Tiempo | Criterios de aceptación | Estado |
|---|---|---|---|---|---|---|
| [common-00](../../common/lessons/common-00-how-to-learn/lesson-es.md) | Cómo aprender (how to learn) | Metacognición (metacognition) | ninguno | 15 min | Un ciclo predecir–ejecutar–comparar–explicar–registrar y un siguiente paso registrados | drafted |
| [common-01](../../common/lessons/common-01-game-invariant/lesson-es.md) | El juego como invariante (the game as an invariant) | Especificación (specification) | ninguno | 50 min | Dos ejecuciones idénticas; una ambigüedad y un criterio binario registrados | drafted |
| [common-02](../../common/lessons/common-02-deterministic-harness/lesson-es.md) | Arnés determinista (deterministic harness) | Reproducibilidad (reproducibility) | common-01 | 60 min | La misma semilla → la misma transcripción, dos veces | drafted |
| [common-03](../../common/lessons/common-03-glossary/lesson-es.md) | Glosario (glossary) | Vocabulario compartido (shared vocabulary) | ninguno | 20 min | Diez términos definidos en inglés y español, una línea y una concepción errónea cada uno | drafted |
| [common-04](../../common/lessons/common-04-build-and-run/lesson-es.md) | Compilar y ejecutar (build and run) | Reproducibilidad de la compilación | common-02 | 30 min | Un comando se ejecuta desde dos directorios; la misma semilla da la misma transcripción | drafted |
| [common-05](../../common/lessons/common-05-golden-traces/lesson-es.md) | Trazas de referencia (golden traces) | Contrato de comportamiento (behavioral contract) | common-02 | 45 min | Las sumas pasan; una traza reproducida; comando, salida y explicación registrados | drafted |

Las filas siguen el orden de aprendizaje. `common-03` es una referencia lateral que puedes leer
antes de `common-02` o `common-04`.

**common-01** es el documento más importante del proyecto. Si las reglas son ambiguas,
todo contraste es inválido.

---

## 1. Pista A — Construcción (construction)

Las variantes de mecanismo (`mechanism-01`…`mechanism-05`) están incorporadas en
`construction-04`…`construction-07` y `construction-19`; no son IDs de lección separados.

En las lecciones de diseño, el criterio de aceptación es la **prueba de cambio**; en la lección de
especificación (`construction-00`), es la **comprobación de predicción**.

| ID | Título | Concepto | Prerrequisito | Tiempo | Criterio de aceptación | Estado |
|---|---|---|---|---|---|---|
| [construction-00](../../construction/lessons/construction-00-read-the-rules/lesson-es.md) | Leer las reglas, ejecutar la línea base de referencia | Especificación | common-01, common-02 | 50 min | Predecir el siguiente movimiento de una partida con semilla | drafted |
| [construction-01](../../construction/lessons/construction-01-non-structured/lesson-es.md) | Programación no estructurada (non-structured programming) | Flujo de control arbitrario | construction-00 | 70 min | Añadir «saltar turno»; contar los saltos recableados | drafted |
| [construction-02](../../construction/lessons/construction-02-structured/lesson-es.md) | Programación estructurada (structured programming) | Secuencia, selección, iteración | construction-01 | 65 min | El mismo cambio, localizado; sin `goto` | drafted |
| [construction-03](../../construction/lessons/construction-03-functions/lesson-es.md) | Funciones y parámetros (functions and parameters) | Ámbito y descomposición | construction-02 | 45 min | Extraer un movimiento sin variables globales | drafted |
| [construction-04](../../construction/lessons/construction-04-records/lesson-es.md) | Registros (records) | Agregación | construction-03 | 55 min | Añadir `captured` a un caballo | drafted |
| [construction-05](../../construction/lessons/construction-05-array-of-records/lesson-es.md) | Arreglo de estructuras (array of records) | Colección y recorrido | construction-04 | 62 min | Recorrer el tablero sin 64 variables | drafted |
| [construction-06](../../construction/lessons/construction-06-references/lesson-es.md) | Referencias (references) | Alias sin direcciones | construction-05 | 45 min | Mutar un registro a través de un parámetro | drafted |
| [construction-07](../../construction/lessons/construction-07-pointers/lesson-es.md) | Punteros (pointers) | Indirección y ciclo de vida | construction-06 | 88 min | Acceder a una casilla por puntero; explicar el paso de fila; trazas sin cambios | drafted |
| [construction-08](../../construction/lessons/construction-08-modular/lesson-es.md) | Programación modular (modular programming) | Ocultamiento de información | construction-03, construction-07 | 92 min | Tablero 8×8 → 10×10; solo cambia el módulo del tablero | drafted |
| [construction-09](../../construction/lessons/construction-09-abstract-data-type/lesson-es.md) | Tipo abstracto de datos (abstract data type, TAD) | Independencia de la representación | construction-08 | 97 min | Cambiar la representación del tablero; los llamadores compilan; las trazas pasan | drafted |
| [construction-10](../../construction/lessons/construction-10-adt-invariants/lesson-es.md) | Invariantes del tipo abstracto de datos (abstract data type invariants) | Validez y ciclo de vida | construction-09 | 60 min | Escrituras fuera de rango rechazadas; board_is_valid verdadero; quitar la guarda expone el estado inválido | drafted |
| [construction-11](../../construction/lessons/construction-11-encapsulation/lesson-es.md) | Encapsulación (encapsulation) | Clase con invariante | construction-10 | 60 min | El estado y las operaciones privadas preservan el invariante; el estado público expone un tablero inválido | drafted |
| [construction-12](../../construction/lessons/construction-12-polymorphism/lesson-es.md) | Polimorfismo (polymorphism) | Ligadura dinámica (late binding) | construction-11 | 80 min | Añadir una subclase de Piece; cero ediciones en el bucle; trazas originales sin cambios | drafted |
| [construction-13](../../construction/lessons/construction-13-composition/lesson-es.md) | Composición (composition) | Composición sobre herencia | construction-12 | 68 min | Sustituir herencia por delegación; comportamiento y 20 trazas sin cambios | drafted |
| [construction-14](../../construction/lessons/construction-14-generic-functions/lesson-es.md) | Funciones genéricas (generic functions) | Parámetros de tipo | construction-12 o construction-13 | 68 min | Un generador de movimientos para dos tipos de pieza | drafted |
| [construction-15](../../construction/lessons/construction-15-generic-classes/lesson-es.md) | Clases genéricas y conceptos (generic classes and concepts) | Restricciones | construction-14 | 81 min | La misma plantilla de motor instancia 8×8 y 10×10; las trazas de 8×8 pasan | drafted |
| [construction-16](../../construction/lessons/construction-16-ownership/lesson-es.md) | Propiedad y ciclo de vida de recursos (ownership) | Ciclo de vida | construction-15 | 70 min | Propiedad con unique_ptr; copia eliminada; movimiento disponible; 20 trazas pasan | drafted |
| [construction-17](../../construction/lessons/construction-17-stl/lesson-es.md) | Biblioteca de plantillas estándar y algoritmos (Standard Template Library and algorithms) | Abstracción de la iteración | construction-05, construction-16 | 68 min | La misma transcripción tras reemplazar tres recorridos | drafted |
| [construction-18](../../construction/lessons/construction-18-functional/lesson-es.md) | Estilo funcional (functional style) | Pureza e inmutabilidad | construction-17 | 85 min | Deshacer/rehacer gratis mediante el historial del tablero | drafted |
| [construction-19](../../construction/lessons/construction-19-data-oriented/lesson-es.md) | Diseño orientado a datos (data-oriented design) | Localidad | construction-05, construction-17 | 85 min | 10,000 partidas; perfil medido | drafted |
| [construction-20](../../construction/lessons/construction-20-hexagonal/lesson-es.md) | Hexagonal (hexagonal) | Puertos y adaptadores | construction-08, construction-17 | 92 min | El mismo núcleo, adaptadores de consola y de prueba | drafted |
| [construction-21](../../construction/lessons/construction-21-concurrency/lesson-es.md) | Concurrencia (concurrency) | Estado compartido frente a mensajes | construction-12, construction-20 | 115 min | Condición de carrera detectada y eliminada; determinismo conservado | drafted |
| [construction-22](../../construction/lessons/construction-22-error-contracts/lesson-es.md) | Contratos de error (error contracts) | Recuperación | construction-09, construction-21 | 75 min | Movimiento inválido gestionado sin `exit` | drafted |
| [construction-23](../../construction/lessons/construction-23-port/lesson-es.md) | Portar a otro lenguaje (port to another language) | La sintaxis es desechable | construction-09 o construction-12 | 195 min | Las mismas trazas desde Python/Rust | drafted |
| [construction-24](../../construction/lessons/construction-24-capstone/lesson-es.md) | Proyecto final: informe de costes (capstone: cost report) | Justificación de compromisos | construction-12, construction-15 | 175 min | Defender una decisión de diseño con números | drafted |

### Ruta de la pista A (recomendada)

`construction-00 → construction-01 → construction-02 → construction-03 → construction-04 →
construction-05 → construction-06 → construction-07 → construction-08 → construction-09 →
construction-10 → construction-11 → construction-12 → construction-14 → construction-15`

Lentes (elige dos; `construction-20` es la primera por defecto): `construction-16`,
`construction-17 → construction-18`, `construction-20`.
Proyecto final: `construction-24`.

---

## 2. Pista B — Arqueología (archaeology)

| ID | Título | Clase de fallo | Prerrequisito | Tiempo | Criterios de aceptación | Estado |
|---|---|---|---|---|---|---|
| [archaeology-00](../../archaeology/lessons/archaeology-00-orientation/lesson-es.md) | Orientación: leer un código heredado | — | common-04 | 45 min | Mapa de código + propósito en una frase | drafted |
| [archaeology-01](../../archaeology/lessons/archaeology-01-build-today/lesson-es.md) | Compilar y ejecutar código heredado | — | common-04 | 62 min | Compilación ejecutable o bloqueo documentado | drafted |
| [archaeology-02](../../archaeology/lessons/archaeology-02-code-inventory/lesson-es.md) | Inventario de código y grafo de llamadas | — | archaeology-01 | 60 min | Inventario + grafo de llamadas con evidencia | drafted |
| [archaeology-03](../../archaeology/lessons/archaeology-03-reverse-spec/lesson-es.md) | Ingeniería inversa de la especificación | — | archaeology-02 | 90 min | Especificación + trazas del original | drafted |
| [archaeology-04](../../archaeology/lessons/archaeology-04-correctness-failures/lesson-es.md) | Fallos de corrección (failure-001…failure-006) | correctness | archaeology-03 | 100 min | 6 fichas de fallo + pruebas | drafted |
| [archaeology-05](../../archaeology/lessons/archaeology-05-global-state/lesson-es.md) | Estado global y acoplamiento oculto | design | archaeology-04 | 60 min | Mapa de mutaciones por cada global | drafted |
| [archaeology-06](../../archaeology/lessons/archaeology-06-duplication-switch/lesson-es.md) | Duplicación y el switch de ocho casos | design | archaeology-04 | 60 min | Una extracción, comportamiento conservado | drafted |
| [archaeology-07](../../archaeology/lessons/archaeology-07-representation-leakage/lesson-es.md) | Fuga de representación y arreglos paralelos | design | archaeology-05 | 60 min | Registro introducido; llamadores actualizados | drafted |
| [archaeology-08](../../archaeology/lessons/archaeology-08-module-boundary/lesson-es.md) | Frontera de módulo ausente | design | archaeology-05 | 90 min | Cabecera + implementación; interior oculto | drafted |
| [archaeology-09](../../archaeology/lessons/archaeology-09-abstract-data-type/lesson-es.md) | Tipo abstracto de datos ausente | design | archaeology-08 | 90 min | Tipo opaco; cambio de representación | drafted |
| [archaeology-10](../../archaeology/lessons/archaeology-10-polymorphism/lesson-es.md) | Polimorfismo ausente | design | archaeology-09 | 90 min | Switch sustituido por despacho | drafted |
| [archaeology-11](../../archaeology/lessons/archaeology-11-input-output/lesson-es.md) | Entrada y salida y capacidad de prueba | design | archaeology-09 | 60 min | Núcleo ejecutable sin consola | drafted |
| [archaeology-12](../../archaeology/lessons/archaeology-12-naming-comments/lesson-es.md) | Nombres y comentarios | professional-practice | archaeology-04 | 60 min | Renombrado + reescritura de comentarios con motivos | drafted |
| [archaeology-13](../../archaeology/lessons/archaeology-13-documentation/lesson-es.md) | Documentación técnica para mantenedores | professional-practice | archaeology-03 | 60 min | README + especificación + compilación/ejecución/prueba | drafted |
| [archaeology-14](../../archaeology/lessons/archaeology-14-tests-determinism/lesson-es.md) | Pruebas y determinismo | professional-practice | archaeology-03 | 90 min | Trazas de referencia + control de semilla | drafted |
| [archaeology-15](../../archaeology/lessons/archaeology-15-static-analysis/lesson-es.md) | Análisis estático y advertencias del compilador | professional-practice | archaeology-04 | 60 min | Advertencias clasificadas; cero advertencias; comportamiento conservado | drafted |
| [archaeology-16](../../archaeology/lessons/archaeology-16-platform-lock-in/lesson-es.md) | Dependencia de plataforma | robustness | archaeology-01 | 60 min | Compilación portable; adaptador para consola | drafted |
| [archaeology-17](../../archaeology/lessons/archaeology-17-code-review/lesson-es.md) | Revisión de código de una función heredada | professional-practice | archaeology-04 | 60 min | 3 hallazgos + severidad + una prueba | drafted |
| [archaeology-18](../../archaeology/lessons/archaeology-18-refactor-workshop/lesson-es.md) | Taller de refactorización | all failure classes | archaeology-04, archaeology-05, archaeology-08, archaeology-09 | 180 min | Legado → peldaño de la pista A, trazas iguales | drafted |
| [archaeology-19](../../archaeology/lessons/archaeology-19-review-retrospective/lesson-es.md) | Lista de revisión y retrospectiva | — | archaeology-18 | 90 min | Lista reutilizable; retrospectiva final | drafted |

### Ruta de la pista B (recomendada)

`archaeology-00 → archaeology-01 → archaeology-02 → archaeology-03 → archaeology-04 → archaeology-05
→ archaeology-08 → archaeology-09 → archaeology-14 → archaeology-15 → archaeology-17 →
archaeology-18 → archaeology-19`

Lentes opcionales: `archaeology-06`, `archaeology-07`, `archaeology-10`, `archaeology-11`,
`archaeology-12`, `archaeology-13`, `archaeology-16`.

La ruta es una **columna vertebral**, no una prisión: quien revisa puede saltar a la clase de fallo
que necesite.

> **Hueco y alcance de la clase:** falta el peldaño histórico de la clase, y la versión moderna de
> la clase se especifica aquí en `construction-11`, `construction-12` y `construction-13`. Cualquier
> otro paso que no está en los originales — programación modular, tipos abstractos de datos,
> programación orientada a objetos, programación genérica y las lentes — nunca se pensó para
> resolverse en los archivos heredados. Son extensiones del plan de estudios, no artefactos
> históricos ausentes. Consulta [`archaeology-track-es.md`](archaeology-track-es.md)
> para la nota de alcance.

---

## 3. Producto mínimo viable (alcance)

| Pista | Incluido | Por qué |
|---|---|---|
| Común | common-00 … common-05 | Sin especificación, arnés y una forma de aprender, ninguna lección es válida |
| A | construction-00 … construction-09 | Cubre flujo de control, agregación, indirección, modularidad, tipo abstracto de datos — el núcleo conceptual |
| B | archaeology-00 … archaeology-10 + las primeras 15 fichas de fallo | Cubre corrección, diseño y los huecos profesionales que más importan |

Todas las lecciones están ahora en borrador; el MVP sigue siendo el alcance mínimo de la primera
entrega. Las quince fichas de fallo están en borrador.

---

## 4. Grafo de dependencias (textual)

La columna `Prereq` de arriba es la fuente autoritativa. Este es un mapa de alto nivel, no una
segunda especificación.

```
Common core — learning order
  common-00 learn ─► common-01 spec ─► common-02 harness
  common-02 harness ─┬─► common-04 build ─► common-05 traces
                     └─► common-05 traces
  common-03 glossary ─► before common-02 or common-04

Track A — paradigm ladder
  construction-00 ─► construction-01 ─► construction-02 ─► construction-03 ─►
  construction-04 ─► construction-05 ─► construction-06 ─► construction-07
  construction-03, construction-07 ─► construction-08 ─► construction-09 ─►
  construction-10 ─► construction-11 ─► construction-12
  construction-12 ─► construction-13, construction-14
  construction-14 ─► construction-15 ─► construction-16
  construction-05, construction-16 ─► construction-17
  construction-17 ─► construction-18, construction-19, construction-20
  construction-12, construction-20 ─► construction-21
  construction-09, construction-21 ─► construction-22
  construction-09 or construction-12 ─► construction-23
  construction-12, construction-15 ─► construction-24

Track B — autopsy spine
  archaeology-00 (common-04); archaeology-01 (common-04)
    ─► archaeology-02 ─► archaeology-03 ─► archaeology-04
    ─► archaeology-05 ─► archaeology-08 ─► archaeology-09
  archaeology-03 ─► archaeology-13, archaeology-14
  archaeology-04 ─► archaeology-06, archaeology-12
  archaeology-05 ─► archaeology-07
  archaeology-09 ─► archaeology-10, archaeology-11
  archaeology-01 ─► archaeology-16
  archaeology-04 ─► archaeology-15, archaeology-17
  archaeology-04, archaeology-05, archaeology-08, archaeology-09 ─► archaeology-18 ─► archaeology-19
```

Consulta la tabla para las aristas exactas. El mapa existe para mostrar la forma, no para sustituir
los prerrequisitos.

---

## 5. Ficha de lección (lesson card)

Toda lección de diseño, cuando se escribe, parte de esta ficha:

```yaml
id: construction-04
slug: records
title: "Records (Registros)"
track: A
concept: "Aggregation (agregación)"
paradigm: "Structured (programación estructurada)"
syntax: "struct"
artifact: "construction/code/mechanism-02-records/"
question: "Can I add a field without touching every signature?"
prereq: [construction-03]
time_min: 60
change_test: "Add 'captured' to a knight; count touched files."
engineering_check: "Predict which signatures change; run the acceptance script; explain the diff."
evidence:
  - working modified variant
  - prediction note and its comparison with the observation
  - one-paragraph explanation without using the word 'struct'
misconceptions:
  - "A struct is just a bag of variables."
  - "Records are for large programs only."
languages: [en, es]
status: drafted
```

La ficha es el contrato. Si una lección no puede rellenar `change_test`, no es una lección; es un
artículo.
