# archaeology-05 — Estado global y acoplamiento oculto

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** estado global y acoplamiento oculto · **Prerrequisito:**
> archaeology-04 · **Tiempo estimado:** 60 minutos
> **Lectura:** ~13 minutos
> **Rol de la lección:** tronco

## Por qué existe esta lección

`archaeology-04` diagnosticó fallos que producen un resultado equivocado: un error de límite, un
desbordamiento de búfer, una condición de victoria invertida. Cada uno tiene un síntoma que se puede
señalar. Esta lección es distinta. El programa que tienes delante funciona. Ubica dos caballos,
alterna turnos, imprime un tablero y se detiene cuando un caballo es capturado o queda encerrado.
Nada falla. Nada imprime un ganador equivocado.

El defecto es que el programa no tiene costura. Cada función alcanza los mismos quince nombres de
alcance de archivo, escribe algunos, lee otros e imprime mientras lo hace. Un cambio en el orden de
turnos toca la función de ubicación. Una prueba para un movimiento necesita el programa completo de
consola. El estado no está mal; es **sin dueño**.

Por eso este es un fallo de **diseño**, no de corrección. La taxonomía de
[`archaeology-track.md`](../../../docs/guide/archaeology-track.md) lo dice sin rodeos: el programa
funciona, pero la estructura encarece el cambio. Esta lección te enseña a ver el coste antes de
sentirlo.

El producto es un **mapa de mutaciones por global** y un mapa por función de lecturas, escrituras y
efectos de impresión. El mapa es el diagnóstico. El arreglo viene después, en dos tamaños.

## Qué podrás hacer

Al terminar esta lección puedes:

- distinguir estado global mutable de constantes de archivo de solo lectura;
- leer una función y listar qué globales lee, qué globales escribe y qué imprime;
- distinguir acoplamiento de lectura, de escritura, de impresión y temporal;
- trazar una ruta de mutación desde `main` hasta la celda que cambia;
- nombrar la ruta de mutación más difícil de probar y explicar por qué;
- separar lo que la fuente prueba de lo que infieres por los sitios de llamada;
- proponer un arreglo mínimo que cree una costura testeable;
- proponer un arreglo estructural que elimine la clase y vincularlo a un peldaño de la pista A;
- mantener el archivo de solo lectura mientras trabajas.

El mapa está terminado cuando cada global tiene dueño y cada función
tiene una fila.

## El concepto: estado global y acoplamiento oculto

Un **global** es un nombre declarado fuera de toda función. En este archivo los globales están
declarados en las líneas 16, 17, 18 y 29. Cualquier función puede leerlos o escribirlos sin decirlo
en su firma.

El **acoplamiento oculto** es la dependencia que el lector no puede ver desde la llamada.
`movcab(cf1, cc1)` parece una función de dos enteros. No lo es. Lee el vector de orden de
movimientos, el número de jugada actual, el caballo actual, el tablero y ambas posiciones de
caballo, y escribe el tablero, la bandera de ganador y el número de jugada. La firma es una
fracción del contrato.

De ahí se siguen tres consecuencias, y son las que vas a mapear:

1. **No puedes probar una función aislada.** Para llamar a `movcab` primero debes construir el
   estado que espera, incluido el estado que escribieron otras tres funciones.
2. **No puedes cambiar una regla en un solo lugar.** El orden de turnos se fija en `cabubicacion`,
   se consume en `impresionorden` y se vuelve a leer dentro de `movcab` a través de `cab`.
3. **No puedes confiar en el grafo de llamadas.** Una función que parece pura puede imprimir, y una
   función que parece una consulta puede mutar.

### Qué cuenta como estado global aquí

El escáner informa quince nombres. Doce son estado mutable. Tres son símbolos `const char` usados
para imprimir: `simcab1`, `simcab2` y `sep`. Son de alcance de archivo, pero no son estado; no
pueden cambiar en tiempo de ejecución. Contarlos como mutables es el primer error de precisión que
esta lección corrige.

