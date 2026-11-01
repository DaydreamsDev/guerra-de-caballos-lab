# Funciones y parámetros (functions and parameters)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** ámbito y descomposición · **Tiempo:** unos 45 minutos · **Estado:** borrador
**Lectura:** ~15 minutos

## Por qué existe esta lección

`construction-02` eliminó el salto arbitrario. El turno se convirtió en una región, y la región tuvo
una sola entrada. Esta lección hace otra pregunta sobre esa misma región: ¿qué tiene permitido leer?

En `construction-02` la respuesta es «cualquier cosa del ámbito global». Nada en una llamada
dice qué variables globales toca una función. En `construction-03` la respuesta está escrita en la
lista de parámetros, y el compilador la hace cumplir. La frase que hay que retener es corta:

> Una variable global es un parámetro que ninguna firma menciona.

Vas a comprobar esa frase dos veces. Una, de forma estática: cuántos nombres mutables viven en el
ámbito de espacio de nombres en cada archivo. Otra, de forma operativa: ¿puedes ejercitar una
decisión de movimiento sin construir antes el juego? La primera comprobación es un número.
La segunda es la que cambia tu forma de probar.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar ámbito y descomposición mediante una firma de función;
- extraer una decisión de movimiento sin usar variables globales.

## El concepto: una firma es la lista de nombres que el cuerpo puede leer

Cinco formas de nombre hacen el trabajo en este archivo:

```text
global      int successfulMoveCount;       visible en todas las funciones
local       int destinationRow = 0;        visible en un bloque
parámetro   int activeKnightRow            local, inicializado por quien llama
referencia  int &destinationRow            nombre local para el objeto de quien llama
retorno     MoveOutcome                    el único valor que el cuerpo entrega
```

El cuerpo de una función puede alcanzar tres clases de nombre: sus parámetros y locales, las
funciones que llama y las variables globales. La primera está en el texto alrededor de la llamada y
la segunda se nombra en la llamada. La tercera no está en ninguna parte. Esa es la asimetría que
esta lección pone precio. Las constantes son la excepción: visibles en todas partes, pero no son
estado, porque no cambian entre llamadas.

En `construction-02`, `successfulMoveCount` es global. `main` la lee, el bucle la lee, la rama de
movimiento la incrementa, la cabecera la imprime. Ninguna de esas sentencias dice «esta función
depende de un contador que posee `main`». Lo aprendes leyendo el archivo, o cambiando el contador y
descubriendo qué salidas se mueven.

En `construction-03` el mismo contador es un local de `main` y un parámetro por referencia de
`playTurn` y `playGame`. La dependencia está en la llamada. El compilador rechaza una llamada que lo
omita.

De ahí se siguen tres consecuencias, y son la lección:

1. **Una función que no lee ninguna global se puede llamar antes de que exista el juego.** `tryMove`
   decide un movimiento solo con sus parámetros, así que el Paso 4 puede llamarla con un tablero
   local y tres enteros. Ese es el criterio de aceptación.
2. **El estado nuevo del turno hay que pasarlo.** Un contador que el turno necesita debe aparecer en
   la firma de cada función entre su dueño y su usuario. Eso es un coste real. El Paso 5 lo mide.
3. **Una global todavía puede esconderse.** `randomState` sigue siendo global, así que `playTurn` no
   se puede probar de la misma forma. El cambio de ámbito movió el estado del juego, no el del
   proceso.

La línea base es honesta sobre lo que la firma *no* dice, y esta lección también:

- `bool visitedCells[][boardColumnCount + 1]` decae a un puntero a un arreglo de 9 `bool`. El número
  de filas no está en el tipo. Quien llama puede pasar un tablero de 8 filas o uno de 3; el
  compilador no distingue.
- `tryMove` escribe el destino antes de decidir. Con `outsideBoard`, los dos parámetros de salida
  contienen coordenadas fuera del tablero. La firma no dice «el destino solo tiene sentido cuando el
  resultado es `legal` o `capture`».
- Una función devuelve un valor. `tryMove` produce tres (resultado, fila, columna), así que dos
  viajan por referencia. `construction-04` los reemplaza con un registro.

## Antes de empezar

- [`construction-02`](../construction-02-structured/lesson-es.md) — tienes sus dos recuentos: sitios
  de edición y aristas entrantes a la entrada de turno.
- Un compilador de C++20 (`g++` o `clang++`). `cmake` es opcional.

