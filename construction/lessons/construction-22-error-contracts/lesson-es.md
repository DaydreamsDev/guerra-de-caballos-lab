# Contratos de error (errores)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** recuperación · **Tiempo:** unos 60 minutos · **Estado:** borrador
**Lectura:** ~25 minutos

## Por qué existe esta lección

`construction-21` terminó con un núcleo cuya aleatoriedad pertenecía al motor. La comprobación del
movimiento, en cambio, todavía devolvía su respuesta por dos parámetros de salida:

```cpp
MoveOutcome tryMove(const Knight *otherKnight, const MoveCandidate &candidate,
                    int &destinationRow, int &destinationColumn) const
```

Quien llamaba tenía que declarar los dos enteros antes de la llamada, y el enumerado no decía nada
sobre ellos:

```cpp
int destinationRow = 0;
int destinationColumn = 0;
MoveOutcome outcome = tryMove(otherKnight, candidate, destinationRow, destinationColumn);
```

De una llamada vuelven dos cosas, y solo una es la respuesta. El compilador no puede distinguir si
`0` era una coordenada intencionada o un relleno. `construction-22` reúne la respuesta en un solo
valor:

```cpp
struct MoveResult
{
    MoveOutcome outcome;
    int destinationRow;
    int destinationColumn;
};
```

Ahora la llamada devuelve la respuesta completa, y quien llama no puede quedarse con un resultado a
medias:

```cpp
MoveResult moveResult = tryMove(otherKnight, candidate);
```

La frase que conviene retener:

> Un error es un valor que quien llama puede inspeccionar. Una salida del proceso es una decisión
> que tomó el llamado por todos.

Tres notas honestas antes de la comparación.

1. El código crece 41 líneas, de 1.030 en 18 archivos a 1.071 en 20. Dos archivos son nuevos:
   `move_result.h` (16 líneas) y `error_probe.cpp` (25). Un archivo cambió: `game_engine.h`, 27
   líneas añadidas y 27 eliminadas, saldo cero, todavía 246 líneas. Los otros diecisiete son
   idénticos byte a byte a `construction-21`.
2. El objeto crece 32 bytes de texto: `game.o` pasa de 4.280 a 4.312. El binario enlazado sigue el
   mismo movimiento, de 10.869 a 10.901 bytes de texto. Los datos siguen en 876, BSS sigue en 16 y
   `sizeof(GameEngine<8, 8>)` sigue en 40. `sizeof(MoveResult)` es 12 y `sizeof(MoveOutcome)` es 4.
   El objeto de la sonda, `error_probe.o`, mide 2.633 bytes de texto.
3. La transcripción no cambia. Las 20 trazas de referencia coinciden byte a byte. No se movió
   ninguna regla, ni el tablero, ni el generador, ni los adaptadores.

La comparación no es «devolver una estructura es mejor». Es más estrecha: una respuesta que llegaba
en tres piezas llega en una, el caso inválido se convierte en una rama que escribe quien llama, y
ninguna ruta del núcleo mata el proceso.

## Qué podrás hacer

Al terminar esta lección puedes explicar recovery y aplicar la prueba de cambio de la lección.

## El concepto: tres formas de decir no

### Las tres formas

Un error puede viajar hacia quien llama de tres formas, y la forma decide quién manda.

| Forma | Quién decide qué pasa después | Qué debe escribir quien llama | Qué se puede olvidar | Esta línea base |
|---|---|---|---|---|
| Valor devuelto | quien llama | una rama sobre el valor | sí, en silencio | sí |
| Excepción | quien lanza la inicia, quien llama puede detenerla | `try` / `catch` | no, se propaga | no |
| `std::expected<T, E>` | quien llama | una rama sobre `.has_value()` | sí, en silencio | no |

El valor devuelto es la forma más barata y la más fácil de ignorar. Un `MoveOutcome` ignorado no
cuesta nada en tiempo de compilación y produce una transcripción incorrecta en tiempo de ejecución.
La lección mantiene esa debilidad visible en lugar de esconderla detrás de un tipo que obligue a
ramificar.

Dos notas honestas sobre las otras formas.

- El núcleo no lanza excepciones porque nada de un movimiento es excepcional en el sentido de C++.
  Un movimiento inválido es un evento normal: las reglas lo esperan, la transcripción lo registra y
  el bucle continúa. Las excepciones son para «esta función no puede producir su resultado», no para
  «la respuesta es no».
- `std::expected` es C++23, y esta línea base es C++20. Si compilas esta lección en C++23,
  `std::expected<MoveResult, MoveError>` es la misma idea con un tipo de biblioteca: un valor de
  retorno que lleva un resultado o un motivo. La forma es lo que importa aquí, no la cabecera.
  Escribir `MoveResult` a mano es la manera C++20 de ver la forma antes de que una biblioteca la
  nombre.

### Por qué no `exit`

`exit` dentro del núcleo tiene tres costes, y son concretos en este proyecto.

1. **Destruye la elección de quien llama.** Quien quería informar el intento, reintentar con otro
   candidato o seguir la partida ya no puede. El llamado ya decidió.
2. **Trunca la observación.** Las 20 trazas de referencia son observaciones a nivel de proceso. Un
   `exit` en medio de una partida termina la transcripción antes de tiempo, y la comparación falla
   en un archivo más corto, no en una regla incorrecta.
3. **Hace que el camino de error no se pueda probar en proceso.** `evaluate_move` devuelve un valor,
   así que una prueba puede llamarlo mil veces y contar los resultados. Un `exit` solo se observa
   arrancando un proceso, o bifurcándolo.

La afirmación precisa de esta lección: el núcleo nunca llama a `exit`. El `main` de consola devuelve
`2` ante argumentos incorrectos. Sigue siendo una decisión, pero es la decisión de quien llama,
tomada después de que quien llama vio el problema. Un estado de retorno distinto de cero y una
salida del proceso no son lo mismo; el primero es un valor que lee el shell, la segunda es un salto
de control que ejecuta el llamado.

### Cuatro resultados, un orden

`MoveOutcome` tiene exactamente cuatro valores. No hay `unknown` ni `none`, y toda llamada devuelve
uno de ellos.

| Resultado | Valor del enumerado | Significado | Qué hace `playTurn` |
|---|---|---|---|
| `outsideBoard` | 0 | destino fuera de las filas 1–8 o las columnas 1–8 | `illegal_attempt(..., "outside")`, siguiente candidato |
| `visited` | 1 | destino ya visitado por cualquiera de los caballos | `illegal_attempt(..., "visited")`, siguiente candidato |
| `capture` | 2 | destino es la casilla del otro caballo | `capture_attempt`, acepta, fin de partida |
| `legal` | 3 | dentro, no visitado y vacío | `legal_attempt`, acepta, cambia de turno |

