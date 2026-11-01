# Estilo funcional: pureza e inmutabilidad (functional style)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** pureza e inmutabilidad · **Tiempo:** unos 85 minutos · **Estado:** borrador
**Lectura:** ~23 minutos

## Por qué existe esta lección

`construction-17` terminó con una pregunta que no podía responder. Tres recorridos tenían nombre y
un bucle no, porque su trabajo por elemento imprime, muta y bifurca. Darle nombre al bucle habría
escondido sus efectos. Esta lección hace otra pregunta sobre el mismo código:

> ¿Cuáles de esos efectos tienen que existir, y cuál se puede reemplazar por un valor?

La respuesta es más estrecha que «hacer el programa funcional». Una función ya es pura y no cambia.
Una clase pasa a ser un valor que se copia en lugar de mutarse. Un contenedor guarda las copias. Y
el bucle del juego sigue mutando, porque un turno todavía tiene que imprimir, decidir y mover un
caballo.

La frase que conviene retener:

> Una función pura devuelve el mismo valor para la misma entrada y no cambia nada. Un valor
> inmutable nunca se modifica después de construirse; un «cambio» produce un valor nuevo. Cuando los
> valores anteriores se conservan, deshacer deja de ser un cálculo y pasa a ser una consulta.

Tres notas honestas antes de la comparación.

1. Cambia un archivo. `game_engine.h` crece de 302 a 346 líneas, y el diff añade 44 líneas y no
   elimina ninguna. Los otros once archivos son idénticos byte a byte a `construction-17`. Eso
   incluye `board.h` y `move_generator.h`: la pureza en la generación de movimientos y la semántica
   de valor del tablero ya estaban ahí. Esta lección añade inmutabilidad en la frontera del
   historial, no pureza en las hojas.
2. La estructura apenas se mueve: 5 encabezados de `for`, 3 puntos de llamada a algoritmos, 3
   lambdas, de 41 a 45 cuerpos de función con nombre, de 31 a 34 declaraciones públicas. Lo que
   cambia es el tamaño del motor, de 24 a 56 bytes, porque ahora posee un `std::vector` y un
   `std::size_t`.
3. El coste es memoria, y crece con la partida. En esta máquina, las 20 semillas de referencia
   asignan entre 8.272 y 32.848 bytes cada una, frente a 336 bytes fijos en `construction-17`.
   Deshacer se vuelve barato en tiempo y caro en espacio. Ese intercambio es la lección, y el
   registro debe decir qué lado pesa.

La comparación no es «inmutable es mejor que mutable». Es más estrecha: un contenedor de
instantáneas convierte deshacer y rehacer en una copia de valor en lugar de una repetición de las
reglas, la transcripción es idéntica antes y después, y el motor paga el historial en bytes.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar pureza e inmutabilidad;
- implementar deshacer y rehacer mediante el historial del tablero.

## El concepto: un valor que nunca se cambia

### La pureza es un contrato, no una palabra clave

`generate_move_candidates` es una función pura de una pieza:

```cpp
template <typename PieceType>
std::array<MoveCandidate, moveCandidateCount> generate_move_candidates(const PieceType &piece)
{
    std::array<MoveCandidate, moveCandidateCount> candidates = {};
    std::ranges::transform(
        std::views::iota(0, moveCandidateCount), candidates.begin(),
        [&piece](int index)
        {
            return MoveCandidate{
                index + 1,
                piece.row() + knightRowChangePerMove[index],
                piece.column() + knightColumnChangePerMove[index],
            };
        });
    return candidates;
}
```

Cuatro propiedades la hacen pura, y ninguna es una palabra clave del lenguaje:

1. **La entrada es de solo lectura.** `const PieceType &` no se puede escribir a través de ella.
2. **La salida se devuelve.** La función no escribe en un parámetro ni en un miembro.
3. **No lee estado oculto.** Ni tablero, ni secuencia aleatoria, ni contador global.
4. **Misma entrada, misma salida.** Dos llamadas con la misma pieza devuelven los mismos ocho
   registros.

Esta función es idéntica byte a byte a la de `construction-17`. El diff de la lección no la toca.
Ese es el primer punto honesto: **la pureza ya estaba en las hojas, y esta lección no la añade
ahí.** Lo que la lección añade es un lugar donde un objeto entero se trata como un valor.

### La inmutabilidad es una propiedad del almacenamiento

`Board` es un tipo de valor:

```cpp
using CellRow = std::array<bool, maximumColumnCount>;

std::array<CellRow, maximumRowCount> visited;
```

`sizeof(Board<8, 8>)` es 256 bytes. No hay puntero, ni `new`, ni indirección. Una copia de un
tablero es una copia de 256 bytes, y la copia no comparte nada con el original.

Esa última frase es todo el mecanismo. Considera la alternativa:

```cpp
std::vector<Board<RowCount, ColumnCount> *> history; // no es esta línea base
```

Con punteros, `history.push_back(board.get())` guarda la dirección del tablero *actual*. El
siguiente `mark_visited` cambiaría también la instantánea, porque la instantánea y el tablero
serían el mismo objeto. El historial no registraría nada. Aquí la inmutabilidad no es una
disciplina que quien llama promete respetar; es una consecuencia de guardar valores.

### Deshacer es una consulta, no una repetición

Hay dos maneras de implementar deshacer.

**Repetición.** Para deshacer el movimiento 21, se descarta el estado y se vuelven a ejecutar los
movimientos 1 a 20 desde la semilla. Eso es correcto solo si la repetición reproduce cada entrada:
el estado de la secuencia aleatoria, las posiciones iniciales, el orden de las tiradas, el orden de
los movimientos. Cuesta O(k) trabajo deshacer el movimiento k, y cuesta un contrato de repetición
determinista que este programa no tiene, porque `random_sequence` guarda estado oculto que avanza.

**Instantánea.** Se guarda el tablero después de cada movimiento aceptado. Para deshacer, se mueve
el índice y se copia:

```cpp
bool undo_last_move()
{
    if (historyIndex == 0)
    {
        return false;
    }
    historyIndex -= 1;
    *board = history[historyIndex];
    return true;
}
```

Son 2 líneas de cambio de estado y una asignación de 256 bytes, sin importar qué movimiento
deshagas. No conoce las reglas. No puede equivocarse en las reglas.