| Tipo | Nombres | Mutabilidad |
|---|---|---|
| Tablero y orden de movimientos | `vectab[64]`, `vecjugadas[8]` | Arreglos mutables. |
| Posiciones de caballo y turno | `cab1`, `cab2`, `cf1`, `cc1`, `cf2`, `cc2`, `cab` | Enteros mutables. |
| Progreso del juego | `jugada`, `orden`, `ganador` | Enteros mutables. |
| Símbolos de impresión | `simcab1`, `simcab2`, `sep` | `const char`; solo lectura. |

`cf1`, `cc1`, `cf2` y `cc2` merecen una segunda mirada. Son una caché derivada: `conversionvm` los
escribe a partir de `cab1` y `cab2`, y `movcab` los muta mediante parámetros por referencia. La
misma posición se guarda dos veces, en dos representaciones, y se mantiene sincronizada a mano. Eso
no es un global; es un pequeño objeto distribuido sin dueño.

### Cuatro acoplamientos que nombrar

| Acoplamiento | Qué significa | Ejemplo en este archivo |
|---|---|---|
| **De lectura** | Una función depende de un valor que nadie le pasó. | `movcab` lee `cab` para saber de quién es el turno. |
| **De escritura** | Una función cambia estado que nadie le pidió cambiar. | `ordenjuego` escribe `vectab[cab1]`, `vectab[cab2]` y `jugada`. |
| **De impresión** | Una función produce salida como efecto lateral de calcular. | `movcab` imprime números de caso y llama a `impresion()`. |
| **Temporal** | Una función solo funciona si otra corrió antes, en un orden dado. | `impresionorden` debe fijar `cab` antes de cada llamada a `movcab`. |

El acoplamiento temporal es el más difícil de ver y el más caro. La fuente no dice "llama a
`conversionvm` antes de leer `cf1`". El orden vive en el cuerpo de `cabubicacion`, entre un bucle de
reintento y seis líneas de `cout`.

### Por qué es un fallo de diseño

La severidad tiene que ver con la consecuencia, no con la fealdad. Los globales de este archivo no
causan por sí mismos comportamiento indefinido. El comportamiento indefinido de este archivo es el
bucle `i <= 64`, y eso pertenece a `archaeology-04`. Lo que causan los globales es:

- **Severidad media:** cada cambio de regla es una edición en varias funciones;
- **Severidad media:** un solo movimiento no se puede probar sin una consola y ocho globales
  preconfigurados;
- **Coste alto en revisión:** quien revisa no puede saber desde un diff qué estado toca una función.

Esa es la clase de diseño: cambio caro, estructura frágil. Se arregla con planificación, no en un
parche urgente. La lección te pide ponerle precio, no entrar en pánico.

## Antes de empezar

Lee estos documentos completos:

- [`README-es.md`](README-es.md) — la ficha de la lección y el criterio de aceptación.
- [`GLOBAL-STATE-MAP-TEMPLATE-es.md`](GLOBAL-STATE-MAP-TEMPLATE-es.md) — el mapa que completas.
- [archaeology-04](../archaeology-04-correctness-failures/lesson-es.md) — el prerrequisito: síntoma,
  causa raíz, arreglo mínimo, arreglo estructural.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, la
  taxonomía de fallos; §5, la escala de severidad.
- [`../../../docs/archive.md`](../../../docs/archive.md) — el archivo fijado y su alcance.

También necesitas:

- un clon local del archivo en la etiqueta `v1.0.0`;
- `bash` y `grep`;
- un editor de texto y una forma de tomar notas;
- [`scan_globals.sh`](scan_globals.sh), que viene con esta lección.

> **Regla:** el archivo es de solo lectura. El escáner lo lee; nunca escribe en él.

## El recorrido de 60 minutos

