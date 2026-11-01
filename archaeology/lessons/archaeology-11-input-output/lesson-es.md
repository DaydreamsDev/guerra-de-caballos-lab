# archaeology-11 — Entrada, salida y capacidad de prueba

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** separación de entrada/salida y capacidad de prueba ·
> **Prerrequisito:** archaeology-09 · **Tiempo estimado:** 60 minutos
> **Lectura:** ~21 minutos

## Por qué existe esta lección

`archaeology-08` trazó una frontera entre módulos y `archaeology-09` convirtió el tablero en un tipo
opaco. Los dos cambios eran sobre el interior del programa. Esta lección trata su borde: el lugar
donde el programa se encuentra con una persona, un archivo u otro programa.

En el archivo, ese borde no existe. La función de movimiento imprime:

```text
234:  int movcab(int &cf,int &cc)
262:      cout<<" 1.";//muestra si ejecuto este caso
704:      cout<<"No hay movimientos posibles ...";
```

`movcab` ocupa las líneas 234–724, unos 491 renglones, y contiene diecinueve líneas con `cout`.
Mueve un caballo, decide si el juego terminó y narra ambas cosas. El bucle de turnos en 753 limpia
la pantalla con `system("cls")` y espera una tecla con `getch()`. Para observar un movimiento hay
que ejecutar el programa, aguantar los borrados de pantalla y pulsar una tecla.

Ese es el defecto que el catálogo llama `failure-012`: **entrada y salida dentro de la lógica de
movimiento**. El programa funciona. El coste lo paga quien quiera probarlo, automatizarlo o darle
una segunda interfaz.

Esta lección tiene una línea base que ya separa la consola del núcleo. Tu trabajo no es creer la
afirmación; es verificarla, encontrar qué sigue acoplado, reforzarlo y demostrar que los caminos de
consola y de prueba coinciden. La línea base es un banco pequeño, no el juego completo de caballos.

El producto es una frontera que puedes demostrar: un núcleo que no enlaza ninguna consola, un
adaptador de consola que lo representa, un adaptador de prueba que lo afirma y un arnés que compara
los dos en lugar de anunciar que coinciden.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar qué son un puerto de entrada, un puerto de salida y un adaptador, sin usar la palabra
  "capa";
- separar tres niveles de la frontera: el dispositivo, el formato y la política;
- probar que una unidad de traducción no contiene dependencia de consola, con el enlazador y `nm`,
  no con un comentario;
- leer un `GameResult` y decir qué partes son datos del dominio y qué partes ya son presentación;
- ejecutar un núcleo desde un adaptador de prueba que nunca enlaza la consola;
- explicar por qué `lines.size() == 8` es una aserción frágil y qué afirmar en su lugar;
- mover el formato de la transcripción fuera del núcleo sin cambiar un byte de la salida de consola;
- reparar un arnés que imprimía `match=yes` sin comparar nada;
- indicar el coste de 2008 con números de línea: diecinueve impresiones dentro de `movcab`, cinco
  `getch`, cinco `system("cls")` y un carácter de dibujo atravesando firmas del dominio;
- nombrar qué no arregla la separación: validación, números mágicos, el adaptador de entrada ausente
  y el archivo mismo;
- mantener el archivo de solo lectura mientras trabajas.

La frontera está hecha cuando
otra persona puede reproducir ambas ejecuciones.

## El concepto: la frontera con el exterior

### Puertos y adaptadores, en una imagen

Un **puerto** es la forma de una conversación. Un **adaptador** es una implementación de esa
conversación.

```text
 entrada ──► [ núcleo: run_game ] ──► GameResult ──┬──► adaptador de consola ──► terminal
                                                   └──► adaptador de prueba  ──► aserciones
```

El núcleo no sabe quién lo llamó. La consola no sabe cómo se juega. La prueba no sabe dibujar un
tablero. Cada lado puede cambiar sin el otro.

La línea base tiene un puerto de salida (`GameResult`), dos adaptadores de salida (`console.cpp`,
`test.cpp`) y un puerto de entrada (`run_game(int seed, const std::string &playerName)`). **No tiene
adaptador de entrada**: los valores están escritos a mano en `main.cpp`. Esa es una carencia real, y
esta lección la nombra en lugar de esconderla.

### Tres niveles de separación

"Separado de la consola" no es una sola propiedad. Son tres, y se confunden a menudo.

| Nivel | Pregunta | Línea base |
|---|---|---|
| **Dispositivo** | ¿El núcleo escribe en una terminal? | Separado: `core.cpp` no incluye `<cstdio>`, ni `cout`, ni `getch`. |
| **Formato** | ¿El núcleo decide el texto? | No separado: `lines` guarda cadenas ya formateadas como `"move=1 position=2"`. |
| **Política** | ¿El núcleo decide qué se muestra y cuándo? | En parte: ganador y número de movimientos son datos; el orden y la redacción son formato. |

El primer nivel es el que se puede probar mecánicamente. El segundo es el que muerde después: un
núcleo que devuelve cadenas es comprobable, pero sigue siendo dueño de la transcripción. Cambia la
redacción y tocas el núcleo. El tercero es una decisión de diseño: algo de la política pertenece al
núcleo (el ganador es un hecho del juego), algo pertenece al adaptador (si se limpia la pantalla).

Esta tabla es la lección. Quien solo quita `printf` hizo el nivel uno y lo llamó arquitectura.

### Qué separa realmente la línea base

Lee los `include` antes de leer la prosa.

| Archivo | Líneas | Incluye | ¿Imprime? |
|---|---:|---|---|
| `core.h` | 13 | `<string>`, `<vector>` | no |
| `core.cpp` | 26 | `core.h`, `<cstdint>`, `<string>` | no |
| `console.h` | 5 | `core.h` | no |
| `console.cpp` | 11 | `core.h`, `<cstdio>` | sí, la transcripción |
| `test.cpp` | 20 | `core.h`, `<cstdio>` | sí, su propio informe |
| `main.cpp` | 9 | `console.h`, `core.h` | no |

