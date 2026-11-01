# archaeology-12 — Nombres y comentarios

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** nombres y comentarios · **Prerrequisito:** archaeology-04 ·
> **Tiempo estimado:** 60 minutos
> **Lectura:** ~19 minutos

## Por qué existe esta lección

`archaeology-04` encontró fallos que cambian el comportamiento. Esta lección trata un fallo que no
cambia nada en tiempo de ejecución y aun así cuesta dinero todos los días: el código funciona, pero
sus nombres y comentarios obligan a cada lector a reconstruir lo que significa.

El catálogo lo llama `failure-019`, un fallo de práctica profesional. La línea base del archivo
tiene 930 líneas y 119 líneas con `//`. Algunos comentarios explican una decisión. Muchos narran la
sentencia siguiente. Algunos son falsos porque se copiaron de otra rama y nunca se actualizaron.
Los identificadores son abreviaturas españolas breves: `vectab`, `cab1`, `cf1`, `cc1`, `jugada`,
`orden`, `ganador`. Ninguno es un defecto por sí mismo. El defecto es que el nombre no carga la
decisión, así que el comentario tiene que cargarla, y el comentario no se prueba.

El banco de pruebas es deliberadamente pequeño. `before.cpp` y `after.cpp` calculan el mismo índice
de tablero con nombres distintos. Ambos imprimen `index=19`. La salida es idéntica porque un
renombrado debe preservar el comportamiento. La lección no es "hacerlo más bonito"; es "cambiar el
nombre, mantener el contrato y escribir por qué el nombre nuevo es mejor".

## Qué podrás hacer

Al terminar esta lección puedes:

- leer un identificador breve y decir qué decisión esconde;
- renombrar una función, sus parámetros y una variable local sin cambiar el comportamiento;
- decidir si un comentario es un motivo, una advertencia, una restricción, narración o una mentira;
- borrar un comentario que un nombre mejor reemplaza, y conservar los que el código no puede decir;
- demostrar un renombrado con la misma salida y un `match=yes` explícito;
- nombrar qué no arregla un renombrado: números mágicos, comprobaciones de límites ausentes, pruebas
  ausentes;
- precificar un defecto de nombres por tiempo de lectura y coste de incidente, no por gusto;
- mantener el archivo de solo lectura mientras trabajas.

La lección está hecha cuando
otra persona puede reproducir tu ejecución y seguir tus motivos.

## El concepto: nombres y comentarios

### Un nombre es una compresión de una decisión

Cada identificador es una promesa sobre lo que el lector puede dejar de pensar. `calc` promete casi
nada: dice que ocurre un cálculo, lo cual es cierto en casi cualquier función. `board_cell_index`
promete un dominio (tablero), una operación (índice) y un resultado (un índice de celda). El lector
puede saltarse el cuerpo hasta que necesite la fórmula.

La misma prueba se aplica a los parámetros. `f` y `c` no son incorrectos por ser cortos; son
incorrectos porque el cuerpo `f * 8 + c` tiene dos lecturas plausibles, por filas y por columnas, y
los nombres no eligen. `row` y `column` eligen. La multiplicación por 8 se lee entonces como "ocho
columnas por fila", que es la regla real.

Un nombre útil tiene cuatro propiedades:

| Propiedad | Prueba | Malo | Mejor |
|---|---|---|---|
| Pronunciable | ¿Puedes decirlo en una revisión? | `vectab` | `board` |
| Buscable | ¿Puedes buscarlo sin ruido? | `i`, `aux` | `move_index`, `swap_value` |
| Del dominio | ¿Usa las palabras del juego? | `data1` | `knight_cell` |
| Consistente | ¿El mismo concepto conserva un nombre? | `cell`, `pos`, `index` | `cell_index` en todas partes |

La abreviatura es un presupuesto, no un estilo. `cfg`, `id`, `url` e `i` están bien porque son
universales en su contexto. `vectab` no es universal: es `vector tabla`, una abreviatura española
para un arreglo del tablero. Ahorró unas pulsaciones una vez y cuesta a cada lector posterior un
paso de interpretación.

El idioma es una decisión aparte. El archivo está en español porque el dominio es español. Eso no
es un defecto. El defecto sería mezclar español e inglés para el mismo concepto dentro de una
frontera. Este laboratorio usa identificadores en inglés porque las lecciones están en inglés; el
archivo conserva sus nombres españoles porque es evidencia. Elige un idioma por frontera, escribe la
decisión y traduce mediante un glosario (`common-02`), no sobre la marcha.

### Un comentario es un contrato sobre lo que el código no puede decir

Un comentario no es documentación gratis. Es una segunda fuente de verdad, y no tiene prueba. Cuando
el código cambia y el comentario no, el comentario se convierte en una mentira que el lector puede
creer más que al código. Un comentario obsoleto es un caso especial: fue verdad una vez, y el código
siguió adelante. Git puede mostrar cuándo cambió el comentario; el compilador no.

Hay cinco clases de comentario que vale la pena conservar:

| Clase | Ejemplo | Por qué sobrevive |
|---|---|---|
| Motivo | `// The sentinel 0 ends the game; 1 is a real move.` | La convención no está en el tipo. |
| Advertencia | `// Caller must pass 0 <= row < 8.` | La precondición no está en el tipo. |
| Restricción | `// El tablero es 8 por 8: 64 celdas, índice 0..63.` | Las dimensiones no están en el tipo. |
| Solución temporal | `// MSVC 6.0 rejects the standard form.` | La restricción es invisible en la fuente. |
| Invariante no obvio | `// The counter also ends the loop; do not reset it.` | El acoplamiento no está en el tipo. |

Todo lo demás es candidato a borrarse. `// increment i` sobre `i++` es narración.
`// calculates the index` sobre `board_cell_index` es narración con pasos extra. Un comentario
que describe *qué* hace la línea siguiente casi siempre se puede reemplazar por un nombre que lo
diga una vez.

La regla de esta lección es directa: **si un nombre mejor puede reemplazar el comentario, reemplaza
el comentario**. Si el comentario dice algo que el nombre no puede — un motivo, una advertencia, una
unidad, una restricción — consérvalo y hazlo preciso.

### Qué contiene realmente el código heredado

El banco de pruebas es una reconstrucción, y el registro debe decirlo. `conversionmv` en
`variant-01:226` es exactamente `cf * 8 + cc`. El banco lo reduce a `calc` y acorta `cf, cc` a
`f, c` para que el fallo de una sola letra sea visible en ocho líneas. El archivo ya usa parámetros
de dos letras; el banco los comprime. Es una compresión didáctica, no una afirmación sobre el
original.

Lo que sí es literal es el estilo de nombres y el estilo de comentarios:

| Anclaje | `variant-01` | Qué muestra |
|---|---:|---|
| Comentario de cabecera con una descripción del programa | 1–8 | Una descripción, no un contrato. |
| `//librerias necesarias para el programa` | 9 | Narración del siguiente `#include`. |
| `simcab1=21,simcab2=156,sep=219` | 16 | Tres números mágicos sin significado nombrado. |
| `vectab[64],vecjugadas[8]` | 17 | Dos conceptos distintos en un comentario. |
| `cab1,cab2,jugada,orden,ganador` | 18 | El comentario menciona tres de cinco nombres. |
| `inicializacion()//... ke kontiene ...` | 19 | Error ortográfico en la única explicación. |
| `conversionvm()//// ...` | 22 | Marcador duplicado; nombre gemelo casi idéntico. |
| `conversionmv(int cf,int cc)` | 226 | La fórmula que el banco aísla. |
| `srand(time(NULL));// cambia la semilla` | 76 | El comentario repite la llamada. |
| `tirocab1=...;//... para el caballo 2` | 78 | El comentario nombra al caballo equivocado. |
| `if ( cab==1 )//... no haya estado ...` | 310, 366, 422, 478, 591 | El comentario describe la comprobación siguiente. |
| Comprobación de caballo encerrado `i==8&&cab==1` | 702 | Un comentario largo que explica una regla real. |
| `ganador=0` al capturar | 257 y 15 más | El nombre dice "ganador"; cero significa que hay ganador. |

El propio `05-known-issues.md` del archivo dice lo mismo: los comentarios a menudo narran lo obvio,
contienen errores ortográficos o describen comportamiento que cambió. Es el archivo admitiendo la
deuda, no el laboratorio inventándola.

Dos hechos merecen una pausa.

Primero, `ganador` se inicializa a `1` y se pone a `0` cuando un caballo captura al otro. La
impresión pregunta después `if(ganador==0)`. El nombre sugiere un ganador, pero el valor es una
bandera invertida: `0` significa "la partida tiene ganador". Renombrarlo `winner` preservaría el
fallo en una palabra más bonita. El renombrado honesto es `game_has_winner` con la polaridad
corregida, o `winner_exists` con `true` al capturar. Eso es un cambio de comportamiento y pertenece
a un paso separado, no a un renombrado puro.

Segundo, `jugada` es a la vez el contador de movimientos y el centinela de fin de partida: se
incrementa hacia `vectab[ccab]`, y ponerlo a `0` termina el bucle `while (jugada!=0)`. Un nombre no
puede cargar los dos significados. Un renombrado solo escondería el acoplamiento; el arreglo real
son dos variables, que es territorio de `archaeology-05`. Nombrar expone el diseño; no lo
reemplaza.

## Antes de empezar

Lee estos documentos completos:

- [`README-es.md`](README-es.md) — la ficha de la lección, el comando de compilación y el criterio
  de aceptación.
- [`RENAME-TEMPLATE-es.md`](RENAME-TEMPLATE-es.md) — la tabla que vas a completar.
- [archaeology-04](../archaeology-04-correctness-failures/lesson-es.md) — el prerrequisito:
  fichas de fallo, arreglos mínimos y la diferencia entre síntoma y causa raíz.
