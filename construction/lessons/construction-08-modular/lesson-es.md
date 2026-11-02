# Programación modular (modular programming)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** ocultamiento de información · **Tiempo:** unos 92 minutos ·
**Prerrequisitos:** construction-03, construction-07 · **Estado:** borrador
**Lectura:** ~20 minutos · **Prerrequisitos:** construction-03, construction-07
**Rol de la lección:** tronco

## Por qué existe esta lección

`construction-07` dejó el diseño del tablero a la vista. Cuatro firmas llevaban
`BoardCell (*)[9]`, `getBoardCell` devolvía un puntero cuyo paso solo conocía la declaración, y
cualquier llamador podía nombrar la estructura de la casilla. La línea base modular mueve ese
conocimiento detrás de tres cabeceras y cuatro unidades de traducción. El juego es el mismo: las 20
trazas de 8×8 son idénticas byte a byte antes de que cambies nada.

Después viene la prueba de cambio. Pon el tablero activo en 10×10 dentro de `board.cpp`. Cambian un
archivo y dos líneas. `game.cpp` y `main.cpp` no se tocan. Solo se recompila un objeto y el juego
sigue ejecutándose. El comportamiento cambia, así que las trazas de 8×8 dejan de aplicar. La lección
no es que el código modular conserve las trazas. Es que el tamaño del tablero pasó a ser un hecho
que solo posee el módulo del tablero.

La frase que hay que retener:

> Una declaración pública dice qué nombres puede usar un llamador y qué tipos tienen; la
> implementación decide el estado y los números detrás de esos nombres.

Tres notas honestas antes de la comparación.

1. El código crece. La línea base con punteros tiene 381 líneas en un archivo. La línea base modular
   tiene 463 líneas repartidas en siete archivos fuente: cuatro `.cpp` y tres `.h`. Añade
   `CMakeLists.txt` y `run.sh`, y el artefacto es más grande. La modularidad es una frontera, no una
   dieta.
2. La cabecera no oculta que existe un tablero, ni la convención de coordenadas desde 1, ni los
   recuentos en ejecución. Oculta la representación, el almacenamiento y las funciones auxiliares.
   Un llamador sigue sabiendo que hay un tablero y que `board_row_count()` responde con un `int`.
3. `run.sh` sin CMake compila las cuatro unidades de traducción en un solo comando. Por eso una
   recompilación ingenua recompila todo. Para contar recompilaciones necesitas archivos objeto o un
   sistema de compilación que siga dependencias. Esta lección mide con archivos objeto explícitos.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar el ocultamiento de información entre cabecera e implementación;
- cambiar el tablero de 8x8 a 10x10 y contar los módulos tocados.

## El concepto: una declaración es una promesa, el .cpp la cumple

Un programa de C++ se compila unidad de traducción por unidad de traducción. Una unidad de
traducción es un archivo `.cpp` después de que el preprocesador ha pegado cada `#include`. El
compilador ve las declaraciones de las cabeceras y las definiciones del `.cpp`. No ve los demás
archivos `.cpp`.

```text
main.cpp            incluye solo game.h
game.cpp            incluye game.h, board.h, random_sequence.h
board.cpp           incluye solo board.h
random_sequence.cpp incluye solo random_sequence.h
```

Ese grafo es todo el diseño en cuatro líneas. `main.cpp` puede llamar a `run_game` porque `game.h`
la declara. No puede nombrar `BoardCell`, `boardCells`, `Knight` ni `randomState`, porque ninguna
cabecera que incluye los declara. La restricción no es un sistema de permisos. Es una regla de
visibilidad de nombres: si el compilador no ha visto la declaración, el nombre no existe en esa
unidad de traducción.

La superficie pública son nueve declaraciones en tres cabeceras:

| Módulo | Cabecera pública | El llamador puede nombrar | La implementación oculta |
|---|---|---|---|
| `random_sequence` | `random_sequence.h` | `random_set_seed`, `random_below`, `random_integer` | multiplicador, incremento, estado |
| `board` | `board.h` | `board_row_count`, `board_column_count`, `board_reset`, `board_was_visited`, `board_mark_visited` | `BoardCell`, `boardCells`, dimensiones máxima y activa, comprobaciones de límites |
| `game` | `game.h` | `run_game` | `Knight`, `MoveOutcome`, tablas de movimientos, 17 auxiliares, bucle de turnos, transcripción |

Los archivos `.cpp` definen 28 funciones. Nueve están declaradas en cabeceras, dieciocho son
internas (17 en `game.cpp` y `parseArguments` en `main.cpp`) y una es `main`. Esos auxiliares viven
en un espacio de nombres anónimo:

```cpp
namespace
{
constexpr int maximumMoveCount = 200;
// ...
}
```

