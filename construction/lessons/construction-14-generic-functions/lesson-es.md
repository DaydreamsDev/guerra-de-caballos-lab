# Funciones genéricas (generic functions)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** parámetros de tipo · **Tiempo:** unos 68 minutos ·
**Prerrequisitos:** construction-12 o construction-13 · **Estado:** borrador
**Lectura:** ~24 minutos
**Rol de la lección:** tronco

## Por qué existe esta lección

`construction-13` terminó con un bucle que nombraba `Knight` en diez firmas de ayuda. Una de esas
ayudas, `tryMove`, también poseía la geometría del movimiento: dos arreglos constantes y dos líneas
que sumaban un desplazamiento a la casilla del caballo activo. El bucle podía mover un solo tipo de
pieza, y la regla para producir destinos vivía dentro de la lógica de turnos.

Esta línea base mueve la geometría a una plantilla de función. `move_generator.h` guarda los
desplazamientos y un generador:

```cpp
template <typename PieceType>
std::array<MoveCandidate, moveCandidateCount> generate_move_candidates(const PieceType &piece)
{
    std::array<MoveCandidate, moveCandidateCount> candidates = {};
    for (int index = 0; index < moveCandidateCount; index += 1)
    {
        candidates[index] = {
            index + 1,
            piece.row() + knightRowChangePerMove[index],
            piece.column() + knightColumnChangePerMove[index],
        };
    }
    return candidates;
}
```

`game.cpp` la llama una vez por turno con un `Knight`. `tryMove` ya no recibe un número de
movimiento ni el caballo activo; recibe un candidato que ya lleva un destino.

La frase que hay que retener:

> Una plantilla de función es un patrón para una función: el compilador escribe una función por cada
> tipo que la usa.

Tres notas honestas antes de la comparación.

1. El código crece 31 líneas, de 556 en once archivos a 587 en doce. `game.cpp` crece una línea, de
   280 a 281, y el encabezado nuevo tiene 30 líneas. La lógica de turnos pierde la aritmética de
   desplazamientos pero gana una búsqueda de candidato, así que el archivo no se reduce.
2. La plantilla hace más aritmética por turno, no menos. `construction-13` calculaba un par de
   destinos por intento; esta versión calcula ocho candidatos al inicio de cada turno. La semilla 1
   tiene 22 turnos y 36 intentos: 176 candidatos frente a 36 pares. En las veinte trazas son 4.168
   candidatos frente a 1.017 pares. La ganancia es separación y reutilización, no velocidad.
3. Las 20 trazas siguen pasando. La transcripción registra el juego, no cómo se produjo un destino.

La comparación con `construction-13` no es "las plantillas son mejores". Es más precisa: la
plantilla compra un generador para cualquier tipo con `row()` y `column()`, y la paga con una
instanciación por tipo, un cuerpo que debe permanecer visible en un encabezado y mensajes de error
que solo aparecen cuando se usa un tipo. El bucle sigue nombrando `Knight` en todo lo demás. Esta
lección mide ambos lados.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar parámetros de tipo en una función;
- usar un generador de movimientos para dos tipos de pieza.

## El concepto: una función con un parámetro de tipo

`construction-13` preguntó: ¿puede una pieza funcionar sin clase base? La respuesta fue sí, si la
posición se convierte en un miembro. El bucle entonces nombraba `Knight` en cada firma de ayuda, y
la geometría del movimiento se quedaba en `tryMove`.

Esta lección hace otra pregunta: ¿puede escribirse la geometría una vez para más de un tipo de
pieza, sin clase base y sin copiar el bucle?

Sí, si el tipo concreto se convierte en un parámetro de la función. `generate_move_candidates` no
nombra `Knight`. Nombra `PieceType`, un marcador de posición que el compilador sustituye por el tipo
en cada llamada:

```cpp
template <typename PieceType>
std::array<MoveCandidate, moveCandidateCount> generate_move_candidates(const PieceType &piece);
```

Una plantilla de función no es una función. Es un patrón. El compilador produce una función cuando
ve una llamada con un tipo concreto. Ese acto se llama **instanciación**. Una llamada con `Knight`
produce `generate_move_candidates<Knight>`; una llamada con `ScoutingPawn` produce
`generate_move_candidates<ScoutingPawn>`. Ambas salen de las mismas 14 líneas.

| | construction-13 | construction-14 |
|---|---|---|
| Dónde viven los desplazamientos | `game.cpp:18–19` | `move_generator.h:7–8` |
| Quién calcula un destino | `tryMove`, una vez por intento | `generate_move_candidates`, una vez por turno |
| Qué recibe `tryMove` | caballo activo y número de movimiento | un `MoveCandidate` |
| Tipos que acepta la geometría | solo `Knight` | cualquier tipo con `row()` y `column()` |
| Cuándo se fija el tipo concreto | al compilar el bucle | en cada instanciación |
| Despacho en tiempo de ejecución | ninguno | ninguno |
| Copias de la geometría | una, en `game.cpp` | una por tipo instanciado |
| El tipo del bucle | `Knight` | `Knight` |

### El contrato es estructural, no declarado

La plantilla no incluye `knight.h`. No hereda de una interfaz. Solo usa dos expresiones:

```cpp
piece.row() + knightRowChangePerMove[index]
piece.column() + knightColumnChangePerMove[index]
```

Cualquier tipo cuyo `row()` y `column()` puedan llamarse sobre un objeto `const` y cuyos resultados
puedan sumarse a un `int` satisface el cuerpo. C++ acepta el tipo mirando las operaciones que usa.
A esto se le llama a veces **tipado por pato** (*duck typing*), y es la razón por la que el mismo
generador sirve para un caballo y para un tipo de prueba sin una base común.

El contrato tiene dos partes que el código no declara explícitamente:

- las operaciones deben existir: `row()` y `column()`;
- deben poder llamarse a través de `const PieceType &`, así que ambos accesores deben ser funciones
  miembro `const` (o funciones libres que acepten una referencia constante).

Si falta cualquiera de las dos partes, el error aparece en la instanciación, no en la definición de
la plantilla. Eso es un costo, y `construction-15` lo aborda con conceptos.

### El generador produce candidatos, no movimientos legales

`generate_move_candidates` suma ocho desplazamientos fijos a la casilla de la pieza. No conoce el
tablero, ni el otro caballo, ni las casillas visitadas. Una pieza en `(1, 1)` produce destinos en la
fila `0` y la columna `-1`; esos son candidatos, no movimientos. `tryMove` sigue decidiendo:

```cpp
if (destinationRow < 1 || destinationRow > board.row_count() ||
    destinationColumn < 1 || destinationColumn > board.column_count())
{
    return MoveOutcome::outsideBoard;
}
```

La separación es el punto. El generador posee la geometría; el bucle posee las reglas. Ninguno sabe
más de lo que necesita.

### `MoveCandidate` es el registro nuevo

El generador devuelve un arreglo de tamaño fijo de registros:

```cpp
struct MoveCandidate
{
    int moveNumber;
    int destinationRow;
    int destinationColumn;
};
```

El registro ocupa 12 bytes en esta plataforma y el arreglo 96 bytes. `playTurn` lo construye una vez
y lo indexa por número de movimiento:

```cpp
auto moveCandidates = generate_move_candidates(*activeKnight);
...
const MoveCandidate &candidate = moveCandidates[attemptedMoveNumber - 1];
```

La versión anterior pasaba `attemptedMoveNumber` a `tryMove` y allí indexaba los arreglos de
desplazamientos. La versión nueva mueve la aritmética del índice al generador y pasa el candidato ya
terminado.

## Antes de empezar

- [`construction-13`](../construction-13-composition/lesson-es.md) — la línea base de la que parte
  esta lección. Su `game.cpp` es el lado izquierdo del diff del Paso 5.
- [`construction-12`](../construction-12-polymorphism/lesson-es.md) — el prerrequisito del catálogo
  y el otro diseño anterior. Si hiciste `construction-12` pero omitiste `construction-13`, lee la
  sección "Qué cuesta la composición" de la lección de composición antes del diff; la prueba de
  cambio supone la línea base de composición.
- [`construction-11`](../construction-11-encapsulation/lesson-es.md) — la frontera de clase.
  `Position` y `Knight` mantienen estado privado y operaciones públicas.
- [`construction-04`](../construction-04-records/lesson-es.md) — registros. `MoveCandidate` es un
  registro con tres campos públicos.
- [`construction-03`](../construction-03-functions/lesson-es.md) — declaraciones, parámetros y
  tipos de retorno. Una declaración de plantilla se lee igual, con un parámetro extra.
- Un compilador de C++20 (`g++` o `clang++`). `cmake` es opcional; `run.sh` recurre a una
  compilación directa cuando CMake no está.

No necesitas saber cómo instancia una plantilla el compilador. Necesitas leer una plantilla,
llamarla con dos tipos y comparar el programa generado con la línea base anterior.

### Qué necesitas

- La línea base de funciones genéricas:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/move_generator.h`](code/move_generator.h),
  [`code/knight.h`](code/knight.h), [`code/knight.cpp`](code/knight.cpp),
  [`code/position.h`](code/position.h), [`code/position.cpp`](code/position.cpp),
  [`code/board.h`](code/board.h), [`code/board.cpp`](code/board.cpp),
  [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- La línea base anterior, para el diff:
  [`code/game.cpp`](../construction-13-composition/code/game.cpp) y sus diez compañeros en
  [`../construction-13-composition/code/`](../construction-13-composition/code/).
- Guiones de compilación y notas: [`run.sh`](run.sh), [`CMakeLists.txt`](CMakeLists.txt) y
  [`README-es.md`](README-es.md).
- Las reglas: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §1 fija el tablero
  y su numeración desde 1, §4 fija el orden de movimiento del caballo, §8 fija cuándo una casilla
  pasa a visitada.
- El formato de transcripción:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- Una traza de referencia: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- Esta plantilla del aprendiz: [`EVIDENCE-es.md`](EVIDENCE-es.md).

Comprueba que los artefactos están presentes antes de empezar, desde este directorio de lección:

```bash
test -f code/main.cpp &&
test -f code/game.h &&
test -f code/game.cpp &&
test -f code/move_generator.h &&
test -f code/knight.h &&
test -f code/knight.cpp &&
test -f code/position.h &&
test -f code/position.cpp &&
test -f code/board.h &&
test -f code/board.cpp &&
test -f code/random_sequence.h &&
test -f code/random_sequence.cpp &&
test -f ../construction-13-composition/code/game.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

Si falta un archivo, la comparación no tiene segundo lado. Registra el archivo faltante en
`EVIDENCE-es.md` §11 y vuelve cuando esté disponible. Un recuento contra un archivo que no has leído
no es evidencia.

La línea base reproduce las 20 trazas de `common/traces/` byte a byte, antes de que cambies nada.
Ese es el contrato de comportamiento, y no dice nada sobre cómo se calcula un destino.

## Paso 1 — Inventariar el generador (~10 min)

Lee primero el encabezado nuevo. La forma es la lección:

```bash
grep -n '^' code/move_generator.h
```

```text
1:#pragma once
2:
3:#include <array>
4:
5:constexpr int moveCandidateCount = 8;
6:
7:constexpr int knightRowChangePerMove[moveCandidateCount] = {-1, -2, -2, -1, 1, 2, 2, 1};
8:constexpr int knightColumnChangePerMove[moveCandidateCount] = {2, 1, -1, -2, -2, -1, 1, 2};
9:
10:struct MoveCandidate
11:{
12:    int moveNumber;
13:    int destinationRow;
14:    int destinationColumn;
15:};
16:
17:template <typename PieceType>
18:std::array<MoveCandidate, moveCandidateCount> generate_move_candidates(const PieceType &piece)
19:{
20:    std::array<MoveCandidate, moveCandidateCount> candidates = {};
21:    for (int index = 0; index < moveCandidateCount; index += 1)
22:    {
23:        candidates[index] = {
24:            index + 1,
25:            piece.row() + knightRowChangePerMove[index],
26:            piece.column() + knightColumnChangePerMove[index],
27:        };
28:    }
29:    return candidates;
30:}
```

Ahora el mapa de líneas. Rellénalo en `EVIDENCE-es.md` §2.2 desde el código, no desde esta tabla:

| Líneas | Contenido |
|---|---|
| 5 | `moveCandidateCount`, el número de candidatos y el límite del arreglo. |
| 7–8 | Los ocho desplazamientos de fila y columna, en orden de movimiento. |
| 10–15 | `MoveCandidate`, un registro con tres campos públicos. |
| 17–18 | La cabecera de la plantilla y la firma de la función. |
| 20 | El arreglo de candidatos, inicializado a valor. |
| 21–28 | El bucle que llena las ocho entradas. |
| 23–27 | Un candidato: número de movimiento, fila de destino, columna de destino. |
| 25–26 | Las dos únicas operaciones que la plantilla exige a `PieceType`. |
| 29 | El arreglo se devuelve por valor. |