No necesitas saber C++ a fondo. Necesitas leer una lista de parámetros, una referencia (`&`) y un
`return`. Esa es toda la destreza que ejercita esta lección.

### Qué necesitas

- La línea base con funciones: [`code/functions-baseline.cpp`](code/functions-baseline.cpp).
- La línea base estructurada:
  [`structured-baseline.cpp`][structured-baseline].
- Los scripts de compilación y las notas: [`run.sh`](run.sh) y [`README-es.md`](README-es.md).
- Las reglas: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §12 fija el orden
  de los sorteos.
- El formato de la transcripción:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- Una traza de referencia: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- Esta plantilla para quien aprende: [`EVIDENCE-es.md`](EVIDENCE-es.md).

Comprueba que los artefactos existen antes de empezar, desde este directorio de la lección:

```bash
test -f code/functions-baseline.cpp &&
test -f ../construction-02-structured/code/structured-baseline.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

Si falta cualquiera de las dos líneas base, la comparación no tiene segundo lado. Registra el
archivo ausente en `EVIDENCE-es.md` §10 y vuelve cuando esté disponible. Una prueba contra una
función que no has leído no es evidencia.

Ambas líneas base reproducen las 20 trazas de `common/traces/` byte a byte. El juego es idéntico;
solo cambia el ámbito de los nombres. Eso es lo que hace que la comparación valga la pena.

## Paso 1 — Inventariar los dos ámbitos (~10 min)

Lee los dos archivos una vez. Después pregúntale al compilador qué ve. Los datos del ámbito de
espacio de nombres con enlace externo aparecen en el archivo objeto; los locales y los parámetros
no.

```bash
mkdir -p /tmp/scope-test
g++ -std=c++20 -c -o /tmp/scope-test/structured.o \
  ../construction-02-structured/code/structured-baseline.cpp
