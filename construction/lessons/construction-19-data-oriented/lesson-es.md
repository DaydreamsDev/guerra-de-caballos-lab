# Data-oriented design (orientado a datos)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** layout (disposición) · **Tiempo:** unos 90 minutos · **Estado:** borrador
**Lectura:** ~23 minutos

## Por qué existe esta lección

`construction-17` terminó con un motor que posee tres objetos en el heap mediante
`std::unique_ptr`, y con un tablero que es una clase con datos privados y métodos.
`construction-19` conserva todo eso:

```cpp
std::unique_ptr<Board<8, 8>> board;
std::unique_ptr<Knight> playerKnight;
std::unique_ptr<Knight> machineKnight;
```

El motor sigue midiendo 24 bytes. Cada partida sigue haciendo tres reservas de memoria dinámica. El
tablero sigue siendo una clase, y sus operaciones siguen siendo métodos. Lo único que cambia es la
representación *dentro* de `Board<8, 8>`:

- `construction-17` guarda las casillas visitadas como un
  `std::array<std::array<bool, 16>, 16>` anidado.
- `construction-19` las guarda como un `std::array<std::uint64_t, 4>` empaquetado por bits, y
  agrega un `std::array<std::uint8_t, 256> occupant` para que la captura sea una consulta en tabla.

Ese es todo el cambio. Es un cambio de **layout** (disposición), y esta lección mide qué compra y
qué no compra el layout por sí solo. La respuesta honesta: el layout por sí solo no redujo las
reservas, no achicó el motor y no produjo una mejora de velocidad medible. La ganancia es una
representación distinta y un objeto tablero más grande; la propiedad no cambió.

Tres notas honestas antes de la comparación.

1. El código crece en dos archivos. `board.h` pasa de 75 a 118 líneas (+43) y `game_engine.h` de
   302 a 313 (+11). Los doce archivos del juego pasan de 611 a 686 líneas, y el banco de pruebas
   agrega 21 más. Diez de los doce archivos del juego son idénticos byte a byte a `construction-17`.
2. `sizeof(Board<8, 8>)` pasa de 256 a 288 (+32). `sizeof(GameEngine<8, 8>)` sigue en 24 en ambas
   líneas base. Las reservas para 10,000 partidas siguen en 30,000 en ambas. El texto de `game.o`
   crece 305 bytes, y el texto del binario enlazado 304.
3. El tiempo de reloj no mejora. En esta máquina, 10,000 partidas tardan unos 270–285 ms en
   `construction-17` y unos 270–290 ms en `construction-19`. Los rangos se solapan, así que la
   diferencia es ruido, no un resultado. El formato de la transcripción domina la ejecución, y
   ambos tableros son pequeños.

La frase para retener:

> El layout es parte del diseño: elige una representación para los datos y mídela. Pero el layout es
> una decisión entre varias. Cambiar la representación del tablero no cambia quién lo posee, ni
> cuánto mide el motor, ni cuántas peticiones hace el programa al heap.

La comparación no es "el diseño orientado a datos es más rápido que el diseño orientado a objetos".
Es más acotada: el tablero guarda los mismos hechos con otra forma, el objeto tablero crece 32
bytes, el motor y el número de reservas no se mueven, y la transcripción sigue idéntica.

## Qué podrás hacer

Al terminar esta lección puedes leer la representación de un tablero, decir qué guarda y dónde,
medir su tamaño y su costo en código objeto, y explicar por qué un cambio de layout por sí solo no
movió las reservas ni el reloj.

## El concepto: el layout es el diseño

### La regla de una sola variable

Esta lección cambia una sola variable. Todo lo demás queda fijo a propósito.

| Decisión | En esta lección |
|---|---|
| Propiedad | Fija: `std::unique_ptr<Board<8, 8>>` y dos `std::unique_ptr<Knight>` |
| Estilo de métodos | Fijo: `Board` es una clase; `reset`, `was_visited`, `mark_visited`, `occupant_at`, `set_occupant` y `clear_occupant` son métodos |
| Tamaño del motor | Fijo: 24 bytes en ambas líneas base |
| Reservas | Fijas: tres por partida en ambas líneas base |
| Layout del tablero | Cambia: el arreglo anidado de `bool` pasa a palabras empaquetadas por bits más una tabla de ocupantes |

Si cambiaras la propiedad y el layout en el mismo paso, un número menor de reservas podría venir de
cualquiera de los dos. Tendrías dos explicaciones para un solo número, y ninguna forma de elegir.
Esta lección mantiene fija la propiedad y el estilo de métodos para que cada número que reporta se
pueda atribuir solo al layout.

Por eso el resultado también es modesto. El layout es una decisión real, pero no es la decisión que
elimina el asignador. `construction-16` y `construction-17` se ocupan de esa pregunta. Aquí el motor
sigue pidiendo al heap un tablero y dos caballos, porque ese es el diseño que se mantiene constante.

### Dos layouts para el mismo tablero

`construction-17` guarda el tablero como un arreglo anidado de filas:

```cpp
using CellRow = std::array<bool, 16>;
std::array<CellRow, 16> visited;
```

Son 16 filas de 16 valores `bool`: 256 bytes, un byte por casilla. El acceso es
`visited[row][column]`, y el compilador calcula el salto entre filas. La captura se detecta
comparando el destino con la fila y la columna del otro caballo; no hay almacenamiento de ocupantes.

`construction-19` conserva la misma clase y los mismos métodos, y cambia solo los miembros privados:

```cpp
std::array<std::uint64_t, visitedWordCount> visitedWords;
std::array<std::uint8_t, maximumCellCount> occupant;
```

`visitedWords` empaqueta un bit por casilla en cuatro palabras de 64 bits: 32 bytes para las 256
casillas. `occupant` guarda un byte por casilla: 256 bytes. El total es 288 bytes. Ambos arreglos
están dimensionados para el tablero máximo de 16×16, no para el área activa de 8×8. Las casillas
activas son un subconjunto, y `is_inside` rechaza las coordenadas que quedan fuera. Por eso el campo
de visitadas mide 32 bytes y el de ocupantes 256, y no 8 y 64.

