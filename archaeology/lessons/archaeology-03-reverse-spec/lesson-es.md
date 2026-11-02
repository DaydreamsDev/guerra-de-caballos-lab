# archaeology-03 — Reconstruir la especificación

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** especificación de comportamiento ·
> **Prerrequisito:** archaeology-02 · **Tiempo estimado:** 90 minutos
> **Lectura:** ~19 minutos
> **Rol de la lección:** tronco

## Por qué existe esta lección

El archivo no contiene una especificación. Contiene código fuente, algunos informes y un README por
variante. Si quieres saber qué hacía el programa de 2008, tienes dos opciones: adivinar a partir del
nombre de la carpeta, o leer el código y enunciar las reglas que realmente implementa. Esta lección
es la segunda opción.

Suena sencillo hasta que lo intentas. El código mezcla tres cosas que una especificación debe
mantener separadas: las reglas del juego, el generador aleatorio y la presentación en consola.
Guarda el tablero como un `int[64]` plano, marca las casillas visitadas con números de jugada,
imprime bytes CP437 y termina la partida poniendo a cero la misma variable que cuenta las jugadas.
Quien copia la estructura a la especificación ha escrito una descripción de la implementación, no
una especificación de comportamiento.

Hay una segunda trampa, más sutil. El laboratorio tiene una especificación moderna en
[`common/spec/game-rules.md`](../../../common/spec/game-rules.md). Es clara, completa y
determinista. La tentación es leerla primero y luego "encontrarla" en el archivo de 2008. Eso es
sesgo de confirmación con una cita. La especificación moderna es un **objetivo de comparación**, no
evidencia de lo que hacía el programa viejo. Lee primero el código viejo, escribe lo que hace, y
solo después abre el documento moderno.

El producto son dos artefactos: una especificación de comportamiento reconstruida a partir del
código, y al menos una traza heredada capturada. Ninguno de los dos alcanza por separado. El código
te dice las reglas; la traza muestra una ejecución de esas reglas con una semilla. La especificación
sin traza es una lectura. La traza sin especificación es un montón de bytes.

## Qué podrás hacer

Al terminar esta lección puedes:

- separar las reglas del juego, la aleatoriedad y la presentación en un programa heredado;
- escribir una cláusula de comportamiento observable y citar las líneas que la sostienen;
- distinguir lo que el código hace de lo que afirman un comentario, un nombre de carpeta o la
  especificación moderna;
- capturar una traza heredada con una semilla fija sin editar el archivo;
- explicar qué cambia el parche de semilla y qué no puede cambiar;
- leer un tablero CP437 y reconstruir los primeros movimientos;
- comparar la especificación reconstruida con la moderna y clasificar cada diferencia;
- enunciar los límites de la evidencia en cuatro categorías: verificado, inferido, solo documentado
  y desconocido;
- registrar la especificación, la traza y la comparación en `SPEC-TEMPLATE-es.md`,
  `TRACE-TEMPLATE-es.md` y `EVIDENCE-es.md`.

## El concepto: especificación de comportamiento

Una **especificación de comportamiento** es un contrato enunciado en términos de lo que un
observador puede ver: el tablero, las piezas, los movimientos legales, el orden de los turnos, las
condiciones de final y la salida. No es una descripción de las estructuras de datos, de los nombres
de funciones ni del flujo de control.

Cuatro artefactos se confunden con facilidad. Mantenlos separados durante toda la lección.

| Artefacto | Pregunta que responde | Ejemplo en este proyecto |
|---|---|---|
| **Especificación** | ¿Qué debe ocurrir? | "Una captura termina la partida y gana el caballo que captura." |
| **Implementación** | ¿Cómo se hace que ocurra? | `ganador=0` dentro del caso de `movcab`. |
| **Traza** | ¿Qué ocurrió en una ejecución? | Las 591 líneas producidas con la semilla 12345. |
| **Prueba** | ¿Cómo sabemos que la implementación sigue coincidiendo? | Un arnés que repite una semilla y compara transcripciones. |

El programa de 2008 tiene una implementación y, después de capturarla, trazas. No tiene
especificación ni prueba. Esa asimetría es la arqueología.

### El código es evidencia, no autoridad

En un sistema mantenido, la especificación es la autoridad y el código debe seguirla. En una
arqueología, el código es el único testigo que sobrevive. No puedes decir "el código está mal porque
la especificación moderna dice otra cosa". Dices: "el código hace X; la especificación moderna dice
Y; aquí está la diferencia y aquí está lo que cuesta".

Eso no significa que cada línea sea intencional. El código puede contener un defecto, y el defecto
es parte del comportamiento que realmente produjo. Registra primero el comportamiento. Diagnostícalo
después, en `archaeology-04`. El protocolo fija el orden por una razón: descripción antes que
juicio.

### Tres capas dentro de un archivo

Las 930 líneas contienen tres capas. Una especificación útil las nombra por separado.

