# Tipo abstracto de datos (abstract data type, ADT)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** independencia de representación · **Tiempo:** unos 97 minutos · **Estado:** borrador
**Lectura:** ~22 minutos

## Por qué existe esta lección

`construction-08` movió el tablero detrás de cinco funciones. Fue una frontera real: `game.cpp` no
podía nombrar `BoardCell`, ni `boardCells`, ni las dos constantes de dimensión. Pero el tablero
seguía sin ser un valor. Un arreglo vivía en almacenamiento estático dentro de `board.cpp`, y las
cinco funciones no llevaban parámetros. Un llamador no podía sostener un tablero, ni pasar uno a una
función, ni crear un segundo. El tablero era un conjunto de funciones globales sobre un objeto
global.

Esta línea base cambia eso. `board.h` declara un nombre y se detiene:

```cpp
struct Board;
```

Sin campos, sin tamaño, sin arreglo. Siete operaciones llevan `const Board *` o `Board *`. La
definición del tipo vive en `board.cpp`, junto al almacenamiento. El juego es idéntico byte a byte:
las 20 trazas de 8×8 coinciden antes de que cambies nada.

Después viene la prueba de cambio. Reemplaza el arreglo dentro de `board.cpp` por un `std::vector`
plano y una función de índice. Cambia un archivo. `board.h`, `game.cpp` y `main.cpp` no se tocan.
Las 20 trazas siguen pasando, porque el diseño nunca salió de `board.cpp`.

La frase que hay que retener:

> Un tipo es abstracto cuando su declaración le da un nombre pero no un tamaño, y cada operación
> recibe ese tipo como parámetro.

Tres notas honestas antes de la comparación.

1. El código crece un poco. `construction-08` tiene 463 líneas en siete archivos fuente; esta línea
   base tiene 479. `board.h` pasa de 7 a 11 líneas, `board.cpp` de 57 a 65 y `game.cpp` de 284 a
   288. Casi todo el crecimiento es un parámetro por función.
2. El cambio no es gratis a nivel binario. El archivo objeto de `board.cpp` lleva 504 bytes de
   texto con el arreglo, 1311 con un vector plano y 4172 con un vector de vectores. Los nombres
   exportados son idénticos en los tres; el coste de las operaciones no lo es.
3. Las 20 trazas no lo verifican todo. Quita la inicialización de `board_create` y las trazas siguen
   pasando, porque `board_reset` corre antes y asigna. Una traza es un contrato de comportamiento,
   no un contrato de seguridad de memoria. `construction-10` y `construction-11` cierran ese hueco.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar la independencia de la representación;
- cambiar la representación del tablero mientras todos los llamadores compilan.

## El concepto: la cabecera nombra el tipo, el .cpp le da un tamaño

Una declaración no es una definición. `struct Board;` es una declaración con punto y coma después
del nombre. Le dice tres cosas al compilador y le oculta una:

```text
dice:     existe un tipo llamado Board, en el ámbito de espacio de nombres
dice:     el nombre puede aparecer en declaraciones
dice:     Board es un struct, no una union, un enum ni una clase
oculta:   cuántos bytes ocupa un Board y qué hay dentro
```

Eso es un **tipo incompleto**. El estándar de C++ lo llama exactamente así, y las restricciones
salen del tamaño que falta. Con un tipo incompleto puedes:

- declarar un puntero: `Board *board;`
- declarar una referencia: `Board &board = *puntero;`
- declarar una función que recibe o devuelve un puntero a él;
- comparar dos punteros a él, o probar uno contra `nullptr`.

Con un tipo incompleto no puedes:

- declarar un objeto: `Board board;` necesita el tamaño para reservar pila;
- tomar `sizeof(Board)`;
- acceder a un miembro: `board->visited` necesita el diseño para calcular un desplazamiento;
- indexarlo ni copiarlo por valor;
- hacer `delete` de un puntero a él: el compilador no puede saber si existe un destructor.

Lee esa lista otra vez. Cada operación prohibida es una forma de averiguar el diseño. Cada
operación permitida es una forma de sostener el objeto sin averiguarlo. Ese es todo el mecanismo de
un tipo abstracto de datos en C++, y lo impone el compilador, no una convención ni una revisión de
código.

Las siete declaraciones son toda la superficie pública:

```cpp
struct Board;

Board *board_create();
void board_destroy(Board *board);
int board_row_count(const Board *board);
int board_column_count(const Board *board);
void board_reset(Board *board);
bool board_was_visited(const Board *board, int row, int column);
void board_mark_visited(Board *board, int row, int column);
```

| Operación | Parámetro | Por qué esa forma |
|---|---|---|
| `board_create` | ninguno | Devuelve `Board *`, nunca `Board`. Una función no puede devolver un tipo incompleto por valor: el llamador necesitaría el tamaño para reservar espacio al resultado. Un puntero tiene un tamaño conocido apunte a lo que apunte. |
| `board_destroy` | `Board *` | Libera el objeto. La definición es visible dentro de `board.cpp`, así que `delete` es legal ahí e ilegal en cualquier llamador. |
| `board_row_count` | `const Board *` | Lee. `const` dice que la operación no modifica el objeto. |
| `board_column_count` | `const Board *` | Lee. |
| `board_reset` | `Board *` | Escribe en todas las casillas. |
| `board_was_visited` | `const Board *` | Lee una casilla. |
| `board_mark_visited` | `Board *` | Escribe una casilla. |

