# archaeology-17 — Revisión de código de una función heredada

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** revisión de código · **Prerrequisito:** archaeology-04 ·
> **Tiempo estimado:** 60 minutos
> **Lectura:** ~20 minutos

## Por qué existe esta lección

`archaeology-04` te dio un catálogo: seis fallos, cada uno con una ficha, una línea y una prueba. La
revisión real no funciona así. Nadie te entrega la lista de defectos. Recibes una función y un
llamador, y tienes que decidir qué vale la pena decir.

Esa decisión tiene tres partes, y la mayoría de las revisiones solo hace la primera:

1. **Observación** — qué hace el código. "La línea 300 escribe `a`."
2. **Hallazgo** — por qué importa, a quién y cuánto. "La función que responde '¿ganó alguien?'
   también termina la partida, así que ningún llamador puede hacer la pregunta sin causar el
   efecto."
3. **Prioridad** — qué arreglar primero, según consecuencia y coste.

Una revisión que se detiene en la observación es un resumen. Una revisión que salta la prioridad es
una lista de deseos. Una revisión que salta la evidencia es una opinión con formato.

Esta lección es el protocolo del medio: una función, al menos tres hallazgos, un orden de severidad
y una prueba que detectaría el hallazgo de mayor riesgo. La función es la unidad porque una revisión
de archivo se convierte en una propuesta de reescritura, y una propuesta de reescritura no es una
revisión.

## Qué podrás hacer

Al terminar esta lección puedes:

- elegir una función que valga la pena revisar y acotar el alcance por escrito;
- separar una observación, un hallazgo y una preferencia;
- escribir un hallazgo con ubicación, síntoma, causa raíz, severidad y evidencia;
- asignar severidad por consecuencia, no por lo desagradable que se vea el código;
- ordenar los hallazgos con una regla que puedas enunciar, no por intuición;
- nombrar una prueba que falla antes del arreglo y pasa después;
- explicar la costura mínima que la prueba necesita, incluso cuando la función actual no se puede
  llamar;
- declarar qué leíste, qué ejecutaste y qué inferiste;
- mantener la fuente heredada de solo lectura mientras trabajas.

La lección está hecha cuando otra persona revisora puede leer tu informe y actuar sin preguntarte
qué hallazgo importa.

## El concepto: una revisión es un conjunto ordenado de afirmaciones

### Tres artefactos que se parecen

Una revisión produce frases, y no toda frase es un hallazgo.

| Artefacto | Ejemplo | Qué le falta |
|---|---|---|
| Observación | "La línea 300 asigna `a = 0`." | Consecuencia. Es verdadera e inerte. |
| Preferencia | "Yo devolvería un enum en lugar de imprimir." | Evidencia de daño. Aun así puede ser correcta. |
| Hallazgo | "El predicado termina la partida como efecto secundario, así que un llamador no puede hacer la pregunta dos veces." | Nada, si la evidencia sostiene la afirmación. |

La preferencia no está prohibida. Es otro tipo de afirmación y va en una sección aparte. Mezclar
preferencias con hallazgos es cómo una revisión pierde autoridad: quien lee no puede distinguir qué
comentarios respalda el código y cuáles son gusto.

La prueba es directa. Pregunta: **¿qué pasa si nadie actúa sobre esto?** Si la respuesta es "el
código se ve distinto a mi estilo", es una preferencia. Si la respuesta es "un resultado es
incorrecto", "un llamador se rompe" o "el próximo cambio cuesta tres archivos", es un hallazgo.

### Anatomía de un hallazgo

Cada hallazgo de esta lección tiene seis campos. La plantilla tiene una fila por hallazgo, y la fila
es la unidad de revisión.

| Campo | Qué pregunta responde | Débil | Fuerte |
|---|---|---|---|
| Ubicación | ¿Dónde está la afirmación? | "En el código de victoria." | `pruebatriunfo`, líneas 296–299. |
| Síntoma | ¿Qué se puede observar? | "Es confuso." | El mensaje nombra a la máquina en cada victoria. |
| Causa raíz | ¿Por qué ocurre? | "Mal diseño." | El ganador es un literal; no se deriva del lado que movió. |
| Severidad | ¿Qué pasa si nadie lo arregla? | "Alta, es feo." | Alta: el resultado informado es incorrecto en cada final. |
| Evidencia | ¿Cómo compruebo la afirmación? | "Todo el mundo lo sabe." | Cuatro comparaciones `!= -1` en 296–297, mensaje en 299. |
| Prueba | ¿Qué lo detectaría? | "Escribe pruebas." | `outcome_reports_capturing_side`. |

La columna débil no es un hombre de paja. Es lo que parece una revisión cuando quien revisa tiene
prisa, y es la razón por la que la persona que mantiene el código no puede actuar sobre ella.

### La severidad es consecuencia, no fealdad

Usa cuatro niveles. Son los que ya usa la ruta de arqueología, y tratan sobre lo que pasa, no sobre
el estilo.

