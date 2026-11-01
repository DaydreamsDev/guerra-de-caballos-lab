# archaeology-07 — Fuga de representación y arreglos paralelos

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** fuga de representación y registros ·
> **Prerrequisito:** archaeology-05 · **Tiempo estimado:** 60 minutos

Esta lección reemplaza las variables sueltas de los caballos y el arreglo paralelo del tablero
por un registro `Knight` y un valor `Board`. Los programas de antes y después ejecutan el mismo
escenario y deben imprimir el mismo resumen.

## Qué hace la persona que aprende

- Lee la versión con arreglos paralelos y la versión con registro.
- Ejecuta ambas y compara la salida.
- Nombra cada llamador que tuvo que cambiar.
- Registra el coste: archivos, líneas, conceptos nuevos y resultado de la comparación.

## La línea base

```text
code/parallel_arrays.cpp
code/record.cpp
```

`parallel_arrays.cpp` usa `playerRow`, `playerColumn`, `machineRow`, `machineColumn` y un
`std::array<bool, 64>`.

`record.cpp` usa `Knight`, una clase `Board` con el arreglo visitado privado y `visited_count()`.

Ambos imprimen:

```text
player=2,3 machine=3,4 visited=3
```

## Ejecutar la comparación

```bash
./run.sh
```

Salida esperada:

```text
parallel=player=2,3 machine=3,4 visited=3
record=player=2,3 machine=3,4 visited=3
match=yes
```

## Archivos

| Archivo | Propósito |
|---|---|
| `README.md` | Este archivo. |
| `README-es.md` | Edición en español. |
| `lesson.md` | La lección guiada. |
| `lesson-es.md` | Edición en español de la lección guiada. |
| `code/parallel_arrays.cpp` | La versión con arreglos paralelos. |
| `code/record.cpp` | La versión con registro y tablero. |
| `run.sh` | Compila ambas y compara la salida. |
| `meta.yml` | Metadatos de la lección. |
| `EVIDENCE.md` | Registro de evidencias en inglés. |
| `EVIDENCE-es.md` | Registro de evidencias en español. |

## Criterio de aceptación

La **Autocomprobación (binaria)** de la lección es la lista autoritativa. `meta.yml` la refleja para
las herramientas.