El orden de las comprobaciones dentro de `tryMove` es una regla, no una elección de estilo.

```cpp
if (outside the board)      -> outsideBoard
if (other knight's cell)    -> capture
if (already visited)        -> visited
otherwise                   -> legal
```

`outsideBoard` va primero porque las otras dos comprobaciones suponen coordenadas dentro del
tablero. `capture` va antes que `visited` porque la casilla del otro caballo es la única excepción
legal a la regla de visitadas (game-rules §7). Si intercambias las dos últimas, una captura sobre
una casilla visitada se convierte en un intento ilegal, la transcripción gana una línea
`illegal=visited` y las trazas divergen.

Ese orden es la razón por la que el enumerado no es un simple booleano `wasLegal`. Un booleano
juntaría `outsideBoard` y `visited` en una sola respuesta, y la transcripción los distingue.

### Los valores son valores del enumerado

La sonda imprime:

```text
outside=0 capture=2 legal=3
```

Son posiciones en el enumerado, no códigos de error elegidos a mano. El `static_cast<int>` de
`error_probe.cpp` es el único lugar donde aparecen los números. Los números son un detalle de
implementación; los nombres son el contrato. Si alguien reordena el enumerado, la salida de la sonda
cambia y la transcripción no.

### Dónde se esconde el cuarto valor

La sonda usa un motor recién construido. Un `Board` nuevo empieza con todas las casillas sin
visitar, así que `evaluate_move` no puede devolver `visited` en ese programa. La sonda demuestra
tres de los cuatro resultados. El cuarto necesita un tablero con historia: ejecutar una partida y
después preguntar por los destinos.

Es un hueco de la sonda, no del contrato. El paso 3.5 muestra la medición, y `EVIDENCE-es.md` §11 es
donde lo nombras. Una lección que esconde el caso ausente enseña la sonda, no el contrato.

## Antes de empezar

- [`construction-09`](../construction-09-abstract-data-type/lesson.md) — el prerrequisito formal del
  catálogo. El contrato es un tipo valor pequeño y una función total; basta con leer `enum class` y
  un `struct` agregado, sin diseñar una jerarquía de tipos.
- [`construction-21`](../construction-21-concurrency/lesson.md) — la línea base inmediata y el lado
  izquierdo del diff del Paso 4. Su directorio `code/` es el artefacto del que parte esta lección,
  aunque el catálogo indique `construction-09`.
- [`construction-10`](../construction-10-adt-invariants/lesson.md) y
  [`construction-11`](../construction-11-encapsulation/lesson.md) — opcionales. El `Board` sigue
  siendo dueño de su invariante, y `was_visited` sigue respondiendo `false` para una coordenada
  fuera de rango en lugar de indexar fuera del arreglo.
- Un compilador de C++20 (`g++` o `clang++`). `cmake` es opcional; `run.sh` recurre a una
  compilación directa cuando CMake no está.

No necesitas excepciones, `std::expected`, mónadas de error ni una biblioteca de registro. La línea
base es un enumerado, una estructura, una cadena de `if` y un `return 2`.

### Qué necesitas

