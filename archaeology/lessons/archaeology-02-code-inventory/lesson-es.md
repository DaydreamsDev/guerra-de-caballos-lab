# archaeology-02 — Inventario de código y grafo de llamadas

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** inventario estructural · **Prerrequisito:** archaeology-01 ·
> **Tiempo estimado:** 60 minutos
> **Lectura:** ~16 minutos
> **Rol de la lección:** tronco

## Por qué existe esta lección

`archaeology-01` respondió una pregunta: ¿se ejecuta? La respuesta fue sí, mediante una sonda, con
las llamadas a consola fallando. Esa lección terminó con un binario que funciona y una pila de
código sin leer. Esta lección lee la pila como un sistema.

La fuente bajo estudio es un solo archivo de 930 líneas. Leerlo de arriba abajo da una historia:
primero pasa esto, después aquello. Una historia no es un mapa. La historia omite las partes que te
aburrieron, y esas suelen ser las que sostienen la estructura. Un inventario es el mapa: cada unidad
contada, cada llamada dibujada, nada descartado por parecer poco importante.

La tentación es saltar al defecto que ya notaste. La línea 61 escribe una posición más allá del
final de `vectab`, y se ve desde lejos. No lo persigas. Un defecto encontrado antes del mapa es una
conjetura con número de línea. El protocolo fija el orden porque el mapa dice si un defecto es local
o estructural, y esa diferencia decide cuánto cuesta corregirlo.

Este es el paso 3 del protocolo de autopsia, acotado. El paso 3 del protocolo también pide comparar
versiones. Esta lección descarta toda comparación y conserva solo la estructura: **qué existe, qué
llama a qué y qué estado se comparte.** Comparar dos versiones es otra pregunta con otra evidencia,
y no es la de hoy.

## Qué podrás hacer

Al terminar esta lección puedes:

- contar archivos, funciones, globales y puntos de entrada en un archivo de código desconocido;
- distinguir un prototipo de una definición, y una declaración de una mutación;
- completar una tabla de inventario que otra persona podría verificar línea por línea;
- dibujar un grafo de llamadas y responder cinco preguntas: raíces, hojas, ciclos, sin llamador y
  sin llamadas salientes;
- separar sitios de llamada de aristas, y explicar por qué los dos números difieren;
- nombrar la función más grande, la más conectada y el global más mutado, con evidencia;
- etiquetar cada hallazgo como `verificado`, `inferido` o `desconocido`;
- enunciar qué predice la forma del grafo sobre el costo del próximo cambio;
- registrar el inventario completo en `EVIDENCE-es.md`.

## El concepto: inventario estructural

Un **inventario estructural** es una lista completa y contable de las unidades de un archivo de
código, más las aristas dirigidas entre ellas. Tiene cuatro clases de unidad y una clase de arista.

| Idea | Significado en esta lección |
|---|---|
| **Unidad** | Un archivo, una función, un global o un punto de entrada. Algo que se nombra y se cuenta. |
| **Función** | Una definición con cuerpo. Un prototipo es una promesa, no una unidad de trabajo. |
| **Global** | Un nombre declarado fuera de toda función, cuyo valor sobrevive a la llamada. |
| **Arista de llamada** | "La función A contiene una llamada a la función B". Dirección: llamador → llamado. |
| **Sitio de llamada** | Una ocurrencia de esa llamada. A→B puede ser una arista y veinticuatro sitios. |
| **Raíz** | Un nodo sin llamada entrante desde dentro del archivo. El programa empieza aquí. |
| **Hoja** | Un nodo sin llamada saliente a otra función del archivo. Termina un camino. |
| **Ciclo** | Un camino que vuelve a su propio inicio. Recursión, directa o indirecta. |
| **Código muerto** | Código que ningún camino desde una raíz puede alcanzar. No es lo mismo que una hoja. |
| **Confianza** | Cómo lo sabes: lo leíste (`verificado`), lo dedujiste (`inferido`) o lo supones (`desconocido`). |

### Por qué contar en lugar de leer

Contar es falsable. "La función más grande es `movcab`, 491 líneas" se comprueba en diez segundos y
se refuta en cinco. "El código es un desastre" no se puede comprobar. Los números te obligan a mirar
el archivo completo en lugar de la parte que te llamó la atención, y te hacen honesto sobre lo que
saltaste: un inventario con una fila vacía es una pregunta, un ensayo con una sección ausente es una
mentira por omisión.

### Por qué el grafo de llamadas no es el programa

