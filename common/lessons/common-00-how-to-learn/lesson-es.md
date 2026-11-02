# common-00 — Cómo aprender (how to learn)

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** metacognición (metacognition) · **Prerrequisitos:** ninguno ·
> **Tiempo estimado:** 15 minutos
> **Lectura:** ~9 minutos

## Por qué existe esta lección

Todas las demás lecciones de este laboratorio te piden construir algo y comprobarlo. Esta hace otra
pregunta: **¿sabes lo que sabes, y cómo lo averiguarías?**

Esa pregunta no es filosofía. Es la habilidad que decide si la próxima hora sirve. Quien no
distingue «entendí la explicación» de «puedo predecir el comportamiento» vuelve a leer la misma
página y vuelve a sorprenderse con los mismos fallos. Quien distingue ambas cosas sabe exactamente
dónde poner el siguiente esfuerzo.

La herramienta es pequeña y poco vistosa: **predecir → ejecutar → comparar → explicar → registrar**.
La vas a ejecutar una vez, en una tarea lo bastante pequeña como para terminarla en minutos, y
después escribirás un autodiagnóstico con cuatro preguntas. La brecha entre lo que esperabas y lo
que ocurrió es todo el asunto.

Sin nota, sin examen, sin fecha límite, sin grupo. Nada aquí se puntúa y nadie lo aprueba.

## Qué podrás hacer

Al terminar esta lección puedes:

- ejecutar un ciclo completo de predecir → ejecutar → comparar → explicar → registrar en una tarea
  pequeña;
- nombrar la brecha exacta entre tu predicción y la observación, en lugar de «coincidió casi todo»;
- explicar la brecha nombrando la suposición equivocada que la produjo;
- responder las cuatro preguntas del autodiagnóstico y convertirlas en un siguiente paso;
- distinguir un autodiagnóstico de una autocalificación.

## El concepto: metacognición (metacognition)

La **metacognición** es conocimiento sobre tu propio conocimiento: qué puedes hacer sin ayuda, qué
solo reconoces cuando lo ves y qué estás adivinando. Su forma práctica es la **calibración**: cuánto
se acercan tus predicciones a la realidad.

| Idea | Significado en esta lección |
|---|---|
| **Metacognición** | Saber lo que sabes y cómo lo averiguarías. |
| **Calibración** | El tamaño de la brecha entre lo que predijiste y lo que ocurrió. |
| **Predicción** | Una afirmación falsable sobre comportamiento observable, escrita antes de ejecutar. |
| **Brecha** | Las líneas o valores exactos donde la predicción y la observación discrepan. |
| **Explicación** | La suposición equivocada que produjo la brecha, dicha en lenguaje llano. |
| **Autodiagnóstico** | Cuatro preguntas que terminan en una decisión, no en una nota. |

Se siguen dos cosas, y es fácil confundirlas:

- Una brecha **pequeña** significa que tu modelo mental funciona para esta tarea. No significa que
  seas «bueno».
- Una brecha **grande** significa que encontraste el lugar exacto para estudiar después. No
  significa que hayas fracasado.

La brecha es información. Es la única retroalimentación que este método necesita.

### El ciclo

Los cinco pasos son los mismos de toda lección con artefacto. Aquí son lo bastante baratos como para
ejecutarlos en minutos.

| Paso | Qué haces | Qué conservas |
|---|---|---|
| **Predecir** | Escribes qué ocurrirá, antes de ejecutar nada. | Una frase falsable. |
| **Ejecutar** | Ejecutas la tarea tal como la escribiste, con la salida visible. | El comando y su salida. |
| **Comparar** | Pones la predicción junto a la observación. | La línea o el valor exacto que difiere. |
| **Explicar** | Dices por qué existe la brecha; nombras la suposición equivocada. | Un párrafo, sin palabras de sintaxis. |
| **Registrar** | Guardas predicción, comando, salida y explicación. | La entrada en `EVIDENCE-es.md`. |

No es un examen. Es a libro abierto, sin tiempo, sin nota, y el comportamiento observado es la
respuesta.

### Las cuatro preguntas

El autodiagnóstico viene de
[`docs/guide/how-learning-works.md`](../../../docs/guide/how-learning-works.md) §4.4. Cuatro
preguntas, sin niveles, sin puntuación:

1. **¿Puedo explicarlo?** Sin notas, sin palabras de sintaxis.
2. **¿Puedo cambiarlo?** A pedido, con un costo medido.
3. **¿Puedo ampliarlo?** A un requisito que no he visto.
4. **¿Coincidió mi predicción?** Si no, ¿qué parte de mi modelo mental estaba equivocada?

