# failure-005 — Condición de victoria invertida en el precursor de 2007

> [English](failure-005-inverted-win.md) | [Español](failure-005-inverted-win-es.md)

- **Clase:** corrección
- **Severidad:** alta
- **Concepto violado:** la condición de final y el ganador son una sola decisión
- **Archivo:** etiqueta `v1.0.0`,
  `original/variant-00-precursor-2007/source/ajedrez-caballo-2007.txt`

## Evidencia

- **Variante:** `variant-00-precursor-2007`.
- **Línea:** 299, dentro de `pruebatriunfo()`; la condición está en las líneas 296–297.
- **Código:**

```cpp
if( tablero[peon1f][peon1c]!=-1&&tablero[peon2f][peon2c]!=-1
&&tablero[peon3f][peon3c]!=-1&&tablero[peon4f][peon4c]!=-1 )
{
    cout<<"\n\nTRIUNFO PARA LA MIQUINA :D\n\n";   // línea 299
    a=0;
}
```

El centinela de un peón es `-1`. Un peón comido ya no está en su casilla original, así que la
casilla deja de valer `-1`. Las cuatro comprobaciones `!= -1` son verdaderas cuando los cuatro
peones han sido comidos.

## Síntoma

Cuando se captura el último peón, el programa imprime `TRIUNFO PARA LA MIQUINA` y se detiene. La
única pieza que se mueve y captura es el caballo; el mensaje le atribuye la victoria a la máquina.
Quien juega ve al caballo comerse el último peón y luego lee que ganó la máquina.

## Causa raíz

La condición de final y la etiqueta del ganador se escribieron por separado. La condición son
cuatro comprobaciones negadas del centinela; la etiqueta es una cadena fija. No hay un valor
`Outcome` ni un modelo de jugador que diga a qué lado pertenece el caballo. El comentario sobre la
función dice que la máquina se come los peones, pero los comentarios de este archivo describen a
veces un comportamiento viejo o imaginado. El código nunca deduce el ganador del lado que movió.

## Consecuencia

El juego informa un resultado incorrecto. Cualquier marcador, transcripción o prueba que lea el
mensaje está mal. Las cuatro negaciones también hacen fácil volver a invertir el cambio siguiente:
quien lee tiene que simular el centinela para saber si la condición significa "todos presentes" o
"todos comidos". El defecto es de corrección, no una preferencia de presentación.

## Prueba que lo detecta

`inverted_win_test()` en `tests/failure_tests.cpp`. Asigna `allPawnsEaten = true` a la cadena
`"machine"` y devuelve `true` cuando ese resultado no es `"player"`. La prueba reproduce la
asignación que diagnostica la tarjeta: el lado ganador está fijado a la máquina.
`tests/run_failure_tests.sh` imprime `failure-005=detected`.

La prueba no construye el tablero, no mueve el caballo ni lee el código original. Aísla la tabla de
decisión.

## Arreglo mínimo

Nombra el predicado, declara el ganador de forma explícita y haz la condición positiva:

```cpp
const bool allPawnsCaptured =
    tablero[peon1f][peon1c] != -1 &&
    tablero[peon2f][peon2c] != -1 &&
    tablero[peon3f][peon3c] != -1 &&
    tablero[peon4f][peon4c] != -1;

if (allPawnsCaptured)
{
    cout << "\n\nTRIUNFO PARA EL JUGADOR :D\n\n";
    a = 0;
}
```

Si la máquina es dueña del caballo, conserva la etiqueta de la máquina y renombra el predicado a
`allPawnsCaptured`. La idea es deducir la etiqueta del actor, no fijarla. El arreglo mínimo elimina
el ganador incorrecto; no añade un modelo completo de resultado.

## Arreglo estructural

Representa el resultado como un valor, no como una cadena impresa dentro de un predicado:

```cpp
enum class Outcome { Playing, PlayerWins, MachineWins, Trap };
Outcome evaluateOutcome(const Board &board);
```

Una función decide el resultado; la capa de presentación traduce `Outcome` a un mensaje. La decisión
se puede probar sin `cout`.

Peldaño de la pista A: [`construction-10-adt-invariants`][c10]
(invariantes y una sola fuente de verdad para el estado).

[c10]: ../../construction/lessons/construction-10-adt-invariants/lesson.md

## Confianza

Alta en que la condición son cuatro comprobaciones `!= -1` y en que el mensaje está fijado a la
máquina. Alta en que el código se activa cuando los cuatro peones han sido comidos. Media en el
ganador pretendido, porque el código no tiene modelo de jugador y el comentario puede
estar equivocado. La tarjeta registra la discrepancia; la intención es una inferencia.

## Nota sobre la prueba

La prueba es una reproducción aislada del patrón del fallo. No es una prueba del programa original.
Codifica la asignación que diagnostica la tarjeta, no el estado del tablero que la activa.
