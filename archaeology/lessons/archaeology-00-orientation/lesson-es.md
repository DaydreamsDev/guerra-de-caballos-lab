# archaeology-00 — Orientación: leer un código heredado

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** modelo mental · **Prerrequisito:** common-04 ·
> **Tiempo estimado:** 45 minutos
> **Lectura:** ~16 minutos

## Por qué existe esta lección

Un archivo heredado no llega con un manual. Llega como una sola unidad de traducción, un montón de
prototipos, algunos nombres con alcance de archivo y un `main` que llama a cosas que nunca viste. El
primer trabajo no es juzgarlo, arreglarlo ni explicar su historia. El primer trabajo es construir un
modelo lo bastante bueno para responder: **¿qué es este programa y dónde miraría después?**

Cada lección posterior de esta pista hace una pregunta que empieza con "dónde". ¿Dónde está el
estado? ¿Dónde ocurre un movimiento? ¿De dónde sale la salida? Sin un mapa, cada pregunta reinicia
la lectura desde cero. Con un mapa, cada pregunta aterriza en una línea.

Esta lección es la primera pasada. Produce tres cosas:

1. un **mapa de código** con el punto de entrada, las funciones, el estado y el flujo de datos;
2. una **frase de propósito** que puedes defender contra el código;
3. una **siguiente pregunta** lo bastante concreta para responderla en la lección siguiente.

No diagnostica un defecto, no propone una reparación y no califica el código. Esos son trabajos
posteriores, y son más baratos cuando el mapa ya existe.

## Qué podrás hacer

Al terminar esta lección puedes:

- escribir una predicción falsable sobre un programa desconocido antes de leer sus cuerpos;
- localizar el punto de entrada y decir qué inicia y qué termina el programa;
- leer los prototipos como un índice y agrupar las funciones por trabajo;
- llenar una fila por función con propósito, quién la llama, a quién llama, qué lee y qué escribe;
- separar los globales mutables de las constantes de solo lectura con alcance de archivo;
- trazar un camino desde la entrada hasta la salida a través del estado que cambia;
- registrar un resultado de compilación/ejecución, incluida una compilación que no tiene éxito;
- comprimir toda la lectura en una frase que predice comportamiento;
- escribir una siguiente pregunta que nombre un archivo, una línea o un símbolo.

## El concepto: modelo mental

Un **modelo mental** es un conjunto pequeño de afirmaciones que te permite predecir qué hará un
programa y dónde aterrizará un cambio. No es un resumen del archivo. Un resumen te dice qué existe;
un modelo te dice cómo se relacionan las partes.

Un modelo útil tiene cuatro capas:

| Capa | Pregunta que responde | Qué registra el mapa |
|---|---|---|
| **Frontera** | ¿Dónde empieza y termina el programa? | Archivo de entrada, función de entrada, condición de salida. |
| **Unidades** | ¿Cuáles son las piezas con nombre y qué hacen? | Una fila por función: propósito, llamadores, llamadas. |
| **Estado** | ¿Qué sobrevive entre llamadas? | Globales y estructuras, con quién escribe y quién lee. |
| **Flujo** | ¿Cómo se convierte la entrada en salida? | Entrada, transformación, salida, sitios de mutación. |

Las cuatro capas se leen en ese orden. La frontera es barata y te dice dónde empieza la historia.
Las unidades te dan vocabulario. El estado explica por qué las unidades no se pueden leer aisladas.
El flujo convierte el vocabulario en una frase.

### Un mapa no es un resumen

Compara estas dos afirmaciones sobre el mismo archivo:

- **Resumen:** "El archivo tiene doce funciones y quince nombres con alcance de archivo."
- **Mapa:** "`main` llama a `vecjug` y luego a `impresionorden`; `impresionorden` asigna `cab` y
  llama a `movcab`; `movcab` escribe `vectab` y `ganador`; `main` repite mientras `jugada != 0`."

La primera es inventario. La segunda es un modelo: te permite predecir que un cambio en `cab`
afecta a la función de movimiento, y que el bucle termina cuando algo escribe `jugada = 0`. El
inventario es un subproducto del mapa, no el objetivo.

### La confianza tiene cuatro niveles

Cada afirmación de tu mapa lleva una confianza. Usa estas cuatro etiquetas y no las mezcles:

| Etiqueta | Significado | Ejemplo |
|---|---|---|
| **Leído** | La afirmación se ve en una línea citada. | `ganador = 0` aparece en la línea 257. |
| **Ejecutado** | Lo observaste en una compilación o una ejecución. | La compilación ingenua se detuvo en `conio.h`. |
| **Inferido** | La afirmación se deduce de los sitios de llamada, no de un cuerpo leído. | `main` llega a `movcab` a través de `impresionorden`. |
| **Desconocido** | Todavía no puedes saberlo. | Qué rama se ejecuta primero en tiempo de ejecución. |

La distinción importa porque un modelo construido leyendo y uno construido ejecutando fallan de
formas distintas. Una afirmación leída se comprueba abriendo el archivo. Una afirmación ejecutada
está atada a tu máquina. Una inferencia puede estar mal aunque sus dos extremos sean correctos.

### El orden de lectura vence al volumen

No leas el archivo desde la línea 1 hasta la 930. Tres pasadas son más rápidas y producen un mejor
modelo:

1. **Pasada de frontera.** Encuentra `main`, léelo completo y anota cada llamada que hace.
2. **Pasada de unidades.** Lee los prototipos y la primera y la última línea de cada función. Agrupa
   las funciones por trabajo. Todavía no leas los cuerpos largos.
3. **Pasada de estado y flujo.** Lista los nombres con alcance de archivo y sigue un valor desde la
   entrada hasta la salida.

Los cuerpos largos van al final, y solo las partes que tu mapa no puede explicar. Una función de
491 líneas con ocho casos casi idénticos no son 491 líneas de información nueva; es una idea
repetida ocho veces.

## Antes de empezar

Lee estos archivos completos:

- [`README.md`](README.md) — la ficha de la lección y el criterio de aceptación.
- [`CODE-MAP-TEMPLATE-es.md`](CODE-MAP-TEMPLATE-es.md) — el mapa que vas a llenar.
- [`EVIDENCE-es.md`](EVIDENCE-es.md) — el registro que vas a llenar.
- [`archaeology/legacy/README-es.md`](../../legacy/README-es.md) — dónde vive el código fijado.

También necesitas:

- una copia local del código fijado;
- un compilador de C++ (`c++`, `g++` o `clang++`) para la comprobación ligera de compilación;
- `wc`, `grep` y un editor de texto;
- un directorio temporal fuera del árbol del código.

> **Regla:** el código es de solo lectura. Puedes copiarlo a un directorio temporal para compilarlo.
> No puedes editar el original, renombrar sus archivos ni escribir artefactos de compilación dentro
> de su árbol.

## El recorrido de 45 minutos

| Min | Paso | Qué haces | Evidencia |
|---|---|---|---|
| 0–7 | Predecir | Lees solo las declaraciones; escribes cinco predicciones | Tabla de predicción |
| 7–18 | Inspeccionar | Lees la frontera, los prototipos y las funciones en orden | Notas de lectura |
| 18–30 | Mapear | Llenas una fila por función y una fila por global | `CODE-MAP-TEMPLATE-es.md` |
| 30–36 | Compilar y ejecutar | Intentas la compilación ingenua; registras el resultado o el bloqueo | Registro de compilación |
| 36–42 | Explicar | Escribes la frase de propósito y la pones a prueba | Frase de propósito |
| 42–45 | Registrar | Escribes la siguiente pregunta y llenas `EVIDENCE-es.md` | Entrada del portafolio |

Si el mapa tarda veinte minutos porque `movcab` es grande, eso es información sobre `movcab`, no
sobre ti. El límite de tiempo es una guía para la primera pasada, no un corte estricto.

### La comprobación de ingeniería en esta lección

| Paso | Dónde ocurre | Qué produce |
|---|---|---|
| **Predecir** | Paso 1 | Cinco afirmaciones sobre comportamiento, estado y fallo, por escrito. |
| **Inspeccionar** | Paso 2 | La frontera, los grupos de funciones y los nombres que debes explicar. |
| **Mapear** | Paso 3 | Una fila por función y por global, con etiquetas de confianza. |
| **Explicar** | Paso 5 | Una frase de propósito que predice comportamiento. |
| **Registrar** | Paso 6 | `EVIDENCE-es.md`, el resultado de compilación y la siguiente pregunta. |

## Paso 1 — Predecir antes de leer los cuerpos (7 min)

