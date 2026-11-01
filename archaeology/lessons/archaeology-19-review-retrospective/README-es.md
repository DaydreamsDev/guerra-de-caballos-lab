# archaeology-19 — Lista de revisión y retrospectiva

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** revisión y retrospectiva ·
> **Prerrequisito:** archaeology-18 · **Tiempo estimado:** 90 minutos

Esta es la última lección de Track B. No diagnostica un fallo nuevo. Convierte el recorrido en un
instrumento reutilizable: una lista de revisión que otra persona puede aplicar a código desconocido,
y una retrospectiva que separa lo que aprendiste de lo que solo leíste.

El archivo ofrece un conjunto finito de fallos. El hábito de revisar es lo que sobrevive al archivo.

La lección termina cuando otra
persona puede aplicar tu lista y reproducir tu veredicto a partir de tu evidencia.

## Qué hace la persona que aprende

- Inspecciona el portafolio producido en `archaeology-00` a `archaeology-18`.
- Audita el `REVIEW-CHECKLIST-es.md` incluido y reescribe los elementos vagos como pruebas
  observables.
- Construye una lista reutilizable sobre corrección, diseño, robustez, práctica profesional y
  evidencia.
- Aplica la lista a una variante sellada del archivo, en una copia de trabajo y con tiempo acotado.
- Compara hallazgos detectados, omitidos y falsos positivos con el catálogo de fallos.
- Escribe una retrospectiva del recorrido completo: qué conservar, qué dejar y qué empezar.

## El instrumento

```text
REVIEW-CHECKLIST.md      la plantilla de partida
REVIEW-CHECKLIST-es.md   edición en español de la plantilla
```

La plantilla es corta a propósito. Nombra las cinco familias y ofrece una frase por elemento. Tu
trabajo es convertir cada frase en un elemento que dos revisores puedan aplicar y alcanzar el mismo
veredicto con la misma evidencia.

Un elemento es utilizable cuando nombra:

1. la pregunta;
2. la evidencia que la responde;
3. el criterio de aprobación;
4. la clase de fallo;
5. el escalón de Track A que previene la clase.

## Las cinco familias

| Familia | Pregunta | Fallos de ejemplo | Ancla en Track A |
|---|---|---|---|
| Corrección | ¿Hace lo que afirma? | failure-001…failure-006 | `construction-02`, `construction-04`, `construction-05` |
| Diseño | ¿La estructura encarece el cambio? | failure-007…failure-014 | `construction-08`, `construction-09`, `construction-12` |
| Robustez | ¿Funciona solo en un entorno? | failure-006, failure-015 | `construction-08`, `construction-20` |
| Práctica profesional | ¿Otra persona puede compilarlo, confiar en él y mantenerlo? | failure-016…failure-019 | `common-01`, `common-03`, `common-04` |
| Evidencia | ¿La afirmación se puede comprobar? | todas las clases | `common-02`, `common-04` |

## Ficheros

| Fichero | Propósito |
|---|---|
| `README.md` | Este fichero. |
| `README-es.md` | Edición en español. |
| `lesson.md` | La lección guiada. |
| `lesson-es.md` | Edición en español de la lección guiada. |
| `REVIEW-CHECKLIST.md` | La plantilla que se audita y se amplía. |
| `REVIEW-CHECKLIST-es.md` | Edición en español de la plantilla. |
| `meta.yml` | Metadatos de la lección. |
| `EVIDENCE.md` | Registro de evidencia en inglés. |
| `EVIDENCE-es.md` | Registro de evidencia en español. |

## Criterio de aceptación

La **Autocomprobación (binaria)** de la lección es la lista autoritativa. `meta.yml` la refleja para
las herramientas.