- La línea base de contratos de error:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/game_engine.h`](code/game_engine.h),
  [`code/move_result.h`](code/move_result.h), [`code/error_probe.cpp`](code/error_probe.cpp),
  [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp),
  [`code/transcript_port.h`](code/transcript_port.h),
  [`code/console_transcript.h`](code/console_transcript.h),
  [`code/console_transcript.cpp`](code/console_transcript.cpp),
  [`code/recording_transcript.h`](code/recording_transcript.h),
  [`code/adapter_probe.cpp`](code/adapter_probe.cpp),
  [`code/parallel_runner.cpp`](code/parallel_runner.cpp), [`code/board.h`](code/board.h),
  [`code/move_generator.h`](code/move_generator.h), [`code/knight.h`](code/knight.h),
  [`code/knight.cpp`](code/knight.cpp), [`code/position.h`](code/position.h),
  [`code/position.cpp`](code/position.cpp).
- La línea base anterior, para el diff:
  [`code/game_engine.h`](../construction-21-concurrency/code/game_engine.h), y sus diecisiete
  compañeros en [`../construction-21-concurrency/code/`](../construction-21-concurrency/code/).
- Guiones de compilación y notas: [`run.sh`](run.sh), [`CMakeLists.txt`](CMakeLists.txt) y
  [`README.md`](README.md).
- Las reglas: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §7 fija cuándo un
  movimiento es legal, §8 cuándo una casilla pasa a visitada, §9 la captura.
- El formato de transcripción:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md) — las líneas
  `illegal=outside` e `illegal=visited`.
- Una traza de referencia: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- Esta plantilla de registro: [`EVIDENCE-es.md`](EVIDENCE-es.md).

Comprueba que los artefactos están presentes antes de empezar, desde este directorio de la lección:

```bash
test -f code/main.cpp &&
test -f code/game.h &&
test -f code/game.cpp &&
test -f code/game_engine.h &&
test -f code/move_result.h &&
test -f code/error_probe.cpp &&
test -f code/random_sequence.h &&
test -f code/random_sequence.cpp &&
test -f code/transcript_port.h &&
test -f code/console_transcript.h &&
test -f code/console_transcript.cpp &&
test -f code/recording_transcript.h &&
test -f code/adapter_probe.cpp &&
test -f code/parallel_runner.cpp &&
test -f code/board.h &&
test -f code/move_generator.h &&
test -f code/knight.h &&
test -f code/knight.cpp &&
test -f code/position.h &&
test -f code/position.cpp &&
test -f ../construction-21-concurrency/code/game_engine.h &&
test -f ../../../common/traces/seed-0001.txt
```

Si falta un archivo, la comparación no tiene segundo lado. Anota el archivo ausente en
`EVIDENCE-es.md` §11 y vuelve cuando esté disponible. Un recuento sobre un archivo que no has leído
no es evidencia.

La línea base de 8×8 reproduce las 20 trazas de `common/traces/` byte a byte, antes de que cambies
nada. Ese es el contrato de comportamiento, y esta lección no lo toca.

## Paso 1 — Inventariar el contrato (~10 min)

Empieza por el tipo. El contrato son dos declaraciones en una cabecera:

```bash
grep -n 'enum class\|struct\|MoveOutcome\|destinationRow' code/move_result.h
```

```text
3:enum class MoveOutcome
4:{
5:    outsideBoard,
6:    visited,
7:    capture,
8:    legal
9:};
11:struct MoveResult
12:{
13:    MoveOutcome outcome;
14:    int destinationRow;
15:    int destinationColumn;
16:};
```

Ahora la función que lo produce:

```bash
grep -n 'evaluate_move\|MoveResult tryMove\|tryMove(' code/game_engine.h
```

```text
67:    MoveResult evaluate_move(const Knight &otherKnight, const MoveCandidate &candidate) const
69:        return tryMove(&otherKnight, candidate);
139:    MoveResult tryMove(const Knight *otherKnight, const MoveCandidate &candidate) const
187:            MoveResult moveResult = tryMove(otherKnight, candidate);
```

Cuatro líneas. `evaluate_move` es el punto de entrada público, `tryMove` es la implementación
privada y `playTurn` es el único consumidor de producción. La sonda es el segundo consumidor.

Ahora las salidas del proceso. Es la afirmación que hace la lección, así que tiene comando:

```bash
grep -rn '\bexit\b' code/
```

Sin salida. El núcleo, los adaptadores, las sondas y el ejecutor nunca llaman a `exit`. Los únicos
retornos distintos de cero son errores de argumentos:

```bash
grep -n 'return 2' code/main.cpp code/parallel_runner.cpp
```

```text
code/main.cpp:74:        return 2;
code/parallel_runner.cpp:181:        return 2;
code/parallel_runner.cpp:207:    return 2;
```

Recuentos de esta línea base. La regla de conteo se enuncia para que puedas reproducirla:

- Archivos: 20.
- Cabeceras: 11 (`board.h`, `console_transcript.h`, `game.h`, `game_engine.h`, `knight.h`,
  `move_generator.h`, `move_result.h`, `position.h`, `random_sequence.h`, `recording_transcript.h`,
  `transcript_port.h`).
- Unidades de traducción en la compilación de consola: 6 (`main.cpp`, `game.cpp`,
  `console_transcript.cpp`, `knight.cpp`, `position.cpp`, `random_sequence.cpp`).
- Unidades de traducción en la compilación de la sonda: 4 (`error_probe.cpp`, `knight.cpp`,
  `position.cpp`, `random_sequence.cpp`).
- Definiciones de clase completas en el núcleo: 9 (las mismas nueve que `construction-21`).
- Variables globales mutables en el núcleo: 0.
- Valores en `MoveOutcome`: 4.
- Miembros en `MoveResult`: 3.
- Líneas de código: 1.071.
- Archivos idénticos byte a byte a `construction-21`: 17 de 18.
- Archivos cambiados: 1 (`game_engine.h`).
- Archivos nuevos: 2 (`move_result.h`, `error_probe.cpp`).

Después responde en `EVIDENCE-es.md` §2:

1. ¿Qué archivo declara `MoveOutcome` y `MoveResult`?
2. ¿Cuántos valores tiene `MoveOutcome`, y cuáles son sus nombres y números?
3. ¿Qué devuelve `tryMove`, y qué devolvía en `construction-21`?
4. ¿Qué método público expone el contrato sin ejecutar una partida?
5. ¿Qué función es el único consumidor de producción de `tryMove`?
6. ¿Cuántas ramas toma `playTurn` sobre el resultado, y qué hace cada una?
7. ¿Algún archivo de `code/` llama a `exit`? ¿Qué comando lo muestra?
8. ¿Qué devuelve `main` ante argumentos incorrectos, y por qué eso no es un `exit`?

La pregunta 8 tiene una respuesta precisa: devuelve `2`, el proceso termina porque `main` retornó, y
quien llamó a `main` —el shell— recibe el estado. El programa no saltó desde el medio de una
función; terminó.

## Paso 2 — Predecir antes de ejecutar (~10 min)

Escribe estas predicciones antes de ejecutar nada. No mires las respuestas del Paso 3 primero.

### 2.1 La tabla de estructura

| Pregunta | Mi predicción |
|---|---|
| Archivos en `construction-21` | |
| Archivos aquí | |
| Cabeceras aquí | |
| Unidades de traducción en la compilación de consola | |
| Líneas de código en `construction-21` | |
| Líneas de código aquí | |
| Archivos cambiados | |
| Archivos nuevos | |
| Archivos idénticos byte a byte | |
| Valores en `MoveOutcome` | |
| Miembros en `MoveResult` | |

La fila fácil de fallar es «archivos cambiados». El motor parece distinto, pero el puerto, los
adaptadores, el tablero, los caballos y el ejecutor están intactos. Una cabecera movió un tipo fuera
de sí misma; ese es todo el diff fuera de los dos archivos nuevos.

### 2.2 La tabla de la sonda

| Pregunta | Mi predicción |
|---|---|
| Línea de salida de `error_probe` | |
| Estado de salida de `error_probe` | |
| Qué resultado falta en la sonda, y por qué | |
| Estado de salida de `main` sin `--name` | |
| Estado de salida de `main` con una partida válida | |
| Trazas que coinciden, de 20 | |

Predice los tres números como posiciones del enumerado, no como códigos de error. Predice el
resultado ausente como `visited`, porque un tablero nuevo no tiene ninguna casilla visitada.

### 2.3 La tabla de tamaños

| Pregunta | Mi predicción |
|---|---|
| `sizeof(MoveOutcome)` | |
| `sizeof(MoveResult)` | |
| `sizeof(GameEngine<8, 8>)`, antes y después | |
| Bytes de texto de `game.o`, antes y después | |
| Bytes de texto del binario, antes y después | |
| Bytes de datos del binario, antes y después | |
| Bytes de BSS del binario, antes y después | |
| Bytes de texto de `error_probe.o` | |

Predice `MoveResult` en 12: un enumerado de 4 bytes y dos enteros de 4, sin relleno que añadir.
Predice que `game.o` crece, porque cada llamada ahora construye y devuelve un valor de 12 bytes en
lugar de escribir por dos referencias. Predice datos y BSS planos: no hay global nuevo ni estático
nuevo.

### 2.4 La tabla de decisión

| Pregunta | Mi predicción |
|---|---|
| ¿Quién decide continuar tras un movimiento inválido? | |
| ¿Quién decide informarlo? | |
| ¿Quién decide detenerse? | |
| Qué sabe el núcleo de la transcripción | |
| Qué sabe el núcleo del proceso | |

Predice «quien llama» en las tres filas. Si tu respuesta nombra a `tryMove` en alguna fila, el
contrato todavía no se movió.

## Paso 3 — Ejecutar la línea base (~15 min)

### 3.1 Las trazas

Primero las trazas, sin modificar:

```bash
mkdir -p /tmp/error-test
for trace in ../../../common/traces/seed-*.txt; do
    traceName="$(basename "${trace}" .txt)"
    seedText="${traceName#seed-}"
    seedNumber="$((10#${seedText}))"
    ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