- [`../../../docs/guide/archaeology-track-es.md`](../../../docs/guide/archaeology-track-es.md) — §3,
  la taxonomía de fallos; §5, la escala de severidad; §6, tono y ética.
- [`../../../docs/archive-es.md`](../../../docs/archive-es.md) — el archivo fijado, la etiqueta y el
  commit.

También necesitas:

- un clon local del archivo en la etiqueta `v1.0.0`;
- un compilador de C++20 (`c++`, `g++` o `clang++`);
- `bash`;
- un directorio de trabajo temporal fuera de `original/`.

> **Regla:** el archivo es de solo lectura. Copia cualquier fichero que quieras cambiar a un
> directorio de trabajo temporal; nunca edites `original/`. El banco de pruebas de esta lección
> también es evidencia: cópialo antes de editarlo.

## El camino de 60 minutos

| Min | Paso | Qué haces | Evidencia |
|---|---|---|---|
| 0–8 | Predecir | Adivinar qué significa cada nombre y si un renombrado cambia la salida | Tabla de predicciones |
| 8–20 | Inspeccionar | Leer ambos programas, ejecutarlos, censar nombres y comentarios del archivo | Notas de anclajes y `match=yes` |
| 20–35 | Renombrar | Completar la tabla, editar una copia de trabajo, compilar | Fichero renombrado y motivos |
| 35–43 | Comparar | Ejecutar la línea base y la copia renombrada, comparar la salida | `before=index=19`, `after=index=19` |
| 43–53 | Explicar | Auditar cada comentario y precificar el defecto | Tabla de comentarios y nota de coste |
| 53–60 | Registrar | Rellenar `EVIDENCE-es.md`; confirmar que el archivo está limpio | Entrada de portafolio |

Si la auditoría de comentarios toma quince minutos, esa investigación es la lección.

### La comprobación de ingeniería en esta lección

Track B usa el mismo hábito que Track A, con otro verbo: el comportamiento ya existe, así que lo
**preservas** mientras cambias los nombres.

| Paso | Dónde ocurre | Qué produce |
|---|---|---|
| **Predecir** | Paso 1 | Una conjetura falsable sobre significado y salida. |
| **Inspeccionar** | Paso 2 | Los dos programas, la ejecución y los anclajes del archivo. |
| **Renombrar** | Paso 3 | Una copia de trabajo con nombres nuevos y una tabla completa. |
| **Comparar** | Paso 4 | Salida idéntica y un `match=yes` explícito. |
| **Explicar** | Paso 5 | Auditoría de comentarios y coste. |
| **Registrar** | Paso 6 | `EVIDENCE-es.md` con comandos, motivos y fronteras. |

## Dos sesiones

Esta lección es larga a propósito. Divídela en dos sesiones:

- **Sesión 1 — Núcleo (Pasos 1–3, unos 35 min).** Detente después del Paso 3, cuando los nombres
  están renombrados y el diff es visible.
- **Sesión 2 — Extensión (Pasos 4–6, unos 25 min).** Empieza solo después de la Sesión 1. Vas a
  comparar, explicar y registrar el renombrado.

## Paso 1 — Predecir (8 min)

Antes de abrir la fuente, responde por escrito estas cuatro preguntas. El objetivo es comprometerte
con respuestas sobre las que puedas equivocarte.

1. ¿Qué calcula `calc`: una fila, una columna, un índice de celda del tablero o una distancia?
2. ¿Qué significan `f` y `c`, y en qué orden se multiplican?
3. Si renombras la función y los dos parámetros, ¿cambia la salida impresa?
4. ¿Cuántos comentarios del archivo explican un motivo en lugar de narrar una sentencia?

Una predicción útil es falsable. "Los nombres son malos" no lo es. "`calc` es por filas, con `f`
como fila y `c` como columna, así que `calc(2, 3)` es 19, y un renombrado no puede cambiar eso" sí
lo es.

### Una predicción trabajada

Antes de leer, yo escribiría:

- `calc` devuelve un índice de celda del tablero, no un cálculo geométrico;
- `f` es `fila`, `c` es `columna`, así que la fórmula es `row * 8 + column`;
- la salida sigue siendo `index=19`, porque el compilador borra los nombres;
- la mayoría de los comentarios narran; unos pocos cargan un motivo o una advertencia.

Cada afirmación se puede comprobar. Si tu predicción dice "el código es ilegible", hiciste un
juicio, no una estimación. Escribe el significado que esperas y luego pruébalo.

## Paso 2 — Inspeccionar (12 min)

### Leer el banco de pruebas

```bash
cd archaeology/lessons/archaeology-12-naming-comments
nl -ba code/before.cpp
nl -ba code/after.cpp
diff -u code/before.cpp code/after.cpp
```

`before.cpp` tiene 13 líneas:

```cpp
#include <cstdio>

// calcula el indice del tablero
int calc(int f, int c)
{
    return f * 8 + c;
}

int main()
{
    std::printf("index=%d\n", calc(2, 3));
    return 0;
}
```

