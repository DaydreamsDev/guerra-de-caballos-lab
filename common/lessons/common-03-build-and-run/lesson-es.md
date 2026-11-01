# common-03 — Punto de entrada y directorio de trabajo (entry point and working directory)

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** robustez del punto de entrada (entry-point robustness) ·
> **Prerrequisitos:** common-01 · **Tiempo estimado:** 30 minutos
> **Lectura:** ~11 minutos

## Por qué existe esta lección

Un comando que solo funciona desde un directorio no es un comando reproducible. Puede funcionar en
tu máquina, en tu shell, el día que lo escribiste, y fallar para la siguiente persona que lo ejecuta
desde la raíz del repositorio o desde un directorio temporal. Antes de comparar dos ejecuciones,
necesitas un punto de entrada que se comporte igual desde donde lo inicies.

La referencia no tiene una fase de compilación y enlace. Es cierto, pero no es la lección. La
lección
es el punto de entrada: el shell resuelve la ruta antes de que el programa arranque, y un envoltorio
que resuelve su propio directorio elimina el directorio de trabajo de quien llama.

En esta lección ejecutas la misma semilla desde la raíz del repositorio y desde otro directorio,
comparas los bytes y explicas qué línea del envoltorio hace que funcione la ruta absoluta. La
lección
no cambia código; cambia cómo lo ejecutas y lo observas.

## Qué podrás hacer

Al terminar esta lección puedes:

- ejecutar [`common/harness/run.sh`](../../harness/run.sh) desde la raíz del repositorio con un
  comando;
- ejecutar el mismo punto de entrada desde otro directorio sin copiar el arnés;
- explicar qué hace `run.sh` con su propia ubicación y por qué eso lo hace independiente del
  directorio;
- explicar por qué una ruta relativa desde otro directorio falla antes de que el envoltorio
  arranque;
- demostrar que la misma semilla y el mismo nombre producen la misma transcripción desde ambas
  ubicaciones;
- registrar la predicción, los comandos, la versión del intérprete, los resúmenes y la explicación
  en `EVIDENCE-es.md`.

## El concepto: robustez del punto de entrada

Un **punto de entrada** es el comando que inicia el programa. La **robustez del punto de entrada**
significa que el comando funciona desde cualquier directorio de trabajo, con los mismos argumentos y
el mismo intérprete.

El shell resuelve la ruta antes de que el envoltorio arranque. Una ruta relativa se resuelve contra
el
directorio actual; una absoluta no. Por eso el envoltorio debe localizar su propio directorio en
lugar de asumir el de quien llama.

| Idea | Significado en esta lección |
|---|---|
| **Punto de entrada** | El comando que inicia el programa; aquí, `common/harness/run.sh`. |
| **Directorio de trabajo** | El directorio que el shell usa para resolver rutas relativas. |
| **Ruta relativa** | Una ruta resuelta contra el directorio actual. |
| **Ruta absoluta** | Una ruta que no depende del directorio actual. |
| **Independencia de ubicación** | El envoltorio encuentra la referencia desde cualquier directorio.
|
| **Robustez del punto de entrada** | El mismo comando y las mismas entradas producen el mismo
comportamiento desde cualquier directorio. |

### Qué hace el envoltorio

El envoltorio pregunta al shell dónde vive el script, cambia a ese directorio, guarda la ruta
absoluta
y ejecuta la referencia que está junto a él. El directorio de trabajo de quien llama nunca se
convierte en el directorio de la referencia.

### Qué no significa

La independencia de ubicación no significa que una ruta relativa funcione desde cualquier lugar. No
funciona. El shell resuelve primero la ruta relativa; si el archivo no está, el envoltorio nunca
arranca. La ruta absoluta es la que llega al envoltorio desde otro directorio.

Tampoco significa que el entorno sea irrelevante. La versión del intérprete, los argumentos y la
semilla siguen siendo controles. La lección elimina una variable —el directorio de quien llama—, no
todas.

## Antes de empezar

Lee estos documentos completos:

- [`common/harness/README-es.md`](../../harness/README-es.md) — requisitos, línea de comandos y la
  frase sobre la ausencia de paso de compilación.
