# common-04 — Trazas de referencia (golden traces)

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** contrato de comportamiento (behavioral contract) ·
> **Prerrequisitos:** common-01 · **Tiempo estimado:** 45 minutos
> **Lectura:** ~13 minutos

## Por qué existe esta lección

Un archivo de traza es solo texto. Por sí solo no prueba nada: cualquiera puede pegar veinte líneas
que parezcan correctas. Lo que convierte una traza en evidencia es la cadena que la rodea: el
comando que produjo la observación, la salida observada y la explicación que las conecta con una
regla.

Esta distinción importa porque los equipos suelen saltársela. «Coincide con la referencia» es una
afirmación. «Aquí está el comando, aquí está el diff vacío y aquí está por qué un diff vacío es el
resultado esperado» es evidencia. La primera nadie más puede comprobarla; la segunda sí.

El laboratorio guarda su contrato de comportamiento como veinte trazas en
[`common/traces/`](../../traces/). En esta lección verificas su integridad, reproduces una desde
cero y después explicas qué prueba esa reproducción y qué no prueba.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar por qué una traza de referencia es un contrato de comportamiento y qué deja fijado;
- verificar el conjunto de trazas contra [`checksums.txt`](../../traces/checksums.txt) y leer el
  resultado correctamente, incluido su texto traducido;
- reproducir una traza y mostrar un diff vacío, con el comando y la salida registrados;
- ejecutar un control negativo que produzca un diff no vacío y explicar qué aísla;
- enunciar qué no prueban veinte trazas que pasan;
- registrar el comando, la salida observada y la explicación como una sola pieza de evidencia.

## El concepto: contrato de comportamiento (behavioral contract)

Un **contrato de comportamiento** es un acuerdo sobre la conducta observable: dadas las mismas
entradas, una implementación debe producir la misma salida. El conjunto de trazas es donde ese
acuerdo queda escrito.

| Idea | Significado en esta lección |
|---|---|
| **Traza de referencia** | Una salida esperada registrada, conservada como referencia para una entrada. |
| **Contrato de comportamiento** | La misma semilla y el mismo nombre deben producir la misma transcripción. |
| **Autoridad** | La traza manda para su propia semilla; la especificación manda sobre ambas. |
| **Regresión** | Un cambio que rompe una traza ha cambiado comportamiento acordado, a propósito o no. |
| **Control** | Una ejecución contra la cual comparar para que una diferencia signifique algo. |
| **Evidencia** | Comando + salida observada + explicación, juntos y reproducibles por otra persona. |

Tres frases sostienen la lección:

1. La traza registra comportamiento; no describe intención.
2. El resumen criptográfico prueba que los bytes no cambiaron desde que se registraron; no prueba
   nada sobre si esos bytes son correctos.
3. La reproducción prueba que el código actual sigue produciendo esos bytes; solo la explicación
   dice por qué eso importa.

### Qué fija una traza y qué deja abierto

Una traza fija un camino a través de un programa. Dice: para esta semilla, esta es la secuencia de
turnos, intentos y el resultado. No dice que el programa sea correcto para toda semilla, y no dice
que las reglas se hayan entendido. Son afirmaciones distintas, que se comprueban con medios
distintos.

[`game-rules.md`](../../spec/game-rules.md) §13 enuncia el acuerdo con claridad: *dos
implementaciones coinciden cuando producen la misma transcripción para la misma semilla y el mismo
nombre de jugador*. Ese es todo el contrato. El resto de la lección es cómo lo demuestras en lugar
de afirmarlo.

### La frase sobre la que se construye esta lección

> Una traza es evidencia solo cuando el comando y la salida observada se registran con ella.

Léela dos veces. El archivo solo es un espécimen. El comando es cómo otra persona obtiene el mismo
espécimen. La salida es lo que debería ver. La explicación es por qué el espécimen resuelve la
pregunta. Si quitas cualquiera de los tres, la afirmación se vuelve una opinión con un archivo
adjunto.

## Antes de empezar

Lee estos documentos completos:

- [`common/traces/README.md`](../../traces/README.md) — el conjunto de trazas, su autoridad y su
  procedimiento de regeneración.
- [`common/harness/README.md`](../../harness/README.md) — la línea de comandos y cómo comprobar una
  traza.
- [`common/spec/game-rules.md`](../../spec/game-rules.md) — en especial §13, la transcripción como
  contrato de comportamiento.
- [`common/spec/transcript-format.md`](../../spec/transcript-format.md) — las formas de registro que
  vas a comparar.
- [`docs/guide/verification.md`](../../../docs/guide/verification.md) — qué cuenta como evidencia.

