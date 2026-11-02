# Biblioteca de plantillas estándar y algoritmos (standard template library and algorithms)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** abstracción de iteración · **Tiempo:** unos 68 minutos ·
**Prerrequisitos:** construction-05, construction-16 · **Estado:** borrador
**Lectura:** ~23 minutos
**Rol de la lección:** tronco

## Por qué existe esta lección

`construction-16` terminó con un motor que posee sus recursos de forma explícita y que todavía
escribe cada recorrido a mano:

```cpp
for (int row = 1; row <= RowCount; row += 1)
{
    for (int column = 1; column <= ColumnCount; column += 1)
    {
        visited[row][column] = false;
    }
}
```

Esta lección cambia tres de esos recorridos y deja el resto. El reinicio del tablero pasa a
`std::ranges::fill`. La generación de movimientos y el orden de movimientos pasan a
`std::ranges::transform` sobre `std::views::iota`. El bucle de intentos se queda, y la razón por la
que se queda es el contraejemplo sobre el que está construida la lección.

La frase que conviene retener:

> Un bucle enuncia los pasos; un algoritmo enuncia el recorrido. Reemplaza el bucle cuando el
> contrato es «haz esto con cada elemento», y conserva el bucle cuando el contrato es «imprime,
> decide y muta, un elemento a la vez, hasta que uno sea aceptado».

Tres notas honestas antes de la comparación.

1. El código crece 5 líneas, de 606 en doce archivos a 611 en doce. Nueve de los doce archivos son
   idénticos byte a byte a `construction-16`. `board.h` se reduce 1 línea (de 76 a 75),
   `move_generator.h` crece 4 (de 30 a 34) y `game_engine.h` crece 2 (de 300 a 302). El diff son 26
   líneas añadidas y 21 eliminadas: cinco líneas de `include`, tres puntos de llamada a algoritmos,
   el almacenamiento `std::array` y los cuerpos de bucle que reemplazan.
2. Desaparecen cuatro encabezados de `for`, de nueve a cinco. Los tres recorridos reemplazados se
   convierten en tres llamadas a algoritmos. El código objeto apenas se mueve: `game.o` crece 18
   bytes de texto y el binario enlazado 16. `sizeof(Board<8, 8>)` sigue en 256 y
   `sizeof(GameEngine<8, 8>)` sigue en 24.
3. La STL no hace este programa más rápido en nada que esta lección mida. En esta máquina, la
   entrada preprocesada de `game.cpp` crece 17.827 líneas, y el tiempo de compilación pasa de unos
   0,50 s a unos 0,61 s. Lo que compra el cambio es el contrato en el punto de llamada: el nombre
   del algoritmo dice qué pasa con cada elemento, y el cuerpo del bucle deja de ser el lugar
   donde quien lee tiene que inferirlo. Si ese intercambio vale la pena es decisión de quien
   aprende, y el registro debe decir qué lado pesa.

La comparación no es «los algoritmos son mejores que los bucles». Es más estrecha: tres recorridos
ahora nombran su contrato, un recorrido no puede nombrarlo sin esconder efectos, y la transcripción
es idéntica antes y después. Esta lección mide el código, el objeto, el coste de compilación y la
única diferencia semántica que introduce el reinicio del tablero.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar la abstracción de la iteración en el punto de llamada;
- reemplazar tres recorridos por algoritmos y mantener la transcripción sin cambios.

## El concepto: qué promete un recorrido

### Tres formas, tres nombres

Cada recorrido de este programa tiene una de tres formas:

1. **Poner todos los elementos al mismo valor.** El bucle existe para tocarlos todos, y el resultado
   es la misma colección en un estado conocido.
2. **Producir una salida por cada entrada.** El bucle transforma un índice en un valor, y la cuenta
   de entrada es igual a la de salida.
3. **Detenerse en el primer elemento que cumple una condición.** El bucle existe para evitar visitar
   el resto.

La biblioteca estándar tiene un nombre para cada forma:

| Forma | Forma escrita a mano | Algoritmo | Qué promete el nombre |
|---|---|---|---|
| Poner todos los elementos | `for` anidado sobre filas y columnas | `std::ranges::fill` | cada elemento pasa a ser el valor dado |
| Transformar uno por uno | `for` con un índice | `std::ranges::transform` | misma cuenta de entrada y de salida |
| Primera coincidencia | `for` con `return` o `break` | `std::ranges::any_of`, `find_if` | salida temprana, una respuesta |
| Imprimir, decidir y mutar | `for` con `continue` y una bandera | ninguno encaja | efectos intercalados con la decisión |

La tabla es la lección. Las tres primeras filas tratan de la forma del recorrido. La cuarta trata de
qué pasa cuando el recorrido no es solo un recorrido.

### `views::iota` no es un contenedor

`std::views::iota(0, moveCandidateCount)` produce los enteros del 0 al 7 bajo demanda. Almacena dos
enteros, no ocho elementos. No reserva nada y no posee un rango de valores.

Por eso `std::ranges::transform` puede tomarlo como origen y `candidates.begin()` como destino:

```cpp
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
```

Lee la llamada como tres hechos:

- **Origen:** los enteros de 0 a 7, generados de forma perezosa.
- **Destino:** las ocho posiciones de `candidates`, en orden.
- **Operación:** una lambda que convierte un índice en un `MoveCandidate`.

El cuerpo del bucle hacía las mismas tres cosas. La diferencia es dónde las encuentra quien lee. En
el bucle, el límite, el paso y el acceso al elemento están repartidos entre el encabezado del `for`
y el cuerpo. En la llamada al algoritmo, el rango de origen, el rango de destino y la operación son
tres argumentos, y el nombre `transform` enuncia la relación entre ellos.

### Lo que el algoritmo no elimina

La iteración sigue ahí. Se mudó a un encabezado. La aritmética de índices sigue ahí. Se mudó a la
lambda. La rama que decide cuándo parar sigue ahí, dentro de la biblioteca.

