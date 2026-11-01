# Portar a otro lenguaje (portar)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** la sintaxis es desechable · **Tiempo:** unos 195 minutos · **Estado:** borrador
**Lectura:** ~21 minutos

## Por qué existe esta lección

`construction-22` terminó con un contrato: cuatro resultados con nombre, un valor que lleva la
respuesta y quien llama decide qué pasa después. Todo el contrato vivía en tipos de C++: un
`enum class`, un `struct` y un `switch` sin `default`.

Esta lección hace una pregunta más estrecha que «¿sabes escribir Python?». Pregunta qué parte de ese
contrato era el juego y qué parte era el lenguaje.

La línea base de C++ tiene 1.071 líneas en 20 archivos. El port tiene 245 líneas en 2. Las mismas 20
semillas producen los mismos bytes.

La frase que conviene retener:

> Un port no es una traducción del código. Es una nueva derivación del comportamiento a partir de
> las reglas. La sintaxis es desechable; las reglas y la transcripción son el contrato.

Tres notas honestas antes de la comparación.

1. **Los recuentos de líneas no son una medida de calidad.** La línea base de C++ incluye dos
   programas de sonda, un adaptador de registro y un ejecutor en paralelo que el port no tiene.
   Parte de las 826 líneas que faltan es ceremonia; parte es evidencia. Un programa más corto no es
   automáticamente mejor, y esta lección nunca lo afirma.
2. **Las trazas no se mueven.** Las 20 trazas de referencia coinciden byte a byte, medidas en esta
   máquina, antes y después de la prueba de cambio. Ese es el contrato de comportamiento, y el port
   no lo toca.
3. **Esta no es la primera implementación en Python.**
   [`common/harness/reference.py`](../../../common/harness/reference.py) tiene 251 líneas de Python
   y produjo las trazas de referencia. El port no lo importa ni lo ejecuta. Pero los dos archivos
   son primos cercanos: mismas constantes, mismo orden de funciones, mismo bucle. Que coincidan es
   evidencia de que ambos codifican las mismas reglas; no es prueba independiente de que las reglas
   sean correctas. Conserva esa distinción durante toda la lección.

Lo que esta lección no es: no es «Python es mejor», no es «menos código es mejor», no es «los tipos
son innecesarios». Es una medición de lo que sobrevive cuando se quitan el compilador, las clases y
los adaptadores, y solo quedan las reglas y la transcripción.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar por qué la sintaxis es desechable y el comportamiento es el contrato;
- reproducir las trazas desde el port de Python y nombrar qué cambió.

## El concepto: cuatro capas, un contrato

Un port tiene cuatro capas. Solo una debe coincidir exactamente, y confundir las capas es la razón
habitual para dar un port por terminado cuando no lo está.

| Capa | Qué es | ¿Debe coincidir? | En la línea base de C++ | En el port |
|---|---|---|---|---|
| Comportamiento | los bytes en stdout para cada semilla | sí, exactamente | 20 trazas | 20 trazas |
| Reglas | LCG, orden de movimientos, captura, visitadas, trampa, empate | sí, como reglas | `game_engine.h`, `board.h` | `run_game`, `build_move_order` |
| Representación | cómo se guarda el estado | no | `Board`, `Knight`, `Position`, `MoveResult` | tuplas, un conjunto, una lista |
| Proceso | argv, stdout, estado de salida | en parte, como promesa | `main` devuelve 2 | `argparse` sale con 2 |

La transcripción es la única capa que se puede comparar mecánicamente. Las reglas explican por qué
coincide. La representación es libre. La interfaz del proceso es una promesa para quien ejecuta el
programa, no para el juego.

### Qué significa «el mismo comportamiento»

Los mismos bytes, en el mismo orden, con el mismo salto de línea final, para las mismas 20 semillas
y el mismo nombre de jugador. No «el mismo ganador». No «se parece». `cmp` calla o no es.

### Qué sostenía la capa de C++ y adónde fue

| C++ | Python | Qué pasó con el concepto |
|---|---|---|
| Clase `Board` con un invariante | constantes de módulo y `is_inside_board` | el invariante se vuelve un predicado |
| `Position` con fila y columna | `tuple[int, int]` | el tipo se vuelve una forma |
| `Knight` con nombre y posición | dos variables de posición y una cadena | la identidad se vuelve una etiqueta |
| `MoveGenerator` | `KNIGHT_MOVE_CHANGES` y `build_move_order` | el objeto se vuelve datos |
| `RandomSequence` | `RandomSequence` | se conserva, más pequeño |
| `TranscriptPort` y dos adaptadores | `list[str]`, unida al final | el puerto se vuelve una lista |
| `MoveOutcome`, `MoveResult`, `switch` | orden de ramas y fragmentos de cadena | el enumerado se vuelve palabras en la traza |
| `GameEngine::playTurn` | el cuerpo de `run_game` | el método se vuelve un bucle |

Lee las dos últimas filas dos veces. Los cuatro nombres de resultado no desaparecieron. `outside`,
`visited`, `capture` y `legal` siguen en el programa, como palabras literales en las líneas de la
transcripción. El contrato sobrevivió como comportamiento, no como tipo. Eso es la lección completa
en una frase.