| Capa | Qué pertenece allí | Dónde vive en el código |
|---|---|---|
| **Reglas del juego** | Tablero, piezas, movimientos legales, captura, trampa, final | `movcab`, `ordenjuego`, `impresionorden` |
| **Aleatoriedad** | Generador, semilla, orden de sorteos, rangos, política de repetición | `cabubicacion`, `vecjug` |
| **Presentación** | Símbolos, diseño del tablero, mensajes, limpieza de pantalla, pausas | `impresion`, `impresion0`, `system`, `getch` |

La especificación moderna pone las reglas y el algoritmo aleatorio en un documento, y el formato de
salida en otro. El programa heredado pone las tres cosas en una sola unidad de traducción. Tu
trabajo es separarlas.

### El determinismo es parte del contrato

Una especificación que dice "aleatorio" está incompleta. Una especificación que dice "extrae un
número de 0 a 50 con este generador, en este orden, y repite de esta manera" es reproducible. La
diferencia importa porque el programa heredado **no** es reproducible: llama a `srand(time(NULL))` y
a `rand()`, y no tiene argumento de semilla.

La sonda parchea la semilla en una copia de trabajo. Esa intervención hace posible una traza y
también cambia el programa. Registra ambos hechos. Una traza producida por un programa parcheado es
evidencia sobre el programa parcheado primero, y sobre el original después. La distancia entre ambos
es parte de la lección.

### La regla de la cita

Cada cláusula de tu especificación debe llevar una de tres marcas:

- **Verificado**: las líneas del código lo dicen y las citas.
- **Inferido**: el código lo implica, pero ninguna línea lo afirma. Di de qué lo inferiste.
- **Desconocido**: el código no puede responderlo. Di qué evidencia haría falta.

Una cláusula sin marca es una opinión. Las plantillas tienen espacio para la marca; úsalo.

## Antes de empezar

Lee esto completo:

- [`README-es.md`](README-es.md) — la ficha de la lección, el comando de captura y el criterio de
  aceptación.
- [`docs/guide/archaeology-track-es.md`](../../../docs/guide/archaeology-track-es.md) — §2, el
  protocolo de autopsia, en especial el paso 4, "Especificación de comportamiento".
- [`docs/archive-es.md`](../../../docs/archive-es.md) — el archivo fijado, el commit y el alcance.
- [`docs/guide/lesson-catalog-es.md`](../../../docs/guide/lesson-catalog-es.md) — el prerrequisito
  `archaeology-02` y el criterio de aceptación de `archaeology-03`.

También necesitas:

- un compilador de C++ (`c++`, `g++` o `clang++`) y su versión;
- `bash`, `timeout`, `wc`, `sha256sum` y `grep` — todos estándar;
- una copia local del archivo en la etiqueta `v1.0.0`;
- [`SPEC-TEMPLATE-es.md`](SPEC-TEMPLATE-es.md) y [`TRACE-TEMPLATE-es.md`](TRACE-TEMPLATE-es.md);
- [`code/capture_trace.sh`](code/capture_trace.sh), que viene con esta lección.

> **Regla:** el archivo es de solo lectura. `capture_trace.sh` copia el código a un directorio
> temporal y parchea la semilla allí. Nunca escribe dentro del archivo. Si te descubres editando
> `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`, detente.

## El camino de 90 minutos

| Min | Paso | Qué haces | Evidencia |
|---|---|---|---|
| 0–10 | Predecir | Escribes qué esperas del programa, antes de leer el código | Nota de predicción |
| 10–30 | Inspeccionar | Lees el código en orden de ejecución; construyes el registro de reglas | Registro con citas de línea |
| 30–50 | Capturar | Ejecutas `capture_trace.sh` con una semilla fija; lees el tablero y los mensajes | Archivo de traza y registro de captura |
| 50–65 | Comparar | Pones las reglas reconstruidas junto a la especificación moderna | Tabla de diferencias |
| 65–80 | Explicar | Clasificas las diferencias y enuncias los límites de la evidencia | Párrafo de límites |
| 80–90 | Registrar | Completas las dos plantillas y `EVIDENCE-es.md` | Entrada de portafolio |

Si una función tarda veinte minutos porque el flujo de control está
enredado, eso es información sobre la función, no sobre ti.

### La comprobación de ingeniería en esta lección

Track B usa el mismo hábito que Track A, con una sustitución: el programa ya está compilado, así que
**capturas** en lugar de compilar.

| Paso | Dónde ocurre | Qué produce |
|---|---|---|
| **Predecir** | Paso 1 | Cinco afirmaciones falsables sobre reglas, aleatoriedad y salida. |
| **Inspeccionar** | Paso 2 | Un registro de reglas con líneas y marcas de confianza. |
| **Capturar** | Paso 3 | Una traza con semilla, su estado y sus volúmenes. |
| **Comparar** | Paso 4 | Una tabla de diferencias contra la especificación moderna. |
| **Explicar** | Paso 5 | Por qué existen las diferencias y dónde se detiene la evidencia. |
| **Registrar** | Paso 6 | Las dos plantillas completas y `EVIDENCE-es.md`. |

## Paso 1 — Predecir antes de leer (10 min)

Tienes el archivo y no has abierto el código. Escribe cinco predicciones:

1. **Tablero y piezas.** ¿De qué tamaño es el tablero, cuántos caballos hay y cómo se muestran?
2. **Inicio.** ¿Cómo se elige quién empieza y dónde arrancan los caballos?
3. **Prioridad de movimiento.** ¿Cómo elige un caballo entre sus ocho movimientos?
4. **Aleatoriedad.** ¿Qué es aleatorio y serían idénticas dos ejecuciones con la misma semilla?
5. **Final.** ¿Cómo termina la partida y qué imprime cuando termina?

Después escribe una frase falsable sobre el conjunto. Por ejemplo:

> Espero un tablero de 8×8 con dos caballos, una tirada aleatoria para elegir quién empieza, una
> ubicación aleatoria, un orden rotado de los ocho movimientos del caballo y un final cuando un
> caballo captura al otro; espero que el programa no sea reproducible porque siembra desde el reloj.

Esa frase puede estar mal en al menos tres puntos. Bien. Una predicción que no puedes falsar no
enseña nada.

### Una predicción trabajada

Toma la afirmación 4, la aleatoriedad:

- respuesta esperada: el programa siembra desde el reloj y llama a `rand()`;
- evidencia esperada: `srand(time(NULL))` y `rand()` en la función de ubicación;
- veredicto esperado: sin argumento de semilla, dos ejecuciones no son comparables.

Esa predicción es lo bastante específica para fallar. Si el programa resulta extraer los mismos
números dentro de un mismo segundo de reloj, la predicción falla de una manera interesante.
Escríbela antes de mirar.

## Paso 2 — Inspeccionar el código (20 min)

Lee el archivo en orden de ejecución, no de arriba abajo. Las declaraciones de arriba son un mapa;
el comportamiento está en las funciones.

| Orden | Líneas | Qué buscar |
|---:|---|---|
| 1 | 16–18, 29 | Constantes, globales y las variables que llevan el estado |
| 2 | 32–55 | Punto de entrada y bucle exterior del juego |
| 3 | 57–67 | Inicialización del tablero |
| 4 | 69–112 | Ubicación, tiradas y conversión de índice a coordenada |
| 5 | 176–205 | Generación del orden de movimientos |
| 6 | 226–232 | Conversión de coordenada a índice |
| 7 | 234–724 | Evaluación de movimientos, captura y trampa |
| 8 | 726–751 | Quién empieza y cómo se marcan las casillas iniciales |
| 9 | 753–868 | Bucle de turnos |
| 10 | 870–930 | Impresión del tablero |

Mientras lees, llena un **registro de reglas**. Una fila por comportamiento, con las líneas que lo
sostienen y una marca de confianza.

| Regla | Líneas | Evidencia | Marca |
|---|---:|---|---|
| El tablero es de 8×8 | 874–875 | Dos bucles anidados, `0..7` cada uno | Verificado |
| El tablero se guarda como 64 casillas | 17, 61–65 | `int vectab[64]` | Verificado |
| El índice de una casilla es `fila*8+columna` | 226–232 | `conversionmv` | Verificado |
| Un movimiento de caballo es uno de ocho desplazamientos | 241–695 | Ocho cuerpos `case` | Verificado |
| Una captura pone `ganador=0` | 254–258 | La comprobación contra el otro caballo | Verificado |
| Una trampa termina la partida | 702–717 | `jugada=0` tras ocho movimientos fallidos | Verificado |

### Hallazgo 1: el tablero es plano, pero la pantalla es una cuadrícula

El tablero es `int vectab[64]` (línea 17). Una casilla se direcciona con `fila*8+columna`
(línea 229). La función de ubicación convierte un índice a fila y columna base cero con `cab/8` y
`cab%8` (líneas 106–109), y luego imprime `fila:cf+1` y `columna:cc+1` (líneas 96–98). La cuadrícula
interna es 0..7 y la que se muestra es 1..8.

Eso es un detalle de presentación, no un cambio de regla. La especificación moderna usa 1..8
directamente. Cuando escribas la especificación, enuncia las coordenadas mostradas y menciona la
convención interna solo si afecta el comportamiento.

### Hallazgo 2: las casillas iniciales se marcan como visitadas

Este es el hallazgo que más lectores pasan por alto. En `ordenjuego`, la casilla inicial del primer
jugador se escribe con `jugada` (1), luego `jugada` se incrementa a 2, y la casilla del segundo
jugador se escribe con 2 (líneas 730–748). Por eso el primer movimiento real recibe el número 3.

Como la comprobación de visitada es `vectab[ccab]==0 || vectab[ccab]==vectab[otro]` (líneas 252,
276), una casilla inicial distinta de cero se trata como visitada. Un caballo no puede volver a su
propia casilla inicial, y el otro no puede entrar en ella salvo para capturar. La especificación
moderna dice que las casillas iniciales **no** están visitadas. Es una diferencia real de regla, no
de codificación.

### Hallazgo 3: el orden de movimientos se extrae una vez por par de turnos

