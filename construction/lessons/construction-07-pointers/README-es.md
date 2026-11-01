# construction-07 — Línea base con punteros

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** indirección y ciclo de vida · **Prerrequisito:**
> construction-06 · **Tiempo estimado:** 88 minutos

Este directorio contiene el artefacto **después** de `construction-07`. Tiene el mismo
comportamiento observable que `construction-06`, pero los alias de los caballos y el acceso a las
casillas del tablero usan punteros. La transcripción no cambia; el código hace visibles la
indirección y el diseño por filas.

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
| `code/pointers-baseline.cpp` | El juego con alias por puntero y acceso a casillas por puntero. |
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
    -o build/pointers-baseline code/pointers-baseline.cpp
```

El binario se escribe en `build/pointers-baseline`.

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

## Qué hacen los punteros

- `selectActiveKnight` devuelve `Knight *`; el llamador sostiene una dirección, no un alias.
- `selectOtherKnight` devuelve `const Knight *`; el objeto apuntado no se puede mutar por ahí.
- `moveKnight(Knight *, ...)` muta el objeto apuntado, no una copia.
- `getBoardCell` devuelve `BoardCell *`; el llamador desreferencia con `->`.
- El tablero sigue siendo un arreglo bidimensional; el puntero expone la dirección de una casilla
  en el diseño por filas.

## Prueba de cambio

Reescribir un acceso al tablero con aritmética de punteros dentro de una fila:

```cpp
BoardCell *rowStart = boardCells[row];
BoardCell *cell = rowStart + column;
```

Después explicar qué se rompería si el ancho de fila cambiara de 9 a otro valor. Contar los lugares
que asumen el diseño por filas y comparar el conteo con la línea base de referencias de
`construction-06`.

## Criterio de aceptación

La persona que aprende puede acceder a una casilla del tablero mediante un puntero, explicar el
paso de fila y mostrar que la transcripción no cambia mientras el código expone el diseño.