El README dice que "`console.cpp` es el único archivo que imprime". Eso es una abreviatura, no un
hecho: `test.cpp` imprime su informe de una línea. La afirmación precisa es que **`console.cpp` es
el único adaptador que representa la transcripción del juego**. Un adaptador de prueba puede
informar; el núcleo no puede imprimir en absoluto.

`test.cpp` incluye `core.h` y enlaza `core.cpp`. Nunca ve `console.cpp`. Esa es la prueba mecánica
de capacidad de prueba, y el paso 2 la repite con el enlazador.

### Por qué la capacidad de prueba es el premio

Una prueba es un llamador sin terminal. Si el núcleo necesita una terminal, la prueba no puede
llamarlo. Si el núcleo devuelve datos, la prueba puede. Ese es todo el argumento, y es estructural,
no cultural.

`movcab` no puede ser llamada por una prueba. Imprime, lee globales (`vectab`, `cab`, `jugada`) y el
bucle de turnos espera una tecla. `run_game` sí puede ser llamada porque sus entradas son parámetros
y su salida es un valor.

La capacidad de prueba no es un marco que se instala. Es una propiedad del grafo de llamadas:
**¿puede esta función terminar sin el exterior?** Si la respuesta es no, ningún marco de aserciones
ayuda.

### La forma de 2008, medida

| Anclaje | `variant-01` | Qué muestra |
|---|---:|---|
| Líneas con `cout` | 62 | La impresión está repartida por todo el programa. |
| `movcab` | 234–724 | Una función, 491 líneas. |
| Líneas con `cout` dentro de `movcab` | 19 | Movimiento y narración en el mismo cuerpo. |
| `getch()` | 42, 781, 806, 835, 861 | El avance depende de una tecla. |
| `system("cls")` | 43, 784, 809, 838, 864 | El bucle de turnos es dueño de la pantalla. |
| `system("color 1f")` | 38 | El punto de entrada es dueño del tema. |
| `srand(time(NULL))` | 76 | La ejecución no es reproducible. |
| `impresionorden` | 753–868 | El bucle de turnos imprime, espera y limpia. |

`variant-02` añade el lado de la entrada:

| Anclaje | `variant-02` | Qué muestra |
|---|---:|---|
| `char nombre[250];` | 32 | Un búfer fijo para el nombre del jugador. |
| `cin>>nom.nombre;` | 46 | La entrada se lee en `main`, sin límite. |
| `movcab(..., char &pisadas)` | 285 | Un carácter de dibujo entra en el movimiento. |
| `ordenjuego(..., char &pisadas)` | 775 | El mismo detalle de dibujo llega al orden de turnos. |
| `impresionorden(char pisadas)` | 802 | La impresora lo recibe de vuelta. |

Ese último grupo es la evidencia más filosa. `pisadas` es el primer carácter del nombre del jugador.
Existe para que el tablero dibuje al caballo con ese carácter. Viaja por `cabubicacion`,
`ordenjuego` y `movcab`, que son funciones del dominio. Un detalle de presentación contaminó tres
firmas. Eso es "entrada y salida mezcladas con la lógica" en la práctica: no solo `cout` dentro de
un bucle, sino datos de dibujo dentro del grafo de llamadas.

### La prueba de cambio

Una lección de diseño se juzga por un cambio. Aquí el cambio es: **añadir una línea a la
transcripción**, por ejemplo `version=1`, entre `start=player` y el primer movimiento.

En la línea base, la línea se construye en `core.cpp`. Editas el núcleo, la consola imprime nueve
líneas y `test.cpp` falla porque afirma `lines.size() == 8`. Un cambio de presentación rompió una
prueba.

En la versión separada del paso 3, el núcleo devuelve `seed`, `playerName`, `startingPlayer`,
`moves` y `winner`. La consola arma el texto. Añade `version=1` en `console.cpp` y la prueba sigue
pasando: no cambió ningún hecho del dominio. Esa es la diferencia medida, y está en dónde cae la
edición, no en cuántas líneas ocupa.

### La restricción heredada

`getch()` y `system("cls")` eran razonables para una demostración de consola. La consola era el
producto. El programa mostraba un tablero y esperaba para que el usuario pudiera leerlo. Imprimir
desde `movcab` evitaba pasar un tablero y una transcripción en un programa pequeño.

La lección no es "el código estaba mal". Es "la estructura encareció el siguiente requisito, y el
siguiente requisito era una prueba". El coste aparece cuando alguien quiere llamar a la regla de
movimiento sin una persona frente a la pantalla.

## Antes de empezar

Lee completos:

- [`README-es.md`](README-es.md) — la ficha de la lección, el comando de compilación y el criterio
  de aceptación.
- [archaeology-08](../archaeology-08-module-boundary/README-es.md) — la frontera entre módulos que
  esta lección asume.
- [archaeology-09](../archaeology-09-abstract-data-type/README-es.md) — el prerrequisito: un tipo
  opaco y un cambio de representación.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, la
  taxonomía de fallos; §5, la escala de severidad; §9, el mapeo a Track A.
- [`../../../docs/archive.md`](../../../docs/archive.md) — el archivo fijado, la etiqueta y el
  commit.

También necesitas:

- un clon local del archivo en la etiqueta `v1.0.0` (para los anclajes; el banco funciona sin él);
- un compilador de C++20 (`c++`, `g++` o `clang++`);
- `bash`, `nm` y `diff`;
- un directorio de trabajo fuera de `original/`.

> **Regla:** el archivo es de solo lectura. Copia el archivo que quieras cambiar a un directorio de
> trabajo; nunca edites `original/`. La línea base de esta lección también es evidencia: cópiala
> antes de cambiarla.

## El camino de 60 minutos

| Min | Paso | Qué haces | Evidencia |
|---|---|---|---|
| 0–8 | Predecir | Estimar el grafo de enlace, la comprobación de símbolos y lo que afirma la prueba | Tabla de predicción |
| 8–20 | Inspeccionar | Leer los seis archivos y los anclajes; ejecutar la línea base | Notas de anclajes y las cuatro líneas |
| 20–35 | Separar | Probar que el núcleo no enlaza consola; mover el formato al adaptador | Salida de `nm` y transcripción idéntica |
| 35–47 | Probar | Romper la aserción vieja, comprobar determinismo, reparar el arnés | Ejecución fallida, dos semillas, `match=yes` que compara |
| 47–55 | Explicar | Poner precio a la forma de 2008 y decir qué no arregla la separación | Tabla de costes y prueba de cambio |
| 55–60 | Registrar | Completar `EVIDENCE-es.md`; confirmar que el archivo está limpio | Entrada de portafolio |

