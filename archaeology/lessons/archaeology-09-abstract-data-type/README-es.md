# archaeology-09 — Tipo abstracto de datos ausente

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** tipo abstracto de datos · **Prerrequisito:** archaeology-08 ·
> **Tiempo estimado:** 90 minutos

Esta lección convierte el tablero en un tipo opaco. `main.cpp` ve solo operaciones; la
representación vive en una de dos implementaciones. El mismo llamador compila y ejecuta contra
ambas.

## Qué hace la persona que aprende

- Lee el `board.h` opaco.
- Ejecuta el llamador contra `board_array.cpp`.
- Ejecuta el mismo llamador contra `board_bitmask.cpp`.
- Compara la salida y confirma que `main.cpp` no cambió.
- Registra qué oculta el tipo y qué cuesta ahora un cambio de representación.

## El módulo

```text
code/board.h
code/board_array.cpp
code/board_bitmask.cpp
code/main.cpp
```

`board.h` declara `struct Board;` y las operaciones. `board_array.cpp` guarda un `bool[64]`.
`board_bitmask.cpp` guarda un `std::uint64_t`. `main.cpp` incluye solo `board.h`.

## Compilar y ejecutar

```bash
./run.sh
```

Salida esperada:

```text
array=visited=2 first=1 second=1
bitmask=visited=2 first=1 second=1
match=yes
```

## Archivos

| Archivo | Propósito |
|---|---|
| `README.md` | Este archivo. |
| `README-es.md` | Edición en español. |
| `lesson.md` | La lección guiada. |
| `lesson-es.md` | Edición en español de la lección guiada. |
| `code/board.h` | Tipo opaco y operaciones. |
| `code/board_array.cpp` | Representación con arreglo. |
| `code/board_bitmask.cpp` | Representación con máscara de bits. |
| `code/main.cpp` | Llamador que usa solo las operaciones. |
| `run.sh` | Compila ambas representaciones y compara la salida. |
| `meta.yml` | Metadatos de la lección. |
| `EVIDENCE.md` | Registro de evidencias en inglés. |
| `EVIDENCE-es.md` | Registro de evidencias en español. |

## Criterio de aceptación

La **Autocomprobación (binaria)** de la lección es la lista autoritativa. `meta.yml` la refleja para
las herramientas.