Los métodos esconden la representación exactamente como en `construction-17`. `was_visited` pasa a
ser un desplazamiento y una máscara; `mark_visited` pasa a ser un OR con un bit desplazado;
`occupant_at` pasa a ser una consulta en tabla. Quien llama sigue escribiendo
`board->was_visited(row, column)`. Nada del sitio de llamada cambia.

### Qué cambió, con precisión

Los dos layouts difieren en cuatro puntos, y conviene mantenerlos separados:

1. **Almacenamiento de visitadas.** 17 usa un byte por casilla en un arreglo anidado. 19 usa un bit
   por casilla en cuatro palabras. El campo de visitadas baja de 256 bytes a 32.
2. **Almacenamiento de ocupantes.** 17 no tiene arreglo de ocupantes; detecta una captura comparando
   el destino con las coordenadas del otro caballo. 19 guarda un byte de ocupante por casilla, así
   que una captura es una consulta en tabla. El campo de ocupantes agrega 256 bytes.
3. **Acceso a la casilla.** 17 indexa dos dimensiones del arreglo. 19 calcula un índice plano y
   luego un índice de palabra y un desplazamiento de bit. El bitboard es compacto, pero quien lo lee
   tiene que conocer la correspondencia.
4. **Tamaño neto del tablero.** 288 en lugar de 256. Los 224 bytes ahorrados en visitadas quedan más
   que compensados por los 256 bytes agregados para ocupantes.

Todo lo demás se queda. El motor sigue teniendo tres miembros `unique_ptr`. El tablero sigue siendo
una clase con datos privados y métodos. La copia sigue eliminada y el movimiento sigue siendo
predeterminado. Los caballos, el generador, las reglas y la transcripción no se tocan.

### Qué significa layout aquí

El layout trata de qué bytes están cerca de cuáles y qué bytes se tocan juntos:

- **Localidad espacial:** las casillas consecutivas son adyacentes en memoria.
  `cell_index(row, column)` convierte una coordenada del tablero en un desplazamiento plano, así que
  un recorrido por casillas toca direcciones consecutivas.
- **Localidad temporal:** el mismo tablero se lee y se escribe en cada turno. El motor no lo
  reconstruye entre turnos.
- **Agrupación por campo:** el layout de 19 agrupa por patrón de acceso. Los bits de visitadas se
  leen y se escriben como un bitboard; los ocupantes se leen y se escriben como bytes. Cada arreglo
  se indexa con el mismo `cell_index`.

Esta es la idea de struct-of-arrays a nivel del tablero: dos arreglos paralelos, cada uno indexado
por el mismo índice de casilla. No es una regla universal. Rinde cuando un recorrido toca un campo
de muchas casillas. Aquí el tablero mide solo 288 bytes, y ambos layouts caben en unas pocas
líneas de caché. El argumento de localidad es real, pero el banco de pruebas no es lo bastante
sensible para mostrarlo. Ese es el punto de medir en lugar de suponer.

### Por qué nada más se movió

**El motor no se achicó.** `sizeof(GameEngine<8, 8>)` es 24 en ambas líneas base porque los tres
miembros siguen siendo tres punteros. Cambió el contenido del tablero; no cambió su ubicación. Un
`unique_ptr` es un puntero tanto si apunta a 256 bytes como a 288.

**El número de reservas no bajó.** El motor sigue llamando a `std::make_unique` tres veces por
partida. El constructor del tablero ahora inicializa otro par de arreglos, pero el tablero sigue
siendo un bloque separado en el heap. 10,000 partidas siguen haciendo 30,000 peticiones en ambas
líneas base.

**El tiempo de reloj no mejoró.** El banco de pruebas redirige la salida estándar a `/dev/null`, así
que las llamadas de escritura son baratas, pero `printf` sigue formateando cada línea. Cada partida
imprime una cabecera, una línea por turno, una línea por intento de movimiento y un pie. El formato
domina la ejecución. Además, el tablero es pequeño en ambos layouts, así que el conjunto de trabajo
cabe en unas pocas líneas de caché en cualquiera de los dos casos.

**El código objeto creció.** El texto de `game.o` pasa de 4,656 a 4,961 (+305). Las operaciones de
bits (`index / 64`, desplazamientos, máscaras) y las escrituras de ocupantes son más instrucciones
que un índice de arreglo anidado y una comparación de coordenadas. El texto del binario enlazado
crece 304. Este es un costo que sí se mueve, y se mueve en la dirección esperable para una
representación más compacta con más aritmética por acceso.

**La transcripción no se movió.** Las 20 trazas de referencia coinciden byte a byte. Las reglas no
cambiaron, así que el comportamiento no cambió.

### Qué no prueba el cambio

No prueba que el diseño orientado a datos sea más rápido en general. No prueba que struct-of-arrays
sea mejor que cualquier otro layout para cualquier carga. No prueba que los fallos de caché sean
irrelevantes; prueba que este tablero es demasiado pequeño y esta carga está demasiado dominada por
el formato como para que este banco de pruebas los exponga. No prueba que el motor use menos
memoria; el motor mide lo mismo y el tablero es más grande. No prueba que la propiedad haya
cambiado, porque no cambió.

Prueba una cosa acotada: la representación dentro de una clase puede cambiar mientras la propiedad,
el estilo de métodos, el tamaño del motor, el número de reservas y el comportamiento quedan fijos, a
un costo medible en bytes del tablero y en texto objeto.

## Antes de empezar

- [`construction-05`](../construction-05-array-of-records/lesson.md) — colección y recorrido. El
  tablero es una colección de casillas, y esta lección cambia cómo se guardan esas casillas.
- [`construction-17`](../construction-17-stl/lesson.md) — la línea base inmediata y el lado
  izquierdo de la comparación. Su directorio `code/` es el artefacto del que parte esta lección.
- [`construction-16`](../construction-16-ownership/lesson.md) — contexto opcional. El modelo de
  propiedad (copia eliminada, movimiento predeterminado) se hereda de ahí y aquí se mantiene fijo.
