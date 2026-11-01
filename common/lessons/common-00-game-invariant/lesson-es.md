# common-00 — El juego como invariante (the game as an invariant)

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** especificación (specification) ·
> **Prerrequisitos:** ninguno · **Tiempo estimado:** 45 minutos
> **Lectura:** ~9 minutos

## Por qué existe esta lección

El juego es el invariante. Cada variante de este laboratorio debe producir la misma transcripción
para la misma semilla. Esa promesa solo funciona si las reglas están escritas de modo que dos
lectores independientes coincidan en lo que el programa debe hacer.

Una especificación no es el código. Es la decisión sobre el comportamiento que el código debe
satisfacer. Si las reglas son ambiguas, dos implementaciones pueden afirmar que las siguen y aun así
discrepar. Entonces la transcripción deja de ser un contrato y se convierte en una opinión.

Esta lección entrena el primer movimiento de ingeniería: leer las reglas como decisiones, ejecutar
la referencia dos veces y convertir una ambigüedad en un criterio de aceptación binario.

## Qué podrás hacer

Al terminar esta lección puedes:

- separar una regla normativa de un ejemplo ilustrativo;
- ejecutar la referencia dos veces con la misma semilla y mostrar que las transcripciones son
  idénticas;
- nombrar una regla ambigua y explicar por qué es ambigua;
- escribir un criterio de aceptación binario que haga observable el comportamiento en disputa.

## El concepto: especificación (specification)

El glosario define **especificación** como *el comportamiento que un programa debe exhibir*. Cinco
ideas importan aquí:

| Idea | Significado en esta lección |
|---|---|
| **Especificación** | Una decisión sobre el comportamiento observable, escrita antes del código. |
| **Invariante** | El comportamiento que nunca cambia entre variantes: misma semilla, misma transcripción. |
| **Contrato** | La transcripción es el contrato de comportamiento entre las reglas y cada implementación. |
| **Ambigüedad** | Dos lectores competentes pueden seguir el texto y producir líneas observables distintas. |
| **Criterio binario** | Una prueba con dos resultados: pasa o todavía no. |

La fuente canónica de las cuatro capas es
[`conceptual-framework.md`](../../../docs/guide/conceptual-framework-es.md). Lee con atención la
capa de concepto: un concepto sobrevive a un cambio de lenguaje; una especificación sobrevive a un
cambio de implementación.

## Antes de empezar

Lee estos documentos completos:

- [`common/spec/game-rules.md`](../../spec/game-rules-es.md) — las 14 secciones que definen el
  juego.
- [`common/spec/transcript-format.md`](../../spec/transcript-format-es.md) — las formas de registro.

También necesitas:

- Python 3.8 o posterior y una terminal;
- un directorio temporal para las transcripciones (no agregues la salida de las ejecuciones al
  repositorio);
- la implementación de referencia: [`common/harness/reference.py`](../../harness/reference.py).

La referencia es la forma ejecutable de las reglas. Las trazas de referencia en
[`common/traces/`](../../traces/) son el contrato registrado para las semillas 1 a 20. Usa la
semilla 7: tiene una traza de referencia y no aparece en el ejemplo ilustrativo del formato de
transcripción.

## La ruta de 45 minutos

| Min | Paso | Qué haces | Evidencia |
|---|---|---|---|
| 0–10 | Leer | Lee las reglas y el formato de transcripción; marca reglas y ejemplos. | Tres reglas marcadas |
| 10–15 | Predecir | Escribe lo que mostrarán las dos ejecuciones, antes de ejecutar. | Nota de predicción |
| 15–20 | Ejecutar | Ejecuta la referencia dos veces con la misma semilla y jugador. | Dos transcripciones |
| 20–30 | Comparar | Compara A con B; compara la predicción con la observación. | Líneas exactas que difieren |
| 30–40 | Investigar | Nombra una ambigüedad; escribe un criterio de aceptación binario. | Ambigüedad + criterio |
| 40–45 | Explicar y registrar | Explica por qué importa; completa `EVIDENCE-es.md`. | Entrada del portafolio |

Un paso puede tomar más; eso es información
sobre el paso, no sobre ti.

## Paso 1 — Leer decisiones, no código (10 min)

Lee `game-rules.md` una vez para la forma y otra con lápiz. Marca:

1. **Lenguaje normativo:** *es*, *debe*, *extrae*, *vuelve a extraer hasta*, *gana*, *pierde*,
   *termina*.