Lo que cambió es quién escribe el bucle y qué tiene que enunciar el punto de llamada. Esa es toda la
abstracción: el algoritmo fija el flujo de control para que el punto de llamada pueda describir el
flujo de datos.

Esto importa para la columna del coste. `std::ranges::transform` es una plantilla de función. El
compilador ve su cuerpo, lo inserta y genera aproximadamente las mismas instrucciones que generaría
un bucle escrito a mano. El tamaño del texto en el paso 4 lo confirma: 18 bytes de diferencia en
`game.o`, no una capa nueva en tiempo de ejecución.

### El bucle de intentos es el contraejemplo

Este es el bucle que se queda:

```cpp
bool moveWasAccepted = false;
for (int attemptIndex = 0;
     attemptIndex < moveCandidateCount && !moveWasAccepted;
     attemptIndex += 1)
{
    int attemptedMoveNumber = moveOrder[attemptIndex];
    const MoveCandidate &candidate = moveCandidates[attemptedMoveNumber - 1];
    int destinationRow = 0;
    int destinationColumn = 0;
    MoveOutcome outcome = tryMove(otherKnight, candidate, destinationRow,
                                  destinationColumn);

    if (outcome == MoveOutcome::outsideBoard)
    {
        printIllegalAttempt(attemptedMoveNumber, destinationRow,
                            destinationColumn, "outside");
        continue;
    }
    if (outcome == MoveOutcome::visited)
    {
        printIllegalAttempt(attemptedMoveNumber, destinationRow,
                            destinationColumn, "visited");
        continue;
    }
    if (outcome == MoveOutcome::capture)
    {
        printCaptureAttempt(attemptedMoveNumber, destinationRow,
                            destinationColumn, otherKnightName);
        successfulMoveCount += 1;
        printMove(successfulMoveCount, activeKnight, destinationRow,
                  destinationColumn, true);
        board->mark_visited(destinationRow, destinationColumn);
        winnerName = activeKnightName;
        moveWasAccepted = true;
        continue;
    }

    printLegalAttempt(attemptedMoveNumber, destinationRow, destinationColumn);
    successfulMoveCount += 1;
    printMove(successfulMoveCount, activeKnight, destinationRow,
              destinationColumn, false);
    board->mark_visited(destinationRow, destinationColumn);
    moveKnight(activeKnight, destinationRow, destinationColumn);
    playerIsActive = !playerIsActive;
    moveWasAccepted = true;
}
```

Cuatro propiedades mantienen este bucle como bucle:

1. **Cada intento produce salida antes de que la decisión esté completa.** Los intentos ilegales
   imprimen una línea y continúan. La transcripción la construye el bucle, no algo posterior.
2. **El intento aceptado muta cuatro piezas de estado.** `successfulMoveCount`, el tablero, el
   caballo activo y `playerIsActive` cambian dentro de la iteración.
3. **Hay cuatro resultados, no dos.** `outsideBoard`, `visited`, `capture` y `legal` toman caminos
   distintos, y dos de ellos terminan el turno.
4. **El bucle tiene dos formas de parar.** Se puede alcanzar el límite, o `moveWasAccepted` puede
   volverse verdadero. La segunda es la salida temprana.

Un algoritmo puede forzarse a hacer esto. `std::ranges::find_if` con un predicado que imprime y
registra el resultado compilaría. Pero entonces el predicado decide, imprime y muta, y su nombre
promete una sola decisión. `std::ranges::for_each` no puede romper el recorrido, así que la salida
temprana necesitaría una bandera o una excepción. `std::ranges::any_of` devuelve un `bool` y pierde
cuál de los resultados ocurrió.

La regla precisa no es «salida temprana significa conservar el bucle». `Board::is_valid` tiene una
salida temprana y un predicado puro, así que `std::ranges::any_of` lo expresaría con honestidad. La
regla es: **cuando el trabajo por elemento tiene efectos que el nombre del algoritmo no menciona, el
bucle es la forma honesta.** El bucle de intentos imprime, muta y ramifica; el algoritmo escondería
las tres cosas detrás de un predicado.

`Board::is_valid` es un resto en esta línea base. Es un recorrido con un predicado puro que podría
pasar a `any_of`, y no se cambió. Tampoco lo llama el juego, así que cambiarlo no sería observable
en la transcripción. El paso 5 le da al aprendiz un recorrido visible en la transcripción.

## Antes de empezar

- [`construction-05`](../construction-05-array-of-records/lesson-es.md) — colección y recorrido. El
  tablero es una colección de celdas, y el recorrido es la unidad que las visita. Esta lección
  cambia la forma de esa unidad.
- [`construction-16`](../construction-16-ownership/lesson-es.md) — la línea base inmediata y el lado
  izquierdo del diff del paso 6. Su directorio `code/` es el artefacto del que parte esta lección,
  aunque el catálogo liste `construction-05` como prerrequisito formal.
- [`construction-14`](../construction-14-generic-functions/lesson-es.md) — contexto opcional. Los
  algoritmos son plantillas de función, pero no necesitas escribir una para usar una.
- Un compilador de C++20 (`g++` o `clang++`). `cmake` es opcional; `run.sh` recurre a una
  compilación directa cuando CMake no está. Los algoritmos de rangos que se usan aquí son de C++20,
  no de C++23.

No necesitas saber cómo está implementado `std::ranges::transform`. Necesitas leer un bucle, decir
qué promete, reemplazarlo por un algoritmo que prometa lo mismo y mostrar que la transcripción no
cambió.

### Qué necesitas