El `const` no es decoración. `board_was_visited` recibe un `const Board *`, así que dentro de
`board.cpp` no puede asignar a través de ese puntero. Si alguien escribe una asignación ahí más
adelante, el compilador la rechaza. `game.cpp` pasa el mismo puntero a `board_was_visited` y a
`board_mark_visited`; la primera llamada no puede cambiar el objeto, la segunda sí. Esa distinción
se comprueba en el punto de llamada y dentro de la implementación.

Ahora la parte honesta. ¿Qué compró esto sobre `construction-08`?

| | `construction-08` | `construction-09` |
|---|---|---|
| Nombre de tipo en la cabecera | ninguno | `Board`, incompleto |
| Operaciones | 5 | 7 |
| Operaciones que reciben un tablero | 0 | 7 |
| Tableros vivos a la vez | 1 | cualquier número |
| Almacenamiento del tablero | arreglo estático en `board.cpp` | objeto en el montón con `new` / `delete` |
| Ciclo de vida del tablero | todo el programa | de `board_create` a `board_destroy` |
| El llamador puede sostener un tablero | no | sí, como `Board *` |
| El llamador puede ver el diseño | no | no |

El cambio de representación **no** es lo que separa a las dos. `construction-08` también podía
reemplazar su arreglo por un vector dentro de `board.cpp` sin que ningún llamador lo notara, porque
el arreglo ya estaba en un espacio de nombres anónimo. Para un solo tablero, el cambio ya era
seguro.

Lo que el TAD añade es **multiplicidad, ciclo de vida y un valor que pasar**. En `construction-08`
el tablero es un singleton en almacenamiento estático; una segunda partida en el mismo proceso lo
compartiría. Aquí cada `board_create` devuelve un objeto separado, y el llamador decide cuándo
muere. Eso es lo que permite que una prueba construya un tablero limpio en lugar de reiniciar el
global, y es lo que convierte la propiedad (`construction-16`) en una pregunta que vale la pena
hacer.

Lo que la cabecera **no** hace:

- **No impone un contrato de nulos.** `board_row_count(nullptr)` devuelve 8 en esta
  implementación, porque el parámetro no tiene nombre y se ignora. `board_reset(nullptr)`
  desreferencia y es comportamiento indefinido. Ninguna de las dos llamadas se comprueba.
- **No impide copiar el manejador.** `Board *segundo = board;` compila. Dos manejadores ahora
  poseen un objeto, y destruir ambos es un doble liberado. Nada en la cabecera dice "un solo
  dueño".
- **No impone los invariantes.** `board_mark_visited(board, 0, 0)` sigue sin hacer nada, en
  silencio. Los límites son `construction-10` y `construction-11`.
- **No oculta la identidad del objeto.** Un llamador puede guardar el puntero en su propia
  estructura, comparar dos punteros y pasar el manejador. El diseño está oculto; la dirección no.
- **No abarata las operaciones.** Las versiones con vector de abajo cuestan más texto y más
  indirección que el arreglo. La independencia de representación es una propiedad de diseño, no una
  propiedad de rendimiento.

## Antes de empezar

- [`construction-08`](../construction-08-modular/lesson-es.md) — el prerrequisito del catálogo.
  Necesitas su tablero modular delante, porque toda la lección es una diferencia contra él.
- [`construction-03`](../construction-03-functions/lesson-es.md) — puedes leer la declaración de
  una función, una lista de parámetros y un tipo de retorno.
- Un compilador de C++20 (`g++` o `clang++`). `cmake` es opcional; `run.sh` recurre a una
  compilación directa cuando CMake no está.

No necesitas saber nombres decorados, ABI ni cómo `new` encuentra memoria. Necesitas leer una
cabecera, un archivo `.cpp` y un diagnóstico del compilador.

### Qué necesitas

- La línea base con TAD:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/board.h`](code/board.h),
  [`code/board.cpp`](code/board.cpp), [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- La línea base anterior, para la comparación:
  [`../construction-08-modular/code/board.h`](../construction-08-modular/code/board.h) y
  [`../construction-08-modular/code/board.cpp`](../construction-08-modular/code/board.cpp).
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
test -f ../construction-08-modular/code/board.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

Si falta un archivo, la comparación no tiene segundo lado. Registra el archivo ausente en
`EVIDENCE-es.md` §11 y vuelve cuando esté disponible. Un conteo contra un archivo que no has leído
no es evidencia.

Ambas líneas base reproducen las 20 trazas de `common/traces/` byte a byte antes de que cambies
nada. El mismo juego, cinco funciones libres contra siete operaciones sobre un manejador. Eso es lo
que hace justa la comparación.

## Paso 1 — Inventariar el tipo incompleto (~12 min)

Lee `board.h` primero, y después deja que el compilador te diga dónde está la definición.

```bash
grep -n . code/board.h
grep -rn 'struct Board' code/
```

El primer comando imprime 11 líneas. El segundo imprime dos coincidencias: la declaración adelantada
en `board.h` y la definición en `board.cpp`. Dos coincidencias, dos archivos distintos, y ese es el
diseño.

Ahora el grafo de inclusión, tal como lo ve el compilador:

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

`game.cpp` incluye `board.h`, así que ve `Board` y las siete operaciones. No incluye `board.cpp`,
así que no ve la definición. Ese único `#include` que falta es la abstracción.