2. **Números que limitan el comportamiento:** 8×8, 1..50, 1..8, 200 movimientos exitosos.
3. **Ejemplos:** el ejemplo de transcripción es explícitamente ilustrativo; las trazas de referencia
   son autoritativas para sus semillas.
4. **Lugares donde preguntas «¿cuál?» o «¿cuándo exactamente?»** Esos son los puntos de presión.

Una regla de lectura útil: una regla es normativa cuando cambia una línea observable. Si dos
lecturas no cambian ninguna línea, la diferencia es prosa, no comportamiento.

### Ejemplo resuelto: una contradicción que puedes resolver

La sección 4 dice:

- §4.3: si las tiradas son iguales, vuelve a extraer `roll_machine` hasta que difiera de
  `roll_player`.
- §4.5: si las tiradas son iguales, vuelve a extraer ambas tiradas.

Bajo §4.3, la igualdad se elimina antes de §4.4, así que §4.5 es inalcanzable. Bajo §4.5, el
procedimiento consume una secuencia distinta de valores aleatorios. Dos lectores pueden producir
encabezados y partidas distintas.

La sección 12 resuelve el orden de las llamadas: `roll_machine` se vuelve a extraer hasta que
difiera de `roll_player`. La especificación en conjunto tiene una respuesta, pero §4.5 la
contradice. La ambigüedad es real; la resolución también. Tu trabajo es hacer la resolución
comprobable.

### Puntos de presión para investigar (elige uno)

No lo trates como una lista de verificación. Úsalo para empezar tu propia lectura.

| Dónde | Pregunta que puede hacer un lector | Por qué puede cambiar la transcripción |
|---|---|---|
| §4.3 vs §4.5 | Con tiradas iguales, ¿se vuelve a extraer una tirada o ambas? | Distintas llamadas producen tiradas y comienzos distintos. |
| §10 + formato | ¿Un turno atrapado emite una línea `turn=` y consume una extracción `start`, o solo `no_legal_move`? | El registro de trampa puede ser una línea o varias. |
| §11 + §10 | En el movimiento 200, ¿la partida es empate o victoria por trampa? | Cambia la línea final `winner=`. |
| Encabezado | ¿Qué caracteres puede contener `player=<name>`? | Un salto de línea o un espacio puede romper el formato. |

Elige un punto de presión distinto del ejemplo §4.3 / §4.5 para tu propio criterio.

## Paso 2 — Predecir (5 min)

Escribe la predicción **antes** de ejecutar nada. La predicción trata sobre el invariante, no sobre
adivinar dígitos aleatorios.

Responde al menos esto:

1. ¿Las ejecuciones A y B serán idénticas byte a byte? ¿Por qué?
2. ¿Qué partes de la transcripción podrían diferir si el programa usara aleatoriedad sin semilla, el
   reloj o una colección sin orden?
3. Si el primer `start` fuera 5, ¿cuál sería el campo `order=`? Dedúcelo de §6 sin ejecutar.
4. ¿Qué forma tendrán las dos últimas líneas (`winner=...`, `moves=...`)?

Escribe una frase falsable, por ejemplo:

> Las ejecuciones A y B serán idénticas byte a byte porque la única aleatoriedad tiene semilla y el
> programa no lee otra entrada.

Una predicción no es una suposición que puedas corregir después. La brecha entre predicción y
observación es la señal de aprendizaje; editar la predicción después de la ejecución la destruye.

## Paso 3 — Ejecutar (5 min)

Usa la misma semilla y el mismo nombre de jugador en ambas ejecuciones. Desde la raíz del
repositorio:

```bash
mkdir -p "$HOME/common-00-scratch"
common/harness/run.sh --seed 7 --name Learner > "$HOME/common-00-scratch/run-a.txt"
common/harness/run.sh --seed 7 --name Learner > "$HOME/common-00-scratch/run-b.txt"
```

Luego compara los dos archivos:

```bash
diff -u "$HOME/common-00-scratch/run-a.txt" "$HOME/common-00-scratch/run-b.txt"
```

Si `diff` no imprime nada, las dos transcripciones son idénticas. Comprueba también el estado de
salida:

```bash
cmp -s "$HOME/common-00-scratch/run-a.txt" "$HOME/common-00-scratch/run-b.txt" \
  && echo "IDENTICAL" || echo "DIFFERENT"
```

Registra el comando exacto que usaste. Un comando que no puedes reproducir no es evidencia.

**Extensión opcional:** ejecuta con otra semilla y confirma que la transcripción cambia. Eso muestra
que la semilla hace su trabajo; no es necesario para esta lección.

