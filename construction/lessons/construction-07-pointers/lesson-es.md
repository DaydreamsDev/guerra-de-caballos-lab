# Punteros (pointers)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** indirección y ciclo de vida · **Tiempo:** unos 88 minutos ·
**Prerrequisitos:** construction-06 · **Estado:** borrador
**Lectura:** ~18 minutos · **Prerrequisitos:** construction-06
**Rol de la lección:** tronco

## Por qué existe esta lección

`construction-06` le dio al caballo seleccionado un nombre que no podía ser nulo, no podía
reasignarse y no se veía en el sitio de llamada. Ese nombre alcanzaba para mover un caballo, pero
ocultaba dos hechos: dónde vive el registro y cuánto vive.

La línea base con punteros sustituye cada referencia de caballo por un puntero, añade una función
que devuelve la dirección de una casilla del tablero y deja el juego intacto. Las 20 trazas siguen
siendo idénticas byte a byte. El diff tiene 40 líneas añadidas y 34 eliminadas, 6 netas; aparece una
función (`getBoardCell`), las 22 definiciones pasan a 23 y los 72 parámetros pasan a 75.

La frase que hay que retener:

> Un valor que nombra una ubicación puede estar vacío, puede redirigirse y puede sobrevivir a lo que
> nombra; el tipo dice qué hay allí, no si sigue siendo válido.

Se siguen dos mediciones. La primera es estructural: reescribe un acceso al tablero como
`rowStart + column`, y las 20 trazas siguen idénticas porque la dirección es la misma. La segunda es
aritmética: la casilla (2,3) está a 21 `BoardCell` de (0,0) cuando el ancho de fila es 9, y a 25
cuando es 11. Un paso escrito como el literal `9` coincide con el primer diseño y lee mal el segundo
en silencio.

Una nota honesta antes de la comparación. La lección anterior prometió que el sitio de llamada
mostraría el `&`. Lo hace donde se toma la dirección, en `main`:

```cpp
initializePositions(&playerKnight, &machineKnight);
```

No lo hace dentro de `playTurn`, donde la dirección ya se tiene:

```cpp
moveKnight(activeKnight, destinationRow, destinationColumn);
```

Esa línea se lee igual que la versión con referencias. Un puntero hace visible la indirección donde
se crea la dirección, no en cada uso. La otra nota honesta: los punteros ya estaban en este archivo.
`char **argumentValues`, `const char *activeKnightName` y
`BoardCell boardCells[][boardColumnCount + 1]` son tipos puntero, y el último es un puntero a una
fila. Esta lección no introduce el token `*`. Lleva los alias de caballo y un acceso al tablero al
lado de los punteros, y hace visibles el diseño y el ciclo de vida.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar indirección, ciclo de vida y el paso de fila;
- acceder a una casilla por puntero y mantener las trazas sin cambios.

## El concepto: un valor que nombra una ubicación

Tres formas nombran un caballo, y una forma nueva nombra una casilla del tablero:

```text
parámetro por valor      void moveKnight(Knight knight, ...)      la función posee una copia
parámetro de referencia  void moveKnight(Knight &knight, ...)     otro nombre; debe vincularse
parámetro de puntero     void moveKnight(Knight *knight, ...)     una ubicación; puede ser nula
retorno de puntero       Knight *selectActiveKnight(...)          el llamador sostiene una ubicación
retorno de casilla       BoardCell *getBoardCell(...)             una ubicación dentro del tablero
```

Lado a lado, los sitios que nombran al caballo y a la casilla:

| Decisión | `construction-06` | `construction-07` |
|---|---|---|
| Caballo activo | `Knight &` devuelto por `selectActiveKnight` | `Knight *` devuelto por `selectActiveKnight` |
| Otro caballo | `const Knight &` devuelto por `selectOtherKnight` | `const Knight *` devuelto por `selectOtherKnight` |
| Movimiento | `moveKnight(Knight &, ...)` | `moveKnight(Knight *, ...)` |
| Casilla | `boardCells[row][column]` | `getBoardCell(...)` que devuelve `BoardCell *` |

Las dos líneas base difieren en una decisión: cómo se nombran el caballo seleccionado y la casilla.
Diez firmas cambian un parámetro o un retorno de caballo, y ninguna cambia su cantidad de
parámetros. Tres sitios de llamada ganan un `&`; tres expresiones del tablero ganan una llamada al
accesor. Ningún argumento existente se reordena y ninguna sentencia del cuerpo desaparece salvo el
índice directo del tablero. La transcripción no se mueve.

Lado a lado, la diferencia es lo que permite el tipo:

| Pregunta | `Knight &` | `Knight *` |
|---|---|---|
| Qué guarda el parámetro | otro nombre | una ubicación |
| Una escritura dentro de la función | cambia el registro del llamador | cambia el objeto apuntado |
| Qué ve el llamador después de la llamada | los valores nuevos de los campos | los valores nuevos de los campos |
| Qué dice el sitio de llamada sobre la mutación | nada | `&` donde se toma la dirección, nada después |
| Puede el parámetro ser nulo | no | sí |
| Puede reasignarse después | no; la asignación copia el registro | sí; la asignación redirige el nombre |
| Qué cuesta la llamada | un nombre pasado | una dirección pasada |
| Qué comprueba el compilador | vínculo y `const` | compatibilidad del objeto apuntado y `const` |
| Qué no puede comprobar el compilador | el ciclo de vida | nulo, límites, ciclo de vida entre ámbitos |

Lo que un puntero **no** es, en esta lección:

- **No es un entero.** Es una ubicación con tipo. `rowStart + 1` avanza `sizeof(BoardCell)`, no un
  byte.
- **No siempre es válido.** El nulo es representable, y también lo es una ubicación que dejó de ser
  válida.
- **No es un propietario.** Aquí no se reserva ni se libera memoria. La propiedad es
  `construction-16`.
- **No comprueba límites.** `rowStart + 9` compila aunque la fila tenga índices `0..8`.
- **No es una referencia.** Se puede comparar, copiar, reasignar y poner a nulo.
- **No es visible en cada llamada.** El `&` aparece donde se toma la dirección; después, la variable
  puntero la lleva en silencio.
- **No es una medida de seguridad.** Le da a la función todos los campos del objeto apuntado y no
  añade ningún invariante. Eso empieza en `construction-10` y `construction-11`.

Se siguen tres consecuencias:

1. **La selección tiene un solo hogar.** `selectActiveKnight` devuelve una ubicación, así que
   `playTurn` no repite el ternario que elige entre los dos registros.
2. **La escritura tiene un solo hogar.** `moveKnight` escribe los dos campos a través del puntero.
3. **El acceso al tablero tiene un solo hogar.** `getBoardCell` calcula la dirección una vez, y el
   resto del juego lee o escribe `->visited`.

El costo es lo cuarto: el nulo, la reasignación y el ciclo de vida pasan a ser responsabilidad de
quien programa. El tipo dice qué hay en la ubicación; no dice si la ubicación es válida.

## Antes de empezar

- [`construction-06`](../construction-06-references/lesson-es.md) — tienes sus alias de referencia,
  su frontera `const` y su experimento por valor.
- Un compilador de C++20 (`g++` o `clang++`). `cmake` es opcional.

No necesitas saber C++ a fondo. Necesitas leer una lista de parámetros, un tipo de retorno y una
expresión con `*` o `->`. La lección trata sobre lo que esas marcas permiten, no sobre cómo se
escriben.

### Qué necesitas

- La línea base con punteros: [`code/pointers-baseline.cpp`](code/pointers-baseline.cpp).
- La línea base con referencias:
  [`references-baseline.cpp`](../construction-06-references/code/references-baseline.cpp).
- Los scripts de compilación y las notas: [`run.sh`](run.sh) y [`README-es.md`](README-es.md).
- Las reglas: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §12 fija el
  orden de los sorteos.
- El formato de la transcripción:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- Una traza de referencia: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- Esta plantilla para quien aprende: [`EVIDENCE-es.md`](EVIDENCE-es.md).

Comprueba que los artefactos existen antes de empezar, desde este directorio de la lección:

```bash
test -f code/pointers-baseline.cpp &&
test -f ../construction-06-references/code/references-baseline.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

Si falta cualquiera de las dos líneas base, la comparación no tiene segundo lado. Registra el
archivo ausente en `EVIDENCE-es.md` §11 y vuelve cuando esté disponible. Un conteo contra un archivo
que no has leído no es evidencia.

Ambas líneas base reproducen las 20 trazas de `common/traces/` byte a byte antes de que cambies
nada. El mismo juego, dos formas de nombrar al caballo y a la casilla. Eso es lo que hace justa la
comparación.

## El diseño: por filas, nueve casillas de ancho

El tablero se declara con una fila y una columna centinela:

```cpp
BoardCell boardCells[boardRowCount + 1][boardColumnCount + 1] = {};
```

Con `boardRowCount` y `boardColumnCount` en 8, son 9 filas de 9 casillas: 81 casillas, de las cuales
se usan 64 (filas `1..8`, columnas `1..8`). La fila 0 y la columna 0 existen para que el juego
numere las casillas desde 1 sin restar uno en cada acceso.

```text
índice:     0      1      2     ...     8
fila 0:  [0,0]  [0,1]  [0,2]   ...   [0,8]    fila centinela
fila 1:  [1,0]  [1,1]  [1,2]   ...   [1,8]
fila 2:  [2,0]  [2,1]  [2,2]   ...   [2,8]
...
fila 8:  [8,0]  [8,1]  [8,2]   ...   [8,8]
```

Las casillas están en un bloque contiguo, fila tras fila. La dirección de `(row, column)` es:

```text
base + (row * 9 + column) * sizeof(BoardCell)
```

Ese `9` es el ancho de fila, no la cantidad de columnas. Es `boardColumnCount + 1`, y es el paso
entre una fila y la siguiente. Eso significa **por filas** (row-major): el índice de fila avanza
filas completas y el de columna avanza una casilla dentro de la fila actual.

Dos expresiones exponen el diseño:

```cpp
BoardCell *rowStart = boardCells[row];      // un BoardCell[9] decae a BoardCell *
BoardCell *cell = rowStart + column;        // column casillas adelante, no column bytes
```

`boardCells[row]` no es una casilla; es una fila, y decae a un puntero a su primera casilla.
`rowStart + column` es la dirección de `boardCells[row][column]`, porque la aritmética de punteros
cuenta elementos del tipo apuntado.

El tipo del parámetro conserva el ancho:

```cpp
BoardCell boardCells[][boardColumnCount + 1]
```

Un parámetro de arreglo decae, pero solo su dimensión más externa. Lo que recibe la función es
`BoardCell (*)[9]`: un puntero a una fila de 9 casillas. La aritmética sobre ese tipo avanza una
fila completa. Por eso el compilador rechaza una llamada con un arreglo de otro tamaño.

`getBoardCell` devuelve `BoardCell *`. Ese tipo de retorno descarta el ancho. Desde el puntero
devuelto, `+1` es la columna siguiente, pero `+9` es la fila siguiente solo mientras el ancho sea 9.
El accesor oculta el índice bidimensional y, con él, el paso de fila.

## Paso 1 — Inventariar los punteros (~12 min)

Lee los dos archivos una vez. Después deja que el texto te diga dónde se nombra un caballo o una
casilla por una ubicación:

```bash
grep -n 'Knight \*' code/pointers-baseline.cpp
grep -n 'const Knight \*' code/pointers-baseline.cpp
grep -n 'BoardCell \*\|getBoardCell' code/pointers-baseline.cpp
```

El primer comando también encuentra las líneas `const Knight *`; el segundo imprime el subconjunto
de solo lectura. El tercero imprime el accesor y sus sitios de llamada.

Después lee el cambio completo en una sola vista:

```bash
diff -u ../construction-06-references/code/references-baseline.cpp \
  code/pointers-baseline.cpp