- Un compilador de C++20 (`g++` o `clang++`). `cmake` es opcional; `run.sh` recurre a una
  compilación directa cuando CMake no está disponible.

No necesitas saber cómo se implementa un bitboard. Necesitas leer dos layouts, decir qué guarda cada
uno y dónde, predecir el perfil, ejecutar 10,000 partidas y explicar qué números se movieron y
cuáles no.

### Qué necesitas

- La línea base orientada a datos:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/game_engine.h`](code/game_engine.h),
  [`code/board.h`](code/board.h), [`code/move_generator.h`](code/move_generator.h),
  [`code/knight.h`](code/knight.h), [`code/knight.cpp`](code/knight.cpp),
  [`code/position.h`](code/position.h), [`code/position.cpp`](code/position.cpp),
  [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp) y
  [`code/benchmark.cpp`](code/benchmark.cpp).
- La línea base anterior, para la comparación:
  [`code/board.h`](../construction-17-stl/code/board.h),
  [`code/game_engine.h`](../construction-17-stl/code/game_engine.h) y sus diez compañeros en
  [`../construction-17-stl/code/`](../construction-17-stl/code/).
- Guiones de compilación y notas: [`run.sh`](run.sh), [`CMakeLists.txt`](CMakeLists.txt) y
  [`README.md`](README.md).
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
test -f code/benchmark.cpp &&
test -f ../construction-17-stl/code/board.h &&
test -f ../construction-17-stl/code/game_engine.h &&
test -f ../../../common/traces/seed-0001.txt
```

Si falta un archivo, la comparación no tiene segundo lado. Registra el archivo faltante en
`EVIDENCE-es.md` §10 y vuelve cuando esté disponible. Un conteo contra un archivo que no leíste no
es evidencia.

La línea base de 8×8 reproduce las 20 trazas de `common/traces/` byte a byte, antes de que cambies
nada. Ese es el contrato de comportamiento, y esta lección no lo toca.

## El recorrido con tiempos

Esta lección es larga a propósito. Divídela en dos sesiones:

- **Sesión 1 — Núcleo (pasos 1–3, unos 40 min).** Detente después del paso 3, cuando ambos layouts
  estén medidos y los tamaños sean conocidos.
- **Sesión 2 — Extensión (pasos 4–6, unos 45 min).** Empieza solo después de la sesión 1. Perfilarás
  10,000 partidas y explicarás el resultado plano.

Los cinco minutos restantes son margen para el registro y la autoevaluación.

## Paso 1 — Inventariar los dos layouts (~15 min)

Lee `board.h` y `game_engine.h` en ambas líneas base. No infieras el layout desde el README; lee las
declaraciones.

```bash
repoRoot="$(git rev-parse --show-toplevel)"
lesson17="${repoRoot}/construction/lessons/construction-17-stl"
lesson19="${repoRoot}/construction/lessons/construction-19-data-oriented"

grep -n 'class Board\|visited\|occupant\|unique_ptr\|make_unique' \
  "${lesson17}/code/board.h" "${lesson17}/code/game_engine.h"
grep -n 'class Board\|visitedWords\|occupant\|unique_ptr\|make_unique' \
  "${lesson19}/code/board.h" "${lesson19}/code/game_engine.h"
```

Ahora confirma que la propiedad y el estilo de métodos son iguales en ambos lados:

```bash
grep -n 'std::unique_ptr\|std::make_unique' "${lesson17}/code/game_engine.h"
grep -n 'std::unique_ptr\|std::make_unique' "${lesson19}/code/game_engine.h"

grep -n '^    \(void\|int\|bool\|std::uint8_t\) ' "${lesson17}/code/board.h"
grep -n '^    \(void\|int\|bool\|std::uint8_t\) ' "${lesson19}/code/board.h"
```

Las líneas de propiedad coinciden: tres llamadas a `make_unique` en el constructor y tres miembros
`unique_ptr`. Ambos listados son métodos. 19 agrega `occupant_at`, `set_occupant` y
`clear_occupant`; el resto conserva sus nombres. Ningún método se volvió una función libre, y ambos
tableros siguen siendo clases.

Completa esta tabla desde el código:

| Pregunta | construction-17 | construction-19 |
|---|---|---|
| Tipo del tablero | `Board<8, 8>` (clase) | `Board<8, 8>` (clase) |
| Almacenamiento de visitadas | `std::array<std::array<bool, 16>, 16>` | `std::array<std::uint64_t, 4>` |
| Bits por casilla visitada | 8 (un `bool`) | 1 |
| Bytes de visitadas | 256 | 32 |
| Almacenamiento de ocupantes | ninguno; la captura compara coordenadas | `std::array<std::uint8_t, 256>` |
| Bytes de ocupantes | 0 | 256 |
| Bytes del tablero | 256 | 288 |
| Miembros del motor | 3 `std::unique_ptr` | 3 `std::unique_ptr` |
| Bytes del motor | 24 | 24 |
| Reservas por partida | 3 | 3 |
| Operaciones del tablero | métodos | métodos |
| Copia / movimiento del motor | copia eliminada, movimiento predeterminado | copia eliminada, movimiento predeterminado |

Luego responde en `EVIDENCE-es.md` §2:

1. ¿Qué dos archivos difieren entre las líneas base, y cuáles diez son idénticos byte a byte?
2. ¿Cuántos bytes usa cada campo de visitadas, y cuántos bits por casilla?
3. ¿Cuántos bytes agrega el campo de ocupantes, y qué reemplaza?
4. ¿Cuánto mide `sizeof(Board<8, 8>)` en cada línea base?
5. ¿Cuánto mide `sizeof(GameEngine<8, 8>)` en cada una, y por qué es igual?
6. ¿Cuántas reservas hace cada partida en cada línea base?
7. ¿Qué declaraciones de miembros muestran que la propiedad no cambió?
8. ¿Qué invariante agrega el arreglo de ocupantes, y qué clase lo mantiene privado?
9. ¿Qué debe saber quien lee `was_visited` en 19 que 17 no exigía?
10. ¿Qué diez archivos son idénticos byte a byte, y qué prueba eso sobre el cambio?