| Propiedad | Repetición | Instantánea |
|---|---|---|
| Trabajo para deshacer el movimiento k | O(k) movimientos repetidos | un paso de índice y 256 bytes |
| Necesita las reglas | sí, todas | no |
| Necesita rebobinar la secuencia aleatoria | sí | no |
| Necesita un contrato de repetición determinista | sí | no |
| Memoria | un tablero | un tablero por movimiento |
| Puede fallar cuando cambia una regla | sí | no |

La tabla es la lección. La instantánea gana en todas las filas menos en memoria, y la memoria es la
fila que esta lección mide.

### La rama de rehacer

`record_history` son cinco líneas, y las dos primeras son las que importan:

```cpp
void record_history()
{
    history.erase(history.begin() + static_cast<std::ptrdiff_t>(historyIndex + 1),
                  history.end());
    history.push_back(*board);
    historyIndex = history.size() - 1;
}
```

`erase` borra todo lo que hay después del índice actual antes de añadir la instantánea nueva. Esa es
la regla «un movimiento nuevo después de deshacer borra la rama de rehacer». Después de deshacer,
las instantáneas que están delante del índice describen un futuro que ya no se sigue del presente.
Conservarlas permitiría que rehacer volviera a un tablero que nunca vino después del actual.

### Lo que la frontera no cubre

`history` guarda `Board<RowCount, ColumnCount>`. No guarda las posiciones de los caballos, la
bandera `playerIsActive` ni el estado de la secuencia aleatoria.

Así que `undo_last_move()` restaura las casillas visitadas y nada más. Es un deshacer honesto a
nivel de tablero, no a nivel de partida. Un deshacer completo necesita un registro `GameState` que
contenga el tablero, los dos caballos, la bandera de turno y la posición del generador; entonces el
historial pasa a ser un historial de ese registro.

Este es el límite real de la lección, y el paso 4.4 lo mide en lugar de esconderlo.

## Antes de empezar

- [`construction-17`](../construction-17-stl/lesson-es.md) — la línea base inmediata y el lado
  izquierdo del diff del paso 6. Su directorio `code/` es el artefacto del que parte esta lección.
- [`construction-16`](../construction-16-ownership/lesson-es.md) — contexto opcional. Los tres
  miembros `unique_ptr` sobreviven aquí, y son la razón por la que el motor posee sus partes en
  lugar de copiarlas.
- [`construction-05`](../construction-05-array-of-records/lesson-es.md) — colección y recorrido. El
  tablero es una colección de celdas, y una instantánea es una copia de esa colección.
- [`construction-09`](../construction-09-abstract-data-type/lesson-es.md) — contexto opcional. La
  frontera entre lo que el motor expone y lo que oculta es lo que hace privado el historial.
- Un compilador de C++20 (`g++` o `clang++`). `cmake` es opcional; `run.sh` recurre a una
  compilación directa cuando CMake no está.

No necesitas saber cómo crece `std::vector`. Necesitas leer una asignación, decir qué copia y
mostrar que la transcripción no cambió.

### Qué necesitas

- La línea base funcional:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/game_engine.h`](code/game_engine.h),
  [`code/board.h`](code/board.h), [`code/move_generator.h`](code/move_generator.h),
  [`code/knight.h`](code/knight.h), [`code/knight.cpp`](code/knight.cpp),
  [`code/position.h`](code/position.h), [`code/position.cpp`](code/position.cpp),
  [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- La línea base anterior, para el diff:
  [`code/game_engine.h`](../construction-17-stl/code/game_engine.h) y sus once compañeros en
  [`../construction-17-stl/code/`](../construction-17-stl/code/).
- Guiones y notas de compilación: [`run.sh`](run.sh), [`CMakeLists.txt`](CMakeLists.txt) y
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
test -f ../construction-17-stl/code/game_engine.h &&
test -f ../../../common/traces/seed-0001.txt
```

Si falta un archivo, la comparación no tiene segundo lado. Anota el archivo que falta en
`EVIDENCE-es.md` §11 y vuelve cuando esté disponible. Un recuento contra un archivo que no has leído
no es evidencia.

La línea base de 8×8 reproduce las 20 trazas de `common/traces/` byte a byte, antes de que cambies
nada. Ese es el contrato de comportamiento, y esta lección no lo toca.

## Paso 1 — Inventariar el estado (~10 min)

Empieza por lo que cambió y lo que no:

```bash
for file in board.h move_generator.h game_engine.h; do
  diff -u "../construction-17-stl/code/${file}" "code/${file}"
done
```

Solo `game_engine.h` produce salida. Ahora los recuentos:

```bash
grep -rn 'for (' code/ | wc -l
grep -rn 'ranges::\|views::' code/ | wc -l
grep -n 'history' code/game_engine.h
```

```text
5
3
```

El recuento de `for` y el de algoritmos son los mismos que en `construction-17`. Esta lección no
elimina ningún bucle ni añade ningún algoritmo. La búsqueda de `history` es la superficie nueva.

`game_engine.h`, rangos relevantes para el historial:

| Líneas | Contenido |
|---|---|
| 8–16 | Includes. `<cstddef>` y `<vector>` son nuevos. |
| 54–57 | `history_size()`. Pública, `const`. |
| 59–68 | `undo_last_move()`. Pública. Se niega en el índice 0. |
| 70–79 | `redo_last_move()`. Pública. Se niega al final. |
| 81–101 | `run`. Limpia el historial, añade el tablero inicial, fija el índice 0. |
| 104–108 | Miembros: `board`, dos caballos, `history`, `historyIndex`. |
| 231–237 | `record_history()`. Borra la rama de rehacer, añade, mueve el índice. |
| 262–330 | `playTurn`. Llama a `record_history` en la línea 308 y en la 320. |

`board.h`, rangos relevantes para el valor:

| Líneas | Contenido |
|---|---|
| 65–74 | `maximumRowCount`, `maximumColumnCount`, `CellRow` y el almacenamiento `std::array`. |
| 42–45 | `reset()`. `std::ranges::fill` sobre el almacenamiento. |
| 47–63 | `was_visited` y `mark_visited`. El único mutador es `mark_visited`. |

Recuentos de esta línea base. La regla de conteo se enuncia para que puedas reproducirla:

- Encabezados: 7. Unidades de traducción: 5. Definiciones de clase completas: 5.
- Cuerpos de función con nombre escritos a mano: 45, frente a 41. Los cuatro nuevos son
  `history_size`, `undo_last_move`, `redo_last_move` y `record_history`.
