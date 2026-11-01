# archaeology-10 — Polimorfismo ausente

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** polimorfismo · **Prerrequisito:** archaeology-09 ·
> **Tiempo estimado:** 90 minutos

Esta lección reemplaza un switch sobre el tipo de pieza por despacho virtual. Una interfaz `Piece`
tiene dos implementaciones; una línea base con switch calcula los mismos deltas. La prueba compara
ambas para 400 casos.

## Qué hace la persona que aprende

- Lee la interfaz `Piece` y las dos implementaciones.
- Ejecuta la comparación entre el switch y el camino de despacho.
- Confirma 400 comparaciones y cero discrepancias.
- Añade una tercera pieza en una copia de trabajo y cuenta las ediciones en cada camino.
- Registra qué compró el despacho y qué costó.

## La línea base

```text
code/polymorphism.cpp
```

`Knight` y `MirrorKnight` implementan `Piece::move`. `delta_with_switch` conserva el camino con
switch para la comparación. `main` compara ambos caminos para 2 piezas × 8 movimientos × 25
posiciones = 400 casos.

## Compilar y ejecutar

```bash
./run.sh
```

Salida esperada:

```text
comparisons=400 mismatches=0
```

## Archivos

| Archivo | Propósito |
|---|---|
| `README.md` | Este archivo. |
| `README-es.md` | Edición en español. |
| `lesson.md` | La lección guiada. |
| `lesson-es.md` | Edición en español de la lección guiada. |
| `code/polymorphism.cpp` | Interfaz, implementaciones, switch y comparación. |
| `run.sh` | Compila y ejecuta la línea base. |
| `meta.yml` | Metadatos de la lección. |
| `EVIDENCE.md` | Registro de evidencias en inglés. |
| `EVIDENCE-es.md` | Registro de evidencias en español. |

## Criterio de aceptación

La **Autocomprobación (binaria)** de la lección es la lista autoritativa. `meta.yml` la refleja para
las herramientas.
