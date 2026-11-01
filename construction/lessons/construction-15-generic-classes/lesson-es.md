# Clases genéricas y conceptos (generic classes and concepts)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** restricciones · **Tiempo:** unos 81 minutos · **Estado:** borrador
**Lectura:** ~22 minutos

## Por qué existe esta lección

`construction-14` terminó con una función genérica. `generate_move_candidates` aceptaba cualquier
tipo con `row()` y `column()`, y el bucle del juego seguía nombrando `Knight` en diez firmas. El
tablero era una clase concreta con dos constantes, `activeRowCount` y `activeColumnCount`, fijadas
en 8.

Esta línea base mueve las dimensiones al tipo. `Board` y `GameEngine` son plantillas de clase:

```cpp
template <int RowCount, int ColumnCount>
class Board
{
    ...
};

template <int RowCount, int ColumnCount>
class GameEngine
{
    ...
};
```

`game.cpp` instancia `GameEngine<8, 8>`. Un programa temporal instancia `GameEngine<10, 10>` desde
el mismo código. El contrato de la pieza se nombra con un concepto de C++20:

```cpp
template <typename PieceType>
concept PositionedPiece = requires(PieceType piece, const PieceType constPiece) {
    { constPiece.row() } -> std::convertible_to<int>;
    { constPiece.column() } -> std::convertible_to<int>;
    { piece.set_position(1, 1) } -> std::same_as<void>;
    { constPiece.role_name() } -> std::convertible_to<const char *>;
};

static_assert(PositionedPiece<Knight>);
```

La frase que hay que retener:

> Una plantilla de clase es un patrón para una clase: el compilador escribe una clase por cada
> conjunto de argumentos. Un concepto nombra las operaciones que un tipo debe ofrecer, y el
> compilador las comprueba donde se usa el tipo.

Tres notas honestas antes de la comparación.

1. El código crece 11 líneas, de 587 en doce archivos a 598 en doce. La forma cambia más que el
   recuento: `board.cpp` (65 líneas) desaparece, `game.cpp` baja de 281 a 10, `board.h` crece de 21
   a 76, y `game_engine.h` es nuevo con 292.
2. El texto enlazado se reduce 86 bytes, de 9.496 a 9.410, y los datos siguen en 692. El archivo
   objeto de `game.cpp` crece 479 bytes, de 3.913 a 4.392, porque el código del motor ahora vive en
   un encabezado y se emite en la unidad que lo instancia. La dirección depende de qué unidad midas.
3. El motor es genérico sobre dimensiones, no sobre el tipo de pieza. `GameEngine` todavía declara
   `Knight playerKnight;` y `Knight machineKnight;`, y seis firmas de miembro nombran `Knight`. El
   concepto comprueba `Knight` una vez, con `static_assert`; no restringe un parámetro `PieceType`,
   porque no existe tal parámetro. Esa brecha es el límite de la lección, y el Paso 6 la nombra.

La comparación con `construction-14` no es "las plantillas de clase son mejores". Es más precisa: la
plantilla de clase compra un tablero y un motor para cualquier dimensión entre 1 y 16, y paga con
una clase por lista de argumentos, un cuerpo visible en el encabezado, un almacenamiento fijo de
16×16 y un tipo de pieza que sigue siendo concreto. Esta lección mide ambos lados.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar restricciones en una plantilla de clase;
- instanciar el mismo motor para tableros de 8x8 y 10x10.

## El concepto: una clase con parámetros

`construction-14` preguntó: ¿puede una función servir para más de un tipo de pieza? La respuesta fue
sí, si el tipo de pieza se convierte en un parámetro de la función. El bucle seguía nombrando
`Knight`, y el tablero seguía siendo una clase de 8×8.

Esta lección pregunta algo distinto: ¿pueden el tablero y el motor escribirse una vez para más de un
tamaño de tablero, sin copiar el bucle de turnos?

Sí, si las dimensiones se convierten en parámetros de la clase. `Board` no nombra 8. Nombra
`RowCount` y `ColumnCount`, marcadores que el compilador reemplaza con valores en cada
instanciación:

```cpp
template <int RowCount, int ColumnCount>
class Board;
```

Una plantilla de clase no es una clase. Es un patrón. El compilador produce una clase cuando ve un
uso con argumentos concretos. `Board<8, 8>` y `Board<10, 10>` son dos tipos distintos generados
desde las mismas 76 líneas. Lo mismo ocurre con `GameEngine<8, 8>` y `GameEngine<10, 10>`.

### Tres clases de parámetro de plantilla

| Clase | Sintaxis | Qué varía | Dónde aparece aquí |
|---|---|---|---|
| Parámetro de tipo | `typename PieceType` | un tipo | `generate_move_candidates<PieceType>` |
| Parámetro que no es de tipo | `int RowCount` | un valor en tiempo de compilación | `Board<RowCount, ColumnCount>` |
| Parámetro de tipo restringido | `PositionedPiece PieceType` | un tipo que satisface un concepto | no lo usa `GameEngine`; solo hay un `static_assert` |

La primera clase apareció en `construction-14`. La segunda es esta lección. La tercera está
disponible en C++20 y es el siguiente paso natural, pero esta línea base no la usa como parámetro.
Esa distinción importa: un concepto puede ser una restricción sobre un parámetro de plantilla, o
puede ser una comprobación independiente en tiempo de compilación. Aquí es la segunda.

### Qué es genérico y qué está restringido

| Pregunta | Respuesta |
|---|---|
| ¿Qué es genérico? | `RowCount` y `ColumnCount`; el código del tablero y del motor. |
| ¿Qué está restringido? | Las dimensiones, a 1..16, y el tipo de pieza, por `PositionedPiece`. |
| ¿Qué cambia con las dimensiones? | Límites, número de casillas, posiciones iniciales, movimientos legales, transcripción. |
| ¿Qué no cambia con las dimensiones? | Almacenamiento, `sizeof`, las reglas, la geometría, el tipo de pieza. |
| ¿Qué sigue siendo concreto? | `Knight`, el máximo de 16×16, el empate a 200 movimientos, los arreglos de desplazamientos. |

Dos restricciones, dos mecanismos:

1. **Dimensiones.** `board.h:72–73` contiene dos `static_assert`:
   `RowCount >= 1 && RowCount <= maximumRowCount`, y lo mismo para las columnas. Es una comprobación
   local de rango sobre parámetros que no son de tipo. No es un concepto.
2. **Operaciones de la pieza.** `game_engine.h:14–20` define `PositionedPiece`, y la línea 22
   comprueba `static_assert(PositionedPiece<Knight>)`. Es un predicado nombrado y reutilizable sobre
   un tipo. Es un concepto, pero se usa como comprobación independiente, no como restricción de un
   parámetro de plantilla.