También necesitas:

- Python 3.8 o superior, un shell POSIX con `diff`, `cmp`, `sha256sum` y `grep`;
- Bash para la forma con sustitución de procesos que se usa abajo (la forma con redirección funciona
  en cualquier shell POSIX);
- un directorio de trabajo para tus propias copias (aquí se usa `$HOME/common-04-scratch`);
- el directorio de trazas: [`common/traces/`](../../traces/).

No edites ningún archivo de `common/traces/`. Son entradas de esta lección, no espacio de trabajo.

## El camino de 45 minutos

| Min | Paso | Qué haces | Evidencia |
|---|---|---|---|
| 0–8 | Leer | Lees el conjunto, el archivo de resúmenes y una traza completa. | Traza anotada |
| 8–15 | Predecir | Escribes qué prueban los resúmenes y qué mostrará la reproducción. | Nota de predicción |
| 15–22 | Verificar | Ejecutas `sha256sum -c checksums.txt` y lees cada línea. | Comando + salida |
| 22–32 | Reproducir | Reconstruyes una traza desde su semilla y la comparas. | Comando + diff vacío |
| 32–38 | Controlar | Fuerzas un diff no vacío cambiando una sola entrada. | La línea exacta que difiere |
| 38–42 | Explicar | Explicas por qué la traza es evidencia solo con comando y salida. | Un párrafo |
| 42–45 | Registrar | Completas `EVIDENCE-es.md`. | Entrada del portafolio |

Un paso puede llevar más; eso es información
sobre el paso, no sobre ti.

## Paso 1 — Leer el conjunto de trazas (8 min)

Tres archivos definen el contrato. Léelos en este orden.

### 1.1 El archivo de resúmenes

[`common/traces/checksums.txt`](../../traces/checksums.txt) tiene veinte líneas, una por traza. Cada
línea es un resumen SHA-256 de 64 caracteres, dos espacios y un nombre de archivo sin ruta:

```text
bf74281f720ef49df792887caff7dd1512803639b18b2f6754c161663212158b  seed-0001.txt
```

Dos detalles importan y ambos importarán después:

- el nombre es **relativo**, no una ruta, así que la comprobación depende del directorio desde el
  que la ejecutes;
- el resumen identifica bytes. No dice nada sobre su significado.

El archivo cubre `seed-0001.txt` hasta `seed-0020.txt`, y nada más. Una traza nueva fuera de ese
rango no queda cubierta por la comprobación, por correcta que parezca.

### 1.2 Una traza, de principio a fin

Abre [`seed-0001.txt`](../../traces/seed-0001.txt). Tiene 89 líneas y cuatro regiones:

1. **Cabecera** — siete registros: `seed`, `player`, dos tiradas, `start` y las dos casillas
   iniciales.
2. **Turnos** — una línea `turn=` por turno, con el caballo activo, su casilla y el `order` de ocho
   movimientos.
3. **Intentos y movimientos** — líneas `try=` con dos espacios iniciales y luego una línea `move=`
   para el intento que tuvo éxito.
4. **Resultado** — `winner=machine` y `moves=21`.

Fíjate en lo que el archivo no contiene: ni marcas de tiempo, ni rutas, ni versión, ni nombre de
máquina. Una traza es portable precisamente porque nada de la máquina se filtra en ella. Por eso un
resumen sobre ella significa algo.

### 1.3 La forma del conjunto

Cinco trazas bastan para ver el patrón. Lee tú mismo las líneas de resultado; no aceptes la tabla
sin comprobarla.

| Traza | Semilla | Ganador | `moves=` | Líneas `turn=` | Final |
|---|---|---|---|---|---|
| `seed-0001.txt` | 1 | machine | 21 | 22 | trampa |
| `seed-0007.txt` | 7 | player | 10 | 10 | captura |
| `seed-0010.txt` | 10 | machine | 22 | 22 | captura |
| `seed-0011.txt` | 11 | machine | 8 | 9 | trampa |
| `seed-0020.txt` | 20 | machine | 47 | 48 | trampa |

Las dos últimas columnas son lo interesante. En una captura, el movimiento ganador es un movimiento
exitoso, así que las cuentas coinciden. En una trampa, el turno final consume una tirada de `start`
y una línea `turn=`, pero no registra ninguna línea `move=`, así que los turnos superan a los
movimientos en uno. Esa relación es una consecuencia pequeña y comprobable de §6, §10 y el formato
de transcripción, y es exactamente el tipo de cosa que una predicción debería nombrar.

