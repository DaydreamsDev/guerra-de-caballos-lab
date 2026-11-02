# common-02 — Arnés determinista (deterministic harness)

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** construcción del arnés (harness construction) ·
> **Prerrequisitos:** common-01 · **Tiempo estimado:** 60 minutos ·
> **Lectura:** ~8 minutos
> **Rol de la lección:** tronco

## Por qué existe esta lección

Una traza es evidencia solo si el programa que la produjo no tiene entradas ocultas. El arnés es lo
bastante pequeño para leerlo: tres entradas, una secuencia pseudoaleatoria y un escritor de
transcripción. Si puedes nombrar esas partes, puedes decir si una diferencia vino del cambio que
pruebas o de algo que el arnés olvidó controlar.

`common-01` ya probó el invariante de la misma semilla. Esta lección no repite esa prueba. Abre el
arnés y responde otra pregunta: **¿qué hace que la salida dependa de la semilla y de nada más?**

## Qué podrás hacer

Al terminar esta lección puedes:

- nombrar las tres entradas del arnés de referencia;
- localizar las constantes del LCG de 64 bits y la máscara que mantiene el estado en rango;
- trazar el orden de las llamadas aleatorias que llegan a la transcripción;
- mostrar que la semilla 7 y la semilla 8 producen partidas distintas;
- nombrar una entrada oculta que rompería el arnés si llegara a la salida;
- explicar por qué una semilla fija es un control, no una prueba de corrección.

## El concepto: construcción del arnés (harness construction)

Un arnés es un programa pequeño con tres entradas y una secuencia determinista. Su trabajo es hacer
que la salida dependa de esas entradas y de nada más. El concepto no es «la aleatoriedad es buena»;
es «el camino de la semilla a los bytes es explícito y controlado».

| Idea | Significado en esta lección |
|---|---|
| **Entrada** | Todo lo que el programa lee: semilla, nombre del jugador, máximo de movimientos. |
| **Variable de control** | Una entrada que mantienes fija para que no pueda explicar una
diferencia. |
| **LCG** | El generador lineal congruencial de 64 bits que convierte la semilla en una secuencia. |
| **Camino de la semilla** | La línea donde la semilla inicializa el estado del generador. |
| **Orden de llamadas** | El orden en que se extraen los valores aleatorios y se escriben en la
transcripción. |
| **Atribución** | Saber qué entrada causó qué diferencia observada. |

La semilla no elimina la aleatoriedad. Hace que la aleatoriedad sea repetible. Un arnés con una
entrada oculta —un reloj, una variable de entorno, una colección sin orden— rompe esa propiedad
aunque la semilla esté fija.

## Antes de empezar

Lee estos documentos completos:

- [`common/harness/README-es.md`](../../harness/README-es.md) — la línea de comandos y las garantías
  de determinismo.
- [`common/spec/game-rules-es.md`](../../spec/game-rules-es.md) — sobre todo la §12, el orden exacto
  de las llamadas al generador aleatorio.
- [`common/spec/transcript-format-es.md`](../../spec/transcript-format-es.md) — qué contiene una
  transcripción.
- [`common/traces/README-es.md`](../../traces/README-es.md) — las trazas de referencia y cómo se
  verifican.

También necesitas:

- Python 3.8 o posterior y una terminal;
- un directorio temporal para las salidas (no agregues la salida de las ejecuciones al repositorio);
- la implementación de referencia: [`common/harness/reference.py`](../../harness/reference.py);
- el directorio de trazas: [`common/traces/`](../../traces/).

El arnés es la forma ejecutable de las reglas. Su determinismo no es un accidente de la
implementación; es un requisito de la §12, que fija el orden de cada llamada aleatoria.

## El recorrido de 60 minutos

