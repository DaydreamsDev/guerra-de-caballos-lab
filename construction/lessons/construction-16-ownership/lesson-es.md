# Propiedad y ciclo de vida de recursos (ownership and resource lifetime)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** ciclo de vida de recursos · **Tiempo:** unos 70 minutos ·
**Prerrequisitos:** construction-15 · **Estado:** borrador · **Lectura:** ~27 minutos
**Rol de la lección:** tronco

## Por qué existe esta lección

`construction-15` terminó con tres miembros por valor:

```cpp
Board<RowCount, ColumnCount> board;
Knight playerKnight;
Knight machineKnight;
```

Nadie preguntó quién los destruye, porque la respuesta era automática: el motor muere, los miembros
mueren, en orden inverso, y nada se había reservado. La clase no tenía ningún problema de ciclo de
vida que resolver.

Esta lección hace la pregunta que los miembros por valor escondían. ¿Quién posee un recurso? ¿Quién
lo libera? ¿Qué pasa cuando alguien copia al dueño? La línea base responde las tres en el tipo:

```cpp
std::unique_ptr<Board<RowCount, ColumnCount>> board;
std::unique_ptr<Knight> playerKnight;
std::unique_ptr<Knight> machineKnight;
```

La construcción y la asignación por copia están eliminadas. La construcción y la asignación por
movimiento están por defecto. No hay `new` ni `delete` crudos en ningún punto del motor.

La frase que conviene retener:

> La propiedad es la respuesta a «¿quién destruye esto, y cuándo?». `unique_ptr` convierte esa
> respuesta en un tipo: un solo dueño, destrucción cuando muere el dueño, copia rechazada en
> tiempo de compilación.

Tres notas honestas antes de la comparación.

1. El código crece 8 líneas, de 598 en doce archivos a 606 en doce. Once de los doce archivos son
   idénticos byte a byte a `construction-15`; solo cambió `game_engine.h`, de 292 a 300. El diff es
   un `include`, un constructor, cuatro funciones especiales, tres miembros de datos y `board.` que
   pasa a ser `board->` en diez puntos de llamada.
2. El motor se reduce de 336 bytes a 24, pero el programa no se reduce. El tablero y los dos
   caballos se mudaron al montón. Los bytes poseídos pasan de 336 a 360: 24 bytes de punteros más
   los mismos 336 bytes de objetos. Cada motor cuesta ahora tres reservas, y el texto enlazado crece
   242 bytes.
3. Para estos dos tipos, los miembros por valor ya no tenían fugas ni `new` crudo. Esta lección no
   afirma que `unique_ptr` arregle un error que existía. Afirma algo más estrecho: la propiedad se
   vuelve explícita, la copia se vuelve un error de compilación y la regla de vida se escribe una
   vez en lugar de quedar implícita en tres declaraciones. El precio son tres reservas y una
   indirección por cada acceso al tablero. El paso 6 mide ese precio.

La comparación no es «los punteros inteligentes son mejores». Es más precisa: esta línea base compra
un dueño declarado y un rechazo de copia en tiempo de compilación, y paga con reserva, indirección y
un estado movido desde el que `run()` no puede trabajar. Esta lección mide ambos lados y nombra el
estado que deja el movimiento por defecto.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar propiedad, ciclo de vida y los miembros especiales;
- mostrar propiedad con unique_ptr, copia eliminada y movimiento disponible.

## El concepto: ¿quién destruye esto, y cuándo?

### Tres preguntas, una respuesta

Todo recurso de un programa plantea las mismas tres preguntas:

1. **¿Quién lo reserva?** ¿Qué línea lo crea, y cuántas veces?
2. **¿Quién lo libera?** ¿Qué línea lo destruye, y esa línea se ejecuta en todos los caminos,
   incluidos los de error?
3. **¿Qué pasa cuando se copia al dueño?** ¿Dos dueños, un recurso, o dos recursos?

Los miembros por valor responden las tres de forma implícita. El motor está en la pila, así que el
tablero está en la pila. El compilador escribe el destructor, así que nadie escribe un `delete`.
Copiar el motor copia el tablero, lo cual es correcto pero silencioso.

`unique_ptr` responde las mismas tres preguntas en el sistema de tipos:

| Pregunta | Miembro por valor | Miembro `unique_ptr` |
|---|---|---|
| ¿Quién reserva? | el almacenamiento del objeto | `std::make_unique`, en el constructor |
| ¿Quién libera? | el destructor del compilador | el destructor del `unique_ptr`, al morir el dueño |
| ¿Copiar al dueño? | permitido, copia el valor | rechazado en tiempo de compilación |
| ¿Mover al dueño? | permitido, copia o mueve el valor | permitido, transfiere el puntero |
| Coste por acceso | ninguno | una indirección |
| Coste por objeto | ninguno | una reserva |

La tabla es la lección entera. Todo lo que sigue mide una fila u otra.

### Las cuatro funciones especiales

Una clase tiene cuatro operaciones que deciden cómo se copia y cómo se mueve. El compilador escribe
las cuatro salvo que digas lo contrario. Esta línea base dice lo contrario:

```cpp
GameEngine(const GameEngine &) = delete;
GameEngine &operator=(const GameEngine &) = delete;
GameEngine(GameEngine &&) = default;
GameEngine &operator=(GameEngine &&) = default;
```

Lee las cuatro líneas como un contrato:

| Función | Línea | Significado |
|---|---|---|
| Constructor de copia | 46 | Dos motores no pueden poseer el mismo tablero. Rechazado. |
| Asignación de copia | 47 | Un motor existente no puede tomar el tablero de otro. Rechazado. |
| Constructor de movimiento | 48 | Un motor nuevo puede tomar los punteros de uno que muere. Permitido. |
| Asignación de movimiento | 49 | Un motor existente puede tomar los punteros de otro. Permitido. |

Aquí está la parte sutil, y la razón de ser de esta lección. Como `unique_ptr` solo se mueve, el
compilador ya habría eliminado las operaciones de copia por su cuenta. Las líneas 46 y 47 no cambian
nada de lo que compila. Lo que cambian es dónde apunta el error y quién es responsable de la regla.

Sin las líneas 46–47, un intento de copia falla dentro de `std::unique_ptr`, con un mensaje sobre un
constructor eliminado en un encabezado estándar. Con ellas, el mensaje nombra `GameEngine` y la
línea 46. El paso 5 muestra ambos mensajes uno al lado del otro. La eliminación explícita es
documentación que el compilador hace cumplir.

Las operaciones de movimiento son el caso opuesto. El compilador también las escribiría, y las
escribiría bien: movimiento miembro a miembro de tres `unique_ptr`. Las líneas 48 y 49 dicen
`= default` para hacer visible la decisión. Quien lee no tiene que conocer las reglas de generación
implícita de funciones especiales para saber que esta clase se puede mover.