`after.cpp` tiene 12 líneas. Las únicas diferencias son el comentario, el nombre de la función y los
nombres de los parámetros. La expresión, la constante, la llamada y la salida son idénticas.

Lee el diff dos veces. La primera, pregunta qué cambió. La segunda, pregunta qué **no** cambió: el
`8`, la falta de comprobaciones de límites, el único sitio de llamada, la ausencia de una prueba. Un
renombrado es un cambio quirúrgico, y el diff es la prueba de que siguió siendo quirúrgico.

### Ejecutar la línea base

```bash
./run.sh
```

Salida esperada:

```text
before=index=19
after=index=19
match=yes
```

El script compila ambos ficheros con:

```text
-std=c++20 -Wall -Wextra -Werror -O2
```

`-Werror` importa. Un renombrado que introduce un parámetro sin usar o una variable sin usar hará
fallar la compilación, que es exactamente la retroalimentación que quieres. Si tu compilador rechaza
la línea base, registra el compilador y el diagnóstico; no silencies el aviso para que la ejecución
pase.

### Censar los nombres del archivo

En el archivo fijado en la etiqueta `v1.0.0`:

```bash
grep -c '//' original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
grep -n 'vectab\|vecjugadas\|cab1\|cf1\|jugada\|orden\|ganador' \
    original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp | head -20
```

El primer comando informa `119`. El fichero tiene 930 líneas, así que aproximadamente una de cada
ocho líneas lleva un comentario.

Ahora lee las declaraciones de las líneas 15–30. Son la superficie de nombres más densa del fichero:

```text
16: const char simcab1=21,simcab2=156,sep=219;//simbolos usados para imprimir el tablero
17: int  vectab[64],vecjugadas[8];//vector que almacena la informacion del tablero ...
18: int  cab1,cab2,jugada=1,orden,ganador=1;//variables para identificar la posicion ...
```

La línea 16 declara tres números mágicos sin nombres. La línea 17 mete dos conceptos
distintos en un comentario. La línea 18 nombra tres de las cinco variables y omite `orden` y
`ganador`. Nada de esto es un crash. Todo esto es deuda para el lector.

Cuenta las referencias antes de proponer un renombrado global:

```bash
grep -c 'vectab' original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
grep -c 'jugada' original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

`vectab` aparece en 80 líneas y `jugada` en 76. Esa es la diferencia entre renombrar una local en
el banco y renombrar una global en el archivo. El banco es un fichero con un sitio de llamada; el
archivo es una sola unidad de traducción donde cada nombre es global.

### Comprobar tu predicción

`calc(2, 3)` es `2 * 8 + 3 = 19`. `f` es la fila y `c` es la columna. La salida no cambia con un
renombrado. La mayoría de los comentarios narran; la cabecera describe el programa, y el comentario
del caballo encerrado explica una regla.

Si tu predicción decía que los comentarios explican más de lo que lo hacen, encontraste la primera
brecha: la mayoría repite la sentencia siguiente. Anótalo. La brecha entre predicción y observación
es la señal de aprendizaje.

## Paso 3 — Renombrar (15 min)

### Hacer la copia de trabajo

```bash
mkdir -p build/scratch
cp code/before.cpp build/scratch/renamed.cpp
```

De aquí en adelante, edita `build/scratch/renamed.cpp`. No edites `code/before.cpp`; es la línea
base que ejecutará la siguiente persona.

### Completar la tabla de renombrado

Abre [`RENAME-TEMPLATE-es.md`](RENAME-TEMPLATE-es.md) y completa la primera tabla. Un motivo no es
"es más claro". Un motivo nombra la decisión que el nombre anterior escondía y el lector que pagaba
por ella.

| Nombre anterior | Nombre nuevo | Motivo | Alcance |
|---|---|---|---|
| `calc` | `board_cell_index` | `calc` nombra una acción que toda función realiza; el resultado es un índice de celda del tablero. | Función, un fichero. |
| `f` | `row` | `f` podía ser `fila` o `factor`; la multiplicación por 8 la convierte en la fila. | Parámetro, una función. |
| `c` | `column` | `c` podía ser `column`, `count` o `cell`; el término `+ c` es el desplazamiento de columna. | Parámetro, una función. |

Tres filas, tres motivos, tres alcances. Si una fila dice "más descriptivo", reescríbela hasta que
nombre la ambigüedad que el nombre nuevo elimina. Tus nombres pueden diferir: `cell_index` o
`board_index` también son defendibles si el motivo nombra la lectura que eligen. El criterio de
aceptación es el motivo, no la palabra exacta.

### Aplicar el renombrado

```cpp
#include <cstdio>

int board_cell_index(int row, int column)
{
    return row * 8 + column;
}

int main()
{
    std::printf("index=%d\n", board_cell_index(2, 3));
    return 0;
}
```

Compila con las banderas que trae la lección y ejecuta:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 build/scratch/renamed.cpp \
    -o build/scratch/renamed
./build/scratch/renamed
```

Salida esperada:

```text
index=19
```

