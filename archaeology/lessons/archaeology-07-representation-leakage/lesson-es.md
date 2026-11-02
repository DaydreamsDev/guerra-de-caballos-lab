# archaeology-07 — Fuga de representación y arreglos paralelos

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** fuga de representación y registros · **Prerrequisito:**
> archaeology-05 · **Tiempo estimado:** 60 minutos
> **Lectura:** ~18 minutos
> **Rol de la lección:** opcional · puedes omitirla

## Por qué existe esta lección

`archaeology-04` diagnosticó fallos que producen un resultado equivocado. `archaeology-05`
diagnosticó estado que ninguna función posee. `archaeology-06` diagnosticó una regla copiada ocho
veces. Esta lección trata un defecto más silencioso: el programa funciona y la forma de sus datos es
pública.

En la línea base de 2008, la posición de un caballo se guarda dos veces. `cab1` y `cab2` contienen
índices planos del tablero de `0` a `63`. `cf1`, `cc1`, `cf2` y `cc2` contienen las mismas dos
posiciones como fila y columna. `conversionvm` escribe la copia de fila/columna a partir del índice
plano; `conversionmv` convierte una fila y una columna de vuelta a un índice plano. Nada garantiza
que las dos copias coincidan. El llamador las mantiene sincronizadas a mano:

```cpp
cab1 = movcab(cf1, cc1);
```

Una línea actualiza `cf1` y `cc1` mediante parámetros por referencia y escribe el índice plano
devuelto en `cab1`. Dos representaciones, dos mecanismos de actualización, una asignación. Si la
función retorna antes de tiempo, o si una edición futura actualiza una y no la otra, la posición se
desincroniza.

El tablero tiene el mismo problema. `vectab` es `int[64]`, y cada llamador lo indexa directamente:
`vectab[cab1]`, `vectab[ccab]`, `vectab[k]`. La fórmula `cf * 8 + cc` no está escondida detrás de
una operación; es el contrato. Un llamador que lee una celda debe saber que el tablero es un arreglo
plano y debe convertir sus coordenadas con la misma convención.

Esto es un fallo de **diseño**, no de corrección. La taxonomía de
[`archaeology-track.md`](../../../docs/guide/archaeology-track.md) lo nombra: el programa funciona,
pero la estructura encarece el cambio. El catálogo lo registra como `failure-009` (arreglos
paralelos y variables sueltas de los caballos) y `failure-010` (un envoltorio que no es un tipo
abstracto de datos).

El producto es un **registro y un dueño**: un `Knight` que agrupa fila y columna, y un `Board` que
posee sus celdas visitadas y expone `mark` y `visited_count`. La prueba es una ejecución: el mismo
escenario, el mismo resumen, `match=yes`.

## Qué podrás hacer

Al terminar esta lección puedes:

- distinguir un **registro** de un conjunto de variables paralelas, y decir qué vuelve imposible el
  registro;
- nombrar cada lugar donde el código de 2008 expone la representación del tablero;
- explicar por qué dos representaciones de una posición son un riesgo de desincronización, no una
  comodidad;
- contar los llamadores que deben cambiar cuando cambia el tamaño del tablero o la fórmula interna
  del índice;
- introducir `Knight` y `Board` en una copia de trabajo y actualizar el llamador;
- comparar los programas de antes y después y enunciar exactamente qué prueba `match=yes`;
- enunciar la frontera de aislamiento: la línea base aísla el cambio de registro/tablero, no el
  archivo;
- vincular el arreglo a peldaños de la pista A sin confundir registros con tipos abstractos de
  datos;
- mantener el archivo de solo lectura mientras trabajas.

El refactor está terminado cuando otra persona puede reproducir la
ejecución.

## El concepto: fuga de representación y registros

### Dos representaciones de una posición

El código de 2008 guarda un caballo dos veces:

| Representación | Nombres | Rango | La produce | La consume |
|---|---|---|---|---|
| Índice plano del tablero | `cab1`, `cab2` | `0..63` | `rand() % 64` | `vectab[cab1]`, `vectab[cab2]` |
| Fila y columna | `cf1`, `cc1`, `cf2`, `cc2` | `0..7` cada una | `conversionvm` | `movcab(cf1, cc1)` |

`conversionvm` son cuatro asignaciones:

```cpp
cf1 = cab1 / 8;
cc1 = cab1 % 8;
cf2 = cab2 / 8;
cc2 = cab2 % 8;
```

`conversionmv` es una expresión:

```cpp
ccab = cf * 8 + cc;
```

El par es una caché mantenida a mano. La fuente no dice cuál es la autoritativa. A veces lo es el
índice plano: `ordenjuego` escribe `vectab[cab1] = jugada`. A veces lo es el par fila/columna:
`movcab` comprueba los límites con `cf` y `cc`. El lector tiene que decidir, en cada llamada, cuál
copia está vigente.

### Variables paralelas

Un registro agrupa campos que pertenecen juntos. Las variables paralelas son lo contrario:
pertenecen juntas pero se declaran separadas.

```cpp
int playerRow = 1;
int playerColumn = 1;
int machineRow = 8;
int machineColumn = 8;
```

