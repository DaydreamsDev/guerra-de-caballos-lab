# Programación estructurada (structured programming)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** secuencia, selección e iteración · **Tiempo:** unos 60 minutos ·
**Estado:** borrador
**Lectura:** ~17 minutos

## Por qué existe esta lección

La diferencia no es el número de saltos. Es el número de *destinos*. La secuencia dice que la
siguiente sentencia es la de abajo. La selección dice que ambas ramas se reúnen después del bloque.
La iteración dice que el cuerpo se repite en el mismo lugar hasta que cambia la condición. Cada
transferencia nombra una región que el lector puede resolver desde dentro hacia fuera, sin un mapa.

Esta lección aplica una regla a dos archivos que ya producen las mismas transcripciones. Vas a
contar dos cosas distintas: las ediciones que exige la regla y las aristas que un lector debe
revisar para creer que las ediciones están en todos los caminos. El resultado interesante es qué
recuento se mueve y cuál no.

## Qué podrás hacer

Al terminar esta lección puedes explicar secuencia, selección e iteración y aplicar la prueba de
cambio de la lección.

## El concepto: una región es un nombre para la siguiente sentencia

Tres construcciones hacen todo el trabajo de nombrar:

```text
secuencia:    a; b; c;
selección:    if (p) { a; } else { b; }   ambas ramas siguen después del bloque
iteración:    while (p) { a; }            una condición, un cuerpo, una arista de retorno
```

La unidad de lectura es la región, no la línea. Una secuencia nombra a su sucesora por la posición
en el bloque. Una selección nombra un punto de reunión. Una iteración nombra un cuerpo y una
condición, y el final del cuerpo vuelve a nombrar la condición. En todos los casos el lector puede
responder «¿adónde va el control después?» sin salir del bloque que lo contiene.

La sintaxis que obliga en esta línea base es la ausencia de un **destino arbitrario**. No hay `goto`
ni etiquetas. Hay `continue`, que apunta al bucle que lo contiene. No es un contraejemplo:
`continue` nombra una frontera que el lector ve desde el bloque, y una edición futura no puede
hacerlo aterrizar en otra línea. Una lectura estricta de entrada única y salida única reescribiría
`continue` como un bloque con guarda. Esta lección usa la definición práctica: una transferencia es
estructurada cuando su destino es una región que la contiene, no una posición arbitraria. La
distinción importa más adelante, así que reténla.

De ahí se siguen dos consecuencias, y son toda la lección:

1. **Un turno empieza en una sola región.** El cuerpo del bucle del juego es el único lugar donde
   empieza un turno. Su entrada tiene un solo predecesor: la condición del bucle. Para demostrar que
   todos los turnos pasan una comprobación, basta leer esa región.
2. **El siguiente turno es la arista de retorno del bucle.** Un salto de turno solo tiene que llegar
   a la frontera del bucle. Después, el bucle vuelve a entrar en el cuerpo por la misma condición
   que usa siempre. No se nombra ningún destino nuevo.

Esto es lo que significará «localizado» aquí: el cambio de flujo de control y el argumento de su
completitud viven en una región con una sola entrada. Una declaración fuera del bucle es estado, no
flujo; no añade ningún camino. No significa necesariamente menos pulsaciones. Vas a medir ambas
cosas y a mantenerlas separadas.

## Antes de empezar

- [`construction-01`](../construction-01-non-structured/lesson-es.md) — ya pusiste precio a R-SKIP
  en la línea base no estructurada y tienes su recuento.
- Un compilador de C++20 (`g++` o `clang++`). `cmake` es opcional.

No necesitas saber C++ a fondo. Necesitas leer un bucle, un `if` y la línea donde empieza un turno.
Esa es toda la destreza que ejercita esta lección.

### Qué necesitas

- La línea base estructurada: [`code/structured-baseline.cpp`](code/structured-baseline.cpp).
- La línea base no estructurada:
  [`non-structured-baseline.cpp`][non-structured-baseline].
- Los scripts de compilación y las notas: [`run.sh`](run.sh) y [`README-es.md`](README-es.md), más
  los equivalentes de construction-01.