| Min | Paso | Qué haces | Evidencia |
|---|---|---|---|
| 0–15 | Leer | Lees `reference.py` ; listas las tres entradas, las constantes del LCG y el orden de
llamadas. | Notas de entradas y LCG |
| 15–25 | Predecir | Predices semilla 7 vs semilla 8 y una entrada oculta que rompería el arnés. |
Nota de predicción |
| 25–35 | Ejecutar | Ejecutas la semilla 7 y la semilla 8 una vez cada una. | Dos transcripciones |
| 35–45 | Comparar | Comparas las dos semillas; registras la primera diferencia y los conteos. |
Diff y conteos |
| 45–55 | Trazar | Trazas la semilla, el LCG y las tres primeras llamadas aleatorias. | Notas de
traza |
| 55–60 | Explicar y registrar | Explicas el control; completas `EVIDENCE-es.md` . | Entrada del
portafolio |

Un paso puede tomar más; eso es información
sobre el paso, no sobre ti.

## Paso 1 — Leer el arnés (10 min)

Lee [`common/harness/reference.py`](../../harness/reference.py) con una sola pregunta en mente:
**¿qué puede cambiar la salida?**

Empieza en `build_transcript`. La función recibe tres valores: `seed`, `playerName` y
`maximumMoveCount`. Todo lo que escribe depende de esos tres valores y de nada más.

Sigue las llamadas aleatorias en orden:

1. `create_initial_positions` extrae dos celdas y vuelve a extraer la celda de la máquina mientras
   sea igual a la del jugador.
2. `create_initial_rolls` extrae dos tiradas y vuelve a extraer la tirada de la máquina mientras sea
   igual a la del jugador.
3. En cada turno, una llamada a `random_integer` extrae el número de movimiento inicial.

Esa es la lista completa. No hay lectura del reloj, ni variable de entorno, ni llamada de red, ni
recorrido de una colección desordenada que pueda llegar a la salida. El conjunto `visitedPositions`
se usa solo para comprobar pertenencia, nunca para construir una línea.

### Las tres entradas

| Entrada | Opción | Efecto en la transcripción |
|---|---|---|
| Semilla | `--seed` | Selecciona toda la secuencia: posiciones, tiradas, órdenes, partida. |
| Nombre del jugador | `--name` | Se copia en la línea de cabecera `player=`. |
| Máximo de movimientos | `--max-moves` | Cambia cuándo la partida termina en tablas; por defecto
`200` . |

Dos de estas se olvidan con facilidad. El nombre del jugador forma parte de los bytes aunque no
toque el generador aleatorio. El máximo de movimientos forma parte del control aunque sea opcional.

### Una pregunta antes de continuar

Si cambias solo el nombre del jugador, ¿cambia la partida? Predice la respuesta y luego razona desde
el código: el nombre se escribe después de las llamadas aleatorias y ninguna llamada aleatoria lo
lee. La partida debería ser idéntica salvo por la línea `player=`. Guarda esa predicción; puedes
probarla más adelante como extensión opcional.

## Paso 2 — Predecir (10 min)

Escribe la predicción **antes** de ejecutar nada. Predices causas, no dígitos aleatorios.

Responde al menos:

1. ¿Qué tres entradas pueden cambiar la transcripción?
2. ¿Qué partes de la transcripción dependen de la semilla?
3. ¿La semilla 7 y la semilla 8 producirán la misma partida? ¿Cuál es la primera línea que debe
   diferir?
4. Si el arnés leyera el reloj, una variable de entorno o una colección sin orden, ¿dónde aparecería
   primero la inestabilidad?
5. ¿Por qué el nombre de la persona cambia los bytes sin cambiar la partida?

Escribe una frase falsable, por ejemplo:

> La semilla 7 y la semilla 8 diferirán porque la semilla selecciona toda la secuencia de tiradas,
> inicios y órdenes de movimiento; el nombre solo cambiará la línea `player=`.

Una predicción no es una suposición que puedes corregir después. La brecha entre predicción y
observación es la señal de aprendizaje; editar la predicción después de ejecutar destruye esa señal.

## Paso 3 — Ejecutar (10 min)

Desde la raíz del repositorio, ejecuta una partida con cada semilla. No ejecutes la misma semilla
dos
veces; esa prueba pertenece a `common-01`.

