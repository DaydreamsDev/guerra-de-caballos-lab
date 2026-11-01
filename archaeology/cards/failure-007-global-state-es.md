# failure-007 — Los globales mutables convierten cada función en un contrato oculto

> [English](failure-007-global-state.md) | [Español](failure-007-global-state-es.md)

- **Clase:** diseño
- **Severidad:** media
- **Concepto violado:** el estado debe tener un dueño y una interfaz visible
- **Archivo:** etiqueta `v1.0.0`,
  `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`

## Evidencia

- **Variante:** `variant-01-baseline-2008`. El mismo estado vuelve en `variant-02`,
  `variant-03` y `variant-04`, a veces detrás de los envoltorios de punteros.
- **Líneas:** 17, 18, 29 y 234.
- **Código:**

```cpp
int  vectab[64],vecjugadas[8];              // línea 17
int  cab1,cab2,jugada=1,orden,ganador=1;    // línea 18
int cf1,cc1,cf2,cc2,cab;                    // línea 29

int movcab(int &cf,int &cc)                 // línea 234
```

`movcab` lee `vecjugadas`, `jugada`, `cab`, `vectab`, `cab1` y `cab2`. Escribe `ganador`,
`vectab[ccab]` y `jugada`. También imprime el número de caso en la línea 262 y llama a
`impresion()` en la línea 705. La firma nombra dos enteros.

El escáner de `archaeology-05` cuenta las menciones: `vectab=138`, `vecjugadas=15`,
`cab1=30`, `cab2=30`, `jugada=69`, `orden=10`, `ganador=24`, `cf1=5`, `cc1=5`,
`cf2=5`, `cc2=5`, `cab=15`. Los conteos son menciones, no asignaciones.

## Síntoma

El programa funciona. Coloca dos caballos, alterna turnos, imprime un tablero y termina con
una captura o una trampa. Nada falla y nada imprime un ganador incorrecto. El defecto es que
ninguna función declara el estado que usa. Un lector no puede ver en `movcab(cf1, cc1)` que
la llamada depende de seis globales, escribe otros tres y necesita una consola.

Un solo movimiento no se puede llamar de forma aislada. La prueba debe construir primero el
tablero, el orden de jugadas, el número de jugada, el selector de turno y ambas posiciones.
La preparación es más larga que la aserción.

## Causa raíz

Los nombres de ámbito de archivo son un borrador compartido. Cualquier función puede leerlos
o escribirlos sin declarar la dependencia. Tres colisiones concretas empeoran el acoplamiento:

- `jugada` es un contador de jugadas, un centinela de bucle (`0`) y un valor de caso
  especial (`2`);
- `ganador` es solo una bandera; `cab` dice quién ganó;
- `cab1` y `cf1`/`cc1` son dos copias de una posición, sincronizadas a mano.

Por eso el grafo de llamadas subestima la dependencia. `impresion` parece una hoja, pero lee
el tablero, ambos caballos y la posición del ganador.

## Consecuencia

Cada cambio de regla se convierte en una edición de varias funciones. El orden de turnos se
fija en `cabubicacion`, se consume en `impresionorden` y se vuelve a leer dentro de `movcab`.
Una prueba necesita el programa completo y una consola. Un incidente no se puede reproducir
desde una sola entrada, porque el estado que falla no tiene una frontera donde imprimirlo.

Esto no es comportamiento indefinido por sí mismo. La escritura `i <= 64` es `failure-001`;
los globales son un costo de diseño, no un error de memoria.

## Prueba que lo detecta

`archaeology/lessons/archaeology-05-global-state/scan_globals.sh` produce las líneas de
declaración y los conteos de menciones. La prueba de comportamiento es la repetición que
propone la lección: extrae un turno en una copia de trabajo y llámalo dos veces con los mismos
argumentos. Si la segunda llamada difiere, la diferencia es el estado oculto. El
`GLOBAL-STATE-MAP-TEMPLATE.md` completado es el registro: una fila por global y una fila por
función.

El escáner no demuestra una mutación. Cuenta menciones; el mapa es la evidencia.

## Arreglo mínimo

Crea una sola costura. Haz que `movcab` declare sus entradas y devuelva su resultado:

```cpp
struct MoveResult {
    bool moved;
    bool captured;
    int  cell;
};

MoveResult movcab(int &cf, int &cc, int knight, int moveNumber,
                  const int (&order)[8], int board[64]);
```

Mueve la impresión a `impresionorden`. Esto no elimina los globales; hace que una función se
pueda llamar sin la consola y sin leer al llamador.

## Arreglo estructural

Dale un dueño al estado. Un registro `GameState` guarda el tablero, ambos caballos, el número
de jugada y el resultado. Un `Board` posee sus casillas y un `Knight` posee su posición, así
que `cab1` y `cf1`/`cc1` no pueden desviarse. Cada función recibe el estado que necesita como
parámetro.

Peldaños de la pista A: [`construction-11-encapsulation`][c11] para el invariante privado,
[`construction-13-composition`][c13] para el tablero dentro de la partida, y
[`construction-08-modular`][c08] para la frontera que hace visible al dueño.

[c08]: ../../construction/lessons/construction-08-modular/lesson.md
[c11]: ../../construction/lessons/construction-11-encapsulation/lesson.md
[c13]: ../../construction/lessons/construction-13-composition/lesson.md

## Confianza

Alta en que las funciones citadas leen y escriben los globales nombrados. Media en el orden
de las mutaciones en ejecución: el mapa se lee de la fuente, no se traza con un depurador.

## Nota sobre el archivo

Esta tarjeta es evidencia, no una reparación del archivo. El archivo permanece de solo
lectura; cualquier arreglo va en una copia de trabajo o en `reference/`, nunca en `original/`.
