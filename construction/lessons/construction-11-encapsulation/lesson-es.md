# Encapsulación (encapsulation)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** clase con invariante · **Tiempo:** unos 60 minutos · **Estado:** borrador
**Lectura:** ~19 minutos

## Por qué existe esta lección

`construction-10` hizo observable el invariante. Añadió `board_is_valid`, dos guardas y un barrido,
y después nombró el hueco: nadie llamaba al verificador, y el estado seguía siendo alcanzable para
cualquier código que pudiera ver la definición. La regla era real, pero el lenguaje no imponía quién
podía escribir el campo.

Esta línea base mueve el estado detrás de una frontera de clase:

```cpp
class Board
{
public:
    Board();
    int row_count() const;
    int column_count() const;
    bool is_valid() const;
    void reset();
    bool was_visited(int row, int column) const;
    void mark_visited(int row, int column);

private:
    static constexpr int maximumRowCount = 16;
    static constexpr int maximumColumnCount = 16;
    static constexpr int activeRowCount = 8;
    static constexpr int activeColumnCount = 8;

    bool visited[maximumRowCount][maximumColumnCount];
};
```

Siete operaciones públicas. Cinco declaraciones de datos privados. Un arreglo privado. `game.cpp`
posee un `Board` por valor y solo puede llamar a las operaciones públicas. No hay `board_create`, no
hay `board_destroy` y no hay `Board *` en el camino del juego.

La frase que hay que retener:

> La encapsulación no consiste en ocultar los bytes. Consiste en hacer que las únicas escrituras
> legales sean las que el propio objeto realiza.

Tres notas honestas antes de la comparación.

1. El código se reduce: 488 líneas en siete archivos, frente a 502 en `construction-10` y 479 en
   `construction-09`. `board.h` crece de 12 a 21 líneas; `board.cpp` baja de 87 a 65.
2. El archivo objeto se reduce: 552 bytes de texto para `board.o`, frente a 608 en `construction-10`
   y 504 en `construction-09`. Ocho símbolos exportados, el mismo número que en `construction-10`,
   pero los nombres ahora son miembros `Board::` y las dos variantes del constructor.
3. Las 20 trazas siguen pasando. No pueden ver la frontera: una traza nunca escribe
   `board.visited[0][0] = true`, y fuera de la clase esa línea no compila.

La comparación con `construction-10` no es "la clase es mejor". Es más precisa: el TAD opaco
ocultaba la representación manteniendo la definición fuera de la cabecera. La clase mantiene la
definición en la cabecera y oculta los escritores con control de acceso. Son cosas distintas, y la
prueba de cambio muestra exactamente cuál compra esta línea base.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar una frontera de clase que preserva un invariante;
- mostrar que el estado privado preserva el invariante y el público lo expone.

## El concepto: una frontera que impone el compilador

`construction-09` preguntó: ¿puede un llamador ver la representación? No, porque la cabecera
declaraba `struct Board;` y la definición vivía en `board.cpp`. `construction-10` preguntó:
¿puede un llamador confiar en los valores? Solo si alguien llama al verificador, y nadie lo hacía.
Esta lección hace una tercera pregunta: ¿puede un llamador construir un estado inválido?

La respuesta ahora es no, a través de la interfaz pública. `visited` es privado. El compilador
rechaza cualquier expresión fuera de `Board` que lo nombre. Los únicos escritores son las funciones
miembro, y los dos escritores protegen sus escrituras. Es una garantía distinta de la de
`construction-10`:

| Línea base | Cómo se oculta la representación | Quién puede escribir `visited` |
|---|---|---|
| construction-10 | Tipo incompleto en la cabecera; definición en `board.cpp`. | Cualquier código de `board.cpp`; los llamadores solo mediante funciones libres. |
| construction-11 | Clase completa en la cabecera; acceso `private:`. | Solo las funciones miembro de `Board`. |

La tabla es toda la lección. El TAD oculta el diseño al código fuera de `board.cpp`. La clase oculta
el conjunto de escritores al código fuera de `Board`. Ninguno oculta las dos cosas. Si mueves el
`struct` de `construction-10` a la cabecera, `visited` se vuelve visible para todos los llamadores.
Si mueves la clase de `construction-11` a una cabecera —ya está ahí—, `visited` sigue inalcanzable
por `private:`. Esa es la mejora: la frontera es una propiedad del tipo, no de la ubicación del
archivo.

El invariante no cambia: solo las casillas dentro de las filas 1..8 y las columnas 1..8 pueden
marcarse como visitadas. Lo que cambia es el conjunto de código que puede intentar la escritura.