## Paso 4 — Comparar (10 min)

Haz estas comprobaciones, en este orden.

### 4.1 Ejecución A contra ejecución B

El invariante dice: misma semilla, misma transcripción. El único resultado aceptable es **ninguna
diferencia**.

Si los archivos difieren, registra:

- la primera línea que difiere;
- el campo que difiere;
- la causa más pequeña que puedas defender (aleatoriedad sin semilla, tiempo, orden de iteración,
  entorno).

No «arregles» la comparación normalizando la salida. Normalizar oculta el defecto.

### 4.2 Ejecución A contra la traza de referencia

El conjunto de trazas es el contrato registrado. Compara tu ejecución con la traza de la misma
semilla:

```bash
diff -u common/traces/seed-0007.txt "$HOME/common-00-scratch/run-a.txt"
```

Un diff vacío significa que tu ejecución coincide con el comportamiento confirmado. Un diff no
vacío significa que tu comando usó otra semilla u otro nombre de jugador, o que la implementación y
la traza discrepan. Registra cuál.

### 4.3 Predicción contra observación

Compara tu nota de predicción con la transcripción real:

| Pregunta | Observación |
|---|---|
| ¿Fueron idénticas las dos ejecuciones? | sí / no |
| ¿Coincidió el campo `order=` con tu deducción? | sí / no; la línea exacta |
| ¿El encabezado tuvo los siete registros esperados? | sí / no; la línea exacta |
| ¿El resultado tuvo la forma esperada? | sí / no; la línea exacta |

Registra las líneas exactas que difieren. «Coincidió casi todo» no es evidencia.

### 4.4 Conformidad con el formato

Comprueba la transcripción contra `transcript-format.md`:

- los registros del encabezado aparecen una vez, en el orden documentado;
- cada línea `turn=` tiene ocho números de movimiento en `order=`;
- las líneas de intento empiezan con los dos espacios documentados;
- una captura añade `capture` a la línea del movimiento exitoso;
- el resultado tiene `winner=` y `moves=`.

Una transcripción puede ser determinista y aun así violar el formato. Ambas comprobaciones importan.

## Paso 5 — Investigar: una ambigüedad, un criterio binario (10 min)

Este es el entregable de la lección. Hazlo en tres movimientos: nombrar, probar, escribir.

### 5.1 Nombrar la ambigüedad

Una ambigüedad no es «no entiendo». Es una afirmación precisa:

> Bajo la lectura A, la transcripción contiene la línea X. Bajo la lectura B, contiene la línea Y.
> El texto sostiene ambas lecturas.

Una buena ambigüedad tiene tres propiedades:

1. Vive en texto normativo, no en un ejemplo ilustrativo.
2. Cambia al menos una línea observable.
3. No está ya resuelta por una sección que no leíste. Si otra sección la resuelve, dilo; entonces el
   problema es una contradicción o redundancia, que también vale la pena registrar.

### 5.2 Probar la ambigüedad

Para la regla que elijas, escribe:

- la referencia de sección;
- las dos lecturas, en una frase cada una;
- la línea observable que cambiaría;
- la entrada que expone la diferencia (semilla, nombre de jugador o posición del tablero).

Si no puedes nombrar una línea observable, tienes una intuición, no una ambigüedad. Sigue leyendo.

### 5.3 Escribir un criterio de aceptación binario

Usa esta forma:

```text
Dado <entrada>, la transcripción muestra <condición observable>.
Pasa: la condición se cumple.
Todavía no: la condición no se cumple.
```

Un buen criterio:

- nombra la entrada (por ejemplo, `seed=7` y `player=Learner`);
- nombra lo observable (un campo, una línea, un conteo, un diff);
- tiene exactamente dos resultados;
- no usa «correcto», «razonable» o «como se esperaba» sin una referencia;

**Ejemplo resuelto de criterio (no lo reutilices):**

- Ambigüedad: §10 y el formato de transcripción no dicen si un turno atrapado emite una línea
  `turn=` y líneas de intento, o solo la línea `no_legal_move`. La referencia y `seed-0007.txt`
  emiten la línea de turno y los ocho intentos.
- Criterio binario: *Dado `seed=7`, después de la última línea `move=` la transcripción contiene una
  línea `turn=`, ocho líneas `try=` y una línea `no_legal_move trapped=player`. Pasa cuando esa
  secuencia aparece exactamente; todavía no cuando falta o se reordena alguna línea.*

Por qué funciona: nombra una entrada, algo observable y dos resultados. No dice qué lectura «se
siente bien»; dice qué debe mostrar la transcripción.