### Lo que `unique_ptr` hace y lo que no hace

Lo que hace:

- Garantiza exactamente un dueño. Dos `unique_ptr` no pueden apuntar al mismo objeto.
- Destruye lo apuntado cuando se destruye el dueño, incluso cuando el dueño es un miembro de un
  objeto cuyo constructor lanza a mitad de camino.
- Se niega a copiar, en tiempo de compilación, sin comprobación en ejecución y sin bandera.
- Cuesta lo mismo que un puntero crudo en tamaño: 8 bytes en esta plataforma, medidos en el paso 3.
- Se mueve en tiempo constante y no lanza. Un movimiento son tres asignaciones de puntero.

Lo que no hace:

- No elimina `new` ni `delete` del programa. Los traslada a `std::make_unique` y
  `std::default_delete`. `game.o` sigue referenciando `operator new` y `operator delete`; el paso 4
  muestra los símbolos. La reserva no desapareció, desapareció el punto de llamada.
- No vuelve `const` lo apuntado. `unique_ptr<Knight>` todavía te deja llamar a `set_position`.
- No hace gratis el acceso. Cada `board->` es una carga que `board.` no necesitaba.
- No define qué puede hacer un motor movido desde. Después de un movimiento los punteros son nulos y
  `run()` los desreferencia. El paso 5 reproduce la caída.
- No vuelve copiable la clase en sentido profundo. Si quieres una copia, tienes que escribirla, y
  entonces la decisión sobre qué significa una copia es tuya.

Los dos últimos son los límites honestos, y esta lección los registra en lugar de esconderlos.

### Por qué tres reservas y no una

El motor tiene tres objetos poseídos: un tablero y dos caballos. Cada `make_unique` es una reserva.
Una sola reserva para los tres sería posible con una estructura y un `unique_ptr` a ella, y esa
variante merece nombrarse: cambiaría tres reservas por una, y acoplaría los tres ciclos de vida en
uno. Esta línea base los mantiene separados porque el tablero y los dos caballos tienen identidades
independientes, y porque la lección trata de la regla de propiedad, no del número de reservas. El
paso 6 registra el recuento como un coste, no como un defecto.

## Antes de empezar

- [`construction-15`](../construction-15-generic-classes/lesson-es.md) — la línea base de la que
  parte esta lección. Su `game_engine.h` es el lado izquierdo del diff estructural del paso 6.
- [`construction-11`](../construction-11-encapsulation/lesson-es.md) — la frontera de clase.
  `Position` y `Knight` mantienen estado privado y operaciones públicas. La propiedad solo tiene
  sentido cuando una clase tiene una frontera detrás de la cual poseer.
- [`construction-09`](../construction-09-abstract-data-type/lesson-es.md) — independencia de la
  representación. Las operaciones públicas del tablero no cambiaron cuando su almacenamiento se mudó
  al montón.
- Un compilador de C++20 (`g++` o `clang++`). `cmake` es opcional; `run.sh` recurre a una
  compilación directa cuando CMake no está.

No necesitas saber cómo está implementado `unique_ptr`. Necesitas leer un encabezado, responder
quién reserva y quién libera, y mostrar que el compilador se niega a copiar al dueño.

### Qué necesitas

- La línea base con propiedad:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/game_engine.h`](code/game_engine.h),
  [`code/board.h`](code/board.h), [`code/move_generator.h`](code/move_generator.h),
  [`code/knight.h`](code/knight.h), [`code/knight.cpp`](code/knight.cpp),
  [`code/position.h`](code/position.h), [`code/position.cpp`](code/position.cpp),
  [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- La línea base anterior, para el diff:
  [`code/game_engine.h`](../construction-15-generic-classes/code/game_engine.h) y sus once
  compañeros en
  [`../construction-15-generic-classes/code/`](../construction-15-generic-classes/code/).
- Guiones de compilación y notas: [`run.sh`](run.sh), [`CMakeLists.txt`](CMakeLists.txt) y
  [`README-es.md`](README-es.md).
- Las reglas: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §1 fija el tablero
  y su numeración desde 1, §4 fija el orden de movimientos del caballo, §8 fija cuándo una casilla
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
test -f code/game_engine.h &&
test -f code/board.h &&
test -f code/move_generator.h &&
test -f code/knight.h &&
test -f code/knight.cpp &&
test -f code/position.h &&
test -f code/position.cpp &&
test -f code/random_sequence.h &&
test -f code/random_sequence.cpp &&
test -f ../construction-15-generic-classes/code/game_engine.h &&
test -f ../../../common/traces/seed-0001.txt
```

Si falta un archivo, la comparación no tiene segundo lado. Registra el archivo que falta en
`EVIDENCE-es.md` §11 y vuelve cuando esté disponible. Un recuento sobre un archivo que no has leído
no es evidencia.

La línea base de 8×8 reproduce las 20 trazas de `common/traces/` byte a byte, antes de que cambies
nada. Ese es el contrato de comportamiento, y esta lección no lo toca.

## Paso 1 — Inventariar la propiedad (~12 min)

Lee primero el encabezado. La forma es la lección:

```bash
grep -n 'unique_ptr\|make_unique\|GameEngine(const\|GameEngine(GameEngine\|operator=' \
  code/game_engine.h
```

```text
40:        : board(std::make_unique<Board<RowCount, ColumnCount>>()),
41:          playerKnight(std::make_unique<Knight>("player")),
42:          machineKnight(std::make_unique<Knight>("machine"))
46:    GameEngine(const GameEngine &) = delete;
47:    GameEngine &operator=(const GameEngine &) = delete;
48:    GameEngine(GameEngine &&) = default;
49:    GameEngine &operator=(GameEngine &&) = default;
71:    std::unique_ptr<Board<RowCount, ColumnCount>> board;
72:    std::unique_ptr<Knight> playerKnight;
73:    std::unique_ptr<Knight> machineKnight;
```

Ahora la prueba de ausencia. Este es el `grep` que nombra el criterio de aceptación:

```bash
grep -n '\bnew\b\|\bdelete\b\|malloc\|free(' code/*.h code/*.cpp
```

```text
code/game_engine.h:46:    GameEngine(const GameEngine &) = delete;
code/game_engine.h:47:    GameEngine &operator=(const GameEngine &) = delete;
```

Dos coincidencias, y ninguna libera memoria. Ambas son la palabra clave `delete` usada para prohibir
una operación. Es una coincidencia de vocabulario que vale la pena notar: la palabra que elimina una
función es la misma que libera una reserva, y este archivo solo usa el primer significado.

Rellena el mapa de líneas en `EVIDENCE-es.md` §2.2 desde el código, no desde esta tabla.