Un espacio de nombres anónimo da enlace interno a sus nombres. Existen en esa unidad de traducción y
en ninguna otra. Con `-O2` muchos desaparecen del todo; el archivo objeto exporta solo los nombres
de la cabecera más `main`. Las nueve declaraciones son la interfaz, y el objeto lo demuestra.

Una declaración no es una definición. El compilador comprueba que cada llamada coincide con una
declaración que ha visto. El enlazador comprueba que cada nombre externo usado tiene exactamente una
definición. Si `board.cpp` pierde `board_column_count`, `game.cpp` sigue compilando y el enlace
falla con una referencia sin definir. Esa división es lo que significa "compilación separada": el
compilador comprueba cada unidad y el enlazador las une.

La cabecera del tablero cabe en una pantalla:

```cpp
#pragma once

int board_row_count();
int board_column_count();
void board_reset();
bool board_was_visited(int row, int column);
void board_mark_visited(int row, int column);
```

Cinco nombres, ninguna estructura, ningún arreglo, ninguna constante. Un llamador no puede declarar
un `BoardCell`, no puede indexar `boardCells` y no puede leer las dimensiones activas sin llamar a
una función. El archivo de implementación guarda todo eso:

```cpp
namespace
{
constexpr int maximumBoardRowCount = 16;
constexpr int maximumBoardColumnCount = 16;
constexpr int activeBoardRowCount = 8;
constexpr int activeBoardColumnCount = 8;

struct BoardCell
{
    bool visited;
};

BoardCell boardCells[maximumBoardRowCount][maximumBoardColumnCount] = {};
}
```

Fíjate en lo que la división hizo con las dimensiones. Son valores devueltos por funciones, no tipos
en una firma. `game.cpp` escribe `row <= board_row_count()` y `column <= board_column_count()`.
Ninguna firma menciona 8, y ningún llamador tiene que recompilarse cuando cambia el recuento activo.
En `construction-07`, el ancho formaba parte del tipo del parámetro, `BoardCell (*)[9]`; cada
frontera que recibía el tablero lo repetía. Aquí el ancho es un dato dentro de una unidad de
traducción.

Lo que la cabecera **no** hace:

- **No impone el contrato.** `board_was_visited(0, 0)` devuelve `false` y
  `board_mark_visited(0, 0)` no hace nada. La cabecera no puede impedir que un llamador lo intente.
  Los invariantes son `construction-10` y `construction-11`.
- **No oculta la convención de coordenadas.** Los llamadores pasan filas y columnas desde 1, y el
  tablero rechaza en silencio lo que queda fuera del rango activo. La convención es un comentario en
  la implementación, no un hecho del tipo.
- **No vuelve confiable al llamador.** Un llamador puede incluir `board.h` y llamar a cualquier
  función pública. La frontera es una convención de diseño más un grafo de inclusiones, no un
  candado del lenguaje.
- **No impide una segunda definición.** Dos archivos `.cpp` que definan `board_reset` producen un
  error de enlace, no un error de diseño.
- **No hace más pequeño el programa.** Hace más pequeña la unidad de cambio.

## Antes de empezar

- [`construction-03`](../construction-03-functions/lesson-es.md) — puedes leer la declaración de
  una función, una lista de parámetros y un tipo de retorno. Es el prerrequisito del catálogo.
- [`construction-07`](../construction-07-pointers/lesson-es.md) — la comparación usa su línea base
  con punteros. Si no leíste esa lección, lee la línea base como una versión en un solo archivo del
  mismo juego; el contraste sigue funcionando.
- Un compilador de C++20 (`g++` o `clang++`). `cmake` es opcional; las mediciones siguientes no lo
  necesitan.

No necesitas conocer el enlazador en detalle. Necesitas leer una cabecera, un archivo `.cpp` y la
lista de objetos que produce una compilación.

### Qué necesitas

- La línea base modular, sus cuatro unidades de traducción y sus tres cabeceras:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/board.h`](code/board.h),
  [`code/board.cpp`](code/board.cpp), [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- El archivo único de la lección anterior:
  [`pointers-baseline.cpp`](../construction-07-pointers/code/pointers-baseline.cpp).
- Los scripts de compilación y las notas: [`run.sh`](run.sh), [`CMakeLists.txt`](CMakeLists.txt) y
  [`README-es.md`](README-es.md).
- Las reglas: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §12 fija el orden
  de los sorteos.
- El formato de la transcripción:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- Una traza de referencia: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- Esta plantilla para quien aprende: [`EVIDENCE-es.md`](EVIDENCE-es.md).

Comprueba que los artefactos existen antes de empezar, desde este directorio de la lección:

```bash
test -f code/main.cpp &&
test -f code/game.h &&
test -f code/game.cpp &&
test -f code/board.h &&
test -f code/board.cpp &&
test -f code/random_sequence.h &&
test -f code/random_sequence.cpp &&
test -f ../construction-07-pointers/code/pointers-baseline.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

