# archaeology-08 — Frontera de módulo ausente

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** frontera de módulo · **Prerrequisito:** archaeology-05 ·
> **Tiempo estimado:** 90 minutos
> **Lectura:** ~14 minutos

## Por qué existe esta lección

`archaeology-05` mostró que el tablero no tiene dueño. Cada función alcanza `vectab` y lo escribe.
Esa lección terminó con una pregunta que no respondió: ¿por qué podía cada función alcanzarlo?

Porque no había nada a través de lo cual alcanzarlo. El programa archivado de 2008 es **una sola
unidad de traducción**. Tiene 930 líneas, once funciones, un `main`, once prototipos y **cero
cabeceras**. El tablero se declara en la línea 17 como `int vectab[64]` y cualquier función del
archivo puede indexarlo directamente:

```text
17:  int  vectab[64],vecjugadas[8];
63:          vectab[i]=0;
122:         if(vectab[k]==0)
269:  vectab[ccab]=jugada;
293:  vectab[ccab]=jugada;
```

Eso no es un accidente de estilo. Un solo archivo con estado de alcance de archivo es el camino más
corto a un programa que funciona para un desarrollador, con un compilador y una fecha de entrega.
Fue un intercambio razonable para un programa local. El coste llega después, cuando una segunda
persona necesita saber qué puede hacer el tablero.

El defecto es que **el compilador no puede ayudar**. Nada en el archivo dice "esto es asunto del
tablero". Una función nueva puede inventar una novena forma de indexar el arreglo y el compilador la
compilará. El tablero es público porque todo lo que hay en una unidad de traducción es público.

Este es un fallo de **diseño**. El programa funciona. El coste es que cada cambio en el tablero es
un cambio en todo el archivo, y ninguna herramienta puede avisarte cuando cruzaste la línea.

El producto es un **módulo**: una cabecera que declara lo que los llamadores pueden usar, y una
implementación que guarda todo lo demás. La prueba no es una opinión. Es un error del compilador en
la línea donde un llamador intenta tocar la representación.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar qué es una unidad de traducción y por qué el estado de alcance de archivo no tiene
  frontera;
- separar una **interfaz** (lo que los llamadores pueden usar) de una **implementación** (cómo se
  hace);
- escribir una cabecera que declara operaciones y esconde la aritmética de índices;
- mostrar que un llamador no puede alcanzar el arreglo visitado, con las palabras del compilador
  como evidencia;
- indicar qué mitad de la frontera impone el compilador y qué mitad impone el enlazador;
- listar qué saben los llamadores, qué no saben y cómo aprenden lo que la cabecera omite;
- nombrar los cambios que quedan dentro del módulo y los que alcanzan al llamador;
- explicar por qué una cabecera de C++ sigue mostrando miembros privados y por qué eso no es lo
  mismo que el acceso;
- distinguir una frontera de módulo de un tipo abstracto de datos, y decir qué lección viene
  después;
- mantener el archivo de solo lectura mientras trabajas.

El módulo está terminado cuando una persona ajena puede
reconstruirlo solo con la cabecera y `main.cpp`.

## El concepto: frontera de módulo

### Dos archivos, un contrato

Una **unidad de traducción** es un archivo fuente después del preprocesamiento: el `.cpp` más todo
lo que `#include`. El compilador ve las unidades de traducción de una en una. Nunca ve el programa
completo.

Una **frontera de módulo** aprovecha ese hecho. La cabecera declara nombres y tipos. La
implementación los define. El compilador comprueba cada lado contra la declaración y el enlazador
une las dos partes.

| Parte | Archivo | Quién lo lee | Qué puede contener |
|---|---|---|---|
| Interfaz | `board.h` | Todo llamador y la implementación | Declaraciones que el llamador necesita. |
| Implementación | `board.cpp` | El compilador, una vez | Definiciones, ayudantes privados, representación. |
| Llamador | `main.cpp` | El compilador, una vez | Solo lo que ofrece la cabecera. |

