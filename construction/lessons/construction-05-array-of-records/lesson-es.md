# Arreglo de estructuras (array of records)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** colección y recorrido · **Tiempo:** unos 62 minutos · **Estado:** borrador
**Lectura:** ~24 minutos

## Por qué existe esta lección

`construction-04` le dio un registro al caballo y dejó el tablero en paz. El tablero sigue siendo
`bool visitedCells[9][9]`: 64 valores de verdad, cada uno alcanzado por un par de índices escrito
en el sitio que lo necesita. El registro agrupó lo que pertenecía a un caballo. El tablero es una
colección de cosas que necesitan los mismos campos, y nunca recibió el mismo trato.

El paso mecánico es pequeño: declarar un registro para la casilla y cambiar el tipo de elemento
del arreglo. La lección es lo que eso cambia sobre la unidad. El tablero deja de ser una colección
de hechos y pasa a ser una colección de casillas. El almacenamiento y el recorrido empiezan en el
mismo lugar.

La frase que hay que retener:

> Un arreglo de estructuras hace que la casilla sea a la vez la unidad de almacenamiento y la
> unidad de recorrido.

Las dos mitades se pueden medir. La primera es un conteo: un hecho por casilla cuesta una línea en
el tipo y una línea en cada sitio de escritura, con cero firmas y cero arreglos nuevos. La segunda
es una lectura: el parámetro del tablero nombra un tipo de elemento y no dice qué campos lee el
cuerpo. El registro no eliminó la aritmética de índices; eliminó el segundo arreglo que tenía que
coincidir con el primero.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar colección y recorrido sobre un arreglo de registros;
- recorrer el tablero sin 64 variables separadas.

## El concepto: un elemento, todos los hechos que posee

Cinco formas de nombre hacen el trabajo en esta comparación:

```text
arreglo de hechos  bool visitedCells[9][9];              el elemento es un valor de verdad
arreglo paralelo   bool visitedCells[9][9];
                   int  visitedMoveNumbers[9][9];        dos elementos por casilla, una convención
elemento registro  struct BoardCell { bool visited; };  el elemento es una casilla
colección          BoardCell boardCells[9][9];           el tablero es una colección de casillas
recorrido          for row ... for column ...            todas las casillas, en un par de bucles
```

Las dos líneas base difieren en exactamente una decisión, y el diff es pequeño: 15 líneas añadidas
y 10 eliminadas. Cinco clases de cambio:

1. Un tipo de elemento nuevo, cinco líneas.
2. Tres firmas cambian de tipo, con la misma aridad: `tryMove`, `playTurn`, `playGame`.
3. Tres sitios de llamada cambian un nombre de argumento, con el mismo número de argumentos.
4. Tres accesos a campo ganan `.visited`: una lectura y dos escrituras.
5. La declaración en `main` cambia de tipo, y también la llamada que hace.

Nada más se mueve. El orden de los sorteos es idéntico, el flujo de control es idéntico, la
transcripción es idéntica, el archivo sigue teniendo 19 definiciones de función y 63 parámetros.

Lado a lado, la diferencia es un sustantivo:

| Pregunta | `bool visitedCells[9][9]` | `BoardCell boardCells[9][9]` |
|---|---|---|
| Qué es un elemento | un valor de verdad | una casilla con campos |
| Cómo se lee el hecho de visitada | `visitedCells[r][c]` | `boardCells[r][c].visited` |
| Cómo se lee un segundo hecho por casilla | un segundo arreglo, los mismos subíndices | otro campo, el mismo elemento |
| Qué cuesta un hecho nuevo por casilla | una declaración, parámetros, argumentos | una línea en el tipo |
| Sobre qué itera un recorrido | valores de verdad | casillas |
| Qué mantiene dos hechos en una casilla | subíndices iguales, por convención | el elemento mismo |
| Qué sabe el tablero de su tamaño | nada; el tamaño está en constantes | nada; el tamaño está en constantes |

Esa es la versión de colección de la afirmación de `construction-04`. Un campo no llega a la
firma. Un campo por casilla no llega a la firma en absoluto, porque el tablero ya cruza todas las
fronteras que el campo necesita cruzar.

Se siguen tres consecuencias:

1. **El hecho y la casilla no pueden separarse.** Un elemento, un arreglo. Un segundo hecho de la
   misma casilla es otro campo del mismo elemento, seleccionado por el mismo par de coordenadas.
2. **El recorrido se escribe una vez.** Dos bucles cubren 64 casillas y no nombran ninguna. No hay
   una lista de 64 variables que mantener en sintonía con el tablero.
3. **Un segundo hecho por casilla es una línea en el tipo.** No es un segundo arreglo, ni un
   segundo parámetro, ni un segundo argumento en el sitio de llamada. Un tercer hecho es una línea
   más. El total de parámetros sigue en 63.

Lo que el arreglo de estructuras **no** es, en esta lección:

- **No es un contenedor.** `BoardCell boardCells[9][9]` es un arreglo crudo de C. No conoce su
  tamaño, no comprueba límites y decae a puntero cuando se pasa. Los límites viven en dos
  constantes y en la memoria de quien llama.
