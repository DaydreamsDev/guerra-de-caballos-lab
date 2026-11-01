# construction-04 — Línea base con registros

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** agregación · **Prerrequisito:** construction-03 · **Tiempo
> estimado:** 60 minutos

Este directorio contiene el artefacto **después** de `construction-04`. Tiene el mismo
comportamiento observable que `construction-03`, pero los dos caballos se representan con un
registro `Knight` en lugar de variables separadas de fila y columna.

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
| `code/records-baseline.cpp` | El juego con un registro `Knight` y parámetros de registro. |
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
    -o build/records-baseline code/records-baseline.cpp
```

El binario se escribe en `build/records-baseline`.

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

## Por qué esta línea base usa un registro

- `Knight` agrupa la fila y la columna que siempre viajan juntas.
- `initializePositions`, `printGameHeader`, `playTurn`, `playGame` y `tryMove` reciben una
  referencia a `Knight` o una referencia `const Knight`.
- El registro puede ganar un campo sin cambiar todas las firmas de función.
- El registro es la unidad que una persona puede inspeccionar, copiar y comparar.

La prueba de cambio es añadir `captured` a `Knight`. El compilador y los llamadores dicen qué
localiza realmente el registro.

## Prueba de cambio

Añadir un campo a `Knight`:

```cpp
bool captured;
```

Inicializarlo para ambos caballos y decidir si el juego debe leerlo. Contar las firmas de función
que cambian. Comparar el conteo con la versión de variables separadas de `construction-03`.

## Criterio de aceptación

La persona que aprende puede añadir `captured` al registro y mostrar qué llamadores cambiaron y
cuáles no.
