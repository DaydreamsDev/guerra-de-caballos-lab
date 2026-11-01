# construction-21 — Línea base de concurrencia

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** estado compartido frente a mensajes ·
> **Prerrequisito:** construction-12 · **Tiempo estimado:** 115 minutos

Este directorio contiene el artefacto **después** de `construction-21`. Conserva el núcleo
hexagonal de `construction-20` y añade dos cosas: la secuencia aleatoria pasa a ser estado de cada
motor, y un ejecutor paralelo compara resultados compartidos con un mutex y con resultados por hilo.

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
| `code/game_engine.h` | Motor central con un `RandomSequence` por motor. |
| `code/random_sequence.h` | Clase `RandomSequence`; un estado por motor. |
| `code/random_sequence.cpp` | El LCG de 64 bits sin estado global. |
| `code/transcript_port.h` | El puerto de `construction-20`. |
| `code/console_transcript.h` | Declaración del adaptador de consola. |
| `code/console_transcript.cpp` | Adaptador de consola; el único que imprime la transcripción. |
| `code/recording_transcript.h` | Adaptador de prueba; registra ganador y movimientos. |
| `code/adapter_probe.cpp` | Ejecuta una partida con el adaptador de registro. |
| `code/parallel_runner.cpp` | Modos secuencial, carrera, mutex y fusión para 16,000 partidas. |
| `code/board.h` | Estado del tablero e invariante. |
| `code/move_generator.h` | Generación pura de candidatos de movimiento. |
| `code/knight.h` | `Knight` con un miembro `Position`. |
| `code/knight.cpp` | Delegación a `Position` y nombre de rol. |
| `code/position.h` | Tipo de valor `Position`. |
| `code/position.cpp` | Estado de fila y columna. |
| `CMakeLists.txt` | Compilación C++20 de la línea base, la sonda y el ejecutor. |
| `run.sh` | Compila cuando hace falta y ejecuta la línea base de consola. |

## El cambio de estado compartido

`construction-20` usaba un `randomState` global dentro de `random_sequence.cpp`. Dos hilos que
ejecutaran dos partidas escribirían la misma variable. `construction-21` mueve el estado a un objeto
`RandomSequence` que posee cada `GameEngine`. La secuencia del LCG no cambia, así que la
transcripción de consola tampoco; la propiedad es lo que hace deterministas las partidas en
paralelo.

El ejecutor paralelo tiene cuatro modos:

- `sequential` — una partida a la vez; la suma de verificación de referencia.
- `race` — un `nextIndex` compartido y un vector de resultados preasignado, sin sincronización.
- `mutex` — el mismo vector compartido protegido por `std::mutex`.
- `merge` — un vector de resultados por hilo, fusionado después del join; sin estado compartido
  mutable en los resultados.

## Compilar y ejecutar

```bash
./run.sh --seed 21 --name Learner
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

## Ejecutar el experimento de concurrencia

```bash
c++ -std=c++20 -O2 -pthread -Icode -o parallel-runner \
    code/parallel_runner.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
./parallel-runner --mode sequential
./parallel-runner --mode race
./parallel-runner --mode mutex
./parallel-runner --mode merge
```

En esta máquina, 16,000 partidas producen:

| Modo | Escritas | Suma de verificación |
|---|---:|---:|
| sequential | 16,000 | 2826026307342797653 |
| race | 14,719–14,821 | varía |
| mutex | 16,000 | 2826026307342797653 |
| merge | 16,000 | 2826026307342797653 |

El modo carrera pierde escrituras y su suma cambia entre ejecuciones. Los modos mutex y fusión
coinciden con la suma secuencial. La carrera es comportamiento indefinido intencional; ejecutarla
más de una vez para ver cómo se mueve el resultado.

## Prueba de cambio

Ejecutar `sequential`, después `race`, y después `mutex` y `merge`. Registrar el número de
escrituras y la suma de verificación. Eliminar la carrera protegiendo el índice compartido o dando a
cada hilo su propio vector de resultados. Mostrar que la suma corregida coincide con la secuencial y
que las 20 trazas de referencia siguen pasando.

## Criterio de aceptación

La persona que aprende puede reproducir una carrera de escrituras perdidas, eliminarla con
protección del estado compartido o con resultados por hilo, y mostrar que la ejecución paralela es
determinista y que la transcripción no cambia.
