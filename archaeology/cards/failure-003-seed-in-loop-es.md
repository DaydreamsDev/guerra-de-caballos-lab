# failure-003 — `srand(time(NULL))` dentro del bucle de reintento de colocación

> [English](failure-003-seed-in-loop.md) | [Español](failure-003-seed-in-loop-es.md)

- **Clase:** corrección
- **Severidad:** alta
- **Concepto violado:** la aleatoriedad tiene un punto de entrada y una semilla
- **Archivo:** etiqueta `v1.0.0`,
  `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`

## Evidencia

- **Variante y línea:** `variant-01:76`, `variant-02:87`, `variant-03:99`, `variant-04:105`.
- **Código:**

```cpp
int i = 1;
while (i == 1)
{
    srand(time(NULL));          // línea 76
    tirocab1 = rand() % 51;
    tirocab2 = rand() % 51;
    cab1 = rand() % 64;
    cab2 = rand() % 64;
    if (cab1 != cab2 && tirocab1 != tirocab2)
    {
        i++;
    }
}
```

La semilla se fija dentro del `while` que reintenta una colisión, no una vez antes del bucle.

## Síntoma

Dos ejecuciones en segundos distintos colocan los caballos de forma diferente. Una ejecución con un
parche de semilla fija puede repetir la misma colisión para siempre: el bucle vuelve a sembrar el
mismo valor en cada intento, obtiene los mismos cuatro números, falla la misma condición y lo
intenta otra vez.

## Causa raíz

La semilla está acoplada al reloj y al camino de reintento. El código original quería una secuencia
distinta en cada intento, así que puso la llamada donde empieza el intento. Eso reinicializa el
generador en lugar de avanzarlo. El bucle solo puede progresar si el reloj cambia entre iteraciones,
lo que convierte una colisión en una espera de hasta un segundo.

## Consecuencia

Colocación no reproducible. Dos ejecuciones no se pueden comparar, no se puede capturar una traza
dorada sin parchear y una prueba que dependa de la colocación es inestable. El reintento puede girar
un tic de reloj por colisión; con una semilla fija puede colgarse. `archaeology-03` registra la
semilla 9999 como `run_status=124`: la compilación parcheada nunca sale de este bucle.

## Prueba que lo detecta

`seed_in_loop_test()` en `tests/failure_tests.cpp`. Usa un generador congruencial lineal pequeño, lo
siembra con `1`, guarda el primer valor, vuelve a sembrar con `1` tres veces y comprueba si el
siguiente valor se repite. Devuelve `true` cuando la secuencia se reinicia, que es exactamente el
comportamiento que hace repetir el bucle de colocación. `tests/run_failure_tests.sh` imprime
`failure-003=detected`.

La prueba detecta el sembrado repetido. No llama a `rand()` y no reproduce el cuelgue.

## Arreglo mínimo

Mueve la llamada de semilla antes del bucle, o quítala del camino de reintento:

```cpp
srand(time(NULL));
while (i == 1)
{
    // sortear y reintentar
}
```

Mejor aún: recibe la semilla como parámetro para que quien llama decida si la ejecución es
reproducible.

## Arreglo estructural

Inyecta la fuente aleatoria en lugar de llamar al generador global desde dentro de la lógica de
colocación. Una interfaz pequeña `RandomSource` con una semilla y un método `next()` permite que
producción use el reloj y que las pruebas usen una secuencia fija.

Peldaño de la pista A:
[`construction-03-functions`](../../construction/lessons/construction-03-functions/lesson.md)
(parámetros en lugar de estado oculto) y
[`construction-20-hexagonal`](../../construction/lessons/construction-20-hexagonal/lesson.md)
(el generador como adaptador en la frontera).

## Confianza

Alta para la ubicación del código y la no reproducibilidad. Alta para el sembrado repetido. Media
para el tiempo exacto de espera, que depende de la resolución del reloj y del número de colisiones.

## Nota sobre la prueba

La prueba es una reproducción aislada del patrón del fallo. No es una prueba del programa original.
El generador congruencial lineal sustituye al generador de la plataforma porque su reinicio es fácil
de observar.
