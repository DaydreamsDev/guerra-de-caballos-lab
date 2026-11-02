# Composición (composition)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** composición frente a herencia · **Tiempo:** unos 68 minutos ·
**Prerrequisitos:** construction-12 · **Estado:** borrador · **Lectura:** ~22 minutos
**Rol de la lección:** tronco

## Por qué existe esta lección

`construction-12` puso la pieza detrás de una interfaz. El bucle del juego guardaba `Piece *` y
`const Piece *`, formulaba cuatro preguntas y nombraba el tipo concreto en exactamente dos líneas de
`run_game`. La posición vivía en dos enteros dentro de `KnightPiece`, y una tabla virtual decidía
qué función respondía.

Esta línea base elimina la clase base. `Knight` posee un `Position` y le reenvía las tres preguntas
de posición:

```cpp
class Knight
{
public:
    explicit Knight(const std::string &roleName);
    int row() const;
    int column() const;
    void set_position(int row, int column);
    const char *role_name() const;

private:
    Position position;
    std::string roleName;
};
```

No hay `Piece`, ni `virtual`, ni `override`, ni tabla. `Knight::row()` llama a `position.row()`.
`Knight::set_position()` llama a `position.set()`. El tipo concreto ya no está oculto en la raíz:
aparece en cada firma de función de `game.cpp`.

La frase que hay que retener:

> La composición coloca la parte dentro del todo y hace que el todo responda reenviando la pregunta.

Tres notas honestas antes de la comparación.

1. El código crece: 556 líneas en once archivos, frente a 540 en diez de `construction-12`.
   `game.cpp` baja una línea, de 281 a 280. El tipo `Position` y sus dos archivos añaden 16 líneas
   netas, y `Knight` repite las cuatro operaciones que antes declaraba la interfaz.
2. El objeto se reduce: `sizeof(KnightPiece)` es 48 bytes y `sizeof(Knight)` es 40. Los ocho bytes
   son el puntero a la tabla de clase, y la tabla desaparece: el binario enlazado pierde 184 bytes
   de datos de solo lectura. El código objeto cuenta una historia mixta: `game.o` crece 129 bytes
   aunque la indirección desaparece, mientras que los 665 bytes de `knight_piece.o` se convierten
   en 563 entre `knight.o` y `position.o`.
3. Las veinte trazas siguen pasando. Las trazas registran el juego, no la estructura. No pueden
   decirte si existe una tabla.

La comparación con `construction-12` no es "la composición es mejor". Es más precisa: la herencia
compró sustituibilidad y la pagó con una clase base, una tabla y una indirección. La composición
compra un objeto más pequeño y una llamada directa, y la paga con un bucle que nombra un tipo
concreto en todas partes. Qué factura es menor depende del siguiente cambio, y esta lección mide
ambas.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar composición sobre herencia;
- sustituir herencia por delegación y mantener las trazas sin cambios.

## El concepto: una parte dentro de un todo

`construction-12` preguntó: ¿puede el bucle funcionar sin saber qué pieza concreta está moviendo? La
respuesta fue sí, a través de una clase base. Esta lección hace otra pregunta: ¿puede la pieza
funcionar sin una clase base?

Sí, si la posición se convierte en una parte. `Knight` tiene un miembro `Position`. El miembro es un
objeto completo con su propio estado privado y sus propias operaciones. `Knight` no hereda de él: lo
contiene y reenvía las llamadas que no responde por sí mismo.

| | construction-12 | construction-13 |
|---|---|---|
| Relación | `KnightPiece` es un `Piece` | `Knight` tiene un `Position` |
| Cómo se guarda la posición | dos enteros en `KnightPiece` | un miembro `Position` en `Knight` |
| Cómo se responde `row()` | `override` de una operación virtual | `Knight::row()` llama a `position.row()` |
| Dónde vive la lógica de posición | `knight_piece.cpp` | `position.cpp` |
| Qué decide una llamada | el tipo dinámico del objeto | el tipo declarado, en compilación |
| Coste por objeto | puntero a tabla más estado | solo estado |
| Tipo concreto en el bucle | dos construcciones | todas las firmas |

### Las dos clases

`position.h` tiene trece líneas y sostiene una idea: una casilla es un valor con dos coordenadas y
un escritor protegido.

```cpp
#pragma once

class Position
{
public:
    int row() const;
    int column() const;
    void set(int row, int column);

private:
    int rowValue = 0;
    int columnValue = 0;
};
```

Los inicializadores de miembro por defecto hacen que un `Position` construido por defecto sea la
casilla `(0, 0)`. El juego sobrescribe ambas coordenadas durante la preparación. `Position` no tiene
constructor propio, así que el compilador genera uno que aplica esos inicializadores.

`knight.h` añade identidad y las cuatro operaciones que el bucle ya conoce:

```cpp
#pragma once

#include "position.h"

#include <string>

class Knight
{
public:
    explicit Knight(const std::string &roleName);
    int row() const;
    int column() const;
    void set_position(int row, int column);
    const char *role_name() const;

private:
    Position position;
    std::string roleName;
};
```

La superficie pública es casi la misma que la de `KnightPiece`: un constructor y cuatro operaciones.
Lo que cambió está detrás. `Knight` no tiene base ni declaraciones virtuales. Las cuatro operaciones
son funciones miembro ordinarias, resueltas desde el tipo declarado.

### La delegación es reenvío, no herencia

`knight.cpp` es todo el mecanismo:

```cpp
int Knight::row() const
{
    return position.row();
}

int Knight::column() const
{
    return position.column();
}

void Knight::set_position(int row, int column)
{
    position.set(row, column);
}

const char *Knight::role_name() const
{
    return roleName.c_str();
}
```

Tres de las cuatro operaciones reenvían. `role_name` no lo hace: el nombre pertenece a `Knight`, no
a `Position`. Esa es la forma de la delegación. El todo responde la pregunta preguntando a su parte,
y responde solo las preguntas que la parte no posee.

