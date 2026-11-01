# archaeology-15 — Análisis estático y advertencias del compilador

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** análisis estático · **Prerrequisito:** archaeology-04 ·
> **Tiempo estimado:** 60 minutos

Esta lección usa las advertencias del compilador como evidencia. La persona compila un banco pequeño
con advertencias, clasifica cada una, la corrige sin cambiar el comportamiento y llega a una
compilación limpia con `-Wall -Wextra -Werror`.

## Qué hace la persona que aprende

- Compila `warnings.cpp` con `-Wall -Wextra` y lee cada advertencia.
- Clasifica cada advertencia: defecto real, inocua pero eliminable, o falso positivo.
- Compara `warnings.cpp` y `fixed.cpp`.
- Verifica que la salida sea idéntica y que la compilación corregida tenga cero advertencias.

## Archivos

| Archivo | Propósito |
|---|---|
| `README.md` | Este archivo. |
| `README-es.md` | Edición en español. |
| `lesson.md` | La lección guiada. |
| `lesson-es.md` | Edición en español de la lección guiada. |
| `code/warnings.cpp` | El banco con advertencias. |
| `code/fixed.cpp` | El banco corregido sin cambiar comportamiento. |
| `run.sh` | Compila ambos y compara la salida. |
| `meta.yml` | Metadatos de la lección. |
| `EVIDENCE.md` | Registro de evidencias en inglés. |
| `EVIDENCE-es.md` | Registro de evidencias en español. |

## Criterio de aceptación

La **Autocomprobación (binaria)** de la lección es la lista autoritativa. `meta.yml` la refleja para
las herramientas.