Qué atrapa cada uno:

- `GameEngine<17, 8>` falla en `board.h:72` con `static assertion failed`.
- `GameEngine<0, 8>` falla en la misma línea.
- `static_assert(PositionedPiece<IncompletePawn>)` falla con `constraints not satisfied`.
- Un tipo con `row()` y `column()` pero sin `set_position` no satisface el concepto.

Qué no atrapa ninguno:

- Un tipo que satisface `PositionedPiece` pero se mueve como un alfil. El concepto nombra
  operaciones, no semántica.
- Una dimensión legal pero demasiado pequeña para jugar bien, como 1×1. El `static_assert` la
  permite; el juego puede quedar atrapado de inmediato. Es un límite semántico, no un error de tipo.

### Qué cambia con las dimensiones

| Cosa | 8×8 | 10×10 | Por qué |
|---|---|---|---|
| Casillas activas | 64 | 100 | `RowCount * ColumnCount` |
| Almacenamiento del tablero | 256 bytes | 256 bytes | `bool[16][16]` fijo |
| `sizeof(Board)` | 256 | 256 | mismo almacenamiento |
| `sizeof(GameEngine)` | 336 | 336 | tablero más dos caballos |
| Jugador inicial (semilla 1) | `8,5` | `2,3` | `random_below(64)` frente a `random_below(100)` |
| Máquina inicial (semilla 1) | `4,4` | `2,10` | la misma razón |
| Tiradas (semilla 1) | `11,20` | `11,20` | mismo número de extracciones, mismo estado del LCG |
| Turnos (semilla 1) | 22 | 27 | movimientos legales distintos |
| Intentos (semilla 1) | 36 | 55 | tablero distinto |
| Ganador (semilla 1) | machine | machine | misma semilla, juego distinto |
| Movimientos (semilla 1) | 21 | 27 | juego distinto |
| Líneas de transcripción (semilla 1) | 89 | 118 | juego distinto |
| Trazas de referencia | 20 de 20 | ninguna | no hay contrato para 10×10 |

Las tiradas son idénticas porque `run()` extrae dos números de casilla y luego dos tiradas. El
número de extracciones no depende del tamaño del tablero, así que el estado del LCG en la tercera
extracción es el mismo. Solo cambian los límites y, con ellos, los números de casilla.

### El contrato de la pieza está nombrado, no implícito

En `construction-14`, los requisitos del generador vivían solo en su cuerpo:

```cpp
piece.row() + knightRowChangePerMove[index]
piece.column() + knightColumnChangePerMove[index]
```

Un tipo incorrecto producía una cascada de errores dentro de `move_generator.h`. En esta línea base,
las mismas operaciones se escriben una vez, en un concepto:

```cpp
template <typename PieceType>
concept PositionedPiece = requires(PieceType piece, const PieceType constPiece) {
    { constPiece.row() } -> std::convertible_to<int>;
    { constPiece.column() } -> std::convertible_to<int>;
    { piece.set_position(1, 1) } -> std::same_as<void>;
    { constPiece.role_name() } -> std::convertible_to<const char *>;
};
```

El concepto añade dos requisitos que el generador no necesitaba: `set_position` y `role_name`. Los
exige el motor, no el generador. Es la primera señal de que el contrato ahora es el contrato del
motor, no el del generador.

`static_assert(PositionedPiece<Knight>)` hace que la comprobación forme parte de la compilación. Si
`Knight` pierde una operación, la compilación se detiene en la línea 22 con un mensaje. Esa es la
mejora frente a la cascada.

El límite está en el párrafo siguiente. Un concepto describe la forma de un tipo. No describe qué
significan las operaciones, y no hace que el motor acepte un segundo tipo de pieza. `GameEngine`
sigue declarando dos miembros `Knight`. El concepto es una comprobación, no un parámetro.

## Antes de empezar

- [`construction-14`](../construction-14-generic-functions/lesson-es.md) — la línea base de la que
  parte esta lección. Su `game.cpp` es el lado izquierdo del diff estructural del Paso 6.
- [`construction-11`](../construction-11-encapsulation/lesson-es.md) — el límite de la clase.
  `Position` y `Knight` guardan estado privado y operaciones públicas. `Board` también guarda su
  almacenamiento privado.
- [`construction-09`](../construction-09-abstract-data-type/lesson-es.md) — independencia de la
  representación. Las operaciones públicas del tablero son las mismas; solo se movieron sus
  definiciones.
- [`construction-04`](../construction-04-records/lesson-es.md) — registros. `MoveCandidate` sigue
  siendo un registro con tres campos públicos.
- Un compilador de C++20 (`g++` o `clang++`) con soporte de conceptos. `cmake` es opcional; `run.sh`
  recurre a una compilación directa cuando CMake no está.

No necesitas saber cómo instancia una plantilla de clase el compilador. Necesitas leer una
plantilla, instanciarla con dos listas de argumentos y comparar los programas generados.

### Qué necesitas

- La línea base de clases genéricas:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/game_engine.h`](code/game_engine.h),
  [`code/board.h`](code/board.h), [`code/move_generator.h`](code/move_generator.h),
  [`code/knight.h`](code/knight.h), [`code/knight.cpp`](code/knight.cpp),
  [`code/position.h`](code/position.h), [`code/position.cpp`](code/position.cpp),
  [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- La línea base anterior, para el diff:
  [`code/game.cpp`](../construction-14-generic-functions/code/game.cpp) y sus once compañeros en
  [`../construction-14-generic-functions/code/`](../construction-14-generic-functions/code/).
- Guiones y notas de compilación: [`run.sh`](run.sh), [`CMakeLists.txt`](CMakeLists.txt) y
  [`README.md`](README.md).
- Las reglas: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §1 fija el tablero
  y su numeración desde 1, §4 fija el orden de movimientos del caballo, §8 fija cuándo una casilla
  pasa a visitada.
- El formato de transcripción:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- Una traza de referencia: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- Esta plantilla de registro: [`EVIDENCE-es.md`](EVIDENCE-es.md).

Comprueba que los artefactos están presentes antes de empezar, desde este directorio de lección:

```bash
test -f code/main.cpp &&
test -f code/game.h &&
test -f code/game.cpp &&
test -f code/game_engine.h &&
test -f code/board.h &&
test -f code/move_generator.h &&
test -f code/knight.h &&
test -f code/knight.cpp &&
test -f code/position.h &&
test -f code/position.cpp &&
test -f code/random_sequence.h &&
test -f code/random_sequence.cpp &&
test -f ../construction-14-generic-functions/code/game.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