- Las reglas: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md).
- El formato de la transcripción:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- Una traza de referencia: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- Esta plantilla para quien aprende: [`EVIDENCE-es.md`](EVIDENCE-es.md).

Comprueba que los artefactos existen antes de empezar, desde este directorio de la lección:

```bash
test -f code/structured-baseline.cpp &&
test -f ../construction-01-non-structured/code/non-structured-baseline.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

Si falta cualquiera de las dos líneas base, la comparación no tiene segundo lado. No inventes un
recuento y no describas un programa que no has leído. Registra el archivo ausente en
`EVIDENCE-es.md` §10 y vuelve cuando esté disponible. Un coste medido contra una alternativa
imaginaria no es evidencia.

Ambas líneas base reproducen las 20 trazas de `common/traces/` byte a byte. Esta lección usa la
semilla 1 para la comprobación detallada y un diff extra para demostrar que los dos archivos son el
mismo juego.

## Paso 1 — Inventariar las dos formas (~10 min)

Lee los dos archivos una vez, de principio a fin, sin juzgarlos. Después construye dos inventarios.

Tres comandos te dan un punto de partida, no la respuesta completa:

```bash
grep -nE '^[[:space:]]*[A-Za-z_][A-Za-z0-9_]*:' \
  ../construction-01-non-structured/code/non-structured-baseline.cpp
grep -n 'goto ' ../construction-01-non-structured/code/non-structured-baseline.cpp
grep -nE '\b(while|for|continue|break)\b' code/structured-baseline.cpp
```

El patrón de etiquetas también coincide con `std:` dentro de llamadas indentadas, y un salto puede
compartir línea con una condición. Lee los archivos para confirmar lo que reportan los comandos. El
inventario es el archivo, no el grep.

Después responde en `EVIDENCE-es.md` §2:

1. Archivo no estructurado: ¿cuántas etiquetas y cuántos saltos hay?
2. Archivo no estructurado: ¿qué posición inicia un turno y qué aristas entran en ella? Enumera
   cada línea y di si la arista es un salto o una caída directa.
3. Archivo estructurado: ¿qué bucle inicia un turno, cuál es su condición y cuál es el rango de
   líneas de la región que se ejecuta una vez por turno?
4. Archivo estructurado: ¿qué aristas entran en esa región? Cuenta una sola vez la arista
   condición-cuerpo del bucle.
5. En cada archivo: ¿dónde se asigna el lado activo y dónde se incrementa `successfulMoveCount`?
6. ¿Qué archivo escribe «aquí empieza un turno» como una construcción, y cuál lo escribe como una
   posición?

La pregunta 4 es la que importa. Respóndela dibujando las aristas, no confiando en la indentación.

## Paso 2 — Predecir el coste antes de medirlo (~10 min)

La prueba de cambio es un solo requisito. Es el mismo que aplicaste en `construction-01`. Léelo dos
veces y después escribe tu predicción.

> **R-SKIP — saltar turno.** Cuenta los turnos cuando empiezan, produzcan o no un movimiento. Ese
> contador es independiente del valor `moves=` de la transcripción. Cuando el contador es múltiplo
> de 5, el caballo activo no mueve: el turno se anuncia igual con sus campos `turn=` y `order=`, no
> le sigue ninguna línea `try=` ni ninguna línea `move=`, y el turno pasa al otro caballo. Un turno
> saltado no cuenta para los 200 movimientos exitosos.

R-SKIP es determinista a propósito: no añade ningún sorteo, así que no altera el orden de llamadas
de §12 de las reglas. El turno saltado sigue sorteando e imprimiendo su orden; el salto ocurre
después de la cabecera. Una regla que sorteara un número sería un cambio distinto y más grande.

Antes de contar nada, fija las dos unidades.

### 2.1 La regla de conteo

Un recuento solo es comprobable si la regla para contar se fija antes de contar. Usa esta.

- Una **entrada de turno** es donde empieza un turno. En el archivo no estructurado es la etiqueta
  `gameLoopStart`; en el archivo estructurado es el cuerpo del bucle del juego.
- Una **región de inicio de turno** es el código que se ejecuta una vez por turno, desde la entrada
  de turno hasta la sentencia que imprime la cabecera. Termina antes del primer intento. En el
  archivo no estructurado, la comprobación de terminación está dentro de esta región; en el
  estructurado, esa comprobación es la condición del bucle, que es la arista de entrada.
- Una **arista entrante** es una arista de flujo de control que entra en la entrada de turno.
  - En un archivo con etiquetas, cuenta cada salto cuyo destino sea la entrada, más cualquier caída
    directa en ella.
  - En un archivo con un bucle, cuenta una sola vez la arista condición-cuerpo. El primer turno y
    todos los siguientes usan esa misma arista; no multipliques por el número de turnos.
- Un **sitio de edición** es un cambio contiguo de código que el requisito obliga a hacer: una
  declaración, un incremento, un argumento de formato o un bloque de comprobación con su
  transición. Cuenta un bloque contiguo una sola vez, aunque contenga dos sentencias.
- Una **arista nueva de flujo de control** es una arista que no existe antes del cambio. Un
  `continue` hacia un bucle existente y un `goto` hacia una etiqueta existente cuentan una vez cada
  uno. Una etiqueta nueva se cuenta aparte.
- **No** cuentes comentarios, líneas en blanco, formato, trazas generadas ni el número de turnos que
  ejecuta el programa.

`construction-01` contaba los saltos cuyo destino era la entrada de turno. Esta regla cuenta además
una caída directa en la región, así que el número no estructurado puede ser uno mayor que el que
trajiste. Es la unidad, no un error; registra ambos en §1 y §3.

Cuenta sitios de edición y aristas entrantes por separado. Responden a preguntas distintas: cuánto
texto cambia y cuántos caminos debe revisar un lector para saber que el texto está en todos los
caminos.

Por qué la arista del bucle cuenta una sola vez: el cuerpo de un bucle tiene un solo predecesor, la
condición. Se puede llegar a la condición desde antes del bucle y desde el final del cuerpo, pero
ambas rutas entran en el cuerpo por la misma arista. Eso es un hecho del grafo de flujo de control,
no una comodidad.

La región tiene ramas internas y, en el archivo no estructurado, la comprobación de terminación
puede abandonar el juego. Un camino que sale ahí no inicia ningún turno. Todo camino que inicia un
turno llega a la cabecera, y una comprobación colocada justo después de la cabecera está en todos
ellos. Por eso basta contar las aristas que entran en la entrada de turno.

### 2.2 Un ejemplo resuelto del método

Esto es una demostración de conteo, no una de las líneas base. Sus números no son tu respuesta.

```text
estructurado                     no estructurado
while (p) {                      inicio:
    a;                               a;
}                                    if (q) goto inicio;
                                     goto inicio;
