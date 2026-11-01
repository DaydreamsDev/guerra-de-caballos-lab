# archaeology-08 — Frontera de módulo ausente

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** frontera de módulo · **Prerrequisito:** archaeology-05 ·
> **Tiempo estimado:** 90 minutos

Esta lección convierte un arreglo de tablero transparente en un módulo con cabecera e
implementación. La cabecera expone operaciones; la representación es privada en `board.cpp`.

## Qué hace la persona que aprende

- Lee el acceso al tablero en una sola unidad de traducción del archivo.
- Lee la separación `board.h` / `board.cpp` / `main.cpp`.
- Ejecuta el módulo y registra la superficie pública.
- Intenta acceder al arreglo visitado desde `main.cpp`; el compilador lo rechaza.
- Registra la frontera: qué saben los llamadores, qué no saben y qué puede cambiar sin tocarlos.

## El módulo

```text
code/board.h
code/board.cpp
code/main.cpp
```

`Board` expone `reset`, `mark`, `is_visited` y `visited_count`. El arreglo visitado y la aritmética
de índices son privados.

## Compilar y ejecutar

```bash
./run.sh
```

Salida esperada:

```text
visited=2 first=1 second=1
```

## Archivos

| Archivo | Propósito |
|---|---|
| `README.md` | Este archivo. |
| `README-es.md` | Edición en español. |
| `lesson.md` | La lección guiada. |
| `lesson-es.md` | Edición en español de la lección guiada. |
| `code/board.h` | Interfaz pública. |
| `code/board.cpp` | Representación oculta e implementación. |
| `code/main.cpp` | Llamador que usa solo la interfaz pública. |
| `run.sh` | Compila y ejecuta el módulo. |
| `meta.yml` | Metadatos de la lección. |
| `EVIDENCE.md` | Registro de evidencias en inglés. |
| `EVIDENCE-es.md` | Registro de evidencias en español. |

## Criterio de aceptación

La **Autocomprobación (binaria)** de la lección es la lista autoritativa. `meta.yml` la refleja para
las herramientas.
