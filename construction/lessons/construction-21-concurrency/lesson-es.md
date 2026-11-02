# Concurrency (concurrencia)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** estado compartido frente a mensajes · **Tiempo:** 120 minutos ·
**Prerrequisitos:** construction-12, construction-20 · **Estado:** borrador
**Lectura:** ~29 minutos

## Por qué existe esta lección

`construction-20` terminó con un núcleo que podía ejecutarse sin consola. No podía ejecutar dos
partidas a la vez. La secuencia aleatoria era una variable estática de archivo:

```cpp
namespace
{
std::uint64_t randomState = 0;
}
```

Cada `GameEngine` del proceso avanzaba el mismo `randomState`. Dos hilos, dos partidas, una
variable. La transcripción de consola era correcta solo porque el programa de consola ejecutaba una
partida a la vez.

`construction-21` mueve ese estado al motor:

```cpp
class RandomSequence
{
public:
    void set_seed(std::int64_t seed);
    std::uint64_t below(std::uint64_t bound);
    int integer(int minimum, int maximum);

private:
    std::uint64_t state = 0;
};
```

La secuencia no cambia. La propiedad sí. Cada `GameEngine` posee un `RandomSequence`, así que dos
motores no pueden tocar el estado del otro. La transcripción de consola es idéntica byte a byte.

La frase que conviene retener:

> Una partida es determinista cuando el estado que muta le pertenece. Ejecutar muchas partidas a la
> vez no crea determinismo; muestra quién posee el estado.

Tres notas honestas antes de la comparación.

1. El código crece 215 líneas, de 815 en diecisiete archivos a 1.030 en dieciocho. Tres archivos
   cambiaron, uno es nuevo y catorce son idénticos byte a byte a `construction-20`. El archivo nuevo
   es `parallel_runner.cpp`, de 208 líneas; las otras siete líneas son la declaración de la clase y
   los puntos de llamada que ahora pasan por `randomSequence`.
2. El objeto del núcleo crece: el texto de `game.o` pasa de 4.138 a 4.280 bytes, y
   `sizeof(GameEngine<8, 8>)` pasa de 32 a 40. Los ocho bytes nuevos son el miembro
   `RandomSequence`. El binario crece de 10.748 a 10.869 bytes de texto, los datos siguen en 876 y
   BSS *se reduce* de 24 a 16 porque el generador global desaparece.
3. La transcripción no cambia. Las 20 trazas de referencia coinciden byte a byte. El tablero, el
   generador de movimientos, el caballo, la posición, el puerto y ambos adaptadores son los mismos
   archivos, sin cambios.

La comparación no es «la concurrencia es mejor». Es más estrecha: una pieza de estado cambia de
dueño, un segundo programa ejecuta muchas partidas a la vez y un recolector de resultados roto a
propósito muestra la diferencia entre poseer el estado y compartirlo.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar estado compartido frente a estado propio;
- detectar y eliminar una carrera manteniendo trazas deterministas.

## El concepto: quién posee el estado

### Dos estados compartidos, no uno

Esta lección contiene dos problemas distintos de estado compartido, y mezclarlos es la principal
fuente de confusión.

1. **El estado aleatorio del juego.** En `construction-20` vivía en un `randomState` estático de
   archivo. Cada partida del proceso lo usaba. `construction-21` lo mueve a un miembro
   `RandomSequence` de `GameEngine`. Esta es la corrección de propiedad, y ya está aplicada en la
   línea base que lees.
2. **El estado de resultados del ejecutor.** `parallel_runner.cpp` inicia 8 hilos, cada uno con
   2.000 partidas. Los hilos deben guardar 16.000 resultados en algún sitio. El modo `race` los
   guarda en un `std::vector` compartido mediante un `nextIndex` compartido, sin sincronización. Ese
   es el fallo que la prueba de cambio te pide reproducir y eliminar.

El primer problema está resuelto en el artefacto. El segundo es el ejercicio. El ejecutor es un
segundo programa, no forma parte de la compilación de consola.

### La propiedad elimina el primer problema

`RandomSequence` tiene un `std::uint64_t state` privado. `GameEngine` tiene un miembro
`RandomSequence`. El motor se construye por partida, así que la secuencia es por partida. Dos hilos
no pueden escribir el mismo entero porque los enteros son objetos distintos.

Por eso no hay `std::mutex` en el motor. La sincronización más barata es la que no necesitas: si dos
hilos no pueden alcanzar el mismo objeto, no hay nada que bloquear.

El LCG no cambia:

```cpp
state = state * randomMultiplier + randomIncrement;
return state % bound;
```

Mismas constantes, misma actualización, mismo módulo. Las 20 trazas son la evidencia de que la
secuencia sigue produciendo los mismos bytes. El cambio de propiedad es invisible para el programa
de consola, que es exactamente lo que debería hacer una refactorización correcta.

### El ejecutor expone el segundo problema

`run_race` preasigna el vector de resultados y entrega a cada hilo una referencia al mismo
`nextIndex`:

```cpp
int resultIndex = nextIndex;
nextIndex += 1;
results[resultIndex] = result;
```

Son tres operaciones sobre dos objetos compartidos. Dos hilos pueden leer el mismo `resultIndex`,
incrementar ambos y escribir la misma casilla. Se pierde un incremento, un resultado sobrescribe a
otro y algunas casillas conservan el `GameResult{0, 0, ""}` por defecto. El programa termina igual,
que es lo que hace peligroso el error.

El vector está preasignado a propósito. Si creciera con `push_back` sin bloqueo, la carrera también
afectaría al tamaño del vector y a su búfer interno, y la salida sería aún más difícil de leer. El
vector preasignado aísla la carrera en `nextIndex` y en las escrituras de casillas, y eso es
suficiente para la lección.

### Cuatro modos

El ejecutor tiene cuatro modos, y cada uno responde una pregunta distinta.

| Modo | Estado de resultados compartido | Sincronización | ¿Ordena por semilla? | Qué prueba |
|---|---|---|---|---|
| `sequential` | ninguno | ninguna | ya está en orden | La suma de verificación de referencia |
| `race` | un vector preasignado y un `nextIndex` | ninguna | no | El estado mutable compartido sin contrato pierde escrituras |
| `mutex` | un vector | `std::mutex` alrededor del añadido | sí | Un bloqueo restaura la completitud |
| `merge` | un vector por hilo | join y después fusión | sí | La propiedad por hilo restaura la completitud sin bloqueo |

Los dos modos corregidos no son el mismo diseño. `mutex` es estado compartido con un bloqueo.
`merge` es propiedad más una entrega: cada hilo llena su propio vector y el padre fusiona los
vectores después del join. La entrega es el «mensaje» del título de la lección. No es una cola ni un
sistema de actores; es la forma más pequeña de paso de mensajes que resuelve este problema.

### Por qué la carrera es comportamiento indefinido

`nextIndex += 1` es una lectura, una suma y una escritura. C++ no hace esos tres pasos atómicos. Dos
hilos que leen el mismo valor escriben ambos el mismo sucesor, así que un incremento desaparece.
Peor aún, dos hilos pueden recibir el mismo `resultIndex` y escribir el mismo elemento del
`std::vector` al mismo tiempo. Ambas son carreras de datos, y una carrera de datos en C++ es
comportamiento indefinido.