Nada en el lenguaje impide que una edición futura escriba `playerRow = 2` y deje `playerColumn` en
`1`. Nada impide que una llamada mezcle la fila del jugador con la columna de la máquina. El
invariante —*una posición es una fila y una columna que se mueven juntas*— existe solo en la cabeza
de quien programa.

El archivo de 2008 tiene la misma forma a mayor escala: cuatro enteros de coordenadas sueltos, dos
índices planos, un selector de turno y un arreglo de tablero. El sistema de tipos ve siete enteros
independientes y un arreglo de tablero. El dominio ve dos caballos y un tablero.

### Qué se filtra

**Fuga de representación** significa que un llamador debe conocer cómo se guarda un valor para
poder usarlo.

En `parallel_arrays.cpp`, el llamador calcula el índice del tablero tres veces:

```cpp
visited[(playerRow - 1) * 8 + (playerColumn - 1)] = true;
```

Esa expresión codifica cuatro decisiones que el llamador no debería poseer:

- el tablero tiene ocho columnas de ancho;
- el almacenamiento es en orden por filas;
- las coordenadas son de base uno en el punto de llamada;
- el conjunto de visitadas es un arreglo plano de 64 celdas.

Cambia cualquiera de ellas y el llamador cambia. El tablero no puede cambiar su representación sin
editar cada punto de llamada.

### El registro y el dueño

El refactor tiene dos movimientos.

1. **Registro.** `Knight` agrupa `row` y `column`. Una posición pasa a ser un valor que se asigna,
   se pasa y se compara como una unidad.
2. **Dueño.** `Board` posee el arreglo de visitadas. Expone `mark(const Knight &)` y
   `visited_count()`. La fórmula del índice se muda dentro de `mark`, que es donde corresponde.

El llamador queda así:

```cpp
board.mark(player);
player = Knight{2, 3};
board.mark(player);
```

Sin índice, sin ancho, sin origen, sin arreglo. El llamador enuncia la operación del dominio.

### Qué compra un registro y qué no

Un registro compra:

- **Un solo lugar para cambiar.** Agrega un campo a `Knight` y el campo viaja con el valor.
- **Un invariante que puedes nombrar.** Una posición es un objeto, no cuatro variables.
- **Una unidad testeable.** Puedes construir un `Knight` y compararlo sin un tablero.
- **Una frontera.** `Board` puede cambiar su arreglo por un bitset, un `std::vector` o una grilla
  de 10×10, y los llamadores siguen compilando.

Un registro no compra:

- **Validación.** `Knight{99, 99}` sigue siendo construible. Los límites necesitan un invariante
  (`construction-10`) o un constructor privado (`construction-11`).
- **Propiedad.** Agrupar fila y columna no le da dueño al tablero. `Board` hace eso.
- **Un programa más corto.** La versión con registro es más larga: 67 líneas contra 47.

La longitud es el precio. La lección te pide cotizar el cambio, no contar líneas porque sí.

### Por qué esto es un fallo de diseño

La severidad es media en la escala de la pista: cambio caro, estructura frágil, se corrige en una
planificación. El programa no es comportamiento indefinido por sí mismo. El comportamiento
indefinido del archivo es el bucle `i <= 64`, y eso pertenece a `archaeology-04`. Lo que causa la
representación es:

- **Amplificación del cambio.** Un cambio de tamaño del tablero toca cada expresión de índice.
- **Desincronización silenciosa.** Dos copias de una posición pueden discrepar y ningún compilador
  lo ve.
- **Coste de revisión.** Quien revisa debe reconstruir la convención del índice para leer un diff.
- **Coste de prueba.** Una prueba unitaria debe construir la representación, no solo el valor del
  dominio.
- **Coste de conocimiento.** Un lector nuevo debe aprender `8`, `64`, las coordenadas de base uno
  y las dos funciones de conversión antes de tocar un movimiento.

## Antes de empezar

Lee estos documentos completos:

- [`README.md`](README.md) — la ficha de la lección, el comando de compilación y el criterio de
  aceptación.
- [archaeology-05](../archaeology-05-global-state/lesson.md) — el prerrequisito: las globales, la
  caché derivada `cf1`/`cc1`/`cf2`/`cc2` y el mapa de mutaciones.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, la
  taxonomía de fallos; §5, la escala de severidad; §9, el mapeo a la pista A.
- [`../../../docs/archive.md`](../../../docs/archive.md) — el archivo fijado, la etiqueta y el
  commit.

También necesitas:

- un clon local del archivo en la etiqueta `v1.0.0`;
- un compilador de C++20 (`c++`, `g++` o `clang++`);
- `bash`;
- un directorio de trabajo temporal fuera de `original/`.

> **Regla:** el archivo es de solo lectura. Copia el archivo que quieras cambiar a un directorio
> temporal; nunca edites `original/`. La línea base de esta lección también es evidencia: cópiala
> antes de editarla.

## El recorrido de 60 minutos