Un bucle silencioso es 20 de 20. Si `run.sh` no puede compilar, compila una vez a mano:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
    -o /tmp/error-test/error-baseline \
    code/main.cpp code/game.cpp code/console_transcript.cpp \
    code/knight.cpp code/position.cpp code/random_sequence.cpp
```

Después ejecuta el mismo bucle con `/tmp/error-test/error-baseline` en lugar de `./run.sh`.

### 3.2 La sonda de errores

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode -o /tmp/error-test/error-probe \
    code/error_probe.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
/tmp/error-test/error-probe
echo "exit=$?"
```

La salida en esta máquina:

```text
outside=0 capture=2 legal=3
exit=0
```

Tres resultados, tres números, una salida normal. La sonda es un consumidor completo: construye un
`RecordingTranscript`, un motor y un `otherKnight`, hace tres preguntas e imprime las respuestas.
Nunca inicia una partida.

Lee los tres candidatos de `error_probe.cpp` contra la tabla:

| Candidato | Destino | Resultado | Por qué |
|---|---|---|---|
| `{1, 0, 0}` | fila 0, columna 0 | `outsideBoard` (0) | la fila 0 está fuera de 1–8 |
| `{1, 1, 1}` | fila 1, columna 1 | `capture` (2) | el otro caballo está ahí |
| `{1, 2, 3}` | fila 2, columna 3 | `legal` (3) | dentro, vacía, no visitada |

El campo `moveNumber` es `1` en los tres porque `evaluate_move` no lo lee. Solo lee el destino. Vale
la pena notarlo: `MoveCandidate` lleva un número para la transcripción, y el contrato lo ignora. Un
tipo más pequeño llevaría solo el destino, pero la línea base reutiliza el candidato que el bucle
del juego ya tiene.

### 3.3 El camino de argumentos

```bash
/tmp/error-test/error-baseline --seed 1
echo "exit=$?"
```

```text
usage: /tmp/error-test/error-baseline --seed INTEGER --name TEXT
exit=2
```

El mensaje va a la salida de error, la transcripción queda vacía y el estado es `2`. Compáralo con
una partida válida:

```bash
/tmp/error-test/error-baseline --seed 1 --name Learner > /dev/null
echo "exit=$?"
```

```text
exit=0
```

Dos estados, ambos producidos por un `return` desde `main`. No aparece ninguna llamada a `exit` en
ninguno de los dos caminos.

### 3.4 Tamaños

La sonda va en el directorio de trabajo temporal, nunca en los archivos versionados:

```bash
cat > /tmp/error-test/sizes.cpp <<'EOF'
#include "game_engine.h"
#include "move_result.h"
#include "recording_transcript.h"

#include <cstdio>

int main()
{
    std::printf("sizeof(MoveOutcome)=%zu\n", sizeof(MoveOutcome));
    std::printf("sizeof(MoveResult)=%zu\n", sizeof(MoveResult));
    std::printf("sizeof(GameEngine<8,8>)=%zu\n", sizeof(GameEngine<8, 8>));
    return 0;
}
EOF
c++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
    -o /tmp/error-test/sizes /tmp/error-test/sizes.cpp \
    code/knight.cpp code/position.cpp code/random_sequence.cpp
/tmp/error-test/sizes
```

La salida en esta plataforma:

```text
sizeof(MoveOutcome)=4
sizeof(MoveResult)=12
sizeof(GameEngine<8,8>)=40
```

`MoveResult` mide 12 bytes: el enumerado 4, cada entero 4, y el total no tiene relleno. El motor
sigue en 40, exactamente como en `construction-21`. El contrato es un valor que se devuelve; no se
convierte en miembro.

### 3.5 Opcional: el cuarto resultado

La sonda muestra tres resultados. Para ver `visited`, dale historia al tablero:

```bash
cat > /tmp/error-test/visited_probe.cpp <<'EOF'
#include "game_engine.h"
#include "recording_transcript.h"

#include <cstdio>

int main()
{
    RecordingTranscript transcript;
    GameEngine<8, 8> engine(transcript);
    engine.run(1, "Learner");

    Knight otherKnight("other");
    otherKnight.set_position(1, 1);

    int counts[4] = {0, 0, 0, 0};
    for (int row = 0; row <= 9; row += 1)
    {
        for (int column = 0; column <= 9; column += 1)
        {
            MoveResult result =
                engine.evaluate_move(otherKnight, MoveCandidate{1, row, column});
            counts[static_cast<int>(result.outcome)] += 1;
        }
    }
    std::printf("outside=%d visited=%d capture=%d legal=%d\n", counts[0], counts[1], counts[2],
                counts[3]);
    return 0;
}
EOF
c++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
    -o /tmp/error-test/visited-probe /tmp/error-test/visited_probe.cpp \
    code/knight.cpp code/position.cpp code/random_sequence.cpp
/tmp/error-test/visited-probe
```

La salida en esta máquina:

```text
outside=36 visited=21 capture=1 legal=42
```

100 destinos: 36 fuera del tablero de 8×8 (el recorrido va de 0 a 9), 21 visitados por la partida, 1
ocupado por el otro caballo, 42 legales. Los recuentos exactos dependen de la semilla; lo importante
es que los cuatro resultados aparecen cuando el tablero tiene historia.

Este programa llama a `evaluate_move` sobre un motor que ya ejecutó una partida. Es un uso legítimo
de un método público, pero también es una advertencia: `evaluate_move` lee el tablero actual del
motor y el otro caballo que le pasas. Es una consulta, no una función pura de sus dos argumentos. La
lección nombra ese acoplamiento en §6.3.

### 3.6 Objeto y binario

```bash
c++ -std=c++20 -O2 -Icode -c code/game.cpp -o /tmp/error-test/game22.o
c++ -std=c++20 -O2 -I../construction-21-concurrency/code \
    -c ../construction-21-concurrency/code/game.cpp -o /tmp/error-test/game21.o
size /tmp/error-test/game21.o /tmp/error-test/game22.o
c++ -std=c++20 -O2 -Icode -c code/error_probe.cpp -o /tmp/error-test/error_probe.o
size /tmp/error-test/error_probe.o
```

