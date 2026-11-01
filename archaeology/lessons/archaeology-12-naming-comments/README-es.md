# archaeology-12 — Nombres y comentarios

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** nombres y comentarios · **Prerrequisito:** archaeology-04 ·
> **Tiempo estimado:** 60 minutos

Esta lección reescribe nombres y comentarios con motivos. El programa de antes usa una abreviatura y
parámetros de una letra; el de después usa nombres descriptivos y ningún comentario que repita el
código.

## Qué hace la persona que aprende

- Lee los programas de antes y después.
- Ejecuta ambos y compara la salida.
- Completa la tabla de renombrado con nombre anterior, nombre nuevo y motivo.
- Reescribe o elimina cada comentario y registra por qué.

## La línea base

```text
code/before.cpp
code/after.cpp
```

`before.cpp` usa `calc`, `f`, `c` y un comentario que narra lo obvio. `after.cpp` usa
`board_cell_index`, `row`, `column` y ningún comentario. Ambos imprimen `index=19`.

## Compilar y ejecutar

```bash
./run.sh
```

Salida esperada:

```text
before=index=19
after=index=19
match=yes
```

## Archivos

| Archivo | Propósito |
|---|---|
| `README.md` | Este archivo. |
| `README-es.md` | Edición en español. |
| `lesson.md` | La lección guiada. |
| `lesson-es.md` | Edición en español de la lección guiada. |
| `RENAME-TEMPLATE.md` | Tabla de renombrado y comentarios. |
| `RENAME-TEMPLATE-es.md` | Edición en español de la tabla. |
| `code/before.cpp` | El programa de antes. |
| `code/after.cpp` | El programa de después. |
| `run.sh` | Compila ambos y compara la salida. |
| `meta.yml` | Metadatos de la lección. |
| `EVIDENCE.md` | Registro de evidencias en inglés. |
| `EVIDENCE-es.md` | Registro de evidencias en español. |

## Criterio de aceptación

La **Autocomprobación (binaria)** de la lección es la lista autoritativa. `meta.yml` la refleja para
las herramientas.