**Complemento (opcional, 5 min):** toma la contradicción §4.3 / §4.5. La sección 4.3 vuelve a
extraer solo `roll_machine`; §4.5 vuelve a extraer ambas tiradas. Escribe las dos lecturas, la línea
observable y un criterio binario que distinga el orden de llamadas de §12 del orden de §4.5. Es
práctica; no es tu entregable.

**Tu turno (obligatorio):** elige una ambigüedad distinta del ejemplo de la trampa y escribe tu
propio criterio en `EVIDENCE-es.md`.

## Paso 6 — Explicar (5 min)

Escribe un párrafo, sin palabras de sintaxis. Responde:

- ¿Por qué una regla ambigua invalida toda comparación?
- ¿Qué supuesto trajiste que el texto no sostenía?
- ¿Qué necesitarías cambiar en las reglas para eliminar la ambigüedad: añadir una regla, eliminar
  una regla o nombrar una prioridad entre dos reglas?

Una frase útil para terminar: *«Una regla ambigua no es un pequeño error de documentación; significa
que dos implementaciones pueden afirmar que siguen las reglas y aun así discrepar.»*

No nombres un lenguaje de programación, un tipo de dato ni un elemento de sintaxis. Si necesitas uno
para explicar la idea, la idea todavía no se separó de la implementación.

## Paso 7 — Registrar (5 min)

Completa [`EVIDENCE-es.md`](EVIDENCE-es.md). Es tu entrada del portafolio. Incluye:

- la predicción, escrita antes de la ejecución;
- los comandos exactos y su salida;
- la diferencia, incluida «sin diferencias» cuando ese sea el resultado;
- la ambigüedad, con su referencia de sección;
- tu criterio de aceptación binario;
- el párrafo de explicación;
- el siguiente paso.

## Autocomprobación (binaria)

- [ ] Leí las 14 secciones de `common/spec/game-rules.md` y el formato de transcripción.
- [ ] Escribí la predicción antes de ejecutar la referencia.
- [ ] Ejecuté la referencia dos veces con la misma semilla y jugador.
- [ ] Las dos transcripciones son idénticas byte a byte, o registré la primera línea que difiere.
- [ ] Comparé la predicción con la observación y registré las líneas exactas.
- [ ] Comparé la ejecución con la traza de referencia de la misma semilla.
- [ ] Nombré una regla ambigua con su referencia de sección.
- [ ] Escribí un criterio de aceptación binario con una entrada y una condición observable.
- [ ] Expliqué por qué importa la ambigüedad sin usar palabras de sintaxis.
- [ ] Completé `EVIDENCE-es.md` y sé cuál es mi siguiente paso.

Si una casilla no está marcada, la lección todavía no está terminada **para ti**. La casilla sin
marcar nombra el siguiente paso. No hay penalización por repetir un paso.

## Misconcepciones que esta lección refuta

| Concepción errónea | Por qué es incorrecta | Qué hacer en su lugar |
|---|---|---|
| «El código es la especificación.» | El código es evidencia de una implementación; la especificación es la decisión que debe satisfacer. | Lee las reglas y luego prueba el código contra ellas. |
| «El ejemplo es la especificación.» | El ejemplo es ilustrativo; las trazas son autoritativas para sus semillas. | Usa el ejemplo para la forma, no para resolver una disputa. |
| «Si ejecuta, es correcto.» | Un programa puede ejecutar y aun así violar el contrato. | Compara transcripciones y comprueba el formato. |
| «Una ambigüedad es un problema menor.» | Permite que dos implementaciones discrepen mientras ambas afirman cumplir. | Escribe un criterio binario que haga observable la diferencia. |
| «Un criterio necesita una nota para ser útil.» | Una nota oculta la decisión; una condición binaria la expone. | Usa pasa / todavía no, con la salida observada. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Especificación | El comportamiento que un programa debe exhibir. |
| Invariante | La propiedad que debe cumplirse siempre entre variantes. |
| Determinismo | La misma entrada y semilla producen la misma salida. |
| Semilla | El valor que inicializa la secuencia pseudoaleatoria. |
| Transcripción | El comportamiento observable de una ejecución, un registro por línea. |
| Criterio de aceptación | Una condición observable y binaria que define si el artefacto cumple su objetivo. |
| Ambigüedad | Dos lecturas del mismo texto producen comportamientos observables distintos. |

Las definiciones canónicas están en [`glossary.md`](../../../docs/guide/glossary-es.md).

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