Si falta un archivo, la comparación no tiene segundo lado. Registra el archivo faltante en
`EVIDENCE-es.md` §11 y vuelve cuando esté disponible. Un recuento contra un archivo que no has leído
no es evidencia.

La línea base de 8×8 reproduce las 20 trazas de `common/traces/` byte a byte, antes de que cambies
nada. Ese es el contrato de comportamiento para 8×8. No hay traza de referencia para 10×10; la
segunda instanciación es una prueba de humo, no un contrato de comportamiento.

## Paso 1 — Inventariar las plantillas (~12 min)

Lee primero los dos encabezados. La forma es la lección:

```bash
grep -n 'template\|static_assert\|class Board\|class GameEngine\|GameEngine<' \
  code/board.h code/game_engine.h code/game.cpp
```

```text
code/board.h:3:template <int RowCount, int ColumnCount>
code/board.h:4:class Board
code/board.h:72:    static_assert(RowCount >= 1 && RowCount <= maximumRowCount);
code/board.h:73:    static_assert(ColumnCount >= 1 && ColumnCount <= maximumColumnCount);
code/game_engine.h:14:template <typename PieceType>
code/game_engine.h:22:static_assert(PositionedPiece<Knight>);
code/game_engine.h:34:template <int RowCount, int ColumnCount>
code/game_engine.h:35:class GameEngine
code/game.cpp:8:    GameEngine<8, 8> engine;
```

Ahora el mapa de líneas. Rellénalo en `EVIDENCE-es.md` §2.2 desde el código, no desde esta tabla.

`board.h`:

| Líneas | Contenido |
|---|---|
| 1 | `#pragma once` |
| 3–4 | La cabecera de plantilla y `class Board`. |
| 7–10 | Constructor por defecto; inicializa `visited` a cero. |
| 12–20 | `row_count()` y `column_count()` devuelven los parámetros. |
| 22–37 | `is_valid()` recorre el almacenamiento máximo. |
| 39–48 | `reset()` limpia el área activa. |
| 50–57 | `was_visited()` comprueba límites contra los parámetros. |
| 59–66 | `mark_visited()` comprueba límites contra los parámetros. |
| 68–75 | Almacenamiento máximo privado, las comprobaciones de dimensión, el arreglo `visited`. |
| 72–73 | Las dos restricciones de dimensión. |
| 75 | `bool visited[16][16]`. |

`game_engine.h`:

| Líneas | Contenido |
|---|---|
| 1–12 | Includes: tablero, caballo, generador, secuencia aleatoria, cabeceras estándar. |
| 14–20 | `PositionedPiece`, el contrato nombrado de la pieza. |
| 22 | `static_assert(PositionedPiece<Knight>)`. |
| 24–30 | `MoveOutcome`, los cuatro resultados. |
| 32 | `maximumMoveCount = 200`. |
| 34–36 | La cabecera de plantilla y `class GameEngine`. |
| 38–41 | El constructor crea los dos caballos. |
| 43–60 | `run()`: semilla, reinicio, posiciones, tiradas, cabecera, juego. |
| 62–65 | Datos privados: un tablero y dos caballos. |
| 67–75 | La conversión de número de casilla usa `column_count()`. |
| 77–90 | Las posiciones iniciales usan `row_count() * column_count()`. |
| 92–100 | Tiradas. |
| 102–112 | Cabecera del juego. |
| 114–121 | Orden de movimientos. |
| 123–138 | Cabecera del turno. |
| 140–158 | Impresores de intentos. |
| 160–170 | Impresor de movimientos. |
| 172–185 | Selección y movimiento de caballos. |
| 187–208 | `tryMove` comprueba límites contra los parámetros. |
| 210–276 | `playTurn`. |
| 278–291 | `playGame`. |

Recuentos de esta línea base. La regla de recuento se indica para que puedas reproducirla:

- Encabezados: 7 (`board.h`, `game.h`, `game_engine.h`, `knight.h`, `move_generator.h`,
  `position.h`, `random_sequence.h`).
- Unidades de traducción: 5 (`main.cpp`, `game.cpp`, `knight.cpp`, `position.cpp`,
  `random_sequence.cpp`).
- Definiciones de clase completas: 5 (`Board`, `GameEngine`, `Knight`, `Position`,
  `MoveCandidate`).
- Tipos incompletos: 0.
- Definiciones de función: 41.
- Ayudas internas en `game_engine.h`: 17.
- Ayuda interna en `main.cpp`: 1.
- Puntos de entrada del programa: 1.
- Declaraciones de datos privados en encabezados: 10.
- Declaraciones públicas, contadas como funciones miembro públicas (constructores incluidos),
  campos de datos públicos, funciones libres y conceptos y enumeraciones de ámbito de espacio de
  nombres: 27.

Comparación con `construction-14`:

- Encabezados: 6 → 7.
- Unidades de traducción: 6 → 5.
- Definiciones de clase completas: 4 → 5.
- Definiciones de función: 39 → 41.
- Declaraciones de datos privados en encabezados: 9 → 10.
- Declaraciones públicas: 23 → 27.
- Líneas de código: 587 → 598.
- Archivos: 12 → 12.
- `board.cpp`: 65 líneas → ausente.
- `board.h`: 21 líneas → 76.
- `game.cpp`: 281 líneas → 10.
- `game_engine.h`: ausente → 292.

Después responde en `EVIDENCE-es.md` §2:

1. ¿Qué archivo contiene `Board`, y en qué líneas?
2. ¿Qué archivo contiene `GameEngine`, y en qué líneas?
3. ¿Qué clase de parámetros de plantilla declara cada uno? ¿De tipo o que no son de tipo?
4. ¿Dónde está `PositionedPiece`, y qué cuatro operaciones exige?
5. ¿Dónde está `static_assert(PositionedPiece<Knight>)`?
6. ¿`GameEngine` recibe un tipo de pieza como parámetro? ¿Qué declara en su lugar?
7. ¿Cuántas firmas de miembro en `game_engine.h` nombran `Knight`?
8. ¿Dónde se instancia `GameEngine<8, 8>`?
9. ¿Dónde están las restricciones de dimensión, y qué rango permiten?
10. ¿Qué pasó con `board.cpp`?

La pregunta 3 tiene una respuesta precisa: ambos declaran dos parámetros `int` que no son de tipo,
`RowCount` y `ColumnCount`. Ninguno declara un parámetro de tipo. `PositionedPiece` es un parámetro
de tipo solo dentro de su propia definición de concepto.

La pregunta 6 tiene una respuesta precisa: no. `GameEngine` declara `Knight playerKnight;` y
`Knight machineKnight;` en las líneas 64–65.