`game_engine.h`, rangos relevantes para la propiedad:

| Líneas | Contenido |
|---|---|
| 1–13 | Includes. La línea 12 añade `<memory>`. |
| 35–36 | La cabecera de plantilla y `class GameEngine`. |
| 39–44 | Constructor: tres llamadas a `make_unique` en orden de declaración. |
| 46–47 | Constructor de copia y asignación de copia, eliminados. |
| 48–49 | Constructor de movimiento y asignación de movimiento, por defecto. |
| 51–68 | `run()`. La línea 54 llama a `board->reset()`. |
| 71–73 | Los tres miembros que poseen. |
| 75–216 | Ayudas que leen a través de `board->` y de los dos caballos. |
| 218–284 | `playTurn`. |
| 286–299 | `playGame`. |

El resto del archivo es `construction-15` con `board.` reemplazado por `board->` y `&knight`
reemplazado por `knight.get()`.

Recuentos de esta línea base. La regla de conteo se enuncia para que puedas reproducirla:

- Encabezados: 7 (`board.h`, `game.h`, `game_engine.h`, `knight.h`, `move_generator.h`,
  `position.h`, `random_sequence.h`).
- Unidades de traducción: 5 (`main.cpp`, `game.cpp`, `knight.cpp`, `position.cpp`,
  `random_sequence.cpp`).
- Definiciones de clase completas: 5 (`Board`, `GameEngine`, `Knight`, `Position`,
  `MoveCandidate`).
- Cuerpos de función escritos a mano: 41, los mismos 41 que `construction-15`.
- Funciones especiales declaradas con `= delete` o `= default`: 4. Son declaraciones, no cuerpos
  escritos a mano, así que se cuentan aparte.
- Declaraciones de datos privados en encabezados: 10, contando los tres miembros que poseen, los dos
  campos de `Position`, los dos campos de `Knight`, `Board::visited` y los dos máximos de `Board`.
- Declaraciones públicas: 31, que son 27 de `construction-15` más las cuatro funciones especiales.
- Líneas de código: 606, que son 598 más 8.

Después responde en `EVIDENCE-es.md` §2:

1. ¿Qué tres miembros poseen recursos, y en qué líneas?
2. ¿Dónde se eliminan las operaciones de copia, y en qué líneas?
3. ¿Dónde se ponen por defecto las operaciones de movimiento, y en qué líneas?
4. ¿Cuántas llamadas a `make_unique` hay, y dónde?
5. ¿Qué encabezado proporciona `unique_ptr` y `make_unique`?
6. ¿Cuántas coincidencias devuelve el `grep` de `new`/`delete` crudos, y cuáles son?
7. ¿El orden de inicialización de las líneas 40–42 coincide con el orden de declaración de las
   líneas 71–73?
8. ¿Qué llama la línea 54, y qué llamaba en `construction-15`?
9. ¿Qué dos expresiones reemplazaron a `&playerKnight` y `&machineKnight`?
10. ¿Cuántos de los doce archivos difieren de `construction-15`?

La pregunta 7 tiene una respuesta precisa: sí, tablero, jugador, máquina, en ese orden. Si no fuera
así, GCC avisaría con `-Wreorder`, y `-Werror` detendría la compilación. El orden importa por una
razón: los miembros se destruyen en orden inverso al de declaración, así que el tablero sobrevive a
los dos caballos. Aquí nada depende de eso, pero quien lee debe saber en qué dirección corre la
regla.

La pregunta 9 tiene una respuesta precisa: `playerKnight.get()` y `machineKnight.get()`. `.get()`
devuelve el puntero crudo sin transferir la propiedad. El motor sigue poseyendo ambos caballos;
`playTurn` solo los presta durante un turno.

La pregunta 10 tiene una respuesta precisa: uno. Ejecuta `diff -rq` sobre los dos directorios
`code/` y cuenta.

## Paso 2 — Predecir antes de ejecutar (~10 min)

### 2.1 La tabla de tamaños

Escribe estas predicciones antes de ejecutar nada. No mires primero las respuestas del paso 3.

| Pregunta | Mi predicción |
|---|---|
| `sizeof(GameEngine<8, 8>)` en `construction-15` | |
| `sizeof(GameEngine<8, 8>)` aquí | |
| `sizeof(Board<8, 8>)` | |
| `sizeof(std::unique_ptr<Board<8, 8>>)` | |
| `sizeof(Knight)` | |
| Bytes poseídos totales: motor más tablero más dos caballos | |
| Reservas de montón por motor | |
| Liberaciones por motor, tras la destrucción | |
| Bytes de texto del binario enlazado, antes y después | |
| Bytes de datos del binario enlazado, antes y después | |

La fila que hay que tomar en serio es la cuarta. Un `unique_ptr` es un puntero. Si predijiste 256 o
264, predijiste lo apuntado. Lo apuntado ya no está dentro del motor; ese es el cambio completo.

La fila fácil de equivocar es la sexta. El motor se reduce 312 bytes, pero los objetos que posee no
se reducen. Suma los tres números y compara con 336.

### 2.2 La tabla de reservas

| Pregunta | Mi predicción |
|---|---|
| Llamadas a `make_unique` en el constructor | |
| Reservas contadas tras construir un motor | |
| Liberaciones contadas tras morir ese motor | |
| Reservas para veinte motores | |
| Liberaciones tras morir esos veinte | |
| ¿Depende el recuento de la semilla? | |
| ¿Depende el recuento del número de turnos? | |

Las dos últimas filas son la prueba estructural. La reserva ocurre en el constructor, no en el bucle
de juego. Si el recuento cambiara con la semilla, la reserva estaría dentro de `playTurn`, y ese
sería otro diseño.

### 2.3 Las predicciones de la prueba de cambio

La prueba de cambio es: preguntarle al compilador si este motor se puede copiar y si se puede mover.

| Pregunta | Mi predicción |
|---|---|
| `std::is_copy_constructible_v<GameEngine<8, 8>>` | |
| `std::is_copy_assignable_v<GameEngine<8, 8>>` | |
| `std::is_move_constructible_v<GameEngine<8, 8>>` | |
| `std::is_move_assignable_v<GameEngine<8, 8>>` | |
| ¿Compila un programa que copia un motor? | |
| ¿Qué línea nombra el error? | |
| ¿Un programa que mueve un motor y ejecuta el destino coincide con la semilla 1? | |
| ¿Un programa que mueve un motor y ejecuta el origen, cómo termina? | |
| Semillas que coinciden, de 20, tras el cambio | |

Predice la última fila como 20. El cambio es una regla de tipos; no toca el juego.

Predice la penúltima fila antes de ejecutarla. El motor movido desde tiene tres punteros nulos, y
`run()` llama a `board->reset()`. No es una predicción sutil una vez que escribes qué le hace un
movimiento al objeto de origen.