Si alguna respuesta es «todavía no», el siguiente paso es explícito: repetir la tarea, releer el
ejemplo resuelto o volver al prerrequisito. En una lección de quince minutos, «todavía no» en las
preguntas 2 y 3 es normal e informativo; describen trabajo que lleva más que un ciclo.

## Antes de empezar

Lee estas secciones completas:

- [`docs/guide/how-learning-works.md`](../../../docs/guide/how-learning-works.md) §4 — verificación
  sin notas, en especial §4.4 (autodiagnóstico) y §4.5 (la comprobación de ingeniería).
- [`docs/guide/verification.md`](../../../docs/guide/verification.md) §4 y §5 — el ciclo y las
  cuatro preguntas tal como los usa el proyecto.

También necesitas:

- una terminal y unos quince minutos;
- una tarea pequeña cuya salida puedas observar;
- [`EVIDENCE-es.md`](EVIDENCE-es.md) abierto en un editor;
- opcionalmente, [`common/harness/reference.py`](../../harness/reference.py) para el ejemplo
  resuelto.

«Pequeña» es un requisito, no una sugerencia. Si la tarea tarda más de cinco minutos en ejecutarse,
es demasiado grande para un ciclo. Pártela hasta que la observación quepa en una pantalla.

## El camino de 15 minutos

| Min | Paso | Qué haces | Evidencia |
|---|---|---|---|
| 0–2 | Elegir | Eliges una tarea pequeña; anotas qué hace ahora. | Nota de la tarea |
| 2–5 | Predecir | Escribes una predicción falsable con una razón. | Frase de predicción |
| 5–7 | Ejecutar | Ejecutas una vez; capturas el comando y la salida. | Comando + salida |
| 7–10 | Comparar | Nombres la línea o el valor exacto que difiere. | La brecha |
| 10–12 | Explicar | Nombres la suposición equivocada detrás de la brecha. | Un párrafo |
| 12–15 | Registrar | Completas `EVIDENCE-es.md`, respondes las cuatro preguntas y decides el siguiente paso. | Entrada del portafolio |

Los tiempos son una sugerencia, no una fecha límite. Si el ciclo tarda veinte minutos, la lección
funcionó igual.

## Paso 1 — Elegir una tarea pequeña (2 min)

Elige una tarea con tres propiedades:

- se ejecuta en menos de cinco minutos;
- su salida es observable como texto, número o estado de salida;
- no estás completamente seguro de qué hará.

La última propiedad es la que se suele saltar. Si ya sabes la respuesta, el ciclo no enseña nada:
escribirás una predicción, la verás cumplirse y aprenderás que ya sabías algo. No es una pérdida,
pero no es el ejercicio.

### Ejemplo resuelto: el turno de la trampa

Úsalo para ver la forma de un ciclo. No lo copies como propio; ejecuta el tuyo.

**Tarea.** Contar los turnos y los movimientos exitosos de una traza de referencia, y predecir la
relación entre ambos números antes de contar.

**Predicción.** La traza tiene `moves=21`, y cada movimiento exitoso ocurre en un turno, así que el
archivo debería contener veintiuna líneas `turn=`. Razón: un turno por movimiento exitoso, sin
excepciones.

**Ejecución.**

```bash
grep -c '^turn=' common/traces/seed-0001.txt
grep -c '^move=' common/traces/seed-0001.txt
tail -3 common/traces/seed-0001.txt
```

**Observación.** Las cuentas son `22` y `21`. Las últimas tres líneas son una línea `turn=`, una
línea `no_legal_move trapped=player` y `winner=machine`.

**La brecha.** Un turno más que movimientos. La predicción suponía que todo turno termina en un
movimiento exitoso.

**Explicación.** La partida termina en trampa: el caballo activo no tiene movimiento legal, así que
el turno queda registrado —incluido su campo `order=`— pero no sigue ninguna línea `move=`. El turno
consumió igualmente una tirada de `start`, porque §6 sortea el orden antes de intentar los
movimientos, y la trampa se descubre después de ese sorteo. Mi modelo decía «turno implica
movimiento»; las reglas dicen «turno implica intento, y los intentos pueden fallar todos».

**Registro.** Predicción, dos comandos, las dos cuentas, las últimas tres líneas, el párrafo.

Eso es un ciclo completo. Fíjate en qué lo hizo funcionar: la predicción era lo bastante específica
para estar equivocada (`21`) y la observación era lo bastante pequeña para explicarla por completo
(`22`).

### Un menú, si quieres uno

