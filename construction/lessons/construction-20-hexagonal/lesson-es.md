# Hexagonal (hexagonal)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** puertos y adaptadores · **Tiempo:** unos 92 minutos ·
**Prerrequisitos:** construction-08, construction-17 · **Estado:** borrador
**Lectura:** ~27 minutos

## Por qué existe esta lección

`construction-17` terminó con un motor que sabía imprimir:

```cpp
void printTurnHeader(int turnNumber, const Knight *activeKnight,
                     const int moveOrder[moveCandidateCount]) const
{
    std::printf("turn=%d who=%s at=%d,%d order=", turnNumber,
                activeKnight->role_name(), activeKnight->row(),
                activeKnight->column());
    // ...
}
```

Cada evento del juego era también un `printf`. El núcleo decidía qué pasaba y cómo se veía, en la
misma función. Esta lección quita la segunda mitad de esa frase. El núcleo sigue decidiendo *qué
pasó*; un objeto fuera de él decide *cómo se ve*.

La frase que conviene retener:

> El núcleo anuncia eventos en el lenguaje del juego; un adaptador convierte esos eventos en una
> transcripción, un conteo o cualquier otra cosa. El núcleo no sabe cuál.

Tres notas honestas antes de la comparación.

1. El código crece 204 líneas, de 611 en doce archivos a 815 en diecisiete. Cinco archivos son
   nuevos, dos cambiaron y diez son idénticos byte a byte a `construction-17`. `game_engine.h` se
   reduce de 302 líneas a 245 en gran parte porque seis métodos de impresión salieron de él; el
   puerto, los adaptadores y la sonda nuevos son 260 líneas.
2. El objeto del núcleo se reduce: el texto de `game.o` pasa de 4.656 a 4.138 bytes, una pérdida de
   518. El objeto del adaptador de consola añade 1.341 bytes, y el binario enlazado crece de 9.668
   a 10.748 bytes de texto y de 692 a 876 bytes de datos. Aparecen ocho llamadas indirectas en
   `game.o`, donde `construction-17` no tenía ninguna. Ese es el precio del despacho, y es medible.
3. La transcripción no cambia. Las 20 trazas de referencia coinciden byte a byte. El tablero, el
   generador de movimientos, el caballo, la posición y la secuencia aleatoria son los mismos
   archivos, sin cambios.

La comparación no es «hexagonal es mejor». Es más estrecha: una flecha de dependencia se invierte,
aparece una nueva indirección y un segundo programa puede ejecutar el mismo núcleo sin consola.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar puertos, adaptadores y la flecha de dependencia;
- ejecutar el mismo núcleo con adaptadores de consola y de prueba.

## El concepto: quién decide cómo se ve

### La flecha de dependencia

`construction-17` tenía una sola flecha: el motor incluía `<cstdio>` y llamaba a `std::printf`. El
núcleo dependía de la consola.

`construction-20` tiene otra forma:

```text
        +----------------+          +------------------+
        |   GameEngine   | -------> |  TranscriptPort  | <------- ConsoleTranscript
        +----------------+  calls   +------------------+  implements
              |                            ^
              |                            |
              |                     RecordingTranscript
              |
        owns Board, Knight, Position, random state
```

Lee las flechas como dependencias de código fuente, no como llamadas en tiempo de ejecución.

- `GameEngine` incluye `transcript_port.h`. Nunca incluye `console_transcript.h`.
- `ConsoleTranscript` incluye `transcript_port.h`. Puede incluir `<cstdio>`.
- `RecordingTranscript` incluye `transcript_port.h`. No incluye ningún encabezado de salida.
- La raíz de composición, `game.cpp`, incluye el motor y el adaptador de consola, y los conecta.

La flecha del motor a la consola desapareció. Queda una flecha del motor a un contrato abstracto y
una flecha de cada adaptador al mismo contrato. La consola es ahora una hoja.

### Un puerto es un contrato que posee el núcleo

El puerto tiene un método por evento de la transcripción:

| Método | Qué anuncia |
|---|---|
| `game_header` | semilla, nombre del jugador, tiradas, quién empieza, posiciones iniciales |
| `turn_header` | número de turno, rol activo, posición, orden de movimientos |
| `illegal_attempt` | número de movimiento, destino y por qué fue ilegal |
| `capture_attempt` | número de movimiento, destino y el caballo que sería capturado |
| `legal_attempt` | número de movimiento y destino |
| `move` | número de movimiento, rol, origen, destino y si hubo captura |
| `no_legal_move` | el rol que queda atrapado |
| `winner` | el nombre del ganador y el conteo final de movimientos |

Dos propiedades hacen que esto sea un puerto y no un ayudante de impresión.

1. **Los nombres son eventos del juego.** `capture_attempt` dice qué pasó en la partida. `printf`
   con una cadena de formato dice qué debería mostrar la consola. El primero sobrevive a un cambio
   de salida; el segundo no.
2. **Los parámetros son valores, no texto.** El núcleo pasa una fila y una columna, no `"3,4"`.
   Pasa el arreglo de orden de movimientos, no `"4,5,6,7,8,1,2,3"`. El formato se queda del lado
   del adaptador.

El núcleo sigue decidiendo *qué* eventos ocurren, *cuándo* y *con qué valores*. Ese es el
conocimiento del juego, y pertenece al núcleo. El puerto no saca ese conocimiento; saca la
representación.

### Un adaptador es una elección

Dos adaptadores implementan los mismos ocho métodos.

`ConsoleTranscript` convierte cada evento en los bytes exactos de
`common/spec/transcript-format.md`. Es el único archivo de la compilación base que imprime la
transcripción. Tiene 20 líneas que mencionan `printf`.

`RecordingTranscript` cuenta eventos y guarda el ganador. Tiene ocho contadores, un `std::string`
para el ganador y un entero para el conteo final de movimientos. No imprime nada. Es solo
encabezado, así que la sonda puede incluirlo sin una unidad de traducción aparte.

El mismo tipo `GameEngine<8, 8>` se ejecuta con ambos. El parámetro de plantilla es el tamaño del
tablero; el adaptador no es un parámetro de plantilla. El motor depende de la clase base, y la
clase base es lo único que necesita conocer.

### Dónde se sitúa la costura

La costura está en la frontera de eventos, no en la celda ni en el movimiento.

- Si la costura fuera más fina, por ejemplo una llamada por cada escritura de celda, el adaptador
  necesitaría las reglas para saber qué escrituras importan. El núcleo filtraría sus decisiones.
- Si la costura fuera más gruesa, por ejemplo una llamada por cada línea de transcripción ya
  formateada, el núcleo conservaría la decisión de formato. El adaptador sería un `puts`.
- Ocho eventos es el punto medio. Cinco son eventos de intento o movimiento dentro del bucle de
  turno; tres son eventos de cabecera, trampa y ganador alrededor de él.