| Rol | Operación | Acceso | Qué decide |
|---|---|---|---|
| Construcción | `Board::Board()` | público | Las 256 casillas empiezan en `false`. |
| Observación | `is_valid()` | público, `const` | Si una casilla fuera de 1..8 está marcada. |
| Cumplimiento | `mark_visited()` | público, no `const` | Si una escritura llega al arreglo. |
| Cumplimiento | `was_visited()` | público, `const` | Si una lectura llega al arreglo. |
| Reinicio | `reset()` | público, no `const` | Limpia 1..8; deja 0 y 9..15 intactas. |
| Frontera | `private:` | — | Qué código puede nombrar `visited`. |

El calificador `const` forma parte de la frontera. `is_valid`, `was_visited`, `row_count` y
`column_count` no pueden modificar el arreglo; el compilador rechaza una escritura a través de una
referencia `const Board&`. `mark_visited` y `reset` son las únicas operaciones no `const`. En
`game.cpp`, `tryMove` e `initializePositions` reciben `const Board&`; `playTurn` y `playGame`
reciben `Board&`. Las firmas dicen qué funciones observan y qué funciones mutan, y el compilador lo
comprueba.

Dos límites, declarados ahora para que la aceptación sea honesta:

1. **La frontera no es una frontera de seguridad.** `#define private public`, una declaración
   `friend` o un `reinterpret_cast` pueden derrotarla. Es una frontera de diseño impuesta contra
   código ordinario.
2. **La frontera no hace correctas a las funciones miembro.** Un error dentro de `mark_visited`
   todavía puede romper el invariante. La clase mueve la frontera de confianza de cada llamador a la
   implementación de la clase; no elimina la necesidad del verificador ni de las pruebas.

El control de acceso es una regla de tiempo de compilación. No añade ninguna comprobación en
ejecución ni almacenamiento por objeto: `sizeof(Board)` es 256 bytes en esta plataforma, el arreglo
mismo. La frontera no cuesta nada en ejecución; cuesta una regla en tiempo de compilación.

Por eso `is_valid()` sigue siendo público y sigue teniendo cero puntos de llamada en el juego. La
clase elimina al escritor externo; el verificador sigue siendo el diagnóstico para el escritor
interno.

## Antes de empezar

- [`construction-10`](../construction-10-adt-invariants/lesson-es.md) — el prerrequisito del
  catálogo. Necesitas la línea base con invariantes delante, porque esta lección es una diferencia
  contra ella.
- [`construction-09`](../construction-09-abstract-data-type/lesson-es.md) — el `Board` opaco y sus
  siete operaciones, para que veas qué cambia la clase y qué conserva.
- [`construction-03`](../construction-03-functions/lesson-es.md) — puedes leer la declaración de
  una función, una lista de parámetros y un tipo de retorno.
- Un compilador de C++20 (`g++` o `clang++`). `cmake` es opcional; `run.sh` recurre a una
  compilación directa cuando CMake no está.

No necesitas saber cómo implementa el compilador el control de acceso. Necesitas leer una
declaración de clase, identificar las secciones `public:` y `private:`, y compilar un archivo
pequeño dos veces.

### Qué necesitas