Elige uno, o trae el tuyo:

| Tarea | Qué predecir | Cómo observar |
|---|---|---|
| Forma de la cabecera | Cuántos registros de cabecera tiene una traza y en qué orden | `head -7` sobre una traza |
| Orden de movimientos | El campo `order=` cuando el sorteo es 5 | `game-rules.md` §6, luego una traza |
| Efecto del nombre | Si cambiar solo el nombre del jugador cambia la partida | Dos ejecuciones, un `diff` |
| Mensaje del resumen | El texto exacto de una línea de resumen que pasa | `sha256sum -c checksums.txt` |
| Tu propia herramienta | La salida, el estado de salida o el mensaje de un comando que usas a menudo | Ejecutarlo |

La última fila es la más útil a largo plazo y la menos dependiente de este repositorio.

## Paso 2 — Predecir (3 min)

Escribe la predicción **antes** de ejecutar nada. Usa esta forma:

```text
Espero <valor o línea observable>.
Porque <la regla o el mecanismo que creo que se aplica>.
```

La segunda línea es lo que hace explicable la brecha después. Una predicción sin razón solo puede
decirte que te equivocaste; con una razón, te dice *qué* estaba mal.

Una predicción es falsable cuando otra persona podría estar en desacuerdo. «Funcionará» no es
falsable. «El archivo contendrá cuarenta líneas `turn=` porque cada movimiento ocupa un turno» sí lo
es.

No ejecutes la tarea mientras escribes la predicción. No mires la respuesta «solo para comprobar».
El valor del ciclo está en la brecha; mirar antes la borra.

## Paso 3 — Ejecutar (2 min)

Ejecuta la tarea una vez, exactamente como la describiste, y captura la salida. No un resumen de la
salida: la salida.

```bash
<tu comando> 2>&1 | tee "$HOME/common-00-run.txt"
echo "exit=$?"
```

`tee` conserva lo que viste. El estado de salida importa tanto como el texto: un comando puede
imprimir un mensaje útil y aun así fallar.

Si la ejecución produjo más de una pantalla de salida, elegiste una tarea demasiado grande. No la
analices; reduce la tarea y ejecuta de nuevo.

## Paso 4 — Comparar (3 min)

Pon la predicción y la observación una al lado de la otra. Nombra la diferencia con precisión:

- un valor, con ambos números: predije `21`, observé `22`;
- una línea, citada exactamente, con la marca del diff que indica de qué lado viene;
- un estado de salida, con ambos valores;
- una ausencia: «esperaba una línea y no hubo ninguna».

«Coincidió casi todo» no es una comparación. Tampoco «está bastante cerca». La brecha o es cero o
tiene un lugar, y encontrar el lugar es la habilidad que entrena esta lección.

Si la predicción coincidió exactamente, no te detengas: **hazla más precisa**. Añade una segunda
afirmación: la cuenta exacta, la última línea exacta, el estado de salida, el texto del mensaje.
Después ejecuta otra vez. Una predicción que siempre se cumple porque dice poco no es calibración;
es una tautología.

### Por qué la brecha es el entregable

El criterio de aceptación de esta lección pide una brecha registrada. Es deliberado. Un ciclo sin
brecha no puso a prueba tu modelo; confirmó que elegiste algo que ya sabías. La salida útil del
ciclo es la frase que empieza con *«Supuse que…»*.

## Paso 5 — Explicar (2 min)

Escribe un párrafo, sin palabras de sintaxis, que haga tres cosas:

1. enuncie la suposición en la que se apoyaba la predicción;
2. enuncie qué muestra la observación en su lugar;
3. enuncie la regla o el mecanismo que hace correcta la observación.

Para el ejemplo resuelto: *«Supuse que un turno siempre termina en un movimiento exitoso. La
observación muestra un turno más que movimientos. Las reglas sortean un orden de movimientos para
cada turno y después intentan los movimientos; cuando ninguno es legal, el turno queda registrado y
la partida termina, así que el último turno existe sin movimiento.»*

No nombres un lenguaje de programación, un tipo de dato ni un elemento de sintaxis. Si necesitas
uno, la idea sigue pegada a la superficie.

### La diferencia entre explicar y excusar

- **Explicación:** «Supuse X; las reglas dicen Y; por eso el número difiere.»
- **Excusa:** «Es un caso raro.» / «La salida es confusa.» / «Lo habría visto con más tiempo.»

Una excusa cierra la brecha. Una explicación la convierte en una regla que puedes usar la próxima
vez.

## Paso 6 — Registrar y autodiagnosticar (3 min)