La pregunta 7 tiene una respuesta precisa: seis firmas de miembro nombran `Knight` —
`printTurnHeader`, `printMove`, `selectActiveKnight`, `selectOtherKnight`, `moveKnight` y `tryMove`.
`playTurn` usa punteros a `Knight` como locales, pero su firma no nombra el tipo. Los dos miembros
de datos son aparte.

La pregunta 10 tiene una respuesta precisa: se eliminó. Una plantilla de clase necesita sus
definiciones visibles en el punto de instanciación, así que las definiciones de `Board` se movieron
de `board.cpp` a `board.h`.

## Paso 2 — Predecir antes de ejecutar (~12 min)

### 2.1 La tabla de dimensiones

Escribe estas predicciones antes de ejecutar nada. Semilla 1, nombre `Learner`.

| Pregunta | Mi predicción para 8×8 | Mi predicción para 10×10 | Confianza |
|---|---|---|---|
| Posición inicial del jugador | | | |
| Posición inicial de la máquina | | | |
| Tiradas | | | |
| Turnos | | | |
| Intentos | | | |
| Ganador | | | |
| Movimientos | | | |
| Líneas de transcripción | | | |
| `sizeof(Board)` | | | |
| `sizeof(GameEngine)` | | | |
| ¿Compila? | | | |
| ¿Misma salida que 8×8? | | | |

Las filas que hay que tomar en serio son las posiciones, los turnos y los tamaños. Un tablero con 36
casillas más cambia los números aleatorios de casilla, así que las posiciones iniciales cambian. No
cambia el almacenamiento, porque el arreglo sigue siendo 16×16. Predice ambas cosas y explica por
qué.

### 2.2 La prueba de cambio

El cambio es: instanciar el mismo motor para 10×10, desde un archivo temporal, sin editar la línea
base.

| Pregunta | Mi predicción |
|---|---|
| Archivos que añadiré | |
| Archivos que editaré | |
| Líneas del archivo nuevo | |
| Conceptos nuevos que exige el cambio | |
| Semillas que coinciden, de 20, tras el cambio | |
| ¿Compila 10×10? | |
| ¿La salida de 10×10 es idéntica a 8×8? | |
| Archivos rastreados que tocaré | |

Las filas sexta y séptima son la aceptación. Predice "compila" y "distinta" antes de medir. Si la
salida de 10×10 fuera idéntica, las dimensiones no estarían haciendo nada.

### 2.3 Instanciación y tamaños

| Pregunta | Mi predicción |
|---|---|
| Instanciaciones de `GameEngine` en el juego de 8×8 | |
| Instanciaciones de `GameEngine` en una unidad con ambos tamaños | |
| Símbolos `nm -C both.o` para `GameEngine<8, 8>`, en `-O0` | |
| El mismo recuento para `GameEngine<10, 10>` | |
| Símbolos de `GameEngine<8, 8>` en `-O2` | |
| `sizeof(Board<8, 8>)` | |
| `sizeof(Board<10, 10>)` | |
| `sizeof(GameEngine<8, 8>)` | |
| `sizeof(GameEngine<10, 10>)` | |
| Llamadas indirectas en `both.o` | |

Los recuentos de símbolos son la prueba del mecanismo. Una plantilla de clase que se inlinea tiene
menos símbolos; una que no se inlinea tiene un conjunto por lista de argumentos. Predice ambos
recuentos antes de compilar nada.

## Paso 3 — Ejecutar la línea base y la segunda instanciación (~12 min)

Primero las trazas, sin modificar:

```bash
mkdir -p /tmp/generic-classes-test
./run.sh --seed 1 --name Learner > /tmp/generic-classes-test/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/generic-classes-test/baseline-0001.txt
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

Un diff vacío y un bucle silencioso: 20 de 20. Si `run.sh` no puede compilar, compila una vez a
mano:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o build/generic-classes-baseline code/main.cpp code/game.cpp code/knight.cpp \
  code/position.cpp code/random_sequence.cpp
```

Ahora la sonda de 10×10. Va en el directorio temporal, nunca en los archivos rastreados:

```bash
cat > /tmp/generic-classes-test/ten.cpp <<'EOF'
#include "game_engine.h"

int main()
{
    GameEngine<10, 10> engine;
    return engine.run(1, "Learner");
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/generic-classes-test/ten \
  /tmp/generic-classes-test/ten.cpp code/knight.cpp code/position.cpp \
  code/random_sequence.cpp
/tmp/generic-classes-test/ten > /tmp/generic-classes-test/ten-0001.txt
```

La salida en esta plataforma:

```text
$ wc -l /tmp/generic-classes-test/baseline-0001.txt \
        /tmp/generic-classes-test/ten-0001.txt
  89 /tmp/generic-classes-test/baseline-0001.txt
 118 /tmp/generic-classes-test/ten-0001.txt
$ head -n 8 /tmp/generic-classes-test/ten-0001.txt
seed=1
player=Learner
roll_player=11
roll_machine=20
start=machine
initial_player=2,3
initial_machine=2,10
turn=1 who=machine at=2,10 order=1,2,3,4,5,6,7,8
$ tail -n 2 /tmp/generic-classes-test/ten-0001.txt
winner=machine
moves=27
```

La salida de 8×8 empieza con `initial_player=8,5` e `initial_machine=4,4`, y termina con `moves=21`.
Las tiradas son las mismas. Las posiciones no. El tablero cambió, así que el juego cambió.

Ahora los tamaños:

```bash
cat > /tmp/generic-classes-test/sizes.cpp <<'EOF'
#include "game_engine.h"

#include <cstdio>

int main()
{
    std::printf("sizeof(Board<8,8>)=%zu\n", sizeof(Board<8, 8>));
    std::printf("sizeof(Board<10,10>)=%zu\n", sizeof(Board<10, 10>));
    std::printf("sizeof(GameEngine<8,8>)=%zu\n", sizeof(GameEngine<8, 8>));
    std::printf("sizeof(GameEngine<10,10>)=%zu\n", sizeof(GameEngine<10, 10>));
    std::printf("sizeof(Knight)=%zu\n", sizeof(Knight));
    std::printf("sizeof(MoveCandidate)=%zu\n", sizeof(MoveCandidate));
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/generic-classes-test/sizes \
  /tmp/generic-classes-test/sizes.cpp code/knight.cpp code/position.cpp
/tmp/generic-classes-test/sizes
```

```text
sizeof(Board<8,8>)=256
sizeof(Board<10,10>)=256
sizeof(GameEngine<8,8>)=336
sizeof(GameEngine<10,10>)=336
sizeof(Knight)=40
sizeof(MoveCandidate)=12
```