Comportamiento indefinido significa que el estándar no promete un número equivocado; no promete
nada. El `written=14.719` observado es un programa compilado en una máquina. Otro compilador, otro
nivel de optimización u otro planificador pueden producir un número distinto, incluido 16.000. Por
eso la lección te pide ejecutar `race` más de una vez y nunca trata una sola salida de carrera como
un hecho estable.

### Qué requiere el determinismo

Los modos corregidos coinciden con la suma secuencial porque tres condiciones se cumplen juntas.

1. **El estado por partida es poseído.** Cada partida tiene su propio `RandomSequence`, así que el
   resultado de la partida es una función de su semilla y de nada más.
2. **La recolección de resultados es completa.** Cada uno de los 16.000 resultados se escribe
   exactamente una vez. El mutex lo garantiza serializando el añadido; la fusión lo garantiza dando
   a cada hilo un vector privado.
3. **El orden es explícito.** La suma de verificación es FNV-1a, sensible al orden. `sequential`
   produce el orden de semillas por construcción. `mutex` y `merge` llaman a `sort_by_seed` antes de
   la suma. Sin ese orden, un conjunto completo de resultados seguiría produciendo una suma
   distinta.

La tercera condición es la que se olvida. El determinismo no es solo «los mismos valores»; es «los
mismos valores en el mismo orden». La corrección del motor te da los valores. El orden te da la
secuencia.

Un detalle que vale la pena notar: `merge` concatena bloques de hilos en orden de hilo, y este
ejecutor asigna las semillas en bloques (`threadIndex * gamesPerThread + gameIndex + 1`). Así que la
concatenación ya está ordenada por semilla para este reparto, y la llamada a `sort_by_seed` es
defensiva. `mutex` no tiene esa suerte: los hilos terminan en orden arbitrario, así que su orden es
obligatorio. El código ordena en ambos sitios porque el orden es un contrato, no una casualidad del
reparto actual de semillas.

### Qué no afirma esta lección

- No afirma que el motor sea seguro entre hilos. Un `GameEngine` compartido por dos hilos sigue
  siendo una carrera de datos. La corrección es un motor por hilo, no un bloqueo dentro del motor.
- No afirma que el adaptador de transcripción sea seguro entre hilos. Es una referencia; el
  ejecutor da a cada partida su propio `RecordingTranscript`, así que la pregunta no surge.
- No afirma que el paralelismo haga más rápido el juego. El ejecutor es un experimento de
  corrección; no se mide tiempo.
- No afirma que la carrera sea un patrón. Es comportamiento indefinido intencional para enseñar.
- No afirma que las 20 trazas prueben el código paralelo. Prueban la transcripción de consola; la
  suma de verificación prueba el resultado paralelo.

## Antes de empezar

- [`construction-12`](../construction-12-polymorphism/lesson-es.md) — el prerrequisito formal del
  catálogo. El motor sigue usando el puerto de `construction-20`; necesitas leer
  `virtual void f() = 0;` y `override` sin diseñar una jerarquía desde cero.
- [`construction-20`](../construction-20-hexagonal/lesson-es.md) — la línea base inmediata y el lado
  izquierdo del diff del paso 4. Su directorio `code/` es el artefacto del que parte esta lección,
  aunque el catálogo liste `construction-12`.
- [`construction-16`](../construction-16-ownership/lesson-es.md) — contexto opcional. El motor posee
  su tablero y sus caballos mediante `std::unique_ptr`; esta lección añade un valor poseído más.
- Un compilador de C++20 (`g++` o `clang++`) con `std::thread` y `std::mutex`. El ejecutor necesita
  `-pthread`. `cmake` es opcional; `run.sh` recurre a una compilación directa cuando CMake no está.
- Opcional: ThreadSanitizer (`-fsanitize=thread`). Hace visible la carrera como un informe, no solo
  como un número equivocado.

No necesitas saber programación sin bloqueos, modelos de memoria ni frameworks de actores. La línea
base es un entero privado, un `std::mutex` y un vector por hilo.

### Qué necesitas

- La línea base de concurrencia:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/game_engine.h`](code/game_engine.h),
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
  [`code/random_sequence.h`](../construction-20-hexagonal/code/random_sequence.h),
  [`code/random_sequence.cpp`](../construction-20-hexagonal/code/random_sequence.cpp),
  [`code/game_engine.h`](../construction-20-hexagonal/code/game_engine.h), y sus catorce
  compañeros en [`../construction-20-hexagonal/code/`](../construction-20-hexagonal/code/).
- Guiones de compilación y notas: [`run.sh`](run.sh), [`CMakeLists.txt`](CMakeLists.txt) y
  [`README.md`](README.md).
- Las reglas: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §12 fija las
  constantes del LCG, §6 la prioridad de movimientos, §7 los movimientos del caballo, §8 cuándo una
  casilla pasa a visitada.
- El formato de transcripción:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- Una traza de referencia: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- Esta plantilla para el aprendiz: [`EVIDENCE-es.md`](EVIDENCE-es.md).

Comprueba que los artefactos están presentes antes de empezar, desde este directorio de lección:

```bash
test -f code/main.cpp &&
test -f code/game.h &&
test -f code/game.cpp &&
test -f code/game_engine.h &&
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
test -f ../construction-20-hexagonal/code/random_sequence.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

Si falta un archivo, la comparación no tiene segundo lado. Registra el archivo faltante en
`EVIDENCE-es.md` §11 y vuelve cuando esté disponible. Un conteo contra un archivo que no has leído
no es evidencia.

La línea base de 8×8 reproduce las 20 trazas de `common/traces/` byte a byte, antes de que cambies
nada. Ese es el contrato de comportamiento, y esta lección no lo toca.

## Dos sesiones

Esta lección es larga a propósito. Divídela en dos sesiones:

- **Sesión 1 — Núcleo (Pasos 1–4, unos 70 min).** Detente después del Paso 4, cuando la propiedad
  y la carrera se entienden.
- **Sesión 2 — Extensión (Pasos 5–8, unos 45 min).** Empieza solo después de la Sesión 1. Vas a
  eliminar la carrera, comparar y registrar la evidencia.

## Paso 1 — Inventariar el estado (~15 min)

Empieza por el estado que se movió. En `construction-20`:

```bash
grep -n 'randomState' ../construction-20-hexagonal/code/random_sequence.cpp
```

```text
7:std::uint64_t randomState = 0;
12:    randomState = static_cast<std::uint64_t>(seed);
17:    randomState = randomState * randomMultiplier + randomIncrement;
18:    return randomState % bound;
```

En `construction-21`:

```bash
grep -n 'state' code/random_sequence.h code/random_sequence.cpp
```

```text
code/random_sequence.h:13:    std::uint64_t state = 0;
code/random_sequence.cpp:11:    state = static_cast<std::uint64_t>(seed);
code/random_sequence.cpp:16:    state = state * randomMultiplier + randomIncrement;
code/random_sequence.cpp:17:    return state % bound;
```

La variable es ahora un miembro privado. La única forma de tocarla es mediante los tres métodos.

Ahora los puntos de llamada en el motor:

```bash
grep -nE 'randomSequence\.' code/game_engine.h
```