### Lo que Python no te da

- **No hay compilador.** `-Wall -Wextra -Werror` desaparece. Una rama equivocada es una
  transcripción equivocada en ejecución, no un error de compilación.
- **No hay `switch` exhaustivo.** Añadir un quinto resultado en C++ rompe la compilación hasta que
  cada `switch` lo trate. En Python, una rama que falta es un `elif` que falta, y la traza discrepa
  en silencio.
- **No hay `MoveResult`.** Ningún valor cruza el límite de una función con la respuesta. El bucle
  escribe en la transcripción directamente y hace `break`. El «quien llama decide» de
  `construction-22` se convierte en «el bucle decide en línea».
- **No hay `const`.** Nada impide que una función mute un conjunto que recibió. El port no lo hace;
  el lenguaje no lo pregunta.
- **Los tipos son pistas.** `tuple[int, int]` documenta la intención y no comprueba nada en
  ejecución.

Nada de esto es un defecto de Python. Es el precio de la capa que el port decidió quitar: el
contrato en tiempo de compilación. La lección mide ese precio en lugar de esconderlo.

## Antes de empezar

- [`construction-09`](../construction-09-abstract-data-type/lesson.md) o
  [`construction-12`](../construction-12-polymorphism/lesson.md) — el prerrequisito formal
  del catálogo. Basta con leer un tipo valor pequeño, una función y un bucle, sin diseñar ninguno.
- [`construction-22`](../construction-22-error-contracts/lesson.md) — recomendada, no exigida por el
  catálogo. Aporta el contrato que este port debe conservar: cuatro resultados, primero los límites,
  captura antes que visitada y quien llama decide. Si la omites, lee
  [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) §7–§9 antes del Paso 1.
- Python 3.10 o posterior. El port usa `tuple[int, int]` y `str | None`. Comprueba con
  `python3 --version`.
- Un compilador de C++20 (`g++` o `clang++`) solo para el lado de la comparación. Si no lo tienes,
  las trazas de referencia son la salida registrada de C++ y puedes usarlas como el otro lado.
- Ningún paquete de Python de terceros. `argparse` está en la biblioteca estándar.

### Qué necesitas

- El port: [`code/game.py`](code/game.py), [`code/main.py`](code/main.py) y [`run.sh`](run.sh).
- El lado de C++, para el diff:
  [`../construction-22-error-contracts/code/`](../construction-22-error-contracts/code/) y su
  [`run.sh`](../construction-22-error-contracts/run.sh).
- El oráculo independiente: [`common/harness/reference.py`](../../../common/harness/reference.py).
- Las reglas: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — el LCG en §4, el
  orden de movimientos en §6, la legalidad en §7, las casillas visitadas en §8, la captura en §9, la
  trampa y el empate en §10.
- El formato de transcripción:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- Las 20 trazas: [`common/traces/`](../../../common/traces/).
- Esta plantilla de registro: [`EVIDENCE-es.md`](EVIDENCE-es.md).

Comprueba que los artefactos están presentes antes de empezar, desde este directorio de la lección:

```bash
test -f code/game.py &&
test -f code/main.py &&
test -f run.sh &&
test -f ../../../common/harness/reference.py &&
test -f ../../../common/traces/seed-0001.txt &&
test -f ../construction-22-error-contracts/code/main.cpp &&
test -f ../construction-22-error-contracts/run.sh
```

Si falta un archivo, la comparación no tiene segundo lado. Anótalo en `EVIDENCE-es.md` §11 y vuelve
cuando esté disponible.

## Paso 1 — Inventariar el port (~20 min)

Empieza por el tamaño y la forma:

```bash
wc -l code/game.py code/main.py ../../../common/harness/reference.py
```

```text
  211 code/game.py
   34 code/main.py
  251 ../../../common/harness/reference.py
  496 total
```

Dos archivos, 245 líneas para el port. El oráculo es un archivo de 251 líneas.

Ahora las definiciones de nivel superior:

```bash
grep -n '^class \|^def ' code/game.py code/main.py
```

```text
code/game.py:27:class RandomSequence:
code/game.py:43:def format_position(position: tuple[int, int]) -> str:
code/game.py:47:def is_inside_board(position: tuple[int, int]) -> bool:
code/game.py:55:def convert_cell_number_to_position(cell_number: int) -> tuple[int, int]:
code/game.py:61:def create_initial_positions(
code/game.py:74:def create_initial_rolls(
code/game.py:84:def build_move_order(starting_move_number: int) -> list[int]:
code/game.py:91:def other_knight(active_knight: str) -> str:
code/game.py:95:def run_game(
code/main.py:6:def parse_arguments() -> argparse.Namespace:
code/main.py:27:def main() -> int:
```

Una clase y ocho funciones en `game.py`; dos funciones en `main.py`. Sin cabecera, sin unidad de
traducción, sin paso de compilación y sin importaciones de terceros.

Ahora la parte que no debe moverse. El LCG:

```bash
grep -n 'RANDOM_\|KNIGHT_MOVE_CHANGES\|DEFAULT_MAXIMUM_MOVE_COUNT' code/game.py
```

