# archaeology-06 — Duplicación y el switch de ocho casos

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** duplicación y extracción · **Prerrequisito:**
> archaeology-04 · **Tiempo estimado:** 60 minutos
> **Lectura:** ~19 minutos
> **Rol de la lección:** opcional · puedes omitirla

## Por qué existe esta lección

`archaeology-04` diagnosticó fallos que producen un resultado equivocado. `archaeology-05`
diagnosticó estado que ninguna función posee. Ambos defectos se anuncian: una celda corrupta, una
función que lee seis globales que nunca recibió. Esta lección es más silenciosa. El programa
funciona. El movimiento es legal, el caballo cae en la casilla correcta, el tablero registra la
jugada. Nada falla y nada imprime un ganador equivocado.

El defecto es que la misma regla está escrita ocho veces. En `movcab`, el `variant-01` archivado
tiene un `switch` de ocho casos cuyos cuerpos ocupan 455 líneas. Si quitas los comentarios, los
espacios y los dígitos de esos cuerpos, la secuencia de tokens es la misma. La única diferencia real
entre el caso 1 y el caso 8 son dos enteros: un delta de fila y un delta de columna. Cuatrocientas
cincuenta y cinco líneas codifican dieciséis enteros.

El código original lo muestra. En la línea 301, entre el caso 1 y el caso 2, la fuente dice:

```text
//los demas casos tienen la misma estructura que esta explicada en el caso 1
```

Ese comentario es el diagnóstico, escrito por la persona que pagó por él.

Este es un fallo de **diseño**, no de corrección. La taxonomía de
[`archaeology-track.md`](../../../docs/guide/archaeology-track.md) define la clase: el programa
funciona, pero la estructura encarece el cambio. El precio no se paga hoy. Se paga en el próximo
cambio de regla, por la próxima persona, ocho veces.

El producto es una **extracción**: reemplazar los cuerpos de caso repetidos por una tabla, congelar
primero el comportamiento anterior y demostrar que el código nuevo lo reproduce. La prueba es una
ejecución, no una opinión: 200 comparaciones, cero discrepancias. El registro es el coste: líneas
eliminadas, casos eliminados, entradas de tabla conservadas, resultado de la prueba.

## Qué podrás hacer

Al terminar esta lección puedes:

- distinguir **texto** duplicado de **conocimiento** duplicado, y decir cuál es este switch;
- leer un cuerpo `case` y separar los datos variables del esqueleto repetido;
- contar lo que cuesta un cambio de regla en el switch y en la tabla, en líneas tocadas;
- capturar un oráculo antes de borrarlo, para que la comparación no sea circular;
- ejecutar una línea base que conserva ambas implementaciones e informa un conteo de discrepancias;
- eliminar el switch en una copia de trabajo y conservar la tabla;
- explicar qué prueban 200 comparaciones con cero discrepancias, y qué no;
- enunciar la frontera de aislamiento: la línea base extrae el delta, no la lógica de límites,
  visitadas, captura, turno o impresión del original;
- vincular la extracción a un peldaño de la pista A sin confundirla con la lección de polimorfismo;
- mantener el archivo de solo lectura mientras trabajas.

La extracción está terminada cuando otra persona puede reproducir la
evidencia.

## El concepto: duplicación y extracción

### La duplicación es conocimiento repetido

Dos líneas parecidas no son automáticamente duplicación. Un cuerpo de bucle y una función que
comparten un `+ 1` no son el mismo defecto. La definición útil trata del **conocimiento**: hay
duplicación cuando una regla está guardada en más de un lugar, de modo que cambiarla exige más de
una edición.

Hay dos formas habituales:

| Forma | Cómo se ve | Por qué duele |
|---|---|---|
| **Clon textual** | Las mismas líneas pegadas dos veces. | Un arreglo en una copia deja la otra mal. |
| **Clon de conocimiento** | La misma regla expresada con otros nombres, orden o sintaxis. | Quien lee no ve que las copias deben moverse juntas. |

El switch de ocho casos es un clon de conocimiento. Los cuerpos no son idénticos byte a byte: el
caso 1 usa `cf-1, cc+2`, el caso 6 usa `cf+2, cc-1`, y los comentarios y espacios difieren. Pero la
regla es una sola: *un caballo se mueve con uno de ocho deltas, y después el mismo esqueleto
comprueba el tablero, el turno, la casilla visitada y la captura, aplica el delta, registra la
jugada e imprime el número de caso*. El delta es dato. El esqueleto es comportamiento. El switch
guarda el delta ocho veces y el esqueleto dieciséis veces.

### El movimiento de extracción

**Extraer** significa: nombrar la parte que varía, nombrar la parte que se queda, conservar una
copia de la parte que se queda y mover las variantes a datos.

En esta lección la parte variable es un par de enteros:

| Caso | Delta de fila | Delta de columna |
|---:|---:|---:|
| 1 | -1 | +2 |
| 2 | -2 | +1 |
| 3 | -2 | -1 |
| 4 | -1 | -2 |
| 5 | +1 | -2 |
| 6 | +2 | -1 |
| 7 | +2 | +1 |
| 8 | +1 | +2 |