El grafo dice quién *puede* llamar a quién. No dice con qué frecuencia, bajo qué condición ni con
qué estado. `impresion` es una hoja, y quien empieza lee "hoja" como "poco importante". La llaman
tres funciones distintas, lo que la convierte en la unidad con más llamadores distintos y en el
único lugar donde tendría que aterrizar un cambio al formato del tablero. Hoja significa *terminal*,
no *trivial*.

La advertencia vale en la otra dirección. Un grafo sin ciclos parece limpio, pero trece aristas
sobre doce nodos es casi un árbol, y un programa con forma de árbol y quince globales no es modular.
Es el trabajo de una sola función repartido en doce cuerpos que meten la mano en el mismo cajón. El
grafo muestra las llamadas; los globales muestran el acoplamiento.

### Las tres etiquetas de confianza

Cada afirmación de tu inventario lleva exactamente una etiqueta. No hay una cuarta, ni un "casi
verificado".

| Etiqueta | Prueba | Ejemplo |
|---|---|---|
| **Verificado** | Leíste la línea y la línea lo dice. | `movcab` ocupa 234–724, así que tiene 491 líneas. |
| **Inferido** | La fuente lo sostiene, pero combinaste dos hechos. | `ganador` nunca se reinicia, así que la partida termina en la primera captura. |
| **Desconocido** | La fuente no lo decide. | Si `orden` siempre se asigna antes de leerse en todos los caminos. |

La etiqueta intermedia es donde vive la mayoría de los hallazgos reales. No promuevas una inferencia
a hecho porque parezca obvia; escribe los dos hechos que combinaste y deja que quien lea compruebe
la unión.

## Antes de empezar

Lee estos archivos completos:

- [`README-es.md`](README-es.md) — la ficha de la lección y el criterio de aceptación.
- [`CODE-INVENTORY-TEMPLATE-es.md`](CODE-INVENTORY-TEMPLATE-es.md) — la tabla que vas a completar.
- [`docs/guide/archaeology-track-es.md`](../../../docs/guide/archaeology-track-es.md) — §2, el
  protocolo de autopsia, paso 3; §3, la taxonomía de fallos que *todavía no* aplicas.
- [`docs/guide/lesson-catalog-es.md`](../../../docs/guide/lesson-catalog-es.md) — el prerrequisito
  `archaeology-01` y el criterio de aceptación de esta lección.

También necesitas:

- una copia de la fuente fuera del original de solo lectura, por ejemplo en `/tmp/legacy-probe`;
- `wc`, `grep` y `awk` — todos estándar;
- un editor de texto y algo para dibujar, papel incluido.

**No** necesitas volver a ejecutar el programa. `archaeology-01` ya lo hizo. El inventario es una
actividad de lectura, y mezclarla con ejecución te invita a explicar comportamiento en lugar de
estructura.

> **Regla:** el original es de solo lectura. Cada comando de esta lección lee una copia. Si te
> descubres editando la fuente para que el conteo sea más fácil, detente y corrige el comando.

## El camino de 60 minutos

| Min | Paso | Qué haces | Evidencia |
|---|---|---|---|
| 0–10 | Inspeccionar | Cuentas líneas, includes, definiciones, declaraciones | Salida de cuatro comandos |
| 10–25 | Inventariar funciones | Completas la tabla de funciones, 12 filas | Tabla de funciones |
| 25–35 | Inventariar globales | Completas la tabla de globales, cuentas escrituras | Tabla de globales |
| 35–50 | Graficar | Dibujas el grafo, respondes las cinco preguntas | Grafo + raíces/hojas |
| 50–55 | Explicar | Nombras la forma y lo que predice | Párrafo de forma |
| 55–60 | Registrar | Completas `EVIDENCE-es.md` con etiquetas de confianza | Entrada de portafolio |

Si la tabla de funciones tarda veinte minutos porque sigues leyendo cuerpos, eso es información
sobre el archivo, no sobre ti. `movcab` tiene 491 líneas; leerlo no es un desvío, es el trabajo.

### La comprobación de ingeniería en esta lección

| Paso | Dónde ocurre | Qué produce |
|---|---|---|
| **Inspeccionar** | Paso 1 | La superficie: tamaño, includes, conteo de definiciones y de declaraciones. |
| **Inventariar** | Pasos 2–3 | Dos tablas: funciones y globales, con rangos de líneas y escritores. |
| **Graficar** | Paso 4 | El grafo de llamadas, sus raíces, sus hojas, su tabla de grados. |
| **Explicar** | Paso 5 | Un párrafo que nombra la forma y lo que predice. |
| **Registrar** | Paso 6 | `EVIDENCE-es.md`, cada hallazgo con evidencia y confianza. |

## Paso 1 — Inspeccionar la superficie (10 min)