| Min | Paso | Qué haces | Evidencia |
|---|---|---|---|
| 0–10 | Predecir | Ordena los globales por riesgo antes de leer las funciones | Tabla de predicción |
| 10–25 | Inspeccionar | Ejecuta el escáner; lee las cuatro declaraciones y las doce funciones | Salida del escáner y notas |
| 25–45 | Mapear | Completa una fila por global y una fila por función | `GLOBAL-STATE-MAP-TEMPLATE-es.md` |
| 45–55 | Explicar | Traza tres rutas de mutación; nombra la más difícil de probar | Notas de rutas |
| 55–60 | Registrar | Completa `EVIDENCE-es.md`; confirma que el archivo está limpio | Entrada de portafolio |

Si `movcab` tarda veinte minutos, eso es información sobre
`movcab`.

### La comprobación de ingeniería en esta lección

La pista B usa el mismo hábito que la pista A, con otro verbo: el programa ya existe, así que
**inspeccionas** en lugar de compilar.

| Paso | Dónde ocurre | Qué produce |
|---|---|---|
| **Predecir** | Paso 1 | Una lista de globales ordenada por riesgo con una afirmación falsable. |
| **Inspeccionar** | Paso 2 | La salida del escáner y las cuatro declaraciones. |
| **Mapear** | Paso 3 | Lecturas, escrituras e impresiones por global y por función. |
| **Explicar** | Paso 4 | Tres rutas de mutación y la más difícil de probar. |
| **Registrar** | Paso 5 | El mapa, los arreglos y `EVIDENCE-es.md`. |

## Paso 1 — Predecir (10 min)

Antes de leer las funciones, mira solo las declaraciones de las líneas 16–18 y 29. Conoces los
nombres y los tipos. Escribe una frase por grupo:

1. ¿Qué global esperas que escriban más funciones?
2. ¿Qué global esperas que se lea pero no se vuelva a escribir después de la configuración?
3. ¿Qué global esperas que cargue más de un significado?
4. ¿Qué función esperas que sea la más difícil de probar, y por qué?

Una predicción útil es falsable. "`jugada` es importante" no lo es. "`jugada` la escriben tres
funciones y la leen cuatro, y su valor 0 es un centinela de bucle" sí lo es.

### Una predicción trabajada

Toma `orden`:

- escritores esperados: uno, en la ruta de ubicación;
- lectores esperados: la impresora de turnos;
- riesgo esperado: bajo, porque no cambia después de la configuración;
- coste de prueba esperado: bajo una vez que sea un parámetro.

Esa predicción es útil aunque sea errónea. Si `orden` resultara escrito dentro del bucle de turnos,
el riesgo subiría. Escríbela antes de mirar.

## Paso 2 — Inspeccionar (15 min)

### Ejecutar el escáner

Desde la raíz del laboratorio:

```bash
./archaeology/lessons/archaeology-05-global-state/scan_globals.sh /ruta/al/archivo
```

Una ejecución de referencia contra el archivo fijado imprime:

```text
vectab=138
vecjugadas=15
cab1=30
cab2=30
jugada=69
orden=10
ganador=24
cf1=5
cc1=5
cf2=5
cc2=5
cab=15
simcab1=17
simcab2=17
sep=17
```

Los conteos son un punto de partida. Cuentan **menciones**, no mutaciones. Un nombre que aparece 138
veces puede escribirse en tres lugares; un nombre que aparece 5 veces puede escribirse en dos. El
mapa es el trabajo; el conteo solo te dice dónde mirar.

### Leer el escáner con honestidad

Sobre el archivo fijado, el escáner imprime un encabezado de función reconocible, no doce:

```text
function_headers:
226:int conversionmv(int cf,int cc)
```

Eso es una limitación de la expresión regular, no evidencia de que las otras once funciones no
existan. La mayoría de los encabezados llevan un comentario al final o espacios extra, y el patrón
no los admite. Registra el hueco en tu frontera de evidencia. Una herramienta que reporta de menos
sigue siendo útil si dices qué se le escapa.