Dieciséis enteros. Ese es todo el contenido de información que cambia entre los ocho casos.

La parte que se queda es el esqueleto: límites, conversión, turno, visitada, captura, aplicación,
registro, impresión. En el original está copiado dos veces por caso, una para `cab==1` y otra para
el otro caballo, así que solo el bloque de aplicación existe dieciséis veces.

La extracción no acorta el programa por el gusto de acortarlo. Abarata la **prueba de cambio**. Una
lección de diseño se juzga por un cambio, no por una forma.

### La prueba de cambio

Imagina que la regla cambia: el caballo también se mueve como un camello, o un delta se corrige.
Cuenta los lugares que debes tocar.

| Cambio | Switch | Tabla |
|---|---|---|
| Añadir un noveno movimiento. | Un cuerpo `case` nuevo, de unas 56–61 líneas, más el límite del bucle. | Una entrada nueva en la tabla, más el límite del bucle. |
| Corregir un delta. | Buscar el caso, editar la comprobación de límites, la primera conversión, las dos asignaciones y la segunda conversión. Si olvidas una, el movimiento se aplica a medias. | Editar dos enteros en una fila. |
| Renombrar la etiqueta impresa del caso. | Ocho sitios `cout` (dieciséis con ambas ramas de turno). | Una impresión, guiada por el índice. |

La tabla gana en todas las filas. Ese es el argumento. No es "el switch es feo"; es "el switch
multiplica cada edición por ocho".

### Por qué es un fallo de diseño

El programa es correcto para las entradas que recibe. El coste es estructural:

- **Amplificación del cambio.** Un cambio de regla toca ocho cuerpos.
- **Divergencia silenciosa.** Un arreglo aplicado a siete cuerpos deja un caso con otro
  comportamiento. El compilador no ve la omisión.
- **Coste de revisión.** Un cambio de dos enteros llega como un diff de unas 60 líneas, o como un
  diff con forma de ocho archivos dentro de una función. Quien revisa no ve la regla por culpa de
  las copias.
- **Coste de prueba.** Para probar la regla o ejercitas ocho casos o comparas dos implementaciones.
  La segunda opción es la que construye esta lección.
- **Coste de fusión.** Dos personas que editan casos distintos entran en conflicto en la misma
  función aunque sus cambios sean independientes.

La severidad es media en la escala de la pista: cambio caro, estructura frágil, se arregla con
planificación. No es crítica, porque nada aquí es comportamiento indefinido por sí mismo. El bucle
`i <= 64` de `archaeology-04` es crítico; esto no lo es. El catálogo nombra este defecto
`failure-008`: ocho cuerpos `case` copiados a mano.

### Lo que este switch no es

Tres distinciones mantienen la lección honesta.

1. **No es el problema del despacho.** Un `switch` sobre el número de movimiento también huele a
   polimorfismo ausente (`failure-011`), pero esta lección no reemplaza el despacho con objetos. La
   tabla sigue despachando por índice. Elimina aritmética copiada, no la decisión de despachar. El
   polimorfismo es `archaeology-10`.
2. **No es una lección de rendimiento.** Un compilador moderno puede convertir ya el switch en una
   tabla de salto. La extracción compra coste de cambio, no ciclos. No declares una mejora de
   velocidad que no mediste.
3. **No es la regla completa.** El `movcab` original también comprueba límites, turno, casillas
   visitadas y captura, imprime y puede terminar la partida. La línea base de esta lección aísla el
   cálculo del delta. Es un banco para una decisión, no una reimplementación de `movcab`.

### La restricción heredada

Copiar un caso ocho veces era razonable bajo las restricciones: una sola unidad de traducción, sin
pruebas, sin control de versiones, un compilador antiguo, una fecha límite. Un caso copiado a mano
es auditable a la vista: puedes comparar dos bloques lado a lado. Una tabla exige confiar en una
convención de índice (`moveNumber - 1`) y en un diseño de datos. Esa confianza es lo que da una
prueba, y el proyecto heredado no tenía pruebas.

La lección no es "el código fue descuidado". Es "la estructura encareció el próximo cambio, y una
tabla más una prueba eliminan ese coste".

## Antes de empezar

Lee estos documentos completos:

- [`README-es.md`](README-es.md) — la ficha de la lección, el comando de compilación y el criterio
  de aceptación.
- [archaeology-04](../archaeology-04-correctness-failures/lesson-es.md) — el prerrequisito: síntoma,
  causa raíz, arreglo mínimo, arreglo estructural.
- [archaeology-05](../archaeology-05-global-state/lesson-es.md) — recomendado: el cuerpo de `movcab`
  y sus globales, que es donde vive este switch.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, la
  taxonomía de fallos; §5, la escala de severidad; §9, el mapeo a la pista A.
- [`../../../docs/archive.md`](../../../docs/archive.md) — el archivo fijado, la etiqueta y el
  commit.