- [`common/harness/run.sh`](../../harness/run.sh) — el punto de entrada de seis líneas.
- [`common/harness/reference.py`](../../harness/reference.py) — la implementación de referencia; lee
  las importaciones y `build_transcript`.
- [`common/traces/README-es.md`](../../traces/README-es.md) — las trazas de referencia y cómo se
  verifican.
- [`common/spec/game-rules-es.md`](../../spec/game-rules-es.md) — la §12, el orden de las llamadas
  aleatorias, y la §13, la transcripción como contrato de comportamiento.

También necesitas:

- un shell POSIX con `bash`;
- Python 3.8 o posterior;
- `cmp`, `sha256sum` y `diff`;
- un directorio temporal para las salidas (no agregues la salida de las ejecuciones al repositorio).

El arnés es la forma ejecutable de la especificación. Su determinismo no es un accidente de la
implementación; es un requisito de la §12, que fija el orden de cada llamada aleatoria.

## El recorrido de 30 minutos

| Min | Paso | Qué haces | Evidencia |
|---|---|---|---|
| 0–5 | Leer | Lees `run.sh` , el README del arnés y los puntos de entrada de la referencia. | Notas
sobre las seis líneas |
| 5–9 | Predecir | Escribes qué hará cada comando desde cada directorio. | Nota de predicción |
| 9–14 | Ejecutar | Ejecutas la semilla 7 desde la raíz del repositorio. | `root.txt` y estado de
salida |
| 14–19 | Ejecutar | Ejecutas la semilla 7 desde otro directorio; pruebas la ruta relativa como
control. | `elsewhere.txt` y estado de salida |
| 19–24 | Comparar | Comparas bytes y resúmenes. | `cmp`, SHA-256, diff vacío |
| 24–27 | Explicar | Explicas por qué no hay paso de compilación y qué hay que controlar. | Un
párrafo |
| 27–30 | Registrar | Completas `EVIDENCE-es.md` con comandos, versiones y observaciones. | Entrada
del portafolio |

Si un paso tarda más, eso es información sobre
el paso, no sobre ti.

### La comprobación de ingeniería en esta lección

| Paso | Dónde ocurre | Qué produce |
|---|---|---|
| **Predecir** | Paso 2 | Los estados de salida y la igualdad de bytes para cada directorio. |
| **Ejecutar** | Pasos 3 y 4 | Dos transcripciones, una desde cada directorio de trabajo. |
| **Comparar** | Paso 5 | `cmp`, dos resúmenes y dos diffs. |
| **Explicar** | Paso 6 | Un párrafo sobre por qué no hay paso de compilación. |
| **Registrar** | Paso 7 | Un `EVIDENCE-es.md` completado. |

El artefacto es el par de transcripciones.

## Paso 1 — Leer el punto de entrada (5 min)

Lee [`common/harness/run.sh`](../../harness/run.sh). Tiene seis líneas, incluida la línea en blanco.
Cada línea tiene una función:

```bash
#!/usr/bin/env bash
set -euo pipefail

scriptDirectory="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

exec python3 "${scriptDirectory}/reference.py" "$@"
```

Esto es lo que hace cada parte, en orden:

1. La primera línea selecciona el shell que ejecuta el script.
2. `set -euo pipefail` hace que el script se detenga ante un error, una variable sin definir o una
   tubería fallida. El envoltorio no puede ocultar un fallo y salir con éxito.
3. `scriptDirectory` pregunta al shell dónde vive el propio script, entra en ese directorio, imprime
   la ruta absoluta y la guarda. El valor no depende de dónde llamaste al script.
4. `exec` reemplaza el proceso del shell por `python3` y pasa cada argumento sin cambios. No hay
   copia intermedia ni reescritura de argumentos.

La línea importante es la tercera. `BASH_SOURCE[0]` es la ruta del script tal como la ve el shell.
Si la ruta es relativa, el subproceso entra en su directorio y `pwd` devuelve una ruta absoluta. Si
la ruta es absoluta, el resultado sigue siendo absoluto. En ambos casos, `reference.py` se encuentra
junto al envoltorio, no junto a quien lo llama.

### Una pregunta antes de continuar