- **No es encapsulación.** Toda función que tenga el tablero puede escribir cualquier campo de
  cualquier casilla. `boardCells[0][0].visited = true;` compila en toda función que reciba el
  tablero. Nada aquí lo impide.
- **No es independencia de la representación.** La forma 9×9, la fila 0 y la columna 0 sin usar, y
  los nombres de campo son la representación. Aplanar el arreglo reescribe todos los sitios de
  acceso. `construction-09` pone precio a ese cambio.
- **No es una abstracción del recorrido.** Sigues escribiendo los dos bucles y los dos límites.
  `construction-17` sustituye los bucles por un algoritmo; esta lección solo hace posible escribir
  el bucle una vez.

## Antes de empezar

- [`construction-04`](../construction-04-records/lesson-es.md) — tienes su registro, sus dos
  conteos (firmas y sitios de llamada por `captured`) y su distinción entre lo que un registro
  localiza y lo que no.
- Un compilador de C++20 (`g++` o `clang++`). `cmake` es opcional.

No necesitas saber C++ a fondo. Necesitas leer la declaración de un arreglo, un tipo de elemento,
un par de subíndices (`boardCells[row][column]`) y un `for` anidado. Esa es toda la destreza.

### Qué necesitas

- La línea base con arreglo de estructuras:
  [`code/board-cells-baseline.cpp`](code/board-cells-baseline.cpp).
- La línea base con registros: [`records-baseline.cpp`][records-baseline].
- Los scripts de compilación y las notas: [`run.sh`](run.sh) y [`README-es.md`](README-es.md).
- Las reglas: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §12 fija el
  orden de los sorteos.
- El formato de la transcripción:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- Una traza de referencia: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- Esta plantilla para quien aprende: [`EVIDENCE-es.md`](EVIDENCE-es.md).

Comprueba que los artefactos existen antes de empezar, desde este directorio de la lección:

```bash
test -f code/board-cells-baseline.cpp &&
test -f ../construction-04-records/code/records-baseline.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

Si falta cualquiera de las dos líneas base, la comparación no tiene segundo lado. Registra el
archivo ausente en `EVIDENCE-es.md` §11 y vuelve cuando esté disponible. Un conteo contra un
archivo que no has leído no es evidencia.

Ambas líneas base reproducen las 20 trazas de `common/traces/` byte a byte antes de que cambies
nada. El mismo juego, dos tipos de elemento. Eso es lo que hace justa la comparación.

## Paso 1 — Leer el tablero y sus clientes (~10 min)

Lee los dos archivos una vez. Después deja que el texto te diga dónde aparece el tipo de elemento:

```bash
grep -n 'BoardCell' code/board-cells-baseline.cpp
grep -n '\.visited' code/board-cells-baseline.cpp
grep -n 'boardCells' code/board-cells-baseline.cpp
```

El primer comando imprime cinco líneas: la definición del tipo, tres parámetros y una declaración.
El segundo imprime tres accesos a campo. Completa esta tabla en `EVIDENCE-es.md` §2.1 mientras
lees.

| Función | Línea | Recibe | Campos que lee | Campos que escribe |
|---|---|---|---|---|
| `tryMove` | 218 | `BoardCell[][9]` | `visited` | — |
| `playTurn` | 241 | `BoardCell[][9]` | mediante `tryMove` | `visited` |
| `playGame` | 310 | `BoardCell[][9]` | — | — |
| `main` | 328 | — | — | declara y pasa |

Dos filas merecen un minuto.

- `playGame` recibe el tablero y no lee ningún campo. Es un pasamanos, exactamente como lo era
  para los dos caballos en `construction-04`. El cambio de tipo de elemento no le dio una opinión
  sobre las casillas. Su línea de firma cambió de tipo y nada más.
- `main` declara el tablero y nunca toca un campo. Solo lo entrega. Los campos se conocen en
  exactamente dos funciones, mientras que cuatro nombres mencionan el tipo.

Después responde en `EVIDENCE-es.md` §2:

1. ¿Cuántas funciones reciben el tablero? ¿Cuántas leen un campo? ¿Cuántas escriben uno?
2. ¿Qué función recibe el tablero y no lee ningún campo de ninguna casilla?
3. ¿Qué sitios de llamada pasan el tablero? Nombra quién llama y quién recibe en cada uno.
4. ¿Qué dice `bool visitedCells[9][9]` que `BoardCell boardCells[9][9]` no dice, y al revés?
5. ¿Por qué la primera dimensión es 9 si el tablero tiene 8 filas? ¿Cuántos elementos tiene el
   arreglo y cuántos de ellos son casillas del tablero?
6. Si `BoardCell` ganara un tercer campo, ¿cuáles de las 19 definiciones no compilarían?
   Compruébalo antes de responder; el número es el punto.

La pregunta 6 tiene la respuesta que medirás en el Paso 4. La pregunta 4 tiene la respuesta que
argumentarás en el Paso 7.

## Paso 2 — Predecir antes de medir (~8 min)

Tres predicciones, todas escritas antes de cualquier ejecución: una sobre el campo, una sobre el
recorrido y una sobre las alternativas.

### 2.1 El cambio bajo prueba

El requisito, fijo para las dos líneas base:

> **R-LASTVISIT — registrar cuándo se visitó cada casilla.** Toda casilla que pasa a visitada
> guarda el número del movimiento exitoso que la visitó. Ninguna función lee todavía el campo
> nuevo, y la salida del juego no cambia. Un recorrido sobre el tablero devuelve el número de
> movimiento más alto almacenado.

La cláusula que dice que nada lee el campo es la que mantiene honesto el cambio. La comprobación
de trazas puede probar que el juego no cambió; no puede probar que el campo tenga el valor
correcto, porque el juego no lo imprime. El Paso 5 muestra qué sí puede.

Para el archivo con arreglo de estructuras, predice antes de editar:

- Sitios de declaración del campo:
- Sitios de asignación, y en qué ramas de `playTurn`:
- Firmas que cambian:
- Sitios de llamada que cambian:
- Arreglos nuevos:
- Globales nuevas:
- Líneas añadidas:
- Trazas que difieren, de 20:
- Líneas de la función de recorrido:
- ¿El número del recorrido coincide con `moves=` en las 20 semillas? (sí / no)
- Confianza, 0–100 %:

El argumento que estás probando: `playTurn` ya tiene `boardCells` y `successfulMoveCount` en el
momento en que marca una casilla. El hecho que necesitas registrar está a una línea de la marca.
Nada tiene que pasarse que no esté ya ahí.

### 2.2 El mismo hecho sin el registro

Las dos alternativas parten de la **línea base con registros** de `construction-04`, cuyo tablero
sigue siendo `bool visitedCells[9][9]`. Fija primero las unidades; son las mismas cinco que en
`construction-04`, más una.

- Un **sitio de edición** es un cambio contiguo del código que el requisito obliga: una
  declaración, una asignación, una línea de firma, una línea de llamada o un bloque de guarda con
  su transición.
- Un **cambio de firma** es una función cuyo lista de parámetros o tipo de retorno cambia.
  Cuéntalo una vez, aparte de los sitios de llamada que obliga.
- Un **sitio de llamada** es un lugar donde una expresión de llamada debe editarse porque cambió
  una firma.
- Un **llamador** es una función que contiene un sitio de llamada. Un llamador puede tener varios.
- Una **global nueva** es un nombre mutable añadido en el ámbito de espacio de nombres.
- Un **arreglo nuevo** es una segunda colección añadida para guardar un hecho por casilla.
- **No** cuentes comentarios, líneas en blanco, formato, trazas generadas ni el número de turnos.

| Métrica | Campo en `BoardCell` | Arreglo `int` paralelo | 64 nombres `int` |
|---|---|---|---|
| Sitios de edición | | | |
| Cambios de firma | | | |
| Sitios de llamada cambiados | | | |
| Arreglos nuevos | | | |
| Globales nuevas | | | |
| Líneas añadidas | | | |
| Trazas de referencia rotas (de 20) | | | |
| Cómo se identifica una casilla | | | |

Compromete un número en cada celda. «La versión con registro es más limpia» no es una predicción;
no dice qué fila se mueve.

### 2.3 Cuál de las tres comparaciones es justa

La versión con 64 nombres no es una contendiente seria. Es el peldaño anterior al arreglo,
incluida para que puedas decir por qué el arreglo existe. La comparación real es el campo contra
el arreglo paralelo. Responde ahora, antes de medir:

- En la versión con registro, ¿cuántos pares de subíndices identifican los dos hechos de una
  casilla?
- En la versión paralela, ¿cuántos pares de subíndices identifican los dos hechos de una casilla?
- ¿Qué hace que los dos pares de subíndices paralelos se refieran a la misma casilla: el lenguaje
  o una convención?
- Si un sitio de escritura olvida el segundo arreglo, ¿dónde aparece primero el error: en la
  transcripción o en el recorrido?
- Si un sitio de escritura intercambia los dos subíndices en el segundo arreglo, ¿qué dice el
  compilador?

Escribe las respuestas en `EVIDENCE-es.md` §3.4. El Paso 5 comprueba las dos últimas.

## Paso 3 — Ejecutar las dos líneas base (~5 min)

Crea primero el directorio de trabajo; todos los comandos de abajo escriben en él.

```bash
mkdir -p /tmp/array-test
./run.sh --seed 1 --name Learner > /tmp/array-test/board-cells-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/array-test/board-cells-0001.txt
```

Después la línea base con registros:

```bash
cd ../construction-04-records
./run.sh --seed 1 --name Learner > /tmp/array-test/records-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/array-test/records-0001.txt
```

Después las dos entre sí:

```bash
diff -u /tmp/array-test/records-0001.txt /tmp/array-test/board-cells-0001.txt
```

Tres diffs vacíos. Vuelve a este directorio de la lección y confirma las 20 semillas, no solo la
primera:

```bash
mkdir -p /tmp/array-test
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