- La línea base con STL:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/game_engine.h`](code/game_engine.h),
  [`code/board.h`](code/board.h), [`code/move_generator.h`](code/move_generator.h),
  [`code/knight.h`](code/knight.h), [`code/knight.cpp`](code/knight.cpp),
  [`code/position.h`](code/position.h), [`code/position.cpp`](code/position.cpp),
  [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- La línea base anterior, para el diff:
  [`code/board.h`](../construction-16-ownership/code/board.h),
  [`code/move_generator.h`](../construction-16-ownership/code/move_generator.h),
  [`code/game_engine.h`](../construction-16-ownership/code/game_engine.h) y sus nueve compañeros en
  [`../construction-16-ownership/code/`](../construction-16-ownership/code/).
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
test -f ../construction-16-ownership/code/board.h &&
test -f ../construction-16-ownership/code/move_generator.h &&
test -f ../construction-16-ownership/code/game_engine.h &&
test -f ../../../common/traces/seed-0001.txt
```

Si falta un archivo, la comparación no tiene segundo lado. Registra el archivo que falta en
`EVIDENCE-es.md` §11 y vuelve cuando esté disponible. Un recuento sobre un archivo que no has leído
no es evidencia.

La línea base de 8×8 reproduce las 20 trazas de `common/traces/` byte a byte, antes de que cambies
nada. Ese es el contrato de comportamiento, y esta lección no lo toca.

## Paso 1 — Inventariar los recorridos (~10 min)

Empieza por la cuenta. Este es el número del que trata la aceptación:

```bash
grep -rn 'for (' code/ | wc -l
```

```text
5
```

El mismo recuento contra `../construction-16-ownership/code/` da 9. La diferencia son cuatro
encabezados de bucle: dos en `Board::reset`, uno en `generate_move_candidates` y uno en
`buildMoveOrder`. Los cinco que quedan:

| Ubicación | Bucle |
|---|---|
| `board.h:27` | bucle exterior de filas en `is_valid` |
| `board.h:29` | bucle interior de columnas en `is_valid` |
| `game_engine.h:139` | bucle que imprime el orden en `printTurnHeader` |
| `game_engine.h:235` | bucle de intentos en `playTurn` |
| `main.cpp:17` | bucle de argumentos en `parseArguments` |

Ahora las llamadas a algoritmos:

```bash
grep -rn 'ranges::\|views::' code/
```

```text
code/board.h:44:        std::ranges::fill(visited, CellRow{});
code/move_generator.h:23:    std::ranges::transform(
code/move_generator.h:24:        std::views::iota(0, moveCandidateCount), candidates.begin(),
code/game_engine.h:125:        std::ranges::transform(
code/game_engine.h:126:            std::views::iota(0, moveCandidateCount), moveOrder,
```

Tres puntos de llamada, dos de ellos `transform` sobre `iota`, uno `fill`.

`board.h`, rangos relevantes para el recorrido:

| Líneas | Contenido |
|---|---|
| 3–4 | Includes. `<algorithm>` aporta `fill`; `<array>` aporta `std::array`. |
| 6–7 | El encabezado de plantilla y `class Board`. |
| 25–40 | `is_valid()`. `for` anidado, `return false` temprano. Sin cambios. |
| 42–45 | `reset()`. Una llamada a `std::ranges::fill`. |
| 47–63 | `was_visited` y `mark_visited`. Sin cambios. |
| 65–74 | Constantes privadas, `CellRow` y el almacenamiento `std::array`. |

`move_generator.h`, rangos relevantes para el recorrido:

| Líneas | Contenido |
|---|---|
| 3–5 | Includes. `<algorithm>` y `<ranges>` son nuevos; `<array>` ya estaba. |
| 9–10 | Los ocho cambios de fila y columna, como arreglos `constexpr`. |
| 12–17 | `MoveCandidate`. |
| 19–34 | `generate_move_candidates`. Un `transform` sobre `iota`. |

`game_engine.h`, rangos relevantes para el recorrido:

| Líneas | Contenido |
|---|---|
| 123–131 | `buildMoveOrder`. Un `transform` sobre `iota`. |
| 133–148 | `printTurnHeader`. Un `for` con separador. Sin cambios. |
| 234–286 | El bucle de intentos de `playTurn`. Sin cambios. |

Recuentos de esta línea base. La regla de conteo se enuncia para que puedas reproducirla:

- Encabezados: 7 (`board.h`, `game.h`, `game_engine.h`, `knight.h`, `move_generator.h`,
  `position.h`, `random_sequence.h`).
- Unidades de traducción: 5 (`main.cpp`, `game.cpp`, `knight.cpp`, `position.cpp`,
  `random_sequence.cpp`).
- Definiciones de clase completas: 5 (`Board`, `GameEngine`, `Knight`, `Position`,
  `MoveCandidate`).
- Cuerpos de función con nombre escritos a mano: 41, los mismos 41 que `construction-16`. Las tres
  lambdas son nuevas y se cuentan aparte.
- Lambdas: 3, una por punto de llamada a algoritmo.
- Encabezados de `for`: 5, antes 9.
- Puntos de llamada a algoritmos: 3 (`fill`, `transform`, `transform`).
- Líneas de código: 611, que son 606 más 5.
- Archivos que difieren de `construction-16`: 3 de 12.

Después responde en `EVIDENCE-es.md` §2:

1. ¿Qué tres recorridos pasaron a algoritmos, y en qué líneas?
2. ¿Qué cuatro encabezados de `for` desaparecieron?
3. ¿Cuántos encabezados de `for` quedan, y dónde?
4. ¿Cuántos puntos de llamada a algoritmos hay, y qué algoritmos son?
5. ¿Qué encabezado aporta `fill`, y cuál aporta `transform` e `iota`?
6. ¿Qué archivo creció más, y cuántas líneas?
7. ¿Qué archivos son idénticos byte a byte a `construction-16`?
8. ¿Qué limpia ahora `Board::reset` que el bucle anterior no limpiaba?
9. ¿Qué recorrido restante tiene una salida temprana pura y podría pasar a `any_of`?
10. ¿Qué recorrido restante no puede pasar a un algoritmo sin esconder efectos?

