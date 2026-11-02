# Registros (records)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** agregación · **Tiempo:** unos 55 minutos · **Prerrequisitos:** construction-03 ·
**Estado:** borrador
**Lectura:** ~17 minutos · **Prerrequisitos:** construction-03
**Rol de la lección:** tronco

## Por qué existe esta lección

`construction-03` terminó con una factura sin pagar. `tryMove` produce tres resultados — el
resultado, la fila de destino, la columna de destino — y una función devuelve uno. Dos de ellos
viajan como referencias. El siguiente paso obvio es agrupar esos tres en un solo valor.

Esa no es la lección. La lección está un paso antes. Los dos caballos nunca fueron tres resultados.
Eran dos cosas — un caballo del jugador y un caballo de la máquina — cada una con una fila y una
columna, y el código guardaba cuatro números en cuatro nombres separados. Agrupar lo que ya
pertenece junto es **agregación**.

La frase que hay que retener es más corta y menos cómoda que la de `construction-03`:

> Un registro hace gratis añadir un campo a costa de hacer menos precisa la lista de parámetros.

Las dos mitades se pueden medir. La primera es un conteo: cero firmas cambian. La segunda es una
lectura: un parámetro que dice `const Knight &` ya no dice qué campos toca el cuerpo. Vas a medir la
primera y a argumentar la segunda, contra las mismas 20 trazas.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar la agregación y qué mantiene junto un registro;
- añadir un campo captured y listar las firmas que cambiaron.

## El concepto: un registro es un nombre para campos que viajan juntos

Cinco formas de nombre hacen el trabajo en este archivo:

```text
separadas   int playerRow; int playerColumn;    dos nombres, dos ciclos de vida que sincronizar
registro    struct Knight { int row; int column; };
miembro     playerKnight.row                    un objeto, un ciclo de vida, campos dentro
referencia  Knight &activeKnight                un nombre para el objeto de quien llama
ref. const  const Knight &otherKnight           acceso de solo lectura al objeto de quien llama
```

En `construction-03`, una función que necesita un caballo necesita sus dos números por separado.
`playTurn` recibe `playerRow, playerColumn, machineRow, machineColumn` como cuatro parámetros por
referencia. En `construction-04` recibe `Knight &playerKnight, Knight &machineKnight`.

Sobre las 19 definiciones de función, los totales de parámetros son 75 y 63. Las siete funciones
que difieren:

| Función | Parámetros en `construction-03` | Parámetros en `construction-04` |
|---|---|---|
| `initializePositions` | 4 | 2 |
| `printGameHeader` | 9 | 7 |
| `printTurnHeader` | 5 | 4 |
| `printMove` | 7 | 6 |
| `tryMove` | 8 | 6 |
| `playTurn` | 8 | 6 |
| `playGame` | 8 | 6 |
| **Las 19 definiciones** | **75** | **63** |

Desaparecen doce parámetros. Nada del juego cambió para hacerlo posible: los mismos dos objetos
estuvieron siempre ahí, y los cuatro nombres eran una elección.

Ahora lee juntas las dos líneas de `tryMove`.

```cpp
MoveOutcome tryMove(int activeKnightRow, int activeKnightColumn, int otherKnightRow,
                    int otherKnightColumn, int moveNumber, bool visitedCells[...],
                    int &destinationRow, int &destinationColumn);
```

```cpp
MoveOutcome tryMove(const Knight &activeKnight, const Knight &otherKnight, int moveNumber,
                    bool visitedCells[...], int &destinationRow, int &destinationColumn);
```

La primera nombra cuatro enteros y nada más. Quien lee sabe qué números puede leer el cuerpo sin
abrirlo. La segunda nombra dos objetos, y el cuerpo puede leer cualquier campo que uno de ellos
tenga ahora o gane después. La segunda es más corta y dice menos. Ese intercambio es la lección.

Se siguen tres consecuencias:

1. **Un campo nuevo no llega a la firma.** Toda función que ya recibe un `Knight` puede usar el
   campo sin cambiar de firma. Añadir `captured` cuesta una línea en el tipo y una asignación donde
   ocurre el hecho.
2. **Un registro tiene un solo valor por defecto, y cubre los campos añadidos después.**
   `Knight playerKnight = {};` pone a cero todos los miembros, incluido uno que no existe cuando se
   escribe esa línea. Cuatro enteros paralelos no tienen un defecto compartido: un quinto es una
   quinta declaración que recordar.
