# archaeology-06 — Duplicación y el switch de ocho casos

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** duplicación y extracción · **Prerrequisito:**
> archaeology-04 · **Tiempo estimado:** 60 minutos

Esta lección aísla el switch de movimiento de ocho casos del código archivado y reemplaza los
cuerpos repetidos por una tabla. El comportamiento se compara antes y después; la prueba debe
reportar cero discrepancias.

## Qué hace la persona que aprende

- Lee la estructura original de ocho casos y cuenta los deltas repetidos.
- Ejecuta la línea base, que conserva `apply_switch` y `apply_table`.
- Confirma que las 200 comparaciones de movimiento/posición coinciden.
- Elimina el switch en una copia de trabajo y conserva la tabla; la prueba sigue pasando.
- Registra el coste de la extracción: líneas, casos, entradas de tabla y resultado de la prueba.

## La línea base

```text
code/movement.cpp
```

La línea base tiene:

- `apply_switch` — ocho cuerpos `case` explícitos.
- `apply_table` — un `std::array<Delta, 8>` y una búsqueda indexada.
- `main` — compara ambas funciones para 8 movimientos × 25 posiciones = 200 comparaciones.

El switch original también contiene comprobaciones de límites, casillas visitadas y captura. Esta
línea base aísla el cálculo del delta duplicado; no pretende ser la regla completa.

## Compilar y ejecutar

Desde este directorio:

```bash
./run.sh
```

Salida esperada:

```text
comparisons=200 mismatches=0
```

## Archivos

| Archivo | Propósito |
|---|---|
| `README.md` | Este archivo. |
| `README-es.md` | Edición en español. |
| `lesson.md` | La lección guiada. |
| `lesson-es.md` | Edición en español de la lección guiada. |
| `code/movement.cpp` | Implementaciones con switch y tabla, más la prueba de comparación. |
| `run.sh` | Compila y ejecuta la línea base. |
| `meta.yml` | Metadatos de la lección. |
| `EVIDENCE.md` | Registro de evidencias en inglés. |
| `EVIDENCE-es.md` | Registro de evidencias en español. |

## Criterio de aceptación

La **Autocomprobación (binaria)** de la lección es la lista autoritativa. `meta.yml` la refleja para
las herramientas.