Trabaja sobre una copia. Después ejecuta cuatro comandos y nada más. No abras el archivo en un
editor todavía; el editor te muestra el medio y esconde los bordes.

```bash
cp /ruta/al/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp .
wc -l guerra-de-caballos-baseline.cpp
grep -n '^#include' guerra-de-caballos-baseline.cpp
grep -nE '^(void|int)[[:space:]]+[A-Za-z0-9_]+[[:space:]]*\(' guerra-de-caballos-baseline.cpp
grep -nE '^(const[[:space:]]+)?(char|int|float|double)[[:space:]]' guerra-de-caballos-baseline.cpp
```

En la copia de referencia, las cuatro respuestas son:

```text
930
10:#include<stdio.h>   11:#include<conio.h>   12:#include<iostream.h>
13:#include<stdlib.h>  14:#include<time.h>
23 líneas coinciden: 11 prototipos (19–28, 30) y 12 definiciones
16, 17, 18, 29
```

Esa es toda la superficie: un archivo, cinco includes, veintitrés líneas que parecen funciones y
cuatro líneas de globales. Tres de esos números son trampas.

### Trampa 1 — los prototipos no son funciones

Las líneas 19–28 y 30 son once prototipos, cada uno terminado en `;`. Las líneas 32–930 contienen
doce definiciones. Un `grep` ingenuo no distingue unos de otras, y quien confíe en su conteo
inventariará once funciones sin cuerpo y se perderá que `main` no tiene prototipo. La regla: **una
definición tiene `{` después de la lista de parámetros; un prototipo tiene `;`.** Ante la duda,
busca la llave de apertura.

Doce definiciones, once prototipos, un `main` sin prototipo y sin firma estándar. No es un defecto
para corregir hoy; es un hecho para registrar.

### Trampa 2 — el patrón de identificadores esconde una función

La primera versión del patrón usaba `[A-Za-z_]+`, que excluye dígitos. Devuelve 21 coincidencias y
pierde en silencio las dos apariciones de `impresion0`: la definición de la línea 114 y el prototipo
de la línea 30. Un conteo equivocado por dos es peor que ningún conteo, porque parece completo. Todo
patrón que busque identificadores debe incluir dígitos: `[A-Za-z0-9_]+`.

### Trampa 3 — las líneas de declaración no son variables

Las líneas 16, 17, 18 y 29 declaran quince nombres. Cuatro líneas. Si cuentas líneas reportarás
cuatro globales y te equivocarás en once. Cuenta nombres, no sentencias:

| Línea | Nombres | Clase |
|---|---|---|
| 16 | `simcab1`, `simcab2`, `sep` | `const char`, se fijan una vez, nunca se reescriben |
| 17 | `vectab[64]`, `vecjugadas[8]` | arreglos `int`, el tablero y el orden de jugadas |
| 18 | `cab1`, `cab2`, `jugada`, `orden`, `ganador` | estado `int` suelto, un nombre por hecho |
| 29 | `cf1`, `cc1`, `cf2`, `cc2`, `cab` | estado `int` suelto, declarado lejos de los demás |

Quince nombres en cuatro líneas, con la línea 29 entre dos prototipos. Esa ubicación explica por qué
el segundo grupo es fácil de perder: el bloque de declaraciones termina en la línea 18 y la línea 29
se esconde en la lista de prototipos. Es un hecho estructural, no una queja de estilo. Quien busca
globales encontrará trece y perderá dos.

## Paso 2 — Inventariar las funciones (15 min)

Una fila por definición, doce filas. Usa las columnas de la plantilla y estas reglas:

- **Líneas**: desde la línea de definición hasta la línea con la llave de cierre, incluyendo
  blancos. Escribe la regla en tu registro para que los números se puedan reproducir.
- **Entradas**: parámetros, con `&` marcado. "Ninguna" es una respuesta válida y significativa.
- **Salida**: el tipo de retorno y qué significa el valor, no solo `int`.
- **Efectos laterales**: globales escritos, salida por consola, entrada leída. Una función sin
  ninguno es un ayudante puro; dilo.
- **Llama a**: solo funciones definidas por el programa. Las llamadas de biblioteca van en efectos
  laterales.