El bucle exterior llama a `vecjug()` una vez y luego a `impresionorden()` (líneas 48–49).
`impresionorden()` atiende los turnos de ambos caballos (líneas 756–868) y nunca vuelve a llamar a
`vecjug()`. Por eso el mismo orden de ocho movimientos se usa para el movimiento del primer jugador
y para la respuesta del segundo. La siguiente iteración exterior extrae un orden nuevo.

Se ve en la traza. Con la semilla 12345, los dos primeros turnos imprimen
`El vector de movimiento es: 2,3,4,5,6,7,8,1.` (líneas 41 y 64 de la traza). La especificación
moderna extrae un orden nuevo en cada turno. Es otra diferencia real.

### Hallazgo 4: `rand() % 51` no es 1..50

Las líneas 78–79 dicen:

```cpp
tirocab1= rand() % 51;
tirocab2= rand() % 51;
```

El comentario las llama "tiros" pero no enuncia un rango. `rand() % 51` produce 0..50 inclusive. La
especificación moderna dice 1..50. La diferencia es pequeña y real. Registra el rango del código, no
el rango que esperabas del documento moderno.

### Hallazgo 5: la semilla se reinicia dentro del bucle de reintento

`cabubicacion` ejecuta un bucle `while (i==1)` (línea 73). Dentro, cada iteración llama a
`srand(time(NULL))` (línea 76), luego extrae dos tiradas y dos posiciones (líneas 78–82). Si
cualquiera de los dos pares coincide, el bucle se repite y vuelve a sembrar desde el reloj
(línea 83).

En el original, una colisión hace que el programa gire hasta que cambie la resolución de un segundo
del reloj. Con el parche `srand(SEED)` de la sonda, la misma semilla se aplica en cada iteración,
así que una semilla con colisión gira **para siempre**. La semilla 9999 es una de ellas: sus dos
posiciones son 19. La sonda devuelve `run_status=124` después del límite de cinco segundos y no
produce salida estándar. Eso es un artefacto de la sonda, no una prueba de que el original se
cuelgue.

### Hallazgo 6: el juego no tiene empate

`jugada` es a la vez el contador de jugadas y la bandera de final. Empieza en 1, pasa a 3 después de
la ubicación, y se incrementa tras cada movimiento exitoso (líneas 778, 803, 832, 858). Una captura
pone `ganador=0` (líneas 257, 282 y los bloques equivalentes de los otros casos). Una trampa pone
`jugada=0` (líneas 708, 716). No hay límite de 200 movimientos ni estado de `empate`. La partida
termina solo por captura o por trampa.

Como cada movimiento exitoso marca una casilla nueva y el tablero tiene 64 casillas, una trampa es
inevitable en un número finito de movimientos. La ausencia de regla de empate sigue siendo una
diferencia de especificación.

### Qué hacer cuando el código es ambiguo

El código tiene comentarios que narran, comentarios que mienten e identificadores en español con
erratas. No resuelvas la ambigüedad eligiendo la lectura más agradable. Escribe la ambigüedad:

- qué hace el código en el camino que puedes trazar;
- qué afirma el comentario;
- cuál usaste y por qué;
- qué probaría una segunda traza.

Esa es la diferencia entre una especificación y una suposición.

## Paso 3 — Capturar una traza (20 min)

Desde este directorio, ejecuta:

```bash
./code/capture_trace.sh /ruta/al/archivo 12345 /tmp/legacy-trace-12345.txt
```

El script copia el código a un directorio temporal, reemplaza `srand(time(NULL));` por
`srand(12345);`, compila con `-std=c++20 -fpermissive -Dmain=legacy_main -Icode/compat`, enlaza el
envoltorio y ejecuta el resultado con `timeout 5s`. Imprime cinco líneas.

Una ejecución de referencia en esta máquina:

```text
seed=12345
output=/tmp/legacy-trace-12345.txt
run_status=0
stdout_lines=591
stderr_lines=25
```

Tus números cambiarán si cambian tu biblioteca C, tu compilador o tu máquina. Solo `run_status=0` y
la forma de la salida son estables. Los conteos de líneas son una muestra, no un contrato.

### Qué significan las cinco líneas

| Línea | Significado | Qué no significa |
|---|---|---|
| `seed=12345` | El valor sustituido en la copia de trabajo | Que el original aceptara un argumento de semilla |
| `output=...` | Dónde se escribió la salida estándar | Que el archivo sea una transcripción moderna |
| `run_status=0` | El proceso llegó a su propio final | Que el comportamiento sea correcto |
| `stdout_lines=591` | Líneas de tablero y mensajes | Que 591 sea una propiedad estable |
| `stderr_lines=25` | Líneas de llamadas `system` fallidas | Que el programa informara errores |

Las 25 líneas de error son el shell de Linux quejándose de `color` y `cls`. El programa llama a
`system("color 1f")` una vez (línea 38) y a `system("cls")` entre turnos, y una vez al inicio
(líneas 43, 784, 809, 838, 864). En Linux esos comandos no existen, así que cada llamada escribe
una línea de error localizada. El conteo es un `color` más un `cls` por movimiento. No es un error
del programa.

### Leer la traza