```text
   text    data     bss     dec     hex filename
   4280       0       0    4280    10b8 /tmp/error-test/game21.o
   4312       0       0    4312    10d8 /tmp/error-test/game22.o
   text    data     bss     dec     hex filename
   2633       0       0    2633     a49 /tmp/error-test/error_probe.o
```

Ahora los binarios enlazados:

```bash
c++ -std=c++20 -O2 -Icode -o /tmp/error-test/bin22 \
    code/main.cpp code/game.cpp code/console_transcript.cpp \
    code/knight.cpp code/position.cpp code/random_sequence.cpp
c++ -std=c++20 -O2 -I../construction-21-concurrency/code \
    -o /tmp/error-test/bin21 \
    ../construction-21-concurrency/code/main.cpp \
    ../construction-21-concurrency/code/game.cpp \
    ../construction-21-concurrency/code/console_transcript.cpp \
    ../construction-21-concurrency/code/knight.cpp \
    ../construction-21-concurrency/code/position.cpp \
    ../construction-21-concurrency/code/random_sequence.cpp
size /tmp/error-test/bin21 /tmp/error-test/bin22
```

```text
   text    data     bss     dec     hex filename
  10869     876      16   11761    2df1 /tmp/error-test/bin21
  10901     876      16   11793    2e11 /tmp/error-test/bin22
```

El texto crece 32 bytes en el objeto y 32 en el binario. Los datos y BSS no se mueven. El contrato
es un valor de retorno, no una tabla ni un global.

Si la primera comparación de trazas no está vacía, detente. Esa es otra línea base distinta de la
que describe esta lección; anótala en `EVIDENCE-es.md` §11 y no ajustes la predicción para que
coincida.

## Paso 4 — Comparar: qué lleva el valor (~10 min)

Abre `move_result.h`, `game_engine.h`, `game.cpp` y `main.cpp`. Cinco hechos, leídos contra las
mediciones.

1. **Una respuesta, un valor.** `tryMove` devuelve `MoveResult`. Los dos parámetros de salida
   desaparecieron de la firma, y quien llama ya no declara dos enteros antes de la llamada.
2. **El destino siempre está lleno.** `tryMove` inicializa la estructura con el destino del
   candidato en la primera línea y después solo sobrescribe el resultado. No hay ruta que devuelva
   una coordenada sin inicializar, incluido el camino `outsideBoard`, donde la coordenada es la que
   se rechazó.
3. **El resultado es total.** Toda llamada devuelve uno de cuatro valores. `evaluate_move` puede
   llamarse desde una sonda sin partida, y el compilador sigue exigiendo una rama para cada valor en
   un `switch` sin `default`.
4. **Quien llama decide.** `playTurn` ramifica sobre `moveResult.outcome`: dos resultados continúan
   al siguiente candidato e informan, uno acepta una captura, uno acepta un movimiento legal.
   Ninguna rama llama a `exit`.
5. **El estado del proceso es aparte.** `main` devuelve `2` ante argumentos incorrectos y el valor
   de `run_game` ante una partida. Ambos son retornos. El estado que ve el shell es un valor que
   cruzó la frontera del proceso, no un salto desde el medio del núcleo.

### 4.1 El diff exacto

Dos archivos son nuevos y uno cambió. El diff contra `construction-21`:

```bash
diff -rq ../construction-21-concurrency/code code
diff -u ../construction-21-concurrency/code/game_engine.h code/game_engine.h
```

| Archivo | Añadidas | Eliminadas | Saldo | Qué cambió |
|---|---:|---:|---:|---|
| `game_engine.h` | 27 | 27 | 0 | `MoveOutcome` sale; `tryMove` devuelve `MoveResult`; aparece `evaluate_move` |
| `move_result.h` | 16 | 0 | +16 | Cabecera nueva: el enumerado y la estructura |
| `error_probe.cpp` | 25 | 0 | +25 | Programa nuevo: tres llamadas, tres números |
| Total | 68 | 27 | +41 | Uno cambiado, dos nuevos, diecisiete idénticos |

Los diecisiete archivos idénticos son `board.h`, `move_generator.h`, `knight.h`, `knight.cpp`,
`position.h`, `position.cpp`, `transcript_port.h`, `console_transcript.h`,
`console_transcript.cpp`, `recording_transcript.h`, `game.h`, `main.cpp`, `game.cpp`,
`adapter_probe.cpp`, `parallel_runner.cpp`, `random_sequence.h` y `random_sequence.cpp`.

Fíjate en lo que **no** cambió dentro de `game_engine.h`: `playTurn` conserva las mismas cuatro
ramas y las mismas dos condiciones de parada. Solo cambió la forma de recibir la respuesta. El diff
es en buena parte mecánico, y ese es el punto: un cambio de contrato que reescribiera la lógica
sería otra lección, más grande.

### 4.2 Lo que no cambió

- Las reglas. El orden de movimientos, la captura, la trampa, el empate y el límite de 200
  movimientos son los mismos.
- La transcripción. Las 20 trazas coinciden byte a byte, incluidas las líneas `illegal=outside` e
  `illegal=visited`.
- El tablero, el generador de movimientos, el caballo, la posición y la secuencia aleatoria.
- El puerto y ambos adaptadores.
- La interfaz pública del juego: `game.h` y `run_game` conservan sus firmas.
- El tamaño del motor: 40 bytes, igual que en `construction-21`.

### 4.3 Lo que cuesta

- 41 líneas nuevas y dos archivos nuevos, uno de ellos un programa.
- 32 bytes de texto más en `game.o` y 32 más en el binario enlazado.
- Una cabecera más en el grafo de inclusión. `game_engine.h` incluye `move_result.h`; toda unidad de
  traducción que incluye el motor ahora ve el contrato.
- Un método público nuevo, `evaluate_move`, que es un segundo punto de entrada a `tryMove`. Dos
  puntos de entrada significan dos contratos que mantener sincronizados.
- Una carga de nombres: `MoveResult` no es un `Move`, y `outcome` no es un `status`. El tipo dice lo
  que es, y el lector todavía tiene que aprender los cuatro nombres.

Nada de esto es un defecto. Es el precio de convertir un protocolo oculto en uno visible, y el
registro debe anotarlo junto al beneficio.

### 4.4 La tabla del contrato

Esta tabla es el centro de la comparación. Léela contra la sonda.