| Función | Líneas | Entradas | Salida | Efectos laterales | Llama a |
|---|---:|---|---|---|---|
| `main` | 32–55 (24) | ninguna | `void` | configuración de consola, bucle de juego | `inicializacion`, `cabubicacion`, `vecjug`, `impresionorden` |
| `inicializacion` | 57–67 (11) | ninguna | `void` | escribe `vectab[0..64]` | ninguna |
| `cabubicacion` | 69–102 (34) | ninguna | `void` | escribe `cab1`, `cab2`, `orden`; siembra `rand`; imprime | `ordenjuego`, `conversionvm` |
| `conversionvm` | 104–112 (9) | ninguna | `void` | escribe `cf1`, `cc1`, `cf2`, `cc2` | ninguna |
| `impresion0` | 114–174 (61) | ninguna | `void` | lee `vectab`, `cab1`, `cab2`; imprime | ninguna |
| `vecjug` | 176–205 (30) | ninguna | `void` | escribe `vecjugadas[0..7]`; llama a `rand` | ninguna |
| `imprevecjug` | 207–224 (18) | ninguna | `void` | lee `vecjugadas`; imprime | ninguna |
| `conversionmv` | 226–232 (7) | `cf`, `cc` (por valor) | `int`, índice del vector | ninguna — pura | ninguna |
| `movcab` | 234–724 (491) | `cf`, `cc` (por referencia) | `int`, índice del vector | escribe `vectab`, `jugada`, `ganador`; imprime | `conversionmv` ×24, `impresion` ×2 |
| `ordenjuego` | 726–751 (26) | `tirocab1`, `tirocab2` (por ref.) | `int`, primer jugador | escribe `vectab`, `jugada`; imprime | `impresion` ×2 |
| `impresionorden` | 753–868 (116) | ninguna | `void` | escribe `cab1`, `cab2`, `cab`, `jugada`; imprime | `imprevecjug` ×4, `movcab` ×4, `impresion0` ×4, `impresion` ×4 |
| `impresion` | 870–930 (61) | ninguna | `void` | lee `vectab`, `cab1`, `cab2`; imprime | ninguna |

Suma los rangos y obtienes 888 líneas; las 42 restantes son el encabezado, los includes, los
globales y las líneas en blanco entre funciones. Si tu total no llega a 930, te faltó un hueco. Esa
es la comprobación de que la tabla está completa.

### Lo que la tabla ya te dice

Cuatro números, antes de cualquier interpretación:

1. **Una función es la mitad del archivo.** `movcab` tiene 491 líneas, 52,8% de 930. La siguiente
   tiene 116. La proporción es 4:1, y contra la función más pequeña es 70:1.
2. **Las cuatro funciones más grandes suman 78%.** `movcab` 491 + `impresionorden` 116 +
   `impresion` 61 + `impresion0` 61 = 729 líneas. Una mueve; las otras tres imprimen o coordinan la
   salida.
3. **Diez de doce no retornan nada.** Solo `conversionmv` y `ordenjuego` devuelven un valor, y ambas
   devuelven un entero pequeño. El trabajo del archivo ocurre por efectos laterales, no por
   resultados.
4. **Dos funciones toman referencias, una las escribe.** `movcab` escribe `cf` y `cc`;
   `ordenjuego` recibe `tirocab1` y `tirocab2` por referencia y nunca asigna ninguna. Es un hecho
   verificado con una pregunta obvia adherida, y esa pregunta va en hallazgos.

### La trampa del rango de líneas

Si cuentas "sentencias" en lugar de líneas, `movcab` parece de 40 sentencias porque ocho cuerpos
`case` son copias entre sí. Los dos números son ciertos y miden cosas distintas. Registra el rango
de líneas, porque es lo que paga quien revisa al abrir el archivo, y anota la duplicación como una
observación separada con su propia evidencia.

## Paso 3 — Inventariar los globales (10 min)

Una fila por nombre, quince filas. La columna difícil es **Mutado por**, y tiene una definición
precisa:

> Una mutación es una asignación (`=`), un incremento (`++`) o un decremento (`--`). Una comparación
> (`==`, `!=`, `<=`, `>=`) es una lectura. Pasar el nombre a `cout` también es una lectura.

Si te equivocas aquí, `vectab` parece escribirse 59 veces. Se escribe 21 veces. Las otras 38
coincidencias son comparaciones y lecturas.

