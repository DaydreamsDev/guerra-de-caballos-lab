# construction-03 — Línea base con funciones y parámetros

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** ámbito y descomposición
> **Prerrequisito:** construction-02 · **Tiempo estimado:** 45 minutos

Este directorio contiene el artefacto **después** de `construction-03`. Tiene el mismo
comportamiento observable que `construction-02`, pero el juego está descompuesto en funciones con
parámetros. Las posiciones, el contador de movimientos, el ganador y el tablero de casillas
visitadas ya no están controlados por un único `main` grande.

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
| `code/functions-baseline.cpp` | El juego descompuesto en funciones con parámetros. |
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
    -o build/functions-baseline code/functions-baseline.cpp
```

El binario se escribe en `build/functions-baseline`.

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

## Por qué esta línea base usa funciones y parámetros

- `parseArguments` recibe los argumentos y escribe la semilla y el nombre por referencia.
- `initializePositions` e `initializeRolls` escriben sus resultados por referencia.
- `tryMove` recibe las posiciones de ambos caballos, el número de movimiento y el tablero de
  visitadas; devuelve un resultado y escribe el destino por referencia.
- `playTurn` recibe las posiciones, el tablero de visitadas, el contador de movimientos y el ganador
  por referencia.
- `playGame` posee el bucle y recibe el estado que necesita.
- `printTurnHeader`, las funciones de intento y `printMove` reciben todos los valores que imprimen.

El único global que queda es `randomState`, porque el generador es una secuencia del proceso. El
estado del juego salió del ámbito global.

## Prueba de cambio

Aplicar **R-SKIP**: al inicio de un turno, si el número de turnos iniciados es un múltiplo positivo
de cinco, el caballo activo no se mueve. El turno se anuncia igualmente con sus campos `turn=` y
`order=`; no sigue ninguna línea `try=` ni `move=`. El turno pasa al otro caballo y el turno saltado
no cuenta para los 200 movimientos exitosos.

Predecir el coste antes de editar. Identificar la frontera de función más pequeña que posee el turno
y contar cuántos llamadores deben cambiar. Comparar el resultado con `construction-02`.

## Criterio de aceptación

La persona que aprende puede extraer una decisión de movimiento a una función con parámetros y
mostrar que no necesita estado global del juego para probar esa decisión.
