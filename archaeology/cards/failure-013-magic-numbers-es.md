# failure-013 — Tamaño, ancho, glifos y límite de tablas son literales sin nombre

> [English](failure-013-magic-numbers.md) | [Español](failure-013-magic-numbers-es.md)

- **Clase:** diseño
- **Severidad:** baja
- **Concepto violado:** una decisión del dominio merece un nombre
- **Archivo:** etiqueta `v1.0.0`,
  `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`

## Evidencia

- **Variante:** `variant-01-baseline-2008`.
- **Líneas:** 16, 17, 61, 81–82, 106–109 y 229.
- **Código:**

```cpp
const char simcab1=21,simcab2=156,sep=219;  // línea 16
int  vectab[64],vecjugadas[8];              // línea 17

for (i=0;i<=64;i++)                         // línea 61
cab1= rand() % 64;                          // línea 81
cab2= rand() % 64;                          // línea 82

cf1=cab1/8;  cc1=cab1%8;                    // líneas 106–107
cf2=cab2/8;  cc2=cab2%8;                    // líneas 108–109
ccab=cf * 8 +cc;                            // línea 229
```

`64` es la cantidad de casillas y el módulo de colocación. `8` es el ancho del tablero, la
cantidad de movimientos y el divisor de fila. `21`, `156` y `219` son puntos de código CP437
para los dos caballos y la pared; están escritos como enteros decimales sin nombrar la página
de código.

`200` no está en el archivo. Es el límite de tablas de la especificación moderna y el valor por
defecto del arnés: `common/spec/game-rules.md` §11 y `common/harness/README.md`
(`--max-moves`, por defecto `200`). La fuente de 2008 no tiene cláusula de tablas;
`archaeology-13` registra esa ausencia como regla inferida. La tarjeta incluye `200` porque es
la misma clase en la implementación de referencia, no porque el archivo lo contenga.

## Síntoma

El programa funciona en 8×8. El significado de cada número vive en la cabeza del lector.
Redimensionar el tablero exige encontrar cada `64`, cada `8` y cada conversión, y decidir qué
`8` es el ancho, cuál es la cantidad de movimientos y cuál es el divisor de fila.

Los glifos son peores: `21`, `156` y `219` parecen enteros arbitrarios. Nada dice que son
bytes CP437, así que una reescritura a UTF-8 puede cambiar el tablero en silencio.

## Causa raíz

El código codifica decisiones del dominio como literales en el punto de uso. El mismo dígito
tiene varios significados, así que un cambio no se puede hacer editando un solo nombre. No hay
`boardWidth`, ni `boardCells`, ni `moveCount`, ni `knightGlyph`, ni `maxMoves`.

El límite de tablas es un caso aparte: el archivo no tiene regla de tablas, así que `200` existe
solo en la especificación y el arnés modernos. Es una política literal fuera del archivo, y
tiene el mismo problema: un valor por defecto enterrado en una herramienta.

## Consecuencia

Los cambios son propensos a error. Un tablero de 10×10 toca el tamaño del arreglo, el módulo de
colocación, cuatro líneas de conversión y cada comprobación de límites. Un `8` olvidado deja el
ancho viejo en un camino. Un cambio de página de código rompe los glifos sin error de
compilación.

El programa es correcto para 8×8 y CP437. El costo lo paga el primer redimensionamiento o
cambio de codificación.

## Prueba que lo detecta

Ninguna lección reemplaza estos literales. La comprobación de comportamiento disponible es
`archaeology/lessons/archaeology-14-tests-determinism/run.sh`, que congela una transcripción y
compara un programa reconstruido contra ella:

```text
seed=1
move=1 position=2
move=2 position=5
move=3 position=0
winner=player
moves=3
golden=yes
```

La comprobación concreta del cambio es: sustituye un literal por una constante con nombre,
reconstruye y compara la salida con la traza congelada. Repite para `64`, `8`, `21`, `156`,
`219` y el `200` de la especificación. Un `grep -n` antes y después muestra si queda algún
literal suelto.

`archaeology-12` deja explícitamente el `8` mágico en su lugar: renombrar una variable es un
cambio distinto de nombrar una constante. No afirmes que esa lección corrige esta tarjeta.

## Arreglo mínimo

Nombra las decisiones:

```cpp
constexpr int boardWidth = 8;
constexpr int boardCells = boardWidth * boardWidth;
constexpr int moveCount = 8;

constexpr char knightOneGlyph = 0x15;   // CP437, no UTF-8
constexpr char knightTwoGlyph = 0x9C;
constexpr char wallGlyph = 0xDB;

constexpr int maxMoves = 200;           // especificación y arnés, no el archivo
```

Usa los nombres en las declaraciones, los bucles, las conversiones y las comprobaciones de
límites. Los valores no cambian; el significado se vuelve visible.

## Arreglo estructural

Dale al tablero un tipo que posea su ancho y su cantidad de casillas, y a las reglas un tipo que
posea el límite de tablas:

```cpp
class Board
{
public:
    static constexpr int width = 8;
    static constexpr int cellCount = width * width;
    // ...
};
```

Los glifos pasan a una enumeración o a un registro `Glyphs` con un comentario que nombre CP437.
El límite de tablas pasa a un campo de `GameRules`, no a un valor por defecto de la línea de
comandos duplicado en el arnés.

Peldaños de la pista A: [`construction-04-records`][c04] para campos con nombre y
[`construction-17-stl`][c17] para `std::array`, cuyo tamaño viene del tipo.

[c04]: ../../construction/lessons/construction-04-records/lesson.md
[c17]: ../../construction/lessons/construction-17-stl/lesson.md

## Confianza

Alta en los literales del archivo y en sus líneas. Alta en que `200` está ausente del archivo y
presente en la especificación y el arnés. Media en la cantidad exacta de apariciones de `8` que
son ancho de tablero frente a cantidad de movimientos: el mismo dígito sirve para ambos, y ese
es el defecto.

## Nota sobre el archivo

Esta tarjeta es evidencia, no una reparación del archivo. El archivo permanece de solo
lectura; las constantes con nombre van en una copia de trabajo o en `reference/`.