```text
55:        randomSequence.set_seed(seed);
94:        int playerCellNumber = static_cast<int>(randomSequence.below(boardCellCount));
95:        int machineCellNumber = static_cast<int>(randomSequence.below(boardCellCount));
98:            machineCellNumber = static_cast<int>(randomSequence.below(boardCellCount));
108:        playerRoll = randomSequence.integer(1, 50);
109:        machineRoll = randomSequence.integer(1, 50);
112:            machineRoll = randomSequence.integer(1, 50);
166:        int startingMoveNumber = randomSequence.integer(1, moveCandidateCount);
```

Ocho puntos de llamada. Las funciones libres `random_set_seed`, `random_below` y `random_integer`
desaparecieron; el motor ahora envía los mismos mensajes a un objeto que posee.

Ahora el estado compartido del ejecutor:

```bash
grep -n 'nextIndex' code/parallel_runner.cpp
grep -n 'threadCount\|gamesPerThread\|totalGameCount' code/parallel_runner.cpp
grep -n 'lock_guard\|std::mutex' code/parallel_runner.cpp
grep -n 'perThreadResults' code/parallel_runner.cpp
```

```text
76:    int nextIndex = 0;
88:                    int resultIndex = nextIndex;
89:                    nextIndex += 1;
108:                totalGameCount, nextIndex, writtenResults.size(),
15:constexpr int threadCount = 8;
16:constexpr int gamesPerThread = 2000;
17:constexpr int totalGameCount = threadCount * gamesPerThread;
115:    std::mutex resultsMutex;
127:                    std::lock_guard<std::mutex> lock(resultsMutex);
143:    std::vector<std::vector<GameResult>> perThreadResults(threadCount);
154:                    perThreadResults[threadIndex].push_back(run_one_game(seed));
```

Lee las cuatro funciones de modo en orden: `run_sequential` en la línea 61, `run_race` en la 73,
`run_mutex` en la 112 y `run_merge` en la 141. La diferencia entre `race` y `mutex` son cuatro
líneas; la diferencia entre `mutex` y `merge` es la propiedad del vector de resultados.

Conteos de esta línea base. La regla de conteo se enuncia para que puedas reproducirla:

- Archivos: 18.
- Cabeceras: 10 (`board.h`, `console_transcript.h`, `game.h`, `game_engine.h`, `knight.h`,
  `move_generator.h`, `position.h`, `random_sequence.h`, `recording_transcript.h`,
  `transcript_port.h`).
- Unidades de traducción en la compilación de consola: 6 (`main.cpp`, `game.cpp`,
  `console_transcript.cpp`, `knight.cpp`, `position.cpp`, `random_sequence.cpp`).
- Unidades de traducción en la compilación del ejecutor: 4 (`parallel_runner.cpp`, `knight.cpp`,
  `position.cpp`, `random_sequence.cpp`).
- Unidades de traducción en la compilación de la sonda: 4 (`adapter_probe.cpp`, `knight.cpp`,
  `position.cpp`, `random_sequence.cpp`).
- Definiciones de clase completas en el núcleo: 9 (las ocho de `construction-20` más
  `RandomSequence`). El ejecutor añade el registro local `GameResult`; no forma parte del núcleo.
- Variables globales mutables en el núcleo: 0. En `construction-20`: 1 (`randomState`).
- Objetos mutables compartidos en el ejecutor: 2 en `race` (`nextIndex` y `results`), 1 en
  `mutex` (`results`), 0 en `merge`.
- Líneas de código: 1.030.
- Archivos idénticos byte a byte a `construction-20`: 14 de 17.
- Archivos cambiados: 3 (`game_engine.h`, `random_sequence.h`, `random_sequence.cpp`).
- Archivos nuevos: 1 (`parallel_runner.cpp`).

Después responde en `EVIDENCE-es.md` §2:

1. ¿Dónde estaba el estado del generador en `construction-20` y dónde está en `construction-21`?
2. ¿Cuántos bytes ocupa `RandomSequence` y qué miembro lo explica?
3. ¿Cuántos puntos de llamada `randomSequence.*` hay en `game_engine.h` y qué funciones libres
   reemplazaron?
4. ¿Qué tres archivos cambiaron, qué archivo es nuevo y qué catorce son idénticos byte a byte?
5. ¿Cuántos hilos, partidas por hilo y partidas totales usa el ejecutor?
6. ¿Qué modos comparten un vector de resultados y qué modo da uno propio a cada hilo?
7. ¿Qué modo no tiene sincronización y qué dos objetos se comparten en él?
8. ¿Qué modos ordenan por semilla antes de la suma y por qué la suma necesita ese orden?
9. ¿Qué centinela usa el escaneo de la carrera y por qué es seguro para estas semillas?
10. ¿Qué estado salió de BSS y cuántos bytes perdió BSS?

La pregunta 9 tiene una respuesta precisa: el escaneo cuenta resultados cuyo `seed` no es 0, y el
ejecutor asigna semillas de 1 a 16.000, así que 0 es una marca segura de «no escrito». Si las
semillas pudieran ser 0, el escaneo contaría de menos. La pregunta 10: `randomState` salió de
`.bss`, y BSS pasó de 24 a 16 bytes.

## Paso 2 — Predecir antes de ejecutar (~15 min)

Escribe estas predicciones antes de ejecutar nada. No mires las respuestas del paso 3 primero.

### 2.1 La tabla de estructura

| Pregunta | Mi predicción |
|---|---|
| Archivos en `construction-20` | |
| Archivos aquí | |
| Cabeceras aquí | |
| Unidades de traducción en la compilación de consola | |
| Unidades de traducción en la compilación del ejecutor | |
| Líneas de código en `construction-20` | |
| Líneas de código aquí | |
| Archivos cambiados | |
| Archivos nuevos | |
| Archivos idénticos byte a byte | |
| Variables globales mutables en el núcleo, antes y después | |
| Objetos mutables compartidos en el ejecutor | |

La fila fácil de equivocar es «archivos cambiados». El motor parece distinto, pero solo cambiaron
sus puntos de llamada; el puerto, los adaptadores, el tablero y los caballos están intactos.

### 2.2 La tabla de la carrera

| Pregunta | Mi predicción |
|---|---|
| `written` de `race`, de 16.000 | |
| `next_index` de `race` | |
| ¿`written` es igual a `next_index`? | |
| ¿La suma de `race` es estable entre ejecuciones? | |
| Suma de `sequential` | |
| Suma de `mutex` | |
| Suma de `merge` | |
| ¿Qué modo no ordena por semilla? | |
| ¿Qué modos coinciden con la suma secuencial? | |

Predice `written` por debajo de 16.000. Si predices 16.000, estás suponiendo que el incremento es
atómico, y no lo es. Predice la suma como inestable: la carrera cambia el conjunto de resultados y
su orden.

### 2.3 La tabla de tamaños y texto

| Pregunta | Mi predicción |
|---|---|
| `sizeof(GameEngine<8, 8>)`, antes y después | |
| `sizeof(RandomSequence)` | |
| Texto de `game.o`, antes y después | |
| Texto del binario, antes y después | |
| Datos del binario, antes y después | |
| BSS del binario, antes y después | |
| Semillas que coinciden, de 20 | |