Las preguntas 8 y 10 tienen respuestas precisas, y ambas se desarrollan más adelante. Pregunta 8: el
nuevo `fill` limpia las 16×16 celdas, mientras que el bucle anterior limpiaba solo el área activa de
8×8. Pregunta 10: el bucle de intentos, por las cuatro razones de la sección del concepto.

## Paso 2 — Predecir antes de ejecutar (~10 min)

Escribe estas predicciones antes de ejecutar nada. No mires las respuestas del paso 3 primero.

### 2.1 La tabla de estructura

| Pregunta | Mi predicción |
|---|---|
| Encabezados de `for` en `construction-16` | |
| Encabezados de `for` aquí | |
| Puntos de llamada a algoritmos aquí | |
| Líneas de código en `construction-16` | |
| Líneas de código aquí | |
| Archivos que difieren | |
| Qué archivo se reduce | |
| Qué archivo crece más | |
| Lambdas añadidas | |

La fila fácil de equivocar es la tercera. Se reemplazaron tres recorridos, pero uno de ellos era un
bucle anidado, así que cuatro encabezados de bucle se convirtieron en tres llamadas a algoritmos.

### 2.2 La tabla de tamaños y texto

| Pregunta | Mi predicción |
|---|---|
| `sizeof(Board<8, 8>)` aquí | |
| `sizeof(GameEngine<8, 8>)` aquí | |
| `sizeof(std::array<MoveCandidate, 8>)` | |
| Bytes de texto de `game.o`, antes y después | |
| Bytes de texto del binario, antes y después | |
| Bytes de datos del binario, antes y después | |
| Reservas de montón por motor, antes y después | |
| Líneas preprocesadas de `game.cpp`, antes y después | |
| Tiempo de compilación de `game.cpp`, antes y después | |
| Semillas que coinciden, de 20, después del cambio | |

Predice la fila de reservas como cero y cero. `views::iota` es una vista perezosa; no reserva. El
motor sigue reservando tres veces por los `unique_ptr`, y ese es el coste de `construction-16`, no
el de esta lección.

Predice la fila del tiempo de compilación al final. Es la que hace visible el precio de la STL, y
depende de la máquina.

### 2.3 Las predicciones de la prueba de cambio

La prueba de cambio es: reemplazar un recorrido que esté en el camino de la transcripción por un
algoritmo, y mostrar que la transcripción no cambia.

| Pregunta | Mi predicción |
|---|---|
| El recorrido que voy a reemplazar | |
| Su rango de líneas | |
| El algoritmo que voy a usar | |
| Encabezados de `for` después del cambio | |
| ¿Cambia la transcripción? | |
| Semillas que coinciden, de 20, después del cambio | |
| El bucle de intentos después del cambio | |

Predice el bucle de intentos como sin cambios. Si tu cambio lo toca, elegiste el contraejemplo por
error.

## Paso 3 — Ejecutar la línea base (~10 min)

Primero las trazas, sin modificar:

```bash
mkdir -p /tmp/stl-test
./run.sh --seed 1 --name Learner > /tmp/stl-test/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/stl-test/baseline-0001.txt
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
  -o build/stl-baseline code/main.cpp code/game.cpp code/knight.cpp \
  code/position.cpp code/random_sequence.cpp
```

Ahora los tamaños. La sonda va en el directorio temporal, nunca en los archivos versionados:

```bash
cat > /tmp/stl-test/sizes.cpp <<'EOF'
#include "game_engine.h"

#include <cstdio>

int main()
{
    std::printf("sizeof(Board<8,8>)=%zu\n", sizeof(Board<8, 8>));
    std::printf("sizeof(GameEngine<8,8>)=%zu\n", sizeof(GameEngine<8, 8>));
    std::printf("sizeof(Knight)=%zu\n", sizeof(Knight));
    std::printf("sizeof(Position)=%zu\n", sizeof(Position));
    std::printf("sizeof(MoveCandidate)=%zu\n", sizeof(MoveCandidate));
    std::printf("sizeof(std::array<MoveCandidate,8>)=%zu\n",
                sizeof(std::array<MoveCandidate, moveCandidateCount>));
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/stl-test/sizes /tmp/stl-test/sizes.cpp \
  code/knight.cpp code/position.cpp
/tmp/stl-test/sizes
```

La salida en esta plataforma:

```text
sizeof(Board<8,8>)=256
sizeof(GameEngine<8,8>)=24
sizeof(Knight)=40
sizeof(Position)=8
sizeof(MoveCandidate)=12
sizeof(std::array<MoveCandidate,8>)=96
```

Todos los números coinciden con `construction-16`. `std::array` no cambió el tamaño del tablero, y
los algoritmos no cambiaron el tamaño del motor.

Ahora el código objeto:

```bash
g++ -std=c++20 -O2 -Icode -c code/game.cpp -o /tmp/stl-test/game17.o
g++ -std=c++20 -O2 -I../construction-16-ownership/code \
  -c ../construction-16-ownership/code/game.cpp -o /tmp/stl-test/game16.o
size /tmp/stl-test/game16.o /tmp/stl-test/game17.o
```

```text
   text	   data	    bss	    dec	    hex	filename
   4638	      0	      0	   4638	   121e	/tmp/stl-test/game16.o
   4656	      0	      0	   4656	   1230	/tmp/stl-test/game17.o
```

Dieciocho bytes. Enlaza ambos programas y compara los binarios:

```bash
g++ -std=c++20 -O2 -Icode -o /tmp/stl-test/bin17 \
  code/main.cpp code/game.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
g++ -std=c++20 -O2 -I../construction-16-ownership/code -o /tmp/stl-test/bin16 \
  ../construction-16-ownership/code/main.cpp ../construction-16-ownership/code/game.cpp \
  ../construction-16-ownership/code/knight.cpp ../construction-16-ownership/code/position.cpp \
  ../construction-16-ownership/code/random_sequence.cpp
size /tmp/stl-test/bin16 /tmp/stl-test/bin17
```