3. **El uso de un campo nuevo sigue llegando a toda función que lo lea, y ninguna firma los
   enumera.** Esa es la segunda mitad de la frase. `grep` pasa a ser la única forma de responder
   «¿quién lee `captured`?», y la respuesta es una búsqueda de texto, no una lista de parámetros.

Lo que el registro **no** es, en esta lección:

- **No es encapsulación.** `.row`, `.column` y `.captured` son alcanzables desde toda función que
  tenga un `Knight`. `playerKnight.row = 0;` compila. Nada aquí lo impide.
- **No es independencia de la representación.** Los nombres de campo aparecen en 14 lugares de seis
  funciones. Sustituir `row` y `column` por un índice de casilla reescribiría los 14.
  `construction-09` pone precio a ese cambio.
- **No es un valor con comportamiento.** `Knight` no tiene funciones propias, ni igualdad, ni
  impresión, ni invariante. Es una forma para datos, no un guardián de ellos. `construction-10` y
  `construction-11` añaden el guardián.

## Antes de empezar

- [`construction-03`](../construction-03-functions/lesson-es.md) — tienes sus dos conteos: sitios de
  edición y llamadores cambiados por R-SKIP, y sabes qué función no lee ninguna global.
- Un compilador de C++20 (`g++` o `clang++`). `cmake` es opcional.

No necesitas saber C++ a fondo. Necesitas leer la definición de un tipo, un acceso a miembro
(`knight.row`) y un parámetro por referencia. Esa es toda la destreza que ejercita esta lección.

### Qué necesitas

- La línea base con registros: [`code/records-baseline.cpp`](code/records-baseline.cpp).
- La línea base con funciones: [`functions-baseline.cpp`][functions-baseline].
- Los scripts de compilación y las notas: [`run.sh`](run.sh) y [`README-es.md`](README-es.md).
- Las reglas: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §12 fija el orden
  de los sorteos.
- El formato de la transcripción:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- Una traza de referencia: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- Esta plantilla para quien aprende: [`EVIDENCE-es.md`](EVIDENCE-es.md).

Comprueba que los artefactos existen antes de empezar, desde este directorio de la lección:

```bash
test -f code/records-baseline.cpp &&
test -f ../construction-03-functions/code/functions-baseline.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

Si falta cualquiera de las dos líneas base, la comparación no tiene segundo lado. Registra el
archivo ausente en `EVIDENCE-es.md` §10 y vuelve cuando esté disponible. Un conteo contra un archivo
que no has leído no es evidencia.

Ambas líneas base reproducen las 20 trazas de `common/traces/` byte a byte antes de que cambies
nada. El mismo juego, dos representaciones de un mismo hecho. Eso es lo que hace justa la
comparación.

## Paso 1 — Leer el registro y sus llamadores (~10 min)

Lee los dos archivos una vez. Después deja que el texto te diga dónde aparece el tipo:

```bash
grep -n 'Knight' code/records-baseline.cpp
grep -n '^\(void\|int\|bool\|MoveOutcome\|std::uint64_t\) [a-zA-Z]*(' \
  code/records-baseline.cpp