Si falta un archivo, la comparación no tiene segundo lado. Registra el archivo ausente en
`EVIDENCE-es.md` §11 y vuelve cuando esté disponible. Un conteo contra un archivo que no has leído
no es evidencia.

Ambas líneas base reproducen las 20 trazas de `common/traces/` byte a byte antes de que cambies
nada. El mismo juego, una unidad de traducción contra cuatro. Eso es lo que hace justa la
comparación.

## Paso 1 — Inventariar la interfaz (~12 min)

Lee primero las tres cabeceras. Después deja que el texto te diga dónde vive cada definición:

```bash
grep -n . code/board.h code/game.h code/random_sequence.h
grep -n '^int \|^void \|^bool \|^std::' code/board.cpp code/random_sequence.cpp code/game.cpp
```

El primer comando imprime cada línea de las cabeceras. El segundo imprime las definiciones de
función que empiezan en la columna cero; los auxiliares indentados dentro de espacios de nombres
anónimos no coinciden, que es justo el punto. `run_game` empieza en la columna cero en `game.cpp` y
aparece; los 17 auxiliares no.

Ahora el grafo de inclusiones, tal como lo ve el compilador:

```bash
g++ -std=c++20 -Icode -MM code/main.cpp
g++ -std=c++20 -Icode -MM code/game.cpp
g++ -std=c++20 -Icode -MM code/board.cpp
g++ -std=c++20 -Icode -MM code/random_sequence.cpp
```

La salida es corta:

```text
main.o: code/main.cpp code/game.h
game.o: code/game.cpp code/game.h code/board.h code/random_sequence.h
board.o: code/board.cpp code/board.h
random_sequence.o: code/random_sequence.cpp code/random_sequence.h
```

Completa esta tabla en `EVIDENCE-es.md` §2.1 mientras lees.

| Módulo | Cabecera | Líneas de la cabecera | Declaraciones | Definiciones en | Datos ocultos |
|---|---|---|---|---|---|
| `random_sequence` | `random_sequence.h` | 1–7 | 3 | `random_sequence.cpp` | multiplicador, incremento, estado |
| `board` | `board.h` | 1–7 | 5 | `board.cpp` | `BoardCell`, arreglo, 4 constantes |
| `game` | `game.h` | 1–6 | 1 | `game.cpp` | `Knight`, `MoveOutcome`, 5 constantes, 2 tablas, 17 auxiliares |
| `main` | ninguna | — | 0 | `main.cpp` | `parseArguments` |

Recuentos: 3 cabeceras, 4 unidades de traducción, 9 declaraciones públicas, 28 definiciones, 18
auxiliares internos, 1 punto de entrada del programa.

Después responde en `EVIDENCE-es.md` §2:

1. ¿Qué nombres conoce `main.cpp`? ¿Qué nombres no conoce?
2. ¿Qué nombres obtiene `game.cpp` de `board.h`, y qué nombres del tablero no puede ver?
3. ¿Qué cuatro números se mudaron del código del juego a `board.cpp`?
4. ¿Qué funciones de `game.cpp` no están declaradas en ninguna cabecera, y qué las hace internas?
5. ¿Podría `main.cpp` llamar a `board_reset`? ¿Qué tendría que añadir, y por qué eso es una
   decisión de diseño y no una restricción del compilador?
6. ¿`board_row_count()` devuelve una constante de compilación o un valor de ejecución? ¿Qué cambia
   eso en las firmas que la usan?
7. `game.cpp` contiene el literal `8` en `moveOrderMaximum = 8`. ¿Es una dimensión del tablero?
8. ¿Cuál es la diferencia entre lo que comprueba el compilador y lo que comprueba el enlazador?

La pregunta 5 es la comprobación de honestidad. `main.cpp` *podría* incluir `board.h`; nada lo
impide. La frontera es una convención, sostenida por el grafo de inclusiones y por la revisión, no
por el lenguaje. La pregunta 7 importa: el único `8` que queda en `game.cpp` es la cantidad de
movimientos del caballo, no el ancho del tablero. Búscalo y demuéstralo.

## Paso 2 — Predecir antes de ejecutar (~15 min)

### 2.1 Las trazas

Escribe estas predicciones antes de ejecutar nada. Los archivos están delante; el ejercicio es ver
cuánto del comportamiento puedes explicar sin medirlo.

| Pregunta | Mi predicción |
|---|---|
| Línea base modular vs `seed-0001.txt` | |
| Línea base con punteros vs `seed-0001.txt` | |
| Modular vs punteros, semilla 1 | |
| Semillas que coinciden, de 20 | |
| Primera línea que podría diferir, y por qué | |

### 2.2 La superficie de cambio

Vas a cambiar `activeBoardRowCount` y `activeBoardColumnCount` en `board.cpp` de 8 a 10. Predice el
costo antes de tocar la edición.