```text
   text	   data	    bss	    dec	    hex	filename
   9652	    692	     24	  10368	   2880	/tmp/stl-test/bin16
   9668	    692	     24	  10384	   2890	/tmp/stl-test/bin17
```

El texto crece 16 bytes. Los datos y el BSS no se mueven. Ninguno de los dos archivos objeto
contiene una llamada indirecta:

```bash
objdump -d -C /tmp/stl-test/game16.o | grep -c 'call.*\*' || true
objdump -d -C /tmp/stl-test/game17.o | grep -c 'call.*\*' || true
```

```text
0
0
```

Los algoritmos son plantillas, así que las llamadas son directas y se insertan en línea. La STL no
añadió una capa de despacho.

El coste de compilación es real, y es el número por el que normalmente no se pregunta a la STL:

```bash
g++ -std=c++20 -Icode -E code/game.cpp | wc -l
g++ -std=c++20 -I../construction-16-ownership/code \
  -E ../construction-16-ownership/code/game.cpp | wc -l
for run in 1 2 3; do
  /usr/bin/time -f '%e' g++ -std=c++20 -O2 -Icode -c code/game.cpp \
    -o /tmp/stl-test/t17.o
done
for run in 1 2 3; do
  /usr/bin/time -f '%e' g++ -std=c++20 -O2 -I../construction-16-ownership/code \
    -c ../construction-16-ownership/code/game.cpp -o /tmp/stl-test/t16.o
done
```

```text
54432
72259
0.50
0.51
0.49
0.62
0.61
0.60
```

La entrada preprocesada crece 17.827 líneas, alrededor del 33 por ciento. El tiempo de compilación
crece unos 0,11 s, alrededor del 22 por ciento, en esta máquina y con este compilador. Tus números
variarán; la dirección no debería.

Si el primer diff no está vacío, detente. Esa es una línea base distinta de la que describe esta
lección; regístrala en `EVIDENCE-es.md` §11 y no ajustes la predicción para que coincida.

## Paso 4 — Comparar: qué demuestran los algoritmos (~10 min)

Abre `board.h`, `move_generator.h` y `game_engine.h`. Cinco hechos, leídos contra las mediciones.

1. **Tres recorridos ahora nombran su contrato.** `fill` dice que cada elemento recibe el mismo
   valor. `transform` dice que hay una salida por cada entrada. El punto de llamada ya no contiene
   el límite, el paso ni el acceso al elemento.
2. **El reinicio del tablero cambió más que su forma.** El bucle anterior limpiaba 64 celdas, filas
   1–8 y columnas 1–8. El nuevo `fill` limpia las 256 celdas del almacenamiento de 16×16. La
   transcripción no puede ver la diferencia, porque las celdas fuera del área activa nunca se
   marcan y `is_valid` solo comprueba que sean falsas. El trabajo por reinicio es cuatro veces
   mayor, una vez por partida.
3. **`std::array` es un valor, no un puntero.** `sizeof(Board<8, 8>)` sigue en 256. El
   `std::array<bool, 16>` anidado tiene la misma disposición que `bool[16][16]`, y `CellRow{}` es
   una fila a cero. El cambio compra semántica de valor y soporte de rangos, no un tablero más
   pequeño.
4. **`views::iota` no reserva nada.** El motor sigue haciendo tres reservas, y vienen de las tres
   llamadas a `make_unique` que introdujo `construction-16`. Los algoritmos no añaden ninguna
   llamada a `operator new`.
5. **El código objeto apenas se mueve.** `game.o` crece 18 bytes de texto y el binario 16. El
   compilador insertó los cuerpos de los algoritmos y generó el mismo tipo de bucle que el código ya
   no escribe.

### 4.1 El diff exacto

Cambiaron tres archivos. `diff -u` contra `construction-16` da 26 líneas añadidas y 21 eliminadas:

```bash
for file in board.h move_generator.h game_engine.h; do
  diff -u "../construction-16-ownership/code/${file}" "code/${file}"
done
```

| Archivo | Añadidas | Eliminadas | Neto | Qué cambió |
|---|---|---|---|---|
| `board.h` | 7 | 8 | -1 | Dos includes, un `fill`, almacenamiento `std::array` |
| `move_generator.h` | 12 | 8 | +4 | Dos includes, un `transform` sobre `iota` |
| `game_engine.h` | 7 | 5 | +2 | Un include, un `transform` sobre `iota` |
| Total | 26 | 21 | +5 | Tres puntos de llamada, cinco includes |

Los otros nueve archivos son idénticos byte a byte. Las reglas, el formato de transcripción, el
modelo de propiedad y el bucle de turnos no se tocaron.

### 4.2 Lo que no cambió

- La transcripción. Las 20 trazas coinciden byte a byte.
- Las reglas. El orden de movimientos, la captura, la trampa, el empate y el límite de 200
  movimientos son los mismos.
- Las interfaces. `reset`, `was_visited`, `mark_visited`, `generate_move_candidates` y
  `buildMoveOrder` conservan sus firmas.
- El modelo de propiedad. Tres miembros `unique_ptr`, copia eliminada, movimiento por defecto.
- Los tamaños. Tablero 256, motor 24, caballo 40, candidato 12.
- La cuenta de reservas. Tres por motor, sin cambios.
- Las llamadas indirectas en `game.o`. Cero antes, cero después.

### 4.3 Lo que cuesta

- Cinco líneas de `include`, y el peso transitivo que traen `<algorithm>` y `<ranges>`. En esta
  máquina, la entrada preprocesada de `game.cpp` crece 17.827 líneas.
- Unos 0,11 s más de compilación para `game.cpp` en esta máquina.
- 18 bytes más de texto en `game.o`, 16 en el binario enlazado.
- Cuatro veces las escrituras por reinicio del tablero, porque `fill` limpia todo el
  almacenamiento de 16×16.
- Una cosa más que leer: la lambda. El cuerpo del bucle sigue ahí, ahora como una expresión que
  devuelve un valor.