Mira solo tres cosas:

1. el nombre del archivo y su cantidad de líneas (`wc -l`);
2. la lista de `include` y las declaraciones con alcance de archivo (las primeras 30 líneas);
3. los prototipos, si el archivo los tiene.

No leas todavía ningún cuerpo de función. Las declaraciones alcanzan para una primera conjetura, y
una conjetura hecha antes de los cuerpos es la única que puedes falsar con limpieza.

Responde estas cinco preguntas:

1. **Propósito.** En una línea, ¿qué hace este programa?
2. **Interfaz.** ¿La persona ve una pantalla, un registro que se desplaza o ninguna salida?
3. **Estado.** ¿Qué declaraciones parecen estado mutable y cuáles parecen constantes?
4. **Frontera.** ¿Qué inicia el programa y qué condición lo termina?
5. **Fallo.** ¿Qué esperas que se rompa primero en tu máquina?

Después escribe una frase falsable sobre el conjunto. Por ejemplo:

> Espero un juego de consola que coloca dos caballos en un tablero de 8x8, los mueve en un orden
> aleatorio y termina cuando uno cae sobre el otro; espero que la compilación falle por un
> encabezado de consola ausente antes de ejecutar nada.

Esa frase puede estar mal en al menos tres lugares. Bien. Una predicción que no puedes falsar no
enseña nada.

### Una predicción trabajada a partir de las declaraciones

Toma las primeras treinta líneas:

- `const char simcab1=21, simcab2=156, sep=219;` — tres nombres con alcance de archivo cuyos valores
  parecen códigos de carácter. El `const` dice que son de solo lectura. Son símbolos de impresión,
  no estado.
- `int vectab[64], vecjugadas[8];` — dos arreglos. Un arreglo de 64 celdas junto a uno de 8 sugiere
  un tablero y una lista de movimientos.
- `int cab1, cab2, jugada=1, orden, ganador=1;` — enteros sueltos. Nombres como `cab1` y `cab2`
  sugieren dos piezas; `jugada` se inicializa en 1 y parece un contador; `ganador` empieza en 1 y
  puede ser una bandera.
- Los prototipos nombran su propio trabajo: `inicializacion`, `cabubicacion`, `impresion`, `vecjug`,
  `movcab`, `ordenjuego`, `impresionorden`.

Con eso ya puedes predecir un juego de tablero con dos piezas, una colocación aleatoria, un
generador de movimientos y un dibujante de consola. Todavía no puedes predecir la condición de
victoria ni el orden de los turnos. Escribe ambas como preguntas, no como hechos.

## Paso 2 — Inspeccionar (11 min)

### Leer primero la frontera

Abre `main`. En el código fijado empieza en la línea 32. Lee cada línea antes que cualquier otra
cosa y responde cuatro preguntas:

- ¿Cuál es la primera llamada?
- ¿Cuál es la última llamada?
- ¿Cuál es la condición del bucle?
- ¿Qué puede cambiar la condición del bucle?

El cuerpo es corto:

```cpp
void main()
{
    system("color 1f");
    inicializacion();
    cabubicacion();
    cout << endl;
    getch();
    system("cls");
    while (jugada != 0)
    {
        if (jugada != 0)
        {
            vecjug();
            impresionorden();
        }
    }
}
```

El fragmento está normalizado: el original usa tabulaciones, líneas en blanco y comentarios en
línea.

Del cuerpo salen cuatro hechos de inmediato:

1. El programa es un bucle gobernado por un global, `jugada`.
2. La preparación son dos llamadas: limpiar el tablero y colocar las piezas.
3. La mayor parte de la salida se produce dentro de las llamadas; `main` solo imprime un salto de
   línea.
4. El programa no tiene `return`; termina cuando la condición del bucle se vuelve falsa y el control
   cae al final de `main`.

Ese último hecho es una afirmación de frontera, no un informe de defecto. Regístralo como "termina
al caer al final", con el número de línea.

### Leer los prototipos como índice

Los prototipos de las líneas 19–30 son un índice comprimido del archivo. Agrúpalos por trabajo en
lugar de leerlos en orden alfabético:

| Trabajo | Funciones | Qué te dice el grupo |
|---|---|---|
| Preparación | `inicializacion`, `cabubicacion`, `ordenjuego` | Limpiar estado, colocar piezas, decidir quién empieza. |
| Generación de movimientos | `vecjug`, `imprevecjug` | Construir e imprimir el orden en que se prueban los movimientos. |
| Ejecución de movimientos | `movcab`, `conversionmv`, `conversionvm` | Probar un movimiento, convertir entre índices y pares fila/columna. |
| Salida | `impresion`, `impresion0`, `impresionorden` | Dibujar el tablero, dibujar la captura, orquestar el turno. |

La agrupación es una hipótesis. La corregirás cuando leas los cuerpos. Eso es lo esperado: un índice
puede estar equivocado, siempre que el mapa registre la corrección.

### Leer las funciones en un orden que añade una idea cada vez

Usa este orden. Cada función añade un tipo nuevo de relación, así el mapa crece en lugar de
reiniciarse:

1. **`inicializacion`** — el escritor más simple. Un arreglo global, sin parámetros, un bucle.
2. **`conversionvm` y `conversionmv`** — un par de contraste. Una escribe cuatro globales y no
   devuelve nada; la otra toma dos parámetros, no toca globales y devuelve un valor. El par muestra
   la diferencia entre una función con salidas ocultas y una conversión pura.
3. **`vecjug` y `imprevecjug`** — un escritor y un lector del mismo arreglo. Léelos juntos.
4. **`ordenjuego`** — una función que decide, muta el tablero, incrementa el contador de jugadas e
   imprime. Es el primer lugar donde "una función, varios trabajos" se hace visible.
5. **`impresion` e `impresion0`** — lectores puros del tablero que imprimen. La segunda es la
   variante de captura; lee los bucles y fíjate en la bandera extra que evita dibujar dos veces las
   piezas.
6. **`movcab`** — lee solo el caso 1 y la rama `default`. Después mira la forma de los otros siete
   casos. Repiten la misma guarda, la misma conversión y la misma escritura en el tablero.
7. **`impresionorden`** — el orquestador. Léelo después de `movcab`, porque asigna el selector de
   turno y llama a la función de movimiento.
8. **`main` otra vez** — ahora la condición del bucle tiene un escritor que puedes nombrar.

### Saltar la repetición a propósito

`movcab` va de la línea 234 a la 724. Casi todo eso son ocho casos con la misma estructura:

```text
comprobar el número de caso y la bandera de juego
comprobar el borde del tablero
convertir la celda candidata
comprobar el selector de turno
comprobar la celda de destino
quizá asignar la bandera de ganador
imprimir el número de caso
actualizar fila y columna
escribir el número de jugada en el tablero
break
```

Lee un caso completo. Para los otros siete, confirma que los desplazamientos cambian y la forma no.
Registra "ocho casos repetidos" como una observación estructural, no como ocho hallazgos separados.
La repetición es el tema de una lección posterior; aquí solo te dice dónde no gastar la primera
pasada.

### Marcar lo que no leíste

Al final de la pasada, escribe tres listas:

- **Leído completo:** `main`, las dos conversiones, `vecjug`, `imprevecjug`, `ordenjuego`, un caso
  de `movcab`, `impresionorden`.
- **Leído en parte:** `movcab` (un caso y el `default`), `impresion` e `impresion0` (los bucles).
- **No leído:** los siete casos restantes, el formato exacto de impresión, el comportamiento de las
  secuencias de consola.

Un mapa que dice "no leído" es más confiable que uno que implica cobertura total. La lista de no
leídos también es tu lista corta de siguientes preguntas.

## Paso 3 — Mapear (12 min)

Llena [`CODE-MAP-TEMPLATE-es.md`](CODE-MAP-TEMPLATE-es.md). La plantilla tiene seis secciones; el
trabajo está en las secciones 2 y 3. Muestra tres filas de ejemplo en cada tabla; agrega filas hasta
que cada función y cada nombre con alcance de archivo tenga la suya.

### Una fila por función

Para cada función, llena propósito, quién la llama, a quién llama, qué lee y qué escribe. Usa
nombres, no rangos de líneas, en las columnas de relación; pon los números de línea en las notas.

Filas trabajadas:

| Función | Propósito | Llamada por | Llama a | Lee | Escribe |
|---|---|---|---|---|---|
| `main` | Preparar y luego repetir hasta que el juego termina | sistema operativo | `inicializacion`, `cabubicacion`, `vecjug`, `impresionorden` | `jugada` | nada directamente |
| `conversionmv` | Convertir un par fila/columna en un índice del tablero | `movcab` | ninguna | parámetros `cf`, `cc` | local `ccab` |
| `movcab` | Probar los ocho movimientos en orden; aplicar el primero legal | `impresionorden` | `conversionmv`, `impresion` | `vecjugadas`, `jugada`, `cab`, `vectab`, `cab1`, `cab2` | `cf`, `cc`, `vectab`, `ganador`, `jugada` |

Tres filas alcanzan para mostrar la forma. La primera es la frontera. La segunda es la función pura.
La tercera es la acoplada. Si tu mapa solo tiene filas del primer tipo, describiste el archivo, no
lo modelaste.

### Una fila por global

Para cada nombre con alcance de archivo, registra tipo, quién lo escribe, quién lo lee y por qué
existe. Después separa estado de constantes:

| Nombre | Tipo | Escrito por | Leído por | Por qué existe |
|---|---|---|---|---|
| `vectab` | `int[64]` | `inicializacion`, `ordenjuego`, `movcab` | `movcab`, `impresion`, `impresion0` | Celdas del tablero; guarda números de jugada y posiciones. |
| `jugada` | `int` | `ordenjuego`, `movcab`, `impresionorden` | `main`, `ordenjuego`, `movcab`, `impresionorden` | Contador de jugadas, centinela del bucle y valor de caso especial. |
| `simcab1`, `simcab2`, `sep` | `const char` | nunca | `cabubicacion`, `ordenjuego`, `impresion`, `impresion0`, `impresionorden` | Símbolos de impresión. Solo lectura; no son estado. |

La tercera fila es la comprobación de precisión. Un nombre con alcance de archivo no es
automáticamente estado mutable. Si es `const` y nunca se asigna después de la inicialización,
pertenece al mapa como constante, con una nota que diga que lo excluiste de la tabla de estado.

### Seguir un valor de extremo a extremo

Elige un valor y trázalo. El más barato es `jugada`, porque aparece en la condición del bucle, en
las escrituras del tablero y en la función de movimiento. Escribe el camino como una cadena:

```text
main (44) -> while jugada != 0
  ordenjuego (733, 735) -> jugada++ dos veces durante la colocación
  movcab (269) -> vectab[ccab] = jugada
  movcab (708, 716) -> jugada = 0 cuando no hay movimiento posible
  impresionorden (772, 797) -> jugada = 0 cuando una pieza es capturada
main (44) -> el bucle termina
```

Esa cadena es una afirmación de flujo de datos y es comprobable: cada flecha tiene una línea. Si una
flecha no tiene línea, márcala como **Inferido** y escribe qué leerías para confirmarla.

### La regla para las celdas vacías

Una celda vacía es una pregunta, no un fracaso. Escribe `?` y una de las etiquetas de confianza. Al
final de la pasada, las celdas con `?` son tu lista de siguientes preguntas. Un mapa con tres `?`
honestos es mejor que uno con tres respuestas inventadas.

## Paso 4 — Compilar y ejecutar, sin profundizar (6 min)

Este paso produce un campo del mapa: **resultado de compilación/ejecución**. No es la lección de
compilación. El objetivo es saber si el código corre en tu máquina y registrar el primer hecho
observable en cualquier caso.

Copia el código a un directorio temporal y prueba la compilación ingenua:

```bash
mkdir -p /tmp/legacy-orientation && cd /tmp/legacy-orientation
cp /ruta/al/codigo/guerra-de-caballos-baseline.cpp .
LC_ALL=C c++ -std=c++20 guerra-de-caballos-baseline.cpp -o legacy
```

En una cadena de herramientas moderna se espera que el primer error sea un encabezado ausente:

```text
guerra-de-caballos-baseline.cpp:11:9: fatal error: conio.h: No such file or directory
```

Registra el comando exacto y el primer error exacto. No lo arregles aquí. La compilación adaptada,
los encabezados de compatibilidad y el límite de tiempo pertenecen a `archaeology-01`; esta lección
solo necesita el resultado.

Hay tres resultados aceptables:

| Resultado | Qué registras | Qué significa para el mapa |
|---|---|---|
| **Compila y ejecuta** | Comando de compilación, comando de ejecución, primera línea de salida, estado de salida | La afirmación de frontera queda confirmada por una ejecución. |
| **Compila, no ejecuta** | Comando de compilación, comando de ejecución, primer error o tiempo agotado | La frontera es leída, no observada. |
| **No compila** | Comando de compilación, primer error literal, número de línea | El resultado es un bloqueo; el mapa sigue siendo válido. |