- Declaraciones públicas: 34, frente a 31. Las tres nuevas son `history_size`, `undo_last_move` y
  `redo_last_move`.
- Lambdas: 3. Encabezados de `for`: 5. Puntos de llamada a algoritmos: 3. Todo sin cambios.
- Líneas de código: 655, que es 611 más 44.
- Archivos que difieren de `construction-17`: 1 de 12.

Después responde en `EVIDENCE-es.md` §2:

1. ¿Qué archivo cambió, y en cuántas líneas?
2. ¿Qué dos archivos no cambiaron aunque esperabas que cambiaran, y por qué no lo hicieron?
3. ¿Cuántos cuerpos de función con nombre hay, y cuáles cuatro son nuevos?
4. ¿Cuántas declaraciones públicas hay, y cuáles tres son nuevas?
5. ¿Dónde se llama a `record_history`, y en qué líneas?
6. ¿Qué tipo guarda `history`, y cuánto mide un elemento?
7. ¿Qué le hace `undo_last_move` a `historyIndex`?
8. ¿Qué borra `record_history` antes de añadir?
9. ¿Qué partes del estado de la partida no están en el historial?
10. ¿Qué función de esta línea base es pura, y qué propiedades lo demuestran?

La pregunta 2 tiene una respuesta precisa: `move_generator.h` y `board.h` no cambiaron, porque la
pureza en la generación de movimientos y la semántica de valor del tablero ya eran ciertas en
`construction-17`. Lo nuevo no es el tipo de valor; es el contenedor de valores.

## Paso 2 — Predecir antes de ejecutar (~10 min)

Escribe estas predicciones antes de ejecutar nada. No mires las respuestas del paso 3 primero.

### 2.1 La tabla de estructura

| Pregunta | Mi predicción |
|---|---|
| Archivos que difieren de `construction-17` | |
| Líneas de código en `construction-17` | |
| Líneas de código aquí | |
| Líneas de `game_engine.h`, antes y después | |
| Líneas añadidas y eliminadas en el diff | |
| Cuerpos de función con nombre, antes y después | |
| Declaraciones públicas, antes y después | |
| Encabezados de `for`, antes y después | |
| Puntos de llamada a algoritmos, antes y después | |
| Lambdas, antes y después | |

La fila fácil de equivocar es la última terna. Esta lección no cambia ninguna de las tres, y quien
espere que el «estilo funcional» elimine bucles predecirá una caída.

### 2.2 La tabla de tamaños y memoria

| Pregunta | Mi predicción |
|---|---|
| `sizeof(Board<8, 8>)` | |
| `sizeof(GameEngine<8, 8>)`, antes y después | |
| `sizeof(std::vector<Board<8, 8>>)` | |
| Asignaciones de montón por partida, antes y después | |
| Bytes asignados por partida, semilla 1, antes y después | |
| `history_size()` después de la semilla 1 | |
| `history_size()` de la semilla más larga de las 20 | |
| Bytes de texto de `game.o`, antes y después | |
| Bytes de texto del binario, antes y después | |
| Bytes de datos del binario, antes y después | |
| Semillas que coinciden, de 20 | |

Predice `history_size()` como `moves + 1`. La semilla 1 informa 21 movimientos, así que predice 22
instantáneas: el tablero inicial más una por movimiento aceptado.

Predice el número de asignaciones como pequeño y no constante. Un `std::vector` se reasigna al
crecer, así que el número depende de cuántos movimientos dure la partida.

### 2.3 Las predicciones de la prueba de cambio

La prueba de cambio es: observar que una instantánea del tablero realmente se restaura, desde fuera
del motor, y mostrar que la transcripción no cambia.

| Pregunta | Mi predicción |
|---|---|
| Qué añadiré a la copia temporal | |
| Qué archivo editaré | |
| `sizeof(GameEngine<8, 8>)` después del cambio | |
| ¿El tablero cambia al deshacer? | |
| ¿El tablero es idéntico tras deshacer y rehacer? | |
| Semillas que coinciden, de 20, después del cambio | |
| Deshacer exitosos desde el final de la semilla 1 | |
| Rehacer exitosos desde el principio | |

Predice el tamaño como sin cambios. Una función miembro que devuelve una referencia no añade ningún
miembro de datos.

## Paso 3 — Ejecutar la línea base (~15 min)

Primero las trazas, sin modificar:

```bash
mkdir -p /tmp/functional-test
./run.sh --seed 1 --name Learner > /tmp/functional-test/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/functional-test/baseline-0001.txt
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
  -o build/functional-baseline code/main.cpp code/game.cpp code/knight.cpp \
  code/position.cpp code/random_sequence.cpp
```

La transcripción termina con:

```text
winner=machine
moves=21
```

Ahora la sonda. Va en el directorio temporal, nunca en los archivos versionados:

```bash
cat > /tmp/functional-test/probe.cpp <<'EOF'
#include "game_engine.h"

#include <cstdio>
#include <vector>

int main()
{
    std::printf("sizeof(Board<8,8>)=%zu\n", sizeof(Board<8, 8>));
    std::printf("sizeof(GameEngine<8,8>)=%zu\n", sizeof(GameEngine<8, 8>));
    std::printf("sizeof(Knight)=%zu\n", sizeof(Knight));
    std::printf("sizeof(Position)=%zu\n", sizeof(Position));
    std::printf("sizeof(MoveCandidate)=%zu\n", sizeof(MoveCandidate));
    std::printf("sizeof(std::vector<Board<8,8>>)=%zu\n",
                sizeof(std::vector<Board<8, 8>>));

    GameEngine<8, 8> engine;
    engine.run(1, "Learner");
    std::printf("history=%zu\n", engine.history_size());
    std::printf("undo=%d\n", engine.undo_last_move());
    std::printf("redo=%d\n", engine.redo_last_move());

    int undoCount = 0;
    while (engine.undo_last_move())
    {
        undoCount += 1;
    }
    int redoCount = 0;
    while (engine.redo_last_move())
    {
        redoCount += 1;
    }
    std::printf("undo_successes=%d\n", undoCount);
    std::printf("redo_successes=%d\n", redoCount);
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/functional-test/probe /tmp/functional-test/probe.cpp \
  code/game.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
/tmp/functional-test/probe | tail -9
```