## Paso 3 — Ejecutar la línea base (~10 min)

Primero las trazas, sin modificar:

```bash
mkdir -p /tmp/ownership-test
./run.sh --seed 1 --name Learner > /tmp/ownership-test/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/ownership-test/baseline-0001.txt
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

Un diff vacío y un bucle silencioso: 20 de 20. Si `run.sh` no puede compilar, compila a mano:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o build/ownership-baseline code/main.cpp code/game.cpp code/knight.cpp \
  code/position.cpp code/random_sequence.cpp
```

Ahora los tamaños. La sonda va en el directorio temporal, nunca en los archivos rastreados:

```bash
cat > /tmp/ownership-test/sizes.cpp <<'EOF'
#include "game_engine.h"

#include <cstdio>

int main()
{
    std::printf("sizeof(Board<8,8>)=%zu\n", sizeof(Board<8, 8>));
    std::printf("sizeof(GameEngine<8,8>)=%zu\n", sizeof(GameEngine<8, 8>));
    std::printf("sizeof(unique_ptr<Board<8,8>>)=%zu\n",
                sizeof(std::unique_ptr<Board<8, 8>>));
    std::printf("sizeof(Knight)=%zu\n", sizeof(Knight));
    std::printf("sizeof(Position)=%zu\n", sizeof(Position));
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/ownership-test/sizes /tmp/ownership-test/sizes.cpp \
  code/knight.cpp code/position.cpp
/tmp/ownership-test/sizes
```

La salida en esta plataforma:

```text
sizeof(Board<8,8>)=256
sizeof(GameEngine<8,8>)=24
sizeof(unique_ptr<Board<8,8>>)=8
sizeof(Knight)=40
sizeof(Position)=8
```

Veinticuatro bytes, tres punteros, y ningún tablero dentro. El tablero son 256 bytes en otro sitio.

Ahora el contador de reservas. Esta sonda reemplaza los `operator new` y `operator delete` globales
por versiones que cuentan, así que funciona sin detector de fugas:

```bash
cat > /tmp/ownership-test/count.cpp <<'EOF'
#include "game_engine.h"

#include <cstdio>
#include <cstdlib>
#include <new>

namespace
{
long allocationCount = 0;
long deallocationCount = 0;
}

void *operator new(std::size_t size)
{
    allocationCount += 1;
    if (void *memory = std::malloc(size))
    {
        return memory;
    }
    throw std::bad_alloc();
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmismatched-new-delete"
void operator delete(void *memory) noexcept
{
    if (memory != nullptr)
    {
        deallocationCount += 1;
    }
    std::free(memory);
}

void operator delete(void *memory, std::size_t) noexcept
{
    operator delete(memory);
}
#pragma GCC diagnostic pop

int main()
{
    {
        GameEngine<8, 8> engine;
        std::printf("after construction: allocations=%ld frees=%ld\n",
                    allocationCount, deallocationCount);
    }
    std::printf("after destruction: allocations=%ld frees=%ld\n",
                allocationCount, deallocationCount);
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/ownership-test/count /tmp/ownership-test/count.cpp \
  code/knight.cpp code/position.cpp code/random_sequence.cpp
/tmp/ownership-test/count
```

```text
after construction: allocations=3 frees=0
after destruction: allocations=3 frees=3
```

Tres y tres. Cambia `GameEngine<8, 8> engine;` por `GameEngine<8, 8> engines[20];` y la misma sonda
imprime `60` y `60`. El recuento escala con el número de motores, no con el juego.

El `#pragma` está ahí porque GCC 16 avisa cuando un `operator new` reemplazado devuelve memoria de
`malloc` que libera un `operator delete` reemplazado. El aviso es sobre la sonda, no sobre el motor.
Si tu compilador no lo emite, el `pragma` es inofensivo.

Ahora los recuentos de símbolos, para ver si las operaciones de movimiento cuestan algo:

```bash
cat > /tmp/ownership-test/sym.cpp <<'EOF'
#include "game_engine.h"

int main()
{
    GameEngine<8, 8> engine;
    return engine.run(1, "Learner");
}
EOF
g++ -std=c++20 -O0 -Icode -c /tmp/ownership-test/sym.cpp \
  -o /tmp/ownership-test/sym-O0.o
g++ -std=c++20 -O2 -Icode -c /tmp/ownership-test/sym.cpp \
  -o /tmp/ownership-test/sym-O2.o
nm -C /tmp/ownership-test/sym-O0.o | grep -c 'GameEngine<8, 8>'
nm -C /tmp/ownership-test/sym-O2.o | grep -c 'GameEngine<8, 8>'
```

```text
24
1
```

Los mismos 24 y 1 que `construction-15`. Ahora busca las operaciones de movimiento por nombre:

```bash
nm -C /tmp/ownership-test/sym-O0.o | grep 'GameEngine<8, 8>::GameEngine' | sort | uniq -c
```

```text
      1 n GameEngine<8, 8>::GameEngine()
      2 W GameEngine<8, 8>::GameEngine()
```

Tres símbolos de constructor: dos débiles (el constructor por defecto, emitido una vez como
definición débil y una vez como alias local) y uno local. Ningún símbolo se llama
`GameEngine(GameEngine&&)`. El constructor de movimiento por defecto está definido en el cuerpo de
la clase, y el compilador lo inserta en el único punto de llamada que lo necesita. Un movimiento de
tres punteros no merece una llamada a función.

El destructor sí obtiene símbolo:

```bash
nm -C /tmp/ownership-test/sym-O0.o | grep '~GameEngine'
```

```text
W GameEngine<8, 8>::~GameEngine()
```

Ese símbolo es donde viven las tres liberaciones. El motor no escribió un destructor; el compilador
escribió uno que destruye tres `unique_ptr`, y cada uno de esos llama a `delete` sobre lo apuntado.

Si el primer diff no está vacío, detente. Esa es otra línea base distinta de la que describe esta
lección; regístrala en `EVIDENCE-es.md` §11 y no ajustes la predicción para que coincida.

## Paso 4 — Comparar: qué demuestra el `unique_ptr` (~10 min)

Abre `game_engine.h` y `game.cpp`. Cinco hechos, leídos contra las mediciones.

1. **El motor ya no contiene el tablero.** `sizeof(GameEngine<8, 8>)` es 24, antes 336. Tres
   punteros de ocho bytes. El tablero son 256 bytes en el montón.
2. **Los objetos no se redujeron.** `sizeof(Board<8, 8>)` sigue siendo 256 y `sizeof(Knight)` sigue
   siendo 40. Los bytes poseídos son 24 + 256 + 80 = 360, antes 336. Los 24 bytes extra son los tres
   punteros. Cambiaste 312 bytes de pila por 360 bytes de montón más 24 bytes de contabilidad.
