# construction-19 — Línea base de layout

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** layout (disposición) ·
> **Prerrequisito:** construction-05, construction-17 · **Tiempo estimado:** 85 minutos

Este directorio contiene el artefacto **después** de `construction-19`. Tiene el mismo
comportamiento observable que `construction-17`, y conserva el mismo modelo de propiedad y el mismo
estilo de métodos. Solo cambia la representación del tablero: las casillas visitadas se empaquetan
por bits, y una tabla de ocupantes convierte la captura en una consulta.

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
| `code/game_engine.h` | Motor con tablero y caballos en `unique_ptr`; propiedad sin cambios. |
| `code/board.h` | `Board<8, 8>` con palabras de visitadas empaquetadas y tabla de ocupantes. |
| `code/move_generator.h` | Generación pura de candidatos de movimiento. |
| `code/knight.h` | `Knight` con un miembro `Position`. |
| `code/knight.cpp` | Delegación a `Position` y nombre de rol. |
| `code/position.h` | Tipo de valor `Position`. |
| `code/position.cpp` | Estado de fila y columna. |
| `code/random_sequence.h` | Interfaz pública de la secuencia aleatoria. |
| `code/random_sequence.cpp` | Estado oculto del generador. |
| `code/benchmark.cpp` | Ejecuta 10,000 partidas con la transcripción redirigida a `/dev/null`. |
| `CMakeLists.txt` | Compilación C++20 de la línea base y el banco de pruebas. |
| `run.sh` | Compila cuando hace falta y ejecuta la línea base. |

## El layout de datos

`Board<8, 8>` conserva los mismos métodos públicos que en `construction-17`. Sus miembros privados
son:

- `std::array<std::uint64_t, 4> visitedWords` — un bit por casilla.
- `std::array<std::uint8_t, 256> occupant` — cero, jugador o máquina.

`reset`, `was_visited`, `mark_visited`, `occupant_at`, `set_occupant` y `clear_occupant` son
métodos, y ambos arreglos siguen siendo privados. El motor sigue poseyendo el tablero y los dos
caballos mediante tres miembros `std::unique_ptr`. En esta línea base no hay ningún cambio de
propiedad.

## Compilar y ejecutar

```bash
./run.sh --seed 19 --name Learner
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

## Perfilar 10,000 partidas

Compilar el banco de pruebas:

```bash
mkdir -p build
c++ -std=c++20 -O2 -Icode -o build/benchmark \
    code/benchmark.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
./build/benchmark
```

El banco redirige la salida estándar a `/dev/null` e informa los milisegundos transcurridos por la
salida de error. En esta máquina el resultado es de unos 270–290 ms para 10,000 partidas, frente a
unos 270–285 ms de `construction-17`. Los rangos se solapan, así que la diferencia es ruido.

Los números estructurales son igual de honestos: `sizeof(Board<8, 8>)` crece de 256 a 288,
`sizeof(GameEngine<8, 8>)` sigue en 24, y ambas líneas base hacen 30,000 reservas para 10,000
partidas (tres por partida). El texto de `game.o` crece de 4,656 a 4,961 bytes. La ganancia es una
representación distinta, no menos reservas ni una transcripción más rápida.

## Prueba de cambio

Ejecutar 10,000 partidas y registrar seis mediciones: `sizeof(Board<8, 8>)`,
`sizeof(GameEngine<8, 8>)`, reservas en el heap, milisegundos transcurridos, texto de `game.o` y
número de trazas que coinciden. Compararlas con `construction-17`. Explicar qué números se movieron
y cuáles no.

## Criterio de aceptación

La persona que aprende puede ejecutar 10,000 partidas, informar el perfil y explicar por qué el
tablero creció 32 bytes mientras el tamaño del motor, el número de reservas y el tiempo de reloj no
se movieron.