Predice el motor en 40 bytes: el motor de `construction-20` tiene 32, y el miembro
`RandomSequence` tiene 8. Predice BSS en 16: el `randomState` global tenía 8 bytes y desapareció.

### 2.4 La tabla de eliminación

| Pregunta | Mi predicción |
|---|---|
| La corrección que aplicaré | |
| Archivos que editaré | |
| Líneas que añadiré | |
| `written` después de la corrección | |
| Suma después de la corrección | |
| Semillas que coinciden, de 20, después de la corrección | |
| Resultado de compilación con `-Wall -Wextra -Werror` | |

Predice un archivo: `parallel_runner.cpp`. El motor, la transcripción y las trazas no deberían
moverse.

## Paso 3 — Ejecutar la línea base (~20 min)

### 3.1 Las trazas

Primero las trazas, sin modificar:

```bash
mkdir -p /tmp/concurrency-test
for trace in ../../../common/traces/seed-*.txt; do
    traceName="$(basename "${trace}" .txt)"
    seedText="${traceName#seed-}"
    seedNumber="$((10#${seedText}))"
    ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

Un bucle silencioso son 20 de 20. Si `run.sh` no puede compilar, compila una vez a mano:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
    -o /tmp/concurrency-test/concurrency-baseline \
    code/main.cpp code/game.cpp code/console_transcript.cpp \
    code/knight.cpp code/position.cpp code/random_sequence.cpp
```

Después ejecuta el mismo bucle con `/tmp/concurrency-test/concurrency-baseline` en lugar de
`./run.sh`.

### 3.2 Los cuatro modos

Compila el ejecutor:

```bash
c++ -std=c++20 -O2 -pthread -Icode -o /tmp/concurrency-test/parallel-runner \
    code/parallel_runner.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
```

Ejecuta cada modo:

```bash
for mode in sequential race mutex merge; do
    /tmp/concurrency-test/parallel-runner --mode "$mode"
done
```

La salida en esta máquina:

```text
mode=sequential games=16000 written=16000 checksum=2826026307342797653
mode=race games=16000 next_index=14719 written=14719 checksum=17876456234189950703
mode=mutex games=16000 written=16000 checksum=2826026307342797653
mode=merge games=16000 written=16000 checksum=2826026307342797653
```

La línea de `race` es una muestra. Tu `next_index`, tu `written` y tu suma serán distintos.

| Modo | Estado de resultados compartido | Sincronización | Escritas | Suma de verificación |
|---|---|---|---|---|
| `sequential` | ninguno | ninguna | 16.000 | 2826026307342797653 |
| `race` | un vector preasignado y `nextIndex` | ninguna | 14.719–14.821 | varía |
| `mutex` | un vector | `std::mutex` | 16.000 | 2826026307342797653 |
| `merge` | un vector por hilo | join y después fusión | 16.000 | 2826026307342797653 |

Los modos corregidos coinciden exactamente con la suma secuencial. La carrera no, y el número de
escrituras se queda corto.

### 3.3 Repetir la carrera

Una salida de carrera es una anécdota. Ejecútala cuatro veces:

```bash
for run in 1 2 3 4; do
    /tmp/concurrency-test/parallel-runner --mode race
done
```

Cuatro ejecuciones separadas en esta máquina:

| Ejecución | `next_index` | `written` | Suma de verificación |
|---|---:|---:|---:|
| 1 | 14.719 | 14.719 | 17.876.456.234.189.950.703 |
| 2 | 14.821 | 14.821 | 12.745.221.390.738.065.136 |
| 3 | 14.778 | 14.778 | 13.488.474.134.281.646.459 |
| 4 | 14.766 | 14.766 | 8.624.756.145.285.332.907 |

La tabla conserva el orden en que se registraron las ejecuciones; lo importante es que ambas
columnas se mueven. El emparejamiento es ilustrativo, no una propiedad del programa.

Dos observaciones que explicar.

1. **`written` es igual a `next_index` en estas ejecuciones.** El bucle escribe cada índice que
   producen los incrementos que sobreviven, y el escaneo cuenta semillas distintas de cero. Un hilo
   que pierde un incremento escribe igualmente en el índice que leyó, así que la casilla no queda
   vacía; queda sobrescrita. Esta igualdad es una observación de este programa compilado bajo
   comportamiento indefinido, no una garantía.
2. **La suma se mueve por dos razones.** Faltan resultados y la carrera no ordena por semilla.
   Aunque `written` fuera 16.000, el orden del vector no sería el de las semillas, así que la suma
   seguiría difiriendo de la secuencial. El número de escrituras es la evidencia directa de las
   escrituras perdidas; la suma es evidencia de un conjunto de resultados distinto *y* de un orden
   distinto.

Ese segundo punto importa para la prueba de cambio. Una corrección que solo completara las
escrituras, sin ordenar, seguiría fallando la suma. La aceptación es «la suma paralela coincide con
la secuencial», y eso requiere completitud y orden.

### 3.4 Tamaños

La sonda va en el directorio temporal, nunca en los archivos versionados:

```bash
cat > /tmp/concurrency-test/sizes.cpp <<'EOF'
#include "game_engine.h"
#include "random_sequence.h"

#include <cstdio>

int main()
{
    std::printf("sizeof(GameEngine<8,8>)=%zu\n", sizeof(GameEngine<8, 8>));
    std::printf("sizeof(RandomSequence)=%zu\n", sizeof(RandomSequence));
    return 0;
}
EOF
c++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
    -o /tmp/concurrency-test/sizes /tmp/concurrency-test/sizes.cpp \
    code/knight.cpp code/position.cpp code/random_sequence.cpp
/tmp/concurrency-test/sizes
```

La salida en esta plataforma:

```text
sizeof(GameEngine<8,8>)=40
sizeof(RandomSequence)=8
```

El motor creció 8 bytes, exactamente el miembro `RandomSequence`. La secuencia en sí es un entero
de 64 bits.

### 3.5 Objeto y binario

```bash
c++ -std=c++20 -O2 -Icode -c code/game.cpp -o /tmp/concurrency-test/game21.o
c++ -std=c++20 -O2 -I../construction-20-hexagonal/code \
    -c ../construction-20-hexagonal/code/game.cpp -o /tmp/concurrency-test/game20.o
size /tmp/concurrency-test/game20.o /tmp/concurrency-test/game21.o
```

```text
   text    data     bss     dec     hex filename
   4138       0       0    4138    102a /tmp/concurrency-test/game20.o
   4280       0       0    4280    10b8 /tmp/concurrency-test/game21.o
```

El objeto del núcleo crece 142 bytes de texto: los métodos de la clase y los puntos de llamada que
ahora pasan `this`.

Ahora los binarios enlazados:

```bash
c++ -std=c++20 -O2 -Icode -o /tmp/concurrency-test/bin21 \
    code/main.cpp code/game.cpp code/console_transcript.cpp \
    code/knight.cpp code/position.cpp code/random_sequence.cpp
c++ -std=c++20 -O2 -I../construction-20-hexagonal/code \
    -o /tmp/concurrency-test/bin20 \
    ../construction-20-hexagonal/code/main.cpp \
    ../construction-20-hexagonal/code/game.cpp \
    ../construction-20-hexagonal/code/console_transcript.cpp \
    ../construction-20-hexagonal/code/knight.cpp \
    ../construction-20-hexagonal/code/position.cpp \
    ../construction-20-hexagonal/code/random_sequence.cpp
size /tmp/concurrency-test/bin20 /tmp/concurrency-test/bin21
```

