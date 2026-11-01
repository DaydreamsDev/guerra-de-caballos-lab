# construction-08 — Línea base modular

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** ocultamiento de información · **Prerrequisito:**
> construction-03 · **Tiempo estimado:** 92 minutos

Este directorio contiene el artefacto **después** de `construction-08`. Tiene el mismo
comportamiento observable que las líneas base anteriores para el juego de 8×8, pero el código está
dividido en módulos con cabeceras y unidades de traducción separadas.

## Módulos

| Módulo | Interfaz | Implementación oculta |
|---|---|---|
| `random_sequence` | semilla, límite superior aleatorio, entero aleatorio | multiplicador, incremento, estado |
| `board` | número de filas, número de columnas, reinicio, consulta de visitada, marca de visitada | representación de casilla, dimensiones activas y máximas |
| `game` | `run_game(seed, playerName)` | caballos, orden de movimientos, bucle de turnos, transcripción |
| `main` | análisis de argumentos | — |

`main.cpp` incluye solo `game.h`. `game.cpp` incluye `board.h` y `random_sequence.h`. Un llamador de
la interfaz del tablero no puede ver `BoardCell`.

## Qué implementa el programa

- El generador lineal congruencial de 64 bits de `common/spec/game-rules.md`.
- El tablero de 8×8, los dos caballos, las posiciones iniciales y las tiradas iniciales.
- La prioridad de movimientos, los movimientos legales e ilegales, las casillas visitadas, la
  captura, la trampa y el empate a los 200 movimientos.
- El formato de transcripción de `common/spec/transcript-format.md`, byte a byte.

El programa acepta `--seed INTEGER --name TEXT` en cualquier orden y escribe la transcripción en la
salida estándar.

## Archivos

| Archivo | Propósito |
|---|---|
| `code/main.cpp` | Analiza la línea de comandos y llama a `run_game`. |
| `code/game.h` | Interfaz pública del juego. |
| `code/game.cpp` | Reglas, bucle de turnos y transcripción. |
| `code/board.h` | Interfaz pública del tablero. |
| `code/board.cpp` | Representación y dimensiones ocultas del tablero. |
| `code/random_sequence.h` | Interfaz pública de la secuencia aleatoria. |
| `code/random_sequence.cpp` | Estado oculto del generador. |
| `CMakeLists.txt` | Compilación C++20 con `-Wall -Wextra -Werror`. |
| `run.sh` | Compila cuando hace falta y ejecuta el binario. |

## Compilar y ejecutar

```bash
./run.sh --seed 17 --name Learner
```

`run.sh` usa CMake cuando está instalado. Cuando CMake no está disponible, compila todas las
unidades de traducción directamente con el compilador indicado por `CXX`, o `c++` por defecto.

## Verificar las trazas de referencia

Desde este directorio:

```bash
for trace in ../../../common/traces/seed-*.txt; do
    traceName="$(basename "${trace}" .txt)"
    seedText="${traceName#seed-}"
    seedNumber="$((10#${seedText}))"
    ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

Las 20 trazas, de `seed-0001.txt` a `seed-0020.txt`, coinciden byte a byte.

## Prueba de cambio

Cambiar las dimensiones activas del tablero en `board.cpp` de 8×8 a 10×10. Contar qué archivos deben
cambiar y cuáles deben recompilarse. El comportamiento cambia, así que las trazas de referencia de
8×8 ya no aplican; la lección trata sobre la superficie de cambio, no sobre preservar trazas.

## Criterio de aceptación

La persona que aprende puede cambiar el tamaño del tablero en el módulo del tablero y mostrar que
`game.cpp` y `main.cpp` no requieren ediciones.