Si explicar la prueba con `nm` toma cinco minutos, esa explicación
es la lección.

### La comprobación de ingeniería en esta lección

| Paso | Dónde ocurre | Qué produce |
|---|---|---|
| **Predecir** | Paso 1 | Una estimación falsable del grafo de enlace y de las aserciones. |
| **Inspeccionar** | Paso 2 | Los seis archivos, los anclajes y la ejecución de la línea base. |
| **Separar** | Paso 3 | Una copia de trabajo donde el núcleo devuelve datos y el adaptador arma el texto. |
| **Probar** | Paso 4 | Una prueba que afirma hechos del dominio, una aserción frágil reproducida y un arnés que compara. |
| **Explicar** | Paso 5 | La factura de 2008, los dos arreglos y el acoplamiento restante. |
| **Registrar** | Paso 6 | `EVIDENCE-es.md` con comandos, conteos y resultados. |

## Dos sesiones

Esta lección es larga a propósito. Divídela en dos sesiones:

- **Sesión 1 — Núcleo (Pasos 1–3, unos 35 min).** Detente después del Paso 3, cuando el núcleo
  está separado de la consola.
- **Sesión 2 — Extensión (Pasos 4–6, unos 25 min).** Empieza solo después de la Sesión 1. Vas a
  probar la costura y registrar la evidencia.

## Paso 1 — Predecir (8 min)

Antes de abrir la fuente, responde por escrito estas cinco preguntas. Comprométete con respuestas
que puedas equivocar.

1. ¿Cuántos archivos fuente enlaza el binario de consola? ¿Cuántos enlaza el binario de prueba?
2. ¿`core.o` referencia algún símbolo de stdio o de consola? Nombra los símbolos que esperas.
3. ¿Cuántas líneas imprime la consola, y de dónde sale ese número?
4. ¿Qué afirma `test.cpp`, y qué no afirma?
5. Si añades una línea a la transcripción dentro del núcleo, ¿qué prueba falla y por qué?

Una predicción útil es falsable. "El núcleo está desacoplado" no lo es. "`core.o` no tiene `printf`,
`cout` ni `getch` indefinidos, y el binario de prueba enlaza solo `core.cpp` y `test.cpp`" sí lo es.

### Una predicción trabajada

Antes de leer, yo escribiría:

- binario de consola: tres fuentes — `core.cpp`, `console.cpp`, `main.cpp`;
- binario de prueba: dos fuentes — `core.cpp`, `test.cpp`; sin consola;
- `core.o`: símbolos indefinidos para `memcpy`, `operator new`, `operator delete` e internos de
  cadena; ninguno para `printf`, `cout` o `getch`;
- líneas de consola: ocho, producidas por `result.lines`, no por un contador del adaptador;
- aserciones de la prueba: cantidad de líneas, ganador y número de movimientos; no el texto de cada
  línea, no el determinismo, no la coincidencia con la consola;
- añadir una línea: la edición en el núcleo hace que `lines.size()` sea nueve, así que la aserción
  de tamaño falla aunque el juego no cambió.

Cada afirmación se puede comprobar en el paso siguiente. Si tu predicción dice "la separación es
buena", todavía no predijiste nada; eso es una conclusión.

## Paso 2 — Inspeccionar (12 min)

### Leer los seis archivos

```bash
cd archaeology/lessons/archaeology-11-input-output
wc -l code/*.h code/*.cpp
```

La línea base tiene 84 líneas de C++ en seis archivos:

| Parte | Líneas | Qué es |
|---|---:|---|
| `core.h` | 13 | `GameResult` y la declaración de `run_game`. |
| `core.cpp` | 26 | Un bucle determinista de tres movimientos que arma líneas. |
| `console.h` | 5 | Una declaración: `print_result`. |
| `console.cpp` | 11 | El único representador de la transcripción. |
| `test.cpp` | 20 | El adaptador de prueba: tres booleanos y un informe. |
| `main.cpp` | 9 | El punto de entrada de consola; la entrada está escrita a mano. |

Lee `core.h` primero. El resultado es:

```cpp
struct GameResult
{
    std::vector<std::string> lines;
    std::string winner;
    int moveCount;
};
```

Dos campos son hechos del dominio: `winner` y `moveCount`. Un campo ya es presentación: `lines`.
Guarda esa idea; el paso 3 trata de ella.

Lee `core.cpp`. Siembra un estado de 64 bits, avanza un generador congruencial lineal tres veces,
envuelve una posición módulo 8 y empuja una cadena por movimiento. Incluye `<cstdint>` y `<string>`.
No hay `<cstdio>`, ni `printf`, ni `cout`, ni `getch`.

Lee `console.cpp`. Recorre `result.lines` y llama a `std::printf("%s\n", line.c_str())`. Ese es todo
el adaptador. No sabe qué es un caballo.

Lee `test.cpp`. Llama a `run_game(11, "Learner")` y calcula tres booleanos:

```cpp
bool linesMatch = result.lines.size() == 8;
bool winnerMatches = result.winner == "player";
bool movesMatch = result.moveCount == 3;
```

Lee `main.cpp`. Llama a `run_game(11, "Learner")` y a `print_result(result)`. El puerto de entrada
existe, pero no hay adaptador de entrada: la semilla y el nombre son literales.

### Probar que el núcleo no tiene consola

No confíes en la lista de `include`. Pregúntale al compilador y al enlazador.

```bash
mkdir -p build
c++ -std=c++20 -Wall -Wextra -Werror -O2 -c code/core.cpp -o build/core.o
nm -C -u build/core.o
```

Símbolos indefinidos esperados en esta plataforma (nombres de cadena abreviados):

```text
memcpy
memmove
operator new(unsigned long)
operator delete(void*, unsigned long)
std::__cxx11::basic_string<...>::_M_dispose()
std::__cxx11::basic_string<...>::_M_replace_cold(...)
std::__throw_length_error(char const*)
__gxx_personality_v0
_Unwind_Resume
```