Ninguno de estos puntos es un defecto. Son el precio de mudar el recorrido a un algoritmo con
nombre, y el registro debe listarlos junto al beneficio.

### 4.4 La semántica del reinicio, con precisión

`construction-16` guardaba `bool visited[16][16]` y reiniciaba las filas 1..`RowCount`, columnas
1..`ColumnCount`. `construction-17` guarda `std::array<std::array<bool, 16>, 16>` y llama a
`std::ranges::fill(visited, CellRow{})`, que asigna una fila a cero a cada una de las 16 filas.

Ambas versiones parten de `visited{}` en el constructor, así que ambas empiezan todo a falso.
`mark_visited` rechaza coordenadas fuera del área activa, y `was_visited` también las rechaza.
`is_valid` comprueba que el área exterior siga falsa. Por lo tanto, las dos versiones son
observacionalmente equivalentes, y las 192 escrituras extra por reinicio son invisibles.

Ese es el tipo de diferencia que una transcripción no puede atrapar, y vale la pena nombrarla: un
algoritmo puede ensanchar el rango que toca aunque su nombre suene como el bucle al que reemplazó.

## Paso 5 — Prueba de cambio: reemplazar un recorrido visible (~12 min)

La aceptación nombra la prueba de cambio: **eliminar un bucle de recorrido, usar un algoritmo y
mostrar que la transcripción no cambia.**

Los tres algoritmos de la línea base ya son el lado «después». Para la prueba de cambio, toma un
bucle que siga ahí y que la transcripción pueda ver. `printTurnHeader` es el correcto: imprime el
orden de movimientos en cada turno, así que un error cambia todas las trazas.

### 5.1 Hacer una copia temporal

Nunca edites el directorio versionado `code/`. Cópialo:

```bash
rm -rf /tmp/stl-test/change
cp -r code /tmp/stl-test/change
```

### 5.2 Reemplazar el bucle

En `/tmp/stl-test/change/game_engine.h`, reemplaza las líneas 139–146 por:

```cpp
std::ranges::for_each(
    std::views::iota(0, moveCandidateCount),
    [moveOrder](int orderIndex)
    {
        std::printf("%d", moveOrder[orderIndex]);
        if (orderIndex + 1 < moveCandidateCount)
        {
            std::printf(",");
        }
    });
```

El encabezado del bucle, el límite y el paso desaparecieron. El rango de origen, la operación y la
regla del separador son los tres argumentos que quien lee puede ver. La lambda sigue usando un
índice, porque el separador depende de la posición; esa aritmética no desapareció, se mudó.