Un bucle silencioso son 20 coincidencias. Vuelve a este directorio de la lección si lo cambiaste.
Si `run.sh` no puede compilar, compila una vez a mano:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o build/board-cells-baseline code/board-cells-baseline.cpp
```

Si un diff no está vacío, detente. Regístralo en `EVIDENCE-es.md` §11. No ajustes una traza para
que coincida con el código.

## Paso 4 — Añadir el campo y registrar el movimiento (~10 min)

Copia la línea base. El archivo versionado no se toca.

```bash
mkdir -p /tmp/array-test
cp code/board-cells-baseline.cpp /tmp/array-test/board-cells-move-number.cpp
```

Tres ediciones. La primera es el tipo:

```cpp
struct BoardCell
{
    bool visited;
    int moveNumber;
};
```

La segunda es la rama de captura de `playTurn`, después de marcar la casilla como visitada y
después de incrementar el contador de movimientos exitosos:

```cpp
            boardCells[destinationRow][destinationColumn].visited = true;
            boardCells[destinationRow][destinationColumn].moveNumber = successfulMoveCount;
```

La tercera es la rama legal, en el mismo orden:

```cpp
        boardCells[destinationRow][destinationColumn].visited = true;
        boardCells[destinationRow][destinationColumn].moveNumber = successfulMoveCount;
```

Compila y ejecuta las 20 semillas:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/array-test/board-cells-move-number /tmp/array-test/board-cells-move-number.cpp
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/array-test/board-cells-move-number --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

20 coincidencias, sin salida. El diff contra el archivo versionado es de tres líneas añadidas y
cero eliminadas:

```bash
diff -u code/board-cells-baseline.cpp /tmp/array-test/board-cells-move-number.cpp
```

Cuatro observaciones, cada una parte de la respuesta.

**Una — no se movió ninguna firma.** Compara los tres parámetros del tablero; su aridad no cambió.
El total de parámetros sigue en 63 sobre 19 definiciones. El compilador no tuvo nada nuevo que
comprobar porque nada se repitió. Confírmalo con la lista de ediciones, no con la ausencia de
errores:

```bash
grep -n 'moveNumber' /tmp/array-test/board-cells-move-number.cpp
```

El campo se declara una vez y se asigna dos veces. Las demás apariciones son el nombre de
parámetro `moveNumber`, que ya significa otra cosa en este archivo. Ver la nota de nombres más
abajo.

**Dos — la traza no puede ver el campo.** Cambia las dos asignaciones por una constante, recompila
y ejecuta las 20 semillas. Todas pasan, porque nada lee el campo. La traza comprueba el juego; no
comprueba el hecho. Anótalo: una traza de referencia es un contrato de comportamiento, no un
contrato de datos.

**Tres — el campo tiene dos escritores, no uno.** Una casilla pasa a visitada en la rama de
captura y en la rama legal, y las dos ramas incrementan el contador de movimientos exitosos. Si
editaste solo una rama, las trazas siguen pasando y el campo está mal en algunas casillas. El Paso
5 convierte «algunas casillas» en un número.

**Cuatro — ningún nombre nuevo en el ámbito de espacio de nombres.** El tipo está en ese ámbito;
no es dato.

```bash
g++ -std=c++20 -c -o /tmp/array-test/board-cells.o \
  /tmp/array-test/board-cells-move-number.cpp
nm -C --defined-only -g /tmp/array-test/board-cells.o | grep ' [BbDd] '
```

Sigue habiendo una línea, `randomState`. `sizeof(BoardCell)` pasa de 1 a 8: cuatro bytes para el
`int` en esta plataforma, uno para el `bool` y tres bytes de relleno para que el siguiente elemento
empiece alineado. El tablero de 9×9 pasa de 81 a 648 bytes. Nada en la transcripción depende de eso,
y nada en esta lección depende del relleno. `construction-19` es donde el diseño en memoria pasa a
ser el tema.

**Nota de nombres.** El campo se llama `moveNumber` porque el requisito lo dice. En este archivo
ese nombre ya carga dos significados: el parámetro de `tryMove` es el orden de movimiento
intentado, de 1 a 8, y el parámetro de `printMove` es el contador de movimientos exitosos,
de 1 a 200. El campo se une al segundo significado. El acceso al miembro hace legal el código y
más difícil el trabajo de quien lee. Si lo nombraras tú, `visitedOnMove` diría cuál de los dos es.

Escribe `EVIDENCE-es.md` §5: los comandos exactos, el diff, cuáles de tus predicciones de §2.1 se
cumplieron y cuál falló. El fallo habitual es el número de escritores: se predice una asignación,
porque un movimiento parece un solo evento, y se olvida que una captura también marca una casilla.

## Paso 5 — Recorrer el tablero (~10 min)

Ahora la pregunta por la que se añadió el campo. Una función, dos bucles:

```cpp
int lastRecordedMoveNumber(const BoardCell boardCells[][boardColumnCount + 1])
{
    int highestMoveNumber = 0;
    for (int rowIndex = 1; rowIndex <= boardRowCount; rowIndex += 1)
    {
        for (int columnIndex = 1; columnIndex <= boardColumnCount; columnIndex += 1)
        {
            if (boardCells[rowIndex][columnIndex].moveNumber > highestMoveNumber)
            {
                highestMoveNumber = boardCells[rowIndex][columnIndex].moveNumber;
            }
        }
    }
    return highestMoveNumber;
}
```

Cuatro decisiones en quince líneas.

- **El parámetro es `const` y nombra la segunda dimensión.** Un parámetro de arreglo bidimensional
  debe indicar el ancho de fila para que el compilador calcule el salto entre filas. `const` dice
  que el recorrido lee y nunca escribe. La primera dimensión puede quedar sin tamaño.
- **Los bucles van de 1 a 8.** El arreglo tiene nueve filas y nueve columnas; solo de 1 a 8 son
  casillas del tablero. La fila 0 y la columna 0 existen para que una coordenada base 1 se use
  como índice directamente. Empezar en 0 leería el borde sin usar, y empezar en 0 y terminar en 7
  se perdería la última fila y la última columna. El límite es el tamaño del tablero, no el del
  arreglo.
- **El valor por defecto es 0.** Un tablero sin tocar devuelve 0. Eso no es un número de
  movimiento; es el único valor disponible cuando no se visitó ninguna casilla. El tipo de retorno
  no tiene forma de decir «ningún movimiento». Registra esa brecha; `construction-22` trata
  contratos como este.
- **El recorrido no se detiene antes.** No puedes conocer el máximo hasta haber visto todas las
  casillas, y el tablero no guarda el orden de los movimientos en ningún otro sitio. 64
  comparaciones es el costo honesto.

Para comprobar el recorrido sin cambiar la transcripción, añade esa función encima de `playGame`
en el mismo archivo de trabajo e imprime por la salida de error estándar:

```cpp
    std::fprintf(stderr, "last_recorded=%d moves=%d\n",
                 lastRecordedMoveNumber(boardCells), successfulMoveCount);