```

El cambio tiene cinco clases de bloque: los tipos de los parámetros de caballo, las reescrituras de
`.` a `->`, el accesor nuevo, las dos escrituras del tablero que pasaron a ser llamadas, y los tres
`&` añadidos en `main`.

Completa esta tabla en `EVIDENCE-es.md` §2.1 mientras lees. Los números de línea son de la línea
base con punteros.

| Función | Línea | Parámetros `Knight` | Forma | Lee | Escribe |
|---|---|---|---|---|---|
| `initializePositions` | 125 | 2 | `Knight *` | — | `row`, `column` |
| `printGameHeader` | 149 | 2 | `const Knight *` | `row`, `column` | — |
| `printTurnHeader` | 171 | 1 | `const Knight *` | `row`, `column` | — |
| `printMove` | 206 | 1 | `const Knight *` | `row`, `column` | — |
| `selectActiveKnight` | 218 | 2 | `Knight *`, devuelve `Knight *` | — | — (concede acceso de escritura) |
| `selectOtherKnight` | 223 | 2 | `const Knight *`, devuelve `const Knight *` | — | — |
| `moveKnight` | 229 | 1 | `Knight *` | — | `row`, `column` |
| `getBoardCell` | 235 | 0 | devuelve `BoardCell *` | la dirección de la casilla | — |
| `tryMove` | 240 | 2 | `const Knight *` | `row`, `column`; casilla | — |
| `playTurn` | 265 | 2 | `Knight *` | mediante auxiliares | mediante `moveKnight` y `getBoardCell` |
| `playGame` | 334 | 2 | `Knight *` | — | reenvía |
| `main` | 352 | 0 | declara dos registros | — | inicializa |

Recuentos: 9 parámetros de caballo mutables, 8 de solo lectura, 0 por valor, 2 retornos de puntero a
caballo, 3 retornos de puntero en total (el tercero es `BoardCell *`) y 3 sitios de llamada a
`getBoardCell`.

Dos filas merecen un minuto.

- `playGame` recibe los dos caballos como punteros mutables y no escribe ninguno. Los reenvía a
  `playTurn`. El puntero mutable es un paso de testigo, igual que la referencia mutable en
  `construction-06`. El tipo no dice "escribo"; dice "puedo".
- Las funciones de impresión reciben `const Knight *` y solo leen. Podrían recibir el caballo por
  valor y seguirían siendo correctas; la copia costaría ocho bytes por llamada sin beneficio, y el
  puntero marca la intención de solo lectura en la declaración. `const` está en el objeto apuntado,
  no en el puntero.

Después responde en `EVIDENCE-es.md` §2:

1. ¿Cuántas funciones reciben un caballo por puntero mutable? ¿Por puntero `const`? ¿Por valor?
2. ¿Qué dos funciones devuelven un puntero a caballo, y qué permite cada tipo de retorno?
3. ¿Qué función es la única que escribe los campos de un caballo a través de un parámetro?
4. ¿Qué función devuelve una casilla del tablero, y qué sabe y qué no sabe su tipo de retorno?
5. ¿En qué se convirtió cada `&` en la declaración, y dónde pasó un `.` a `->`?
6. ¿Qué sitios de llamada muestran ahora un `&`, y cuáles no?
7. Quedan doce parámetros de referencia en el archivo. ¿Cuáles son, y por qué no se convirtieron?
8. ¿Qué sintaxis de puntero ya existía en `construction-06` antes de esta lección?

La pregunta 4 es la pregunta del diseño. La pregunta 7 es la comprobación de honestidad: no todo
alias debe volverse puntero. `int &destinationRow` y `std::string &winnerName` son parámetros de
salida; no pueden faltar, y convertirlos en punteros añadiría el nulo como estado sin ganancia.

## Paso 2 — Predecir antes de ejecutar (~15 min)

### 2.1 El diff

Escribe estas predicciones antes de ejecutar `diff`. El archivo está delante; el ejercicio es ver
cuánto del cambio puedes explicar sin medirlo.

| Pregunta | Mi predicción |
|---|---|
| Líneas añadidas / eliminadas / netas | |
| Definiciones de función antes / después | |
| Parámetros antes / después | |
| Firmas que cambian un tipo de caballo | |
| Sitios de llamada que ganan un `&` | |
| Expresiones del tablero que pasan a llamar al accesor | |
| Parámetros de referencia que quedan | |
| Trazas que difieren, de 20 | |

### 2.2 Las direcciones del tablero

Responde sin compilar en `EVIDENCE-es.md` §3.2.

| Pregunta | Mi predicción |
|---|---|
| Desplazamiento en elementos de (2,3) desde (0,0), ancho 9 | |
| Desplazamiento en elementos de (2,3) desde (0,0), ancho 11 | |
| Qué es `boardCells[2]`, y a qué decae | |
| ¿`boardCells[2] + 3` es igual a `&boardCells[2][3]`? | |
| Qué suma `+ 3`: bytes o elementos | |
| A qué apunta `getBoardCell(...) + 9`, ancho 9 | |
| A qué apunta `getBoardCell(...) + 9`, ancho 11 | |
| Cuál de estas cosas comprueba el compilador | |

La unidad importa. `rowStart + column` suma `column` elementos de tipo `BoardCell`. En esta
plataforma `sizeof(BoardCell)` es 1 porque el único campo es un `bool`, así que elementos y bytes
coinciden por casualidad. No construyas una regla sobre esa coincidencia; el compilador puede añadir
relleno a la estructura más adelante.

### 2.3 Los límites

Responde ahora, sin probar:

- ¿Puede un `Knight *` ser nulo, de modo que "ningún caballo" sea representable?
- ¿Se puede hacer que `activeKnight` nombre al otro registro después de inicializarse?
- ¿Qué hace ahora `activeKnight = machineKnight;`, y qué hacía en `construction-06`?
- ¿Qué pasa si un puntero sobrevive al registro que apunta?
- ¿`moveKnight(activeKnight, ...)` le dice a quien lee que el objeto apuntado puede cambiar?
- ¿Cuál de estas cosas detecta el compilador?

Escribe las respuestas en `EVIDENCE-es.md` §3.3. El paso 6 las comprueba.

## Paso 3 — Ejecutar ambas líneas base (~8 min)

Crea primero el directorio de trabajo; todos los comandos siguientes escriben allí.

```bash
mkdir -p /tmp/pointer-test
./run.sh --seed 1 --name Learner > /tmp/pointer-test/pointers-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/pointer-test/pointers-0001.txt
```

Después la línea base anterior:

```bash
cd ../construction-06-references
./run.sh --seed 1 --name Learner > /tmp/pointer-test/references-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/pointer-test/references-0001.txt
```

Después las dos entre sí:

```bash
diff -u /tmp/pointer-test/references-0001.txt /tmp/pointer-test/pointers-0001.txt
```

Tres diffs vacíos. Vuelve a este directorio de la lección y confirma las 20 semillas, no solo la
primera:

```bash
cd ../construction-07-pointers
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
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o build/pointers-baseline code/pointers-baseline.cpp
```

Si un diff no está vacío, detente. Regístralo en `EVIDENCE-es.md` §11. No ajustes una traza para
que coincida con el código.

## Paso 4 — Reescribir un acceso al tablero (~15 min)

Copia la línea base. El archivo versionado no se toca.

```bash
cp code/pointers-baseline.cpp /tmp/pointer-test/row-start.cpp
```

En `tryMove`, sustituye la llamada al accesor por aritmética de punteros explícita:

```cpp
BoardCell *rowStart = boardCells[destinationRow];
BoardCell *destinationCell = rowStart + destinationColumn;
if (destinationCell->visited)
```

Compila y ejecuta las 20 semillas:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/pointer-test/row-start /tmp/pointer-test/row-start.cpp
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/pointer-test/row-start --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

Las 20 coinciden, porque la dirección es idéntica: `boardCells[row]` es la primera casilla de la
fila, y `+ column` avanza `column` casillas dentro de ella.

### 4.1 Medir el paso de fila

Escribe una sonda de trabajo con las mismas declaraciones e imprime los desplazamientos:

```cpp
#include <cstdio>