Las preguntas 8 y 9 son los costos de diseño. Pregunta 8: el arreglo de ocupantes debe coincidir con
las posiciones de los caballos en cada movimiento que continúa la partida. Un movimiento sin captura
limpia la casilla vieja y fija la nueva; una captura fija la nueva y termina la partida, así que la
casilla vieja queda obsoleta pero nunca se vuelve a leer. El arreglo es privado, así que `Board`
sigue siendo dueño del invariante; el motor lo mantiene llamando a los métodos en el orden correcto.
Pregunta 9: el bitboard exige el índice plano y la separación entre palabra y bit, que la clase
esconde pero que quien lee `board.h` debe entender.

## Paso 2 — Predecir antes de ejecutar (~10 min)

Escribe estas predicciones en `EVIDENCE-es.md` §3 antes de ejecutar nada. No mires primero los
números medidos.

### 2.1 La tabla del layout

| Pregunta | Mi predicción |
|---|---|
| `sizeof(Board<8, 8>)` en 17 | |
| `sizeof(Board<8, 8>)` en 19 | |
| `sizeof(GameEngine<8, 8>)` en 17 | |
| `sizeof(GameEngine<8, 8>)` en 19 | |
| Reservas por partida en 17 | |
| Reservas por partida en 19 | |
| Reservas totales para 10,000 partidas en 17 | |
| Reservas totales para 10,000 partidas en 19 | |
| Archivos que difieren entre las líneas base | |
| Trazas que coinciden, de 20, en cada una | |

### 2.2 La tabla del perfil

| Pregunta | Mi predicción |
|---|---|
| Tiempo de 10,000 partidas en 17 | |
| Tiempo de 10,000 partidas en 19 | |
| Cuál es más rápida | |
| Diferencia en milisegundos | |
| Texto de `game.o` en 17 y 19 | |
| Texto del binario en 17 y 19 | |
| Datos del binario en 17 y 19 | |
| Líneas de código en 17 y 19 | |

Antes de completar la tabla, decide qué predice tu modelo. Los miembros y las llamadas a
`make_unique` no cambian, así que un modelo que ata las reservas a la propiedad predice que el
tamaño del motor y el número de reservas no se mueven. Un modelo que ata las reservas a la
representación del tablero predice una caída. Escribe tu predicción y luego mide. Para el tamaño del
tablero, compara lo que ahorra el bitboard con lo que agrega la tabla de ocupantes antes de decidir
la dirección. La diferencia entre tu predicción y la observación es la lección, sea cero o no.

### 2.3 La prueba de explicación

| Pregunta | Mi predicción |
|---|---|
| ¿Cambiará el tamaño del tablero? | |
| ¿Cambiará el tamaño del motor? | |
| ¿Cambiará el número de reservas? | |
| ¿Cambiará el tiempo de reloj? | |
| ¿Por qué? | |
| ¿Qué ve la transcripción? | |
| ¿Qué incluye `sizeof(GameEngine<8, 8>)`? | |
| ¿Qué no incluye? | |

Las dos últimas filas son las que se equivocan con facilidad. `sizeof(GameEngine<8, 8>)` incluye los
tres miembros `unique_ptr`. No incluye el tablero ni los caballos, que viven en bloques separados
del heap. Cambiar el contenido del tablero no cambia el tamaño del puntero que lo posee.

## Paso 3 — Ejecutar las trazas y los tamaños (~15 min)

Compila ambas líneas base con las mismas opciones:

```bash
mkdir -p /tmp/layout-test

c++ -std=c++20 -Wall -Wextra -Werror -O2 -I"${lesson17}/code" \
  -o /tmp/layout-test/baseline17 \
  "${lesson17}/code/main.cpp" "${lesson17}/code/game.cpp" \
  "${lesson17}/code/knight.cpp" "${lesson17}/code/position.cpp" \
  "${lesson17}/code/random_sequence.cpp"

c++ -std=c++20 -Wall -Wextra -Werror -O2 -I"${lesson19}/code" \
  -o /tmp/layout-test/baseline19 \
  "${lesson19}/code/main.cpp" "${lesson19}/code/game.cpp" \
  "${lesson19}/code/knight.cpp" "${lesson19}/code/position.cpp" \
  "${lesson19}/code/random_sequence.cpp"
```

Comprueba las 20 trazas en cada lado:

```bash
check_traces() {
  local binary="$1"
  local passed=0
  local total=0
  local trace
  for trace in "${repoRoot}"/common/traces/seed-*.txt; do
    local traceName
    local seedText
    local seedNumber
    traceName="$(basename "${trace}" .txt)"
    seedText="${traceName#seed-}"
    seedNumber="$((10#${seedText}))"
    total=$((total + 1))
    if "${binary}" --seed "${seedNumber}" --name Learner | cmp -s - "${trace}"; then
      passed=$((passed + 1))
    fi
  done
  printf '%s: %d/%d traces matched\n' "${binary}" "${passed}" "${total}"
}

check_traces /tmp/layout-test/baseline17
check_traces /tmp/layout-test/baseline19
```

```text
/tmp/layout-test/baseline17: 20/20 traces matched
/tmp/layout-test/baseline19: 20/20 traces matched
```

Ahora los tamaños. La sonda va en el directorio temporal, nunca en los archivos versionados:

```bash
cat > /tmp/layout-test/sizes.cpp <<'EOF'
#include "game_engine.h"

#include <cstdio>

int main()
{
    std::printf("sizeof(Board<8,8>)=%zu\n", sizeof(Board<8, 8>));
    std::printf("sizeof(GameEngine<8,8>)=%zu\n", sizeof(GameEngine<8, 8>));
    std::printf("sizeof(Knight)=%zu\n", sizeof(Knight));
    std::printf("sizeof(Position)=%zu\n", sizeof(Position));
    return 0;
}
EOF

c++ -std=c++20 -O2 -I"${lesson17}/code" \
  -o /tmp/layout-test/sizes17 /tmp/layout-test/sizes.cpp \
  "${lesson17}/code/knight.cpp" "${lesson17}/code/position.cpp"

c++ -std=c++20 -O2 -I"${lesson19}/code" \
  -o /tmp/layout-test/sizes19 /tmp/layout-test/sizes.cpp \
  "${lesson19}/code/knight.cpp" "${lesson19}/code/position.cpp"

/tmp/layout-test/sizes17
/tmp/layout-test/sizes19
```