Recuentos de esta línea base: 6 encabezados, 6 unidades de traducción, 22 declaraciones públicas, 9
declaraciones de datos privados, 4 definiciones de clase completas, 0 tipos incompletos, 39
definiciones de función, 17 ayudas internas en `game.cpp`, 1 ayuda interna en `main.cpp`, 1 punto de
entrada.

Compara con `construction-13`: 5 encabezados, 6 unidades de traducción, 19 declaraciones públicas, 9
declaraciones de datos privados, 3 clases completas, 38 definiciones. El encabezado nuevo añade tres
declaraciones de datos públicos (`moveNumber`, `destinationRow`, `destinationColumn`), una
definición de clase completa (`MoveCandidate`) y una definición de función (la plantilla). No añade
ninguna unidad de traducción, porque la plantilla vive solo en el encabezado.

Después responde en `EVIDENCE-es.md` §2:

1. ¿Qué archivo contiene la plantilla y en qué líneas?
2. ¿Cuántos parámetros de tipo declara y cómo se llama el parámetro?
3. ¿Qué operaciones exige el cuerpo a `PieceType` y en qué líneas?
4. ¿Incluye `move_generator.h` a `knight.h` o menciona `Knight` en algún lugar?
5. ¿Cuántas veces llama `game.cpp` a `generate_move_candidates`?
6. ¿Qué recibe `tryMove` ahora y qué recibía en `construction-13`?
7. ¿Dónde están los arreglos de desplazamientos y cuántas entradas tiene cada uno?
8. ¿Cuántos miembros de datos públicos tiene `MoveCandidate`? ¿Son privados?
9. ¿Cuántas instanciaciones crea el juego? ¿Qué tipo se usa?
10. ¿Qué pasaría si `Knight::row()` no fuera `const`?

La pregunta 4 tiene una respuesta precisa: no. `grep -n 'Knight' code/move_generator.h` no imprime
nada. Esa es la afirmación estructural de esta línea base: el generador no conoce el tipo de pieza.

La pregunta 5 tiene una respuesta precisa: una vez, en `game.cpp:184`. Las ocho lecturas de `row()`
y `column()` ocurren dentro de la función instanciada, no en el código del bucle.

La pregunta 9 tiene una respuesta precisa: una, para `Knight`. `game.cpp` llama al generador una vez
y solo con ese tipo, así que el juego contiene una instanciación.

La pregunta 10 tiene una respuesta precisa: el programa deja de compilar en la instanciación con un
error como `passing 'const Knight' as 'this' argument discards qualifiers`. La plantilla toma
`const PieceType &`, así que un accesor no constante no puede llamarse a través de ella.

## Paso 2 — Predecir antes de ejecutar (~10 min)

### 2.1 La tabla de candidatos

Escribe estas predicciones antes de ejecutar nada. El caballo está en `(4, 5)`. El tipo de prueba,
`ScoutingPawn`, está en `(1, 1)` a propósito: sus candidatos incluyen casillas fuera del tablero, lo
que muestra qué decide y qué no decide el generador.

| Movimiento | `Knight` en (4, 5) | `ScoutingPawn` en (1, 1) | Confianza |
|---|---|---|---|
| 1 | | | |
| 2 | | | |
| 3 | | | |
| 4 | | | |
| 5 | | | |
| 6 | | | |
| 7 | | | |
| 8 | | | |

Las últimas cuatro filas son las que hay que tomar en serio. Un peón en la esquina produce
coordenadas negativas. Predice si el generador las rechaza y dónde ocurre el rechazo si no lo hace.

### 2.2 La prueba de cambio

El cambio es el que produjo esta línea base: parte de `construction-13` y mueve la geometría a una
plantilla.

| Pregunta | Mi predicción |
|---|---|
| Archivos que añadiré | |
| Archivos que editaré | |
| Líneas del encabezado nuevo | |
| Líneas que añadiré en `game.cpp` | |
| Líneas que quitaré en `game.cpp` | |
| Tramos del diff de `game.cpp` | |
| Funciones de `game.cpp` cuya firma cambia | |
| Líneas de lógica que cambiaré (no nombres) | |
| Semillas que coinciden, de 20, tras el cambio | |
| Conceptos nuevos que exige el cambio | |

La octava fila es la aceptación. Predice cero antes de medirlo. Una refactorización que cambia una
regla es otra lección.

### 2.3 Instanciación y generación de código

| Pregunta | Mi predicción |
|---|---|
| Instanciaciones del generador en el juego | |
| Símbolos de `generate_move_candidates` en `game14.o`, con `-O2` | |
| El mismo recuento con `-O0` | |
| Texto de `game.o`: ¿crece, se reduce o igual? | |
| Llamadas indirectas en `game14.o` | |
| `sizeof(MoveCandidate)` | |
| `sizeof(std::array<MoveCandidate, 8>)` | |
| Candidatos calculados en la semilla 1, frente a pares de destino en `construction-13` | |

La segunda y la tercera fila son la prueba del mecanismo. Una plantilla que se inserta en línea no
tiene símbolo; una plantilla que no se inserta tiene un símbolo por tipo. Predice ambos recuentos
antes de compilar nada.

## Paso 3 — Ejecutar la línea base y la sonda de instanciación (~10 min)

Primero las trazas, sin modificar:

```bash
mkdir -p /tmp/generic-test
./run.sh --seed 1 --name Learner > /tmp/generic-test/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/generic-test/baseline-0001.txt
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
  -o build/generic-baseline code/main.cpp code/game.cpp code/board.cpp \
  code/knight.cpp code/position.cpp code/random_sequence.cpp
```

Ahora la sonda de instanciación. Va en el directorio temporal, nunca en los archivos versionados:

```bash
mkdir -p /tmp/generic-test/probe
cat > /tmp/generic-test/probe/instantiation.cpp <<'EOF'
#include "knight.h"
#include "move_generator.h"

#include <cstdio>

struct ScoutingPawn
{
    int rowValue;
    int columnValue;

    int row() const { return rowValue; }
    int column() const { return columnValue; }
};

int main()
{
    Knight knight("player");
    knight.set_position(4, 5);
    ScoutingPawn pawn{1, 1};

    auto knightCandidates = generate_move_candidates(knight);
    auto pawnCandidates = generate_move_candidates(pawn);

    for (int index = 0; index < moveCandidateCount; index += 1)
    {
        std::printf("%d knight=%d,%d pawn=%d,%d\n", knightCandidates[index].moveNumber,
                    knightCandidates[index].destinationRow,
                    knightCandidates[index].destinationColumn,
                    pawnCandidates[index].destinationRow,
                    pawnCandidates[index].destinationColumn);
    }

    std::printf("sizeof(MoveCandidate)=%zu sizeof(array)=%zu\n",
                sizeof(MoveCandidate), sizeof(knightCandidates));
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/generic-test/probe/instantiation \
  /tmp/generic-test/probe/instantiation.cpp code/knight.cpp code/position.cpp
/tmp/generic-test/probe/instantiation
```

La salida en esta plataforma:

```text
1 knight=3,7 pawn=0,3
2 knight=2,6 pawn=-1,2
3 knight=2,4 pawn=-1,0
4 knight=3,3 pawn=0,-1
5 knight=5,3 pawn=2,-1
6 knight=6,4 pawn=3,0
7 knight=6,6 pawn=3,2
8 knight=5,7 pawn=2,3
sizeof(MoveCandidate)=12 sizeof(array)=96
```

Lee la columna del peón contra el tablero. El peón en `(1, 1)` produjo `(0, 3)`, `(-1, 2)`,
`(-1, 0)` y `(0, -1)`. El generador no las rechazó. Sumó los desplazamientos y devolvió. Eso es
correcto: la validación pertenece a `tryMove`, que conoce el tablero.

Ambas columnas salieron de la misma plantilla. Una llamada se instanció para `Knight`, otra para
`ScoutingPawn`. Para ver las dos instanciaciones por nombre, compila la sonda sin optimización:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O0 -Icode \
  -c /tmp/generic-test/probe/instantiation.cpp \
  -o /tmp/generic-test/probe/instantiation-O0.o
nm -C /tmp/generic-test/probe/instantiation-O0.o | grep 'generate_move_candidates'
```

```text
0000000000000000 W std::array<MoveCandidate, 8ul> generate_move_candidates<ScoutingPawn>(
    ScoutingPawn const&)
0000000000000000 W std::array<MoveCandidate, 8ul> generate_move_candidates<Knight>(
    Knight const&)
```

Dos símbolos, ambos débiles, ambos del mismo cuerpo de 14 líneas. La `W` significa que el enlazador
puede fusionar copias duplicadas de cada instanciación entre unidades de traducción. No hay tabla,
ni clase base, ni tipo en tiempo de ejecución.

Si el primer diff no está vacío, detente. Esa es una línea base distinta de la que describe esta
lección; regístrala en `EVIDENCE-es.md` §11 y no ajustes la predicción para que coincida.

## Paso 4 — Comparar: qué demuestra la plantilla (~8 min)

Abre `move_generator.h`, `tryMove` y `playTurn`. Cinco hechos, leídos contra las dos líneas base.

1. **Una definición, dos instanciaciones.** Con `-O0`, `nm` muestra
   `generate_move_candidates<Knight>` y `generate_move_candidates<ScoutingPawn>`. Con `-O2`, la
   misma sonda no tiene ese símbolo, porque el compilador insertó ambas instanciaciones en línea. El
   código tiene un generador; el programa tiene una función por tipo, o cero después de la
   inserción.
2. **El bucle ya no calcula desplazamientos.** `tryMove`, en `game.cpp:147–168`, lee
   `candidate.destinationRow` y `candidate.destinationColumn`. Los dos arreglos de desplazamientos
   ya no están en `game.cpp`; están en `move_generator.h:7–8`.
3. **`tryMove` cambió su entrada.** Recibe `const MoveCandidate &candidate` en lugar de
   `const Knight *activeKnight` e `int moveNumber`. `playTurn` construye el arreglo una vez en la
   línea 184 y lo indexa en la línea 192.
4. **Las 20 trazas son idénticas.** El generador produce los mismos ocho destinos que producía la
   fórmula anterior, en el mismo orden. La transcripción de la semilla 1 tiene 89 líneas, con 22
   turnos y 36 intentos, y coincide byte a byte con `construction-13`.
5. **El código objeto crece y los datos no.** El texto de `game.o` pasa de 3.817 a 3.913 bytes
   (+96), y el texto enlazado de 9.400 a 9.496 (+96). Los datos siguen en 692 bytes y las llamadas
   indirectas siguen en 0 en ambos lados. Los +96 bytes son el arreglo y el manejo del candidato, no
   una tabla.

El recuento de candidatos es el costo honesto. En la semilla 1, `construction-13` calculó 36 pares
de destino, uno por intento. Esta versión calcula 22 × 8 = 176 candidatos, porque llena el arreglo
antes del primer intento. En las veinte trazas: 521 turnos × 8 = 4.168 candidatos frente a 1.017
pares. La plantilla hace más trabajo por turno. No hace el juego más rápido.

### 4.1 Qué emite el compilador

Compila `game.cpp` solo en ambos lados:

```bash
g++ -std=c++20 -O2 -Icode -c code/game.cpp -o /tmp/generic-test/game14.o
g++ -std=c++20 -O2 -I../construction-13-composition/code \
  -c ../construction-13-composition/code/game.cpp -o /tmp/generic-test/game13.o
size /tmp/generic-test/game13.o /tmp/generic-test/game14.o
nm -C /tmp/generic-test/game14.o | grep -c 'generate_move_candidates'
objdump -d /tmp/generic-test/game14.o | grep -c 'call.*\*'
```

Con este compilador y `-O2`, el recuento de `nm` es 0 y el de llamadas indirectas es 0. Con `-O0`,
el mismo `game.cpp` emite un símbolo débil, `generate_move_candidates<Knight>`. Registra ambos en
`EVIDENCE-es.md` §5.2.

La ausencia de símbolo con `-O2` no es una propiedad de las plantillas. Es una decisión de
optimización. Un cuerpo más grande, una compilación de depuración o otro compilador pueden dejar la
función fuera de línea. La afirmación estructural es más estrecha y verdadera en ambos casos: el
tipo concreto se fija al compilar y no hay despacho en tiempo de ejecución que resolver.

### 4.2 Tres modos de fallo que vale la pena ver

El contrato de la plantilla es invisible hasta que se usa un tipo. Mira el primer fallo en el
directorio temporal:

```bash
cat > /tmp/generic-test/probe/missing_operations.cpp <<'EOF'
#include "move_generator.h"