La frontera no es decoración. Cambia lo que es *posible*: un llamador que nunca ha visto `board.cpp`
no puede nombrar nada de su interior.

### Qué impone el compilador y qué no

Dos herramientas distintas imponen dos mitades distintas. Confundirlas es el error más común de esta
lección.

| Pregunta | La impone | Cuándo | Modo de fallo |
|---|---|---|---|
| ¿Puede un llamador nombrar este miembro? | El compilador | En compilación | `error: ... is private within this context` |
| ¿Existe cada función declarada? | El enlazador | En enlace | `undefined reference to ...` |
| ¿Hace la función lo que dice su nombre? | Nadie | Nunca | Comportamiento equivocado y silencioso |

La tercera fila es la honesta. Una frontera impide que un llamador **alcance** el arreglo. No impide
que un llamador llame a `mark(-5, 900)`. La cabecera promete un nombre y un tipo; el comportamiento
vive en el cuerpo, y en esta lección el comportamiento del cuerpo no está documentado en ninguna
parte.

### Frontera, encapsulación, tipo abstracto de datos

Tres ideas que viajan juntas y no son lo mismo:

| Idea | La pregunta que responde | Dónde se enseña |
|---|---|---|
| Frontera de módulo | ¿Quién puede ver este nombre? | Esta lección. |
| Encapsulación | ¿Quién puede romper este invariante? | `construction-11`, `archaeology-09`. |
| Tipo abstracto de datos | ¿Se puede cambiar la representación? | `construction-09`, `archaeology-09`. |

Una cabecera te da la primera. Te da un poco de la segunda, porque `private` bloquea el acceso
directo. No te da casi nada de la tercera, porque el miembro privado sigue escrito en la cabecera.
Ese último punto es el puente a la lección siguiente, y esta lección no va a fingir lo contrario.

### El problema de honestidad de C++

`private` es control de acceso, no ocultamiento de información. Mira la cabecera base:

```cpp
private:
    static constexpr int rowCount = 8;
    static constexpr int columnCount = 8;
    static constexpr int cellCount = rowCount * columnCount;

    int cell_index(int row, int column) const;
    bool is_inside(int row, int column) const;

    bool visited[cellCount];
```

Un llamador no puede escribir `board.visited[0]`. Pero un llamador puede leer que el tablero son 64
`bool` y debe recompilar cuando esa línea cambie, porque el tamaño y la disposición del objeto
cambiaron. La frontera compra **control de acceso y aislamiento de recompilación**, no secreto.

Dilo en voz alta en tu registro. Es la diferencia entre una lección que enseña un mecanismo y una
lección que enseña una creencia.

## Antes de empezar

Lee esto completo:

- [`README-es.md`](README-es.md) — la ficha de la lección y el criterio de aceptación.
- [archaeology-05](../archaeology-05-global-state/lesson-es.md) — el prerrequisito: quién es dueño
  del estado.
- [`../../../docs/guide/archaeology-track-es.md`](../../../docs/guide/archaeology-track-es.md) — §3,
  la taxonomía de fallos; §8, la secuencia de lecciones; §9, el mapeo a la pista A.
- [`../../../docs/guide/lesson-catalog-es.md`](../../../docs/guide/lesson-catalog-es.md) — la fila
  `archaeology-08` y su criterio de aceptación.
- [`../../../docs/archive-es.md`](../../../docs/archive-es.md) — el archivo fijado, etiqueta y
  commit.

También necesitas:

- un clon local del archivo en la etiqueta `v1.0.0`;
- un compilador de C++20 (`c++`, `g++` o `clang++`);
- un directorio de trabajo temporal fuera del archivo y fuera de los archivos versionados de este
  repositorio.

> **Regla:** el archivo es de solo lectura. Copia lo que necesites al directorio temporal; nunca
> edites `original/`.

## El camino de 90 minutos

