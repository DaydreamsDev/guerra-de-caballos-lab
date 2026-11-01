# Referencias (references)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** alias sin direcciones · **Tiempo:** unos 45 minutos · **Estado:** borrador
**Lectura:** ~16 minutos

## Por qué existe esta lección

`construction-05` seleccionaba el caballo activo con un alias local y lo movía con dos asignaciones
en medio de `playTurn`. Ambas escrituras nombraban el mismo registro. La selección y la escritura
eran correctas, pero la escritura no tenía nombre propio, y una llamada a cualquier función auxiliar
no podía decir si el registro cambiaría.

La línea base con referencias sustituye los dos alias locales por dos funciones que devuelven
alias, y las dos asignaciones por una función que escribe a través de un parámetro de referencia. El
comportamiento no cambia: las 20 trazas siguen siendo idénticas byte a byte. Lo que cambia es dónde
se crea el alias y qué permite su tipo.

La frase que hay que retener:

> Una referencia es un segundo nombre para un registro que ya existe; una escritura a través de
> cualquiera de los dos nombres es una escritura al mismo registro.

Se siguen dos mediciones. La primera es de comportamiento: quita un `&` de `moveKnight` y las 20
trazas divergen, porque la función escribe en una copia. La segunda es estructural: el archivo tiene
17 parámetros de tipo `Knight`, 9 son alias mutables, 8 son alias de solo lectura y ninguno es por
valor. El primer número muestra qué hace una referencia; el segundo, cuánto depende el archivo de
ella.

Una nota honesta antes de la comparación: las referencias no son nuevas en este archivo.
`construction-05` ya tenía `initializePositions(Knight &, Knight &)`,
`tryMove(..., int &destinationRow, ...)` y los dos alias locales. Esta lección no introduce la
sintaxis. Convierte el alias en una frontera con nombre, lo separa en mutable y de solo lectura, y
le da a la escritura su propia función. Si esperas una palabra clave nueva, te vas a decepcionar;
la lección está en los tipos.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar el alias sin direcciones y qué permite una referencia;
- mutar un registro a través de un parámetro y mostrar la divergencia por valor.

## El concepto: un registro, dos nombres

Cinco formas de nombre hacen el trabajo en esta comparación:

```text
parámetro por valor      void moveKnight(Knight knight, ...)      la función posee una copia
parámetro de referencia  void moveKnight(Knight &knight, ...)     la función nombra el registro
referencia const         const Knight &otherKnight                puede leer, no escribir
retorno de referencia    Knight &selectActiveKnight(...)          el llamador recibe el registro
alias local              Knight &activeKnight = ...               un segundo nombre en el cuerpo
```

Lado a lado, los tres sitios que nombran al caballo:

| Decisión | `construction-05` | `construction-06` |
|---|---|---|
| Caballo activo | `Knight &` local desde un ternario | `Knight &` devuelto por `selectActiveKnight` |
| Otro caballo | `Knight &` local mutable desde un ternario | `const Knight &` devuelto por `selectOtherKnight` |
| Movimiento | dos asignaciones en `playTurn` | `moveKnight(Knight &, ...)` |

Las dos líneas base difieren en una decisión: cómo se nombra el caballo seleccionado. El diff tiene
21 líneas añadidas y 4 eliminadas, 17 netas. Aparecen tres funciones, las 19 definiciones pasan a
22 y los 63 parámetros pasan a 72. Ninguna firma existente cambia de tipo y ningún sitio de llamada
existente cambia un argumento. La transcripción no se mueve.

Lado a lado, la diferencia es lo que promete un parámetro:

| Pregunta | `Knight` por valor | `Knight &` |
|---|---|---|
| Qué guarda el parámetro | una copia | un nombre para el registro del llamador |
| Una escritura dentro de la función | cambia la copia | cambia el registro del llamador |
| Qué ve el llamador después de la llamada | nada | los valores nuevos de los campos |
| Qué dice el sitio de llamada sobre la mutación | nada | nada |
| Puede el parámetro ser nulo | no | no; debe vincularse a un registro |
| Puede cambiar de destinatario después | no aplica | no; la asignación copia el registro |
| Qué cuesta la llamada | ocho bytes copiados | un nombre pasado |
| Qué comprueba el compilador | una copia escrita y nunca leída (advertencia) | vínculos y violaciones de `const` |

A nivel de lenguaje, una referencia no expone una dirección ni añade una al código fuente. La
implementación puede usar una dirección; el código no. Por eso este peldaño va antes que los
punteros: aprendes el alias sin la dirección, y después `construction-07` te da la dirección y la
factura.

Lo que una referencia **no** es, en esta lección:

- **No es un puntero.** No hay `&` en la llamada, no hay `*` para leer, no hay aritmética y no hay
  valor nulo.
- **No es una copia.** `moveKnight(activeKnight, ...)` escribe en el registro que pasó el llamador,
  no en un duplicado que se tira al terminar.
- **No se puede reasignar.** `activeKnight = machineKnight;` copia el registro de la máquina dentro
  del registro del jugador. No hace que `activeKnight` nombre a la máquina a partir de ahí.
- **No es un propietario.** La referencia no prolonga el ciclo de vida del registro. Una referencia
  que sobrevive a su registro queda colgando, y el compilador no te detiene.
- **No es visible en el sitio de llamada.** `moveKnight(activeKnight, destinationRow,
  destinationColumn)` parece una lectura. La firma es el único lugar que dice lo contrario.
- **No es una medida de seguridad.** Una referencia mutable le da a la función todos los campos. No
  protege ningún invariante. Eso empieza en `construction-10` y `construction-11`.

Se siguen tres consecuencias:

1. **La selección tiene un solo hogar.** `selectActiveKnight` devuelve el alias, así que `playTurn`
   no repite el ternario que elige entre los dos registros.
2. **La escritura tiene un solo hogar.** `moveKnight` escribe los dos campos, así que las dos
   asignaciones que tenían que mantenerse juntas no pueden separarse.
3. **El lado de solo lectura está impuesto.** `selectOtherKnight` devuelve `const Knight &`, así que
   una llamada que intente mover al otro caballo no compila.

El costo es lo cuarto: el sitio de llamada no dice nada. Quien lee tiene que conocer la firma, y el
compilador es el único que la comprueba.

## Antes de empezar

- [`construction-05`](../construction-05-array-of-records/lesson-es.md) — tienes su tablero de
  registros, su prueba de cambio sobre `moveNumber` y su distinción entre lo que un registro
  localiza y lo que no.
- Un compilador de C++20 (`g++` o `clang++`). `cmake` es opcional.

No necesitas saber C++ a fondo. Necesitas leer una lista de parámetros, un tipo de retorno y una
llamada. La única marca nueva es `&`, y la lección trata sobre lo que esa marca permite, no sobre
cómo se escribe.

### Qué necesitas

- La línea base con referencias: [`code/references-baseline.cpp`](code/references-baseline.cpp).
- La línea base con arreglo de estructuras:
  [`board-cells-baseline.cpp`](../construction-05-array-of-records/code/board-cells-baseline.cpp).
- Los scripts de compilación y las notas: [`run.sh`](run.sh) y [`README-es.md`](README-es.md).
- Las reglas: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §12 fija el
  orden de los sorteos.
- El formato de la transcripción:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- Una traza de referencia: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- Esta plantilla para quien aprende: [`EVIDENCE-es.md`](EVIDENCE-es.md).

Comprueba que los artefactos existen antes de empezar, desde este directorio de la lección:

```bash
test -f code/references-baseline.cpp &&
test -f ../construction-05-array-of-records/code/board-cells-baseline.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

Si falta cualquiera de las dos líneas base, la comparación no tiene segundo lado. Registra el
archivo ausente en `EVIDENCE-es.md` §11 y vuelve cuando esté disponible. Un conteo contra un archivo
que no has leído no es evidencia.

Ambas líneas base reproducen las 20 trazas de `common/traces/` byte a byte antes de que cambies
nada. El mismo juego, dos formas de nombrar al caballo. Eso es lo que hace justa la comparación.

## Paso 1 — Inventariar los alias (~8 min)

Lee los dos archivos una vez. Después deja que el texto te diga dónde se nombra un caballo por
referencia:

```bash
grep -n 'Knight &' code/references-baseline.cpp
grep -n 'const Knight &' code/references-baseline.cpp
grep -n 'selectActiveKnight\|selectOtherKnight\|moveKnight' code/references-baseline.cpp
```

El primer comando también encuentra las líneas `const Knight &`; el segundo imprime el subconjunto
de solo lectura. El tercero imprime las tres funciones nuevas y cada llamada a ellas.

Después lee el cambio completo en una sola vista:

```bash
diff -u ../construction-05-array-of-records/code/board-cells-baseline.cpp \
  code/references-baseline.cpp
