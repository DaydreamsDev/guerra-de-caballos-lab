# archaeology-10 — Polimorfismo ausente

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** polimorfismo · **Prerrequisito:** archaeology-09 ·
> **Tiempo estimado:** 90 minutos
> **Lectura:** ~22 minutos
> **Rol de la lección:** opcional · puedes omitirla

## Por qué existe esta lección

`archaeology-06` eliminó un `switch` sobre el número de movimiento y lo reemplazó por una tabla. El
programa quedó más pequeño y la regla pasó a los datos. Esa fue la reparación correcta para la
duplicación, y dejó una pregunta abierta: el bucle todavía decide **qué pieza** se mueve, leyendo un
entero.

Esta lección trata esa segunda decisión. No *qué delta*, sino *de quién son los deltas*. La
diferencia importa porque las dos decisiones fallan de maneras distintas y se reparan con mecanismos
distintos.

En el archivo, la pieza no es un tipo. Son dos pares de coordenadas globales y un entero:

```text
18:  int  cab1,cab2,jugada=1,orden,ganador=1;
29:  int cf1,cc1,cf2,cc2,cab;
```

`cab` vale `1` o `2`. Se asigna antes de cada movimiento, y `movcab` lo lee para elegir qué
coordenadas y qué celdas visitadas tocar. Hay nueve sitios con `if (cab==1)` en `variant-01`, y diez
en las otras tres variantes. `impresionorden` contiene cuatro bloques de turno que nombran a mano un
caballo, sus coordenadas y su contador.

Añadir una tercera pieza a ese diseño no es un cambio de una línea. Necesitas un tercer par de
coordenadas, un tercer contador, una tercera rama en cada sitio con `cab==1`, un cuarto bloque de
turno y una regla de movimiento para la pieza nueva. El switch sobre el número de movimiento no sabe
que la pieza existe; el bucle la conoce en nueve lugares.

Ese es el defecto que el catálogo llama `failure-011`: **un `switch` sobre el tipo de pieza en lugar
de despacho**. El programa funciona. El coste lo paga la próxima pieza.

El producto de esta lección es un **reemplazo**: el switch sobre el tipo de pieza pasa a despacho
virtual, y una traza congelada de 400 líneas demuestra que el comportamiento no cambió. El registro
es el precio: qué compró el despacho, qué costó y qué no arregló.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar la diferencia entre un tipo estático y un tipo dinámico, y cuál decide una llamada;
- describir qué hace una llamada virtual en tiempo de ejecución y qué le impide hacer al compilador;
- separar un switch sobre el **tipo de pieza** de un switch sobre el **número de movimiento**, y
  decir por qué necesitan reparaciones distintas;
- leer la interfaz `Piece` de la línea base y sus dos implementaciones;
- congelar la salida del switch como oráculo antes de borrar el switch;
- reemplazar el switch por despacho en una copia de trabajo y mantener 400 comparaciones en cero
  discrepancias;
- añadir una tercera pieza en cada camino y contar qué toca el cambio, no solo cuántas líneas añade;
- indicar el coste real en el archivo: nueve sitios con `cab==1`, cuatro bloques de turno y las
  globales;
- nombrar qué no arregla el despacho: la tabla de movimientos, el resto de lo que sabe el bucle y
  las pruebas ausentes;
- mantener el archivo de solo lectura mientras trabajas.

El reemplazo está hecho cuando
otra persona puede reproducir la ejecución.

## El concepto: despacho

### Dos decisiones que se parecen

Ambos son sentencias `switch`. No son el mismo problema.

```cpp
switch (moveNumber) { case 1: return Delta{-1, 2}; /* ... */ }
switch (pieceKind)  { case PieceKind::knight: /* todo el conjunto de reglas */ }
```

| | Switch sobre el número de movimiento | Switch sobre el tipo de pieza |
|---|---|---|
| La pregunta | ¿Qué **dato** aplica a este movimiento? | ¿Qué **conjunto de reglas** aplica a esta pieza? |
| Los casos difieren en | Constantes dentro de una regla. | Algoritmos completos, cada uno con sus constantes. |
| El conjunto es | Cerrado: un caballo tiene ocho movimientos. | Abierto: el juego puede crecer en piezas. |
| La reparación | Una tabla. `archaeology-06` ya lo hizo. | Despacho. Esta lección. |
| Qué queda en el bucle | El índice del movimiento. | Nada sobre la pieza. |

Una tabla de deltas indexada por número de movimiento sigue siendo despacho. Despacha por índice.
Elimina aritmética copiada; no crea una frontera alrededor de una pieza. Puedes tener una tabla de
movimientos perfecta y seguir sin polimorfismo, que es exactamente la situación del archivo.

A la inversa, una jerarquía de clases no elimina la necesidad de un índice de movimiento.
`Knight::move(3, ...)` todavía tiene que saber que el movimiento 3 es `(-2, -1)`. El polimorfismo
responde *quién se mueve*; la tabla responde *a dónde*. Los dos mecanismos se componen. Confundirlos
produce la reparación equivocada: un `Delta table[pieceKind][moveNumber]` hace desaparecer el switch
de tipo, pero el bucle sigue enumerando cada tipo, y una pieza cuyo movimiento dependa del tablero
no se puede expresar en esa tabla.

### Tipo estático, tipo dinámico y la llamada

Un puntero tiene dos tipos. El **tipo estático** es lo que ve el compilador. El **tipo dinámico** es
lo que el objeto realmente es.

```cpp
std::unique_ptr<Piece> knight = std::make_unique<Knight>();
Piece *piece = knight.get();       // tipo estático: Piece*
piece->move(1, Position{0, 0});    // tipo dinámico: Knight
```