El conjunto no tiene tablas. Es una propiedad de estas veinte semillas, no una regla: una tabla
requiere ambos caballos vivos en el movimiento 200. Guarda ese dato; se convierte en un límite que
debes declarar en el Paso 6.

## Paso 2 — Predecir (7 min)

Escribe la predicción **antes** de ejecutar nada. Predices qué establece cada comando, no adivinas
dígitos aleatorios.

Responde al menos esto:

1. Si `sha256sum -c checksums.txt` pasa, ¿qué has probado exactamente sobre las trazas?
2. ¿Qué dirá la salida correcta, palabra por palabra, en una máquina con configuración regional en
   español? ¿Cuál será el estado de salida?
3. ¿La reproducción de la semilla 1 producirá un diff vacío? ¿Cuántos registros de cabecera tendrá
   la transcripción reproducida?
4. ¿Qué pasa si ejecutas la comprobación desde la raíz del repositorio en lugar de desde
   `common/traces/`? ¿Por qué?
5. Si reproduces la semilla 1 con el nombre de jugador `Alex`, ¿el diff estará vacío? ¿Qué línea o
   líneas diferirán?
6. ¿Qué podría estar mal en la implementación y aun así pasar las veinte trazas?

Escribe una frase falsable, por ejemplo:

> La comprobación de resúmenes pasará con estado de salida 0, y la transcripción reproducida de la
> semilla 1 dará un diff vacío contra `seed-0001.txt`, porque ambos comandos solo leen archivos que
> ya están en el repositorio.

Una predicción no es una suposición que puedas corregir después. La brecha entre predicción y
observación es la señal de aprendizaje; editar la predicción después de ejecutar la destruye.

## Paso 3 — Verificar el conjunto de resúmenes (7 min)

Ejecuta la comprobación desde el directorio que el archivo de resúmenes espera:

```bash
cd common/traces
sha256sum -c checksums.txt
echo "exit=$?"
```

Esperado: veinte líneas, cada una informando una coincidencia, y `exit=0`. En configuración regional
inglesa la línea termina en `OK`; en española, en `La suma coincide`. Ambas son un aprobado. El
texto está traducido; el estado de salida no.

Este es el primer lugar donde una predicción puede fallar de forma instructiva. Si escribiste
«imprimirá OK», predijiste una traducción, no un comportamiento. El comportamiento es: cada línea
informa éxito y el proceso sale con `0`.

Cuenta las líneas para confirmar que la comprobación cubrió lo que crees que cubrió:

```bash
sha256sum -c checksums.txt | wc -l
```

Veinte es la única respuesta aceptable. Diecinueve significa que una traza no está referenciada;
veintiuna significa que el archivo menciona algo que no debería estar ahí.

### 3.1 La trampa del directorio

Ahora ejecuta la misma comprobación desde la raíz del repositorio y conserva el fallo:

```bash
cd ../..
sha256sum -c common/traces/checksums.txt
echo "exit=$?"
```

Falla, y el mensaje nombra un archivo ausente como `seed-0001.txt`, aunque puedes ver ese archivo
bajo `common/traces/`. La razón está en el archivo que leíste en el Paso 1: las entradas son nombres
sin ruta. `sha256sum` los resuelve contra el directorio actual, no contra el directorio del archivo
de resúmenes.

Registra esto. No es un defecto del repositorio; es cómo la herramienta lee el archivo. También es
el tipo de desajuste que produce una falsa alarma en una tarea de integración continua, y la
explicación es una sola frase sobre rutas relativas.

Si prefieres una versión que funcione desde la raíz, sé explícito en lugar de adivinar:

```bash
cd common/traces && sha256sum -c checksums.txt
```

### 3.2 Si una línea falla

Una línea `FAILED` significa que los bytes en disco ya no coinciden con el resumen registrado. No lo
arregles regenerando el resumen. Eso borraría la evidencia de que algo cambió. Registra:

- el nombre del archivo y el resumen informado frente al esperado;
- si el archivo fue modificado en tu árbol de trabajo (`git status`);
- qué cambiaste, si cambiaste algo.

Una traza se regenera solo cuando cambió la implementación o la especificación, y entonces se
regenera el conjunto completo y se revisa su diff, según el procedimiento de
[`common/traces/README.md`](../../traces/README.md).

## Paso 4 — Reproducir una traza (10 min)

Ahora produce la traza tú mismo y compárala. Desde la raíz del repositorio:

```bash
diff -u common/traces/seed-0001.txt \
  <(common/harness/run.sh --seed 1 --name Learner)
echo "exit=$?"
```

Esperado: sin salida, `exit=0`. Un diff vacío significa que el código actual reproduce el
comportamiento registrado para la semilla 1 y el jugador `Learner`.