| El escáner te da | El escáner no te da |
|---|---|
| Las líneas de declaración. | Qué función escribe cada global. |
| Un conteo de menciones por nombre. | Lecturas frente a escrituras. |
| Los encabezados que coinciden con una expresión. | Encabezados con comentarios o espaciado raro. |
| Nombres, incluidos los símbolos `const`. | Mutabilidad, propiedad o acoplamiento. |

### Leer la fuente

Lee las cuatro líneas de declaración y después las doce funciones en este orden. El orden está
elegido para que cada función añada un tipo nuevo de acoplamiento:

1. `inicializacion` — el escritor más simple. Un global, sin parámetros.
2. `conversionvm` — un escritor con cuatro salidas y sin valor de retorno.
3. `conversionmv` — el par de contraste: no accede a ningún global.
4. `vecjug` — un escritor de arreglo, sin parámetros.
5. `imprevecjug` — un lector puro que imprime.
6. `impresion` e `impresion0` — lectores que imprimen y leen índices de caballo.
7. `movcab` — la grande: seis lecturas globales, tres escrituras globales, impresión y parámetros
   por referencia.
8. `ordenjuego` — una función que decide, muta e imprime.
9. `impresionorden` — la orquestadora: lee, escribe, imprime y marca el ritmo de la consola.
10. `cabubicacion` — ubicación, orden de turnos y reporte en un solo cuerpo.
11. `main` — un bucle cuya condición es un global escrito tres llamadas más adentro.

## Paso 3 — Mapear (20 min)

### El protocolo del mapa

Para cada global, completa seis campos:

1. **Declaración.** Archivo y línea, más el tipo.
2. **Escrito por.** Cada función y línea que lo asigna. Incluye escritores indirectos alcanzados por
   una llamada, y márcalos como indirectos.
3. **Leído por.** Cada función y línea que lo lee. Incluye lecturas usadas solo para indexar otro
   arreglo.
4. **Efectos laterales.** Impresión, control de consola o escritura de otro global en la misma ruta.
5. **Ruta de mutación.** La cadena de llamadas más corta desde `main` hasta la escritura.
6. **Confianza.** Alta cuando la asignación es visible; media cuando se alcanza por una llamada;
   baja cuando el valor depende del comportamiento en ejecución.

Para cada función, completa cuatro campos: lee, escribe, imprime, acoplamiento oculto. El último es
una frase, no una lista.

### Global trabajado: `jugada`

Es el global más difícil del archivo, así que es el ejemplo trabajado. Complétalo una vez con la
clase y después haz los otros once tú.

| Campo | Entrada |
|---|---|
| Declaración | `variant-01:18` — `int cab1,cab2,jugada=1,orden,ganador=1;` |
| Escrito por | `ordenjuego` (733, 735, 743, 745); `impresionorden` (772, 778, 797, 803, 827, 832, 852, 858); `movcab` (708, 716) |
| Leído por | `main` (44, 46); `movcab` (243, 304, 360, 416, 472, 528, 585, 641, 697); `impresionorden` (756, 761, 775, 782, 800, 807, 814, 829, 836, 855, 862) |
| Efectos laterales | Ninguno propio. Condiciona la impresión y las escrituras del tablero en sus lectores. |
| Ruta de mutación | `main` → `ordenjuego` → `jugada`; `main` → `impresionorden` → `jugada`; `main` → `impresionorden` → `movcab` → `jugada` |
| Acoplamiento oculto | Tres significados en un entero: número de jugada, centinela de bucle (`0`) y valor de caso especial (`2`). |
| Confianza | Alta. Cada asignación es visible en la fuente. |

Los tres significados son la lección. `jugada` se incrementa para numerar los movimientos, se
compara con `0` para detener el juego y se compara con `2` para saltar una impresión. Un nombre,
tres contratos, y ningún comentario lo dice.