| Min | Paso | Qué haces | Evidencia |
|---|---|---|---|
| 0–10 | Predecir | Estimas los sitios de índice, las variables acopladas y el coste de redimensionar | Tabla de predicción |
| 10–25 | Inspeccionar | Lees los anclajes del archivo y ejecutas la línea base | Notas de anclajes y `match=yes` |
| 25–40 | Refactorizar | Introducir `Knight` y `Board` en una copia; actualizar el llamador | Archivo temporal y `golden.txt` |
| 40–45 | Comparar | Ejecutar el refactor contra el resumen congelado | La misma línea, `match=yes` |
| 45–55 | Explicar | Cotizar la fuga y nombrar la clase | Prueba de cambio y tabla de costes |
| 55–60 | Registrar | Completar `EVIDENCE-es.md`; confirmar que el archivo está limpio | Entrada de portafolio |

Si contar los sitios de `vectab[` tarda diez minutos, ese conteo es
la lección.

### La comprobación de ingeniería en esta lección

La pista B usa el mismo hábito que la pista A, con otro verbo: el comportamiento ya existe, así que
lo **congelas** en lugar de especificarlo desde cero.

| Paso | Dónde ocurre | Qué produce |
|---|---|---|
| **Predecir** | Paso 1 | Conteos falsables de sitios de índice y coste de redimensión. |
| **Inspeccionar** | Paso 2 | Los anclajes del archivo y la ejecución de la línea base. |
| **Refactorizar** | Paso 3 | Una copia temporal con `Knight` y `Board`. |
| **Comparar** | Paso 4 | El mismo resumen desde ambas versiones. |
| **Explicar** | Paso 5 | La prueba de cambio y el coste de la fuga. |
| **Registrar** | Paso 6 | `EVIDENCE-es.md` con conteos, diff y resultado. |

## Paso 1 — Predecir (10 min)

Antes de abrir la fuente, responde estas cuatro preguntas por escrito. El objetivo es comprometerte
con un número del que puedas equivocarte.

1. ¿Cuántas veces calcula `parallel_arrays.cpp` un índice del tablero a mano?
2. ¿Cuántas variables deben cambiar juntas para mover un caballo?
3. Si el tablero pasa a 10×10, ¿cuántas líneas cambian en cada versión?
4. ¿Qué esconde `Knight` y qué no esconde?

Una predicción útil es falsable. «Los arreglos paralelos son malos» no lo es. «El llamador calcula
el índice tres veces, mover un caballo toca dos enteros, y un tablero de 10×10 toca cuatro líneas en
la versión paralela y dos dentro de `Board`» sí lo es.

### Una predicción trabajada

Antes de leer, yo escribiría:

- sitios de índice: tres, uno por marca;
- variables acopladas: dos por caballo, fila y columna;
- coste de redimensión: el tamaño del arreglo y tres expresiones de índice en la versión paralela;
  el tamaño del arreglo y una fórmula dentro de `Board` en la versión con registro;
- qué esconde `Knight`: que una posición son dos enteros; qué no esconde: el rango válido, el ancho
  del tablero ni el almacenamiento.

Cada afirmación se puede comprobar. Ese es el estándar. Si tu predicción dice «varias», todavía no
has predicho nada; la fuente ya te dijo que hay cuatro variables de coordenadas.

## Paso 2 — Inspeccionar (15 min)

### Los anclajes del archivo

Abre `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp` en la etiqueta
`v1.0.0`. El archivo tiene 930 líneas. Usa esta tabla para navegar; los números de línea son del
archivo fijado.

| Línea | Qué hay allí |
|---:|---|
| 17 | `int vectab[64],vecjugadas[8];` — el tablero plano. |
| 18 | `int cab1,cab2,jugada=1,orden,ganador=1;` — índices planos de los caballos. |
| 29 | `int cf1,cc1,cf2,cc2,cab;` — la caché fila/columna y el selector de turno. |
| 56–66 | `inicializacion` — limpia el tablero. |
| 61–63 | `for (i=0;i<=64;i++) vectab[i]=0;` — el off-by-one de `archaeology-04`. |
| 68–101 | `cabubicacion` — ubicación aleatoria. |
| 81–82 | `cab1=rand()%64; cab2=rand()%64;` — aquí nacen los índices planos. |
| 90 | `conversionvm();` — la caché fila/columna se deriva una vez. |
| 104–112 | `conversionvm` — escribe `cf1`, `cc1`, `cf2`, `cc2`. |
| 226–232 | `conversionmv` — `ccab=cf*8+cc`. |
| 234 | `movcab(int &cf,int &cc)` — muta fila/columna, devuelve un índice plano. |
| 247 | `ccab=conversionmv(cf-1,cc+2);` — una celda destino calculada a mano. |
| 252 | `vectab[ccab]==0||vectab[ccab]==vectab[cab2]` — la lógica de movimiento lee el tablero plano. |
| 269 | `vectab[ccab]=jugada;` — el tablero guarda el número de jugada. |
| 726–751 | `ordenjuego` — escribe `vectab[cab1]` y `vectab[cab2]`. |
| 766 | `cab1=movcab(cf1,cc1);` — dos representaciones actualizadas en una línea. |
| 791 | `cab2=movcab(cf2,cc2);` — el mismo patrón para el otro caballo. |
| 821 | `cab2=movcab(cf2,cc2);` — repetido bajo el otro orden de turno. |
| 846 | `cab1=movcab(cf1,cc1);` — repetido otra vez. |
| 870–930 | `impresion` — recorre `vectab[k]` y compara con `vectab[cab1]`. |