```bash
mkdir -p "$HOME/common-02-scratch"
common/harness/run.sh --seed 7 --name Learner > "$HOME/common-02-scratch/seed-7.txt"
common/harness/run.sh --seed 8 --name Learner > "$HOME/common-02-scratch/seed-8.txt"
```

Comprueba ambos estados de salida:

```bash
common/harness/run.sh --seed 7 --name Learner > /dev/null && echo "seed 7 exit 0"
common/harness/run.sh --seed 8 --name Learner > /dev/null && echo "seed 8 exit 0"
```

Registra los comandos exactos. Un comando que no puedes reproducir no es evidencia.

La semilla 7 juega 10 movimientos exitosos y termina con `winner=player`. La semilla 8 termina con
`winner=machine` después de 26 movimientos exitosos. Son hechos sobre las semillas, no reglas.

## Paso 4 — Comparar las dos semillas (10 min)

Usa tres herramientas independientes; que coincidan es más fuerte que una sola.

### 4.1 Comparación de bytes

```bash
cmp -s "$HOME/common-02-scratch/seed-7.txt" "$HOME/common-02-scratch/seed-8.txt" \
  && echo "IDENTICAL" || echo "DIFFERENT"
```

El resultado esperado es `DIFFERENT`. No es un fallo; es la prueba de que la semilla selecciona la
partida.

### 4.2 Comparación con diff

```bash
diff -u "$HOME/common-02-scratch/seed-7.txt" "$HOME/common-02-scratch/seed-8.txt"
```

Registra la primera línea que difiere y los campos que cambian. Comprueba si también cambian las
tiradas, los inicios, las posiciones, los movimientos y el resultado, o solo la línea de la semilla.

### 4.3 Conteo de turnos

Cuenta las líneas `turn=` de cada transcripción:

```bash
grep -c '^turn=' "$HOME/common-02-scratch/seed-7.txt"
grep -c '^turn=' "$HOME/common-02-scratch/seed-8.txt"
```

Los conteos son distintos porque las partidas son distintas. Registra ambos números.

### 4.4 Qué prueba esto

Una diferencia entre la semilla 7 y la semilla 8 prueba que la semilla hace trabajo real. **No**
prueba que ninguna de las dos partidas sea correcta. La corrección se comprueba contra las reglas y
las trazas de referencia.

## Paso 5 — Trazar la semilla por el arnés (10 min)

Abre [`common/harness/reference.py`](../../harness/reference.py) y localiza el generador aleatorio.

Registra:

- las cuatro constantes del LCG y la máscara;
- la línea donde la semilla inicializa el estado;
- el orden de las tres primeras llamadas aleatorias;
- un lugar donde se actualiza el estado;
- un lugar donde el valor generado llega a la transcripción.

Después responde: si se quitara la máscara, ¿qué cambiaría primero? Si se intercambiara el orden de
las llamadas, ¿qué línea de la transcripción se movería? No necesitas editar el arnés; razona desde
el código.

## Paso 6 — Explicar por qué se requiere una semilla fija (5 min)

Escribe un párrafo, sin palabras de sintaxis. Responde:

- ¿Qué necesita una comparación antes de poder atribuir una diferencia a una causa?
- Si la semilla cambiara entre los dos lados de una comparación, ¿qué dejarías de poder afirmar?
- ¿Por qué “parecía aleatorio” no excusa una ejecución no reproducible?
- ¿Cuál es la diferencia entre eliminar la aleatoriedad y hacerla repetible?

Una frase útil para terminar: *“Una semilla fija no es una comodidad; es el control que hace que una
diferencia signifique algo.”*

No nombres un lenguaje de programación, un tipo de dato ni un elemento de sintaxis. Si necesitas uno
para explicar la idea, la idea todavía no está separada de la implementación.

### La trampa que debes evitar

La explicación equivocada tentadora es: *“La semilla hace que la salida sea correcta.”* No es así.
Una semilla fija hace que la salida sea **repetible**. La corrección es una pregunta distinta, que
se responde comparando la transcripción con las reglas y con las trazas de referencia. Una respuesta
equivocada reproducible sigue siendo equivocada.