3. **La reserva es real.** `game.o` tiene referencias indefinidas a `operator new` y `operator
   delete`. El código del motor no tiene `new` ni `delete`. Ambas afirmaciones son ciertas a la vez,
   y quien compruebe solo una sacará la conclusión equivocada.
4. **El recuento de reservas lo fija el constructor.** Tres por motor, tres liberaciones al morir, y
   veinte motores dan sesenta y sesenta. El bucle de juego no reserva nada.
5. **Las operaciones de movimiento no cuestan símbolo.** El constructor de movimiento por defecto se
   inserta en línea. El destructor sí obtiene símbolo, y ahí están las liberaciones.

### 4.1 La reubicación que demuestra el punto

```bash
g++ -std=c++20 -O2 -Icode -c code/game.cpp -o /tmp/ownership-test/game16.o
nm -C /tmp/ownership-test/game16.o | grep -i 'operator new\|operator delete'
```

```text
                 U operator delete(void*, unsigned long)
                 U operator new(unsigned long)
```

`U` significa indefinido: esta unidad de traducción las llama y no las define. El enlazador las
proporciona desde el tiempo de ejecución de C++. `std::make_unique` llama a `operator new`, y
`std::default_delete` llama a `operator delete`. La regla de propiedad trasladó la llamada a un tipo
de biblioteca; no eliminó la llamada.

Compara el tamaño de texto:

```bash
g++ -std=c++20 -O2 -I../construction-15-generic-classes/code \
  -c ../construction-15-generic-classes/code/game.cpp \
  -o /tmp/ownership-test/game15.o
size /tmp/ownership-test/game15.o /tmp/ownership-test/game16.o
```

```text
   text	   data	    bss	    dec	    hex	filename
   4392	      0	      0	   4392	   1128	/tmp/ownership-test/game15.o
   4638	      0	      0	   4638	   121e	/tmp/ownership-test/game16.o
```

`game.o` crece 246 bytes. El binario enlazado crece 242:

```text
   text	   data	    bss	    dec	    hex	filename
   9410	    692	     24	  10126	   278e	binario de construction-15
   9652	    692	     24	  10368	   2880	binario de construction-16
```

Datos y BSS no se mueven. El tablero dejó la pila, pero la pila no está en el binario; el montón
tampoco. El crecimiento de texto es la reserva, el destructor y la carga extra por acceso.

### 4.2 Lo que no cambió

- La transcripción. Las 20 trazas coinciden, byte a byte. La propiedad no es observable en la
  salida.
- Las reglas. Orden de movimientos, captura, trampa, empate y el límite de 200 movimientos siguen
  intactos.
- La interfaz del tablero. `reset`, `was_visited`, `mark_visited`, `row_count`, `column_count` son
  las mismas llamadas; solo cambió la sintaxis para llegar a ellas, de `.` a `->`.
- El contrato de la pieza. `PositionedPiece` y su `static_assert` no cambiaron.
- Las restricciones de dimensión. `Board` sigue comprobando 1..16 en `board.h:72–73`.
- Las llamadas indirectas en `game.o`. Cero antes, cero después. `unique_ptr` no añade despacho
  virtual; el destructor no es virtual, y `default_delete` se resuelve en tiempo de compilación.

El último punto merece enunciarse porque es una suposición común. Los punteros inteligentes no
implican una vtable. Son una plantilla con un puntero dentro, y el `delete` es una llamada directa.

## Paso 5 — Ejecutar la prueba de cambio (~12 min)

La prueba de cambio es la que nombra el criterio de aceptación: preguntarle al compilador qué
permite.

### 5.1 La sonda en tiempo de compilación

```bash
cat > /tmp/ownership-test/probe.cpp <<'EOF'
#include "game_engine.h"

#include <type_traits>

static_assert(!std::is_copy_constructible_v<GameEngine<8, 8>>);
static_assert(!std::is_copy_assignable_v<GameEngine<8, 8>>);
static_assert(std::is_move_constructible_v<GameEngine<8, 8>>);
static_assert(std::is_move_assignable_v<GameEngine<8, 8>>);
static_assert(std::is_destructible_v<GameEngine<8, 8>>);

int main()
{
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode -c \
  /tmp/ownership-test/probe.cpp -o /tmp/ownership-test/probe.o
```

Sin salida y con un archivo `.o`: las cinco afirmaciones se cumplen. Nota que `-c` basta. La sonda
no necesita `knight.cpp`, ni `position.cpp`, ni `random_sequence.cpp`, porque nunca ejecuta el
motor. La pregunta la responde el sistema de tipos solo, que es el punto.

Las tres afirmaciones que nombra el criterio de aceptación son las tres primeras. La cuarta y la
quinta están para mostrar que la asignación de movimiento y la destrucción también están
disponibles; sin ellas, «se puede mover» sería una afirmación sobre la mitad de las operaciones.

### 5.2 Los fallos de copia

Ahora pide la copia que la clase rechaza:

```bash
cat > /tmp/ownership-test/copy_fail.cpp <<'EOF'
#include "game_engine.h"

int main()
{
    GameEngine<8, 8> original;
    GameEngine<8, 8> copy(original);
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode -c \
  /tmp/ownership-test/copy_fail.cpp -o /tmp/ownership-test/copy_fail.o
```

El compilador informa, en la línea que intenta la copia:

```text
code/game_engine.h:46:5: error: use of deleted function
  'GameEngine<RowCount, ColumnCount>::GameEngine(
     const GameEngine<RowCount, ColumnCount>&) [with int RowCount = 8;
     int ColumnCount = 8]'
   46 |     GameEngine(const GameEngine &) = delete;
      |     ^~~~~~~~~~
```

Lo mismo para la asignación:

```bash
cat > /tmp/ownership-test/copy_assign.cpp <<'EOF'
#include "game_engine.h"

int main()
{
    GameEngine<8, 8> original;
    GameEngine<8, 8> other;
    other = original;
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode -c \
  /tmp/ownership-test/copy_assign.cpp -o /tmp/ownership-test/copy_assign.o
```

```text
code/game_engine.h:47:17: error: use of deleted function
  'GameEngine<RowCount, ColumnCount>&
   GameEngine<RowCount, ColumnCount>::operator=(
     const GameEngine<RowCount, ColumnCount>&) [with int RowCount = 8;
     int ColumnCount = 8]'
   47 |     GameEngine &operator=(const GameEngine &) = delete;
      |                 ^~~~~~~~
```

Ambos errores apuntan a la clase, a la línea que enuncia la regla. Esa es la diferencia entre una
eliminación implícita y una explícita. Para ver la versión implícita, comenta las líneas 46 y 47 en
una copia temporal del encabezado y repite la compilación. El error se muda dentro de
`std::unique_ptr` y nombra un constructor eliminado dentro de un encabezado estándar. La regla no
cambió; solo cambió su dirección.