Completa [`EVIDENCE-es.md`](EVIDENCE-es.md). Es tu registro, no una entrega. Después responde las
cuatro preguntas por escrito, en este orden:

1. **¿Puedo explicarlo?** Di la explicación en voz alta, sin notas. Si tienes que buscar una nota,
   escribe «todavía no» y nombra la parte que perdiste.
2. **¿Puedo cambiarlo?** Nombra un cambio que podrías hacer y cuánto costaría: cuántos archivos,
   cuántas líneas, cuántos conceptos.
3. **¿Puedo ampliarlo?** Nombra un requisito que no hayas visto y di si podrías intentarlo ahora.
4. **¿Coincidió mi predicción?** Si no, cita la suposición equivocada del Paso 5.

Después escribe el siguiente paso. Debe tener una de estas cuatro formas:

- repetir este ciclo con una predicción más precisa;
- releer la regla específica que te sorprendió;
- ir a la lección que enseña el concepto detrás de la brecha;
- avanzar, porque la brecha está cerrada y la explicación se sostiene sin notas.

### El autodiagnóstico no es una autocalificación

No hay puntuación, ni nivel, ni porcentaje, ni comparación con nadie. Las cuatro preguntas producen
una decisión: qué hacer después. «Todavía no» es un lugar, no un veredicto. Responder «todavía no»
con honestidad es el uso correcto de la herramienta; responder «sí» para quedar bien la inutiliza.

## Autocomprobación (binaria)

- [ ] Elegí una tarea que se ejecuta en menos de cinco minutos y cuya salida es observable.
- [ ] Escribí una predicción con un valor y una razón, antes de ejecutar.
- [ ] Ejecuté la tarea una vez y capturé el comando, la salida y el estado de salida.
- [ ] Nombré la brecha por su valor o línea exactos, no como «coincidió casi todo».
- [ ] Registré al menos una brecha entre predicción y observación.
- [ ] Expliqué la brecha nombrando la suposición equivocada.
- [ ] Respondí las cuatro preguntas del autodiagnóstico por escrito.
- [ ] Registré un siguiente paso, en una de las cuatro formas permitidas.
- [ ] Puedo decir por qué esto fue un diagnóstico y no una nota.

Si una casilla no está marcada, la lección todavía no está terminada **para ti**. La casilla sin
marcar nombra el siguiente paso.

## Misconcepciones que esta lección refuta

| Concepción errónea | Por qué es falsa | Qué hacer en su lugar |
|---|---|---|
| «Predecir es adivinar.» | Una predicción enuncia un observable esperado y la regla detrás; una adivinanza no enuncia ninguna de las dos. | Escribe el valor y la razón. |
| «Una predicción equivocada significa que fracasé.» | La brecha es la retroalimentación; localiza la suposición equivocada. | Registra la brecha y explícala. |
| «Una predicción correcta significa que lo aprendí.» | Puede significar que la tarea era demasiado fácil para probar algo. | Haz la predicción más precisa hasta que pueda fallar. |
| «El autodiagnóstico es autocalificarse.» | No tiene puntuación, ni nivel, ni comparación; produce un siguiente paso. | Responde con honestidad y decide la siguiente acción. |
| «Puedo corregir la predicción después de ver el resultado.» | Eso borra la única señal que produce el ciclo. | Escribe la predicción primero y no la cambies. |
| «Lo entendí porque la explicación tenía sentido.» | Reconocer no es recordar, y ninguna de las dos es predecir. | Explícalo sin notas y después predice el comportamiento. |
| «Una tarea más grande da mejor evidencia.» | Una tarea grande esconde la brecha en el ruido. | Reduce la tarea hasta que la salida quepa en una pantalla. |
| «Todavía no significa volver al principio.» | Nombra una pieza concreta que falta. | Repite solo el paso al que apunta la brecha. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Metacognición | Conocimiento sobre tu propio conocimiento y cómo ponerlo a prueba. |
| Calibración | Cuánto se acercan tus predicciones a lo que ocurre. |
| Nota de predicción | El comportamiento esperado, escrito antes de ejecutar el artefacto. |
| Brecha | El lugar exacto donde la predicción y la observación discrepan. |
| Autodiagnóstico | Cuatro preguntas que terminan en una decisión, no en una nota. |
| Comprobación de ingeniería | Predecir, ejecutar, comparar, explicar, registrar. |
| Alfabetización en retroalimentación | Juzgar el trabajo contra criterios y actuar sobre la evidencia. |

Las definiciones canónicas están en [`glossary-es.md`](../../../docs/guide/glossary-es.md).

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