## Paso 7 — Registrar (5 min)

Completa [`EVIDENCE-es.md`](EVIDENCE-es.md). Es tu entrada de portafolio. Incluye:

- la predicción escrita antes de ejecutar;
- los comandos y estados de salida para la semilla 7 y la semilla 8;
- la primera línea que difiere y los campos que cambiaron;
- los conteos de `turn=`;
- las constantes del LCG, la línea de la semilla y las tres primeras llamadas aleatorias;
- el párrafo de explicación;
- el siguiente paso.

## Autocomprobación (binaria)

- [ ] Leí el arnés y nombré sus tres entradas.
- [ ] Localicé las constantes del LCG y la máscara.
- [ ] Trazé las tres primeras llamadas aleatorias.
- [ ] Escribí la predicción antes de ejecutar.
- [ ] Ejecuté la semilla 7 y la semilla 8 una vez cada una.
- [ ] `cmp` reporta las dos transcripciones como diferentes.
- [ ] Registré la primera línea que difiere y los campos que cambiaron.
- [ ] Registré el conteo de `turn=` para ambas semillas.
- [ ] Nombré una entrada oculta que rompería el arnés si llegara a la salida.
- [ ] Puedo explicar por qué una semilla fija es un control, no una prueba de corrección.
- [ ] Completé `EVIDENCE-es.md` y sé mi siguiente paso.

## Misconcepciones que esta lección refuta

| Concepción errónea | Por qué es incorrecta | Qué hacer en su lugar |
|---|---|---|
| “Determinista significa que no hay aleatoriedad.” | La partida está llena de extracciones
aleatorias; el determinismo significa que se repiten. | Di “con semilla” o “repetible”, no “sin
aleatoriedad”. |
| “Basta la semilla; el nombre no importa.” | El nombre se copia en la transcripción, así que cambia
los bytes. | Mantén fija cada entrada, no solo la semilla. |
| “Dos ejecuciones idénticas prueban que el programa es correcto.” | Prueban reproducibilidad. Un
programa reproducible puede violar las reglas. | Compara con la especificación y las trazas. |
| “Cualquier semilla sirve para comparar.” | Cualquier semilla *fija* sirve, pero ambos lados deben
usar la misma. | Fija una semilla y regístrala con el resultado. |
| “Resúmenes iguales prueban corrección.” | Un resumen solo prueba que dos secuencias de bytes son
iguales. | Usa el resumen para identidad y las reglas para corrección. |
| “La semilla es un secreto o una contraseña.” | Es una entrada ordinaria que selecciona una
secuencia. | Trátala como parte de la línea de comandos. |
| “Una salida distinta significa que el arnés está roto.” | Otra semilla debe producir otra partida.
| Compara ejecuciones con la misma semilla para probar estabilidad; varía la semilla para probar
sensibilidad. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Determinismo | Las mismas entradas y la misma semilla producen la misma salida. |
| Reproducibilidad | Una ejecución independiente bajo los mismos controles produce los mismos bytes.
|
| Semilla | El valor que inicializa la secuencia pseudoaleatoria. |
| Pseudoaleatorio | Una secuencia repetible que se comporta estadísticamente como valores
aleatorios. |
| Variable de control | Una entrada que se mantiene fija para que no pueda explicar una diferencia.
|
| Atribución | Asignar una diferencia observada a una causa específica. |
| Transcripción | El comportamiento observable de una ejecución, un registro por línea. |
| Resumen | Una huella de longitud fija de una secuencia de bytes. |

Las definiciones canónicas están en [`glossary-es.md`](../../../docs/guide/glossary-es.md).

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.


## Siguiente paso

- Continúa con `common-05` — trazas de referencia — y usa el arnés que acabas de leer para verificar
  el conjunto de trazas.
- Si quieres primero el lado del punto de entrada, ve a `common-04` — punto de entrada e
  independencia del directorio de trabajo.
- Si la semilla 7 y la semilla 8 produjeron la misma transcripción, detente e investiga antes de
  avanzar.
