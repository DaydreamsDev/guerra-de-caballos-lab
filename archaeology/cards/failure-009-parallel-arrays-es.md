# failure-009 — La posición de un caballo se guarda dos veces en variables paralelas

> [English](failure-009-parallel-arrays.md) | [Español](failure-009-parallel-arrays-es.md)

- **Clase:** diseño
- **Severidad:** media
- **Concepto violado:** un valor tiene una representación y un dueño
- **Archivo:** etiqueta `v1.0.0`,
  `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`

## Evidencia

- **Variante:** `variant-01-baseline-2008`. Las variantes de punteros mantienen la misma
  separación.
- **Líneas:** 17, 18, 29, 106–109, 229 y 766.
- **Código:**

```cpp
int  vectab[64],vecjugadas[8];              // línea 17
int  cab1,cab2,jugada=1,orden,ganador=1;    // línea 18
int cf1,cc1,cf2,cc2,cab;                    // línea 29

cf1=cab1/8;  cc1=cab1%8;                    // líneas 106–107
cf2=cab2/8;  cc2=cab2%8;                    // líneas 108–109
ccab=cf*8+cc;                               // línea 229

cab1=movcab(cf1,cc1);                       // línea 766
```

`cab1` y `cab2` son índices planos del tablero en `0..63`. `cf1`, `cc1`, `cf2` y `cc2` son
las mismas dos posiciones como fila y columna en `0..7`. `conversionvm` escribe la copia de
fila/columna desde el índice plano; `conversionmv` convierte de vuelta. Nada garantiza que las
copias coincidan.

El tablero es `int vectab[64]`, y cada llamador lo indexa directamente: `vectab[cab1]`,
`vectab[ccab]`, `vectab[k]`. La fórmula `cf * 8 + cc` es el contrato público.

## Síntoma

El programa funciona mientras las dos copias se mantienen sincronizadas. Un movimiento
actualiza `cf` y `cc` por referencia y escribe el índice plano devuelto en `cab1` en una sola
línea. Si la función retorna antes, o una edición futura actualiza una copia y no la otra, la
posición se desvía. El tablero marca una casilla para una posición y comprueba la captura para
otra.

La desviación es silenciosa: ambos valores son enteros válidos, así que ni el compilador ni una
comprobación de límites ve la discrepancia.

## Causa raíz

Cuatro enteros de coordenadas sueltos, dos índices planos y un arreglo de tablero se tratan como
variables independientes. El invariante del dominio —una posición es una fila y una columna que
se mueven juntas— existe solo en la cabeza del programador. La representación es pública, así
que cada llamador debe conocer el ancho del tablero, la fórmula por filas y la convención de
impresión basada en uno.

`movcab(int &cf, int &cc)` empeora la separación: muta la copia de fila/columna y devuelve la
copia plana, así que una llamada actualiza una posición por dos mecanismos.

## Consecuencia

Amplificación del cambio: redimensionar el tablero toca cada expresión de índice. Desviación
silenciosa: las dos copias pueden discrepar y ningún compilador lo ve. Costo de revisión: el
revisor debe reconstruir la convención de índices para leer un diff. Costo de prueba: una
prueba unitaria debe construir la representación, no solo el valor del dominio.

El programa no causa comportamiento indefinido por sí mismo. La escritura `i <= 64` es
`failure-001`.

## Prueba que lo detecta

`archaeology/lessons/archaeology-07-representation-leakage/run.sh` ejecuta el mismo escenario
con una versión de arreglos paralelos y una versión con registro, y compara el resumen:

```text
parallel=player=2,3 machine=3,4 visited=3
record=player=2,3 machine=3,4 visited=3
match=yes
```

La lección congela la salida paralela antes de refactorizar, luego actualiza el llamador a
`board.mark(player)` y `player = Knight{2, 3}`. La prueba demuestra que la versión con registro
hace las mismas tres marcas para este escenario. No demuestra equivalencia para toda posición.

## Arreglo mínimo

Agrupa las dos coordenadas en un solo valor:

```cpp
struct Knight
{
    int row;
    int column;
};
```

Mover un caballo pasa a ser una asignación: `player = Knight{2, 3};`. El llamador todavía
calcula el índice del tablero, pero la fila y la columna ya no pueden separarse. Esto elimina la
mitad de variables paralelas, no la fuga de representación.

## Arreglo estructural

Dale un dueño al tablero. `Board` guarda un arreglo privado y expone `mark(const Knight &)` y
`visited_count()`. La fórmula del índice se mueve dentro de `mark`, donde corresponde. El
llamador expresa la operación del dominio y ya no necesita el ancho, el origen ni el
almacenamiento.

Peldaños de la pista A: [`construction-04-records`][c04] para `Knight`,
[`construction-05-array-of-records`][c05] para las casillas del tablero,
[`construction-09-abstract-data-type`][c09] para la independencia de representación, y
[`construction-11-encapsulation`][c11] para el arreglo privado.

[c04]: ../../construction/lessons/construction-04-records/lesson.md
[c05]: ../../construction/lessons/construction-05-array-of-records/lesson.md
[c09]: ../../construction/lessons/construction-09-abstract-data-type/lesson.md
[c11]: ../../construction/lessons/construction-11-encapsulation/lesson.md

## Confianza

Alta en que las dos representaciones existen y se actualizan por mecanismos distintos. Media en
la frecuencia con que se desvían en las ejecuciones originales: la fuente muestra el riesgo, no
un fallo medido.

## Nota sobre el archivo

Esta tarjeta es evidencia, no una reparación del archivo. El archivo permanece de solo
lectura; el registro y el dueño van en una copia de trabajo o en `reference/`.