| Pregunta | Mi predicción |
|---|---|
| Archivos fuente editados | |
| Líneas editadas | |
| Cabeceras editadas | |
| Archivos `.cpp` editados fuera de `board.cpp` | |
| Objetos recompilados (de 4) | |
| ¿Compila sin otras ediciones? | |
| Semillas que coinciden con las trazas de 8×8, de 20 | |
| ¿El juego sigue llegando a un ganador o a un empate? | |

### 2.3 Qué ocultan las cabeceras

Predice qué nombres puede usar cada unidad de traducción antes de comprobarlo con el compilador.
Escribe `sí` o `no`.

| Nombre | `main.cpp` | `game.cpp` | `board.cpp` |
|---|---|---|---|
| `run_game` | | | |
| `board_column_count` | | | |
| `BoardCell` | | | |
| `boardCells` | | | |
| `Knight` | | | |
| `randomState` | | | |
| `moveOrderMaximum` | | | |

Las filas interesantes son las que son `no` en una unidad y `sí` en otra. Esa diferencia es la
frontera. Escribe la tabla en `EVIDENCE-es.md` §3.3 antes de compilar.

## Paso 3 — Ejecutar ambas líneas base (~10 min)

Crea primero el directorio de trabajo; todos los comandos siguientes escriben allí.

```bash
mkdir -p /tmp/modular-test
./run.sh --seed 1 --name Learner > /tmp/modular-test/modular-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/modular-test/modular-0001.txt
```

Después la línea base anterior:

```bash
cd ../construction-07-pointers
./run.sh --seed 1 --name Learner > /tmp/modular-test/pointers-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/modular-test/pointers-0001.txt
```

Después las dos entre sí:

```bash
diff -u /tmp/modular-test/pointers-0001.txt /tmp/modular-test/modular-0001.txt
```

Tres diffs vacíos. Vuelve al directorio de la lección modular y confirma las 20 semillas, no solo la
primera:

```bash
cd ../construction-08-modular
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

Un bucle silencioso son 20 coincidencias. Si `run.sh` no puede compilar, compila una vez a mano:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o build/modular-baseline code/main.cpp code/game.cpp code/board.cpp \
  code/random_sequence.cpp
```

Si un diff no está vacío, detente. Regístralo en `EVIDENCE-es.md` §11. No ajustes una traza para
que coincida con el código.

## Paso 4 — Mapear cada cabecera a su implementación (~12 min)

La comparación de trazas dice que las dos versiones se comportan igual. Este paso dice *por qué* la
división es real y no cosmética. Compila cada unidad de traducción a su propio objeto y pregúntale
al objeto qué exporta.

```bash
mkdir -p /tmp/modular-test/objects
for source in main game board random_sequence; do
  g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
    -c "code/${source}.cpp" -o "/tmp/modular-test/objects/${source}.o"
done
```

Ahora lista las definiciones externas de cada objeto:

```bash
for object in /tmp/modular-test/objects/*.o; do
  echo "== ${object} =="
  nm -C --defined-only "${object}" | grep ' T '
done
```

Forma esperada en esta plataforma:

```text
main.o              main
game.o              run_game(long, std::string const&)
board.o             board_reset, board_row_count, board_was_visited,
                    board_column_count, board_mark_visited
random_sequence.o   random_below, random_integer, random_set_seed
```

`T` significa una definición externa en el segmento de texto. El objeto del tablero exporta
exactamente los cinco nombres de `board.h`. El objeto del juego exporta exactamente un nombre:
`run_game`. Los 17 auxiliares son internos y, con `-O2`, en su mayoría quedan alineados dentro del
código y desaparecen. Así se ve "la cabecera es la interfaz" en un binario. El tipo `std::string`
está abreviado arriba; `nm -C` imprime su forma completa de plantilla.

Completa esta tabla en `EVIDENCE-es.md` §5.1:

| Cabecera | Declaraciones | Objeto | Símbolos externos | Nombres ocultos |
|---|---|---|---|---|
| `board.h` | 5 | `board.o` | | |
| `game.h` | 1 | `game.o` | | |
| `random_sequence.h` | 3 | `random_sequence.o` | | |

### 4.1 La comprobación del enlazador

El compilador confía en las declaraciones. El enlazador comprueba que cada declaración usada tiene
una definición. Demuéstralo en una copia de trabajo; nunca en el archivo versionado.

```bash
cp -r code /tmp/modular-test/link-check
cd /tmp/modular-test/link-check
```

Borra el cuerpo de `board_column_count` de `board.cpp`, conserva la declaración en `board.h`,
compila las cuatro unidades de traducción y enlaza:

```bash
for source in main game board random_sequence; do
  g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
    -c "${source}.cpp" -o "${source}.o"
done
g++ -o modular-baseline main.o game.o board.o random_sequence.o
```