| Nivel | Significado | Acción |
|---|---|---|
| **Crítica** | Comportamiento indefinido, pérdida de datos, seguridad, bloqueo legal. | Arreglar antes de publicar. |
| **Alta** | Comportamiento incorrecto, resultados no reproducibles, un incidente difícil. | Arreglar en el parche mínimo. |
| **Media** | Un cambio caro o una estructura frágil. | Arreglo estructural, planificado. |
| **Baja** | Legibilidad, nombres, comentarios. | Mejorar al tocar el archivo. |

La disciplina es una frase por hallazgo:

> Si esto se dispara, **\<observable\>** le pasa a **\<quién\>**, en **\<contexto\>**.

Si no puedes llenar los huecos, no asignaste una severidad; asignaste un estado de ánimo. "Alta" no
es sinónimo de "no me gusta". Un error de etiqueta de una línea puede ser alto; un problema de
estilo de 200 líneas puede ser bajo. El coste y la dificultad tampoco son severidad. Un arreglo
barato y uno caro pueden tener la misma severidad, y el barato suele hacerse primero.

La severidad también es **contextual**. La misma etiqueta incorrecta es alta en un juego que informa
un ganador y baja en una demo interna que nadie lee. El informe debe nombrar el contexto que
precifica. Una severidad sin contexto es un número sin unidad.

### La prioridad no es la severidad

La severidad dice cuán mala es la consecuencia. La prioridad dice qué hacer primero. Normalmente
coinciden, y cuando no coinciden la razón debe quedar escrita.

Ordena con tres reglas, aplicadas en orden:

1. **Severidad primero.** Crítica antes que alta, alta antes que media, media antes que baja.
2. **Alcanzabilidad después.** Dentro de un nivel, el hallazgo del camino normal va antes que el
   del camino excepcional.
3. **Coste y certeza al final.** Dentro de un nivel y un camino, el arreglo barato y seguro va antes
   que el caro e incierto.

Después divide la lista en tres acciones:

- **Arreglar primero** — el hallazgo de arriba; aquel cuya prueba nombras.
- **Arreglar después** — real, pero no en el mismo cambio.
- **Dejar como evidencia** — registrado, no arreglado; la razón forma parte de la revisión.

El último grupo no es un cajón de sastre. Es donde va un hallazgo cuando el arreglo cambiaría un
comportamiento del que depende el producto, o cuando la evidencia no basta para justificar el
cambio.

### Una prueba es una afirmación que se puede comprobar

Un hallazgo dice "esto está mal". Una prueba dice "así sabrías que lo está". La segunda frase es lo
que separa una revisión de una queja.

Una prueba nombrada necesita cinco partes:

| Parte | Pregunta |
|---|---|
| Nombre | ¿Qué comportamiento protege? |
| Dado | ¿Qué estado se prepara? |
| Cuando | ¿Qué acción única se ejecuta? |
| Entonces | ¿Qué observable debe cumplirse? |
| Oráculo | ¿Por qué esa es la respuesta correcta, con independencia de la implementación? |

Además importan dos propiedades:

- **Falla antes del arreglo.** Si la prueba pasa con el código actual, no detecta el hallazgo.
- **Pasa después del arreglo.** Si no puede pasar sin una reescritura, el "arreglo mínimo" no era
  mínimo.

Si la función actual devuelve `void` e imprime, puede que no puedas escribir la prueba en absoluto.
Eso no es razón para omitirla. Es un **hallazgo de capacidad de prueba**: la función no tiene una
costura donde un llamador pueda observar el resultado. Nombra la costura mínima que la prueba
necesita. En esta lección nombras la prueba; no tienes que implementarla.

### La escalera de evidencia

No toda afirmación llega al mismo peldaño. Di a qué peldaño llegó cada hallazgo.

| Peldaño | Qué hiciste | Qué demuestra |
|---|---|---|
| 1. Leer | Leíste la función y sus llamadores. | El código dice lo que citas. |
| 2. Grep | Encontraste cada uso de la función, el global o el centinela. | La afirmación no es local a una línea. |
| 3. Compilar | Compilaste el archivo o una copia. | El compilador acepta el código; las advertencias son visibles. |
| 4. Ejecutar | Ejecutaste el programa con una entrada conocida. | El comportamiento ocurre al menos una vez. |
| 5. Probar | Ejecutaste una prueba que falla con el defecto. | La afirmación es reproducible y falsable. |

Una revisión que dice "esto es un error" desde el peldaño 1 no está equivocada, pero debe decir
peldaño 1. Una revisión que dice "esto se cae" desde el peldaño 1 está exagerando. La diferencia no
es pedantería; es lo que permite que la siguiente persona reproduzca tu trabajo.

### Límites de la revisión

Toda revisión tiene un borde, y el borde forma parte del entregable.

- **Qué leí** — archivo, función, líneas, llamadores.
- **Qué ejecuté** — comandos, entradas, códigos de salida.
- **Qué no ejecuté** — y por qué.
- **Qué inferí** — las afirmaciones que dependen de un modelo del programa, no de una ejecución.

