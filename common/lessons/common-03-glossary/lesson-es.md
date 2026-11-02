# common-03 — Glosario (glossary)

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** vocabulario compartido (shared vocabulary) ·
> **Prerrequisitos:** ninguno · **Tiempo estimado:** 20 minutos
> **Lectura:** ~13 minutos
> **Rol de la lección:** tronco

## Por qué existe esta lección

Una especificación se escribe primero con palabras y después con código. Si dos personas leen la
misma palabra y entienden dos decisiones distintas, toda comparación posterior se construye sobre
arena. La transcripción puede ser idéntica byte a byte mientras la conversación que la rodea no lo
es: una persona cree que una *semilla* es un secreto, la otra cree que es una entrada ordinaria; una
persona cree que un *contrato* es documentación, la otra cree que es comportamiento que puede
fallar.

Esta lección trata de esa capa. Seleccionas diez términos del glosario del proyecto, escribes una
línea que fija el significado de cada uno, nombras una concepción errónea que esa línea descarta y
después compruebas el mismo término contra la especificación. La comprobación no es decoración: es
la forma de descubrir si el vocabulario que usas es el vocabulario que el proyecto usa de verdad.

La lección no cambia código ni documentos canónicos. Su producto es una tabla que puedes defender:
diez términos, diez definiciones, diez concepciones erróneas y diez veredictos con referencias de
línea.

## Qué podrás hacer

Al terminar esta lección puedes:

- seleccionar diez términos que importan al juego, no diez palabras que suenan técnicas;
- escribir una definición de una línea que nombra una categoría y una diferencia;
- nombrar una concepción errónea plausible por término, no una errata ni un error trivial;
- buscar cada término en [`common/spec/game-rules-es.md`](../../spec/game-rules-es.md) y leer la
  línea en contexto;
- clasificar el resultado como consistente, sentido distinto o ausente, con una referencia de línea;
- explicar por qué un término compartido es un control de la conversación, no un sinónimo;
- registrar la predicción, los comandos, la tabla y la explicación en `EVIDENCE-es.md`.

## El concepto: vocabulario compartido

Un vocabulario compartido es un acuerdo sobre qué distinciones importan. No es una lista de
sinónimos. Dos palabras pueden señalar el mismo objeto y aun así codificar decisiones distintas; una
palabra puede señalar dos objetos y ocultar un desacuerdo. El glosario existe para hacer explícitas
esas distinciones y para que se puedan traducir.

Cinco ideas sostienen esta lección:

| Idea | Significado en esta lección |
|---|---|
| **Término** | Una palabra que nombra una distinción de la que depende el proyecto. |
| **Definición de una línea** | Una frase que sitúa el término en una categoría y nombra lo que lo diferencia. |
| **Concepción errónea** | Un modelo equivocado y estable que predice una acción equivocada, no una errata. |
| **Consistencia** | El mismo término conserva el mismo sentido en el glosario y en la especificación. |
| **Par de traducción** | Los nombres en inglés y en español conservan la misma distinción. |

### Qué debe hacer una definición de una línea

Una definición útil tiene dos partes: la categoría a la que pertenece el término y la diferencia que
lo separa de sus vecinos. Compara:

- Débil: *“Una semilla es algo aleatorio.”* No dice qué clase de cosa es una semilla y además es
  falso: la semilla es la entrada que hace repetible una secuencia aleatoria.
- Fuerte: *“Una semilla es el valor que inicializa una secuencia pseudoaleatoria determinista.”*
  Categoría: un valor. Diferencia: inicializa la secuencia y la hace repetible.

La versión fuerte se puede comprobar contra una línea de la especificación. La débil no: casi
cualquier valor aleatorio la satisfaría.

Una definición no es un eslogan. Mantenla en una frase, pero haz que la frase se gane su lugar.

### Qué debe hacer una concepción errónea

Una concepción errónea es un modelo equivocado que sobrevive a una explicación correcta. Es
plausible, es estable y lleva a una acción equivocada. “La semilla es una contraseña” es una
concepción errónea: es plausible porque ambas son valores cortos que se pasan a un programa, y lleva
a una acción equivocada, como ocultar la semilla o tratarla como un secreto.

“La semilla se escribe con cuatro letras” no es una concepción errónea. Es una observación
ortográfica. No predice ninguna decisión equivocada.

Para cada término, escribe la concepción errónea como una frase entre comillas y añade una cláusula
que diga por qué es incorrecta. Esa cláusula convierte una opinión en una comprobación.

## Antes de empezar

Lee estos documentos completos. Son breves y la lección depende de su redacción exacta:

- [`docs/guide/glossary-es.md`](../../../docs/guide/glossary-es.md) — la fuente de terminología. Lee
  todas las secciones, incluidos los términos escolares rechazados y la regla de mantenimiento.
- [`common/spec/game-rules-es.md`](../../spec/game-rules-es.md) — las catorce secciones de la
  especificación del juego. La buscarás, pero buscar no sustituye a leerla una vez.
- [`docs/guide/how-learning-works-es.md`](../../../docs/guide/how-learning-works-es.md) — la §4.5
  define la comprobación de ingeniería que se usa al final de esta lección.

También necesitas:

- una terminal con `grep` (o una herramienta de búsqueda equivalente);
- un archivo de texto plano para tu tabla de trabajo;
- [`docs/guide/glossary.md`](../../../docs/guide/glossary.md) si trabajas en inglés;
- [`common/spec/game-rules.md`](../../spec/game-rules.md) para la comprobación en inglés.

La especificación en inglés es la autoridad del comportamiento. La especificación en español es su
traducción; si las dos no coinciden, registra el desacuerdo en lugar de elegir la versión que
prefieras.

## El recorrido de 20 minutos

| Min | Paso | Qué haces | Evidencia |
|---|---|---|---|
| 0–3 | Seleccionar | Lees el glosario; eliges diez términos y registras por qué. | Lista de selección |
| 3–6 | Predecir | Para cada término, escribes la sección, el sentido y el veredicto esperados. | Tabla de predicción |
| 6–10 | Ejecutar | Buscas cada término en la especificación. | Comandos y coincidencias |
| 10–15 | Comparar | Lees cada coincidencia; asignas consistente, sentido distinto o ausente. | Tabla de diez filas |
| 15–18 | Explicar | Escribes por qué el vocabulario compartido es un control. | Un párrafo |
| 18–20 | Registrar | Completas `EVIDENCE-es.md` con la tabla y la explicación. | Entrada del portafolio |

Si un término tarda más, eso es información
sobre el término, no sobre ti.

### La comprobación de ingeniería en esta lección

| Paso | Dónde ocurre | Qué produce |
|---|---|---|
| **Predecir** | Paso 2 | La sección, el sentido y el veredicto esperados para cada término. |
| **Ejecutar** | Paso 3 | Los comandos de búsqueda y sus coincidencias. |
| **Comparar** | Paso 4 | La tabla de diez filas con un veredicto por término. |
| **Explicar** | Paso 5 | Un párrafo sobre por qué el vocabulario compartido es un control. |
| **Registrar** | Paso 6 | Un `EVIDENCE-es.md` completado. |

El artefacto es la tabla.

## Paso 1 — Seleccionar diez términos (3 min)

Estás eligiendo diez términos de los que depende el juego. Usa estas reglas:

1. Incluye al menos un término de cada una de estas secciones del glosario: Fundamentos, Diseño,
   Verificación y Vocabulario del proyecto.
2. Incluye un término que ya puedas definir sin mirar y otro que no puedas.
3. No selecciones términos de la sección de términos escolares rechazados; el glosario los rechaza a
   propósito.
4. Cada término debe poder usarse en una frase sobre el juego, el arnés o la evidencia.
5. Registra una cláusula que diga por qué el término está en tu conjunto.

Abajo hay un conjunto sugerido. Puedes reemplazar cualquier término, pero mantén la distribución y
registra el motivo de cada reemplazo.

| English | Español | Sección |
|---|---|---|
| Specification | Especificación | Diseño |
| Invariant (project sense) | Invariante (del proyecto) | Vocabulario del proyecto |
| Determinism | Determinismo | Verificación |
| Seed | Semilla | Verificación |
| Contract | Contrato | Diseño |
| Evidence | Evidencia | Verificación |
| Implementation | Implementación | Fundamentos |
| Reference | Referencia | Representación de datos |
| Golden trace | Traza de referencia | Verificación |
| Harness | Arnés (banco de pruebas) | Verificación |

El conjunto es deliberadamente mixto. Tres términos no se comportarán como espera un principiante:
*referencia* tiene un sentido distinto en la especificación, *traza de referencia* allí se llama
solo *traza*, y *arnés* aparece únicamente dentro de una ruta. No es una trampa. Encontrar esas
brechas es parte de la habilidad.

Si prefieres otro conjunto, elígelo ahora, antes de buscar. Cambiar el conjunto después de ver las
coincidencias destruye el paso de predicción.

## Paso 2 — Predecir antes de buscar (3 min)