| Min | Paso | Qué haces | Evidencia |
|---:|---|---|---|
| 0–15 | Predecir | Predices qué arreglará y qué no arreglará una cabecera | Tabla de predicción |
| 15–35 | Inspeccionar | Cuentas el alcance del tablero en el archivo; lees el módulo incluido | Tabla de alcance y salida |
| 35–60 | Separar | Reconstruyes el módulo solo desde `main.cpp`, en temporal | Tu `board.h` y tu `board.cpp` |
| 60–75 | Compilar | Rompes la frontera a propósito; capturas ambos errores | Mensajes exactos |
| 75–85 | Explicar | Listas qué saben los llamadores y qué puede cambiar | Tabla de frontera |
| 85–90 | Registrar | Rellenas `EVIDENCE-es.md`; confirmas que el archivo está limpio | Entrada de portafolio |

Si el error del compilador tarda quince minutos en reproducirse, eso
es información sobre tu cadena de herramientas, no un fallo.

### La comprobación de ingeniería en esta lección

La pista B usa el mismo hábito que la pista A con otro verbo: el módulo ya existe en la lección, así
que lo **reconstruyes** en lugar de escribirlo desde cero.

| Paso | Dónde ocurre | Qué produce |
|---|---|---|
| **Predecir** | Paso 1 | Una afirmación falsable sobre qué arregla una cabecera. |
| **Inspeccionar** | Paso 2 | El conteo de menciones y la superficie pública. |
| **Separar** | Paso 3 | Tu propia cabecera y tu propia implementación. |
| **Compilar** | Paso 4 | Dos errores: uno del compilador y uno del enlazador. |
| **Explicar** | Paso 5 | La tabla de conocimiento del llamador y la prueba de cambio. |
| **Registrar** | Paso 6 | La nota de frontera y `EVIDENCE-es.md`. |

## Paso 1 — Predecir (15 min)

Antes de leer `code/board.h`, escribe cinco frases. Cada una debe ser falsable: debes poder decir
qué observación demostraría que está equivocada.

1. En el archivo de 930 líneas, ¿cuántas líneas mencionan `vectab` y cuántas cabeceras tiene todo el
   archivo? Da dos números.
2. Si el tablero pasa detrás de una cabecera, ¿cuántas de las once funciones deben cambiar?
3. ¿Qué rechazará exactamente el compilador cuando `main.cpp` toque el arreglo visitado?
4. ¿Cambiará la salida del programa?
5. ¿Qué seguirá revelando la cabecera sobre la representación?

### Una predicción trabajada

> "`vectab` se menciona en más de cien líneas, porque cada cuerpo de caso lo escribe. Moverlo
> detrás de una cabecera cambia todas las funciones que lo indexan, pero no cambia ningún
> comportamiento, así que la salida sigue siendo `visited=2 first=1 second=1`. El compilador
> rechazará `board.visited[0]` con un error de privacidad. La cabecera seguirá mostrando que el
> tablero es un arreglo de 64 `bool`, porque C++ exige declarar el miembro ahí."

Esa predicción es útil incluso donde se equivoca. Si la salida cambiara, habrías encontrado un
comportamiento que dependía de que la representación fuera visible, que es exactamente el
acoplamiento del que trata esta lección.

## Paso 2 — Inspeccionar (20 min)

### Contar el alcance del tablero en el archivo

Desde la raíz del archivo:

```bash
grep -c "vectab" original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
grep -n "vectab" original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp | head -20
find original -name "*.h" -o -name "*.hpp" | wc -l
```

Una ejecución de referencia da:

```text
138
0
```

Ciento treinta y ocho menciones de un arreglo, y ni una sola cabecera en el archivo. Las menciones
no están repartidas por igual; se concentran donde los ocho cuerpos de caso escriben el número de
jugada.

| Región | Líneas | Qué hace con el tablero |
|---|---|---|
| `main` | 32–55 | Nunca lo toca; la condición del bucle es `jugada`. |
| `inicializacion` | 57–67 | Lo limpia, con el error de límite de `archaeology-04`. |
| `impresion0` | 114–174 | Lo lee para decidir qué imprimir. |
| `impresion` | 870–930 | Lo lee para dibujar la rejilla. |
| `movcab` | 234–724 | Lo lee y lo escribe: 48 menciones, 16 de ellas escrituras. |
| `ordenjuego` | 726–751 | Escribe dos celdas e incrementa `jugada`. |