g++ -std=c++20 -c -o /tmp/scope-test/functions.o code/functions-baseline.cpp
nm -C --defined-only -g /tmp/scope-test/structured.o | grep ' [BbDd] '
nm -C --defined-only -g /tmp/scope-test/functions.o | grep ' [BbDd] '
```

El primer listado imprime 16 nombres. El segundo imprime uno:

```text
0000000000000000 B randomState
```

`B` es dato inicializado a cero, `D` es dato inicializado. Ambos son mutables y ambos son
visibles en todas las funciones de esa unidad de traducción. El tamaño del tablero, las tablas de
movimientos y las constantes del generador, todos `constexpr`, no aparecen: las constantes no son
estado y tienen enlace interno.

Después responde en `EVIDENCE-es.md` §2:

1. ¿Cuántos nombres mutables en el ámbito de espacio de nombres define cada archivo? Enuméralos con
   su número de línea.
2. ¿Cuáles de los 16 siguen siendo globales en `construction-03`, y por qué ese?
3. Para cada uno de los otros 15: ¿es ahora un local de `main`, un local de otra función, un
   parámetro por valor o un parámetro por referencia? Nombra la función.
4. ¿Cuántas definiciones de función tiene cada archivo? (Cuenta `main`.)
5. ¿Cuántos parámetros tiene `tryMove`, y cuáles son de salida?
6. ¿Qué funciones dependen de `randomState` sin nombrarlo en su propia lista de parámetros?
7. ¿En qué archivo puedes responder «de qué depende esta función» solo con su línea de definición?

La pregunta 3 es el inventario. La pregunta 6 es el contraejemplo que mantiene honesta la lección.

## Paso 2 — Predecir antes de medir (~8 min)

Dos predicciones, ambas escritas antes de cualquier ejecución: una sobre una llamada y otra sobre un
coste.

### 2.1 Predecir una llamada

El Paso 4 llama a `tryMove` con estos argumentos. Escribe la respuesta ahora, desde el cuerpo de la
función:

```text
caballo activo      fila 2, columna 1
otro caballo        fila 8, columna 8
número de movimiento 8
tablero de visitadas solo la casilla (3,3) está marcada
```

La tabla de movimientos está al principio del archivo: el movimiento 8 cambia la fila en +1 y la
columna en +2. El orden de las comprobaciones dentro de `tryMove` es la parte que conviene leer dos
veces.

Registra en `EVIDENCE-es.md` §3.1: el `MoveOutcome` que esperas, la fila y la columna de destino, y
tu confianza. Si predices `legal`, te saltaste una comprobación; eso es una señal de calibración, no
un error que haya que esconder.

### 2.2 Predecir el coste de R-SKIP

La prueba de cambio es la misma regla que aplicaste en `construction-02`. Léela otra vez y después
predice.

> **R-SKIP — saltar turno.** Cuenta los turnos cuando empiezan, produzcan o no un movimiento. Cuando
> el recuento es un múltiplo positivo de 5, el caballo activo no mueve: el turno se anuncia igual
> con sus campos `turn=` y `order=`, no le sigue ninguna línea `try=` ni ninguna línea `move=`, y el
> turno pasa al otro caballo. Un turno saltado no cuenta para los 200 movimientos exitosos.

R-SKIP no añade ningún sorteo, así que no altera el orden de llamadas de §12 de las reglas. El turno
sigue sorteando e imprimiendo su orden; el salto ocurre después de la cabecera.

Fija las unidades antes de contar.

- Un **sitio de edición** es un cambio contiguo de código que el requisito obliga a hacer: una
  declaración, una asignación, un argumento de formato, una línea de firma o un bloque de
  comprobación con su transición.
- Un **cambio de firma** es una función cuya lista de parámetros o tipo de retorno cambia. Cuéntalo
  una vez, aparte de los sitios de llamada que obliga.
- Un **sitio de llamada** es un lugar donde hay que editar una expresión de llamada porque cambió
  una firma.
- Un **llamador** es una función que contiene un sitio de llamada. Un llamador puede contener
  varios.
- Una **global nueva** es un nombre mutable añadido en el ámbito de espacio de nombres.
- **No** cuentes comentarios, líneas en blanco, formato, trazas generadas ni el número de turnos.

La pregunta que hace el README es la que hay que comprometer: ¿cuántos **llamadores** deben cambiar?
Respóndela con `grep` antes de predecir con intuición:

```bash
grep -n 'playTurn(' code/functions-baseline.cpp
grep -n 'playGame(' code/functions-baseline.cpp
```

Cada nombre aparece dos veces: una definición y una llamada. Ese es el grafo de llamadores, y es dos
líneas más ancho de lo que probablemente esperas.

Después completa la tabla de coste en `EVIDENCE-es.md` §3.2: sitios de edición, cambios de firma,
sitios de llamada, llamadores de `playTurn` cambiados, llamadores de `playGame` cambiados, globales
nuevas, aristas nuevas de flujo de control, archivos tocados y trazas de referencia rotas. Usa tus
números de `construction-02` para la segunda columna. Si no los conservaste, vuelve a deducirlos del
§6 de esa lección.

Comprométete con un número en cada fila. «La versión con funciones es más limpia» no es una
predicción; no dice qué recuento se mueve.

## Paso 3 — Ejecutar las dos líneas base (~5 min)

Desde este directorio de la lección:

```bash
./run.sh --seed 1 --name Learner > /tmp/scope-test/functions-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/scope-test/functions-0001.txt
```

Después, la línea base estructurada:

```bash
cd ../construction-02-structured
./run.sh --seed 1 --name Learner > /tmp/scope-test/structured-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/scope-test/structured-0001.txt
```

Por último, las dos entre sí:

```bash
diff -u /tmp/scope-test/structured-0001.txt /tmp/scope-test/functions-0001.txt
```

Tres diffs vacíos. Los dos primeros dicen que cada archivo es el juego. El tercero dice que los dos
archivos son el mismo juego, que es la única razón por la que la comparación de ámbitos es justa.

Vuelve a este directorio de la lección. Si `run.sh` no puede compilar, compila una vez a mano:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o build/functions-baseline code/functions-baseline.cpp
```

Si un diff no está vacío, detente. Regístralo en `EVIDENCE-es.md` §10. No ajustes la traza para que
coincida con el código.

## Paso 4 — Llamar a una decisión de movimiento con estado local (~7 min)

Este es el criterio de aceptación de la lección: una decisión de movimiento, ejercitada a través de
una función con parámetros, usando solo estado local.

Copia la línea base. El archivo versionado no se toca.

```bash
mkdir -p /tmp/scope-test
cp code/functions-baseline.cpp /tmp/scope-test/functions-probe.cpp
```

En la copia, haz una edición mecánica: cambia `int main(` por `int baselineMain(`. Después añade
esto:

```cpp
const char *outcomeName(MoveOutcome outcome)
{
    switch (outcome)
    {
    case MoveOutcome::outsideBoard:
        return "outside";
    case MoveOutcome::visited:
        return "visited";
    case MoveOutcome::capture:
        return "capture";
    case MoveOutcome::legal:
        return "legal";
    }
    return "unknown";
}

int main()
{
    // Solo estado local: un tablero, un caballo activo, otro caballo.
    bool visitedCells[boardRowCount + 1][boardColumnCount + 1] = {};
    visitedCells[3][3] = true;

    int destinationRow = 0;
    int destinationColumn = 0;
    MoveOutcome outcome = tryMove(2, 1, 8, 8, 8, visitedCells, destinationRow,
                                  destinationColumn);
    std::printf("outcome=%s to=%d,%d\n", outcomeName(outcome), destinationRow,
                destinationColumn);
    return 0;
}
```

Compila y ejecuta:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/scope-test/functions-probe /tmp/scope-test/functions-probe.cpp
/tmp/scope-test/functions-probe
```

La salida es:

```text
outcome=visited to=3,3
```

Compárala con tu predicción de §3.1. Después cambia una entrada cada vez y confirma los otros tres
resultados. El tablero y las coordenadas son tuyos; no se inicializó ningún juego.

| Activo | Movimiento | Otro | Tablero | Salida esperada |
|---|---|---|---|---|
| 2,1 | 8 | 8,8 | (3,3) visitada | `outcome=visited to=3,3` |
| 2,1 | 1 | 8,8 | (3,3) visitada | `outcome=legal to=1,3` |
| 2,1 | 8 | 3,3 | (3,3) visitada | `outcome=capture to=3,3` |
| 1,1 | 2 | 8,8 | (3,3) visitada | `outcome=outside to=-1,2` |

Dos filas merecen un comentario.

- El caso de captura gana al de casilla visitada aunque la casilla esté marcada. `tryMove` comprueba
  la captura antes de las visitadas. El orden de las comprobaciones forma parte del contrato, y la
  prueba es cómo lo ves.
- El caso fuera del tablero imprime `-1,2`, un destino fuera del tablero, porque los parámetros de
  salida se escribieron antes de la comprobación de límites. El resultado dice que el destino no
  significa nada. El tipo no lo dice.

Ahora el sentido del ejercicio. Este `main` nunca llama a `parseArguments`, nunca siembra el
generador, nunca inicializa posiciones, nunca crea el juego. No puede: `tryMove` no necesita nada de
eso. Sus entradas son sus parámetros, y los parámetros se ven en la llamada.

Escribe en `EVIDENCE-es.md` §5:

- El comando exacto que compiló la prueba y la salida exacta.
- Cuál de las cuatro filas predijiste y cuál te sorprendió.
- Una frase: por qué esta llamada es posible en `construction-03` y no en `construction-02`.

Si quieres el contraste, llama a `playTurn` desde la misma prueba. Compila, pero tienes que asignar
`randomState` primero, porque su primera acción sortea con el generador. Registra qué te cuesta: una
línea de preparación y el conocimiento de que la función tiene una entrada que la firma no menciona.

## Paso 5 — Aplicar R-SKIP y contar la frontera (~10 min)

Vuelve a este directorio de la lección. Copia la línea base otra vez a un segundo archivo de
trabajo:

```bash
cp code/functions-baseline.cpp /tmp/scope-test/functions-rskip.cpp
```

Aplica R-SKIP a la copia. La regla es la de `construction-02`; estos son los detalles que
interactúan con el ámbito:

1. El contador de turnos iniciados se declara **dentro de `playGame`**, junto a su bucle, no en
   `main`.
2. `playTurn` lo recibe por referencia y lo incrementa antes del sorteo del orden.
3. `printTurnHeader` recibe el contador en lugar de `successfulMoveCount + 1`.
4. Después de la cabecera y antes del primer intento, si el contador es múltiplo de 5, invierte
   `playerIsActive` y vuelve de `playTurn`.
5. El límite de 200 movimientos y `moves=` siguen sobre `successfulMoveCount`.

¿Por qué no las otras dos colocaciones? Esta tabla es la decisión de ámbito, y merece un minuto.

| Dónde vive el contador | ¿Sobrevive a una llamada? | ¿Visible sin parámetro? | Veredicto |
|---|---|---|---|
| local en `playTurn` | no | no | se reinicia cada turno; no cuenta turnos |
| `static` en `playTurn` | sí | sí | una global con un nombre más pequeño |
| local en `playGame`, por referencia | sí | no | el contador pertenece al bucle |

La regla es el ámbito más estrecho que cubre todos los usos. Todos los usos están dentro del turno;
la vida debe abarcar varios turnos; por lo tanto, el dueño es el bucle.

Cuenta primero y después compara con la tabla siguiente. Es la forma esperada en la revisión que
acompaña a esta lección.

| Sitio | Línea | Qué cambia |
|---|---|---|
| firma de `playTurn` | 232–234 | un parámetro por referencia más |
| primera sentencia de `playTurn` | después de 235 | incrementar el contador |
| llamada a `printTurnHeader` | 247 | `startedTurnCount` reemplaza `successfulMoveCount + 1` |
| después de la cabecera | después de 248 | el bloque de comprobación y su `return` |
| cuerpo de `playGame` | antes de 317 | contador local, inicializado a 0 |
| llamada a `playTurn` | 319–320 | pasar el contador |

Seis sitios de edición, una firma, un sitio de llamada, una arista nueva de flujo de control (el
`return` a la frontera de la función), cero globales nuevas. El llamador de `playGame` —`main`— no
cambia, porque el contador se posee por debajo.

Compila y ejecuta:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/scope-test/functions-rskip /tmp/scope-test/functions-rskip.cpp
/tmp/scope-test/functions-rskip --seed 1 --name Learner > /tmp/scope-test/rskip-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/scope-test/rskip-0001.txt | head
```