### Qué no toca el renombrado

- La constante `8`. Sigue siendo un número mágico; nombrarla `board_width` es otro cambio.
- Los límites. `board_cell_index(9, 9)` devuelve 81, fuera de un tablero de 64 celdas.
- Los argumentos del sitio de llamada. `(2, 3)` sigue siendo una fila y una columna, ahora de forma
  visible.
- El formato de salida. `index=%d` es un contrato con el llamador y no cambió.
- La prueba ausente. Un `printf` es una demostración, no una prueba de regresión.

Esta frontera es el punto de un renombrado mínimo. Cambiaste tres nombres y un comentario, y nada
más. Si tu diff contiene un helper nuevo, una constante nueva o una comprobación nueva, te saliste
de la lección.

### La versión a escala del archivo

Un renombrado global en el archivo es mecánico pero no gratis. `vectab` aparece en 80 líneas,
`jugada` en 76, y ambos son globales en un solo fichero de 930 líneas. La secuencia profesional
es:

1. renombrar un concepto a la vez;
2. compilar después de cada renombrado;
3. mantener los cambios de comportamiento en un commit separado;
4. usar el compilador, no un buscar-y-reemplazar, para encontrar los sitios de llamada;
5. registrar el número de referencias tocadas.

No renombres `jugada` a `move_number` y lo des por terminado. También es el centinela que termina la
partida. Un renombrado puro haría el doble significado más difícil de ver, no más fácil. Eso es un
arreglo de diseño, no un arreglo de nombres.

## Paso 4 — Comparar (8 min)

### Ejecutar ambos caminos

```bash
./run.sh
c++ -std=c++20 -Wall -Wextra -Werror -O2 build/scratch/renamed.cpp \
    -o build/scratch/renamed
./build/scratch/renamed
```

Observado:

```text
before=index=19
after=index=19
match=yes
index=19
```

La copia renombrada imprime la misma línea que `before.cpp` y `after.cpp`. Tres nombres cambiaron,
cero bytes de salida cambiaron.

### Qué prueba la igualdad

- El compilador resuelve los nombres nuevos a la misma expresión.
- El orden de argumentos sigue siendo fila y luego columna. Intercambiarlos imprimiría `index=26`.
- El renombrado no cambió la constante, el operador ni la cadena de formato.
- Para esta entrada, el renombrado preserva el comportamiento.

### Qué no prueba la igualdad

| Afirmación | ¿Probada? | Por qué |
|---|---|---|
| El renombrado es correcto para toda entrada. | No. | Solo se ejecutó `(2, 3)`. |
| Los nombres nuevos son los mejores posibles. | No. | Eso es un juicio de revisión, no una prueba. |
| La función es correcta para un tablero real. | No. | Sin comprobación de límites; `(9, 9)` devuelve 81. |
| Los comentarios son ahora verdaderos. | No. | El banco tiene un comentario; el archivo tiene 119 líneas de ellos. |
| Las globales del archivo se renombraron. | No. | El banco no tiene globales. |
| El código es más fácil de mantener. | En parte. | La tabla de motivos es el argumento; la ejecución solo prueba igualdad. |

### La dirección del oráculo

La línea base es el oráculo porque es el código que existía. La copia renombrada es el candidato
porque es el código en el que estás confiando. Si generas ambos desde los mismos nombres nuevos, la
comparación es circular.

Es la misma regla que en `archaeology-06` y `archaeology-10`, aplicada a un cambio más pequeño:
**congela el comportamiento antiguo antes de cambiar las palabras**.

### Cuando aparece una discrepancia

Un renombrado no debería cambiar la salida. Si lo hace, la causa es casi siempre una de estas:

- los argumentos se intercambiaron (`index=26` en lugar de `19`);
- la constante se transcribió como `9` o el operador como `+` en lugar de `*`;
- la cadena de formato se editó junto con los nombres;
- la llamada sigue apuntando a la función antigua porque la edición fue parcial.

Imprime la expresión que falla y compárala con la línea base. Una discrepancia es un error de
transcripción, no un motivo para cambiar la salida esperada.

## Paso 5 — Explicar (10 min)

### Auditar los comentarios

Completa la segunda tabla de [`RENAME-TEMPLATE-es.md`](RENAME-TEMPLATE-es.md). Por cada comentario
que toques, registra cuatro cosas: dónde está, qué decía, qué dice ahora y por qué. Los cuatro
verbos son **conservar**, **reescribir**, **borrar** y **reparar**.