La salida en esta plataforma:

```text
sizeof(Board<8,8>)=256
sizeof(GameEngine<8,8>)=56
sizeof(Knight)=40
sizeof(Position)=8
sizeof(MoveCandidate)=12
sizeof(std::vector<Board<8,8>>)=24
history=22
undo=1
redo=1
undo_successes=21
redo_successes=21
```

Lee con cuidado las últimas cuatro líneas. El motor termina en el índice 21 de 22 instantáneas. Un
deshacer devuelve verdadero, un rehacer devuelve verdadero, y el tablero vuelve a donde estaba.
Después la frontera es exacta: 21 deshaceres tienen éxito y el 22 devuelve falso, porque el índice 0
es el tablero inicial y no hay nada antes. Desde el índice 0, 21 rehaceres tienen éxito y el 22
devuelve falso, porque el índice 21 es la última instantánea.

Ahora el coste de asignación, con un contador en `operator new`:

```bash
cat > /tmp/functional-test/alloc.cpp <<'EOF'
#include "game_engine.h"

#include <cstdio>
#include <cstdlib>
#include <new>

static std::size_t allocationCount = 0;
static std::size_t allocatedBytes = 0;

void *operator new(std::size_t size)
{
    allocationCount += 1;
    allocatedBytes += size;
    return std::malloc(size == 0 ? 1 : size);
}

void operator delete(void *memory) noexcept
{
    std::free(memory);
}

int main()
{
    for (int seed = 1; seed <= 20; seed += 1)
    {
        allocationCount = 0;
        allocatedBytes = 0;
        GameEngine<8, 8> engine;
        std::fflush(stdout);
        engine.run(seed, "Learner");
        std::fprintf(stderr, "seed=%d history=%zu allocations=%zu bytes=%zu\n",
                     seed, engine.history_size(), allocationCount, allocatedBytes);
        std::fflush(stdout);
    }
    return 0;
}
EOF
g++ -std=c++20 -O2 -Wno-mismatched-new-delete -Icode \
  -o /tmp/functional-test/alloc /tmp/functional-test/alloc.cpp \
  code/game.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
/tmp/functional-test/alloc > /dev/null
```

```text
seed=1 history=22 allocations=9 bytes=16464
seed=2 history=29 allocations=9 bytes=16464
seed=3 history=29 allocations=9 bytes=16464
seed=4 history=41 allocations=10 bytes=32848
seed=5 history=50 allocations=10 bytes=32848
seed=6 history=21 allocations=9 bytes=16464
seed=7 history=11 allocations=8 bytes=8272
seed=8 history=27 allocations=9 bytes=16464
seed=9 history=44 allocations=10 bytes=32848
seed=10 history=23 allocations=9 bytes=16464
seed=11 history=9 allocations=8 bytes=8272
seed=12 history=37 allocations=10 bytes=32848
seed=13 history=15 allocations=8 bytes=8272
seed=14 history=18 allocations=9 bytes=16464
seed=15 history=29 allocations=9 bytes=16464
seed=16 history=21 allocations=9 bytes=16464
seed=17 history=10 allocations=8 bytes=8272
seed=18 history=13 allocations=8 bytes=8272
seed=19 history=35 allocations=10 bytes=32848
seed=20 history=48 allocations=10 bytes=32848
```

La misma sonda contra `construction-17` informa `allocations=3 bytes=336` en todas las semillas. Las
tres asignaciones son los destinos de los `unique_ptr`: un tablero y dos caballos.

Los bytes siguen una regla que puedes comprobar a mano. Un vector que termina con capacidad 16
asignó 1 + 2 + 4 + 8 + 16 = 31 bloques de 256 bytes, que son 7.936, más los 336 bytes de los tres
destinos de `unique_ptr`: 8.272. La capacidad 32 da 63 bloques: 16.464. La capacidad 64 da 127
bloques: 32.848. El número de asignaciones es el número de duplicaciones más los tres destinos de
`unique_ptr`.

La memoria viva al final de una partida es menor que los bytes asignados: una partida que llega a 50
instantáneas usa 50 × 256 = 12.800 bytes de un búfer de 16.384. El resto son los búferes abandonados
por el camino.

El peor caso está acotado, y el límite está en el código: `maximumMoveCount` es 200, así que una
partida tiene como máximo 200 movimientos aceptados y como máximo 201 instantáneas. El vector
llegaría a capacidad 256, que son 65.536 bytes, más los 336 bytes de los tres destinos de
`unique_ptr`.

Ahora el código objeto y el coste de compilación:

```bash
g++ -std=c++20 -O2 -Icode -c code/game.cpp -o /tmp/functional-test/game18.o
g++ -std=c++20 -O2 -I../construction-17-stl/code \
  -c ../construction-17-stl/code/game.cpp -o /tmp/functional-test/game17.o
size /tmp/functional-test/game17.o /tmp/functional-test/game18.o
g++ -std=c++20 -O2 -Icode -o /tmp/functional-test/bin18 \
  code/main.cpp code/game.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
g++ -std=c++20 -O2 -I../construction-17-stl/code -o /tmp/functional-test/bin17 \
  ../construction-17-stl/code/main.cpp ../construction-17-stl/code/game.cpp \
  ../construction-17-stl/code/knight.cpp ../construction-17-stl/code/position.cpp \
  ../construction-17-stl/code/random_sequence.cpp
size /tmp/functional-test/bin17 /tmp/functional-test/bin18
g++ -std=c++20 -Icode -E code/game.cpp | wc -l
g++ -std=c++20 -I../construction-17-stl/code -E ../construction-17-stl/code/game.cpp | wc -l
```

```text
   text	   data	    bss	    dec	    hex	filename
   4656	      0	      0	   4656	   1230	/tmp/functional-test/game17.o
   7240	      0	      0	   7240	   1c48	/tmp/functional-test/game18.o
   text	   data	    bss	    dec	    hex	filename
   9668	    692	     24	  10384	   2890	/tmp/functional-test/bin17
  12368	    700	     24	  13092	   3324	/tmp/functional-test/bin18
  76303
  72259
```

El motor creció 2.584 bytes de texto en `game.o` y 2.700 en el binario. Eso es la maquinaria del
vector y las dos copias de la asignación del tablero, alineadas. Los datos crecen 8 bytes.

Tiempo de compilación, tres ejecuciones de cada uno:

```bash
for run in 1 2 3; do
  /usr/bin/time -f '%e' g++ -std=c++20 -O2 -Icode -c code/game.cpp \
    -o /tmp/functional-test/t18.o
done
for run in 1 2 3; do
  /usr/bin/time -f '%e' g++ -std=c++20 -O2 -I../construction-17-stl/code \
    -c ../construction-17-stl/code/game.cpp -o /tmp/functional-test/t17.o
done
```

```text
0.72
0.72
0.72
0.60
0.63
0.61
```

Unos 0,11 s más, y 4.044 líneas preprocesadas más para `game.cpp`. Tus números variarán; la
dirección no debería.

Si el primer diff no está vacío, detente. Esa es una línea base distinta de la que describe esta
lección; anótala en `EVIDENCE-es.md` §11 y no ajustes la predicción para que coincida.

## Paso 4 — Comparar: qué demuestra el historial (~15 min)

Abre `game_engine.h` y `board.h`. Cinco hechos, leídos contra las mediciones.

1. **La instantánea es un valor, no una dirección.** `history` guarda `Board<RowCount, ColumnCount>`
   por valor. `sizeof(Board<8, 8>)` es 256, y un `push_back` copia 256 bytes. Nada se comparte entre
   una instantánea y el tablero vivo.
2. **Deshacer no conoce las reglas.** `undo_last_move` mueve `historyIndex` y asigna
   `*board = history[historyIndex]`. No hay llamada a `tryMove`, ni orden de movimientos, ni dados.
   La función no puede equivocarse en una regla que nunca lee.
3. **La rama de rehacer se borra, no se marca.** `record_history` llama a `erase` sobre la cola
   antes de `push_back`. No hay una marca de «inválido» que se pueda olvidar de comprobar.
4. **El motor creció, el tablero no.** `sizeof(GameEngine<8, 8>)` pasa de 24 a 56, porque un
   `std::vector` son 24 bytes y `historyIndex` son 8. `sizeof(Board<8, 8>)` sigue en 256.
5. **El coste de memoria es por movimiento y está acotado.** 256 bytes por instantánea, como máximo
   201 instantáneas, como máximo 65.536 bytes de búfer para este tamaño de tablero.

### 4.1 El diff exacto

Cambió un archivo. `diff -u` contra `construction-17` da 44 líneas añadidas y ninguna eliminada:

```bash
diff -u ../construction-17-stl/code/game_engine.h code/game_engine.h
```

| Región | Añadidas | Eliminadas | Qué cambió |
|---|---|---|---|
| Includes | 2 | 0 | `<cstddef>`, `<vector>` |
| API pública | 27 | 0 | `history_size`, `undo_last_move`, `redo_last_move` |
| `run` | 3 | 0 | Limpiar, añadir el tablero inicial, fijar el índice 0 |
| Miembros | 2 | 0 | `history`, `historyIndex` |
| `record_history` | 8 | 0 | Borrar la cola, añadir, mover el índice |
| `playTurn` | 2 | 0 | Dos llamadas a `record_history()` |
| Total | 44 | 0 | Un archivo, once sin cambios |

No se eliminó ni una línea de lógica. El cambio es puramente aditivo.

### 4.2 Lo que no cambió

- La transcripción. Las 20 trazas coinciden, byte a byte.
- Las reglas. Orden de movimientos, captura, trampa, empate y el límite de 200 movimientos siguen
  igual.
- El bucle de turno. `playTurn` sigue imprimiendo cada intento, mutando cuatro piezas de estado,
  manejando cuatro resultados y deteniéndose de dos maneras.
- `generate_move_candidates`. Idéntica byte a byte, todavía pura.
- `board.h`. Idéntico byte a byte, todavía un valor de 256 bytes.
- El recuento de recorridos. 5 encabezados de `for`, 3 puntos de llamada a algoritmos, 3 lambdas.
- Los tres miembros `unique_ptr`. El motor sigue poseyendo su tablero y sus dos caballos.
- El recuento de asignaciones de las tres partes. 3 asignaciones, 336 bytes, más el historial.

### 4.3 Lo que cuesta

| Métrica | construction-17 | construction-18 | Diferencia |
|---|---|---|---|
| `sizeof(GameEngine<8, 8>)` | 24 | 56 | +32 |
| Asignaciones, semilla 1 | 3 | 9 | +6 |
| Bytes asignados, semilla 1 | 336 | 16.464 | +16.128 |
| Bytes asignados, semilla más larga | 336 | 32.848 | +32.512 |
| Instantáneas vivas, semilla 1 | 0 | 22 | +22 |
| Memoria viva del tablero, semilla 1 | 256 | 5.632 | +5.376 |
| Texto de `game.o` | 4.656 | 7.240 | +2.584 |
| Texto del binario | 9.668 | 12.368 | +2.700 |
| Líneas preprocesadas | 72.259 | 76.303 | +4.044 |
| Tiempo de compilación | ~0,61 s | ~0,72 s | +0,11 s |

La fila de memoria viva del tablero es la honesta: 22 instantáneas × 256 bytes = 5.632 bytes
retenidos al final de la semilla 1, frente a 256 bytes antes. La fila de bytes asignados es mayor
porque un vector que crece abandona sus búferes anteriores.

Nada de esto es un defecto. Es el precio de conservar cada estado en lugar de recalcularlo, y el
registro debe anotarlo junto al beneficio.

### 4.4 La frontera, con precisión

El historial guarda el tablero. No guarda:

- la fila y la columna de `playerKnight`,
- la fila y la columna de `machineKnight`,
- la bandera `playerIsActive`, que es una local en `run` y se pasa por referencia por el bucle,
- el estado de la secuencia aleatoria, que vive en `random_sequence.cpp` y avanza en cada llamada.

Por lo tanto, `undo_last_move()` después de una partida restaura las casillas visitadas del turno
anterior y deja todo lo demás en su valor final. La sonda del paso 3 muestra exactamente eso:
`undo=1`, y la transcripción no participa, porque deshacer no imprime nada.

Esto no es un error de la línea base; es el tamaño de la frontera que la línea base eligió. Un
deshacer a nivel de partida necesita un registro de estado:

```cpp
struct GameState
{
    Board<8, 8> board;
    Knight player;
    Knight machine;
    bool playerIsActive;
    std::uint64_t generatorState;
};
```