```

Tres bloques: las tres funciones nuevas, los dos alias locales que pasaron a ser llamadas, y las
dos asignaciones que pasaron a ser una llamada.

Completa esta tabla en `EVIDENCE-es.md` §2.1 mientras lees. Los números de línea son de la línea
base con referencias.

| Función | Línea | Parámetros `Knight` | Forma | Lee | Escribe |
|---|---|---|---|---|---|
| `initializePositions` | 125 | 2 | `Knight &` | — | `row`, `column` |
| `printGameHeader` | 149 | 2 | `const Knight &` | `row`, `column` | — |
| `printTurnHeader` | 171 | 1 | `const Knight &` | `row`, `column` | — |
| `printMove` | 206 | 1 | `const Knight &` | `row`, `column` | — |
| `selectActiveKnight` | 218 | 2 | `Knight &`, devuelve `Knight &` | — | — (concede acceso de escritura) |
| `selectOtherKnight` | 223 | 2 | `const Knight &`, devuelve `const Knight &` | — | — |
| `moveKnight` | 229 | 1 | `Knight &` | — | `row`, `column` |
| `tryMove` | 235 | 2 | `const Knight &` | `row`, `column` | — |
| `playTurn` | 258 | 2 | `Knight &` | mediante auxiliares | mediante `moveKnight` |
| `playGame` | 327 | 2 | `Knight &` | — | reenvía |
| `main` | 345 | 0 | declara dos registros | — | inicializa |

Recuentos: 9 parámetros de caballo mutables, 8 de solo lectura, 0 por valor y 2 retornos de
referencia.

Dos filas merecen un minuto.

- `playGame` recibe los dos caballos como referencias mutables y no escribe ninguno. Los reenvía a
  `playTurn`. El alias mutable es un paso de testigo, igual que el parámetro del tablero en
  `construction-05`. El tipo no dice "escribo"; dice "puedo".
- `printGameHeader`, `printTurnHeader` y `printMove` reciben `const Knight &` y solo leen. Podrían
  recibir `Knight` por valor y seguirían siendo correctas. La copia costaría ocho bytes por llamada
  sin beneficio, pero la corrección no es la razón de la referencia; el acceso lo es.

Después responde en `EVIDENCE-es.md` §2:

1. ¿Cuántas funciones reciben un caballo por referencia mutable? ¿Por referencia `const`? ¿Por
   valor?
2. ¿Qué dos funciones devuelven un alias de caballo, y qué permite cada tipo de retorno?
3. ¿Qué función es la única que escribe los campos de un caballo a través de un parámetro?
4. ¿Qué tres funciones podrían recibir un caballo por valor sin cambiar el comportamiento, y por
   qué el valor no es el criterio correcto allí?
5. ¿Qué dos líneas de `construction-05` desaparecieron, y qué las sustituyó?
6. ¿Por qué los parámetros de `selectOtherKnight` son `const` además de su tipo de retorno?
7. ¿Qué permitiría `Knight &otherKnight` que `const Knight &otherKnight` impide?
8. ¿Cuáles de estos alias ya existían en `construction-05` antes de esta lección? Nómbralos por
   línea.

La pregunta 4 es la que separa "por valor está mal" de "por valor está mal donde una escritura debe
escapar". La pregunta 8 es la comprobación de honestidad: la sintaxis ya estaba allí.

## Paso 2 — Predecir antes de ejecutar (~8 min)

### 2.1 El cambio de un solo token

El cambio bajo prueba es un token: elimina el `&` después de `Knight` en la firma de `moveKnight`.
No toques el cuerpo, la llamada ni ninguna otra función.

```cpp
void moveKnight(Knight knight, int destinationRow, int destinationColumn)
```

El código sigue siendo C++ válido. Asigna `knight.row` y `knight.column`, y termina. La asignación
es legal porque `knight` es un objeto local de la función.

Escribe estas predicciones antes de compilar:

| Pregunta | Mi predicción |
|---|---|
| ¿Compila con `-Wall -Wextra -Werror`? | |
| Si no, ¿qué clase de diagnóstico aparece? | |
| Primera línea de `seed-0001.txt` que difiere (número y texto) | |
| ¿Sigue apareciendo la línea `move=`? | |
| ¿Qué dice el siguiente encabezado `turn=` de ese caballo? | |
| Movimientos de la ejecución por valor en la semilla 1 (oro: 21) | |
| Semillas que difieren, de 20 | |
| Cambios de firma / sitios de llamada editados / caracteres eliminados | |
| ¿Termina el juego por valor? | |
| Líneas añadidas o eliminadas en el cuerpo | |

El argumento que estás probando: el movimiento se imprime antes de llamar a `moveKnight`, a partir
de `destinationRow` y `destinationColumn` calculados en `playTurn`. La llamada es lo único que
escribe la posición nueva en el registro. Si la llamada escribe en una copia, la impresión sobrevive
y la posición no.

### 2.2 Los tipos de retorno del selector

Tres cambios de una línea, cada uno en una copia de trabajo. Responde antes de compilar en
`EVIDENCE-es.md` §3.2.

| Cambio | ¿Compila? | Qué dice el compilador | Qué haría el programa |
|---|---|---|---|
| `selectActiveKnight` devuelve `Knight` | | | |
| `selectActiveKnight` devuelve `const Knight &` | | | |
| `selectOtherKnight` devuelve `Knight &` | | | |

La línea que recibe el resultado es
`Knight &activeKnight = selectActiveKnight(playerIsActive, playerKnight, machineKnight);`.
Úsala. Una referencia no constante no puede vincularse a un temporal ni a un objeto `const`.

### 2.3 Los límites

Responde ahora, sin probar:

- ¿Puede una referencia ser nula, de modo que "ningún caballo" sea representable?
- ¿Se puede hacer que `activeKnight` nombre al otro registro después de inicializarse?
- ¿`moveKnight(activeKnight, ...)` le dice a quien lee que el argumento puede cambiar?
- ¿Qué pasa si una referencia sobrevive al registro que nombra?
- ¿Cuál de las cuatro cosas detecta el compilador?

Escribe las respuestas en `EVIDENCE-es.md` §3.3. Los pasos 4 y 5 las comprueban.

## Paso 3 — Ejecutar ambas líneas base (~5 min)

Crea primero el directorio de trabajo; todos los comandos siguientes escriben allí.

```bash
mkdir -p /tmp/reference-test
./run.sh --seed 1 --name Learner > /tmp/reference-test/references-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/reference-test/references-0001.txt
```

Después la línea base anterior:

```bash
cd ../construction-05-array-of-records
./run.sh --seed 1 --name Learner > /tmp/reference-test/board-cells-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/reference-test/board-cells-0001.txt
```

Después las dos entre sí:

```bash
diff -u /tmp/reference-test/board-cells-0001.txt /tmp/reference-test/references-0001.txt
```

Tres diffs vacíos. Vuelve a este directorio de la lección y confirma las 20 semillas, no solo la
primera:

```bash
cd ../construction-06-references
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