El compilador ve `Piece *`. No puede saber que el objeto es un `Knight`, porque el puntero podría
contener cualquier implementación. Así que no emite una llamada a `Knight::move`. Emite una llamada
a través de la tabla de la clase del objeto, y la tabla decide en tiempo de ejecución. Eso es
**ligadura dinámica**, o despacho dinámico.

El mecanismo es lo bastante pequeño para enunciarlo con precisión:

- cada objeto polimórfico guarda un puntero oculto, el **vptr**, a la tabla de su clase;
- cada clase polimórfica tiene una **vtable**, emitida una vez por clase, con las direcciones de sus
  funciones virtuales;
- una llamada virtual carga el vptr, carga la ranura de esa función y llama a través de ella.

`Piece` declara tres funciones virtuales en esta línea base: el destructor, `move` y `role_name`.
`Knight` y `MirrorKnight` aportan dos sobreescrituras cada uno. `= 0` hace puras a `move` y
`role_name`: la interfaz no tiene cuerpo, así que `Piece` no se puede instanciar. `override` es una
comprobación, no decoración: hace que el compilador rechace una función que solo parece una
sobreescritura.

El destructor virtual no es cosmético. Los objetos los posee `std::unique_ptr<Piece>`; borrar a
través de un puntero a la base sin un destructor virtual es comportamiento indefinido. La interfaz
paga esa seguridad con una ranura de la vtable.

### Qué cuesta una llamada virtual

La ligadura dinámica no es gratis, y el coste tiene varias partes:

| Coste | Mecanismo | En esta línea base |
|---|---|---|
| Tamaño del objeto | Un vptr por objeto. | `sizeof(Piece)` es 8 bytes en esta plataforma. |
| Tabla por clase | Una vtable por clase. | Dos vtables, emitidas una vez. |
| La llamada | Cargar vptr, cargar ranura, llamada indirecta. | 400 llamadas en toda la ejecución. |
| Inlining | Bloqueado salvo que el compilador pruebe el tipo. | Las llamadas del bucle siguen indirectas. |
| Predicción | Los destinos indirectos son más difíciles de predecir. | Dos destinos, uno por pieza. |
| Asignación | Un objeto en el montón por pieza. | Dos asignaciones al arrancar. |

Dos cosas mantienen esto honesto.

Primero, el compilador puede **devirtualizar** cuando prueba el tipo dinámico. Un objeto `Knight`
local, una clase `final`, o una llamada que el optimizador puede rastrear hasta un constructor
pueden convertirse en una llamada directa. Eso es una optimización dentro de una función, no un
cambio de diseño. No leas una llamada devirtualizada como evidencia de que la interfaz era
innecesaria.

Segundo, el `switch` tampoco es gratis. Un switch denso sobre un entero suele convertirse en una
tabla de salto, que también es una rama indirecta, más el trabajo de comprobar el enum. Comparar
"llamada virtual" con "coste cero" es incorrecto. Compárala con "switch más una rama por tipo".

A la escala de esta lección, 400 llamadas, ninguno de los dos mecanismos importa. Si quieres afirmar
una diferencia de rendimiento, mídela: 100 millones de llamadas, mismo compilador y banderas, varias
ejecuciones, y una nota de que la diferencia puede ser ruido. Si la diferencia está por debajo del
ruido, la frase honesta es "no medido". La afirmación de esta lección es el coste de cambio, no los
ciclos.

### La prueba de cambio

Una lección de diseño se juzga por un cambio, no por una forma. El cambio aquí es: **añadir una
tercera pieza con su propia regla de movimiento**. Un camello, por ejemplo, con deltas `(1,3)`,
`(3,1)`, `(3,-1)`, `(1,-3)`, `(-1,-3)`, `(-3,-1)`, `(-3,1)`, `(-1,3)`.

En el camino de despacho, eso es una clase nueva y un registro. En el camino con switch, es un valor
nuevo del enum, una rama nueva de 22 líneas dentro de la función que comparten todas las piezas, y
una entrada nueva dondequiera que se liste el tipo. La tabla medida está en el paso 5. El punto no
es que un camino sea más corto — los conteos de líneas son parecidos. El punto es **dónde cae la
edición**.

### Qué contiene realmente el código de 2008

Esta es la parte donde la lección tiene que ser precisa, porque la abreviatura del catálogo no es la
fuente.

El archivo **no tiene un switch sobre el tipo de pieza**. Tiene un switch sobre el número de
movimiento en
`original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp:239`, y tiene el entero
`cab` con ramas `if (cab==1)`. La decisión sobre el tipo de pieza está dispersa, no centralizada.

La línea base de esta lección, `code/polymorphism.cpp`, reconstruye esa decisión como
`delta_with_switch(PieceKind, int)`. Esa reconstrucción es un **banco de pruebas**: aísla la
decisión para poder comparar los dos mecanismos sobre las mismas 400 entradas. No es código de 2008
literal, y `MirrorKnight` es una invención del laboratorio: los dos caballos del archivo se mueven
igual.

Dilo en tu registro. Un diagnóstico que cita la línea base como si fuera el original no es
evidencia.

### La restricción heredada

Un `switch` y un `if` sobre un entero eran razonables bajo las restricciones: una sola unidad de
traducción, sin pruebas, sin control de versiones, un compilador antiguo y dos piezas del mismo
tipo. No había una segunda regla de movimiento a la que despachar. El coste del diseño aparece
cuando llega una tercera pieza.

La lección no es "el código estaba mal". Es "la estructura encareció la siguiente pieza, y una
frontera de despacho elimina ese coste".

## Antes de empezar

Lee estos documentos completos:

- [`README.md`](README.md) — la ficha de la lección, el comando de compilación y el criterio de
  aceptación.
- [archaeology-06](../archaeology-06-duplication-switch/lesson-es.md) — el switch sobre el número de
  movimiento y su tabla. Esta lección asume esa reparación y no la repite.