- La línea base con encapsulación:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/board.h`](code/board.h),
  [`code/board.cpp`](code/board.cpp), [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- La línea base anterior, para la comparación: el
  [`board.h`](../construction-10-adt-invariants/code/board.h) y el
  [`board.cpp`](../construction-10-adt-invariants/code/board.cpp) de `construction-10`.
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
test -f ../construction-10-adt-invariants/code/board.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

Si falta un archivo, la comparación no tiene segundo lado. Registra el archivo ausente en
`EVIDENCE-es.md` §11 y vuelve cuando esté disponible. Un conteo contra un archivo que no has leído
no es evidencia.

La línea base reproduce las 20 trazas de `common/traces/` byte a byte antes de que cambies nada. Ese
es el contrato de comportamiento, y no dice nada sobre la frontera.

## Paso 1 — Inventariar la frontera de la clase (~10 min)

Lee primero la cabecera. Tiene 21 líneas, y la forma es la lección:

```bash
grep -n '^' code/board.h
```

```text
1:#pragma once
2:
3:class Board
4:{
5:public:
6:    Board();
7:    int row_count() const;
8:    int column_count() const;
9:    bool is_valid() const;
10:    void reset();
11:    bool was_visited(int row, int column) const;
12:    void mark_visited(int row, int column);
13:
14:private:
15:    static constexpr int maximumRowCount = 16;
16:    static constexpr int maximumColumnCount = 16;
17:    static constexpr int activeRowCount = 8;
18:    static constexpr int activeColumnCount = 8;
19:
20:    bool visited[maximumRowCount][maximumColumnCount];
21:};
```

Ahora el mapa de líneas de `board.cpp`. Rellénalo en `EVIDENCE-es.md` §2.2 desde el código fuente,
no desde esta tabla:

| Líneas | Contenido |
|---|---|
| 3–6 | `Board::Board()` — inicializa `visited` con valor. |
| 8–11 | `Board::row_count` |
| 13–16 | `Board::column_count` |
| 18–34 | `Board::is_valid` — el barrido fuera del área activa. |
| 36–45 | `Board::reset` — limpia filas y columnas 1..8. |
| 47–55 | `Board::was_visited` — la guarda de lectura. |
| 57–65 | `Board::mark_visited` — la guarda de escritura. |

Conteos de esta línea base: 3 cabeceras, 4 unidades de traducción, 11 declaraciones públicas, 5
declaraciones de datos privados, 1 definición de clase completa, 0 tipos incompletos, 30
definiciones, 17 auxiliares internos en `game.cpp`, 1 auxiliar interno en `main.cpp`, 1 punto de
entrada.

Comparado con `construction-10`: 12 declaraciones públicas y 31 definiciones. La superficie del
tablero pasó de ocho funciones libres a siete miembros públicos: `board_create` y `board_destroy`
desaparecen, y el constructor reemplaza la creación. El balance es una declaración pública menos y
una definición menos. Los llamadores cambiaron solo en `game.cpp`: cada `board_*(&board, ...)` se
convirtió en una llamada miembro `board.*(...)`.

Después responde en `EVIDENCE-es.md` §2:

1. ¿Qué línea abre `public:` y qué línea abre `private:`?
2. ¿Cuántas operaciones públicas declara `Board`? Nómbralas.
3. ¿Cuántas declaraciones de datos privados hay? Nómbralas.
4. ¿Qué operaciones públicas son `const` y qué impide eso?
5. ¿Dónde se define la dimensión del arreglo y por qué no puede usarla un llamador?
6. ¿Qué operación construye el tablero y qué valor da a cada casilla?
7. ¿Qué operación limpia casillas y cuáles deja intactas?
8. ¿Cuántos puntos de llamada tiene `is_valid()` en `code/`?
9. ¿Qué pasó con `board_create` y `board_destroy`?
10. ¿Qué posee `game.cpp` y cómo pasa el tablero a los auxiliares?

La pregunta 8 tiene una respuesta precisa: cero. La declaración está en `board.h:9`, la definición
en `board.cpp:18`, y ninguna unidad de traducción la llama. La encapsulación elimina al escritor
externo; no añade un llamador para el observador.

La pregunta 3 tiene cinco respuestas: cuatro dimensiones `static constexpr int` y un
`bool visited[16][16]`. Las dimensiones son privadas, pero siguen siendo visibles para una persona
que lee la cabecera. Es un intercambio, no un accidente: la clase oculta el campo al código, no el
diseño a quien lee.

La pregunta 9 tiene la respuesta estructural. `board_create` devolvía `new Board{}`;
`board_destroy` llamaba a `delete`. La clase reemplaza ambos con un valor: `Board board;` en
`run_game`, en `game.cpp:267`, y un destructor implícito al final de la función. La propiedad es un
efecto secundario aquí; `construction-16` la toma como tema principal.

## Paso 2 — Predecir antes de ejecutar (~10 min)

### 2.1 Las operaciones públicas

Escribe estas predicciones antes de ejecutar nada. El arreglo es `bool visited[16][16]`; el área
activa es filas 1..8 y columnas 1..8.

| Secuencia de llamadas | `is_valid()` después | `was_visited` después |
|---|---|---|
| `Board board;` | | — |
| `board.reset()` | | — |
| `board.mark_visited(1, 1)` | | |
| `board.mark_visited(8, 8)` | | |
| `board.mark_visited(0, 0)` | | |
| `board.mark_visited(9, 9)` | | |
| `board.mark_visited(4, 0)` | | |
| `board.mark_visited(8, 9)` | | |

La primera fila es la que hay que tomar en serio. Un tablero recién construido debe ser válido.
Predice `true` y después mide. Si predices `false`, escribe qué línea del constructor o del barrido
lo haría falso, y comprueba esa línea en el Paso 3.

Las dos filas siguientes son las escrituras legales. Las cuatro últimas están fuera del área activa
en direcciones distintas: por debajo, por encima, en la columna reservada 0 y más allá de la columna
activa 8. Predice qué deja cada una.

### 2.2 La prueba de cambio

| Pregunta | Mi predicción |
|---|---|
| ¿Qué pasa cuando se borra `private:` y compila `board.visited[0][0] = true`? | |
| `is_valid()` después de esa escritura | |
| ¿Qué pasa cuando se restaura `private:` y se compila la misma sonda? | |
| Semillas que coinciden, de 20, con la clase sin cambios | |
| Semillas que coinciden, de 20, con `visited` público | |
| ¿El juego llama a `is_valid()`? | |
| Tamaño de texto de `board.o`: ¿crece, encoge o queda igual? | |

La primera fila es la demostración. Borrar una línea cambia lo que el compilador acepta. Predice si
la escritura compila y qué dice `is_valid()` después.

### 2.3 El compilador

| Pregunta | Mi predicción |
|---|---|
| La línea a la que apunta el compilador cuando `visited` es privado | |
| El texto exacto del error que espero | |
| Por qué el compilador puede rechazar la escritura aunque `board.h` esté incluido | |
| Por qué el compilador no puede rechazar un índice malo dentro de `mark_visited` | |
| Mi razón | |

Las dos últimas filas son el límite de la frontera. El control de acceso es una regla de tiempo de
compilación sobre nombres. Los límites del arreglo no. `mark_visited(0, 0)` compila aunque `visited`
sea privado; la guarda dentro de la función es lo que lo rechaza. No confundas las dos
comprobaciones.

## Paso 3 — Ejecutar la línea base y una sonda de frontera (~10 min)

Primero las trazas, sin modificar:

```bash
mkdir -p /tmp/encapsulation-test
./run.sh --seed 1 --name Learner > /tmp/encapsulation-test/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/encapsulation-test/baseline-0001.txt
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
  -o build/encapsulation-baseline code/main.cpp code/game.cpp code/board.cpp \
  code/random_sequence.cpp
