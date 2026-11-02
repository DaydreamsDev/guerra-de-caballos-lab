# construction-22 — Línea base de contratos de error

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** recuperación ·
> **Prerrequisito:** construction-09, construction-21 · **Tiempo estimado:** 75 minutos

Este directorio contiene el artefacto **después** de `construction-22`. Conserva el núcleo de
concurrencia de `construction-21` y convierte el camino del movimiento inválido en un valor
explícito. El motor nunca llama a `exit`, no lanza excepciones y no mata el proceso; devuelve un
`MoveResult`, y quien llama decide qué hacer.

## Qué implementa el programa

- El generador lineal congruencial de 64 bits de `common/spec/game-rules.md`.
- El tablero de 8×8, los dos caballos, las posiciones iniciales y las tiradas iniciales.
- La prioridad de movimientos, los movimientos legales e ilegales, las casillas visitadas, la
  captura, la trampa y el empate a los 200 movimientos.
- El formato de transcripción de `common/spec/transcript-format.md`, byte a byte.

El programa de consola acepta `--seed INTEGER --name TEXT` en cualquier orden y escribe la
transcripción en la salida estándar.

## Archivos

| Archivo | Propósito |
|---|---|
| `code/main.cpp` | Analiza la línea de comandos y llama a `run_game`. |
| `code/game.h` | Interfaz pública del juego. |
| `code/game.cpp` | Conecta el adaptador de consola con el núcleo. |
| `code/game_engine.h` | Motor central; `tryMove` devuelve `MoveResult`. |
| `code/move_result.h` | `MoveOutcome` y `MoveResult`; el contrato de error. |
| `code/error_probe.cpp` | Llama a `evaluate_move` con movimientos inválido, captura y legal. |
| `code/random_sequence.h` | Clase `RandomSequence`; un estado por motor. |
| `code/random_sequence.cpp` | El LCG de 64 bits sin estado global. |
| `code/transcript_port.h` | El puerto de `construction-20`. |
| `code/console_transcript.h` | Declaración del adaptador de consola. |
| `code/console_transcript.cpp` | Adaptador de consola; el único que imprime la transcripción. |
| `code/recording_transcript.h` | Adaptador de prueba; registra ganador y movimientos. |
| `code/adapter_probe.cpp` | Ejecuta una partida con el adaptador de registro. |
| `code/parallel_runner.cpp` | Modos secuencial, carrera, mutex y fusión. |
| `code/board.h` | Estado del tablero e invariante. |
| `code/move_generator.h` | Generación pura de candidatos de movimiento. |
| `code/knight.h` | `Knight` con un miembro `Position`. |
| `code/knight.cpp` | Delegación a `Position` y nombre de rol. |
| `code/position.h` | Tipo de valor `Position`. |
| `code/position.cpp` | Estado de fila y columna. |
| `CMakeLists.txt` | Compilación C++20 de la línea base, las sondas y el ejecutor. |
| `run.sh` | Compila cuando hace falta y ejecuta la línea base de consola. |

## El contrato

`MoveOutcome` tiene cuatro valores:

- `outsideBoard`
- `visited`
- `capture`
- `legal`

`MoveResult` lleva el resultado y las coordenadas de destino:

```cpp
struct MoveResult
{
    MoveOutcome outcome;
    int destinationRow;
    int destinationColumn;
};
```

`GameEngine::tryMove` devuelve `MoveResult` en lugar de escribir por dos parámetros de salida. La
función pública `evaluate_move` expone el mismo contrato sin ejecutar una partida. `playTurn` decide
según `moveResult.outcome` y continúa, informa o acepta el movimiento. Ninguna rama llama a `exit`.

## Compilar y ejecutar

```bash
./run.sh --seed 22 --name Learner
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

## Ejecutar la sonda de errores

```bash
c++ -std=c++20 -O2 -Icode -o error-probe \
    code/error_probe.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
./error-probe
```

La sonda imprime:

```text
outside=0 capture=2 legal=3
```

Los valores son los del enumerado. La sonda termina con estado 0; un movimiento inválido es un
valor, no un fallo del proceso.

## Prueba de cambio

Llamar a `evaluate_move` con un movimiento fuera del tablero, una captura y un movimiento legal.
Registrar el resultado y el destino. Después hacer que quien llama decida: continuar la partida,
informar el intento o detenerse. Mostrar que ninguna ruta llama a `exit` y que las 20 trazas siguen
pasando.

## Criterio de aceptación

La persona que aprende puede gestionar un movimiento inválido mediante un valor devuelto, dejar que
quien llama elija la recuperación, y mostrar que el proceso continúa y que la transcripción no
cambia.