También necesitas:

- un clon local del archivo en la etiqueta `v1.0.0`;
- un compilador de C++20 (`c++`, `g++` o `clang++`);
- `bash`;
- un directorio de trabajo fuera de `original/`.

> **Regla:** el archivo es de solo lectura. Copia el archivo que quieras cambiar a un directorio de
> trabajo; nunca edites `original/`. La línea base de esta lección también es evidencia: cópiala
> antes de borrar nada.

## El camino de 60 minutos

| Min | Paso | Qué haces | Evidencia |
|---|---|---|---|
| 0–10 | Predecir | Estimar el tamaño del caso, las líneas que cambian y el coste de una edición | Tabla de predicción |
| 10–25 | Inspeccionar | Leer el switch original y ejecutar la línea base | Notas de anclaje y `comparisons=200 mismatches=0` |
| 25–40 | Extraer | Congelar el oráculo, quitar el switch, conservar la tabla | Copia de trabajo y `golden.txt` |
| 40–45 | Comparar | Ejecutar la versión extraída contra la traza congelada | 200 comparaciones, 0 discrepancias |
| 45–55 | Explicar | Poner precio a la duplicación y nombrar la clase | Tabla de costes y prueba de cambio |
| 55–60 | Registrar | Completar `EVIDENCE-es.md`; confirmar que el archivo está limpio | Entrada de portafolio |

Si contar un cuerpo de caso te lleva diez minutos, ese conteo es la
lección.

### La comprobación de ingeniería en esta lección

La pista B usa el mismo hábito que la pista A, con otro verbo: el comportamiento ya existe, así que
lo **congelas** en lugar de especificarlo desde cero.

| Paso | Dónde ocurre | Qué produce |
|---|---|---|
| **Predecir** | Paso 1 | Una estimación falsable del tamaño del caso y del coste de edición. |
| **Inspeccionar** | Paso 2 | Los anclajes originales, la tabla de deltas y la ejecución de la línea base. |
| **Extraer** | Paso 3 | Una copia de trabajo con una tabla y sin switch. |
| **Comparar** | Paso 4 | 200 comparaciones contra el oráculo congelado, 0 discrepancias. |
| **Explicar** | Paso 5 | La prueba de cambio y el coste de la duplicación. |
| **Registrar** | Paso 6 | `EVIDENCE-es.md` con conteos, diff y resultado. |

## Paso 1 — Predecir (10 min)

Antes de abrir la fuente, responde estas cuatro preguntas por escrito. El objetivo es comprometerte
con un número del que puedas equivocarte.

1. ¿Cuántas líneas ocupa un cuerpo `case`, incluidos su comentario y sus llaves?
2. ¿Cuántas líneas dentro de ese cuerpo cambian realmente de un caso a otro?
3. Si corriges un delta, ¿cuántas líneas deben moverse juntas en un caso?
4. ¿Qué comprará la tabla y qué costará?

Una predicción útil es falsable. "El switch es repetitivo" no lo es. "Cada cuerpo tiene unas 50
líneas, dos líneas llevan el delta y una edición toca cuatro líneas de un cuerpo" sí lo es.

### Una predicción trabajada

Antes de leer, yo escribiría:

- tamaño del caso: unas 50 líneas, porque el cuerpo tiene dos ramas de turno y cada rama repite el
  bloque de aplicación;
- líneas que cambian: dos enteros, pero aparecen en cuatro lugares por rama (límites, primera
  conversión, asignación, segunda conversión);
- coste de edición: un caso, unas seis líneas, si encuentro todas las apariciones;
- beneficio de la tabla: una fila por movimiento, una búsqueda, un lugar donde cambiar;
- coste de la tabla: una convención de índice y una guarda `default` ausente.

Cada una de esas afirmaciones se puede comprobar. Ese es el estándar. Si tu predicción dice "ocho
casos", todavía no has predicho nada; la fuente ya te dijo que hay ocho.

## Paso 2 — Inspeccionar (15 min)

### Leer el switch original

Abre `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp` en la etiqueta
`v1.0.0` y busca `movcab`. El switch empieza en la línea 239. Usa esta tabla de anclajes para
navegar; los números de línea son del archivo fijado.

| Línea | Qué hay |
|---:|---|
| 239 | `switch(vecjugadas[i])` — despacho por número de movimiento. |
| 241 | `case 1` — delta `(-1, +2)`. |
| 301 | El comentario: los demás casos tienen la misma estructura que el caso 1. |
| 302 | `case 2` — delta `(-2, +1)`. |
| 359 | `case 3` — delta `(-2, -1)`. |
| 415 | `case 4` — delta `(-1, -2)`. |
| 471 | `case 5` — delta `(+1, -2)`. |
| 527 | `case 6` — delta `(+2, -1)`. |
| 584 | `case 7` — delta `(+2, +1)`. |
| 640 | `case 8` — delta `(+1, +2)`. |
| 696 | `default` — la rama fuera de rango. |
| 241–695 | Los ocho cuerpos de caso: 455 líneas. |