La salida en esta plataforma:

```text
sizeof(Board<8,8>)=256
sizeof(GameEngine<8,8>)=24
sizeof(Knight)=40
sizeof(Position)=8
sizeof(Board<8,8>)=288
sizeof(GameEngine<8,8>)=24
sizeof(Knight)=40
sizeof(Position)=8
```

Las primeras cuatro líneas son `construction-17`; las últimas cuatro son `construction-19`. El
tablero creció 32 bytes. El motor no se movió.

Ahora el código objeto:

```bash
c++ -std=c++20 -O2 -I"${lesson17}/code" -c "${lesson17}/code/game.cpp" \
  -o /tmp/layout-test/game17.o
c++ -std=c++20 -O2 -I"${lesson19}/code" -c "${lesson19}/code/game.cpp" \
  -o /tmp/layout-test/game19.o
size /tmp/layout-test/game17.o /tmp/layout-test/game19.o
size /tmp/layout-test/baseline17 /tmp/layout-test/baseline19
```

```text
   text	   data	    bss	    dec	    hex	filename
   4656	      0	      0	   4656	   1230	/tmp/layout-test/game17.o
   4961	      0	      0	   4961	   1361	/tmp/layout-test/game19.o
   text	   data	    bss	    dec	    hex	filename
   9668	    692	     24	  10384	   2890	/tmp/layout-test/baseline17
   9972	    692	     24	  10688	   29c0	/tmp/layout-test/baseline19
```

`game.o` crece 305 bytes de texto. El binario enlazado crece 304 bytes de texto. Los datos y el BSS
son idénticos: 692 y 24. Lo que excede a `game.o` es disposición y alineación del enlazador, no una
segunda implementación. La dirección es lo que importa: las operaciones de bits y las escrituras de
ocupantes son instrucciones reales, y aparecen en el tamaño del texto.

Registra los comandos, los tamaños y los números del objeto en `EVIDENCE-es.md` §4.

Si cualquiera de las comprobaciones de trazas no da 20 de 20, detente. Esa es una línea base
distinta de la que describe esta lección; regístrala en `EVIDENCE-es.md` §10 y no ajustes la
predicción para que coincida.

## Paso 4 — Perfilar 10,000 partidas (~20 min)

El banco de pruebas construye un motor nuevo para cada semilla, ejecuta la partida con la salida
estándar redirigida a `/dev/null` e informa los milisegundos transcurridos por la salida de error.
`code/benchmark.cpp` viene con la línea base 19. Usa la misma sonda en ambos lados para que la
comparación sea simétrica:

```bash
cat > /tmp/layout-test/benchmark.cpp <<'EOF'
#include "game_engine.h"

#include <chrono>
#include <cstdio>

int main()
{
    std::freopen("/dev/null", "w", stdout);
    auto startTime = std::chrono::steady_clock::now();
    for (int seedValue = 1; seedValue <= 10000; seedValue += 1)
    {
        GameEngine<8, 8> engine;
        engine.run(seedValue, "Learner");
    }
    auto endTime = std::chrono::steady_clock::now();
    auto elapsedMilliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::fprintf(stderr, "milliseconds=%lld\n",
                 static_cast<long long>(elapsedMilliseconds.count()));
    return 0;
}
EOF

c++ -std=c++20 -O2 -I"${lesson17}/code" -o /tmp/layout-test/bench17 \
  /tmp/layout-test/benchmark.cpp "${lesson17}/code/knight.cpp" \
  "${lesson17}/code/position.cpp" "${lesson17}/code/random_sequence.cpp"

c++ -std=c++20 -O2 -I"${lesson19}/code" -o /tmp/layout-test/bench19 \
  /tmp/layout-test/benchmark.cpp "${lesson19}/code/knight.cpp" \
  "${lesson19}/code/position.cpp" "${lesson19}/code/random_sequence.cpp"
```

Si `/dev/null` no existe en tu plataforma, quita la línea `freopen` y redirige la salida estándar
cuando ejecutes el binario.

Ejecuta cada lado tres veces:

```bash
for run in 1 2 3; do /tmp/layout-test/bench17; done
for run in 1 2 3; do /tmp/layout-test/bench19; done
```

La salida en esta máquina:

```text
milliseconds=275
milliseconds=271
milliseconds=276
milliseconds=278
milliseconds=273
milliseconds=268
```

Las primeras tres líneas son `construction-17`; las últimas tres son `construction-19`. Los rangos
se solapan, y otra ejecución puede poner a cualquiera de los dos primero. Registra tus propios tres
valores para cada lado; no copies estos. La afirmación de la lección es "dentro del ruido", no
"19 es más rápida".

### 4.1 Contar las reservas

El banco de pruebas no cuenta nada. Agrega un `operator new` que cuente, en el directorio temporal:

```bash
cat > /tmp/layout-test/allocations.cpp <<'EOF'
#include "game_engine.h"

#include <cstdio>
#include <cstdlib>
#include <new>

namespace
{
std::size_t allocationCount = 0;
}

void *operator new(std::size_t size)
{
    allocationCount += 1;
    if (void *pointer = std::malloc(size == 0 ? 1 : size))
    {
        return pointer;
    }
    throw std::bad_alloc();
}

void operator delete(void *pointer) noexcept
{
    std::free(pointer);
}

void operator delete(void *pointer, std::size_t) noexcept
{
    std::free(pointer);
}

int main()
{
    std::freopen("/dev/null", "w", stdout);
    for (int seedValue = 1; seedValue <= 10000; seedValue += 1)
    {
        GameEngine<8, 8> engine;
        engine.run(seedValue, "Learner");
    }
    std::fprintf(stderr, "allocations=%zu\n", allocationCount);
    return 0;
}
EOF

c++ -std=c++20 -O2 -I"${lesson17}/code" -o /tmp/layout-test/alloc17 \
  /tmp/layout-test/allocations.cpp "${lesson17}/code/knight.cpp" \
  "${lesson17}/code/position.cpp" "${lesson17}/code/random_sequence.cpp"

c++ -std=c++20 -O2 -I"${lesson19}/code" -o /tmp/layout-test/alloc19 \
  /tmp/layout-test/allocations.cpp "${lesson19}/code/knight.cpp" \
  "${lesson19}/code/position.cpp" "${lesson19}/code/random_sequence.cpp"

/tmp/layout-test/alloc17
/tmp/layout-test/alloc19
```