Para cada uno de tus diez términos, escribe tres cosas en tu tabla de trabajo:

- la sección de la especificación donde esperas encontrarlo, o “ausente”;
- el sentido que esperas que use la especificación, en una línea;
- el veredicto que esperas: consistente, sentido distinto o ausente.

Después escribe una frase falsable sobre el conjunto completo. Por ejemplo:

> Espero que ocho de mis diez términos se usen en el mismo sentido que el glosario, que uno tenga un
> sentido distinto y que uno esté ausente de la especificación.

Una predicción no es una suposición que puedes reparar después. La brecha entre predicción y
observación es la señal de aprendizaje; editar la predicción después de la búsqueda la destruye.

### Una predicción resuelta

Toma *Semilla*. Antes de buscar, escribe:

- secciones esperadas: §3, *Posiciones iniciales*, y §12, *Aleatoriedad*;
- sentido esperado: el valor que inicia la secuencia determinista;
- veredicto esperado: consistente.

Esa predicción es lo bastante específica para fallar. Si la especificación usara *semilla* para
significar una contraseña del jugador, la predicción sería incorrecta, y ese error sería útil.

## Paso 3 — Ejecutar la búsqueda de consistencia (4 min)

La búsqueda es la “ejecución” de esta lección. Es de solo lectura: buscas evidencia, no cambias el
repositorio.

Fija la ruta del repositorio una vez y después busca en la especificación en español:

```bash
REPO="/ruta/a/38-guerra-de-caballos-lab"
cd "$REPO"

for term in especificación invariante determinismo semilla contrato evidencia \
  implementación referencia traza arnés; do
  printf '\n== %s ==\n' "$term"
  grep -n -i "$term" common/spec/game-rules-es.md || echo "(no encontrado)"
done
```

Sustituye la ruta por tu copia. Si reemplazaste un término, reemplaza la palabra en la lista.

Dos detalles importan:

- `-n` imprime el número de línea, así que la coincidencia se puede citar después.
- `-i` ignora mayúsculas y minúsculas, pero no resuelve las formas de la palabra. *Determinismo* es
  el sustantivo del glosario; la especificación usa el adjetivo *determinista*. Busca ambas formas
  cuando el sustantivo no aparezca:

```bash
grep -n -i -E "determinismo|determinista" common/spec/game-rules-es.md
```

Para la edición en inglés, ejecuta el mismo bucle contra la especificación en inglés:

```bash
for term in specification invariant determinism seed contract evidence implementation \
  reference trace harness; do
  printf '\n== %s ==\n' "$term"
  grep -n -i "$term" common/spec/game-rules.md || echo "(not found)"
done
```

Una coincidencia es un puntero, no un veredicto. `grep` puede decirte que una palabra aparece en la
línea 102; no puede decirte si la línea usa la palabra en el sentido del glosario. Lee cada
coincidencia en contexto, incluida la frase anterior y la posterior.

### Qué cuenta como coincidencia

| Observación | Qué significa |
|---|---|
| Aparece el término exacto y la frase coincide con el sentido del glosario. | Candidato a consistente. |
| Aparece otra forma de la palabra, como *determinista* por *determinismo*. | Revisa el sentido; la forma por sí sola no es un desacuerdo. |
| La misma palabra aparece con un sentido distinto. | Candidato a sentido distinto; registra ambos sentidos. |
| La palabra aparece solo como parte de una ruta o un nombre de archivo. | El concepto está ausente; registra la ruta y el concepto más cercano. |
| No hay ninguna coincidencia. | Ausente; registra “ausente” y el término más cercano que sí usa la especificación. |

## Paso 4 — Comparar glosario y especificación (5 min)

Para cada término, pon lado a lado la definición del glosario, la línea de la especificación y tu
veredicto. El veredicto tiene exactamente tres valores. No inventes un cuarto valor para evitar una
decisión.

| Veredicto | Prueba | Qué registrar |
|---|---|---|
| **Consistente** | El sentido del glosario encaja en la línea de la especificación sin reescribir ninguno de los dos. | El número de línea exacto y la frase citada. |
| **Sentido distinto** | La misma palabra nombra un concepto distinto en los dos documentos. | Ambos sentidos y la línea que muestra la diferencia. |
| **Ausente** | El término, como concepto, no aparece en la especificación. | “ausente”, el término más cercano y la línea donde aparece ese término. |

### Ejemplo resuelto 1 — Semilla (consistente)

- Glosario: *“El valor que inicializa una secuencia pseudoaleatoria.”*
- Concepción errónea: *“La semilla es una contraseña.”* Es incorrecta porque la semilla no es
  secreta; selecciona una secuencia y cualquiera que la conozca puede reproducir la ejecución.