Ahora filtra por la consola:

```bash
nm -C -u build/core.o | grep -E 'printf|cout|getch|fwrite|puts'
```

El comando no imprime nada y sale con 1. Ese es el resultado que quieres: el objeto del núcleo no
tiene ningún símbolo de consola indefinido. `memcpy` es la biblioteca estándar moviendo bytes; no es
una terminal.

Enlaza la prueba sin la consola:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 code/core.cpp code/test.cpp -o build/core-test
./build/core-test
```

Esperado:

```text
lines=8 winner=player moves=3
test=ok
```

Si la consola fuera necesaria, este enlace fallaría. No falla. Esa es una prueba más fuerte que
cualquier comentario en una cabecera.

### Leer el arnés

`run.sh` construye dos binarios e imprime cuatro líneas. Lee con cuidado las líneas 16–24:

```bash
consoleLines="$("${buildDirectory}/console-app" | wc -l)"
testOutput="$("${buildDirectory}/core-test")"

printf 'console_lines=%s\n' "${consoleLines}"
printf 'test_output=%s\n' "${testOutput}"
if [[ "${testOutput}" != *"test=ok"* ]]; then
    exit 1
fi
printf 'match=yes\n'
```

`match=yes` se imprime incondicionalmente después de que la prueba pasa. El script nunca compara
`consoleLines` con la cantidad de líneas que vio la prueba. No compara nada. La palabra "match" es
una afirmación, no una comprobación.

No es un defecto fatal en una demostración, pero es exactamente la clase de arnés que hace que una
suite se vea verde mientras dos adaptadores discrepan. El paso 4 lo repara.

### Ejecutar la línea base

```bash
./run.sh
```

Salida esperada:

```text
console_lines=8
test_output=lines=8 winner=player moves=3
test=ok
match=yes
```

El script compila con:

```text
-std=c++20 -Wall -Wextra -Werror -O2
```

`-Werror` importa: la línea base está limpia de advertencias con las banderas que trae. Si tu
compilador la rechaza, registra el compilador y el diagnóstico; no silencies la advertencia para que
la ejecución pase.

### Leer los anclajes del archivo

En el archivo fijado en la etiqueta `v1.0.0`:

| Anclaje | `variant-01` | Qué muestra |
|---|---:|---|
| `system("color 1f")` | 38 | El punto de entrada es dueño del tema de la terminal. |
| `getch()` en `main` | 42 | La primera pantalla espera una tecla. |
| `system("cls")` en `main` | 43 | La primera pantalla se limpia. |
| `while (jugada!=0)` | 45 | El bucle del juego es el bucle del programa. |
| `srand(time(NULL))` | 76 | La ubicación no es reproducible. |
| `movcab` | 234–724 | Movimiento y narración comparten un cuerpo. |
| `cout` dentro de `movcab` | 19 líneas | Cada rama se narra a sí misma. |
| Mensajes de caballo encerrado | 704, 712 | `movcab` imprime el final. |
| `impresionorden` | 753–868 | El bucle de turnos imprime y pausa. |
| `getch()` en el bucle de turnos | 781, 806, 835, 861 | Cada turno espera una tecla. |
| `system("cls")` en el bucle de turnos | 784, 809, 838, 864 | Cada turno limpia la pantalla. |

Cuenta las impresiones tú mismo:

```bash
grep -c 'cout' original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
awk 'NR>=234 && NR<=724 && /cout/ {n++} END {print n}' \
    original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

Las respuestas son 62 y 19. El primer número es todo el programa; el segundo es solo la función de
movimiento.

Las otras variantes tienen la misma forma:

| Variante | Líneas | Líneas con `cout` | `getch` | `system("cls")` |
|---|---:|---:|---:|---:|
| `variant-01-baseline-2008` | 930 | 62 | 5 | 5 |
| `variant-02-records-2008` | 1021 | 76 | 5 | 5 |
| `variant-03-pointers-dev-2008` | 1058 | 75 | 5 | 5 |
| `variant-04-pointers-final-2008` | 1079 | 54 | 5 | 5 |

Las variantes con punteros cambiaron el contenedor. No separaron la consola. Vale la pena notarlo:
un cambio de sintaxis no es un cambio de arquitectura.

### Comprobar tu predicción

- El binario de consola enlaza tres fuentes; el de prueba enlaza dos y nunca ve `console.cpp`.
- `core.o` no tiene `printf`, `cout` ni `getch` indefinidos. Sí tiene `memcpy`, asignación y
  símbolos de cadena.
- La consola imprime ocho líneas porque `result.lines` tiene ocho elementos.
- La prueba afirma tamaño, ganador y número de movimientos. No afirma el texto de las líneas, ni el
  orden, ni el determinismo, ni la coincidencia con la consola.
- Añadir una línea dentro del núcleo hace fallar la aserción de tamaño.

Si tu predicción decía que el arnés verifica la coincidencia, encontraste la brecha entre el "match"
del README y el trabajo real del script. Anótalo; es parte del diagnóstico.

## Paso 3 — Separar (15 min)

### Crear la copia de trabajo

Crea un directorio de trabajo fuera de `original/`. `build/` está ignorado por este repositorio:

```bash
mkdir -p build/scratch
cd build/scratch
cp ../../code/core.h ../../code/core.cpp ../../code/console.h \
   ../../code/console.cpp ../../code/test.cpp ../../code/main.cpp .
```

De aquí en adelante, edita los archivos de `build/scratch/`. No edites `code/`; es la línea base que
ejecutará la próxima persona.

### Mover el formato fuera del núcleo

La línea base separa el dispositivo. Este paso separa el formato. El núcleo devolverá datos del
dominio; la consola decidirá el texto.

Reemplaza `core.h` por:

```cpp
#pragma once

#include <string>
#include <vector>

struct Move
{
    int number;
    int position;
};

struct GameResult
{
    int seed;
    std::string playerName;
    std::string startingPlayer;
    std::vector<Move> moves;
    std::string winner;
};

GameResult run_game(int seed, const std::string &playerName);
```

Reemplaza `core.cpp` por:

```cpp
#include "core.h"

#include <cstdint>
#include <string>

GameResult run_game(int seed, const std::string &playerName)
{
    std::uint64_t state = static_cast<std::uint64_t>(seed);
    int position = 0;
    std::vector<Move> moves;

    for (int moveNumber = 1; moveNumber <= 3; moveNumber += 1)
    {
        state = state * 6364136223846793005ULL + 1442695040888963407ULL;
        position = (position + static_cast<int>(state % 3) + 1) % 8;
        moves.push_back(Move{moveNumber, position});
    }

    return GameResult{seed, playerName, "player", moves, "player"};
}
```

Reemplaza `console.cpp` por:

```cpp
#include "console.h"

#include <cstdio>

void print_result(const GameResult &result)
{
    std::printf("seed=%d\n", result.seed);
    std::printf("player=%s\n", result.playerName.c_str());
    std::printf("start=%s\n", result.startingPlayer.c_str());
    for (const Move &move : result.moves)
    {
        std::printf("move=%d position=%d\n", move.number, move.position);
    }
    std::printf("winner=%s\n", result.winner.c_str());
    std::printf("moves=%zu\n", result.moves.size());
}
```

`console.h` y `main.cpp` no cambian.

`test.cpp` ya no compila: lee `lines` y `moveCount`, que el nuevo `GameResult` no tiene. Reemplázalo
por la versión que afirma campos del dominio. El paso 4 explica cada aserción.

```cpp
#include "core.h"

#include <cstdio>

int main()
{
    GameResult result = run_game(11, "Learner");
    bool seedMatches = result.seed == 11;
    bool playerMatches = result.playerName == "Learner";
    bool startMatches = result.startingPlayer == "player";
    bool movesMatch = result.moves.size() == 3;
    bool firstMoveMatches = movesMatch && result.moves[0].number == 1 &&
                            result.moves[0].position == 2;
    bool winnerMatches = result.winner == "player";
    std::printf("moves=%zu winner=%s\n", result.moves.size(), result.winner.c_str());
    if (!seedMatches || !playerMatches || !startMatches || !movesMatch ||
        !firstMoveMatches || !winnerMatches)
    {
        std::printf("test=failed\n");
        return 1;
    }
    std::printf("test=ok\n");
    return 0;
}
```

Compila y ejecuta el binario de consola:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 core.cpp console.cpp main.cpp -o console-app
./console-app
```

Esperado, sin cambios:

```text
seed=11
player=Learner
start=player
move=1 position=2
move=2 position=5
move=3 position=0
winner=player
moves=3
```

Compárala con la línea base:

```bash
cd ../..
./build/console-app > /tmp/console-baseline.txt
cd build/scratch
./console-app > /tmp/console-separated.txt
diff /tmp/console-baseline.txt /tmp/console-separated.txt
```

`diff` no imprime nada. La transcripción es idéntica byte a byte. Ese es el punto: mover el formato
no cambió la salida.

### Qué acabas de cambiar

| Antes | Después |
|---|---|
| `GameResult` guarda `lines` | `GameResult` guarda `seed`, `playerName`, `startingPlayer`, `moves`, `winner` |
| `moveCount` es un campo fijo en 3 | `moves.size()` lo deriva de los datos |
| `core.cpp` arma `"move=1 position=2"` | `console.cpp` formatea la cadena |
| La prueba debe contar líneas | La prueba puede afirmar campos |
| Un cambio de redacción edita el núcleo | Un cambio de redacción edita el adaptador |

Las dos últimas filas son la prueba de cambio. Las tres primeras son el mecanismo.

Nota lo que **no** cambió: el núcleo sigue jugando tres movimientos, el LCG es el mismo, las
posiciones son las mismas y la salida de consola es la misma. Una separación que cambia el
comportamiento es una reescritura, no una separación. Mantén el diff neutral y pruébalo con `diff`.

### No toques la línea base

`code/core.h`, `code/core.cpp`, `code/console.cpp` y `code/test.cpp` se quedan como están. La línea
base es el artefacto que permite a la próxima persona reproducir tu comparación. Tu copia de trabajo
es el candidato.

## Paso 4 — Probar (12 min)

### Qué afirma la prueba que viene incluida

`test.cpp` calcula tres booleanos e imprime una línea de resumen y un veredicto. Eso basta para
detectar un movimiento ausente, un ganador equivocado y un conteo equivocado. No basta para detectar
una transcripción equivocada.

| Aserción | Detecta | No detecta |
|---|---|---|
| `lines.size() == 8` | Una línea ausente o extra | Texto equivocado, orden equivocado, saltos embebidos |
| `winner == "player"` | Una cadena de ganador equivocada | Un ganador correcto por accidente |
| `moveCount == 3` | Un número de movimientos equivocado | Un conteo que no concuerda con `lines` |

### Qué no afirma

- el texto de ninguna línea;
- el orden de las líneas;
- que dos ejecuciones con la misma semilla sean idénticas;
- que dos semillas produzcan juegos distintos;
- que la salida de consola coincida con la vista de la prueba;
- que la entrada sea válida.

Las dos últimas son las que importan para esta lección.

### Romperla a propósito

En una copia desechable de la línea base incluida, añade `lines.push_back("version=1");` antes del
retorno. Recompila la prueba y ejecútala. Verás:

```text
lines=9 winner=player moves=3
test=failed
```

En la copia separada, el error equivalente es `moves.push_back(Move{0, 0});`. La prueba imprime
`moves=4 winner=player` y falla por el conteo de movimientos. En cualquier caso, el juego no cambió.
La transcripción ganó una línea, o la lista de movimientos ganó un movimiento falso, y la prueba
falló.

Esa es una aserción frágil: mide la forma de la presentación, no el comportamiento del juego.

No es un argumento contra las aserciones. Es un argumento para afirmar lo correcto. Una prueba que
falla cuando añades una línea inofensiva enseña al equipo a dejar de añadir líneas, o a dejar de
confiar en la prueba.

### Comprobar el determinismo

Compila y ejecuta la prueba de la copia dos veces con la misma semilla:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 core.cpp test.cpp -o core-test
./core-test
./core-test
```

Ambas ejecuciones imprimen `moves=3 winner=player` y `test=ok`. El LCG se siembra desde el
parámetro, no desde el reloj, así que el resultado es reproducible.