```

Las dos versiones ponen una comprobación antes de `a`. La comprobación tiene el mismo tamaño. El
cuerpo estructurado tiene una arista entrante, la condición. La entrada no estructurada tiene tres:
la caída directa, el salto condicional y el salto incondicional. Un lector que quiera saber que
ningún camino se salta la comprobación debe revisar una arista en el primer archivo y tres en el
segundo. La edición es igual; la verificación no.

La lección del ejemplo: el coste de un cambio es el texto que escribes más el conjunto de caminos
que debes revisar para saber que el texto está en todos los caminos. La segunda parte es la que se
dispersa.

Después responde en `EVIDENCE-es.md` §3:

1. ¿Cuántos sitios de edición necesita R-SKIP en el archivo no estructurado?
2. ¿Cuántos sitios de edición necesita en el archivo estructurado?
3. ¿Cuántas aristas entrantes tiene la entrada de turno en cada archivo antes del cambio?
4. ¿Cuántas aristas nuevas de flujo de control añade el cambio en cada archivo?
5. ¿Cuántas etiquetas nuevas necesita en cada archivo?
6. ¿Cuántos archivos toca en cada caso?
7. ¿Cuántos conceptos nuevos de lenguaje exige en cada caso?
8. ¿Qué línea de la traza de referencia esperas que difiera primero, y por qué?
9. Confianza, de 0 a 100 %, para la pregunta 3 en el archivo estructurado.
10. Una frase: ¿dónde esperas que aparezca la diferencia y dónde esperas un empate?

Comprométete con un número para cada archivo. «La estructurada es mejor» no es una predicción; no
dice qué recuento se mueve. La brecha entre tus números y los números contados es la señal de
calibración.

## Paso 3 — Ejecutar las dos líneas base (~10 min)

Compila ambas y reproduce una traza de referencia. Este paso establece que los dos programas que
vas a comparar son el juego, no dos primos del juego.

Desde este directorio de la lección:

```bash
./run.sh --seed 1 --name Learner > /tmp/structured-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/structured-0001.txt
```

Después, la línea base no estructurada:

```bash
cd ../construction-01-non-structured
./run.sh --seed 1 --name Learner > /tmp/nonstructured-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/nonstructured-0001.txt
```

Por último, compara los dos archivos entre sí:

```bash
diff -u /tmp/nonstructured-0001.txt /tmp/structured-0001.txt
```

Tres diffs vacíos. Los dos primeros dicen que cada archivo es el juego. El tercero dice que los dos
archivos son el mismo juego. Sin el tercero, estarías poniendo precio a un cambio contra un programa
que podría comportarse distinto; la comparación mediría otra cosa.

Si `run.sh` no puede compilar, compila una vez a mano y registra el comando exacto. Para el archivo
estructurado:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o build/structured-baseline code/structured-baseline.cpp
```