| Nombre | Tipo | Declarado en | Mutado por | Leído por |
|---|---|---|---|---|
| `simcab1`, `simcab2`, `sep` | `const char` | 16 | nada — `const` | `cabubicacion`, `ordenjuego`, `impresionorden`, `impresion0`, `impresion` |
| `vectab[64]` | `int[64]` | 17 | `inicializacion` (1), `ordenjuego` (4), `movcab` (16) | `movcab`, `impresion0`, `impresion` |
| `vecjugadas[8]` | `int[8]` | 17 | `vecjug` (2) | `imprevecjug`, `movcab` |
| `cab1`, `cab2` | `int` | 18 | `cabubicacion` (2), `impresionorden` (4) | `conversionvm`, `ordenjuego`, `movcab`, `impresion0`, `impresion` |
| `jugada` | `int` | 18 | `ordenjuego` (4), `impresionorden` (8), `movcab` (2) | `main`, `movcab`, `ordenjuego`, `impresionorden` |
| `orden` | `int` | 18 | `cabubicacion` (1) | `impresionorden` |
| `ganador` | `int` | 18 | `movcab` (16) | `impresionorden` |
| `cf1`, `cc1`, `cf2`, `cc2` | `int` | 29 | `conversionvm` (4), `movcab` (vía `&`) | `cabubicacion`, `impresionorden`, `movcab` |
| `cab` | `int` | 29 | `impresionorden` (4) | `movcab` |

Los conteos de escritura, verificados contando solo `=` simple y `++`:

| Nombre | Escrituras | Funciones que escriben | Veredicto |
|---|---:|---:|---|
| `vectab` | 21 | 3 | el más mutado |
| `jugada` | 14 | 3 | el reloj de la partida |
| `ganador` | 16 | 1 | un solo valor, dieciséis lugares |
| `vecjugadas` | 2 | 1 | se reconstruye cada turno |
| `orden` | 1 | 1 | se escribe una vez, se lee una vez |

### Tres hechos que la tabla te obliga a ver

**`vectab` es la pizarra compartida.** Veintiuna escrituras desde tres funciones y lecturas desde
tres. Ninguna función es dueña del tablero; todas meten la mano en el mismo arreglo. Ese es el
acoplamiento que el grafo de llamadas no puede mostrar.

**`ganador` es una bandera pegajosa.** Dieciséis asignaciones, todas `ganador=0`, y ni un solo
reinicio a `1`. La declaración de la línea 18 la inicia en `1` y el programa nunca la restaura. Si
eso importa es una pregunta de comportamiento para `archaeology-03`; que nunca se reinicie es un
hecho estructural que puedes verificar hoy.

**`orden` es un canal de un escritor y un lector.** `cabubicacion` la escribe en la línea 89,
`impresionorden` la lee en las líneas 756 y 812. Nada más la toca. Un escritor y un lector es el
acoplamiento más barato posible, y vale nombrarlo para que los acoplamientos caros resalten.

### El estado que no se ve en la tabla

`cf1`, `cc1`, `cf2`, `cc2` y `cab` se escriben de dos maneras: directamente por `conversionvm` y a
través de parámetros por referencia dentro de `movcab`. El segundo camino no aparece en la tabla
como escritura por nombre, porque el parámetro se llama `cf`, no `cf1`. Si solo buscas `cf1=`, te
perderás cada movimiento del caballo. Registra el alias explícitamente: **`movcab` muta `cf1`/`cc1`
o `cf2`/`cc2` a través de sus parámetros por referencia, según `cab`.** Esa frase es la diferencia
entre un inventario y un volcado de `grep`.

## Paso 4 — Construir el grafo de llamadas (15 min)

Para cada función, lee su cuerpo y anota las funciones definidas por el programa que llama. Ignora
`cout`, `rand`, `getch` y `system`: son llamadas de biblioteca o de plataforma, y meterlas en el
grafo entierra la estructura bajo ruido. Después dibuja las aristas.

```text
main
├── inicializacion                         hoja
├── cabubicacion
│   ├── ordenjuego
│   │   └── impresion                      hoja
│   └── conversionvm                       hoja
├── vecjug                                 hoja
└── impresionorden
    ├── imprevecjug                        hoja
    ├── movcab
    │   ├── conversionmv                   hoja
    │   └── impresion                      hoja
    ├── impresion0                         hoja
    └── impresion                          hoja
```

Ahora las cinco preguntas de la plantilla, respondidas desde el dibujo:

- **Raíces:** solo `main`. Es el único nodo sin llamada entrante desde dentro del archivo.
- **Hojas:** `inicializacion`, `conversionvm`, `vecjug`, `imprevecjug`, `conversionmv`,
  `impresion0`, `impresion`. Siete nodos sin llamada saliente a otra función del archivo.
- **Ciclos:** ninguno. El grafo es acíclico; ninguna función se alcanza a sí misma por ningún
  camino.
- **Funciones sin llamador:** `main`. Todas las demás tienen al menos uno.
- **Funciones sin llamadas salientes:** las mismas siete hojas.

### Las aristas no son los sitios de llamada

`movcab` contiene veinticuatro llamadas a `conversionmv`: tres por `case`, ocho casos. En el grafo
eso es **una arista**. Si cuentas sitios de llamada obtienes cincuenta llamadas a funciones del
programa repartidas en trece aristas distintas:

| Llamador | Llamados distintos | Sitios de llamada |
|---|---:|---:|
| `main` | 4 | 4 |
| `cabubicacion` | 2 | 2 |
| `movcab` | 2 | 26 (24 + 2) |
| `ordenjuego` | 1 | 2 |
| `impresionorden` | 4 | 16 (4 × 4) |
| **Total** | **13 aristas** | **50 sitios** |

Los dos números son evidencia. El conteo de aristas dice la forma; el de sitios dice dónde vive el
texto. Repórtalos por separado y nunca dejes que uno sustituya al otro.

### Grado: el nodo más conectado

El grado es aristas entrantes más salientes, contando vecinos distintos.

| Nodo | Entrantes | Salientes | Grado |
|---|---:|---:|---:|
| `impresionorden` | 1 | 4 | **5** |
| `main` | 0 | 4 | 4 |
| `movcab` | 1 | 2 | 3 |
| `cabubicacion` | 1 | 2 | 3 |
| `impresion` | 3 | 0 | 3 |
| `ordenjuego` | 1 | 1 | 2 |
| las seis hojas restantes | 1 | 0 | 1 |

Hay dos respuestas defendibles para "el más conectado", y debes decir cuál usas:

- **`impresionorden` tiene el grado más alto (5).** Toca cuatro subsistemas y la toca la raíz. Es el
  punto de coordinación del archivo.
- **`impresion` tiene el grado entrante más alto (3).** Tres funciones distintas imprimen a través
  de ella, lo que la convierte en la unidad con más llamadores distintos y en el único cuello de
  botella del formato de salida.

Escribe ambas, etiquetadas. Quien pregunte "¿cuál es la más conectada?" y reciba un número sin
definición te habrá descubierto siendo vago.

### Profundidad y alcanzabilidad

El camino más largo tiene tres aristas: `main → impresionorden → movcab → impresion`. El grafo es
poco profundo y ancho, típico de un archivo escrito como una secuencia de pasos y no como una
jerarquía. Las doce funciones son alcanzables desde `main`, así que hay **cero funciones muertas**.
Es un hallazgo y uno algo sorprendente: un archivo de 930 líneas sin pruebas y sin encabezados sigue
sin huérfanos.

La única pieza inalcanzable es más pequeña que una función. La rama `default:` del `switch` dentro
de `movcab` (líneas 696–719) solo puede ejecutarse si `vecjugadas[i]` está fuera de 1–8, y `vecjug`
es la única escritora de ese arreglo, que llena exactamente 1–8 en las líneas 188 y 199. Así que la
rama es **inalcanzable por construcción**, no por accidente. Regístrala como código muerto con su
evidencia y resiste la tentación de llamarla error; ese juicio necesita la especificación de
comportamiento.

## Paso 5 — Explicar la forma (5 min)

Tienes tres tablas y un grafo. Ahora responde tres preguntas en lenguaje llano y comprime las
respuestas en un párrafo.

1. **¿Dónde vive el trabajo?** `movcab` (491) e `impresionorden` (116) suman 607 líneas, 65% del
   archivo. Todo lo demás es preparación, conversión o impresión.
2. **¿Dónde vive el estado?** En quince globales declarados en cuatro líneas. Ninguna función es
   dueña de sus datos; el tablero, el reloj, el turno y el ganador son alcanzables desde todas
   partes.
3. **¿Qué dice el grafo sobre el cambio?** Un cambio de regla aterriza en una función de 491 líneas.
   Un cambio al formato del tablero aterriza en dos impresoras, `impresion` e `impresion0`, que
   comparten `vectab` y las mismas constantes de símbolo; un cambio al registro de jugadas aterriza
   en `imprevecjug`, que comparte solo `vecjugadas`. Un cambio de estado aterriza en el bloque de
   globales y se propaga a cada lector.

### Un párrafo de forma resuelto

> El archivo es un sistema plano: una sola raíz, trece aristas de llamada, siete hojas, ningún ciclo
> y quince globales que toda función puede alcanzar. El trabajo se concentra en dos cuerpos,
> `movcab` e `impresionorden`, que juntos tienen 65% de las líneas, mientras las otras diez
> funciones son conversores, impresoras y un inicializador. Como el estado vive fuera de toda
> función, el grafo subestima el acoplamiento: `impresion` parece una hoja, pero lee el tablero, los
> caballos y la posición del ganador, así que un cambio a la representación del tablero la alcanza
> desde tres llamadores. La forma predice que cualquier cambio al movimiento es caro, cualquier
> cambio a la salida es barato pero está triplicado, y cualquier cambio al estado es barato de
> escribir y caro de verificar.