struct BoardCell
{
    bool visited;
};

constexpr int boardRowCount = 8;
constexpr int boardColumnCount = 8;

int main()
{
    BoardCell board[boardRowCount + 1][boardColumnCount + 1] = {};
    BoardCell *rowStart = board[2];
    BoardCell *cell = rowStart + 3;
    BoardCell *flat = &board[0][0] + 2 * (boardColumnCount + 1) + 3;
    std::printf("rowStart + 3 == &board[2][3]: %d\n", cell == &board[2][3]);
    std::printf("flat == &board[2][3]: %d\n", flat == &board[2][3]);
    std::printf("element offset: %td\n", &board[2][3] - &board[0][0]);
    std::printf("byte offset: %td\n",
                reinterpret_cast<const char *>(&board[2][3]) -
                reinterpret_cast<const char *>(&board[0][0]));
    std::printf("sizeof(BoardCell): %zu\n", sizeof(BoardCell));
    return 0;
}
```

Compílala con las opciones del proyecto y ejecútala:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/pointer-test/stride /tmp/pointer-test/stride.cpp
/tmp/pointer-test/stride
```

Números esperados en esta plataforma: las dos comparaciones imprimen `1`, el desplazamiento en
elementos es `21`, el desplazamiento en bytes es `21` y `sizeof(BoardCell)` es `1`. El
desplazamiento en elementos es `2 * 9 + 3`; el de bytes es ese valor multiplicado por el tamaño de
una casilla.

Cambia `boardColumnCount` a 10 en la sonda y ejecútala otra vez. El desplazamiento en elementos pasa
a `25` (`2 * 11 + 3`), y la comparación de `flat` sigue imprimiendo `1` solo porque la sonda
recalcula el paso desde la constante. Sustituye `(boardColumnCount + 1)` por el literal `9` y la
comparación imprime `0`: el puntero aplanado ahora apunta a la casilla equivocada.

Escribe `EVIDENCE-es.md` §5: la reescritura, el resultado de la compilación, las 20 coincidencias,
los cuatro números y la frase que separa un desplazamiento en elementos de uno en bytes.

## Paso 5 — Cambiar el ancho de fila (~15 min)

La reescritura del paso 4 todavía nombra la fila mediante `boardCells[row]`, así que el compilador
conoce el ancho. Ahora separa las dos formas de escribir el paso.

Copia la línea base otra vez y cambia una constante:

```bash
cp code/pointers-baseline.cpp /tmp/pointer-test/width-10.cpp
```