Haz ese experimento en `/tmp`, nunca en `code/`. El encabezado rastreado se queda como está.

### 5.3 El movimiento que funciona

```bash
cat > /tmp/ownership-test/move_demo.cpp <<'EOF'
#include "game_engine.h"

int main()
{
    GameEngine<8, 8> original;
    GameEngine<8, 8> moved(std::move(original));
    return moved.run(1, "Learner");
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/ownership-test/move_demo /tmp/ownership-test/move_demo.cpp \
  code/knight.cpp code/position.cpp code/random_sequence.cpp
/tmp/ownership-test/move_demo > /tmp/ownership-test/moved-0001.txt
cmp /tmp/ownership-test/moved-0001.txt ../../../common/traces/seed-0001.txt
```

`cmp` vacío: el motor de destino juega el mismo juego. Tres punteros cambiaron de dueño, y el
comportamiento no. Para eso sirve un movimiento.

### 5.4 El estado movido desde

Ahora el otro lado del mismo movimiento:

```bash
cat > /tmp/ownership-test/moved_from.cpp <<'EOF'
#include "game_engine.h"

int main()
{
    GameEngine<8, 8> original;
    GameEngine<8, 8> moved(std::move(original));
    return original.run(1, "Learner");
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/ownership-test/moved_from /tmp/ownership-test/moved_from.cpp \
  code/knight.cpp code/position.cpp code/random_sequence.cpp
/tmp/ownership-test/moved_from > /tmp/ownership-test/moved_from.txt
echo "exit=$?"
```

En esta plataforma:

```text
Segmentation fault (core dumped)
exit=139
```

Lee la razón, no el número. Un `unique_ptr` movido desde es nulo. `run()` llama a `board->reset()`,
que desreferencia un puntero nulo. La regla de C++ dice que un objeto movido desde es *válido pero
no especificado*: puedes destruirlo y puedes asignarle, y nada más. Llamar a `run()` sobre él queda
fuera del contrato, y la violación del contrato es comportamiento indefinido, no un código de error.

Este es el hueco honesto del diseño, y tiene nombre: `run()` supone que el motor está en el estado
construido, y nada en el tipo lo impone. Hay tres reparaciones posibles; esta lección no elige una:

1. Hacer que `run()` compruebe los punteros y devuelva un error. Eso es un contrato de error, que es
   `construction-22`.
2. Hacer de `run()` una función libre que reciba el motor por referencia y se llame solo sobre un
   objeto construido. Eso es un cambio de diseño, no de ciclo de vida.
3. No mover motores: construirlos donde se usan, como hace `game.cpp` en la línea 8.

La línea base usa la tercera opción por accidente: `game.cpp` nunca mueve un motor. Las operaciones
de movimiento existen porque el criterio de aceptación las exige, y porque una clase que posee
recursos debe declarar qué significan copiar y mover. Una clase que se puede mover y no se puede
ejecutar después de un movimiento es una clase normal de C++; la lección es que «se puede mover» y
«se puede usar después de mover» son afirmaciones distintas.

### 5.5 La evidencia de fugas cuando no se ejecuta un detector

Un detector de fugas es la evidencia habitual de esta lección, y en esta máquina no corre ninguno:

```bash
command -v valgrind
g++ -fsanitize=address /tmp/ownership-test/asan.cpp -o /tmp/ownership-test/asan
```

```text
valgrind: absent
/usr/bin/ld.bfd: cannot find /usr/lib64/libasan.so.8.0.0
```

Así que el registro tiene dos clases de evidencia, y debe mantenerlas separadas.

**Evidencia estructural.** Léela, no la ejecutes:

- Tres miembros que poseen, todos `std::unique_ptr` (`game_engine.h:71–73`).
- Tres llamadas a `make_unique`, una por miembro, todas en el constructor (`game_engine.h:40–42`).
- Ni `new` crudo, ni `delete` crudo, ni `malloc`, ni `free` en ninguno de los doce archivos. Los
  únicos tokens `delete` son las dos operaciones de copia eliminadas.
- Copia eliminada, así que dos motores no pueden compartir un puntero.
- Movimiento por defecto, así que la propiedad se transfiere en lugar de duplicarse.
- Sin destructor escrito a mano, así que no hay camino donde una liberación se olvide o se repita.

**Evidencia empírica.** Ejecútala:

- La sonda contadora imprime 3 reservas y 3 liberaciones para un motor, 60 y 60 para veinte.
- `game.o` referencia `operator new` y `operator delete`, así que las reservas son reales y las
  llamadas correspondientes existen.
- El símbolo del destructor existe, así que hay un lugar donde corren las liberaciones.

Lo que esta evidencia no demuestra: que las liberaciones ocurran exactamente una vez en todos los
caminos, incluido un camino donde el constructor lanza. La sonda contadora cubre el camino normal.
El camino de excepción se argumenta estructuralmente, no se mide, porque en este programa no ocurre
ninguna excepción y no hay sanitizador disponible para forzarla.

Registra ese hueco en `EVIDENCE-es.md` §5.5. No escribas «sin fugas, verificado con ASan» cuando
ASan no corrió. Escribe «sin fugas en el camino normal, contadas; argumento estructural para el
camino de excepción; sin sanitizador disponible en esta máquina». Esa frase vale más que una casilla
falsa.

Si tienes un detector, ejecútalo y registra su salida exacta en su lugar. La lección no depende de
qué herramienta produjo la evidencia; depende de que el registro diga qué herramienta fue.

## Paso 6 — Comparar con `construction-15` (~8 min)

El diff estructural es donde la decisión de propiedad muestra su coste.

| Métrica | construction-15 | construction-16 | Diferencia |
|---|---|---|---|
| Archivos | 12 | 12 | 0 |
| Encabezados | 7 | 7 | 0 |
| Unidades de traducción | 5 | 5 | 0 |
| Líneas de código | 598 | 606 | +8 |
| `game_engine.h` | 292 | 300 | +8 |
| Otros 11 archivos | — | idénticos byte a byte | 0 |
| Clases completas | 5 | 5 | 0 |
| Cuerpos de función escritos a mano | 41 | 41 | 0 |
| Funciones especiales declaradas | 0 | 4 | +4 |
| Declaraciones públicas | 27 | 31 | +4 |
| `sizeof(GameEngine<8, 8>)` | 336 | 24 | -312 |
| Reservas de montón por motor | 0 | 3 | +3 |
| Bytes poseídos, total | 336 | 360 | +24 |
| Texto de `game.o` | 4.392 | 4.638 | +246 |
| Texto / datos del binario | 9.410 / 692 | 9.652 / 692 | +242 / 0 |
| Llamadas indirectas en `game.o` | 0 | 0 | 0 |
| Trazas | 20 de 20 | 20 de 20 | 0 |

