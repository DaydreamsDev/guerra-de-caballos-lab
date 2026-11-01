# construction-06 — Línea base con referencias

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** alias sin direcciones · **Prerrequisito:** construction-05 ·
> **Tiempo estimado:** 45 minutos

Este directorio contiene el artefacto **después** de `construction-06`. Tiene el mismo
comportamiento observable que `construction-05`, pero el alias es explícito: el caballo activo se
selecciona como referencia, el otro como referencia `const`, y un movimiento muta el registro
seleccionado a través de un parámetro de referencia.

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
| `code/references-baseline.cpp` | El juego con alias por referencia y un parámetro que muta. |
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
    -o build/references-baseline code/references-baseline.cpp
```

El binario se escribe en `build/references-baseline`.

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

## Qué hacen las referencias

- `selectActiveKnight` devuelve un `Knight &`, así que el llamador recibe un alias del registro del
  jugador o de la máquina.
- `selectOtherKnight` devuelve un `const Knight &`, así que el llamador puede leer pero no mutar el
  otro registro.
- `moveKnight(Knight &knight, ...)` muta el registro del llamador a través de un parámetro de
  referencia.
- No hace falta puntero, operador de dirección ni desreferencia manual.
- La referencia hace explícito el alias en la frontera de la función y en el nombre local.

## Prueba de cambio

Cambiar `moveKnight` para que reciba `Knight knight` por valor. Ejecutar la misma semilla y observar
que la transcripción ya no muestra el movimiento. Restaurar la referencia y observar que sí lo
muestra. Contar las firmas y llamadores que toca el cambio.

## Criterio de aceptación

La persona que aprende puede mutar un registro a través de un parámetro de referencia y explicar por
qué la misma función con un parámetro por valor no puede cambiar el registro del llamador.