La primera pantalla es un tablero, luego las tiradas y las posiciones:

```text
Juega primero el caballo 2 <sym> porque:

El tiro del caballo 1 <sym> fue:

15

El tiro del caballo 2 <sym> fue:

44

El numero aleatorio para ubicar el caballo 1 <sym> fue:  45

En terminos de filas y columnas es: fila:6 columna:6

El numero aleatorio para ubicar el caballo 2 <sym> fue:  59

En terminos de filas y columnas es: fila:8 columna:4
```

`<sym>` representa el byte CP437 crudo. El archivo real contiene `0x15` para el caballo 1 y `0x9C`
para el caballo 2, no el texto `<sym>`. Una terminal UTF-8 muestra caracteres de reemplazo.

Las tiradas son 15 y 44. La mayor es 44, así que empieza el caballo 2. Las posiciones son 45 y 59:
el índice 45 es fila 6, columna 6; el índice 59 es fila 8, columna 4. El tablero del inicio de la
traza muestra al caballo 2 con el número de jugada 1 y al caballo 1 con el número 2, porque la
casilla del primer jugador se escribe primero.

La traza termina con una captura:

```text
evaluo los casos: 6, 7, 8, 1.El  CABALLO 1 <sym> HA GANADO
```

El último tablero muestra el número de jugada 26 en la casilla que captura. Las dos ubicaciones
iniciales consumieron los números 1 y 2, así que la partida tuvo 24 movimientos reales de caballo.
Esa distinción pertenece a tu registro de traza.

### Demostrar que la semilla la hace reproducible

Ejecuta la misma semilla dos veces y compara los bytes:

```bash
./code/capture_trace.sh /ruta/al/archivo 12345 /tmp/trace-a.txt
./code/capture_trace.sh /ruta/al/archivo 12345 /tmp/trace-b.txt
sha256sum /tmp/trace-a.txt /tmp/trace-b.txt
```

Los dos hashes coinciden. Eso es determinismo **del programa parcheado**: misma semilla, misma
biblioteca C, mismo compilador, mismos bytes. No es determinismo del original, que siembra desde el
reloj y no tiene argumento de semilla.

### Ejecutar una segunda semilla y observar la varianza

Los conteos de líneas se mueven con la semilla porque la duración de la partida se mueve con la
semilla. Medido en la máquina de referencia con glibc:

| Semilla | `run_status` | Líneas de salida | Líneas de error | Final |
|---:|---:|---:|---:|---|
| 12345 | 0 | 591 | 25 | captura, caballo 1 |
| 1 | 0 | 476 | 20 | trampa |
| 7 | 0 | 729 | 31 | captura, caballo 1 |
| 42 | 0 | 522 | 22 | captura, caballo 1 |
| 777 | 0 | 269 | 11 | captura, caballo 2 |
| 2024 | 0 | 1074 | 46 | trampa |
| 9999 | 124 | 0 | 1 | el bucle de ubicación no terminó |

Dos filas merecen atención. La semilla 1 termina en trampa, que es la única forma de ver el mensaje
de trampa sin una captura. La semilla 9999 es el caso de colisión del hallazgo 5: el programa
parcheado nunca sale del bucle de ubicación. Un `124` aquí es una afirmación sobre la sonda, no
sobre el binario de 2008.

### Qué prueba la traza

- El código parcheado compila y se ejecuta en esta máquina.
- Para esta semilla y esta biblioteca C, el programa produce un tablero y un final específicos.
- El tablero usa bytes CP437 y mensajes en español.
- El programa no tiene transcripción, ni números de turno, ni coordenadas en forma legible por una
  máquina.

### Qué no prueba la traza

- Que la ejecución original de 2008 produjera estos bytes.
- Que el original usara la misma secuencia de `rand()`.
- Que todas las semillas terminen de la misma manera.
- Que la transcripción moderna pueda reproducirse a partir de esta traza.

## Paso 4 — Comparar con la especificación moderna (15 min)

Abre [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) solo ahora. Pon los dos
documentos lado a lado y clasifica cada cláusula como **misma regla**, **regla diferente** o **no
observable en la salida heredada**.

### Qué coincide

Estos son acuerdos reales, y vale la pena enunciarlos:

- el tablero es de 8×8;
- hay dos caballos, uno por bando;
- cada caballo se mueve con los mismos ocho desplazamientos;
- un movimiento es legal solo dentro del tablero y hacia una casilla no visitada, salvo una captura;
- el caballo activo prueba el orden rotado y toma el primer movimiento legal;
- una captura termina la partida de inmediato;
- un caballo encerrado termina la partida;
- después de un movimiento exitoso, mueve el otro caballo;
- las coordenadas mostradas son 1..8.

### Qué difiere