En esa copia, pon `boardColumnCount` en `10`. Compila con las opciones del proyecto:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/pointer-test/width-10 /tmp/pointer-test/width-10.cpp
```

Compila limpio. El juego ahora es de 8 filas por 10 columnas, así que las casillas iniciales, los
movimientos legales y la transcripción cambian; las 20 trazas difieren. Eso no es una regresión. Las
trazas de referencia codifican un tablero de 8×8, y esta sonda cambia el juego a propósito.
Regístralo como una observación estructural, no como una traza fallida.

Lo que muestra la sonda:

- `boardCells[row]` y `rowStart + column` siguen funcionando. El tipo del parámetro es ahora
  `BoardCell (*)[11]`, así que el compilador recalculó el paso desde la declaración nueva.
- Un acceso aplanado con el literal `9` sigue compilando y lee la casilla equivocada:

  ```cpp
  BoardCell *flatStart = &boardCells[0][0];
  BoardCell *wrongCell = flatStart + destinationRow * 9 + destinationColumn;
  ```

  En `(2,3)` con ancho 11, `wrongCell` es el elemento 21 del bloque, que es `(1,10)`, no `(2,3)` en
  el elemento 25. Sin diagnóstico, sin caída; solo la casilla equivocada.
- Un acceso aplanado que usa `boardColumnCount + 1` da la dirección correcta, pero ahora la función
  necesita la constante, o un parámetro extra `rowWidth`. Una función que recibe solo `BoardCell *`
  no puede recuperar el paso desde el puntero.

### 5.1 Contar dónde vive el diseño

Cuenta los lugares que nombran el ancho de fila, en las dos líneas base:

| Línea base | Expresiones de dos índices (cada una depende del paso) | Declaraciones con `[][boardColumnCount + 1]` |
|---|---|---|
| `construction-06` | 3 (`tryMove`, dos en `playTurn`) | 3 (`tryMove`, `playTurn`, `playGame`) |
| `construction-07` | 1 (`getBoardCell`) | 4 (`getBoardCell`, `tryMove`, `playTurn`, `playGame`) |

Los totales son parecidos; el lugar cambió. En `construction-06`, cada escritura nombra los dos
índices. En `construction-07`, las escrituras nombran un puntero a casilla, y el cálculo de la
dirección está en una sola función. El ancho sigue apareciendo en cada frontera que recibe el
tablero, porque la segunda dimensión es parte del tipo del parámetro. Lo que el accesor quita de los
sitios de llamada, no lo quita de las firmas.

Son posibles dos fallos cuando cambia el ancho:

- **Ruidoso:** una declaración escrita como `BoardCell boardCells[][9]` con la segunda dimensión
  literal. Cuando el arreglo real pasa a `[11]`, la llamada no coincide con el tipo del parámetro y
  el compilador la rechaza.
- **Silencioso:** un `BoardCell *` aplanado más un `9` literal en la aritmética. Compila y apunta a
  la casilla equivocada. Solo la ejecución, o una prueba que compruebe la dirección, lo nota.

Escribe `EVIDENCE-es.md` §6: el ancho que elegiste, el resultado de la compilación, qué trazas
cambiaron y por qué es esperado, las dos formas aritméticas y los conteos de la tabla.

## Paso 6 — Nulo, reasignación y ciclo de vida (~12 min)

Los pasos anteriores cambiaron dónde vive el diseño. Este paso mide lo que añade el puntero.

### 6.1 Nulo

Copia la línea base a un tercer archivo de trabajo:

```bash
cp code/pointers-baseline.cpp /tmp/pointer-test/null-check.cpp
```

En `playTurn`, antes del bucle, añade:

```cpp
Knight *noKnight = nullptr;
moveKnight(noKnight, 1, 1);
```

Compila con las opciones del proyecto:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/pointer-test/null-check /tmp/pointer-test/null-check.cpp
```

Compila limpio. Ejecuta la semilla 1:

```bash
/tmp/pointer-test/null-check --seed 1 --name Learner
```

El proceso cae con una violación de segmento (estado de salida 139 en esta plataforma). El
compilador no pudo rechazarlo porque el tipo permite el nulo y el valor solo se conoce en ejecución.
Esta es la primera parte de la factura: `construction-06` no podía representar "ningún caballo";
`construction-07` puede, y cada desreferencia es ahora una promesa de que la ubicación es válida.

Restaura el archivo, o copia otra vez la línea base, antes de continuar.

### 6.2 Reasignación

Un puntero puede redirigirse. En una sonda de trabajo, o en una copia que no ejecutes como juego:

```cpp
Knight *activeKnight = selectActiveKnight(playerIsActive, playerKnight, machineKnight);
Knight *before = activeKnight;
activeKnight = machineKnight;
```

`activeKnight` ahora apunta al registro de la máquina. `before` sigue apuntando al original. Esto
compila porque la asignación en un puntero cambia la ubicación, no el objeto apuntado. En
`construction-06`, `activeKnight = machineKnight;` copiaba los valores de la máquina dentro del
registro del jugador y dejaba el nombre vinculado al jugador.

No conectes esto al juego: el siguiente `moveKnight` movería al caballo equivocado. La sonda alcanza
para ver la diferencia. Un puntero es un valor que puedes reasignar; una referencia es un nombre que
no puedes reasignar.

