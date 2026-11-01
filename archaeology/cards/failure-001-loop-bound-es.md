# failure-001 — El límite del bucle escribe un elemento fuera del tablero

> [English](failure-001-loop-bound.md) | [Español](failure-001-loop-bound-es.md)

- **Clase:** corrección
- **Severidad:** crítica
- **Concepto violado:** el límite es parte del tipo
- **Archivo:** etiqueta `v1.0.0`,
  `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`

## Evidencia

- **Variante:** `variant-01-baseline-2008`. El mismo patrón aparece en `variant-02:72`,
  `variant-03:82` y `variant-04:88`.
- **Línea:** 61.
- **Código:**

```cpp
int vectab[64];          // línea 17

for (i=0;i<=64;i++)      // línea 61
{
    vectab[i]=0;
}
```

La condición es `<=`, así que el cuerpo se ejecuta para `i = 0..64`: 65 iteraciones sobre un arreglo
de 64 elementos. La última escritura es `vectab[64]`, un elemento después del final.

## Síntoma

El programa inicializa un tablero de 64 casillas y luego escribe una casilla 65. Nada en la salida
informa de la escritura extra. El programa suele continuar, y eso es lo que hace peligroso el fallo:
el daño es silencioso hasta que deja de serlo.

## Causa raíz

El tamaño del arreglo y el límite del bucle son dos literales separados. El límite se escribió desde
un modelo mental basado en 1, o se copió de un bucle que cuenta 64 elementos de forma inclusiva, y
`<` se convirtió en `<=`. El compilador no puede comparar `i` con la longitud del arreglo porque la
longitud no forma parte de la expresión del bucle. Un segundo literal puede desviarse sin aviso.

## Consecuencia

Comportamiento indefinido. En esta unidad de traducción `vectab` es un arreglo global, así que la
escritura extra puede caer sobre `vecjugadas`, sobre otro global o sobre relleno. En una copia donde
el tablero sea local, puede corromper el canario de la pila o un objeto adyacente. Los resultados
visibles van desde ningún síntoma hasta un tablero incorrecto, un bloqueo o un hallazgo de
seguridad. AddressSanitizer lo reporta de inmediato; una demostración lo oculta durante años.

## Prueba que lo detecta

`loop_bound_test()` en `tests/failure_tests.cpp`. Crea un `std::array` de 65 elementos, escribe el
valor centinela `7` en el índice 64, ejecuta el mismo bucle `<= 64` y devuelve `true` cuando el
centinela fue borrado. El elemento 65 hace que la reproducción esté definida; la escritura 65 del
original no lo está. `tests/run_failure_tests.sh` imprime `failure-001=detected` cuando la prueba
devuelve `true`.

La prueba verifica el límite, no el diseño de memoria del original. No compila ni ejecuta el archivo
archivado.

## Arreglo mínimo

```cpp
for (i = 0; i < 64; i++)
{
    vectab[i] = 0;
}
```

Un carácter elimina la escritura fuera de límites. No impide que el siguiente bucle repita el mismo
error.

## Arreglo estructural

Guarda el tablero en `std::array<int, 64>` y límpialo con `board.fill(0)`, o recórrelo con un `for`
basado en rango. El límite pasa a venir del tipo y no se puede escribir mal.

Peldaño de la pista A:
[`construction-02-structured`][c02] y [`construction-17-stl`][c17].

[c02]: ../../construction/lessons/construction-02-structured/lesson.md
[c17]: ../../construction/lessons/construction-17-stl/lesson.md

## Confianza

Alta. La declaración, el bucle y la escritura son visibles en las líneas 17 y 61. El comportamiento
indefinido se deduce de la regla del lenguaje, no de una suposición sobre la ejecución.

## Nota sobre la prueba

La prueba es una reproducción aislada del patrón del fallo. No es una prueba del programa original,
y una ejecución correcta no dice nada sobre el binario de 2008.