Vale la pena tomar cuatro conteos:

| Medida | Conteo |
|---|---:|
| Líneas que contienen `vectab[` | 80 |
| Líneas que contienen `conversionmv(` | 26 |
| Llamadas a `conversionmv` (sin prototipo ni definición) | 24 |
| Sitios de llamada a `movcab` | 4 |

Los conteos son menciones, no decisiones distintas. Te dicen dónde se expone la representación. Lee
los sitios para ver qué asume cada uno.

### La doble representación en una línea

La línea 766 es toda la lección:

```cpp
cab1=movcab(cf1,cc1);
```

- `cf1` y `cc1` se pasan por referencia; la función llamada las muta.
- `cab1` recibe el valor de retorno, que es el nuevo índice plano.
- El llamador ahora tiene dos copias de la misma posición, actualizadas por dos mecanismos.

Si todos los movimientos se rechazan, `cf` y `cc` conservan la posición anterior, pero la función
devuelve igualmente el último destino `ccab` que calculó. El llamador guarda ese valor en `cab1`. La
fuente no enuncia la postcondición. Eso es lo que eliminan un registro y un dueño: un valor, una
actualización.

### La línea base

La línea base son dos programas pequeños, no el archivo. Léelos en este orden.

`code/parallel_arrays.cpp` tiene 47 líneas:

| Parte | Líneas | Qué es |
|---|---:|---|
| `Summary` | 4–11 | Cinco enteros sueltos para la salida. |
| `run_scenario` | 13–38 | El llamador. |
| Variables de caballo | 15–18 | Cuatro enteros de coordenadas. |
| Tablero | 19 | `std::array<bool, 64>`. |
| Sitios de marca | 21, 24, 27 | Tres índices calculados a mano. |
| Bucle de conteo | 29–36 | Recorre el arreglo. |
| `main` | 40–47 | Imprime el resumen. |

`code/record.cpp` tiene 67 líneas:

| Parte | Líneas | Qué es |
|---|---:|---|
| `Knight` | 4–8 | Una fila y una columna. |
| `Board` | 10–33 | El dueño. |
| `mark` | 13–16 | Convierte un `Knight` en índice, dentro de la clase. |
| `visited_count` | 18–29 | Recorre el arreglo privado. |
| Arreglo privado | 31–32 | `std::array<bool, 64>`. |
| `Summary` | 35–42 | La misma forma de salida. |
| `run_scenario` | 44–58 | El llamador, ahora con forma de registro. |
| `main` | 60–67 | Imprime el mismo resumen. |

### Ejecutar la línea base

Desde el directorio de esta lección:

```bash
cd archaeology/lessons/archaeology-07-representation-leakage
./run.sh
```

Salida esperada:

```text
parallel=player=2,3 machine=3,4 visited=3
record=player=2,3 machine=3,4 visited=3
match=yes
```

El script compila ambos archivos con:

```text
-std=c++20 -Wall -Wextra -Werror -O2
```

`-Werror` importa: la línea base está limpia de advertencias bajo las banderas que trae. Si tu
compilador la rechaza, registra el compilador y el diagnóstico; no silencies la advertencia para que
la ejecución pase.

### Leer la línea base con honestidad

La línea base es un banco de pruebas, no el juego. Aísla dos cambios:

1. cuatro enteros de coordenadas sueltos pasan a dos registros `Knight`;
2. el llamador deja de calcular índices del tablero; `Board::mark` lo hace.

No reproduce:

- el índice plano `cab1` y la caché fila/columna `cf1`/`cc1` guardados a la vez en el archivo;
- la carga del tablero del archivo: `vectab` guarda números de jugada, la línea base guarda `bool`;
- la ubicación aleatoria, la selección de turno, la captura, la impresión ni el bucle del juego;
- el off-by-one `i <= 64`, las globales ni el switch de ocho casos.

Dilo en tu registro. Una ejecución que pasa es prueba sobre el banco, no sobre el archivo.

### Comprobar tu predicción

La versión paralela calcula un índice en las líneas 21, 24 y 27: tres sitios. Mover un caballo toca
dos enteros. Un tablero de 10×10 cambia cuatro líneas en la versión paralela (el tamaño del arreglo
y tres fórmulas de índice) y dos líneas dentro de `Board` (el tamaño del arreglo y la fórmula). El
llamador no cambia en la versión con registro. Ese es el argumento de diseño en una medición.

## Paso 3 — Refactorizar (15 min)

### Congelar la salida primero

Antes de cambiar nada, captura la salida de la versión paralela:

```bash
cd archaeology/lessons/archaeology-07-representation-leakage
mkdir -p build/scratch
./build/parallel-arrays > build/scratch/golden.txt
cat build/scratch/golden.txt
```

Esperado:

```text
player=2,3 machine=3,4 visited=3
```

Esa línea es el oráculo. Si refactorizas primero y luego comparas contra un resumen que escribiste a
partir del código nuevo, la comparación es circular.