Un bloqueo es un resultado. "No compila" no lo es; el error citado sí. Si la compilación tiene
éxito, ejecútala con un límite de tiempo para que un bucle heredado no cuelgue tu sesión:

```bash
timeout 3s ./legacy < /dev/null > out.txt 2> err.txt; echo "status=$?"
head -1 out.txt
```

Registra `status`, la primera línea de salida y la primera línea de error. Si `status=124`, el
programa no terminó en tres segundos; registra "ejecutó, no terminó", no "está roto".

## Paso 5 — Explicar (6 min)

### Comprobar la predicción contra el mapa (2 min)

La predicción no es decoración. Antes de comprimir el modelo, da un veredicto a cada una de las
cinco predicciones, usando el mapa como evidencia:

| Veredicto | Prueba | Qué registrar |
|---|---|---|
| **Confirmada** | El mapa o el resultado de compilación coincide con la predicción. | La fila o la línea que lo confirma. |
| **Contradicha** | El mapa dice algo distinto y puedes nombrarlo. | Ambas versiones, una al lado de la otra. |
| **Sin resolver** | El mapa todavía no puede decidirlo. | Qué leerías o ejecutarías después. |

Una predicción contradicha es la fila más útil. Marca el lugar donde tu primera lectura estaba
equivocada, que es exactamente donde el modelo aprendió algo.

La frase de propósito es la prueba de compresión de todo el mapa. Si la frase es correcta, puedes
predecir un cambio. Si es vaga, el mapa todavía tiene un hueco.

Usa esta forma:

> Este programa **\_\_\_\_** mediante **\_\_\_\_** y termina cuando **\_\_\_\_**.

Frase trabajada:

> Este programa juega una persecución de dos caballos en un tablero de 8x8 probando los ocho
> movimientos de caballo en un orden que empieza en un movimiento distinto en cada turno, y termina
> cuando un caballo cae sobre el otro o un caballo no tiene movimiento legal.

Pon la frase a prueba de tres maneras:

1. **Predicción.** Si cambio el orden de movimientos, ¿qué cambia? La frase dice que el orden
   empieza en un movimiento distinto en cada turno, así que cambia el camino pero no las condiciones
   de fin.
2. **Compresión.** ¿La frase cubre el punto de entrada, el estado y la salida sin nombrar cada
   función? Debería.
3. **Falsabilidad.** ¿Qué línea probaría que está mal? Para la condición de fin, `jugada = 0` en
   `movcab` y en `impresionorden`. Para la persecución, las escrituras de `ganador = 0`.

Si no puedes responder una de las tres, la frase no está lista. Reescribe la frase, no el mapa.

### Qué no debe incluir la frase

- **Calidad.** "Está mal estructurado" es un juicio, no un propósito.
- **Entorno.** "Apunta a una consola" es contexto, no comportamiento.
- **Intención.** "El programa estaba pensado como un juego" no se puede probar desde el código.
- **Detalle de implementación.** "Usa `switch`" describe el código, no lo que hace el programa.

La frase responde "¿qué es este programa?". Quien la lea debería saber qué esperar cuando el binario
se ejecute.

### Si una frase no alcanza

Escribe una segunda frase, pero mantenla separada:

> No lee entrada significativa: las pulsaciones de tecla son pausas y el azar viene del reloj.

Esa frase registra un hecho de interfaz que la frase de propósito no necesita. Dos frases están
bien; un párrafo es señal de que el mapa todavía está haciendo el trabajo.

## Paso 6 — Registrar (3 min)

Llena [`EVIDENCE-es.md`](EVIDENCE-es.md). Pide la preparación, la predicción, el punto de entrada,
las tablas de funciones y estado, el flujo de datos, el resultado de compilación/ejecución y el
modelo mental.

Dos hábitos:

- **Cita líneas.** "`movcab` escribe `vectab`" es una opinión; "`movcab` escribe `vectab` en 269"
  es una afirmación que puedes comprobar.
- **Etiqueta la confianza.** Leído, ejecutado, inferido, desconocido. Un mapa sin etiquetas esconde
  sus partes más débiles.