Una revisión sin límites se lee como completa. Una revisión con límites es confiable, porque quien
lee sabe dónde mirar después.

## Antes de empezar

Lee esto completo:

- [`README-es.md`](README-es.md) — la ficha de la lección y el criterio de aceptación.
- [`CODE-REVIEW-TEMPLATE-es.md`](CODE-REVIEW-TEMPLATE-es.md) — el informe que completas.
- [archaeology-04](../archaeology-04-correctness-failures/lesson-es.md) — el prerrequisito: cómo se
  nombra, evidencia y prueba un defecto.
- [`../../../docs/guide/archaeology-track-es.md`](../../../docs/guide/archaeology-track-es.md) — §5,
  la escala de severidad; §2, el protocolo de autopsia.
- [`../../../docs/guide/lesson-catalog-es.md`](../../../docs/guide/lesson-catalog-es.md) — la
  secuencia de la ruta B.

También necesitas:

- un clon local de la fuente heredada, de solo lectura;
- `nl`, `grep`, `sed` y `wc`;
- opcionalmente un compilador de C++, para las comprobaciones del peldaño 3;
- un directorio de trabajo fuera de `original/`, para cualquier copia que hagas.

> **Regla:** la fuente heredada es de solo lectura. Copia cualquier archivo que quieras cambiar a un
> directorio de trabajo; nunca edites `original/`. Esta lección no parchea el código. La prueba se
> nombra, no se aplica.

## La ruta de 60 minutos

| Min | Paso | Qué haces | Evidencia |
|---:|---|---|---|
| 0–10 | Inspeccionar | Eliges una función, acotas el alcance, la lees tres veces | Ficha de alcance |
| 10–25 | Revisar | Escribes al menos tres hallazgos distintos con severidad | Tabla de hallazgos |
| 25–35 | Priorizar | Ordenas por severidad, alcanzabilidad y coste | Arreglar primero / después / dejar |
| 35–45 | Probar | Nombras una prueba para el hallazgo de mayor riesgo | Contrato de prueba |
| 45–55 | Explicar | Escribes el comentario de revisión y los límites | Comentario y límites |
| 55–60 | Registrar | Completas la plantilla de revisión y `EVIDENCE-es.md`; confirmas que la fuente está limpia | Entrada de portafolio |

Si el diseño de la prueba tarda quince minutos porque la función no tiene costura, esa investigación
es la lección. No la acortes para cumplir el horario.

### La comprobación de ingeniería en esta lección

La ruta B usa el mismo hábito que la ruta A, con otro verbo: el comportamiento ya existe, así que
**inspeccionas** lo que hace antes de juzgarlo.

| Paso | Dónde ocurre | Qué produce |
|---|---|---|
| **Inspeccionar** | Paso 1 | Una función, un rango de líneas, una ficha de alcance. |
| **Revisar** | Paso 2 | Al menos tres hallazgos, cada uno con evidencia y severidad. |
| **Priorizar** | Paso 3 | Un orden con una regla enunciada. |
| **Probar** | Paso 4 | Una prueba nombrada para el hallazgo de mayor riesgo. |
| **Explicar** | Paso 5 | El comentario de revisión, los no-hallazgos y los límites. |
| **Registrar** | Paso 6 | `EVIDENCE-es.md` con comandos y fronteras. |

## Paso 1 — Inspeccionar (10 min)

### Elegir la función

Una función vale la pena revisarla cuando tiene un contrato, toca estado y se puede alcanzar. Usa
estos cinco filtros:

| Filtro | Por qué importa |
|---|---|
| Una función nombrada | Una revisión de archivo se convierte en una propuesta de reescritura. |
| Aproximadamente 5–60 líneas | Suficientemente pequeña para tenerla en la cabeza; suficiente para tener contrato. |
| Al menos una lectura o escritura de un global | El estado es donde vive el acoplamiento oculto. |
| Al menos una rama o un bucle | Una línea recta tiene menos formas de estar mal. |
| Alcanzable desde `main`, directa o mediante un llamador | Una función inalcanzable es una pieza de museo. |

La longitud no es el riesgo. El ejemplo guiado tiene cinco líneas y carga un resultado incorrecto,
una escritura de estado oculta y cuatro globales posicionales repetidos. Una función de 60 líneas
con un contrato claro puede ser más limpia que una de cinco líneas con tres trabajos.

Si ya tienes una función en mente, úsala. Si quieres un camino guiado, usa el ejemplo:
`pruebatriunfo()` en
`original/variant-00-precursor-2007/source/ajedrez-caballo-2007.txt`, líneas 296–300.

### Acotar el alcance

Escribe la ficha de alcance antes de escribir un hallazgo. Evita que la revisión crezca hasta el
archivo entero.