Si tu shell no admite `<(...)`, usa un archivo. Es la misma observación, un paso más larga:

```bash
mkdir -p "$HOME/common-04-scratch"
common/harness/run.sh --seed 1 --name Learner > "$HOME/common-04-scratch/seed-0001.txt"
diff -u common/traces/seed-0001.txt "$HOME/common-04-scratch/seed-0001.txt"
cmp -s common/traces/seed-0001.txt "$HOME/common-04-scratch/seed-0001.txt" \
  && echo "IDENTICAL" || echo "DIFFERENT"
```

`IDENTICAL` más un diff vacío es el mismo hecho dicho dos veces. La redundancia es deliberada:
`diff` muestra dónde, `cmp` da una respuesta binaria que puedes usar en un script.

Registra el resumen de ambos lados, porque una ruta no prueba que los bytes coincidieran:

```bash
sha256sum common/traces/seed-0001.txt "$HOME/common-04-scratch/seed-0001.txt"
```

Los dos resúmenes deben ser iguales, y deben ser iguales a la primera línea de `checksums.txt`.

### 4.1 Qué acabas de establecer

Tres hechos separados, y no son el mismo hecho:

1. la traza registrada coincide con su resumen (Paso 3);
2. la implementación actual reproduce esos bytes (Paso 4);
3. los bytes reproducidos tienen la forma que exige el formato (Paso 1).

Una afirmación de «coincide» colapsa los tres. Tu archivo de evidencia debe mantenerlos separados,
porque cada uno puede fallar por una razón distinta.

## Paso 5 — Ejecutar un control negativo (6 min)

Una comprobación que nunca viste fallar no es una comprobación; podría estar comparando un archivo
consigo mismo. Demuestra que el diff puede ver una diferencia cambiando exactamente una entrada.

```bash
diff -u common/traces/seed-0001.txt \
  <(common/harness/run.sh --seed 1 --name Alex)
```

Esperado: exactamente un bloque, y dentro de él exactamente un par de líneas cambiadas:

```text
-player=Learner
+player=Alex
```

`diff` sale con `1` cuando encuentra diferencias. Ese estado de salida es un resultado, no un error.
Se siguen dos conclusiones, y ambas son útiles:

- la comparación es sensible al nombre del jugador, que forma parte de la transcripción aunque nunca
  llega al generador aleatorio;
- el resto de la partida no cambia, porque el nombre se escribe en la cabecera y nada más lo lee.

Esa segunda conclusión es la interesante. El diff aísla el efecto de una entrada. Sin el control,
«diff vacío» y «diff que no puede detectar nada» se ven idénticos.

### Segundo control opcional

Cambia la semilla en lugar del nombre:

```bash
diff -u common/traces/seed-0001.txt \
  <(common/harness/run.sh --seed 2 --name Learner)
```

Predice el tamaño del diff antes de ejecutarlo. Será grande y empezará en la línea `seed=`. Si fuera
pequeño, las semillas no estarían seleccionando partidas distintas, lo que contradiría §12. No
registres este control salvo que te haya enseñado algo que el primero no enseñó.

## Paso 6 — Explicar (4 min)

Escribe un párrafo, sin palabras de sintaxis. Responde:

- ¿Qué prueba el resumen y qué no prueba?
- ¿Qué prueba la reproducción y qué no prueba?
- ¿Por qué el comando forma parte de la evidencia y no es solo una comodidad para producirla?
- ¿Qué tendrías que añadir al conjunto de trazas para cubrir una tabla, y por qué no puedes afirmar
  que las tablas funcionan a partir de estos veinte archivos?

Una frase útil para terminar: *«La traza es la afirmación; el comando y la salida son cómo otra
persona comprueba la afirmación; la explicación es por qué la afirmación resuelve la pregunta.»*

No nombres un lenguaje de programación, un tipo de dato ni un elemento de sintaxis. Si necesitas uno
para explicar la idea, la idea todavía no está separada de la implementación.

### Los límites que debes declarar

Veinte trazas son una red de regresión, no una prueba. Fijan veinte caminos. Un error que solo
aparece con la semilla 21 sobrevive a todo el conjunto. El camino de las tablas nunca se ejercita,
porque ninguna de estas semillas empata. Y una traza puede, en principio, contradecir la
especificación; en ese caso la traza se regenera desde el arnés corregido, y la especificación
manda.

Decir esto en voz alta forma parte de la lección. La evidencia con alcance declarado es más fuerte
que la evidencia presentada como universal.

### La trampa que debes evitar