Lee ese párrafo otra vez y nota lo que no dice. No dice que el código sea malo. No nombra un
defecto. Describe una forma, y la forma tiene costos. Esa es toda la disciplina.

### Lo que la forma predice

Las predicciones son el puente a las próximas lecciones. Escribe dos, cada una como un conteo que
podrías verificar:

- **Agregar un tercer caballo** tocaría siete de las doce funciones — `cabubicacion`, `ordenjuego`,
  `conversionvm`, `movcab`, `impresionorden`, `impresion`, `impresion0` — más el bloque de globales
  (`cab1`/`cab2`, `cf1`/`cc1`/`cf2`/`cc2`, `cab`). Si tu conteo difiere, encontraste una dependencia
  que el grafo perdió.
- **Cambiar el formato del tablero** tocaría `impresion` e `impresion0`: dos funciones, nada de
  lógica. Ese es el cambio barato, y el grafo explica por qué.

No implementes ninguno. Las predicciones son salida del inventario; actuar sobre ellas es un paso
posterior con una prueba adjunta.

## Paso 6 — Registrar (5 min)

Completa [`EVIDENCE-es.md`](EVIDENCE-es.md). Pide los comandos, las tres tablas, el grafo, las cinco
respuestas, los hallazgos y las etiquetas de confianza.

Dos hábitos que vale conservar:

- **Pega el comando, no la memoria.** `wc -l` devolviendo `930` es evidencia. "Como novecientas
  líneas" es un rumor.
- **Da a cada hallazgo un número de línea y una etiqueta.** Un hallazgo sin `archivo:línea` es una
  opinión; un hallazgo sin etiqueta es una opinión con traje.

## El original de un vistazo

| Métrica | Valor | De dónde sale |
|---|---:|---|
| Archivos | 1 | `find` en el directorio de la fuente |
| Líneas | 930 | `wc -l` |
| Definiciones de función | 12 | `grep` + comprobación de llave |
| Prototipos de función | 11 | líneas 19–28 y 30 |
| Nombres globales | 15 | líneas 16, 17, 18, 29 |
| Líneas de declaración global | 4 | las mismas |
| Aristas de llamada distintas | 13 | grafo de llamadas |
| Sitios de llamada | 50 | conteo por cuerpo |
| Raíces | 1 | `main` |
| Hojas | 7 | grafo de llamadas |
| Ciclos | 0 | recorrido |
| Funciones muertas | 0 | recorrido |
| Función más grande | `movcab`, 491 líneas | 234–724 |
| Más conectada | `impresionorden`, grado 5 | tabla de grados |
| Más llamada | `impresion`, grado entrante 3 | tabla de grados |
| Global más mutado | `vectab`, 21 escrituras | conteo de escrituras |
| Camino más profundo | 3 aristas | `main → impresionorden → movcab → impresion` |

## Qué cuenta como hallazgo

Un hallazgo tiene tres partes, y no es hallazgo hasta tenerlas:

1. **Una afirmación** que podría ser falsa.
2. **Evidencia**: un archivo y una línea, o un comando y su salida.
3. **Una etiqueta de confianza**: `verificado`, `inferido` o `desconocido`.

| Débil | Por qué falla | Fuerte |
|---|---|---|
| "El código es demasiado largo." | No es falsable. | "`movcab` tiene 491 de 930 líneas (52,8%), verificado en 234–724." |
| "Hay demasiados globales." | Sin conteo ni consecuencia. | "Quince nombres globales en cuatro líneas de declaración; `vectab` tiene 21 escrituras desde 3 funciones." |
| "`ganador` tiene un error." | Diagnóstico sin especificación. | "`ganador` se asigna 16 veces, siempre `0`, nunca se reinicia (consecuencia inferida)." |
| "`impresion` es código muerto." | Falso: es una hoja, no es inalcanzable. | "`impresion` es una hoja con grado entrante 3: la llaman `ordenjuego`, `movcab`, `impresionorden`." |

La última fila es la que hay que memorizar. **Hoja no es muerto.** Muerto significa que ningún
camino desde una raíz llega; hoja significa que ningún camino continúa. Una hoja es donde termina un
camino, y en este archivo tres de las siete hojas son impresoras, que es exactamente donde vive el
contrato de salida.

## Lo que esta lección no hace

- No diagnostica un defecto. El error de una posición en la línea 61 se registra como hecho
  estructural —el inicializador escribe el índice 64 de un arreglo de 64 elementos— y se entrega a
  `archaeology-04`. Nombrarlo es inventario; explicar su costo es diagnóstico.