```text
5:KNIGHT_MOVE_CHANGES = (
20:DEFAULT_MAXIMUM_MOVE_COUNT = 200
22:RANDOM_MULTIPLIER = 6364136223846793005
23:RANDOM_INCREMENT = 1442695040888963407
24:RANDOM_MASK = (1 << 64) - 1
```

Compáralo con el lado de C++:

```bash
grep -rn '6364136223846793005\|1442695040888963407\|200' \
    ../construction-22-error-contracts/code/random_sequence.cpp \
    ../construction-22-error-contracts/code/game_engine.h
```

Las constantes son los mismos enteros decimales. La máscara es el mismo `2^64 - 1`, escrito como
desplazamiento en lugar de literal. Un port que cambiara un dígito seguiría ejecutándose, seguiría
produciendo una transcripción y fallaría todas las trazas.

Recuentos de esta línea base. La regla de conteo se enuncia para que puedas reproducirla:

- Archivos en el port: 2 (más `run.sh`, que no es código).
- Líneas de código: 245 (`game.py` 211, `main.py` 34).
- Cabeceras: 0.
- Unidades de traducción: 0.
- Pasos de compilación: 0.
- Dependencias de terceros: 0.
- Definiciones de nivel superior: 11 (1 clase, 10 funciones).
- Valores en el LCG: 3 constantes y un estado de 64 bits.
- Desplazamientos de caballo: 8, en orden fijo.
- Trazas de referencia: 20.

Después responde en `EVIDENCE-es.md` §2:

1. ¿Qué archivo contiene las constantes del LCG y qué archivo contiene el bucle del juego?
2. ¿Cuántas líneas tiene cada archivo y cuál es el total?
3. ¿Dónde guarda la línea base de C++ los cuatro resultados? ¿Dónde los guarda el port?
4. ¿Qué reemplaza a `Board`, `Knight` y `Position`?
5. ¿Qué reemplaza a `TranscriptPort` y sus dos adaptadores?
6. ¿Qué reemplaza a `MoveResult`?
7. ¿Cuántas definiciones de nivel superior tiene `game.py`?
8. ¿Qué archivo decide el estado de salida del proceso y mediante qué llamada?

La pregunta 8 tiene una respuesta precisa: lo decide `argparse`. `parse_arguments` no captura el
error y `main` no devuelve 2; `argparse` imprime el uso y llama a `sys.exit(2)` dentro de la
biblioteca. El estado es el mismo que en la línea base de C++; el mecanismo no.

## Paso 2 — Predecir antes de ejecutar (~25 min)

Escribe estas predicciones antes de ejecutar nada. No leas el Paso 3 primero.

### 2.1 La tabla de estructura

| Pregunta | Mi predicción |
|---|---|
| Archivos en la línea base de C++ | |
| Archivos en el port | |
| Líneas de código en la línea base de C++ | |
| Líneas de código en el port | |
| Cabeceras en el port | |
| Pasos de compilación en el port | |
| Definiciones de nivel superior en `game.py` | |
| Trazas de referencia que coinciden | |

La fila fácil de fallar es «definiciones de nivel superior». El port no tiene `Board`, ni `Knight`,
ni `GameEngine`, pero sigue necesitando el LCG, las posiciones iniciales, las tiradas iniciales, el
orden de movimientos, la comprobación de límites y el bucle. Quitar clases no quita funciones.

### 2.2 La tabla de comportamiento

| Pregunta | Mi predicción |
|---|---|
| Semillas que coinciden, de 20 | |
| Trazas que terminan en captura | |
| Trazas que terminan en trampa | |
| Trazas que terminan en empate | |
| Partida más larga, en movimientos | |
| Trazas con al menos una línea `illegal=visited` | |

Predice las tres últimas razonando sobre las reglas, no leyendo las trazas. Una partida termina en
captura o en trampa; un empate necesita 200 movimientos. El tablero es de 8×8 y ambos caballos
comparten el conjunto de visitadas, así que una partida larga es posible pero no segura.

### 2.3 La tabla de tiempos

| Pregunta | Mi predicción |
|---|---|
| 20 partidas con `run.sh`, tiempo de reloj | |
| 20 partidas en un solo proceso de Python | |
| 20 procesos de C++, tiempo de reloj | |
| 20 arranques vacíos de `python3 -c 'pass'` | |

Predice el número en proceso muy por debajo del número con `run.sh`. Si tus cuatro números están
cerca, estás prediciendo cómputo; la diferencia es sobre todo arranque.

### 2.4 La tabla de la prueba de cambio

| Pregunta | Mi predicción |
|---|---|
| Trazas que fallan si la captura se comprueba después de visitada | |
| Trazas que fallan si se invierte el ganador de la trampa | |
| Trazas que fallan si el límite de empate pasa de 200 a 199 | |

Para la tercera fila, piensa antes de responder: una regla solo puede romper una traza que la
alcanza.

## Paso 3 — Ejecutar el port (~50 min)

### 3.1 Las 20 trazas

Desde este directorio de la lección:

```bash
for trace in ../../../common/traces/seed-*.txt; do
    traceName="$(basename "${trace}" .txt)"
    seedText="${traceName#seed-}"
    seedNumber="$((10#${seedText}))"
    ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

Un bucle silencioso es 20 de 20. Si `cmp` imprime un desplazamiento de byte, detente: esa es otra
línea base, distinta de la que describe esta lección.

### 3.2 El lado de C++

Las trazas son la salida registrada de C++, pero ejecuta la línea base una vez para ver los dos
programas juntos:

```bash
diff <(./run.sh --seed 7 --name Learner) \
     <(../construction-22-error-contracts/run.sh --seed 7 --name Learner)
```

Sin salida. Dos lenguajes, una transcripción. Repite con una semilla que termina en trampa, por
ejemplo la 1, y con una que termina en captura, por ejemplo la 2.

Si el `run.sh` de C++ tiene que compilar, usa CMake cuando está disponible y recurre a una
compilación directa cuando no. La primera invocación es lenta; la comparación no trata sobre esa
compilación.

### 3.3 La tercera implementación

```bash
python3 ../../../common/harness/reference.py --seed 7 --name Learner \
    | cmp - <(./run.sh --seed 7 --name Learner)
```

Sin salida. El oráculo que generó las trazas y el port coinciden en la semilla 7.

Ahora la advertencia honesta, como comando:

```bash
diff -u ../../../common/harness/reference.py code/game.py | head -40
```

El diff es sobre todo de nombres: `DeterministicRandom` pasa a `RandomSequence`, `random_below` pasa
a `below`, `position_is_inside_board` pasa a `is_inside_board`, y el bucle se reformatea. No son dos
diseños independientes que convergen. Es un diseño escrito dos veces en el mismo lenguaje. La
coincidencia de trazas es real; la independencia es parcial. Dilo en `EVIDENCE-es.md` §5.

### 3.4 Los tiempos, medidos con honestidad

```bash
start=$(date +%s.%N)
for n in $(seq 1 20); do ./run.sh --seed "$n" --name Learner > /dev/null; done
end=$(date +%s.%N)
echo "$end - $start" | bc
```

En esta máquina, con Python 3.14.8:

```text
.867748884
```

Veinte partidas mediante veinte procesos `run.sh` separados tardan unos 0,87 s. Las mismas 20
partidas con la línea base de C++ tardan unos 0,03 s:

```bash
start=$(date +%s.%N)
for n in $(seq 1 20); do ../construction-22-error-contracts/run.sh \
    --seed "$n" --name Learner > /dev/null; done
end=$(date +%s.%N)
echo "$end - $start" | bc
```

```text
.026168650
```

No leas eso como «Python es 30 veces más lento». Mide de qué está hecho el número:

```bash
start=$(date +%s.%N)
for n in $(seq 1 20); do python3 -c 'pass'; done
end=$(date +%s.%N)
echo "$end - $start" | bc
```

```text
.256494341
```

Veinte intérpretes vacíos ya cuestan 0,26 s. Ahora el trabajo del juego sin el arranque de procesos:

```bash
start=$(date +%s.%N)
python3 - <<'EOF'
import sys
sys.path.insert(0, "code")
from game import run_game
for seed in range(1, 21):
    run_game(seed, "Learner")