Los dos tipos de tablero tienen el mismo tamaño porque el almacenamiento está fijo en 16×16. Los
parámetros cambian el área activa, no el arreglo. Esa es la primera sorpresa para la mayoría de los
aprendices: "10×10" no significa "100 bytes".

Ahora pon las dos instanciaciones en una sola unidad de traducción y cuenta los símbolos:

```bash
cat > /tmp/generic-classes-test/both.cpp <<'EOF'
#include "game_engine.h"

int main()
{
    GameEngine<8, 8> eight;
    GameEngine<10, 10> ten;
    return eight.run(1, "Learner") + ten.run(1, "Learner");
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O0 -Icode -c \
  /tmp/generic-classes-test/both.cpp -o /tmp/generic-classes-test/both.o
nm -C /tmp/generic-classes-test/both.o | grep -c 'GameEngine<8, 8>'
nm -C /tmp/generic-classes-test/both.o | grep -c 'GameEngine<10, 10>'
```

```text
24
24
```

En `-O2`, la misma unidad tiene un símbolo por motor, `playTurn`, y ningún símbolo de `Board`,
porque el resto se inlineó:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode -c \
  /tmp/generic-classes-test/both.cpp -o /tmp/generic-classes-test/both-O2.o
nm -C /tmp/generic-classes-test/both-O2.o | grep -c 'GameEngine<8, 8>'
nm -C /tmp/generic-classes-test/both-O2.o | grep -c 'GameEngine<10, 10>'
nm -C /tmp/generic-classes-test/both-O2.o | grep -c 'Board<8, 8>'
```

```text
1
1
0
```

Si el primer diff no está vacío, detente. Esa es una línea base distinta de la que describe esta
lección; regístrala en `EVIDENCE-es.md` §11 y no ajustes la predicción para que coincida.

## Paso 4 — Comparar: qué demuestran las plantillas de clase (~10 min)

Abre `board.h`, `game_engine.h` y `game.cpp`. Cinco hechos, leídos contra las dos instanciaciones.

1. **Un código, dos clases.** En `-O0`, `nm` muestra 24 símbolos para `GameEngine<8, 8>` (22
   débiles, 2 locales) y 24 para `GameEngine<10, 10>`. En `-O2`, uno cada uno. El código tiene un
   `GameEngine`; el archivo objeto tiene un conjunto de miembros por lista de argumentos, o menos
   después del inline.
2. **Las dimensiones son de tiempo de compilación.** No hay argumento de constructor para filas o
   columnas, no hay `resize`, y no hay rama por dimensiones. `row_count()` devuelve `RowCount`;
   `column_count()` devuelve `ColumnCount`.
3. **El almacenamiento es fijo.** `Board<8, 8>` y `Board<10, 10>` miden ambos 256 bytes, porque
   `visited` es `bool[16][16]`. Los parámetros cambian el área activa, no el almacenamiento.
   `GameEngine` mide 336 bytes en ambos tamaños: 256 del tablero más 80 de dos caballos.
4. **La transcripción de 8×8 es idéntica a la de `construction-14`.** La de 10×10 difiere porque el
   tablero cambió: 27 turnos en vez de 22, 55 intentos en vez de 36, 27 movimientos en vez de 21,
   118 líneas en vez de 89.
5. **El texto total del binario se reduce 86 bytes**, de 9.496 a 9.410, y los datos siguen en 692.
   `game.o` crece 479 bytes, de 3.913 a 4.392, porque el motor se emite en la unidad que lo
   instancia. Las llamadas indirectas siguen en 0 en ambos lados.

La dirección del cambio de tamaño depende de la unidad. El binario total es la medida honesta del
programa; `game.o` es la medida honesta del coste de instanciación. Di cuál de las dos usas.

### 4.1 Qué emite el compilador

Compila `game.cpp` solo en ambos lados:

```bash
g++ -std=c++20 -O2 -Icode -c code/game.cpp -o /tmp/generic-classes-test/game15.o
g++ -std=c++20 -O2 -I../construction-14-generic-functions/code \
  -c ../construction-14-generic-functions/code/game.cpp \
  -o /tmp/generic-classes-test/game14.o
size /tmp/generic-classes-test/game14.o /tmp/generic-classes-test/game15.o
nm -C /tmp/generic-classes-test/game15.o | grep -c 'GameEngine<8, 8>'
objdump -d /tmp/generic-classes-test/game15.o | grep -c 'call.*\*'
```

Con este compilador y `-O2`, el recuento de `nm` es 1 (el símbolo `playTurn`) y el de llamadas
indirectas es 0. En `-O0`, el mismo `game.cpp` emite 24 símbolos para `GameEngine<8, 8>` (22
débiles, 2 locales) y 8 para `Board<8, 8>` (7 débiles, 1 local). Registra ambos en
`EVIDENCE-es.md` §5.2.

La presencia de un símbolo en `-O2` no es una propiedad de las plantillas de clase. Es una decisión
de optimización. Un cuerpo más grande, una compilación de depuración o un compilador distinto pueden
dejar más miembros fuera de línea. La afirmación estructural es más estrecha y verdadera en ambos
casos: los argumentos concretos se fijan en tiempo de compilación, y no hay despacho en tiempo de
ejecución que resolver.

Una nota de compilación: `run.sh` compara cada archivo `.h` de `code/` con el binario y recompila
cuando uno es más reciente. Editar `game_engine.h` o `board.h` provoca una recompilación; no
necesitas borrar `build/` a mano.

### 4.2 El concepto: un mensaje claro, sin coste en ejecución

El contrato del concepto se comprueba donde se usa el tipo. Míralo pasar y fallar en el directorio
temporal:

```bash
cat > /tmp/generic-classes-test/probe/concept.cpp <<'EOF'
#include "game_engine.h"

#include <cstdio>

struct GoodPawn
{
    int rowValue = 1;
    int columnValue = 1;

    int row() const { return rowValue; }
    int column() const { return columnValue; }
    void set_position(int row, int column) { rowValue = row; columnValue = column; }
    const char *role_name() const { return "pawn"; }
};

struct MissingSetter
{
    int row() const { return 1; }
    int column() const { return 1; }
    const char *role_name() const { return "pawn"; }
};

static_assert(PositionedPiece<GoodPawn>);
static_assert(!PositionedPiece<MissingSetter>);