| Campo | Ejemplo guiado |
|---|---|
| Archivo | `original/variant-00-precursor-2007/source/ajedrez-caballo-2007.txt` |
| Función | `pruebatriunfo()` |
| Líneas revisadas | 296–300 |
| Llamadores | El bucle de turnos; encuéntralos con `grep -n 'pruebatriunfo'`. |
| Globales leídos | `tablero`, `peon1f`, `peon1c`, `peon2f`, `peon2c`, `peon3f`, `peon3c`, `peon4f`, `peon4c`. |
| Globales escritos | `a` (línea 300). |
| Salidas | Un mensaje en `cout` (línea 299). |
| Fuera de alcance | El bucle de turnos, la representación del tablero, el resto del archivo. |

La ficha de alcance no es papeleo. Es la respuesta a "¿por qué no mencionaste las otras 900
líneas?". La respuesta es "porque están fuera de alcance, y aquí está la línea donde termina el
alcance".

### Leerla tres veces

Lee las mismas cinco líneas tres veces, con una pregunta distinta cada vez.

**Pasada 1 — contrato.** ¿Qué promete el nombre? `pruebatriunfo` promete una prueba: responde si
alguien ganó. Una prueba no debería imprimir ni terminar la partida. Guarda esa idea.

**Pasada 2 — estado.** ¿Qué lee? Nueve globales: el tablero y cuatro pares de coordenadas. ¿Qué
escribe? `a`. La escritura no está en el nombre ni en el tipo de retorno, porque no hay tipo de
retorno.

**Pasada 3 — flujo de control.** Hay un `if`, cuatro comprobaciones negadas de centinela, un mensaje
y una asignación. Sin bucle, sin retorno temprano, sin camino de error. La condición es verdadera
cuando las cuatro celdas de peón ya no son `-1`.

Ahora lee los llamadores. La pregunta no es "¿esta función es buena?" sino "¿quién depende del
efecto secundario?":

```bash
grep -n 'pruebatriunfo' original/variant-00-precursor-2007/source/ajedrez-caballo-2007.txt
```

La respuesta te dice si `a = 0` es la única forma de terminar la partida. Si lo es, el efecto
secundario sostiene el programa y un hallazgo que diga "quítalo" está mal. Un hallazgo que diga
"nómbralo" todavía puede ser correcto.

### El código bajo revisión

```cpp
if( tablero[peon1f][peon1c]!=-1&&tablero[peon2f][peon2c]!=-1
&&tablero[peon3f][peon3c]!=-1&&tablero[peon4f][peon4c]!=-1 )
{
    cout<<"\n\nTRIUNFO PARA LA MIQUINA :D\n\n";   // line 299
    a=0;
}
```

Las líneas 296–297 contienen la condición, la 299 el mensaje, la 300 la asignación. Cita las líneas
en el informe; no las parafrasees. Una revisión que parafrasea no se puede comprobar.

### Qué no hace la inspección

La inspección no asigna severidad, no propone un arreglo y no decide prioridad. Esos son los tres
pasos siguientes. Mantenerlos separados es lo que impide que la primera impresión se convierta en
toda la revisión.

## Paso 2 — Revisar (15 min)

### Seis sondas

Haz las mismas seis preguntas contra la función. Están ordenadas del contrato a la capacidad de
prueba, y cada una produce como máximo un hallazgo.

| Sonda | Pregunta | Qué detecta |
|---|---|---|
| Contrato | ¿La función hace lo que promete su nombre? | Una consulta que también ordena. |
| Frontera | ¿Está comprobado cada índice, longitud y centinela? | Off-by-one, desbordamiento, validación ausente. |
| Estado | ¿Qué globales lee y escribe? | Acoplamiento oculto, dependencia del orden. |
| Camino de fallo | ¿Qué pasa con la entrada mala o el caso vacío? | Éxito silencioso, valor por defecto incorrecto. |
| Acoplamiento | ¿Qué debe cambiar cuando cambia una regla? | Literales duplicados, globales posicionales. |
| Capacidad de prueba | ¿Puede un llamador observar el resultado sin la consola? | Sin costura, sin oráculo, sin prueba. |

Una sonda puede producir más de un hallazgo, pero solo si las consecuencias son distintas. La
sección siguiente es la regla para eso.

### Escribir hallazgos, no un recorrido

El mínimo es tres hallazgos. El máximo es seis, y el máximo no es una meta. Una revisión con tres
hallazgos distintos es más fuerte que una con diez solapados.

**Regla de distinción:** dos filas son un hallazgo cuando comparten causa raíz y consecuencia.
Sepáralas solo cuando actuar sobre una no actúe sobre la otra.

El ejemplo guiado tiene tres hallazgos:

| ID | Ubicación | Síntoma | Causa raíz | Severidad | Evidencia | Prueba que lo detectaría |
|---|---|---|---|---|---|---|
| F-01 | 296–299 | El mensaje nombra a la máquina en cada victoria, sea quien sea que capturó el último peón. | El ganador es una cadena literal; no se deriva del lado que movió. | Alta | Cuatro comprobaciones `!= -1` en 296–297; mensaje fijo en 299. | `outcome_reports_capturing_side` |
| F-02 | 300 | Preguntar "¿ganó alguien?" también asigna `a = 0` y termina la partida. | La consulta y la orden están fusionadas; la función no devuelve nada. | Media | Asignación en 300; `a` controla el bucle de turnos. | No nombrada; misma costura que F-01. |
| F-03 | 296–297 | Cuatro posiciones de peón se comparan a mano contra el literal `-1`. | El centinela del tablero y el conjunto de peones son globales posicionales, no un predicado nombrado. | Media | Cuatro pares de coordenadas y cuatro comparaciones `!= -1`. | No nombrada; estructural. |