```text
   text    data     bss     dec     hex filename
  10748     876      24   11648    2d80 /tmp/concurrency-test/bin20
  10869     876      16   11761    2df1 /tmp/concurrency-test/bin21
```

El texto crece 121 bytes, los datos no se mueven y BSS se reduce 8. El último número es el
generador global saliendo del binario: `randomState` vivía en `.bss`, y ahora el estado vive dentro
de un `GameEngine` asignado en el montón.

Si el primer diff de trazas no está vacío, detente. Esa es una línea base distinta de la que
describe esta lección; regístrala en `EVIDENCE-es.md` §11 y no ajustes la predicción para que
coincida.

### 3.6 Opcional: ver la carrera como informe

ThreadSanitizer convierte el número equivocado en un diagnóstico:

```bash
c++ -std=c++20 -O1 -g -fsanitize=thread -pthread -Icode \
    -o /tmp/concurrency-test/race-tsan \
    code/parallel_runner.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
/tmp/concurrency-test/race-tsan --mode race
```

Busca `WARNING: ThreadSanitizer: data race`. Una pila apunta a la lectura o escritura de
`nextIndex`; la otra apunta a otro hilo haciendo lo mismo. TSan ralentiza la ejecución y cambia el
entrelazado, así que el número de `written` no es comparable con la compilación optimizada. El
informe es la evidencia, no el número.

Si TSan no está instalado, omite este paso y regístralo en `EVIDENCE-es.md` §11. No instales un
conjunto de herramientas solo por esta lección.

## Paso 4 — Comparar: qué prueba la propiedad (~20 min)

Abre `random_sequence.h`, `random_sequence.cpp`, `game_engine.h` y `parallel_runner.cpp`. Cinco
hechos, leídos contra las mediciones.

1. **El núcleo no tiene un generador global mutable.** `randomState` desapareció. El único `state`
   es un miembro privado de `RandomSequence`, y la única forma de alcanzarlo es mediante
   `set_seed`, `below` e `integer`.
2. **La secuencia no cambia.** Las constantes, la multiplicación-suma y el módulo son los mismos.
   Las 20 trazas son la comprobación: si la secuencia hubiera cambiado, la primera traza divergiría.
3. **Cada motor posee su secuencia.** `sizeof(GameEngine<8, 8>)` es 40, antes 32, y
   `sizeof(RandomSequence)` es 8. El motor lleva el estado, así que dos motores llevan dos estados.
4. **BSS se reduce.** La BSS del binario pasa de 24 a 16 bytes. El global tenía 8 bytes; el miembro
   vive en el motor, que el ejecutor asigna por partida. Los datos siguen en 876.
5. **La carrera del ejecutor es un segundo estado compartido, separado.** Eliminar el generador
   global no eliminó la necesidad de pensar en la recolección de resultados. El motor es
   determinista por partida; el ejecutor todavía tiene que recolectar las partidas correctamente.

### 4.1 El diff exacto

Tres archivos cambiaron y uno es nuevo. El diff contra `construction-20`:

```bash
diff -u ../construction-20-hexagonal/code/random_sequence.h code/random_sequence.h
diff -u ../construction-20-hexagonal/code/random_sequence.cpp code/random_sequence.cpp
diff -u ../construction-20-hexagonal/code/game_engine.h code/game_engine.h
```

| Archivo | Añadidas | Eliminadas | Neto | Qué cambió |
|---|---:|---:|---:|---|
| `game_engine.h` | 9 | 8 | +1 | Ocho puntos de llamada pasan por `randomSequence`; se añade un miembro |
| `random_sequence.h` | 10 | 3 | +7 | Las funciones libres pasan a una clase con estado privado |
| `random_sequence.cpp` | 7 | 8 | -1 | `randomState` sale del espacio anónimo; los métodos usan `state` |
| `parallel_runner.cpp` | 208 | 0 | +208 | Nuevo segundo programa con cuatro modos |
| Total | 234 | 19 | +215 | Tres cambiados, uno nuevo, catorce idénticos |

Los catorce archivos idénticos son `board.h`, `move_generator.h`, `knight.h`, `knight.cpp`,
`position.h`, `position.cpp`, `transcript_port.h`, `console_transcript.h`,
`console_transcript.cpp`, `recording_transcript.h`, `game.h`, `main.cpp`, `game.cpp` y
`adapter_probe.cpp`. Las reglas, el formato de transcripción y el bucle del juego están intactos.

### 4.2 Qué no cambió

- El LCG. Mismo multiplicador, mismo incremento, mismo módulo, mismo manejo de la semilla.
- La transcripción. Las 20 trazas coinciden byte a byte.
- Las reglas. Orden de movimientos, captura, trampa, empate y límite de 200 movimientos son los
  mismos.
- El diseño del tablero. `board.h` es idéntico byte a byte; `sizeof(Board<8, 8>)` sigue en 256.
- El generador de movimientos. `move_generator.h` es idéntico byte a byte.
- El bucle de intentos. `playTurn` tiene las mismas cuatro salidas y las mismas dos condiciones de
  parada.
- La interfaz pública del juego. `game.h` y `run_game` conservan sus firmas.
- El número de trazas. 20 de 20 antes y después.

### 4.3 Qué cuesta

- 215 líneas nuevas y un archivo nuevo, la mayoría del ejecutor.
- 8 bytes más por motor para el miembro `RandomSequence`.
- 142 bytes más de texto en `game.o` y 121 bytes más de texto en el binario enlazado.
- Una regla de vida nueva dentro del motor: la secuencia se construye con el motor y se destruye con
  él. No hay registro compartido ni reinicio entre partidas.
- Una carga de lectura nueva: para saber cómo obtiene el azar una partida, el lector sigue
  `GameEngine::randomSequence` en lugar de una función libre.
- Un segundo programa que compilar y ejecutar. La compilación de consola no enlaza el ejecutor.

Nada de esto es un defecto. Es el precio de mover el estado al objeto que lo usa, y el registro debe
anotarlo junto al beneficio.

### 4.4 La tabla de estado

Esta tabla es el corazón de la comparación. Léela contra los cuatro modos.

| Estado | Dueño en `construction-20` | Dueño en `construction-21` | ¿Compartido entre partidas? | Sincronización |
|---|---|---|---|---|
| Estado del generador | un `randomState` estático de archivo | un `RandomSequence` por motor | No | Ninguna necesaria |
| Tablero | un `Board` por motor | un `Board` por motor | No | Ninguna necesaria |
| Transcripción | un adaptador por partida | un adaptador por partida | No | Ninguna necesaria |
| Vector de resultados | — | uno por proceso en `race` y `mutex`; uno por hilo en `merge` | Sí en `race` y `mutex` | Ninguna en `race`; mutex en `mutex`; join en `merge` |
| `nextIndex` | — | uno por proceso en `race` | Sí | Ninguna, intencional |

Las tres primeras filas son poseídas. Las dos últimas son el problema del ejecutor. El título de la
lección trata de la diferencia: el estado poseído no necesita protocolo; el estado compartido sí.