Rellena esta tabla en `EVIDENCE-es.md` §2.1 mientras lees.

| Cabecera | Líneas | Declaraciones | Tipo incompleto | Definición en |
|---|---|---|---|---|
| `board.h` | 1–11 | 7 | `Board` | `board.cpp` |
| `game.h` | 1–6 | 1 | ninguno | `game.cpp` |
| `random_sequence.h` | 1–7 | 3 | ninguno | `random_sequence.cpp` |
| `main` | sin cabecera | 0 | ninguno | `main.cpp` |

Conteos: 3 cabeceras, 4 unidades de traducción, 11 declaraciones públicas, 1 tipo incompleto, 30
definiciones, 18 auxiliares internos (17 en `game.cpp`, 1 en `main.cpp`) y 1 punto de entrada del
programa.

Las 30 definiciones se reparten así: 7 en `board.cpp`, 18 en `game.cpp`, 3 en
`random_sequence.cpp` y 2 en `main.cpp`. Compara con `construction-08`: 28 definiciones y 9
declaraciones públicas. El TAD añadió dos declaraciones públicas y dos definiciones, y cambió la
firma de cinco.

Después responde en `EVIDENCE-es.md` §2:

1. ¿Qué nombres conoce `main.cpp`?
2. ¿Qué nombres obtiene `game.cpp` de `board.h`, y qué nombres del tablero sigue sin poder ver?
3. ¿Cuántos bytes ocupa `Board`, hasta donde `game.cpp` sabe?
4. ¿En qué archivo se define `struct Board`, y qué hay dentro?
5. ¿Qué añadió `board_create` que `construction-08` no tenía en absoluto?
6. ¿Qué dos operaciones reciben un `Board *` no constante, y por qué?
7. `board_row_count` no usa su parámetro. ¿Es un error?
8. ¿Cuál es la diferencia entre "el llamador no puede ver el diseño" y "el llamador no puede ver el
   objeto"?

La pregunta 3 tiene una respuesta precisa: cero información. `game.cpp` puede escribir
`sizeof(Board *)`, que es el tamaño de un puntero, pero no `sizeof(Board)`, que necesitaría el
tamaño del objeto.

La pregunta 7 es la comprobación de honestidad. No es un error para este juego, porque los conteos
son constantes de compilación en `board.cpp`. Pero es una señal: una constante disfrazada de método.
En el momento en que el tablero sea configurable (`construction-15`), el parámetro empieza a usarse
y la firma no cambia.

La pregunta 8 separa dos afirmaciones distintas. El llamador no puede leer los bytes. El llamador
puede copiar la dirección, compararla, guardarla y pasarla. Ocultar el diseño no es ocultar el
objeto.

## Paso 2 — Predecir antes de ejecutar (~15 min)

### 2.1 Las trazas

Escribe estas predicciones antes de ejecutar nada. Los dos archivos están delante; el ejercicio es
ver cuánto del comportamiento puedes explicar sin medirlo.

| Pregunta | Mi predicción |
|---|---|
| Línea base con TAD vs `seed-0001.txt` | |
| Línea base modular vs `seed-0001.txt` | |
| TAD vs modular, semilla 1 | |
| Semillas que coinciden, de 20, en cada línea base | |
| Primera línea que podría diferir, y por qué | |

La última fila es una trampa. No hay razón para que difiera ninguna línea: las reglas, el orden de
sorteos y las dimensiones del tablero no cambian. Lo único que se movió es dónde vive el arreglo. Si
una línea difiere, has encontrado un error en una de las dos líneas base, no una propiedad del
diseño.

### 2.2 La superficie del cambio

Vas a reemplazar el arreglo dentro de `board.cpp` por un `std::vector` plano más una función de
índice. Predice el coste antes de tocar la edición.

| Pregunta | Mi predicción |
|---|---|
| Archivos fuente editados | |
| Líneas editadas | |
| Cabeceras editadas | |
| Archivos `.cpp` editados fuera de `board.cpp` | |
| Objetos recompilados (de 4) | |
| ¿Compila sin otras ediciones? | |
| Semillas que coinciden, de 20 | |
| ¿El archivo objeto crece, encoge o queda igual? | |

La pregunta del tamaño es la que casi todo el mundo falla. Un vector necesita un puntero, un tamaño
y una capacidad, y cada acceso pasa por una llamada que el optimizador puede eliminar o no. Predice
la dirección antes de medirla.

### 2.3 Qué permite la cabecera

Predice qué expresiones compilan en cada unidad de traducción, antes de comprobarlo con el
compilador. Escribe `sí`, `no` o `error de enlace`.

| Expresión | `main.cpp` | `game.cpp` | `board.cpp` |
|---|---|---|---|
| `Board *board = board_create();` | | | |
| `sizeof(Board)` | | | |
| `board->visited` | | | |
| `Board board;` | | | |
| `delete board;` | | | |
| `board_was_visited(board, 1, 1)` | | | |
| `Board *copy = board;` | | | |