### 6.1 El diff exacto

Solo cambió un archivo:

```bash
diff -u ../construction-15-generic-classes/code/game_engine.h code/game_engine.h
```

El diff tiene nueve bloques: 27 líneas añadidas y 19 eliminadas, para un crecimiento neto de ocho.
En palabras:

| Tipo de cambio | Cantidad |
|---|---|
| `#include <memory>` añadido | 1 |
| Líneas del constructor reescritas | 3 |
| Funciones especiales añadidas: dos eliminadas, dos por defecto | 4 |
| Miembros de datos reescritos como `unique_ptr` | 3 |
| Accesos `board.` reescritos como `board->` | 10 |
| Accesos a miembros de caballo reescritos como `->` | 6 |
| Expresiones `&knight` reescritas como `knight.get()` | 4 |

Ese es el cambio de propiedad completo. El bucle de turnos, las reglas y el formato de transcripción
no se tocaron.

### 6.2 Qué compra

- Un dueño declarado. `board` posee el tablero; nada más puede.
- Copia rechazada en tiempo de compilación, con el error en la clase, no en un encabezado estándar.
- Movimiento permitido, así que el motor puede devolverse desde una función o guardarse en un
  contenedor.
- Seguridad ante excepciones en el constructor. Si el tercer `make_unique` lanza, los dos miembros
  `unique_ptr` ya construidos se destruyen y sus objetos se liberan. Con tres punteros crudos y un
  destructor escrito a mano, el mismo constructor fuga dos reservas salvo que añadas un
  `try`/`catch`. Es una ventaja real, argumentada desde las reglas del lenguaje, no medida aquí.
- Sin destructor escrito a mano, así que ningún camino olvida una liberación.
- Un motor más pequeño: 24 bytes en lugar de 336, lo cual importa si guardas muchos motores.

### 6.3 Qué cuesta

- Tres reservas por motor. Con 20 motores, 60 reservas; con 10.000 motores, 30.000. Es un coste real
  para la lección orientada a datos de `construction-19`.
- Una indirección por acceso al tablero. Diez puntos de llamada en `game_engine.h` cargan ahora un
  puntero primero.
- 242 bytes más de texto, 246 en `game.o`.
- Un estado movido desde que `run()` no puede manejar. La clase se puede mover, pero no se puede
  usar después de un movimiento.
- Ninguna copia profunda. Si necesitas dos motores con la misma posición, esta clase no te los da.
  Escribirías un `clone()` o un constructor de copia que reserve nuevos objetos, y entonces la
  decisión sobre qué significa «la misma posición» es tuya.
- El motor ya no es trivialmente copiable, lo cual cambia qué contenedores y algoritmos lo aceptan.

### 6.4 El hueco: la propiedad es explícita, pero el tablero no se comparte

`GameEngine` posee el tablero, y `playTurn` toma prestados dos caballos mediante `.get()`. Esa es
una historia de propiedad limpia dentro de un motor. No dice nada sobre dos motores, dos hilos, o un
motor entregado a dos funciones.

La línea base también mantiene la secuencia aleatoria en una variable de ámbito de archivo
(`random_sequence.cpp:7`). La propiedad del tablero y de los caballos es explícita; la propiedad del
estado del generador no lo es. `run()` lo reinicia con `random_set_seed(seed)` en la línea 53, y por
eso dos motores ejecutados en secuencia producen la misma transcripción. Dos motores ejecutados al
mismo tiempo compartirían un generador, y las transcripciones dependerían del entrelazado. Ese es el
problema de concurrencia, y `construction-21` se ocupa de él. Esta lección lo nombra y se detiene.

La frase de aceptación precisa es más estrecha que «el programa no tiene fugas»:

> La persona que aprende puede mostrar que el motor posee sus recursos con `unique_ptr`, que la
> copia está eliminada explícitamente, que el movimiento está disponible y que las 20 trazas
> siguen pasando.

Cuatro afirmaciones, cuatro piezas de evidencia: los tres miembros, las líneas 46–47, las líneas
48–49 y el bucle de trazas. Nada sobre compartir, hilos o copias profundas.

## Paso 7 — Explicar (~6 min)

Escribe `EVIDENCE-es.md` §8. Dos partes.

**Primero, una frase.** Di qué cambió entre los dos motores y qué decide el cambio, sin usar las
palabras *puntero inteligente*, *propiedad*, *ciclo de vida*, *RAII*, *destructor*, *mejor*, *peor*,
*limpio*, *seguro* o *moderno*. Si tu frase necesita una de ellas, nombraste el mecanismo y no la
decisión.

**Segundo, un párrafo.** Responde:

- Qué deciden los tres miembros: un dueño cada uno, liberado al morir el motor.
- Qué deciden las líneas 46–47: dos motores no pueden compartir un tablero, y el rechazo es un error
  de compilación.
- Qué deciden las líneas 48–49: la propiedad se puede transferir, en tiempo constante, sin copiar el
  tablero.
- Qué demuestra el `grep` de `new`/`delete`: ningún punto de llamada del motor reserva ni libera a
  mano.
- Qué demuestra la salida de `nm`: la reserva sigue ocurriendo, a través de `operator new`.
- Qué demuestra la sonda contadora: tres reservas y tres liberaciones por motor, en el camino
  normal.
- Qué demuestran las trazas: el cambio de propiedad es invisible en la transcripción.
- Qué demuestra la caída del motor movido desde: poder moverse no es lo mismo que poder usarse
  después de un movimiento.
- Cuál es el coste: tres reservas, una indirección, 242 bytes de texto, 24 bytes extra de
  contabilidad.
- Qué tenía ya la versión con miembros por valor: ninguna fuga, ningún `new` crudo y una copia que
  funcionaba.
- Qué lección posterior se ocupa de la abstracción algorítmica: `construction-17`.
- Qué lección se ocupa del estado compartido del generador: `construction-21`.

Una respuesta fuerte mantiene separadas cuatro cosas: **quién posee** (los tres `unique_ptr`), **qué
rechaza el compilador** (la copia), **qué permite** (el movimiento) y **qué puede ver una
ejecución** (la transcripción, sin cambios). Una respuesta débil dice «los punteros inteligentes
gestionan la memoria automáticamente», lo cual colapsa las tres primeras en un eslogan e ignora la
cuarta.

## Paso 8 — Comprobación de aceptación y registro (~2 min)

El criterio de aceptación de esta lección es: **mostrar que el motor posee sus recursos con
`unique_ptr`, que la copia está eliminada explícitamente, que el movimiento está disponible y que
las 20 trazas siguen pasando.**