Lee el par como un contrato y su uso:

| | `Position` | `Knight` |
|---|---|---|
| Estado | `rowValue`, `columnValue`, ambos privados | un `Position` y un `std::string`, ambos privados |
| Operaciones | `row`, `column`, `set` | constructor, `row`, `column`, `set_position`, `role_name` |
| Relación con el otro | ninguna | contiene un `Position` |
| Qué decide | cómo se almacena una casilla | quién posee la casilla y el nombre |
| Qué reenvía | — | `row`, `column`, `set_position` |

El anidamiento crea dos fronteras. Un llamador de `Knight` no puede alcanzar los campos de
`Position`. `Knight` tampoco puede alcanzarlos: solo puede llamar a `position.row()`,
`position.column()` y `position.set()`. La parte está encapsulada contra su propio todo, no solo
contra extraños.

### Qué cuesta la composición

La composición no es gratuita, y los costes son concretos:

- **Trabajo de reenvío.** `Knight` repite la interfaz de la parte. Tres métodos existen solo para
  llamar a otros tres métodos. Si `Position` renombra `set` a `move`, `Knight::set_position` debe
  cambiar en el mismo commit.
- **Sin sustituibilidad.** Un `Knight` no es un `Piece`, porque no hay `Piece`. Una función que
  acepta `Knight &` no puede aceptar ningún otro tipo de pieza. `construction-12` podía aceptar un
  `SpareKnightPiece` con tres líneas en la raíz; esta línea base no puede.
- **El tipo concreto se propaga.** El bucle nombra `Knight` en diez firmas auxiliares y en dos
  construcciones. El tipo no está oculto en la raíz: es el único tipo que el bucle puede ver.
- **Un miembro por valor se copia.** Copiar un `Knight` copia su `Position`. Eso es simple y seguro
  para dos enteros, y se convierte en una pregunta real cuando un miembro posee un recurso
  (`construction-16`).
- **El código crece.** Un tipo más y una unidad de traducción más: 556 líneas en once archivos
  frente a 540 en diez. El objeto se reduce; el código no.

### Qué no cubre la composición

El bucle sigue siendo dueño de la geometría del movimiento:

```cpp
constexpr int knightRowChangePerMove[moveOrderMaximum] = {-1, -2, -2, -1, 1, 2, 2, 1};
constexpr int knightColumnChangePerMove[moveOrderMaximum] = {2, 1, -1, -2, -2, -1, 1, 2};
```

`tryMove` indexa esas tablas en `game.cpp:152–153`. `Position` guarda dónde está una pieza; no sabe
cómo se mueve un caballo. Una regla de movimiento distinta sigue siendo un cambio en `game.cpp`,
exactamente como en `construction-12`.

El segundo límite es nuevo. La herencia permitía que el bucle guardara un tipo base y aceptara
cualquier implementación que respondiera las cuatro preguntas. La composición le da al bucle un solo
tipo concreto. Eso es una pérdida real, no un detalle. `construction-14` recupera un bucle para dos
tipos de pieza con una plantilla, y `construction-15` restringe la plantilla con conceptos. Ninguna
de las dos necesita una clase base.

## Antes de empezar

- [`construction-12`](../construction-12-polymorphism/lesson-es.md) — el prerrequisito del catálogo
  y el otro lado de la diferencia. Necesitas sus `piece.h`, `knight_piece.h` y `knight_piece.cpp`
  para eliminarlos en el Paso 5.
- [`construction-11`](../construction-11-encapsulation/lesson-es.md) — la frontera de clase.
  `Position` repite la misma disciplina: estado privado, operaciones públicas y `const` en los
  lectores.
- [`construction-04`](../construction-04-records/lesson-es.md) — registros y agregación. Un miembro
  de tipo clase es la misma idea con un constructor y una frontera.
- [`construction-03`](../construction-03-functions/lesson-es.md) — puedes leer una declaración, una
  lista de parámetros y un tipo de retorno.
- Un compilador de C++20 (`g++` o `clang++`). `cmake` es opcional; `run.sh` recurre a una
  compilación directa cuando CMake no está.

No necesitas saber cómo dispone las clases tu compilador. Necesitas leer dos declaraciones de clase,
seguir una llamada de reenvío y comparar dos compilaciones.

### Qué necesitas