En una copia temporal de `test.cpp`, cambia la semilla de `11` a `12` y relaja la aserción de
semilla (`bool seedMatches = true;`). Recompila y ejecútala. Las posiciones cambian:

| Semilla | Movimiento 1 | Movimiento 2 | Movimiento 3 |
|---:|---:|---:|---:|
| 11 | 2 | 5 | 0 |
| 12 | 1 | 2 | 3 |

El ganador y el conteo no cambian, porque este banco siempre juega tres movimientos y siempre
declara ganador al jugador. Esa es una simplificación del banco, no una regla del juego.

### Reparar el arnés

El `run.sh` incluido imprime `match=yes` sin comparar. Repáralo en la copia de trabajo.

El adaptador de prueba del paso 3 ya imprime `moves=3 winner=player`. Reemplaza el último bloque de
`run.sh` por una comparación de los campos que ambos adaptadores exponen:

```bash
consoleOutput="$("${buildDirectory}/console-app")"
testOutput="$("${buildDirectory}/core-test")"
consoleLines="$(printf '%s\n' "${consoleOutput}" | wc -l)"

printf 'console_lines=%s\n' "${consoleLines}"
printf 'test_output=%s\n' "${testOutput}"
if [[ "${testOutput}" != *"test=ok"* ]]; then
    exit 1
fi

consoleMoves="$(printf '%s\n' "${consoleOutput}" | sed -n 's/^moves=//p')"
consoleWinner="$(printf '%s\n' "${consoleOutput}" | sed -n 's/^winner=//p')"
testMoves="$(printf '%s\n' "${testOutput}" | sed -n 's/.*moves=\([0-9]*\).*/\1/p')"
testWinner="$(printf '%s\n' "${testOutput}" | sed -n 's/.*winner=\([a-z]*\).*/\1/p')"

if [[ "${consoleMoves}" != "${testMoves}" || "${consoleWinner}" != "${testWinner}" ]]; then
    printf 'match=no\n'
    exit 1
fi
printf 'match=yes\n'
```

Ejecútalo:

```bash
./run.sh
```

Esperado:

```text
console_lines=8
test_output=moves=3 winner=player
test=ok
match=yes
```

Ahora `match=yes` significa algo: los campos `winner` y `moves` de la consola son iguales a los de
la prueba. Si cambias la consola para imprimir `moves=4`, el script imprime `match=no` y sale con 1.
Esa es la diferencia entre un informe y una comprobación.

### La comprobación filosa (opcional)

Si quieres ver mentir al arnés incluido, usa una copia desechable de la línea base (no la copia
separada). Haz que `main.cpp` lea el nombre desde `argv` y pasa un nombre con un salto de línea:

```bash
./console-app $'Learner\ninjected' | wc -l
```

La consola imprime nueve líneas. `test.cpp` sigue viendo `lines.size() == 8`, porque su nombre es
`"Learner"`. El `run.sh` incluido seguiría imprimiendo `match=yes`. Los dos adaptadores discrepan y
el script dice que coinciden.

La causa es la validación de entrada: `run_game` acepta cualquier cadena y la copia a la
transcripción. El salto de línea no se escapa. Es la misma clase que el `cin>>nom.nombre` sin límite
en `variant-02:46`, un nivel más arriba: el programa confía en que la entrada tenga la forma que la
impresora espera.

No tienes que arreglar la validación en esta lección. Sí tienes que notar que un conteo de líneas no
es un contrato.

## Paso 5 — Explicar (8 min)

### La factura de 2008

| Coste | Dónde | Consecuencia |
|---|---|---|
| 19 impresiones dentro de `movcab` | 234–724 | El movimiento no corre sin consola. |
| 5 llamadas a `getch()` | 42, 781, 806, 835, 861 | El avance necesita una tecla humana. |
| 5 llamadas a `system("cls")` | 43, 784, 809, 838, 864 | El bucle de turnos es dueño de la pantalla. |
| `srand(time(NULL))` | 76 | Dos ejecuciones no son comparables. |
| `pisadas` atravesando firmas del dominio | `variant-02:285`, 775 | Un carácter de dibujo contamina el grafo de llamadas. |
| `cin>>nom.nombre` en `char[250]` | `variant-02:32, 46` | Entrada sin límite y sin validación. |

Las tres primeras filas son la razón por la que una prueba no puede llamar a la regla de movimiento.
La cuarta es la razón por la que una traza de referencia no se puede comparar. La quinta es la
razón por la que la firma del dominio miente sobre lo que necesita. La sexta es la razón por la que
el lado de la entrada también es un problema de frontera, no solo la salida.

### Dos arreglos, dos tamaños

**Arreglo mínimo — devolver un resultado.** Extrae la impresión de `movcab` a una impresora que
reciba el tablero y el movimiento, y devuelve la posición resultante y el estado del juego. La
función de movimiento ya no imprime; el llamador decide qué mostrar. Esta es la forma de la línea
base: `run_game` devuelve un `GameResult` y `console.cpp` lo imprime. Quita la consola del núcleo y
hace que el movimiento se pueda llamar desde una prueba.

**Arreglo estructural — puertos y adaptadores.** Define el resultado como datos del dominio, no
como texto. Dale a la consola un representador. Dale a la prueba un adaptador de aserciones. Dale
al punto de entrada un adaptador de entrada. Entonces el formato vive en un solo lugar y la política
vive donde corresponde:

```text
argv / stdin ──► adaptador de entrada ──► run_game ──► GameResult ──► adaptador de consola
                                                          └────────► adaptador de prueba
```

Enlaces a Track A: `construction-20` (hexagonal: puertos y adaptadores) es el peldaño directo.
`construction-08` (programación modular) es el prerrequisito: sin una frontera entre módulos no hay
"núcleo" que poner detrás de un puerto. `common-01` (arnés determinista) es la contraparte de
pruebas: una ejecución sembrada y repetible es lo que hace significativa la comparación de
adaptadores.

### Qué no arregla la separación

- **Validación.** `run_game` acepta cualquier nombre, incluido uno con un salto de línea. El
  contrato de la transcripción no se hace cumplir.
