# construction-12 — Línea base con polimorfismo

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** ligadura dinámica · **Prerrequisito:** construction-11 ·
> **Tiempo estimado:** 80 minutos

Este directorio contiene el artefacto **después** de `construction-12`. Tiene el mismo
comportamiento observable que `construction-11`, pero los dos caballos se acceden mediante una
interfaz `Piece` y el bucle del juego usa despacho virtual en lugar de
llamadas concretas al caballo.

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
| `code/piece.h` | Interfaz abstracta `Piece`. |
| `code/knight_piece.h` | Implementación `KnightPiece` de `Piece`. |
| `code/knight_piece.cpp` | Estado del caballo e implementaciones virtuales. |
| `code/board.h` | Clase `Board` con invariante privado. |
| `code/board.cpp` | Representación y operaciones del tablero. |
| `code/random_sequence.h` | Interfaz pública de la secuencia aleatoria. |
| `code/random_sequence.cpp` | Estado oculto del generador. |
| `CMakeLists.txt` | Compilación C++20 con `-Wall -Wextra -Werror`. |
| `run.sh` | Compila cuando hace falta y ejecuta el binario. |

## La interfaz

`piece.h` declara:

```cpp
class Piece
{
public:
    virtual ~Piece() = default;
    virtual int row() const = 0;
    virtual int column() const = 0;
    virtual void set_position(int row, int column) = 0;
    virtual const char *role_name() const = 0;
};
```

`game.cpp` sostiene `Piece *` y `const Piece *` y solo llama a esas operaciones virtuales. Nunca
decide según un tipo concreto de pieza.

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

Añadir una nueva subclase de `Piece` en un archivo temporal, implementar las cuatro operaciones
virtuales y sustituirla en el sitio de construcción dentro de `run_game`. El bucle del juego no debe
necesitar ninguna edición.

## Criterio de aceptación

La persona que aprende puede añadir un nuevo tipo de `Piece` y mostrar que el bucle del juego, la
lógica de turnos y el código de transcripción no requieren ediciones.