R-SKIP cambia el juego, así que el diff no debe estar vacío. Para la semilla 1, la primera
divergencia es el quinto turno: a la cabecera `turn=5` le sigue la cabecera `turn=6` en lugar de una
línea `try=`.

Si todavía tienes el binario de trabajo de `construction-02`, compara las dos transcripciones
modificadas. Deben ser idénticas: una regla, dos formas, el mismo juego.

```bash
diff -u /tmp/rskip/structured-0001.txt /tmp/scope-test/rskip-0001.txt
```

Después ejecuta las 20 semillas contra las trazas de referencia. Todas diferirán, porque el salto
ocurre en el turno 5 en todo juego que llegue hasta ahí. El número mide el cambio de contrato,
no una regresión.

Ahora responde §6.6:

- ¿Cuántos llamadores tiene `playTurn`? ¿Cuántos cambian?
- ¿Cuántos llamadores tiene `playGame`? ¿Cuántos cambian?
- ¿Qué recuento empata con `construction-02` y cuál no?
- ¿Qué compró la firma extra? Nombra la comprobación que ahora hace el compilador.
- ¿Dónde podría filtrarse estado nuevo del turno en cada archivo, y cómo lo notaría un lector?

La respuesta honesta a la tercera pregunta sale de tus propios totales. La versión con funciones
añade una firma y un sitio de llamada; no hace este cambio más barato. Hace que la dependencia nueva
sea imposible de añadir en silencio. Si tu total de `construction-02` era cinco, aquí verás seis.

## Paso 6 — Explicar la diferencia (~3 min)

Escribe §7. En dos partes.

**Primero, una frase.** Di qué decide la firma sobre el cuerpo, sin usar *mejor*, *peor*, *limpio*,
*más limpio*, *bonito*, *elegante*, *feo* ni *modular*. Esas palabras nombran un veredicto, no un
hecho de ámbito. Si tu frase necesita una, has descrito tu reacción.

**Segundo, un párrafo.** Responde:

- ¿Qué nombres puede leer `tryMove` que su llamador no le haya pasado?
- ¿Qué nombres puede leer `playTurn` que su llamador no le haya pasado?
- ¿Qué le hace esa diferencia a la prueba del Paso 4?
- ¿Qué debe sostener un lector en la memoria de trabajo para conocer las entradas de una función en
  cada archivo?
- Si se añade un estado nuevo del turno, ¿dónde debe declararse y qué obliga a pasarlo?
- ¿Qué sigue sin decir la firma, y qué lección posterior lo arregla?

Una buena respuesta separa **el conjunto de nombres que un cuerpo puede alcanzar** del **número de
ediciones que cuesta el cambio**. Una respuesta débil dice que el archivo con funciones está más
limpio. Limpio es lo que ves; una lista de globales vacía y una lista de parámetros llena son lo que
lo causa.