`main.cpp` incluye solo `game.h`, así que ni siquiera ve el nombre `Board`. Esa fila es `no` por una
razón distinta a las demás: no es un tipo incompleto, es un nombre desconocido.

Escribe la tabla en `EVIDENCE-es.md` §3.3 antes de compilar. Las filas interesantes son las que son
`no` en una unidad y `sí` en otra.

## Paso 3 — Ejecutar ambas líneas base (~10 min)

Crea primero el directorio de trabajo; cada comando de abajo escribe ahí.

```bash
mkdir -p /tmp/adt-test
./run.sh --seed 1 --name Learner > /tmp/adt-test/adt-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/adt-test/adt-0001.txt
```

Después la línea base anterior:

```bash
cd ../construction-08-modular
./run.sh --seed 1 --name Learner > /tmp/adt-test/modular-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/adt-test/modular-0001.txt
```

Después las dos entre sí:

```bash
diff -u /tmp/adt-test/modular-0001.txt /tmp/adt-test/adt-0001.txt
```

Tres diferencias vacías. Vuelve a este directorio de la lección y confirma las 20 semillas en la
línea base con TAD:

```bash
cd ../construction-09-abstract-data-type
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

Un bucle silencioso son 20 coincidencias. Ejecuta el mismo bucle en `../construction-08-modular` si
no lo hiciste en esa lección.

Si `run.sh` no puede compilar, compila una vez a mano:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o build/adt-baseline code/main.cpp code/game.cpp code/board.cpp \
  code/random_sequence.cpp
```

Si una diferencia no está vacía, detente. Regístrala en `EVIDENCE-es.md` §11. No ajustes una traza
para que coincida con el código.

## Paso 4 — Demostrar que el tipo es incompleto (~12 min)

Las trazas dicen que las dos versiones se comportan igual. Este paso dice que la abstracción es
real, no una convención de nombres. Escribe la sonda en el directorio de trabajo; nunca en los
archivos versionados.

```bash
mkdir -p /tmp/adt-test/probe
cat > /tmp/adt-test/probe/probe.cpp <<'EOF'
#include "board.h"

int main()
{
    Board *board = board_create();
    int size = static_cast<int>(sizeof(Board));
    bool visited = board->visited;
    Board local;
    delete board;
    (void)local;
    return size + (visited ? 1 : 0);
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -Icode \
  -c /tmp/adt-test/probe/probe.cpp -o /tmp/adt-test/probe/probe.o
```

Al menos cuatro diagnósticos, uno por operación prohibida: `sizeof` sobre un tipo incompleto, acceso
a un miembro de un tipo incompleto, un objeto local de un tipo incompleto y `delete` sobre un
puntero a un tipo incompleto. El caso de `delete` suele producir dos, porque el compilador también
informa que lo apuntado no tiene definición. Una nota apunta a la declaración adelantada en
`board.h`. La redacción exacta depende del compilador y del idioma; registra la clase de cada
diagnóstico, no la frase.

Ahora compila el `board.cpp` real a un objeto y pregunta qué exporta:

```bash
mkdir -p /tmp/adt-test/objects
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -c code/board.cpp -o /tmp/adt-test/objects/board.o
nm -C --defined-only /tmp/adt-test/objects/board.o | grep ' T '
```

Forma esperada en esta plataforma:

```text
board_create()
board_destroy(Board*)
board_row_count(Board const*)
board_column_count(Board const*)
board_reset(Board*)
board_was_visited(Board const*, int, int)
board_mark_visited(Board*, int, int)
```

Siete símbolos, exactamente las siete declaraciones. Ningún diseño de `Board` aparece en la
superficie externa del objeto. El tipo es incompleto en la cabecera y completo en un solo archivo
objeto, y el objeto exporta solo las operaciones.

### 4.1 Los nombres decorados, y por qué el cambio es seguro

`nm` sin `-C` muestra los nombres decorados (mangled names):

```bash
nm --defined-only /tmp/adt-test/objects/board.o | grep ' T '
```

```text
_Z12board_createv
_Z13board_destroyP5Board
_Z15board_row_countPK5Board
_Z18board_column_countPK5Board
_Z11board_resetP5Board
_Z17board_was_visitedPK5Boardii
_Z18board_mark_visitedP5Boardii
```

Lee la codificación: `P5Board` es "puntero a un tipo cuyo nombre es `Board` y cuya longitud es 5".
`K` marca `const`. `ii` marca dos parámetros `int`. Nada en el nombre registra un tamaño, un campo
ni una dimensión de arreglo. Por eso el enlazador no tiene nada que renegociar cuando cambia la
representación: el símbolo al que un llamador hace referencia es la misma cadena antes y después del
cambio.

Compara con `construction-08`:

```text
_Z11board_resetv
_Z15board_row_countv
_Z17board_was_visitedii
_Z18board_column_countv
_Z18board_mark_visitedii
```

Cinco símbolos, y la `v` significa "sin parámetros". El tablero no estaba en la firma en absoluto.
Después de esta lección está en todas las firmas, como un nombre sin tamaño.

Rellena esta tabla en `EVIDENCE-es.md` §5.2:

| Cabecera | Declaraciones | Objeto | Símbolos externos | ¿Diseño en el nombre del símbolo? |
|---|---|---|---|---|
| `board.h` | 7 | `board.o` | | |
| `game.h` | 1 | `game.o` | | |
| `random_sequence.h` | 3 | `random_sequence.o` | | |

La última columna es el punto del paso. Si algún símbolo exportado codificara el diseño, el cambio
alteraría la ABI y todos los llamadores tendrían que recompilarse y reenlazarse. No lo hace.

## Paso 5 — Cambiar la representación (~25 min)

Copia la línea base. Los archivos versionados quedan intactos.

```bash
lessonDirectory="$(pwd)"
cp -r code /tmp/adt-test/flat
cd /tmp/adt-test/flat
```

Reemplaza `board.cpp` por este archivo. `board.h`, `game.cpp`, `main.cpp` y los dos archivos de la
secuencia aleatoria no se tocan.

```cpp
#include "board.h"

#include <vector>

namespace
{
constexpr int activeBoardRowCount = 8;
constexpr int activeBoardColumnCount = 8;

int cellIndex(int row, int column)
{
    return row * (activeBoardColumnCount + 1) + column;
}
}

struct Board
{
    std::vector<unsigned char> visited;
};

Board *board_create()
{
    Board *board = new Board{};
    board->visited.assign((activeBoardRowCount + 1) * (activeBoardColumnCount + 1), 0);
    return board;
}

void board_destroy(Board *board)
{
    delete board;
}

int board_row_count(const Board *)
{
    return activeBoardRowCount;
}

int board_column_count(const Board *)
{
    return activeBoardColumnCount;
}

void board_reset(Board *board)
{
    board->visited.assign((activeBoardRowCount + 1) * (activeBoardColumnCount + 1), 0);
}

bool board_was_visited(const Board *board, int row, int column)
{
    if (row < 1 || row > activeBoardRowCount || column < 1 ||
        column > activeBoardColumnCount)
    {
        return false;
    }
    return board->visited[cellIndex(row, column)] != 0;
}

void board_mark_visited(Board *board, int row, int column)
{
    if (row < 1 || row > activeBoardRowCount || column < 1 ||
        column > activeBoardColumnCount)
    {
        return;
    }
    board->visited[cellIndex(row, column)] = 1;
}
```

Cambiaron tres cosas dentro del archivo, y nada más:

1. El almacenamiento: `bool visited[16][16]` pasó a `std::vector<unsigned char>`. Las constantes de
   dimensión máxima desaparecieron por completo, porque un vector crece. Los conteos activos
   quedaron.
2. Un nuevo auxiliar interno `cellIndex` traduce una fila y una columna a un índice plano. El paso
   es `activeBoardColumnCount + 1`, o sea 9, así que la fila 1 columna 1 es el índice 10 y la fila 8
   columna 8 es el índice 80. El `+ 1` deja la fila 0 y la columna 0 reservadas y sin uso,
   exactamente como el arreglo.
3. `board_create` y `board_reset` ahora asignan. Un vector construido por defecto está vacío, así
   que `assign` es lo que hace válido al objeto.

Por qué `unsigned char` y no `bool`: `std::vector<bool>` es una especialización empaquetada en bits.
Funciona, pero `board->visited[indice]` devuelve un objeto proxy en lugar de un `bool &`, lo que
rompe en el momento en que intentas ligar una referencia. `unsigned char` mantiene cada elemento
como un byte real. La variante anidada de abajo usa `std::vector<bool>` a propósito, para mostrar
que la diferencia es invisible para los llamadores.

### 5.1 Compilar y ejecutar la variante plana

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o flat-baseline main.cpp game.cpp board.cpp random_sequence.cpp
cd "${lessonDirectory}"
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/adt-test/flat/flat-baseline --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

Un bucle silencioso son 20 de 20. La misma transcripción, otro almacenamiento.

### 5.2 Una segunda representación: vector de vectores

Copia la línea base otra vez y usa un diseño distinto. Este se parece más al original, porque
conserva dos subíndices.

```bash
cp -r "${lessonDirectory}/code" /tmp/adt-test/nested
cd /tmp/adt-test/nested
```

Solo cambian tres regiones de `board.cpp`:

```cpp
struct Board
{
    std::vector<std::vector<bool>> visited;
};

Board *board_create()
{
    Board *board = new Board{};
    board->visited.assign(activeBoardRowCount + 1,
                          std::vector<bool>(activeBoardColumnCount + 1, false));
    return board;
}

void board_reset(Board *board)
{
    for (std::vector<bool> &rowCells : board->visited)
    {
        rowCells.assign(rowCells.size(), false);
    }
}
```

`board_was_visited` y `board_mark_visited` conservan sus comprobaciones de límites y leen
`board->visited[row][column]` en lugar de `board->visited[cellIndex(row, column)]`. Las constantes
`maximum*` también desaparecen aquí.

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o nested-baseline main.cpp game.cpp board.cpp random_sequence.cpp
cd "${lessonDirectory}"
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/adt-test/nested/nested-baseline --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

Silencio otra vez: 20 de 20. Dos diseños distintos, una transcripción sin cambios.

### 5.3 Qué muestra el objeto