| Ubicación | Antes | Después | Motivo |
|---|---|---|---|
| `before.cpp:3` | `// calcula el indice del tablero` | *(borrado)* | El nombre de la función ya lo dice; el comentario era narración. |
| `v01:1–8` | Descripción del programa | Conservar | El código no dice para qué sirve el programa. |
| `v01:9` | `//librerias necesarias...` | *(borrado)* | Las líneas `#include` lo dicen. |
| `v01:16` | `//simbolos usados...` | `// Board glyphs: knight, knight, wall.` | El comentario enumera números; los nombres deben decir qué dibujan. |
| `v01:18` | Nombra tres de cinco variables | Reescribir o borrar | El comentario está incompleto y se desviará. |
| `v01:19` | `//inicializa ... ke kontiene...` | *(borrado)* | `clear_board()` lo dice; el error ortográfico se va con él. |
| `v01:40` | `// // designa...` | *(borrado)* | Marcador duplicado y narración. |
| `v01:76` | `// cambia la semilla` | *(borrado)* | `srand(time(NULL))` es la sentencia. |
| `v01:78` | `//... para el caballo 2` | Reparar o borrar | Describe `tirocab1` como caballo 2; el comentario miente. |
| `v01:310` | `//... no haya estado...` | *(borrado)* | El `if` comprueba de quién es el turno; la comprobación de visitadas es la línea siguiente. |
| `v01:702` | Comentario largo del caballo encerrado | Acortar a la regla | La regla es real; la narración no. |

Dos filas merecen una nota más larga.

**La mentira de la línea 310.** El comentario dice "verifica que el caballo no haya estado en esa
posición antes", pero la sentencia es `if (cab==1)`, que selecciona el caballo que se mueve. La
comprobación de visitadas es el `if` siguiente. El comentario se copió de la rama de abajo y nunca
se actualizó. Un lector que confíe en él buscará una comprobación de visitadas en el lugar
equivocado. Es la clase de comentario más cara: no inútil, sino falso.

**Los nombres gemelos de 22 y 226.** `conversionvm` convierte un índice de vector a fila y columna;
`conversionmv` convierte fila y columna a índice de vector. Los nombres difieren en dos letras y
describen direcciones opuestas. Ningún comentario puede reparar eso de forma fiable; los nombres
deben pasar a `cell_to_row_column` y `row_column_to_cell`. Es un defecto de nombres que un
comentario empeoró, porque ambos comentarios usan las mismas palabras en otro orden.

### Severidad y coste

La escala de severidad del track pone nombres, comentarios y legibilidad en **baja**: mejorar cuando
se toque el fichero. Eso es correcto por incidente y engañoso por año. Un mal nombre cuesta unos
segundos en cada lectura. Un comentario falso cuesta confianza, y la confianza es lo que hace que un
revisor deje de comprobar.

El coste se vuelve alto cuando:

- alguien nuevo se incorpora y no distingue `conversionvm` de `conversionmv`;
- una revisión de incidente confía en un comentario que describe el comportamiento antiguo;
- el mismo concepto carga tres nombres en un fichero;
- un revisor deja de comprobar porque los comentarios antes acertaban.

Declara el coste en tu contexto. Un script privado de una sola vez y una biblioteca publicada no
precifican el mismo nombre.

### Dos arreglos, dos tamaños

**Arreglo mínimo — renombrar y reparar los comentarios que tocas.** Renombra `calc`, `f` y `c`;
borra el comentario que narra. Eso es lo que hacen los pasos 3–5. Elimina la ambigüedad local y deja
en pie el `8` mágico, la comprobación de límites ausente y la prueba ausente.

**Arreglo estructural — hacer que los nombres sean parte del contrato.** Añade un glosario
(`common-02`), una regla de revisión para comentarios y una tabla de nombres para las globales del
archivo. En el archivo, eso significa un nombre por concepto y una política de comentarios. Enlaces
a Track A: `common-02` (glosario), `construction-03` (funciones y parámetros) y `construction-08`
(frontera de módulo).

### Qué no arregla un renombrado

- El `8` mágico. Sigue siendo un ancho de tablero sin nombre.
- La comprobación de límites ausente. `board_cell_index(9, 9)` sigue siendo 81.
- El doble significado de `jugada`. Un renombrado puede esconderlo; solo un cambio de diseño lo
  elimina.
- La bandera invertida `ganador`. Renombrarla `winner` preservaría el fallo de polaridad.
- Las globales. El banco no tiene; el archivo tiene `vectab`, `vecjugadas` y el resto.
- Las pruebas ausentes. Un `printf` es una demostración.
- Las 119 líneas de comentarios del archivo. Auditaste una muestra, no el fichero.

### El compromiso, dicho con justicia

Un renombrado es barato en un alcance local y caro a través de una frontera pública. En el banco,
tres nombres y un sitio de llamada: cinco minutos. En el archivo, `vectab` aparece en 80 líneas y
cada una es una edición comprobada por el compilador; un renombrado global es un cambio del tamaño
de una revisión incluso cuando la herramienta escribe.

Los comentarios tienen el compromiso inverso. Borrar un comentario que narra es gratis. Borrar un
comentario que explica un motivo es caro, porque el motivo puede no existir en ningún otro sitio.
Por eso la auditoría pide un motivo por fila: la tabla es el único lugar donde "conservar" y
"borrar" se distinguen por evidencia y no por gusto.

## Paso 6 — Registrar (7 min)