- [archaeology-09](../archaeology-09-abstract-data-type/README-es.md) — el prerrequisito: un tipo
  opaco, un cambio de representación y el llamador que no cambió.
- [`../../../docs/guide/archaeology-track-es.md`](../../../docs/guide/archaeology-track-es.md) — §3,
  la taxonomía de fallos; §5, la escala de severidad; §9, el mapeo a Track A.
- [`../../../docs/archive-es.md`](../../../docs/archive-es.md) — el archivo fijado, la etiqueta y el
  commit.

También necesitas:

- un clon local del archivo en la etiqueta `v1.0.0`;
- un compilador de C++20 (`c++`, `g++` o `clang++`);
- `bash`;
- un directorio de trabajo temporal fuera de `original/`.

> **Regla:** el archivo es de solo lectura. Copia el fichero que quieras cambiar a un directorio de
> trabajo temporal; nunca edites `original/`. La línea base de esta lección también es evidencia:
> cópiala antes de borrar nada.

## El camino de 90 minutos

| Min | Paso | Qué haces | Evidencia |
|---|---|---|---|
| 0–10 | Predecir | Estimar el tamaño de la interfaz, el coste de la llamada y la edición de la tercera pieza | Tabla de predicciones |
| 10–30 | Inspeccionar | Leer la interfaz, el switch y los anclajes del archivo; ejecutar la línea base | Notas de anclajes y `comparisons=400 mismatches=0` |
| 30–55 | Despachar | Congelar el oráculo, borrar el switch, conservar las clases | Copia de trabajo y `golden.txt` |
| 55–65 | Comparar | Ejecutar la versión con despacho contra la traza congelada | 400 comparaciones, 0 discrepancias |
| 65–80 | Explicar | Preciar la llamada virtual, hacer la prueba de cambio, separar los dos switches | Tabla de costes y prueba de cambio |
| 80–90 | Registrar | Rellenar `EVIDENCE-es.md`; confirmar que el archivo está limpio | Entrada de portafolio |

Si la explicación de la vtable toma veinte minutos, esa explicación
es la lección.

### La comprobación de ingeniería en esta lección

Track B usa el mismo hábito que Track A, con otro verbo: el comportamiento ya existe, así que lo
**congelas** en lugar de especificarlo desde cero.

| Paso | Dónde ocurre | Qué produce |
|---|---|---|
| **Predecir** | Paso 1 | Una estimación falsable del tamaño de la interfaz, el coste de la llamada y el coste de edición. |
| **Inspeccionar** | Paso 2 | La interfaz, los dos switches, los anclajes del archivo y la ejecución de la línea base. |
| **Despachar** | Paso 3 | Una copia de trabajo con clases y sin switch. |
| **Comparar** | Paso 4 | 400 comparaciones contra el oráculo congelado, 0 discrepancias. |
| **Explicar** | Paso 5 | La prueba de cambio, el coste de la llamada y la distinción entre los dos switches. |
| **Registrar** | Paso 6 | `EVIDENCE-es.md` con conteos, diff y resultado. |

## Paso 1 — Predecir (10 min)

Antes de abrir la fuente, responde por escrito estas cinco preguntas. El objetivo es comprometerte
con números sobre los que puedas equivocarte.

1. ¿Cuántas funciones virtuales declara la interfaz `Piece`?
2. ¿Cuántas líneas tiene `delta_with_switch` y cuántas ramas contiene?
3. ¿Qué cuesta una llamada virtual en términos de máquina, y cuántas llamadas hace la ejecución?
4. Si añades una tercera pieza, ¿cuántas declaraciones existentes deben cambiar en cada camino?
5. ¿Cuál de los dos switches — número de movimiento o tipo de pieza — contiene realmente el archivo?

Una predicción útil es falsable. "Las llamadas virtuales son lentas" no lo es. "Una llamada
indirecta a través de una ranura de la vtable, 400 llamadas por ejecución, y el coste es irrelevante
a esta escala" sí lo es.

### Una predicción trabajada

Antes de leer, yo escribiría:

- interfaz: tres declaraciones virtuales — destructor, `move`, `role_name`;
- switch: unas 50 líneas, dos switches internos de ocho casos más un `default` cada uno;
- llamada: una carga del vptr y una llamada indirecta, 400 llamadas, así que el coste es invisible
  al lado de `printf`;
- tercera pieza: una clase nueva en el camino de despacho, un valor de enum y una rama larga en el
  camino con switch;
- archivo: el switch es sobre el número de movimiento; la pieza es un entero llamado `cab`.

Cada afirmación se puede comprobar. Ese es el estándar. Si tu predicción dice "el polimorfismo es
mejor", no has predicho nada todavía; eso es una conclusión, no una estimación.

## Paso 2 — Inspeccionar (20 min)

### Leer la interfaz

```bash
cd archaeology/lessons/archaeology-10-polymorphism
grep -n 'virtual\|override\|class ' code/polymorphism.cpp
```

La línea base tiene 163 líneas. Sus partes:

| Parte | Líneas | Qué es |
|---|---:|---|
| `Position`, `Delta` | 5–17 | Una fila y una columna; los dos enteros que varían. |
| `PieceKind` | 19–23 | El enum sobre el que despacha el switch. |
| `knightDeltas`, `mirrorDeltas` | 25–33 | Los dos conjuntos de reglas como datos. |
| `Piece` | 35–41 | Tres declaraciones virtuales, dos de ellas puras. |
| `Knight`, `MirrorKnight` | 43–71 | Dos implementaciones, cada una con `move` y `role_name`. |
| `delta_with_switch` | 73–123 | El oráculo: un switch sobre el tipo y otro sobre el movimiento. |
| `move_with_switch` | 125–129 | Aplica el delta del switch a una posición. |
| `main` | 131–163 | 2 piezas × 8 movimientos × 25 posiciones = 400 comparaciones. |