Cinco de las once funciones indexan el arreglo directamente, y solo `movcab` concentra 48 de las
menciones. `main` nunca lo toca. Ese es el alcance que una cabecera tiene que interrumpir.

### Leer el módulo incluido

La lección trae la separación ya hecha. Lee los tres archivos en este orden, y lee `main.cpp`
primero:

1. [`code/main.cpp`](code/main.cpp) — 13 líneas. Esta es toda la vista del tablero que tiene el
   llamador.
2. [`code/board.h`](code/board.h) — 23 líneas. Este es el contrato.
3. [`code/board.cpp`](code/board.cpp) — 55 líneas. Esta es la representación.

Después ejecútalo:

```bash
./run.sh
```

Salida esperada:

```text
visited=2 first=1 second=1
```

Vuelve a leer `main.cpp` con la pregunta: *¿qué tendría que saber este archivo si `Board` siguiera
siendo `int vectab[64]`?* La respuesta es la fórmula del índice, los límites de fila y columna y
el significado del cero. Nada de eso aparece en el archivo. Esa ausencia es el producto de esta
lección.

### La superficie pública

| Declaración | Línea de la cabecera | Qué promete la firma |
|---|---|---|
| `Board()` | 8 | Existe un tablero. |
| `void reset()` | 9 | Toda celda pasa a no visitada. |
| `void mark(int row, int column)` | 10 | La celda pasa a visitada. |
| `bool is_visited(int row, int column) const` | 11 | Responde si la celda está visitada. |
| `std::size_t visited_count() const` | 12 | Cuenta las celdas visitadas. |

Cinco nombres. Compáralo con el archivo: once funciones, once prototipos, cuatro líneas de
declaración y ninguna afirmación, en ninguna parte, sobre de qué puede fiarse un llamador.

> **La brecha que la cabecera no cierra.** `mark(-5, 900)` compila. `is_visited(0, 0)` también. La
> cabecera no dice nada sobre coordenadas, y el cuerpo ignora en silencio la entrada fuera de rango.
> El comportamiento es real, no está documentado y, por tanto, no forma parte de ningún contrato que
> un revisor pueda comprobar. Regístralo; `archaeology-13` es donde se escribe.

## Paso 3 — Separar (25 min)

Esta es la prueba de cambio. Vas a reconstruir el módulo para que el `main.cpp` incluido compile y
se ejecute **sin ser editado**.

### Preparar la copia temporal

```bash
mkdir -p /tmp/board-split
cp archaeology/lessons/archaeology-08-module-boundary/code/main.cpp /tmp/board-split/
cd /tmp/board-split
```

`main.cpp` es ahora tu único punto fijo. No lo toques. Todo lo demás lo escribes tú.

### Escribir la cabecera

Tres reglas deciden qué entra:

1. **Declara solo lo que el llamador usa.** Cuatro operaciones y un constructor. Nada más.
2. **No declares ayudantes.** `cell_index` e `is_inside` son detalles de implementación.
   Pertenecen a `board.cpp` como funciones libres en un espacio de nombres anónimo.
3. **Declara el miembro de datos, pero nada sobre él.** C++ exige el miembro en la definición de la
   clase. Es un límite del lenguaje, no una decisión de diseño; dilo en tu registro en lugar de
   fingir que la cabecera lo esconde.

La implementación de referencia de la regla 2 reduce la cabecera de 23 líneas a 17 y hace crecer
`board.cpp` de 55 a 60. Nada más cambia y la salida es idéntica:

```text
visited=2 first=1 second=1
```

Ese es el intercambio en una sola medida: seis líneas salen de la cabecera y la implementación crece
en cinco. La cabecera es ahora la vista del llamador, no la anatomía del módulo.

### Escribir la implementación

Mueve las definiciones, conserva el comportamiento. El cuerpo que escribas debe cumplir las cuatro
promesas de arriba, incluidas las dos que la cabecera no enuncia:

- las filas y las columnas son **base 1**, `1..8`;
- la entrada fuera de rango la **ignora** `mark` y es **falsa** para `is_visited`.

Si no puedes reproducir ese comportamiento solo con `main.cpp`, esa es la lección: la cabecera no es
el contrato, es la mitad de uno.

### Compilar tu versión

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 board.cpp main.cpp -o board-split
./board-split
```

| Comprobación | Esperado |
|---|---|
| Compilación | Sin diagnósticos; `-Werror` convierte cualquier aviso en fallo. |
| Salida | `visited=2 first=1 second=1` |
| Ediciones en `main.cpp` | Cero. |
| Tamaño de la cabecera | Entre 12 y 20 líneas es el rango honesto; la referencia son 17. |

Si tu cabecera pasa de 20 líneas, pregunta qué declaración necesita realmente un llamador. Si baja
de 12, probablemente eliminaste una promesa de la que el llamador depende.

## Paso 4 — Compilar (15 min)

Ahora rompe la frontera a propósito. No es vandalismo; es la única forma de ver qué compra la
frontera.

### La mitad del compilador

Añade una línea a una copia temporal de `main.cpp`:

```cpp
std::printf("%d\n", board.visited[0] ? 1 : 0);
```

Compila. La captura de referencia es de GCC; los números de línea dependen de dónde pongas la línea:

```text
main.cpp: In function ‘int main()’:
main.cpp:8:31: error: ‘bool Board::visited [64]’ is private within this context
    8 |     std::printf("%d\n", board.visited[0] ? 1 : 0);
      |                               ^~~~~~~
In file included from main.cpp:1:
board.h:22:10: note: declared private here
   22 |     bool visited[cellCount];
      |          ^~~~~~~
```

En ese mensaje hay tres cosas, y las tres importan:

- el compilador sabe que el miembro existe;
- el compilador sabe que es privado;
- el compilador te dice exactamente dónde está la declaración.

Compáralo con el archivo, donde la misma línea compilaría en silencio y corrompería el tablero.

### La mitad del enlazador

Borra la definición de `reset()` de tu `board.cpp` pero deja la declaración. Compila un llamador que
la use:

```text
/usr/bin/ld: main.cpp:(.text.startup+0x10): undefined reference to `Board::reset()'
collect2: error: ld returned 1 exit status
```

El compilador aceptó la llamada, porque la cabecera la prometía. El enlazador la rechazó, porque la
promesa no tenía cuerpo. Dos herramientas, dos mitades, una frontera.

### La prueba que habría detectado la clase de fallo