Las otras variantes de 2008 llevan el mismo switch con sus propios nombres:

| Variante | Línea del `switch` | Primer `case` |
|---|---:|---:|
| `variant-01-baseline-2008` | 239 | 241 |
| `variant-02-records-2008` | 290 | 292 |
| `variant-03-pointers-dev-2008` | 316 | 318 |
| `variant-04-pointers-final-2008` | 330 | 332 |

Las variantes con punteros cambiaron el contenedor, no la duplicación. Merece la pena notarlo: un
cambio de sintaxis no eliminó el problema de diseño.

### El esqueleto, una vez

Lee el caso 1 desde la línea 241 hasta la 295. Contiene dos ramas casi idénticas: una para `cab==1`
y otra para el otro caballo. Esta es la rama de `cab==1`, con los pasos repetidos marcados:

| Línea | Paso | ¿Varía? |
|---:|---|---|
| 243 | Guarda: este caso es el movimiento actual y la partida no terminó. | Solo el número de caso. |
| 245 | Comprobación de límites con el delta. | El delta. |
| 247 | `ccab = conversionmv(cf-1, cc+2)` — casilla destino. | El delta. |
| 249 | Rama de turno: `if (cab==1)`. | No. |
| 252 | Comprobación de casilla visitada contra el tablero. | No. |
| 254 | Comprobación de captura contra el otro caballo. | No. |
| 257 | `ganador=0` al capturar. | No. |
| 262 | `cout << " 1."` — imprime el número de caso. | El número de caso. |
| 263 | `i=-1` — deja de recorrer casos. | No. |
| 265–266 | Aplicar el delta a `cf` y `cc`. | El delta. |
| 267 | Recalcular la casilla destino. | No. |
| 269 | `vectab[ccab]=jugada` — registra la jugada. | No. |
| 270 | `break`. | No. |

Después, las líneas 274–295 repiten el mismo bloque para el otro caballo. Un cuerpo de caso contiene
el bloque de aplicación dos veces; ocho casos contienen dieciséis copias. El switch no son ocho
copias de una regla. Son dieciséis copias de un esqueleto envueltas alrededor de ocho copias de una
carga de dos enteros.

### Los datos variables

Los únicos valores que cambian entre casos son los dos deltas. Todo lo demás es la misma secuencia
de tokens después de quitar comentarios, espacios y dígitos. Puedes comprobarlo: copia los ocho
cuerpos, elimina comentarios y líneas en blanco, reemplaza cada dígito por `N` y compáralos con
`diff`. Los casos 1 y 2 quedan idénticos; el resto difiere solo en los signos de los dos deltas.

Esa es la evidencia más fuerte de la lección. También es la razón por la que la tabla es posible: si
la estructura es la misma, la estructura se puede escribir una vez.

### Ejecutar la línea base

Desde el directorio de esta lección:

```bash
cd archaeology/lessons/archaeology-06-duplication-switch
./run.sh
```

Salida esperada:

```text
comparisons=200 mismatches=0
```

El script compila `code/movement.cpp` con:

```text
-std=c++20 -Wall -Wextra -Werror -O2
```

`-Werror` importa: la línea base está limpia de advertencias bajo las banderas con las que se
distribuye. Si tu compilador la rechaza, registra el compilador y el diagnóstico; no silencies la
advertencia para que la ejecución pase.

### Leer la línea base con honestidad

`code/movement.cpp` tiene 83 líneas. Contiene:

| Parte | Líneas | Qué es |
|---|---:|---|
| `Position` | 4–10 | Una fila y una columna, con `operator==`. |
| `Delta` | 12–16 | Los dos enteros que varían. |
| `moveDeltas` | 18–27 | Ocho entradas: los datos extraídos. |
| `apply_switch` | 29–52 | El oráculo: ocho casos explícitos. |
| `apply_table` | 54–58 | La extracción: una búsqueda indexada. |
| `main` | 60–83 | 8 movimientos × 25 posiciones = 200 comparaciones. |

Las 25 posiciones son la rejilla de `-2` a `+2` en ambas coordenadas. Esa rejilla ejercita deltas
positivos y negativos sin necesitar un tablero. No prueba los límites del tablero, porque la línea
base no tiene tablero.

Lo que la línea base **no** contiene: comprobaciones de límites, de visitadas, de captura, selección
de turno, impresión ni terminación. Aísla el cálculo del delta. No describas una ejecución correcta
como prueba de que `movcab` se preservó; es prueba de que el delta se preservó.

### Comprobar tu predicción

Compara los cuatro números que escribiste en el paso 1 con los anclajes. El tamaño medido del caso
es de 56–61 líneas (42–43 sin líneas en blanco). El resumen de la pista los llama cuerpos de "~48
líneas"; el conteo exacto depende de si cuentas comentarios y líneas en blanco. De cualquier forma,
el orden de magnitud es el punto: una regla, decenas de líneas por copia.

## Paso 3 — Extraer (15 min)

### Crear la copia de trabajo