Lee `Piece` dos veces. La interfaz responde exactamente dos preguntas: *¿a dónde vas desde aquí en
el movimiento n?* y *¿cómo te llamas?* Eso basta para esta lección y no basta para un juego
completo. El límite es el tema del paso 5.

Fíjate en lo que la interfaz **no** contiene: posición, captura, celdas visitadas, orden de turno.
La línea base no tiene tablero. Es un banco de pruebas para una decisión.

### Leer el switch

`delta_with_switch` tiene 51 líneas para dieciséis enteros. El switch externo selecciona el conjunto
de reglas; cada switch interno selecciona el movimiento. Los switches internos terminan en
`default: return Delta{0, 0};`.

Ese `default` merece una pausa. Con `-Wall -Wextra -Werror`, el compilador avisa cuando un `switch`
sobre un enum omite un caso — pero solo cuando el switch no tiene `default`. El `default` interno
suprime el aviso para el error más probable de este código: añadir una pieza y olvidar uno de sus
ocho movimientos. Lo verifiqué: una rama `Camel` con siete casos compila limpia y devuelve `(0, 0)`
para el octavo. La pieza, en silencio, no se mueve en ese movimiento.

El camino de despacho tiene una tabla con el mismo peligro: un `std::array<Delta, 8>` con siete
inicializadores rellena el octavo con ceros, también en silencio. Tenlo en cuenta cuando alguien
afirme que el compilador te protegerá. Protege el switch de tipo, no los datos dentro de un conjunto
de reglas.

### Leer los anclajes del archivo

En el archivo fijado en la etiqueta `v1.0.0`:

| Anclaje | `variant-01` | Qué muestra |
|---|---:|---|
| `switch(vecjugadas[i])` | 239 | Despacho sobre el número de movimiento, no sobre la pieza. |
| `int cf1,cc1,cf2,cc2,cab;` | 29 | La pieza son cuatro coordenadas sueltas y un entero. |
| `int cab1,cab2,jugada=1,...` | 18 | Dos piezas, dos contadores, ningún tipo. |
| Sitios con `if (cab==1)` | 9 | La decisión de tipo, dispersa por `movcab`. |
| `impresionorden` | 753 | El bucle de turnos que nombra cada caballo a mano. |
| `cab=1;` / `cab=2;` | 765, 790, 820, 844 | Cuatro bloques de turno, dos por orden de salida. |
| `movcab(int &cf, int &cc)` | 234 | La función de movimiento recibe coordenadas, no una pieza. |

Las otras variantes tienen la misma forma: el switch está en 290 en `variant-02`, en 316 en
`variant-03` y en 330 en `variant-04`. Las variantes con punteros cambiaron el contenedor; no
añadieron un tipo de pieza.

Cuenta los sitios con `cab==1` tú mismo:

```bash
grep -c 'cab==1' original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

Nueve en `variant-01`, diez en las demás (la variante de registros añade uno en la impresión). Ese
número es el coste real de una tercera pieza en 2008: nueve o diez sitios, más los cuatro bloques de
turno, más las globales, más una regla de movimiento.

### Ejecutar la línea base

```bash
./run.sh
```

Salida esperada:

```text
comparisons=400 mismatches=0
```

El script compila con:

```text
-std=c++20 -Wall -Wextra -Werror -O2
```

`-Werror` importa: la línea base está limpia de avisos con las banderas que trae. Si tu compilador
la rechaza, registra el compilador y el diagnóstico; no silencies el aviso para que la ejecución
pase.

La ejecución compara el camino con switch con el camino de despacho para cada pieza, cada movimiento
y cada posición de la rejilla de 5×5 de `-2` a `+2`. La rejilla ejercita deltas positivos y
negativos sin necesidad de un tablero.

### Comprobar tu predicción

La interfaz tiene tres declaraciones virtuales, no dos: el destructor cuenta. `delta_with_switch`
tiene 51 líneas con dos switches internos de ocho casos. La ejecución hace 400 llamadas virtuales y
400 llamadas al switch. El switch del archivo es sobre el número de movimiento; la pieza es `cab`.

Si tu predicción decía que el archivo tiene un switch sobre el tipo de pieza, encontraste la brecha
entre la abreviatura del catálogo y la fuente. Anótala; es parte del diagnóstico.

## Paso 3 — Despachar (25 min)

### Hacer la copia de trabajo

Crea un directorio temporal fuera de `original/`. `build/` está ignorado por este repositorio y es
un buen lugar:

```bash
mkdir -p archaeology/lessons/archaeology-10-polymorphism/build/scratch
cd archaeology/lessons/archaeology-10-polymorphism/build/scratch
cp ../../code/polymorphism.cpp polymorphism.cpp
```

De aquí en adelante, edita `build/scratch/polymorphism.cpp`. No edites `code/polymorphism.cpp`; es
la línea base que ejecutará la siguiente persona.

### Congelar el oráculo antes de borrarlo

Si borras `delta_with_switch` y luego comparas el despacho con una tabla que escribiste desde los
mismos deltas, has comparado el código consigo mismo. Cero discrepancias no probaría nada.

Captura primero la salida del switch. Reemplaza el cuerpo de `main` por este bucle de emisión, antes
de borrar nada:

```cpp
std::array<PieceKind, 2> kinds = {PieceKind::knight, PieceKind::mirror};