**F-01** es el hallazgo de mayor riesgo. Se dispara en cada final de partida y produce un resultado
incorrecto que lee una persona. Es alta, no crítica: no se corrompe memoria ni se pierden datos. El
arreglo es una decisión de etiqueta, y la prueba es barata.

**F-02** es real pero contextual. Hoy el llamador del alcance quiere que la partida termine, así
que el efecto secundario sostiene el programa. Mañana, un segundo llamador que solo quiera hacer la
pregunta terminará la partida por accidente. Eso es una estructura frágil, que es media. Si la
función formara parte de una biblioteca, la misma fila sería alta; el informe debe decir qué
contexto precifica.

**F-03** es el hallazgo de coste de cambio. Añadir un quinto peón significa editar la condición, y
el significado de `-1` no está declarado en ninguna parte. Es media: no está mal hoy, es caro la
próxima vez.

F-02 y F-03 no reciben una prueba nombrada en esta revisión, y el informe dice por qué. La
consecuencia de F-02 es un efecto secundario sobre un llamador; la prueba sería
`outcome_has_no_side_effects`, y se vuelve escribible con la misma costura que F-01. F-03 es
estructural: su comprobación es un conteo de archivos tocados, no una prueba de ejecución. La
lección exige una prueba para el hallazgo de mayor riesgo; no exige una por fila. Una fila sin
prueba es más débil, y decirlo forma parte de la revisión.

### El hallazgo que no es un hallazgo

El mensaje está en mayúsculas, termina con una cara sonriente y dice `MIQUINA`. A quien revisa
pueden no gustarle las tres cosas. Ninguna es un hallazgo en esta lección, porque ninguna cambia un
resultado, rompe un llamador ni encarece un cambio. Son preferencias, y van en "Qué no cambiaría" o
en una nota de estilo aparte.

El `cout` de la línea 299 es otro caso. Es el mecanismo por el que F-01 se hace visible, así que
pertenece a la evidencia de F-01. Si listas "imprimir dentro de un predicado" como hallazgo
separado, debes nombrar una consecuencia que F-01 no cubra ya, como "el resultado no se puede
observar sin capturar stdout". Si no, dividiste una causa raíz en dos filas y quien lee pagará la
duplicación.

### Comprobar cada hallazgo contra la escalera

Escribe el peldaño junto al hallazgo.

| Hallazgo | Peldaño alcanzado | Qué significa |
|---|---|---|
| F-01 | 1 (leer) y 2 (grep) | La condición y la etiqueta son visibles; el mensaje es el único escritor del resultado. |
| F-02 | 1 y 2 | La asignación es visible; la búsqueda de llamadores muestra quién depende de ella. |
| F-03 | 1 | Los cuatro pares y el centinela son visibles en un rango de líneas. |

Ninguno de los tres llegó al peldaño 4. Es honesto, y basta para una revisión de este tamaño. Si
compilas una copia y ejecutas una partida hasta el final, puedes subir F-01 al peldaño 4. Dilo si lo
haces.

### Un hallazgo que se puede enviar

La fila del informe es para el registro. El comentario que enviarías es más corto y tiene una forma
fija:

> En `pruebatriunfo`, líneas 296–299, cuando se capturan los cuatro peones, el mensaje acredita a
> la máquina. Esperaba que el ganador fuera el lado que hizo la última captura, porque la función es
> el único lugar que decide el resultado. Severidad: alta. Prueba:
> `outcome_reports_capturing_side`.

Eso es un hallazgo, una afirmación, una consecuencia y una prueba. No dice "código malo" y no
menciona a la persona que lo escribió. Dice qué pasa y qué lo demostraría.

## Paso 3 — Priorizar (10 min)

### Aplicar las tres reglas

| Orden | Hallazgo | Regla que decide | Por qué |
|---:|---|---|---|
| 1 | F-01 | Severidad | Alta vence a media; además está en el camino normal. |
| 2 | F-02 | Severidad, luego alcanzabilidad | Media; se dispara en el mismo camino, pero solo importa cuando existe un segundo llamador. |
| 3 | F-03 | Severidad, luego coste | Media; estructural, planificado, no va en el parche mínimo. |

El orden no es igual a la columna de severidad, y eso es lo importante. La severidad fija las
bandas; la alcanzabilidad y el coste ordenan las filas dentro de una banda.

### Escribir las tres acciones

- **Arreglar primero:** F-01. Cambia lo que lee el jugador al final de cada partida. El arreglo
  mínimo es derivar la etiqueta del lado que movió, no reescribir la función.