Esa elección es el diseño. También es la parte que un cambio posterior puede invalidar: un
adaptador que necesita un dato que el puerto no lleva obliga a que el puerto crezca.

### Lo que el puerto no es

- No es un framework de registro. No hay niveles, categorías, marcas de tiempo ni registro global.
- No es un contenedor de inyección de dependencias. La dependencia es una referencia que se pasa
  al constructor.
- No es una bolsa genérica de callbacks. Los métodos llevan nombres del juego, y el conjunto está
  cerrado por la clase abstracta.
- No es una forma de eliminar todas las dependencias. El núcleo sigue dependiendo del puerto. Lo
  que cambia es que la dependencia apunta a un contrato que el núcleo define, no a un dispositivo
  que el núcleo no controla.

### El coste de la costura

La costura no es gratis, y los números del paso 3 lo muestran.

- **Despacho.** Cada llamada `port.method(...)` pasa por una vtable. `game.o` tiene ocho llamadas
  indirectas donde `construction-17` no tenía ninguna.
- **Tamaño.** El motor crece de 24 a 32 bytes porque guarda una referencia al puerto. El objeto del
  adaptador de consola es de 8 bytes: un puntero a la vtable y ningún estado.
- **Archivos.** Cinco archivos nuevos y 260 líneas nuevas, la mayoría la declaración del puerto y
  los dos adaptadores.
- **Un contrato de ciclo de vida.** El motor guarda `TranscriptPort &`, así que el adaptador debe
  vivir más que el motor. `game.cpp` declara el adaptador primero y el motor segundo, lo que da el
  orden de destrucción correcto. Un puntero permitiría un adaptador nulo y un reenganche
  posterior; una referencia hace explícito el contrato y elimina esos dos modos de fallo.
- **Un contrato de cambio.** Un evento nuevo cambia la clase abstracta, y el compilador entonces se
  niega a compilar cualquier adaptador que no haya respondido. Ese es el objetivo del paso 5.

La línea base también elimina el constructor de movimiento y la asignación de movimiento que
`construction-17` dejaba por defecto. Un motor movido seguiría refiriéndose al mismo adaptador, así
que esto es una elección y no un requisito del patrón: el punto de conexión queda fijo durante toda
la vida del motor.

## Antes de empezar

- [`construction-08`](../construction-08-modular/lesson-es.md) — programación modular. La costura
  de esta lección es una frontera entre módulos, y la lección trata sobre la dirección de la
  dependencia a través de ella.
- [`construction-17`](../construction-17-stl/lesson-es.md) — la línea base inmediata y el lado
  izquierdo del diff del paso 6. Su directorio `code/` es el artefacto del que parte esta lección,
  aunque el catálogo liste `construction-08` como prerrequisito formal.
- [`construction-12`](../construction-12-polymorphism/lesson-es.md) — contexto opcional. El puerto
  usa `virtual` y `override`. No necesitas diseñar una jerarquía de clases desde cero, pero sí
  leer `virtual void f() = 0;` y saber que la llamada se resuelve en tiempo de ejecución.
- Un compilador de C++20 (`g++` o `clang++`). `cmake` es opcional; `run.sh` recurre a una
  compilación directa cuando CMake no está.

No necesitas conocer la literatura de la arquitectura hexagonal ni un framework. La línea base es
una clase abstracta, dos subclases y un miembro de referencia.

### Qué necesitas

