# archaeology-14 — Pruebas y determinismo

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** pruebas y determinismo · **Prerrequisito:** archaeology-03 ·
> **Tiempo estimado:** 90 minutos

Esta lección convierte un programa no reproducible en uno con control de semilla y una traza de
referencia. La línea base usa un generador portátil de 64 bits; la lección explica por qué el
`rand()` del archivo no puede ser una traza portátil sin una nota de plataforma.

## Qué hace la persona que aprende

- Lee las llamadas `srand(time(NULL))` y `rand()` del archivo.
- Ejecuta la línea base con la misma semilla dos veces.
- Compara ambas ejecuciones con la traza de referencia.
- Ejecuta otra semilla y explica por qué cambia la salida.
- Registra qué necesita una traza portátil y qué no puede prometer una traza heredada capturada.

## La línea base

```text
code/seed_control.cpp
code/tests/golden-seed-1.txt
```

El programa acepta `--seed` y usa el LCG de 64 bits del laboratorio. El archivo de referencia es la
salida exacta para la semilla 1.

## Compilar y ejecutar

```bash
./run.sh
```

Salida esperada:

```text
first=seed=1
move=1 position=2
move=2 position=5
move=3 position=0
winner=player
moves=3
same=yes
golden=yes
```

## Archivos

| Archivo | Propósito |
|---|---|
| `README.md` | Este archivo. |
| `README-es.md` | Edición en español. |
| `lesson.md` | La lección guiada. |
| `lesson-es.md` | Edición en español de la lección guiada. |
| `code/seed_control.cpp` | Línea base con control de semilla. |
| `code/tests/golden-seed-1.txt` | Salida de referencia para la semilla 1. |
| `run.sh` | Ejecuta dos veces y compara con la traza de referencia. |
| `meta.yml` | Metadatos de la lección. |
| `EVIDENCE.md` | Registro de evidencias en inglés. |
| `EVIDENCE-es.md` | Registro de evidencias en español. |

## Criterio de aceptación

La **Autocomprobación (binaria)** de la lección es la lista autoritativa. `meta.yml` la refleja para
las herramientas.
