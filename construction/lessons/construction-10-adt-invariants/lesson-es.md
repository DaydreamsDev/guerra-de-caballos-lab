# Invariantes del tipo abstracto de datos (abstract data type invariants)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** validez y ciclo de vida · **Tiempo:** unos 60 minutos ·
**Prerrequisitos:** construction-09 · **Estado:** borrador · **Lectura:** ~17 minutos

## Por qué existe esta lección

`construction-09` volvió opaco a `Board`. Un llamador puede sostenerlo, pasarlo y destruirlo, y no
puede ver sus bytes. Lo que tampoco podía hacer era preguntar si el objeto está en un estado legal.
Opaco no es lo mismo que válido: la cabecera ocultó el diseño y no prometió nada sobre los valores
de dentro.

Esta línea base añade una declaración y una definición:

```cpp
bool board_is_valid(const Board *board);
```

Ahora hay ocho operaciones, y una regla detrás: solo las casillas dentro del área activa de 8×8
pueden marcarse como visitadas. Tres lugares sostienen esa regla:

| Comprobación | Ubicación | Qué hace |
|---|---|---|
| Guarda de `board_mark_visited` | `board.cpp:81` | Ignora una fila o columna fuera de 1..8. |
| Guarda de `board_was_visited` | `board.cpp:71` | Devuelve `false` fuera de 1..8. |
| Barrido del área activa | `board.cpp:36–56` | Devuelve `false` si una casilla fuera del área activa está marcada. |

Después viene la parte que el README no dice. El juego **nunca llama** a `board_is_valid`:

```bash
grep -rn 'board_is_valid' code/
```

Dos coincidencias: la declaración en `board.h` y la definición en `board.cpp`. Cero puntos de
llamada. Las 20 trazas pasan sin evaluar el invariante ni una vez, así que no pueden decirte si se
cumple. El único llamador de esta lección es la sonda que vas a escribir.

Eso no es un defecto del verificador. Es el hueco que la lección mide: la regla tiene dos puntos de
cumplimiento y un observador, y ningún cliente. La prueba de cambio cierra el círculo. Quita la
guarda en una copia de trabajo, marca `(0, 0)` y el verificador atrapa el estado ilegal. Restaura la
guarda y la misma llamada se rechaza antes de llegar al arreglo.

La frase que hay que retener:

> Un invariante es un predicado, un punto de cumplimiento y un llamador que lo comprueba. Esta línea
> base entrega los dos primeros; el tercero eres tú.

Tres notas honestas antes de la comparación.

1. El código crece 23 líneas: 502 en siete archivos, frente a 479 en `construction-09`. `board.h`
   pasa de 11 a 12 líneas y `board.cpp` de 65 a 87. El resto no cambia.
2. El objeto crece 104 bytes de texto: 608 para `board.o`, frente a 504 en `construction-09`. Ocho
   símbolos exportados en lugar de siete. El nuevo es `_Z14board_is_validPK5Board`.
3. Veinte de veinte trazas no ejercitan el verificador, porque nadie lo llama. La sonda sí, y la
   prueba de cambio también. Una traza es un contrato de comportamiento; un invariante necesita su
   propio llamador.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar validez y ciclo de vida de un tipo abstracto de datos;
- rechazar una escritura fuera de rango y mostrar que quitar la guarda expone el estado inválido.

## El concepto: una comprobación que nadie llama sigue siendo una costura

`construction-09` respondió una pregunta: ¿puede un llamador ver la representación? La respuesta fue
no. Esta lección hace otra pregunta: ¿puede un llamador confiar en los valores de dentro del objeto?
La respuesta no está en la cabecera. La cabecera te da un nombre y ocho operaciones; no dice qué
estados son legales.

Un invariante es una propiedad que debe cumplirse en todo estado legal del objeto. Para este tablero
hay dos:

1. **Límites.** Ninguna casilla fuera de las filas 1..8 y las columnas 1..8 se marca como visitada.
2. **Ciclo de vida.** Un tablero es usable desde `board_create` hasta `board_destroy`, y todo estado
   entre ambos cumple la regla 1.

La regla 1 tiene dos puntos de cumplimiento y un observador:

| Rol | Código | Qué decide |
|---|---|---|
| Cumplimiento | `board_mark_visited` | Si una escritura llega al arreglo. |
| Cumplimiento | `board_was_visited` | Si una lectura llega al arreglo. |
| Observación | `board_is_valid` | Si el arreglo cumple la regla 1 ahora mismo. |

Cumplir y observar son trabajos distintos. Las guardas hacen que una escritura ilegal *no ocurra*.
El verificador dice si el estado es legal *en este momento*. Un diseño puede tener uno sin el otro,
y esta línea base es el ejemplo: tiene los tres, y ningún llamador para el tercero.

¿Por qué importa el observador, si las guardas ya impiden las escrituras ilegales? Porque una guarda
es una decisión local y un invariante es una afirmación global. Una guarda puede desaparecer con una
edición, saltarse con una segunda función o quedar vencida por un error en el índice del arreglo. El
verificador es el único lugar que mira el objeto entero y responde una pregunta de sí o no. También
es el único lugar al que una prueba puede llamar.

Este verificador es correcto, y la lección lo muestra en tres partes:

- **El dominio del barrido es el arreglo.** `row` y `column` van de 0 a 15, con `<`, sobre
  `bool visited[16][16]`. Cada casilla se lee exactamente una vez, y ninguna se lee dos veces ni se
  omite.
- **El predicado coincide con las guardas.** Una casilla está fuera del área activa cuando
  `row < 1 || row > 8 || column < 1 || column > 8`. Es la misma condición que usan las dos guardas,
  así que el observador y los puntos de cumplimiento coinciden en qué significa "fuera".
- **El caso nulo es explícito.** `board_is_valid(nullptr)` devuelve `false` antes de tocar el
  arreglo, en lugar de desreferenciar.

Lo que el verificador no puede hacer es llamarse a sí mismo. La cabecera ofrece la comprobación; el
juego no la toma. Ese es el hueco de diseño que esta lección expone, y `construction-11` lo cierra
desde el otro lado: hace inalcanzable el campo, así que un llamador no puede construir el estado
inválido en primer lugar.

La mitad del ciclo de vida es más delgada de lo que parece. `board_create` devuelve `new Board{}`,
que inicializa a cero las 256 casillas. `board_reset` limpia las filas y columnas 1..8. Nada limpia
las filas 9..15 después de crear. Eso es sólido mientras el invariante se cumpla, porque esas
casillas empiezan en `false` y ninguna escritura legal las alcanza. Deja de serlo en el momento en
que una escritura ilegal sí las alcanza.

## Antes de empezar

- [`construction-09`](../construction-09-abstract-data-type/lesson-es.md) — el prerrequisito del
  catálogo. Necesitas el `Board` opaco y sus siete operaciones delante, porque esta lección es una
  diferencia contra esa línea base.
- [`construction-03`](../construction-03-functions/lesson-es.md) — puedes leer la declaración de
  una función, una lista de parámetros y un tipo de retorno.
- Un compilador de C++20 (`g++` o `clang++`). `cmake` es opcional; `run.sh` recurre a una
  compilación directa cuando CMake no está.

No necesitas saber cómo el enlazador resuelve símbolos. Necesitas leer dos límites de bucle,
compararlos con el tamaño de un arreglo y ejecutar un programa pequeño.

### Qué necesitas