Completa también las secciones 3 y 4 de [`RENAME-TEMPLATE-es.md`](RENAME-TEMPLATE-es.md): la salida
de antes y después, el compilador, la coincidencia, qué no cambiaste, los nombres que no pueden
cambiar sin tocar llamadores y los comentarios que todavía son necesarios.

Rellena [`EVIDENCE-es.md`](EVIDENCE-es.md):

- configuración y fijación del archivo;
- las cuatro predicciones, escritas antes de inspeccionar;
- el inventario del banco: 13 líneas antes, 12 después, un sitio de llamada;
- los anclajes del archivo: 119 líneas de comentario, 80 líneas con `vectab`, 76 con `jugada`;
- la ejecución de la línea base y su salida exacta;
- la tabla de renombrado con un motivo por fila;
- la auditoría de comentarios con un verbo por fila;
- el resultado de la comparación y la dirección del oráculo;
- la frontera de evidencia: qué prueba la ejecución y qué no;
- la comprobación de integridad del archivo.

Una frase como "los nombres son más claros" es una opinión. "Tres nombres, un comentario borrado,
mismo `index=19`, 119 líneas de comentario en el archivo" es un registro.

## El original en detalle

Datos que puedes citar, con números de línea de `variant-01` en la etiqueta `v1.0.0`:

| Ubicación | Qué hay |
|---:|---|
| 1–8 | Comentario de cabecera: descripción del programa. |
| 9 | `//librerias necesarias para el programa` — narración. |
| 15 | `//variables,vectores y prototipos...` — narración. |
| 16 | `simcab1=21,simcab2=156,sep=219` — tres números mágicos. |
| 17 | `vectab[64],vecjugadas[8]` — dos conceptos, un comentario. |
| 18 | `cab1,cab2,jugada=1,orden,ganador=1` — cinco nombres, tres explicados. |
| 19 | `inicializacion()` con `ke kontiene` — error en la única explicación. |
| 22 | `conversionvm()////` — marcador duplicado y nombre gemelo. |
| 32 | `void main()//funcion principal` — narración sobre un defecto de portabilidad. |
| 40 | `cabubicacion();// // designa...` — marcador duplicado. |
| 61 | `for (i=0;i<=64;i++)` — el off-by-one que los comentarios no atraparon. |
| 76 | `srand(time(NULL));// cambia la semilla` — narración. |
| 78 | `tirocab1=...;//... para el caballo 2` — caballo equivocado. |
| 226 | `int conversionmv(int cf,int cc)` — la fórmula que el banco aísla. |
| 249 | `if ( cab==1 )//verifica de quien es el turno...` — comentario correcto. |
| 310, 366, 422, 478, 591 | `if ( cab==1 )` con el comentario de visitadas — sentencia equivocada. |
| 702 | Comprobación de caballo encerrado con un comentario largo pero correcto. |
| 930 | Fin del fichero; sin pruebas, sin fichero de compilación. |

La superficie de nombres, con reemplazos sugeridos. Son propuestas, no un mandato; cada una tiene un
coste en el sitio de llamada.

| Nombre del archivo | Significado | Nombre sugerido | Nota |
|---|---|---|---|
| `vectab` | Ocupación del tablero, 64 celdas | `board` | 80 líneas lo mencionan. |
| `vecjugadas` | Orden de los ocho movimientos | `move_order` | 8 entradas. |
| `cab1`, `cab2` | Índice de celda del caballo | `knight_one_cell`, `knight_two_cell` | Global. |
| `cf1`, `cc1` | Fila y columna del caballo uno | `knight_one_row`, `knight_one_column` | Cuatro globales. |
| `jugada` | Número de movimiento y centinela de fin | `move_number` + `game_over` | Necesita un cambio de diseño. |
| `orden` | Qué caballo empieza | `first_player` | Se compara con `1` y `2`. |
| `ganador` | Bandera invertida de ganador | `game_has_winner` | Corregir la polaridad aparte. |
| `cab` | Qué caballo se mueve | `moving_knight` | Ocho sitios `if (cab==1)` más las comprobaciones de encierro. |
| `ccab` | Celda de destino | `destination_cell` | Local en `movcab`. |
| `tirocab1`, `tirocab2` | Tirada para el orden de salida | `knight_one_roll`, `knight_two_roll` | Local. |
| `simcab1`, `simcab2` | Glifo de consola de un caballo | `knight_one_glyph`, `knight_two_glyph` | Constante de glifo. |
| `sep` | Glifo de consola de pared | `wall_glyph` | Constante de glifo. |
| `conversionvm` | Celda a fila y columna | `cell_to_row_column` | Opuesto de la fila siguiente. |
| `conversionmv` | Fila y columna a celda | `row_column_to_cell` | Nombre gemelo. |

Lee dos veces las filas de `jugada` y `ganador`. Son los casos donde un renombrado no basta. El
nombre no es solo poco claro; carga dos significados o una convención invertida. Renombrar sin
cambiar el diseño haría que el código se viera mejor y se comportara igual, que es exactamente la
trampa contra la que advierte esta lección.