| Pregunta | `construction-21` | `construction-22` |
|---|---|---|
| Qué devuelve `tryMove` | `MoveOutcome`, más dos parámetros de salida | un `MoveResult` |
| Quién inicializa el destino | quien llama, antes de la llamada | `tryMove`, en su primera línea |
| Puede olvidarlo quien llama | sí, `0` compila | no, el valor lo lleva |
| Puede ignorar el resultado | sí | sí |
| Número de rutas de retorno | 4, cada una devuelve un enumerado | 4, cada una devuelve una estructura |
| Quién decide continuar | `playTurn` | `playTurn`, a partir de un valor |
| Quién decide detener el proceso | nadie en el núcleo | nadie en el núcleo |
| Cómo ve el contrato una sonda | no disponible; `tryMove` es privado | `evaluate_move` |
| Tamaño de la respuesta | 4 bytes más dos enteros de quien llama | 12 bytes |

La última fila es el intercambio honesto: la respuesta es más grande, y está completa.

## Paso 5 — Prueba de cambio: que decida quien llama (~15 min)

La aceptación nombra la prueba de cambio: **llamar a `evaluate_move` con un movimiento inválido, una
captura y un movimiento legal; registrar el resultado; hacer que quien llama decida continuar,
informar o detenerse; mostrar que ninguna ruta llama a `exit` y que las 20 trazas siguen pasando.**

Ya viste los tres resultados en el paso 3.2. Ahora escribe la política.

### 5.1 Haz una copia de trabajo

Nunca edites el directorio versionado `code/`. Cópialo:

```bash
rm -rf /tmp/error-test/change
cp -r code /tmp/error-test/change
```

### 5.2 La política de quien llama

Crea `/tmp/error-test/change/recovery_probe.cpp`:

```cpp
#include "game_engine.h"
#include "recording_transcript.h"

#include <cstdio>
#include <cstring>

enum class Recovery
{
    continueGame,
    reportAndContinue,
    stop
};

Recovery decide(MoveOutcome outcome, bool stopOnInvalid)
{
    switch (outcome)
    {
    case MoveOutcome::legal:
    case MoveOutcome::capture:
        return Recovery::continueGame;
    case MoveOutcome::outsideBoard:
    case MoveOutcome::visited:
        return stopOnInvalid ? Recovery::stop : Recovery::reportAndContinue;
    }
    return Recovery::stop;
}

int main(int argumentCount, char **argumentValues)
{
    bool stopOnInvalid = argumentCount == 2 &&
                         std::strcmp(argumentValues[1], "--stop-on-invalid") == 0;

    RecordingTranscript transcript;
    GameEngine<8, 8> engine(transcript);
    Knight otherKnight("other");
    otherKnight.set_position(1, 1);

    const MoveCandidate candidates[3] = {
        {1, 0, 0},
        {1, 1, 1},
        {1, 2, 3},
    };

    for (const MoveCandidate &candidate : candidates)
    {
        MoveResult result = engine.evaluate_move(otherKnight, candidate);
        Recovery recovery = decide(result.outcome, stopOnInvalid);
        std::printf("try=%d to=%d,%d outcome=%d recovery=%d\n", candidate.moveNumber,
                    result.destinationRow, result.destinationColumn,
                    static_cast<int>(result.outcome), static_cast<int>(recovery));
        if (recovery == Recovery::stop)
        {
            return 0;
        }
    }
    return 0;
}
```

Tres cosas que leer en ese archivo.

1. `decide` es una función total sobre `MoveOutcome`. No hay etiqueta `default`, así que añadir un
   quinto valor al enumerado es un error de compilación hasta que la política lo trate. Es el
   compilador haciendo cumplir el contrato.
2. La política es un parámetro, no una constante. `--stop-on-invalid` convierte los mismos tres
   resultados en una recuperación distinta sin tocar `tryMove`.
3. `return 0` es la forma en que el programa se detiene. Es un retorno normal desde `main`, no un
   `exit`.

### 5.3 Ejecútalo

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 -I/tmp/error-test/change \
    -o /tmp/error-test/recovery-probe \
    /tmp/error-test/change/recovery_probe.cpp \
    /tmp/error-test/change/knight.cpp \
    /tmp/error-test/change/position.cpp \
    /tmp/error-test/change/random_sequence.cpp
/tmp/error-test/recovery-probe
echo "exit=$?"
/tmp/error-test/recovery-probe --stop-on-invalid
echo "exit=$?"
```

La salida en esta máquina:

```text
try=1 to=0,0 outcome=0 recovery=1
try=1 to=1,1 outcome=2 recovery=0
try=1 to=2,3 outcome=3 recovery=0
exit=0
try=1 to=0,0 outcome=0 recovery=2
exit=0
```

La primera ejecución informa el movimiento inválido y continúa hacia la captura y el movimiento
legal. La segunda se detiene tras el primer movimiento inválido, con estado 0. Ambas son decisiones
de quien llama, tomadas a partir del valor devuelto. Ninguna es una salida del proceso dentro del
núcleo.

Anota la correspondencia que usaste: `continueGame=0`, `reportAndContinue=1`, `stop=2`. Si reordenas
el enumerado `Recovery`, los números cambian y la lección no.

### 5.4 Las trazas

La prueba de cambio tocó un archivo temporal. La compilación de consola no cambió, así que las
trazas deben seguir pasando:

```bash
for trace in ../../../common/traces/seed-*.txt; do
    traceName="$(basename "${trace}" .txt)"
    seedText="${traceName#seed-}"
    seedNumber="$((10#${seedText}))"
    ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

El bucle queda silencioso: 20 de 20. Un cambio de contrato que alterara la transcripción habría
cambiado el juego, y este no lo hace.

### 5.5 Qué prueba la prueba de cambio, y qué no

Prueba cinco cosas.

1. El movimiento inválido es observable como valor. Tres llamadas, tres resultados, uno inválido.
2. La recuperación pertenece a quien llama. Los mismos tres resultados producen informar-y-continuar
   o detenerse, según un parámetro de quien llama.
3. El núcleo no necesita conocer la política. `tryMove` y `playTurn` no se tocan en el cambio.
4. Detenerse es un retorno normal. Ambas ejecuciones terminan con estado 0, y no aparece ningún
   `exit`.
5. La transcripción es independiente de la política. Las 20 trazas pasan porque la compilación de
   consola nunca vio la sonda.

No prueba que un movimiento inválido sea imposible, que quien llama siempre compruebe el valor, ni
que una política sea correcta. No prueba que `visited` se pueda producir desde un motor nuevo; no se
puede, y el paso 3.5 es el rodeo. No prueba que el orden del enumerado sea estable entre versiones;
los nombres son el contrato, los números no.

## Paso 6 — Comparar con `construction-21` (~5 min)

