# failure-011 — El movimiento se selecciona por número de jugada, no por tipo de pieza

> [English](failure-011-switch-on-move-number.md) |
> [Español](failure-011-switch-on-move-number-es.md)

- **Clase:** diseño
- **Severidad:** media
- **Concepto violado:** una pieza debe poseer su regla; el bucle no debe enumerar tipos
- **Archivo:** etiqueta `v1.0.0`,
  `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`

## Evidencia

- **Variante:** `variant-01-baseline-2008`. La misma forma está en `variant-02:290`,
  `variant-03:316` y `variant-04:330`.
- **Líneas:** 29, 239, 249 y 765.
- **Código:**

```cpp
int cf1,cc1,cf2,cc2,cab;                    // línea 29

switch(vecjugadas[i])                       // línea 239
{
case 1:
    // ...
    if (cab==1)                             // línea 249
    {
        // la rama del caballo 1
    }
    // ... el mismo bloque para el otro caballo ...
}

cab=1;                                      // línea 765, uno de cuatro bloques de turno
```

El archivo **no tiene un switch sobre el tipo de pieza**. Tiene un switch sobre el número de
jugada y un entero `cab` con ramas `if (cab==1)`. La decisión de tipo está dispersa: nueve
sitios `if (cab==1)` en `variant-01`, diez en las otras variantes, más cuatro bloques de turno
en 765, 790, 820 y 844.

La abreviatura del catálogo, "switch sobre el tipo de pieza", describe el olor de diseño, no
la fuente. Cita la fuente: el switch es sobre `vecjugadas[i]`, y la pieza es `cab`.

## Síntoma

El programa funciona con dos caballos idénticos. Añadir una tercera pieza no es un cambio de
una línea. Exige un tercer par de coordenadas, un tercer contador, una tercera rama en cada
sitio `cab==1`, un cuarto bloque de turno y una regla de movimiento para la pieza nueva. El
switch sobre el número de jugada no sabe que la pieza existe; el bucle lo sabe en nueve
lugares.

El lector no puede responder "¿dónde está la regla de movimiento de un caballo?" con una sola
ubicación. La regla está repartida entre los casos del número de jugada y las ramas de turno.

## Causa raíz

La pieza no es un tipo. Es dos pares de coordenadas globales, dos índices planos y un entero
selector. El bucle nombra cada caballo a mano, y `movcab(int &cf, int &cc)` recibe coordenadas
en lugar de una pieza. El switch sobre el número de jugada responde *adónde* va la pieza; las
ramas `cab` responden *de quién* son las coordenadas. Ambas decisiones viven en el mismo
cuerpo.

Una tabla de deltas indexada por número de jugada sigue siendo despacho por índice. Elimina
aritmética copiada; no crea una frontera alrededor de una pieza. Por eso `failure-008` y esta
tarjeta son reparaciones distintas.

## Consecuencia

Una pieza nueva edita la función de movimiento compartida y el bucle de turnos. Un cambio en la
regla de una pieza arriesga a la otra, porque ambas comparten los mismos cuerpos de caso. Una
prueba debe fijar `cab`, las coordenadas, el tablero y el orden de jugadas para ejercitar un
turno.

El programa es correcto para dos caballos. El costo lo paga la siguiente pieza, no la ejecución
actual.

## Prueba que lo detecta

`archaeology/lessons/archaeology-10-polymorphism/run.sh` compara una base con switch sobre el
tipo con una versión con despacho para 2 piezas × 8 movimientos × 25 posiciones:

```text
comparisons=400 mismatches=0
```

La lección añade después un camello a cada camino y cuenta los lugares tocados. La adición con
despacho es una tabla de deltas y una clase; la adición con switch es un valor de enumeración y
una rama dentro de la función compartida. Los conteos de líneas son parecidos; la diferencia
está en dónde cae la edición.

La base reconstruye la decisión de tipo. No es código de 2008 literal, y los dos caballos del
archivo se mueven igual. La prueba demuestra el reemplazo para 400 entradas muestreadas, no el
comportamiento del archivo.

## Arreglo mínimo

Introduce un tipo de pieza y deja que responda la pregunta de movimiento:

```cpp
class Piece
{
public:
    virtual ~Piece() = default;
    virtual Position move(int moveNumber, Position position) const = 0;
    virtual const char *role_name() const = 0;
};
```

`Knight` guarda su propia tabla de deltas. El bucle mantiene `Piece *` y deja de nombrar `cab`.
Conserva la tabla de números de jugada dentro de la clase; la tabla sigue respondiendo *adónde*,
el tipo responde *quién*.

## Arreglo estructural

Mueve posición, identidad y movimiento a la pieza, y deja que el bucle mantenga `Piece &` sin
coordenadas:

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

Ahora `cab` desaparece, y `movcab(int &cf, int &cc)` se sustituye por una llamada a la pieza.
El bucle de turnos deja de enumerar tipos.

Peldaños de la pista A: [`construction-11-encapsulation`][c11] para el estado privado,
[`construction-12-polymorphism`][c12] para la interfaz, y
[`construction-13-composition`][c13] para la siguiente pregunta: ¿una pieza debe *tener* una
regla de movimiento en lugar de *ser* una?

[c11]: ../../construction/lessons/construction-11-encapsulation/lesson.md
[c12]: ../../construction/lessons/construction-12-polymorphism/lesson.md
[c13]: ../../construction/lessons/construction-13-composition/lesson.md

## Confianza

Alta en que el archivo no tiene un switch sobre el tipo de pieza y en que `cab` es un selector
entero. Alta en que existen los nueve sitios `if (cab==1)` en `variant-01`. Media en el conteo
exacto de ediciones para una tercera pieza: depende de cuántos bloques de turno y sitios de
impresión toque también el cambio.

## Nota sobre el archivo

Esta tarjeta es evidencia, no una reparación del archivo. El archivo permanece de solo
lectura; el tipo de pieza va en una copia de trabajo o en `reference/`.