Un bucle silencioso son 20 coincidencias. Si `run.sh` no puede compilar, compila una vez a mano:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o build/references-baseline code/references-baseline.cpp
```

Si un diff no está vacío, detente. Regístralo en `EVIDENCE-es.md` §11. No ajustes una traza para
que coincida con el código.

## Paso 4 — Romper la frontera (~10 min)

Copia la línea base. El archivo versionado no se toca.

```bash
cp code/references-baseline.cpp /tmp/reference-test/by-value.cpp
```

Elimina el `&`:

```cpp
void moveKnight(Knight knight, int destinationRow, int destinationColumn)
```

Compila con las opciones del proyecto:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/reference-test/by-value /tmp/reference-test/by-value.cpp
```

### 4.1 Por qué se queja el compilador

Con GCC, la compilación se detiene:

```text
error: parameter 'knight' set but not used
```

La opción relevante es `-Werror=unused-but-set-parameter`. Tu compilador puede imprimir el
mensaje en otro idioma; la clase de diagnóstico es `unused-but-set-parameter`. Lee lo que dice: el
parámetro se escribe y nunca se lee. Un parámetro por valor que se escribe y nunca se lee es un
callejón sin salida local. El compilador no sabe que querías cambiar el registro del llamador. Solo
ve una escritura que nadie va a observar.

Esa es la primera evidencia, y llega antes de ejecutar el programa. El cambio es C++ válido, así
que el compilador no puede rechazarlo por principio; solo puede advertir sobre la escritura muerta.

### 4.2 Qué muestra la transcripción

Si la compilación se detuvo, degrada esa advertencia de error a advertencia, para poder compilar y
observar el programa:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -Wno-error=unused-but-set-parameter -O2 \
  -o /tmp/reference-test/by-value /tmp/reference-test/by-value.cpp
/tmp/reference-test/by-value --seed 1 --name Learner \
  > /tmp/reference-test/by-value-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/reference-test/by-value-0001.txt | head -20
```

Si tu compilador no conoce esa opción, quita `-Werror` solo para esta compilación de trabajo.

Las primeras líneas útiles del diff:

```text
 move=1 machine 4,4->3,6
-turn=3 who=machine at=3,6 order=3,4,5,6,7,8,1,2
+turn=3 who=machine at=4,4 order=3,4,5,6,7,8,1,2
```

La línea `move=` sigue ahí, y sigue diciendo `4,4->3,6`. El siguiente turno de la máquina es la
prueba: el caballo sigue en `4,4`. El movimiento se imprimió antes de la llamada, a partir de
valores calculados en `playTurn`; la llamada escribió en una copia que murió en la llave de cierre.

Así que la transcripción no deja de mostrar el movimiento. Deja de mostrar su **efecto**, y se
contradice una línea después. Esa es una observación más fuerte que una línea ausente: una
transcripción puede imprimir una intención y aun así refutarla.

Ahora las 20 semillas:

```bash
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/reference-test/by-value --seed "${seedNumber}" --name Learner \
    > "/tmp/reference-test/by-value-${traceName}.txt"
  if cmp -s "${trace}" "/tmp/reference-test/by-value-${traceName}.txt"; then
    echo "seed ${seedNumber} match"
  else
    echo "seed ${seedNumber} differs"
  fi