- No compara versiones ni rastrea de dónde viene un archivo. Esa pregunta tiene su propia evidencia
  y su propia lección.
- No mide rendimiento. Los conteos de líneas no son tiempos de ejecución.
- No refactoriza. Una función de 491 líneas es un hallazgo, no una instrucción.
- No escribe pruebas. La prueba que detectaría un problema estructural viene después de la
  especificación de comportamiento.
- No sustituye la especificación de comportamiento. El grafo dice quién llama a quién, no qué
  significa el programa.

## Autocomprobación (binaria)

- [ ] Ejecuté los cuatro comandos de inspección y registré su salida.
- [ ] Separé los once prototipos de las doce definiciones, con la regla de la llave.
- [ ] Conté quince nombres globales, no cuatro líneas de declaración.
- [ ] Mi tabla de funciones tiene doce filas y sus rangos suman 888, más 42 del resto.
- [ ] Identifiqué `movcab` como la función más grande, con su rango de 491 líneas.
- [ ] Conté escrituras solo como `=`, `++`, `--`, y obtuve 21 para `vectab`.
- [ ] Registré que `ganador` se asigna dieciséis veces y nunca se reinicia.
- [ ] Registré que `movcab` muta `cf1`/`cc1` o `cf2`/`cc2` mediante referencias.
- [ ] Dibujé el grafo y nombré una raíz y siete hojas.
- [ ] Reporté trece aristas y cincuenta sitios de llamada como números separados.
- [ ] Di dos respuestas defendibles para "la más conectada" y definí el grado.
- [ ] Encontré cero funciones muertas y una rama `switch` inalcanzable.
- [ ] Cada hallazgo tiene número de línea y etiqueta de confianza.
- [ ] Escribí el párrafo de forma y dos predicciones verificables.
- [ ] Completé `EVIDENCE-es.md` y el original está sin modificar.

Si una casilla no está marcada, la casilla sin marcar nombra el paso siguiente. No hay penalización
por repetir uno.

## Ideas erróneas que esta lección refuta

| Idea errónea | Por qué está mal | Qué hacer en su lugar |
|---|---|---|
| "Una hoja es código muerto." | Hoja significa sin llamadas salientes; muerto significa sin camino desde una raíz. | Recorre desde la raíz y etiqueta las dos cosas por separado. |
| "Más líneas significa más importante." | `movcab` es grande porque ocho casos están copiados, no porque haga más. | Reporta el rango y la duplicación como dos hechos. |
| "La lista de prototipos es la lista de funciones." | Once prototipos, doce definiciones, y `main` no tiene prototipo. | Exige una `{` después de la lista de parámetros. |
| "Un global es global porque dice `int`." | Tres nombres `const char` también son globales, y otros cinco se esconden en la línea 29. | Cuenta nombres en el bloque de declaración, donde sea que esté. |
| "Contar llamadas da el número de aristas." | Cincuenta sitios de llamada se reducen a trece aristas distintas. | Cuenta ambos y di cuál usas. |
| "El grafo de llamadas muestra el orden de ejecución." | Muestra quién puede llamar a quién, no cuándo ni con qué frecuencia. | Lee las condiciones del cuerpo para el orden. |
| "Una función `void` no hace nada." | Diez de doce son `void` y las diez mutan estado o imprimen. | Registra efectos laterales, no valores de retorno, como el trabajo. |
| "Un archivo significa un sistema simple." | Un archivo con quince globales y trece aristas no tiene ninguna frontera. | Dibuja la tabla de estado junto al grafo de llamadas. |
| "`grep` lo encontró, entonces es verdad." | Un `grep` desnudo cuenta comparaciones como escrituras y prototipos como funciones. | Declara el patrón y la regla de exclusión. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Inventario | Lista completa y contable de unidades y sus relaciones. |
| Definición | Función con cuerpo; lo único que se puede llamar. |
| Prototipo | Declaración de una firma sin cuerpo. |
| Arista de llamada | Llamador → llamado, contada una vez por par. |
| Sitio de llamada | Una ocurrencia de una llamada; puede haber muchas por arista. |
| Raíz | Nodo sin llamada entrante desde dentro del archivo. |
| Hoja | Nodo sin llamada saliente a otra función del archivo. |
| Ciclo | Camino que vuelve a su propio inicio. |
| Código muerto | Código inalcanzable desde cualquier raíz. |
| Grado | Aristas entrantes distintas más salientes. |
| Efecto lateral | Escritura de estado o interacción con consola. |
| Verificado / inferido / desconocido | Cómo lo sabes: lo leíste, lo dedujiste o no puedes decidirlo. |

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
