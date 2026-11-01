# construction-10 — Línea base con invariantes del TAD

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** validez y ciclo de vida · **Prerrequisito:**
> construction-09 · **Tiempo estimado:** 60 minutos

Este directorio contiene el artefacto **después** de `construction-10`. Tiene el mismo
comportamiento observable que `construction-09`, pero el TAD `Board` ahora expone y hace cumplir un
invariante: `board_is_valid` comprueba que ninguna casilla fuera del área activa esté marcada como
visitada.

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
| `code/board.h` | Tipo `Board` opaco, operaciones y comprobación de validez. |
| `code/board.cpp` | Representación oculta, comprobaciones de límites e invariante. |
| `code/random_sequence.h` | Interfaz pública de la secuencia aleatoria. |
| `code/random_sequence.cpp` | Estado oculto del generador. |
| `CMakeLists.txt` | Compilación C++20 con `-Wall -Wextra -Werror`. |
| `run.sh` | Compila cuando hace falta y ejecuta el binario. |

## El invariante

`board.h` añade:

```cpp
bool board_is_valid(const Board *board);
```

La implementación garantiza:

- un tablero creado no es nulo;
- solo las casillas dentro del área activa pueden marcarse como visitadas;
- `board_mark_visited` ignora filas y columnas fuera de rango;
- `board_was_visited` devuelve falso para filas y columnas fuera de rango;
- `board_reset` limpia el área activa;
- `board_is_valid` devuelve falso si alguna casilla fuera del área activa está visitada.

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

En una copia temporal de `board.cpp`, quitar la comprobación de límites de `board_mark_visited`.
Llamar a `board_mark_visited(0, 0)` y después a `board_is_valid`. El estado inválido ahora se puede
construir. Restaurar la comprobación y repetir; el estado inválido se rechaza.

## Criterio de aceptación

La persona que aprende puede mostrar que una escritura fuera de rango se rechaza, que
`board_is_valid` sigue siendo verdadero y que quitar la comprobación hace
visible el estado inválido.