Si ejecutas `common/harness/run.sh` desde la raíz del repositorio, el shell encuentra el script
porque la ruta relativa existe. Si ejecutas la misma ruta relativa desde `/tmp`, el shell no puede
encontrar el script: busca `/tmp/common/harness/run.sh`, que no existe. El envoltorio nunca arranca,
así que su lógica de ubicación nunca se ejecuta.

Si ejecutas la ruta absoluta desde `/tmp`, el shell encuentra el script, el envoltorio calcula su
propio directorio y `reference.py` se encuentra. La partida debería ser idéntica.

Guarda esa distinción: *el shell resuelve el punto de entrada antes de que el envoltorio se
ejecute*. La independencia de ubicación pertenece al envoltorio, no a la ruta de quien llama.

## Paso 2 — Predecir (4 min)

Escribe la predicción **antes** de ejecutar nada. Estás prediciendo estados de salida e igualdad de
bytes, no adivinando dígitos aleatorios.

Responde al menos esto:

1. Desde la raíz del repositorio, ¿`common/harness/run.sh --seed 7 --name Learner` saldrá con `0`?
   ¿Por qué?
2. Desde `/tmp`, ¿el mismo comando relativo saldrá con `0`? ¿Qué dirá el shell?
3. Desde `/tmp`, ¿la ruta absoluta a `run.sh` saldrá con `0`? ¿Por qué?
4. ¿Producirán las dos ejecuciones exitosas transcripciones idénticas byte a byte? ¿Qué podría
   hacerlas diferentes?
5. ¿Qué línea de `run.sh` hace que la ejecución absoluta funcione desde otro directorio?
6. Si `run.sh` llamara a `python3 reference.py` sin `scriptDirectory`, ¿qué pasaría desde `/tmp`?
7. ¿Qué predice la lógica de ubicación del envoltorio sobre las salidas? ¿Qué no predice?

Escribe una frase falsable, por ejemplo:

> La ejecución desde la raíz y la ejecución por ruta absoluta producirán transcripciones idénticas
> byte a byte porque el directorio de trabajo no se escribe en la salida; la ejecución relativa
> desde `/tmp` fallará antes de que Python arranque.

Una predicción no es una suposición que puedes corregir después. La brecha entre predicción y
observación es la señal de aprendizaje; editar la predicción después de la ejecución la destruye.

### Una predicción resuelta

Toma la pregunta 3. Antes de ejecutar, escribe:

- estado de salida esperado: `0`;
- razón esperada: el shell encuentra la ruta absoluta, `scriptDirectory` se convierte en el
  directorio del arnés y `exec python3` recibe los mismos argumentos;
- resumen esperado: el mismo que la ejecución desde la raíz.

Esa predicción es lo bastante específica para fallar. Si la ejecución absoluta fallara, la razón
estaría en el envoltorio o en el entorno, no en la partida.

## Paso 3 — Ejecutar desde la raíz del repositorio (5 min)

Fija la ruta del repositorio una vez y después ejecuta desde la raíz. Sustituye la ruta por tu
copia.

```bash
REPO="/ruta/a/38-guerra-de-caballos-lab"
cd "$REPO"
mkdir -p "$HOME/common-03-scratch"

python3 --version
common/harness/run.sh --seed 7 --name Learner > "$HOME/common-03-scratch/root.txt"
echo "root exit=$?"
```

El `echo` imprime el estado de salida del comando anterior. Registra el estado exacto y la versión
exacta del intérprete. “Python 3” no es suficiente: la versión forma parte de la cadena de
herramientas.

Comprueba la forma de la salida:

```bash
wc -l "$HOME/common-03-scratch/root.txt"
tail -n 2 "$HOME/common-03-scratch/root.txt"
```

Para la semilla 7 y el jugador `Learner`, la transcripción tiene 41 líneas y termina con:

```text
winner=player
moves=10
```

Ese resultado es un hecho sobre la semilla 7, no una regla. No lo generalices a otras semillas.

## Paso 4 — Ejecutar desde otro directorio (5 min)

Cambia a un directorio fuera del repositorio. `/tmp` está bien; usa un directorio temporal donde
puedas escribir. Ejecuta el mismo punto de entrada por ruta absoluta, con la misma semilla y el
mismo nombre.