Las cuatro compilaciones pasan. El enlace falla con una referencia sin definir a
`board_column_count()`. La redacción exacta depende del enlazador y del idioma, pero la clase de
fallo es estable: éxito en compilación, fallo en enlace. Registra el diagnóstico en
`EVIDENCE-es.md` §5.3.

Esa es la división del trabajo:

- **Compilador:** cada unidad de traducción ve declaraciones y comprueba que las llamadas y las
  definiciones coinciden con ellas.
- **Enlazador:** cada nombre externo usado tiene exactamente una definición.
- **Ninguno de los dos:** que la llamada tenga sentido, que la fila esté en rango o que el tablero
  se haya reiniciado.

## Paso 5 — Cambiar el tablero a 10×10 (~20 min)

Copia la línea base. El archivo versionado no se toca.

```bash
lessonDirectory="$(pwd)"
cp -r code /tmp/modular-test/board-10
cd /tmp/modular-test/board-10
```

En `board.cpp`, cambia dos líneas:

```cpp
constexpr int activeBoardRowCount = 8;      // antes
constexpr int activeBoardColumnCount = 8;   // antes
```

por:

```cpp
constexpr int activeBoardRowCount = 10;     // después
constexpr int activeBoardColumnCount = 10;  // después
```

No cambies `maximumBoardRowCount` ni `maximumBoardColumnCount`. Valen 16, y 10 cabe. No toques
ninguna cabecera, ni `game.cpp`, ni `main.cpp`.

### 5.1 Compilar y contar qué se recompiló

Compila cuatro objetos desde cero, después edita y luego recompila solo los obsoletos:

```bash
mkdir -p objects
for source in main game board random_sequence; do
  g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
    -c "${source}.cpp" -o "objects/${source}.o"
done
# ahora aplica la edición de dos líneas en board.cpp
for source in main game board random_sequence; do
  if [[ "${source}.cpp" -nt "objects/${source}.o" ]]; then
    echo "compiling ${source}.cpp"
    g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
      -c "${source}.cpp" -o "objects/${source}.o"
  fi
done
g++ -o modular-baseline objects/main.o objects/game.o objects/board.o \
  objects/random_sequence.o
```

La única línea `compiling` es `compiling board.cpp`. Los otros tres objetos conservan su marca de
tiempo. El enlace vuelve a ejecutarse porque el binario depende de los cuatro objetos. Esa es la
superficie de cambio: una unidad de traducción y después el enlace.

Este bucle es deliberadamente ingenuo: solo mira las marcas de tiempo de los `.cpp`. Alcanza aquí
porque no cambia ninguna cabecera. Un sistema de compilación real también sigue las cabeceras; §5.4
muestra por qué importa.

### 5.2 Ejecutar el juego nuevo

Ejecuta la semilla 1 y compara con la traza de 8×8. Vuelve primero al directorio de la lección;
las rutas de las trazas son relativas a él.

```bash
cd "${lessonDirectory}"
/tmp/modular-test/board-10/modular-baseline --seed 1 --name Learner | head -8
/tmp/modular-test/board-10/modular-baseline --seed 1 --name Learner |
  diff -u ../../../common/traces/seed-0001.txt -
```

Para la semilla 1, la primera diferencia está en las líneas `initial_*`. La casilla inicial se
sortea de `board_row_count() * board_column_count()`, ahora 100 en vez de 64, y la conversión usa
el nuevo recuento de columnas. Las tiradas se sortean después de las posiciones, así que ellas y
cada sorteo posterior vienen de un estado distinto. Las reglas no cambiaron; el tablero sí.

Ejecuta las 20 semillas contra las trazas de 8×8:

```bash
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/modular-test/board-10/modular-baseline --seed "${seedNumber}" \
    --name Learner | cmp - "${trace}"
done
```

Espera `0` coincidencias de 20. No es una regresión. Las trazas de referencia codifican un tablero
de 8×8, y esta sonda cambia el juego a propósito. Regístralo como una observación estructural: la
línea base sin modificar coincidía 20 de 20; la sonda de 10×10 coincide 0 de 20; las dos son
correctas para sus propias reglas.

¿El juego nuevo termina? Ejecuta la semilla 1 hasta el final y mira las dos últimas líneas. Debe
imprimir una línea `winner=` y una línea `moves=`, porque el bucle de turnos sigue terminando por
captura, trampa o empate a los 200 movimientos. El comportamiento cambió; el programa no se rompió.

### 5.3 La respuesta archivo por archivo

Completa esta tabla en `EVIDENCE-es.md` §6.3. Es la respuesta directa al criterio de aceptación.

| Archivo | ¿Editado? | ¿Recompilado? |
|---|---|---|
| `code/board.cpp` | sí, 2 líneas | sí |
| `code/board.h` | no | no |
| `code/game.cpp` | no | no |
| `code/game.h` | no | no |
| `code/main.cpp` | no | no |
| `code/random_sequence.cpp` | no | no |
| `code/random_sequence.h` | no | no |