- Especificación: línea 29 (“Dada una semilla:”), línea 102 (“La referencia usa un generador lineal
  congruencial de 64 bits”), línea 150 (“la misma semilla y el mismo nombre de jugador”).
- Veredicto: consistente. La especificación trata la semilla como una entrada que selecciona la
  secuencia. Nunca la trata como un secreto.

Este ejemplo muestra la forma de una fila completa. La definición es una frase, la concepción
errónea es un modelo equivocado con una razón, y el veredicto cita líneas reales.

### Ejemplo resuelto 2 — Referencia (sentido distinto)

- Glosario: *“Un alias a un objeto existente.”*
- Concepción errónea: *“Una referencia es un puntero.”* Es incorrecta porque una referencia es un
  alias con garantías distintas, no una dirección sobre la que puedas hacer aritmética.
- Especificación: línea 8 (“la referencia determinista”), línea 120 (“Implementación de
  referencia”).
- Veredicto: sentido distinto. La entrada del glosario trata de la indirección en el código; la
  especificación habla de la implementación canónica del juego.

Registra ambos sentidos. No finjas que son el mismo y no descartes el término en silencio. Una
palabra con dos sentidos es un riesgo de vocabulario: un lector puede estar de acuerdo con el
glosario y aun así malinterpretar la especificación.

### Ejemplo resuelto 3 — Arnés (ausente como concepto)

- Glosario: *“Un ejecutor que corre una variante y captura su comportamiento.”*
- Concepción errónea: *“Un arnés es un marco de pruebas.”* Es incorrecta porque un arnés ejecuta una
  variante y captura su comportamiento; no tiene que afirmar, simular ni informar como un marco de
  pruebas.
- Especificación: la cadena `harness` aparece solo dentro de la ruta `common/harness/reference.py`
  (línea 125). No hay definición ni uso de *arnés* como concepto.
- Veredicto: ausente como término. La especificación llama al mismo papel “implementación de
  referencia” en la §14.

Esto es un hallazgo sobre el alcance, no un fallo de tu trabajo. La especificación puede nombrar el
artefacto por su ruta y su papel en lugar de usar la palabra del glosario. Tu evidencia debe decirlo
con claridad: *“ausente como concepto; término más cercano: implementación de referencia, §14.”*

### La tabla de diez filas

Tu tabla de evidencia tiene una fila por término y estas columnas:

| English | Español | Definición de una línea | Una concepción errónea | Línea(s) de la especificación | Veredicto |
|---|---|---|---|---|---|

Escribe la definición con tus propias palabras. Copiar el glosario está permitido solo si puedes
explicar cada palabra; el objetivo es hacer tuyo el término, no transcribirlo.

Usa estas reglas cuando la fila sea difícil:

- Si un término tiene dos sentidos, pon ambos en la celda de definición, separados por un punto y
  coma, y explica la diferencia en la celda del veredicto.
- Si un término está ausente, escribe “ausente” en la celda del veredicto y nombra el término más
  cercano de la especificación.
- Si el término en español conserva el sentido pero el inglés no, registra una brecha de traducción
  en lugar de promediar los dos.
- Si la especificación contradice el glosario, la especificación gana para el comportamiento, pero
  el glosario sigue necesitando una corrección. Registra la contradicción; no edites ninguno de los
  dos archivos en esta lección.

### Las reglas de consistencia

1. La consistencia es de sentido, no de ortografía. *Determinismo* y *determinista* pueden ser
   consistentes.
2. Un término puede ser consistente en una sección y no en otra. Cita la sección que revisaste.
3. Una traducción debe conservar la distinción, no la palabra. Comprueba el término en español
   contra la especificación en español también.
4. La ausencia es un hallazgo. Te dice qué palabras eligió la especificación en su lugar.
5. Un veredicto sin referencia de línea es una opinión. Añade la línea o marca la fila incompleta.

## Paso 5 — Explicar (3 min)

Escribe un párrafo, sin palabras de sintaxis. Responde:

- ¿Por qué un término compartido es un control de la conversación y no una comodidad?
- ¿Qué no pueden afirmar dos personas cuando usan la misma palabra para sentidos distintos?
- ¿Cuál de tus diez términos fue más difícil de definir y qué reveló esa dificultad?
- ¿Qué término añadirías al glosario después de esta lección y por qué?

Una frase útil para terminar: *“Una definición no es una palabra más bonita para la cosa; es la
línea que nos dice cuándo seguimos hablando de lo mismo.”*

