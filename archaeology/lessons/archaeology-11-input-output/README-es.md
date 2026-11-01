# archaeology-11 — Entrada, salida y capacidad de prueba

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** separación de entrada/salida y capacidad de prueba ·
> **Prerrequisito:** archaeology-09 · **Tiempo estimado:** 60 minutos

Esta lección separa un núcleo de juego de la consola. El núcleo devuelve un `GameResult`; un
adaptador de consola lo imprime; un adaptador de prueba llama al núcleo sin consola.

## Qué hace la persona que aprende

- Lee la interfaz del núcleo y los dos adaptadores.
- Ejecuta el camino de consola y cuenta las líneas.
- Ejecuta el camino de prueba y comprueba el ganador y el número de movimientos.
- Registra qué puede hacer el núcleo sin consola y qué lo sigue acoplando a una transcripción.

## El núcleo

```text
code/core.h
code/core.cpp
code/console.h
code/console.cpp
code/test.cpp
code/main.cpp
```

`run_game` devuelve `GameResult{lines, winner, moveCount}`. No incluye `<cstdio>` ni llama a ninguna
función de impresión. `console.cpp` es el único archivo que imprime la transcripción; `test.cpp`
imprime su propio informe de prueba. `test.cpp` llama a `run_game` directamente.

Este es un banco pequeño, no el juego completo de caballos. Aísla la frontera de entrada/salida.

## Compilar y ejecutar

```bash
./run.sh
```

Salida esperada:

```text
console_lines=8
test_output=lines=8 winner=player moves=3
test=ok
match=yes
```

## Archivos

| Archivo | Propósito |
|---|---|
| `README.md` | Este archivo. |
| `README-es.md` | Edición en español. |
| `lesson.md` | La lección guiada. |
| `lesson-es.md` | Edición en español de la lección guiada. |
| `code/core.h` | Tipo de resultado y punto de entrada del núcleo. |
| `code/core.cpp` | Lógica del núcleo; sin consola. |
| `code/console.h` | Declaración del adaptador de consola. |
| `code/console.cpp` | Adaptador de consola; el único que imprime. |
| `code/test.cpp` | Adaptador de prueba. |
| `code/main.cpp` | Punto de entrada de consola. |
| `run.sh` | Compila y ejecuta ambos adaptadores. |
| `meta.yml` | Metadatos de la lección. |
| `EVIDENCE.md` | Registro de evidencias en inglés. |
| `EVIDENCE-es.md` | Registro de evidencias en español. |

## Criterio de aceptación

La **Autocomprobación (binaria)** de la lección es la lista autoritativa. `meta.yml` la refleja para
las herramientas.