Crea un directorio de trabajo fuera de `original/`. `build/` está ignorado por este repositorio y es
un buen lugar:

```bash
mkdir -p archaeology/lessons/archaeology-06-duplication-switch/build/scratch
cd archaeology/lessons/archaeology-06-duplication-switch/build/scratch
cp ../../code/movement.cpp movement.cpp
```

De aquí en adelante, edita `build/scratch/movement.cpp`. No edites `code/movement.cpp`; es la línea
base que ejecutará la próxima persona.

### Congelar el oráculo antes de borrarlo

Este es el paso que hace honesta la comparación. Si borras `apply_switch` y después comparas
`apply_table` con una tabla que escribiste a partir de los mismos deltas, has comparado la tabla
consigo misma. Cero discrepancias no probaría nada.

Captura primero la salida del switch. Reemplaza el cuerpo de `main` por este bucle temporal, antes
de quitar nada:

```cpp
for (int moveNumber = 1; moveNumber <= 8; moveNumber += 1)
{
    for (int row = -2; row <= 2; row += 1)
    {
        for (int column = -2; column <= 2; column += 1)
        {
            Position result = apply_switch(moveNumber, Position{row, column});
            std::printf("%d %d %d %d %d\n", moveNumber, row, column,
                        result.row, result.column);
        }
    }
}
```

Mantén `return 0;` después del bucle. Compílalo, ejecútalo y guarda la salida estándar como
`golden.txt`:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 movement.cpp -o movement-golden
./movement-golden > golden.txt
wc -l golden.txt
```

`wc -l` debe imprimir `200`. Cada línea es una observación: número de movimiento, fila inicial,
columna inicial, fila resultante, columna resultante. Ese archivo es ahora el oráculo. El switch ya
no hace falta para producirlo.

### Quitar el switch

En la copia de trabajo:

1. borra la función `apply_switch` (24 líneas, líneas 29–52 de la línea base);
2. conserva `moveDeltas` y `apply_table`;
3. reescribe `main` para leer `golden.txt` y comparar cada línea con `apply_table`;
4. conserva la línea de salida `comparisons=200 mismatches=0`;
5. conserva el código de salida: cero solo cuando no hay discrepancias.

Un lector compacto se parece a esto:

```cpp
#include <fstream>

// inside main
int comparisonCount = 0;
int mismatchCount = 0;
std::ifstream golden{"golden.txt"};
int moveNumber = 0;
int row = 0;
int column = 0;
int expectedRow = 0;
int expectedColumn = 0;
while (golden >> moveNumber >> row >> column >> expectedRow >> expectedColumn)
{
    Position result = apply_table(moveNumber, Position{row, column});
    comparisonCount += 1;
    if (result.row != expectedRow || result.column != expectedColumn)
    {
        mismatchCount += 1;
    }
}
```

Después del bucle, imprime `comparisons=%d mismatches=%d` y devuelve cero solo cuando
`mismatchCount` sea cero. Compila con las mismas banderas y ejecuta desde el directorio que contiene
`golden.txt`:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 movement.cpp -o movement
./movement
```

Salida esperada, sin cambios:

```text
comparisons=200 mismatches=0
```

El switch desapareció de la copia de trabajo. La tabla permanece. La prueba sigue pasando.

### El coste de la extracción

Registra estos números, no una impresión:

| Medida | Valor |
|---|---:|
| Líneas eliminadas con `apply_switch` | 24 |
| Cuerpos `case` eliminados | 8 |
| Entradas de tabla conservadas | 8 |
| Líneas añadidas para el lector de `golden.txt` | unas 20 |
| Comparaciones después de la extracción | 200 |
| Discrepancias después de la extracción | 0 |

En el original, la misma extracción es mayor: 455 líneas de cuerpos de caso se convierten en una
tabla de 10 líneas y una búsqueda. El esqueleto —límites, visitadas, captura, aplicación, registro—
todavía hay que escribirlo una vez, fuera de la tabla. Ese es el siguiente tamaño del arreglo, y
pertenece al paso 5.

### Prueba de cambio opcional

Añade un noveno delta a `moveDeltas` y cambia el límite del bucle a 9. En la versión con tabla son
una entrada y un límite. En la versión con switch es un cuerpo `case` nuevo de unas 56–61 líneas más
el límite. Cuenta ambos. La proporción es el argumento de diseño en un solo número.

Si haces esto, regenera `golden.txt` desde el switch primero, o deja el noveno movimiento fuera de
la comparación. No compares una tabla de nueve movimientos contra una traza de ocho y llames
discrepancia a la línea que falta.

## Paso 4 — Comparar (5 min)

### Qué dice la ejecución

```text
comparisons=200 mismatches=0
```

Doscientas observaciones, cero diferencias. Para cada movimiento 1–8 y cada posición inicial de la
rejilla de 5×5, `apply_table` devuelve el mismo `Position` que el `apply_switch` congelado.

### Qué prueba la igualdad