## Qué prueba y qué no prueba el renombrado

| Afirmación | ¿Respaldada? | Por qué |
|---|---|---|
| La copia renombrada imprime la misma salida. | Sí | `index=19` desde ambos caminos. |
| El renombrado preserva el comportamiento para toda entrada. | No | Solo se ejecutó `(2, 3)`. |
| Los nombres nuevos eliminan la ambigüedad local. | Sí, por la tabla de motivos | `row` y `column` eligen la lectura. |
| Los nombres nuevos son los mejores posibles. | No | Es un juicio de revisión. |
| El borrado del comentario es seguro. | Sí, para el banco | El nombre carga el significado. |
| Los comentarios del archivo están todos reparados. | No | Se auditó una muestra. |
| Las globales del archivo se renombraron. | No | El banco no tiene globales. |
| La función es correcta para un tablero real. | No | Sin comprobación de límites. |
| El código ahora tiene pruebas. | No | Un `printf` es una demostración. |

Esta tabla es la comprobación de honestidad. Un registro que dice "el código es más limpio" sin
nombrar la muestra se está excediendo. Di "para esta entrada" cuando eso es lo que ejecutaste.

## Autocomprobación (binaria)

- [ ] Escribí las cuatro predicciones antes de abrir la fuente.
- [ ] Leí `before.cpp` y `after.cpp` y los comparé con `diff`.
- [ ] Ejecuté `./run.sh` y vi `before=index=19`, `after=index=19`, `match=yes`.
- [ ] Registré el compilador y las banderas.
- [ ] Completé las tres filas de la tabla de renombrado con un motivo y un alcance.
- [ ] Renombré la función y los dos parámetros en una copia de trabajo.
- [ ] La copia renombrada imprime `index=19`.
- [ ] Borré o reescribí el comentario que narraba y registré por qué.
- [ ] Clasifiqué al menos seis comentarios del archivo como conservar, reescribir, borrar o reparar.
- [ ] Cité el comentario del caballo equivocado en la línea 78 y el de la sentencia equivocada en
  310.
- [ ] Nombré qué no arregla el renombrado: `8` mágico, límites, `jugada`, `ganador`, pruebas.
- [ ] Precifiqué el defecto por tiempo de lectura y coste de incidente, no por gusto.
- [ ] Enlacé el arreglo estructural a `common-02` y `construction-03`.
- [ ] El archivo está sin modificar (`git status` en el archivo está limpio).

Si una casilla no está marcada, la casilla sin marcar nombra el siguiente paso. No hay penalización
por repetir uno.

## Misconcepciones que esta lección refuta

| Idea equivocada | Por qué está mal | Qué hacer en su lugar |
|---|---|---|
| "Renombrar es cosmético." | Un nombre es la compresión que el lector hace de una decisión; uno malo cuesta en cada lectura. | Renombra con una tabla de motivos y demuestra igualdad de salida. |
| "Los nombres cortos siempre son malos." | `i`, `id` y `cfg` son convencionales y locales. | Juzga por alcance y ambigüedad, no por longitud. |
| "Los identificadores en español son el defecto." | El idioma no es el problema; la decisión ausente sí. | Conserva el idioma del dominio y haz específico el nombre. |
| "Todo comentario es documentación." | Un comentario sin prueba se convierte en una segunda verdad que se desvía. | Conserva motivos, advertencias y restricciones; borra la narración. |
| "Un comentario puede arreglar un mal nombre." | El comentario no se comprueba y se copiará con el código. | Arregla el nombre; conserva el comentario solo para lo que el nombre no puede decir. |
| "Un comentario falso es inofensivo." | Envía al lector a la sentencia equivocada, peor que no tener comentario. | Bórralo o repáralo con el número de línea como evidencia. |
| "Un renombrado y un arreglo de comportamiento pueden compartir commit." | Entonces el diff ya no prueba que el renombrado preservó el comportamiento. | Separa los dos cambios. |
| "Renombrar `ganador` a `winner` lo arregla." | El valor está invertido; el nombre nuevo preservaría el fallo. | Renombra a `game_has_winner` y corrige la polaridad aparte. |
| "El banco es el código original." | El banco comprime `conversionmv` y acorta `cf, cc` a `f, c`. | Declara la frontera de reconstrucción en el registro. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Identificador | Un nombre que el compilador resuelve a una declaración. |
| Comentario de narración | Un comentario que repite lo que hace la sentencia siguiente. |
| Comentario de motivo | Un comentario que explica por qué el código tiene esta forma. |
| Comentario obsoleto | Un comentario que describía una versión anterior del código. |
| Oráculo | La implementación cuyo comportamiento confías mientras cambias el código. |
| Preservar el comportamiento | La salida no cambia para las entradas probadas. |
| Alcance | La región donde un nombre se puede cambiar sin tocar llamadores. |
| Número mágico | Un literal cuyo significado no está nombrado. |
| Frontera de evidencia | La línea entre lo que prueba la ejecución y lo que infieres. |

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