Una compilación que compila el módulo y el llamador como unidades de traducción separadas, con
`-Werror`, y que busca en el llamador el nombre de la representación:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -c board.cpp -o board.o
c++ -std=c++20 -Wall -Wextra -Werror -c main.cpp -o main.o
c++ board.o main.o -o board-split
grep -n "visited\[" main.cpp && exit 1
```

Los tres primeros comandos demuestran que la frontera compila desde ambos lados. El cuarto demuestra
que el llamador nunca nombra el arreglo. En el archivo, el `grep` equivalente sobre `vectab` habría
coincidido en cinco funciones.

## Paso 5 — Explicar (10 min)

### Qué saben y qué no saben los llamadores

Rellena esto con tu propio módulo, no con el incluido.

| Pregunta | ¿Lo sabe el llamador? | Cómo lo aprende |
|---|---|---|
| Qué operaciones existen | Sí | La cabecera. |
| Tipos de parámetros y de retorno | Sí | La cabecera. |
| Qué operaciones mutan | Sí | El `const` en los métodos de consulta. |
| Que las filas y columnas son base 1 | No | Solo leyendo `board.cpp` o las pruebas. |
| Que la entrada fuera de rango se ignora | No | Solo leyendo `board.cpp`. |
| Que el tablero es de 8×8 | No | Solo leyendo `board.cpp` o probando. |
| Que el tablero es un arreglo de 64 `bool` | Es visible, pero inutilizable | La sección privada de la cabecera. |
| La fórmula del índice | No | `board.cpp`. |
| Que `reset` es redundante tras construir | No | `board.cpp`: el constructor ya pone ceros. |

Dos filas merecen una segunda mirada. "Visible pero inutilizable" es el compromiso de C++, y es la
razón de que exista la lección siguiente. "`reset` es redundante" es una ambigüedad real del
contrato en el módulo incluido: el constructor ya produce un tablero vacío, así que un llamador no
puede saber si `reset()` es obligatorio antes de usar. La cabecera ofrece dos caminos al mismo
estado y no dice cuál es el contrato. Regístralo como defecto del módulo, no del llamador.

### La prueba de cambio

Una lección de diseño se juzga por un cambio, no por una forma. Dos listas, ambas tomadas del módulo
que construiste.

**Cambios que quedan dentro del módulo:**

| Cambio | Fuente del llamador | Recompilación | Salida |
|---|---|---|---|
| `bool visited[64]` → una máscara de 64 bits | Sin cambios | Sí | Idéntica. |
| `bool[64]` → `std::array<bool, 64>` | Sin cambios | Sí | Idéntica. |
| Fórmula `(row-1)*8+(col-1)` → `(row-1)*8+col-1` | Sin cambios | Sí | Idéntica. |
| Bucle de `visited_count` → popcount | Sin cambios | Sí | Idéntica. |
| Ayudantes fuera de la clase, dentro del `.cpp` | Sin cambios | Sí | Idéntica. |
| Añadir una caché privada del conteo | Sin cambios | Sí | Idéntica. |

**Cambios que alcanzan al llamador:**

| Cambio | Por qué alcanza al llamador |
|---|---|
| Renombrar `mark` a `set_visited` | El nombre está en la cabecera; cada llamada se edita. |
| Hacer que `is_visited` no sea `const` | La declaración cambia; rompe a quien tiene un `const Board&`. |
| Pasar a coordenadas base 0 | Compila, se ejecuta y significa otra cosa en silencio. |
| Hacer que `mark` lance fuera de rango | Compila, se ejecuta y cambia el comportamiento en el borde. |
| Añadir un miembro de datos público | Cambia el tamaño del objeto; todo llamador recompila. |

Fíjate en la diferencia entre las dos tablas. La primera columna es "recompilar, no reescribir":
el llamador no necesita edición, pero la compilación igualmente lo toca, porque la representación
está en la cabecera. Ese es exactamente el coste que elimina la lección siguiente.

### Dos arreglos, dos tamaños

**Arreglo mínimo.** La separación que acabas de escribir. Es mínimo en el sentido que importa aquí:
`main.cpp` queda intacto, el comportamiento es idéntico y el compilador ahora rechaza la
representación. Eso basta para el criterio de aceptación.

**Arreglo estructural.** Sacar la representación de la cabecera por completo: un tipo opaco, un
`std::unique_ptr<Impl>` con declaración adelantada, o una API de funciones libres sobre un tipo
incompleto. Entonces cambiar `bool[64]` por una máscara deja de recompilar a los llamadores. Enlaces
con la pista A: `construction-08` (programación modular) para la frontera, `construction-09` (tipo
abstracto de datos) para el intercambio, `construction-11` (encapsulación) para el invariante. Esta
lección se detiene a propósito en el primer peldaño; `archaeology-09` toma el segundo.

### Qué no arregla la frontera

- No hace correcto el tablero. El error de límite pertenece a `archaeology-04`.
- No le da dueño al tablero. El mapa de propiedad pertenece a `archaeology-05`.
- No enuncia el contrato. Las coordenadas base 1 y la política silenciosa de fuera de rango siguen
  sin documentar; eso es `archaeology-13`.
- No impide que un llamador llame a `mark(-5, 900)`. El control de acceso no es validación.
- No esconde la representación del compilador. Solo el peldaño siguiente hace eso.

## Paso 6 — Registrar (5 min)

Rellena [`EVIDENCE-es.md`](EVIDENCE-es.md):

- configuración y fijación del archivo;
- la tabla de predicción, escrita antes del paso 2;
- el conteo de alcance y la tabla de regiones;
- el módulo que escribiste, con conteos de líneas antes y después;
- el error exacto del compilador y el error exacto del enlazador;
- la tabla de conocimiento del llamador;
- las dos listas de cambios;
- la frontera de evidencia: verificado, inferido, desconocido;
- la comprobación de integridad del archivo.

Una frase como "el módulo esconde el tablero" es una opinión. Un error del compilador pegado con su
número de línea es evidencia.

## El original en detalle

Hechos que citarás repetidamente, con números de línea del archivo fijado
(`original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`):

| Ubicación | Qué hay |
|---|---|
| 16 | `const char simcab1=21,simcab2=156,sep=219;` — símbolos de impresión, no estado. |
| 17 | `int vectab[64],vecjugadas[8];` — el tablero, en alcance de archivo, a la vista. |
| 18 | `int cab1,cab2,jugada=1,orden,ganador=1;` — posiciones, jugada, orden, ganador. |
| 19–30 | Once prototipos escritos a mano, en el mismo archivo que las definiciones. |
| 29 | `int cf1,cc1,cf2,cc2,cab;` — la caché derivada y el selector de turno. |
| 32–55 | `void main()` — llama a cuatro funciones y repite sobre un global. |
| 57–67 | `inicializacion` — `for (i=0;i<=64;i++) vectab[i]=0;`. |
| 63 | La escritura con error de límite. Una posición de más, en cada ejecución. |
| 114–174 | `impresion0` — lee `vectab` para decidir qué caballo dibujar. |
| 122–162 | Trece sitios de lectura dentro de `impresion0`. |
| 234 | `int movcab(int &cf,int &cc)` — dos referencias, seis globales. |
| 269, 293, 329, 352, 385, 408, 441, 464, 497, 520, 554, 577, 610, 633, 666, 689 | `vectab[ccab]=jugada;` — dieciséis escrituras, una por caso y por caballo. |
| 726–751 | `ordenjuego` — escribe dos celdas e imprime. |
| 753–868 | `impresionorden` — orquesta, imprime, llama a `getch`. |
| 870–930 | `impresion` — lee el tablero y dibuja la rejilla. |

El archivo contiene **cero** archivos `.h` o `.hpp` en las cuatro variantes. Eso es un hecho sobre
el artefacto, no una inferencia.

## Qué prueba y qué no prueba la frontera

| Afirmación | ¿Respaldada? | Por qué |
|---|---|---|
| Un llamador no puede nombrar el arreglo visitado. | Sí | El compilador lo rechaza; capturaste el mensaje. |
| Toda función declarada tiene cuerpo. | Sí, para las que el llamador usa | El enlazador solo comprueba lo referenciado. |
| La salida no cambia con la separación. | Sí | Dos ejecuciones, misma línea, mismos números. |
| El llamador ya no necesita la fórmula del índice. | Sí | `main.cpp` no la contiene. |
| La representación está escondida del compilador. | No | Está escrita en la sección privada de la cabecera. |
| Los llamadores sobreviven a un cambio de representación sin recompilar. | No | La disposición del objeto cambia; recompilan. |
| El módulo documenta su sistema de coordenadas. | No | Ni la cabecera ni ningún archivo dice "base 1". |
| El módulo rechaza entrada inválida. | No | La ignora, en silencio. |
| El programa de 2008 compilaría hoy sin cambios. | No | `iostream.h`, `conio.h`, `void main`; ver `archaeology-16`. |

Esta tabla es la comprobación de honestidad. La afirmación más fuerte de esta lección es un mensaje
del compilador; la más débil es "el diseño es mejor". Mantenlas separadas.

## Autocomprobación (binaria)

- [ ] Escribí cinco predicciones falsables antes de leer `board.h`.
- [ ] Conté las menciones de `vectab` y las cabeceras del archivo.
- [ ] Leí `main.cpp` antes que `board.h`.
- [ ] Ejecuté el módulo incluido y obtuve `visited=2 first=1 second=1`.
- [ ] Reconstruí `board.h` y `board.cpp` en temporal sin editar `main.cpp`.
- [ ] Mi cabecera no declara ayudantes de implementación.
- [ ] Mi cabecera mide entre 12 y 20 líneas y puedo justificar cada declaración.
- [ ] Capturé el error exacto del compilador para `board.visited[0]`.
- [ ] Capturé el error exacto del enlazador para una función declarada y no definida.
- [ ] Rellené la tabla de conocimiento del llamador, incluidas las filas que la cabecera no
  responde.
- [ ] Listé tres cambios que quedan dentro del módulo y tres que alcanzan al llamador.
- [ ] Expliqué por qué un cambio de representación obliga a recompilar pero no a editar.
- [ ] Enuncié la diferencia entre frontera de módulo y tipo abstracto de datos.
- [ ] Registré que `reset()` es redundante tras construir.
- [ ] El archivo está sin modificar (`git status` en el archivo está limpio).

Si una casilla no está marcada, la casilla sin marcar nombra el paso siguiente. Repetir uno no tiene
penalización.

## Misconcepciones que esta lección refuta

| Idea equivocada | Por qué es falsa | Qué hacer en su lugar |
|---|---|---|
| "Una cabecera esconde la implementación." | El miembro privado está declarado en la cabecera; el compilador lo ve. | Di "control de acceso", no "ocultamiento". |
| "`private` es seguridad." | Cualquiera puede leer la cabecera y todo llamador puede recompilarse. | Trátalo como una regla de acceso en compilación. |
| "Separar un archivo es refactorizar." | Una separación sin contrato son dos archivos con el mismo acoplamiento. | Declara operaciones; deja la representación en el `.cpp`. |
| "El compilador comprueba que la función es correcta." | Solo comprueba nombres y tipos. | Añade un contrato de comportamiento y una prueba. |
| "Si compila, la frontera funciona." | Una función declarada y no definida compila y falla al enlazar. | Compila y enlaza por separado, luego ejecuta. |
| "El módulo puede rechazar coordenadas inválidas." | Las ignora; nada valida. | Registra la política; la validación es otra lección. |
| "Hay que llamar a `reset()` antes de usar." | El constructor ya produce un tablero vacío. | Registra la ambigüedad y elige un contrato. |
| "Una cabecera hace portable el código." | Los problemas del archivo son `conio.h` e `iostream.h`, no la disposición de archivos. | Mantén `archaeology-16` aparte. |
| "Más cabeceras significan más modularidad." | Dividir en diez cabeceras con globales compartidos no esconde nada. | Una frontera por cada estado con dueño. |
| "La frontera elimina la necesidad de pruebas." | Elimina una clase de mal uso, no el comportamiento equivocado. | Conserva la traza de referencia. |
| "Esto es la misma lección que el tipo abstracto de datos." | Esta esconde nombres; aquella esconde la representación. | Continúa a `archaeology-09` y compara. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Unidad de traducción | Un archivo fuente tras el preprocesamiento, compilado por separado. |
| Frontera de módulo | La línea entre lo que los llamadores pueden usar y lo que el módulo guarda. |
| Interfaz | Las declaraciones de las que un llamador puede depender. |
| Implementación | Las definiciones, ayudantes y representación detrás de la interfaz. |
| Control de acceso | La regla del compilador sobre quién puede nombrar un miembro. |
| Comprobación en enlace | La regla del enlazador de que toda declaración usada tiene definición. |
| Ocultamiento de información | Mantener la representación fuera de la vista y de la compilación del llamador. |
| Representación | Cómo se almacena el estado, frente a lo que significa. |
| Prueba de cambio | La edición que mides para juzgar si una frontera ayuda. |
| Recompilar, no reescribir | Un cambio que obliga a reconstruir pero no a editar al llamador. |
| Tipo opaco | Un tipo cuya definición el llamador nunca ve. |
| Contrato de comportamiento | Lo que significan las operaciones, más allá de sus nombres y tipos. |

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