EOF
end=$(date +%s.%N)
echo "$end - $start" | bc
```

```text
.015848224
```

Veinte partidas completas en un proceso tardan unos 0,016 s, menos de un milisegundo por partida.
Los 0,87 s son casi por completo veinte arranques de intérprete más el envoltorio `run.sh`. La
afirmación honesta es: en esta máquina, la cifra de Python está dominada por el arranque de
procesos, así que no es una comparación de núcleos. Un banco de pruebas necesitaría un bucle dentro
de un proceso en ambos lados, y esta lección no lo ejecuta.

### 3.5 La ruta de argumentos

```bash
./run.sh --seed 1
echo "exit=$?"
```

```text
usage: main.py [-h] --seed INTEGER --name TEXT
main.py: error: the following arguments are required: --name
exit=2
```

Compara con la línea base de C++, que también sale con 2:

```bash
../construction-22-error-contracts/run.sh --seed 1
echo "exit=$?"
```

Mismo estado, distinto dueño. En C++, `main` devolvió 2 y el proceso terminó normalmente. En Python,
`argparse` llamó a `sys.exit(2)` desde dentro de la biblioteca. `construction-22` insistió en que el
núcleo nunca llama a `exit`; la capa de argumentos del port delega esa decisión. La transcripción no
se ve afectada porque no empieza ninguna partida.

### 3.6 Predicción y observación

Rellena `EVIDENCE-es.md` §4.6 antes de seguir. La distancia es el resultado, incluso cuando es cero.

## Paso 4 — Comparar (~30 min)

Abre `game.py` y `main.py` junto a `game_engine.h`, `board.h`, `knight.h` y `game.cpp`. Cinco
hechos, leídos contra las mediciones.

1. **Las reglas están todas, en el mismo orden.** Primero los límites, después la captura, después
   las visitadas, después legal. El movimiento inicial y la rotación son los mismos. El LCG se llama
   el mismo número de veces en el mismo orden.
2. **La representación desapareció.** No hay `Board`, ni `Knight`, ni `Position`, ni `MoveResult`.
   Una posición es una tupla de dos enteros, las casillas visitadas son un conjunto de tuplas y la
   transcripción es una lista de cadenas.
3. **El contrato de error se volvió flujo de control.** En C++, `tryMove` devolvía un valor y
   `playTurn` ramificaba sobre él. En Python, el bucle añade la línea, incrementa el contador, fija
   el ganador y hace `break`, todo en el mismo bloque. No hay valor que inspeccionar ni segundo
   consumidor.
4. **El trabajo del compilador se mudó a las trazas.** Los cuatro resultados siguen distinguidos,
   pero por el texto `illegal=outside`, `illegal=visited`, `capture=...` y `legal`. Un caso que
   falta produce una transcripción equivocada, no un error de compilación.
5. **La interfaz del proceso es casi la misma.** `--seed` y `--name` conservan sus nombres, stdout
   conserva sus bytes y un argumento inválido sigue saliendo con 2. `--max-moves` no está en el
   port; el oráculo sí lo tiene.

### 4.1 Tres cosas que cambiaron

Nómbralas con tus palabras en `EVIDENCE-es.md` §5, con una línea de evidencia para cada una.

1. **Representación y propiedad.** `Board`, `Knight`, `Position`, `MoveGenerator`, `TranscriptPort`
   y los dos adaptadores se reducen a constantes de módulo, tuplas, un conjunto, una lista y
   funciones. No hay objeto que sea dueño del invariante del tablero; `is_inside_board` comprueba
   una coordenada cuando se le pide.
2. **El contrato en tiempo de compilación.** `MoveOutcome` con cuatro valores, un `switch`
   exhaustivo y `MoveResult` con el destino se reemplazan por orden de ramas y cadenas literales.
   Nada comprueba el port al compilar, y no hay un `evaluate_move` público que sondear.
3. **La compilación y el proceso.** Un binario compilado pasa a ser un guion interpretado detrás de
   `run.sh`. Cada ejecución paga el arranque del intérprete, y `argparse` es dueño del error de uso
   y llama a `sys.exit(2)`.

Un cuarto cambio merece nombre si lo quieres: 1.071 líneas en 20 archivos pasan a 245 en 2. Es una
consecuencia de los tres primeros, no una decisión independiente.

### 4.2 Tres cosas que no cambiaron

1. **El generador.** `6364136223846793005`, `1442695040888963407`, la máscara de `2^64`, una
   actualización de estado por tirada, el mismo número de tiradas en el mismo orden.
2. **El orden de movimientos y el orden de comprobación.** Ocho desplazamientos en la misma
   secuencia, la misma rotación desde el movimiento inicial, límites antes que captura, captura
   antes que visitada, visitada antes que legal.
3. **La transcripción.** Cada línea de cabecera, cada `turn=`, cada `try=`, cada `move=`, la línea
   de trampa, el límite de 200 movimientos, `winner=` y `moves=`. Las 20 trazas coinciden byte a
   byte.

También sin cambios: los nombres de línea de comandos `--seed` y `--name`, la salida solo por stdout
y el estado 2 para argumentos inválidos.

### 4.3 Lo que el port quitó y no reemplazó

Esta es la parte que un port esconde si nadie mira. Faltan cinco cosas, y cada una era evidencia
que la línea base de C++ tenía.

| Quitado | Qué aportaba | Qué lo reemplaza |
|---|---|---|
| `parallel_runner.cpp` | dos partidas en paralelo, determinismo comprobado | nada; el port es de un solo proceso |
| `error_probe.cpp` y `evaluate_move` | los cuatro resultados sin jugar una partida | nada; los resultados solo existen como cadenas |
| `adapter_probe.cpp` | el adaptador de registro ejercitado sin la consola | nada; solo queda stdout |
| `RecordingTranscript` | la transcripción como dato, inspeccionable en proceso | nada; la transcripción es stdout |
| `--max-moves` | una partida corta, para llegar al empate a propósito | nada; el límite es una constante |

El oráculo conserva `--max-moves`; el port no. La consecuencia es medible: la rama de empate de
`run_game` no la alcanzan las 20 semillas y, sin la opción, tampoco se puede alcanzar a propósito.
Es un hueco en la evidencia del port, no en sus reglas. Anótalo en §7.

### 4.4 El diff de comportamiento

```bash
for trace in ../../../common/traces/seed-*.txt; do
    traceName="$(basename "${trace}" .txt)"
    seedText="${traceName#seed-}"
    seedNumber="$((10#${seedText}))"
    diff <(./run.sh --seed "${seedNumber}" --name Learner) "${trace}"