Comprueba tu registro contra él:

- [ ] Localicé los tres miembros que poseen y sus líneas.
- [ ] Localicé las dos operaciones de copia eliminadas y las dos de movimiento por defecto.
- [ ] Confirmé que el `grep` de `new`/`delete` crudos devuelve solo las dos líneas `= delete`.
- [ ] Confirmé que `game.o` sigue referenciando `operator new` y `operator delete`.
- [ ] Compilé la sonda con los tres `static_assert` y compiló.
- [ ] Reproduje el error de construcción de copia y leí la línea que nombra.
- [ ] Reproduje el error de asignación de copia y leí la línea que nombra.
- [ ] Moví un motor y el motor de destino reprodujo la semilla 1.
- [ ] Ejecuté el motor movido desde y registré qué pasó.
- [ ] Conté tres reservas y tres liberaciones para un motor.
- [ ] Conté sesenta y sesenta para veinte motores.
- [ ] Medí `sizeof(GameEngine<8, 8>)` en 24 y `sizeof(Board<8, 8>)` en 256.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Registré que no había detector de fugas y qué evidencia usé en su lugar.
- [ ] No edité ningún archivo de código rastreado.
- [ ] Completé `EVIDENCE-es.md` §1–§10 y la autocomprobación de §12.

El criterio no es «el código usa `unique_ptr`». Es «puedes decir quién posee qué, qué rechaza el
compilador, qué permite y qué muestra la ejecución».

## Autocomprobación (binaria)

- [ ] Leí `game_engine.h` y localicé los tres dueños, las cuatro funciones especiales y las llamadas
      a `make_unique` por línea.
- [ ] Comparé la línea base con propiedad con `construction-15` y nombré las ocho líneas añadidas.
- [ ] Predije la tabla de tamaños, la tabla de reservas y la prueba de cambio antes de ejecutar.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Compilé la sonda con `static_assert(!std::is_copy_constructible_v<GameEngine<8, 8>>)`,
      `static_assert(!std::is_copy_assignable_v<GameEngine<8, 8>>)` y
      `static_assert(std::is_move_constructible_v<GameEngine<8, 8>>)`.
- [ ] Reproduje ambos errores de copia y leí la línea que nombra cada uno.
- [ ] Moví un motor y coincidí con la semilla 1 usando el objeto de destino.
- [ ] Ejecuté el motor movido desde y registré el resultado.
- [ ] Conté reservas y liberaciones para un motor y para veinte.
- [ ] Medí el motor en 24 bytes y el tablero en 256.
- [ ] Registré que no había detector de fugas y usé evidencia estructural más contada.
- [ ] Nombré qué no cubre el cambio de propiedad: el generador compartido y el estado movido desde.
- [ ] Completé `EVIDENCE-es.md`.

Son criterios de aceptación. Si una casilla queda sin marcar,
escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §10.

## Misconcepciones que esta lección refuta

| Error | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| «`unique_ptr` elimina `new` y `delete` del programa.» | Los traslada a `make_unique` y `default_delete`; `game.o` sigue llamándolos. | `nm -C game.o \| grep 'operator new'`. |
| «El motor no reserva porque `grep new` está vacío.» | El `grep` busca en el código, no en el archivo objeto. | Busca en ambos y di en cuál buscaste. |
| «Eliminar las operaciones de copia es lo que vuelve no copiable la clase.» | `unique_ptr` ya lo hace; las líneas cambian el diagnóstico, no la regla. | Coméntalas en un encabezado temporal y compara los errores. |
| «`= default` en las operaciones de movimiento no hace nada.» | Enuncia la decisión y mantiene las operaciones disponibles cuando un miembro las suprimiría. | Quita una y comprueba `is_move_constructible_v`. |
| «Un motor movido desde todavía se puede usar.» | Sus punteros son nulos; `run()` los desreferencia. | Ejecútalo, registra la caída y nombra la reparación. |
| «`unique_ptr` añade una vtable.» | Es una plantilla con un puntero; la liberación es una llamada directa. | Cuenta llamadas indirectas en `game.o`; no hay ninguna. |
| «El motor es más pequeño, así que el programa usa menos memoria.» | El motor son 24 bytes, pero posee 336 bytes en el montón más 24 de punteros. | Suma los bytes poseídos y compara con 336. |
| «El tablero se copia dentro del `unique_ptr`.» | El puntero apunta a un objeto del montón; el tablero no está dentro del motor. | Mide `sizeof(GameEngine)` y `sizeof(Board)`. |
| «Sin fugas significa sin errores.» | La caída del motor movido desde es un error que un detector de fugas no reportaría. | Prueba el camino movido desde por separado. |
| «La propiedad resuelve el compartir.» | Prohíbe compartir dentro de un motor y no dice nada sobre dos motores o dos hilos. | Lee el generador en `random_sequence.cpp:7`. |
| «`unique_ptr` hace gratis el acceso.» | Cada `board->` es una carga que `board.` no necesitaba. | Cuenta los diez puntos de llamada y los 242 bytes extra. |
| «Volver a añadir una copia sería trivial.» | Una copia profunda exige decidir qué copiar y quién posee el resultado. | Escribe el constructor de copia que necesitarías; nota qué reserva. |
| «Las 20 trazas demuestran que la propiedad es correcta.» | Demuestran que el comportamiento no cambió; no dicen nada sobre liberaciones. | Usa la sonda contadora y el argumento estructural. |
| «`std::move` mueve algo.» | Es una conversión a referencia de rvalue; el constructor de movimiento hace el trabajo. | Lee el constructor de movimiento: tres asignaciones de puntero. |
| «Los miembros por valor tenían fugas.» | Nunca reservaban, así que no había nada que fugar. | Di qué compra el cambio, no qué arregla. |

## Siguiente paso

`construction-17` se ocupa de la abstracción algorítmica. El motor todavía escribe sus bucles a
mano: el orden de movimientos, el recorrido de candidatos, el reinicio del tablero. `std::array`,
`find_if` y los rangos expresan los mismos recorridos sin el índice manual. La pregunta de propiedad
se cierra aquí y no se reabre allí; los cuerpos de los bucles son lo siguiente que cambia.

Quedan dos hilos abiertos que pertenecen a lecciones posteriores. El estado del generador en
`random_sequence.cpp:7` sigue siendo global, así que dos motores lo comparten, y `construction-21`
se ocupa de ese problema. El motor movido desde todavía se puede ejecutar para el sistema de tipos y
no se puede ejecutar de hecho, y `construction-22` se ocupa del contrato de error que lo convertiría
en un valor en lugar de una caída.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog.md) para la ruta completa.

Si faltaba cualquiera de las dos líneas base, no empieces `construction-16`. El siguiente contraste
necesita ambos lados.