for (int pieceIndex = 0; pieceIndex < 2; pieceIndex += 1)
{
    for (int moveNumber = 1; moveNumber <= 8; moveNumber += 1)
    {
        for (int row = -2; row <= 2; row += 1)
        {
            for (int column = -2; column <= 2; column += 1)
            {
                Position result = move_with_switch(kinds[pieceIndex], moveNumber,
                                                   Position{row, column});
                std::printf("%d %d %d %d %d %d\n", pieceIndex, moveNumber, row, column,
                            result.row, result.column);
            }
        }
    }
}
```

Mantén `return 0;` después del bucle. Compila y ejecuta, y guarda la salida estándar como
`golden.txt`:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 polymorphism.cpp -o polymorphism-golden
./polymorphism-golden > golden.txt
wc -l golden.txt
```

`wc -l` debe imprimir `400`. Cada línea es una observación: índice de pieza, número de movimiento,
fila inicial, columna inicial, fila resultante, columna resultante. Ese fichero es ahora el oráculo.
El switch ya no hace falta para producirlo.

### Borrar el switch

En la copia de trabajo:

1. borra `delta_with_switch` (51 líneas) y `move_with_switch` (5 líneas);
2. borra el enum `PieceKind` (5 líneas), que nada más necesita;
3. conserva `Piece`, `Knight`, `MirrorKnight` y las dos tablas de deltas;
4. reescribe `main` para conservar las dos líneas de `make_unique` y el arreglo `pieces`, leer
   `golden.txt` y llamar a `pieces[pieceIndex]->move(...)`;
5. mantén la línea de salida `comparisons=400 mismatches=0`;
6. mantén el código de salida: cero solo cuando no hay discrepancias.

Un lector compacto se ve así:

```cpp
std::FILE *golden = std::fopen("golden.txt", "r");
if (golden == nullptr)
{
    std::printf("cannot open golden.txt\n");
    return 2;
}

int pieceIndex = 0;
int moveNumber = 0;
int row = 0;
int column = 0;
int expectedRow = 0;
int expectedColumn = 0;
while (std::fscanf(golden, "%d %d %d %d %d %d", &pieceIndex, &moveNumber, &row, &column,
                   &expectedRow, &expectedColumn) == 6)
{
    Position actual = pieces[pieceIndex]->move(moveNumber, Position{row, column});
    comparisonCount += 1;
    if (actual.row != expectedRow || actual.column != expectedColumn)
    {
        mismatchCount += 1;
    }
}
std::fclose(golden);
```

El índice de pieza de la traza es ahora el índice del arreglo. Eso es lo único que la traza sabe del
tipo, y es la cantidad correcta: la prueba nombra una posición en una lista, no un tipo.

Compila con las mismas banderas y ejecuta desde el directorio que contiene `golden.txt`:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 polymorphism.cpp -o polymorphism
./polymorphism
```

Salida esperada, sin cambios:

```text
comparisons=400 mismatches=0
```

El switch desapareció de la copia de trabajo. Las clases quedan. La prueba sigue pasando.

### Qué borraste

| Elemento borrado | Líneas |
|---|---:|
| `delta_with_switch` | 51 |
| `move_with_switch` | 5 |
| `PieceKind` | 5 |
| Cuerpo anterior de `main` | 33 |

Lo que los reemplazó: un fichero de texto de 400 líneas y unas 30 líneas de lector. El código de
producción se hizo más pequeño; la evidencia se hizo más grande. Ese intercambio es el punto de un
oráculo congelado.

No borres `code/polymorphism.cpp`. La línea base es el artefacto que permite a la siguiente persona
reproducir tu comparación.

## Paso 4 — Comparar (10 min)

### Qué dice la ejecución

```text
comparisons=400 mismatches=0
```

Cuatrocientas observaciones, cero diferencias. Para cada pieza, cada movimiento de 1 a 8 y cada
posición inicial de la rejilla de 5×5, la llamada virtual devuelve la misma `Position` que el switch
congelado.

### Qué prueba la igualdad

- Las dos tablas de deltas de las clases coinciden con los deltas codificados en el switch.
- La convención de índice `moveNumber - 1` mapea 1–8 a las entradas 0–7 en ambos caminos.
- El despacho selecciona el mismo conjunto de reglas que seleccionaba el enum.
- Para estas 400 entradas, reemplazar el switch preserva el comportamiento.

### Qué no prueba la igualdad

| Afirmación | ¿Probada? | Por qué |
|---|---|---|
| El despacho es equivalente para todo `int` de movimiento. | No. | Solo se probaron 1–8. |
| Una entrada fuera de rango se comporta como el switch. | No. | El switch devolvía `(0, 0)`; la tabla no se comprueba. |
| El `movcab` del archivo quedó preservado. | No. | La línea base aísla el delta. |
| Una tercera pieza funcionará. | No. | No hay una tercera pieza en la traza. |
| El despacho es más rápido. | No. | No se hizo ninguna medición. |
| El despacho es más fácil de extender. | Por la prueba de cambio, sí. | Cuenta lugares tocados, no nanosegundos. |

### La dirección del oráculo

El orden importa. El switch es el oráculo porque es el código que existía. El despacho es el
candidato porque es el código en el que estás confiando. Si generas ambos desde la misma tabla, la
comparación es circular.

Es la misma regla que en `archaeology-06`, aplicada a otra decisión: **congela el comportamiento
antiguo antes de cambiar la estructura**.

### Cuando aparece una discrepancia

No "arregles" la clase hasta entender la discrepancia. Imprime la observación que falla:

```text
piece=1 move=6 row=-1 column=2 expected=(1,1) actual=(1,-1)
```

La tupla te dice si el delta está mal, si el signo está mal o si el índice está desfasado en uno.
Una discrepancia es información sobre el reemplazo, no un fallo de la lección.

## Paso 5 — Explicar (15 min)

### La prueba de cambio, medida

Añade un camello a cada camino en una copia de trabajo separada. La adición al despacho es una tabla
de deltas y una clase:

```cpp
constexpr std::array<Delta, 8> camelDeltas = {
    Delta{1, 3}, Delta{3, 1}, Delta{3, -1}, Delta{1, -3},
    Delta{-1, -3}, Delta{-3, -1}, Delta{-3, 1}, Delta{-1, 3},
};