### Función trabajada: `movcab`

| Campo | Entrada |
|---|---|
| Lee | `vecjugadas`, `jugada`, `cab`, `vectab`, `cab1`, `cab2` |
| Escribe | `ganador` (257, 282, …), `vectab[ccab]` (269, 293, …), `jugada` (708, 716) |
| Imprime | Números de caso (262, 286, …), "No hay movimientos posibles" (704, 712), y llama a `impresion()` (705, 713) |
| Acoplamiento oculto | La firma promete dos referencias de entrada/salida. El cuerpo tiene seis entradas globales, tres salidas globales y salida de consola. |

Ahora compárala con `conversionmv`:

```cpp
int conversionmv(int cf,int cc)
{
	int ccab;
	ccab=cf * 8 +cc;
	return ccab;
}
```

Dos parámetros, un retorno, ningún global, ninguna impresión. Es la única función del archivo que
puedes probar con una sola llamada y sin configuración. Ese es el par de contraste de toda la
lección: el mismo archivo ya contiene la forma que quieres. El problema no es que el código no
supiera escribirla; el problema es que la forma no se aplicó al estado.

### Índice de anclas

Usa esta tabla para comprobar tu trabajo, no para reemplazarlo. Da la declaración y el primer
escritor/lector; tú todavía enumeras el resto.

| Global | Declarado | Primera escritura de función | Primera lectura | Imprime cerca |
|---|---|---|---|---|
| `vectab` | 17 | 63 (`inicializacion`) | 122 (`impresion0`) | 124, 132 |
| `vecjugadas` | 17 | 188 (`vecjug`) | 216 (`imprevecjug`) | 210 |
| `cab1` | 18 | 81 (`cabubicacion`) | 95 (`cabubicacion`) | 95 |
| `cab2` | 18 | 82 (`cabubicacion`) | 97 (`cabubicacion`) | 97 |
| `jugada` | 18 | 732 (`ordenjuego`) | 44 (`main`) | 737 |
| `orden` | 18 | 89 (`cabubicacion`) | 756 (`impresionorden`) | 759 |
| `ganador` | 18 | 257 (`movcab`) | 768 (`impresionorden`) | 770 |
| `cf1` | 29 | 106 (`conversionvm`) | 96 (`cabubicacion`) | 96 |
| `cc1` | 29 | 107 (`conversionvm`) | 96 (`cabubicacion`) | 96 |
| `cf2` | 29 | 108 (`conversionvm`) | 98 (`cabubicacion`) | 98 |
| `cc2` | 29 | 109 (`conversionvm`) | 98 (`cabubicacion`) | 98 |
| `cab` | 29 | 765 (`impresionorden`) | 249 (`movcab`) | 759 |

Los tres símbolos `const char` no están en la tabla porque no son mutaciones. Añade una nota en tu
mapa diciendo que los excluiste y por qué.

## Paso 4 — Explicar (10 min)

### Tres rutas de mutación

Escribe cada ruta como una cadena de llamadas y termina en la celda o variable que cambia.

**Ruta 1 — ubicación.**

```text
main → inicializacion → vectab[0..64]
main → cabubicacion → cab1, cab2
     → ordenjuego → vectab[cab1], vectab[cab2], jugada
     → impresion (lee vectab)
     → conversionvm → cf1, cc1, cf2, cc2
```

**Ruta 2 — turno.**

```text
main → vecjug → vecjugadas[0..7]
main → impresionorden → cab
     → movcab → ganador, vectab[ccab], jugada
     → impresion (lee vectab)
     → cab1 o cab2 (valor de retorno)
     → jugada++
```

**Ruta 3 — terminación.**

```text
movcab (default) → jugada=0
impresionorden (ganador==0) → jugada=0
main (jugada==0) → sale del bucle
```