Siete archivos fuente, uno editado, uno recompilado y después el enlace. `game.cpp` y `main.cpp` no
necesitan ediciones porque nunca nombran el tamaño. Preguntan por `board_row_count()` y
`board_column_count()` en ejecución.

### 5.4 Cambiar la cabecera en su lugar (el contraste)

La medición anterior cambió un archivo de implementación. Ahora cambia un archivo de interfaz en una
segunda copia de trabajo y observa cómo se invierte la dirección.

```bash
cp -r code /tmp/modular-test/header-10
cd /tmp/modular-test/header-10
# compila cuatro objetos y después añade una línea de comentario a board.h
```

Después de tocar `board.h`, recompila con reglas de dependencia: un objeto está obsoleto si su
`.cpp` es más nuevo que el objeto **o** si cualquier cabecera que incluye es más nueva que el
objeto. Para este proyecto:

| Objeto | Depende de |
|---|---|
| `main.o` | `main.cpp`, `game.h` |
| `game.o` | `game.cpp`, `game.h`, `board.h`, `random_sequence.h` |
| `board.o` | `board.cpp`, `board.h` |
| `random_sequence.o` | `random_sequence.cpp`, `random_sequence.h` |

Ejecuta este bucle una vez para compilar los cuatro objetos, añade una línea de comentario a
`board.h` y ejecútalo otra vez:

```bash
mkdir -p objects
for source in main game board random_sequence; do
  object="objects/${source}.o"
  stale=0
  [[ "${source}.cpp" -nt "${object}" ]] && stale=1
  case "${source}" in
    main) headers="game.h" ;;
    game) headers="game.h board.h random_sequence.h" ;;
    board) headers="board.h" ;;
    random_sequence) headers="random_sequence.h" ;;
  esac
  for header in ${headers}; do
    [[ "${header}" -nt "${object}" ]] && stale=1
  done
  if [[ "${stale}" -eq 1 ]]; then
    echo "compiling ${source}.cpp"
    g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
      -c "${source}.cpp" -o "${object}"
  fi
done
```

La segunda ejecución compila `board.cpp` y `game.cpp`; `main.o` y `random_sequence.o` se quedan. Un
comentario en `board.h` basta para forzar dos compilaciones porque el sistema de compilación es
conservador: no lee el comentario, lee la marca de tiempo.

Esta es la regla que hay que llevarse:

- Un cambio en un `.cpp` recompila ese objeto.
- Un cambio en una cabecera recompila cada objeto que la incluye, directa o transitivamente.
- Un cambio en una interfaz tiene un radio de impacto mayor que un cambio en una implementación.

## Paso 6 — Comparar la superficie de cambio (~10 min)

La prueba de cambio de la ruta pide cuatro números; esta tabla añade dos que importan para este
cambio. Mídelos en las dos líneas base.

| Métrica | Línea base con punteros (un archivo) | Línea base modular |
|---|---|---|
| Archivos tocados | 1 (`pointers-baseline.cpp`) | 1 (`board.cpp`) |
| Líneas cambiadas | 2 | 2 |
| Firmas cuyo tipo de parámetro cambia | 4 | 0 |
| Conceptos nuevos necesarios | 0 | 0 |
| Unidades de traducción recompiladas | 1 de 1: tablero, juego y `main` juntos | 1 de 4: `board.o`, después el enlace |
| Trazas de 8×8 rotas | 20 | 20 |

Lee la fila de unidades de traducción con cuidado. Los dos cambios tocan un archivo y dos líneas,
así que el conteo de líneas no los separa. La diferencia es **qué hay dentro de la unidad
recompilada**. En el archivo único, las constantes del tablero, el bucle del juego y `main` son una
sola unidad de traducción; cambiar el tablero recompila todo ese código. En la línea base modular,
la unidad recompilada contiene el tablero y nada más. `game.cpp` y `main.cpp` no son leídos por el
compilador para este cambio.

La fila de firmas es la versión de tipos del mismo hecho. En el archivo único, el ancho forma parte
de cuatro tipos de parámetro, así que cambiarlo cambia cuatro firmas. En la línea base modular,
ninguna firma menciona el ancho, así que no cambia ningún tipo.

Lo que compró el cambio:

- Una unidad de conocimiento más pequeña. La representación, el almacenamiento y las dimensiones del
  tablero están en un archivo.
- Una unidad de recompilación más pequeña. Un objeto en vez de todo el juego.
- Una respuesta en ejecución en vez de un hecho de compilación. Los llamadores preguntan por el
  recuento; ninguna firma lleva el ancho.
- Una frontera para el cambio siguiente. Si la representación del tablero pasa de un arreglo a un
  bitboard dentro de `board.cpp`, estos llamadores no necesitan edición. Esa es la lección
  siguiente.

Lo que costó el cambio:

- 463 líneas en vez de 381, repartidas en siete archivos fuente en vez de uno.
- Un archivo de compilación y un grafo de dependencias que mantener.
- Una llamada a función donde antes había una constante; el compilador puede alinearla, pero el
  código ya no muestra el número.
- Una convención que los llamadores deben respetar: las filas y columnas empiezan en 1, y las
  llamadas fuera de rango se ignoran en vez de rechazarse.
- 20 trazas que ya no describen esta compilación. Un juego de 10×10 necesita sus propias trazas; las
  antiguas siguen siendo el contrato de 8×8.

Una medición honesta más. Si el cambio fuera "sustituir el arreglo por una lista enlazada o un
bitboard", el archivo único tocaría el acceso al tablero en `tryMove`, las dos escrituras en
`playTurn`, la declaración en `main` y los tipos de parámetro en tres firmas. La línea base modular
tocaría los cinco cuerpos de función en `board.cpp`. La cabecera no cambiaría, porque nunca nombró
`BoardCell`. Esa es la prueba de `construction-09`, y es la razón más fuerte para esta frontera.

## Paso 7 — Comparar y explicar (~10 min)

Escribe `EVIDENCE-es.md` §8. Dos partes.

**Primero, una frase.** Di qué decide una declaración pública sobre un llamador, sin usar las
palabras *cabecera*, *archivo*, *módulo*, *compilar*, *mejor*, *peor*, *limpio*, *limpia*, *bonito*,
*elegante*, *feo* ni *modular*. Si tu frase necesita una de ellas, has descrito tu reacción al
diseño y no a la frontera.

**Segundo, un párrafo.** Responde:

- Qué decide una declaración pública: qué nombres puede usar un llamador y qué tipos llevan.
- Qué oculta el archivo de implementación: representación, almacenamiento, constantes, funciones
  auxiliares, estado.
- Qué comprueba el compilador: cada unidad de traducción contra las declaraciones que ha visto.
- Qué comprueba el enlazador: una definición para cada nombre externo usado.
- Qué muestra el grafo de compilación: qué unidades de traducción deben recompilarse tras un cambio.
- Qué sigue exponiendo la interfaz: la existencia de un tablero, la convención de coordenadas, los
  recuentos en ejecución y los nombres de las cinco operaciones del tablero.
- Qué **no** impone: límites, invariantes, disciplina del llamador ni una única definición.
- Qué solo prueba una ejecución: que la transcripción no cambia antes del cambio, que 0 de 20
  coinciden después, y que el juego de 10×10 llega a un final.
- Qué costó la modularidad: 82 líneas fuente más, seis archivos fuente más, un grafo de compilación
  y una convención documentada en vez de escrita en el tipo.
- Qué lección posterior hace explícito el cambio de representación: `construction-09`; invariantes:
  `construction-10` y `construction-11`; propiedad: `construction-16`; puertos y adaptadores:
  `construction-20`.

Una respuesta fuerte mantiene separadas cuatro cosas: **qué permite la interfaz**, **qué oculta la
implementación**, **qué comprueba la cadena de herramientas** y **qué muestra un registro de
compilación o una ejecución**. Una respuesta débil dice que el código modular es más limpio, lo que
oculta las cuatro.

## Paso 8 — Comprobación de aceptación y registro (~3 min)

El criterio de aceptación de esta lección es: **cambiar el tamaño del tablero en el módulo del
tablero y mostrar que `game.cpp` y `main.cpp` no requieren ediciones; explicar qué ocultan las
cabeceras.**

Comprueba tu registro contra él:

- [ ] Ejecuté las dos líneas base sin modificar y obtuve tres diffs vacíos, más 20 semillas
      coincidentes.
- [ ] Inventarié las 9 declaraciones públicas, las 28 definiciones y los 18 auxiliares internos.
- [ ] Mapeé cada cabecera a su objeto y listé los símbolos externos con `nm`.
- [ ] Mostré que el compilador acepta una definición ausente y el enlazador la rechaza.
- [ ] Predije las trazas y la superficie de cambio antes de medir.
- [ ] Cambié dos constantes en `board.cpp` a 10 y 10.
- [ ] `board.h`, `game.h`, `game.cpp`, `main.cpp` y los dos archivos aleatorios no se editaron.
- [ ] Solo se recompiló `board.o`, y después se enlazó.
- [ ] La compilación de 10×10 se ejecuta y llega a un ganador o a un empate.
- [ ] Obtuve 20 de 20 coincidencias antes del cambio y 0 de 20 después.
- [ ] Expliqué por qué 0 de 20 es esperado y no una regresión.
- [ ] Mostré que un cambio de cabecera recompila a sus inclusores, mientras que un cambio de `.cpp`
      no.
- [ ] Medí el costo de la superficie de cambio en las dos líneas base y dije qué compró y qué costó
      la modularidad.