done
```

Sin salida, 20 veces. Ese es el único diff que importa para el comportamiento. El diff estructural
no es un diff: no hay correspondencia línea a línea entre `game_engine.h` y `run_game`. Un port es
una reescritura que coincide en la salida.

## Paso 5 — Prueba de cambio: una regla, solo en el port (~40 min)

El criterio de aceptación nombra la prueba de cambio: **ejecutar el port con las 20 semillas,
comparar con la línea base de C++, cambiar una regla solo en el port, observar qué trazas fallan y
restaurarla.**

Nunca edites el directorio versionado `code/`. Cópialo:

```bash
rm -rf /tmp/port-change
cp -r code /tmp/port-change
```

`main.py` importa `game` desde su propio directorio, así que ejecutar `/tmp/port-change/main.py` usa
la copia sin ninguna variable de entorno.

### 5.1 Cambio A: intercambiar captura y visitada

En `/tmp/port-change/game.py`, el bloque de captura son las líneas 160–174 y el bloque de visitada
son las líneas 176–181. Mueve el bloque de visitada por encima del bloque de captura. No cambies
nada más.

Esta es la regla que `construction-22` dijo que no era una elección de estilo. Ahora puedes ver
cuánto cuesta en un lenguaje sin compilador que la imponga.

Ejecuta las 20 y muestra los fallos:

```bash
for n in $(seq 1 20); do
    if ! python3 /tmp/port-change/main.py --seed "$n" --name Learner \
        | cmp -s - "../../../common/traces/seed-$(printf '%04d' "$n").txt"; then
        echo "seed $n differs"
    fi