```text
allocations=30000
allocations=30000
```

La sustitución cuenta cada llamada a `operator new` del proceso. En este programa las únicas
reservas son las tres del motor por partida, así que el conteo se corresponde directamente con el
motor. Es 30,000 en ambos lados porque el cambio de layout no tocó las tres llamadas a
`make_unique`. Los arreglos del tablero están dentro del bloque del heap que
`make_unique<Board<8, 8>>` ya crea; empaquetar las casillas visitadas en cuatro palabras no elimina
ese bloque.

Registra los tiempos y los conteos de reservas en `EVIDENCE-es.md` §5.

## Paso 5 — Comparar (~15 min)

**La prueba de cambio.** Mide seis cosas en ambas líneas base: `sizeof(Board<8, 8>)`,
`sizeof(GameEngine<8, 8>)`, reservas para 10,000 partidas, tiempo de reloj, texto de `game.o` y
número de trazas que coinciden. El tamaño del tablero y el texto objeto se mueven. El tamaño del
motor, el número de reservas y el tiempo de reloj no. Las trazas siguen idénticas. La tabla de abajo
registra las seis.

| Métrica | construction-17 | construction-19 | Diferencia |
|---|---|---|---|
| Líneas de código (juego) | 611 | 686 | +75 |
| Líneas de código (total) | 611 | 707 | +96 |
| Líneas de `board.h` | 75 | 118 | +43 |
| Líneas de `game_engine.h` | 302 | 313 | +11 |
| `sizeof(Board<8, 8>)` | 256 | 288 | +32 |
| `sizeof(GameEngine<8, 8>)` | 24 | 24 | 0 |
| `sizeof(Knight)` | 40 | 40 | 0 |
| Reservas por partida | 3 | 3 | 0 |
| Reservas por 10,000 partidas | 30,000 | 30,000 | 0 |
| Tiempo de 10,000 partidas | unos 270–285 ms | unos 270–290 ms | dentro del ruido |
| Texto de `game.o` | 4,656 | 4,961 | +305 |
| Texto del binario | 9,668 | 9,972 | +304 |
| Datos del binario | 692 | 692 | 0 |
| BSS del binario | 24 | 24 | 0 |
| Trazas | 20 de 20 | 20 de 20 | 0 |

### 5.1 Qué se movió

- **La representación del tablero.** Las casillas visitadas pasaron de un byte cada una a un bit
  cada una. Los ocupantes pasaron de "no se guardan" a un byte cada uno. La clase ahora tiene dos
  arreglos en lugar de uno.
- **El tamaño del tablero.** De 256 a 288. El bitboard ahorra 224 bytes en visitadas, y el arreglo
  de ocupantes agrega 256.
- **El texto objeto.** +305 bytes en `game.o`. Las operaciones de bits (`index / 64`,
  desplazamientos, máscaras) y las escrituras de ocupantes son más instrucciones que un índice de
  arreglo anidado y una comparación de coordenadas.
- **El código fuente.** +75 líneas en los archivos del juego, +96 con el banco de pruebas.

### 5.2 Qué no se movió

- **El tamaño del motor.** 24 bytes en ambas. Los tres miembros `unique_ptr` no cambian.
- **El número de reservas.** 30,000 en ambas. Tres llamadas a `make_unique` por partida en ambas.
- **El tiempo de reloj, dentro del ruido.** Este es el número que la lección se niega a maquillar.
- **La transcripción.** Las 20 trazas coinciden en ambos lados.
- **Las reglas.** El orden de movimientos, la captura, la trampa, el empate y el límite de 200
  movimientos son iguales.
- **El contrato de propiedad.** La copia está eliminada y el movimiento es predeterminado en ambas.
- **El estilo de métodos.** `Board` es una clase con datos privados y métodos en ambas.
- **Diez de los doce archivos del juego.** `main.cpp`, `game.h`, `game.cpp`, `move_generator.h`,
  `knight.h`, `knight.cpp`, `position.h`, `position.cpp`, `random_sequence.h` y
  `random_sequence.cpp` son idénticos byte a byte.

### 5.3 Qué cuesta

- **32 bytes más por tablero.** 288 en lugar de 256. El arreglo de ocupantes cuesta más de lo que el
  bitboard ahorra.
- **305 bytes más de texto en `game.o`.** 304 en el binario enlazado. La representación compacta no
  es gratis a nivel de instrucciones.
- **Acceso a casillas menos legible.** `visited[row][column]` pasa a ser
  `(visitedWords[index / 64] >> (index % 64)) & 1`. La clase lo esconde, pero quien lee `board.h`
  tiene que conocer la correspondencia.
- **Un invariante que mantener.** El arreglo de ocupantes debe coincidir con las posiciones de los
  caballos en cada movimiento que continúa la partida. Un movimiento sin captura limpia la casilla
  vieja y fija la nueva; una captura fija la nueva y termina la partida, así que la casilla vieja
  queda obsoleta pero nunca se vuelve a leer. Una limpieza omitida en un movimiento sin captura
  produce un ocupante fantasma, y la transcripción puede mostrarlo si un caballo vuelve después a
  esa casilla. El arreglo es privado, así que `Board` sigue haciendo cumplir sus propios
  invariantes; el motor mantiene este mediante los métodos.
- **Más código fuente.** 75 líneas en los archivos del juego, 96 con el banco de pruebas.
- **Ninguna ganancia en reservas ni en velocidad.** Los dos números que alguien podría esperar de
  "diseño orientado a datos" son planos aquí, porque el diseño mantuvo fija la propiedad.