class Camel : public Piece
{
public:
    Position move(int moveNumber, Position position) const override
    {
        const Delta &delta = camelDeltas[moveNumber - 1];
        return Position{position.row + delta.rowChange, position.column + delta.columnChange};
    }

    const char *role_name() const override
    {
        return "camel";
    }
};
```

La adición al switch es un valor de enum y una rama dentro de `delta_with_switch`. Medí ambas sobre
esta línea base:

| Medida | Camino con switch | Camino con despacho |
|---|---:|---:|
| Líneas añadidas a la regla | 23 | 20 |
| Declaraciones de regla existentes editadas | 2 | 0 |
| Tipos nuevos | 0 | 1 |
| Líneas de arnés para registrarla | 3 | 3 |
| Olvidar un movimiento: ¿compila limpio? | Sí, devuelve `(0, 0)` | Sí, la entrada es `(0, 0)` |
| ¿Puede responder "cómo te llamas"? | No | Sí, `role_name` |
| Conflicto de fusión con la regla de otra pieza | Sí, misma función | No |

Lee la primera fila con honestidad: los conteos de líneas son parecidos. Quien defienda el despacho
por el conteo de líneas está discutiendo por un error de redondeo. El argumento son las otras filas.

**Radio de impacto.** La edición del switch cae dentro de `delta_with_switch`, la función que
comparten todas las piezas. Un cambio concurrente en la regla del caballo entra en conflicto con
ella en revisión y en fusión. La edición de la clase cae en un tipo nuevo que ninguna declaración
existente nombra. Cero líneas existentes se mueven.

**Abierto frente a cerrado.** El camino con switch debe añadir el tipo a `PieceKind` y a la función
compartida. En el archivo debe añadir además una rama en nueve sitios con `cab==1` y un cuarto
bloque de turno. El camino de despacho añade un tipo y una línea de registro. Ambos caminos siguen
editando el arreglo del arnés y el límite del bucle; esa parte es un empate, y los arreglos de
tamaño fijo del banco lo exageran.

**Omisión silenciosa.** Olvida uno de los ocho movimientos del camello en el switch y el programa
compila limpio con `-Wall -Wextra -Werror`, porque el `default` interno se traga el caso ausente y
devuelve `(0, 0)`. La pieza simplemente no se mueve en el movimiento 7. El despacho tiene el mismo
peligro en su tabla, así que no vendas de más el compilador: lo que el despacho elimina es la
omisión de **tipo**, no la omisión de **dato**.

**Un nombre y un lugar.** `role_name` existe en la interfaz y en las dos clases. El camino con
switch no tiene dónde ponerlo: el nombre no es un delta, así que no pertenece a `delta_with_switch`.
Es la primera señal de que una pieza es más que su regla de movimiento, y la razón por la que
`construction-12` pone identidad y posición también en la interfaz.

### El coste real en el archivo

La línea base es un banco. El archivo es la factura.

| Cambio para una tercera pieza | Dónde cae en 2008 |
|---|---|
| Un tercer par de coordenadas y contador | Globales nuevas junto a `cf1,cc1,cf2,cc2` y `cab1,cab2`. |
| Un tercer valor de `cab` | Cada sitio con `if (cab==1)`: 9 en `variant-01`, 10 en las demás. |
| Un cuarto bloque de turno | `impresionorden`, que ya tiene cuatro bloques en 765, 790, 820 y 844. |
| Una regla de movimiento | `movcab`, cuyo switch es sobre el número de movimiento y no conoce el tipo. |
| Un tercer símbolo impreso | `simcab1`, `simcab2` e `impresion0`. |

Eso es lo que significa "añadir una pieza edita el switch y el bucle del juego" en concreto. No es
una edición. Es un cambio en la forma del programa, hecho a mano en cada sitio que conoce `cab`.

### Qué cuesta una llamada virtual, con precisión

| Coste | En esta línea base |
|---|---|
| Un vptr por objeto | `sizeof(Piece)` es 8 bytes. |
| Una vtable por clase | Dos, emitidas una vez en el fichero objeto. |
| Una llamada indirecta por movimiento | 400 llamadas en la ejecución. |
| Inlining bloqueado en el bucle | El bucle contiene `Piece *`; la llamada sigue indirecta. |
| Devirtualización posible | Sí, donde el compilador prueba el tipo; no en este bucle. |
| Asignación en el montón | Dos objetos, una vez. |

El switch cuesta una tabla de salto o una cadena de comparaciones, más la comprobación del enum.
Ninguno de los dos mecanismos es gratis, y con 400 llamadas ninguno se ve. Si quieres un número,
mídelo y registra compilador, banderas, CPU y repeticiones. Si no puedes ver la diferencia por
encima del ruido, escribe "no medido". Esa frase vale más que un porcentaje inventado.

### Dos arreglos, dos tamaños

**Arreglo mínimo — despacho sobre el tipo.** Reemplaza `delta_with_switch` por la interfaz `Piece` y
una clase por tipo. Conserva la tabla de movimientos dentro de cada clase. Eso es lo que hace el
paso
3. Elimina el switch de tipo y el enum. No elimina la tabla de movimientos, el tablero, las globales
ni las pruebas ausentes.

**Arreglo estructural — una pieza que posee su estado.** Mueve posición, identidad y movimiento a la
pieza, y deja que el bucle contenga `Piece &` sin coordenadas:

```cpp
class Piece
{
public:
    virtual ~Piece() = default;
    virtual Position position() const = 0;
    virtual void set_position(Position position) = 0;
    virtual Position move(int moveNumber) const = 0;
    virtual const char *role_name() const = 0;
};
```

Ahora el bucle no pasa `cf` y `cc` por referencia, y `cab` desaparece. Enlaces a Track A:
`construction-11` (encapsulación) para el estado privado, `construction-12` (polimorfismo) para la
interfaz, y `construction-13` (composición) para la siguiente pregunta: ¿una pieza debería *tener*
una regla de movimiento en lugar de *ser* una?

El `movcab(int &cf, int &cc)` del archivo es exactamente la firma que este arreglo elimina. La
función muta dos enteros que posee el llamador; la pieza no posee ninguno.

### Qué no arregla el despacho

- La tabla de movimientos. `moveNumber - 1` sigue siendo un índice sin comprobar; `move(9, ...)` es
  comportamiento indefinido.
- El `default` que devolvía `(0, 0)`. Si conservas una tabla, conserva una guarda o declara la
  precondición.
- Las globales. El despacho en una copia de trabajo no elimina `vectab`, `jugada` ni `ganador`.
- La impresión y el bucle de turnos. `impresionorden` sigue nombrando piezas a mano.
- Las pruebas ausentes. Una traza congelada para una decisión no es una suite de pruebas.
- Los sitios con `cab==1` del archivo. El banco del laboratorio no tiene ninguno; el original tiene
  nueve.

### El compromiso, dicho con justicia

El despacho cuesta una interfaz, una vtable, una asignación y un concepto más que aprender. Compra
una frontera: el bucle deja de enumerar tipos, y una pieza nueva es un tipo nuevo. En un programa de
una sola vez con dos piezas idénticas, el switch es más simple y no hay nada que extender. En un
programa que espera una tercera pieza, el switch le cobra a la siguiente persona en cada sitio que
conoce `cab`.

La lección te pide elegir con la prueba de cambio, no con el gusto. Si tu contexto dice que el juego
nunca tendrá una tercera pieza, el switch es defendible. Escribe ese contexto; es parte del
diagnóstico.

## Paso 6 — Registrar (10 min)

Rellena [`EVIDENCE-es.md`](EVIDENCE-es.md):

- configuración y fijación del archivo;
- las cinco predicciones, escritas antes de inspeccionar;
- el inventario de la interfaz: tres declaraciones virtuales, dos sobreescrituras por clase;
- los anclajes del switch y los nueve sitios con `cab==1`;
- la ejecución de la línea base y su salida exacta;
- la captura del oráculo: comando, `golden.txt`, número de líneas;
- el diff del reemplazo: qué se borró, qué se conservó, qué se añadió;
- el resultado de la comparación y la dirección del oráculo;
- la prueba de cambio: 23 líneas en el switch, 20 en el despacho, y dónde cae cada una;
- el coste de la llamada, con la medición que hiciste o la frase "no medido";
- la frontera de evidencia: qué prueba la ejecución y qué no;
- la comprobación de integridad del archivo.

Una frase como "el polimorfismo es más limpio" es una opinión. "Tres declaraciones virtuales, 400
comparaciones, 0 discrepancias, 23 líneas en la función compartida frente a 20 en un tipo nuevo,
nueve sitios con `cab==1` en el original" es un diagnóstico.

## El original en detalle

Datos que puedes citar, con números de línea de `variant-01` en la etiqueta `v1.0.0`:

| Ubicación | Qué hay |
|---:|---|
| 18 | `cab1,cab2,jugada,orden,ganador` — dos piezas como dos contadores. |
| 29 | `cf1,cc1,cf2,cc2,cab` — coordenadas y el tipo como un entero. |
| 234 | `int movcab(int &cf,int &cc)` — el movimiento recibe coordenadas, no una pieza. |
| 239 | `switch(vecjugadas[i])` — despacho sobre el número de movimiento. |
| 249, 310, 366, 422, 478, 534, 591, 647 | `if (cab==1)` dentro de los ocho cuerpos de caso. |
| 702 | `if (i==8&&cab==1)` — la comprobación de caballo encerrado, también por tipo. |
| 753 | `void impresionorden()` — el bucle de turnos. |
| 765, 790, 820, 844 | `cab=1;` y `cab=2;` — cuatro bloques de turno. |
| 114 | `impresion0` — impresión que nombra cada caballo. |

La misma forma aparece en las otras variantes:

| Variante | Línea del `switch` | Sitios con `cab==1` |
|---|---:|---:|
| `variant-01-baseline-2008` | 239 | 9 |
| `variant-02-records-2008` | 290 | 10 |
| `variant-03-pointers-dev-2008` | 316 | 10 |
| `variant-04-pointers-final-2008` | 330 | 10 |

Las variantes con punteros cambiaron el contenedor, no la decisión. Vale la pena notarlo: un cambio
de sintaxis no añadió un tipo de pieza.

## Qué prueba y qué no prueba el reemplazo

| Afirmación | ¿Respaldada? | Por qué |
|---|---|---|
| El despacho reproduce el switch para 400 entradas. | Sí | La ejecución imprimió 0 discrepancias. |
| El despacho es equivalente para todas las entradas. | No | La prueba cubre 8 movimientos × 25 posiciones × 2 piezas. |
| El comportamiento de `movcab` quedó preservado. | No | La línea base aísla el delta. |
| El reemplazo elimina el switch de tipo. | Sí | `delta_with_switch` y `PieceKind` desaparecieron. |
| El reemplazo elimina el switch de número de movimiento. | No | Cada clase sigue indexando su tabla. |
| Una tercera pieza necesita cero ediciones en el bucle. | Sí, en el banco | Un registro en el sitio de construcción. |
| Una tercera pieza necesita cero ediciones en el archivo. | No | Nueve sitios con `cab==1` y cuatro bloques de turno. |
| El despacho es más rápido. | No medido | No lo afirmes. |
| El despacho elimina el índice sin comprobar. | No | `moveNumber - 1` permanece. |
| El despacho elimina las globales. | No | Están fuera de este banco. |

Esta tabla es la comprobación de honestidad. Un registro que dice "el refactor es correcto" sin
nombrar la muestra está excediéndose. Di "para estas 400 entradas" cuando eso es lo que ejecutaste.

## Autocomprobación (binaria)

- [ ] Escribí las cinco predicciones antes de abrir la fuente.
- [ ] Nombré las tres declaraciones virtuales y las dos sobreescrituras por clase.
- [ ] Leí `delta_with_switch` y conté sus dos switches internos.
- [ ] Encontré el switch del archivo en la línea 239 y confirmé que es sobre el número de
  movimiento.
- [ ] Conté los sitios con `cab==1`: 9 en `variant-01`, 10 en las demás.
- [ ] Ejecuté `./run.sh` y vi `comparisons=400 mismatches=0`.
- [ ] Registré el compilador y las banderas.
- [ ] Copié la línea base a un directorio de trabajo antes de editar.
- [ ] Capturé `golden.txt` desde `move_with_switch` antes de borrarlo.
- [ ] `golden.txt` tiene 400 líneas.
- [ ] Borré `delta_with_switch`, `move_with_switch` y `PieceKind`.
- [ ] La versión con despacho imprime `comparisons=400 mismatches=0`.
- [ ] Añadí una tercera pieza en ambos caminos y conté los lugares tocados.
- [ ] Expliqué por qué los conteos de líneas son parecidos y por qué el lugar de la edición difiere.
- [ ] Expliqué qué cuesta una llamada virtual, con una medición o con "no medido".
- [ ] Separé el switch sobre el tipo de pieza del switch sobre el número de movimiento.
- [ ] Indiqué que la línea base reconstruye el switch de tipo y no es código de 2008 literal.
- [ ] Noté que los dos caballos del archivo se mueven igual.
- [ ] Noté que el `default` interno oculta un movimiento ausente.
- [ ] Enlacé el arreglo estructural a `construction-11`, `construction-12` y `construction-13`.
- [ ] El archivo está sin modificar (`git status` en el archivo está limpio).

Si una casilla no está marcada, la casilla sin marcar nombra el siguiente paso. No hay penalización
por repetir uno.

## Misconcepciones que esta lección refuta

| Idea equivocada | Por qué está mal | Qué hacer en su lugar |
|---|---|---|
| "Polimorfismo significa herencia." | La herencia es una forma de compartir código; el punto es el despacho. | Añade un tipo a través de la interfaz y cuenta ediciones del bucle. |
| "Una tabla es polimorfismo." | Una tabla despacha por índice y el bucle sigue enumerando tipos. | Separa la decisión del dato de la decisión del tipo. |
| "El archivo tiene un switch sobre el tipo de pieza." | Tiene un switch sobre el número de movimiento y sitios con `cab==1`. | Cita la línea 239 y las nueve ramas. |
| "Las llamadas virtuales son demasiado lentas para usarlas aquí." | 400 llamadas, un vptr cada una; el coste es invisible. | Mide, o escribe "no medido". |
| "El switch es más rápido, así que es mejor." | Un switch denso también es una rama indirecta. | Precia el coste de cambio, no solo los ciclos. |
| "El despacho hace el código más corto." | Aquí son 20 líneas frente a 23. | Cuenta lugares tocados y radio de impacto. |
| "El compilador detectará una pieza ausente." | `-Wall -Werror` detecta el switch de tipo solo sin `default`; el `default` interno oculta un movimiento ausente. | Prueba el comportamiento; no confíes en el aviso. |
| "Cero discrepancias prueba que el refactor es correcto." | Prueba igualdad en 400 entradas muestreadas con un oráculo congelado. | Declara la muestra y la dirección del oráculo. |
| "La línea base es la regla original." | Aísla el delta e inventa `MirrorKnight`. | Escribe la frontera de aislamiento en el registro. |
| "Borra el código viejo y luego prueba el nuevo." | Sin un oráculo congelado, la comparación es circular. | Captura la traza de referencia primero. |
| "El despacho también arregla las globales." | El banco no tiene tablero, ni bucle de turnos, ni impresión. | Lista qué queda fuera del banco. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Polimorfismo | Una interfaz, muchas implementaciones. |
| Ligadura dinámica | El tipo dinámico decide qué función se ejecuta. |
| Tipo estático | El tipo que ve el compilador en el sitio de la llamada. |
| Tipo dinámico | El tipo del objeto en tiempo de ejecución. |
| Despacho | Elegir qué código se ejecuta, por valor o por tipo. |
| vptr | El puntero oculto que cada objeto polimórfico guarda a la tabla de su clase. |
| vtable | La tabla por clase con las direcciones de las funciones virtuales. |
| Devirtualización | Sustituir una llamada indirecta por una directa cuando el tipo es demostrable. |
| Virtual pura | Una declaración con `= 0`; la interfaz no tiene cuerpo. |
| Sobreescritura | Una implementación de una función virtual, comprobada por el compilador. |
| Conjunto de reglas | La geometría de movimiento que pertenece a un tipo de pieza. |
| Prueba de cambio | Un cambio concreto cuyos lugares tocados miden el diseño. |
| Radio de impacto | Cuánto código existente obliga a tocar o revisar un cambio. |
| Oráculo | La implementación en la que confías mientras reemplazas. |
| Traza de referencia | Un registro congelado de salidas usado para comparar una implementación nueva. |
| Frontera de evidencia | La línea entre lo que prueba la ejecución y lo que infieres. |

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
