# failure-010 — `struct stablero` es un envoltorio transparente, no un tipo abstracto de datos

> [English](failure-010-transparent-struct.md) | [Español](failure-010-transparent-struct-es.md)

- **Clase:** diseño
- **Severidad:** media
- **Concepto violado:** un tipo se define por sus operaciones, no por su diseño de memoria
- **Archivo:** etiqueta `v1.0.0`,
  `original/variant-03-pointers-dev-2008/source/guerra-de-caballos-pointers-dev.cpp`

## Evidencia

- **Variantes:** `variant-03-pointers-dev-2008` y `variant-04-pointers-final-2008`.
- **Líneas:** `variant-03:34–44`, `variant-03:86`; `variant-04:44–49`, `variant-04:92`.
- **Código:**

```cpp
struct stablero {          // variant-03, línea 34
    int  mvectab;          // línea 36
};
struct stablero vectab[64];                         // línea 42
struct stablero *pvectab,*pvectab1,*pvectab2;       // línea 44

pvectab->mvectab=0;        // línea 86, la traducción de vectab[i]=0
```

`variant-04` repite la idea con otro nombre: `struct tab { int vectab; }` en las líneas 44–45,
`struct tab tablero[64]` en la línea 48, y `ptablero->vectab=0` en la línea 92.

El campo es la interfaz. `variant-03` tiene 102 líneas que mencionan `mvectab`; `variant-04`
tiene 146 líneas que mencionan `tablero`. El envoltorio renombró el tipo y no cambió nada de la
dependencia.

## Síntoma

El programa funciona. La estructura compila, el arreglo se declara y el bucle de punteros limpia
el tablero. Un lector podría concluir que el tablero ya es un tipo. No lo es: cada llamador
sigue nombrando el almacenamiento, sigue sabiendo que una casilla es un entero y sigue
escribiendo el campo directamente.

La traducción mecánica es visible. `vectab[i]=0;` se convirtió en `pvectab->mvectab=0;`. El
puntero se movió; el acoplamiento se quedó.

## Causa raíz

El código envolvió un entero en una estructura y la llamó tipo. Una estructura con un campo
público no tiene operaciones, ni invariante, ni representación oculta. La única prueba honesta
de un tipo abstracto de datos es un intercambio: reemplazar la representación, conservar la
interfaz y ejecutar el mismo llamador. El código de 2008 no puede pasar esa prueba, porque no
hay interfaz que conservar.

`struct svecjugadas { int mjugadas; }` falla la misma prueba para la lista de jugadas. El
archivo tiene dos envoltorios y cero abstracciones.

## Consecuencia

La representación sigue siendo pública, así que un cambio en el almacenamiento del tablero
llega a cada llamador. Renombrar `mvectab` a `visited` impediría compilar 102 líneas; cambiar
solo el tipo dejaría compilar a la mayoría, porque C++ convierte enteros a `bool`. Eso es peor:
el acoplamiento sobrevive al cambio de tipo.

El programa no se vuelve incorrecto. El costo es la amplificación del cambio y una falsa
sensación de encapsulación.

## Prueba que lo detecta

`archaeology/lessons/archaeology-09-abstract-data-type/run.sh` compila el mismo `main.cpp`
contra dos representaciones y compara la salida:

```text
array=visited=2 first=1 second=1
bitmask=visited=2 first=1 second=1
match=yes
```

El llamador y el encabezado se resumen con hash antes y después; solo cambia el archivo de
implementación en la línea de enlace. La lección también captura al compilador rechazando
`sizeof(Board)`, un `Board` en la pila y `board->visited` a través de un tipo incompleto.

Esa es la prueba de intercambio que el envoltorio no puede pasar. El archivo no tiene
encabezado ni operación que conservar.

## Arreglo mínimo

Sustituye la estructura transparente por un tipo opaco y operaciones:

```cpp
// board.h
struct Board;
Board *board_create();
void board_destroy(Board *);
void board_reset(Board *);
void board_mark(Board *, int row, int column);
bool board_is_visited(const Board *, int row, int column);
std::size_t board_visited_count(const Board *);
```

La definición vive en un solo `.cpp`. El llamador puede mantener y pasar un `Board *` y no
puede dimensionarlo, asignarlo ni inspeccionarlo. Es el cambio más pequeño que saca el campo
del contrato.

## Arreglo estructural

Usa una clase con pimpl, o una fábrica que devuelva un tipo por valor con destructor fuera de
línea. Así el llamador obtiene RAII, copia y movimiento, y la representación sigue oculta. El
manejador al estilo C compra independencia de representación y paga con tiempo de vida manual;
el pimpl devuelve la comprobación del tiempo de vida al compilador a cambio de una indirección
por operación.

Peldaños de la pista A: [`construction-09-abstract-data-type`][c09] para el intercambio,
[`construction-10-adt-invariants`][c10] para la validez,
[`construction-11-encapsulation`][c11] para la clase, y
[`construction-16-ownership`][c16] para el tiempo de vida.

[c09]: ../../construction/lessons/construction-09-abstract-data-type/lesson.md
[c10]: ../../construction/lessons/construction-10-adt-invariants/lesson.md
[c11]: ../../construction/lessons/construction-11-encapsulation/lesson.md
[c16]: ../../construction/lessons/construction-16-ownership/lesson.md

## Confianza

Alta en que el envoltorio es transparente y en que los llamadores nombran el campo. Alta en que
el archivo no tiene encabezados. Media en los conteos exactos de otras variantes: los conteos
citados vienen de `grep -c` sobre los archivos fijados.

## Nota sobre el archivo

Esta tarjeta es evidencia, no una reparación del archivo. El archivo permanece de solo
lectura; el tipo opaco va en una copia de trabajo o en `reference/`.