Ninguno de estos puntos es un defecto. Son el precio de una representación más compacta de las
visitadas más una tabla de captura, y el registro debe listarlos junto al beneficio.

### 5.4 El resultado honesto

Un cambio de layout es fácil de vender de más. El campo compacto de visitadas es real: 256 bytes se
convierten en 32. La tabla de captura es real: una comparación se convierte en una consulta. Pero el
objeto tablero es más grande, el motor mide lo mismo, el asignador se sigue llamando tres veces por
partida y la ejecución no se acelera. El cambio compró una representación, no un perfil de recursos.

Ese es el resultado que hay que reportar. Quien dice "el tablero está empaquetado por bits, así
que el programa es más rápido" se saltó la medición. Quien dice "el programa no es más rápido, así
que el cambio de layout no sirvió" se saltó la representación. La lección es reportar ambos y
explicar por qué divergen.

Registra la tabla de comparación en `EVIDENCE-es.md` §6.

## Paso 6 — Explicar y registrar (~10 min)

Escribe `EVIDENCE-es.md` §7. Dos partes.

**Primero, una frase.** Di qué cambió entre las dos líneas base y qué decide el cambio, sin usar
las palabras *caché*, *struct-of-arrays*, *array-of-structs*, *orientado a datos*, *mejor*,
*más rápido*, *más lento* u *optimización*. Si tu frase necesita una de ellas, nombraste el
mecanismo o el veredicto en lugar de la decisión.

**Segundo, un párrafo.** Responde:

- Qué decide el layout: los bits de visitadas y los ocupantes se guardan en arreglos planos
  separados, cada uno indexado por el mismo índice de casilla.
- Qué decide el bitboard: 256 casillas caben en cuatro palabras de 64 bits, y una lectura o
  escritura es un desplazamiento y una máscara.
- Qué decide el arreglo de ocupantes: una captura es una consulta en tabla, no una comparación con
  las coordenadas del otro caballo, y el tablero debe mantener la tabla en cada movimiento que
  continúa la partida.
- Qué quedó fijo: el motor posee el tablero y ambos caballos mediante tres miembros `unique_ptr`, y
  `Board` sigue siendo una clase con métodos.
- Qué prueban los tamaños: el tablero crece 32 bytes y el motor sigue en 24.
- Qué prueba el número de reservas: 30,000 peticiones siguen siendo 30,000, porque las tres llamadas
  a `make_unique` no cambiaron.
- Qué prueba el tiempo de reloj: la ejecución no se acelera, porque el formato domina y el
  tablero es pequeño en ambos casos.
- Qué prueba el código objeto: las operaciones de bits y las escrituras de ocupantes cuestan 305
  bytes de texto en `game.o`.
- Qué prueba la transcripción: el comportamiento no cambió.
- Qué no prueba la lección: que este layout sea más rápido en general, que el layout elimine el
  asignador, o que los fallos de caché sean irrelevantes en todas partes.
- Qué le corresponde a la lección siguiente: puertos y adaptadores, `construction-20`.

Una respuesta sólida mantiene cuatro cosas separadas: **qué promete el layout** (campos agrupados
por acceso), **qué quedó fijo** (propiedad, métodos, tamaño del motor, reservas), **qué paga el
compilador** (305 bytes de texto y un tablero más grande) y **qué puede ver una ejecución** (la
transcripción sin cambios, el tiempo sin cambios, el número de reservas sin cambios). Una respuesta
débil dice "el diseño orientado a datos es más rápido" o "el motor guarda su estado en línea"; ambas
son falsas para esta línea base.

Luego contrasta tu registro con los criterios de aceptación de la autoevaluación.

## Autoevaluación (binaria)

- [ ] Comparé los dos layouts campo por campo y nombré los dos archivos que difieren.
- [ ] Predije las tablas de layout y de perfil antes de ejecutar.
- [ ] Reproduje las 20 trazas en ambas líneas base.
- [ ] Medí `sizeof(Board<8, 8>)` en 256 y 288.
- [ ] Medí `sizeof(GameEngine<8, 8>)` en 24 en ambas.
- [ ] Conté las reservas en ambos lados: 30,000 y 30,000.
- [ ] Ejecuté 10,000 partidas en ambos lados y registré el tiempo transcurrido.
- [ ] Medí el texto de `game.o` en 4,656 y 4,961, y el del binario en 9,668 y 9,972.
- [ ] Expliqué por qué el tablero creció 32 bytes: el bitboard ahorra 224, el arreglo de ocupantes
      agrega 256.
- [ ] Expliqué por qué el tamaño del motor y el número de reservas no se movieron: la propiedad no
      cambió.
- [ ] Expliqué por qué el tiempo de reloj no se movió: el formato domina y el tablero es pequeño.
- [ ] Nombré el invariante de ocupantes y el costo de legibilidad del bitboard.
- [ ] No edité ningún archivo de código versionado.
- [ ] Completé `EVIDENCE-es.md` §1–§9 y la autoevaluación en §11.

Estos son criterios de aceptación. Si una casilla queda sin marcar, escribe la siguiente acción más
pequeña en `EVIDENCE-es.md` §9.

## Ideas equivocadas que esta lección refuta

