# construction-20 — Línea base hexagonal

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** puertos y adaptadores ·
> **Prerrequisito:** construction-08, construction-17 · **Tiempo estimado:** 92 minutos

Este directorio contiene el artefacto **después** de `construction-20`. Tiene el mismo
comportamiento observable que `construction-17`, pero el núcleo del juego ya no escribe en la
consola. El núcleo emite eventos a través de `TranscriptPort`, y dos adaptadores implementan ese
puerto: un adaptador de consola para la transcripción real y un adaptador de registro para pruebas.

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
| `code/game.cpp` | Conecta el adaptador de consola con el núcleo. |
| `code/game_engine.h` | Motor central; depende de `TranscriptPort`, no de `stdio`. |
| `code/transcript_port.h` | El puerto: cabecera, turno, intento, movimiento, trampa y ganador. |
| `code/console_transcript.h` | Declaración del adaptador de consola. |
| `code/console_transcript.cpp` | Adaptador de consola; el único archivo que imprime la transcripción. |
| `code/recording_transcript.h` | Adaptador de prueba; registra conteos y el ganador. |
| `code/adapter_probe.cpp` | Ejecuta una partida con el adaptador de registro e imprime los conteos. |
| `code/board.h` | Estado del tablero e invariante. |
| `code/move_generator.h` | Generación pura de candidatos de movimiento. |
| `code/knight.h` | `Knight` con un miembro `Position`. |
| `code/knight.cpp` | Delegación a `Position` y nombre de rol. |
| `code/position.h` | Tipo de valor `Position`. |
| `code/position.cpp` | Estado de fila y columna. |
| `code/random_sequence.h` | Interfaz pública de la secuencia aleatoria. |
| `code/random_sequence.cpp` | Estado oculto del generador. |
| `CMakeLists.txt` | Compilación C++20 de la línea base y la sonda de adaptadores. |
| `run.sh` | Compila cuando hace falta y ejecuta la línea base de consola. |

## El puerto y los adaptadores

El puerto tiene un método por evento de la transcripción:

- `game_header`
- `turn_header`
- `illegal_attempt`
- `capture_attempt`
- `legal_attempt`
- `move`
- `no_legal_move`
- `winner`

El núcleo llama solo a esos métodos. `ConsoleTranscript` los imprime en el formato exacto de la
transcripción. `RecordingTranscript` incrementa contadores y guarda el ganador. El núcleo no incluye
`<cstdio>` y no contiene `printf`.

## Compilar y ejecutar

```bash
./run.sh --seed 20 --name Learner
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

## Ejecutar el adaptador de prueba

```bash
c++ -std=c++20 -O2 -Icode -o adapter-probe \
    code/adapter_probe.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
./adapter-probe
```

La sonda ejecuta la semilla 1 e imprime los conteos de eventos. El núcleo es idéntico a la ejecución
de consola; solo cambia el adaptador.

## Prueba de cambio

Ejecutar la misma semilla con el adaptador de consola y con el adaptador de registro. La
transcripción de consola debe ser byte a byte igual a la traza de referencia. El adaptador de
registro debe informar el mismo número de turnos, intentos, movimientos y el mismo ganador. Después,
añadir un evento nuevo al puerto e implementarlo en ambos adaptadores sin tocar el núcleo.

## Criterio de aceptación

La persona que aprende puede ejecutar el mismo núcleo con dos adaptadores, mostrar que la
transcripción de consola no cambia y añadir un evento al puerto sin editar el bucle del juego.