Para el archivo no estructurado, ejecuta el mismo comando desde su directorio con su propio nombre
de archivo.

Si un diff no está vacío, detente. Alguna línea base no es fiel y cada recuento que produzcas
describiría otro programa. Registra el diff en `EVIDENCE-es.md` §10 y repórtalo; no ajustes la traza
para que coincida con el código.

## Paso 4 — Aplicar R-SKIP a ambas y medir (~15 min)

Vuelve antes a este directorio de la lección; el Paso 3 termina en el directorio de
construction-01. Copia los dos archivos a un directorio de trabajo fuera del repositorio. Las líneas
base versionadas son tu control; editarlas destruye la comparación.

```bash
mkdir -p /tmp/rskip
cp code/structured-baseline.cpp /tmp/rskip/structured-rskip.cpp
cp ../construction-01-non-structured/code/non-structured-baseline.cpp \
  /tmp/rskip/nonstructured-rskip.cpp
```

Aplica el mismo requisito a las dos copias. El requisito es el párrafo del Paso 2; estas viñetas
fijan los detalles para que las dos implementaciones sean comparables:

1. Declara un contador de turnos iniciados, inicializado a 0, junto al resto del estado del turno.
2. Increméntalo una vez por turno, después de la comprobación de terminación y antes del sorteo del
   orden.
3. Imprime `turn=` desde ese contador, no desde `successfulMoveCount + 1`. Si dejas la cabecera en
   el recuento de movimientos, un turno saltado imprime un número que se volverá a imprimir, y la
   transcripción deja de ser una secuencia de turnos.
4. Después de la cabecera y antes del primer intento, si el contador es múltiplo positivo de 5:
   invierte el lado activo, no registres ningún movimiento y pasa al siguiente turno.
5. Mantén el límite de 200 movimientos y `moves=` sobre `successfulMoveCount`. Un turno saltado no
   cuenta como movimiento exitoso.

No copies la forma de la transferencia de un archivo al otro. Cada archivo ya tiene una manera de
empezar otro turno; usa la que ese archivo ofrece. No añadas una etiqueta nueva al archivo no
estructurado salvo que puedas demostrar que la existente no se puede reutilizar. Si la añades,
registra por qué: es un coste real, no un error.