- La línea base con composición:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/knight.h`](code/knight.h),
  [`code/knight.cpp`](code/knight.cpp), [`code/position.h`](code/position.h),
  [`code/position.cpp`](code/position.cpp), [`code/board.h`](code/board.h),
  [`code/board.cpp`](code/board.cpp), [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- La línea base anterior, para la diferencia:
  [`code/piece.h`](../construction-12-polymorphism/code/piece.h),
  [`code/knight_piece.h`](../construction-12-polymorphism/code/knight_piece.h),
  [`code/knight_piece.cpp`](../construction-12-polymorphism/code/knight_piece.cpp) y
  [`code/game.cpp`](../construction-12-polymorphism/code/game.cpp).
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
test -f code/knight.h &&
test -f code/knight.cpp &&
test -f code/position.h &&
test -f code/position.cpp &&
test -f code/board.h &&
test -f code/board.cpp &&
test -f code/random_sequence.h &&
test -f code/random_sequence.cpp &&
test -f ../construction-12-polymorphism/code/piece.h &&
test -f ../construction-12-polymorphism/code/knight_piece.h &&
test -f ../construction-12-polymorphism/code/knight_piece.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

Si falta un archivo, la comparación no tiene segundo lado. Registra el archivo ausente en
`EVIDENCE-es.md` §11 y vuelve cuando esté disponible. Un conteo contra un archivo que no has leído
no es evidencia.

La línea base reproduce las 20 trazas de `common/traces/` byte a byte antes de que cambies nada. Ese
es el contrato de comportamiento, y no dice nada sobre cómo se almacena la posición.

## Paso 1 — Inventariar la composición (~10 min)

Lee primero las dos cabeceras nuevas. La forma es la lección:

```bash
grep -n '^' code/position.h
grep -n '^' code/knight.h
```

```text
1:#pragma once
2:
3:class Position
4:{
5:public:
6:    int row() const;
7:    int column() const;
8:    void set(int row, int column);
9:
10:private:
11:    int rowValue = 0;
12:    int columnValue = 0;
13:};
```

```text
1:#pragma once
2:
3:#include "position.h"
4:
5:#include <string>
6:
7:class Knight
8:{
9:public:
10:    explicit Knight(const std::string &roleName);
11:    int row() const;
12:    int column() const;
13:    void set_position(int row, int column);
14:    const char *role_name() const;
15:
16:private:
17:    Position position;
18:    std::string roleName;
19:};
```

Ahora el mapa de líneas de `knight.cpp`. Rellénalo en `EVIDENCE-es.md` §2.2 desde el código fuente,
no desde esta tabla:

| Líneas | Contenido |
|---|---|
| 3–6 | `Knight::Knight` — construye `position` por defecto y copia el nombre del rol. |
| 8–11 | `Knight::row` — reenvía a `Position::row`. |
| 13–16 | `Knight::column` — reenvía a `Position::column`. |
| 18–21 | `Knight::set_position` — reenvía a `Position::set`. |
| 23–26 | `Knight::role_name` — devuelve un puntero a la cadena almacenada; no reenvía. |

Conteos de esta línea base: 5 cabeceras, 6 unidades de traducción, 19 declaraciones públicas, 9
declaraciones de datos privados, 3 definiciones de clase completas, 0 tipos incompletos, 38
definiciones, 17 auxiliares internos en `game.cpp`, 1 auxiliar interno en `main.cpp`, 1 punto de
entrada.

Compara con `construction-12`: 5 cabeceras, 5 unidades de traducción, 21 declaraciones públicas, 8
declaraciones de datos privados, 3 clases completas, 35 definiciones. La composición elimina cinco
declaraciones de `piece.h` y cinco de `knight_piece.h`, y añade cinco a `knight.h` y tres a
`position.h`: 21 − 5 − 5 + 5 + 3 = 19. El tablero no cambia: sus 7 operaciones públicas, sus 5
declaraciones privadas y su cabecera de 21 líneas son idénticas byte a byte.

Después responde en `EVIDENCE-es.md` §2:

1. ¿Qué clase contiene un `Position`, y en qué línea?
2. ¿Cuántas operaciones públicas declara `Knight`? Nómbralas.
3. ¿Cuáles reenvían a `Position`, y cuál no?
4. ¿Qué operación de `Position` cambia su estado?
5. ¿Cuántos miembros de datos tiene `Position`? ¿Son públicos?
6. ¿Alguna cabecera de `code/` contiene `virtual`, `override` o `: public`?
7. ¿Cuántos puntos de llamada en `game.cpp` piden a un caballo `row`, `column`, `set_position` o
   `role_name`? Cuenta los operadores, no las líneas.
8. ¿En cuántas firmas de función de `game.cpp` aparece `Knight`?
9. ¿Dónde se construye el tipo concreto?
10. ¿Qué se rompería si `Position` hiciera público `rowValue`?

La pregunta 7 tiene una respuesta precisa: 19 puntos de llamada, en 13 líneas. Seis `row`, seis
`column`, tres `set_position`, cuatro `role_name`. El conteo es el mismo que en `construction-12`;
solo cambió la resolución.

La pregunta 8 tiene una respuesta precisa: diez firmas auxiliares más `run_game`. Los auxiliares
son `initializePositions`, `printGameHeader`, `printTurnHeader`, `printMove`,
`selectActiveKnight`, `selectOtherKnight`, `moveKnight`, `tryMove`, `playTurn` y `playGame`.

La pregunta 6 tiene una respuesta precisa: no. `grep -rn 'virtual\|override\|: public' code/` no
devuelve nada. Esa es la afirmación estructural de esta línea base.

## Paso 2 — Predecir antes de ejecutar (~10 min)

### 2.1 La tabla de delegación

Escribe estas predicciones antes de ejecutar nada. Cada expresión nombra un `Knight`; no hay tipo
base detrás del cual esconderse.

| Expresión | Qué función se ejecuta | Salida prevista | Confianza |
|---|---|---|---|
| `playerKnight.set_position(3, 4)` | | | |
| `playerKnight.row()` | | | |
| `playerKnight.column()` | | | |
| `playerKnight.role_name()` | | | |
| `asKnight.row()` a través de `const Knight &` | | | |
| `copy.set_position(5, 6)` después de `Knight copy = playerKnight` | | | |

La última fila es la que hay que tomar en serio. La copia es un objeto separado con su propio
`Position`. Predice qué le pasa al original y si los dos objetos comparten algo.

### 2.2 La prueba de cambio

El cambio es el que produjo esta línea base: partir de `construction-12` y sustituir la interfaz por
un miembro.

| Pregunta | Mi predicción |
|---|---|
| Archivos que voy a eliminar | |
| Archivos que voy a añadir | |
| Líneas que voy a cambiar en `game.cpp` | |
| Funciones de `game.cpp` cuya firma cambia | |
| Líneas de lógica que voy a cambiar (no nombres) | |
| Semillas que coinciden, de 20, después del cambio | |
| Conceptos nuevos que exige el cambio | |

La quinta fila es la aceptación. Predice cero antes de medirlo. Un refactor que cambia una regla es
otra lección.

### 2.3 La sonda de tamaño e indirección

| Pregunta | Mi predicción |
|---|---|
| `sizeof(Position)` | |
| `sizeof(Knight)` | |
| `sizeof(KnightPiece)` | |
| Bytes ahorrados por objeto | |
| Líneas de `nm -C knight.o` con `vtable` o `typeinfo` | |
| Llamadas indirectas en `game12.o` | |
| Llamadas indirectas en `game13.o` | |
| Texto de `game.o`: ¿crece, se reduce o queda igual? | |

La sexta y la séptima fila son la prueba del mecanismo. Un diseño emite llamadas a través de una
tabla; el otro emite llamadas directas. Predice ambos conteos antes de compilar nada.

## Paso 3 — Ejecutar la línea base y la sonda de delegación (~10 min)

Primero las trazas, sin modificar:

```bash
mkdir -p /tmp/composition-test
./run.sh --seed 1 --name Learner > /tmp/composition-test/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/composition-test/baseline-0001.txt
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
  -o build/composition-baseline code/main.cpp code/game.cpp code/board.cpp \
  code/knight.cpp code/position.cpp code/random_sequence.cpp
