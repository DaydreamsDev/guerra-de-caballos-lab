# failure-004 — La inicialización con punteros solo limpia la primera casilla

> [English](failure-004-broken-initialization.md) |
> [Español](failure-004-broken-initialization-es.md)

- **Clase:** corrección
- **Severidad:** alta
- **Concepto violado:** un bucle de inicialización debe avanzar el cursor
- **Archivo:** etiqueta `v1.0.0`,
  `original/variant-03-pointers-dev-2008/source/guerra-de-caballos-pointers-dev.cpp`

## Evidencia

- **Variante:** `variant-03-pointers-dev-2008`.
- **Líneas:** 84–86.
- **Código:**

```cpp
for (i=0;i<=64;i++)
{
    pvectab=&vectab[0];         // línea 84
    *pvectab=*(pvectab+i);      // línea 85
    pvectab->mvectab=0;         // línea 86
}
```

`pvectab` es un `struct stablero *` y `vectab` es `struct stablero vectab[64]`.

## Síntoma

Solo se limpia `vectab[0]`. Las casillas 1 a 63 conservan lo que tenían antes de la llamada. En un
proceso nuevo los globales se inicializan a cero, así que el tablero parece correcto y el fallo es
invisible. En una segunda partida, en un tablero reutilizado o en una prueba que rellena el arreglo
antes de llamar, los valores viejos permanecen.

## Causa raíz

El puntero se reinicia a `&vectab[0]` dentro de cada iteración, así que cada escritura apunta al
elemento 0. La línea `*pvectab=*(pvectab+i)` tampoco avanza el cursor: copia el elemento `i` en el
elemento 0, y la línea siguiente sobrescribe esa copia con cero. El bucle pretendido era
probablemente `pvectab = &vectab[i]; pvectab->mvectab = 0;`, pero el reinicio quedó dentro del
cuerpo.

## Consecuencia

La inicialización no hace nada para 63 de las 64 casillas. Una segunda ejecución empieza con los
números de jugada de la ejecución anterior, así que el estado del juego es incorrecto de una forma
que parece un error de jugabilidad. El fallo es caro porque una demostración nueva pasa: el defecto
se esconde hasta que el tablero se reutiliza.

## Prueba que lo detecta

`broken_initialization_test()` en `tests/failure_tests.cpp`. Crea un arreglo de ocho unos, reinicia
el puntero al primer elemento en cada iteración y limpia a través de él. Devuelve `true` cuando el
índice 0 es cero y el índice 1 sigue siendo uno, que es la firma del reinicio.
`tests/run_failure_tests.sh` imprime `failure-004=detected`.

La prueba reproduce el reinicio del puntero, no la estructura `stablero` ni el arreglo original.

## Arreglo mínimo

Quita el reinicio y la copia; indexa el arreglo directamente:

```cpp
for (i = 0; i < 64; i++)
{
    vectab[i].mvectab = 0;
}
```

Esto también elimina el límite `<=` de failure-001 en esta función. Las dos tarjetas se solapan en
`variant-03:82`; mantén los arreglos separados en tu registro para que el razonamiento siga visible.

## Arreglo estructural

Usa `std::array<stablero, 64>` y `fill` o un bucle basado en rango. El contenedor es dueño de su
longitud y no hay cursor que reiniciar por accidente.

Peldaño de la pista A:
[`construction-05-array-of-records`][c05] (arreglo de registros) y
[`construction-07-pointers`][c07] (aritmética de punteros y tiempo de vida).

[c05]: ../../construction/lessons/construction-05-array-of-records/lesson.md
[c07]: ../../construction/lessons/construction-07-pointers/lesson.md

## Confianza

Alta. El reinicio, la copia y la escritura son visibles en las líneas 84–86. El efecto de "solo la
primera casilla" se deduce del flujo de control, y la prueba lo reproduce.

## Nota sobre la prueba

La prueba es una reproducción aislada del patrón del fallo. No es una prueba del programa original.
Usa enteros simples para que el error de punteros sea visible sin el resto del juego.