| Métrica | `construction-21` | `construction-22` | Diferencia |
|---|---:|---:|---:|
| Archivos | 18 | 20 | +2 |
| Cabeceras | 10 | 11 | +1 |
| Unidades de traducción (compilación de consola) | 6 | 6 | 0 |
| Líneas de código | 1.030 | 1.071 | +41 |
| Archivos cambiados | — | 1 | +1 |
| Archivos nuevos | — | 2 | +2 |
| Archivos idénticos byte a byte | — | 17 de 18 | — |
| Valores en `MoveOutcome` | 4 | 4 | 0 |
| Miembros en `MoveResult` | — | 3 | +3 |
| `sizeof(MoveResult)` | — | 12 | +12 |
| `sizeof(GameEngine<8, 8>)` | 40 | 40 | 0 |
| Texto de `game.o` | 4.280 | 4.312 | +32 |
| Texto del binario | 10.869 | 10.901 | +32 |
| Datos del binario | 876 | 876 | 0 |
| BSS del binario | 16 | 16 | 0 |
| Trazas | 20 de 20 | 20 de 20 | 0 |
| Llamadas a `exit` en `code/` | 0 | 0 | 0 |

### 6.1 Qué compra

- Un valor de retorno lleva la respuesta completa. Quien llama no puede tener un destino sin un
  resultado, porque llegan juntos.
- El caso inválido es una rama que escribe quien llama, no un salto que ejecuta el llamado.
- El contrato se puede probar sin partida: `evaluate_move` es público, `const` y devuelve un valor.
- El resultado es exhaustivo. Un `switch` sin `default` hace que el compilador pregunte por cada
  valor.
- Los dos motivos ilegales del formato de transcripción se corresponden uno a uno con dos valores
  del enumerado, así que el adaptador recibe el motivo en lugar de recalcularlo.
- El núcleo no tiene `exit`, así que quien llama puede ejecutar una partida dentro de un programa
  mayor y mantener el proceso vivo.
- Las 20 trazas siguen pasando, así que el comportamiento de consola no cambió.

### 6.2 Qué cuesta

- 41 líneas nuevas, dos archivos nuevos y una cabecera más en el grafo de inclusión.
- 32 bytes de texto más en `game.o` y en el binario. La respuesta mide 12 bytes en lugar de 4.
- Un segundo punto de entrada público (`evaluate_move`) hacia una función privada. Dos puntos de
  entrada significan dos contratos que mantener sincronizados.
- Los valores numéricos del enumerado ahora son visibles a través de una sonda. Reordenar el
  enumerado es un cambio incompatible para la sonda y no para la transcripción; la lección tiene que
  decir cuál es el contrato.
- Quien llama todavía puede ignorar el resultado. Un valor devuelto no es un valor comprobado, y el
  sistema de tipos no fuerza la rama.

### 6.3 El hueco: un valor no es una política

La línea base no es «el programa maneja errores». Es «el motor devuelve una respuesta, y quien llama
decide». Quedan cinco acoplamientos, y vale la pena nombrarlos.

1. **`evaluate_move` lee el estado actual del motor.** Es `const`, pero es una consulta sobre el
   tablero y el otro caballo, no una función pura de sus dos argumentos. En un motor nuevo no puede
   devolver `visited`; después de una partida sí. La sonda demuestra tres de cuatro resultados por
   esa razón.
2. **Quien llama puede ignorar el resultado.** Nada en `MoveResult` fuerza una rama. Un consumidor
   futuro que descarte el valor compila y produce una transcripción incorrecta. `[[nodiscard]]` en
   `tryMove` y `evaluate_move` haría que el compilador avisara; la línea base no lo usa, y esa
   omisión es deliberada y medible.
3. **La política está fuera de la línea base.** `playTurn` es una política: informar y probar el
   siguiente candidato. La prueba de cambio escribe otra. Un programa real necesitaría una regla
   para lo que pasa después de `stop`, y esta lección no la define.
4. **Los números del enumerado no son estables.** `outside=0 capture=2 legal=3` es la distribución
   actual. Los nombres son el contrato; los números son una comodidad de depuración.
5. **`exit` está ausente de `code/`, no del proceso.** Una biblioteca todavía puede llamar a
   `std::abort` ante una aserción, y el sistema operativo todavía puede matar el proceso. La
   afirmación es estrecha: este código no decide terminar el proceso ante un movimiento inválido.

La frase de aceptación precisa es más estrecha que «el programa maneja errores»:

> La persona que aprende puede gestionar un movimiento inválido mediante un valor devuelto, dejar
> que quien llama elija la recuperación, y mostrar que el proceso continúa y que la transcripción no
> cambia.

Valor devuelto, quien llama decide, el proceso continúa, las trazas no cambian. Nada sobre seguridad
ante excepciones, jerarquías de error ni registro.

## Paso 7 — Explicar (~5 min)

Escribe `EVIDENCE-es.md` §8. Dos partes.

**Primero, una frase.** Di qué cambió entre las dos líneas base y qué decide el cambio, sin usar las
palabras *struct*, *enumerado*, *valor de retorno*, *parámetro de salida*, *excepción*, *expected*,
*mejor*, *peor*, *limpio* ni *desacoplado*. Si tu frase necesita una de ellas, nombraste el
mecanismo y no la decisión.

**Segundo, un párrafo.** Responde:

- Qué cambió: `tryMove` devuelve un `MoveResult` en lugar de un resultado más dos parámetros de
  salida; `MoveOutcome` se mudó a su propia cabecera; `evaluate_move` expone el contrato en público.
- Qué se quedó: los cuatro resultados, su orden, la transcripción, el tablero, el bucle y las 20
  trazas.
- Qué añade la sonda: tres llamadas sin partida, y tres números que son posiciones del enumerado.
- Qué omite la sonda: `visited`, porque un tablero nuevo no tiene casillas visitadas.
- Qué decide quien llama: continuar, informar o detenerse; `playTurn` informa y continúa, la prueba
  de cambio escribe una segunda política.
- Qué hace el proceso: retorna desde `main`; ninguna rama de `code/` llama a `exit`.
- Qué prueban los números: `sizeof(MoveResult)` 12, `game.o` de 4.280 a 4.312, binario de 10.869 a
  10.901 de texto, datos y BSS planos, trazas 20 de 20.
- Qué le toca a la lección siguiente: el port a otro lenguaje, `construction-23`.