int main()
{
    std::printf("good=%d missing=%d knight=%d\n", PositionedPiece<GoodPawn>,
                PositionedPiece<MissingSetter>, PositionedPiece<Knight>);
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/generic-classes-test/probe/concept \
  /tmp/generic-classes-test/probe/concept.cpp code/knight.cpp code/position.cpp
/tmp/generic-classes-test/probe/concept
```

```text
good=1 missing=0 knight=1
```

Ahora el fallo. Un tipo que no satisface el concepto:

```bash
cat > /tmp/generic-classes-test/probe/missing_operations.cpp <<'EOF'
#include "game_engine.h"

struct IncompletePawn
{
};

static_assert(PositionedPiece<IncompletePawn>);

int main()
{
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode -c \
  /tmp/generic-classes-test/probe/missing_operations.cpp \
  -o /tmp/generic-classes-test/probe/missing_operations.o
```

El compilador informa, en el `static_assert`:

```text
error: static assertion failed
  • constraints not satisfied
  • required by the constraints of 'template<class PieceType> concept PositionedPiece'
```

Un mensaje en la comprobación, más los requisitos. Compáralo con `construction-14`, donde el mismo
tipo de error producía una cascada dentro de `move_generator.h`. El concepto mueve el diagnóstico a
la declaración. Esa es la ganancia.

No hay coste en ejecución. Una expresión `requires` no se evalúa, y un `static_assert` se evalúa en
tiempo de compilación. El concepto no añade objeto, ni tabla virtual, ni rama. `objdump` no muestra
llamadas indirectas.

### 4.3 Qué no comprueba el concepto

`PositionedPiece` comprueba que las operaciones existen y devuelven tipos compatibles. No comprueba:

- que `row()` y `column()` devuelvan la casilla actual de la pieza;
- que `set_position` actualice el mismo estado que leen `row()` y `column()`;
- que `role_name()` devuelva una cadena estable;
- que la pieza se mueva como un caballo;
- que el tipo se pueda construir a partir de un nombre de rol.

El último punto importa para el siguiente cambio. `GameEngine` construye `Knight("player")` y
`Knight("machine")` en la línea 39. `PositionedPiece` no exige un constructor desde `std::string`.
Un motor completamente genérico debe añadir ese requisito al concepto, o aceptar las piezas del
llamador. Esta línea base evita la pregunta fijando `Knight`.

## Paso 5 — Ejecutar la prueba de cambio (~15 min)

La prueba de cambio es la que nombra la aceptación: instanciar el mismo motor para 10×10. La línea
base no se edita. El archivo nuevo es temporal.

```bash
lessonDirectory="$(pwd)"
rm -rf /tmp/generic-classes-test/change
mkdir -p /tmp/generic-classes-test/change
cp -r code /tmp/generic-classes-test/change/code
cd /tmp/generic-classes-test/change
```

### 5.1 Añadir la segunda instanciación

```bash
cat > ten.cpp <<'EOF'
#include "game_engine.h"

int main()
{
    GameEngine<10, 10> engine;
    return engine.run(1, "Learner");
}
EOF
```

Son 9 líneas en un archivo nuevo. Incluye el mismo encabezado que incluye el juego de 8×8. No nombra
ningún tipo de tablero directamente; el motor posee el tablero.

### 5.2 Compilar y ejecutar

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o ten ten.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
./ten > ten-0001.txt
cd "${lessonDirectory}"
diff -u /tmp/generic-classes-test/baseline-0001.txt \
        /tmp/generic-classes-test/change/ten-0001.txt | head -n 20
```

El diff no está vacío. Ese es el punto. La salida de 8×8 y la de 10×10 comparten la semilla, el
nombre del jugador y las tiradas, y difieren en las posiciones, los turnos y los movimientos.

### 5.3 Contar qué cambió

| | 8×8 | 10×10 | Diferencia |
|---|---|---|---|
| Líneas de código temporales | 0 | 9 | +9 |
| Archivos rastreados editados | 0 | 0 | 0 |
| Instanciaciones de `GameEngine` | 1 | 1 | 0 |
| Casillas activas | 64 | 100 | +36 |
| Bytes de almacenamiento | 256 | 256 | 0 |
| Turnos (semilla 1) | 22 | 27 | +5 |
| Intentos (semilla 1) | 36 | 55 | +19 |
| Movimientos (semilla 1) | 21 | 27 | +6 |
| Líneas de transcripción (semilla 1) | 89 | 118 | +29 |
| Trazas coincidentes | 20 de 20 | no comparadas | — |

El cambio cuesta un archivo nuevo y cero ediciones a la línea base. Compra un segundo tamaño de
tablero desde el mismo código del motor. No compra un segundo tipo de pieza.

### 5.4 El límite de la aceptación

La aceptación dice: instanciar el mismo motor para 8×8 y 10×10. No dice que el motor sea genérico
sobre piezas, y no lo es. `GameEngine` sigue teniendo miembros `Knight` y seis firmas `Knight`. La
instanciación de 10×10 demuestra que las dimensiones son parámetros. No demuestra que el tipo de
pieza lo sea.

Mide ese cambio con honestidad en `EVIDENCE-es.md` §6.3: hacer que el tipo de pieza sea un
parámetro tocaría la cabecera de plantilla, los dos miembros de datos, el constructor, seis firmas y
los lugares que llaman operaciones de `Knight`. El concepto está listo para ese cambio; el motor
todavía no lo usa como parámetro.

## Paso 6 — Comparar con construction-14 (~10 min)

El diff estructural es donde la plantilla de clase muestra su coste.

| Métrica | construction-14 | construction-15 | Diferencia |
|---|---|---|---|
| Archivos | 12 | 12 | 0 |
| Encabezados | 6 | 7 | +1 |
| Unidades de traducción | 6 | 5 | -1 |
| Líneas de código | 587 | 598 | +11 |
| `board.cpp` | 65 | ausente | -65 |
| `board.h` | 21 | 76 | +55 |
| `game.cpp` | 281 | 10 | -271 |
| `game_engine.h` | ausente | 292 | +292 |
| Clases completas | 4 | 5 | +1 |
| Definiciones de función | 39 | 41 | +2 |
| Datos privados en encabezados | 9 | 10 | +1 |
| Declaraciones públicas | 23 | 27 | +4 |
| Texto de `game.o` | 3.913 | 4.392 | +479 |
| Texto / datos del binario | 9.496 / 692 | 9.410 / 692 | -86 / 0 |
| Llamadas indirectas | 0 | 0 | 0 |
| Trazas | 20 de 20 | 20 de 20 (8×8) | 0 |

La forma es la historia: `board.cpp` desaparece porque una plantilla de clase debe mantener sus
definiciones visibles; `game.cpp` se convierte en una instanciación de diez líneas; `game_engine.h`
absorbe el bucle de turnos. El código crece 11 líneas netas, pero la distribución cambia en cientos.

### 6.1 Qué se movió y adónde

| Código | construction-14 | construction-15 |
|---|---|---|
| Declaración del tablero | `board.h` | `board.h` |
| Definiciones del tablero | `board.cpp` | `board.h`, en línea |
| Dimensiones activas | `activeRowCount`, `activeColumnCount` | `RowCount`, `ColumnCount` |
| Bucle de turnos | `game.cpp`, funciones libres | `game_engine.h`, funciones miembro |
| Estado del motor | locales en `run_game` y ayudas | datos miembro: tablero, dos caballos |
| Punto de entrada | `run_game` | `run_game` llama a `GameEngine<8, 8>::run` |
| Contrato de la pieza | implícito en el generador | concepto `PositionedPiece` |

### 6.2 Qué compra la plantilla de clase

- Un tablero para cualquier dimensión entre 1 y 16.
- Un motor para cualquier dimensión entre 1 y 16.
- Una comprobación de dimensión en tiempo de compilación, no en tiempo de ejecución.
- Un contrato de pieza nombrado, con un diagnóstico claro.
- Ninguna duplicación del bucle de turnos para 10×10.

### 6.3 Qué cuesta

- Una clase por lista de argumentos en el archivo objeto, o un conjunto de símbolos en `-O0`.
- El cuerpo debe ser visible en un encabezado; `board.cpp` desaparece y `game_engine.h` tiene 292
  líneas.
- El almacenamiento máximo está fijo en 16×16; un tablero mayor necesita un máximo nuevo y una
  recompilación.
- El tipo de pieza sigue siendo concreto; el concepto se comprueba una vez, no se usa como
  restricción.
- Las dimensiones son de tiempo de compilación; no puedes elegir 8×8 o 10×10 en tiempo de ejecución
  sin instanciar ambos y seleccionar uno.

### 6.4 La brecha: genérico sobre dimensiones, concreto sobre piezas

`grep -n 'Knight' code/game_engine.h` devuelve 38 líneas. Seis firmas de miembro nombran `Knight`;
los dos miembros de datos son `Knight`; el constructor crea dos objetos `Knight`. `game.cpp` tiene
cero menciones del tipo `Knight`; su única referencia es el `#include "knight.h"` en minúsculas, que
es redundante porque `game_engine.h` ya lo incluye. La plantilla de clase no eliminó el tipo de
pieza; lo movió.

El nivel completo parameterizaría también la pieza:

```cpp
template <int RowCount, int ColumnCount, PositionedPiece PieceType>
class GameEngine
{
    ...
    PieceType playerKnight;
    PieceType machineKnight;
};
```

Entonces `GameEngine<8, 8, Knight>` y `GameEngine<8, 8, ScoutingPawn>` serían dos motores desde un
solo código. Ese cambio también exigiría que las seis firmas usaran `PieceType`, una forma de
construir las piezas, y el concepto como restricción del parámetro de plantilla en lugar de un
`static_assert` sobre `Knight`. Esta línea base no hace eso. Dilo antes de afirmar que el motor es
completamente genérico.

### 6.5 Qué emite el compilador, otra vez

`nm -C game15.o` en `-O2` muestra un símbolo `GameEngine<8, 8>`, `playTurn`. En `-O0` muestra 24. En
la sonda con ambos tamaños, `nm` muestra 24 para cada uno en `-O0` y uno para cada uno en `-O2`.

`objdump -d game15.o | grep -c 'call.*\*'` imprime 0, como lo hacía en `construction-14`. La
plantilla de clase no añade llamadas indirectas ni tabla. La llamada se resuelve en tiempo de
compilación, como cualquier otra función miembro.

La afirmación honesta es estructural: los argumentos concretos se fijan cuando se compila el
programa, y el mismo código produce una clase distinta por lista de argumentos. Si los miembros se
inlinean, y si el tablero permanece en memoria, depende del optimizador. Mide antes de afirmar una
mejora de velocidad; esta versión hace el mismo trabajo por turno y no cambia ninguna regla.

## Paso 7 — Explicar (~8 min)

La comparación en una tabla:

| Pregunta | construction-14 | construction-15 |
|---|---|---|
| Qué se parameteriza | el tipo de pieza en una función | filas y columnas en dos clases |
| Qué es genérico | el generador | el tablero y el motor |
| Qué está restringido | implícito: `row()`, `column()`, `const` | nombrado: `PositionedPiece`; dimensiones 1..16 |
| Qué emite el compilador | una función por tipo | una clase por lista de argumentos |
| Despacho en ejecución | ninguno | ninguno |
| Tipo de pieza del bucle | `Knight` | `Knight` |
| Almacenamiento | 8×8 fijo | 16×16 fijo, el área activa varía |
| Trazas | 20 de 20 | 20 de 20 para 8×8 |

Escribe `EVIDENCE-es.md` §8. Dos partes.

**Primera, una frase.** Di qué comparten los dos motores y qué cambia, sin usar las palabras
*plantilla*, *genérico*, *parámetro de tipo*, *instanciación*, *compilador*, *mejor*, *peor*,
*limpio*, *flexible*, *reutilizable* o *elegante*. Si tu frase necesita una de ellas, has descrito
el mecanismo en lugar de la decisión.

**Segunda, un párrafo.** Responde:

- Qué fija la plantilla de clase: el tablero y el motor se escriben una vez; las dimensiones son
  argumentos.
- Qué impone las dimensiones: los dos `static_assert` de `board.h:72–73`.
- Qué impone el contrato de la pieza: `PositionedPiece` en `game_engine.h:14–20` y el
  `static_assert` de la línea 22.
- Qué cambia con las dimensiones: límites, número de casillas, posiciones iniciales, legalidad,
  transcripción.
- Qué no cambia con las dimensiones: almacenamiento (256 bytes), `sizeof(GameEngine)` (336 bytes),
  las reglas, la geometría, el tipo de pieza.
- Qué demuestran las 20 trazas: que el comportamiento de 8×8 no cambió.
- Qué no demuestran las trazas: que 10×10 sea correcto contra una traza de referencia; no hay
  ninguna.
- Qué demuestra la ejecución de 10×10: que el mismo código compila y se ejecuta en un segundo
  tamaño, con salida distinta.
- Qué no cubre el concepto: la semántica; un tipo puede satisfacer `PositionedPiece` y moverse mal.
- Qué no cubre el motor: el tipo de pieza sigue siendo `Knight`; el concepto es una comprobación,
  no una restricción de parámetro.
- Qué cuesta la prueba de cambio: un archivo nuevo, cero ediciones, ningún concepto nuevo.
- Qué lección posterior se ocupa del ciclo de vida: `construction-16`.
- Dónde se defiende un cambio así con números: el proyecto final, `construction-24`.

Una respuesta fuerte mantiene separadas cuatro cosas: **qué es genérico** (las dimensiones), **qué
está restringido** (las operaciones de la pieza y el rango de dimensiones), **qué emite el
compilador** (una clase por lista de argumentos) y **qué puede ver una ejecución** (la
transcripción). Una respuesta débil dice "la plantilla de clase hace que el código sea
reutilizable", que colapsa las cuatro en un eslogan.

## Paso 8 — Comprobación de aceptación y registro (~2 min)

El criterio de aceptación de esta lección es: **instanciar la misma plantilla de motor para 8×8 y
10×10; explicar qué es genérico, qué está restringido y qué cambia con las dimensiones.**

Comprueba tu registro contra él:

- [ ] Localicé `Board<RowCount, ColumnCount>` y `GameEngine<RowCount, ColumnCount>` por línea.
- [ ] Nombré los dos parámetros que no son de tipo y las cuatro operaciones del concepto.
- [ ] Encontré `static_assert(PositionedPiece<Knight>)` y los `static_assert` de dimensión.
- [ ] Confirmé que `GameEngine` no recibe un tipo de pieza como parámetro.
- [ ] Reproduje las 20 trazas en la línea base de 8×8 sin modificar.
- [ ] Compilé `GameEngine<10, 10>` desde el mismo encabezado y lo ejecuté.
- [ ] Vi que la salida de 10×10 difiere de la de 8×8.
- [ ] Medí el mismo `sizeof(Board)` y `sizeof(GameEngine)` para ambos tamaños.
- [ ] Vi las dos instanciaciones del motor por nombre en `-O0`.
- [ ] Reproduje el fallo de `static_assert` de dimensión para 17×8.
- [ ] Reproduje el fallo del concepto para un tipo sin las operaciones.
- [ ] Nombré qué no comprueba el concepto.
- [ ] Nombré el límite: el tipo de pieza sigue siendo `Knight`.
- [ ] No edité ningún archivo de código rastreado.
- [ ] Completé `EVIDENCE-es.md` §1–§10 y la autocomprobación de §12.

El criterio no es "el código usa plantillas". Es "puedes decir qué varía, qué se comprueba, qué
produjo el compilador y qué cambió la segunda ejecución".

## Autocomprobación (binaria)

- [ ] Leí `board.h`, `game_engine.h` y `game.cpp` y localicé ambas plantillas por línea.
- [ ] Comparé la función genérica con las plantillas de clase y nombré qué posee cada una.
- [ ] Predije la tabla de dimensiones, la prueba de cambio y los recuentos de instanciación antes de
      ejecutar.
- [ ] Reproduje las 20 trazas en la línea base de 8×8 sin modificar.
- [ ] Compilé y ejecuté `GameEngine<10, 10>` desde el mismo encabezado.
- [ ] Confirmé que la salida de 10×10 difiere de la de 8×8.
- [ ] Medí que ambos tipos de tablero miden 256 bytes y ambos motores 336 bytes.
- [ ] Vi las dos instanciaciones por nombre en `-O0`.
- [ ] Reproduje los fallos de dimensión y de concepto.
- [ ] Nombré qué no comprueba el concepto.
- [ ] Nombré el límite: el motor es genérico sobre dimensiones, no sobre piezas.
- [ ] Completé `EVIDENCE-es.md`.

Estos son criterios de aceptación. Si una casilla queda
sin marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §10.

## Misconcepciones que esta lección refuta

| Error | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| "Una plantilla de clase es una clase." | Es un patrón; el compilador escribe una clase por lista de argumentos. | Cuenta las instanciaciones con `nm` en `-O0`. |
| "`GameEngine<8,8>` y `GameEngine<10,10>` comparten un objeto en ejecución." | Son dos tipos distintos, generados en tiempo de compilación. | Busca dos conjuntos de símbolos; no hay objeto compartido. |
| "El tablero de 10×10 usa más almacenamiento." | El almacenamiento está fijo en 16×16; solo cambia el área activa. | Mide `sizeof` en ambos. |
| "Las dimensiones se pueden elegir en ejecución." | Son parámetros de plantilla que no son de tipo, fijados en compilación. | Intenta leerlas de `argv`; necesitas una rama o dos instanciaciones. |
| "Un concepto es una interfaz." | Es un predicado de tiempo de compilación; sin tabla virtual, sin objeto. | Busca llamadas indirectas; no hay. |
| "Un concepto comprueba la semántica." | Comprueba operaciones y tipos de retorno, no qué significan. | Un tipo puede satisfacer `PositionedPiece` y moverse mal. |
| "El motor es genérico sobre piezas." | Todavía declara miembros `Knight` y seis firmas `Knight`. | `grep -n 'Knight' game_engine.h`. |
| "`static_assert` es una comprobación en ejecución." | Se evalúa en tiempo de compilación. | Quítalo y la compilación falla; no hay rama en ejecución. |
| "Las 20 trazas demuestran que 10×10 funciona." | Solo cubren 8×8. No hay trazas de referencia para 10×10. | Compara solo 8×8; llama a la ejecución de 10×10 prueba de humo. |
| "La plantilla de clase hace el binario más pequeño." | El texto enlazado se reduce 86 bytes, pero `game.o` crece 479. | Mide ambas unidades y di cuál usas. |
| "El cuerpo puede vivir en un `.cpp`." | El compilador lo necesita en la instanciación; el enlace falla si no. | Muévelo y reproduce la referencia indefinida. |
| "`Board<8,8>` y `Board<10,10>` tienen el mismo diseño, así que son el mismo tipo." | Tienen tipos distintos; el diseño casualmente mide lo mismo aquí. | Mide `sizeof`; no confundas tamaño con identidad de tipo. |
| "Un concepto elimina la necesidad de pruebas." | Comprueba la forma, no el comportamiento. | Conserva las trazas y añade una prueba semántica. |
| "El motor es completamente genérico porque tiene un concepto." | El concepto se comprueba una vez, no se usa como restricción de parámetro. | Lee la lista de parámetros de plantilla en `game_engine.h:34`. |

## Siguiente paso

`construction-16` se ocupa del ciclo de vida. Las plantillas de clase de aquí guardan miembros por
valor: un tablero y dos caballos. No reservan memoria, así que no responden quién destruye qué.
`construction-16` plantea esa pregunta.

La parameterización completa de la pieza es el siguiente cambio a este diseño: añadir un parámetro
`PieceType` restringido por `PositionedPiece`, hacer que las seis firmas lo usen y decidir cómo se
construyen las piezas. `construction-24` es donde defiendes ese cambio con números. Esta lección se
detiene en las dimensiones, y lo dice.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog-es.md) para la ruta completa.

Si faltaba cualquiera de las dos líneas base, no empieces `construction-15`. El siguiente contraste
necesita ambos lados.