La explicación equivocada tentadora es: *«Los resúmenes pasan, así que las trazas son correctas.»*
Los resúmenes pasan porque los archivos no han cambiado. Si los archivos describen el juego que las
reglas exigen es otra pregunta, que se responde leyendo §1–§13 y comparando comportamiento, no con
un resumen.

## Paso 7 — Registrar (3 min)

Completa [`EVIDENCE-es.md`](EVIDENCE-es.md). Es tu entrada del portafolio. Incluye:

- la predicción, escrita antes de las ejecuciones;
- el comando de resúmenes, su directorio de trabajo y su salida completa, incluido el texto
  traducido;
- la cantidad de líneas de la salida de resúmenes;
- el comando de reproducción, el resultado del diff y el estado de salida;
- los dos resúmenes y la primera línea de `checksums.txt` para comparar;
- el control negativo: la entrada que cambiaste, la línea que difiere y el estado de salida del
  diff;
- el párrafo de explicación, incluidos los límites del conjunto;
- el siguiente paso.

## Autocomprobación (binaria)

- [ ] Leí `checksums.txt`, una traza completa y el README de trazas.
- [ ] Escribí la predicción antes de ejecutar cualquier comando.
- [ ] `sha256sum -c checksums.txt` informó veinte líneas exitosas desde `common/traces`.
- [ ] Registré el texto exacto y el estado de salida, no solo «pasó».
- [ ] Expliqué por qué el mismo comando falla desde la raíz del repositorio.
- [ ] Reproduje la semilla 1 y el diff contra la traza de referencia quedó vacío.
- [ ] Registré el comando, la salida vacía y el estado de salida.
- [ ] El resumen reproducido es igual al resumen registrado.
- [ ] Produje un diff no vacío cambiando una sola entrada y nombré la línea que difiere.
- [ ] Puedo enunciar qué no prueba el conjunto de trazas, sin palabras de sintaxis.
- [ ] Completé `EVIDENCE-es.md` y sé cuál es mi siguiente paso.

Si una casilla no está marcada, la lección todavía no está terminada **para ti**. La casilla sin
marcar nombra el siguiente paso. No hay penalización por repetir un paso.

## Misconcepciones que esta lección refuta

| Concepción errónea | Por qué es falsa | Qué hacer en su lugar |
|---|---|---|
| «La traza es la evidencia.» | Un archivo se puede copiar, editar o quedar obsoleto; la evidencia necesita un comando reproducible y una salida observada. | Registra comando, salida y explicación juntos. |
| «Si los resúmenes pasan, es correcto.» | Un resumen prueba que los bytes no cambiaron, no que obedezcan las reglas. | Usa resúmenes para integridad, reproducción para comportamiento y reglas para corrección. |
| «Un diff vacío prueba que el programa está bien.» | Prueba que el programa reprodujo un camino registrado. | Declara el alcance: veinte semillas, sin tablas. |
| «Si falla un resumen, la traza está mal.» | Significa que los bytes cambiaron; la causa puede ser una edición, una fusión mala o una regeneración. | Investiga el cambio antes de tocar el resumen. |
| «El comando está roto si falla desde la raíz.» | Las entradas son nombres sin ruta que se resuelven contra el directorio actual. | Ejecútalo desde `common/traces` o cambia de directorio explícitamente. |
| «La salida traducida significa que la comprobación falló.» | La herramienta traduce su mensaje; el estado de salida lleva el resultado. | Lee el estado y registra el texto que viste. |
| «Un diff con estado 1 es un error.» | `diff` usa `1` para decir «hay diferencias». | Trata `1` como observación y `2` como error. |
| «Editar una traza a mano es un arreglo rápido.» | Rompe el contrato y esconde el cambio real. | Cambia código o especificación, regenera el conjunto y revisa el diff. |
| «Veinte trazas cubren el juego.» | Cubren veinte semillas y ninguna tabla. | Di «regresión para estas semillas», no «prueba para todas las partidas». |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Traza de referencia | Una salida esperada registrada que sirve de referencia para una entrada. |
| Contrato de comportamiento | El acuerdo de que las mismas entradas producen la misma salida observable. |
| Resumen (digest) | Una huella de longitud fija de una secuencia de bytes. |
| Reproducir | Producir de nuevo la salida registrada con el código actual. |
| Diff | Una comparación línea por línea de dos textos. |
| Control negativo | Una ejecución que se espera distinta, usada para probar que la comparación detecta diferencias. |
| Regresión | Un cambio que rompe comportamiento acordado previamente. |
| Alcance | El conjunto de entradas que una pieza de evidencia cubre realmente. |

Las definiciones canónicas están en [`glossary-es.md`](../../../docs/guide/glossary-es.md).

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