- [ ] Expliqué qué ocultan las cabeceras, qué exponen y qué no impone ninguna de las dos
      herramientas.
- [ ] Mi frase de una oración evita las cuatro palabras de diseño y las ocho de veredicto.
- [ ] No edité ningún archivo de código versionado.
- [ ] Completé `EVIDENCE-es.md` §1–§10 y la autocomprobación de §12.

El criterio no es "más archivos es mejor". Es "puedes decir qué unidad de traducción posee el tamaño
del tablero, qué unidades deben recompilarse cuando cambia, y qué comprobación notaría si una
declaración y su definición no coincidieran".

## Autocomprobación (binaria)

- [ ] Leí los siete archivos fuente e inventarié las tres cabeceras.
- [ ] Predije los resultados de las trazas, la superficie de edición y la de recompilación antes de
      medir.
- [ ] Compilé y ejecuté ambas líneas base y reproduje las 20 trazas.
- [ ] Compilé cuatro objetos e inspeccioné los símbolos externos de cada uno.
- [ ] Mostré la división compilación-enlace con una definición ausente.
- [ ] Cambié el tablero activo a 10×10 en `board.cpp` sin editar `game.cpp` ni `main.cpp`.
- [ ] Conté un archivo editado, dos líneas editadas, un objeto recompilado y un enlace.
- [ ] Vi 0 de 20 trazas coincidir y expliqué por qué es esperado.
- [ ] Contrasté un cambio de `.cpp` con un cambio de cabecera y sus distintos radios de impacto.
- [ ] Comparé el costo de la superficie de cambio y nombré qué compró y qué costó la modularidad.
- [ ] Expliqué qué ocultan las cabeceras y qué no imponen.
- [ ] Completé `EVIDENCE-es.md`.

Estos son criterios de aceptación. Si una casilla queda
sin marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §10.

## Misconcepciones que esta lección refuta

| Error de concepto | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| "Una cabecera es una copia del `.cpp`." | La cabecera lleva declaraciones; los cuerpos quedan en el `.cpp`. | Lee la cabecera como la lista de nombres que puede usar un llamador. |
| "Dividir el archivo cambia el comportamiento." | La línea base modular sin modificar reproduce las 20 trazas. | Compara las trazas antes de juzgar el diseño. |
| "Las cabeceras lo ocultan todo." | Exponen nombres, tipos, la convención de coordenadas y la existencia de un tablero. | Lista qué puede seguir nombrando y qué debe seguir sabiendo un llamador. |
| "El ocultamiento de información es seguridad." | Un llamador puede incluir cualquier cabecera y llamar a cualquier función pública. | Trátalo como una frontera de dependencias, no como control de acceso. |
| "El compilador impone la interfaz." | Comprueba declaraciones por unidad de traducción; el enlazador resuelve definiciones. | Prueba las dos: un tipo incompatible falla en compilación, un cuerpo ausente falla en enlace. |
| "Cambiar `board.cpp` obliga a recompilar todo." | Solo se recompila su objeto y luego se enlaza, si las dependencias se siguen. | Mide con objetos, no con un `run.sh` de un solo comando. |
| "Un cambio de cabecera cuesta lo mismo que uno de `.cpp`." | Cada inclusor se recompila; un comentario en `board.h` también recompila `game.o`. | Usa `g++ -MM` para ver las aristas de dependencia antes de editar. |
| "El cambio a 10×10 debería conservar las trazas." | Las trazas codifican el comportamiento de 8×8; cambiaron los sorteos iniciales y los límites. | Conserva 20/20 antes, espera 0/20 después y di por qué. |
| "El código modular es más pequeño." | 463 líneas fuente contra 381, más un archivo de compilación. | Cuenta las líneas y los archivos, no solo los conceptos. |
| "El tamaño del tablero está oculto en la cabecera." | La cabecera expone `board_row_count()` y `board_column_count()` como valores de ejecución. | Di que el número está oculto, no el hecho de que exista un recuento. |
| "Con que compile, una definición alcanza." | Una definición ausente compila y falla en el enlace. | Compila y enlaza el programa completo antes de llamar completa a la interfaz. |
| "El espacio de nombres anónimo es decoración." | Da enlace interno; sin él los auxiliares serían nombres externos. | Léelo como "este nombre pertenece a esta unidad de traducción". |

## Siguiente paso

`construction-09` — tipo abstracto de datos. El tablero ya oculta su representación detrás de cinco
funciones, pero la cabecera sigue hablando en filas y columnas y el llamador sigue siendo dueño de
la convención de coordenadas. La lección siguiente vuelve opaco el tipo y le da al tablero sus
propios límites, para que el `1..N` deje de ser un hecho que el llamador debe recordar.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog-es.md) para la ruta completa.

Si faltaba cualquiera de las dos líneas base, no empieces `construction-09`. El siguiente contraste
necesita los dos lados.