Las rutas se solapan. Ese es el punto: la misma variable es contador de jugadas en la ruta 1,
compuerta de turno en la ruta 2 y condición de parada en la ruta 3.

### La ruta más difícil de probar

La ruta 2 a través de `impresionorden` es la más difícil. Para ejercitar un turno debes:

- fijar `orden`, `jugada`, `ganador`, `cab`, `cf1`, `cc1`, `vecjugadas` y `vectab`;
- proporcionar una consola, porque la función llama a `getch()` y a `system("cls")`;
- capturar la salida estándar, porque las aserciones están mezcladas con `cout`;
- aceptar que la función escribe `cab1` o `cab2` como efecto lateral.

Ocho globales, dos llamadas de consola y un flujo de salida. Una prueba unitaria para un movimiento
no es imposible; es una reimplementación del programa. Esa frase pertenece a tu mapa en "más difícil
de probar".

### El coste

Ponle precio al fallo de diseño en la moneda del trabajo real:

| Coste | Evidencia en este archivo |
|---|---|
| Un cambio de regla toca varias funciones. | El orden de turnos se fija en `cabubicacion`, se usa en `impresionorden` y se relee en `movcab`. |
| Una prueba necesita el programa completo. | `movcab` necesita seis globales; `impresionorden` necesita ocho más una consola. |
| Quien revisa no puede ver el contrato. | `movcab(int&, int&)` esconde seis entradas y tres salidas. |
| El estado puede desincronizarse. | `cab1` y `cf1`/`cc1` son dos representaciones de una posición. |
| Un fallo es difícil de reproducir. | El estado que falla es implícito; no hay un valor que imprimir en la frontera. |

### Dos arreglos, dos tamaños

**Arreglo mínimo.** Crea una costura. La más útil es hacer que `movcab` declare sus entradas y
devuelva su resultado:

```cpp
struct MoveResult {
    bool moved;
    bool captured;
    int  cell;
};

MoveResult movcab(int &cf, int &cc, int knight, int moveNumber,
                  const int (&order)[8], int board[64]);
```

Esto no elimina los globales. Hace que una función sea testeable sin la consola y sin leer a quien
la llama. Deja la impresión en `impresionorden`; una función que calcula no debería también
imprimir.

**Arreglo estructural.** Dale un dueño al estado. Un registro `GameState` guarda el tablero, los dos
caballos, el número de jugada y el resultado, y cada función lo recibe explícitamente. Después, un
tipo `Board` es dueño de sus celdas y un tipo `Knight` es dueño de su posición, de modo que `cab1` y
`cf1`/`cc1` no puedan desincronizarse. Enlaces a la pista A: `construction-11` (encapsulación) para
el invariante privado, `construction-13` (composición) para el tablero dentro del juego, con
`construction-08` (modular) y `construction-09` (tipo abstracto de datos) como peldaños anteriores.

No te saltes el arreglo mínimo. La regla de la pista es mínimo primero; la reescritura es un último
recurso con un precio. El arreglo mínimo prueba que entiendes el daño. El arreglo estructural
defiende la clase.

### Qué no arreglan los arreglos

El arreglo mínimo no elimina los tres significados de `jugada`; solo evita que una función los lea
implícitamente. El arreglo estructural no corrige el error de límite `i <= 64`; esa es una tarjeta
de corrección de `archaeology-04`. Mantén las dos reparaciones separadas en tu registro.

## Paso 5 — Registrar (5 min)

Completa [`EVIDENCE-es.md`](EVIDENCE-es.md):

- configuración y fijación del archivo;
- la tabla de predicción, escrita antes del mapa;
- la salida del escáner, incluido el hueco de encabezados;
- el mapa de mutaciones por global;
- la tabla de funciones;
- las tres rutas de mutación y la más difícil de probar;
- la frontera de evidencia;
- los arreglos mínimo y estructural con el enlace a la pista A;
- la comprobación de integridad del archivo.