done
```

Las 20 difieren. La semilla 1 termina después de 9 movimientos en vez de 21. El caballo obsoleto
cambia cada decisión de movimiento legal, así que todo el juego diverge, no solo un encabezado.

### 4.3 Restaurar la referencia

Restaura el `&` en la copia de trabajo, o copia otra vez la línea base:

```bash
cp code/references-baseline.cpp /tmp/reference-test/by-value.cpp
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/reference-test/by-value /tmp/reference-test/by-value.cpp
```

La compilación vuelve a estar limpia. Ejecuta otra vez el bucle de 20 semillas del paso 3 y
obtendrás 20 coincidencias.

Cuatro observaciones, cada una parte de la respuesta:

**Uno — un carácter cambió la semántica.** La firma cambió; el cuerpo no; ningún sitio de llamada
cambió. La llamada `moveKnight(activeKnight, destinationRow, destinationColumn)` se lee igual en las
dos versiones. Quien llama no puede ver la diferencia. Ese es el precio de un alias que no está
marcado en la llamada.

**Dos — la escritura ocurrió, en una copia.** `knight.row = destinationRow;` se ejecutó. El registro
de `playTurn` no cambió. Ambas afirmaciones son ciertas a la vez porque `knight` y `activeKnight`
son dos objetos distintos.

**Tres — el siguiente encabezado es la evidencia.** La línea `move=` se imprime con valores locales
antes de la llamada; prueba la intención, no la mutación. El encabezado `turn=` del mismo caballo en
su siguiente turno lee el registro. Ahí se expone la copia.

**Cuatro — el compilador vio la escritura muerta.** `-Wunused-but-set-parameter` no es una
advertencia de estilo. Dice que el parámetro se asigna y nunca se lee, que es exactamente lo que es
una mutación por valor. La advertencia es fácil de silenciar y fácil de ignorar; los 20 diffs no.

Escribe `EVIDENCE-es.md` §5: los comandos exactos, el diff, los 20 resultados, qué predicciones se
cumplieron y cuáles fallaron. El fallo habitual es la compilación: se predice que por valor compila
sin problemas. Es C++ válido, pero con GCC las opciones del proyecto convierten la advertencia de
escritura muerta en error, así que la primera observación es una compilación fallida, no una
transcripción equivocada.

## Paso 5 — `const` es una vía de un solo sentido (~7 min)

Copia la línea base a un segundo archivo de trabajo:

```bash
cp code/references-baseline.cpp /tmp/reference-test/const-check.cpp
```

Añade una línea después de los dos alias en `playTurn`:

```cpp
otherKnight.row = 0;
```

Compílalo. La escritura se rechaza:

```text
error: assignment of member 'Knight::row' in read-only object
```

El texto del mensaje depende del compilador y de su idioma. La causa no: `otherKnight` es un
`const Knight &`, así que cada uso de ese nombre es de solo lectura.

Sustituye la línea por una llamada a la función que muta:

```cpp
moveKnight(otherKnight, 1, 1);
```

Compila otra vez. Se rechaza por la misma razón:

```text
error: binding reference of type 'Knight&' to 'const Knight' discards qualifiers
```

Ahora vuelve a poner las dos líneas:

```cpp
otherKnight.row = 0;
moveKnight(otherKnight, 1, 1);
```

Quita `const` de `selectOtherKnight`, tanto de su tipo de retorno como de sus dos parámetros:

```cpp
Knight &selectOtherKnight(bool playerIsActive, Knight &playerKnight, Knight &machineKnight)
```

Compila. Las dos líneas compilan. No ejecutes el resultado: movería al otro caballo y cambiaría el
juego. Restaura la copia antes de continuar.

Qué prueba el par:

- `const` se comprueba en cada uso del nombre, no solo donde se declara el alias.
- Pasar un alias `const` a un parámetro `Knight &` falla, así que el compilador impide mover al
  caballo equivocado. Es una defensa de tipos, no una convención.
- La defensa es tan fuerte como los alias. Los registros del jugador y de la máquina siguen siendo
  mutables en `main`; cualquier alias no constante puede escribirlos.

Conserva el conteo: dos líneas que no compilan con `const`, y una función cuyo `const` se eliminó
para que compilen. Una ejecución no puede probar lo que un tipo impide; el compilador es la
evidencia.

## Paso 6 — Comparar y explicar (~5 min)

Escribe `EVIDENCE-es.md` §8. Dos partes.

**Primero, una frase.** Di qué decide una referencia sobre un registro, sin usar las palabras
*pointer*, *address*, *mejor*, *peor*, *limpio*, *limpia*, *bonito*, *elegante*, *feo* ni
*modular*. Si tu frase necesita una de ellas, has descrito tu reacción al archivo y no al tipo.

**Segundo, un párrafo.** Responde:

- Qué decide un tipo de parámetro: si la función posee una copia o nombra el registro del llamador.
- Qué decide un tipo de retorno: qué registro nombra el alias del llamador, elegido en ejecución.
- Qué decide `const`, y dónde lo comprueba el compilador.
- Qué dice el sitio de llamada sobre la mutación, y qué oculta.
- Qué localizan las referencias: la selección, la escritura y el lado de solo lectura.
- Qué **no** localizan: nulabilidad, reasignación, ciclo de vida, propiedad y visibilidad en la
  llamada.
- Qué puede probar el compilador, y qué solo prueba una ejecución.
- Qué lección posterior corrige cada hueco restante: `construction-07` para direcciones, nulos y
  reasignación; `construction-09` para límites y representación; `construction-10` y
  `construction-11` para invariantes y acceso; `construction-16` para propiedad y ciclo de vida.

Una respuesta fuerte mantiene separadas tres cosas: **qué permite la firma**, **qué muestra el
sitio de llamada** y **qué impone el compilador**. Una respuesta débil dice que las referencias son
más limpias, lo que oculta las tres.

## Paso 7 — Comprobación de aceptación (~2 min)

El criterio de aceptación de esta lección es: **mutar un registro a través de un parámetro de
referencia, mostrar que un parámetro por valor no puede cambiar el registro del llamador, y
mantener las 20 trazas pasando con la versión con referencias.**

Comprueba tu registro contra él:

- [ ] Ejecuté las dos líneas base sin modificar y obtuve tres diffs vacíos, más 20 semillas
      coincidentes.
- [ ] Inventarié los 17 parámetros de caballo: 9 mutables, 8 de solo lectura, 0 por valor.
- [ ] Identifiqué `selectActiveKnight` devolviendo `Knight &` y `selectOtherKnight` devolviendo
      `const Knight &`.
- [ ] Identifiqué `moveKnight(Knight &, ...)` como la función que muta a través de un parámetro.
- [ ] Predije el resultado por valor antes de compilar, incluido el resultado de la compilación.
- [ ] Cambié `moveKnight` para recibir `Knight` por valor y vi que el registro del llamador dejaba
      de cambiar.
- [ ] Expliqué por qué la línea `move=` sigue apareciendo mientras el siguiente encabezado `turn=`
      está obsoleto.
- [ ] Ejecuté la versión por valor en las 20 semillas y vi 20 diferencias.
- [ ] Restauré la referencia y obtuve 20 coincidencias otra vez.
- [ ] Mostré que `const` rechaza tanto una escritura directa como una llamada a `moveKnight`.
- [ ] Mostré que quitar `const` hace que ambas escrituras compilen.
- [ ] Expliqué qué hace explícito una referencia y qué sigue ocultando en el sitio de llamada.
- [ ] Mi frase de una oración evita las dos palabras de sintaxis y las ocho palabras de veredicto.
- [ ] No edité ninguna de las dos líneas base versionadas.
- [ ] Completé `EVIDENCE-es.md` §1–§10 y la autocomprobación de §12.

El criterio no es "las referencias son mejores que los valores". Es "puedes decir qué permite un
tipo de parámetro, qué ve quien llama y qué comprobación notaría si el permiso fuera incorrecto".

## Autocomprobación (binaria)

- [ ] Leí ambas líneas base e inventarié cada parámetro y alias de caballo.
- [ ] Predije la compilación por valor, la transcripción y los conteos de semillas antes de medir.
- [ ] Compilé ambas líneas base y reproduje las 20 trazas.
- [ ] Cambié `moveKnight` a por valor, observé la posición obsoleta y conté 20 diferencias.
- [ ] Restauré la referencia y reproduje las 20 trazas.
- [ ] Usé `const` para rechazar una escritura equivocada y una llamada equivocada.
- [ ] Comparé la predicción con la observación y nombré los fallos.
- [ ] Expliqué qué localiza una referencia y qué no.
- [ ] Completé `EVIDENCE-es.md`.

Estos son criterios de aceptación. Si una casilla queda
sin marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §10.

## Misconcepciones que esta lección refuta

| Error de concepto | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| "Una referencia es un puntero con sintaxis más agradable." | No hay dirección en el código, ni aritmética, ni nulo, ni reasignación. Eso son rasgos de puntero, y llegan en `construction-07`. | Lee la firma; cuenta lo que permite. |
| "Pasar por referencia siempre es más rápido." | Un registro de dos enteros es barato de copiar. La razón aquí es la mutación, no la velocidad. | Pregunta si una escritura debe sobrevivir al retorno. |
| "El sitio de llamada muestra que el registro puede cambiar." | `moveKnight(activeKnight, ...)` no tiene marca. La firma es el único lugar donde vive la decisión. | Lee la declaración, no la llamada. |
| "Una referencia puede ser nula." | No puede; debe vincularse a un registro. "Ningún caballo" no es representable, y eso es un costo, no una ventaja. | Usa un puntero cuando la ausencia es un estado real. |
| "Puedes hacer que una referencia nombre otro registro después." | `activeKnight = machineKnight;` copia los valores de la máquina en el registro del jugador; no reasigna el nombre. | Reasigna con un puntero, o vuelve a seleccionar. |
| "Los parámetros por valor siempre están mal." | Las tres funciones de impresión solo leen; por valor es correcto allí, solo que copia sin necesidad. | Pregunta dónde debe aterrizar la escritura. |
| "La traza prueba la mutación." | La línea `move=` se imprime antes de la llamada. El siguiente encabezado `turn=` prueba la posición. | Compara el encabezado, no la línea de intención. |
| "El compilador detecta el error de por valor." | Advierte sobre un parámetro escrito que nunca se lee. Sin `-Werror` el programa compila y se ejecuta mal. | Trata la advertencia como evidencia y ejecuta las trazas. |
| "`const` impide toda mutación." | Impide escrituras a través de ese nombre. Otro alias no constante puede escribir el registro. | Cuenta los alias, no las palabras `const`. |
| "La referencia hace que el código sea seguro." | Cualquier alias mutable puede escribir cualquier campo. Los invariantes y el control de acceso llegan después. | Eso es `construction-10` y `construction-11`. |

## Siguiente paso

`construction-07` — punteros. Una referencia es un alias que debe vincularse, no puede ser nula y no
puede reasignarse. Un puntero es una dirección que puedes guardar, comparar, anular y cambiar. El
sitio de llamada muestra el `&`, así que la mutación vuelve a ser visible; la factura son las
comprobaciones de nulo y el ciclo de vida.

Después de los punteros, `construction-08` pone el tablero detrás de una frontera de módulo, y
`construction-09` le da al tablero sus propios límites para que el `1..8` deje de ser un hecho que
el llamador debe recordar.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog-es.md) para la ruta completa.

Si faltaba cualquiera de las dos líneas base, no empieces `construction-07`. El siguiente contraste
necesita los dos lados.
