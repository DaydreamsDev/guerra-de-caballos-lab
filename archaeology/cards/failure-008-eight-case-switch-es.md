# failure-008 — Ocho cuerpos de caso copiados a mano codifican una sola regla de delta

> [English](failure-008-eight-case-switch.md) | [Español](failure-008-eight-case-switch-es.md)

- **Clase:** diseño
- **Severidad:** media
- **Concepto violado:** una regla vive en un solo lugar; la parte variable va en los datos
- **Archivo:** etiqueta `v1.0.0`,
  `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`

## Evidencia

- **Variante:** `variant-01-baseline-2008`. El mismo switch está en `variant-02:290`,
  `variant-03:316` y `variant-04:330`.
- **Líneas:** 239–695. Las ocho etiquetas de caso están en 241, 302, 359, 415, 471, 527, 584
  y 640.
- **Código:**

```cpp
switch(vecjugadas[i])                    // línea 239
{
case 1:                                  // delta (-1, +2)
    if (cf-1>=0&&cf-1<=7&&cc+2>=0&&cc+2<=7)
    {
        ccab=conversionmv(cf-1,cc+2);
        // ... el mismo esqueleto: turno, visitada, captura, aplicar, registrar, imprimir ...
    }
case 2:                                  // línea 302, delta (-2, +1)
    if (cf-2>=0&&cf-2<=7&&cc+1>=0&&cc+1<=7)
    {
        ccab=conversionmv(cf-2,cc+1);
```

Los cuerpos de caso ocupan 455 líneas. Si quitas comentarios, espacios y dígitos, la secuencia
de tokens es la misma. Los únicos valores que cambian son dieciséis enteros: un delta de fila
y un delta de columna para cada uno de ocho movimientos. La fuente lo dice en la línea 301:

```text
//los demas casos tienen la misma estructura que esta explicada en el caso 1
```

## Síntoma

El programa funciona. Cada movimiento legal cae en la casilla correcta, el tablero lo registra
y la partida termina bien. El defecto es invisible en una sola ejecución. Aparece cuando la
regla cambia: corregir un delta o añadir un noveno movimiento exige tocar ocho lugares, y una
copia olvidada diverge en silencio.

El compilador no puede comparar el caso 3 con el caso 6. Una revisión ve un diff de 60 líneas
para un cambio de dos enteros.

## Causa raíz

El código guarda una regla como ocho copias de código en lugar de una tabla de datos. Cada
caso contiene además dos ramas de turno casi idénticas, así que el bloque de aplicación existe
dieciséis veces. El delta es la parte variable; límites, turno, visitada, captura, aplicación,
registro e impresión son el esqueleto. El esqueleto se copió junto con los datos.

Es un clon de conocimiento, no un clon textual. Los cuerpos difieren en comentarios, espacios
y signos, y por eso un diff de bytes no revela la duplicación.

## Consecuencia

Amplificación del cambio: un cambio de regla toca ocho cuerpos. Divergencia silenciosa: un
arreglo aplicado a siete cuerpos deja un caso con otro comportamiento. Costo de revisión: la
regla desaparece detrás de las copias. Costo de fusión: dos ediciones independientes de casos
distintos chocan en una misma función.

El programa es correcto para las entradas que recibe. El costo lo paga el siguiente cambio de
regla, no la ejecución actual.

## Prueba que lo detecta

`archaeology/lessons/archaeology-06-duplication-switch/run.sh` compila la base y compara el
switch con una tabla para 8 movimientos sobre una rejilla de 5×5:

```text
comparisons=200 mismatches=0
```

El switch es el oráculo; la tabla es el candidato. La lección también ejecuta una prueba de
cambio: añade un noveno delta y una novena entrada, y cuenta los lugares tocados en ambos
caminos. En el camino del switch es un nuevo cuerpo de caso de unas 56–61 líneas más el límite
del bucle.

La base aísla el delta. No prueba límites, casillas visitadas, captura ni turno.

## Arreglo mínimo

Sustituye los ocho cuerpos por una tabla de deltas y una búsqueda:

```cpp
struct Delta { int row; int column; };

constexpr Delta moveDeltas[8] = {
    {-1, +2}, {-2, +1}, {-2, -1}, {-1, -2},
    {+1, -2}, {+2, -1}, {+2, +1}, {+1, +2},
};

Delta deltaFor(int moveNumber) { return moveDeltas[moveNumber - 1]; }
```

Mantén una guarda de rango. La tabla pierde el comportamiento `default` del switch para
entradas fuera de rango.

## Arreglo estructural

Extrae toda la aplicación del movimiento a una función que reciba un `Delta` y el estado que
necesita:

```cpp
MoveResult applyMove(Position from, Delta delta, const Board &board, int turn);
```

La función comprueba límites, casillas visitadas y captura, aplica el delta, registra la
jugada y devuelve el resultado. Los ocho casos desaparecen y la tabla queda como dato. Las dos
ramas de turno se reducen a una llamada.

Peldaños de la pista A: [`construction-03-functions`][c03] para el esqueleto,
[`construction-05-array-of-records`][c05] para la tabla, y
[`construction-19-data-oriented`][c19] para la idea de que los deltas son datos, no código.

[c03]: ../../construction/lessons/construction-03-functions/lesson.md
[c05]: ../../construction/lessons/construction-05-array-of-records/lesson.md
[c19]: ../../construction/lessons/construction-19-data-oriented/lesson.md

## Confianza

Alta. Las ocho etiquetas, el tramo de 455 líneas y el esqueleto repetido son visibles. Las 200
comparaciones son una ejecución, no una inferencia. La afirmación se limita al cálculo del
delta.

## Nota sobre el archivo

Esta tarjeta es evidencia, no una reparación del archivo. El archivo permanece de solo
lectura; la extracción va en una copia de trabajo y la base queda disponible como oráculo.