```bash
cd /tmp
"$REPO/common/harness/run.sh" --seed 7 --name Learner \
  > "$HOME/common-03-scratch/elsewhere.txt"
echo "elsewhere exit=$?"
```

La ruta entre comillas es la única diferencia con el comando desde la raíz. Los argumentos son
idénticos. Si la transcripción cambia, el cambio vino de la ubicación o del entorno, no de la
partida.

Ahora ejecuta el control: la ruta relativa desde el mismo directorio.

```bash
cd /tmp
common/harness/run.sh --seed 7 --name Learner > /dev/null
echo "relative exit=$?"
```

El estado esperado es `127`, y el shell informa que el archivo no existe. La redacción exacta
depende de tu configuración regional; regístrala tal como la ves. Esto no es un error del arnés. El
shell resuelve la ruta relativa antes de que el envoltorio se ejecute, y
`/tmp/common/harness/run.sh` no está ahí.

El control importa porque separa dos afirmaciones:

| Afirmación | Qué significa | Cómo se prueba |
|---|---|---|
| El punto de entrada es independiente de la ubicación. | El envoltorio encuentra la referencia
desde cualquier directorio. | La ruta absoluta desde `/tmp` sale con `0` . |
| Una ruta relativa es independiente de la ubicación. | Una ruta relativa a quien llama funciona en
todas partes. | La ruta relativa desde `/tmp` sale con `127` . |

La primera afirmación es verdadera. La segunda es falsa, y se supone que lo sea. Una ruta se
resuelve contra el directorio de trabajo; un envoltorio que resuelve su propio directorio es lo que
elimina la dependencia.

## Paso 5 — Comparar (5 min)

Esta es la prueba. Usa tres herramientas independientes; que coincidan es más fuerte que cualquiera
de ellas por separado.

### 5.1 Comparación byte a byte

```bash
cmp -s "$HOME/common-03-scratch/root.txt" "$HOME/common-03-scratch/elsewhere.txt" \
  && echo "IDENTICAL" || echo "DIFFERENT"
```

`cmp` termina con `0` cuando los archivos son idénticos byte a byte. El único resultado aceptable
aquí es `IDENTICAL`.

### 5.2 Comparación de resúmenes

```bash
sha256sum "$HOME/common-03-scratch/root.txt" "$HOME/common-03-scratch/elsewhere.txt"
```

Los dos resúmenes deben coincidir exactamente. Para la semilla 7 y el jugador `Learner`, el resumen
esperado es:

```text
b8ff5f7999322ac770a5b37056e86f7eb4fe8c71ea3f4fd5c457cc720af27dba
```

Si tu resumen difiere, detente e investiga antes de cambiar nada. Causas habituales: otra semilla,
otro nombre de jugador, otro `--max-moves`, una redirección que agregó salida extra o un archivo de
salida que ya existía y recibió contenido agregado.

### 5.3 Comparación con diff

```bash
diff -u "$HOME/common-03-scratch/root.txt" "$HOME/common-03-scratch/elsewhere.txt"
```

`diff` no imprime nada y termina con `0` cuando los archivos coinciden. Un diff vacío es la forma
visual del mismo hecho.

### 5.5 Qué prueba la comparación

La transcripción no contiene el directorio de trabajo. Contiene la semilla, el nombre del jugador,
las tiradas, las posiciones, los movimientos y el resultado. Dos ejecuciones desde directorios
distintos no tienen ningún campo donde diferir, siempre que se usen el mismo punto de entrada y el
mismo intérprete. La comparación es evidencia de independencia de ubicación, no una coincidencia.

## Paso 6 — Explicar el punto de entrada (3 min)

Escribe un párrafo, sin palabras de sintaxis. Responde:

- qué resuelve el shell antes de que arranque el envoltorio;
- por qué la ruta relativa desde otro directorio falla;
- qué línea del envoltorio hace que funcione la ruta absoluta;
- por qué el directorio de trabajo de quien llama no forma parte de la transcripción;
- qué controla la lección y qué deja sin controlar.