- **Arreglar después:** F-02. Devolver el resultado y dejar que el llamador detenga la partida. Es
  un cambio de costura, y hace posible la prueba del paso 4.
- **Dejar como evidencia:** F-03. Registrar los cuatro globales posicionales y el centinela sin
  nombre. No reemplaces el tablero en esta revisión; es un cambio estructural con su propio precio.

### El contraorden

Si esta función fuera llamada por un segundo componente que solo pregunta "¿terminó la partida?",
F-02 se volvería alta y pasaría primero. La etiqueta incorrecta seguiría estando mal, pero la
detención accidental de la partida rompería a un llamador que no tiene nada que ver con ganar.

Escribe el contraorden. Demuestra que tu prioridad es una decisión con razones, no una
transcripción de la columna de severidad.

### Qué no es la prioridad

La prioridad no es "lo más fácil". El arreglo más fácil puede ser el menos importante. Tampoco es
"lo que más me molesta". Si el orden de la revisión no se puede defender con una frase por fila, el
orden es una preferencia disfrazada de número.

## Paso 4 — Probar (10 min)

### Nombrar la prueba antes de diseñar el arreglo

El hallazgo de mayor riesgo es F-01. Su prueba es:

```text
outcome_reports_capturing_side
```

El nombre declara el comportamiento, no la implementación. No menciona `pruebatriunfo`, `cout` ni la
cadena del mensaje, así que la prueba sobrevive a un renombrado y a una refactorización.

### Completar el contrato de prueba

| Parte | Contenido |
|---|---|
| Nombre | `outcome_reports_capturing_side` |
| Dado | Un tablero donde las cuatro celdas de peón difieren de `-1`, y un último jugador registrado. |
| Cuando | El resultado se evalúa una vez con el jugador como último en mover, y otra con la máquina. |
| Entonces | El primer resultado es victoria del jugador; el segundo es victoria de la máquina. |
| Oráculo | El ganador es el lado que hizo la última captura, no una etiqueta fija. |
| Falla antes | La primera aserción falla: ambos casos de victoria imprimen la etiqueta de la máquina. |
| Pasa después | La etiqueta se deriva del último jugador. |
| Ejecución | Sin consola, sin espera, sin semilla aleatoria; dos casos de victoria y un control. |

El oráculo es la parte que quien revisa omite. "El resultado es correcto" no es un oráculo; es una
esperanza. Aquí el oráculo es una regla del juego, enunciada con independencia del código: el lado
que captura el último peón gana. La prueba codifica la regla, no la salida actual.

### La costura que la prueba necesita

La firma actual no puede expresar la prueba. La función devuelve `void`, imprime en `cout` y escribe
un global. Un llamador no puede pedir el resultado sin causar el efecto.

La costura mínima es un predicado que devuelve un valor:

```cpp
enum class Outcome { Playing, PlayerWins, MachineWins };
Outcome evaluate_outcome(const Board& board, Side last_mover);
```

Eso es un boceto, no un parche. La revisión no lo aplica. El punto es mostrar que la prueba se puede
escribir y precificar la costura: un enum, una firma de función, una actualización del llamador.

Si la costura parece una reescritura, el hallazgo es más grande de lo que parecía. Dilo. Una prueba
que exige un rediseño es evidencia de que F-02 no es media en tu contexto.

### El boceto de la prueba

```cpp
// test: outcome_reports_capturing_side
CHECK(evaluate_outcome(board_with_all_pawns_captured(), Side::Player)
      == Outcome::PlayerWins);
CHECK(evaluate_outcome(board_with_all_pawns_captured(), Side::Machine)
      == Outcome::MachineWins);
CHECK(evaluate_outcome(board_with_pawns_present(), Side::Player)
      == Outcome::Playing);
```

Tres aserciones, un comportamiento. La tercera es el control: demuestra que la prueba no informa una
victoria para cualquier tablero. Una prueba sin caso negativo puede pasar por la razón equivocada.

### Por qué esta prueba detecta F-01

Con la lógica actual, los dos casos de victoria producen la etiqueta de la máquina. La primera
aserción falla. Después de derivar la etiqueta de `last_mover`, la primera pasa y la segunda sigue
pasando. La prueba está roja antes y verde después, que es la única secuencia que demuestra que
prueba el hallazgo.

Si no puedes ejecutar la prueba en esta lección, dilo. Nombrarla es el criterio de aceptación;
ejecutarla es trabajo de la lección siguiente. No afirmes una ejecución verde que no tuviste.

## Paso 5 — Explicar (10 min)

### La revisión es una conversación, no un veredicto

Un comentario de revisión tiene cuatro trabajos: ubicar, describir, justificar y proponer. La
fórmula del paso 2 hace los cuatro en cinco frases. Mantenlo lo bastante corto para que quien lee
pueda responder a una sola afirmación.

Sin buscar culpables no significa sin consecuencias. El código puede estar mal, y la revisión debe
decirlo. Lo que no hace es explicar el defecto por la persona que lo escribió. "Esta etiqueta está
fija" es una afirmación sobre el código. "No pensó" es una afirmación sobre una persona, y no es
revisable.