### 4.5 Por qué coinciden los modos corregidos

Recorre las tres condiciones de la sección de concepto contra el código.

- `run_one_game` construye un `RecordingTranscript` nuevo y un `GameEngine<8, 8>` nuevo por llamada.
  El motor construye un `RandomSequence` nuevo. Ningún estado sobrevive a una partida.
- `run_mutex` añade bajo `std::lock_guard<std::mutex>`, así que el vector tiene exactamente 16.000
  elementos después del join. Después llama a `sort_by_seed`.
- `run_merge` da a cada hilo su propio vector, fusiona después del join y llama a `sort_by_seed`.
- `result_checksum` pliega `seed`, `moveCount` y `winner` en el orden del vector con FNV-1a. No
  ordena. El llamador es dueño del orden.

El modo secuencial empuja las semillas 1 a 16.000 en orden, así que su vector ya está en el orden
que la suma espera. Los modos paralelos corregidos reproducen ese orden explícitamente. Por eso las
tres sumas son iguales.

## Paso 5 — Prueba de cambio: eliminar la carrera (~25 min)

La aceptación nombra la prueba de cambio: **reproducir una carrera de escrituras perdidas,
eliminarla con un mutex o con resultados por hilo, y mostrar que la suma paralela coincide con la
suma secuencial mientras las 20 trazas siguen pasando.**

Ya reprodujiste la carrera en el paso 3. Ahora elimínala, dos veces si puedes: una con un bloqueo y
otra con propiedad por hilo. Las dos correcciones enseñan cosas distintas.

### 5.1 Haz una copia temporal

Nunca edites el directorio `code/` versionado. Cópialo:

```bash
rm -rf /tmp/concurrency-test/change
cp -r code /tmp/concurrency-test/change
```

### 5.2 Corrección A: un mutex alrededor del añadido

En `/tmp/concurrency-test/change/parallel_runner.cpp`, reemplaza el cuerpo de `run_race`. El
original empieza con un vector preasignado y un índice compartido:

```cpp
    std::vector<GameResult> results(totalGameCount);
    int nextIndex = 0;
```

Reemplaza esas dos líneas por un vector que crece y un mutex:

```cpp
    std::vector<GameResult> results;
    results.reserve(totalGameCount);
    std::mutex resultsMutex;
```

Cambia la captura de la lambda de `[&results, &nextIndex, threadIndex]` a
`[&results, &resultsMutex, threadIndex]`, y reemplaza las tres líneas del índice compartido:

```cpp
                    int resultIndex = nextIndex;
                    nextIndex += 1;
                    results[resultIndex] = result;
```

por un añadido protegido:

```cpp
                    std::lock_guard<std::mutex> lock(resultsMutex);
                    results.push_back(result);
```

Después del join, añade el orden y reemplaza el `printf` de la carrera:

```cpp
    sort_by_seed(results);
    std::printf("mode=race games=%d written=%zu checksum=%llu\n", totalGameCount,
                results.size(), static_cast<unsigned long long>(result_checksum(results)));
```

Borra el escaneo `writtenResults`; existía solo para contar los huecos. `run_race` es ahora casi
idéntico a `run_mutex`, que es el punto: el bloqueo convierte el recolector roto en el que funciona.

### 5.3 Corrección B: un vector por hilo

Si quieres la forma de paso de mensajes, reemplaza el cuerpo de `run_race` por el cuerpo de
`run_merge`:

```cpp
    std::vector<std::vector<GameResult>> perThreadResults(threadCount);
    std::vector<std::thread> threads;
    threads.reserve(threadCount);
    for (int threadIndex = 0; threadIndex < threadCount; threadIndex += 1)
    {
        threads.emplace_back(
            [&perThreadResults, threadIndex]
            {
                for (int gameIndex = 0; gameIndex < gamesPerThread; gameIndex += 1)
                {
                    int seed = threadIndex * gamesPerThread + gameIndex + 1;
                    perThreadResults[threadIndex].push_back(run_one_game(seed));
                }
            });
    }
    for (std::thread &thread : threads)
    {
        thread.join();
    }

    std::vector<GameResult> results;
    results.reserve(totalGameCount);
    for (const std::vector<GameResult> &threadResults : perThreadResults)
    {
        results.insert(results.end(), threadResults.begin(), threadResults.end());
    }
    sort_by_seed(results);
```

Después imprime la misma línea de tres campos que en la corrección A. Cada hilo posee su vector; el
join es la entrega; la fusión es la reducción. No aparece ningún bloqueo.

### 5.4 Compilar y comparar

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 -pthread \
    -I/tmp/concurrency-test/change \
    -o /tmp/concurrency-test/change-runner \
    /tmp/concurrency-test/change/parallel_runner.cpp \
    /tmp/concurrency-test/change/knight.cpp \
    /tmp/concurrency-test/change/position.cpp \
    /tmp/concurrency-test/change/random_sequence.cpp
/tmp/concurrency-test/change-runner --mode race
```

Salida esperada para cualquiera de las dos correcciones:

```text
mode=race games=16000 written=16000 checksum=2826026307342797653
```

Ejecútalo más de una vez. La suma no debe moverse. Después ejecuta las 20 trazas en el programa de
consola; la corrección solo tocó el ejecutor:

```bash
for trace in ../../../common/traces/seed-*.txt; do
    traceName="$(basename "${trace}" .txt)"
    seedText="${traceName#seed-}"
    seedNumber="$((10#${seedText}))"
    ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

El bucle es silencioso: 20 de 20. La corrección tocó un archivo y ningún código del juego.

### 5.5 Qué prueba la prueba de cambio y qué no

Prueba cinco cosas.

1. Las escrituras perdidas están en la recolección de resultados, no en la lógica del juego. El
   motor y la transcripción no cambiaron, y la suma corregida coincide con la secuencial.
2. El generador por motor es lo que hace que el resultado de cada partida sea independiente de la
   planificación. Sin él, un bloqueo alrededor del vector seguiría produciendo resultados de partida
   distintos.
3. La coincidencia de la suma necesita resultados completos *y* orden por semilla. El orden forma
   parte de la corrección, no es decoración.
4. La corrección es local: una función en un archivo. La compilación de consola, el puerto y los
   adaptadores están intactos.
5. Ambas formas funcionan. Un bloqueo y un vector por hilo producen la misma suma; difieren en
   coste, no en resultado.

No prueba que el motor sea seguro entre hilos cuando se comparte. Un `GameEngine` usado por dos
hilos sigue siendo una carrera de datos. No prueba que el mutex sea gratis: el añadido se serializa,
y el cálculo de la partida sigue ejecutándose en paralelo. No prueba que la fusión sea más rápida:
evita el bloqueo pero asigna vectores por hilo y los copia al final. No prueba que la carrera sea
reproducible; es comportamiento indefinido, y otra compilación puede ocultarla.

Una variante que vale la pena nombrar: convertir `nextIndex` en un `std::atomic<int>` con
`fetch_add` da a cada hilo un índice único, así que las escrituras se vuelven disjuntas y las
escrituras perdidas desaparecen. Pero el orden sigue siendo arbitrario, así que la suma sigue
difiriendo hasta que ordenes. El atómico corrige el contador, no el contrato de orden, y no se
generaliza a un vector que crece. No lo confundas con una corrección completa.

