# failure-012 — `movcab` imprime y espera teclas dentro de la lógica de movimiento

> [English](failure-012-io-inside-logic.md) | [Español](failure-012-io-inside-logic-es.md)

- **Clase:** diseño
- **Severidad:** media
- **Concepto violado:** el núcleo decide la partida; un adaptador decide la consola
- **Archivo:** etiqueta `v1.0.0`,
  `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`

## Evidencia

- **Variante:** `variant-01-baseline-2008`. `variant-02` añade el lado de entrada.
- **Líneas:** 234–724, 262, 704, 712, 42–43 y 781–864.
- **Código:**

```cpp
int movcab(int &cf,int &cc)                 // línea 234
{
    // ...
    cout<<" 1.";                            // línea 262
    // ...
    cout<<"No hay movimientos posibles ..."; // línea 704
    impresion();                            // línea 705
}

getch();                                    // línea 42, y 781, 806, 835, 861
system("cls");                              // línea 43, y 784, 809, 838, 864
system("color 1f");                         // línea 38
```

`movcab` ocupa unas 491 líneas. Diecinueve contienen `cout`. Mueve un caballo, decide si la
partida terminó y narra ambas cosas. El bucle de turnos limpia la pantalla y espera una tecla
antes del siguiente turno.

## Síntoma

Para observar un movimiento hay que ejecutar el programa, pasar por los borrados de pantalla y
pulsar una tecla. No hay una función que devuelva el resultado de un movimiento. Una prueba no
puede llamar a la regla de movimiento, porque la llamada necesita una terminal.

La salida está mezclada con el cálculo. La misma función que escribe `vectab[ccab]` también
imprime el número de caso. Un movimiento fallido imprime un mensaje y termina la partida desde
dentro de la función de movimiento.

## Causa raíz

La consola era el producto, así que imprimir desde `movcab` evitaba pasar un tablero y una
transcripción. `getch()` y `system("cls")` eran los controles disponibles. El código nunca
separó el dispositivo de la política: la función de movimiento posee la pantalla, la pausa y la
narración.

El acoplamiento no es solo `cout`. En `variant-02`, el carácter de dibujo `pisadas` atraviesa
`cabubicacion`, `ordenjuego` y `movcab`. Un detalle de presentación contamina tres firmas del
dominio.

## Consecuencia

El núcleo no puede ejecutarse sin consola. Una prueba unitaria, un script, una herramienta de
repetición o una segunda interfaz deben reimplementar el programa o simular la terminal. El
formato de salida no está fijado, así que un cambio de redacción puede romper una prueba que
cuenta líneas.

El programa es correcto para una persona frente a una consola. El costo lo paga el primer
llamador que no es una consola.

## Prueba que lo detecta

`archaeology/lessons/archaeology-11-input-output/run.sh` construye un binario de consola y un
binario de prueba. El de prueba enlaza `core.cpp` y `test.cpp` solamente; nunca ve
`console.cpp`:

```text
console_lines=8
test_output=lines=8 winner=player moves=3
test=ok
match=yes
```

La lección demuestra que el núcleo no tiene dependencia de consola a nivel de objeto:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 -c code/core.cpp -o build/core.o
nm -C -u build/core.o | grep -E 'printf|cout|getch|fwrite|puts'
```

El filtro no imprime nada y sale con 1. Ese es el resultado: el objeto del núcleo no tiene
símbolos de consola sin resolver.

La base no es el archivo. Aísla la separación; no llama a `movcab`.

## Arreglo mínimo

Devuelve un resultado y mueve la impresión al llamador:

```cpp
struct MoveResult
{
    bool moved;
    bool captured;
    int cell;
};

MoveResult movcab(int &cf, int &cc);
```

`movcab` deja de imprimir. El llamador decide qué mostrar. Esta es la forma de la base:
`run_game` devuelve un `GameResult` y `console.cpp` lo imprime.

## Arreglo estructural

Define el resultado como dato del dominio, no como texto. Dale a la consola un renderizador, a
la prueba un adaptador de aserciones y al punto de entrada un adaptador de entrada:

```text
argv / stdin ──► adaptador de entrada ──► run_game ──► GameResult ──► adaptador de consola
                                                              └─────► adaptador de prueba
```

El formato vive en un solo lugar y la política vive donde corresponde. Una prueba puede enlazar
el núcleo sin terminal.

Peldaños de la pista A: [`construction-20-hexagonal`][c20] para puertos y adaptadores, y
[`construction-08-modular`][c08] para la frontera de módulo que hace posible un núcleo.

[c08]: ../../construction/lessons/construction-08-modular/lesson.md
[c20]: ../../construction/lessons/construction-20-hexagonal/lesson.md

## Confianza

Alta en que `movcab` imprime y en que el bucle de turnos espera teclas. Alta en que el núcleo no
se puede llamar sin consola. Media en el conteo exacto de líneas `cout` en otras variantes: los
conteos vienen de `grep -c` sobre los archivos fijados.

## Nota sobre el archivo

Esta tarjeta es evidencia, no una reparación del archivo. El archivo permanece de solo
lectura; la separación va en una copia de trabajo o en `reference/`.
