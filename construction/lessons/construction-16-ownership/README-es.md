# construction-16 — Línea base con propiedad

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** ciclo de vida de recursos · **Prerrequisito:**
> construction-15 · **Tiempo estimado:** 60 minutos

Este directorio contiene el artefacto **después** de `construction-16`. Tiene el mismo
comportamiento observable que `construction-15` para el juego de 8×8, pero el motor posee el tablero
y los dos caballos mediante `std::unique_ptr`. La copia está eliminada; el movimiento es explícito.

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
| `code/game_engine.h` | `GameEngine` con propiedad mediante `std::unique_ptr`. |
| `code/board.h` | Plantilla de clase `Board<RowCount, ColumnCount>`. |
| `code/move_generator.h` | `MoveCandidate` y el generador genérico de movimientos. |
| `code/knight.h` | `Knight` con un miembro `Position`. |
| `code/knight.cpp` | Delegación a `Position` y nombre de rol. |
| `code/position.h` | Tipo de valor `Position`. |
| `code/position.cpp` | Estado de fila y columna. |
| `code/random_sequence.h` | Interfaz pública de la secuencia aleatoria. |
| `code/random_sequence.cpp` | Estado oculto del generador. |
| `CMakeLists.txt` | Compilación C++20 con `-Wall -Wextra -Werror`. |
| `run.sh` | Compila cuando hace falta y ejecuta el binario. |

## Propiedad

`GameEngine` posee:

```cpp
std::unique_ptr<Board<RowCount, ColumnCount>> board;
std::unique_ptr<Knight> playerKnight;
std::unique_ptr<Knight> machineKnight;
```

Elimina la construcción y asignación por copia, y conserva la construcción y asignación por
movimiento. No hay `new` ni `delete` crudos en el motor.

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

Compilar una sonda con:

```cpp
static_assert(!std::is_copy_constructible_v<GameEngine<8, 8>>);
static_assert(!std::is_copy_assignable_v<GameEngine<8, 8>>);
static_assert(std::is_move_constructible_v<GameEngine<8, 8>>);
```

Ejecutar el motor y confirmar que no aparece `new` ni `delete` crudo en el código. Si hay un
detector de fugas disponible, ejecutarlo; si no, registrar que la propiedad es estructural y que el
detector no está disponible.

## Criterio de aceptación

La persona que aprende puede mostrar que el motor posee sus recursos con `unique_ptr`, que la copia
está eliminada explícitamente, que el movimiento está disponible y que las 20 trazas siguen pasando.