Entonces `history` pasa a ser `std::vector<GameState>`, y `undo_last_move` asigna el registro
completo. El coste por instantánea sube de 256 bytes a unos 256 + 40 + 40 + 1 + 8, que son 345 bytes
antes del relleno. Ese es el cambio siguiente, y no es esta lección.

### 4.5 Deshacer por consulta frente a deshacer por repetición, con números

| Pregunta | Repetición | Instantánea |
|---|---|---|
| Trabajo para deshacer el último movimiento de la semilla 1 | 21 turnos repetidos | un paso de índice y una copia de 256 bytes |
| Trabajo para deshacer el primer movimiento de la semilla 1 | 1 turno repetido | 20 pasos de índice y una copia de 256 bytes |
| Código que debe ser correcto | cada regla y el generador | una asignación |
| Memoria retenida | 256 bytes | 5.632 bytes en la semilla 1 |
| Memoria retenida, peor caso | 256 bytes | 65.536 bytes de búfer |
| Comportamiento visible en la transcripción | ninguno | ninguno |

Ambas columnas son invisibles para la transcripción. Por eso el criterio de aceptación necesita una
sonda además de las trazas.

## Paso 5 — Prueba de cambio: observar la instantánea desde fuera (~15 min)

El criterio de aceptación nombra la prueba de cambio: **deshacer y rehacer un movimiento mediante el
historial del tablero y mostrar que las 20 trazas siguen pasando.**

La sonda del paso 3 llama a `undo_last_move` y `redo_last_move`, pero no puede ver el tablero:
`board` es privado y no hay accesor. Para observar que la instantánea se restaura de verdad, añade
un accesor de solo lectura en una copia temporal.

### 5.1 Hacer una copia temporal

Nunca edites el directorio versionado `code/`. Cópialo:

```bash
rm -rf /tmp/functional-test/change
cp -r code /tmp/functional-test/change
```

### 5.2 Añadir el accesor

En `/tmp/functional-test/change/game_engine.h`, después de `history_size()` (línea 57), añade:

```cpp
    const Board<RowCount, ColumnCount> &current_board() const
    {
        return *board;
    }
```

Es un accesor `const` que devuelve una referencia. Expone la lectura y prohíbe la escritura. No
añade ningún miembro de datos y no cambia la lógica.

### 5.3 Tomar la huella del tablero

```bash
cat > /tmp/functional-test/fingerprint.cpp <<'EOF'
#include "game_engine.h"

#include <cstdio>
#include <string>

static std::string fingerprint(const Board<8, 8> &board)
{
    std::string bits;
    for (int row = 1; row <= 8; row += 1)
    {
        for (int column = 1; column <= 8; column += 1)
        {
            bits += board.was_visited(row, column) ? '1' : '0';
        }
    }
    return bits;
}

int main()
{
    GameEngine<8, 8> engine;
    engine.run(1, "Learner");
    std::string afterGame = fingerprint(engine.current_board());
    std::printf("history=%zu\n", engine.history_size());
    std::printf("undo=%d\n", engine.undo_last_move());
    std::string afterUndo = fingerprint(engine.current_board());
    std::printf("redo=%d\n", engine.redo_last_move());
    std::string afterRedo = fingerprint(engine.current_board());
    std::printf("state_restored=%d\n", afterGame == afterRedo);
    std::printf("undo_changed_board=%d\n", afterGame != afterUndo);
    std::printf("sizeof(GameEngine<8,8>)=%zu\n", sizeof(GameEngine<8, 8>));
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I/tmp/functional-test/change \
  -o /tmp/functional-test/fingerprint /tmp/functional-test/fingerprint.cpp \
  /tmp/functional-test/change/game.cpp /tmp/functional-test/change/knight.cpp \
  /tmp/functional-test/change/position.cpp /tmp/functional-test/change/random_sequence.cpp
/tmp/functional-test/fingerprint | tail -6
```

```text
history=22
undo=1
redo=1
state_restored=1
undo_changed_board=1
sizeof(GameEngine<8,8>)=56
```

Tres hechos, cada uno medido:

- `undo_changed_board=1`: la huella de 64 bits de las casillas visitadas difiere después de
  deshacer, así que el deshacer restauró un tablero anterior y no el actual.
- `state_restored=1`: después de deshacer y rehacer, la huella es igual a la huella del final de la
  partida. El valor va y vuelve.
- `sizeof(GameEngine<8,8>)=56`: el accesor no añadió datos.

### 5.4 Mostrar que la transcripción no cambia

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I/tmp/functional-test/change \
  -o /tmp/functional-test/change-bin \
  /tmp/functional-test/change/main.cpp /tmp/functional-test/change/game.cpp \
  /tmp/functional-test/change/knight.cpp /tmp/functional-test/change/position.cpp \
  /tmp/functional-test/change/random_sequence.cpp
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/functional-test/change-bin --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

El bucle está silencioso. Las 20 trazas coinciden con el accesor presente.

### 5.5 Qué demuestra la prueba de cambio, y qué no

Demuestra que una instantánea del tablero se restaura mediante una copia de valor, y que añadir una
ventana de solo lectura al motor no cambia ni el tamaño ni la salida.

No demuestra que deshacer sea completo. La huella cubre solo las casillas visitadas, porque el
historial cubre solo las casillas visitadas. Para tomar la huella de los caballos y de la bandera de
turno necesitarías tenerlos en la instantánea, que es el cambio a `GameState` del paso 4.4.

Tampoco demuestra que la rama de rehacer se borre. Ninguna llamada pública de esta línea base hace
un movimiento después de deshacer: el bucle del juego nunca deshace, y `run` limpia el historial. El
borrado es un hecho de lectura de código, visible en `record_history`, y el modelo con enteros del
paso 3 muestra qué hacen las dos líneas sobre una secuencia. Si quieres ejecutarlo, haz público
`record_history` en la copia temporal, llámalo después de deshacer y comprueba que `history_size()`
dejó de crecer. Anota ese intento en `EVIDENCE-es.md` §6.5 si lo haces.

## Paso 6 — Comparar con `construction-17` (~10 min)

