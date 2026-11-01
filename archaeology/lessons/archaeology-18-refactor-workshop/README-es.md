# archaeology-18 — Taller de refactorización

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** todas las clases de fallo · **Prerrequisitos:**
> archaeology-04, archaeology-05, archaeology-08, archaeology-09 · **Tiempo estimado:** 180 minutos

Este es el cierre de Track B. Las demás lecciones terminan con un diagnóstico y un arreglo
estructural propuesto. Esta te obliga a pagarlo: toma un fallo heredado, muévelo a un escalón de
Track A, conserva el comportamiento idéntico y reporta cuánto costó el movimiento.

El taller es un marco, no un guion. Tú eliges el fallo y el escalón; la lección fija el método, la
evidencia y el criterio de aceptación.

## Qué hace la persona que aprende

- Elige un fallo de `archaeology-04`, `archaeology-05`, `archaeology-08` o `archaeology-09`.
- Lo promueve a un banco ejecutable que escribe una traza, y registra esa traza como traza A.
- Elige un escalón objetivo: `construction-08`, `construction-09`, `construction-11`,
  `construction-12` o `construction-13`.
- Escribe la lista de cambios y predice el coste antes de tocar el código.
- Refactoriza en pasos pequeños, manteniendo la traza A idéntica byte a byte después de cada uno.
- Verifica la compilación, las trazas, el determinismo y, cuando aplica, el sanitizador.
- Mide el coste y escribe el veredicto.
- Rellena `WORKSHOP-REPORT-es.md` (o la edición inglesa) y `EVIDENCE-es.md`.

## Las dos pruebas

Una refactorización exige dos pruebas, no una:

| Prueba | Pregunta | Evidencia |
|---|---|---|
| Preservación del comportamiento | ¿Cambió algo de lo que el programa hace? | Traza A idéntica byte a byte antes y después |
| Ganancia de capacidad | ¿El cambio eliminó una clase de fallo? | La prueba de cambio del escalón objetivo, en `lesson-catalog.md` |

Una refactorización que conserva las trazas y no compra nada es un renombrado. Una que compra algo y
cambia la salida es una reescritura. El taller acepta solo la primera forma.

## El taller de un vistazo

```text
README.md
README-es.md
lesson.md
lesson-es.md
meta.yml
EVIDENCE.md
EVIDENCE-es.md
WORKSHOP-REPORT.md
WORKSHOP-REPORT-es.md
```

`WORKSHOP-REPORT-es.md` es la entrega: línea base, objetivo, plan, refactorización, verificación,
tabla de costes y veredicto. `EVIDENCE-es.md` es el registro de mediciones: comandos, salida cruda,
conteos, diferencias y la frontera entre lo verificado y lo inferido. El informe argumenta; la
evidencia demuestra.

## Criterio de aceptación

La **Autocomprobación (binaria)** de la lección es la lista autoritativa. `meta.yml` la refleja para
las herramientas.