- **Números mágicos.** `3` aparece como límite del bucle, como `moveCount` y como la línea
  `moves=3`. La versión separada deriva el conteo de `moves.size()`, lo que elimina una copia, no la
  regla.
- **El ganador.** `"player"` está escrito a mano. No hay una regla del juego que lo decida.
- **El adaptador de entrada.** El puerto existe; el adaptador no. `main` escribe `11` y
  `"Learner"` a mano.
- **El archivo.** Nada de esto toca `movcab`. El programa de 2008 sigue imprimiendo desde su función
  de movimiento.
- **Las pruebas ausentes.** Una semilla y tres movimientos son una prueba de humo, no una suite.
- **El formato de la transcripción.** Incluso después del paso 3, la consola es dueña de un formato
  que ninguna prueba fija byte a byte. Si el texto exacto importa, congélalo como archivo de
  referencia (`common-04`).

### El equilibrio, dicho con justicia

Un adaptador cuesta un archivo, una indirección y un tipo de resultado que diseñar. Una demostración
de consola de un solo uso no lo necesita: imprimir desde la función de movimiento es más corto y no
hay un segundo llamador. El coste aparece cuando llega el segundo llamador: una prueba, un script,
una interfaz gráfica, una herramienta de repetición. En ese momento el núcleo atado a la consola
cobra el precio completo: no puedes llamarlo, así que no puedes probarlo, así que no puedes
cambiarlo con seguridad.

La lección te pide elegir con la prueba de cambio, no con el gusto. Si tu contexto dice que el
programa será siempre una demostración de consola con una persona delante, la forma de 2008 es
defendible. Escribe ese contexto; es parte del diagnóstico.

### La prueba de cambio, medida

| Cambio | Línea base | Separado |
|---|---|---|
| Añadir una línea a la transcripción | Editas `core.cpp`; `test.cpp` falla por `lines.size()` | Editas `console.cpp`; la prueba pasa |
| Cambiar la redacción del ganador | Editas `core.cpp` | Editas `console.cpp` |
| Añadir un adaptador silencioso | Archivo nuevo; el núcleo sigue armando las cadenas que ignora | Archivo nuevo; el núcleo devuelve campos |
| Afirmar la posición del primer movimiento | Parseas `lines[3]` | Lees `moves[0].position` |
| Ejecutar sin terminal | Sí | Sí |

La primera fila es la que hay que registrar. Un cambio de presentación no debería hacer fallar una
prueba del dominio.

## Paso 6 — Registrar (5 min)

Completa [`EVIDENCE-es.md`](EVIDENCE-es.md):

- configuración y fijación del archivo;
- las cinco predicciones, escritas antes de inspeccionar;
- el grafo de enlace: tres fuentes para la consola, dos para la prueba;
- la salida de `nm`: ningún símbolo de stdio o consola en `core.o`;
- la ejecución de la línea base y sus cuatro líneas exactas;
- los anclajes: 62 líneas con `cout`, 19 dentro de `movcab`, 5 `getch`, 5 `cls`;
- el diff de separación: qué se movió, qué quedó y el resultado de `diff` sobre la transcripción;
- la prueba: qué afirma, la aserción rota, las dos semillas;
- la reparación del arnés: la comparación que reemplazó el `match=yes` incondicional;
- la prueba de cambio: añadir una línea y dónde cae la edición en cada versión;
- la frontera de evidencia: qué prueba la ejecución y qué no;
- la comprobación de integridad del archivo.

Una frase como "el núcleo está desacoplado" es una opinión. "`nm -C -u core.o` no tiene `printf`,
`cout` ni `getch`; el binario de prueba enlaza dos fuentes; la transcripción separada de consola es
idéntica byte a byte con `diff`" es un diagnóstico.

## El original en detalle

Hechos que puedes citar, con números de línea de `variant-01` en la etiqueta `v1.0.0`:

| Ubicación | Qué hay |
|---:|---|
| 38 | `system("color 1f")` — el punto de entrada fija el tema de la terminal. |
| 42 | `getch()` — la primera pantalla espera una tecla. |
| 43 | `system("cls")` — la primera pantalla se limpia. |
| 45 | `while (jugada!=0)` — el bucle del juego es el bucle del programa. |
| 76 | `srand(time(NULL))` — la ubicación se siembra desde el reloj. |
| 234 | `int movcab(int &cf, int &cc)` — el movimiento recibe coordenadas, no un tablero. |
| 262, 286, 323, 346, 379, 402, 435, 458 | `cout` dentro de las cuatro primeras ramas. |
| 491, 514, 548, 571, 604, 627, 660, 683 | `cout` dentro de las cuatro últimas ramas. |
| 699 | `cout` para un movimiento que no se pudo aplicar. |
| 704, 712 | Mensajes de caballo encerrado, impresos desde `movcab`. |
| 753 | `void impresionorden()` — el bucle de turnos. |
| 781, 806, 835, 861 | `getch()` — cada turno espera una tecla. |
| 784, 809, 838, 864 | `system("cls")` — cada turno limpia la pantalla. |
| 870 | `void impresion()` — la impresora del tablero. |

`variant-02` añade la entrada:

| Ubicación | Qué hay |
|---:|---|
| 32 | `char nombre[250];` dentro de `struct movimiento`. |
| 45–46 | `cout<<"digite su nombre"; cin>>nom.nombre;` en `main`. |
| 48 | `pisadas2=nom.nombre[0];` — el carácter de dibujo sale del nombre. |
| 285 | `int movcab(int &cf,int &cc,char &pisadas)` — datos de dibujo en el movimiento. |
| 775 | `int ordenjuego(int &tirocab1,int &tirocab2,char &pisadas)`. |
| 802 | `void impresionorden(char pisadas)`. |

El catálogo llama al defecto "entrada y salida dentro de `moverJugador`". La función del fuente es
`movcab`; no hay `moverJugador` en `variant-01`. Cita el nombre del fuente, no la abreviatura.

## Qué prueba y qué no prueba la separación