```

El primer comando imprime la definición, todas las listas de parámetros que nombran el tipo y todos
los accesos a campo dentro de un cuerpo. El segundo imprime 19 definiciones. Complétalo en
`EVIDENCE-es.md` §2.1 mientras lees.

| Función | Línea | Recibe | Campos que lee | Campos que escribe |
|---|---|---|---|---|
| `initializePositions` | 120 | `Knight &` ×2 | — | `row`, `column` |
| `printGameHeader` | 144 | `const Knight &` ×2 | `row`, `column` | — |
| `printTurnHeader` | 166 | `const Knight &` | `row`, `column` | — |
| `printMove` | 201 | `const Knight &` | `row`, `column` | — |
| `tryMove` | 213 | `const Knight &` ×2 | `row`, `column` | — |
| `playTurn` | 236 | `Knight &` ×2 | mediante las llamadas que hace | `row`, `column` |
| `playGame` | 305 | `Knight &` ×2 | — | — |
| `main` | 323 | — | — | declara los dos caballos |

Dos filas merecen un minuto.

- `playGame` recibe dos caballos y no lee ningún campo de ninguno. Es un pasamanos. En
  `construction-03` el mismo pasamanos mide cuatro enteros de ancho; aquí mide dos objetos de ancho.
  El trabajo de la función no cambió, y tampoco cambió su ignorancia de los campos.
- `playTurn` escribe `row` y `column` del caballo **activo**, y lo hace mediante `activeKnight`, una
  referencia elegida a partir de `playerIsActive` en la línea 246. En `construction-03` esa misma
  escritura necesita un `if` con dos ramas, porque no hay referencia que elegir. Guarda esa
  observación; el paso 5 la cobra.

Después responde en `EVIDENCE-es.md` §2:

1. ¿Cuántas funciones reciben un `Knight`? ¿Cuántas lo reciben como `const Knight &`?
2. ¿Qué función recibe caballos y no lee ningún campo de ninguno?
3. ¿Cuántos parámetros tendría hoy `tryMove` si cada `Knight` se expandiera en sus campos?
4. ¿Qué impide `const Knight &`? ¿Qué no impide?
5. ¿Dónde se fija por primera vez `row` de un caballo? ¿Dónde se fija por última vez?
6. Si `Knight` ganara un tercer campo, ¿cuál de las 19 definiciones dejaría de compilar?
   (Compruébalo antes de responder; el número es el punto.)
7. ¿Qué dos nombres del ámbito de espacio de nombres son datos y cuál es un tipo? Confírmalo con
   `nm` si quieres: `g++ -std=c++20 -c -o /tmp/rec.o code/records-baseline.cpp` y después
   `nm -C --defined-only -g /tmp/rec.o | grep ' [BbDd] '`.

La pregunta 6 tiene la respuesta que vas a medir en el paso 4, y la pregunta 4 la que vas a
argumentar en el paso 6.

## Paso 2 — Predecir antes de medir (~8 min)

Tres predicciones, todas escritas antes de cualquier ejecución: una sobre el registro, una sobre el
reemplazo y una sobre la ubicación del reemplazo.

### 2.1 El cambio bajo prueba

El requisito, fijado para los dos archivos:

> **R-CAPTURED — registrar la captura.** Un caballo capturado queda marcado. La marca se activa
> cuando ocurre la captura, empieza en falso para los dos caballos y ninguna función la lee. La
> salida del juego no cambia.

La cláusula que dice que nadie lee la marca es la que mantiene honesto el cambio. La comprobación de
trazas puede demostrar que el juego no cambió; no puede demostrar que el campo guarda el valor
correcto, porque nadie lo lee. El paso 4 muestra por qué importa esa distinción.

Para el archivo con registros, predice antes de editar:

- Firmas que cambian:
- Llamadas que cambian:
- Funciones cuyo cuerpo cambia:
- Líneas añadidas:
- Trazas que difieren, de 20:
- Confianza, 0–100 %:

El argumento que estás probando: `playTurn` ya tiene `Knight &otherKnight` en la línea 247. El hecho
que hay que registrar es sobre `otherKnight`. No hay que pasar nada que no esté ya ahí.

### 2.2 El mismo hecho sin registro

`construction-03` no tiene `Knight`. Para registrar el mismo hecho necesitas dos nombres booleanos
nuevos, uno por caballo, y deben ser alcanzables donde ocurre la captura: dentro de `playTurn`.
Predice el coste de **las dos** ubicaciones. Fija antes las unidades; son las mismas cinco de
`construction-03`.

- Un **sitio de edición** es un cambio contiguo en el código que el requisito obliga: una
  declaración, una asignación, un argumento de formato, una línea de firma o un bloque de guarda con
  su transición.
- Un **cambio de firma** es una función cuya lista de parámetros o tipo de retorno cambia. Se cuenta
  una vez, aparte de las llamadas que obliga.
- Una **llamada** es un lugar donde hay que editar una expresión de llamada porque cambió una firma.
- Un **llamador** es una función que contiene una llamada. Un llamador puede tener varias llamadas.
- Una **global nueva** es un nombre mutable añadido al ámbito de espacio de nombres.
- **No** cuentes comentarios, líneas en blanco, formato, trazas generadas ni el número de turnos.

| Métrica | Registro (`captured`) | Banderas locales en `playGame` | Banderas locales en `main` |
|---|---|---|---|
| Sitios de edición | | | |
| Cambios de firma | | | |
| Llamadas cambiadas | | | |
| Llamadores de `playTurn` cambiados | | | |
| Llamadores de `playGame` cambiados | | | |
| Globales nuevas | | | |
| Líneas añadidas | | | |
| Trazas de referencia rotas (de 20) | | | |

Comprométete con un número en cada celda. «La versión con registro está más limpia» no es una
predicción; no dice qué fila se mueve.

### 2.3 Qué ubicación es la comparación justa

Las dos ubicaciones de las banderas no son igual de justas, y la diferencia es la lección. Responde
antes de medir:

- ¿Qué ámbito guarda el hecho en la versión con registro? Nombra la variable.
- En la versión con banderas, ¿qué ámbito posee el hecho: el cuerpo de `playGame` o el de `main`?
- Si el final de `playGame` imprimiera qué caballo fue capturado, ¿qué ubicación seguiría
  compilando?

Escribe la respuesta en `EVIDENCE-es.md` §3.3. La comprobarás contra los conteos del paso 5.

## Paso 3 — Ejecutar las dos líneas base (~5 min)

Crea primero el directorio de trabajo; todos los comandos siguientes escriben ahí.

```bash
mkdir -p /tmp/records-test
./run.sh --seed 1 --name Learner > /tmp/records-test/records-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/records-test/records-0001.txt
```

Después la línea base con funciones:

```bash
cd ../construction-03-functions
./run.sh --seed 1 --name Learner > /tmp/records-test/functions-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/records-test/functions-0001.txt
```

Después las dos entre sí:

```bash
diff -u /tmp/records-test/functions-0001.txt /tmp/records-test/records-0001.txt
```

Tres diferencias vacías. Vuelve a este directorio de la lección y confirma las 20 semillas, no solo
la primera:

```bash
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