| Métrica | construction-17 | construction-18 | Diferencia |
|---|---|---|---|
| Archivos | 12 | 12 | 0 |
| Encabezados | 7 | 7 | 0 |
| Unidades de traducción | 5 | 5 | 0 |
| Líneas de código | 611 | 655 | +44 |
| `game_engine.h` | 302 | 346 | +44 |
| Otros 11 archivos | — | idénticos byte a byte | 0 |
| Clases completas | 5 | 5 | 0 |
| Cuerpos de función con nombre | 41 | 45 | +4 |
| Declaraciones públicas | 31 | 34 | +3 |
| Lambdas | 3 | 3 | 0 |
| Encabezados de `for` | 5 | 5 | 0 |
| Puntos de llamada a algoritmos | 3 | 3 | 0 |
| `sizeof(Board<8, 8>)` | 256 | 256 | 0 |
| `sizeof(GameEngine<8, 8>)` | 24 | 56 | +32 |
| Asignaciones de montón por partida | 3 | 8 a 10 | +5 a +7 |
| Bytes asignados por partida | 336 | 8.272 a 32.848 | +7.936 a +32.512 |
| Texto de `game.o` | 4.656 | 7.240 | +2.584 |
| Texto / datos del binario | 9.668 / 692 | 12.368 / 700 | +2.700 / +8 |
| Líneas preprocesadas, `game.cpp` | 72.259 | 76.303 | +4.044 |
| Tiempo de compilación, `game.cpp` | ~0,61 s | ~0,72 s | +0,11 s |
| Trazas | 20 de 20 | 20 de 20 | 0 |

### 6.1 Qué compra

- Deshacer y rehacer existen. `construction-17` no tenía manera de retroceder; esta línea base sí,
  en dos funciones públicas.
- Deshacer no puede contradecir las reglas, porque nunca las lee.
- El historial es un contenedor de valores corriente. Cópialo, serialízalo o compara dos
  instantáneas byte a byte; nada necesita un clon a medida.
- La instantánea queda protegida de mutaciones posteriores por la semántica de valor, no por
  disciplina.
- La frontera es explícita. `history` es privado, y la única manera de recorrerlo son las tres
  funciones públicas.

### 6.2 Qué cuesta

- 256 bytes por movimiento aceptado, más el holgura del vector. Hasta 65.536 bytes de búfer en el
  límite de 200 movimientos.
- 32 bytes más en cada `GameEngine`, use o no la partida el deshacer.
- 2.584 bytes de texto más en `game.o`, y 2.700 en el binario.
- 4.044 líneas preprocesadas más y unos 0,11 s más de compilación para `game.cpp`.
- Una segunda copia del estado que puede separarse de la viva si un cambio futuro muta el tablero
  sin llamar a `record_history`. Las dos llamadas a `record_history()` en `playTurn` son lo único
  que las mantiene sincronizadas.
- Un deshacer parcial que quien lee puede confundir con uno completo. El paso 4.4 es la corrección.

### 6.3 El hueco: inmutabilidad en una frontera, mutación en todo lo demás

La línea base no es un programa funcional. Es un juego mutable con un historial inmutable:

- `playTurn` muta el tablero, el caballo activo y la bandera de turno en el sitio.
- `mark_visited` es un mutador, y se llama sobre el tablero vivo.
- `tryMove` es `const`, pero escribe dos resultados por parámetros `int &`. Su firma dice «solo
  lectura» mientras su lista de parámetros dice «tres salidas».
- `generate_move_candidates` es la única función pura en firma y en cuerpo.
- `record_history` copia el tablero, así que la copia es un valor; el bucle que la produjo no lo es.

La frase de aceptación precisa es más estrecha que «el programa es funcional»:

> La persona que aprende puede deshacer y rehacer un movimiento mediante el historial del tablero y
> mostrar que las 20 transcripciones son idénticas byte a byte.

Dos funciones, un contenedor, veinte trazas sin cambios. Nada sobre quitar la mutación del bucle de
turno, y nada sobre hacer inmutable la partida entera.

## Paso 7 — Explicar (~8 min)

Escribe `EVIDENCE-es.md` §8. Dos partes.

**Primero, una frase.** Di qué cambió entre las dos líneas base y qué decide el cambio, sin usar las
palabras *funcional*, *puro*, *inmutable*, *instantánea*, *historial*, *mejor*, *peor*, *limpio* ni
*moderno*. Si tu frase necesita una de ellas, has nombrado el mecanismo y no la decisión.

**Segundo, un párrafo.** Responde:

- Qué decide `generate_move_candidates`: la misma pieza produce los mismos ocho candidatos, y no se
  lee nada fuera de la pieza.
- Qué decide `Board`: es 256 bytes de valor, y una copia no comparte nada con su origen.
- Qué decide `history`: cada movimiento aceptado deja un valor detrás, y el índice nombra cuál es el
  actual.
- Qué decide `undo_last_move`: un paso de índice y una asignación, sin conocimiento de las reglas.
- Qué decide `record_history`: el futuro después del índice actual se descarta antes de añadir un
  presente nuevo.
- Qué demuestra el diff: un archivo cambió, once no, y el archivo cambiado no añadió ningún bucle ni
  ningún algoritmo.
- Qué demuestran los tamaños: el motor creció 32 bytes y el tablero no se movió.
- Qué demuestran los números de asignación: 336 bytes pasaron a 8.272 o 32.848, y el crecimiento
  sigue la duplicación del vector.
- Qué demuestran las trazas: la contabilidad nueva es invisible en el comportamiento.
- Qué demuestra la sonda: deshacer y rehacer mueven de verdad el tablero, y las fronteras devuelven
  falso.
- Qué no cubre la frontera: caballos, bandera de turno y estado del generador.
- Qué le toca a la lección siguiente: disposición de datos y localidad, `construction-19`.

Una respuesta sólida mantiene cuatro cosas separadas: **qué es un valor** (el tablero, las
instantáneas), **qué se sigue mutando** (el tablero vivo, el caballo, la bandera de turno), **qué
compra el cambio** (deshacer como consulta) y **qué cuesta** (256 bytes por movimiento). Una
respuesta débil dice «el programa ya es funcional», que es falso en ambas direcciones: la mayor
parte del estado sigue siendo mutable, y la única función pura no cambió en esta lección.

## Paso 8 — Verificación de aceptación y registro (~2 min)

El criterio de aceptación de esta lección es: **deshacer y rehacer funcionan mediante el historial
del tablero, y las 20 trazas siguen pasando.**

Comprueba tu registro contra él:

- [ ] Localicé las cuatro funciones nuevas y las tres declaraciones públicas nuevas por línea.
- [ ] Expliqué por qué `board.h` y `move_generator.h` no cambiaron.
- [ ] Expliqué por qué una instantánea debe ser un valor y no un puntero.
- [ ] Expliqué qué borra `record_history` y por qué.
- [ ] Predije las tablas de estructura, tamaños y memoria antes de medir.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Ejecuté la sonda y leí `history=22`, `undo=1`, `redo=1`.
- [ ] Encontré las fronteras: 21 deshaceres y 21 rehaceres para la semilla 1.
- [ ] Medí el coste de asignación en las 20 semillas.
- [ ] Añadí un accesor de solo lectura en una copia temporal y tomé la huella del tablero.
- [ ] Obtuve `undo_changed_board=1` y `state_restored=1` en la compilación temporal.
- [ ] Las 20 trazas coincidieron con el accesor presente.
- [ ] `sizeof(GameEngine<8, 8>)` fue 56 antes y después del accesor.
- [ ] Nombré las tres partes del estado que el historial no guarda.
- [ ] Expliqué que deshacer es una copia de valor, no una repetición de las reglas.
- [ ] No edité ningún archivo de código versionado.
- [ ] Completé `EVIDENCE-es.md` §1–§10 y la autocomprobación de §12.

El criterio no es «el código es funcional». Es «puedes retroceder por un contenedor de valores y
mostrar que el comportamiento no se movió».

## Autocomprobación (binaria)

- [ ] Leí `game_engine.h` y localicé `history_size`, `undo_last_move`, `redo_last_move`,
      `record_history` y los dos puntos de llamada por línea.
- [ ] Comparé la línea base con `construction-17` y nombré el único archivo que cambió.
- [ ] Predije las tablas de estructura, tamaños y memoria antes de ejecutar.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Ejecuté la sonda y leí el tamaño del historial, el deshacer, el rehacer y las dos fronteras.
- [ ] Medí las asignaciones y los bytes en las 20 semillas.
- [ ] Medí `sizeof(GameEngine<8, 8>)` en 56 y `sizeof(Board<8, 8>)` en 256.
- [ ] Medí `game.o` en 7.240 bytes de texto y el binario en 12.368.
- [ ] Observé un tablero restaurado desde fuera del motor en una copia temporal.
- [ ] Mostré que las 20 trazas siguen coincidiendo después del cambio.
- [ ] Nombré las tres partes del estado que el historial no guarda.
- [ ] Expliqué el intercambio de memoria y su límite de 200 movimientos.
- [ ] Completé `EVIDENCE-es.md`.

Estos son criterios de aceptación. Si una casilla
queda sin marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §10.

## Misconcepciones que esta lección refuta

| Error | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| «El programa ya es funcional». | Solo `generate_move_candidates` es pura, y ya lo era. El bucle de turno sigue mutando. | Nombra qué valor es inmutable y qué estado se sigue mutando. |
| «`const` hace pura una función». | `tryMove` es `const` y escribe dos resultados por parámetros `int &`. | Revisa los parámetros además del calificador. |
| «Una instantánea puede ser un `Board *`». | El siguiente `mark_visited` cambiaría la instantánea, porque es el mismo objeto. | Guarda el valor y comprueba que una copia no comparte nada. |
| «Deshacer repite los movimientos». | `undo_last_move` mueve un índice y asigna un valor. Nunca llama a las reglas. | Lee el cuerpo: dos cambios de estado, ningún bucle. |
| «Deshacer restaura la partida entera». | El historial guarda solo el tablero; los caballos, la bandera y el generador están fuera. | Toma la huella de lo que afirmas restaurar. |
| «La transcripción demuestra que deshacer funciona». | Deshacer no imprime nada, así que ninguna traza puede verlo. | Combina la comprobación de trazas con una sonda. |
| «La rama de rehacer se conserva y se salta». | `record_history` borra la cola antes de añadir. | Lee la llamada a `erase` y sus argumentos. |
| «La memoria del historial no tiene límite». | `maximumMoveCount` limita la partida a 200 movimientos, así que como máximo 201 instantáneas. | Calcula el peor caso a partir de la constante. |
| «Bytes asignados es igual a memoria retenida». | Un vector que crece abandona sus búferes; la suma de asignaciones es mayor que el búfer vivo. | Compara instantáneas vivas × 256 con los bytes asignados. |
| «`history_size()` cuenta movimientos». | Cuenta instantáneas, incluido el tablero inicial. | Comprueba la semilla 1: 21 movimientos, 22 instantáneas. |
| «Deshacer es gratis». | Es gratis en tiempo y cuesta 256 bytes por movimiento. | Enuncia los dos lados. |
| «El tamaño del motor no cambió». | Pasó de 24 a 56 bytes. | Mide `sizeof(GameEngine<8, 8>)`. |
| «Un tipo de valor no puede ser grande». | Se copian 256 bytes por movimiento, y ese es el coste que esta lección mide. | Cuenta las copias: una por movimiento aceptado. |
| «Añadir un accesor cambia la disposición». | Una función miembro no añade datos. | Mide el tamaño después del cambio. |
| «Deshacer en el índice 0 tiene éxito». | Devuelve falso y no cambia nada. | Ejecuta el bucle de frontera y cuenta. |
| «El estado del generador está en la instantánea». | Vive en `random_sequence.cpp` y avanza en cada llamada. | Lee dónde se guarda el estado del generador. |
| «Puro significa sin efectos, así que imprimir está prohibido». | Imprimir es un efecto; la pureza trata del valor que devuelve la función y del estado que toca. | Separa el contrato de valor de la entrada y salida. |
| «Inmutable significa que el tablero nunca se escribe». | El tablero vivo se escribe en cada movimiento aceptado. | Señala la frontera: el historial, no el tablero. |

## Siguiente paso

`construction-19` se ocupa de la disposición de datos y la localidad. El historial es un vector de
tableros de 256 bytes, y el tablero son 256 bytes de `bool` donde un bit por casilla cabría en 8
bytes. Copiar 32 veces más bytes de los necesarios es invisible en esta lección y visible en un
perfil de 10.000 partidas. La pregunta que sigue no es «el valor es inmutable» sino «cómo está
dispuesto el valor en memoria».

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog.md) para la ruta completa.

Si faltaba cualquiera de las dos líneas base, no empieces `construction-18`. El contraste siguiente
necesita ambos lados.