Compila los tres `board.cpp` a objetos y compáralos:

```bash
for variant in code /tmp/adt-test/flat /tmp/adt-test/nested; do
  name="$(basename "${variant}")"
  g++ -std=c++20 -O2 -I"${variant}" -c "${variant}/board.cpp" \
    -o "/tmp/adt-test/objects/${name}-board.o"
done
size /tmp/adt-test/objects/*-board.o
for name in code flat nested; do
  nm --defined-only "/tmp/adt-test/objects/${name}-board.o" | grep ' T ' |
    awk '{print $3}' > "/tmp/adt-test/${name}.sym"
done
diff /tmp/adt-test/code.sym /tmp/adt-test/flat.sym &&
  diff /tmp/adt-test/code.sym /tmp/adt-test/nested.sym &&
  echo "identical symbols"
```

El `awk` descarta la columna de dirección, que difiere porque cada objeto distribuye su propia
sección de texto. Compara nombres, no direcciones.

Dos resultados, ambos dignos de registro:

- Los símbolos exportados son idénticos byte a byte en los tres objetos. El cambio es invisible para
  el enlazador.
- El tamaño del texto no lo es. En esta plataforma: 504 bytes con el arreglo, 1311 con el vector
  plano y 4172 con el vector de vectores. La abstracción cuesta asignación e indirección, y no le
  oculta ese coste al tiempo de ejecución del programa.

Registra ambos números en `EVIDENCE-es.md` §6.3. Un diseño que oculta un coste del código fuente
sigue pagándolo en ejecución.

### 5.4 La trampa que las trazas no pueden atrapar

Borra el `assign` de `board_create` en la variante plana, y déjalo en `board_reset`:

```cpp
Board *board_create()
{
    return new Board{};
}
```

Recompila y vuelve a ejecutar las 20 trazas. Siguen pasando, las 20. La razón es el orden de
llamadas en `run_game`: `board_create()`, después `board_reset(board)`, después la primera lectura.
`board_reset` asigna, así que el vector vacío nunca llega a un accesor.

La traza es un contrato de comportamiento. Dice que la transcripción coincide. No dice que el
objeto haya sido válido alguna vez entre `create` y `reset`. En la versión con arreglo,
`new Board{}` inicializa el arreglo a cero, así que una lectura antes de `reset` estaría definida y
devolvería `false`. En la versión plana es un acceso fuera de límites sobre un vector vacío, que es
comportamiento indefinido que da la casualidad de no fallar. Las mismas trazas, otro contrato.

Registra esto en `EVIDENCE-es.md` §6.5. Es la razón de que exista `construction-10`: un tipo
abstracto de datos sin invariante es un diseño que no puedes ver, no una garantía en la que puedas
confiar.

### 5.5 La respuesta archivo por archivo

Rellena esta tabla en `EVIDENCE-es.md` §6.4. Es la respuesta directa al criterio de aceptación.

| Archivo | ¿Editado? | ¿Recompilado? |
|---|---|---|
| `code/board.cpp` | sí, la representación | sí |
| `code/board.h` | no | no |
| `code/game.cpp` | no | no |
| `code/game.h` | no | no |
| `code/main.cpp` | no | no |
| `code/random_sequence.cpp` | no | no |
| `code/random_sequence.h` | no | no |

Siete archivos fuente, uno editado, uno recompilado, después el enlace. `game.cpp` no necesita
edición porque nunca escribió `board->visited`; solo llamó a las operaciones. Ese es el criterio de
aceptación, y es una afirmación sobre una unidad de traducción, no sobre el buen gusto.

## Paso 6 — Comparar la superficie del cambio (~10 min)

La prueba de cambio del recorrido pide cuatro números. Esta tabla añade los que importan para este
cambio.

| Métrica | `construction-08` (modular) | `construction-09` (TAD) |
|---|---|---|
| Archivos tocados para el cambio | 1 (`board.cpp`) | 1 (`board.cpp`) |
| Líneas cambiadas | 25 | 25 plana, 17 anidada |
| Firmas cambiadas | 0 | 0 |
| Conceptos nuevos necesarios | 0 | 0 |
| Unidades de traducción recompiladas | 1 de 4 | 1 de 4 |
| Trazas de 8×8 rotas | 0 | 0 |
| Tableros vivos a la vez | 1 | cualquier número |
| Ciclo de vida del tablero | estático, todo el programa | `create` / `destroy` explícitos |
| Operaciones que reciben un tablero | 0 de 5 | 7 de 7 |
| Una prueba puede crear un tablero limpio | no, solo reiniciar el global | sí |

Lee con cuidado las seis primeras filas. Son **empates**. Para este cambio concreto, el tablero
modular y el tablero con TAD cuestan lo mismo: un archivo, casi las mismas líneas, ningún cambio de
firma, ninguna recompilación de llamadores, ninguna traza rota. Si la lección terminara aquí, el TAD
parecería un refactor con ceremonia extra.

Las cuatro últimas filas son donde vive la diferencia. No son sobre el cambio; son sobre para qué es
seguro el cambio. En `construction-08` el arreglo era privado de `board.cpp`, así que cambiarlo ya
era seguro para el único tablero que existía. En `construction-09` el cambio es seguro para todos
los tableros que el programa pueda crear, porque ningún llamador tuvo nunca una forma de ver uno.