### Escribir la siguiente pregunta

La siguiente pregunta es parte del entregable, no un añadido. Una buena pregunta tiene cuatro
propiedades:

1. **Específica.** Nombra un símbolo, un archivo o una línea.
2. **Respondible.** El código puede responderla, o un experimento pequeño puede.
3. **Consecuente.** La respuesta cambia al menos una fila del mapa.
4. **Acotada.** Cabe en una sesión, no en una reescritura.

| Pregunta débil | Por qué es débil | Mejor pregunta |
|---|---|---|
| "¿Cómo funciona el juego?" | Demasiado amplia; el mapa ya la responde. | "¿`movcab` siempre escribe `vectab` antes de retornar?" |
| "¿Este código es bueno?" | Es un juicio, no una pregunta sobre el código. | "¿Qué funciones escriben `jugada` y en qué orden?" |
| "¿Qué significa `cab`?" | Es ambiguo; el nombre no dice si es posición o turno. | "¿Qué función asigna `cab` antes de cada llamada a `movcab`?" |
| "¿Cuál es el bug?" | Presupone un defecto y salta el mapa. | "¿Puede devolverse `ccab` sin inicializar cuando fallan los ocho casos?" |

La última fila es una buena siguiente pregunta porque apunta al camino de retorno de `movcab` y se
puede responder leyendo la rama `default` y la declaración de `ccab`.

## El código en detalle

Hechos que citarás con frecuencia, con números de línea del código fijado:

| Línea | Qué hay | Por qué importa para el mapa |
|---|---|---|
| 16 | `const char simcab1=21, simcab2=156, sep=219;` | Símbolos de impresión de solo lectura, no estado. |
| 17 | `int vectab[64], vecjugadas[8];` | Tablero y arreglo de orden de movimientos. |
| 18 | `int cab1, cab2, jugada=1, orden, ganador=1;` | Posiciones, contador, orden de turno, bandera de ganador. |
| 29 | `int cf1, cc1, cf2, cc2, cab;` | Caché fila/columna y selector de turno. |
| 32 | `void main()` | Punto de entrada; tipo de retorno no estándar. |
| 38 | `system("color 1f")` | Configuración de consola; dependencia del entorno. |
| 42, 43 | `getch()`, `system("cls")` | Pausas y limpieza de pantalla; la entrada no son datos. |
| 44 | `while (jugada != 0)` | La condición del bucle es un solo global. |
| 61 | `for (i=0;i<=64;i++)` | El bucle de limpieza escribe uno más allá del arreglo; anota, no persigas. |
| 76 | `srand(time(NULL))` | Azar sembrado desde el reloj dentro del bucle de colocación. |
| 89 | `orden=ordenjuego(...)` | Un valor de retorno asignado a un global. |
| 104–112 | `conversionvm` | Escribe cuatro globales, no devuelve nada. |
| 176–205 | `vecjug` | Construye el orden de ocho movimientos desde un inicio aleatorio. |
| 226–232 | `conversionmv` | Conversión pura: dos parámetros, un retorno, sin globales. |
| 234–724 | `movcab` | Ocho casos de movimiento repetidos; lee uno y el `default`. |
| 726–751 | `ordenjuego` | Escribe dos celdas, incrementa `jugada`, imprime. |
| 753–868 | `impresionorden` | Asigna `cab`, llama a `movcab`, maneja la captura, imprime. |
| 870–930 | `impresion` | Lee el tablero y ambas posiciones; imprime. |

Los números de línea son anclas, no un sustituto de la lectura. Si tu copia difiere en algunas
líneas, registra tus propios números; lo que estás mapeando es la estructura.

## Qué prueba el mapa y qué no prueba

| Afirmación | ¿Sostenida? | Por qué |
|---|---|---|
| Una función llama a otra en la línea citada. | Sí | La llamada se ve en el cuerpo. |
| Un global se escribe en la línea citada. | Sí | La asignación se ve. |
| Una función lee un global que no recibe. | Sí | La lectura se ve en el cuerpo. |
| El orden de llamadas en ejecución es el del mapa. | Solo como lectura | El mapa es estático; no se traza una ejecución. |
| El valor que una variable contiene en un momento dado. | Inferido | La declaración da el tipo; el valor necesita una ejecución o una traza. |
| El propósito del programa es la frase que escribiste. | Inferido | La frase es un modelo, no una prueba. |
| El resultado de compilación vale en otra máquina. | No | Está atado a tu compilador y tu entorno. |
| El código es correcto o incorrecto. | No | La corrección necesita una especificación y una prueba. |