No nombres un lenguaje de programación, un tipo de dato ni un elemento de sintaxis. Si necesitas uno
para explicar la idea, la idea todavía no está separada de la implementación.

### Calibración

Compara tu tabla de predicción con los diez veredictos:

- Si los veredictos coinciden, tu modelo mental del vocabulario del proyecto funciona. Di qué
  término fue el más fácil de predecir y por qué.
- Si un veredicto difiere, la brecha nombra el término que debes estudiar después. No promedies el
  resultado ni lo llames “suficientemente cerca”.
- Si todos los veredictos coinciden pero las definiciones fueron difíciles de escribir, la brecha
  está en la articulación, no en el reconocimiento. Reescribe las tres definiciones más débiles.


## Paso 6 — Registrar (2 min)

Completa [`EVIDENCE-es.md`](EVIDENCE-es.md). Es tu entrada del portafolio. Incluye:

- los diez términos con nombres en inglés y en español;
- la tabla de predicción, escrita antes de la búsqueda;
- los comandos que ejecutaste y sus estados de salida;
- la tabla de diez filas con definiciones, concepciones erróneas, referencias de línea y veredictos;
- el párrafo de explicación;
- el siguiente paso.

## Autocomprobación (binaria)

- [ ] Leí el glosario completo, incluidos los términos rechazados y la regla de mantenimiento.
- [ ] Leí la especificación una vez antes de buscarla.
- [ ] Seleccioné diez términos y registré por qué está cada uno en el conjunto.
- [ ] Escribí la predicción antes de ejecutar la búsqueda.
- [ ] Busqué en la especificación y registré los comandos exactos.
- [ ] Leí cada coincidencia en contexto, no solo la palabra que coincide.
- [ ] Cada uno de los diez términos tiene nombre en inglés y en español.
- [ ] Cada término tiene una definición de una línea con mis propias palabras.
- [ ] Cada término tiene una concepción errónea plausible y una razón de por qué es incorrecta.
- [ ] Cada término tiene un veredicto con referencia de línea, o “ausente” con el término más
      cercano.
- [ ] Puedo explicar por qué el vocabulario compartido es un control, sin palabras de sintaxis.
- [ ] Completé `EVIDENCE-es.md` y sé cuál es mi siguiente paso.

Si una casilla no está marcada, la lección todavía no está terminada **para ti**. La casilla sin
marcar nombra el siguiente paso. No hay penalización por repetir un paso.

## Misconcepciones que esta lección refuta

| Concepción errónea | Por qué es incorrecta | Qué hacer en su lugar |
|---|---|---|
| “Un glosario es una lista de sinónimos.” | Una definición fija una decisión; un sinónimo solo da otra palabra. | Escribe la categoría y la diferencia. |
| “Si la palabra aparece en la especificación, el término es consistente.” | La misma ortografía puede llevar un sentido distinto. | Lee la línea y compara sentidos. |
| “Otra forma de la palabra significa otro concepto.” | *Determinismo* y *determinista* pueden nombrar la misma idea. | Revisa el sentido antes de llamarlo desacuerdo. |
| “La traducción es cosmética.” | Una traducción que cambia la distinción rompe el vocabulario compartido. | Comprueba el término en español contra la especificación en español. |
| “Una concepción errónea es cualquier error.” | Una errata no predice una decisión equivocada. | Nombra un modelo equivocado y estable, y la acción que causa. |
| “Una definición de una línea puede ser vaga.” | La vaguedad no se puede comprobar contra una línea de la especificación. | Nombra la categoría y la diferencia. |
| “Diez términos significa cobertura.” | El objetivo son diez términos que puedes usar, no una lista completada. | Elige términos de los que depende el juego. |
| “Si un término está ausente, el glosario está mal.” | La ausencia puede ser una elección deliberada de la especificación. | Registra el término más cercano y la brecha. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Vocabulario compartido | Un acuerdo sobre qué distinciones importan al proyecto. |
| Término | Una palabra que nombra una de esas distinciones. |
| Definición de una línea | Una frase con una categoría y una diferencia. |
| Concepción errónea | Un modelo equivocado y estable que lleva a una acción equivocada. |
| Consistencia | El mismo sentido en el glosario y en la especificación. |
| Par de traducción | Nombres en inglés y en español que conservan la misma distinción. |
| Veredicto | Consistente, sentido distinto o ausente, con una referencia de línea. |
| Calibración | La brecha entre el veredicto predicho y el observado. |

El glosario define *concepción errónea* y *calibración*; los demás términos de esta tabla son
locales a esta lección.

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
