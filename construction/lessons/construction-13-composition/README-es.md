# construction-13 — Línea base con composición

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** composición sobre herencia · **Prerrequisito:**
> construction-12 · **Tiempo estimado:** 68 minutos

Este directorio contiene el artefacto **después** de `construction-13`. Tiene el mismo
comportamiento observable que `construction-12`, pero el caballo se compone con un objeto `Position`
en lugar de heredar de una interfaz `Piece`.

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
| `code/knight.h` | `Knight` con un miembro `Position` y operaciones delegadas. |
| `code/knight.cpp` | Delegación a `Position` y nombre de rol. |
| `code/position.h` | Tipo de valor `Position`. |
| `code/position.cpp` | Estado de fila y columna. |
| `code/board.h` | Clase `Board` con invariante privado. |
| `code/board.cpp` | Representación y operaciones del tablero. |
| `code/random_sequence.h` | Interfaz pública de la secuencia aleatoria. |
| `code/random_sequence.cpp` | Estado oculto del generador. |
| `CMakeLists.txt` | Compilación C++20 con `-Wall -Wextra -Werror`. |
| `run.sh` | Compila cuando hace falta y ejecuta el binario. |

## La composición

`Knight` contiene un `Position`:

```cpp
class Knight
{
private:
    Position position;
    std::string roleName;
};
```

`Knight::row()` delega en `position.row()`, `Knight::column()` en `position.column()` y
`Knight::set_position()` en `position.set()`. No hay clase base `Piece` ni función virtual.

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

En una copia temporal, reemplazar la herencia de `Piece` de `construction-12` por un `Knight` que
posee un `Position`. Mantener el comportamiento del juego. Contar el código específico de herencia
eliminado y los métodos de delegación añadidos. Las 20 trazas deben seguir pasando.

## Criterio de aceptación

La persona que aprende puede reemplazar la herencia por delegación y mostrar que el comportamiento
del juego y las 20 trazas permanecen sin cambios.