### 6.3 Ciclo de vida

Un puntero puede sobrevivir a su objeto apuntado. Escribe una sonda de trabajo:

```cpp
struct Knight
{
    int row;
    int column;
};

Knight *makeKnight()
{
    Knight local = {1, 1};
    return &local;
}

int main()
{
    Knight *knight = makeKnight();
    return knight->row;
}
```

Compila con `-Wall -Wextra -Werror`. GCC lo rechaza:

```text
error: address of local variable 'local' returned [-Werror=return-local-addr]
```

El compilador detecta este caso porque la variable local es visible en la misma función. Sin
`-Werror`, el programa compila, y el puntero devuelto queda colgando en el momento en que
`makeKnight` retorna. El compilador no detecta un puntero que sobrevive a un registro de otro
ámbito, a un objeto del montón o a un contenedor que reasigna. La propiedad y el ciclo de vida son
`construction-16`.

En la línea base, `getBoardCell` devuelve un puntero dentro de `boardCells`, que es una variable
local de `main` y sigue viva hasta que `main` retorna, después de `playGame`. Por eso el puntero es
válido durante todo el juego. La garantía viene del grafo de llamadas, no del tipo.

Escribe `EVIDENCE-es.md` §7: la compilación y ejecución con nulo, la observación de reasignación, el
diagnóstico de ciclo de vida y la frase que separa "el tipo dice qué hay" de "el tipo dice que sigue
habiendo".

## Paso 7 — Comparar y explicar (~8 min)

Escribe `EVIDENCE-es.md` §8. Dos partes.

**Primero, una frase.** Di qué decide un puntero sobre un registro o una casilla, sin usar las
palabras *puntero*, *dirección*, *desreferencia*, *mejor*, *peor*, *limpio*, *limpia*, *bonito*,
*elegante*, *feo* ni *modular*. Si tu frase necesita una de ellas, has descrito tu reacción al
archivo y no al tipo.

**Segundo, un párrafo.** Responde:

- Qué decide un tipo puntero: la función recibe una ubicación, no una copia ni un nombre vinculado.
- Qué decide un retorno de puntero: qué registro o casilla puede alcanzar el llamador, elegido en
  ejecución.
- Qué decide `const` sobre el objeto apuntado, y dónde lo comprueba el compilador.
- Qué muestra el sitio de llamada: el `&` donde se toma la dirección, y nada después.
- Qué localiza el accesor del tablero: el cálculo de la dirección, no el paso de fila.
- Qué **no** localiza: el ancho de fila, los límites, el nulo, el ciclo de vida, la propiedad y la
  visibilidad en la llamada después de tener la dirección.
- Qué prueba el compilador: compatibilidad del objeto apuntado, corrección de `const`, una dirección
  local devuelta.
- Qué solo prueba una ejecución: que la aritmética cae en la casilla correcta, que la traza no
  cambia, que una desreferencia nula cae.
- Qué lección posterior corrige límites y representación: `construction-09`; invariantes y acceso:
  `construction-10` y `construction-11`; propiedad y ciclo de vida: `construction-16`.

Una respuesta fuerte mantiene separadas cuatro cosas: **qué permite el tipo**, **qué muestra el
sitio de llamada**, **qué impone el compilador** y **qué solo prueba una ejecución**. Una respuesta
débil dice que los punteros son potentes, lo que oculta las cuatro.

## Paso 8 — Comprobación de aceptación (~3 min)

El criterio de aceptación de esta lección es: **acceder a una casilla del tablero mediante un
puntero, explicar el paso de fila y mostrar que la transcripción no cambia mientras el código expone
el diseño.**

Comprueba tu registro contra él:

- [ ] Ejecuté las dos líneas base sin modificar y obtuve tres diffs vacíos, más 20 semillas
      coincidentes.
- [ ] Inventarié los 17 parámetros de caballo: 9 mutables, 8 de solo lectura, 0 por valor.
- [ ] Identifiqué `Knight *`, `const Knight *` y `moveKnight(Knight *, ...)`.
- [ ] Identifiqué `getBoardCell` devolviendo `BoardCell *` y sus tres sitios de llamada.
- [ ] Predije los conteos del diff y los desplazamientos del tablero antes de medir.
- [ ] Reescribí un acceso al tablero como `rowStart + column` y seguí obteniendo 20 coincidencias.
- [ ] Medí el desplazamiento en elementos: 21 con ancho 9 y 25 con ancho 11.
- [ ] Expliqué que `+ column` suma elementos, no bytes.
- [ ] Cambié el ancho a 10, vi adaptarse la forma bidimensional y vi leer mal un literal `9`.
- [ ] Conté dónde vive el ancho de fila en las dos líneas base.
- [ ] Mostré que `moveKnight(nullptr, ...)` compila y cae.
- [ ] Mostré que devolver una dirección local falla la compilación con `-Wreturn-local-addr`.
- [ ] Mostré que un puntero puede reasignarse y una referencia no.
- [ ] Expliqué qué hace visible el puntero y qué sigue ocultando.
- [ ] Mi frase de una oración evita las tres palabras de sintaxis y las ocho de veredicto.
- [ ] No edité ninguna de las dos líneas base versionadas.
- [ ] Completé `EVIDENCE-es.md` §1–§10 y la autocomprobación de §12.