```

Llámala al final de `playGame`, después de las dos llamadas `printf` existentes. Compila y ejecuta
la semilla 1:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/array-test/board-cells-traversal /tmp/array-test/board-cells-move-number.cpp
/tmp/array-test/board-cells-traversal --seed 1 --name Learner > /dev/null
```

```text
last_recorded=21 moves=21
```

Después las 20 semillas, la salida estándar contra las trazas de referencia y la salida de error
contra la línea `moves=` de la transcripción:

```bash
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/array-test/board-cells-traversal --seed "${seedNumber}" --name Learner \
    2>&1 >/dev/null
  /tmp/array-test/board-cells-traversal --seed "${seedNumber}" --name Learner \
    2>/dev/null | cmp - "${trace}"
done
```

El recorrido devuelve el mismo número que la transcripción ya imprime como `moves=`, en las 20
semillas. No es casualidad. Cada movimiento aceptado incrementa `successfulMoveCount` exactamente
una vez y escribe ese valor en exactamente una casilla, así que el valor máximo almacenado es el
número del último movimiento exitoso. El máximo también es único: dos casillas no pueden guardar
el mismo número de movimiento.

Lo que plantea la pregunta honesta: si el número ya estaba en la transcripción, ¿qué añadió el
recorrido? La **casilla**. El número es redundante; las coordenadas de la casilla con el número
más alto no se imprimen en ningún sitio. Añade dos parámetros de salida y el mismo recorrido
devuelve ambos:

```cpp
int lastRecordedMoveNumber(const BoardCell boardCells[][boardColumnCount + 1],
                           int &lastRow, int &lastColumn)
{
    int highestMoveNumber = 0;
    lastRow = 0;
    lastColumn = 0;
    for (int rowIndex = 1; rowIndex <= boardRowCount; rowIndex += 1)
    {
        for (int columnIndex = 1; columnIndex <= boardColumnCount; columnIndex += 1)
        {
            if (boardCells[rowIndex][columnIndex].moveNumber > highestMoveNumber)
            {
                highestMoveNumber = boardCells[rowIndex][columnIndex].moveNumber;
                lastRow = rowIndex;
                lastColumn = columnIndex;
            }
        }
    }
    return highestMoveNumber;
}
```

Para la semilla 1 devuelve `21` en `4,1`, y `4,1` es el destino de la última línea `move=` en
`seed-0001.txt`. Eso se cumple en las 20 semillas. La tabla completa va en `EVIDENCE-es.md` §6.1.

### 5.1 Qué puede probar el recorrido

Dos experimentos, ambos sobre copias de trabajo.

**Olvidar un escritor.** Quita la asignación de la rama de captura, recompila y ejecuta las 20
semillas. Las 20 trazas siguen pasando. El recorrido ahora no coincide con `moves=` en 11 semillas
y coincide en 9. El reparto es exacto: 11 partidas terminan en captura y 9 terminan en trampa.
Cuando el último movimiento es una captura, la última casilla nunca recibió su número y el máximo
retrocede al movimiento anterior.

**Escribir en la casilla equivocada.** Intercambia los dos subíndices en las dos asignaciones, de
modo que el número se guarde en `[column][row]` en lugar de `[row][column]`. Recompila y ejecuta
las 20 semillas. Las 20 trazas siguen pasando. El recorrido que devuelve solo el máximo también
informa el número correcto en las 20, porque permutar las casillas no cambia el conjunto de
valores. El recorrido que devuelve las coordenadas informa la casilla equivocada en 14 de 20; las
6 que sobreviven tienen su última casilla en la diagonal, donde la transpuesta es la misma
casilla.

