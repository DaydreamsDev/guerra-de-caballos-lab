# archaeology-19 — Lista de revisión y retrospectiva

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** revisión y retrospectiva ·
> **Prerrequisito:** archaeology-18 · **Tiempo estimado:** 90 minutos
> **Lectura:** ~23 minutos
> **Rol de la lección:** proyecto final

## Por qué existe esta lección

Track B te enseñó a diagnosticar un fallo a la vez. Eso es necesario y no es suficiente. Un revisor
que encuentra un off-by-one pero no sabe a qué clase pertenece seguirá encontrando el mismo
off-by-one en cada fichero. Un revisor que nombra la clase, pone precio a la consecuencia y señala
el diseño que la previene ha construido un instrumento que se transfiere.

El archivo es finito. Contiene un conjunto conocido de fallos, documentado en la guía del
recorrido y diagnosticado a lo largo de las lecciones anteriores. No vas a revisar este archivo
durante el resto de tu carrera. Vas a revisar código que nunca has visto, escrito por personas
a las que no puedes preguntar, bajo restricciones que no compartiste. La lista de revisión es
la forma en que el recorrido sale del archivo.

Hay una segunda razón. Una lista de revisión es fácil de escribir mal. "Límites comprobados" suena
profesional y no decide nada. Dos revisores pueden marcarlo y discrepar sobre el mismo fichero. El
trabajo de esta lección es convertir frases en pruebas: una pregunta, una fuente de evidencia, un
criterio de aprobación, una clase de fallo y un escalón de Track A. Esa es la diferencia entre una
lista de deseos y un instrumento.

La retrospectiva es la otra mitad. Veinte lecciones producen mucha actividad y poco juicio sobre
la actividad. La retrospectiva pregunta qué cambió en tu forma de leer código, qué clase sigues
diagnosticando de menos, qué afirmación hiciste demasiado pronto y qué harás distinto en la próxima
revisión. Se trata de tu proceso, no de tu valor.

La lección termina cuando
otra persona puede tomar tu lista, aplicarla a un fichero que nunca has visto y reproducir tu
veredicto a partir de tu evidencia.

## Qué podrás hacer

Al terminar esta lección puedes:

- auditar una lista de revisión e identificar los elementos que no son directamente observables;
- reescribir una frase vaga como una pregunta con evidencia, criterio de aprobación y clase de
  fallo;
- construir una lista reutilizable sobre corrección, diseño, robustez, práctica profesional y
  evidencia;
- ligar cada familia a los identificadores de fallo que detecta y al escalón de Track A que previene
  la clase;
- aplicar la lista a una variante desconocida con tiempo acotado y registrar lo que encontró;
- distinguir un fallo omitido de un falso positivo y calcular una tasa de detección;
- enunciar los límites de una muestra de una variante y un revisor;
- hacer una retrospectiva basada en evidencia sobre tu propio proceso de revisión;
- auditar afirmaciones como verificadas, documentadas, inferidas o desconocidas;
- explicar la lista en 60 segundos y nombrar lo que no puede probar;
- mantener el archivo en solo lectura mientras trabajas.

## El concepto: una lista es un instrumento, no una lista de deseos

### Cinco propiedades de un elemento utilizable

Un elemento de la lista es una prueba pequeña. No es un tema y no es una opinión. Cinco propiedades
lo hacen utilizable.

| Propiedad | Pregunta que responde | Mal ejemplo | Buen ejemplo |
|---|---|---|---|
| **Observable** | ¿Dos revisores llegan al mismo veredicto? | "El código está limpio." | "Sin advertencias del compilador con las banderas del proyecto." |
| **Atribuible** | ¿Dónde está la evidencia? | "Los globales son malos." | "Cada variable mutable de ámbito de espacio de nombres tiene tabla de escritores y una prueba que la asigna dos veces." |
| **Clasificado** | ¿A qué clase de fallo pertenece? | "Mejorar la estructura." | "Diseño: acoplamiento temporal oculto." |
| **Accionable** | ¿Qué hago cuando falla? | "Refactorizar." | "Nombrar los escritores; extraer una función; repetir las trazas." |
| **Reutilizable** | ¿Funciona fuera de este archivo? | "Comprobar los límites de `tablero`." | "Comprobar cada bucle que escribe un arreglo contra el número de elementos." |

La quinta propiedad es la que se salta casi todo el mundo. Una lista atada a `tablero`, `jugada` o
`variant-02` es un diagnóstico de este archivo, no un instrumento de revisión. Los nombres del
proyecto van en la columna de evidencia, nunca en la pregunta.

### Las cinco familias

Las familias no son una taxonomía del gusto. Responden a cinco preguntas distintas, y una revisión
que se salta una de ellas produce un punto ciego predecible.

| Familia | Pregunta | Qué cuesta omitirla | Fallos típicos | Ancla en Track A |
|---|---|---|---|---|
| **Corrección** | ¿Hace lo que afirma? | Comportamiento erróneo, corrupción silenciosa, pérdida de datos. | failure-001…failure-006 | `construction-02`, `construction-04`, `construction-05` |
| **Diseño** | ¿La estructura encarece el cambio? | Cada cambio de regla toca muchos sitios; las pruebas siguen siendo imposibles. | failure-007…failure-014 | `construction-08`, `construction-09`, `construction-12`, `construction-14`, `construction-20` |
| **Robustez** | ¿Funciona solo en un entorno? | Fallo de compilación, deuda de portabilidad, entrada no controlada. | failure-006, failure-015 | `construction-08`, `construction-20`, `construction-22` |
| **Práctica profesional** | ¿Otra persona puede compilarlo, confiar en él y mantenerlo? | Nadie puede reproducir, confiar ni cambiar el artefacto con seguridad. | failure-016…failure-019 | `common-02`, `common-04`, `common-05`, `construction-24` |
| **Evidencia** | ¿La afirmación se puede comprobar? | La revisión se vuelve opinión; lo desconocido se esconde como hecho. | todas | `common-03`, `common-05` |

Los límites se solapan a propósito. `failure-006` (entrada sin límite) es a la vez un fallo de
corrección y de robustez. La lista puede colocar un elemento en una familia y remitir a otra; no
debe fingir que el solapamiento no existe.

### Por qué la evidencia es una familia propia

Un hallazgo sin fuente es una opinión con nombre de fichero. La evidencia tiene cuatro estados, y la
lista debe obligarte a elegir uno:

| Estado | Significado | Ejemplo |
|---|---|---|
| **Verificada** | La reprodujiste o leíste la línea exacta. | `variant-02...cpp:72` tiene `i <= 64` sobre un arreglo de 64 elementos. |
| **Documentada** | El artefacto lo dice, pero no lo reprodujiste. | El informe de compilación nombra Visual C++ 6.0. |
| **Inferida** | Razonaste a partir de otra evidencia. | La codificación es CP1252 porque el byte 219 no es UTF-8 válido. |
| **Desconocida** | No lo comprobaste, o el archivo público no puede responder. | Si el ejecutable original corrió en su máquina objetivo. |

La familia de evidencia no es una sexta área técnica. Es la regla que mantiene honestas a las otras
cuatro. Cada hallazgo de tu revisión recibe una de estas etiquetas. Una lista que no pide la
etiqueta producirá prosa segura y afirmaciones débiles.

### Revisar no es probar

La lista, la prueba y la traza de referencia responden a preguntas distintas. Una revisión puede
detectar una clase que ninguna prueba cubre; una prueba puede detectar un detalle que ningún revisor
leería.

| Instrumento | Fortaleza | Límite |
|---|---|---|
| **Lista de revisión** | Encuentra clases, deuda de diseño y evidencia ausente en todo el artefacto. | No puede probar comportamiento en ejecución; depende de la disciplina del revisor. |
| **Prueba unitaria** | Prueba un comportamiento con una entrada, de forma repetible. | Solo cubre los casos que alguien pensó en escribir. |
| **Traza de referencia** | Prueba que una transcripción no cambió tras una refactorización. | Solo cubre los caminos que la traza ejercita. |

El movimiento profesional no es elegir uno. Es decir qué afirmación sostiene cada instrumento. "El
bucle escribe fuera del arreglo" es un hallazgo de revisión más un resultado de sanitizador, no una
prueba que pasa.

### Falsos positivos, falsos negativos y el coste de cada uno

Dos números importan cuando aplicas una lista.

- **Detección** es la proporción de fallos esperados que la lista encontró:
  `detectados / esperados`.
- **Precisión** es la proporción de hallazgos reportados que eran reales: `reales / reportados`.

Una lista que lo reporta todo tiene detección perfecta y precisión pésima. Una lista que solo
reporta certezas tiene precisión perfecta y omite la clase para la que fue construida. Ninguno de
los dos números es una nota. Son una descripción del instrumento, y te dicen qué elemento
reescribir.

Registra ambos. Un fallo omitido es un defecto de la lista o del revisor; un falso positivo suele
ser un criterio de aprobación vago. La siguiente iteración arregla el elemento, no el carácter del
revisor.

### La severidad es consecuencia, no fealdad

Usa los cuatro niveles del recorrido. Un hallazgo no es alto porque el código se vea mal; es alto
porque la consecuencia es alta en un contexto nombrado.

| Nivel | Significado | Acción |
|---|---|---|
| **Crítica** | Comportamiento indefinido, pérdida de datos, seguridad, bloqueo legal. | Arreglar antes de publicar. |
| **Alta** | Comportamiento erróneo, resultados no reproducibles, incidente difícil. | Arreglar en el parche mínimo. |
| **Media** | Cambio caro, estructura frágil. | Arreglo estructural, programado. |
| **Baja** | Legibilidad, nombres, comentarios. | Mejorar al tocar el fichero. |

Cada hallazgo de tu pasada de aplicación recibe un nivel y una frase de contexto. "Alta porque una
escritura fuera de límites puede corromper el arreglo adyacente" es un precio. "Alta porque es feo"
no lo es.

### Reglas de parada

Una revisión sin reglas de parada se convierte en una reescritura. Escríbelas antes de la pasada
cronometrada.

1. **Tiempo acotado.** Veinte minutos para la primera pasada. Detente cuando suene el temporizador,
   aunque el fichero esté sin terminar. La parte sin terminar es un dato.
2. **Sin arreglos durante la revisión.** Registra el hallazgo y sigue. El arreglo mínimo es un paso
   aparte, y Track B ya te enseñó por qué.
3. **Parada por severidad alta.** Si encuentras comportamiento indefinido o pérdida de datos,
   márcalo y continúa la pasada. No desaparezcas dentro de un solo fallo.
4. **Regla de dos fuentes.** Un hallazgo necesita una línea, un comando o un documento. Si no puedes
   señalar uno, etiquétalo como inferido o desconocido.
5. **Un revisor, una pasada.** No releas el mismo fichero tres veces para mejorar la puntuación. Una
   segunda pasada es un segundo dato, no una corrección del primero.

### Retrospectiva: proceso, no puntuación

La retrospectiva es una revisión de la revisión. Tiene cuatro preguntas:

- **¿Qué pasó?** La línea de tiempo y los artefactos, sin interpretación.
- **¿Y qué?** Qué diagnóstico fue el más fuerte, cuál el más débil, qué clase diagnosticaste de
  menos.
- **¿Y ahora qué?** Un hábito que conservar, uno que dejar, uno que empezar, una cosa que
  automatizar.
- **¿Cuál es la evidencia?** Qué afirmaciones del portafolio están verificadas, documentadas,
  inferidas o desconocidas.

Mantenla basada en evidencia. La pregunta es qué
hizo razonable una decisión entonces y qué haría razonable una decisión distinta ahora. La misma
pregunta se aplica a tu revisión: qué hizo que "límites comprobados" pareciera suficiente, y qué
evidencia lo convertiría en una prueba real.

## Antes de empezar

Lee esto completo:

- [`README-es.md`](README-es.md) — la ficha de la lección, el instrumento y el criterio de
  aceptación.
- [`REVIEW-CHECKLIST-es.md`](REVIEW-CHECKLIST-es.md) — la plantilla que vas a auditar y ampliar.
- [archaeology-18](../archaeology-18-refactor-workshop/WORKSHOP-REPORT-es.md) — el prerrequisito: el
  taller de refactorización y su tabla de costes.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, la
  taxonomía de fallos; §5, la escala de severidad; §7, el catálogo de fallos; §9, el mapeo a Track
  A.
