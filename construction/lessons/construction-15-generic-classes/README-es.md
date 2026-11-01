# construction-15 — Línea base con clases genéricas y conceptos

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** restricciones · **Prerrequisito:** construction-14 ·
> **Tiempo estimado:** 81 minutos

Este directorio contiene el artefacto **después** de `construction-15`. Tiene el mismo
comportamiento observable que `construction-14` para el juego de 8×8, pero el tablero y el motor del
juego son plantillas de clase, y un concepto de C++20 restringe el tipo de pieza.

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
| `code/game_engine.h` | Concepto `PositionedPiece` y `GameEngine<RowCount, ColumnCount>`. |
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

## La frontera genérica

`Board<RowCount, ColumnCount>` posee un almacenamiento máximo fijo y expone las dimensiones activas.
`GameEngine<RowCount, ColumnCount>` posee un tablero y dos caballos y ejecuta el juego en esas
dimensiones.

`PositionedPiece` exige:

```cpp
{ constPiece.row() } -> std::convertible_to<int>;
{ constPiece.column() } -> std::convertible_to<int>;
{ piece.set_position(1, 1) } -> std::same_as<void>;
{ constPiece.role_name() } -> std::convertible_to<const char *>;
```

`static_assert(PositionedPiece<Knight>)` hace que la restricción forme parte de la compilación.

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

Compilar y ejecutar `GameEngine<8, 8>` y `GameEngine<10, 10>` desde la misma plantilla. El motor de
8×8 debe reproducir las 20 trazas. El motor de 10×10 debe compilar y ejecutarse; su transcripción
cambia porque cambió el tablero.

## Criterio de aceptación

La persona que aprende puede instanciar el mismo motor para 8×8 y 10×10 y explicar qué parte es
genérica, qué parte está restringida y qué comportamiento cambia con las dimensiones.