Esta tabla es la comprobación de honestidad. Un mapa que afirma haber trazado una ejecución está
exagerando; escribe "leído del código" cuando eso es lo que hiciste.

## Autocomprobación (binaria)

- [ ] Escribí cinco predicciones antes de leer cualquier cuerpo de función.
- [ ] Leí `main` completo y nombré qué inicia y qué termina el programa.
- [ ] Agrupé los prototipos por trabajo y corregí la agrupación después de leer.
- [ ] Leí un caso de `movcab` y la rama `default`, y registré la repetición.
- [ ] Cada función tiene una fila con propósito, llamadores, llamadas, lecturas y escrituras.
- [ ] Cada global mutable tiene una fila con tipo, escritores, lectores y razón de existir.
- [ ] Separé los símbolos `const char` del estado mutable.
- [ ] Trazé un valor desde la entrada hasta la salida con una línea por flecha.
- [ ] Intenté la compilación y registré el comando y el primer resultado observable.
- [ ] Escribí la frase de propósito y la probé de tres maneras.
- [ ] Escribí una siguiente pregunta que nombra un símbolo o una línea.
- [ ] Etiqueté cada afirmación incierta como inferida o desconocida.
- [ ] El código está sin modificar y no se escribió ningún artefacto en su árbol.

Si una casilla no está marcada, la casilla sin marcar nombra el siguiente paso. No hay penalización
por repetir uno.

## Ideas erróneas que esta lección refuta

| Idea errónea | Por qué está mal | Qué hacer en su lugar |
|---|---|---|
| "Hay que leer el archivo de arriba abajo." | Un archivo largo es sobre todo repetición; la lectura lineal gasta el tiempo en el cuerpo más grande primero. | Leer la frontera, luego las unidades, luego el flujo. |
| "El mapa es un resumen del archivo." | Un resumen lista lo que existe; un mapa afirma relaciones. | Escribir llamadores, llamadas, escritores y lectores. |
| "Todo nombre con alcance de archivo es estado." | `simcab1`, `simcab2` y `sep` son `const char`. | Separar constantes de estado mutable. |
| "La firma de una función es su contrato." | `movcab(int&, int&)` además lee seis globales y escribe tres. | Leer el cuerpo y registrar lecturas y escrituras ocultas. |
| "Si no entiendo una función, debo leerla otra vez." | Un hueco nombrado es progreso; releer sin pregunta repite la misma pasada. | Escribir la pregunta y marcar la fila con `?`. |
| "La compilación debe tener éxito para que la lección cuente." | Un bloqueo citado es un resultado; el mapa no depende de un binario. | Registrar comando, error y número de línea. |
| "Una frase es demasiado corta para ser útil." | La frase es una prueba de compresión; si es vaga, el mapa tiene un hueco. | Reescribir la frase hasta que prediga un cambio. |
| "La siguiente pregunta debe ser el problema abierto más grande." | Una pregunta grande no se responde en una sesión. | Elegir la pregunta más pequeña que cambie el mapa. |
| "Un mapa estático es más débil que un depurador." | Ambos son evidencia; el mapa es lo que una revisión puede leer sin ejecutar. | Decir qué afirmaciones son leídas y cuáles ejecutadas. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Modelo mental | Un conjunto pequeño de afirmaciones que predice comportamiento y localiza cambios. |
| Frontera | El punto de entrada y la condición que termina el programa. |
| Unidad | Una función con nombre o un grupo de funciones con un trabajo. |
| Estado | Un valor que sobrevive entre llamadas y puede cambiar. |
| Flujo de datos | El camino desde la entrada hasta la salida pasando por la transformación. |
| Sitio de mutación | Una línea donde cambia el estado. |
| Llamador | Una función que llama a otra. |
| Llamada | Una función que es llamada por otra. |
| Afirmación leída | Una afirmación visible en una línea citada. |
| Afirmación ejecutada | Una afirmación observada en una compilación o ejecución. |
| Inferencia | Una afirmación que se deduce de los sitios de llamada sin leerlos directamente. |
| Límite de la evidencia | La línea entre lo que el código prueba y lo que supones. |

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