| # | Especificación moderna | Código heredado | Líneas | Clase |
|---:|---|---|---|---|
| 1 | LCG de 64 bits con constantes fijas | `rand()` de la biblioteca C | 78–82, 180 | Generador distinto |
| 2 | La semilla es una entrada | `srand(time(NULL))`, sin argumento | 76 | Control distinto |
| 3 | Posiciones primero, tiradas después | Tiradas primero, posiciones después | 78–82 | Orden de sorteos distinto |
| 4 | Tiradas 1..50 | Tiradas 0..50 | 78–79 | Rango distinto |
| 5 | Repetir solo el valor que colisiona | Repetir los cuatro valores | 83 | Política distinta |
| 6 | Orden nuevo en cada turno | Orden nuevo cada par de turnos | 48–49, 753–868 | Regla distinta |
| 7 | Las casillas iniciales no están visitadas | Las casillas iniciales valen 1 y 2 | 730–748 | Regla distinta |
| 8 | Empate tras 200 movimientos | No hay empate | 44, 708, 716 | Regla ausente |
| 9 | La trampa registra al otro caballo como ganador | Imprime "quedo encerrado", sin línea de ganador | 702–717 | Salida distinta |
| 10 | Transcripción UTF-8 `clave=valor` | Tablero CP437 y prosa en español | 870–930 | Formato distinto |
| 11 | El número de jugada empieza en 1 | El primer movimiento real es 3 | 730–748 | Numeración distinta |
| 12 | `player` / `machine` | `caballo 1` / `caballo 2` | 737, 747 | Vocabulario distinto |

### Las tres clases de diferencia

No todas las filas significan lo mismo. Clasifícalas antes de explicarlas.

**Diferencias de codificación.** La misma regla, expresada de otra forma. Las coordenadas mostradas
son 1..8 en ambos, pero el heredado guarda 0..7. El ganador es el mismo caballo, pero las etiquetas
difieren. Un movimiento es el mismo desplazamiento, pero el heredado imprime un número de jugada en
lugar de un par de coordenadas. Estas diferencias importan para un adaptador de transcripción, no
para el juego.

**Diferencias de regla.** El comportamiento mismo es distinto. Las casillas iniciales están
visitadas en el heredado y no en la especificación moderna. El orden de movimientos se extrae por
par de turnos, no por turno. Las tiradas son 0..50, no 1..50. No hay empate. Estas son las filas que
hacen falsa una afirmación ingenua de "misma semilla, misma partida".

**Diferencias no observables.** La salida heredada no puede decirte algunas cosas: si el código
original pretendía una regla de empate, si el rango 0..50 era deliberado, o qué hacía el `rand()`
original en la herramienta de 2008. Márcalas como desconocidas. No las rellenes con
la especificación moderna.

### El límite de la aleatoriedad

Esta es la diferencia más profunda. La especificación moderna define un generador congruencial
lineal de 64 bits con constantes nombradas. El programa heredado llama a `rand()`, cuyo algoritmo
está definido por la implementación. En glibc es un generador de retroalimentación aditiva; en el
runtime de Microsoft de la época era otro LCG. Aunque parchees la misma semilla en ambos, las
secuencias divergen.

Por eso la traza heredada **no** es una traza de referencia para el motor moderno. Es una muestra de
comportamiento de un programa distinto. Puedes comparar las reglas, el diseño del tablero y las
condiciones de final. No puedes comparar las secuencias de números. Si quieres probar que el motor
moderno reproduce el juego heredado, necesitas un generador común y un formato de transcripción que
el programa heredado nunca tuvo.

Esa conclusión es la lección. La especificación moderna es un contrato diseñado; el programa
heredado es una implementación que nunca tuvo uno. Las diferencias no son defectos automáticos.
Algunas son decisiones, algunas son accidentes y algunas simplemente se perdieron.

## Paso 5 — Explicar los límites (15 min)

Escribe un párrafo que responda: qué probó el código, qué mostró la traza, qué inferiste y dónde se
detiene la evidencia.

Usa esta forma si necesitas una:

> El código prueba **\_\_\_**. La traza muestra **\_\_\_** para la semilla **\_\_\_**. Inferí
> **\_\_\_** de **\_\_\_**, con confianza **\_\_\_**. La evidencia se detiene en **\_\_\_**.

### Las cuatro categorías

Pon cada afirmación que uses en exactamente una categoría.

| Categoría | Prueba | Ejemplos de esta lección |
|---|---|---|
| **Verificado en el código** | Puedes citar las líneas. | Los ocho desplazamientos; el rango `rand()%51`; las escrituras de las casillas iniciales; la rama de trampa. |
| **Observado en una traza** | Lo ejecutaste y leíste los bytes. | Las 591 líneas de la semilla 12345; las tiradas 15 y 44; la captura del caballo 1. |
| **Inferido de la estructura** | El código lo implica, pero ninguna línea lo afirma. | Que el mismo orden se usa para ambos turnos; que las casillas iniciales cuentan como visitadas. |
| **Desconocido** | El código no puede responderlo. | La secuencia original de `rand()`; la intención original; si se consideró una regla de empate. |

### Las cuatro afirmaciones que la traza no sostiene