El segundo resultado es el que hay que retener. **El máximo es invariante ante cualquier
permutación de las casillas.** Un recorrido que devuelve solo un valor no puede decirte dónde se
guardó el valor. Para probar el almacenamiento, devuelve las coordenadas y compáralas con la
última línea `move=`.

Así, las tres comprobaciones prueban cosas distintas:

| Comprobación | Qué prueba | Qué no puede ver |
|---|---|---|
| Traza de referencia | La salida del juego no cambió | Un campo mal escrito o ausente |
| Recorrido del valor | El valor máximo se escribió en algún sitio | Un valor escrito en la casilla equivocada |
| Recorrido de coordenadas | El máximo está en la casilla correcta | Un campo escrito dos veces o un valor de origen equivocado |

Nada en esta lección prueba que cada casilla se escribió exactamente una vez. Eso necesita un
invariante, y los invariantes llegan en `construction-10` y `construction-11`. Lo que hoy puedes
decir es más pequeño y verdadero: el hecho vive en la casilla, el recorrido lee la casilla y las
coordenadas se pueden comprobar.

Escribe `EVIDENCE-es.md` §6: la función, los 20 resultados, los dos experimentos y las respuestas
a las preguntas de abajo.

1. ¿Por qué los bucles empiezan en 1 y no en 0?
2. ¿Por qué el máximo coincide con `moves=` en todas las semillas?
3. ¿Por qué el máximo es único?
4. ¿Qué devuelve un tablero sin tocar y qué significa ese valor?
5. ¿Por qué el recorrido no puede detenerse en la primera casilla que encuentra?
6. ¿Qué añade el recorrido que la transcripción no imprimía ya?
7. ¿Qué sitio de escritura es fácil de olvidar y por qué no lo detecta la traza?
8. ¿Por qué una escritura transpuesta conserva el valor correcto y las coordenadas equivocadas?
9. ¿Cuál de las tres comprobaciones detectaría una escritura que guardó el número de movimiento
   intentado (de 1 a 8) en lugar del número de movimiento exitoso?

## Paso 6 — El mismo hecho sin el registro (~10 min)

Ahora el otro lado. Copia la línea base con registros a un segundo archivo de trabajo:

```bash
cp ../construction-04-records/code/records-baseline.cpp \
  /tmp/array-test/records-parallel.cpp
```

El hecho sigue teniendo que ser alcanzable donde una casilla pasa a visitada: dentro de
`playTurn`, en sus dos sitios de escritura. Añade un arreglo paralelo y llévalo hasta esos sitios.

1. En `main`, junto al tablero booleano:
   `int visitedMoveNumbers[boardRowCount + 1][boardColumnCount + 1] = {};`
2. `playTurn` gana `int visitedMoveNumbers[][boardColumnCount + 1]`.
3. `playGame` gana el mismo parámetro y lo reenvía.
4. Cada sitio de escritura gana una línea:
   `visitedMoveNumbers[destinationRow][destinationColumn] = successfulMoveCount;`
5. La llamada en `main` pasa el arreglo nuevo.

`tryMove` no cambia. Solo lee `visitedCells`, así que el hecho nuevo nunca tiene que llegar hasta
él. Ese es el alcance más pequeño que puede tener la versión paralela, y aun así son dos firmas y
dos sitios de llamada.

Compila y ejecuta las 20 semillas como en el Paso 4. Pasan. El juego no cambió; solo cambió la
forma del hecho. Después cuenta a qué llegó el cambio:

| Métrica | Campo en `BoardCell` | Arreglo `int` paralelo |
|---|---|---|
| Sitios de edición | 3 | 7 |
| Cambios de firma | 0 | 2 (`playTurn`, `playGame`) |
| Sitios de llamada cambiados | 0 | 2 |
| Arreglos nuevos | 0 | 1 |
| Globales nuevas | 0 | 0 |
| Líneas añadidas / eliminadas | 3 / 0 | 10 / 5 |
| Trazas de referencia rotas (de 20) | 0 | 0 |
| Qué elemento seleccionan los subíndices | un elemento, los dos hechos | dos arreglos, una convención |

Los tres sitios de edición del registro son el campo y las dos asignaciones. Los siete de la
versión paralela son la declaración, las dos líneas de firma, las dos líneas de llamada y las dos
asignaciones. El recorrido en la versión paralela necesita el segundo arreglo como parámetro, lo
que es una firma y un sitio de llamada más si lo llamas desde cualquier sitio que no sea `main`.

Ahora la colocación que parece más barata. Convierte `visitedMoveNumbers` en una global de ámbito
de espacio de nombres. Quita los dos parámetros y los dos argumentos de llamada. Recompila y
ejecuta las 20 semillas. Pasan. El conteo baja a tres sitios de edición, cero cambios de firma,
cero sitios de llamada y una global mutable nueva. El costo no desapareció; se mudó fuera de las
firmas a un nombre que toda función puede alcanzar sin decirlo. Es el mismo intercambio que
`construction-04` puso en precio para las banderas de captura.

