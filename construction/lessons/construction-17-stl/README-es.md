# construction-17 — Línea base con STL y algoritmos

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** abstracción de iteración
> **Prerrequisito:** construction-05 · **Tiempo estimado:** 60 minutos

Este directorio contiene el artefacto **después** de `construction-17`. Tiene el mismo
comportamiento observable que `construction-16`, pero la generación de movimientos, el orden de
movimientos y el reinicio del tablero usan algoritmos de rangos de C++20 en lugar de bucles escritos
a mano.

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
| `code/game.cpp` | Instancia `GameEngine<8, 8>`. |
| `code/game_engine.h` | Motor con orden de movimientos basado en rangos. |
| `code/board.h` | `Board` con `std::array` y `std::ranges::fill`. |
| `code/move_generator.h` | `MoveCandidate` y generación basada en rangos. |
| `code/knight.h` | `Knight` con un miembro `Position`. |
| `code/knight.cpp` | Delegación a `Position` y nombre de rol. |
| `code/position.h` | Tipo de valor `Position`. |
| `code/position.cpp` | Estado de fila y columna. |
| `code/random_sequence.h` | Interfaz pública de la secuencia aleatoria. |
| `code/random_sequence.cpp` | Estado oculto del generador. |
| `CMakeLists.txt` | Compilación C++20 con `-Wall -Wextra -Werror`. |
| `run.sh` | Compila cuando hace falta y ejecuta el binario. |

## Los algoritmos

- `Board::reset` usa `std::ranges::fill`.
- `generate_move_candidates` usa `std::ranges::transform` sobre `std::views::iota`.
- `GameEngine::buildMoveOrder` usa `std::ranges::transform` sobre `std::views::iota`.
- El bucle de intentos permanece: tiene salida temprana y un algoritmo ocultaría el contrato del
  turno.

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

Reemplazar un recorrido escrito a mano por un algoritmo de rangos sin cambiar la transcripción. La
generación de movimientos y el reinicio del tablero son los ejemplos seguros; el bucle de intentos
es el contraejemplo.

## Criterio de aceptación

La persona que aprende puede eliminar un bucle de recorrido, usar un algoritmo y mostrar que la
transcripción no cambia.