Una frase útil para terminar: *«El envoltorio es independiente de la ubicación porque resuelve su
propio directorio; la ruta de quien llama no.»*

No nombres un lenguaje de programación, un tipo de dato ni un elemento de sintaxis. Si necesitas uno
para explicar la idea, la idea todavía no está separada de la implementación.

## Paso 7 — Registrar (3 min)

Completa [`EVIDENCE-es.md`](EVIDENCE-es.md). Es tu entrada del portafolio. Incluye:

- la predicción, escrita antes de ejecutar;
- la ruta del repositorio, la ruta temporal y la versión del intérprete;
- el comando y el estado de salida de la ejecución desde la raíz;
- el comando y el estado de salida de la ejecución absoluta desde otro directorio;
- el resultado del control con la ruta relativa;
- los dos resúmenes SHA-256 y el resultado de `cmp`;
- la primera línea que difiere, si existe;
- el párrafo de explicación;
- el siguiente paso.

## Autocomprobación (binaria)

- [ ] Leí `run.sh` y puedo explicar qué hace `scriptDirectory`.
- [ ] Escribí la predicción antes de ejecutar.
- [ ] Ejecuté la semilla 7 y el nombre `Learner` desde la raíz; el estado de salida fue `0`.
- [ ] Ejecuté la ruta absoluta desde otro directorio; el estado de salida fue `0`.
- [ ] Ejecuté la ruta relativa desde otro directorio y observé el fallo esperado.
- [ ] `cmp` reporta las dos transcripciones exitosas como idénticas.
- [ ] Expliqué por qué funciona la ruta absoluta y por qué no la relativa.
- [ ] Completé `EVIDENCE-es.md` y sé mi siguiente paso.

## Misconcepciones que esta lección refuta

| Concepción errónea | Por qué es incorrecta | Qué hacer en su lugar |
|---|---|---|
| “Sin paso de compilación no hay entorno.” | El intérprete, su versión y la biblioteca estándar
forman parte de la ejecución. | Registra la versión del intérprete con el comando. |
| “Si funciona desde la raíz, funciona en todas partes.” | Una ruta relativa depende del directorio
de trabajo. | Ejecuta la ruta absoluta desde otro directorio. |
| “El envoltorio cambia la partida.” | `run.sh` solo localiza y ejecuta `reference.py` ; reenvía los
argumentos sin cambios. | Compara resúmenes, no suposiciones. |
| “Se requiere una compilación para la reproducibilidad.” | El código interpretado puede ser
reproducible si la cadena de herramientas y las entradas están fijas. | Fija el intérprete, los
argumentos y la semilla. |
| “Reproducibilidad de compilación significa binarios idénticos.” | Aquí lo observable es una
transcripción, no un binario. | Compara los bytes de la transcripción. |
| “Basta la misma semilla; el directorio no importa.” | El directorio afecta la resolución de rutas
antes de que el programa arranque. | Ejecuta desde dos directorios y compara. |
| “La versión de Python no puede importar.” | La semántica del lenguaje y la biblioteca estándar
pueden cambiar entre versiones. | Registra la versión; usa Python 3.8 o posterior. |
| “Un diff vacío prueba corrección.” | Prueba identidad o independencia de ubicación. | Compara la
transcripción con la especificación y la traza. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Intérprete | Un programa que lee el código fuente y lo ejecuta. |
| Punto de entrada | El comando que inicia el programa. |
| Directorio de trabajo | El directorio usado para resolver rutas relativas. |
| Independencia de ubicación | El punto de entrada funciona desde cualquier directorio de trabajo. |
| Robustez del punto de entrada | El mismo comando y las mismas entradas producen el mismo
comportamiento desde cualquier directorio. |
| Transcripción | El comportamiento observable de una ejecución, un registro por línea. |
| Resumen | Una huella de longitud fija de una secuencia de bytes. |

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.



## Siguiente paso

- Continúa con `common-04` — trazas de referencia — y usa el punto de entrada que acabas de
  verificar
  para reproducir una traza.
- Si quieres primero los detalles del arnés, ve a `common-01` — arnés determinista.
- Si la ejecución por ruta absoluta falló, no avances. Registra el estado de salida y el primer
  error
  antes de cambiar nada.