struct IncompletePawn
{
};

int main()
{
    IncompletePawn pawn;
    auto candidates = generate_move_candidates(pawn);
    return candidates[0].moveNumber;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode -c \
  /tmp/generic-test/probe/missing_operations.cpp \
  -o /tmp/generic-test/probe/missing_operations.o
```

El compilador informa, en `move_generator.h`:

```text
error: 'const struct IncompletePawn' has no member named 'row'
error: 'const struct IncompletePawn' has no member named 'column'
```

También informa una cascada en la línea 23, porque las expresiones fallidas no pueden inicializar un
`MoveCandidate`. Los dos primeros errores son los útiles. El tercero es ruido que una restricción
eliminaría.

El segundo fallo es sobre `const`. Cambia `ScoutingPawn::row()` y `column()` a miembros no
constantes y compila de nuevo:

```text
error: passing 'const MutablePawn' as 'this' argument discards qualifiers
```

La plantilla toma `const PieceType &`, así que ambos accesores deben ser `const`. Ese requisito es
parte del contrato, y también es invisible en la declaración.

El tercer fallo es sobre visibilidad. Mueve el cuerpo de la plantilla a un archivo `.cpp`, deja solo
una declaración en el encabezado y enlaza:

```bash
mkdir -p /tmp/generic-test/split
cat > /tmp/generic-test/split/split_generator.h <<'EOF'
#pragma once

#include "move_generator.h"

template <typename PieceType>
std::array<MoveCandidate, moveCandidateCount> split_generate(const PieceType &piece);
EOF
cat > /tmp/generic-test/split/split_generator.cpp <<'EOF'
#include "split_generator.h"

template <typename PieceType>
std::array<MoveCandidate, moveCandidateCount> split_generate(const PieceType &piece)
{
    return generate_move_candidates(piece);
}
EOF
cat > /tmp/generic-test/split/main.cpp <<'EOF'
#include "split_generator.h"

struct ScoutingPawn
{
    int row() const { return 4; }
    int column() const { return 5; }
};

int main()
{
    ScoutingPawn pawn;
    return split_generate(pawn)[0].moveNumber;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode -I/tmp/generic-test/split \
  -o /tmp/generic-test/split/split \
  /tmp/generic-test/split/main.cpp /tmp/generic-test/split/split_generator.cpp
```

```text
undefined reference to `std::array<MoveCandidate, 8ul> split_generate<ScoutingPawn>(
    ScoutingPawn const&)'
```

El compilador debe ver el cuerpo en el punto de instanciación. Por eso `move_generator.h` contiene
el bucle y no solo una declaración. Una plantilla no es una función compilada por separado.

El límite de la evidencia: la sonda y las trazas muestran que un generador sirve para dos tipos. No
muestran que el bucle de turnos acepte un segundo tipo de pieza. Sigue nombrando `Knight`. Solo el
generador es genérico.

Registra en `EVIDENCE-es.md` §5 los cinco hechos, los recuentos de candidatos, los números de tamaño
del código y los tres modos de fallo.

## Paso 5 — Ejecutar la prueba de cambio (~15 min)

La prueba de cambio es la refactorización que produjo esta línea base. Parte de `construction-13` y
mueve la geometría a una plantilla. Los archivos versionados no se tocan.

```bash
lessonDirectory="$(pwd)"
rm -rf /tmp/generic-test/change
cp -r ../construction-13-composition/code /tmp/generic-test/change
cd /tmp/generic-test/change
```

### 5.1 Añadir el generador

```bash
cat > move_generator.h <<'EOF'
#pragma once

#include <array>

constexpr int moveCandidateCount = 8;

constexpr int knightRowChangePerMove[moveCandidateCount] = {-1, -2, -2, -1, 1, 2, 2, 1};
constexpr int knightColumnChangePerMove[moveCandidateCount] = {2, 1, -1, -2, -2, -1, 1, 2};

struct MoveCandidate
{
    int moveNumber;
    int destinationRow;
    int destinationColumn;
};

template <typename PieceType>
std::array<MoveCandidate, moveCandidateCount> generate_move_candidates(const PieceType &piece)
{
    std::array<MoveCandidate, moveCandidateCount> candidates = {};
    for (int index = 0; index < moveCandidateCount; index += 1)
    {
        candidates[index] = {
            index + 1,
            piece.row() + knightRowChangePerMove[index],
            piece.column() + knightColumnChangePerMove[index],
        };
    }
    return candidates;
}
EOF
```

Son 30 líneas en un archivo nuevo. El cuerpo debe estar aquí, no en un `.cpp`, porque cada llamada
lo instancia.

### 5.2 Quitar la geometría en línea del bucle

Edita `game.cpp` en tres lugares. Primero, incluye el encabezado nuevo:

```cpp
#include "board.h"
#include "knight.h"
#include "move_generator.h"
#include "random_sequence.h"
```

Segundo, borra los dos arreglos de desplazamientos del espacio de nombres anónimo. Ahora viven en el
encabezado.

Tercero, cambia `tryMove` y su llamador:

```cpp
MoveOutcome tryMove(const Board &board, const Knight *otherKnight,
                    const MoveCandidate &candidate, int &destinationRow,
                    int &destinationColumn)
{
    destinationRow = candidate.destinationRow;
    destinationColumn = candidate.destinationColumn;
    ...
}

void playTurn(...)
{
    ...
    printTurnHeader(successfulMoveCount + 1, activeKnight, moveOrder);
    auto moveCandidates = generate_move_candidates(*activeKnight);

    bool moveWasAccepted = false;
    for (...)
    {
        int attemptedMoveNumber = moveOrder[attemptIndex];
        const MoveCandidate &candidate = moveCandidates[attemptedMoveNumber - 1];
        ...
        MoveOutcome outcome = tryMove(board, otherKnight, candidate, destinationRow,
                                      destinationColumn);
        ...
    }
}
```

El diff contra `construction-13` tiene 5 tramos, 10 líneas añadidas y 8 eliminadas. Cada línea
cambiada es la inclusión, los arreglos eliminados, la firma de `tryMove` o la búsqueda del
candidato. No cambió ningún `if`, ningún límite de bucle, ninguna regla ni ninguna cadena de salida.

### 5.3 Compilar y ejecutar las 20 trazas

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o generic-change main.cpp game.cpp board.cpp knight.cpp position.cpp \
  random_sequence.cpp
cd "${lessonDirectory}"
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/generic-test/change/generic-change --seed "${seedNumber}" \
    --name Learner | cmp - "${trace}"
done
```

Un bucle silencioso es 20 de 20. La transcripción es idéntica porque el generador produce los mismos
destinos en el mismo orden.

### 5.4 Contar lo que se movió

| | construction-13 | construction-14 | Diferencia |
|---|---|---|---|
| Archivos | 11 | 12 | +1 |
| Líneas de código | 556 | 587 | +31 |
| Líneas de `game.cpp` | 280 | 281 | +1 |
| Declaraciones públicas | 19 | 22 | +3 |
| Declaraciones de datos privados | 9 | 9 | 0 |
| Clases completas | 3 | 4 | +1 |
| Definiciones de función | 38 | 39 | +1 |
| Unidades de traducción | 6 | 6 | 0 |
| Funciones del generador instanciadas | 0 | 1 en el juego | +1 |
| `sizeof` de la pieza | 40 | 40 | 0 |
| `sizeof(MoveCandidate)` | — | 12 | nuevo |
| Arreglo de candidatos por turno | — | 96 bytes | nuevo |
| Texto de `game.o` | 3.817 | 3.913 | +96 |
| Texto / datos del binario | 9.400 / 692 | 9.496 / 692 | +96 / 0 |
| Llamadas indirectas en `game.o` | 0 | 0 | 0 |
| Trazas | 20 de 20 | 20 de 20 | 0 |

El cambio no gana en todos los ejes. El código crece 31 líneas, `game.o` crece 96 bytes y el bucle
de turnos calcula ocho candidatos donde antes calculaba un par por intento. El generador se escribe
una vez y acepta cualquier tipo con los dos accesores, y la geometría ya no vive dentro del
verificador de reglas.

### 5.5 El hueco: solo el generador es genérico

En `construction-13`, añadir un segundo tipo de pieza significaba editar diez firmas de ayuda o
copiar el bucle. Esta línea base arregla una parte de ese problema: el generador de candidatos
acepta ambos tipos. No arregla el resto. `tryMove`, `printMove`, `playTurn` y `playGame` siguen
nombrando `Knight`, y `run_game` sigue construyendo dos objetos `Knight`.

Para mover un `ScoutingPawn` por el bucle de turnos, todavía necesitarías una de estas opciones:

- **Duplicar el bucle** para el segundo tipo. Eso duplica diez funciones y la lógica de turnos.
- **Restaurar una interfaz.** Eso es `construction-12` otra vez, con su tabla y su indirección.
- **Parametrizar también el bucle.** Eso es la segunda mitad de esta idea, y es `construction-15`,
  donde el parámetro de tipo llega al juego y un concepto declara el contrato.

Mide la primera opción con honestidad en `EVIDENCE-es.md` §6.5: diez firmas de ayuda más `run_game`
necesitarían una segunda copia o un parámetro de tipo. La segunda opción es la lección de
polimorfismo. La tercera es la siguiente.

La aceptación de esta lección es, por tanto, precisa: **un generador de movimientos para dos tipos
de pieza; el bucle del juego y las 20 trazas sin cambios.** No afirma que el bucle pueda mover un
segundo tipo de pieza. Solo el generador puede, y ese es el alcance medido de este diseño.

"Sin cambios" significa decisiones y salida sin cambios, no un archivo idéntico byte a byte.
`game.cpp` gana una línea, y el diff es de 10 líneas añadidas y 8 eliminadas. No cambió ninguna
regla ni ninguna cadena de salida.

## Paso 6 — La plantilla en el resto del programa (~8 min)

### 6.1 El contrato se comprueba en la llamada, no en la declaración

Una declaración de plantilla casi no dice nada sobre sus requisitos. `move_generator.h` declara un
parámetro de tipo y una referencia constante; los requisitos están enterrados en el cuerpo, en las
líneas 25–26. El compilador los comprueba cuando instancia la función, y reporta el error en el
encabezado, no en el lugar de la llamada. Por eso el primer error de §4.2 apunta a
`move_generator.h:25`, con el lugar de la llamada mostrado como nota.

`construction-15` reemplaza este contrato implícito por un concepto con nombre. El cuerpo seguirá
usando `row()` y `column()`, pero la declaración lo dirá, y un tipo incorrecto fallará en la llamada
con un mensaje claro.

### 6.2 Una instanciación por tipo y un arreglo de 96 bytes por turno

El juego instancia el generador una vez, para `Knight`. Un segundo tipo añade una segunda
instanciación. Cada instanciación es una función completa con su propio bucle. No hay objeto
compartido en tiempo de ejecución ni tabla.

El arreglo devuelto es un valor: ocho registros `MoveCandidate`, 96 bytes, construidos en la pila de
`playTurn` cada turno. El compilador puede mantenerlo en registros o escalarizarlo con `-O2`, pero
la semántica del código es un temporal de 96 bytes. Ese es el precio de devolver una lista completa
de candidatos en lugar de calcular un destino bajo demanda.

No es una razón para evitar la plantilla. Es una razón para medir antes de afirmar que parametrizar
sale gratis. Aquí el arreglo es pequeño y el juego está dominado por `printf`; el costo es invisible
en la transcripción y visible en `game.o` como +96 bytes.

### 6.3 Por qué el cuerpo vive en el encabezado

Una función normal se compila una vez en un `.cpp` y se declara en un encabezado. Una plantilla es
distinta: el compilador necesita el cuerpo para generar la función de cada tipo. Si el cuerpo se
esconde en un `.cpp`, la llamada compila y el enlace falla con una referencia indefinida, como
muestra §4.2.

Eso tiene dos consecuencias:

- `move_generator.h` es un archivo de código en la práctica. Cada unidad de traducción que lo
  incluye paga el análisis del cuerpo.
- El encabezado debe ser autosuficiente. Incluye `<array>` y usa `MoveCandidate` y los arreglos de
  desplazamientos; no incluye `knight.h`, porque no lo necesita.

Los arreglos de desplazamientos son `constexpr` en el espacio de nombres, así que cada unidad de
traducción que incluye el encabezado obtiene su propia copia. En este programa solo `game.cpp` lo
incluye, así que hay una copia en el binario. Si varias unidades lo incluyeran, las copias serían
datos de solo lectura duplicados, no un error de enlace. `inline constexpr` las convertiría en una
sola entidad; ese es un detalle para un programa más grande, no un cambio de regla aquí.

Una nota de compilación: `run.sh` comprueba los archivos `.cpp` y `CMakeLists.txt`, no
`move_generator.h`. Cuando CMake no está, editar solo el encabezado puede dejar un binario obsoleto.
Compila a mano o borra `build/` antes de ejecutar `./run.sh`.

### 6.4 Qué sigue siendo concreto

El generador es el único componente genérico. Todo lo demás sigue nombrando `Knight`:

| Función | Parámetro de `Knight` | Qué puede hacer |
|---|---|---|
| `initializePositions` | `Knight *` | Llamar a `set_position`. |
| `printGameHeader` | `const Knight *` | Leer `row`, `column`. |
| `printTurnHeader` | `const Knight *` | Leer `role_name`, `row`, `column`. |
| `printMove` | `const Knight *` | Leer `role_name`, `row`, `column`. |
| `tryMove` | `const Knight *` para el otro caballo | Leer `row`, `column`. |
| `moveKnight` | `Knight *` | Llamar a `set_position`. |
| `playTurn` | `Knight *` y `const Knight *` | Mover el caballo activo; leer el otro. |

El generador quita la geometría de esta tabla. No quita el tipo. Esa distinción es toda la lección:
una función puede ser independiente del tipo sobre el que opera mientras el programa que la rodea no
lo es.

### 6.5 Qué emite el compilador

`nm -C game14.o` con `-O2` no muestra ningún símbolo `generate_move_candidates`, porque la única
instanciación del juego se insertó en línea. Con `-O0` muestra un símbolo débil. En la sonda, `nm`
muestra dos símbolos débiles, uno por tipo.

`objdump -d game14.o | grep -c 'call.*\*'` imprime 0, como en `construction-13`. La plantilla no
añade ninguna llamada indirecta ni ninguna tabla. La llamada se resuelve al compilar, como cualquier
función libre.

La afirmación honesta es estructural: el tipo concreto se fija cuando se compila el programa, y el
mismo código produce una función separada por tipo. Que esa función se inserte en línea, y que el
arreglo permanezca en memoria, depende del optimizador. Mide antes de afirmar una mejora de
velocidad; en la semilla 1 esta versión hace más aritmética, no menos.

### 6.6 Qué no oculta la plantilla

`grep -n 'Knight' game.cpp` sigue devolviendo muchas líneas: la inclusión, diez firmas de ayuda,
las dos construcciones y las llamadas. `grep -n 'Knight' move_generator.h` no devuelve nada. El tipo
está oculto para el generador y visible en todo lo demás.

Ese es el giro que mide esta lección. La geometría se volvió reutilizable; el bucle no. La lección
siguiente tiene que decidir si parametriza el bucle y cómo declarar el contrato que el generador
ahora deja implícito.

## Paso 7 — Comparar y explicar (~5 min)

La comparación en una tabla:

| Pregunta | construction-13 | construction-14 |
|---|---|---|
| Dónde viven los desplazamientos | `game.cpp` | `move_generator.h` |
| Quién calcula un destino | `tryMove`, por intento | el generador, por turno |
| Qué recibe `tryMove` | caballo activo y número de movimiento | un candidato |
| Tipos que acepta la geometría | solo `Knight` | cualquiera con `row()` y `column()` |
| Cuándo se fija el tipo | al compilar el bucle | en cada instanciación |
| Copias de la geometría | una | una por tipo instanciado |
| Despacho en tiempo de ejecución | ninguno | ninguno |
| Arreglo de candidatos por turno | ninguno | 96 bytes |
| Texto de `game.o` | 3.817 bytes | 3.913 bytes |
| Texto / datos del binario | 9.400 / 692 | 9.496 / 692 |
| Código | 556 líneas, 11 archivos | 587 líneas, 12 archivos |
| Trazas | 20 de 20 | 20 de 20 |

Escribe `EVIDENCE-es.md` §8. Dos partes.

**Primero, una frase.** Di qué hacen los ocho destinos, sin usar las palabras *plantilla*,
*genérico*, *parámetro de tipo*, *instanciación*, *compilador*, *mejor*, *peor*, *limpio*,
*flexible*, *reutilizable* ni *elegante*. Si tu frase necesita una de ellas, has descrito el
mecanismo y no la decisión.

**Segundo, un párrafo.** Responde:

- Qué fija el generador: los ocho destinos se producen en un solo lugar, desde cualquier casilla que
  pueda informar su fila y su columna.
- Qué lo impone: las dos expresiones de acceso en `move_generator.h:25–26` y la referencia constante
  del parámetro.
- Qué cambió respecto a `construction-13`: los desplazamientos salieron de `game.cpp`, `tryMove`
  recibe un candidato en lugar de un número de movimiento, y `playTurn` construye el arreglo una vez
  por turno.
- Qué hacen los 17 lugares de llamada a accesores: cinco lecturas de fila, cinco de columna, tres
  escrituras de posición y cuatro lecturas de nombre, todas directas, más una llamada al generador.
- Qué prueban las 20 trazas: el comportamiento del juego no cambió tras la refactorización.
- Qué no prueban las trazas: qué función se instanció, si se insertó en línea o cuánto ocupa el
  arreglo de candidatos.
- Qué cuesta la prueba de cambio: un archivo añadido, 30 líneas; `game.cpp` con 10 líneas añadidas y
  8 eliminadas en 5 tramos; cero cambios de regla.
- Qué no cubre la plantilla: el bucle de turnos sigue nombrando `Knight` en diez firmas, así que un
  segundo tipo de pieza necesita un bucle parametrizado o una interfaz restaurada.
- Qué ahorra: la geometría se escribe una vez y acepta dos tipos en la sonda; `tryMove` ya no posee
  las tablas de desplazamientos.
- Qué cuesta: 31 líneas de código, 96 bytes de texto objeto, una instanciación por tipo, un cuerpo
  visible en el encabezado y mensajes de error tardíos y sin restricciones.
- Qué lección posterior se apoya en esta: `construction-15`, clases genéricas y conceptos, extiende
  la parametrización al motor y declara el contrato.
- Qué lección posterior posee el ciclo de vida: `construction-16`.

Una respuesta sólida mantiene cuatro cosas separadas: **qué acepta el generador**, **qué sigue
nombrando el bucle**, **qué emite el compilador** y **qué puede ver una ejecución**. Una respuesta
débil dice "la plantilla hace el código reutilizable", que colapsa las cuatro en un eslogan.

## Paso 8 — Comprobación de aceptación y registro (~2 min)

El criterio de aceptación de esta lección es: **un generador de movimientos para dos tipos de pieza;
el bucle del juego y las 20 trazas sin cambios.**

Comprueba tu registro contra él:

- [ ] Localicé la plantilla en `move_generator.h` y nombré su parámetro de tipo.
- [ ] Comprobé que el encabezado no incluye `knight.h` y no menciona `Knight`.
- [ ] Conté los 17 lugares de llamada a accesores y la única llamada al generador en `game.cpp`.
- [ ] Nombré las dos operaciones que exige el generador y sus líneas.
- [ ] Predije la tabla de candidatos y observé ambas columnas.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Compilé la sonda de instanciación y vi los mismos ocho destinos para ambos tipos.
- [ ] Vi los dos símbolos débiles con `-O0` y la ausencia de símbolo con `-O2`.
- [ ] Medí `sizeof(MoveCandidate)=12` y el arreglo de 96 bytes.
- [ ] Reproduje los errores de operación faltante y de acceso no constante.
- [ ] Reproduje la referencia indefinida de una declaración y definición separadas.
- [ ] Añadí `move_generator.h` y edité `game.cpp` en una copia temporal.
- [ ] Obtuve 10 líneas añadidas y 8 eliminadas en `game.cpp` y cero cambios de regla.
- [ ] Obtuve 20 de 20 trazas tras la refactorización.
- [ ] Medí 4.168 candidatos frente a 1.017 pares de destino en las trazas.
- [ ] Nombré el límite de este diseño: el bucle sigue nombrando `Knight`.
- [ ] No edité ningún archivo de código versionado.
- [ ] Completé `EVIDENCE-es.md` §1–§10 y la autocomprobación de §12.

El criterio no es "el código usa una plantilla". Es "puedes decir qué acepta el generador, qué sigue
nombrando el bucle, cuántas funciones produjo el compilador, cuánto cuesta el arreglo y qué cede el
diseño".

## Autocomprobación (binaria)

- [ ] Leí `move_generator.h`, `tryMove` y `playTurn` y localicé el generador por línea.
- [ ] Comparé la geometría en línea con la plantilla y nombré qué posee cada una.
- [ ] Predije la tabla de candidatos, el costo de la prueba de cambio y la sonda de instanciación
      antes de ejecutar.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Compilé la sonda y vi los mismos ocho destinos para `Knight` y `ScoutingPawn`.
- [ ] Confirmé que el generador produce candidatos fuera del tablero sin rechazarlos.
- [ ] Vi las dos instanciaciones por nombre con `-O0`.
- [ ] Reproduje los fallos de operación faltante, acceso no constante y definición separada.
- [ ] Moví la geometría a `move_generator.h` en una copia temporal.
- [ ] Mostré 20 de 20 trazas tras la refactorización.
- [ ] Conté 18 líneas cambiadas en `game.cpp` y cero cambios de regla.
- [ ] Medí las diferencias de tamaño de código y de recuento de candidatos.
- [ ] Nombré el límite de este diseño: solo el generador es genérico.
- [ ] Completé `EVIDENCE-es.md`.

Estos son criterios de aceptación. Si una casilla queda sin
marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §10.

## Misconcepciones que esta lección refuta

| Error conceptual | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| "Una plantilla es una función." | Es un patrón; el compilador escribe una función por tipo. | Cuenta las instanciaciones con `nm` con `-O0`. |
| "La plantilla se ejecuta en tiempo de ejecución y elige un tipo." | El tipo se fija al compilar; no hay tabla. | Busca llamadas indirectas y símbolos de tabla virtual; no hay. |
| "Una plantilla significa una función en el binario." | Hay una instanciación por tipo, salvo que el optimizador la elimine. | Compila la sonda con `-O0` y lee los dos símbolos. |
| "El generador valida movimientos." | Solo suma desplazamientos; un peón en `(1, 1)` obtiene destinos negativos. | Lee la columna del peón y luego lee `tryMove`. |
| "El bucle ahora también es genérico." | `game.cpp` sigue nombrando `Knight` en diez firmas. | `grep -n 'Knight' game.cpp` y cuenta. |
| "La plantilla hace el programa más rápido." | En la semilla 1 calcula 176 candidatos frente a 36 pares. | Cuenta candidatos e intentos; mide antes de afirmar. |
| "La plantilla no cuesta nada porque vive en el encabezado." | `game.o` crece 96 bytes y cada inclusión analiza el cuerpo. | Mide texto y datos por separado. |
| "El cuerpo puede vivir en un `.cpp` como cualquier función." | El compilador lo necesita en la instanciación; si no, el enlace falla. | Reproduce la referencia indefinida. |
| "La plantilla declara sus requisitos." | Los requisitos solo están en el cuerpo, en las líneas 25–26. | Prueba un tipo sin `row()` y lee dónde aparece el error. |
| "Cualquier tipo con `row()` y `column()` sirve." | Los accesores también deben poder llamarse a través de `const`. | Prueba accesores no constantes y lee el error de cualificador. |
| "`MoveCandidate` es una clase con comportamiento." | Es un registro con tres campos públicos y ninguna operación. | Señala las líneas 10–15 del encabezado. |
| "La plantilla elimina la necesidad de una interfaz." | Elimina la interfaz solo para el generador; el bucle sigue nombrando un tipo. | Declara el siguiente cambio antes de afirmar generalidad. |
| "Las 20 trazas prueban que la plantilla funciona." | Prueban el comportamiento, no qué función se ejecutó ni cómo se generó. | Usa `nm`, `size` y la sonda. |
| "El código genérico siempre es más corto." | Esta versión añade 31 líneas y un archivo. | Cuenta el encabezado y el manejo del candidato. |
| "La versión en línea anterior era más lenta." | Calculaba menos destinos por turno, no más. | Compara 1.017 pares con 4.168 candidatos. |

## Siguiente paso

`construction-15` — clases genéricas y conceptos. Esta lección hizo que una función fuera
independiente del tipo de pieza. La siguiente extiende la parametrización al motor y declara el
contrato del generador con un concepto, para que un tipo incorrecto falle en la llamada con un
mensaje claro en lugar de una cascada dentro del encabezado.

`construction-16` posee la cuestión del ciclo de vida que los miembros por valor solo posponen.
`construction-12` y `construction-13` siguen siendo los otros lados de este contraste;
conserva ambas líneas base.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog-es.md) para la ruta completa.

Si faltaba cualquiera de las dos líneas base, no empieces `construction-15`. El siguiente contraste
necesita ambos lados.