### Declarar el contexto que precificaste

La severidad es contextual, así que el informe nombra el contexto:

- **Producto:** un juego cuyo resultado lee una persona. Un ganador incorrecto es alto.
- **Demo interna:** nadie lee el mensaje. El mismo defecto es bajo.
- **Biblioteca:** un segundo llamador puede disparar el efecto secundario. F-02 se vuelve alto.

Si el contexto no se declara, quien lee sustituirá el suyo en silencio, y la prioridad parecerá
arbitraria.

### Qué no cambiaría

La plantilla tiene una sección para el comportamiento que parece raro pero es necesario. Complétala.
En el ejemplo guiado:

- El efecto secundario `a = 0` es cómo termina la partida. Quitarlo sin mover la detención al
  llamador dejaría el bucle corriendo.
- Las cuatro coordenadas de peón son la representación actual. Reemplazarlas es un cambio
  estructural, no parte de esta revisión.
- El texto del mensaje es evidencia de F-01, no un hallazgo de estilo.

Esta sección es lo que impide que una revisión se convierta en una reescritura. También protege a
quien revisa: un hallazgo que dice "esto es intencional" vale tanto como uno que dice "esto está
mal".

### Límites de la revisión

Escribe las cuatro líneas:

- **Leí:** la función, su rango de líneas y sus llamadores.
- **Ejecuté:** los comandos de grep; opcionalmente una compilación de una copia.
- **No ejecuté:** la partida, la prueba, la compilación del programa.
- **Inferí:** la identidad del último jugador y el ganador pretendido, a partir de la lógica de
  turnos que rodea la función.

La última línea importa. El código no tiene modelo de jugador, así que "el jugador debería ganar" es
una inferencia de las reglas del juego, no un hecho de la función. Una revisión que esconde la
inferencia está exagerando.

## Paso 6 — Registrar (5 min)

Completa [`CODE-REVIEW-TEMPLATE-es.md`](CODE-REVIEW-TEMPLATE-es.md) con el alcance, los hallazgos,
la prioridad, lo que no cambiarías y los límites de la revisión. Después completa
[`EVIDENCE-es.md`](EVIDENCE-es.md). Como mínimo:

- entorno: ruta de la fuente, compilador, shell, terminal, directorio de trabajo;
- la ficha de alcance: archivo, función, líneas, llamadores, globales, fuera de alcance;
- la tabla de hallazgos con al menos tres filas y un peldaño por fila;
- la frase de severidad de cada hallazgo: a quién daña, en qué contexto;
- el orden de prioridad y la regla que lo produjo;
- el contrato de prueba del hallazgo de mayor riesgo, con oráculo y costura;
- los no-hallazgos y los comportamientos que no cambiarías;
- los límites de la revisión: leí, ejecuté, no ejecuté, inferí;
- la comprobación de integridad: `original/` sin modificar.

Una frase como "la función está mal diseñada" es una opinión. "Las líneas 296–299 imprimen la
etiqueta de la máquina para ambos últimos jugadores; el ganador es un literal; la prueba
`outcome_reports_capturing_side` falla en el caso del jugador y pasa después de derivar la etiqueta
de `last_mover`" es un registro.

El criterio de aceptación son tres hechos:

```text
findings=3
severity_order=yes
test=outcome_reports_capturing_side
```

Si no puedes producir los tres, la línea que falta nombra el paso siguiente. No hay penalización por
repetir uno.

## El original en detalle

Hechos que puedes citar, de
`original/variant-00-precursor-2007/source/ajedrez-caballo-2007.txt`:

| Ubicación | Qué hay |
|---:|---|
| 296–297 | La condición: cuatro comprobaciones `tablero[peonNf][peonNc] != -1`, unidas con `&&`. |
| 299 | `cout << "\n\nTRIUNFO PARA LA MIQUINA :D\n\n";` — el ganador fijo. |
| 300 | `a = 0;` — la escritura de estado que termina la partida. |

La condición es verdadera cuando las cuatro celdas de peón ya no son `-1`. El mensaje es un literal,
así que no depende de qué lado movió. La asignación es la única escritura de la función, y no es
visible en la firma.

Anclas relacionadas que puedes necesitar:

| Tema | Dónde mirar | Por qué |
|---|---|---|
| El bucle de turnos | El llamador de `pruebatriunfo` | Muestra qué controla `a`. |
| El centinela de peón | La declaración del tablero y el código de captura | Muestra qué significa `-1`. |
| El camino de captura | La lógica de movimiento | Muestra quién puede capturar el último peón. |

No copies un número de línea de un resumen. Vuelve a ejecutar `nl -ba` y cita lo que hay. Una
revisión que cita una línea obsoleta es peor que una sin línea, porque parece comprobable y no lo
es.

## Qué demuestra la revisión y qué no