| Afirmación | Por qué esta traza no puede sostenerla |
|---|---|
| "Esto es lo que imprimía el programa de 2008." | La semilla se parcheó y la compilación usa una biblioteca C moderna. |
| "El motor moderno puede reproducir esta traza." | Los generadores son distintos; los formatos son distintos. |
| "El juego es determinista." | Solo lo es el programa parcheado; el original siembra desde el reloj. |
| "Las reglas son correctas." | La corrección necesita un contrato; esta lección reconstruye uno, no lo valida. |

### La frase honesta

Termina con una frase que defenderías en una revisión de código:

> *"El código de 2008 implementa un juego de dos caballos en 8×8 con `rand()` de la biblioteca C,
> semilla de reloj, un orden de movimientos por par de turnos y casillas iniciales marcadas como
> visitadas; una compilación parcheada con la semilla 12345 produce un tablero CP437 y termina en
> una captura, y la traza es evidencia sobre esa compilación parcheada, no sobre la herramienta
> original ni sobre la transcripción moderna."*

Si tu frase es más corta y más fuerte que esa, revisa qué cláusula eliminaste.

## Paso 6 — Registrar (10 min)

Completa tres documentos:

1. [`SPEC-TEMPLATE-es.md`](SPEC-TEMPLATE-es.md) — la especificación reconstruida, con citas de línea
   y marcas de confianza.
2. [`TRACE-TEMPLATE-es.md`](TRACE-TEMPLATE-es.md) — el registro de captura, el inicio observado, los
   primeros movimientos, el final y los límites.
3. [`EVIDENCE-es.md`](EVIDENCE-es.md) — el registro de portafolio: entorno, predicción, registro de
   reglas, captura, comparación, límites y siguiente paso.

Dos hábitos que vale la pena conservar:

- **Cita la línea, no la memoria.** "Las casillas iniciales están visitadas" es una afirmación.
  `ordenjuego`, líneas 730–748, es evidencia.
- **Registra el entorno.** Compilador, biblioteca C, commit del archivo, semilla y comando. Una
  traza sin entorno es un rumor.

## El original en detalle

Hechos que citarás repetidamente, con números de línea del archivo fijado:

| Línea | Qué hay allí | Por qué importa para la especificación |
|---:|---|---|
| 16 | `simcab1=21`, `simcab2=156`, `sep=219` | Símbolos CP437, no UTF-8. |
| 17–18 | `vectab[64]`, `vecjugadas[8]`, variables sueltas de caballo | El estado es global. |
| 32 | `void main()` | No es C++ estándar; la sonda lo renombra. |
| 38 | `system("color 1f")` | Llamada a consola Windows; falla en Linux. |
| 42 | `getch()` | Pausa; la cuña la convierte en no-op. |
| 43 | `system("cls")` | Limpieza de pantalla; falla en Linux, una vez por turno. |
| 44 | `while (jugada!=0)` | Bucle del juego; `jugada` también es el contador. |
| 61 | `for (i=0;i<=64;i++)` | Error de límite; diagnostícalo en `archaeology-04`. |
| 76 | `srand(time(NULL))` | Semilla de reloj, dentro del bucle de reintento. |
| 78–79 | `rand() % 51` | Tiradas 0..50, no 1..50. |
| 81–82 | `rand() % 64` | Posiciones 0..63. |
| 83 | Comprobación de colisión | Repite los cuatro valores. |
| 104–112 | `conversionvm` | Índice a fila y columna base cero. |
| 176–205 | `vecjug` | Orden de movimientos rotado, un sorteo por llamada. |
| 226–232 | `conversionmv` | `fila*8+columna`. |
| 234–724 | `movcab` | Ocho casos, captura, trampa. |
| 252 | `vectab[ccab]==0` o la casilla del otro caballo | Comprobación de visitada; las casillas iniciales no son cero. |
| 257 | `ganador=0` | Bandera de captura. |
| 702–717 | Rama de trampa | Imprime "quedo encerrado", pone `jugada=0`. |
| 730–748 | `ordenjuego` | La casilla del primero vale 1; la del segundo, 2. |
| 753–868 | `impresionorden` | Dos turnos por llamada, un solo vector de orden. |
| 870–930 | `impresion` | Tablero 8×8 con números de jugada y símbolos de caballo. |

## Qué cuenta como cláusula de especificación

Una cláusula es comprobable cuando un observador podría decidir si ocurrió.

| Cláusula débil | Por qué es débil | Cláusula fuerte |
|---|---|---|
| "El tablero es un `int[64]`." | Es una estructura de datos, no comportamiento. | "El tablero es 8×8; una casilla es `fila,columna` con ambos de 1 a 8." |
| "El código llama a `rand()%51`." | Es un detalle de implementación salvo que el generador sea parte del contrato. | "Las tiradas heredadas se extraen de 0..50 inclusive, en este orden." |
| "Imprime el tablero." | No es comprobable. | "Imprime ocho filas; cada casilla vacía son cuatro bytes `0xDB` y dos espacios." |
| "El caballo se mueve al azar." | No es comprobable. | "El orden es una rotación que empieza en un valor extraído de 1..8." |
| "El juego termina." | No es comprobable. | "El juego termina por captura o trampa; no hay empate." |