### Copiar la línea base

```bash
cp code/parallel_arrays.cpp build/scratch/record_attempt.cpp
```

De aquí en adelante, edita `build/scratch/record_attempt.cpp`. No edites
`code/parallel_arrays.cpp` ni `code/record.cpp`; son las referencias de antes y después que
ejecutará la próxima persona que estudie la lección.

### Introducir el registro

Reemplaza los cuatro enteros de coordenadas por dos valores:

```cpp
struct Knight
{
    int row;
    int column;
};
```

El escenario empieza así:

```cpp
Knight player{1, 1};
Knight machine{8, 8};
```

Mover un caballo pasa a ser una asignación en lugar de dos:

```cpp
player = Knight{2, 3};
```

El compilador ahora ve un valor. No puedes actualizar la fila y olvidar la columna.

### Darle un dueño al tablero

Reemplaza el arreglo público por una clase:

```cpp
class Board
{
public:
    void mark(const Knight &knight)
    {
        visited[(knight.row - 1) * 8 + (knight.column - 1)] = true;
    }

    int visited_count() const
    {
        int count = 0;
        for (bool cell : visited)
        {
            if (cell)
            {
                count += 1;
            }
        }
        return count;
    }

private:
    std::array<bool, 64> visited = {};
};
```

La fórmula sigue ahí. Ahora está en un solo lugar, detrás de una operación. Esa es la diferencia
entre esconder una representación y borrarla. La representación sigue existiendo; el llamador ya no
depende de ella.

### Actualizar el llamador

El llamador es `run_scenario`. En esta línea base hay exactamente uno. El archivo tiene varios
lugares que tocan la representación:
`ordenjuego`, `impresion`, `movcab` y los cuatro sitios de llamada a `movcab` en `impresionorden`.

| Sitio en `parallel_arrays.cpp` | Reemplazo en `record.cpp` |
|---|---|
| Cuatro declaraciones, líneas 15–18 | Dos valores `Knight` y un `Board`, líneas 46–48 |
| `visited[(playerRow-1)*8+(playerColumn-1)] = true;` línea 21 | `board.mark(player);` línea 50 |
| `playerRow = 2; playerColumn = 3;` líneas 22–23 | `player = Knight{2, 3};` línea 51 |
| `visited[(playerRow-1)*8+(playerColumn-1)] = true;` línea 24 | `board.mark(player);` línea 52 |
| `machineRow = 3; machineColumn = 4;` líneas 25–26 | `machine = Knight{3, 4};` línea 53 |
| `visited[(machineRow-1)*8+(machineColumn-1)] = true;` línea 27 | `board.mark(machine);` línea 54 |
| Bucle de conteo en línea, líneas 29–36 | `board.visited_count()` línea 57 |
| `Summary{playerRow, playerColumn, machineRow, machineColumn, visitedCount}` línea 37 | `Summary{player.row, player.column, machine.row, machine.column, board.visited_count()}` líneas 56–57 |

Esa es la evidencia de «llamadores actualizados»: tres cálculos de índice y una construcción de
resumen se convierten en tres llamadas de dominio y una llamada a método. Cuenta las líneas que
tocaste; no describas el cambio como «más limpio» sin un número.

### Compilar y ejecutar

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 \
    build/scratch/record_attempt.cpp -o build/scratch/record_attempt