```

Ahora la sonda de delegación. Va en el directorio temporal, nunca en los archivos versionados:

```bash
mkdir -p /tmp/composition-test/probe
cat > /tmp/composition-test/probe/delegation.cpp <<'EOF'
#include "knight.h"
#include "position.h"

#include <cstdio>

namespace
{
void report(const Knight &knight)
{
    std::printf("role=%s at=%d,%d\n", knight.role_name(), knight.row(), knight.column());
}

void move(Knight &knight, int row, int column)
{
    knight.set_position(row, column);
}
}

int main()
{
    Knight playerKnight("player");
    move(playerKnight, 3, 4);
    report(playerKnight);

    Knight machineKnight("machine");
    machineKnight.set_position(7, 2);
    const Knight &asKnight = machineKnight;
    report(asKnight);

    Knight copy = playerKnight;
    copy.set_position(5, 6);
    std::printf("original=%d,%d copy=%d,%d\n", playerKnight.row(), playerKnight.column(),
                copy.row(), copy.column());

    std::printf("sizeof(Position)=%zu sizeof(Knight)=%zu\n", sizeof(Position), sizeof(Knight));
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/composition-test/probe/delegation \
  /tmp/composition-test/probe/delegation.cpp code/knight.cpp code/position.cpp
/tmp/composition-test/probe/delegation
```

La salida en esta plataforma:

```text
role=player at=3,4
role=machine at=7,2
original=3,4 copy=5,6
sizeof(Position)=8 sizeof(Knight)=40
```

Lee la tercera línea contra la copia. `copy` empezó como copia de `playerKnight` y después se movió
a `(5, 6)`. El original se quedó en `(3, 4)`. El `Position` se copió con el `Knight`; los dos
objetos no comparten estado. Eso es semántica de valor, y es una consecuencia directa de guardar la
parte por valor en lugar de apuntar a ella.

La cuarta línea es el coste de almacenamiento: `Position` son dos enteros, 8 bytes. `Knight` añade
el `std::string`, 32 bytes en esta plataforma, para 40 en total. Los números exactos dependen de la
plataforma; registra los tuyos.

Si el primer diff no está vacío, detente. Esa es otra línea base distinta de la que describe esta
lección; regístrala en `EVIDENCE-es.md` §11 y no ajustes la predicción para que coincida.

## Paso 4 — Comparar: qué prueba la delegación (~8 min)

Abre `knight.cpp` en los reenvíos y `game.cpp` en los puntos de llamada. Cinco hechos, leídos contra
las dos cabeceras.

1. **No queda herencia.** `grep -rn 'virtual\|override\|: public' code/` no devuelve líneas.
   `nm -C knight.o` no contiene `vtable for Knight` ni `typeinfo for Knight`.
2. **Los 19 puntos de llamada son llamadas directas.** Están en 13 líneas y formulan las mismas
   cuatro preguntas:

   | Líneas | Función | Llamadas |
   |---|---|---|
   | 39–52 | `initializePositions` | `set_position` ×2 |
   | 64–75 | `printGameHeader` | `row` ×2, `column` ×2 |
   | 86–100 | `printTurnHeader` | `role_name` ×1, `row` ×1, `column` ×1 |
   | 121–131 | `printMove` | `role_name` ×1, `row` ×1, `column` ×1 |
   | 144–147 | `moveKnight` | `set_position` ×1 |
   | 149–169 | `tryMove` | `row` ×2, `column` ×2 |
   | 171–237 | `playTurn` | `role_name` ×2 |

3. **La delegación vive en tres métodos.** `knight.cpp:8–21` contiene `row`, `column` y
   `set_position`, y cada cuerpo es una llamada de reenvío. `role_name`, en las líneas 23–26, lee la
   cadena directamente. El todo no añade lógica de posición propia.
4. **El objeto perdió el puntero a su tabla.** `sizeof(Knight)` es 40 y `sizeof(KnightPiece)` es 48.
   Los datos de solo lectura del binario enlazado bajan de 876 a 692 bytes: la tabla virtual, la
   información de tipo y su nombre son 184 bytes que ya no existen.
5. **La transcripción sigue siendo impulsada por `role_name()`.** En una copia temporal, cambia los
   dos argumentos del constructor de `"player"` y `"machine"` a `"scout"` y `"rival"`, recompila y
   compara la semilla 1. Cambian exactamente 45 de las 89 líneas: las 22 líneas `who=`, las 21
   líneas `move=`, la línea `trapped=` y la línea `winner=`. Toda línea cambiada contiene un nombre
   de rol. Todas las posiciones, todas las listas `order=` y todas las líneas `try=` son idénticas.
   Repróducelo:

   ```bash
   rm -rf /tmp/composition-test/rename
   cp -r code /tmp/composition-test/rename
   cd /tmp/composition-test/rename
   sed -i 's/playerKnight("player")/playerKnight("scout")/' game.cpp
   sed -i 's/machineKnight("machine")/machineKnight("rival")/' game.cpp
   g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
     -o rename-baseline main.cpp game.cpp board.cpp knight.cpp position.cpp \
     random_sequence.cpp
   ./rename-baseline --seed 1 --name Learner > /tmp/composition-test/renamed-0001.txt
   diff /tmp/composition-test/baseline-0001.txt \
     /tmp/composition-test/renamed-0001.txt | grep -c '^<'
   cd -
   ```

   El último comando imprime `45`: se reemplazaron 45 líneas. El diff tiene 90 líneas porque cada
   línea reemplazada aparece una vez como eliminada y una vez como añadida.

El hecho 5 es el puente entre las dos líneas base. El nombre se sigue leyendo del objeto, así que el
comportamiento observable no cambia. Lo que cambió es cómo viaja la pregunta de posición:
`Knight::row` llama a `Position::row` en lugar de una ranura de tabla que apunta a
`KnightPiece::row`.

Para ver la generación de código, compila `game.cpp` solo en ambos lados y cuenta las llamadas
indirectas:

```bash
g++ -std=c++20 -O2 -Icode -c code/game.cpp -o /tmp/composition-test/game13.o
g++ -std=c++20 -O2 -I../construction-12-polymorphism/code \
  -c ../construction-12-polymorphism/code/game.cpp -o /tmp/composition-test/game12.o
objdump -d /tmp/composition-test/game12.o | grep -c 'call.*\*'
objdump -d /tmp/composition-test/game13.o | grep -c 'call.*\*'
objdump -dr -C /tmp/composition-test/game13.o | grep 'R_X86_64_PLT32' | grep -c 'Knight::'
```

Con este compilador y `-O2`, el primer conteo es 13 y el segundo es 0. El tercero imprime 24: 22
llamadas a accesores más dos llamadas al constructor. El conteo de accesores es mayor que los 19
puntos de llamada porque el optimizador duplica algunas llamadas al insertar en línea los
auxiliares pequeños en `run_game`. Los cinco símbolos distintos son `Knight::row`, `Knight::column`,
`Knight::set_position`, `Knight::role_name` y el constructor. Registra los tres en
`EVIDENCE-es.md` §5.2.

El límite de la evidencia: la sonda y las trazas muestran que las mismas respuestas llegan a los
mismos lugares. No muestran que el bucle pudiera aceptar otro tipo de pieza. Solo la prueba de
cambio lo hace, y aquí muestra lo contrario que en `construction-12`.

Registra en `EVIDENCE-es.md` §5 los cinco hechos, la tabla de puntos de llamada, el conteo del diff
de renombrado y los conteos de indirección.

## Paso 5 — Ejecutar la prueba de cambio (~15 min)

La prueba de cambio es el refactor que produjo esta línea base. Parte de `construction-12` y elimina
la clase base. Los archivos versionados no se tocan.

```bash
lessonDirectory="$(pwd)"
rm -rf /tmp/composition-test/change
cp -r ../construction-12-polymorphism/code /tmp/composition-test/change
cd /tmp/composition-test/change
```

### 5.1 Eliminar la interfaz

```bash
rm piece.h knight_piece.h knight_piece.cpp
```

Tres archivos, 58 líneas, y cinco declaraciones virtuales más cuatro `override` salen de la
compilación. Ningún otro archivo incluía `piece.h` excepto `game.cpp` y `knight_piece.h`.

### 5.2 Añadir la parte

```bash
cat > position.h <<'EOF'
#pragma once

class Position
{
public:
    int row() const;
    int column() const;
    void set(int row, int column);

private:
    int rowValue = 0;
    int columnValue = 0;
};
EOF
cat > position.cpp <<'EOF'
#include "position.h"

int Position::row() const
{
    return rowValue;
}

int Position::column() const
{
    return columnValue;
}

void Position::set(int row, int column)
{
    rowValue = row;
    columnValue = column;
}
EOF
```

Son 30 líneas en dos archivos nuevos.

### 5.3 Añadir el todo

```bash
cat > knight.h <<'EOF'
#pragma once

#include "position.h"

#include <string>

class Knight
{
public:
    explicit Knight(const std::string &roleName);
    int row() const;
    int column() const;
    void set_position(int row, int column);
    const char *role_name() const;

private:
    Position position;
    std::string roleName;
};
EOF
cat > knight.cpp <<'EOF'
#include "knight.h"

Knight::Knight(const std::string &roleName)
    : position(), roleName(roleName)
{
}

int Knight::row() const
{
    return position.row();
}

int Knight::column() const
{
    return position.column();
}

void Knight::set_position(int row, int column)
{
    position.set(row, column);
}

const char *Knight::role_name() const
{
    return roleName.c_str();
}
EOF
```

Son 45 líneas en dos archivos más. El `: position()` del constructor es redundante, porque un
`Position` se construye por defecto de todos modos; está escrito para hacer explícito el miembro.

### 5.4 Renombrar el bucle

Las reglas del juego no cambian. Solo cambian los nombres de tipo. Aplica el renombrado y demuestra
que no queda ningún `Piece`:

```bash
python3 - <<'EOF'
from pathlib import Path
path = Path("game.cpp")
text = path.read_text()
text = text.replace('#include "knight_piece.h"\n#include "piece.h"', '#include "knight.h"')
text = text.replace("KnightPiece", "Knight")
text = text.replace("playerPiece", "playerKnight")
text = text.replace("machinePiece", "machineKnight")
text = text.replace("activePiece", "activeKnight")
text = text.replace("otherPiece", "otherKnight")
text = text.replace("fromPiece", "fromKnight")
text = text.replace("selectActivePiece", "selectActiveKnight")
text = text.replace("selectOtherPiece", "selectOtherKnight")
text = text.replace("movePiece", "moveKnight")
text = text.replace("Piece *", "Knight *")
text = text.replace("void moveKnight(Knight *piece,", "void moveKnight(Knight *knight,")
text = text.replace("    piece->set_position(destinationRow, destinationColumn);",
                    "    knight->set_position(destinationRow, destinationColumn);")
path.write_text(text)
EOF
grep -n 'Piece' game.cpp
diff -u "${lessonDirectory}/../construction-12-polymorphism/code/game.cpp" game.cpp | head -40
```

El `grep` no imprime nada. El diff tiene 14 bloques, 43 líneas añadidas y 44 eliminadas, y cada
línea cambiada es un renombrado de tipo o de identificador. No cambió ningún `if`, ningún límite de
bucle, ninguna regla ni ninguna cadena de salida.

### 5.5 Compilar y ejecutar las 20 trazas

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o composition-change main.cpp game.cpp board.cpp knight.cpp position.cpp \
  random_sequence.cpp
cd "${lessonDirectory}"
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/composition-test/change/composition-change --seed "${seedNumber}" \
    --name Learner | cmp - "${trace}"
done
```

Un bucle silencioso es 20 de 20. La transcripción es idéntica porque ninguna regla cambió. Ese es el
criterio de aceptación de esta lección: sustituir la herencia por un miembro y un reenvío, y
conservar el comportamiento.

### 5.6 Contar qué se movió

| | construction-12 | construction-13 | Diferencia |
|---|---|---|---|
| Archivos | 10 | 11 | +1 |
| Líneas de código | 540 | 556 | +16 |
| Declaraciones públicas | 21 | 19 | −2 |
| Declaraciones de datos privados | 8 | 9 | +1 |
| Clases completas | 3 | 3 | 0 |
| Declaraciones virtuales | 5 | 0 | −5 |
| `override` | 4 | 0 | −4 |
| Métodos de reenvío | 0 | 3 | +3 |
| `sizeof` de la pieza | 48 | 40 | −8 |
| Datos de tabla virtual y tipo | 184 bytes | 0 | −184 |
| Llamadas indirectas en `game.o` | 13 | 0 | −13 |
| Texto de `game.o` | 3.688 | 3.817 | +129 |
| Texto / datos del binario | 9.583 / 876 | 9.400 / 692 | −183 / −184 |
| Trazas | 20 de 20 | 20 de 20 | 0 |

El cambio es un intercambio neto, no una victoria en todos los ejes. El código crece 16 líneas,
`game.o` crece 129 bytes y el bucle queda acoplado a `Knight` en diez firmas. El objeto pierde su
puntero a tabla, el binario pierde 184 bytes de datos y trece llamadas indirectas pasan a ser
directas.

### 5.7 El hueco: la sustituibilidad desapareció

En `construction-12`, esto compilaba y se ejecutaba:

```cpp
class SpareKnightPiece : public Piece { /* cuatro operaciones */ };
SpareKnightPiece playerPiece("player");
initializePositions(board, &playerPiece, &machinePiece);
```

En esta línea base no hay `Piece`, así que la misma sustitución no tiene dónde engancharse. Para que
el bucle acepte un segundo tipo de pieza tienes tres opciones, y cada una tiene un precio:

- **Duplicar el bucle** para el segundo tipo. Eso duplica diez funciones y la lógica de turnos.
- **Restaurar una interfaz.** Eso es `construction-12` otra vez, con su tabla y su indirección.
- **Parametrizar el bucle.** Eso es `construction-14`: una plantilla, sin clase base, y el bucle
  nombra un parámetro de tipo en lugar de `Knight`.

Mide la primera opción con honestidad en `EVIDENCE-es.md` §6.5: diez firmas auxiliares más
`run_game` necesitarían una segunda copia o un parámetro de tipo. La segunda opción es la lección
anterior. La tercera es la siguiente.

La aceptación de esta lección es, por tanto, precisa: **sustituir la herencia por un miembro y tres
reenvíos; comportamiento y las 20 trazas sin cambios.** No afirma que el bucle pueda aceptar un tipo
nuevo de pieza. No puede, y ese es el coste medido de este diseño.

## Paso 6 — La composición en el resto del programa (~8 min)

### 6.1 Un miembro es un valor, no una referencia

La sonda lo mostró: `Knight copy = playerKnight` copia el `Position`, y mover la copia deja el
original en `(3, 4)`. No se comparte nada. Eso es distinto de guardar un `Position *`, donde dos
caballos podrían apuntar a la misma casilla y un movimiento cambiaría a ambos.

La pertenencia por valor es la regla de propiedad más simple: el todo posee la parte, la parte muere
con el todo, y una copia es independiente. `construction-16` examina qué pasa cuando un miembro
posee memoria dinámica en lugar de dos enteros.

### 6.2 El reenvío acopla las dos interfaces

`Knight::set_position` llama a `position.set`. Si `Position` renombra `set` a `move`, el método de
reenvío deja de compilar hasta que se actualice. Eso es una ventaja: el compilador señala la línea
exacta. También es un coste: cada cambio en la interfaz de la parte se propaga a cada todo que la
reenvía.

Pruébalo en la copia temporal: renombra `Position::set` a `Position::move` en `position.h` y
`position.cpp`, recompila y lee el error. Nombra `Knight::set_position` en `knight.cpp:20`. Restaura
el nombre antes de continuar.

### 6.3 `const` sigue separando lectores de escritores

`tryMove` recibe `const Knight *` para ambos caballos y puede llamar a `row`, `column` y
`role_name`. No puede llamar a `set_position`. `playTurn` guarda `Knight *` para el caballo activo y
llama a `set_position` a través de `moveKnight`. El compilador comprueba la misma regla que comprobó
en `Board` en `construction-11` y en `Piece` en `construction-12`.

| Función | Parámetro Knight | Qué puede hacer |
|---|---|---|
| `initializePositions` | `Knight *` | Llamar a `set_position`. |
| `printGameHeader` | `const Knight *` | Leer `row`, `column`. |
| `printTurnHeader` | `const Knight *` | Leer `role_name`, `row`, `column`. |
| `printMove` | `const Knight *` | Leer `role_name`, `row`, `column`. |
| `tryMove` | `const Knight *` | Leer `row`, `column`. |
| `moveKnight` | `Knight *` | Llamar a `set_position`. |
| `playTurn` | `Knight *` y `const Knight *` | Mover el caballo activo; leer el otro. |

Rellena la tabla de `EVIDENCE-es.md` §7 con estas respuestas.

### 6.4 Sin destructor virtual, porque no hay base

`Knight` posee un `Position` y un `std::string` por valor. Cuando un `Knight` sale de ámbito, ambos
miembros se destruyen en orden inverso al de declaración, y el compilador genera ese código. No hay
`delete` a través de un puntero a base, así que no hay pregunta sobre el destructor.

Es una simplificación real, y es condicional. En el momento en que un miembro posee un recurso
mediante un puntero crudo, el todo necesita un destructor, un constructor de copia y una asignación
de copia, o un puntero inteligente. `construction-16` se ocupa de esa pregunta.

### 6.5 Qué emite el compilador

`nm -C knight.o` muestra `Knight::row`, `Knight::column`, `Knight::set_position` y
`Knight::role_name`, y ninguna `vtable` ni `typeinfo`. `objdump -d game13.o | grep -c 'call.*\*'`
imprime 0, frente a 13 en `game12.o`.

La llamada directa no es automáticamente más rápida de una forma que puedas ver en este programa: el
juego está dominado por `printf`. La afirmación honesta es estructural, no una medición de
rendimiento: no hay indirección que resolver ni tabla que cargar. Si eso importa depende de cuántas
veces se ejecuta la llamada y de qué más hace la función. Mide antes de afirmar una mejora de
velocidad.

### 6.6 Qué no oculta la composición

En `construction-12`, `grep -n 'KnightPiece' game.cpp` devolvía dos líneas: las dos construcciones.
En esta línea base, `Knight` aparece en diez firmas auxiliares y en dos construcciones. El bucle
está acoplado a un tipo concreto.

Esa es la inversión de la que trata la lección. La herencia trasladó la decisión de tipo a la raíz y
mantuvo el bucle genérico sobre la interfaz. La composición traslada el tipo a cada firma y mantiene
la llamada directa. Ninguna es gratuita; gastan la complejidad en lugares distintos.

## Paso 7 — Comparar y explicar (~5 min)

La comparación en una tabla:

| Pregunta | construction-12 | construction-13 |
|---|---|---|
| Relación | es-un | tiene-un |
| Parámetro del bucle | `Piece *` / `const Piece *` | `Knight *` / `const Knight *` |
| Dónde vive la posición | dos enteros en `KnightPiece` | un miembro `Position` |
| Quién responde `row` | `KnightPiece::row` virtual | `Knight::row` → `Position::row` |
| Resolución de la llamada | tipo dinámico | tipo declarado, en compilación |
| Tipo concreto en el bucle | dos construcciones | diez firmas y dos construcciones |
| Un segundo tipo con el mismo comportamiento | dos archivos, tres líneas en la raíz | diez firmas o una plantilla |
| `sizeof` de la pieza | 48 | 40 |
| Tabla virtual e información de tipo | 184 bytes | 0 |
| Llamadas indirectas en `game.o` | 13 | 0 |
| Texto de `game.o` | 3.688 bytes | 3.817 bytes |
| Texto / datos del binario | 9.583 / 876 | 9.400 / 692 |
| Código | 540 líneas, 10 archivos | 556 líneas, 11 archivos |
| Trazas | 20 de 20 | 20 de 20 |

Escribe `EVIDENCE-es.md` §8. Dos partes.

**Primero, una frase.** Di qué hace la pieza con su casilla, sin usar las palabras *composición*,
*herencia*, *delegación*, *clase*, *objeto*, *virtual*, *puntero*, *compilador*, *mejor*, *peor*,
*limpio*, *flexible*, *reutilizable* ni *elegante*. Si tu frase necesita una de ellas, has descrito
el mecanismo en lugar de la decisión.

**Segundo, un párrafo.** Responde:

- Qué fija la composición: la casilla se convierte en una parte con nombre y una sola
  implementación, y `Knight` la posee por valor.
- Qué lo impone: estado privado en ambos tipos, las operaciones públicas de `Position` como único
  acceso, y `const` en los lectores.
- Qué cambió respecto a `construction-12`: sin base, sin declaraciones virtuales, sin tabla; tres
  reenvíos y una llamada directa.
- Qué hacen los 19 puntos de llamada: seis lecturas de fila, seis de columna, tres escrituras de
  posición y cuatro lecturas de nombre, todas directas.
- Qué prueban las 20 trazas: el comportamiento del juego no cambió después del refactor.
- Qué no prueban las trazas: qué función se ejecutó, si existe una tabla o cuánto mide el objeto.
- Qué cuesta la prueba de cambio: tres archivos eliminados, cuatro añadidos, uno editado; 43 líneas
  añadidas y 44 eliminadas en `game.cpp`, todas renombrados; cero cambios de reglas.
- Qué no cubre la composición: la sustituibilidad. El bucle nombra `Knight` en diez firmas, así que
  un segundo tipo de pieza necesita una plantilla o una interfaz restaurada. La geometría del
  movimiento sigue en `tryMove`.
- Qué ahorra: 8 bytes por objeto, 184 bytes de datos de tabla y 13 llamadas indirectas.
- Qué cuesta: 16 líneas de código más, un archivo más, tres métodos de reenvío y un bucle acoplado a
  un tipo.
- Qué lección posterior se apoya en esta: `construction-14`, funciones genéricas, recupera un bucle
  para dos tipos sin base; `construction-15` añade restricciones.
- Qué lección posterior posee el ciclo de vida: `construction-16`.

Una respuesta sólida mantiene separadas cuatro cosas: **qué puede nombrar el bucle**, **qué decide
la parte**, **qué emite el compilador** y **qué puede ver una ejecución**. Una respuesta débil dice
"la composición es mejor", que colapsa las cuatro en un veredicto.

## Paso 8 — Comprobación de aceptación y registro (~2 min)

El criterio de aceptación de esta lección es: **sustituir la herencia por un miembro y un reenvío;
comportamiento y las 20 trazas sin cambios.**

Comprueba tu registro contra él:

- [ ] Identifiqué el miembro `Position` en `knight.h` y los tres métodos de reenvío en `knight.cpp`.
- [ ] Conté los 19 puntos de llamada en `game.cpp` y los asigné a funciones.
- [ ] Comprobé que ninguna cabecera contiene `virtual`, `override` ni `: public`.
- [ ] Nombré las diez firmas auxiliares que mencionan `Knight`.
- [ ] Predije la tabla de delegación y observé los valores reenviados.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Compilé la sonda de delegación y vi `role=player at=3,4` a través de `const Knight &`.
- [ ] Medí `sizeof(Position)=8` y `sizeof(Knight)=40`.
- [ ] Confirmé que copiar un caballo copia su posición y no comparte nada.
- [ ] Eliminé los tres archivos de interfaz en una copia temporal y añadí `Position` y `Knight`.
- [ ] Renombré el bucle con 43 líneas añadidas y 44 eliminadas, y cero cambios de reglas.
- [ ] Obtuve 20 de 20 trazas después del refactor.
- [ ] Conté 13 llamadas indirectas en `game12.o` y 0 en `game13.o`.
- [ ] Confirmé que `nm` no muestra tabla virtual ni información de tipo para `Knight`.
- [ ] Nombré la sustituibilidad como límite de este diseño.
- [ ] Expliqué por qué aquí no hace falta un destructor virtual.
- [ ] No edité ningún archivo de código versionado.
- [ ] Completé `EVIDENCE-es.md` §1–§10 y la autocomprobación de §12.

El criterio no es "el código usa composición". Es "puedes decir qué posee la parte, qué reenvía el
todo, qué tipo puede nombrar el bucle, qué ahorra el objeto y qué cede el diseño".

## Autocomprobación (binaria)

- [ ] Leí `position.h`, `knight.h` y `knight.cpp` y localicé los reenvíos por línea.
- [ ] Comparé `KnightPiece` con `Knight` y `Position` y nombré qué expone cada uno.
- [ ] Predije la tabla de delegación, el coste de la prueba de cambio y la sonda de tamaño antes de
      ejecutar.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Compilé la sonda de delegación y vi los valores reenviados.
- [ ] Confirmé que copiar un caballo copia su posición.
- [ ] Sustituí la herencia por un miembro y un reenvío en una copia temporal.
- [ ] Mostré 20 de 20 trazas después del refactor.
- [ ] Conté cero cambios de reglas y 87 líneas cambiadas en `game.cpp`.
- [ ] Medí las diferencias de objeto, tabla e indirección.
- [ ] Nombré la sustituibilidad como límite de la composición.
- [ ] Expliqué por qué no hace falta un destructor virtual.
- [ ] Completé `EVIDENCE-es.md`.

Estos son criterios de aceptación. Si una casilla
queda sin marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §10.

## Misconcepciones que esta lección refuta

| Error | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| "Composición significa no usar clases." | La composición es una relación entre clases: tiene-un en lugar de es-un. | Señala el miembro y las dos declaraciones de clase. |
| "La delegación es herencia con pasos extra." | No hay base, ni sustituibilidad, ni despacho dinámico. | Intenta pasar un `Knight` donde se esperaba un `Piece`; el tipo no existe. |
| "La composición siempre usa menos líneas." | Aquí usa 16 líneas más y un archivo más. | Cuenta el tipo nuevo, su unidad de traducción y los reenvíos. |
| "La composición siempre reduce el binario." | `game.o` crece 129 bytes; los datos se reducen 184. | Mide texto y datos por separado. |
| "El bucle ya no conoce el tipo concreto." | Lo conoce más: diez firmas nombran `Knight`. | Cuenta las firmas, no las construcciones. |
| "Un miembro es un puntero." | `Position position` es un valor; copiar el todo copia la parte. | Ejecuta la sonda de copia y lee el original. |
| "La delegación elimina la necesidad de una interfaz." | Elimina esta interfaz; un segundo tipo sigue necesitando una plantilla o una base. | Declara el siguiente cambio antes de afirmar generalidad. |
| "`Position` es solo dos enteros, así que es gratis." | Son 8 bytes, un constructor y una frontera de llamada que el compilador puede insertar en línea. | Mide `sizeof` y el código objeto. |
| "El destructor virtual sigue haciendo falta." | No hay base ni `delete` a través de una base; los miembros se destruyen solos. | Comprueba si hay propiedad mediante un puntero crudo antes de añadirlo. |
| "Las 20 trazas prueban la composición." | Prueban el comportamiento, no la estructura. | Usa `nm`, `size` y el conteo de indirección. |
| "La composición es mejor que la herencia." | Es más barata para este cambio y más cara para el siguiente. | Nombra el cambio y compara su coste. |
| "El refactor cambió el algoritmo." | Solo cambiaron nombres y el almacenamiento de la posición. | Compara las reglas: cero líneas cambiadas. |
| "El reenvío es duplicación que hay que quitar después." | Los reenvíos son la frontera; quitarlos elimina la encapsulación de la parte. | Consérvalos, o expón la parte de forma deliberada. |

## Siguiente paso

`construction-14` — funciones genéricas. Esta lección eliminó la clase base y pagó con un bucle
acoplado a `Knight`. La siguiente lección escribe un generador de movimientos para dos tipos de
pieza con una plantilla, de modo que el bucle nombre un parámetro de tipo en lugar de un tipo
concreto, y no vuelva ninguna tabla.

`construction-15` añade conceptos para restringir esa plantilla. `construction-16` posee la pregunta
del ciclo de vida que la pertenencia por valor solo aplaza. `construction-12` sigue siendo el otro
lado de este contraste; conserva ambas líneas base.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog.md) para la ruta completa.

Si faltaba alguna de las dos líneas base, no empieces `construction-14`. El siguiente contraste
necesita ambos lados.