Un bucle silencioso son 20 coincidencias. Si `run.sh` no compila, compila a mano una vez:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o build/records-baseline code/records-baseline.cpp
```

Si alguna diferencia no está vacía, detente. Regístrala en `EVIDENCE-es.md` §10. No ajustes una
traza para que coincida con el código.

## Paso 4 — Añadir el campo al registro (~10 min)

Copia la línea base. El archivo versionado no se toca.

```bash
mkdir -p /tmp/records-test
cp code/records-baseline.cpp /tmp/records-test/records-captured.cpp
```

Tres ediciones. La primera es el tipo:

```cpp
struct Knight
{
    int row;
    int column;
    bool captured;
};
```

La segunda es el estado inicial, después de las dos declaraciones de `main`:

```cpp
    Knight playerKnight = {};
    Knight machineKnight = {};
    playerKnight.captured = false;
    machineKnight.captured = false;
```

La tercera es donde ocurre el hecho: la rama de captura de `playTurn`, antes de contar el
movimiento:

```cpp
            otherKnight.captured = true;
```

Compila y ejecuta las 20 semillas:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/records-test/records-captured /tmp/records-test/records-captured.cpp
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/records-test/records-captured --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

20 coincidencias, sin salida. La diferencia contra el archivo versionado son cuatro líneas añadidas
y ninguna eliminada:

```bash
diff -u code/records-baseline.cpp /tmp/records-test/records-captured.cpp
```

Cuatro observaciones, y cada una es parte de la respuesta.

**Una: ninguna firma se movió.** Compara las dos listas de parámetros de `tryMove` y de `playTurn`;
son idénticas. El compilador no tenía nada que comprobar porque nada se volvió a declarar.
Confírmalo con la lista de ediciones, no con la ausencia de errores:

```bash
grep -n 'captured' /tmp/records-test/records-captured.cpp
```

Tres sitios: línea 25 en el tipo, línea 279 en `playTurn`, líneas 338–339 en `main`.

**Dos: la traza no puede ver el campo.** Cambia las dos inicializaciones a `true` y recompila. Las
20 trazas siguen pasando, porque nada lee el campo. La traza comprueba el juego; no comprueba el
hecho. Solo el sitio de asignación se puede comprobar, y solo leyéndolo. Anótalo: una traza de
referencia es un contrato de comportamiento, no un contrato de datos.

**Tres: el valor por defecto ya era correcto.** Devuelve las dos líneas a `false`, después bórralas
y recompila. Las 20 trazas siguen pasando. `Knight playerKnight = {};` pone a cero todos los
miembros, incluido el que acabas de añadir. Las dos líneas explícitas son documentación para quien
lee, no un requisito del programa. Merece la pena saberlo y decirlo: con cuatro enteros paralelos,
olvidar el cuarto inicializador es un error real; con un registro, el inicializador agregado lo
cubre.

**Cuatro: ningún nombre nuevo en el ámbito de espacio de nombres.** El tipo está en el ámbito de
espacio de nombres; no son datos.

```bash
g++ -std=c++20 -c -o /tmp/records-test/rec.o /tmp/records-test/records-captured.cpp
nm -C --defined-only -g /tmp/records-test/rec.o | grep ' [BbDd] '
```

Sigue habiendo una línea, `randomState`. El tipo `Knight` añadió un nombre que el compilador conoce
y que el enlazador no exporta como dato. Dos enteros pasaron a ser un objeto y el conteo de globales
mutables no se movió.

Opcional, si te interesa la disposición en memoria: `sizeof(Knight)` pasa de 8 a 12, con `captured`
en el desplazamiento 8. El compilador rellena el registro hasta un múltiplo de su alineación. Nada
de la transcripción depende de eso, y nada del registro depende del relleno.

Escribe `EVIDENCE-es.md` §5: los comandos exactos, la diferencia, cuáles de tus predicciones de §2.1
se cumplieron y cuál falló. El fallo habitual son «los inicializadores»: se predice cero sitios de
edición y se olvida que un campo con un valor inicial significativo suele querer una línea que lo
muestre.

## Paso 5 — Añadir el mismo hecho sin registro (~12 min)

Ahora el otro lado. Copia la línea base con funciones a un segundo archivo de trabajo:

```bash
cp ../construction-03-functions/code/functions-baseline.cpp \
  /tmp/records-test/functions-captured.cpp