### 5.3 Compilar y comparar

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I/tmp/stl-test/change \
  -o /tmp/stl-test/change-bin \
  /tmp/stl-test/change/main.cpp /tmp/stl-test/change/game.cpp \
  /tmp/stl-test/change/knight.cpp /tmp/stl-test/change/position.cpp \
  /tmp/stl-test/change/random_sequence.cpp
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/stl-test/change-bin --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
grep -rn 'for (' /tmp/stl-test/change | wc -l
```

El bucle queda silencioso, y la cuenta imprime `4`. La transcripción no cambió en las 20 semillas, y
desapareció un encabezado de `for` más.

### 5.4 Qué demuestra la prueba de cambio, y qué no

Demuestra que un recorrido en el camino de la salida puede cambiar de forma sin cambiar la salida.
No demuestra que el algoritmo sea más rápido: esta lección midió tamaño de texto y tiempo de
compilación, no tiempo de ejecución. No demuestra que el resultado sea más claro para cualquier
lector: la lambda es más larga que el cuerpo del bucle al que reemplazó, y quien no conoce
`for_each` tiene que buscarlo.

Tampoco autoriza cambiar el bucle de intentos. Inténtalo si quieres ver el fallo: un `find_if` cuyo
predicado imprime compilará, y el resultado esconderá la impresión, la mutación y los cuatro
resultados dentro de un nombre que promete una sola decisión. Registra ese intento en
`EVIDENCE-es.md` §6 si lo haces; el fallo es parte de la evidencia.

## Paso 6 — Comparar con `construction-16` (~8 min)

| Métrica | construction-16 | construction-17 | Diferencia |
|---|---|---|---|
| Archivos | 12 | 12 | 0 |
| Encabezados | 7 | 7 | 0 |
| Unidades de traducción | 5 | 5 | 0 |
| Líneas de código | 606 | 611 | +5 |
| `board.h` | 76 | 75 | -1 |
| `move_generator.h` | 30 | 34 | +4 |
| `game_engine.h` | 300 | 302 | +2 |
| Otros 9 archivos | — | idénticos byte a byte | 0 |
| Encabezados de `for` | 9 | 5 | -4 |
| Puntos de llamada a algoritmos | 0 | 3 | +3 |
| Lambdas | 0 | 3 | +3 |
| Clases completas | 5 | 5 | 0 |
| Cuerpos de función con nombre | 41 | 41 | 0 |
| Declaraciones públicas | 31 | 31 | 0 |
| `sizeof(Board<8, 8>)` | 256 | 256 | 0 |
| `sizeof(GameEngine<8, 8>)` | 24 | 24 | 0 |
| Reservas de montón por motor | 3 | 3 | 0 |
| Texto de `game.o` | 4.638 | 4.656 | +18 |
| Texto / datos del binario | 9.652 / 692 | 9.668 / 692 | +16 / 0 |
| Líneas preprocesadas, `game.cpp` | 54.432 | 72.259 | +17.827 |
| Tiempo de compilación, `game.cpp` | ~0,50 s | ~0,61 s | +0,11 s |
| Llamadas indirectas en `game.o` | 0 | 0 | 0 |
| Trazas | 20 de 20 | 20 de 20 | 0 |

### 6.1 Qué compra

- El contrato está en el punto de llamada. `fill` y `transform` dicen qué pasa con cada elemento, y
  quien lee no tiene que reconstruir el límite y el paso.
- El rango de destino es explícito. `transform` recibe `candidates.begin()` o `moveOrder`, así que
  el destino es un argumento y no un nombre capturado por el cuerpo del bucle.
- El recorrido es reutilizable. El mismo `fill` y el mismo `transform` funcionan con cualquier rango
  del tipo de elemento adecuado; el bucle había que reescribirlo para cada colección.
- La aritmética de índices queda aislada en una lambda. Quien se interesa por la transformación lee
  la lambda; quien se interesa por el recorrido lee la llamada.
- El almacenamiento del tablero ganó semántica de valor y soporte de rangos sin cambiar de tamaño.

### 6.2 Qué cuesta

- Cinco líneas de `include` y 17.827 líneas preprocesadas para `game.cpp`.
- Unos 0,11 s más de compilación en esta máquina.
- 18 bytes más de texto en `game.o`, 16 en el binario.
- Cuatro veces las escrituras por reinicio, porque `fill` limpia todo el almacenamiento de 16×16.
- Una lambda por punto de llamada, y quien no conoce el algoritmo tiene que buscarlo.
- Un ensanchamiento semántico que la transcripción no puede ver: el reinicio ahora toca celdas
  fuera del área activa.
- Un encabezado que no es autosuficiente: `game_engine.h` llama a `std::ranges::transform` pero
  incluye `<ranges>`, no `<algorithm>`. Obtiene `transform` a través de `move_generator.h`, que
  incluye `<algorithm>`. Añade el include directamente si tocas ese encabezado.

### 6.3 El hueco: tres recorridos cambiaron, cuatro no

La línea base no es «sin bucles manuales». Son tres recorridos con nombre y cinco encabezados de
`for`. Los que quedan son deliberados:

- `printTurnHeader` imprime una lista separada. Tiene un efecto por elemento y una regla de
  separador. La prueba de cambio muestra que puede pasar a `for_each`, pero la línea base lo deja
  como bucle.
- El bucle de intentos imprime, muta y ramifica. Es el contraejemplo.
- `Board::is_valid` tiene una salida temprana pura y podría pasar a `any_of`. El juego no lo usa,
  así que el cambio no sería observable.
- `parseArguments` recorre `argv`. Es análisis de argumentos, no un recorrido de una colección, y un
  `for` es la forma honesta.

La frase de aceptación precisa es más estrecha que «el programa usa la STL»:

> Quien aprende puede eliminar un bucle de recorrido, reemplazarlo por un algoritmo y mostrar que
> las 20 transcripciones son idénticas byte a byte.

Un recorrido, un algoritmo, una transcripción sin cambios. Nada sobre velocidad, claridad para
cualquier lector ni eliminar todos los bucles.

## Paso 7 — Explicar (~6 min)

Escribe `EVIDENCE-es.md` §8. Dos partes.

**Primero, una frase.** Di qué cambió entre las dos líneas base y qué decide el cambio, sin usar las
palabras *STL*, *rangos*, *algoritmo*, *bucle*, *iterador*, *mejor*, *peor*, *limpio* ni *moderno*.
Si tu frase necesita una de ellas, nombraste el mecanismo en lugar de la decisión.

**Segundo, un párrafo.** Responde:

- Qué decide `fill`: cada celda del almacenamiento pasa a falso, incluidas las celdas fuera del área
  activa.
- Qué decide `transform`: una salida por cada entrada, en orden, con la transformación escrita como
  una lambda.
- Qué decide `views::iota`: los índices se generan, no se almacenan, y no se reserva nada.
- Qué demuestra la cuenta de `for`: desaparecieron cuatro encabezados de bucle y quedan cinco.
- Qué deciden los bucles restantes: salida con separadores, el contrato de intentos, una salida
  temprana pura que no se usa y el análisis de argumentos.
- Qué demuestra el diff: cambiaron tres archivos y nueve no.
- Qué demuestran los tamaños: el tablero y el motor no cambiaron de tamaño.
- Qué demuestra el código objeto: 18 bytes de texto, ninguna reserva nueva, ninguna llamada
  indirecta.
- Qué demuestran los números de compilación: la abstracción tiene un precio en tiempo de
  compilación.
- Qué demuestra la transcripción: el cambio es invisible en el comportamiento.
- Qué demuestra el bucle de intentos: un recorrido con efectos no tiene la misma forma que un
  recorrido que solo produce un valor.
- Qué lección posterior se ocupa de la pureza y la inmutabilidad: `construction-18`.

Una respuesta fuerte mantiene cuatro cosas separadas: **qué promete el recorrido** (llenar,
transformar, primera coincidencia), **qué enuncia el punto de llamada** (origen, destino,
operación), **qué paga el compilador** (peso de includes, tiempo de compilación, 18 bytes) y **qué
puede ver una ejecución** (la transcripción, sin cambios). Una respuesta débil dice «la STL
reemplaza los bucles», lo cual es falso en ambos sentidos: el bucle sigue dentro del algoritmo, y en
este código quedan cinco bucles.

## Paso 8 — Verificación de aceptación y registro (~2 min)

El criterio de aceptación de esta lección es: **eliminar un bucle de recorrido, usar un algoritmo y
mostrar que la transcripción no cambia.**

Comprueba tu registro contra él:

- [ ] Localicé los tres puntos de llamada a algoritmos y sus líneas.
- [ ] Localicé los cuatro encabezados de `for` que desaparecieron y los cinco que quedan.
- [ ] Expliqué por qué `Board::reset` ahora limpia 256 celdas en lugar de 64.
- [ ] Expliqué por qué `views::iota` no reserva.
- [ ] Predije las tablas de estructura, tamaños y texto antes de medir.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Reemplacé el bucle de `printTurnHeader` en una copia temporal por `for_each` sobre `iota`.
- [ ] La compilación temporal pasó con `-Wall -Wextra -Werror`.
- [ ] Las 20 trazas coincidieron después del cambio.
- [ ] La cuenta de `for` en la copia temporal bajó de 5 a 4.
- [ ] Medí `sizeof(Board<8, 8>)` en 256 y `sizeof(GameEngine<8, 8>)` en 24.
- [ ] Medí el texto de `game.o` en 4.656 y el del binario en 9.668.
- [ ] Medí la entrada preprocesada y el tiempo de compilación, y registré ambos.
- [ ] Expliqué por qué se queda el bucle de intentos.
- [ ] Nombré `Board::is_valid` como un recorrido que podría ser `any_of` y no se cambió.
- [ ] No edité ningún archivo de código versionado.
- [ ] Completé `EVIDENCE-es.md` §1–§10 y la autocomprobación de §12.

El criterio no es «el código usa la STL». Es «puedes nombrar qué promete un recorrido, reemplazar
uno por un algoritmo y mostrar que el comportamiento no se movió».

## Autocomprobación (binaria)

- [ ] Leí `board.h`, `move_generator.h` y `game_engine.h` y localicé las tres llamadas a algoritmos
      por línea.
- [ ] Conté los encabezados de `for` antes y después: 9 y 5.
- [ ] Comparé la línea base con STL con `construction-16` y nombré los tres archivos cambiados.
- [ ] Predije las tablas de estructura, tamaños y texto antes de ejecutar.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Reemplacé un recorrido visible en la transcripción por un algoritmo en una copia temporal.
- [ ] Mostré que las 20 trazas siguen coincidiendo después del cambio.
- [ ] Medí el tablero en 256 bytes y el motor en 24.
- [ ] Medí `game.o` en 4.656 bytes de texto y el binario en 9.668.
- [ ] Medí la entrada preprocesada y el tiempo de compilación.
- [ ] Nombré el bucle de intentos como contraejemplo y di las cuatro propiedades que lo mantienen
      como bucle.
- [ ] Nombré la diferencia semántica de `Board::reset`: 256 celdas en lugar de 64.
- [ ] Completé `EVIDENCE-es.md`.

Estos son criterios de aceptación. Si una casilla queda
sin marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §10.

## Misconcepciones que esta lección refuta

| Error de concepto | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| «La STL elimina los bucles.» | El bucle está dentro del algoritmo; en este código quedan cinco encabezados de `for`. | Cuenta ambas cosas: llamadas a algoritmos y encabezados de `for` restantes. |
| «Los algoritmos siempre son más rápidos.» | Esta lección mide +18 bytes de texto, más tiempo de compilación y ninguna prueba de ejecución. | Di qué coste mediste y no afirmes el que no mediste. |
| «`std::ranges::fill` limpia solo el área activa de 8×8.» | Llena las 16 filas del almacenamiento de 16×16. | Lee el tipo de almacenamiento y el argumento de `fill`. |
| «`views::iota` reserva un arreglo de índices.» | Es una vista perezosa; guarda dos enteros. | Comprueba que la cuenta de reservas no cambió. |
| «`transform` puede escribir más allá del rango de destino.» | El rango de destino debe tener espacio; aquí es un `std::array` fijo o un arreglo C fijo. | Cuenta el tamaño del origen y el del destino. |
| «Cualquier bucle puede pasar a un algoritmo.» | El bucle de intentos intercala impresión, mutación y ramificación. | Pregunta qué hace el trabajo por elemento, no cuántos elementos hay. |
| «Salida temprana siempre significa conservar el bucle.» | `any_of` expresa una salida temprana pura; `is_valid` es ese caso. | Separa la salida temprana de los efectos que la rodean. |
| «La transcripción demuestra que el algoritmo es correcto.» | Demuestra que el comportamiento no cambió; no dice nada sobre eficiencia ni claridad. | Acompaña la comprobación de trazas con las mediciones de tamaño y compilación. |
| «`std::array` es un puntero.» | Es un tipo de valor que envuelve un arreglo crudo; aquí sigue midiendo 256 bytes. | Mide `sizeof(Board<8, 8>)`. |
| «Los rangos requieren C++23.» | La línea base compila con `-std=c++20`. | Mira la bandera de compilación, no la versión que recuerdas. |
| «La versión con algoritmos no tiene índice.» | La lambda sigue recibiendo un índice; la aritmética se mudó. | Lee el parámetro de la lambda. |
| «El código se hizo más corto.» | Creció 5 líneas. | Cuenta líneas, no encabezados de bucle. |
| «`for_each` puede romper el recorrido.» | No puede; su contrato no tiene salida temprana. | Usa `any_of` o `find_if` para una búsqueda pura, o conserva el bucle. |
| «`std::ranges::transform` y `std::transform` son la misma llamada.» | La versión de rangos recibe un rango; la clásica recibe dos iteradores. | Lee el primer argumento. |
| «Cada encabezado incluye todo lo que usa.» | `game_engine.h` llama a `transform` pero obtiene `<algorithm>` de forma transitiva por `move_generator.h`. | Incluye `<algorithm>` directamente si tocas ese encabezado. |
| «La diferencia del reinicio no importa porque la transcripción coincide.» | Son cuatro veces las escrituras por reinicio, y una transcripción no ve escrituras. | Nombra el rango que toca cada versión. |
| «El bucle de intentos es solo una optimización.» | Es un contrato: imprime cada intento, decide, muta y para en el primero aceptado. | Lee las cuatro propiedades de la sección del concepto. |

## Siguiente paso

`construction-18` se ocupa de la pureza y la inmutabilidad. El motor todavía muta el tablero, los
caballos y la bandera de turno en el lugar. Un estilo funcional devolvería un estado nuevo y
conservaría el anterior, lo que convierte deshacer y rehacer en conservar historia en lugar de
revertir efectos. La pregunta del recorrido se cierra aquí: los bucles que quedan son aquellos cuyos
efectos los algoritmos no pueden nombrar. La siguiente lección pregunta si esos efectos deberían
existir.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog.md) para la ruta completa.

Si falta alguna de las dos líneas base, no empieces `construction-17`. El siguiente contraste
necesita ambos lados.