- La línea base con invariantes:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/board.h`](code/board.h),
  [`code/board.cpp`](code/board.cpp), [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- La línea base anterior, para la comparación: el
  [`board.h`](../construction-09-abstract-data-type/code/board.h) y el
  [`board.cpp`](../construction-09-abstract-data-type/code/board.cpp) de `construction-09`.
- Los scripts de compilación y las notas: [`run.sh`](run.sh), [`CMakeLists.txt`](CMakeLists.txt) y
  [`README-es.md`](README-es.md).
- Las reglas: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §1 fija el tablero
  y su numeración desde 1, §8 fija cuándo una casilla pasa a visitada.
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
test -f ../construction-09-abstract-data-type/code/board.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

Si falta un archivo, la comparación no tiene segundo lado. Registra el archivo ausente en
`EVIDENCE-es.md` §11 y vuelve cuando esté disponible. Un conteo contra un archivo que no has leído
no es evidencia.

La línea base reproduce las 20 trazas de `common/traces/` byte a byte antes de que cambies nada. Ese
es el contrato de comportamiento, y no dice nada sobre la validez.

## Paso 1 — Inventariar las tres comprobaciones (~10 min)

Lee primero la cabecera. Tiene 12 líneas, y la nueva es la quinta operación:

```bash
grep -n . code/board.h
```

```text
1:#pragma once
2:
3:struct Board;
4:
5:Board *board_create();
6:void board_destroy(Board *board);
7:int board_row_count(const Board *board);
8:int board_column_count(const Board *board);
9:bool board_is_valid(const Board *board);
10:void board_reset(Board *board);
11:bool board_was_visited(const Board *board, int row, int column);
12:void board_mark_visited(Board *board, int row, int column);
```

Ahora el mapa de líneas de `board.cpp`. Rellénalo en `EVIDENCE-es.md` §2.2 desde el código fuente,
no desde esta tabla:

| Líneas | Contenido |
|---|---|
| 5–8 | Cuatro constantes: máximo 16, activo 8. |
| 11–14 | `struct Board` con `bool visited[16][16]`. |
| 16–19 | `board_create` |
| 21–24 | `board_destroy` |
| 26–29 | `board_row_count` |
| 31–34 | `board_column_count` |
| 36–56 | `board_is_valid` |
| 58–67 | `board_reset` |
| 69–77 | `board_was_visited` |
| 79–87 | `board_mark_visited` |

Conteos de esta línea base: 3 cabeceras, 4 unidades de traducción, 12 declaraciones públicas, 1 tipo
incompleto, 31 definiciones, 18 auxiliares internos (17 en `game.cpp`, 1 en `main.cpp`) y 1 punto de
entrada del programa.

Compara con `construction-09`: 11 declaraciones públicas y 30 definiciones. Esta lección añadió una
declaración y una definición. Las otras siete declaraciones no cambiaron, y ningún llamador tampoco.

Después responde en `EVIDENCE-es.md` §2:

1. ¿Qué operación es nueva y en qué archivo se declara?
2. ¿Cuántos puntos de llamada tiene `board_is_valid` en `code/`?
3. ¿Qué dos operaciones hacen cumplir la regla de límites, y qué hace cada una con un índice fuera
   de rango?
4. ¿Qué hace `board_is_valid` con un `nullptr`?
5. ¿Qué casillas limpia `board_reset`, y cuáles deja intactas?
6. ¿Qué casillas inicializa `board_create`, y con qué valor?
7. ¿Depende `game.cpp` del verificador de alguna forma?
8. Si borraras `board_is_valid` de los dos archivos, ¿qué dejaría de compilar?

La pregunta 2 tiene una respuesta precisa: cero. La pregunta 8 tiene la misma respuesta: nada. La
función está declarada, definida, exportada y sin usar. No es una contradicción; es la costura donde
se engancharía una prueba. Pero una costura sin nada enganchado no es una garantía.

La pregunta 3 tiene dos mitades. `board_mark_visited` vuelve sin escribir; `board_was_visited`
devuelve `false` sin leer. Ninguna de las dos reporta un error. La operación que reporta es el
verificador.

## Paso 2 — Predecir antes de ejecutar (~10 min)

### 2.1 Las guardas

Escribe estas predicciones antes de ejecutar nada. El arreglo es `bool visited[16][16]`; el área
activa es filas 1..8 y columnas 1..8.

| Secuencia de llamadas | `board_was_visited` después (predicción) | `board_is_valid` después (predicción) |
|---|---|---|
| `board_mark_visited(board, 8, 8)` | | |
| `board_mark_visited(board, 0, 0)` | | |
| `board_mark_visited(board, 9, 9)` | | |
| `board_mark_visited(board, 4, 0)` | | |
| `board_mark_visited(board, 8, 9)` | | |

La primera fila es la única escritura legal. Las otras cuatro están fuera de rango de una forma u
otra: por debajo del área activa, por encima, y en la fila o columna 0 reservada. Predice qué deja
cada una.

### 2.2 El verificador

| Pregunta | Mi predicción |
|---|---|
| `board_is_valid` en un tablero recién creado y reiniciado | |
| `board_is_valid` después de una marca legal | |
| `board_is_valid` después de una marca fuera de rango con guarda | |
| Semillas que coinciden, de 20 | |
| ¿El juego llama al verificador? | |
| ¿Qué pasa con `board_is_valid` si quitas la guarda y marcas `(0, 0)`? | |
| ¿El archivo `board.o` crece, encoge o queda igual? | |

La primera fila es la que hay que tomar en serio. Un verificador que no puede decir "verdadero"
sobre un objeto recién creado no es un verificador. Predice `true` y después mide. Si predices
`false`, escribe qué línea del barrido haría que devolviera falso, y compruébala en el Paso 3.

### 2.3 La demostración

La prueba de cambio de esta lección es: quitar la guarda de límites de `board_mark_visited`, marcar
`(0, 0)` y ver cómo `board_is_valid` pasa a falso. Predice cuál de las dos guardas tienes que quitar
para que eso ocurra, y qué línea del predicado clasifica `(0, 0)` como fuera. Escribe la predicción
en `EVIDENCE-es.md` §3.3.

## Paso 3 — Ejecutar la línea base y una sonda (~10 min)

Primero las trazas, sin modificar nada:

```bash
mkdir -p /tmp/invariant-test
./run.sh --seed 1 --name Learner > /tmp/invariant-test/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/invariant-test/baseline-0001.txt
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
  -o build/invariant-baseline code/main.cpp code/game.cpp code/board.cpp \
  code/random_sequence.cpp