done
```

Medido en esta máquina:

```text
seed 2 differs
seed 3 differs
seed 7 differs
seed 8 differs
seed 9 differs
seed 10 differs
seed 13 differs
seed 14 differs
seed 15 differs
seed 17 differs
seed 19 differs
```

Once de 20. Compara esa lista con las partidas que terminan en captura:

```bash
grep -l 'capture=' ../../../common/traces/seed-*.txt
```

Los mismos once archivos. La explicación es estrecha: el intercambio cambia una respuesta solo
cuando un destino es a la vez la casilla del otro caballo y ya está visitado. En estas once partidas
la captura cae en una casilla visitada, y desde esa línea la transcripción diverge. Las otras nueve
partidas terminan en trampa y nunca intentan una captura, así que no pueden ver el cambio.

### 5.2 Cambio B: invertir el ganador de la trampa

Restaura la copia primero y haz un cambio más pequeño:

```bash
rm -rf /tmp/port-change
cp -r code /tmp/port-change
```

La línea 204 de `game.py` es:

```python
winner_knight = other
```

Cámbiala por `winner_knight = active_knight`. Ejecuta el mismo bucle.

Medido en esta máquina:

```text
seed 1 differs
seed 4 differs
seed 5 differs
seed 6 differs
seed 11 differs
seed 12 differs
seed 16 differs
seed 18 differs
seed 20 differs
```

Nueve de 20, exactamente las partidas con una línea `no_legal_move`. Una regla solo la puede
observar una traza que la alcanza.

### 5.3 Restaurar y confirmar

```bash
rm -rf /tmp/port-change
```

Después vuelve a ejecutar el bucle del Paso 3.1 en el directorio versionado. Vuelve a estar en
silencio: 20 de 20. No hubo que revertir nada, porque los archivos versionados nunca se tocaron.

### 5.4 Qué prueba la prueba de cambio y qué no

Prueba cuatro cosas.

1. Las trazas son un detector de reglas, y tienen resolución: 11 semillas ven el orden de captura, 9
   ven la trampa, 0 ven el empate.
2. El port no tiene guardia en tiempo de compilación. La comparación es la única guardia, y es
   externa.
3. Una regla que no puedes alcanzar es una regla que no has probado. La rama de empate es código, no
   evidencia.
4. El port y la línea base de C++ coinciden en cada semilla que ejercita las dos reglas que acabas
   de romper.

No prueba que el port sea correcto para otras semillas, que C++ y Python no compartan un error ni
que alguno sea rápido. Las dos implementaciones de Python son primas, así que una mala lectura
compartida de las reglas sobreviviría a ambas. Solo la coincidencia con las trazas de C++, o la
lectura de la especificación, detecta eso.

### 5.5 Un tercer cambio, si lo quieres

El límite de empate es `DEFAULT_MAXIMUM_MOVE_COUNT = 200`. Copia `code/` otra vez, cambia la
constante a 199 en la copia y ejecuta las 20 semillas. Predice el resultado antes de mirar: ninguna
traza llega a 200 movimientos, así que no debería fallar nada. Si tu predicción es «algunas trazas
fallan», la sorpresa es la lección: el límite es código real y comportamiento sin probar.

## Paso 6 — Comparar los números (~10 min)

| Métrica | C++ `construction-22` | Port de Python | Diferencia |
|---|---:|---:|---:|
| Archivos | 20 | 2 | −18 |
| Líneas de código | 1.071 | 245 | −826 |
| Cabeceras | 11 | 0 | −11 |
| Unidades de traducción (compilación de consola) | 6 | 0 | −6 |
| Pasos de compilación | 1 | 0 | −1 |
| Dependencias de terceros | 0 | 0 | 0 |
| Programas extra (sondas y ejecutor) | 3 | 0 | −3 |
| Trazas de referencia | 20 de 20 | 20 de 20 | 0 |
| 20 partidas, procesos separados | 0,026 s | 0,868 s | +0,842 s |
| 20 partidas, un proceso | no medido | 0,016 s | — |
| 20 arranques de intérprete | — | 0,256 s | — |
| Estado de salida con argumentos inválidos | 2 | 2 | 0 |

### 6.1 Qué compra

- Un comportamiento, dos lenguajes. Las reglas estaban en la especificación, no en el sistema de
  tipos de C++.
- Quien lee puede ver el juego completo en 245 líneas, de una sentada, sin compilar.
- El port es un segundo lector de la especificación. Donde C++ y Python discrepan, uno de los dos
  leyó mal las reglas.
- Sin cadena de herramientas. Un intérprete de Python 3.10 es toda la dependencia.

### 6.2 Qué cuesta

- El contrato en tiempo de compilación. Sin `switch` exhaustivo, sin `MoveResult`, sin `-Werror`.
- Las sondas y el adaptador de registro. El port no tiene forma de preguntar en proceso «qué
  devolvería este movimiento».
- El ejecutor en paralelo. La concurrencia de `construction-21` no sobrevive.
- El coste de arranque por ejecución, que domina cualquier medición con muchos procesos.
- La opción `--max-moves`, así que el empate no se puede alcanzar a propósito.

### 6.3 El hueco: el port es evidencia, no prueba

Merece la pena nombrar cuatro límites en `EVIDENCE-es.md` §7.

1. **El problema de los primos.** `reference.py` y `game.py` comparten constantes, nombres de
   funciones y forma del bucle. Que coincidan es más débil que la coincidencia entre C++ y Python.
2. **El empate es inalcanzable.** La partida de referencia más larga tiene 49 movimientos; el límite
   es 200. La rama que fija `winner_knight = "draw"` no se ejecuta en las 20 trazas.
3. **El contrato de error desapareció.** Los cuatro resultados existen como cadenas. No hay valor
   que quien llama pueda inspeccionar, así que la aceptación de `construction-22` no se puede
   repetir en el port.
4. **El tiempo es una medición de arranque.** 0,87 s frente a 0,03 s dice cuánto tardan veinte
   lanzamientos, no cuán rápido es el juego.

La frase precisa de aceptación es más estrecha que «el port funciona»:

> Quien aprende puede ejecutar el port de Python, hacer coincidir las 20 trazas de referencia,
> cambiar una regla solo en el port, explicar qué trazas fallan y por qué, restaurarla y nombrar
> tres cosas que cambiaron y tres que no.

Comportamiento igualado, reglas localizadas, representación libre. Nada sobre paridad de rendimiento
ni fidelidad de traducción.

## Paso 7 — Explicar (~15 min)

Escribe `EVIDENCE-es.md` §8. Dos partes.

**Primero, una frase.** Di qué sobrevivió al cambio de lenguaje y qué no, sin usar las palabras
*sintaxis*, *lenguaje*, *traducir*, *mejor*, *peor*, *más limpio* ni *más rápido*. Si tu frase
necesita una de ellas, has nombrado la superficie y no el contrato.

**Segundo, un párrafo.** Responde:

- Qué cambió: representación y propiedad; el contrato en tiempo de compilación; la compilación y el
  proceso.
- Qué quedó: el LCG, el orden de movimientos, el orden de comprobación, la transcripción, las 20
  trazas.
- Qué mostró la prueba de cambio: 11 semillas ven el orden de captura, 9 ven la trampa, 0 ven el
  empate.
- Qué quitó el port: las sondas, el adaptador de registro, el ejecutor en paralelo, `--max-moves`.
- Qué significan los números: 245 frente a 1.071 líneas; 0,87 s de arranque frente a 0,03 s de
  procesos.
- Qué prueban las trazas: dos implementaciones coinciden en 20 semillas, byte a byte.
- Qué no prueban: que alguna sea correcta en una semilla no vista, ni que la rama de empate
  funcione.
- Qué le toca a la lección siguiente: el proyecto final, `construction-24`.

Una respuesta fuerte mantiene cuatro cosas separadas: **cuáles son las reglas** (la especificación),
**qué registra la transcripción** (los bytes), **qué eligió la representación** (tuplas, un
conjunto, una lista) y **qué hace el proceso** (arranque del intérprete, `argparse`, estado 2). Una
respuesta débil dice «el port es el mismo programa en Python», lo cual es falso a nivel de tipos y
no está probado a nivel de reglas.

## Paso 8 — Criterio de aceptación y registro (~5 min)

El criterio de aceptación de esta lección es: **las mismas 20 trazas desde Python, y al menos tres
cosas que cambiaron y tres que no.**

Revisa tu registro contra él:

- [ ] Leí `game.py`, `main.py` y `run.sh`.
- [ ] Conté 245 líneas en 2 archivos y 1.071 en 20 para la línea base de C++.
- [ ] Encontré las tres constantes del LCG y los ocho desplazamientos en ambas implementaciones.
- [ ] Predije las tablas de estructura, comportamiento, tiempos y prueba de cambio antes de medir.
- [ ] Reproduje las 20 trazas desde el port, byte a byte.
- [ ] Comparé el port con la línea base de C++ en una semilla de captura y una de trampa.
- [ ] Ejecuté `reference.py` y expliqué por qué es un oráculo pero no un diseño independiente.
- [ ] Medí 20 partidas con `run.sh`, 20 en un proceso, 20 procesos de C++ y 20 arranques.
- [ ] Afirmé que el arranque domina la cifra de Python en lugar de reclamar una comparación de
  núcleos.
- [ ] Ejecuté la ruta de argumentos inválidos y vi el estado 2 en ambas implementaciones.
- [ ] Nombré tres cosas que cambiaron y tres que no, con evidencia.
- [ ] Intercambié captura y visitada en una copia y vi fallar 11 semillas.
- [ ] Invertí el ganador de la trampa en una copia y vi fallar 9 semillas.
- [ ] Expliqué por qué las otras semillas no pueden ver cada cambio.
- [ ] Registré que ninguna traza de referencia alcanza el empate a 200 movimientos.
- [ ] Restauré la copia y confirmé 20 de 20 otra vez.
- [ ] No edité ningún archivo de código versionado.
- [ ] Completé `EVIDENCE-es.md` §1–§10 y la autocomprobación de §12.

El criterio no es «el port es más corto». Es «el comportamiento sobrevivió al cambio de lenguaje y
puedes nombrar qué no».

## Autocomprobación (binaria)

- [ ] Leí `game.py`, `main.py` y `run.sh`.
- [ ] Expliqué qué es un port: una nueva derivación del comportamiento, no una traducción línea a
  línea.
- [ ] Nombré las cuatro capas y cuál debe coincidir exactamente.
- [ ] Conté 245 líneas en 2 archivos y comparé con 1.071 en 20.
- [ ] Encontré las constantes del LCG, los desplazamientos y el orden de comprobación en ambas
  implementaciones.
- [ ] Predije las cuatro tablas antes de ejecutar.
- [ ] Reproduje las 20 trazas, byte a byte.
- [ ] Comparé el port con la línea base de C++ en al menos dos semillas.
- [ ] Ejecuté el oráculo y expliqué el problema de los primos.
- [ ] Medí los cuatro tiempos y dije qué significan.
- [ ] Ejecuté la ruta de argumentos inválidos y registré el estado 2.
- [ ] Nombré tres cosas que cambiaron y tres que no.
- [ ] Cambié una regla en una copia, observé las semillas que fallan y las expliqué.
- [ ] Restauré la copia y confirmé 20 de 20.
- [ ] Nombré el empate inalcanzable y las sondas quitadas.
- [ ] Completé `EVIDENCE-es.md`.

Estos son criterios de aceptación. Si una casilla
queda sin marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §10.

## Misconcepciones que esta lección refuta

| Error de concepto | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| «Un port es una traducción línea a línea.» | No hay correspondencia entre `game_engine.h` y `run_game`. | Compara comportamiento y después reglas. |
| «Menos líneas significa un programa mejor.» | La línea base de C++ incluye sondas, un adaptador y un ejecutor en paralelo. | Cuenta lo que se quitó, no solo lo que se fue. |
| «Python es 30 veces más lento.» | 0,87 s son veinte arranques de intérprete; 20 partidas en un proceso tardan 0,016 s. | Separa arranque de trabajo. |
| «El port prueba que las reglas son correctas.» | El oráculo y el port son primos y podrían compartir una mala lectura. | Compara con C++ o lee la especificación. |
| «Los cuatro resultados desaparecieron.» | Sobreviven como `illegal=outside`, `illegal=visited`, `capture=` y `legal`. | Sigue las cadenas en la transcripción. |
| «Las pistas de tipo de Python comprueban el port.» | No se aplican en ejecución. | Trátalas como documentación. |
| «El port no tiene contrato de error.» | No tiene contrato devuelto; el bucle decide en línea. | Di qué capa se quitó. |
| «Captura y visitada se pueden comprobar en cualquier orden.» | Intercambiarlas hace fallar 11 de 20 trazas. | Mantén captura antes que visitada, como en `construction-22`. |
| «La regla de la trampa la prueban todas las trazas.» | Solo 9 trazas alcanzan una trampa. | Cuenta las líneas `no_legal_move`. |
| «El empate a 200 movimientos está probado.» | La partida de referencia más larga tiene 49 movimientos. | Registra el hueco; no reclames cobertura. |
| «Que `argparse` salga con 2 es lo mismo que `main` devuelva 2.» | `argparse` llama a `sys.exit(2)` dentro de la biblioteca. | Separa otra vez el retorno del salto. |
| «`run.sh` es un paso de compilación.» | Solo localiza `main.py` y ejecuta el intérprete. | Llámalo lanzador. |
| «El port conserva la concurrencia.» | `parallel_runner.cpp` no se portó. | Enumera lo que el port quitó. |
| «El port es la primera versión en Python.» | `reference.py` generó las trazas de referencia. | Llámalo oráculo, no primera versión. |
| «Que las transcripciones coincidan prueba igual rendimiento.» | Prueba igual salida. | Mide por separado y di qué mediste. |

## Siguiente paso

`construction-24` es el proyecto final. Pide llevar una regla nueva a través del proyecto, y el port
es la evidencia de que el contrato vive en las reglas y no en los tipos. Una regla que existe solo
en una implementación todavía no es una regla; es una costumbre de un archivo.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog-es.md) para la ruta completa.

Si faltaba la línea base de C++, no trates las trazas como el segundo lado. Las trazas registran la
salida; la línea base registra las decisiones que la produjeron.