## Paso 6 — Comparar con `construction-20` (~10 min)

| Métrica | `construction-20` | `construction-21` | Diferencia |
|---|---:|---:|---:|
| Archivos | 17 | 18 | +1 |
| Cabeceras | 10 | 10 | 0 |
| Unidades de traducción (compilación de consola) | 6 | 6 | 0 |
| Líneas de código | 815 | 1.030 | +215 |
| Archivos cambiados | — | 3 | +3 |
| Archivos nuevos | — | 1 | +1 |
| Archivos idénticos byte a byte | — | 14 de 17 | — |
| Generador global mutable | 1 | 0 | -1 |
| `sizeof(GameEngine<8, 8>)` | 32 | 40 | +8 |
| `sizeof(RandomSequence)` | — | 8 | +8 |
| Texto de `game.o` | 4.138 | 4.280 | +142 |
| Texto del binario | 10.748 | 10.869 | +121 |
| Datos del binario | 876 | 876 | 0 |
| BSS del binario | 24 | 16 | -8 |
| Trazas | 20 de 20 | 20 de 20 | 0 |
| Modos paralelos | 0 | 4 | +4 |

### 6.1 Qué compra

- El núcleo no tiene estado global mutable. El estado del generador pertenece al motor que lo usa.
- Dos motores pueden ejecutarse en dos hilos sin coordinar su azar. La secuencia es por partida, no
  por proceso.
- Los modos paralelos corregidos reproducen exactamente la suma secuencial. El resultado de la
  partida es independiente del planificador.
- BSS se reduce 8 bytes porque el global sale del binario.
- El ejecutor separa las dos preguntas: estado del juego (corregido por propiedad) y recolección de
  resultados (corregida por un bloqueo o una entrega).
- Las 20 trazas siguen pasando, así que el comportamiento de consola no cambió.
- La lección deja un diagnóstico reutilizable: `written` frente a `totalGameCount`, más una suma
  sensible tanto a los valores como al orden.

### 6.2 Qué cuesta

- 215 líneas nuevas y un archivo nuevo, la mayoría del ejecutor.
- 8 bytes más por motor para la secuencia.
- 142 bytes más de texto en `game.o` y 121 más en el binario.
- El motor no es reentrante. Un motor por hilo es el contrato, y el tipo no lo impone.
- El orden de resultados debe hacerse explícito. Un vector completo pero sin ordenar sigue fallando
  la suma.
- El adaptador de transcripción es por partida. Un adaptador compartido necesitaría su propia
  sincronización, que esta línea base evita.
- El modo carrera es comportamiento indefinido y no debe copiarse a código de producción.

### 6.3 La brecha: la propiedad no es toda la respuesta

La línea base no es «el programa es concurrente». Es «el estado del juego es poseído, y el ejecutor
muestra qué pasa cuando el estado de resultados no lo es». Quedan cinco acoplamientos, y vale la
pena nombrarlos.

1. **El motor no es seguro entre hilos cuando se comparte.** El miembro `RandomSequence` hace
   privado el estado por partida, no reentrante el motor completo. Dos hilos llamando a `run` sobre
   el mismo motor competirían por el tablero, los caballos y la secuencia.
2. **El puerto de transcripción es una referencia.** Un adaptador compartido necesitaría un bloqueo
   o una cola. El ejecutor evita la pregunta construyendo un `RecordingTranscript` por partida.
3. **La recolección de resultados es un diseño separado.** Los cuatro modos del ejecutor son un
   recurso didáctico, no un planificador general. Un sistema real necesitaría una cola,
   contrapresión y un protocolo de cierre.
4. **La suma es sensible al orden.** «Determinista» incluye el orden de fusión. Un cambio futuro que
   elimine `sort_by_seed` rompería la comparación sin romper ninguna partida individual.
5. **Las trazas no prueban los hilos.** Prueban la transcripción de consola. La afirmación paralela
   se apoya en la suma, y la suma se apoya en el mismo cálculo por partida.

La frase de aceptación precisa es más estrecha que «el programa es concurrente»:

> La persona que aprende puede reproducir una carrera de escrituras perdidas, eliminarla con un
> mutex o con resultados por hilo, y mostrar que la suma paralela coincide con la suma secuencial
> mientras las 20 trazas siguen pasando.

Carrera reproducida, carrera eliminada, suma igual, trazas intactas. Nada sobre código sin bloqueos,
rendimiento o un framework general de actores.

## Paso 7 — Explicar (~5 min)

Escribe `EVIDENCE-es.md` §8. Dos partes.

**Primero, una frase.** Di qué cambió entre las dos líneas base y qué decide el cambio, sin usar
las palabras *hilo*, *mutex*, *bloqueo*, *carrera*, *concurrencia*, *concurrente*, *paralelo*,
*atómico*, *compartido*, *mejor*, *peor*, *limpio* ni *desacoplado*. Si tu frase necesita una de
ellas, has nombrado el mecanismo en lugar de la decisión.

**Segundo, un párrafo.** Responde:

- Qué cambió: el estado del generador pasó de una variable estática de archivo a un miembro privado
  de `RandomSequence`, poseído por cada `GameEngine`.
- Qué quedó igual: las constantes del LCG, la actualización, el módulo, la transcripción, el
  tablero, el bucle y las 20 trazas.
- Qué añade el ejecutor: cuatro modos; `race` es el fallo; `mutex` y `merge` son las correcciones.
- Qué es la carrera: dos objetos compartidos (`nextIndex` y el vector preasignado) sin
  sincronización; escrituras perdidas; comportamiento indefinido.
- Qué son las correcciones: un bloqueo alrededor del añadido, o un vector por hilo fusionado después
  del join; ambas ordenan por semilla antes de la suma.
- Qué requiere el determinismo: propiedad del estado por partida más un orden de resultados
  determinista.
- Qué prueban los números: BSS de 24 a 16 (el global desapareció), motor de 32 a 40 (el miembro se
  añadió), `game.o` de 4.138 a 4.280, binario de 10.748 a 10.869 de texto, datos iguales, trazas 20
  de 20.
- Qué prueba la suma: `mutex` y `merge` igualan a `sequential`; `race` no.
- Qué prueban las trazas: la transcripción de consola no cambió; no prueban los hilos.
- Qué le toca a la siguiente lección: contratos de error, `construction-22`.

Una respuesta fuerte mantiene separadas cuatro cosas: **quién posee el estado del juego** (el
motor), **quién posee el estado de resultados** (el ejecutor, un vector o uno por hilo),
**cuál es el contrato de orden** (ordenar por semilla antes de la suma) y **qué puede ver una
ejecución** (la transcripción y la suma). Una respuesta débil dice «el código es seguro entre
hilos», lo cual es falso: el motor solo es seguro cuando cada hilo tiene el suyo.

## Paso 8 — Comprobación de aceptación y registro (~5 min)

El criterio de aceptación de esta lección es: **carrera detectada y eliminada; determinismo
conservado; 20 trazas pasan.**

Comprueba tu registro contra él:

- [ ] Localicé el `randomState` global en `construction-20` y el miembro `state` privado aquí.
- [ ] Conté los ocho puntos de llamada `randomSequence.*` en `game_engine.h`.
- [ ] Nombré los tres archivos cambiados, el ejecutor nuevo y los catorce archivos idénticos.
- [ ] Predije las tablas de estructura, carrera, tamaños y eliminación antes de medir.
- [ ] Reproduje las 20 trazas en la línea base de consola sin modificar.
- [ ] Ejecuté `sequential`, `race`, `mutex` y `merge` y registré escritas y suma.
- [ ] Ejecuté `race` al menos cuatro veces y vi moverse el resultado.
- [ ] Expliqué por qué `written` es igual a `next_index` en las ejecuciones observadas, con la
      advertencia de comportamiento indefinido.
- [ ] Expliqué por qué la suma de la carrera difiere por dos razones: faltan resultados y el orden
      no está ordenado.
- [ ] Medí `sizeof(GameEngine<8, 8>)` en 40 y `sizeof(RandomSequence)` en 8.
- [ ] Medí el texto de `game.o` en 4.280 y el binario en 10.869 de texto, 876 de datos y 16 de BSS.
- [ ] Eliminé la carrera en una copia temporal con un mutex o con resultados por hilo.
- [ ] La ejecución corregida imprimió `written=16000` y `checksum=2826026307342797653`.
- [ ] La ejecución corregida fue estable en al menos dos ejecuciones.
- [ ] Las 20 trazas coincidieron después de la corrección.
- [ ] La compilación temporal pasó con `-Wall -Wextra -Werror`.
- [ ] Nombré qué no garantiza el motor: un motor compartido por dos hilos sigue siendo una carrera.
- [ ] Nombré el contrato de orden: `sort_by_seed` antes de la suma.
- [ ] No edité ningún archivo de código versionado.
- [ ] Completé `EVIDENCE-es.md` §1–§10 y la autocomprobación de §12.

El criterio no es «el programa es concurrente». Es «puedes mostrar una escritura perdida, eliminarla
y mantener el resultado determinista sin cambiar la transcripción».

## Autocomprobación (binaria)

- [ ] Leí `random_sequence.h`, `random_sequence.cpp`, `game_engine.h` y `parallel_runner.cpp`.
- [ ] Expliqué el cambio de propiedad de un `randomState` estático de archivo a un `RandomSequence`
      por motor.
- [ ] Conté 8 puntos de llamada `randomSequence.*` y 1 `nextIndex` intencional.
- [ ] Comparé la línea base con `construction-20` y nombré los tres archivos cambiados y el ejecutor
      nuevo.
- [ ] Predije las tablas de estructura, carrera, tamaños y eliminación antes de ejecutar.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Ejecuté los cuatro modos y registré escritas y suma para cada uno.
- [ ] Ejecuté `race` más de una vez y expliqué la variación.
- [ ] Medí el motor en 40 bytes y `RandomSequence` en 8.
- [ ] Medí `game.o` en 4.280 bytes de texto, el binario en 10.869 de texto y 876 de datos, y BSS en
      16.
- [ ] Eliminé la carrera en una copia temporal, con un mutex y, si fue posible, con resultados por
      hilo.
- [ ] La suma corregida coincidió con la secuencial y se mantuvo estable entre ejecuciones.
- [ ] Las 20 trazas coincidieron después de la corrección.
- [ ] Nombré el contrato de orden y la no reentrancia del motor.
- [ ] Completé `EVIDENCE-es.md`.

Estos son criterios de aceptación. Si una casilla
queda sin marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §10.

## Misconcepciones que esta lección refuta

| Error | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| «El generador global estaba bien porque cada partida fija la semilla.» | Dos hilos fijan y avanzan la misma variable; el entrelazado corrompe ambas. | Da a cada motor su propio `RandomSequence`. |
| «La concurrencia significa añadir hilos al programa de consola.» | El programa de consola es secuencial; el ejecutor es un segundo programa. | Mantén el experimento separado. |
| «La carrera está en el motor del juego.» | El generador del motor es por motor; la carrera está en el `nextIndex` y el vector del ejecutor. | Lee `parallel_runner.cpp`. |
| «`written` debería ser 16.000 incluso con la carrera.» | Las actualizaciones perdidas dejan huecos; el escaneo cuenta semillas distintas de cero. | Mide `written` y `next_index`. |
| «La suma distinta prueba escrituras perdidas.» | La carrera tampoco ordena, así que el orden difiere incluso con escrituras completas. | Usa el número de escrituras como evidencia directa. |
| «La carrera es reproducible.» | Es comportamiento indefinido; el entrelazado varía. | Ejecútala más de una vez. |
| «Hacer `nextIndex` atómico es suficiente.» | Elimina índices perdidos, pero el orden sigue siendo arbitrario; todavía hay que ordenar. | Corrige la recolección y el orden. |
| «Un mutex vuelve secuencial todo el programa.» | El cálculo de la partida sigue en paralelo; solo se serializa el añadido. | Mide si te importa el tiempo. |
| «La fusión siempre es más rápida que el mutex.» | Evita el bloqueo pero asigna vectores por hilo y copia al final. | Mide ambos. |
| «Las 20 trazas prueban el código paralelo.» | Ejercitan el programa de consola, una partida a la vez. | Usa la suma para la afirmación paralela. |
| «El motor ya es seguro entre hilos.» | Un motor compartido por dos hilos sigue siendo una carrera. | Un motor por hilo. |
| «`sizeof(GameEngine)` creció por el hilo.» | Creció 8 bytes por el miembro `RandomSequence`. | Mide `sizeof(RandomSequence)`. |
| «BSS debería crecer con los hilos.» | El generador global salió de BSS; el miembro vive en el motor asignado en el montón. | Mide BSS. |
| «El adaptador de transcripción es seguro entre hilos.» | Es una referencia; un adaptador compartido necesitaría sincronización. | Un adaptador por partida. |
| «La carrera es un patrón que reutilizar.» | Es comportamiento indefinido intencional para la lección. | Usa un mutex o resultados por hilo. |
| «El orden de la fusión es obligatorio porque la concatenación es aleatoria.» | Para este reparto por bloques, la concatenación ya está ordenada por semilla; el orden es un contrato. | Explica por qué sigue ahí. |
| «El determinismo solo depende del generador.» | El orden de los resultados también importa. | Ordena antes de la suma. |
| «La suma paralela reemplaza a las trazas.» | Prueban cosas distintas: igualdad de resultados frente a bytes de transcripción. | Ejecuta ambas. |

## Siguiente paso

`construction-22` se ocupa de los contratos de error. La costura de `construction-20` y el estado
por motor de esta lección afilan la siguiente pregunta: cuando un movimiento es inválido, ¿quién
decide qué pasa? `tryMove` ya devuelve un valor `MoveOutcome`, pero el programa de consola todavía
devuelve un estado distinto de cero ante una entrada incorrecta de línea de comandos, y el motor no
tiene forma de informar un error recuperable a quien lo llama. La siguiente lección convierte ese
valor en un contrato que el llamador puede manejar sin matar el proceso.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog-es.md) para la ruta completa.

Si faltaba cualquiera de las dos líneas base, no empieces `construction-22`. El siguiente contraste
necesita ambos lados.