```

Ahora la sonda. Va en el directorio de trabajo, nunca en los archivos versionados:

```bash
mkdir -p /tmp/invariant-test/probe
cat > /tmp/invariant-test/probe/probe.cpp <<'EOF'
#include "board.h"

#include <cstdio>

int main()
{
    Board *board = board_create();
    board_reset(board);
    std::printf("fresh: valid=%d\n", static_cast<int>(board_is_valid(board)));

    board_mark_visited(board, 8, 8);
    std::printf("mark(8,8): valid=%d was=%d\n",
                static_cast<int>(board_is_valid(board)),
                static_cast<int>(board_was_visited(board, 8, 8)));

    board_mark_visited(board, 0, 0);
    std::printf("mark(0,0): valid=%d was=%d\n",
                static_cast<int>(board_is_valid(board)),
                static_cast<int>(board_was_visited(board, 0, 0)));

    board_mark_visited(board, 9, 9);
    std::printf("mark(9,9): valid=%d was=%d\n",
                static_cast<int>(board_is_valid(board)),
                static_cast<int>(board_was_visited(board, 9, 9)));

    board_destroy(board);
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/invariant-test/probe/probe /tmp/invariant-test/probe/probe.cpp code/board.cpp
/tmp/invariant-test/probe/probe
```

La salida en esta plataforma:

```text
fresh: valid=1
mark(8,8): valid=1 was=1
mark(0,0): valid=1 was=0
mark(9,9): valid=1 was=0
```

Lee la primera línea dos veces. Un tablero recién creado y reiniciado es válido. Las guardas
hicieron su trabajo: `was=0` en las dos marcas fuera de rango y `was=1` en la legal. Y el
verificador coincidió con ellas después de cada llamada, que es la parte que `construction-09` no
podía ofrecer.

Si tu primera línea es `0`, detente. Esa es otra línea base, distinta de la que describe esta
lección; regístralo en `EVIDENCE-es.md` §11 y no ajustes la predicción para que coincida.

## Paso 4 — Comparar: qué prueba la sonda (~8 min)

Abre `board.cpp` en el barrido, líneas 42–54:

```cpp
for (int row = 0; row < maximumBoardRowCount; row += 1)
{
    for (int column = 0; column < maximumBoardColumnCount; column += 1)
    {
        bool outsideActiveArea =
            row < 1 || row > activeBoardRowCount || column < 1 ||
            column > activeBoardColumnCount;
        if (outsideActiveArea && board->visited[row][column])
        {
            return false;
        }
    }
}
```

Cuatro hechos, leídos contra `bool visited[16][16]`:

1. **El dominio del barrido es exactamente el arreglo.** `maximumBoardRowCount` vale 16, la
   dimensión del arreglo es 16 y la condición es `<`. La última iteración lee `visited[15][15]`, el
   último elemento. Ninguna casilla se omite y ninguna se lee fuera del arreglo.
2. **Almacenamiento y validez son espacios distintos.** `(0, 0)` es un índice válido del arreglo y
   una casilla inválida del tablero. El barrido la visita porque está dentro del arreglo; el
   predicado la rechaza porque `row < 1`. `(16, 16)` está fuera de ambos, y el barrido nunca llega:
   las guardas frenan la escritura y el bucle termina en 15.
3. **El predicado es la misma regla que las guardas.** Compara las líneas 47–48 con las 71–72 y las
   81–82. Las tres dicen: menor que 1, mayor que 8, a la izquierda de 1, a la derecha de 8. Cuando
   las dos mitades coinciden, una escritura con guarda y el verificador no pueden discrepar.
4. **La comprobación de nulo va primero.** `board_is_valid(nullptr)` devuelve `false` en la línea
   40, antes de cualquier desreferencia. Es un contrato de entrada que la cabecera no declara, pero
   que la implementación impone.

La salida de la sonda, una fila por llamada:

| Llamada | Decisión de la guarda | ¿Escribe el arreglo? | `board_was_visited` | `board_is_valid` |
|---|---|---|---|---|
| ninguna, tablero limpio | — | no | — | `1` |
| `mark(8, 8)` | dentro | sí, `visited[8][8]` | `1` | `1` |
| `mark(0, 0)` | fuera | no | `0` | `1` |
| `mark(9, 9)` | fuera | no | `0` | `1` |

Ahora el límite de la evidencia. La sonda prueba que el verificador funciona. Las 20 trazas prueban
que el juego sigue comportándose igual. Ninguna de las dos prueba que el programa use el
verificador, y no lo usa:

- Ninguna unidad de traducción llama a `board_is_valid`. `grep -rn 'board_is_valid' code/` devuelve
  dos coincidencias: una declaración y una definición.
- Una traza ejercita `run_game`; el verificador no está en ese camino.
- La sonda es el único llamador de esta lección, y la prueba de cambio del Paso 5 es el segundo.

Registra en `EVIDENCE-es.md` §5 los cuatro hechos, las cuatro filas de la sonda y el conteo de
puntos de llamada. Ese conteo es el número que separa "el invariante está implementado" de "el
invariante se usa".

## Paso 5 — Ejecutar la prueba de cambio (~12 min)

Copia la línea base. Los archivos versionados quedan intactos.

```bash
lessonDirectory="$(pwd)"
cp -r code /tmp/invariant-test/fixed
cd /tmp/invariant-test/fixed
```

Escribe primero la sonda del cambio, mientras la guarda sigue puesta. Imprime la validez antes y
después de una llamada ilegal:

```bash
mkdir -p /tmp/invariant-test/probe
cat > /tmp/invariant-test/probe/change.cpp <<'EOF'
#include "board.h"

#include <cstdio>

int main()
{
    Board *board = board_create();
    board_reset(board);
    std::printf("before mark: valid=%d\n", static_cast<int>(board_is_valid(board)));

    board_mark_visited(board, 0, 0);
    std::printf("after mark(0,0): valid=%d\n",
                static_cast<int>(board_is_valid(board)));

    board_destroy(board);
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o /tmp/invariant-test/probe/change /tmp/invariant-test/probe/change.cpp board.cpp
/tmp/invariant-test/probe/change
```

```text
before mark: valid=1
after mark(0,0): valid=1
```

Esa es la mitad del cumplimiento funcionando: la llamada se rechaza, el arreglo no se toca y el
estado sigue siendo legal.

### 5.1 Quitar la guarda

Solo en la copia de trabajo, borra la guarda de `board_mark_visited`:

```cpp
void board_mark_visited(Board *board, int row, int column)
{
    board->visited[row][column] = true;
}
```

Recompila y ejecuta la misma sonda:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o /tmp/invariant-test/probe/change /tmp/invariant-test/probe/change.cpp board.cpp
/tmp/invariant-test/probe/change
```

```text
before mark: valid=1
after mark(0,0): valid=0
```

Ahí está. La escritura llegó a `visited[0][0]` y el barrido la vio: la fila 0 está fuera del área
activa, la casilla es `true` y la función devolvió `false`. El estado inválido ahora es construible,
y el verificador es lo que lo hace visible.

### 5.2 Restaurar la guarda

Vuelve a poner la guarda y recompila:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o /tmp/invariant-test/probe/change /tmp/invariant-test/probe/change.cpp board.cpp
/tmp/invariant-test/probe/change
```

```text
before mark: valid=1
after mark(0,0): valid=1
```

La misma llamada, otro resultado, y la única diferencia son cinco líneas de guarda. Esa es la
demostración de aceptación: la regla se impone en la escritura, y el verificador confirma el estado
en los dos casos.

### 5.3 Las trazas, con guarda y sin ella

Compila el programa completo desde la copia de trabajo y ejecuta las 20 trazas:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o invariant-fixed main.cpp game.cpp board.cpp random_sequence.cpp
cd "${lessonDirectory}"
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/invariant-test/fixed/invariant-fixed --seed "${seedNumber}" --name Learner |
    cmp - "${trace}"
done
```

Un bucle silencioso es 20 de 20. Ahora vuelve a quitar la guarda, recompila con el mismo comando y
repítelo: también es 20 de 20. El juego nunca marca una casilla fuera del área activa, así que
quitar la guarda no cambia nada que pueda hacer. La traza no puede ver la diferencia; solo la sonda
puede. Ese es todo el argumento a favor de un llamador: al invariante no lo protegen las pruebas, lo
protegen las comprobaciones, y las comprobaciones necesitan que alguien las ejecute.

Restaura la guarda en la copia de trabajo antes del siguiente paso.

Registra en `EVIDENCE-es.md` §6: el diff de tu único cambio, las tres ejecuciones de la sonda y los
dos conteos de trazas.

## Paso 6 — Ciclo de vida: lo que `reset` no repara (~5 min)

La regla del ciclo de vida dice que un tablero es usable desde `board_create` hasta `board_destroy`.
Dos funciones definen los extremos, y una de ellas es más débil de lo que parece.

```cpp
Board *board_create()
{
    return new Board{};
}
```

`new Board{}` inicializa el objeto a cero, así que las 256 casillas empiezan en `false`. Ese es el
único momento en que se sabe que todo el arreglo está limpio.

```cpp
void board_reset(Board *board)
{
    for (int row = 1; row <= activeBoardRowCount; row += 1)
    {
        for (int column = 1; column <= activeBoardColumnCount; column += 1)
        {
            board->visited[row][column] = false;
        }
    }
}
```

`board_reset` limpia las filas y columnas 1..8. No limpia la 0 ni las 9..15, porque un tablero
correcto nunca tiene nada ahí. Eso es una optimización válida y una reparación inválida. Vuelve a
quitar la guarda de `board_mark_visited` en la copia de trabajo (la misma edición del Paso 5.1),
rompe el invariante y después reinicia:

```bash
cat > /tmp/invariant-test/probe/reset.cpp <<'EOF'
#include "board.h"

#include <cstdio>

int main()
{
    Board *board = board_create();
    board_reset(board);
    board_mark_visited(board, 9, 9);
    std::printf("after unguarded mark(9,9): valid=%d\n",
                static_cast<int>(board_is_valid(board)));
    board_reset(board);
    std::printf("after reset: valid=%d was(9,9)=%d\n",
                static_cast<int>(board_is_valid(board)),
                static_cast<int>(board_was_visited(board, 9, 9)));
    board_destroy(board);
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o /tmp/invariant-test/probe/reset /tmp/invariant-test/probe/reset.cpp board.cpp
/tmp/invariant-test/probe/reset
```

```text
after unguarded mark(9,9): valid=0
after reset: valid=0 was(9,9)=0
```

Y `board_was_visited(9, 9)` devuelve `false` de todos modos, porque su guarda rechaza el índice. El
único observador que puede ver el daño es `board_is_valid`. Por eso el verificador no es decoración
opcional: es el único lector de las casillas que el resto de la interfaz se niega a nombrar.

Rellena esta tabla en `EVIDENCE-es.md` §7:

| Pregunta | Respuesta |
|---|---|
| ¿Quién puede romper el invariante? | |
| ¿Qué función puede detectarlo? | |
| ¿Qué función lo repara? | |
| ¿`board_reset` repara un tablero roto? | |
| ¿Qué pasa con el invariante en `board_destroy`? | |
| ¿Quién es responsable de llamar al verificador? | |

La última fila es el hueco de diseño que esta lección expone. Nadie. La cabecera ofrece la
comprobación; ningún llamador la toma. `construction-11` cierra el hueco desde el otro lado: hace
inalcanzable el campo, así que un llamador no puede construir el estado inválido en primer lugar.
`construction-16` añade la otra mitad del ciclo de vida, un dueño que garantiza que `board_destroy`
se ejecute.

## Paso 7 — Comparar y explicar (~3 min)

Escribe `EVIDENCE-es.md` §8. Dos partes.

**Primero, una frase.** Di qué decide un invariante sobre un objeto, sin usar las palabras
*cabecera*, *archivo*, *módulo*, *compilar*, *puntero*, *struct*, *clase*, *opaco*,
*encapsulación*, *mejor*, *peor*, *limpio*, *más limpio*, *bonito*, *elegante*, *feo* ni *modular*.
Si tu frase necesita una de ellas, describiste la sintaxis y no la decisión.

**Segundo, un párrafo.** Responde:

- Qué fija el invariante: qué estados son legales y qué operaciones pueden pasar de uno a otro.
- Qué lo hace cumplir: dos guardas que frenan accesos ilegales y un barrido que reporta el estado.
- Qué hace correcto al barrido: un dominio de 0..15 sobre un arreglo de 16×16, y un predicado
  idéntico a la condición de las guardas.
- Qué no puede hacer el barrido: observa. No puede impedir una escritura, y nadie lo llama.
- Qué prueban las 20 trazas: que el comportamiento del juego no cambia, con guarda y sin ella.
- Qué no prueban las trazas: que el verificador sea correcto, ni que alguna vez se ejecute.
- Qué prueba la prueba de cambio: con la guarda quitada, el estado ilegal existe y el verificador lo
  reporta; con la guarda restaurada, la misma llamada deja el estado legal.
- Qué cuesta la prueba de cambio: cinco líneas en un archivo, cero ediciones en llamadores, cero
  trazas rotas.
- Qué no hace `board_reset`: reparar un tablero que ya es inválido.
- Qué lección posterior elimina la posibilidad en lugar de comprobarla: `construction-11`.
- Qué lección posterior posee el otro extremo del ciclo de vida: `construction-16`.

Una respuesta fuerte mantiene cuatro cosas separadas: **lo que dice la regla**, **lo que impone el
código**, **lo que observa el verificador** y **lo que una ejecución puede ver**. Una respuesta
débil dice que el tablero está validado, y con eso colapsa las cuatro.

## Paso 8 — Comprobación de aceptación y registro (~2 min)

El criterio de aceptación de esta lección es: **mostrar que una escritura fuera de rango se
rechaza, que `board_is_valid` sigue siendo verdadero y que quitar la guarda hace visible el estado
inválido.**

Comprueba tu registro contra él:

- [ ] Identifiqué `board_is_valid`, las dos guardas de límites y la regla del área activa.
- [ ] Encontré que `board_is_valid` tiene cero puntos de llamada en `code/`.
- [ ] Predije `true` para un tablero limpio y observé `true`.
- [ ] Expliqué el dominio del barrido como `0..15` sobre un arreglo de 16×16.
- [ ] Expliqué por qué `(0, 0)` es un índice válido y una casilla inválida.
- [ ] Mostré que el predicado y las dos guardas usan las mismas cuatro comparaciones.
- [ ] Con la guarda intacta, `board_mark_visited(0, 0)` y `(9, 9)` se rechazan.
- [ ] Con la guarda quitada, `board_mark_visited(0, 0)` hace que `board_is_valid` sea falso.
- [ ] Restauré la guarda y confirmé que el estado inválido se rechaza otra vez.
- [ ] Las 20 trazas pasan con la guarda intacta y con la guarda quitada.
- [ ] Registré que las trazas no pueden ver el verificador, porque nadie lo llama.
- [ ] Mostré que `board_reset` no repara un tablero que ya es inválido.
- [ ] No edité ningún archivo de código versionado.
- [ ] Completé `EVIDENCE-es.md` §1–§10 y la autocomprobación de §12.

El criterio no es "el tablero es válido". Es "puedes decir qué regla se impone, qué función la
observa, quién llama a esa función y qué no cubre una traza que pasa".

## Autocomprobación (binaria)

- [ ] Leí `board.h` y `board.cpp` y localicé las tres comprobaciones por línea.
- [ ] Comparé la cabecera con `construction-09` y encontré la única declaración nueva.
- [ ] Predije el comportamiento de las guardas y el resultado del verificador antes de ejecutar.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Compilé una sonda y vi un tablero limpio reportado como válido.
- [ ] Expliqué el dominio del barrido y por qué `(0, 0)` está fuera del área activa.
- [ ] Quité la guarda en una copia de trabajo y vi que `(0, 0)` volvía inválido el tablero.
- [ ] Restauré la guarda y vi rechazarse el estado inválido.
- [ ] Reproduje 20 de 20 trazas con la guarda y sin ella.
- [ ] Mostré que `board_reset` no repara un tablero roto.
- [ ] Expliqué qué prueba la traza y qué prueba el verificador.
- [ ] Completé `EVIDENCE-es.md`.

Estos son criterios de aceptación. Si una casilla
queda sin marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §10.

## Misconcepciones que esta lección refuta

| Error conceptual | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| "El tipo es opaco, así que debe ser válido." | La opacidad oculta el diseño. No dice nada de los valores. | Separa "no puedo verlo" de "no puede estar mal". |
| "Las guardas hacen innecesario el verificador." | Las guardas frenan las escrituras que conoces. El verificador reporta el estado que no previste. | Conserva ambos y llama al verificador desde una prueba. |
| "Si `board_is_valid` existe, el juego lo usa." | `grep` encuentra cero puntos de llamada. El juego nunca evalúa el invariante. | Cuenta los llamadores antes de confiar en una comprobación. |
| "20 de 20 trazas prueban el invariante." | Las trazas nunca llaman al verificador, y pasan con la guarda quitada. | Una traza prueba el camino que ejecuta, nada más. |
| "Una comprobación que devuelve verdadero se ejecutó." | Una función que nadie llama no devuelve nada. | Cuenta los llamadores y después ejecuta la sonda. |
| "La fila 0 está dentro del tablero porque está dentro del arreglo." | El arreglo es 16×16; el área activa es 1..8. Almacenamiento y validez son distintos. | Escribe el predicado como `row < 1 || row > 8`, igual que las guardas. |
| "`board_reset` restaura un tablero válido." | Solo limpia 1..8. Una marca en 9,9 sobrevive al reset. | Repara en el origen, o limpia todo el arreglo, y di cuál elegiste. |
| "El verificador debería arreglar el tablero que encuentra inválido." | Un verificador que muta es un segundo escritor, y su llamador no sabe qué cambió. | Observa en el verificador; repara en la operación que rompió la regla. |
| "Devolver falso con `nullptr` hace total a la función." | Cubre una entrada mala. No dice nada de un objeto no nulo en mal estado. | Declara el contrato para los dos casos por separado. |
| "Si el compilador acepta el bucle, el bucle está en rango." | El compilador no comprueba límites de arreglos. Solo un sanitizador o una revisión lo hacen. | Lee el límite, o compila una vez con un sanitizador. |
| "Borrar el verificador rompería la compilación." | Nadie lo llama. La compilación sigue en verde. | Trata una comprobación sin usar como una afirmación que nadie verificó. |
| "Un invariante es un comentario al inicio del archivo." | Un comentario no puede fallar. Una función sí. | Haz el invariante invocable, y invócalo. |

## Siguiente paso

`construction-11` — encapsulación. Esta lección hizo observable el invariante y mostró que la
observación sin llamador es una promesa que nadie mantiene. La siguiente lección elimina la
posibilidad en lugar de comprobarla: el campo sale de alcance, toda escritura pasa por una operación
que impone la regla, y el estado inválido deja de ser construible en lugar de ser solo detectable.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog-es.md) para la ruta completa.

Si faltaba alguna de las dos líneas base, no empieces `construction-11`. El siguiente contraste
necesita los dos lados.