- Los ocho deltas de `moveDeltas` coinciden con los ocho deltas codificados en `apply_switch`.
- La convención de índice `moveNumber - 1` mapea 1–8 a las entradas 0–7.
- La tabla aplica ambas coordenadas en la misma dirección que el switch.
- Para estas 200 entradas, la extracción preserva el comportamiento.

### Qué no prueba la igualdad

| Afirmación | ¿Probada? | Por qué |
|---|---|---|
| La tabla es equivalente para todo número de movimiento `int`. | No. | Solo se probaron 1–8. |
| El `movcab` original sigue comportándose igual. | No. | La línea base aísla el delta. |
| La lógica de límites, visitadas, captura y turno se preserva. | No. | No está en la línea base. |
| La extracción maneja entradas fuera de rango como el switch. | No. | El switch tiene `default`; la tabla no. |
| La tabla es más rápida. | No. | No se hizo ninguna medición. |
| La tabla es más fácil de cambiar. | Por la prueba de cambio, sí. | Cuenta las líneas tocadas; no declares un benchmark. |

### La dirección del oráculo

El orden importa. El switch es el oráculo porque es el código que existía. La tabla es la candidata
porque es el código que escribiste. Si generas ambas desde la misma tabla, la comparación es
circular: estás comprobando que un valor es igual a sí mismo.

Esta es la regla transferible: **congela el comportamiento anterior antes de cambiar la
estructura**. Es la misma regla que una traza de referencia en `common-05`, aplicada a un refactor
en lugar de a un programa completo.

### Cuando aparece una discrepancia

No "arregles" la tabla hasta entender la discrepancia. Imprime la observación que falla:

```text
move=6 row=-1 column=2 expected=(1,1) actual=(1,-1)
```

La tupla te dice si el delta está mal, si el signo está mal o si el índice está desfasado en uno.
Una discrepancia es información sobre la extracción, no un fallo de la lección.

## Paso 5 — Explicar (10 min)

### Poner precio a la duplicación

| Coste | Evidencia en el original |
|---|---|
| Un cambio de regla toca ocho cuerpos. | Casos en 241, 302, 359, 415, 471, 527, 584, 640. |
| Una copia olvidada diverge en silencio. | El compilador no compara el caso 3 con el caso 6. |
| La revisión ve unas 60 líneas para un cambio de 2 enteros. | Un delta aparece en la comprobación de límites, la primera conversión, las asignaciones y la segunda conversión. |
| El mismo esqueleto se copia dos veces por caso. | Las líneas 249–270 y 274–295 son las dos ramas de turno del caso 1. |
| La duplicación sobrevive a un cambio de sintaxis. | Las variantes 02, 03 y 04 conservan el switch bajo contenedores distintos. |
| Las pruebas deben enumerar casos o comparar implementaciones. | La línea base elige comparar, lo que exige un oráculo. |

### La clase, no la instancia

La instancia es "ocho casos". La clase es "una regla guardada como copias de código en lugar de
datos". Busca la clase en el resto del archivo:

- las dos ramas de turno dentro de cada caso — dieciséis copias del bloque de aplicación;
- el mismo switch repetido en cuatro variantes — una copia a nivel de archivo;
- los arreglos paralelos y las variables sueltas del caballo — la clase de la próxima lección
  (`archaeology-07`);
- el `switch` sobre el tipo de movimiento — la clase del despacho (`archaeology-10`).

El arreglo sistémico no es "borra estos ocho casos". Es "guarda los datos variables una vez,
conserva un esqueleto y prueba la frontera entre ambos". Ese arreglo elimina la clase.

### Dos arreglos, dos tamaños

**Arreglo mínimo — la tabla.** Reemplaza los ocho cuerpos de caso por una tabla y una búsqueda. En
la línea base, la tabla ya existe; tu extracción elimina el switch. En el original, el mismo
movimiento conservaría el esqueleto de `movcab` y reemplazaría solo la aritmética por caso con una
búsqueda de `Delta`. Es el cambio más pequeño que elimina el delta copiado. No elimina el esqueleto
duplicado, los globales, la impresión ni las pruebas ausentes.

**Arreglo estructural — una función de aplicación.** Extrae toda la aplicación del movimiento a una
función que recibe un `Delta` y el estado que necesita:

```cpp
MoveResult applyMove(Position from, Delta delta, const Board &board, int turn);
```

La función comprueba límites, comprueba la casilla visitada, detecta captura, aplica el delta,
registra la jugada y devuelve el resultado. Los ocho casos desaparecen; la tabla queda como dato.
Las dos ramas de turno se convierten en una llamada. Vínculos con la pista A: `construction-03`
(funciones y parámetros) para el esqueleto, `construction-05` (arreglo de registros) para la tabla y
`construction-19` (diseño orientado a datos) para la idea de que los deltas son datos, no código.

`construction-12` (polimorfismo) es el arreglo del despacho, no de la duplicación. No la cites aquí
como si fuera la misma reparación; `archaeology-10` es dueña de ese argumento.

### Qué no arregla la extracción

