# construction-18 — Línea base con estilo funcional

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** pureza e inmutabilidad · **Prerrequisito:** construction-17 ·
> **Tiempo estimado:** 85 minutos

Este directorio contiene el artefacto **después** de `construction-18`. Tiene el mismo
comportamiento observable que `construction-17`, pero el motor guarda un historial de instantáneas
inmutables del tablero. Deshacer y rehacer son copias de valor, no una repetición de las reglas.

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
| `code/game_engine.h` | Motor con historial, deshacer y rehacer. |
| `code/board.h` | `Board` con semántica de valor. |
| `code/move_generator.h` | Generación pura de candidatos de movimiento. |
| `code/knight.h` | `Knight` con un miembro `Position`. |
| `code/knight.cpp` | Delegación a `Position` y nombre de rol. |
| `code/position.h` | Tipo de valor `Position`. |
| `code/position.cpp` | Estado de fila y columna. |
| `code/random_sequence.h` | Interfaz pública de la secuencia aleatoria. |
| `code/random_sequence.cpp` | Estado oculto del generador. |
| `CMakeLists.txt` | Compilación C++20 con `-Wall -Wextra -Werror`. |
| `run.sh` | Compila cuando hace falta y ejecuta el binario. |

## La frontera funcional

- `generate_move_candidates` es una función pura de una pieza.
- `Board` es un tipo de valor; una instantánea es una copia.
- `GameEngine` guarda `std::vector<Board<RowCount, ColumnCount>> history`.
- `undo_last_move` asigna la instantánea anterior.
- `redo_last_move` asigna la instantánea siguiente.
- Un movimiento nuevo después de deshacer borra la rama de rehacer.

El bucle del juego sigue mutando el tablero actual; la frontera del historial es lo que se vuelve
inmutable.

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

Ejecutar una partida, registrar `history_size()`, llamar a `undo_last_move()` y después a
`redo_last_move()`. Ambos devuelven verdadero y el tablero vuelve al mismo estado. Comparar el coste
con implementar deshacer repitiendo las reglas de movimiento.

## Criterio de aceptación

La persona que aprende puede deshacer y rehacer un movimiento mediante el historial del tablero y
mostrar que las 20 trazas siguen pasando.