Compila las dos copias de trabajo:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/rskip/structured /tmp/rskip/structured-rskip.cpp
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/rskip/nonstructured /tmp/rskip/nonstructured-rskip.cpp
```

Ejecuta ambas y compara:

```bash
/tmp/rskip/structured --seed 1 --name Learner > /tmp/rskip/structured-0001.txt
/tmp/rskip/nonstructured --seed 1 --name Learner > /tmp/rskip/nonstructured-0001.txt
diff -u /tmp/rskip/nonstructured-0001.txt /tmp/rskip/structured-0001.txt
```

El primer diff debe estar vacío: dos implementaciones correctas de la misma regla son el mismo
juego nuevo. Si no está vacío, las implementaciones discrepan; encuentra la primera línea distinta
antes de contar nada.

Ahora compara cada transcripción modificada con la traza de referencia. Las dos deben diferir, y eso
es lo esperado: R-SKIP cambia el comportamiento, así que es un juego nuevo, no una regresión. Para
la semilla 1, la primera divergencia debe ser el quinto turno: a la cabecera `turn=5` le sigue la
cabecera `turn=6` en lugar de una línea `try=`. Si tu primera divergencia está en otro lugar, revisa
el contador y la colocación de la comprobación antes de confiar en el resto de la ejecución.

Registra cuántas de las 20 trazas de referencia dejan de coincidir con las versiones modificadas. El
número debe ser el mismo en ambas; mide el cambio de contrato, no una regresión. El bucle del README
sobre `common/traces/seed-*.txt` también sirve aquí; apúntalo al binario de trabajo.

```bash
diff -u ../../../common/traces/seed-0001.txt /tmp/rskip/structured-0001.txt | head
diff -u ../../../common/traces/seed-0001.txt /tmp/rskip/nonstructured-0001.txt | head
```

Después cuenta. Marca cada sitio de edición en cada copia de trabajo y completa `EVIDENCE-es.md` §6.
Enumera cada arista entrante del archivo sin modificar con su línea y su tipo. Un número sin lista
es una opinión; la lista es lo que hace que el recuento sea reproducible.

Responde a las preguntas de comparación en §6.4:

- ¿Necesitó menos sitios de edición la versión estructurada? Si no, ¿qué localizó?
- ¿Qué recuento se movió entre las dos formas y cuál quedó igual?
- ¿Qué arista entrante del archivo no estructurado era la más fácil de pasar por alto?
- Si se añadiera un quinto `goto gameLoopStart`, ¿la comprobación lo cubriría? ¿Qué leerías para
  saberlo?
- Si se añadiera un segundo bucle que también inicia un turno al archivo estructurado, ¿la
  comprobación lo cubriría? ¿Qué leerías para saberlo?
- ¿Qué hace visible el archivo estructurado en el punto del cambio que el no estructurado deja
  implícito?

Si un número difiere de tu predicción, ese es el resultado. No revises la predicción; escribe la
diferencia al lado.

## Paso 5 — Explicar la diferencia (~10 min)

Escribe la explicación en `EVIDENCE-es.md` §7. En dos partes.

**Primero, una frase.** Di por qué el cambio está localizado en un archivo y disperso en el otro sin
usar *malo*, *limpio*, *legible*, *elegante*, *feo*, *espagueti*, *ilegible* ni *goto*. Esas
palabras nombran un veredicto o una palabra clave, no una causa. Si tu frase necesita una de ellas,
has descrito tu reacción, no el flujo de control.

**Segundo, un párrafo.** Responde:

- ¿Dónde empieza un turno en cada archivo, como hecho de flujo de control?
- ¿A qué apunta la arista del salto de turno en cada archivo: a una frontera de región o a una
  posición?
- ¿Cuántas aristas entran en la entrada de turno en cada archivo, y por qué la arista del
  bucle cuenta una sola vez?
- Si los recuentos de edición empatan, ¿por qué los costes de verificación pueden diferir? Si no
  empatan, ¿qué compró el sitio extra?
- ¿Qué debe sostener un lector en la memoria de trabajo en cada caso?
- ¿Qué haría incompleto un cambio futuro en cada archivo, y cómo lo notaría un lector?

Una buena respuesta separa el *coste de edición* (los lugares donde escribes) del *coste de
verificación* (las aristas que lees). Nombra el número de predecesores y el tipo de destino. Una
respuesta débil dice que un archivo está más limpio. Limpio es lo que ves; un cuerpo con un solo
predecesor es lo que lo causa.

## Paso 6 — Criterio de aceptación (~5 min)

El criterio de aceptación de esta lección es: **aplicar el mismo requisito R-SKIP a las dos líneas
base; mostrar que la versión estructurada localiza el cambio; explicar la diferencia por el flujo de
control, no por el estilo.**

Comprueba tu registro contra él:

- [ ] Ejecuté las dos líneas base sin modificar y obtuve tres diffs vacíos.
- [ ] Apliqué la misma especificación R-SKIP a las dos copias de trabajo.
- [ ] Las dos versiones modificadas producen transcripciones idénticas para la semilla 1.
- [ ] Enumeré cada sitio de edición de los dos archivos con su línea.
- [ ] Conté las aristas entrantes a la entrada de turno con la regla enunciada.
- [ ] Puedo nombrar qué recuento empata y cuál no.
- [ ] Puedo decir qué localiza la versión estructurada y qué no localiza.
- [ ] Mi explicación de una frase evita las ocho palabras prohibidas.
- [ ] No edité las líneas base versionadas.
- [ ] Completé `EVIDENCE-es.md` §1–§9 y la autoverificación de §11.

El criterio no es «gana la versión estructurada». Es «puedes decir exactamente qué localiza, cuánto
y a qué coste». Un empate en sitios de edición es un resultado válido; no es un fallo al buscar una
diferencia.

## Paso 7 — Registrar (~5 min)

Completa `EVIDENCE-es.md` §1–§9. Conserva el inventario, la predicción, los tres diffs de las líneas
base, la implementación de trabajo, la tabla de coste, la lista de aristas y la explicación. Este
archivo es tu registro.

Si faltaba alguna línea base o no era fiel, completa §10 y detente. Todavía no hay nada que
comparar.

## Autocomprobación (binaria)

- [ ] Leí las dos líneas base e inventarié sus formas de flujo de control.
- [ ] Escribí los sitios de edición y las aristas entrantes previstos antes de contar.
- [ ] Compilé las dos líneas base y obtuve tres diffs vacíos.
- [ ] Apliqué el mismo requisito R-SKIP a las dos copias de trabajo.
- [ ] Las dos versiones modificadas coincidieron entre sí y ambas difirieron de la traza de
      referencia.
- [ ] Enumeré cada sitio de edición y cada arista entrante.
- [ ] Comparé la predicción con los recuentos y nombré las diferencias.
- [ ] Expliqué la diferencia en una frase más un párrafo, por el flujo de control.
- [ ] Completé `EVIDENCE-es.md`.

Son criterios de aceptación. Si queda una casilla
sin marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §9.

## Misconcepciones que esta lección refuta

| Concepción errónea | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| «Estructurado significa sin saltos.» | `continue` y `break` transfieren a una frontera que los contiene. El concepto prohíbe destinos arbitrarios, no transferencias. | Pregunta qué nombra la transferencia: una región o una posición. |
| «Menos líneas significa más estructurado.» | La edición de R-SKIP puede medir lo mismo en los dos archivos. | Cuenta aristas entrantes, no líneas. |
| «La versión estructurada tiene una entrada, así que todo turno futuro está cubierto.» | Un segundo bucle es una segunda región de inicio de turno; la comprobación debe repetirse o extraerse. | Cuenta regiones, no archivos. |
| «La versión no estructurada está mal porque usa `goto`.» | Produce las mismas 20 trazas. El coste está en el siguiente cambio, no en la corrección actual. | Compara el cambio, no la palabra clave. |
| «La cuarta arista entrante es obvia.» | Tres saltos y una caída directa se confunden fácilmente con «el bucle». | Enumera cada arista con su línea. |
| «Un empate en sitios de edición significa que los diseños son equivalentes.» | El recuento de edición mide lo que tecleas; el de verificación mide lo que un lector debe probar. | Informa los dos recuentos por separado. |
| «Puedo predecir después de medir.» | La predicción es la señal de calibración; debe ir primero. | Escribe los números y después mide. |
| «Si la traza modificada difiere de la de referencia, rompí la línea base.» | R-SKIP cambia el comportamiento a propósito. | Compara modificada con modificada; reserva la traza para las líneas base sin modificar. |

## Siguiente paso

`construction-03` — funciones y parámetros. El siguiente escalón le da un nombre y una lista de
parámetros a la región. Extraerás un turno a una función y contarás qué debe llevar la firma. Trae
los dos recuentos de esta lección: el de edición y el de aristas entrantes.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog.md) para ver la ruta
completa.

Si faltaba alguna línea base, no empieces `construction-03`. El siguiente contraste necesita los dos
lados.

[non-structured-baseline]: ../construction-01-non-structured/code/non-structured-baseline.cpp
