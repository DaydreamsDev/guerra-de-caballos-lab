# construction-11 — Línea base con encapsulación

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** clase con invariante · **Prerrequisito:** construction-10 ·
> **Tiempo estimado:** 60 minutos

Este directorio contiene el artefacto **después** de `construction-11`. Tiene el mismo
comportamiento observable que `construction-10`, pero `Board` ahora es una clase: los datos son
privados y cada operación pública preserva el invariante.

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
| `code/board.h` | Clase `Board` con operaciones públicas y estado privado. |
| `code/board.cpp` | Representación privada y operaciones que preservan el invariante. |
| `code/random_sequence.h` | Interfaz pública de la secuencia aleatoria. |
| `code/random_sequence.cpp` | Estado oculto del generador. |
| `CMakeLists.txt` | Compilación C++20 con `-Wall -Wextra -Werror`. |
| `run.sh` | Compila cuando hace falta y ejecuta el binario. |

## La frontera de la clase

`Board` expone solo:

```cpp
Board();
int row_count() const;
int column_count() const;
bool is_valid() const;
void reset();
bool was_visited(int row, int column) const;
void mark_visited(int row, int column);
```

El arreglo, las dimensiones y la representación son privados. `game.cpp` posee un `Board` por valor
y solo puede llamar a las operaciones públicas.

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

En una copia temporal de `board.h`, mover `visited` a la sección pública. Compilar una sonda que
escriba `board.visited[0][0] = true` y después llame a `board.is_valid()`. El estado inválido ahora
se puede construir. Restaurar `private` y repetir; la escritura ya no compila.

## Criterio de aceptación

La persona que aprende puede mostrar que ningún campo u operación pública puede romper el invariante
del tablero, y que hacer público el estado permite construir el estado inválido.