El mapa es la evidencia. Una frase como "`movcab` está acoplada" es una opinión; una fila con seis
lecturas, tres escrituras y un sitio de impresión es un diagnóstico.

## El original en detalle

Hechos que citarás repetidamente, con números de línea del archivo fijado:

| Ubicación | Qué hay allí |
|---|---|
| 16 | `const char simcab1=21,simcab2=156,sep=219;` — símbolos de solo lectura. |
| 17 | `int vectab[64],vecjugadas[8];` — tablero y orden de movimientos. |
| 18 | `int cab1,cab2,jugada=1,orden,ganador=1;` — posiciones, jugada, orden, ganador. |
| 29 | `int cf1,cc1,cf2,cc2,cab;` — caché derivada de fila/columna y selector de turno. |
| 61–63 | `for (i=0;i<=64;i++) vectab[i]=0;` — el error de límite de `archaeology-04`. |
| 89 | `orden=ordenjuego(tirocab1,tirocab2);` — un valor de retorno asignado a un global. |
| 104–112 | `conversionvm` — escribe cuatro globales, no devuelve nada. |
| 226–232 | `conversionmv` — la única función sin globales. |
| 234 | `movcab(int &cf,int &cc)` — la firma que esconde el acoplamiento. |
| 239 | `switch(vecjugadas[i])` — lee el vector de orden. |
| 249 | `if (cab==1)` — lee el selector de turno. |
| 257 | `ganador=0;` — bandera de ganador invertida, fijada al capturar. |
| 269 | `vectab[ccab]=jugada;` — el número de jugada se vuelve el valor de la celda. |
| 708, 716 | `jugada=0;` — terminación escrita desde dentro de la función de movimiento. |
| 726–751 | `ordenjuego` — escribe dos celdas, incrementa `jugada`, imprime. |
| 753–868 | `impresionorden` — orquesta turnos, imprime, llama a `getch`. |
| 765, 790, 820, 844 | `cab=1` o `cab=2` — el selector de turno se fija antes de cada llamada. |
| 870–930 | `impresion` — lee el tablero y ambos índices de caballo, imprime. |

## Qué prueba y qué no prueba el mapa

| Afirmación | ¿Soportada? | Por qué |
|---|---|---|
| Un global se escribe en la línea citada. | Sí | La asignación es visible. |
| Una función depende de un global que no recibe. | Sí | La lectura es visible en el cuerpo. |
| Una función imprime como efecto lateral. | Sí | La llamada a `cout` está en el cuerpo. |
| El orden de mutación en ejecución es el del mapa. | Solo como lectura de la fuente | El mapa es estático; no se traza una ejecución. |
| Un valor lo escribe un llamado, no quien llama. | Inferido | El sitio de llamada muestra al llamado, no su cuerpo. |
| El fallo de diseño causa un resultado equivocado. | No | El programa funciona; el coste es cambio y testeabilidad. |
| El arreglo mínimo elimina el acoplamiento. | Solo en una costura | Los globales permanecen. |
| El arreglo estructural elimina la clase. | Por argumento de diseño | El peldaño es un concepto, no un resultado medido. |
| El compilador de 2008 se comportaba como uno moderno. | No | La fuente es el único testigo. |

Esta tabla es la comprobación de honestidad. Un mapa que afirma haber trazado una ejecución está
exagerando; di "leído de la fuente" cuando eso es lo que hiciste.

## Autocomprobación (binaria)