./build/scratch/record_attempt
```

Esperado:

```text
player=2,3 machine=3,4 visited=3
```

Compara con el archivo congelado:

```bash
diff <(./build/scratch/record_attempt) build/scratch/golden.txt
```

Sin salida significa que las líneas coinciden. Luego compara con la versión con registro que viene
incluida:

```bash
diff <(./build/scratch/record_attempt) <(./build/record)
```

Sin salida significa que tu refactor y `code/record.cpp` coinciden en este escenario.

### La prueba de cambio opcional

En una segunda copia temporal, cambia el tablero a 10×10 y cuenta las líneas tocadas.

| Cambio | Versión paralela | Versión con registro |
|---|---|---|
| Tablero 8×8 → 10×10 | Tamaño del arreglo más tres fórmulas de índice | Tamaño del arreglo más una fórmula dentro de `Board` |
| Agregar un tercer caballo | Dos enteros nuevos más una fórmula de índice por marca | Un `Knight` nuevo más una llamada `mark` por marca |
| Cambiar la fórmula interna del índice | Tres fórmulas en los puntos de llamada | Una fórmula dentro de `Board` |
| Agregar una bandera `captured` a un caballo | Un entero nuevo, pasado aparte | Un campo nuevo en `Knight` |

No afirmes un conteo de líneas que no mediste. La tabla es una predicción hasta que la ejecutes.

## Paso 4 — Comparar (5 min)

### Qué dice la ejecución

```text
parallel=player=2,3 machine=3,4 visited=3
record=player=2,3 machine=3,4 visited=3
match=yes
```

Ambos programas ubican al jugador en `(2,3)`, a la máquina en `(3,4)` y cuentan tres celdas
visitadas. La línea `match=yes` la produce `run.sh` solo cuando las dos cadenas de salida son
iguales.

### Qué prueba la igualdad

- La versión con registro realiza las mismas tres marcas que la versión paralela en este escenario.
- `Knight` lleva los mismos valores de fila y columna por la misma secuencia.
- `Board::mark` calcula las mismas celdas que las expresiones de índice escritas a mano.
- `visited_count()` devuelve el mismo conteo que el bucle en línea.
- El llamador ya no necesita el ancho, la fórmula del índice ni el almacenamiento del tablero para
  expresar el escenario.

### Qué no prueba la igualdad

| Afirmación | ¿Probada? | Por qué |
|---|---|---|
| La versión con registro es equivalente para toda posición. | No. | Un escenario, tres marcas. |
| El comportamiento del archivo se preserva. | No. | La línea base no es el archivo. |
| `Board::mark` rechaza coordenadas fuera de rango. | No. | No tiene comprobación de límites. |
| La doble representación `cab1`/`cf1` del archivo se reproduce. | No. | La línea base guarda una sola representación. |
| La carga del tablero del archivo se preserva. | No. | Aquí `bool`, allí número de jugada. |
| La versión con registro es más rápida. | No. | No se hizo ninguna medición. |
| La versión con registro es más corta. | No. | 67 líneas contra 47. |

### La dirección del oráculo

La versión paralela es el «antes»; la versión con registro es el «después». Ambas se escribieron
para esta lección. Ninguna es el programa de 2008. El archivo es el testigo histórico; la línea base
es el banco de pruebas.

Esto importa porque una comparación entre dos programas que escribiste puede coincidir por la razón
equivocada. Congela primero la salida del antes, luego refactoriza. No generes la línea dorada desde
la versión con registro y llames prueba a la coincidencia.

## Paso 5 — Explicar (10 min)

### Cotizar la fuga

| Coste | Evidencia en el archivo |
|---|---|
| El ancho del tablero se copia en cada expresión de índice. | 80 líneas contienen `vectab[`; `conversionmv` fija `8`. |
| Dos copias de una posición deben mantenerse sincronizadas. | `cab1` y `cf1`/`cc1`; `conversionvm` escribe una a partir de la otra. |
| El mecanismo de actualización está partido. | `cab1=movcab(cf1,cc1)` muta por referencia y devuelve un índice plano. |
| Un llamador debe conocer el almacenamiento para leer una celda. | `impresion` recorre `vectab[k]` y compara con `vectab[cab1]`. |
| Un cambio de tamaño del tablero toca cada llamador. | `conversionmv(cf,cc)` se llama en 24 sitios. |
| Un lector nuevo debe aprender primero la convención. | `8`, `64`, filas y columnas de base cero, impresión de base uno (`cf1+1`). |

### La clase, no la instancia

La instancia es «cuatro enteros sueltos y un arreglo plano». La clase es «un valor cuya
representación es pública». Busca la clase en el resto del archivo:

- `ordenjuego` escribe `vectab[cab1]` y `vectab[cab2]` directamente — la ubicación conoce el
  almacenamiento;
- `impresion` compara `vectab[k]` con `vectab[cab1]` — la impresión conoce el almacenamiento;
- `movcab(int &cf,int &cc)` devuelve un índice plano mientras muta fila y columna — un contrato, dos
  representaciones;
- `struct stablero { int mvectab; }` en variant-03 y variant-04 es un envoltorio transparente
  alrededor de un entero — un tipo que no esconde nada (`failure-010`, la próxima lección);
- los números mágicos `64` y `8` aparecen en la inicialización, la conversión y las comprobaciones
  de límites.

El arreglo sistémico no es «reemplazar estas cuatro variables». Es «darle un dueño al valor y
exponer operaciones, no almacenamiento». Ese arreglo elimina la clase.

### Dos arreglos, dos tamaños

**Arreglo mínimo — el registro.** Introduce `Knight` y deja el arreglo plano público:

```cpp
struct Knight
{
    int row;
    int column;
};
```

El llamador todavía calcula `(knight.row - 1) * 8 + (knight.column - 1)`, pero las dos coordenadas
ya no pueden separarse. Es el cambio más pequeño que elimina la mitad del defecto: las variables
paralelas.

**Arreglo estructural — el dueño.** Introduce `Board` con un arreglo privado y `mark(Knight)`. El
llamador deja de indexar. Esta es la versión `record.cpp` de la línea base. Elimina la otra mitad:
la fuga.

**Arreglo más fuerte — el invariante.** Dale a `Knight` una comprobación de validez y a
`Board::mark` una comprobación de límites, para que una posición inválida se rechace en lugar de
escribirse fuera de rango. Eso es `construction-10` (invariantes) y `construction-11`
(encapsulación), no esta lección.

Vínculos con la pista A: `construction-04` (registros) para `Knight`, `construction-05` (arreglo de
registros) para las celdas del tablero, `construction-09` (tipo abstracto de datos) para la
independencia de representación, y `construction-11` (encapsulación) para el arreglo privado.

No cites `construction-09` como si la línea base ya lo alcanzara. `Board` esconde el arreglo, pero
el tipo sigue siendo una clase con una interfaz estrecha; un tipo abstracto de datos completo
también fija el invariante y el ciclo de vida. Di a qué peldaño llegaste.

### El intercambio, dicho con justicia

El registro cuesta tipos nuevos, sintaxis de construcción y más líneas. El dueño cuesta una interfaz
y un lugar para la comprobación de límites. La versión paralela cuesta cada cambio futuro del
tablero.

En un programa de una sola ejecución y sin pruebas, cuatro enteros y un arreglo plano son fáciles de
leer. En un programa mantenido con un segundo llamador, el registro y el dueño son más baratos. La
lección te pide elegir con la prueba de cambio, no con el gusto.

### Qué no arregla el refactor

- `Board` sigue guardando `std::array<bool, 64>`; la representación se esconde, no se elimina.
- `Board::mark` no comprueba límites. `mark(Knight{0, 0})` calcula el índice `-9` y escribe fuera
  de rango. El `movcab` del archivo comprueba límites antes de convertir; la línea base no.
- La convención de base uno sigue dentro de `Board`.
- `Summary` todavía tiene cinco campos paralelos. Es un DTO de salida, no el modelo del dominio; no
  confundas los dos.
- La doble representación `cab1`/`cf1` del archivo no se reproduce; la línea base guarda una sola.
- Las globales, la impresión, la selección de turno, la ubicación aleatoria, la captura y el bucle
  del juego están ausentes.
- El off-by-one `i <= 64` sigue siendo una ficha de corrección aparte de `archaeology-04`.

## Paso 6 — Registrar (5 min)

Completa [`EVIDENCE-es.md`](EVIDENCE-es.md):

- configuración y fijación del archivo;
- la tabla de predicción, escrita antes de inspeccionar;
- los anclajes del archivo y los cuatro conteos;
- la ejecución de la línea base y su salida exacta;
- el `golden.txt` congelado y el diff del refactor;
- la tabla de llamadores: qué cambió en cada sitio;
- el resultado de la comparación y la dirección del oráculo;
- la frontera de evidencia: qué prueba la ejecución y qué no;
- la comprobación de integridad del archivo.

Una frase como «el registro es más limpio» es una opinión. «Tres sitios de índice, 80 menciones de
`vectab[`, 24 llamadas a `conversionmv`, un llamador actualizado, 67 líneas contra 47, `match=yes`»
es un diagnóstico.

## El original en detalle

Hechos que puedes citar, con números de línea de `variant-01` en la etiqueta `v1.0.0`:

| Ubicación | Qué hay allí |
|---:|---|
| 17 | `int vectab[64],vecjugadas[8];` — tablero plano y orden de jugadas. |
| 18 | `int cab1,cab2,jugada=1,orden,ganador=1;` — índices planos de los caballos. |
| 29 | `int cf1,cc1,cf2,cc2,cab;` — caché fila/columna y selector de turno. |
| 61–63 | `for (i=0;i<=64;i++) vectab[i]=0;` — el off-by-one. |
| 81–82 | `cab1=rand()%64; cab2=rand()%64;` — se crean los índices planos. |
| 90 | `conversionvm();` — la caché se deriva una vez. |
| 106–109 | `cf1=cab1/8; cc1=cab1%8; cf2=cab2/8; cc2=cab2%8;` |
| 229–230 | `ccab=cf*8+cc; return ccab;` — la conversión inversa. |
| 234 | `int movcab(int &cf,int &cc)` — muta fila/columna, devuelve plano. |
| 247 | `ccab=conversionmv(cf-1,cc+2);` — una celda destino. |
| 252 | `vectab[ccab]==0||vectab[ccab]==vectab[cab2]` — la lógica de movimiento lee el almacenamiento. |
| 269 | `vectab[ccab]=jugada;` — el tablero guarda el número de jugada. |
| 732, 734 | `vectab[cab1]=jugada; vectab[cab2]=jugada;` — la ubicación escribe el almacenamiento. |
| 766 | `cab1=movcab(cf1,cc1);` — dos representaciones, una línea. |
| 791, 821, 846 | La misma llamada bajo los otros órdenes de turno. |
| 884 | `vectab[k]!=vectab[cab1]&&vectab[k]!=vectab[cab2]` — la impresión lee el almacenamiento. |
| 878–920 | `impresion` recorre el arreglo plano con `k++`. |

Los conteos de esta lección son menciones de línea, no decisiones semánticas distintas. Una línea
puede contener dos lecturas de `vectab[`, y una llamada a `conversionmv` puede calcular la misma
celda dos veces. Mide tu propia copia si necesitas números exactos.

## Qué prueba y qué no prueba el refactor

| Afirmación | ¿Respaldada? | Por qué |
|---|---|---|
| La versión con registro produce el mismo resumen en este escenario. | Sí | La ejecución imprimió `match=yes`. |
| La versión con registro es equivalente para todo tablero y posición. | No | Un escenario, tres marcas. |
| El llamador ya no calcula el índice del tablero. | Sí | El índice está dentro de `Board::mark`. |
| El comportamiento del archivo se preserva. | No | La línea base no es el archivo. |
| `Board` esconde su almacenamiento de los llamadores. | Sí | El arreglo es privado. |
| `Board` elimina la representación plana. | No | Sigue guardando `std::array<bool, 64>`. |
| `Board::mark` valida las coordenadas. | No | No hay comprobación de límites. |
| La versión con registro es más corta. | No | 67 líneas contra 47. |
| La versión con registro es más rápida. | No medido | No lo afirmes. |
| La prueba de cambio es más barata con el registro. | Por conteo de líneas | Cuenta tú mismo las líneas tocadas. |

Esta tabla es la comprobación de honestidad. Una entrada de evidencia que dice «el refactor es
correcto» sin nombrar la muestra está exagerando. Di «para este escenario» cuando eso es lo que
ejecutaste.

## Autocomprobación (binaria)

- [ ] Escribí las cuatro predicciones antes de abrir la fuente.
- [ ] Encontré las declaraciones en las líneas 17, 18 y 29.
- [ ] Leí `conversionvm` y `conversionmv` y expliqué las dos representaciones.
- [ ] Conté las menciones de `vectab[`: 80.
- [ ] Conté las menciones de `conversionmv(`: 26 líneas, 24 llamadas.
- [ ] Encontré los cuatro sitios de llamada a `movcab`: 766, 791, 821, 846.
- [ ] Ejecuté `./run.sh` y vi `match=yes`.
- [ ] Registré el compilador y las banderas.
- [ ] Congelé `golden.txt` antes de refactorizar.
- [ ] Introduje `Knight` en una copia temporal.
- [ ] Introduje `Board` con un arreglo privado y `mark`.
- [ ] Actualicé el llamador y conté los sitios.
- [ ] El refactor temporal imprimió el mismo resumen.
- [ ] Comparé mi refactor con `code/record.cpp`.
- [ ] Enuncié la frontera de aislamiento: solo el cambio de registro/tablero, no el archivo.
- [ ] Noté que `Board` sigue guardando `std::array<bool, 64>`.
- [ ] Noté que `Board::mark` no comprueba límites.
- [ ] Vinculé el arreglo con `construction-04`, `construction-05`, `construction-09` y
      `construction-11`.
- [ ] No afirmé una mejora de rendimiento.
- [ ] El archivo está sin modificar (`git status` en el archivo está limpio).

Si una casilla no está marcada, la casilla sin marcar nombra el siguiente paso. No hay penalización
por repetir uno.

## Misconcepciones que esta lección refuta

| Concepto erróneo | Por qué está mal | Qué hacer en su lugar |
|---|---|---|
| «Un registro es solo una bolsa de variables.» | Un registro agrupa campos para que se muevan como un valor; la bolsa no. | Nombra el invariante que impone la agrupación. |
| «Los arreglos paralelos están bien si tengo cuidado.» | El cuidado no es una comprobación del compilador; dos copias pueden desincronizarse. | Dale un tipo al valor y un dueño al almacenamiento. |
| «La versión con registro es más corta.» | Tiene 67 líneas contra 47. | Cuenta el coste del cambio, no las líneas. |
| «`Board` elimina el arreglo plano.» | Sigue guardando `std::array<bool, 64>`. | Di «esconde», no «elimina». |
| «`match=yes` prueba que el archivo sigue funcionando.» | La línea base no es el archivo. | Enuncia la frontera de aislamiento. |
| «`match=yes` prueba equivalencia para toda posición.» | Un escenario, tres marcas. | Enuncia la muestra. |
| «La versión con registro es más rápida.» | No se hizo ninguna medición. | Argumenta con líneas tocadas, no con ciclos. |
| «`Knight` valida la posición.» | `Knight{99, 99}` es construible. | Agrega un invariante o un constructor privado. |
| «`cab1` y `cf1` del archivo son independientes.» | Son dos copias de una posición. | Marca la caché derivada y sus puntos de sincronización. |
| «Una clase con un arreglo privado es un tipo abstracto de datos completo.» | Esconder el almacenamiento es una propiedad; los invariantes y el ciclo de vida son otras. | Di a qué peldaño llegaste. |
| «El arreglo debe aplicarse a `original/`.» | El archivo es evidencia y debe quedar de solo lectura. | Refactoriza en una copia temporal. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Representación | Cómo se guarda un valor: índice plano, fila/columna, arreglo, bitset. |
| Fuga de representación | Un llamador debe conocer el almacenamiento para usar el valor. |
| Arreglos paralelos | Variables o arreglos separados que deben sincronizarse a mano. |
| Registro | Un tipo que agrupa campos en un valor. |
| Dueño | El tipo responsable de un estado y de su invariante. |
| Encapsulación | Esconder el estado detrás de operaciones. |
| Tipo abstracto de datos | Un tipo definido por sus operaciones, independiente de su representación. |
| Invariante | Una propiedad que siempre debe cumplirse, por ejemplo `1 <= row <= 8`. |
| Prueba de cambio | Un cambio concreto cuyos lugares tocados miden el diseño. |
| Caché derivada | Una segunda copia de un valor, mantenida a mano. |
| Frontera de aislamiento | La línea entre lo que reproduce la línea base y lo que omite. |
| Fallo de diseño | El programa funciona, pero la estructura encarece el cambio. |

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