### 6.1 La versión con 64 nombres

Si rechazas las dos colecciones, el hecho necesita 64 variables `int` junto a los 64 valores de
verdad. Un nombre no se puede calcular a partir de `destinationRow` y `destinationColumn`, así que
cada sitio de escritura se convierte en una decisión sobre 64 nombres: un `switch` con 64 casos, o
una cadena de `if` de la misma longitud. El recorrido se convierte en 63 comparaciones escritas
por nombre. Pasar el conjunto a una función necesita 64 parámetros, o la global que intentabas
evitar.

Los conteos no son el punto; la forma sí. En el momento en que la escritura se puede seleccionar
por una coordenada calculada, has inventado el arreglo. En el momento en que el segundo hecho
tiene su propio arreglo, has inventado el arreglo paralelo. El registro es el peldaño siguiente,
no otra escalera.

Responde esto en `EVIDENCE-es.md` §7.4:

- ¿Por qué la versión paralela necesita solo dos cambios de firma y no tres?
- ¿Qué comprueba el compilador sobre los dos pares de subíndices paralelos? ¿Qué no comprueba?
- ¿Por qué una versión con 64 nombres no se puede seleccionar con `destinationRow` y
  `destinationColumn`?
- ¿Qué versión puede cruzar una frontera de llamada como un solo argumento?
- ¿En qué fila gana la versión paralela, si gana alguna, y por qué eso no es toda la comparación?
- ¿Qué costaría un tercer hecho por casilla en cada versión?

La última pregunta es la que decide el diseño. Para el registro es una línea en el tipo y una
línea en cada sitio de escritura. Para el arreglo paralelo es otra declaración, dos firmas más,
dos sitios de llamada más y otro arreglo que debe coincidir con los dos primeros. Para 64 nombres
son otras 64 declaraciones y otro despacho de 64 vías.

### 6.2 La comparación honesta

No afirmes que el arreglo de estructuras es más barato en total. Convertir el tablero de
`construction-04` a `BoardCell` costó 15 líneas añadidas y 10 eliminadas y no compró nada
observable. Si solo necesitaras el número de la última visita una vez, el arreglo paralelo es más
barato desde la línea base de `construction-04`: 10 líneas añadidas y 5 eliminadas frente a 25
líneas cambiadas más el campo. El registro gana **por cada hecho añadido después de que el tablero
ya es una colección de casillas**, y gana la unión: un elemento, un arreglo, un lugar donde el
hecho se puede leer.

Esa es la misma estructura que `construction-04`. La ventaja del registro es el costo marginal y
la unión, no la primera conversión. Pon precio al cambio que haces, después al siguiente.

## Paso 7 — Comparar y explicar (~7 min)

Escribe `EVIDENCE-es.md` §8. Dos partes.

**Primero, una frase.** Di qué decide el arreglo de estructuras sobre un hecho por casilla, sin
usar la palabra *struct* y sin una palabra de veredicto: *mejor*, *peor*, *limpio*, *más limpio*,
*elegante*, *feo*, *modular*. Si tu frase necesita una de esas, has descrito tu reacción al
archivo y no el conteo.

**Segundo, un párrafo.** Responde:

- Qué decide el tipo de elemento: qué es una casilla, qué campos posee y cuál es su valor por
  defecto.
- Qué decide el recorrido: qué casillas se visitan, en qué orden y dónde se detiene.
- Qué localiza el arreglo de estructuras: el conjunto de campos, el valor por defecto cero, el
  único arreglo y la unión de un hecho con la casilla que describe.
- Qué **no** localiza: los escritores, los límites, la representación y el invariante de que cada
  casilla se escribe una vez.
- Qué debe sostener en la memoria de trabajo quien lee para saber qué campos usa una función, en
  cada archivo.
- Qué puede probar cada una de las tres comprobaciones: traza, recorrido del valor y recorrido de
  coordenadas.
- Qué lección posterior corrige cada brecha restante. Hay cuatro: nombrar la casilla una vez
  (`construction-06`), representación y límites (`construction-09`), invariantes y acceso
  (`construction-10`, `construction-11`) y el bucle mismo (`construction-17`).

Una respuesta fuerte mantiene separadas dos cosas: **el costo de añadir un hecho** y **a qué está
unido el hecho**. El registro gana lo primero en el margen y lo segundo de forma directa. Pierde
el costo de conversión y no elimina ni un subíndice. Una respuesta débil dice que la versión con
registro es más limpia, lo que esconde los dos números.

## Paso 8 — Comprobación de aceptación (~2 min)

El criterio de aceptación de esta lección es: **recorrer el tablero con dos bucles, encontrar el
último número de movimiento, explicar por qué no hace falta un arreglo paralelo ni 64 variables, y
mantener las 20 trazas pasando.**

Comprueba tu registro contra él:

- [ ] Ejecuté las dos líneas base sin modificar y obtuve tres diffs vacíos, más 20 semillas
  coincidentes.
