# construction-09 — Línea base con tipo abstracto de datos

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** independencia de representación · **Prerrequisito:**
> construction-08 · **Tiempo estimado:** 90 minutos

Este directorio contiene el artefacto **después** de `construction-09`. Tiene el mismo
comportamiento observable que `construction-08` para el juego de 8×8, pero `Board` es un tipo
abstracto de datos: la cabecera expone un tipo incompleto y operaciones, mientras que la
representación vive en `board.cpp`.

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
| `code/board.h` | Tipo `Board` opaco y operaciones. |
| `code/board.cpp` | Representación y dimensiones ocultas. |
| `code/random_sequence.h` | Interfaz pública de la secuencia aleatoria. |
| `code/random_sequence.cpp` | Estado oculto del generador. |
| `CMakeLists.txt` | Compilación C++20 con `-Wall -Wextra -Werror`. |
| `run.sh` | Compila cuando hace falta y ejecuta el binario. |

## La frontera del TAD

`board.h` declara solo:

```cpp
struct Board;
Board *board_create();
void board_destroy(Board *board);
int board_row_count(const Board *board);
int board_column_count(const Board *board);
void board_reset(Board *board);
bool board_was_visited(const Board *board, int row, int column);
void board_mark_visited(Board *board, int row, int column);
```

`game.cpp` no puede ver `Board::visited`, las dimensiones del arreglo ni la estrategia de
almacenamiento. Solo puede llamar a las operaciones.

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

Reemplazar el arreglo fijo dentro de `board.cpp` por otra representación, por ejemplo
`std::vector<std::vector<bool>>` o un vector unidimensional con una función de índice. Cambiar solo
`board.cpp`. `game.cpp`, `main.cpp` y `board.h` deben permanecer sin cambios.

## Criterio de aceptación

La persona que aprende puede cambiar la representación del tablero dentro de `board.cpp` y mostrar
que todos los llamadores siguen compilando y las 20 trazas siguen pasando.