```

El hecho tiene que ser alcanzable dentro de la rama de captura de `playTurn`, igual que antes. Hay
dos lugares donde ponerlo, y la diferencia entre ellos es la diferencia que el registro elimina.

| Dónde viven las banderas | Quién posee el hecho | Firmas obligadas a cambiar |
|---|---|---|
| locales en `playGame`, pasadas a `playTurn` | el bucle | solo `playTurn` |
| locales en `main`, pasadas por `playGame` | el juego | `playTurn` y `playGame` |

Aplica primero la segunda, porque es la comparación honesta: el ciclo de vida del caballo es el
ciclo de vida del juego, y `main` es donde viven los caballos.

1. En `main`, junto a las cuatro declaraciones de coordenadas:
   `bool playerCaptured = false; bool machineCaptured = false;`
2. `playTurn` gana `bool &playerCaptured, bool &machineCaptured`.
3. `playGame` gana los mismos dos parámetros y los reenvía.
4. La rama de captura activa la bandera correcta. `playTurn` tiene `otherKnightRow` y
   `otherKnightColumn` como **valores**, no como una referencia a un caballo, así que la rama debe
   elegir:

   ```cpp
   if (playerIsActive)
   {
       machineCaptured = true;
   }
   else
   {
       playerCaptured = true;
   }
   ```

5. La llamada de `main` los pasa.

Compila y ejecuta las 20 semillas como en el paso 4. Pasan. El juego no cambió; solo cambió la forma
del hecho. Después cuenta a qué llegó el cambio:

| Métrica | Registro | Banderas en `main` |
|---|---|---|
| Sitios de edición | 3 | 6 |
| Cambios de firma | 0 | 2 |
| Llamadas cambiadas | 0 | 2 |
| Llamadores cambiados | 0 | 2 (`playTurn`, `playGame`) |
| Globales nuevas | 0 | 0 |
| Líneas añadidas / eliminadas | 4 / 0 | 20 / 7 |
| Trazas de referencia rotas (de 20) | 0 | 0 |
| Funciones que pueden leer el hecho | cualquiera que tenga un caballo | solo las que lo enumeran |

El conteo de sitios de edición del registro es 3 con las dos líneas explícitas de inicialización y 2
sin ellas. Conserva el número que corresponda al archivo que compilaste, y di cuál es.

Ahora la ubicación más barata. Mueve las dos declaraciones de `main` al cuerpo de `playGame`, quita
los dos parámetros de la firma de `playGame` y de la llamada de `main`, y consérvalos en `playTurn`.
Recompila y ejecuta 20 semillas. Pasan. El conteo baja a 4 sitios de edición, 1 cambio de firma,
1 llamada, 1 llamador, 15 líneas añadidas y 3 eliminadas.

Eso es más barato que el registro en una fila — líneas cambiadas — y no es el mismo cambio. Responde
en `EVIDENCE-es.md` §6.6:

- ¿Por qué compila la ubicación estrecha? Nombra la condición exacta de la que depende.
- ¿Qué le ocurre a la ubicación estrecha si el final de `playGame` imprime qué caballo fue
  capturado?
- ¿Qué hace la versión con registro en ese caso? (Pruébalo: añade la impresión a la copia con
  captura.)
- En `construction-03`, ¿cuál es la edición más pequeña que hace que las dos banderas pertenezcan a
  los caballos y no al bucle?

La última pregunta no tiene respuesta barata, y ese es el resultado. Con variables separadas no hay
forma de decir «este hecho pertenece a ese caballo». Puedes elegir un ámbito y enhebrar nombres por
él; la pertenencia es una convención guardada en un comentario o en la cabeza de quien lee. Con un
registro la pertenencia está en el tipo: `otherKnight.captured` es un campo del objeto del que trata
la captura.

Fíjate también en lo que el registro hizo con el condicional. Cuatro líneas de `if`/`else` pasaron a
ser una asignación, porque `otherKnight` ya es una referencia al caballo correcto. La versión con
funciones tuvo que volver a elegir al perdedor a partir de `playerIsActive`, un dato que ya conocía
y que ya había usado en la línea 241 para elegir `otherKnightName`. El registro le permitió
reutilizar la elección en lugar de repetirla.

## Paso 6 — Comparar y explicar (~8 min)

Escribe `EVIDENCE-es.md` §7. Dos partes.

**Primero, una frase.** Di qué decide el registro sobre añadir un campo, sin usar la palabra
*struct* y sin una palabra de veredicto: *mejor*, *peor*, *limpio*, *más limpio*, *bonito*,
*elegante*, *feo*, *modular*. Si tu frase necesita una de ellas, has descrito tu reacción al
archivo y no el conteo.

**Segundo, un párrafo.** Responde:

- A qué nombres tuvo que llegar el campo nuevo en cada versión: el tipo, el inicializador, la
  asignación y los parámetros que lo transportan.
- Qué dejó de decir la lista de parámetros más corta. Da una función donde perdiste información y
  otra donde no perdiste nada.
- Qué localiza el registro: la definición de los campos de un caballo, el valor por defecto de todos
  ellos y la pertenencia de un hecho al objeto que describe.
- Qué **no** localiza el registro: las lecturas, los invariantes y la representación.
- Qué debe sostener en la memoria de trabajo quien lee para saber qué campos usa una función, en
  cada archivo.
- Qué puede y qué no puede demostrar una traza de referencia sobre este cambio.
- Qué lección posterior corrige cada hueco que queda. Hay tres: representación (`construction-09`),
  invariantes (`construction-10`, `construction-11`) y acceso (`construction-11`).

Una respuesta sólida mantiene separadas dos cosas: **el número de ediciones que cuesta añadir un
campo** y **la precisión de la lista de parámetros**. El registro gana la primera y pierde la
segunda. Una respuesta débil dice que la versión con registro está más limpia, que es un veredicto
que esconde los dos números.

## Paso 7 — Comprobación de aceptación (~2 min)

El criterio de aceptación de esta lección es: **`captured` está añadido al registro; puedes mostrar
qué llamadas cambiaron y cuáles no; y puedes explicar qué localiza el registro.**

Comprueba tu registro contra él:

- [ ] Ejecuté las dos líneas base sin modificar y obtuve tres diferencias vacías, más 20 semillas
      coincidentes.
- [ ] Inventarié las siete funciones que reciben un `Knight` y los campos que toca cada una.
- [ ] Predije el conteo de firmas del registro antes de editar.
- [ ] Añadí `captured` al registro y las 20 trazas siguieron pasando.
- [ ] Mostré que las dos líneas `= false` son opcionales porque `= {}` las cubre.
- [ ] Mostré que la traza no puede detectar un valor por defecto equivocado en un campo no leído.
- [ ] Apliqué el mismo hecho a la línea base con funciones con dos banderas separadas.
- [ ] Conté cambios de firma, llamadas y llamadores para las dos ubicaciones.
- [ ] Puedo nombrar la única fila donde la ubicación estrecha gana al registro, y por qué no es el
      mismo cambio.
- [ ] Mi frase de explicación evita *struct* y las ocho palabras de veredicto.
- [ ] No edité ninguna de las dos líneas base versionadas.
- [ ] Completé `EVIDENCE-es.md` §1–§9 y la autocomprobación de §11.

El criterio no es «ganan los registros». Es «puedes decir exactamente a qué llegó añadir un campo, y
qué dejó de decirte la lista de parámetros más corta».

## Autocomprobación (binaria)

- [ ] Leí las dos líneas base e inventarié qué función toca qué campo.
- [ ] Predije el coste del registro y el de las dos ubicaciones de banderas antes de medir.
- [ ] Compilé las dos líneas base y reproduje las 20 trazas.
- [ ] Añadí `captured` a una copia del registro y mantuve las trazas pasando.
- [ ] Apliqué el mismo hecho a la línea base con funciones y conté las dos ubicaciones.
- [ ] Comparé la predicción con los conteos y nombré los fallos.
- [ ] Expliqué qué localiza el registro y qué no.
- [ ] Completé `EVIDENCE-es.md`.

Son criterios de aceptación. Si una casilla queda sin
marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §9.

## Misconcepciones que esta lección refuta

| Error de concepto | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| «Un registro es solo un saco de variables.» | Es un saco con un ciclo de vida, una declaración y un valor por defecto. El saco es el punto: los campos llegan juntos y se ponen a cero juntos. | Compara los inicializadores, no la lista de campos. |
| «Un registro hace que el código sea comprobable.» | El registro cambia firmas. `tryMove` sigue necesitando un tablero, un número de movimiento y dos parámetros de salida, y `playTurn` sigue sorteando desde una global. | Prueba lo que la firma permite; el registro no añadió una costura. |
| «Añadir un campo nunca toca a los llamadores.» | Cierto para la firma, falso para el uso. Toda función que lea el campo hay que encontrarla con `grep`, porque ninguna firma lo enumera. | Cuenta firmas y cuenta lecturas. Son números distintos. |
| «`const Knight &` significa que la función no puede cambiar el caballo.» | Significa que no puede escribir a través de **esa** referencia. `playTurn` tiene una referencia no constante al mismo objeto. | Mira quién más tiene una referencia mutable antes de fiarte de la promesa. |
| «El registro oculta la representación.» | `row` y `column` aparecen en 14 lugares de seis funciones. Cambiarlos por un índice de casilla reescribe los 14. | Eso es trabajo de `construction-09`, no de esta lección. |
| «Si las trazas pasan, el campo nuevo es correcto.» | Un campo no leído no puede cambiar la salida. Un valor por defecto `true` también pasa las 20 trazas. | Lee el sitio de asignación; la traza solo demuestra que el juego no cambió. |
| «La versión con registro es más barata, así que es el mejor diseño.» | Es más barata para *este* cambio. Es más cara para un cambio que deba enhebrar un campo distinto para cada caballo, y para quien lee y quiere saber qué campos usa una función. | Pon precio al cambio que haces, y después al siguiente. |
| «Menos parámetros significa un contrato más débil.» | Significa un contrato más **grueso**: la función puede leer cualquier campo del objeto. Más grueso puede ser más seguro ante campos nuevos y más débil ante la inspección de campos. | Nombra qué dirección le importa al cambio que vas a hacer. |

## Siguiente paso

`construction-05` — arreglo de registros. Un caballo ya es un objeto; el tablero sigue siendo
`bool visitedCells[9][9]`, y cada casilla se alcanza con un par de índices escrito a mano. El
siguiente peldaño le da nombre a la casilla y un recorrido al tablero, para que la aritmética de
índices aparezca una sola vez.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog-es.md) para la ruta completa.

Si faltaba cualquiera de las dos líneas base, no empieces `construction-05`. El siguiente contraste
necesita los dos lados.

[functions-baseline]: ../construction-03-functions/code/functions-baseline.cpp