- La línea base hexagonal:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/game_engine.h`](code/game_engine.h),
  [`code/transcript_port.h`](code/transcript_port.h),
  [`code/console_transcript.h`](code/console_transcript.h),
  [`code/console_transcript.cpp`](code/console_transcript.cpp),
  [`code/recording_transcript.h`](code/recording_transcript.h),
  [`code/adapter_probe.cpp`](code/adapter_probe.cpp), [`code/board.h`](code/board.h),
  [`code/move_generator.h`](code/move_generator.h), [`code/knight.h`](code/knight.h),
  [`code/knight.cpp`](code/knight.cpp), [`code/position.h`](code/position.h),
  [`code/position.cpp`](code/position.cpp), [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- La línea base anterior, para el diff:
  [`code/game_engine.h`](../construction-17-stl/code/game_engine.h),
  [`code/game.cpp`](../construction-17-stl/code/game.cpp) y sus diez compañeros en
  [`../construction-17-stl/code/`](../construction-17-stl/code/).
- Guiones de compilación y notas: [`run.sh`](run.sh), [`CMakeLists.txt`](CMakeLists.txt) y
  [`README-es.md`](README-es.md).
- Las reglas: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §1 fija el
  tablero y su numeración desde 1, §4 fija el orden de movimientos del caballo, §8 fija cuándo una
  casilla pasa a visitada.
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
test -f code/transcript_port.h &&
test -f code/console_transcript.h &&
test -f code/console_transcript.cpp &&
test -f code/recording_transcript.h &&
test -f code/adapter_probe.cpp &&
test -f code/board.h &&
test -f code/move_generator.h &&
test -f code/knight.h &&
test -f code/knight.cpp &&
test -f code/position.h &&
test -f code/position.cpp &&
test -f code/random_sequence.h &&
test -f code/random_sequence.cpp &&
test -f ../construction-17-stl/code/game_engine.h &&
test -f ../construction-17-stl/code/game.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

Si falta un archivo, la comparación no tiene segundo lado. Registra el archivo ausente en
`EVIDENCE-es.md` §11 y vuelve cuando esté disponible. Un conteo contra un archivo que no has leído
no es evidencia.

La línea base de 8×8 reproduce las 20 trazas de `common/traces/` byte a byte, antes de que cambies
nada. Ese es el contrato de comportamiento, y esta lección no lo toca.

## Dos sesiones

Esta lección es larga a propósito. Divídela en dos sesiones:

- **Sesión 1 — Núcleo (Pasos 1–3, unos 39 min).** Detente después del Paso 3, cuando la flecha de
  dependencia y el puerto son visibles.
- **Sesión 2 — Extensión (Pasos 4–8, unos 53 min).** Empieza solo después de la Sesión 1. Vas a
  probar la frontera con la prueba de cambio y la sonda.

## Paso 1 — Inventariar la frontera (~12 min)

Empieza por el conteo del que trata la aceptación:

```bash
grep -rn 'printf' code/ | wc -l
```

```text
26
```

Veinte de esas líneas están en `console_transcript.cpp`, cinco en `main.cpp` (errores de uso en
`stderr`) y una en `adapter_probe.cpp`. Ninguna está en `game_engine.h`, `game.h` ni `game.cpp`.

Ahora el encabezado de salida:

```bash
grep -rn 'cstdio' code/
```

```text
code/console_transcript.cpp:3:#include <cstdio>
code/main.cpp:5:#include <cstdio>
code/adapter_probe.cpp:4:#include <cstdio>
```

Tres archivos lo incluyen. El núcleo no. `game.cpp` tampoco imprime; solo conecta el adaptador.

Ahora el puerto y sus implementaciones:

```bash
grep -c 'virtual void' code/transcript_port.h
grep -rn 'override' code/ | wc -l
grep -rn 'console_transcript.h' code/
grep -rn 'game_engine.h' code/
```

```text
8
16
code/console_transcript.cpp:1:#include "console_transcript.h"
code/game.cpp:3:#include "console_transcript.h"
code/game.cpp:4:#include "game_engine.h"
code/adapter_probe.cpp:1:#include "game_engine.h"
```

Ocho métodos virtuales puros, dieciséis sobrescrituras (ocho por adaptador) y exactamente dos
programas que incluyen el núcleo: el programa de consola y la sonda. El núcleo no incluye la
consola.

Los puntos de llamada al puerto dentro del motor:

```bash
grep -nE 'port\.[a-z_]+\(' code/game_engine.h
```

```text
64:        port.game_header(seed, playerName, playerRoll, machineRoll, playerIsActive,
174:        port.turn_header(successfulMoveCount + 1, activeKnightName, activeKnight->row(),
192:                port.illegal_attempt(attemptedMoveNumber, destinationRow, destinationColumn,
198:                port.illegal_attempt(attemptedMoveNumber, destinationRow, destinationColumn,
204:                port.capture_attempt(attemptedMoveNumber, destinationRow, destinationColumn,
207:                port.move(successfulMoveCount, activeKnightName, activeKnight->row(),
215:            port.legal_attempt(attemptedMoveNumber, destinationRow, destinationColumn);
217:            port.move(successfulMoveCount, activeKnightName, activeKnight->row(),
227:            port.no_legal_move(activeKnightName);
243:        port.winner(winnerName.c_str(), successfulMoveCount);
```

Diez puntos de llamada para ocho métodos: `illegal_attempt` y `move` se llaman cada uno desde dos
ramas. Los números de línea son los que debes usar en `EVIDENCE-es.md` §2.

Recuentos de esta línea base. La regla de conteo se enuncia para que puedas reproducirla:

- Archivos: 17.
- Encabezados: 10 (`board.h`, `console_transcript.h`, `game.h`, `game_engine.h`, `knight.h`,
  `move_generator.h`, `position.h`, `random_sequence.h`, `recording_transcript.h`,
  `transcript_port.h`).
- Unidades de traducción en la compilación base: 6 (`main.cpp`, `game.cpp`,
  `console_transcript.cpp`, `knight.cpp`, `position.cpp`, `random_sequence.cpp`).
  `adapter_probe.cpp` es un segundo programa.
- Definiciones de clase completas: 8 (`Board`, `GameEngine`, `Knight`, `Position`,
  `MoveCandidate`, `TranscriptPort`, `ConsoleTranscript`, `RecordingTranscript`).
- Métodos virtuales puros: 8.
- Sobrescrituras: 16.
- Líneas con `printf`/`fprintf`: 26.
- Inclusiones de `<cstdio>`: 3.
- Líneas de código: 815.
- Archivos idénticos byte a byte a `construction-17`: 10.

Después responde en `EVIDENCE-es.md` §2:

1. ¿Qué archivos incluyen `<cstdio>` y cuál de ellos es el núcleo?
2. ¿Cuántos métodos tiene el puerto y cuántas sobrescrituras escribe cada adaptador?
3. ¿Qué dos programas incluyen `game_engine.h` y qué adaptador usa cada uno?
4. ¿Cuántos puntos de llamada `port.` hay y por qué el número es mayor que ocho?
5. ¿Qué seis métodos de impresión salieron de `GameEngine` y a dónde fueron?
6. ¿Qué archivo es la raíz de composición y qué conecta?
7. ¿Qué diez archivos son idénticos byte a byte a `construction-17`?
8. ¿Qué evento renderiza el adaptador de consola pero el adaptador de registro solo cuenta?
9. ¿Qué parámetros pasa el puerto como valores y no como texto?
10. ¿Qué dos objetos deben declararse en un orden específico, y por qué?

La pregunta 8 tiene una respuesta precisa cuando lees ambos adaptadores: cada evento es renderizado
por la consola y contado por el adaptador de registro; el adaptador de registro también guarda el
nombre del ganador y el conteo final de movimientos. Pregunta 10: el adaptador debe declararse antes
que el motor, porque el motor guarda una referencia y debe destruirse primero.

## Paso 2 — Predecir antes de ejecutar (~12 min)

Escribe estas predicciones antes de ejecutar nada. No mires las respuestas del paso 3 primero.

### 2.1 La tabla de estructura

| Pregunta | Mi predicción |
|---|---|
| Archivos en `construction-17` | |
| Archivos aquí | |
| Encabezados aquí | |
| Unidades de traducción en la compilación base | |
| Líneas de código en `construction-17` | |
| Líneas de código aquí | |
| Líneas de `game_engine.h`, antes y después | |
| Métodos de impresión en el núcleo, antes y después | |
| Inclusiones de `<cstdio>`, antes y después | |
| Archivos idénticos byte a byte | |
| Archivos nuevos | |
| Archivos cambiados | |

La fila que es fácil equivocar es el total de líneas. Cinco archivos nuevos añaden 260 líneas, pero
el motor pierde 57 y `game.cpp` gana 1. El neto no es el tamaño de los archivos nuevos.

### 2.2 La tabla de adaptadores

| Pregunta | Mi predicción |
|---|---|
| Métodos en el puerto | |
| Métodos del adaptador de consola | |
| Métodos del adaptador de registro | |
| Líneas `printf` del adaptador de consola | |
| Líneas de salida del adaptador de registro | |
| `sizeof` del adaptador de consola | |
| `sizeof` del adaptador de registro | |
| Qué guarda el adaptador de registro sobre el ganador | |
| Qué adaptador se enlaza en el binario de consola | |
| Qué adaptador se enlaza en la sonda | |

Predice `sizeof(ConsoleTranscript)` como un puntero: no tiene miembros de datos. Predice
`sizeof(RecordingTranscript)` como mayor: ocho contadores, un `std::string` y un entero, más el
puntero a la vtable.

### 2.3 La tabla de tamaños y texto

| Pregunta | Mi predicción |
|---|---|
| `sizeof(GameEngine<8, 8>)` aquí | |
| `sizeof(Board<8, 8>)` aquí | |
| Texto de `game.o`, antes y después | |
| Texto del objeto del adaptador de consola | |
| Texto del binario, antes y después | |
| Datos del binario, antes y después | |
| BSS del binario, antes y después | |
| Llamadas indirectas en `game.o`, antes y después | |
| Semillas que coinciden, de 20 | |

Predice el motor en 32 bytes: tres miembros `unique_ptr` de 8 bytes cada uno, más una referencia de
8 bytes. Predice el tablero en 256: el puerto no toca el diseño del tablero.

### 2.4 Las predicciones de la prueba de cambio

La prueba de cambio es: añadir un evento al puerto, implementarlo en ambos adaptadores y mostrar
que la transcripción de consola no cambia.

| Pregunta | Mi predicción |
|---|---|
| El evento que voy a añadir | |
| Archivos que voy a editar | |
| Líneas que voy a añadir al núcleo | |
| Líneas que voy a añadir al bucle | |
| ¿Cambia la transcripción de consola? | |
| Semillas que coinciden, de 20, después del cambio | |
| Nuevo conteo en la sonda | |
| Resultado de compilación con `-Wall -Wextra -Werror` | |

Predice el bucle como cero líneas. Si tu cambio toca `playTurn`, has elegido un evento que nace
dentro del bucle; esa es otra prueba, y se describe en el paso 5.4.

## Paso 3 — Ejecutar la línea base (~15 min)

Primero las trazas, sin modificar:

```bash
mkdir -p /tmp/hex-test
./run.sh --seed 1 --name Learner > /tmp/hex-test/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/hex-test/baseline-0001.txt
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
  -o build/hexagonal-baseline code/main.cpp code/game.cpp \
  code/console_transcript.cpp code/knight.cpp code/position.cpp \
  code/random_sequence.cpp
```

Ahora el adaptador de prueba. La sonda enlaza el núcleo con `RecordingTranscript` y no enlaza el
adaptador de consola en absoluto:

```bash
g++ -std=c++20 -O2 -Icode -o /tmp/hex-test/adapter-probe \
  code/adapter_probe.cpp code/knight.cpp code/position.cpp \
  code/random_sequence.cpp
/tmp/hex-test/adapter-probe
```

La sonda imprime una línea. Dividida en campos, dice:

| Campo | Valor |
|---|---|
| `headers` | 1 |
| `turns` | 22 |
| `illegal` | 15 |
| `capture` | 0 |
| `legal` | 21 |
| `moves` | 21 |
| `no_legal` | 1 |
| `winners` | 1 |
| `winner` | `machine` |
| `final_moves` | 21 |

Lee los campos como un resumen de la semilla 1:

- Una cabecera de partida.
- 22 turnos: 21 turnos con un movimiento aceptado, más un turno sin movimiento legal.
- 15 intentos ilegales y 21 intentos legales: 36 intentos en total.
- 0 intentos de captura. La partida terminó atrapando un caballo, no capturando uno.
- 21 movimientos y un evento `no_legal_move`.
- Un evento de ganador, y el ganador es `machine`, tras 21 movimientos.

La aritmética cierra: `15 + 21 = 36` intentos, `21 + 1 = 22` turnos, y `moves = legal = 21`
porque no hubo captura. Si tus números no cierran, el adaptador está contando otra partida.

Ahora los tamaños. La sonda va en el directorio temporal, nunca en los archivos versionados:

```bash
cat > /tmp/hex-test/sizes.cpp <<'EOF'
#include "console_transcript.h"
#include "game_engine.h"
#include "recording_transcript.h"

#include <cstdio>

int main()
{
    std::printf("sizeof(Board<8,8>)=%zu\n", sizeof(Board<8, 8>));
    std::printf("sizeof(GameEngine<8,8>)=%zu\n", sizeof(GameEngine<8, 8>));
    std::printf("sizeof(Knight)=%zu\n", sizeof(Knight));
    std::printf("sizeof(Position)=%zu\n", sizeof(Position));
    std::printf("sizeof(MoveCandidate)=%zu\n", sizeof(MoveCandidate));
    std::printf("sizeof(TranscriptPort)=%zu\n", sizeof(TranscriptPort));
    std::printf("sizeof(ConsoleTranscript)=%zu\n", sizeof(ConsoleTranscript));
    std::printf("sizeof(RecordingTranscript)=%zu\n", sizeof(RecordingTranscript));
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/hex-test/sizes /tmp/hex-test/sizes.cpp \
  code/knight.cpp code/position.cpp code/console_transcript.cpp
/tmp/hex-test/sizes
```

La salida en esta plataforma:

```text
sizeof(Board<8,8>)=256
sizeof(GameEngine<8,8>)=32
sizeof(Knight)=40
sizeof(Position)=8
sizeof(MoveCandidate)=12
sizeof(TranscriptPort)=8
sizeof(ConsoleTranscript)=8
sizeof(RecordingTranscript)=80
```

El tablero, el caballo, la posición y el candidato coinciden con `construction-17`. El motor creció
8 bytes: la referencia al puerto. `TranscriptPort` y `ConsoleTranscript` son un puntero a la vtable
cada uno. `RecordingTranscript` mide 80 bytes porque lleva los contadores y la cadena del ganador.

Ahora el código objeto:

```bash
g++ -std=c++20 -O2 -Icode -c code/game.cpp -o /tmp/hex-test/game20.o
g++ -std=c++20 -O2 -Icode -c code/console_transcript.cpp \
  -o /tmp/hex-test/console20.o
g++ -std=c++20 -O2 -I../construction-17-stl/code \
  -c ../construction-17-stl/code/game.cpp -o /tmp/hex-test/game17.o
size /tmp/hex-test/game17.o /tmp/hex-test/game20.o /tmp/hex-test/console20.o
```

```text
   text	   data	    bss	    dec	    hex	filename
   4656	      0	      0	   4656	   1230	/tmp/hex-test/game17.o
   4138	      0	      0	   4138	   102a	/tmp/hex-test/game20.o
   1341	      0	      0	   1341	    53d	/tmp/hex-test/console20.o
```

El objeto del núcleo se reduce 518 bytes porque el código de impresión sale de él. El objeto del
adaptador de consola añade 1.341. Enlaza ambos programas y compara los binarios:

```bash
g++ -std=c++20 -O2 -Icode -o /tmp/hex-test/bin20 \
  code/main.cpp code/game.cpp code/console_transcript.cpp \
  code/knight.cpp code/position.cpp code/random_sequence.cpp
g++ -std=c++20 -O2 -I../construction-17-stl/code -o /tmp/hex-test/bin17 \
  ../construction-17-stl/code/main.cpp ../construction-17-stl/code/game.cpp \
  ../construction-17-stl/code/knight.cpp ../construction-17-stl/code/position.cpp \
  ../construction-17-stl/code/random_sequence.cpp
size /tmp/hex-test/bin17 /tmp/hex-test/bin20
```

```text
   text	   data	    bss	    dec	    hex	filename
   9668	    692	     24	  10384	   2890	/tmp/hex-test/bin17
  10748	    876	     24	  11648	   2d80	/tmp/hex-test/bin20
```

El texto crece 1.080 bytes y los datos 184. El BSS no se mueve. El programa se hizo más grande
aunque el núcleo se hizo más pequeño: el código del adaptador y sus tablas de despacho tienen que
vivir en algún sitio.

Las llamadas indirectas son el coste visible de la costura:

```bash
objdump -d -C /tmp/hex-test/game17.o | grep -c 'call.*\*' || true
objdump -d -C /tmp/hex-test/game20.o | grep -c 'call.*\*' || true
```

```text
0
8
```

Ocho instrucciones de llamada indirecta, donde `construction-17` no tenía ninguna. Hay diez puntos
de llamada en el código; el compilador fusionó o especializó algunos, pero cada método virtual
distinto sigue despachándose por la vtable. Ese es el intercambio: el núcleo ya no conoce el
adaptador, y la llamada al adaptador ya no es directa.

Si el primer diff no está vacío, detente. Esa es una línea base distinta de la que describe esta
lección; regístrala en `EVIDENCE-es.md` §11 y no ajustes la predicción para que coincida.

## Paso 4 — Comparar: qué demuestra la frontera (~15 min)

Abre `game_engine.h`, `transcript_port.h`, `console_transcript.cpp` y `game.cpp`. Cinco hechos,
leídos contra las mediciones.

1. **El núcleo no tiene encabezado de salida.** `game_engine.h` incluye `board.h`, `knight.h`,
   `move_generator.h`, `random_sequence.h` y el puerto, además de `<array>`, `<concepts>`,
   `<cstdint>`, `<memory>`, `<ranges>` y `<string>`. No incluye `<cstdio>` y no contiene `printf`.
   La única salida de la compilación base está en el adaptador de consola y en la ruta de error de
   `main.cpp`.
2. **Los seis métodos de impresión se convirtieron en ocho eventos.** `printGameHeader`,
   `printTurnHeader`, `printIllegalAttempt`, `printCaptureAttempt`, `printLegalAttempt` y
   `printMove` desaparecieron. Dos sitios directos de `printf`, `no_legal_move` y `winner`, también
   se convirtieron en métodos. El motor llama a `port.*` en diez sitios; la consola implementa ocho
   métodos.
3. **`game.cpp` es la raíz de composición.** Construye `ConsoleTranscript`, luego
   `GameEngine<8, 8>` con una referencia a él, y después llama a `run`. El motor nunca nombra la
   consola. La sonda hace lo mismo con `RecordingTranscript`.
4. **El tablero no se movió.** `board.h` es idéntico byte a byte a `construction-17`, y
   `sizeof(Board<8, 8>)` sigue siendo 256. El puerto trata de eventos, no de diseño de datos.
5. **La costura tiene un coste de despacho.** Ocho llamadas indirectas, 1.080 bytes de texto y 184
   bytes de datos más en el binario enlazado. El objeto del núcleo es más pequeño; el programa es
   más grande.

### 4.1 El diff exacto

Dos archivos cambiaron y cinco son nuevos. El diff contra `construction-17`:

```bash
diff -u ../construction-17-stl/code/game_engine.h code/game_engine.h
diff -u ../construction-17-stl/code/game.cpp code/game.cpp
```

| Archivo | Añadidas | Eliminadas | Neto | Qué cambió |
|---|---|---|---|---|
| `game_engine.h` | 27 | 84 | -57 | Seis métodos de impresión fuera, una referencia añadida, diez llamadas `port.*` |
| `game.cpp` | 3 | 2 | +1 | Construye el adaptador de consola y se lo pasa al motor |
| `transcript_port.h` | 35 | 0 | +35 | Nueva clase abstracta con ocho métodos virtuales puros |
| `console_transcript.h` | 29 | 0 | +29 | Nueva declaración del adaptador |
| `console_transcript.cpp` | 76 | 0 | +76 | Nueva implementación del adaptador, 20 líneas `printf` |
| `recording_transcript.h` | 101 | 0 | +101 | Nuevo adaptador de prueba, solo encabezado |
| `adapter_probe.cpp` | 19 | 0 | +19 | Nuevo programa que ejecuta el núcleo con el adaptador de prueba |
| Total | 290 | 86 | +204 | Cinco archivos nuevos, dos cambiados |

Los otros diez archivos son idénticos byte a byte. Las reglas, el formato de transcripción, el
tablero, el generador de movimientos, los caballos, la posición y la secuencia aleatoria no se
tocaron.

### 4.2 Lo que no cambió

- La transcripción. Las 20 trazas coinciden, byte a byte.
- Las reglas. El orden de movimientos, la captura, la trampa, el empate y el límite de 200
  movimientos son los mismos.
- El diseño del tablero. `board.h` es idéntico byte a byte; `sizeof(Board<8, 8>)` es 256.
- El generador de movimientos. `move_generator.h` es idéntico byte a byte, incluido el `transform`
  sobre `iota`.
- El bucle de intentos. `playTurn` tiene los mismos cuatro resultados y las mismas dos formas de
  parar. Solo cambiaron las llamadas de dentro, de `printX` a `port.x`.
- La interfaz pública del juego. `game.h` y `run_game` conservan sus firmas.
- El conteo de trazas. 20 de 20 antes y después.

### 4.3 Lo que cuesta

- Cinco archivos nuevos y 260 líneas nuevas, sobre todo la declaración del puerto y los cuerpos de
  los adaptadores.
- Ocho llamadas indirectas en `game.o`, donde no había ninguna.
- 8 bytes más por motor para la referencia.
- 1.080 bytes de texto y 184 bytes de datos más en el binario enlazado.
- Una regla de ciclo de vida: el adaptador debe vivir más que el motor.
- Una regla de cambio: un evento nuevo toca el puerto y todos los adaptadores, y el compilador lo
  hace cumplir.
- Una cosa más que leer: la declaración del puerto. Quien quiera saber qué imprime el programa
  ahora lee dos archivos en vez de uno.

Ninguno de estos es un defecto. Son el precio de sacar la decisión de representación del núcleo, y
el registro debe enumerarlos junto al beneficio.

### 4.4 La tabla de eventos

Esta tabla es el corazón de la comparación. Léela contra ambos adaptadores.

| Evento | Punto de llamada | `ConsoleTranscript` | `RecordingTranscript` |
|---|---|---|---|
| `game_header` | `run`, línea 64 | 7 líneas `printf` | `gameHeaderCount += 1` |
| `turn_header` | `playTurn`, línea 174 | prefijo, bucle, salto de línea | `turnHeaderCount += 1` |
| `illegal_attempt` | `playTurn`, líneas 192, 198 | 1 línea | `illegalAttemptCount += 1` |
| `capture_attempt` | `playTurn`, línea 204 | 1 línea | `captureAttemptCount += 1` |
| `legal_attempt` | `playTurn`, línea 215 | 1 línea | `legalAttemptCount += 1` |
| `move` | `playTurn`, líneas 207, 217 | 2 o 3 líneas | `moveCount += 1` |
| `no_legal_move` | `playTurn`, línea 227 | 1 línea | `noLegalMoveCount += 1` |
| `winner` | `playGame`, línea 243 | 2 líneas | conteo, nombre, conteo final |

Cada evento es renderizado por la consola y contado por el adaptador de registro. Ningún evento se
ignora en la línea base. El paso 5 añade uno que la consola ignora a propósito, para mostrar que un
adaptador puede elegir no renderizar un evento sin cambiar el núcleo.

## Paso 5 — Prueba de cambio: añadir un evento al puerto (~20 min)

La aceptación nombra la prueba de cambio: **añadir un evento al puerto sin editar el bucle del
juego.**

El evento es `game_started`. Informa la semilla antes de la cabecera. El adaptador de consola lo
implementa como no-op, así que la transcripción sigue siendo idéntica byte a byte. El adaptador de
registro lo cuenta. El núcleo lo emite una vez, en `run`, antes de que empiece el bucle.

Una nota sobre la redacción. El párrafo de la prueba de cambio del README dice «sin tocar el
núcleo»; la frase de aceptación dice «sin editar el bucle del juego». Esta lección prueba la
segunda afirmación, más estrecha, porque es la que se puede comprobar con un diff: `playTurn` es
idéntico byte a byte. La llamada de una línea en `run` sí es una edición del núcleo, y fingir lo
contrario haría que el diff mintiera. Si quieres una variante sin editar el núcleo, emite el evento
desde `game.cpp` después de que `run` retorne; el evento sale entonces de la raíz de composición, no
del núcleo, y prueba menos sobre la costura.

### 5.1 Hacer una copia temporal

Nunca edites el directorio `code/` versionado. Cópialo:

```bash
rm -rf /tmp/hex-test/change
cp -r code /tmp/hex-test/change
```

### 5.2 Añadir el evento

En `/tmp/hex-test/change/transcript_port.h`, añade una declaración después del destructor:

```cpp
virtual void game_started(std::int64_t seed) = 0;
```

En `/tmp/hex-test/change/game_engine.h`, añade una línea en `run`, después de las tiradas y antes de
la cabecera:

```cpp
port.game_started(seed);
```

En `/tmp/hex-test/change/console_transcript.h`, declara la sobrescritura:

```cpp
void game_started(std::int64_t seed) override;
```

En `/tmp/hex-test/change/console_transcript.cpp`, impleméntala como no-op:

```cpp
void ConsoleTranscript::game_started(std::int64_t seed)
{
    (void)seed;
}
```

El `(void)seed;` no es decoración. La compilación usa `-Wall -Wextra -Werror`, y un parámetro sin
usar es un error. El cast es la forma honesta más pequeña de decir «este adaptador recibe el valor
y no lo usa».

En `/tmp/hex-test/change/recording_transcript.h`, añade el contador y el método:

```cpp
int gameStartedCount = 0;

void game_started(std::int64_t seed) override
{
    (void)seed;
    gameStartedCount += 1;
}
```

Por último, imprime el nuevo conteo en `/tmp/hex-test/change/adapter_probe.cpp`:

```cpp
std::printf("started=%d headers=%d turns=%d illegal=%d capture=%d legal=%d moves=%d "
            "no_legal=%d winners=%d winner=%s final_moves=%d\n",
            transcript.gameStartedCount, transcript.gameHeaderCount,
            // ... the remaining arguments are unchanged
```

La llamada completa tiene un argumento más que antes; mantén el orden alineado con la cadena de
formato.

### 5.3 Compilar y comparar

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I/tmp/hex-test/change \
  -o /tmp/hex-test/change-bin \
  /tmp/hex-test/change/main.cpp /tmp/hex-test/change/game.cpp \
  /tmp/hex-test/change/console_transcript.cpp /tmp/hex-test/change/knight.cpp \
  /tmp/hex-test/change/position.cpp /tmp/hex-test/change/random_sequence.cpp
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/hex-test/change-bin --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

El bucle está silencioso: 20 de 20. La transcripción de consola no cambió, aunque el núcleo ahora
emite un evento más.

Compila la sonda contra la misma copia temporal:

```bash
g++ -std=c++20 -O2 -I/tmp/hex-test/change -o /tmp/hex-test/change-probe \
  /tmp/hex-test/change/adapter_probe.cpp /tmp/hex-test/change/knight.cpp \
  /tmp/hex-test/change/position.cpp /tmp/hex-test/change/random_sequence.cpp
/tmp/hex-test/change-probe
```

El nuevo conteo es 1, y todos los demás campos coinciden con la tabla del paso 3. El mismo núcleo
produjo ambos resultados; solo cambió el adaptador.

Ahora verifica la afirmación sobre el bucle. La única edición en el núcleo es una línea en `run`:

```bash
diff -u code/game_engine.h /tmp/hex-test/change/game_engine.h
```

```text
@@ -61,6 +61,7 @@
         initializeRolls(playerRoll, machineRoll);

         bool playerIsActive = playerRoll > machineRoll;
+        port.game_started(seed);
         port.game_header(seed, playerName, playerRoll, machineRoll, playerIsActive,
                          playerKnight->row(), playerKnight->column(),
                          machineKnight->row(), machineKnight->column());
```

`playTurn` no aparece en el diff. El evento nace antes del bucle, así que el bucle no necesita
saber de él. Ese es el sentido preciso en que este evento se añade «sin editar el bucle del juego».

### 5.4 Qué demuestra la prueba de cambio, y qué no

Demuestra cuatro cosas.

1. La costura acepta un evento nuevo con una línea en el núcleo y un método en cada adaptador.
2. Un adaptador puede ignorar un evento. La consola implementa `game_started` como no-op y la
   transcripción sigue siendo idéntica byte a byte.
3. Un segundo adaptador puede consumir el mismo evento de otra forma. El adaptador de registro lo
   cuenta.
4. El bucle del juego no se toca. El evento se emite fuera de `playTurn`, así que los cuatro
   resultados del bucle y sus dos condiciones de parada no cambian.

No demuestra que todo evento pueda añadirse sin editar el bucle. Un evento que nace dentro del
bucle de intentos, por ejemplo «este intento fue el tercer intento ilegal consecutivo», necesita
una llamada dentro de `playTurn`. Esa es la frontera que prueba este test: el bucle es la parte del
núcleo que más sabe, y un evento que necesita ese conocimiento debe emitirse allí.

Tampoco demuestra que el evento sea útil. Un conteo de un `game_started` por partida no es
interesante por sí mismo. La prueba trata de la forma del cambio, no del valor del evento.

Prueba la otra dirección si quieres ver el coste: añade un evento que la consola deba renderizar,
por ejemplo una línea `turn_finished`. La transcripción cambia, así que las 20 trazas de referencia
también deben cambiar, y el contrato de comportamiento de la lección deja de cumplirse. Por eso el
evento de consola de esta prueba es un no-op: la prueba trata de la costura, no de cambiar el
formato de salida.

## Paso 6 — Comparar con `construction-17` (~10 min)

| Métrica | construction-17 | construction-20 | Diferencia |
|---|---|---|---|
| Archivos | 12 | 17 | +5 |
| Encabezados | 7 | 10 | +3 |
| Unidades de traducción (compilación base) | 5 | 6 | +1 |
| Líneas de código | 611 | 815 | +204 |
| `game_engine.h` | 302 | 245 | -57 |
| `game.cpp` | 10 | 11 | +1 |
| Clases completas | 5 | 8 | +3 |
| Métodos virtuales puros | 0 | 8 | +8 |
| Sobrescrituras | 0 | 16 | +16 |
| Métodos de impresión en el núcleo | 6 | 0 | -6 |
| Inclusiones de `<cstdio>` | 2 | 3 | +1 |
| Líneas `printf`/`fprintf` | 25 | 26 | +1 |
| Llamadas indirectas en `game.o` | 0 | 8 | +8 |
| `sizeof(GameEngine<8, 8>)` | 24 | 32 | +8 |
| `sizeof(Board<8, 8>)` | 256 | 256 | 0 |
| Texto de `game.o` | 4.656 | 4.138 | -518 |
| Texto del objeto del adaptador de consola | — | 1.341 | +1.341 |
| Texto / datos del binario | 9.668 / 692 | 10.748 / 876 | +1.080 / +184 |
| BSS del binario | 24 | 24 | 0 |
| Archivos idénticos byte a byte | — | 10 de 12 | — |
| Trazas | 20 de 20 | 20 de 20 | 0 |

### 6.1 Qué compra

- El núcleo no tiene dependencia de salida. No incluye `<cstdio>` y no contiene `printf`.
- El formato de consola vive en un solo archivo. Un cambio en el formato de transcripción toca
  `console_transcript.cpp`, no el motor.
- El núcleo se ejecuta sin consola. La sonda ejecuta el mismo `GameEngine<8, 8>` con un adaptador
  que no imprime nada.
- Una prueba puede observar el juego mediante conteos en vez de capturar la salida estándar. La
  sonda verifica 22 turnos, 36 intentos, 21 movimientos, una trampa y un ganador sin analizar texto.
- La raíz de composición elige el adaptador. `game.cpp` elige la consola; `adapter_probe.cpp` elige
  el adaptador de registro; el motor no cambia.
- El puerto nombra los eventos en el lenguaje del juego. Ese vocabulario sobrevive a un cambio de
  texto a JSON, a una interfaz gráfica o a un flujo de red.

### 6.2 Qué cuesta

- Cinco archivos nuevos y 260 líneas nuevas.
- Ocho llamadas indirectas en `game.o`.
- 8 bytes más por motor para la referencia.
- 1.080 bytes de texto y 184 bytes de datos más en el binario enlazado.
- Una regla de ciclo de vida: el adaptador debe vivir más que el motor.
- Una regla de cambio: un evento nuevo cambia la clase abstracta y todos los adaptadores.
- Un archivo más que leer antes de saber qué imprime el programa.

### 6.3 El hueco: ocho eventos, una costura

La línea base no es «el núcleo no tiene salida». Es «el núcleo no tiene un *dispositivo* de salida».
Quedan tres acoplamientos, y vale la pena nombrarlos.

1. **El puerto incluye `move_generator.h`.** `transcript_port.h` necesita `moveCandidateCount` para
   el parámetro de `turn_header`. El puerto, por tanto, no es independiente del conteo de
   movimientos del juego. Una firma más neutra tomaría `std::array<int, moveCandidateCount>` o un
   span, pero la constante igual tiene que venir de algún sitio.
2. **Los parámetros `const char *` son prestados.** `role_name()` devuelve un puntero dentro de un
   `std::string` que posee un `Knight`. El puntero es válido durante la llamada. Un adaptador que lo
   guardara quedaría colgando. `RecordingTranscript` copia el nombre del ganador en un
   `std::string`; no guarda nombres de rol.
3. **El núcleo sigue decidiendo qué eventos ocurren.** El adaptador decide cómo se ven, no si
   ocurren. Si un adaptador futuro necesita un dato que el puerto no lleva, el puerto crece y todos
   los adaptadores cambian. Ese es el acoplamiento que la costura no elimina.

La frase de aceptación precisa es más estrecha que «el programa está desacoplado»:

> El aprendiz puede ejecutar el mismo núcleo con dos adaptadores, mostrar que la transcripción de
> consola no cambia y añadir un evento al puerto sin editar el bucle del juego.

Mismo núcleo, dos adaptadores, una transcripción sin cambios, un evento nuevo fuera del bucle. Nada
sobre acoplamiento cero, coste cero o que todo evento se pueda añadir sin editar el núcleo.

## Paso 7 — Explicar (~5 min)

Escribe `EVIDENCE-es.md` §8. Dos partes.

**Primero, una frase.** Di qué cambió entre las dos líneas base y qué decide el cambio, sin usar
las palabras *puerto*, *adaptador*, *interfaz*, *virtual*, *hexagonal*, *dependencia*, *mejor*,
*peor*, *limpio* ni *desacoplado*. Si tu frase necesita una de ellas, has nombrado el mecanismo en
vez de la decisión.

**Segundo, un párrafo.** Responde:

- Qué decide el núcleo: qué eventos ocurren, cuándo y con qué valores.
- Qué decide el adaptador: cómo esos eventos se convierten en bytes, conteos o cualquier otra cosa.
- Qué es el puerto: ocho eventos con nombre, valores en vez de texto, cerrado por una clase
  abstracta.
- Qué hace el adaptador de consola: renderiza la transcripción exacta en un solo archivo.
- Qué hace el adaptador de registro: cuenta eventos y guarda el ganador, sin salida.
- Qué hace `game.cpp`: elige el adaptador de consola y lo conecta al motor.
- Qué hace `adapter_probe.cpp`: elige el adaptador de registro para el mismo motor.
- Qué prueba el diff: cinco archivos nuevos, dos cambiados, diez idénticos byte a byte.
- Qué prueban los tamaños: el núcleo se reduce 518 bytes de texto; el programa crece 1.080.
- Qué prueban las llamadas indirectas: la costura cuesta despacho.
- Qué prueba la prueba de cambio: un evento nuevo fuera del bucle toca el puerto y ambos
  adaptadores, y la transcripción no se mueve.
- Qué prueba el bucle: un evento que necesita el conocimiento del bucle debe emitirse dentro de él.
- Qué lección posterior posee la concurrencia: `construction-21`.

Una respuesta fuerte mantiene separadas cuatro cosas: **quién decide el evento** (el núcleo),
**quién decide la representación** (el adaptador), **qué cuesta la costura** (despacho, tamaño,
archivos) y **qué puede ver una ejecución** (la transcripción, sin cambios). Una respuesta débil
dice «el código está desacoplado», lo cual es falso en ambas direcciones: el núcleo sigue
dependiendo del puerto, y el puerto sigue dependiendo de una constante del juego.

## Paso 8 — Verificación de aceptación y registro (~3 min)

El criterio de aceptación de esta lección es: **mismo núcleo, adaptadores de consola y de prueba;
las 20 trazas pasan; añadir un evento al puerto sin editar el bucle del juego.**

Comprueba tu registro contra él:

- [ ] Localicé los ocho métodos del puerto y las dieciséis sobrescrituras.
- [ ] Localicé los diez puntos de llamada `port.*` y expliqué por qué hay más de ocho.
- [ ] Mostré que `game_engine.h` no incluye `<cstdio>` ni contiene `printf`.
- [ ] Nombré `game.cpp` como raíz de composición y `adapter_probe.cpp` como segundo programa.
- [ ] Predije las tablas de estructura, adaptadores y tamaños antes de medir.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Ejecuté la sonda de adaptadores y leí los conteos: 22 turnos, 36 intentos, 21 movimientos,
      una trampa.
- [ ] Medí `sizeof(GameEngine<8, 8>)` en 32 y `sizeof(Board<8, 8>)` en 256.
- [ ] Medí el texto de `game.o` en 4.138 y el objeto del adaptador de consola en 1.341.
- [ ] Medí el binario en 10.748 bytes de texto y 876 de datos.
- [ ] Conté 8 llamadas indirectas en `game.o`, frente a 0 en `construction-17`.
- [ ] Añadí `game_started` en una copia temporal y lo implementé en ambos adaptadores.
- [ ] La compilación temporal pasó con `-Wall -Wextra -Werror`.
- [ ] Las 20 trazas coincidieron después del cambio.
- [ ] La sonda imprimió `started=1` y todos los demás conteos sin cambios.
- [ ] `playTurn` era idéntico byte a byte en la copia temporal.
- [ ] Expliqué por qué un adaptador puede ignorar un evento.
- [ ] Nombré los tres acoplamientos que quedan: la inclusión del conteo de movimientos, los
      parámetros `const char *` prestados y la elección de eventos del núcleo.
- [ ] No edité ningún archivo de código versionado.
- [ ] Completé `EVIDENCE-es.md` §1–§10 y la autocomprobación de §12.

El criterio no es «la arquitectura es hexagonal». Es «puedes ejecutar un núcleo con dos
adaptadores, mostrar que la transcripción no se movió y añadir un evento fuera del bucle».

## Autocomprobación (binaria)

- [ ] Leí `game_engine.h`, `transcript_port.h`, `console_transcript.cpp` y `game.cpp`.
- [ ] Conté 8 métodos del puerto, 16 sobrescrituras y 10 puntos de llamada `port.*`.
- [ ] Mostré que el núcleo no incluye `<cstdio>` ni contiene `printf`.
- [ ] Comparé la línea base con `construction-17` y nombré los dos archivos cambiados y los cinco
      nuevos.
- [ ] Predije las tablas de estructura, adaptadores y tamaños antes de ejecutar.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Ejecuté la sonda y expliqué los conteos aritméticamente.
- [ ] Medí el motor en 32 bytes y el tablero en 256.
- [ ] Medí `game.o` en 4.138 bytes de texto, el adaptador de consola en 1.341 y el binario en
      10.748 de texto y 876 de datos.
- [ ] Conté 8 llamadas indirectas en `game.o`.
- [ ] Añadí un evento al puerto en una copia temporal, lo implementé en ambos adaptadores y mantuve
      la transcripción sin cambios.
- [ ] Mostré que `playTurn` no cambió.
- [ ] Nombré la regla de ciclo de vida: el adaptador vive más que el motor.
- [ ] Completé `EVIDENCE-es.md`.

Estos son criterios de aceptación. Si una casilla queda
sin marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §10.

## Misconcepciones que esta lección refuta

| Error | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| «El núcleo no tiene dependencias.» | Depende de `TranscriptPort`. | Di qué dependencia se invirtió, no que desaparecieron todas. |
| «Hexagonal significa seis capas.» | La línea base es un puerto y dos adaptadores. | Lee el código: ocho métodos, dos subclases. |
| «El puerto es una biblioteca de registro.» | No tiene niveles, categorías ni formato. | Mira los nombres: son eventos del juego. |
| «El adaptador decide qué pasó.» | El núcleo decide qué eventos ocurren y con qué valores. | Separa el evento de su representación. |
| «Las llamadas virtuales son gratis.» | `game.o` tiene 8 llamadas indirectas y el binario crece 1.080 bytes. | Mide el objeto y el binario. |
| «El núcleo es testeable porque no tiene `printf`.» | Es testeable porque un adaptador puede observarlo. | Ejecuta la sonda; una prueba igual necesita un adaptador. |
| «Añadir un evento nunca toca el núcleo.» | El evento debe emitirse en algún sitio; aquí, una línea en `run`. | Nombra el punto de emisión. |
| «Añadir un evento nunca toca los adaptadores.» | Cada adaptador debe responder al nuevo método virtual puro. | Cuenta los archivos que el compilador rechaza. |
| «El miembro de referencia es opcional.» | Sin él, el motor necesita el adaptador en cada llamada o un puntero. | Lee el constructor y el miembro. |
| «El adaptador de registro demuestra la transcripción.» | Demuestra conteos y el ganador, no bytes. | Combínalo con el diff de consola contra las trazas. |
| «La transcripción de consola la produce el núcleo.» | La produce el adaptador de consola a partir de eventos. | Sigue un evento desde `port.move` hasta `printf`. |
| «El puerto debería pasar cadenas ya formateadas.» | Entonces el núcleo conserva la decisión de formato. | Pasa valores; formatea en el adaptador. |
| «El adaptador puede destruirse antes que el motor.» | El motor guarda una referencia; la llamada quedaría colgando. | Declara el adaptador primero. |
| «Hexagonal necesita un framework o un contenedor de inyección.» | Dos clases y una referencia bastan. | Lee `game.cpp`. |
| «El tablero cambió.» | `board.h` es idéntico byte a byte y el tablero sigue en 256 bytes. | Compara el archivo y mide `sizeof`. |
| «El programa es más pequeño porque el núcleo es más pequeño.» | El binario crece 1.080 bytes de texto. | Mide el binario enlazado, no solo `game.o`. |
| «Las 20 trazas prueban que ambos adaptadores coinciden.» | Prueban el adaptador de consola; la sonda comprueba el de registro. | Usa cada comprobación para su propia afirmación. |
| «Un adaptador puede descartar cualquier evento en silencio.» | Puede elegir no renderizar uno; el evento igual ocurre. | Di «no renderiza», no «no ocurre». |

## Siguiente paso

`construction-21` posee la concurrencia. El puerto es la costura donde un segundo hilo entregaría
eventos: una cola entre el núcleo y el adaptador, con el adaptador consumiendo en otro hilo. Ese
cambio expondría la regla de ciclo de vida de esta lección, porque el adaptador viviría más que el
motor y también más que el hilo que produce los eventos. La costura está en el lugar correcto para
esa pregunta; si es suficiente, es el problema de la siguiente lección.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog.md) para la ruta completa.

Si faltaba cualquiera de las dos líneas base, no empieces `construction-21`. El siguiente contraste
necesita ambos lados.