- La tabla confía en `moveNumber - 1`. `apply_table(0, ...)` y `apply_table(9, ...)` son
  comportamiento indefinido. El `default` del switch de la línea base devolvía la posición sin
  cambios. Si conservas la tabla, conserva una guarda o enuncia la precondición y hazla cumplir en
  el sitio de llamada. Esta es una regresión real que la prueba de 200 comparaciones no puede ver,
  porque la prueba nunca llama fuera de rango.
- La tabla no elimina la duplicación de ramas de turno dentro del `movcab` original.
- La tabla no elimina los globales ni la impresión.
- La tabla no hace que el juego sea testeable de extremo a extremo. Hace testeable un cálculo.

### El compromiso, dicho con justicia

La tabla cuesta una convención de índice, una guarda de rango y una separación entre datos y código.
El switch cuesta ocho copias y un amplificador de cambios. En un programa de un solo uso y sin
pruebas, el switch es más fácil de verificar a ojo. En un programa mantenido y con una prueba, la
tabla es más barata. La lección te pide elegir con la prueba de cambio, no con el gusto.

## Paso 6 — Registrar (5 min)

Completa [`EVIDENCE-es.md`](EVIDENCE-es.md):

- configuración y etiqueta del archivo;
- la tabla de predicción, escrita antes de inspeccionar;
- los anclajes originales y el conteo medido de casos;
- la ejecución de la línea base y su salida exacta;
- la captura del oráculo: comando, `golden.txt`, número de líneas;
- el diff de la extracción: qué se eliminó, qué se conservó, qué se añadió;
- la tabla de costes: líneas, casos, entradas, comparaciones, discrepancias;
- el resultado de la comparación y la dirección del oráculo;
- la frontera de evidencia: qué prueba la ejecución y qué no;
- la comprobación de integridad del archivo.

Una frase como "el switch está duplicado" es una opinión. "Ocho cuerpos, 455 líneas, 16 enteros, 24
líneas eliminadas, 200 comparaciones, 0 discrepancias" es un diagnóstico.

## El original en detalle

Datos que puedes citar, con números de línea de `variant-01` en la etiqueta `v1.0.0`:

| Ubicación | Qué hay |
|---:|---|
| 239 | `switch(vecjugadas[i])` — el despacho de ocho vías. |
| 241–301 | Caso 1: 61 líneas, delta `(-1, +2)`. |
| 301 | Comentario: los demás casos tienen la misma estructura que el caso 1. |
| 302–358 | Caso 2: 57 líneas, delta `(-2, +1)`. |
| 359–414 | Caso 3: 56 líneas, delta `(-2, -1)`. |
| 415–470 | Caso 4: 56 líneas, delta `(-1, -2)`. |
| 471–526 | Caso 5: 56 líneas, delta `(+1, -2)`. |
| 527–583 | Caso 6: 57 líneas, delta `(+2, -1)`. |
| 584–639 | Caso 7: 56 líneas, delta `(+2, +1)`. |
| 640–695 | Caso 8: 56 líneas, delta `(+1, +2)`. |
| 696–718 | `default`: imprime el caso, avanza `i`, detecta caballo encerrado, termina la partida. |
| 245 | La comprobación de límites que la línea base omite. |
| 252 | La comprobación de visitada que la línea base omite. |
| 254 | La comprobación de captura que la línea base omite. |
| 249, 274 | Las dos ramas de turno que duplican el bloque de aplicación. |

Los tamaños de caso son líneas totales, incluidos comentarios, líneas en blanco y llaves. Los
conteos sin líneas en blanco son 42–43. Se miden desde una etiqueta `case` hasta la línea anterior a
la siguiente, así que el caso 1 incluye el comentario de la línea 301. La cifra de "~48 líneas" del
resumen de la pista es una aproximación; cita tu propia medición.

## Qué prueba y qué no prueba la extracción

| Afirmación | ¿Respaldada? | Por qué |
|---|---|---|
| La tabla reproduce el switch para 200 entradas. | Sí | La ejecución imprimió 0 discrepancias. |
| La tabla es equivalente para todas las entradas. | No | La prueba cubre 8 movimientos × 25 posiciones. |
| El comportamiento del `movcab` original se preserva. | No | La línea base aísla el delta. |
| La extracción elimina 24 líneas y 8 casos de la línea base. | Sí | Cuenta el diff. |
| La extracción reduce las 455 líneas de casos del original a una tabla. | Sí, por construcción | La tabla tiene 8 entradas y una búsqueda; el esqueleto es aparte. |
| La extracción elimina el despacho. | No | La tabla sigue siendo despacho por índice. |
| La extracción elimina la duplicación del esqueleto. | No | Solo el arreglo estructural lo hace. |
| La tabla preserva el comportamiento de `default`. | No | La tabla no tiene guarda fuera de rango. |
| La prueba de cambio es más barata con la tabla. | Sí, por conteo de líneas | Una entrada frente a un caso de ~60 líneas. |
| La tabla es más rápida en tiempo de ejecución. | No medido | No lo declares. |