```

Ahora la sonda de frontera. Va en el directorio de trabajo, nunca en los archivos versionados.
Ejercita todas las operaciones públicas y pregunta al verificador después de cada una:

```bash
mkdir -p /tmp/encapsulation-test/probe
cat > /tmp/encapsulation-test/probe/boundary.cpp <<'EOF'
#include "board.h"

#include <cstdio>

int main()
{
    Board board;
    std::printf("after construction: valid=%d rows=%d columns=%d\n",
                static_cast<int>(board.is_valid()), board.row_count(),
                board.column_count());

    board.reset();
    std::printf("after reset: valid=%d\n", static_cast<int>(board.is_valid()));

    board.mark_visited(1, 1);
    std::printf("mark(1,1): valid=%d was=%d\n",
                static_cast<int>(board.is_valid()),
                static_cast<int>(board.was_visited(1, 1)));

    board.mark_visited(8, 8);
    std::printf("mark(8,8): valid=%d was=%d\n",
                static_cast<int>(board.is_valid()),
                static_cast<int>(board.was_visited(8, 8)));

    board.mark_visited(0, 0);
    std::printf("mark(0,0): valid=%d was=%d\n",
                static_cast<int>(board.is_valid()),
                static_cast<int>(board.was_visited(0, 0)));

    board.mark_visited(9, 9);
    std::printf("mark(9,9): valid=%d was=%d\n",
                static_cast<int>(board.is_valid()),
                static_cast<int>(board.was_visited(9, 9)));

    board.mark_visited(4, 0);
    std::printf("mark(4,0): valid=%d was=%d\n",
                static_cast<int>(board.is_valid()),
                static_cast<int>(board.was_visited(4, 0)));

    board.mark_visited(8, 9);
    std::printf("mark(8,9): valid=%d was=%d\n",
                static_cast<int>(board.is_valid()),
                static_cast<int>(board.was_visited(8, 9)));

    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/encapsulation-test/probe/boundary \
  /tmp/encapsulation-test/probe/boundary.cpp code/board.cpp
/tmp/encapsulation-test/probe/boundary
```

La salida en esta plataforma:

```text
after construction: valid=1 rows=8 columns=8
after reset: valid=1
mark(1,1): valid=1 was=1
mark(8,8): valid=1 was=1
mark(0,0): valid=1 was=0
mark(9,9): valid=1 was=0
mark(4,0): valid=1 was=0
mark(8,9): valid=1 was=0
```

Lee la primera línea dos veces. La construcción produce un tablero válido: el constructor inicializa
con valor las 256 casillas, así que ninguna casilla fuera del área activa empieza en `true`. Después
las guardas hacen su trabajo: `was=1` para las dos marcas legales, `was=0` para las cuatro ilegales,
y `is_valid()` sigue en `1` después de cada llamada.

Si tu primera línea es `valid=0`, detente. Es una línea base distinta de la que describe esta
lección; regístrala en `EVIDENCE-es.md` §11 y no ajustes la predicción para que coincida.

## Paso 4 — Comparar: qué prueba la sonda de frontera (~8 min)

Abre `board.h` en la interfaz y `board.cpp` en las guardas. Cinco hechos, leídos contra la
definición de la clase:

1. **La interfaz pública no tiene ningún dato.** La línea 5 abre `public:`; las líneas 6–12
   declaran solo funciones. La primera declaración de datos está después de `private:`, en la línea
   14. No hay ningún campo público que escribir.
2. **Los únicos escritores son miembros.** `reset()` y `mark_visited()` son las dos operaciones no
   `const`. `reset()` escribe solo filas y columnas 1..8; `mark_visited()` vuelve antes de escribir
   cuando el índice está fuera del mismo rango, incluidos los índices negativos.
3. **Los observadores son `const`.** `row_count`, `column_count`, `is_valid` y `was_visited` no
   pueden modificar `visited`. El compilador rechaza una escritura a través de `const Board&`, y por
   eso `tryMove` puede recibir `const Board&` y seguir llamando a `was_visited`.
4. **El constructor es el único inicializador completo.** `Board::Board()` en `board.cpp:3–6` usa
   `visited{}`, que inicializa con valor las 256 casillas a `false`. Ninguna operación pública puede
   marcar una casilla de la fila 0 ni de las filas 9..15, así que esas casillas siguen en `false`
   durante toda la vida del objeto.
5. **El verificador sigue viendo todo el arreglo.** `is_valid()` recorre `0..15` sobre el arreglo de
   16×16, así que puede reportar una escritura ilegal si alguna vez ocurre. Es la única operación
   que mira las casillas que el resto de la interfaz se niega a nombrar.

La salida de la sonda, una fila por llamada:

| Llamada | Decisión de la guarda | ¿Se escribió el arreglo? | `was_visited` | `is_valid` |
|---|---|---|---|---|
| ninguna, recién construido | — | no | — | `1` |
| `reset()` | — | limpia 1..8 | — | `1` |
| `mark(1, 1)` | dentro | sí | `1` | `1` |
| `mark(8, 8)` | dentro | sí | `1` | `1` |
| `mark(0, 0)` | fuera | no | `0` | `1` |
| `mark(9, 9)` | fuera | no | `0` | `1` |
| `mark(4, 0)` | fuera | no | `0` | `1` |
| `mark(8, 9)` | fuera | no | `0` | `1` |

Ahora el límite de la evidencia. La sonda prueba que las operaciones públicas preservan el
invariante. No prueba que ningún llamador pueda escribir el campo, porque la sonda solo usa la
interfaz pública. La mitad que falta es la prueba de cambio: quitar el especificador de acceso y
escribir el campo directamente. Es la única forma de ver qué está haciendo `private:`.

Registra en `EVIDENCE-es.md` §5 los cinco hechos, las ocho filas de la sonda y el conteo de puntos
de llamada. El conteo es el número que separa "la clase se usa" de "el verificador se llama". Sigue
siendo cero.

## Paso 5 — Ejecutar la prueba de cambio (~12 min)

Copia la línea base. Los archivos versionados quedan intactos.

```bash
lessonDirectory="$(pwd)"
cp -r code /tmp/encapsulation-test/public
cd /tmp/encapsulation-test/public
```

### 5.1 Borrar `private:`

Solo en la copia de trabajo, elimina el especificador de acceso de `board.h`. El bloque de datos
queda entonces bajo `public:`, porque la etiqueta `public:` sigue en efecto:

```cpp
class Board
{
public:
    Board();
    int row_count() const;
    int column_count() const;
    bool is_valid() const;
    void reset();
    bool was_visited(int row, int column) const;
    void mark_visited(int row, int column);

    static constexpr int maximumRowCount = 16;
    static constexpr int maximumColumnCount = 16;
    static constexpr int activeRowCount = 8;
    static constexpr int activeColumnCount = 8;

    bool visited[maximumRowCount][maximumColumnCount];
};
```

Es una edición de una línea. El arreglo, las dimensiones y la representación son ahora alcanzables
desde cualquier código que incluya la cabecera.

Borrar la etiqueta es la versión de una línea del movimiento. Si mueves solo la declaración del
arreglo, deja las cuatro constantes por encima: el tipo del arreglo las nombra, y una declaración de
miembro no puede usar un nombre que todavía no se ha declarado. Mover solo la línea del arreglo, por
encima de las constantes, no compila.

### 5.2 Compilar y ejecutar la sonda de cambio

Escribe la sonda primero, mientras el estado es público:

```bash
cat > /tmp/encapsulation-test/probe/change.cpp <<'EOF'
#include "board.h"

#include <cstdio>

int main()
{
    Board board;
    board.reset();
    std::printf("before write: valid=%d\n", static_cast<int>(board.is_valid()));

    board.visited[0][0] = true;
    std::printf("after visited[0][0]=true: valid=%d\n",
                static_cast<int>(board.is_valid()));

    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o /tmp/encapsulation-test/probe/change \
  /tmp/encapsulation-test/probe/change.cpp board.cpp
/tmp/encapsulation-test/probe/change
```

```text
before write: valid=1
after visited[0][0]=true: valid=0
```

Ahí está. La escritura llegó a `visited[0][0]`; la fila 0 está fuera del área activa; el barrido vio
la casilla y devolvió `false`. El estado inválido vuelve a ser construible, y `is_valid()` es la
operación que lo hace visible. Es el mismo estado inválido que `construction-10` podía producir
quitando la guarda, alcanzado por otra ruta: no cambiando al escritor, sino cambiando quién tiene
permiso para escribir.

### 5.3 Restaurar `private:`

Vuelve a poner el especificador de acceso y recompila la misma sonda:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o /tmp/encapsulation-test/probe/change \
  /tmp/encapsulation-test/probe/change.cpp board.cpp
```

```text
change.cpp: In function ‘int main()’:
change.cpp:11:11: error: ‘bool Board::visited [16][16]’ is private within this context
   11 |     board.visited[0][0] = true;
      |           ^~~~~~~
board.h:20:10: note: declared private here
   20 |     bool visited[maximumRowCount][maximumColumnCount];
      |          ^~~~~~~
```

Estado de salida 1. La misma línea que compilaba hace una edición ya no compila. Esa es la
demostración de aceptación: el especificador de acceso es lo que impide que un llamador ordinario
construya el estado inválido.

El texto exacto del error depende del compilador y del idioma. Lo importante es la razón: `visited`
es privado en este contexto. Si tu compilador dice `is inaccessible` o `is private`, la lección es
la misma.

### 5.4 Las trazas, con y sin `private:`

Compila el programa completo desde la copia pública y ejecuta las 20 trazas:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o encapsulation-public main.cpp game.cpp board.cpp random_sequence.cpp
cd "${lessonDirectory}"
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/encapsulation-test/public/encapsulation-public --seed "${seedNumber}" \
    --name Learner | cmp - "${trace}"
done
```

Un bucle silencioso es 20 de 20. Registra ese conteo, restaura `private:` en la copia de trabajo,
recompila con el mismo comando y repite: también 20 de 20. El juego nunca escribe fuera del área
activa, así que cambiar la visibilidad del campo no cambia nada de lo que el juego puede hacer. La
traza no puede ver la frontera; solo la prueba de cambio puede.

Restaura `private:` en la copia de trabajo antes del siguiente paso.

Registra en `EVIDENCE-es.md` §6: el diff de una línea, las dos ejecuciones de la sonda de cambio, el
error del compilador y ambos conteos de trazas.

## Paso 6 — La frontera en el resto del programa (~5 min)

La frontera de la clase no es solo `private:`. Otros dos mecanismos sostienen parte del invariante.

**`const` separa observadores de mutadores.** `tryMove` recibe `const Board&` y llama a `row_count`,
`column_count` y `was_visited`. Las tres son `const`. Una escritura a través de esa referencia no
compila:

```cpp
void observe(const Board &board)
{
    board.mark_visited(1, 1);
}
```

```text
error: passing ‘const Board’ as ‘this’ argument discards qualifiers
```

El texto exacto varía, pero la regla es estable: un observador no puede mutar el objeto que observa.
Por eso las firmas de `game.cpp` son informativas:

| Función | Parámetro del tablero | Qué puede hacer |
|---|---|---|
| `convertCellNumberToRow` | `const Board &` | Leer `column_count`. |
| `convertCellNumberToColumn` | `const Board &` | Leer `column_count`. |
| `initializePositions` | `const Board &` | Leer `row_count` y `column_count`. |
| `tryMove` | `const Board &` | Leer `row_count`, `column_count`, `was_visited`. |
| `playTurn` | `Board &` | Llamar a `mark_visited`. |
| `playGame` | `Board &` | Pasar el tablero a `playTurn`. |

**La propiedad por valor elimina el caso nulo del camino del juego.** `run_game` declara
`Board board;` en `game.cpp:267`. No hay `new`, no hay `delete` y no hay `Board *` que pasar de un
lado a otro. El destructor se ejecuta automáticamente al final de la función. Es una mejora real
para este programa, pero no es el tema de esta lección; `construction-16` se ocupa del ciclo de vida
y examinará copias, movimientos y reglas de recursos en detalle.

La clase es copiable por defecto. Una copia duplica las 256 casillas, así que una copia de un
tablero válido es válida; copiar reproduce el origen en lugar de introducir una marca nueva.
`construction-16` examina la semántica de copia y movimiento en detalle.

Dos cosas que la clase no hace, repetidas porque la aceptación depende de ellas:

- Un `Board *` todavía puede ser nulo. La clase elimina el caso nulo del camino de valor y
  referencia que usa `game.cpp`, no del lenguaje.
- Una función miembro todavía puede estar mal. La clase mueve la frontera de confianza; no hace
  correcta la implementación.

Rellena la tabla de `EVIDENCE-es.md` §7 con estas respuestas. La última fila es la que mantiene
honesta la lección: la clase no es una prueba, es un estrechamiento de quién puede escribir.

## Paso 7 — Comparar y explicar (~3 min)

La comparación en una tabla:

| Pregunta | construction-10 | construction-11 |
|---|---|---|
| ¿Cómo se oculta la representación? | Tipo incompleto en la cabecera; definición en `board.cpp`. | Clase completa en la cabecera; acceso `private:`. |
| ¿Quién puede nombrar `visited`? | El código de `board.cpp`. | Las funciones miembro de `Board`. |
| ¿Qué detiene una escritura fuera de rango? | La guarda en `board_mark_visited`. | El compilador rechaza las escrituras del llamador; `mark_visited` protege las escrituras miembro. |
| ¿Qué observa el estado? | `board_is_valid`, una función libre con comprobación de nulo. | `is_valid`, un miembro `const` sin comprobación de nulo. |
| ¿Cómo se crea el tablero? | `board_create()` devuelve `new Board{}`. | `Board board;` construye un valor. |
| ¿Cómo se destruye? | `board_destroy()` llama a `delete`. | El destructor implícito se ejecuta al final del ámbito. |
| Puntos de llamada del verificador | 0 | 0 |
| ¿Qué edita la prueba de cambio? | Quitar la guarda de `board_mark_visited`. | Borrar `private:` de `board.h`. |
| ¿Qué prueba la prueba de cambio? | Que la guarda es lo que rechaza la escritura. | Que el control de acceso es lo que rechaza la escritura. |

Escribe `EVIDENCE-es.md` §8. Dos partes.

**Primero, una frase.** Di qué decide la clase sobre el estado, sin usar las palabras *clase*,
*privado*, *público*, *acceso*, *compilador*, *palabra clave*, *cabecera*, *archivo*, *módulo*,
*encapsulación*, *mejor*, *peor*, *limpio*, *más limpio*, *bonito*, *elegante*, *feo* ni *modular*.
Si tu frase necesita una de ellas, has descrito la sintaxis y no la decisión.

**Segundo, un párrafo.** Responde:

- Qué fija la clase: qué código puede escribir el estado y qué operaciones preservan el invariante.
- Qué lo impone: el control de acceso al campo, las guardas dentro de los dos escritores y `const`
  en los observadores.
- Qué hace distinta a esta frontera de la de `construction-10`: la definición es visible en la
  cabecera, pero el campo sigue inalcanzable; la frontera es una propiedad del tipo, no de la
  ubicación del archivo.
- Qué hacen las operaciones públicas: el constructor inicializa todas las casillas, `reset` limpia
  1..8, `mark_visited` protege la escritura, `was_visited` protege la lectura, `is_valid` observa y
  las dos operaciones de conteo informan las dimensiones.
- Qué prueban las 20 trazas: que el comportamiento del juego no cambia, con `visited` público y con
  `visited` privado.
- Qué no prueban las trazas: que la frontera se cumpla, porque ninguna traza intenta una escritura
  ilegal.
- Qué prueba la prueba de cambio: con `visited` público, el estado inválido es construible y
  `is_valid()` lo reporta; con `private:`, la misma escritura no compila.
- Qué cuesta la prueba de cambio: una línea en `board.h`, cero ediciones en llamadores, cero trazas
  rotas.
- Qué no arregla la clase: un error dentro de una función miembro, un `friend` o un cast que derrote
  el control de acceso. La clase estrecha el conjunto de escritores; no prueba que los escritores
  sean correctos.
- Qué lección posterior se apoya en la clase: `construction-12`, polimorfismo.
- Qué lección posterior se ocupa del ciclo de vida: `construction-16`.

Una respuesta sólida mantiene separadas cuatro cosas: **qué dice la regla**, **quién tiene permiso
para escribir**, **qué comprueba el compilador** y **qué puede ver una ejecución**. Una respuesta
débil dice que los datos son privados, lo que colapsa las tres primeras en una palabra clave.

## Paso 8 — Comprobación de aceptación y registro (~2 min)

El criterio de aceptación de esta lección es: **mostrar que ningún campo ni operación pública puede
romper el invariante del tablero, y que hacer público el estado permite construir el estado
inválido.**

Comprueba tu registro contra él:

- [ ] Identifiqué el estado privado, las siete operaciones públicas y la línea `private:`.
- [ ] Encontré que `board.h` no tiene ningún dato público.
- [ ] Predije `valid=1` después de la construcción y observé `valid=1`.
- [ ] Mostré que todas las operaciones públicas dejan `is_valid()` en `true`.
- [ ] Encontré que `is_valid()` sigue teniendo cero puntos de llamada en `code/`.
- [ ] Borré `private:` en una copia de trabajo y `board.visited[0][0] = true` compiló.
- [ ] Observé que `is_valid()` se vuelve falso después de esa escritura.
- [ ] Restauré `private:` y la misma escritura dejó de compilar.
- [ ] Leí el error del compilador y expliqué qué nombre rechaza.
- [ ] Las 20 trazas pasan con `visited` público y con `visited` privado.
- [ ] Registré que las trazas no pueden ver la frontera.
- [ ] Expliqué qué impide el control de acceso y qué no impide.
- [ ] Expliqué por qué `mark_visited(0, 0)` todavía necesita una guarda en tiempo de ejecución.
- [ ] No edité ningún archivo de código versionado.
- [ ] Completé `EVIDENCE-es.md` §1–§10 y la autocomprobación de §12.

El criterio no es "el campo es privado". Es "puedes decir qué código puede escribir el estado, qué
regla del compilador rechaza la escritura externa, qué guarda de ejecución rechaza la escritura
interna y qué no cubre una traza que pasa".

## Autocomprobación (binaria)

- [ ] Leí `board.h` y `board.cpp` y localicé la frontera de la clase por línea.
- [ ] Comparé la clase con `construction-10` y nombré qué oculta cada una.
- [ ] Predije el comportamiento de las operaciones públicas y la prueba de cambio antes de ejecutar.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Compilé una sonda de frontera y vi que un tablero recién construido es válido.
- [ ] Mostré que todas las operaciones públicas preservan el invariante.
- [ ] Borré `private:` en una copia de trabajo y vi que el estado inválido se vuelve construible.
- [ ] Restauré `private:` y vi la misma escritura rechazada en tiempo de compilación.
- [ ] Reproduje 20 de 20 trazas con `visited` público y con `visited` privado.
- [ ] Expliqué la diferencia entre control de acceso y límites del arreglo.
- [ ] Expliqué qué no arregla la clase.
- [ ] Completé `EVIDENCE-es.md`.

Estos son criterios de aceptación. Si una casilla queda sin
marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §10.

## Misconcepciones que esta lección refuta

| Error | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| "La encapsulación significa que los datos son invisibles." | La cabecera sigue mostrando el arreglo y sus dimensiones. `private:` detiene al código, no a quien lee. | Separar control de acceso de secreto. |
| "Una clase es encapsulación." | Una clase con un dato público no está encapsulada. La palabra clave sola no prueba nada. | Leer los especificadores de acceso, no la palabra clave. |
| "Lo privado es una frontera de seguridad." | `#define private public`, `friend` y los casts pueden derrotarla. | Tratarla como una frontera de diseño impuesta contra código ordinario. |
| "El compilador comprueba los límites del arreglo." | Comprueba el acceso, no los índices. `mark_visited(0, 0)` compila; la guarda lo rechaza en ejecución. | Mantener la guarda y el verificador. |
| "La clase hace innecesario `is_valid`." | La clase elimina al escritor externo; no prueba que las funciones miembro sean correctas. | Mantener el verificador para las pruebas y los errores internos. |
| "Si el campo es privado, ningún llamador puede romper el invariante." | Una función miembro todavía puede tener un error; un `friend` o un cast todavía pueden escribir. | Enunciar la frontera con precisión: código llamador, no todo el código. |
| "Las 20 trazas prueban la frontera." | Las trazas nunca intentan una escritura ilegal, y pasan con el campo público. | Usar la prueba de cambio. |
| "La clase es estrictamente mejor que el TAD opaco." | La clase oculta el conjunto de escritores pero expone el diseño; el TAD oculta el diseño pero no el conjunto de escritores. | Elegir según lo que necesites ocultar, y combinarlos cuando necesites ambas cosas. |
| "`const` es decoración." | Impide que los observadores escriban a través de una referencia `const`. | Leer el calificador como parte del contrato. |
| "Mover `visited` a público solo cambia la visibilidad." | Cambia qué estados son construibles. La prueba de cambio muestra la diferencia de comportamiento. | Tratar los especificadores de acceso como comportamiento, no como formato. |
| "Un constructor no puede producir un objeto inválido." | Este inicializa el arreglo con valor, pero otro constructor podría olvidar un campo o lanzar una excepción. | Separar construcción de validación, y probar el constructor. |
| "El verificador debería reparar el tablero que encuentra inválido." | Un verificador que muta es un segundo escritor, y su llamador no puede saber qué cambió. | Observar en el verificador; reparar en la operación que rompió la regla. |

## Siguiente paso

`construction-12` — polimorfismo. Esta lección convirtió el tablero en un tipo con frontera: el
estado es privado, las operaciones son públicas y el compilador mantiene a los llamadores ordinarios
fuera. La siguiente lección usa ese tipo como sustrato para la ligadura dinámica: una interfaz de
pieza, dos implementaciones y un bucle de juego que depende de la interfaz en lugar de un `switch`.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog-es.md) para la ruta completa.

Si faltaba cualquiera de las dos líneas base, no empieces `construction-12`. El siguiente contraste
necesita ambos lados.