Una respuesta sólida mantiene cuatro cosas separadas: **qué devuelve el motor** (un valor), **qué
decide quien llama** (continuar, informar, detenerse), **qué registra la transcripción** (los mismos
bytes) y **qué hace el proceso** (devuelve un estado, nunca sale desde el núcleo). Una respuesta
débil dice «el código ahora maneja errores», que es falso: el código devuelve una respuesta, y quien
llama todavía puede ignorarla.

## Paso 8 — Comprobación de aceptación y registro (~5 min)

El criterio de aceptación de esta lección es: **movimiento inválido gestionado sin `exit`; las 20
trazas pasan.**

Comprueba tu registro contra él:

- [ ] Leí `move_result.h`, `game_engine.h`, `error_probe.cpp` y `main.cpp`.
- [ ] Nombré los cuatro resultados y su orden dentro de `tryMove`.
- [ ] Expliqué por qué `capture` se comprueba antes que `visited`.
- [ ] Conté 20 archivos, 11 cabeceras y 1.071 líneas de código.
- [ ] Nombré el archivo cambiado y los dos archivos nuevos.
- [ ] Predije las tablas de estructura, sonda, tamaño y decisión antes de medir.
- [ ] Reproduje las 20 trazas en la línea base de consola sin modificar.
- [ ] Ejecuté `error_probe` y vi `outside=0 capture=2 legal=3` con estado 0.
- [ ] Ejecuté `main` con un argumento incorrecto y vi estado 2.
- [ ] Medí `sizeof(MoveOutcome)` en 4 y `sizeof(MoveResult)` en 12.
- [ ] Medí el texto de `game.o` en 4.312 y el binario en 10.901 de texto, 876 de datos y 16 de BSS.
- [ ] Medí `error_probe.o` en 2.633 bytes de texto.
- [ ] Vi el cuarto resultado con un tablero con historia, o anoté por qué no.
- [ ] Escribí una política de quien llama en una copia temporal y la ejecuté en dos modos.
- [ ] Ambos modos terminaron con estado 0 y sin `exit`.
- [ ] Las 20 trazas coincidieron después de la prueba de cambio.
- [ ] La compilación temporal pasó con `-Wall -Wextra -Werror`.
- [ ] Nombré qué no garantiza el valor: quien llama puede ignorarlo.
- [ ] No edité ningún archivo de código versionado.
- [ ] Completé `EVIDENCE-es.md` §1–§10 y la autocomprobación de §12.

El criterio no es «el programa maneja errores». Es «puedes mostrar el caso inválido como valor,
elegir la recuperación en el punto de llamada, y mantener el proceso y la transcripción sin
cambios».

## Autocomprobación (binaria)

- [ ] Leí `move_result.h`, `game_engine.h`, `error_probe.cpp` y `main.cpp`.
- [ ] Expliqué el cambio de un enumerado más dos parámetros de salida a un solo `MoveResult`.
- [ ] Nombré los cuatro resultados, sus valores de enumerado y su orden.
- [ ] Conté 20 archivos, 11 cabeceras y 1.071 líneas de código.
- [ ] Comparé la línea base con `construction-21` y nombré los archivos cambiados y nuevos.
- [ ] Predije las cuatro tablas antes de ejecutar.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Ejecuté la sonda de errores y registré los tres resultados y el estado de salida.
- [ ] Ejecuté el camino de argumentos y registré el estado 2.
- [ ] Medí `sizeof(MoveOutcome)`, `sizeof(MoveResult)`, `game.o`, el binario y el objeto de la
      sonda.
- [ ] Escribí una política de quien llama en una copia temporal y mostré continuar, informar y
      detenerse.
- [ ] Ninguna ruta de `code/` llama a `exit`.
- [ ] Las 20 trazas coincidieron después de la prueba de cambio.
- [ ] Completé `EVIDENCE-es.md`.

Son criterios de aceptación. Si una casilla queda sin marcar,
escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §10.

## Misconcepciones que esta lección refuta

| Error conceptual | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| «Devolver una estructura es orientado a objetos.» | Es un tipo valor y una función total; no hay jerarquía ni despacho. | Di qué lleva el valor. |
| «`MoveResult` reemplaza a las excepciones.» | La línea base nunca lanzaba; devolvía un enumerado y dos parámetros de salida. | Compara las dos formas de retorno. |
| «La sonda prueba todo el contrato.» | Un tablero nuevo no puede producir `visited`. | Ejecuta una partida primero, o anota el hueco. |
| «`evaluate_move` es pura.» | Lee el tablero y el otro caballo. | Llámala consulta sobre el estado actual. |
| «Quien llama debe comprobar el valor.» | Nada fuerza una rama; el valor se puede descartar. | Añade `[[nodiscard]]` si quieres el aviso. |
| «`return 2` desde `main` es un `exit`.» | Es un retorno normal; el shell lee el estado. | Separa el retorno del salto. |
| «Un movimiento inválido es un error en el sentido de C++.» | Las reglas lo esperan; la transcripción lo registra. | Trátalo como un resultado normal. |
| «Los números del enumerado son la API.» | Los nombres son el contrato; los números son la distribución actual. | No reordenes; compara nombres. |
| «`capture` y `visited` se pueden comprobar en cualquier orden.» | La casilla del otro caballo es la excepción legal a la regla de visitadas. | Mantén capture antes que visited. |
| «`outsideBoard` se puede comprobar al final.» | Las otras comprobaciones suponen coordenadas dentro del tablero. | Comprueba los límites primero. |
| «El destino es opcional.» | `MoveResult` siempre lo lleva, también en el camino inválido. | Lee la coordenada rechazada. |
| «Una respuesta más grande no cuesta nada.» | `game.o` crece 32 bytes de texto y el valor mide 12. | Mídelo. |
| «La política pertenece al motor.» | `playTurn` es una política; la prueba de cambio es otra. | Mantén la política en el punto de llamada. |
| «Las 20 trazas prueban el contrato de error.» | Prueban la transcripción; la sonda prueba el contrato. | Ejecuta ambas. |
| «`std::expected` está disponible en C++20.» | Es C++23. | Escribe el valor a mano, o compila con C++23 y dilo. |

## Siguiente paso

`construction-23` es dueña del port a otro lenguaje. El contrato de esta lección es la parte que
debe sobrevivir a la traducción: cuatro resultados con nombre, un valor que lleva el destino y quien
llama decidiendo. Un port que reproduce las 20 trazas pero llama a `sys.exit` en medio del bucle
movió la sintaxis y perdió la decisión.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog.md) para el recorrido
completo.

Si faltaba alguna de las dos líneas base, no empieces `construction-23`. El port necesita los dos
lados: el comportamiento y el contrato.