- [ ] Inventarié las tres funciones que reciben el tablero y las dos que tocan un campo.
- [ ] Predije el costo del campo y el número de escritores antes de editar.
- [ ] Añadí `moveNumber` a `BoardCell` y lo registré en las dos ramas de `playTurn`.
- [ ] Las 20 trazas siguieron pasando después de añadir el campo.
- [ ] Escribí un recorrido de dos bucles y devolvió el mismo número que `moves=` en las 20
  semillas.
- [ ] Mostré que una escritura de captura olvidada pasa la traza y falla el recorrido en 11
  semillas.
- [ ] Mostré que una escritura transpuesta conserva el valor y mueve las coordenadas en 14
  semillas.
- [ ] Compilé la variante con arreglo paralelo y conté sus firmas y sitios de llamada.
- [ ] Expliqué por qué 64 nombres no se pueden seleccionar con una coordenada calculada.
- [ ] Expliqué qué localiza el registro y qué no.
- [ ] Mi frase de explicación evita *struct* y las ocho palabras de veredicto.
- [ ] No edité ninguna de las dos líneas base versionadas.
- [ ] Completé `EVIDENCE-es.md` §1–§10 y la autocomprobación de §12.

El criterio no es «gana el arreglo de estructuras». Es «puedes decir exactamente cuánto cuesta un
hecho por casilla, a qué está unido y qué comprobación notaría si estuviera mal».

## Autocomprobación (binaria)

- [ ] Leí las dos líneas base e inventarié qué función toca el tablero y qué campo.
- [ ] Predije el costo del campo, el resultado del recorrido y las dos alternativas antes de
  medir.
- [ ] Compilé las dos líneas base y reproduje las 20 trazas.
- [ ] Añadí `moveNumber` a una copia de trabajo y mantuve las trazas pasando.
- [ ] Escribí el recorrido y comprobé su valor y su casilla contra la transcripción.
- [ ] Apliqué el mismo hecho a la línea base con registros usando un arreglo paralelo y lo conté.
- [ ] Comparé la predicción con los conteos y nombré los fallos.
- [ ] Expliqué qué localiza el arreglo de estructuras y qué no.
- [ ] Completé `EVIDENCE-es.md`.

Estos son criterios de aceptación. Si una
casilla queda sin marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §10.

## Misconcepciones que esta lección refuta

| Concepción errónea | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| «Un arreglo de estructuras es un registro con un índice.» | El índice selecciona una casilla; el registro dice qué es una casilla. Son dos decisiones, y aquí solo cambió la segunda. | Compara un arreglo con dos. |
| «El registro elimina la aritmética de índices.» | Todo acceso sigue escribiendo `boardCells[row][column]`. El registro elimina el *segundo* arreglo, no el índice. | Cuenta a cuántos arreglos llega un sitio de escritura: uno aquí, dos allí. |
| «Si las trazas pasan, el campo nuevo es correcto.» | Un campo que nadie lee no puede cambiar la salida. Un escritor olvidado también pasa las 20 trazas. | Compara el recorrido con `moves=`; 11 de 20 semillas lo detectan. |
| «El máximo prueba que las casillas son correctas.» | El máximo es invariante ante cualquier permutación de las casillas. Una escritura transpuesta lo conserva. | Devuelve las coordenadas; 14 de 20 semillas detectan la transpuesta. |
| «Un arreglo paralelo está mal.» | Pasa las 20 trazas y es una segunda colección legítima. Es una convención, no un error. | Pon precio al siguiente hecho por casilla. |
| «64 variables son un arreglo con nombres.» | Un nombre no se puede calcular a partir de una fila y una columna. La escritura se vuelve una decisión de 64 vías. | Intenta seleccionar una por coordenada sin un arreglo. |
| «El tablero conoce su propio tamaño.» | Un arreglo crudo no lo conoce. El tamaño vive en dos constantes y en cada llamador que lo pasa. | Cuenta elementos contra casillas; después lee `construction-09`. |
| «El registro hace seguro el tablero.» | Cualquier tenedor puede escribir cualquier campo de cualquier casilla. `boardCells[0][0].visited = true;` compila. | Eso es trabajo de `construction-11`. |
| «El recorrido es genérico.» | Los límites están fijados a 1..8 y los nombres de campo también. | `construction-17` sustituye los bucles. |
| «El nombre del campo es gratis.» | `moveNumber` ya significa el orden intentado en `tryMove` y el conteo exitoso en `printMove`. | Nombra el significado, no el número. |

## Siguiente paso

`construction-06` — referencias. El sitio de escritura todavía escribe
`boardCells[destinationRow][destinationColumn]` dos veces, una por campo. Una referencia nombra
esa casilla una vez y las dos escrituras pasan por un solo nombre. Ese es el peldaño siguiente: la
casilla ya es la unidad de almacenamiento y de recorrido; la referencia la convierte en unidad de
*acceso*.

Después, `construction-07` lleva el mismo recorrido a un diseño distinto, y `construction-09` le
da al tablero sus propios límites para que el `1..8` deje de ser un hecho que quien llama debe
recordar.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog-es.md) para la ruta
completa.

Si faltaba cualquiera de las dos líneas base, no empieces `construction-06`. El siguiente
contraste necesita los dos lados.

[records-baseline]: ../construction-04-records/code/records-baseline.cpp