El generador es la excepción. Para una especificación reproducible, el generador es parte del
contrato, así que "el código llama a `rand()`" se convierte en "el heredado usa el `rand()` de la
plataforma, que está definido por la implementación". Eso es un límite, no un detalle.

## Qué no hace esta lección

- No diagnostica un solo defecto. El error de límite de la línea 61 es visible y no es el tema de
  hoy. Anótalo y sigue.
- No arregla nada. El archivo es de solo lectura y la sonda es un adaptador de captura.
- No afirma que la traza heredada sea una traza de referencia para el motor moderno.
- No prueba el comportamiento de 2008, la secuencia original de `rand()` ni la herramienta.
- No infiere la intención original. Registra el comportamiento y los límites.
- No valida la especificación moderna. La usa como objetivo de comparación.

Si te descubres explicando *por qué* una regla está mal, detente y escríbelo como pregunta para
`archaeology-04`.

## Autocomprobación (binaria)

- [ ] Escribí cinco predicciones antes de abrir el código.
- [ ] Leí el código en orden de ejecución y construí un registro de reglas con citas de línea.
- [ ] Separé reglas del juego, aleatoriedad y presentación.
- [ ] Registré que las casillas iniciales se marcan como visitadas.
- [ ] Registré que el orden de movimientos se extrae una vez por par de turnos.
- [ ] Registré que las tiradas son 0..50, no 1..50.
- [ ] Expliqué por qué `srand(time(NULL))` está dentro del bucle de reintento.
- [ ] Capturé al menos una traza con `capture_trace.sh` y registré sus cinco líneas.
- [ ] Ejecuté la misma semilla dos veces y confirmé bytes idénticos.
- [ ] Ejecuté una segunda semilla y registré cómo cambiaron los conteos de líneas.
- [ ] Expliqué el `run_status=124` de la semilla 9999 como artefacto de la sonda.
- [ ] Leí el tablero CP437 y reconstruí los primeros movimientos.
- [ ] Comparé la especificación reconstruida con `common/spec/game-rules.md`.
- [ ] Clasifiqué cada diferencia como codificación, regla o no observable.
- [ ] Enuncié el límite de la aleatoriedad y por qué la traza heredada no es una traza de
      referencia moderna.
- [ ] Completé `SPEC-TEMPLATE-es.md`, `TRACE-TEMPLATE-es.md` y `EVIDENCE-es.md`.
- [ ] El archivo está sin modificar (`git status` en el archivo está limpio).

Si una casilla no está marcada, la casilla sin marcar nombra el siguiente paso. No hay penalización
por repetir uno.

## Misconcepciones que esta lección refuta

| Concepto erróneo | Por qué es falso | Qué hacer en su lugar |
|---|---|---|
| "La especificación moderna te dice qué hacía el programa viejo." | La moderna es un contrato diseñado; el viejo nunca tuvo uno. | Lee primero el código y después compara. |
| "El código es la especificación." | El código es una implementación; contiene defectos y detalles de presentación. | Enuncia comportamiento observable y cita las líneas. |
| "Un comentario es documentación." | Los comentarios narran, mienten o describen una versión anterior. | Cita el camino del código, no el comentario. |
| "`rand()%51` significa 1..50." | Significa 0..50 inclusive. | Registra el rango real. |
| "La traza es determinista, así que el programa lo es." | Solo lo es la compilación parcheada; el original siembra desde el reloj. | Separa la sonda del original. |
| "Misma semilla significa misma partida en ambas implementaciones." | Los generadores y el orden de sorteos difieren. | Compara reglas, no secuencias de números. |
| "Un `124` significa que el original se cuelga." | El `srand(SEED)` de la sonda convierte una colisión en un bucle infinito. | Regístralo como artefacto de la sonda. |
| "Las casillas iniciales están vacías." | Valen 1 y 2 y cuentan como visitadas. | Revisa `ordenjuego` y la prueba de visitada. |
| "El tablero imprime un rastro." | Imprime números de jugada; el rastro es implícito en las casillas no cero. | Describe el contenido real de la casilla. |
| "Sin regla de empate, la partida puede durar para siempre." | El tablero es finito y cada movimiento marca una casilla; una trampa la termina. | Enuncia el argumento de finitud y la regla ausente por separado. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Especificación de comportamiento | Un contrato en términos observables, independiente de la implementación. |
| Registro de reglas | Una tabla de comportamientos con las líneas que sostienen cada uno. |
| Traza | La salida observable de una ejecución con una semilla fija. |
| Determinismo | Mismas entradas, mismas salidas, siempre. |
| Semilla | El valor que inicializa el generador aleatorio. |
| Generador | El algoritmo que produce la secuencia aleatoria. |
| Orden de sorteos | La secuencia en que se piden los valores aleatorios. |
| Política de repetición | Qué valor se vuelve a extraer tras una colisión. |
| Diferencia de codificación | La misma regla expresada en otro formato. |
| Diferencia de regla | Un comportamiento genuinamente distinto. |
| Límite de la evidencia | La línea entre lo que verificaste y lo que inferiste. |
| Traza de referencia | Una traza usada como contrato de regresión para una implementación concreta. |

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