## Paso 7 — Criterio de aceptación (~2 min)

El criterio de aceptación de esta lección es: **una decisión de movimiento se ejercita a través de
una función con parámetros; la prueba usa estado local; la explicación nombra el cambio de ámbito.**

Comprueba tu registro contra él:

- [ ] Ejecuté las dos líneas base sin modificar y obtuve tres diffs vacíos.
- [ ] Inventarié las globales de los dos archivos desde los archivos objeto, no de memoria.
- [ ] Predije el resultado de `tryMove` antes de ejecutar la prueba.
- [ ] La prueba llama al `tryMove` versionado, no a una copia de la regla.
- [ ] La prueba no inicializa estado del juego: ni semilla, ni posiciones, ni `playGame`.
- [ ] Registré el comando exacto de la prueba y su salida.
- [ ] Apliqué R-SKIP con el contador en propiedad de `playGame`.
- [ ] Conté los llamadores con `grep`, no por suposición.
- [ ] Puedo decir qué recuento empata con `construction-02` y cuál no.
- [ ] Mi explicación de una frase evita las ocho palabras prohibidas.
- [ ] No edité las líneas base versionadas.
- [ ] Completé `EVIDENCE-es.md` §1–§9 y la autoverificación de §11.

El criterio no es «gana la versión con funciones». Es «puedes decir exactamente qué hizo visible la
lista de parámetros y cuánto cobró por ello».

## Autocomprobación (binaria)

- [ ] Leí las dos líneas base e inventarié sus ámbitos.
- [ ] Predije un resultado de `tryMove` y el coste de R-SKIP antes de medir.
- [ ] Compilé las dos líneas base y obtuve tres diffs vacíos.
- [ ] Llamé a una decisión de movimiento con un tablero local y sin preparar el juego.
- [ ] Apliqué R-SKIP a una copia de trabajo y conté la frontera.
- [ ] Comparé la predicción con los recuentos y nombré las diferencias.
- [ ] Expliqué la diferencia en una frase más un párrafo, por el ámbito.
- [ ] Completé `EVIDENCE-es.md`.

Son criterios de aceptación. Si queda una casilla
sin marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §9.

## Misconcepciones que esta lección refuta

| Concepción errónea | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| «Un parámetro es una global con un nombre más largo.» | Un parámetro se inicializa en cada llamada y se nombra en el sitio de llamada; una global sobrevive a las llamadas y se ve sin nombrarla. | Pregunta si el llamador puede ver la dependencia. |
| «Un parámetro por referencia es lo mismo que una global.» | El llamador elige el objeto y la elección está en la firma. Una global se elige sola. | Lee la lista de parámetros, no el cuerpo. |
| «Un local `static` es un local.» | Tiene duración de almacenamiento estática y sobrevive a las llamadas. Es una global con un nombre más pequeño y sin parámetro. | Pon el estado en el dueño y pásalo. |
| «Devolver un valor siempre es mejor que un parámetro de salida.» | `tryMove` produce tres resultados; una función devuelve uno. | Cuenta los resultados antes de elegir el mecanismo. |
| «La firma enumera todo lo que hace la función.» | El arreglo decae, así que el número de filas no se enuncia; el destino se escribe en la salida temprana. | Lee el cuerpo para el orden de comprobaciones y los puntos de escritura. |
| «Sin globales del juego no hay estado escondido.» | `randomState` sigue siendo global, y `playTurn` depende de ella. | Enumera los nombres que el cuerpo alcanza y ningún parámetro suministra. |
| «Menos globales hace más barata la prueba de cambio.» | Aquí la edición cuesta una firma y un sitio de llamada, no menos tecleo. | Mide la edición y después mide la prueba. |
| «La prueba duplica el juego.» | Llama al `tryMove` versionado; no reimplementa la regla de movimiento. | Enlaza la función real; nunca copies la regla. |

## Siguiente paso

`construction-04` — registros. `tryMove` ya devuelve tres resultados a través de un valor de retorno
y dos referencias. El siguiente escalón los pone en un solo nombre, para que la firma deje de crecer
un parámetro por campo.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog.md) para ver la ruta
completa.

Si faltaba alguna línea base, no empieces `construction-04`. El siguiente contraste necesita los dos
lados.

[structured-baseline]: ../construction-02-structured/code/structured-baseline.cpp