Lo que el TAD compró:

- Un valor. El tablero se puede pasar, guardar, comparar y crear a demanda.
- Multiplicidad. Dos tableros en un proceso ya no chocan.
- Un ciclo de vida explícito. El objeto empieza en `board_create` y termina en `board_destroy`, y
  el hueco entre esas dos llamadas ahora es una pregunta de diseño en lugar de un hecho estático.
- Una costura para pruebas. Una prueba puede construir un tablero 8×8 limpio en lugar de reiniciar
  un global.

Lo que el TAD costó:

- Un parámetro en siete operaciones, y 56 líneas cambiadas en `game.cpp` para enhebrar el manejador
  por 6 firmas y 19 puntos de llamada. Mídelo tú con
  `diff -u ../construction-08-modular/code/game.cpp code/game.cpp`.
- Dos operaciones nuevas y un tipo incompleto en la cabecera: 4 líneas más de cabecera.
- Una obligación de propiedad sin imposición. El llamador debe llamar a `board_destroy`, y nada
  comprueba que lo haga. Una excepción lanzada entre `create` y `destroy` filtra el tablero.
- Una superficie de punteros nulos. Algunas operaciones sobreviven a `nullptr`, otras no, y la
  cabecera no dice cuáles.
- Más texto por operación. 504 bytes pasaron a 1311, o a 4172.

La conclusión honesta: para un solo tablero en un programa de un solo hilo, el TAD es un coste
pequeño sin beneficio medible. Su beneficio aparece en el momento en que existe un segundo tablero,
o una prueba necesita uno, o un llamador necesita poseer uno. Por eso la siguiente lección trata de
invariantes y la posterior de propiedad: un tipo opaco sin esas dos cosas es una promesa sin nada
detrás.

## Paso 7 — Comparar y explicar (~10 min)

Escribe `EVIDENCE-es.md` §8. Dos partes.

**Primero, una frase.** Di qué decide la declaración sobre un llamador, sin usar las palabras
*cabecera*, *archivo*, *módulo*, *compilar*, *puntero*, *struct*, *clase*, *opaco*,
*encapsulación*, *mejor*, *peor*, *limpio*, *más limpio*, *bonito*, *elegante*, *feo* ni *modular*.
Si tu frase necesita una de esas, has descrito la sintaxis en lugar de la decisión.

**Segundo, un párrafo.** Responde:

- Qué fija la declaración: el nombre del tipo, las siete operaciones y cuáles de ellas pueden
  escribir.
- Qué oculta: el tamaño, los campos, el almacenamiento, las dimensiones máximas, la función de
  índice.
- Qué impone el compilador: ningún objeto, ningún `sizeof`, ningún acceso a miembros, ningún
  `delete` fuera de `board.cpp`.
- Qué ve el enlazador: los mismos siete nombres de símbolo antes y después del cambio.
- Qué prueba una ejecución: 20 de 20 antes del cambio, 20 de 20 después, con dos diseños distintos.
- Qué **no** prueba una ejecución: que el objeto fuera válido entre `create` y `reset`, que el
  manejador tenga un solo dueño, ni que un manejador nulo sea rechazado.
- Qué añade la multiplicidad: cualquier número de tableros, cada uno con su propio ciclo de vida.
- Qué cuesta: un parámetro por operación, 56 líneas cambiadas en `game.cpp`, una obligación de
  propiedad, una superficie de nulos y un archivo objeto más grande.
- Qué lección posterior añade invariantes: `construction-10`; encapsulación: `construction-11`;
  propiedad y ciclo de vida: `construction-16`; puertos y adaptadores: `construction-20`.

Una respuesta fuerte mantiene separadas cuatro cosas: **qué permite la declaración**, **qué oculta
la definición**, **qué comprueba la cadena de herramientas** y **qué muestra una ejecución o una
medición de tamaño**. Una respuesta débil dice que el tipo está encapsulado, lo que oculta las
cuatro.

## Paso 8 — Comprobación de aceptación y registro (~3 min)

El criterio de aceptación de esta lección es: **cambiar la representación del tablero dentro de
`board.cpp` y mostrar que todos los llamadores siguen compilando y las 20 trazas siguen pasando.**

Comprueba tu registro contra él:

- [ ] Ejecuté ambas líneas base sin modificar y obtuve tres diferencias vacías, más 20 semillas
      coincidentes en cada una.
- [ ] Inventarié las 11 declaraciones públicas y localicé el único tipo incompleto.
- [ ] Mostré al menos un diagnóstico del compilador por cada operación prohibida sobre `Board`.
- [ ] Listé los siete símbolos exportados de `board.o` y leí los nombres decorados.
- [ ] Expliqué por qué ningún nombre de símbolo codifica el diseño.
- [ ] Predije las trazas y la superficie del cambio antes de medir.
- [ ] Reemplacé el arreglo por un vector plano y una función de índice.
- [ ] Lo reemplacé otra vez por un vector de vectores, para mostrar que la elección es interna.
- [ ] `board.h`, `game.h`, `game.cpp`, `main.cpp` y los dos archivos de la secuencia aleatoria no se
      editaron.