El criterio no es "los punteros son más potentes que las referencias". Es "puedes decir qué permite
una ubicación, dónde se codifica el diseño y qué comprobación notaría si la dirección fuera
incorrecta".

## Autocomprobación (binaria)

- [ ] Leí ambas líneas base e inventarié cada parámetro de caballo, retorno de puntero y acceso al
      tablero.
- [ ] Predije el diff, los desplazamientos y el comportamiento con nulo y ciclo de vida antes de
      medir.
- [ ] Compilé ambas líneas base y reproduje las 20 trazas.
- [ ] Reescribí un acceso al tablero como `rowStart + column` y mantuve 20 coincidencias.
- [ ] Medí el paso de fila y cambié el ancho de fila.
- [ ] Mostré el fallo silencioso de un paso literal y el fallo ruidoso de un tipo equivocado.
- [ ] Mostré que el nulo compila y cae, y que una dirección local colgante falla la compilación.
- [ ] Comparé la predicción con la observación y nombré los fallos.
- [ ] Expliqué qué localiza un puntero y qué no.
- [ ] Completé `EVIDENCE-es.md`.

Estos son criterios de aceptación. Si una casilla queda
sin marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §10.

## Misconcepciones que esta lección refuta

| Error de concepto | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| "Un puntero es solo una dirección entera." | Tiene tipo. `+1` avanza un elemento del tipo apuntado, no un byte. | Di cuál es el tipo apuntado antes de hacer aritmética. |
| "Un puntero y una referencia son lo mismo con otra ortografía." | El nulo, la reasignación, el `&` en la llamada y el ciclo de vida son distintos. | Compara las cuatro propiedades, no la sintaxis. |
| "`rowStart + column` suma bytes." | Suma `column` elementos de tipo `BoardCell`. | Multiplica por `sizeof(BoardCell)` solo cuando necesites bytes. |
| "El parámetro bidimensional es un arreglo." | Decae a `BoardCell (*)[9]`, un puntero a una fila. | Lee el parámetro como "puntero a una fila de 9". |
| "Cambiar el ancho solo toca la declaración." | Un `9` literal en aritmética aplanada compila y lee mal; la forma con constante se adapta. | Cuenta literales, no solo declaraciones. |
| "`getBoardCell` oculta el diseño por completo." | Oculta el índice bidimensional; el `BoardCell *` devuelto ya no conoce el ancho. | Mantén el ancho en el tipo o pásalo explícitamente. |
| "El compilador detecta el nulo." | `moveKnight(nullptr, ...)` compila limpio y cae. | Comprueba el nulo, o usa una forma que no pueda ser nula. |
| "El compilador detecta todos los errores de ciclo de vida." | Detecta una dirección local devuelta; no un puntero que sobrevive a un objeto del montón o a otro ámbito. | Trata el ciclo de vida como una decisión de diseño, no como una advertencia. |
| "Los punteros son más rápidos, así que hay que usarlos." | Un registro de dos enteros es barato de copiar. La razón aquí es el acceso y la opcionalidad. | Pregunta qué debe poder hacer la función. |
| "`const Knight *` significa que el puntero no puede cambiar." | El objeto apuntado es de solo lectura; el puntero sí puede reasignarse. | Lee `const` de izquierda a derecha: `const Knight` es el objeto apuntado. |
| "La aritmética de punteros es segura porque compila." | No hay comprobación de límites. `rowStart + 9` compila. | Mantén los límites en un solo lugar; `construction-09` empieza eso. |

## Siguiente paso

`construction-08` — programación modular. El tablero y el juego pasan detrás de cabeceras y
compilación separada, para que el diseño deje de ser visible para cada llamador. Después,
`construction-09` le da al tablero sus propios límites, y el `1..8` deja de ser un hecho que el
llamador debe recordar.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog-es.md) para la ruta completa.

Si faltaba cualquiera de las dos líneas base, no empieces `construction-08`. El siguiente contraste
necesita los dos lados.