| Afirmación | ¿Sostenida? | Por qué |
|---|---|---|
| La condición y la etiqueta están en 296–299. | Sí | Las líneas citadas. |
| La etiqueta no depende del último jugador. | Sí | Es un literal de cadena. |
| La función escribe `a` en la línea 300. | Sí | La asignación es visible. |
| El juego informa un ganador incorrecto. | Sí, si las reglas dicen que gana quien captura | La inferencia está declarada. |
| La prueba pasa después del arreglo. | No | La prueba se nombra, no se implementa. |
| El arreglo mínimo es seguro para todos los llamadores. | No | Solo se cubren los llamadores que leíste. |
| La función es el único defecto del archivo. | No | El alcance es una función. |
| La revisión está completa. | No | Es una pasada en un peldaño. |

Esta tabla es la comprobación de honestidad. Una revisión que dice "el ganador está mal" sin
nombrar la regla está exagerando. Una revisión que dice "la etiqueta está fija y las reglas dicen
que gana quien captura" es defendible.

## Autocomprobación (binaria)

- [ ] Elegí una función y escribí su archivo, rango de líneas y llamadores.
- [ ] Leí la función tres veces: contrato, estado, flujo de control.
- [ ] Escribí la ficha de alcance antes de los hallazgos.
- [ ] Escribí al menos tres hallazgos distintos.
- [ ] Cada hallazgo tiene ubicación, síntoma, causa raíz, severidad y evidencia.
- [ ] Cada severidad tiene una frase: a quién daña, en qué contexto.
- [ ] Separé al menos una preferencia de los hallazgos.
- [ ] Ordené los hallazgos con las tres reglas y escribí la razón por fila.
- [ ] Nombré las filas arreglar primero, arreglar después y dejar como evidencia.
- [ ] Nombré una prueba para el hallazgo de mayor riesgo.
- [ ] La prueba tiene dado, cuando, entonces y un oráculo independiente de la implementación.
- [ ] Declaré si la prueba falla antes del arreglo y pasa después.
- [ ] Nombré la costura mínima que la prueba necesita.
- [ ] Registré el peldaño de evidencia de cada hallazgo.
- [ ] Escribí qué leí, qué ejecuté, qué no ejecuté y qué inferí.
- [ ] Nombré al menos un comportamiento que no cambiaría, con una razón.
- [ ] La fuente está sin modificar y mi copia vive fuera de `original/`.

Si una casilla no está marcada, la casilla sin marcar nombra el paso siguiente. No hay penalización
por repetir uno.

## Misconcepciones que esta lección refuta

| Concepto erróneo | Por qué está mal | Qué hacer en su lugar |
|---|---|---|
| "Una revisión lista todo lo que está mal." | Una lista sin prioridad no se puede accionar. | Ordena los hallazgos y nombra el primer arreglo. |
| "La severidad es lo feo que se ve el código." | La severidad es consecuencia en un contexto. | Escribe la frase quién/qué/dónde. |
| "Más hallazgos significa mejor revisión." | Las filas solapadas esconden el riesgo real. | Tres hallazgos distintos vencen a diez duplicados. |
| "La prueba es opcional en una revisión." | La prueba es cómo el hallazgo se vuelve comprobable. | Nombra una prueba para el hallazgo de arriba. |
| "Si no puedo probarlo, omito la prueba." | La costura ausente es un hallazgo en sí misma. | Nombra la costura que la prueba necesita. |
| "La prioridad es igual a la severidad." | La alcanzabilidad y el coste reordenan dentro de una banda. | Enuncia las tres reglas y aplícalas. |
| "Los comentarios de estilo son hallazgos." | No tienen consecuencia. | Ponlos en una sección aparte. |
| "Sin culpa significa sin severidad." | Sin culpa es sobre la persona, no sobre el código. | Juzga la decisión, precifica la consecuencia. |
| "Quien revisa debería arreglarlo." | Revisión y parche son actos distintos. | Nombra el cambio mínimo; deja que el parche se revise. |
| "Una pasada es una revisión completa." | Es una pasada en un peldaño. | Declara los límites. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Observación | Una afirmación verdadera sobre el código, sin consecuencia asociada. |
| Hallazgo | Una afirmación de que algo daña a un llamador, un resultado o un cambio. |
| Preferencia | Una opinión de estilo o diseño sin evidencia de daño. |
| Severidad | La consecuencia si nadie arregla el hallazgo. |
| Prioridad | El orden en que deben arreglarse los hallazgos. |
| Alcanzabilidad | Con qué frecuencia el camino normal llega al código. |
| Causa raíz | La decisión que produce el síntoma. |
| Peldaño de evidencia | Hasta dónde se comprobó la afirmación: leer, grep, compilar, ejecutar, probar. |
| Oráculo | La regla que dice cuál es el resultado correcto, con independencia del código. |
| Costura | El límite más pequeño donde un llamador puede observar o reemplazar comportamiento. |
| Capacidad de prueba | Si un llamador puede comprobar el resultado sin la consola ni un global. |
| Límite de revisión | La frontera entre lo que se comprobó y lo que se infirió. |
| Sin culpa | Juzga la decisión, no a la persona. |

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