| Afirmación | ¿Respaldada? | Por qué |
|---|---|---|
| `core.o` no tiene dependencia de consola. | Sí | `nm -C -u` no encuentra `printf`, `cout` ni `getch`. |
| El binario de prueba corre sin `console.cpp`. | Sí | Enlaza dos fuentes e imprime `test=ok`. |
| El núcleo es determinista para una semilla fija. | Sí | Dos ejecuciones con semilla 11 son idénticas. |
| El núcleo no es dueño de presentación. | No | `lines` en la línea base son cadenas ya formateadas. |
| La consola y la prueba coinciden en el arnés incluido. | No | `match=yes` es incondicional. |
| El arnés reparado comprueba la coincidencia. | Sí | Compara `winner` y `moves` y sale con 1 si discrepan. |
| La salida de consola separada no cambió. | Sí | `diff` no imprime nada. |
| El núcleo valida su entrada. | No | Un nombre con salto de línea cambia el conteo impreso. |
| El `movcab` del archivo ya es comprobable. | No | El banco no toca el archivo. |
| El banco es el juego completo de caballos. | No | Juega tres movimientos y fija el ganador. |
| La separación es gratuita. | No | Cuesta un tipo de resultado, un adaptador y un representador. |

Esta tabla es el control de honestidad. Un registro que dice "el código ahora es comprobable" sin
nombrar la muestra está excediéndose. Di "el núcleo se puede llamar sin consola; se afirman una
semilla y tres movimientos" cuando eso es lo que ejecutaste.

## Autocomprobación (binaria)

- [ ] Escribí las cinco predicciones antes de abrir la fuente.
- [ ] Nombré las tres fuentes del binario de consola y las dos del binario de prueba.
- [ ] Compilé `core.cpp` solo e inspeccioné `nm -C -u`.
- [ ] Confirmé que no hay `printf`, `cout` ni `getch` en `core.o`.
- [ ] Ejecuté `./run.sh` y vi las cuatro líneas esperadas.
- [ ] Encontré el `match=yes` incondicional en `run.sh`.
- [ ] Conté 62 líneas con `cout` en `variant-01` y 19 dentro de `movcab`.
- [ ] Conté 5 `getch` y 5 `system("cls")`.
- [ ] Encontré los anclajes de entrada de `variant-02`: `char[250]`, `cin`, `pisadas`.
- [ ] Copié la línea base a `build/scratch/` antes de editar.
- [ ] Moví el formato de la transcripción del núcleo a `console.cpp`.
- [ ] La transcripción separada de consola es idéntica byte a byte con `diff`.
- [ ] Reproduje la aserción frágil: una línea extra hace fallar la prueba vieja.
- [ ] Comprobé el determinismo con dos ejecuciones de la misma semilla y dos semillas distintas.
- [ ] Reparé el arnés para que `match=yes` siga a una comparación real.
- [ ] Indiqué que la línea base todavía es dueña del formato de la transcripción.
- [ ] Indiqué que no hay adaptador de entrada.
- [ ] Indiqué que el banco no es el juego completo de caballos.
- [ ] Enlacé el arreglo estructural a `construction-20`, `construction-08` y `common-01`.
- [ ] El archivo está sin modificar (`git status` en el archivo está limpio).

Si una casilla no está marcada, la casilla sin marcar nombra el siguiente paso. No hay penalización
por repetir uno.

## Misconcepciones que esta lección refuta

| Idea equivocada | Por qué está mal | Qué hacer en su lugar |
|---|---|---|
| "Si el núcleo no tiene `printf`, está separado." | Separa el dispositivo, no el formato. | Comprueba si el núcleo arma cadenas o devuelve datos. |
| "Un `GameResult` con cadenas son datos del dominio." | Las cadenas ya están representadas. | Devuelve campos; deja que el adaptador represente. |
| "La prueba demuestra que consola y núcleo coinciden." | Solo llama al núcleo; `run.sh` no compara nada. | Compara los campos que ambos adaptadores exponen. |
| "Un conteo de líneas es una aserción de comportamiento." | Una línea extra inofensiva la hace fallar. | Afirma campos del dominio y congela el texto aparte. |
| "La capacidad de prueba viene de un marco." | Viene de un grafo de llamadas que termina sin el exterior. | Llama al núcleo desde una prueba sin enlazar consola. |
| "`getch` es solo una pausa." | Condiciona el avance a una tecla humana. | Inyecta la entrada; no leas el teclado en el núcleo. |
| "`system("cls")` es un detalle de dibujo." | El bucle de turnos lo usa para controlar el flujo. | Mueve el control de pantalla al adaptador. |
| "Las variantes con punteros arreglaron el acoplamiento de E/S." | Cambiaron el contenedor; siguen imprimiendo desde `movcab`. | Cuenta las impresiones, no la sintaxis. |
| "El archivo tiene una función llamada `moverJugador`." | `variant-01` la llama `movcab`. | Cita el fuente. |
| "El banco es el juego." | Juega tres movimientos y fija el ganador. | Declara la frontera de aislamiento. |
| "Si la prueba pasa, la salida es correcta." | La prueba comprueba tres hechos, no la transcripción. | Añade un archivo de referencia si el texto importa. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Núcleo | El código que decide el juego, sin terminal. |
| Adaptador | Un traductor entre el núcleo y un exterior. |
| Puerto | La forma de una conversación que el núcleo acepta u ofrece. |
| Puerto de entrada | Los parámetros que un llamador debe aportar. |
| Puerto de salida | El valor que el núcleo devuelve. |
| Separación de dispositivo | El núcleo no escribe en ninguna terminal. |
| Separación de formato | El núcleo no decide ningún texto. |
| Separación de política | El núcleo no decide el momento de la presentación. |
| Adaptador de prueba | Un llamador que afirma en lugar de imprimir una transcripción. |
| Aserción frágil | Una prueba que falla ante un cambio inofensivo. |
| Archivo de referencia | Una salida esperada congelada que se compara byte a byte. |
| Determinismo | La misma entrada produce la misma salida. |
| Grafo de enlace | Qué unidades de traducción terminan en qué binario. |
| Símbolo indefinido | Un nombre que el objeto necesita que otra unidad aporte. |
| Prueba de cambio | Un cambio concreto cuyos lugares tocados miden el diseño. |
| Frontera de evidencia | La línea entre lo que prueba la ejecución y lo que infieres. |

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
