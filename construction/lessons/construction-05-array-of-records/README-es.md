# construction-05 — Línea base con arreglo de registros

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** colección y recorrido · **Prerrequisito:** construction-04 ·
> **Tiempo estimado:** 60 minutos

Este directorio contiene el artefacto **después** de `construction-05`. Tiene el mismo
comportamiento observable que `construction-04`, pero el tablero es un arreglo de registros
`BoardCell` en lugar de un arreglo de booleanos.

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
| `code/board-cells-baseline.cpp` | El juego con un arreglo bidimensional de registros `BoardCell`. |
| `CMakeLists.txt` | Compilación C++20 con `-Wall -Wextra -Werror`. |
| `run.sh` | Compila cuando hace falta y ejecuta el binario. |

## Compilar y ejecutar

```bash
./run.sh --seed 17 --name Learner
```

`run.sh` usa CMake cuando está instalado. Cuando CMake no está disponible, compila directamente con
el compilador indicado por `CXX`, o `c++` por defecto:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
    -o build/board-cells-baseline code/board-cells-baseline.cpp
```

El binario se escribe en `build/board-cells-baseline`.

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

## Por qué esta línea base usa un arreglo de registros

- El tablero es `BoardCell boardCells[9][9]`.
- Cada casilla posee su estado de visitada como campo.
- `tryMove`, `playTurn` y `playGame` pasan el tablero como un solo parámetro.
- Un recorrido es un bucle sobre filas y columnas, no una lista de 64 nombres.
- El tablero puede ganar otro campo por casilla sin añadir un arreglo paralelo.

## Prueba de cambio

Añadir un campo a `BoardCell`:

```cpp
int moveNumber;
```

Registrar el número del movimiento exitoso cuando una casilla pasa a visitada. Después escribir un
recorrido que encuentre el número de movimiento más alto registrado. Contar cuántos arreglos
paralelos o 64 variables haría falta para el mismo dato sin el registro.

## Criterio de aceptación

La persona que aprende puede recorrer el tablero con dos bucles, encontrar el último número de
movimiento y explicar por qué no hace falta un arreglo paralelo.
