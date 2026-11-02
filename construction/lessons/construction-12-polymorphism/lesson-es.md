# Polimorfismo (polymorphism)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** ligadura dinámica · **Tiempo:** unos 80 minutos ·
**Prerrequisitos:** construction-11 · **Estado:** borrador · **Lectura:** ~22 minutos

## Por qué existe esta lección

`construction-11` puso el tablero detrás de una frontera de clase: el estado pasó a ser privado, las
operaciones pasaron a ser públicas y el compilador mantuvo fuera a los llamadores ordinarios. La
pieza no recibió el mismo trato. El bucle del juego seguía guardando un registro y leyendo sus
campos:

```cpp
struct Knight
{
    int row;
    int column;
};
```

El nombre ni siquiera se almacenaba. Se calculaba a partir del turno:

```cpp
const char *activeKnightName = playerIsActive ? "player" : "machine";
```

Esta línea base sustituye el registro por una interfaz. `piece.h` declara cuatro operaciones y un
destructor virtual. `knight_piece.h` las implementa. `game.cpp` guarda `Piece *` y `const Piece *`,
y cada lectura o escritura de una pieza pasa por una llamada virtual.

La frase que hay que retener:

> La ligadura dinámica consiste en que el punto de llamada pregunte al objeto qué función ejecutar,
> en lugar de decidirlo por él.

Tres notas honestas antes de la comparación.

1. El código crece: 540 líneas en diez archivos, frente a 488 en siete de `construction-11`.
   `game.cpp` baja de 287 a 281 líneas; la interfaz y su implementación añaden 58 líneas en tres
   archivos.
2. El código objeto crece: `game.o` pasa de 3.549 a 3.688 bytes de texto (+139).
   `knight_piece.o` añade 665 bytes. El binario enlazado pasa de 8.626 a 9.583 bytes de texto
   (+957). El coste es un puntero por objeto y una tabla por clase, más llamadas indirectas.
3. Las veinte trazas siguen pasando. Una traza registra `who=player`; no registra qué función
   produjo esa palabra.

La comparación con `construction-11` no es "la interfaz es mejor". Es más precisa: el registro hacía
que el bucle leyera la representación. La interfaz hace que el bucle formule cuatro preguntas y
siga sin saber cómo se implementan las respuestas. El tipo concreto no ha desaparecido; se trasladó
a dos líneas de `run_game`.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar la ligadura dinámica en un punto de llamada;
- añadir un tipo de pieza con cero ediciones en el bucle del juego.

## El concepto: una llamada resuelta en tiempo de ejecución

`construction-11` preguntó: ¿puede un llamador construir un estado inválido del tablero? No, porque
los escritores son privados y tienen guardas. Esta lección hace otra pregunta: ¿puede el bucle del
juego funcionar sin saber qué pieza concreta está moviendo?

La respuesta ahora es sí, a través de la interfaz. `game.cpp` guarda un puntero cuyo tipo estático
es `Piece *`. Cuando escribe `piece->row()`, el compilador no sabe si el objeto es un `KnightPiece`
u otra cosa. Emite una llamada a través de la tabla del objeto, y el tipo dinámico del objeto decide
qué función se ejecuta. Eso es ligadura dinámica.

| | construction-11 | construction-12 |
|---|---|---|
| Qué guarda el bucle | `Knight *` | `Piece *` |
| Cómo lee la posición | acceso a campo, `knight->row` | llamada virtual, `piece->row()` |
| Cómo elige el nombre | `playerIsActive ? "player" : "machine"` | `piece->role_name()` |
| Dónde se nombra el tipo concreto | `run_game`, `Knight playerKnight` | `run_game`, `KnightPiece playerPiece` |
| Qué decide la llamada | el compilador, desde el tipo estático | el objeto, desde su tipo dinámico |
| Qué cuesta un tipo nuevo con el mismo comportamiento | ediciones en el bucle | dos archivos nuevos y tres líneas en la raíz |

### La interfaz

`piece.h` tiene once líneas, y todas importan:

```cpp
#pragma once

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

Cinco declaraciones virtuales: cuatro operaciones y el destructor. El `= 0` hace que una operación
sea pura: no tiene cuerpo en `Piece`, así que `Piece` no se puede instanciar. La clase es una
interfaz, no una base con una implementación por defecto.

Tres operaciones son `const`. Eso es lo que permite que el bucle guarde `const Piece *` para la
pieza que no está moviendo y siga leyendo su posición y su nombre. `set_position` es la única
operación no `const`, y necesita un `Piece *`. La misma disciplina de `construction-11` continúa: la
firma dice si una función observa o muta, y el compilador lo comprueba.

`knight_piece.h` implementa la interfaz y añade el estado:

```cpp
class KnightPiece : public Piece
{
public:
    explicit KnightPiece(const std::string &roleName);
    int row() const override;
    int column() const override;
    void set_position(int row, int column) override;
    const char *role_name() const override;

private:
    int rowValue;
    int columnValue;
    std::string roleName;
};
```

Las cuatro palabras clave `override` son una comprobación, no decoración: le dicen al compilador
que rechace una función que no sobreescribe de verdad una operación virtual. El paso 6 muestra qué
pasa sin ellas.

Lee la pareja completa. `Piece` es el contrato; `KnightPiece` es una de sus realizaciones:

| | `Piece` | `KnightPiece` |
|---|---|---|
| Estado | ninguno | `rowValue`, `columnValue`, `roleName`, todos privados |
| Operaciones | cinco declaraciones virtuales puras | cuatro sobreescrituras y un constructor |
| Instanciación | prohibida por `= 0` | permitida |
| Qué ve el bucle | la interfaz | solo a través de la interfaz |
| Qué decide | qué preguntas existen | cómo se responde cada pregunta |

La clase concreta sigue encapsulada en el sentido de `construction-11`: su estado es privado y sus
escritores son miembros. La interfaz añade una segunda frontera: el bucle no puede nombrar la clase
concreta en absoluto.

### Qué pregunta el bucle

El bucle nunca pregunta "¿eres un caballo?". Formula cuatro preguntas:

| Rol | Operación | La implementa | Qué pregunta el bucle |
|---|---|---|---|
| Identidad | `role_name()` | `KnightPiece` | "player" o "machine" |
| Lectura de posición | `row()`, `column()` | `KnightPiece` | la casilla actual |
| Escritura de posición | `set_position()` | `KnightPiece` | mover a una casilla |
| Destrucción | `~Piece()` | el compilador | destruir a través de un puntero base |

Esas cuatro preguntas son todo el contrato. Un tipo nuevo que las responda puede ocupar el lugar de
`KnightPiece`. Un tipo que necesite una quinta pregunta no puede, y ese límite es real. Mira la
última subsección de esta sección.

### Qué cuesta la ligadura dinámica

La ligadura dinámica no es gratis, y el coste se puede medir:

- Cada objeto polimórfico guarda un puntero a la tabla de su clase. En esta plataforma
  `sizeof(Piece)` es 8 bytes y `sizeof(KnightPiece)` es 48: 8 del puntero, 8 de los dos enteros y
  32 del `std::string`.
- Cada clase polimórfica tiene una tabla, emitida una vez en el archivo objeto.
  `nm -C knight_piece.o` muestra `vtable for KnightPiece` y `typeinfo for KnightPiece`; la tabla es
  por clase, no por objeto.
- Cada llamada virtual es una llamada indirecta a través de esa tabla. El código fuente tiene 19
  puntos de llamada a través de la interfaz; con `-O2` seis se desvirtualizan dentro de `run_game`,
  y las llamadas propias del bucle siguen siendo indirectas.
- `game.o` crece 139 bytes de texto. El binario enlazado crece 957 bytes de texto y 184 bytes de
  datos.

El compilador puede desvirtualizar cuando prueba el tipo dinámico. En `run_game` inserta en línea
las funciones de inicialización y de cabecera, ve dos objetos `KnightPiece` locales y sustituye seis
llamadas virtuales por llamadas directas. Eso es una optimización dentro de una función. Los
parámetros del propio bucle siguen siendo `Piece *`, así que el bucle sigue compilando contra la
interfaz. No confundas una llamada desvirtualizada con un diseño de ligadura temprana.

### Qué no cubre la interfaz

El bucle sigue siendo dueño de la geometría del movimiento del caballo:

```cpp
constexpr int knightRowChangePerMove[moveOrderMaximum] = {-1, -2, -2, -1, 1, 2, 2, 1};
constexpr int knightColumnChangePerMove[moveOrderMaximum] = {2, 1, -1, -2, -2, -1, 1, 2};
```

`tryMove` indexa esas dos tablas en `game.cpp:153–154`. La interfaz pregunta dónde está una pieza y
cómo se llama; no pregunta cómo se mueve. Una pieza nueva que se mueva como un caballo se puede
añadir con cero ediciones en el bucle. Una pieza nueva con una regla de movimiento distinta no:
`tryMove` tiene que aprender una pregunta nueva, ya sea como otra operación de la interfaz o como
una bifurcación.

Ese es el límite preciso de aceptación de esta lección, y también el límite honesto de la frase
"añade una pieza; cero ediciones en el bucle". Medirás ambos lados en el paso 5.

## Antes de empezar

- [`construction-11`](../construction-11-encapsulation/lesson-es.md) — el prerrequisito del
  catálogo. La frontera de clase es el sustrato de esta lección, y esta lección es una diferencia
  contra ella.
- [`construction-10`](../construction-10-adt-invariants/lesson-es.md) — el invariante del tablero,
  para que sepas qué sigue garantizando la clase mientras la pieza cambia de forma.
- [`construction-07`](../construction-07-pointers/lesson-es.md) — punteros y referencias. El bucle
  ahora guarda `Piece *` y `const Piece *`, y el despacho necesita uno de los dos.
- [`construction-03`](../construction-03-functions/lesson-es.md) — puedes leer la declaración de una
  función, una lista de parámetros y un tipo de retorno.
- Un compilador de C++20 (`g++` o `clang++`). `cmake` es opcional; `run.sh` recurre a una
  compilación directa cuando CMake no está.

No necesitas conocer la disposición de la tabla virtual de tu compilador. Necesitas leer una
declaración de clase, identificar una función virtual pura y compilar un archivo pequeño dos veces.

### Qué necesitas

- La línea base con polimorfismo:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/piece.h`](code/piece.h),
  [`code/knight_piece.h`](code/knight_piece.h), [`code/knight_piece.cpp`](code/knight_piece.cpp),
  [`code/board.h`](code/board.h), [`code/board.cpp`](code/board.cpp),
  [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- La línea base anterior, para la comparación: el
  [`game.cpp`](../construction-11-encapsulation/code/game.cpp) de `construction-11`, sobre todo el
  `struct Knight` y los lugares donde se elige el nombre del caballo.
- Los scripts de compilación y las notas: [`run.sh`](run.sh), [`CMakeLists.txt`](CMakeLists.txt) y
  [`README-es.md`](README-es.md).
- Las reglas: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §1 fija el tablero
  y su numeración desde 1, §4 fija el orden de movimientos del caballo, §8 fija cuándo una casilla
  pasa a visitada.
- El formato de la transcripción:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- Una traza de referencia: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- Esta plantilla para quien aprende: [`EVIDENCE-es.md`](EVIDENCE-es.md).

Comprueba que los artefactos existen antes de empezar, desde este directorio de la lección:

```bash
test -f code/main.cpp &&
test -f code/game.h &&
test -f code/game.cpp &&
test -f code/piece.h &&
test -f code/knight_piece.h &&
test -f code/knight_piece.cpp &&
test -f code/board.h &&
test -f code/board.cpp &&
test -f code/random_sequence.h &&
test -f code/random_sequence.cpp &&
test -f ../construction-11-encapsulation/code/game.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

Si falta un archivo, la comparación no tiene segundo lado. Registra el archivo ausente en
`EVIDENCE-es.md` §11 y vuelve cuando esté disponible. Un conteo contra un archivo que no has leído
no es evidencia.

La línea base reproduce las 20 trazas de `common/traces/` byte a byte antes de que cambies nada. Ese
es el contrato de comportamiento, y no dice nada sobre qué función se ejecutó.

## Paso 1 — Inventariar la interfaz (~12 min)

Lee primero las dos cabeceras nuevas. La forma es la lección:

```bash
grep -n '^' code/piece.h
grep -n '^' code/knight_piece.h
```

```text
1:#pragma once
2:
3:class Piece
4:{
5:public:
6:    virtual ~Piece() = default;
7:    virtual int row() const = 0;
8:    virtual int column() const = 0;
9:    virtual void set_position(int row, int column) = 0;
10:    virtual const char *role_name() const = 0;
11:};
```

```text
1:#pragma once
2:
3:#include "piece.h"
4:
5:#include <string>
6:
7:class KnightPiece : public Piece
8:{
9:public:
10:    explicit KnightPiece(const std::string &roleName);
11:    int row() const override;
12:    int column() const override;
13:    void set_position(int row, int column) override;
14:    const char *role_name() const override;
15:
16:private:
17:    int rowValue;
18:    int columnValue;
19:    std::string roleName;
20:};
```

Ahora el mapa de líneas de `knight_piece.cpp`. Rellénalo en `EVIDENCE-es.md` §2.2 desde el código
fuente, no desde esta tabla:

| Líneas | Contenido |
|---|---|
| 3–6 | `KnightPiece::KnightPiece` — inicializa ambas coordenadas a 0 y copia el nombre del rol. |
| 8–11 | `KnightPiece::row` |
| 13–16 | `KnightPiece::column` |
| 18–22 | `KnightPiece::set_position` — el único escritor. |
| 24–27 | `KnightPiece::role_name` — devuelve un puntero a la cadena almacenada. |

Conteos de esta línea base: 5 cabeceras, 5 unidades de traducción, 21 declaraciones públicas, 8
declaraciones de datos privados, 3 definiciones de clase completas, 0 tipos incompletos, 35
definiciones, 17 auxiliares internos en `game.cpp`, 1 auxiliar interno en `main.cpp`, 1 punto de
entrada.

Comparado con `construction-11`: 11 declaraciones públicas y 30 definiciones, 1 clase completa, 3
cabeceras, 4 unidades de traducción, 5 declaraciones de datos privados. La interfaz y su
implementación añaden 10 declaraciones públicas, 5 definiciones y 2 clases. El tablero no se toca:
sus 7 operaciones públicas, sus 5 declaraciones privadas y su cabecera de 21 líneas son idénticas
byte a byte.

Después responde en `EVIDENCE-es.md` §2:

1. ¿Qué línea declara el destructor, y por qué es virtual?
2. ¿Cuántas operaciones virtuales puras declara `Piece`? Nómbralas.
3. ¿Qué operaciones son `const`, y qué le permite eso a un `const Piece *`?
4. ¿Qué operación cambia el estado del objeto?
5. ¿Qué clase implementa la interfaz, y en qué líneas?
6. ¿Cuántos datos miembro tiene `KnightPiece`? ¿Son públicos?
7. ¿Cuántos puntos de llamada virtuales hay en `game.cpp`? Cuenta `->row()`, `->column()`,
   `->set_position` y `->role_name`.
8. ¿Qué líneas de `run_game` nombran el tipo concreto?
9. ¿El bucle contiene un `switch`, una etiqueta de tipo o una prueba sobre el tipo de la pieza?
10. ¿Qué impide `= 0`, y qué cambiaría si `row()` lo perdiera?

La pregunta 7 tiene una respuesta precisa: 19. Seis `row`, seis `column`, cuatro `role_name`, tres
`set_position`. El paso 4 las asigna a funciones.

La pregunta 8 tiene dos respuestas: las líneas 264–265, donde se construyen
`KnightPiece playerPiece("player")` y `KnightPiece machinePiece("machine")`. El `#include` de la
línea 4 es el único otro lugar que nombra la cabecera. Nada dentro de `playTurn`, `tryMove`,
`printTurnHeader`, `printMove` ni `playGame` nombra un tipo concreto.

La pregunta 9 tiene una respuesta precisa: no. El bucle no tiene ninguna prueba de tipo. Esa es la
propiedad que mide la prueba de cambio.

## Paso 2 — Predecir antes de ejecutar (~12 min)

### 2.1 La tabla de despacho

Escribe estas predicciones antes de ejecutar nada. Cada expresión tiene tipo estático `Piece *` o
`const Piece *` y apunta a un `KnightPiece`.

| Expresión | Qué función se ejecuta | Salida prevista | Confianza |
|---|---|---|---|
| `playerPiece.set_position(3, 4)` a través de `KnightPiece` | | | |
| `piece->row()` a través de `const Piece &` | | | |
| `piece->column()` a través de `const Piece &` | | | |
| `piece->role_name()` a través de `const Piece &` | | | |
| `otherPiece->row()` a través de `const Piece *` | | | |
| `fromPiece->role_name()` en `printMove` | | | |

La segunda fila es la que hay que tomarse en serio. El tipo estático es `Piece`; el tipo dinámico es
`KnightPiece`. Si la llamada se resolviera desde el tipo estático, tendría que llamar a
`Piece::row`, que no tiene cuerpo. Predice qué función se ejecuta de verdad y qué valor devuelve.

### 2.2 La prueba de cambio

| Pregunta | Mi predicción |
|---|---|
| Archivos que añadiré para implementar un segundo tipo `Piece` | |
| Líneas que cambiaré en `game.cpp` | |
| Ediciones dentro de `playTurn`, `tryMove`, `printTurnHeader`, `printMove`, `playGame` | |
| Semillas que coinciden, de 20, con el sustituto en el sitio de construcción | |
| Semillas que coinciden, de 20, tras restaurar `KnightPiece` | |
| Conceptos nuevos que exige el cambio | |

La tercera fila es la aceptación. Predice cero antes de medirlo. Las dos primeras filas son coste,
no corrección.

### 2.3 La sonda de ligadura

| Pregunta | Mi predicción |
|---|---|
| Salida de `piece.row()` a través de `KnightPiece` tras `set_position(3, 4)` | |
| Salida de `piece.row()` a través de `const Piece &` con `row` virtual | |
| Salida de la misma llamada con `row` no virtual y un cuerpo base que devuelve 0 | |
| `sizeof(Piece)` | |
| `sizeof(KnightPiece)` | |
| Tamaño de texto de `game.o`: ¿crece, encoge o queda igual? | |

La tercera fila es la prueba del mecanismo. Una palabra clave decide si la misma expresión de
llamada alcanza la función derivada o la función base. Predice los dos valores antes de editar nada.

## Paso 3 — Ejecutar la línea base y la sonda de despacho (~12 min)

Primero las trazas, sin modificar:

```bash
mkdir -p /tmp/polymorphism-test
./run.sh --seed 1 --name Learner > /tmp/polymorphism-test/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/polymorphism-test/baseline-0001.txt
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
  -o build/polymorphism-baseline code/main.cpp code/game.cpp code/board.cpp \
  code/knight_piece.cpp code/random_sequence.cpp
```

Ahora la sonda de despacho. Va en el directorio de trabajo, nunca en los archivos versionados:

```bash
mkdir -p /tmp/polymorphism-test/probe
cat > /tmp/polymorphism-test/probe/binding.cpp <<'EOF'
#include "knight_piece.h"
#include "piece.h"

#include <cstdio>

namespace
{
void report(const Piece &piece)
{
    std::printf("role=%s at=%d,%d\n", piece.role_name(), piece.row(), piece.column());
}

void move(Piece &piece, int row, int column)
{
    piece.set_position(row, column);
}
}

int main()
{
    KnightPiece playerPiece("player");
    move(playerPiece, 3, 4);
    report(playerPiece);

    KnightPiece machinePiece("machine");
    machinePiece.set_position(7, 2);
    const Piece &asInterface = machinePiece;
    report(asInterface);

    std::printf("sizeof(Piece)=%zu sizeof(KnightPiece)=%zu\n", sizeof(Piece),
                sizeof(KnightPiece));
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/polymorphism-test/probe/binding \
  /tmp/polymorphism-test/probe/binding.cpp code/knight_piece.cpp
/tmp/polymorphism-test/probe/binding
```

La salida en esta plataforma:

```text
role=player at=3,4
role=machine at=7,2
sizeof(Piece)=8 sizeof(KnightPiece)=48
```

Lee las dos primeras líneas contra los tipos estáticos. `report` recibe `const Piece &`, y escribió
la posición que `move` escribió a través de `Piece &`. Las llamadas alcanzaron `KnightPiece::row` y
`KnightPiece::role_name`, no una implementación base. Eso es la ligadura dinámica en una pantalla.

La tercera línea es el coste de almacenamiento: un puntero en `Piece`, y 48 bytes en `KnightPiece`
por el `std::string`. Los números exactos dependen de la plataforma; registra los tuyos.

Si el primer diff no queda vacío, detente. Esa es una línea base distinta de la que describe esta
lección; regístrala en `EVIDENCE-es.md` §11 y no ajustes la predicción para que coincida.

## Paso 4 — Comparar: qué prueba el despacho (~10 min)

Abre `game.cpp` en los puntos de llamada y `knight_piece.cpp` en las implementaciones. Cinco hechos,
leídos contra la interfaz.

1. **El programa nombra el tipo concreto solo en la raíz.** `grep -n 'KnightPiece' code/game.cpp`
   devuelve dos líneas: las dos construcciones de las líneas 264–265. El `#include` de la línea 4
   trae la cabecera. Todas las demás funciones ven `Piece *` o `const Piece *`.
2. **Hay 19 puntos de llamada virtuales.** No están dispersos al azar; cada uno formula una de las
   cuatro preguntas:

   | Líneas | Función | Llamadas virtuales |
   |---|---|---|
   | 40–53 | `initializePositions` | `set_position` ×2 |
   | 65–76 | `printGameHeader` | `row` ×2, `column` ×2 |
   | 87–101 | `printTurnHeader` | `role_name` ×1, `row` ×1, `column` ×1 |
   | 122–132 | `printMove` | `role_name` ×1, `row` ×1, `column` ×1 |
   | 145–148 | `movePiece` | `set_position` ×1 |
   | 150–170 | `tryMove` | `row` ×2, `column` ×2 |
   | 172–238 | `playTurn` | `role_name` ×2 |

3. **La tabla es por clase, el puntero es por objeto.** `nm -C knight_piece.o` muestra
   `vtable for KnightPiece`, `typeinfo for KnightPiece` y `typeinfo name for KnightPiece`.
   `sizeof(Piece)` es 8, que es el puntero. La tabla en sí no está dentro del objeto.
4. **La llamada es dinámica, no temprana.** La sonda escribió `at=3,4` a través de `const Piece &`.
   La base no tiene implementación a la que recurrir, y la función derivada se ejecutó. El paso 6
   quita `virtual` y muestra la misma expresión devolviendo `0`.
5. **La transcripción está gobernada por `role_name()`.** En una copia de trabajo, cambia los dos
   argumentos del constructor de `"player"` y `"machine"` a `"scout"` y `"rival"`, recompila y haz
   diff de la semilla 1. Cambian exactamente 45 de las 89 líneas: las 22 líneas `who=`, las 21
   líneas `move=`, la línea `trapped=` y la línea `winner=`. Todas las líneas cambiadas contienen un
   nombre de rol. Todas las posiciones, todas las listas `order=` y todas las líneas `try=` son
   idénticas.

El hecho 5 es la evidencia más fuerte de que el bucle pregunta en lugar de suponer. El nombre ya no
se calcula desde `playerIsActive`; se lee del objeto, y la salida cambia exactamente donde aparece
el nombre. Repróducelo:

```bash
rm -rf /tmp/polymorphism-test/rename
cp -r code /tmp/polymorphism-test/rename
cd /tmp/polymorphism-test/rename
sed -i 's/playerPiece("player")/playerPiece("scout")/' game.cpp
sed -i 's/machinePiece("machine")/machinePiece("rival")/' game.cpp
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o rename-baseline main.cpp game.cpp board.cpp knight_piece.cpp random_sequence.cpp
./rename-baseline --seed 1 --name Learner > /tmp/polymorphism-test/renamed-0001.txt
diff /tmp/polymorphism-test/baseline-0001.txt \
  /tmp/polymorphism-test/renamed-0001.txt | grep -c '^<'
cd -
```

El último comando imprime `45`: se reemplazaron 45 líneas. El diff tiene 90 líneas porque cada línea
reemplazada aparece una vez como eliminada y otra como añadida.

Para ver las dos formas de llamada, compila `game.cpp` solo y desensámblalo:

```bash
g++ -std=c++20 -O2 -Icode -c code/game.cpp -o /tmp/polymorphism-test/game.o
g++ -std=c++20 -O2 -Icode -c code/knight_piece.cpp \
  -o /tmp/polymorphism-test/knight_piece.o
objdump -dr -C /tmp/polymorphism-test/game.o | grep -E 'KnightPiece|call.*\*'
nm -C /tmp/polymorphism-test/knight_piece.o | grep -E 'vtable|typeinfo'
```

Las reubicaciones `KnightPiece::` aparecen dentro de `run_game`, donde el compilador probó el tipo
dinámico. Las formas indirectas `call *0x18(%rax)` aparecen donde el tipo dinámico no se conoce.
Registra ambas en `EVIDENCE-es.md` §5.2.

El límite de la evidencia: la sonda y las trazas muestran que las llamadas alcanzan la
implementación. No muestran que el bucle aceptaría otra implementación sin ediciones. Solo la
prueba de cambio lo hace.

Registra en `EVIDENCE-es.md` §5 los cinco hechos, la tabla de puntos de llamada y el conteo del diff
de renombrado.

## Paso 5 — Ejecutar la prueba de cambio (~20 min)

Copia la línea base. Los archivos versionados no se tocan.

```bash
lessonDirectory="$(pwd)"
rm -rf /tmp/polymorphism-test/change
cp -r code /tmp/polymorphism-test/change
cd /tmp/polymorphism-test/change
```

### 5.1 Añadir una segunda implementación

El tipo nuevo conserva deliberadamente el movimiento del caballo, para que las trazas de referencia
sigan siendo el contrato. Es una clase distinta con las mismas cuatro respuestas:

```bash
cat > spare_knight_piece.h <<'EOF'
#pragma once

#include "piece.h"

#include <string>

class SpareKnightPiece : public Piece
{
public:
    explicit SpareKnightPiece(const std::string &roleName);
    int row() const override;
    int column() const override;
    void set_position(int row, int column) override;
    const char *role_name() const override;

private:
    int rowValue;
    int columnValue;
    std::string roleName;
};
EOF
cat > spare_knight_piece.cpp <<'EOF'
#include "spare_knight_piece.h"

SpareKnightPiece::SpareKnightPiece(const std::string &roleName)
    : rowValue(0), columnValue(0), roleName(roleName)
{
}

int SpareKnightPiece::row() const
{
    return rowValue;
}

int SpareKnightPiece::column() const
{
    return columnValue;
}

void SpareKnightPiece::set_position(int row, int column)
{
    rowValue = row;
    columnValue = column;
}

const char *SpareKnightPiece::role_name() const
{
    return roleName.c_str();
}
EOF
```

Son 47 líneas en dos archivos nuevos. Ningún archivo existente ha cambiado.

### 5.2 Ejercitarlo a través de la interfaz

Antes de tocar el juego, demuestra que el tipo nuevo responde las mismas preguntas:

```bash
cat > /tmp/polymorphism-test/probe/spare.cpp <<'EOF'
#include "piece.h"
#include "spare_knight_piece.h"

#include <cstdio>

namespace
{
void report(const Piece &piece)
{
    std::printf("role=%s at=%d,%d\n", piece.role_name(), piece.row(), piece.column());
}
}

int main()
{
    SpareKnightPiece piece("player");
    piece.set_position(3, 4);
    report(piece);
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o /tmp/polymorphism-test/probe/spare \
  /tmp/polymorphism-test/probe/spare.cpp spare_knight_piece.cpp
/tmp/polymorphism-test/probe/spare
```

```text
role=player at=3,4
```

La función recibe `const Piece &` y no sabe nada de `SpareKnightPiece`. Esa es la propiedad que
necesita el bucle.

### 5.3 Sustituirlo en el sitio de construcción

Cambia solo la raíz. El `#include` y las dos construcciones son todo el diff:

```bash
python3 - <<'EOF'
from pathlib import Path
path = Path("game.cpp")
text = path.read_text()
text = text.replace('#include "piece.h"', '#include "piece.h"\n#include "spare_knight_piece.h"')
text = text.replace(
    '    KnightPiece playerPiece("player");\n    KnightPiece machinePiece("machine");',
    '    SpareKnightPiece playerPiece("player");\n'
    '    SpareKnightPiece machinePiece("machine");')
path.write_text(text)
EOF
diff -u "${lessonDirectory}/code/game.cpp" game.cpp
```

```text
@@ -3,6 +3,7 @@
 #include "board.h"
 #include "knight_piece.h"
 #include "piece.h"
+#include "spare_knight_piece.h"
 #include "random_sequence.h"
@@ -261,8 +262,8 @@
     Board board;
     board.reset();

-    KnightPiece playerPiece("player");
-    KnightPiece machinePiece("machine");
+    SpareKnightPiece playerPiece("player");
+    SpareKnightPiece machinePiece("machine");
     initializePositions(board, &playerPiece, &machinePiece);
```

Tres líneas añadidas, dos eliminadas, y todas están en `run_game`. El bucle no se toca.

Compila y ejecuta las 20 trazas:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o polymorphism-change main.cpp game.cpp board.cpp knight_piece.cpp \
  spare_knight_piece.cpp random_sequence.cpp
cd "${lessonDirectory}"
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/polymorphism-test/change/polymorphism-change --seed "${seedNumber}" \
    --name Learner | cmp - "${trace}"
done
```

Un bucle silencioso es 20 de 20. La transcripción es idéntica porque el tipo nuevo responde las
mismas cuatro preguntas con los mismos valores.

### 5.4 Restaurar el original y volver a contar

Vuelve a poner `KnightPiece` en el sitio de construcción, recompila y ejecuta las 20 trazas otra
vez. También 20 de 20. La línea base original sigue pasando con el tipo original; añadir un archivo
no la alteró.

Registra ambos conteos. Son afirmaciones distintas:

- 20 de 20 con `SpareKnightPiece` prueba que el bucle acepta otra implementación.
- 20 de 20 con `KnightPiece` prueba que la línea base no cambió.

### 5.5 El hueco: el movimiento no está en la interfaz

Ahora prueba una pieza con una regla de movimiento distinta. El bucle calcula un destino a partir de
dos tablas constantes:

```cpp
destinationRow = activePiece->row() + knightRowChangePerMove[moveNumber - 1];
destinationColumn = activePiece->column() + knightColumnChangePerMove[moveNumber - 1];
```

Ninguna llamada virtual interviene en la regla de movimiento. Una pieza que se mueva de otra forma
necesita una pregunta nueva en la interfaz, por ejemplo
`move_delta(moveNumber, rowChange, columnChange)`, y un cambio en `tryMove` para formularla. En una
copia de trabajo, esa extensión cuesta:

- 2 líneas nuevas en `piece.h` para la declaración virtual pura,
- 2 líneas nuevas en `knight_piece.h` y 8 en `knight_piece.cpp` para la respuesta del caballo,
- unas 7 líneas cambiadas en `tryMove`, sustituyendo dos búsquedas en tabla por una llamada,
- unas 56 líneas para la segunda implementación.

El juego del caballo sigue coincidiendo en 20 de 20 tras la extensión, lo que prueba que la
extensión preservó el comportamiento. La pieza nueva no tiene traza de referencia, lo que prueba que
el contrato no la cubre. No es un fallo de la interfaz; es la frontera de lo que esta interfaz
abstrae. Regístralo en `EVIDENCE-es.md` §6.5.

La aceptación de esta lección es, por tanto, precisa: **añade una pieza que responda las mismas
cuatro operaciones; cero ediciones en el bucle del juego.** Una pieza con una regla de movimiento
nueva es un cambio distinto, y cuesta un cambio de interfaz.

## Paso 6 — La interfaz en el resto del programa (~8 min)

### 6.1 Quitar `virtual` y ver cambiar la llamada

Copia `piece.h`, `knight_piece.h` y `knight_piece.cpp` a un segundo directorio de trabajo. Solo en
esa copia, dale un cuerpo base a `row` y quita `virtual` de ella y `override` de su declaración:

```cpp
// piece.h
int row() const { return 0; }
```

```cpp
// knight_piece.h
int row() const;
```

Escribe la sonda y compílala contra la copia:

```bash
rm -rf /tmp/polymorphism-test/early
mkdir -p /tmp/polymorphism-test/early
cp code/piece.h code/knight_piece.h code/knight_piece.cpp /tmp/polymorphism-test/early/
cd /tmp/polymorphism-test/early
cat > /tmp/polymorphism-test/probe/early.cpp <<'EOF'
#include "knight_piece.h"
#include "piece.h"

#include <cstdio>

int main()
{
    KnightPiece piece("player");
    piece.set_position(3, 4);

    std::printf("through KnightPiece: row=%d\n", piece.row());

    const Piece &asInterface = piece;
    std::printf("through Piece:      row=%d\n", asInterface.row());
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o /tmp/polymorphism-test/probe/early \
  /tmp/polymorphism-test/probe/early.cpp knight_piece.cpp
/tmp/polymorphism-test/probe/early
```

```text
through KnightPiece: row=3
through Piece:      row=0
```

La misma expresión `asInterface.row()` ahora llama a la función base, porque la llamada se resuelve
desde el tipo estático en tiempo de compilación. Eso es ligadura temprana. La palabra clave es el
interruptor.

Por eso la frase de la lección dice que el punto de llamada pregunta al objeto. Sin `virtual`, el
punto de llamada decide desde el tipo que puede ver.

### 6.2 `override` es una comprobación, no decoración

Cambia la declaración en `knight_piece.h` a `int row_value() const override;` y renombra la
definición en `knight_piece.cpp` a `KnightPiece::row_value`. El compilador rechaza la declaración:

```text
error: 'int KnightPiece::row_value() const' marked 'override', but does not override
```

Ahora quita `override` y conserva el error de nombre. La declaración compila, y `KnightPiece` se
queda silenciosamente abstracta porque `Piece::row` sigue siendo pura. El error aparece más tarde,
en el sitio de construcción:

```text
error: cannot declare variable 'piece' to be of abstract type 'KnightPiece'
note: because the following virtual functions are pure within 'KnightPiece':
note: 'virtual int Piece::row() const'
```

`override` traslada el error del lugar de uso a la declaración, donde está la equivocación. Úsalo en
cada sobreescritura.

### 6.3 El destructor es virtual por una razón

`virtual ~Piece() = default;` cuesta una ranura de la tabla y compra una destrucción correcta a
través de un puntero base. Este programa nunca borra una pieza a través de `Piece *`: ambas piezas
son objetos locales en `run_game`, y sus destructores se ejecutan al final de la función. Así que
las trazas no pueden ver la diferencia.

La interfaz debe seguir siendo segura para el siguiente usuario. Si algún día un `Piece *` posee
memoria del montón, `delete` a través de la base debe ejecutar `KnightPiece::~KnightPiece`, que
destruye el `std::string`. Sin un destructor virtual, ejecutaría solo `~Piece` y filtraría la
cadena. `construction-16` se ocupa del ciclo de vida y examina esto en detalle.

### 6.4 El despacho necesita una referencia o un puntero

`Piece` es abstracta, así que esto no compila:

```cpp
void report(Piece piece);   // error: cannot declare a parameter of abstract type
```

La base abstracta impide el corte por accidente. Si `Piece` fuera concreta, el parámetro copiaría
solo el subobjeto base y perdería el estado derivado; la llamada interior usaría entonces la
implementación base. El despacho funciona a través de `Piece &` o `Piece *`, que conservan el tipo
dinámico.

### 6.5 `const` sigue separando lectores de escritores

`tryMove` recibe `const Piece *` para ambas piezas y puede llamar a `row`, `column` y `role_name`.
No puede llamar a `set_position`. `playTurn` guarda `Piece *` para la pieza activa y llama a
`set_position` a través de `movePiece`. El compilador comprueba la misma regla que comprobó sobre
`Board` en `construction-11`, ahora sobre la interfaz de pieza.

| Función | Parámetro de pieza | Qué puede hacer |
|---|---|---|
| `initializePositions` | `Piece *` | Llamar a `set_position`. |
| `printGameHeader` | `const Piece *` | Leer `row`, `column`. |
| `printTurnHeader` | `const Piece *` | Leer `role_name`, `row`, `column`. |
| `printMove` | `const Piece *` | Leer `role_name`, `row`, `column`. |
| `tryMove` | `const Piece *` | Leer `row`, `column`. |
| `movePiece` | `Piece *` | Llamar a `set_position`. |
| `playTurn` | `Piece *` y `const Piece *` | Mover la pieza activa; leer la otra. |

Rellena la tabla de `EVIDENCE-es.md` §7 con estas respuestas.

## Paso 7 — Comparar y explicar (~4 min)

La comparación en una tabla:

| Pregunta | construction-11 | construction-12 |
|---|---|---|
| Qué guarda el bucle | `Knight *` | `Piece *` |
| Cómo lee la posición | acceso a campo | llamada virtual |
| Cómo nombra la pieza | booleano → literal | `role_name()` |
| Dónde aparece el tipo concreto | `run_game` | `run_game`, dos líneas |
| Qué decide una llamada | el compilador, desde el tipo estático | el objeto, desde su tipo dinámico |
| Qué cuesta un tipo nuevo con el mismo comportamiento | ediciones en el bucle | dos archivos y tres líneas en la raíz |
| Qué no cubre la interfaz | — | la geometría del movimiento |
| Puntos de despacho en `game.cpp` | 0 | 19 |
| Texto de `game.o` | 3.549 bytes | 3.688 bytes |
| Texto del binario | 8.626 bytes | 9.583 bytes |
| Trazas | 20 de 20 | 20 de 20 |

Escribe `EVIDENCE-es.md` §8. Dos partes.

**Primero, una frase.** Di qué decide el punto de llamada, sin usar las palabras *virtual*,
*polimorfismo*, *interfaz*, *clase*, *puntero*, *compilador*, *despacho*, *ligadura dinámica*,
*mejor*, *peor*, *limpio*, *flexible*, *extensible*, *elegante* ni *feo*. Si tu frase necesita una
de ellas, has descrito el mecanismo en lugar de la decisión.

**Segundo, un párrafo.** Responde:

- Qué fija la interfaz: qué cuatro preguntas puede formular el bucle a una pieza, y qué operaciones
  son de solo lectura.
- Qué lo impone: declaraciones virtuales puras, `override` en cada implementación, `const` en los
  observadores y un destructor virtual.
- Qué la hace distinta de `construction-11`: el bucle ya no lee campos ni calcula un nombre desde el
  turno; pregunta al objeto.
- Qué hacen los 19 puntos de llamada: lecturas de posición, escrituras de posición y lecturas de
  nombre.
- Qué prueban las 20 trazas: el comportamiento del juego no cambia con el tipo original ni con el
  sustituto en la raíz.
- Qué no prueban las trazas: qué función se ejecutó. La sonda de ligadura y la retirada de `virtual`
  lo muestran.
- Qué cuesta la prueba de cambio: 47 líneas nuevas en dos archivos y tres líneas en el sitio de
  construcción, cero ediciones en el bucle, cero trazas rotas.
- Qué no cubre la interfaz: la geometría del movimiento, que se queda en `tryMove`.
- Qué sigue costando el tipo concreto: se nombra una vez, en `run_game`, y esa es la raíz de
  composición.
- Qué cuesta la ligadura dinámica en ejecución: un puntero por objeto, una tabla por clase, llamadas
  indirectas y pérdida de inserción en línea donde el compilador no puede probar el tipo.
- Qué lección posterior se apoya en la interfaz: `construction-13`, composición, y
  `construction-14`, funciones genéricas.
- Qué lección posterior se ocupa del ciclo de vida: `construction-16`.

Una respuesta fuerte mantiene separadas cuatro cosas: **qué sabe el punto de llamada**, **qué decide
el objeto**, **qué emite el compilador** y **qué puede ver una ejecución**. Una respuesta débil dice
"la función es virtual", lo que colapsa las tres primeras en una palabra clave.

## Paso 8 — Comprobación de aceptación y registro (~2 min)

El criterio de aceptación de esta lección es: **añade un tipo de pieza; cero ediciones en el bucle
del juego; las trazas originales sin cambios.**

Comprueba tu registro contra él:

- [ ] Identifiqué las cinco declaraciones virtuales de `piece.h` y las cuatro sobreescrituras de
      `knight_piece.h`.
- [ ] Conté los 19 puntos de llamada virtuales de `game.cpp` y los asigné a funciones.
- [ ] Encontré que `KnightPiece` solo se nombra en las dos construcciones, y su cabecera se incluye
      una vez.
- [ ] Predije la tabla de despacho y observé que se ejecutan las funciones derivadas.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Compilé la sonda de ligadura y vi `role=player at=3,4` a través de `const Piece &`.
- [ ] Medí `sizeof(Piece)=8` y `sizeof(KnightPiece)=48`.
- [ ] Añadí `SpareKnightPiece` en un archivo de trabajo y lo ejercité a través de `Piece &`.
- [ ] Lo sustituí en el sitio de construcción con tres líneas y obtuve 20 de 20 trazas.
- [ ] Restauré `KnightPiece` y obtuve 20 de 20 trazas con el tipo original.
- [ ] Confirmé cero ediciones en `playTurn`, `tryMove`, `printTurnHeader`, `printMove` y
      `playGame`.
- [ ] Quité `virtual` en una copia de trabajo y vi la misma llamada devolver el valor base.
- [ ] Expliqué el error de `override` y el error de tipo abstracto.
- [ ] Expliqué por qué el destructor es virtual aunque el juego nunca borre una pieza.
- [ ] Nombré la geometría del movimiento como límite de la interfaz.
- [ ] No edité ningún archivo de código versionado.
- [ ] Completé `EVIDENCE-es.md` §1–§10 y la autocomprobación de §12.

El criterio no es "la clase usa virtual". Es "puedes decir qué puntos de llamada dependen de la
interfaz, qué tipo concreto sigue nombrado en la raíz, qué cuesta una implementación nueva y qué no
cubre la interfaz".

## Autocomprobación (binaria)

- [ ] Leí `piece.h`, `knight_piece.h` y `knight_piece.cpp` y localicé la interfaz por línea.
- [ ] Comparé `struct Knight` con `Piece` y `KnightPiece` y nombré qué expone cada uno.
- [ ] Predije la tabla de despacho, el coste de la prueba de cambio y la sonda de ligadura antes de
      ejecutar.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Compilé la sonda de despacho y vi las funciones derivadas ejecutarse a través de
      `const Piece &`.
- [ ] Añadí una segunda implementación de `Piece` en un archivo de trabajo.
- [ ] Mostré 20 de 20 trazas con el sustituto en el sitio de construcción.
- [ ] Mostré 20 de 20 trazas tras restaurar el `KnightPiece` original.
- [ ] Conté cero ediciones en el bucle del juego.
- [ ] Quité `virtual` en una copia de trabajo y expliqué el resultado de ligadura temprana.
- [ ] Expliqué la comprobación `override` y el destructor virtual.
- [ ] Nombré qué no cubre la interfaz: la geometría del movimiento.
- [ ] Completé `EVIDENCE-es.md`.

Estos son criterios de aceptación. Si una casilla queda sin
marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §10.

## Misconcepciones que esta lección refuta

| Error de concepto | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| "Polimorfismo significa muchas clases." | Sin un punto de llamada que acepte varias implementaciones, hay varios tipos, no polimorfismo. | Señala el punto de llamada y cuenta las implementaciones que acepta. |
| "`virtual` hace que cada objeto guarde una tabla." | El objeto guarda un puntero; la tabla es por clase y se emite una vez. | Lee `sizeof(Piece)` y la salida de `nm`. |
| "El bucle ya no conoce el tipo concreto." | Sigue nombrando `KnightPiece` en `run_game`. Ya no bifurca sobre él. | Separa "nombrado una vez en la raíz" de "bifurcado en todas partes". |
| "Añadir una pieza no requiere ninguna edición." | El sitio de construcción debe nombrar el tipo nuevo. | Cuenta por separado las ediciones del bucle y las de la raíz. |
| "Cualquier pieza nueva funciona sin ediciones." | Solo una que responda las mismas cuatro operaciones y se mueva como un caballo. | Extiende la interfaz antes de afirmar que una regla de movimiento nueva es gratis. |
| "`override` es decoración." | Sin él, un error de firma deja la clase abstracta en silencio. | Mantén `override` en cada implementación y lee el error. |
| "El destructor no necesita `virtual` porque nunca borramos." | Cierto hoy; falso en el momento en que un `Piece *` posea memoria del montón. | Haz virtual el destructor base cuando el tipo se diseñe para uso polimórfico. |
| "El despacho dinámico es gratis." | Un puntero por objeto, una tabla por clase, llamadas indirectas, pérdida de inserción en línea. | Mide `sizeof`, el texto del objeto y el del binario. |
| "Un parámetro por valor está bien." | Una base abstracta no puede ser parámetro; una base concreta cortaría el objeto. | Pasa `Piece &` o `Piece *`. |
| "La interfaz es todo el diseño." | Cubre identidad y posición, no movimiento. | Mete el movimiento en la interfaz antes de afirmar que el bucle está cerrado. |
| "Las 20 trazas prueban el despacho." | Pasan con el original y con el sustituto; no pueden ver qué función se ejecutó. | Usa la sonda de ligadura y la retirada de `virtual`. |
| "`virtual` debe repetirse en la sobreescritura." | La virtualidad se hereda; repetirla es legal pero redundante. | Escribe `override`, no `virtual` otra vez. |
| "La desvirtualización significa que el diseño cambió." | El optimizador probó el tipo dinámico dentro de una función; el código fuente sigue dependiendo de la interfaz. | Separa el contrato del código fuente de una compilación optimizada. |

## Siguiente paso

`construction-13` — composición. Esta lección puso las cuatro operaciones detrás de una clase base y
usó la herencia para compartirlas. La composición hace la siguiente pregunta: ¿y si la pieza guarda
una regla de movimiento como miembro y delega en ella, en lugar de heredar de una base? La prueba de
cambio sustituirá la herencia por delegación y comparará el coste.

`construction-14` toma después las mismas cuatro operaciones y escribe un generador de movimientos
para dos tipos de pieza con una plantilla, sin clase base. `construction-16` se ocupa de la pregunta
de ciclo de vida que el destructor virtual solo insinúa.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog-es.md) para la ruta completa.

Si faltaba alguna de las dos líneas base, no empieces `construction-13`. El siguiente contraste
necesita los dos lados.