Esta tabla es la comprobación de honestidad. Un registro que dice "el refactor es correcto" sin
nombrar la muestra se excede. Di "para estas 200 entradas" cuando eso es lo que ejecutaste.

## Autocomprobación (binaria)

- [ ] Escribí las cuatro predicciones antes de abrir la fuente.
- [ ] Encontré el switch en la línea 239 y los ocho casos en sus anclajes.
- [ ] Leí el comentario de la línea 301 y lo cité.
- [ ] Conté los cuerpos de caso: ocho, 56–61 líneas cada uno, 455 líneas en total.
- [ ] Nombré los 16 enteros que realmente varían.
- [ ] Identifiqué los pasos del esqueleto que no varían.
- [ ] Ejecuté `./run.sh` y vi `comparisons=200 mismatches=0`.
- [ ] Registré el compilador y las banderas.
- [ ] Copié la línea base a un directorio de trabajo antes de editarla.
- [ ] Capturé `golden.txt` desde `apply_switch` antes de borrarlo.
- [ ] `golden.txt` tiene 200 líneas.
- [ ] Eliminé `apply_switch` y conservé `moveDeltas` y `apply_table`.
- [ ] La versión extraída imprime `comparisons=200 mismatches=0`.
- [ ] Registré el coste de la extracción: 24 líneas, 8 casos, 8 entradas, 200/0.
- [ ] Expliqué por qué comparar la tabla consigo misma sería circular.
- [ ] Enuncié que la línea base no incluye límites, visitadas, captura ni turno.
- [ ] Noté que la tabla pierde la guarda `default` del switch.
- [ ] Vinculé el arreglo estructural a `construction-03`, `construction-05` y `construction-19`.
- [ ] No declaré una mejora de rendimiento.
- [ ] El archivo está sin modificar (`git status` en el archivo está limpio).

Si una casilla no está marcada, la casilla sin marcar nombra el siguiente paso. No hay penalización
por repetir uno.

## Misconcepciones que esta lección refuta

| Idea equivocada | Por qué es falsa | Qué hacer en su lugar |
|---|---|---|
| "Duplicación significa texto idéntico." | Los casos difieren en comentarios, espacios y signos, pero comparten una regla. | Compara el conocimiento, no los bytes. |
| "El switch son ocho reglas." | Es una regla de delta más un esqueleto, copiados. | Separa los datos variables de la estructura repetida. |
| "Una tabla siempre es mejor que un switch." | Una tabla añade una convención de índice y pierde la guarda `default`. | Pon precio a ambas con la prueba de cambio. |
| "Cero discrepancias prueba que el refactor es correcto." | Prueba igualdad en 200 entradas de muestra, con un oráculo congelado. | Enuncia la muestra y la dirección del oráculo. |
| "La línea base es la regla original." | Aísla el delta; límites, visitadas, captura y turno están ausentes. | Escribe la frontera de aislamiento en el registro. |
| "Borra el código viejo y luego prueba el nuevo." | Sin un oráculo congelado, la comparación es circular. | Captura la traza de referencia primero. |
| "El compilador ya optimiza el switch, así que la lección no sirve." | La lección trata del coste de cambio, no de ciclos. | Argumenta con líneas tocadas, no con nanosegundos. |
| "Extraer significa tener menos líneas." | El punto es tener menos lugares donde cambiar; el esqueleto puede quedarse. | Cuenta lugares tocados, no solo líneas. |
| "El switch y el polimorfismo son el mismo problema." | El despacho y la aritmética copiada son clases distintas. | Arregla la duplicación aquí; el despacho en `archaeology-10`. |
| "La rama `default` es código muerto." | Es el contrato fuera de rango; la tabla lo pierde. | Conserva una guarda o haz cumplir la precondición. |
| "Una prueba que pasa significa que el original es seguro de cambiar." | El original no tiene prueba ni compilación en este laboratorio. | Mantén el archivo de solo lectura; repara en una copia de trabajo. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Duplicación | Una regla guardada en más de un lugar. |
| Clon de conocimiento | La misma regla escrita con otros nombres, orden o sintaxis. |
| Extracción | Mover la parte variable a datos y conservar una copia del resto. |
| Delta | El par de enteros que se suma a una posición para producir la siguiente. |
| Basado en tabla | Elegir comportamiento indexando datos en lugar de escribir una rama por caso. |
| Oráculo | La implementación cuyo comportamiento confías mientras la reemplazas. |
| Traza de referencia | Un registro congelado de salidas usado para comparar una implementación nueva. |
| Prueba de cambio | Un cambio concreto cuyos lugares tocados miden el diseño. |
| Despacho | Elegir qué código se ejecuta según un valor. |
| Rama por defecto | La ruta del `switch` para valores que ningún caso cubre. |
| Convención de índice | La regla que mapea un número de movimiento a una posición de la tabla, aquí `n - 1`. |
| Fallo de diseño | El programa funciona, pero la estructura encarece el cambio. |
| Frontera de evidencia | La línea entre lo que prueba la ejecución y lo que infieres. |

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