| Idea equivocada | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| "El motor ahora guarda su estado en línea." | Los miembros siguen siendo tres `unique_ptr`; el motor mide 24 bytes en ambas. | Lee las declaraciones de miembros y mide `sizeof`. |
| "Las reservas de memoria bajaron a cero." | Cada partida sigue llamando a `make_unique` tres veces: 30,000 para 10,000 partidas. | Ejecuta la sonda de reservas en ambos lados. |
| "El motor creció a 368 bytes." | Mide 24 bytes en ambas líneas base. | Mide `sizeof(GameEngine<8, 8>)` en cada una. |
| "Los métodos se volvieron funciones libres." | `Board` sigue siendo una clase, y las operaciones siguen siendo métodos. | Lee `board.h` y los sitios de llamada. |
| "Un diseño orientado a datos debe eliminar el heap." | El layout y la propiedad son decisiones separadas; esta lección mantiene fija la propiedad. | Cambia una decisión por vez y atribuye los números. |
| "El bitboard hace que todo el tablero sea más pequeño." | Visitadas baja de 256 a 32 bytes, pero ocupantes agrega 256; el tablero crece a 288. | Compara los campos, luego el objeto completo. |
| "El cambio de layout hizo más rápido el programa." | Los rangos medidos se solapan; la diferencia es ruido. | Reporta el rango, no una sola muestra. |
| "El cambio de layout ahorró memoria." | El tablero es 32 bytes más grande y el motor no cambia. | Suma el tablero y el motor antes de comparar. |
| "`sizeof(GameEngine<8, 8>)` incluye el tablero." | Incluye solo los punteros; el tablero está en un bloque separado del heap. | Lee los miembros, no solo el tamaño. |
| "El arreglo de ocupantes es gratis porque reemplaza una comparación." | Agrega 256 bytes, escribe en cada movimiento y hace crecer el texto objeto. | Cuenta los bytes, las escrituras y el texto. |
| "Struct-of-arrays siempre es mejor que array-of-structs." | Depende del patrón de acceso y del tamaño del conjunto de trabajo. Aquí el tablero es pequeño en ambos casos. | Nombra el patrón de acceso antes de elegir un layout. |
| "Redirigir stdout a `/dev/null` elimina el costo de formateo." | `/dev/null` elimina el costo de escritura, no el costo de formateo. | Di qué incluye el banco: construcción, generación, formateo. |
| "Tres reservas por partida son 30,000 para 10,000 partidas, así que debe notarse." | La lección mide la ejecución completa; el número de reservas por sí solo no prueba una mejora de velocidad. | Reporta ambos números y explica la diferencia. |
| "La transcripción prueba que el layout es mejor." | Prueba que el comportamiento no cambió; no dice nada sobre el costo. | Combina la comprobación de trazas con el perfil. |
| "El bitboard es solo una optimización." | Es una decisión de representación con un costo de legibilidad y un intercambio de tamaño. | Explica la correspondencia y los tamaños de campo. |
| "Una clase no puede ser orientada a datos." | El layout vive dentro de la clase; la interfaz pública no cambia. | Separa representación de propiedad e interfaz. |
| "La línea base 19 cambió las reglas." | Las 20 trazas coinciden; solo difieren `board.h` y `game_engine.h`. | Compara los archivos y ejecuta las trazas. |

## Términos usados aquí

| Término | Significado en esta lección |
|---|---|
| Layout (disposición) | Cómo se ordenan en memoria los campos de un valor: orden, tamaño, relleno y agrupación. |
| Bitboard | Representación que guarda un bit por casilla en un arreglo de palabras de máquina. |
| Struct-of-arrays | Agrupar campos por patrón de acceso en arreglos paralelos en lugar de structs por registro. |
| Localidad espacial | Tendencia de las direcciones cercanas a usarse juntas en el tiempo. |
| Localidad temporal | Reutilizar la misma dirección u objeto en operaciones cercanas. |
| Conjunto de trabajo | Los bytes que una computación toca realmente durante una ejecución. |
| Reserva en el heap | Una petición al asignador, aquí mediante `std::make_unique`. |
| Texto objeto | Los bytes de código máquina de un archivo objeto o binario, informados por `size`. |
| Regla de una sola variable | Cambiar una decisión de diseño por comparación para poder atribuir los números. |
| Traza de referencia | Una transcripción guardada que el programa debe reproducir byte a byte. |

## Qué significa "terminado" aquí

Terminaste cuando todas las casillas de la **Autoevaluación (binaria)** anterior están marcadas y
`EVIDENCE-es.md` §1–§9 y §11 están completos con tus propias mediciones. No hay puntaje. Si una
casilla queda sin marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §9 y hazla
antes de seguir.

El criterio no es "el código es orientado a datos". Es "puedes ejecutar el perfil, reportar el
tamaño del tablero, el tamaño del motor, el número de reservas, el tiempo de reloj y el texto
objeto, y explicar qué números se movieron mientras la propiedad quedaba fija".

## Siguiente paso

`construction-20` se ocupa de puertos y adaptadores. El motor sigue imprimiendo directamente con
`std::printf`, así que el núcleo no puede ejecutarse sin una consola. Un diseño hexagonal pone una
interfaz entre el juego y su salida, lo que permite que el mismo núcleo sirva a un adaptador de
consola y a un adaptador de prueba. La pregunta del layout queda resuelta aquí de forma acotada:
la representación del tablero cambió, el tablero creció 32 bytes, y el motor, el asignador y el
reloj no se movieron. La lección siguiente pregunta quién tiene permitido hablar con ese estado.

Si prefieres el otro enfoque primero, `construction-18` se ocupa de pureza e inmutabilidad.
Pregunta si el tablero debería reemplazarse en lugar de mutarse, una respuesta distinta a la misma
pregunta de costo.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog-es.md) para el recorrido
completo.

Si faltaba cualquiera de las líneas base, no empieces `construction-19`. La comparación necesita
ambos lados.

## Referencias

- [`construction-17` — Standard Template Library and
  algorithms](../construction-17-stl/lesson-es.md), la línea base contra la que compara esta
  lección.
- [Código de `construction-17`](../construction-17-stl/code/) — `board.h` y `game_engine.h` son el
  lado izquierdo de cada tabla aquí.
- [`construction-16` — Propiedad](../construction-16-ownership/lesson-es.md) — el modelo de
  propiedad que se mantiene fijo en esta lección.
- [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — tablero, orden de movimientos,
  captura, trampa, empate y límite de 200 movimientos.
- [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md) — el contrato de
  salida a nivel de byte.
- [`common/traces/`](../../../common/traces/) — las 20 trazas de referencia.
- [`EVIDENCE-es.md`](EVIDENCE-es.md) — el registro del aprendiz para esta lección.
- [cppreference: `std::array`](https://en.cppreference.com/w/cpp/container/array) y
  [cppreference: `std::unique_ptr`](https://en.cppreference.com/w/cpp/memory/unique_ptr) — los dos
  tipos de biblioteca que definen los layouts comparados aquí.