- [`../../../docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — la secuencia de
  Track B y el criterio de aceptación de esta lección.

También necesitas:

- tu portafolio de `archaeology-00` a `archaeology-18`, sobre todo los registros de evidencia y las
  fichas de fallo;
- una copia local del archivo en la etiqueta `v1.0.0`;
- un compilador de C++20, `bash`, `grep`, `wc` y `diff`;
- un directorio de trabajo fuera de `original/`, por ejemplo `build/review-sample/`.

> **Regla:** el archivo es de solo lectura. Copia cualquier fichero que quieras inspeccionar
> de cerca a un directorio de trabajo; nunca edites `original/`. Una revisión que cambia la
> evidencia no es una revisión.

## El camino de 90 minutos

| Min | Paso | Qué haces | Evidencia |
|---:|---|---|---|
| 0–15 | Inspeccionar | Inventariar el portafolio, auditar la plantilla, elegir la muestra sellada | Tabla de inventario, lista de elementos vagos |
| 15–35 | Lista | Reescribir elementos, añadir cobertura, fijar reglas de parada | Lista reutilizable, matriz de cobertura |
| 35–55 | Aplicar | Pasada cronometrada sobre la variante sellada y comparación con el catálogo | Tabla de hallazgos, tasa de detección |
| 55–70 | Retrospectiva | Revisar todo el recorrido y tu propio proceso | Tabla conservar/dejar/empezar, auditoría de afirmaciones |
| 70–82 | Explicar | Dar la versión de 60 segundos y defender las familias | Nota de explicación |
| 82–90 | Registrar | Rellenar `EVIDENCE-es.md`, confirmar que el archivo está limpio | Entrada de portafolio |

Si la auditoría de la plantilla tarda veinticinco minutos porque
contiene más elementos vagos de los que esperabas, esa investigación es la lección. No te saltes la
pasada cronometrada para recuperar el tiempo; acorta la explicación.

### La comprobación de ingeniería en esta lección

Track B usa el mismo hábito que Track A, con otro verbo: el comportamiento ya existe, así que
**congelas** lo que hace antes de moverlo. Aquí el artefacto bajo revisión es tu propio proceso.

| Paso | Dónde ocurre | Qué produce |
|---|---|---|
| **Inspeccionar** | Paso 1 | Inventario del portafolio, auditoría de la plantilla, muestra sellada elegida. |
| **Lista** | Paso 2 | Al menos 15 elementos observables con evidencia y clasificación. |
| **Aplicar** | Paso 3 | Una pasada cronometrada, una tabla de hallazgos y una tasa de detección. |
| **Retrospectiva** | Paso 4 | Conservar/dejar/empezar y una auditoría de afirmaciones de todo el recorrido. |
| **Explicar** | Paso 5 | Un discurso de 60 segundos y los límites del instrumento. |
| **Registrar** | Paso 6 | `EVIDENCE-es.md` con comandos, hallazgos y fronteras. |

## Dos sesiones

Esta lección es larga a propósito. Divídela en dos sesiones:

- **Sesión 1 — Núcleo (Pasos 1–3, unos 55 min).** Detente después del Paso 3, cuando la lista
  existe y se aplicó una vez.
- **Sesión 2 — Extensión (Pasos 4–6, unos 35 min).** Empieza solo después de la Sesión 1. Vas a
  hacer la retrospectiva y registrar la auditoría de afirmaciones.

## Paso 1 — Inspeccionar (15 min)

### Inventariar el portafolio

Empieza por lo que produjo el recorrido. No confíes en la memoria; cuenta ficheros y entradas.

```bash
cd archaeology/lessons
find . -name 'EVIDENCE.md' -o -name 'EVIDENCE-es.md' | sort
find . -name '*.md' | wc -l
grep -rl 'failure-0' --include='*.md' . | sort
```

Registra las lecciones que produjeron un artefacto reutilizable, no solo una lectura. Una ficha de
fallo, una prueba, una traza, un informe de refactorización y un registro de evidencia son
artefactos. Una lección que leíste y disfrutaste no lo es.

| Lección | Artefacto que produje | Estado de evidencia | ¿Reutilizable? |
|---|---|---|---|
| archaeology-00 | Mapa de código | | |
| archaeology-01 | Compilación o bloqueo documentado | | |
| archaeology-02 | Inventario de código y grafo de llamadas | | |
| archaeology-03 | Especificación y trazas | | |
| archaeology-04 | Fichas de fallo y pruebas | | |
| archaeology-05 | Mapa de mutaciones globales | | |
| archaeology-06 | Una extracción con comportamiento preservado | | |
| archaeology-07 | Registro introducido, llamadores actualizados | | |
| archaeology-08 | Cabecera e implementación | | |
| archaeology-09 | Tipo opaco y cambio de representación | | |
| archaeology-10 | Despacho que reemplaza el switch | | |
| archaeology-11 | Núcleo sin consola | | |
| archaeology-12 | Renombrado y reescritura de comentarios | | |
| archaeology-13 | README, especificación e instrucciones de compilación | | |
| archaeology-14 | Trazas de referencia y control de semilla | | |
| archaeology-15 | Análisis estático y advertencias | | |
| archaeology-16 | Compilación portable y adaptador | | |
| archaeology-17 | Revisión de código | | |
| archaeology-18 | Informe de refactorización y tabla de costes | | |

Las celdas vacías son el punto. Si una lección no produjo artefacto, el portafolio tiene un hueco, y
la retrospectiva del paso 4 debe nombrarlo.

### Auditar la plantilla

Abre `REVIEW-CHECKLIST-es.md`. Tiene 25 casillas y cinco frases de retrospectiva. Lee cada
elemento y pregunta: **¿dos revisores pueden aplicarlo y alcanzar el mismo veredicto con la
misma evidencia?**

Algunos elementos ya pasan. "La aleatoriedad tiene semilla" es observable: encuentra la llamada a la
semilla, encuentra el bucle de reintentos, comprueba si la semilla se fija una vez. "Cada afirmación
tiene una fuente" es observable: abre la afirmación y busca un fichero, una línea o un comando.

Otros no. "Límites comprobados" es un tema, no una prueba. ¿Qué límites? ¿Comprobados cómo? ¿Contra
qué? "Estado global mapeado" es mejor, pero todavía no dice qué contiene un mapa ni cuándo está
completo. "El núcleo se ejecuta sin consola" está cerca; necesita un comando y un resultado
esperado.

Escribe los elementos vagos antes de reescribirlos. La lista es evidencia de la calidad de la
plantilla, y es la entrada del paso 2.

| Elemento de la plantilla | Por qué es vago | Qué evidencia lo resolvería |
|---|---|---|
| Límites comprobados. | ¿Qué límites, y contra qué? | |
| Estado global mapeado. | ¿Qué contiene un mapa completo? | |
| Representación oculta. | ¿Oculta para quién, y cómo se prueba? | |
| Los errores son valores o bloqueos documentados. | ¿Qué errores, y dónde se registra el bloqueo? | |
| El árbol está limpio. | ¿Limpio según qué comando, y qué rutas se excluyen? | |

Añade tus propias filas. Cinco es un mínimo, no un objetivo.

### Elegir la muestra sellada

Usa `variant-02-records-2008`. No es la línea base que más has leído, contiene los fallos conocidos
de la variante de registros y es lo bastante pequeña para una pasada de 20 minutos.

```bash
mkdir -p build/review-sample
cp original/variant-02-records-2008/source/guerra-de-caballos-records.cpp \
   build/review-sample/sample.cpp
wc -l build/review-sample/sample.cpp
```

No abras el catálogo de fallos durante la pasada cronometrada. La pasada prueba la lista, no tu
memoria de §7. Compararás con el catálogo en el paso 3.

Por qué esta variante y no `variant-01`:

- `variant-01` es la línea base usada en varias lecciones; tu memoria de sus anclajes es demasiado
  fuerte.
- `variant-02` usa registros, así que los hallazgos de diseño son distintos de la variante de
  punteros.
- La guía del recorrido registra el off-by-one en la línea 72 de esta variante, lo que te da un
  anclaje verificable para calibrar.

### Predecir antes de la pasada

Escribe cinco predicciones. Son sobre la lista, no sobre el código.

1. ¿Cuántos de los diez fallos esperados encontrará la lista en 20 minutos?
2. ¿Qué familia producirá más hallazgos?
3. ¿Qué familia producirá menos?
4. ¿Cuál será el primer hallazgo de severidad alta, y en qué minuto?
5. ¿Qué elemento producirá un falso positivo, y por qué?

Una predicción útil es falsable. "La lista funcionará" no lo es. "Encontrará siete de diez, omitirá
el fallo de comentarios porque los comentarios no son ejecutables, y reportará un falso positivo en
`system("color")` porque el elemento no distingue una llamada de una cadena" sí lo es.

## Paso 2 — Lista (20 min)

### La forma de un elemento

Usa una fila por elemento. Las columnas son el instrumento.

| Columna | Qué contiene | Ejemplo |
|---|---|---|
| **ID** | Nombre estable, prefijo de familia. | `C-03` |
| **Familia** | Corrección, diseño, robustez, práctica o evidencia. | Corrección |
| **Pregunta** | Una pregunta observable, sin nombres del proyecto. | "¿Todo bucle que escribe un arreglo se detiene antes del número de elementos?" |
| **Evidencia** | Fichero, línea, comando o documento que la responde. | "Declaración del arreglo y cabecera del bucle; `grep -n 'for'`." |
| **Criterio de aprobación** | Qué hace que la respuesta sea sí. | "El límite es `< N`, y el último elemento se escribe exactamente una vez." |
| **Clase de fallo** | La clase de la taxonomía. | Corrección |
| **Escalón de Track A** | El escalón que elimina la clase. | `construction-02` |
| **Severidad si falla** | Nivel de consecuencia y una frase. | Alta: escritura silenciosa fuera de límites. |

Si una fila no puede llenar las ocho columnas, no está lista. Una pregunta sin evidencia es un
deseo; un criterio sin clase no se puede priorizar; una clase sin escalón no se puede arreglar
estructuralmente.

### Convertir lo vago en observable

Tres conversiones resueltas. Copia el patrón, no las palabras.

**Ejemplo 1 — corrección.**

- Antes: "Límites comprobados."
- Después: "¿Todo bucle que escribe un arreglo usa un límite estrictamente menor que el número de
  elementos, y el último elemento se escribe exactamente una vez?"
- Evidencia: declaración del arreglo, cabecera del bucle, una ejecución con sanitizador si está
  disponible.
- Aprueba: el límite es `< N`; el último elemento se escribe una vez; ningún informe del sanitizador
  en el camino.
- Clase: corrección. Escalón: `construction-02`. Severidad: crítica si la escritura está fuera de
  límites.

**Ejemplo 2 — diseño.**

- Antes: "Estado global mapeado."
- Después: "¿Cada variable mutable de ámbito de espacio de nombres tiene una tabla de escritores y
  lectores, y una prueba que la asigna dos veces para exponer la dependencia de orden?"
- Evidencia: `grep` de la declaración, sitios de llamada, la prueba.
- Aprueba: cada variable tiene al menos un escritor, un lector y una prueba que falla cuando se
  invierte el orden.
- Clase: diseño. Escalón: `construction-08`. Severidad: alta si un incidente no se puede reproducir.

**Ejemplo 3 — robustez.**

- Antes: "Sin encabezados de plataforma en el núcleo."
- Después: "¿La unidad de traducción del núcleo incluye solo encabezados estándar, y las llamadas de
  plataforma aparecen solo en ficheros adaptadores?"
- Evidencia: `grep -n '#include'`, `nm -C -u` sobre el objeto del núcleo.
- Aprueba: no hay `conio.h`, `windows.h` ni `system("color")` en el núcleo; el adaptador los posee.
- Clase: robustez. Escalón: `construction-20`. Severidad: alta si la compilación falla en un segundo
  objetivo.

Fíjate en qué cambió. La pregunta se convirtió en una frase que alguien puede responder con un
fichero y un comando. Se añadieron la clase y el escalón. La severidad se volvió una consecuencia,
no un adjetivo.

### Construir la lista

Cobertura mínima:

| Familia | Elementos mínimos | Debe incluir |
|---|---:|---|
| Corrección | 3 | Límites, inicialización, aleatoriedad o entrada. |
| Diseño | 3 | Estado global, duplicación, representación o despacho. |
| Robustez | 3 | Frontera de plataforma, validación de entrada, camino de error. |
| Práctica profesional | 3 | Compilación, pruebas o trazas, documentación o higiene del árbol. |
| Evidencia | 3 | Fuente por afirmación, etiqueta de confianza, integridad del archivo, exactitud de etiquetas. |

Quince elementos es el suelo. Veinte es cómodo. Más de treinta suele ser señal de que el
revisor está escribiendo un libro en vez de una lista; fusiona preguntas relacionadas y
mantén los criterios afilados.

Las cinco clases de fallo deben aparecer al menos una vez. Corrección, diseño, robustez, práctica
profesional y evidencia tienen su propia familia.

Conjunto de partida. Sustituye los huecos con tu redacción y tu evidencia.

| ID | Familia | Pregunta | Evidencia | Criterio de aprobación | Clase | Escalón | Severidad si falla |
|---|---|---|---|---|---|---|---|
| C-01 | Corrección | ¿Toda escritura en un arreglo queda por debajo del número de elementos? | | | Corrección | `construction-02` | Crítica |
| C-02 | Corrección | ¿Toda variable se inicializa antes de su primera lectura? | | | Corrección | `construction-04` | Alta |
| C-03 | Corrección | ¿La aleatoriedad se siembra una vez, fuera del bucle de reintentos? | | | Corrección | `common-02` | Alta |
| C-04 | Corrección | ¿La condición de victoria coincide con la especificación escrita? | | | Corrección | `construction-00` | Alta |
| D-01 | Diseño | ¿Todo global mutable tiene un mapa de escritores y lectores? | | | Diseño | `construction-08` | Alta |
| D-02 | Diseño | ¿La lógica duplicada se nombra y se extrae una vez? | | | Diseño | `construction-03` | Media |
| D-03 | Diseño | ¿La representación está oculta tras una operación? | | | Diseño | `construction-09` | Media |
| D-04 | Diseño | ¿El despacho reemplaza al switch de tipo? | | | Diseño | `construction-12` | Media |
| R-01 | Robustez | ¿El núcleo está libre de encabezados y llamadas de plataforma? | | | Robustez | `construction-20` | Alta |
| R-02 | Robustez | ¿Toda longitud de entrada está acotada antes de usarse? | | | Robustez | `construction-22` | Crítica |
| R-03 | Robustez | ¿Todo camino de error devuelve un valor o registra un bloqueo? | | | Robustez | `construction-22` | Alta |
| P-01 | Práctica | ¿Un comando compila el artefacto desde un árbol limpio? | | | Práctica profesional | `common-04` | Alta |
| P-02 | Práctica | ¿Existen trazas de referencia y control de semilla? | | | Práctica profesional | `common-05` | Alta |
| P-03 | Práctica | ¿Están presentes la compilación, las pruebas y la documentación? | | | Práctica profesional | `common-03` | Alta |
| E-01 | Evidencia | ¿Toda afirmación cita un fichero, línea, comando o documento? | | | Todas | `common-05` | Alta |
| E-02 | Evidencia | ¿Toda afirmación está etiquetada como verificada, documentada, inferida o desconocida? | | | Todas | `common-03` | Media |
| E-03 | Evidencia | ¿El archivo sigue sin modificar después de la revisión? | | | Todas | `common-05` | Alta |
| E-04 | Evidencia | ¿Cada etiqueta o nombre de carpeta coincide con el mecanismo realmente presente? | | | Evidencia | `common-03` | Media |

### Control de calidad

Antes de usar la lista, aplícale estas cinco pruebas.

1. **Prueba de dos revisores.** Elige un elemento. ¿Un segundo revisor podría aplicarlo sin
   preguntarte qué quisiste decir? Si no, reescribe la pregunta.
2. **Prueba de evidencia.** ¿El elemento nombra dónde vive la respuesta? "Leer el código" no es una
   fuente de evidencia.
3. **Prueba de aprobación.** ¿El criterio es binario? "Parece razonable" no es un criterio.
4. **Prueba de clase.** ¿El elemento nombra una sola clase de fallo? Si nombra tres, divídelo.
5. **Prueba de reutilización.** ¿La pregunta evita `tablero`, `jugada`, `variant-02` y cualquier
   otro nombre del proyecto? Si no, mueve el nombre a la columna de evidencia.

Un elemento que falla una prueba no es un fracaso moral. Es un borrador. Reescríbelo y guarda la
versión anterior en tu registro de evidencia; la diferencia es el aprendizaje.

### Reglas de parada y severidad

Copia las cinco reglas de parada del concepto en la cabecera de tu lista. Añade una línea para la
severidad: **todo hallazgo recibe un nivel y una frase de contexto, o no es un hallazgo.**

Las reglas de parada son parte del instrumento. Una lista sin ellas produce una revisión que nunca
termina, o una que se convierte en una reescritura no solicitada. Las dos son fallos profesionales.

### La prueba de reutilización

Toma dos elementos y aplícalos a otro código que conozcas: un proyecto personal, un fichero de
código abierto, un repositorio de trabajo. Si el elemento sigue teniendo sentido, es reutilizable.
Si solo tiene sentido en `guerra-de-caballos`, es específico del archivo y pertenece a la columna de
evidencia, no a la pregunta.

Escribe el resultado en una línea por elemento. Esta es la evidencia de que la lista es un
instrumento y no un segundo diagnóstico del archivo.

## Paso 3 — Aplicar (20 min)

### La pasada cronometrada

Pon un temporizador de 20 minutos. Abre solo `build/review-sample/sample.cpp` y tu lista. No abras
el catálogo de fallos, la guía del recorrido ni tus notas antiguas.

Trabaja en este orden:

1. Hojea el fichero por su estructura: includes, globales, funciones, punto de entrada. Dos minutos.
2. Aplica los elementos de corrección. Cinco minutos.
3. Aplica los elementos de diseño. Cinco minutos.
4. Aplica los elementos de robustez y práctica. Cinco minutos.
5. Aplica los elementos de evidencia y escribe la tabla de hallazgos. Tres minutos.

Si un elemento no aplica, márcalo `n/a` y di por qué. Un `n/a` con razón es un dato; un `n/a` sin
razón es un elemento saltado.

### Registrar hallazgos

Una fila por hallazgo, incluidas las omisiones que notes después del temporizador.

| ID | Hallazgo | Evidencia (fichero:línea o comando) | Clase | Severidad | Elemento de la lista | Veredicto |
|---|---|---|---|---|---|---|
| F-01 | | | | | | confirmado / falso positivo / omitido |
| F-02 | | | | | | |
| F-03 | | | | | | |

Reglas de la tabla:

- Un hallazgo **confirmado** tiene una línea, un comando o un documento.
- Un **falso positivo** es un hallazgo que la lista produjo y que la evidencia no sostiene.
- Un hallazgo **omitido** es un fallo esperado que la lista no produjo. Regístralo después de la
  comparación, no durante la pasada cronometrada.
- No edites la muestra. El archivo y la copia quedan como están.

### Comparar con el catálogo

Después del temporizador, abre la guía del recorrido §7 y los problemas conocidos de la variante.
Los fallos esperados para esta muestra son:

| Esperado | Fallo | Clase | Qué debería haber producido la lista |
|---|---|---|---|
| failure-001 | `i <= 64` sobre un arreglo de 64 elementos | Corrección | Un hallazgo de límites con la línea del bucle. |
| failure-002 | `void main()` | Corrección | Un hallazgo de portabilidad con la línea del punto de entrada. |
| failure-003 | `srand(time(NULL))` dentro de un bucle de reintentos | Corrección | Un hallazgo de determinismo con la semilla y el bucle. |
| failure-007 | Estado global mutable | Diseño | Un mapa de escritores/lectores o un hallazgo de que el mapa falta. |
| failure-008 | Ocho cuerpos `case` copiados a mano | Diseño | Un hallazgo de duplicación con un recuento o un rango. |
| failure-009 | Arreglos paralelos o variables sueltas del caballo | Diseño | Un hallazgo de representación con las declaraciones. |
| failure-012 | Entrada y salida dentro de la lógica de movimiento | Diseño | Un hallazgo de testabilidad con la línea de `cout` o `cin`. |
| failure-015 | `conio.h`, `iostream.h`, `system("color")` | Robustez | Un hallazgo de frontera de plataforma con las líneas de include. |
| failure-016 | Sin pruebas, sin arnés, sin semilla | Práctica profesional | Un hallazgo de práctica que nombre el artefacto ausente. |
| failure-019 | Comentarios que narran o mienten | Práctica profesional | Un hallazgo de comentario con la línea y la contradicción. |

La lista es un conjunto de calibración, no un diagnóstico completo. Una revisión real puede
encontrar más; también puede encontrar un fallo que el catálogo no lista. Ambos son válidos, siempre
que la evidencia quede registrada.

### Medir

Calcula los números siguientes.

| Métrica | Fórmula | Mi resultado |
|---|---|---|
| Detectados | Fallos esperados confirmados | |
| Omitidos | Fallos esperados no detectados | |
| Falsos positivos | Hallazgos reportados que la evidencia no sostuvo | |
| Tasa de detección | Detectados / 10 | |
| Tiempo hasta el primer hallazgo de severidad alta | Minutos desde el inicio | |

Después responde tres preguntas por escrito:

1. ¿Qué elemento produjo más valor por minuto?
2. ¿Qué elemento produjo el falso positivo, y cómo lo reescribirías?
3. ¿Qué fallo omitido fue un hueco de la lista, y cuál un hueco del revisor?

La distinción importa. Un hueco de la lista se arregla añadiendo o reescribiendo un elemento. Un
hueco del revisor se arregla con práctica, o con un segundo revisor. No los confundas.

### Qué no puede probar la muestra

Una variante, un revisor y una pasada cronometrada es un experimento pequeño. Enuncia los límites:

- No prueba que la lista detecte todas las clases de fallo.
- No prueba que la tasa de detección se generalice a otro código.
- No prueba que el archivo sea correcto o incorrecto; prueba lo que encontraste.
- No reemplaza una prueba, un sanitizador o una traza.
- No mide falsos negativos fuera de los diez fallos esperados.

Un revisor que enuncia estos límites es más confiable que uno que reporta una puntuación perfecta.

## Paso 4 — Retrospectiva (15 min)

### El recorrido en una tabla

Agrupa las veinte lecciones por lo que enseñaron, no por su número.

| Fase | Lecciones | Qué enseñó | Artefacto |
|---|---|---|---|
| Contexto | 00–03 | Procedencia, compilación, linaje y especificación antes del juicio. | Nota, compilación, tabla, trazas. |
| Corrección | 04 | El programa no hace lo que afirma. | Fichas de fallo y pruebas. |
| Diseño | 05–11 | La estructura encarece el cambio. | Mapas, extracciones, módulos, tipos, despacho, puertos. |
| Práctica | 12–15 | El artefacto debe ser compartible y mantenible. | Nombres, documentación, pruebas, árbol limpio. |
| Entorno | 16 | El programa funciona solo en un entorno. | Adaptador y compilación portable. |
| Enseñanza | 17 | La etiqueta puede mentir sobre el artefacto. | Auditoría de etiquetas. |
| Síntesis | 18 | Una refactorización tiene un precio y un comportamiento preservado. | Informe del taller. |
| Revisión | 19 | El instrumento y la retrospectiva. | Lista y este registro. |

### Cuatro preguntas

Responde antes de escribir la tabla conservar/dejar/empezar.

1. **Diagnóstico más fuerte.** ¿Qué hallazgo de todo el recorrido te da más confianza? Nombra la
   evidencia y por qué sobrevive a una objeción.
2. **Diagnóstico más débil.** ¿Qué hallazgo retirarías o debilitarías hoy? Nombra la evidencia que
   falta.
3. **Clase diagnosticada de menos.** ¿Qué familia saltaste o trataste como estilo de forma
   consistente? ¿Corrección, diseño, robustez, práctica o evidencia?
4. **Clase diagnosticada de más.** ¿Qué familia sobrevaloraste porque era fácil de ver? Un revisor
   que encuentra diez problemas de nombres y ningún fallo de corrección tiene un problema de
   calibración, no un fichero limpio.

### Conservar / dejar / empezar

| Conservar | Dejar | Empezar |
|---|---|---|
| Un hábito que produjo evidencia. | Un hábito que produjo ruido o falsa confianza. | Un hábito que usarás en la próxima revisión. |
| | | |

Ejemplos, no prescripciones:

- Conservar: escribir el número de línea antes de escribir la afirmación.
- Dejar: llamar "alta" a un hallazgo sin una frase de contexto.
- Empezar: aplicar la prueba de dos revisores a cada elemento nuevo de la lista.

### Auditoría de afirmaciones

Elige cinco afirmaciones de tu portafolio. Etiqueta cada una y, cuando sea posible, nombra la
evidencia.

| # | Afirmación de mi portafolio | Estado | Evidencia | Qué la cambiaría |
|---:|---|---|---|---|
| 1 | | verificada / documentada / inferida / desconocida | | |
| 2 | | | | |
| 3 | | | | |
| 4 | | | | |
| 5 | | | | |

Si tres o más afirmaciones son inferidas o desconocidas, el portafolio es más delgado de lo que
parecía. Eso es un hallazgo sobre el proceso, no sobre ti. El trabajo de la retrospectiva es hacer
más fuerte el próximo portafolio.

### Qué cambió en tu forma de leer código

Escribe un párrafo. Debe nombrar un antes y un después, no una virtud.

Débil: "Aprendí a escribir mejor código."

Fuerte: "Antes leía el punto de entrada y seguía el camino feliz. Ahora leo primero las
declaraciones, cuento los escritores de cada global y compruebo los límites del bucle antes de
confiar en la salida. El cambio es que busco la clase antes de buscar la instancia."

### Autorrevisión basada en evidencia

La historia del archivo no es el tema. Tú tampoco eres el tema. El tema es el proceso que hizo que
una decisión pareciera suficiente.

Pregunta: ¿qué hizo que "límites comprobados" pareciera una prueba? ¿Qué hizo que una afirmación
segura sobreviviera sin fuente? ¿Qué habría detectado un segundo revisor en cinco minutos? Las
respuestas son defectos de proceso, y los defectos de proceso se pueden arreglar.

### La retrospectiva es sobre el proceso

No cuentes lecciones completadas y lo llames progreso. Una persona puede terminar veinte
lecciones y seguir escribiendo "límites comprobados". La retrospectiva mide la calidad del
instrumento y la honestidad de la evidencia, no el número de páginas leídas.

## Paso 5 — Explicar (12 min)

### La versión de 60 segundos

Escribe esto y dilo en voz alta. Si tarda más de 60 segundos, no está listo.

> "Esta es una lista de revisión para código heredado. Tiene cinco familias: corrección,
> diseño, robustez, práctica profesional y evidencia. Cada elemento nombra una pregunta, la
> evidencia que la responde, un criterio de aprobación, una clase de fallo y el escalón de
> Track A que previene la clase. La apliqué a una variante del archivo en 20 minutos y encontré
> X de 10 fallos esperados, con Y falsos positivos. La lista no prueba comportamiento en
> ejecución y no reemplaza las pruebas; hace que la revisión sea reproducible y las
> afirmaciones comprobables."

Sustituye X e Y por tus números. No redondees hacia arriba.

### Defender las familias

Para cada familia, responde dos preguntas: qué detecta y qué pasa cuando falta.

| Familia | Qué detecta | Cómo se ve una omisión |
|---|---|---|
| Corrección | Comportamiento erróneo y comportamiento indefinido. | Una escritura silenciosa fuera de límites llega a producción. |
| Diseño | Cambio caro y acoplamiento oculto. | Cada cambio de regla toca ocho sitios. |
| Robustez | Supuestos de entorno y de entrada. | La compilación falla en un segundo objetivo. |
| Práctica profesional | Falta de compilación, pruebas, documentación o árbol limpio. | Nadie puede reproducir o confiar en el artefacto. |
| Evidencia | Afirmaciones sin apoyo y desconocidos ocultos. | La confianza reemplaza a la prueba. |

Un revisor que no puede defender una familia la abandonará bajo presión de tiempo. La defensa es la
razón por la que la familia sobrevive a la próxima revisión bajo presión de tiempo.

### Qué no puede hacer la lista

Nombra al menos tres límites:

- No puede probar comportamiento en ejecución; solo una ejecución, una prueba o una traza pueden.
- No puede reemplazar el conocimiento del dominio; un revisor que no conoce las reglas omitirá una
  regla equivocada.
- No puede eliminar la necesidad de un segundo revisor; una pasada es una perspectiva.
- No puede volver preciso un elemento vago por estar en una lista.
- No puede arreglar el código; solo puede producir evidencia para un arreglo.

### Transferir a otro código

La lista se transfiere si las preguntas son genéricas. Pruébala en un código fuera de este proyecto.
Conserva las familias y la forma; cambia la columna de evidencia. Una lista que solo funciona en
`guerra-de-caballos` es un diagnóstico, no un instrumento.

### Enlace con Track A

Track B dice qué duele. Track A dice qué quita el dolor. El enlace es el escalón, no el eslogan.

| Familia de la lista | Fallo que detecta | Escalón de Track A | Prueba de cambio |
|---|---|---|---|
| Corrección | Off-by-one, estado sin inicializar, no determinismo. | `construction-02`, `construction-04`, `common-02` | Mismo cambio, localizado; traza sembrada dos veces. |
| Diseño | Estado global, duplicación, fuga de representación, switch de tipo. | `construction-08`, `construction-09`, `construction-12` | Tablero 8×8 → 10×10; solo cambia el módulo del tablero. |
| Robustez | Dependencia de plataforma, entrada sin límite, error no tratado. | `construction-20`, `construction-22` | Mismo núcleo, adaptadores de consola y de prueba. |
| Práctica profesional | Sin compilación, sin pruebas, sin documentación, árbol sucio. | `common-04`, `common-05`, `construction-24` | Un comando compila; las trazas pasan; el coste se defiende. |
| Evidencia | Afirmación sin fuente, desconocido oculto. | `common-03`, `common-05` | Cada término definido; cada traza reproducible. |

### El coste de una clase omitida

Un fallo de corrección omitido puede corromper memoria. Un fallo de diseño omitido encarece cada
cambio futuro. Un fallo de robustez omitido convierte una compilación en un proyecto de plataforma.
Un fallo de práctica omitido vuelve el artefacto inutilizable o poco confiable. Un fallo de
evidencia omitido vuelve la propia revisión poco confiable.

Los costes son distintos, y por eso las familias no son intercambiables. Un revisor que optimiza
para la familia más fácil producirá una revisión de aspecto limpio con un punto ciego predecible.

## Paso 6 — Registrar (8 min)

Rellena `EVIDENCE-es.md`. Como mínimo:

- el inventario del portafolio;
- la auditoría de la plantilla y las reescrituras;
- la lista con las ocho columnas;
- la muestra sellada, la pasada cronometrada y la tabla de hallazgos;
- la tasa de detección, los falsos positivos y las omisiones;
- la retrospectiva, la auditoría de afirmaciones y la tabla conservar/dejar/empezar;
- la comprobación de integridad del archivo.

Después confirma que el archivo está limpio:

```bash
cd /ruta/al/archivo
git status --short
```

La salida debe estar vacía. Si no lo está, editaste la evidencia. Revierte tu cambio, copia el
fichero a un directorio de trabajo y repite la inspección allí.

Guarda la lista en tu portafolio. Es el artefacto que sobrevive a la lección. La próxima revisión
empieza desde ella, no desde una página en blanco.

No se requiere commit ni push. Este es un registro de aprendizaje, no una contribución al archivo.

## El archivo en detalle: 19 fallos

Esta tabla es el enlace de vuelta a las clases de fallo y a los escalones de Track A. Úsala para
comprobar cobertura, no para memorizar la lista.

| ID | Fallo | Clase | Diagnosticado en | Escalón de Track A |
|---|---|---|---|---|
| failure-001 | `i <= 64` sobre un arreglo de 64 elementos | Corrección | archaeology-04 | `construction-02` |
| failure-002 | `void main()` | Corrección | archaeology-04 | `construction-02` |
| failure-003 | `srand(time(NULL))` dentro de un bucle de reintentos | Corrección | archaeology-04, archaeology-14 | `common-02` |
| failure-004 | Inicialización rota de punteros en `_dev` | Corrección | archaeology-04 | `construction-07` |
| failure-005 | Condición de victoria invertida en el precursor | Corrección | archaeology-03, archaeology-04 | `construction-00` |
| failure-006 | Entrada de nombre sin límite | Corrección | archaeology-04, archaeology-16 | `construction-22` |
| failure-007 | Estado global mutable | Diseño | archaeology-05 | `construction-08` |
| failure-008 | Ocho cuerpos `case` copiados a mano | Diseño | archaeology-06 | `construction-03` |
| failure-009 | Arreglos paralelos y variables sueltas del caballo | Diseño | archaeology-07 | `construction-04`, `construction-05` |
| failure-010 | `struct stablero` no es un tipo abstracto de datos | Diseño | archaeology-07, archaeology-09 | `construction-09` |
| failure-011 | `switch` sobre el número de jugada e `if (cab == 1)` | Diseño | archaeology-10 | `construction-12` |
| failure-012 | Entrada y salida dentro de la lógica de movimiento | Diseño | archaeology-11 | `construction-20` |
| failure-013 | Números mágicos | Diseño | archaeology-06, archaeology-07 | `construction-04` |
| failure-014 | `cout` de depuración y guarda `enter` viva | Diseño | archaeology-06, archaeology-12 | `construction-03` |
| failure-015 | `conio.h` y `system("color")` | Robustez | archaeology-16 | `construction-08`, `construction-20` |
| failure-016 | Sin pruebas, sin arnés, sin semilla | Práctica profesional | archaeology-14 | `common-02`, `common-05` |
| failure-017 | Sin README, especificación ni instrucciones de compilación | Práctica profesional | archaeology-13, archaeology-15 | `common-03`, `common-04` |
| failure-018 | Binarios y ficheros de IDE en las carpetas de trabajo | Práctica profesional | archaeology-15 | `common-04` |
| failure-019 | Comentarios que narran o mienten | Práctica profesional | archaeology-12 | `common-03` |

El mapeo es bidireccional. Si una revisión encuentra un fallo sin escalón, la taxonomía está
incompleta o el hallazgo todavía no se entiende. Si un escalón no tiene fallo, la lección es
teórica. Los dos casos merecen registrarse.

## Qué prueba y qué no prueba esta lista

| Afirmación | ¿Sostenida? | Por qué |
|---|---|---|
| La lista es reutilizable fuera de este archivo. | Solo después de la prueba de reutilización. | Las preguntas son genéricas; la evidencia no. |
| La lista encontró X de 10 fallos esperados. | Sí, para esta muestra y este revisor. | La pasada cronometrada y la tabla de hallazgos lo registran. |
| La lista detecta todos los fallos de corrección. | No. | Una variante y diez fallos esperados son una muestra. |
| Un fallo omitido es un defecto de la lista. | No siempre. | Puede ser un hueco del revisor o un tiempo acotado mal elegido. |
| El archivo no está modificado. | Sí, si `git status` está vacío. | El comando es la evidencia. |
| La revisión reemplaza una prueba. | No. | Una revisión no puede probar comportamiento en ejecución. |
| La retrospectiva es objetiva. | No. | Es un juicio estructurado con evidencia, no una medición. |

## Autocomprobación (binaria)

- [ ] Inventarié el portafolio y nombré los artefactos ausentes.
- [ ] Audité la plantilla y reescribí al menos cinco elementos vagos.
- [ ] Mi lista tiene al menos 15 elementos en las cinco familias.
- [ ] Cada elemento nombra pregunta, evidencia, criterio de aprobación, clase y escalón.
- [ ] Apliqué la lista a una variante sellada en una copia de trabajo.
- [ ] Registré la pasada cronometrada, los hallazgos y la evidencia de cada hallazgo.
- [ ] Calculé detección, omisiones y falsos positivos.
- [ ] Enuncié los límites de la muestra.
- [ ] Escribí la retrospectiva con un conservar, un dejar y un empezar.
- [ ] Audité cinco afirmaciones como verificadas, documentadas, inferidas o desconocidas.
- [ ] Expliqué la lista en 60 segundos y nombré tres límites.
- [ ] Ligé cada familia a un fallo y a un escalón de Track A.
- [ ] El archivo no está modificado.

## Malentendidos que esta lección refuta

**"Una lista de revisión es una lista de buenas prácticas."** Una buena práctica es una preferencia.
Un elemento de la lista es una prueba con evidencia, criterio de aprobación y clase. La diferencia
es si dos revisores pueden discrepar de forma productiva.

**"Más elementos significa una mejor revisión."** Más elementos significa más tiempo y más falsos
positivos. El objetivo es cubrir las clases, no el volumen. Quince elementos afilados superan a
cincuenta vagos.

**"Un fallo omitido significa que soy un mal revisor."** Un fallo omitido es un dato sobre el
instrumento o el tiempo acotado. Arregla el elemento, la fuente de evidencia o la regla de parada.
No conviertas un defecto de proceso en un juicio de carácter.

**"La retrospectiva es un resumen de lo que leí."** Un resumen no es una retrospectiva. La
retrospectiva nombra qué cambió en tu proceso, qué retirarías y qué harás distinto.

**"La lista reemplaza las pruebas."** La lista encuentra clases; las pruebas prueban comportamiento.
Una revisión que afirma corrección en ejecución sin una ejecución está sobreafirmando.

**"Sin culpa significa sin juicio."** Sin culpa significa juzgar la decisión y el sistema, no a la
persona. La revisión sigue teniendo severidad, evidencia y veredicto.

## Términos usados aquí

| Término | Significado en esta lección |
|---|---|
| **Elemento de la lista** | Una pregunta con evidencia, criterio de aprobación, clase de fallo y escalón. |
| **Tasa de detección** | Fallos esperados detectados dividido por fallos esperados. |
| **Precisión** | Hallazgos reales dividido por hallazgos reportados. |
| **Falso positivo** | Un hallazgo reportado que la evidencia no sostiene. |
| **Omisión** | Un fallo esperado que la lista no produjo. |
| **Muestra sellada** | Una variante inspeccionada sin el catálogo de fallos abierto. |
| **Tiempo acotado** | Una duración fija de la pasada que la termina aunque el fichero esté sin acabar. |
| **Auditoría de afirmaciones** | Etiquetar las afirmaciones del portafolio como verificadas, documentadas, inferidas o desconocidas. |
| **Conservar/dejar/empezar** | Un formato de retrospectiva con un hábito en cada columna. |
| **Sin culpa** | Juzgar la decisión y el sistema, no a la persona. |

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