- [ ] Escribí una predicción ordenada antes de leer las funciones.
- [ ] Ejecuté `scan_globals.sh` y registré los conteos de menciones.
- [ ] Registré que el escáner reconoció un encabezado, no doce, y por qué.
- [ ] Separé los doce globales mutables de los tres símbolos `const char`.
- [ ] Cada global tiene declaración, lista de escritores, lista de lectores y ruta de mutación.
- [ ] Cada función tiene lecturas, escrituras, impresiones y una frase de acoplamiento oculto.
- [ ] Marqué las escrituras indirectas alcanzadas por una llamada.
- [ ] Nombré los tres significados de `jugada`.
- [ ] Expliqué por qué `ganador` necesita a `cab` para identificar al ganador.
- [ ] Trazé tres rutas de mutación desde `main` hasta el valor cambiado.
- [ ] Nombré la ruta más difícil de probar y dije por qué.
- [ ] Escribí la frontera de evidencia: verificado, inferido, desconocido.
- [ ] Propuse un arreglo mínimo que crea una costura.
- [ ] Propuse un arreglo estructural con enlace a la pista A (`construction-11`,
      `construction-13`).
- [ ] El archivo está sin modificar (`git status` en el archivo está limpio).

Si una casilla no está marcada, la casilla sin marcar nombra el siguiente paso. No hay penalización
por repetir uno.

## Ideas erróneas que esta lección refuta

| Idea errónea | Por qué es falsa | Qué hacer en su lugar |
|---|---|---|
| "El estado global siempre es un fallo de corrección." | Este programa funciona; el coste es cambio y testeabilidad. | Clasifícalo como fallo de diseño y ponle precio. |
| "Todo nombre de alcance de archivo es estado mutable." | `simcab1`, `simcab2` y `sep` son `const char`. | Separa constantes de estado en el mapa. |
| "Un conteo alto de menciones implica muchas mutaciones." | El escáner cuenta menciones, no asignaciones. | Enumera las escrituras tú mismo. |
| "`movcab` es una función de dos enteros." | Lee seis globales y escribe tres. | Lee el cuerpo, no la firma. |
| "`ordenjuego` solo decide quién empieza." | También escribe el tablero, avanza `jugada` e imprime. | Lista sus cuatro trabajos. |
| "`ganador` dice quién ganó." | Es una bandera: `0` significa que alguien ganó; `cab` dice quién. | Registra el par, no la bandera sola. |
| "`jugada` es solo un contador." | También es el centinela del bucle y un valor de caso especial. | Nombra sus tres significados. |
| "`cab1` y `cf1`/`cc1` son independientes." | Son dos representaciones de una posición. | Marca la caché derivada y sus puntos de sincronización. |
| "El arreglo mínimo es desechable." | Crea la primera costura testeable y prueba que el daño es removible. | Consérvalo y después defiende el arreglo estructural. |
| "El arreglo estructural debería aplicarse ahora." | La pista prohíbe saltar del mapa a la reescritura. | Mínimo primero; estructural después, con un precio. |
| "Un mapa estático es más débil que un depurador." | Ambos son evidencia; el mapa es lo que quien revisa puede leer sin ejecutar. | Di qué afirmaciones son leídas y cuáles ejecutadas. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Estado global | Un valor declarado fuera de toda función y alcanzable por todas. |
| Acoplamiento oculto | Una dependencia que no se ve en la firma de la función. |
| Acoplamiento de lectura | Una función depende de un valor que nadie le pasó. |
| Acoplamiento de escritura | Una función cambia estado que nadie le pidió cambiar. |
| Acoplamiento de impresión | Una función produce salida como efecto lateral de calcular. |
| Acoplamiento temporal | Una función solo funciona si otra corrió antes, en un orden dado. |
| Ruta de mutación | La cadena de llamadas desde `main` hasta el valor que cambia. |
| Caché derivada | Una segunda copia de un valor, sincronizada a mano. |
| Centinela | Un valor especial que marca un estado; aquí `jugada == 0`. |
| Costura | Un lugar donde el comportamiento se observa o reemplaza sin editar a quien llama. |
| Fallo de diseño | El programa funciona, pero la estructura encarece el cambio. |
| Frontera de evidencia | La línea entre lo que la fuente prueba y lo que infieres. |

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