- [ ] Solo se recompiló `board.o`, y después el enlace.
- [ ] Obtuve 20 de 20 coincidencias antes del cambio y 20 de 20 después.
- [ ] Registré el tamaño del objeto para las tres representaciones y no llamé gratis al cambio.
- [ ] Quité la inicialización de `board_create` y expliqué por qué las trazas siguen pasando.
- [ ] Medí la superficie del cambio en ambas líneas base y dije dónde empatan y dónde no.
- [ ] Expliqué qué permite la declaración, qué oculta y qué no impone ninguna herramienta.
- [ ] Mi frase evita las seis palabras de sintaxis y las ocho palabras de veredicto.
- [ ] No edité ningún archivo de código versionado.
- [ ] Completé `EVIDENCE-es.md` §1–§10 y la autocomprobación de §12.

El criterio no es "un tipo opaco es mejor". Es "puedes decir qué unidad de traducción posee el
diseño, qué comprobación notaría si un llamador intentara leerlo, y qué no prueba el cambio".

## Autocomprobación (binaria)

- [ ] Leí los siete archivos fuente y comparé `board.h` con la versión de `construction-08`.
- [ ] Predije las trazas, la superficie del cambio y la tabla de visibilidad antes de medir.
- [ ] Compilé y ejecuté ambas líneas base y reproduje las 20 trazas en cada una.
- [ ] Compilé una sonda que falla en `sizeof`, acceso a miembros y `delete`.
- [ ] Inspeccioné los siete símbolos exportados y sus nombres decorados.
- [ ] Cambié la representación dos veces, solo dentro de `board.cpp`.
- [ ] Conté un archivo editado, un objeto recompilado y un reenlace.
- [ ] Vi 20 de 20 trazas coincidir después de los dos cambios.
- [ ] Registré el tamaño del objeto para el arreglo y para los dos vectores.
- [ ] Encontré la inicialización que las trazas no pueden comprobar.
- [ ] Comparé la superficie del cambio y nombré las filas que empatan.
- [ ] Completé `EVIDENCE-es.md`.

Estos son criterios de aceptación. Si una casilla queda sin
marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §10.

## Misconcepciones que esta lección refuta

| Error de concepto | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| "Una declaración adelantada basta para usar el tipo." | Basta para sostener un puntero o una referencia, no para crear, medir, copiar ni destruir el objeto. | Enumera las cinco operaciones prohibidas y comprueba cada una con el compilador. |
| "La cabecera oculta el tipo." | Oculta el diseño. El nombre, la dirección y las siete operaciones son públicos. | Di qué hechos están ocultos y cuáles no. |
| "El TAD es lo que hace posible el cambio de representación." | `construction-08` ya podía cambiar su arreglo privado sin editar llamadores. | Compara las dos líneas base y encuentra las filas que empatan. |
| "Un tipo opaco impide el mal uso." | `board_reset(nullptr)` y un doble `board_destroy` siguen siendo alcanzables. | Trata el invariante como tarea de `construction-10` y la propiedad como tarea de `construction-16`. |
| "`const Board *` hace inmutable el objeto." | Hace que esta llamada no pueda escribir a través de este puntero. Otro manejador sí puede. | Lee `const` como una promesa sobre el parámetro, no sobre el ciclo de vida del objeto. |
| "`board_row_count` necesita el tablero." | El parámetro no tiene nombre y se ignora; el conteo es una constante de archivo. | Nota el olor y espera que desaparezca cuando el tablero sea configurable. |
| "El cambio es gratis porque el código fuente no cambió." | El archivo objeto creció de 504 a 1311 o 4172 bytes de texto. | Mide con `size`, no con la diferencia. |
| "20 de 20 trazas significa que el cambio es correcto." | Las trazas pasaron con un vector sin inicializar en `board_create`. | Añade la comprobación del invariante; una traza no es una prueba de seguridad de memoria. |
| "El nombre decorado cambia cuando cambia el diseño." | `P5Board` codifica el nombre y el puntero, no el tamaño. | Lee un nombre decorado y señala el diseño que falta. |
| "Copiar el manejador es copiar el tablero." | `Board *segundo = board;` copia la dirección. Los dos manejadores ahora poseen un objeto. | Di "manejador" y "objeto" por separado, y deja que `construction-16` lo arregle. |
| "El TAD hace el código más rápido o más pequeño." | Las versiones con vector son más grandes y asignan memoria. | Separa la propiedad de diseño de la propiedad de rendimiento. |
| "Un struct con campos ocultos es un TAD." | Si el llamador puede hacer `sizeof`, ponerlo en la pila o copiarlo, el diseño es público. | Prueba las cinco operaciones prohibidas; esa es la definición. |

## Siguiente paso

`construction-10` — invariantes del tipo abstracto de datos. El tablero es opaco, pero nada impide
que `board_create` devuelva un objeto que nunca se inicializó, y nada impide que un llamador
destruya un manejador dos veces. La siguiente lección le da al tipo una regla de validez y un ciclo
de vida, para que la promesa de la cabecera tenga algo detrás.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog-es.md) para el recorrido
completo.

Si faltaba alguna de las dos líneas base, no empieces `construction-10`. El siguiente contraste
necesita los dos lados.
