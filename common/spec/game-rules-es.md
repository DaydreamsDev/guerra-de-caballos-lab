# Reglas del juego — el invariante

> [English](game-rules.md) | [Español](game-rules-es.md)

Este documento es la única fuente de verdad del comportamiento del juego. Toda implementación, traza
y lección debe coincidir con él.

El archivo histórico es evidencia. Esta especificación define la referencia determinista que utiliza
el laboratorio.

## 1. Tablero

- El tablero es una cuadrícula de 8×8.
- Las filas se numeran de 1 a 8 de arriba hacia abajo.
- Las columnas se numeran de 1 a 8 de izquierda a derecha.
- Una casilla se escribe `row,column`, por ejemplo `3,5`.

## 2. Piezas

Hay dos caballos:

- `player` — el caballo del aprendiz.
- `machine` — el caballo de la máquina.

Cada caballo ocupa exactamente una casilla.

## 3. Posiciones iniciales

Dada una semilla:

1. Extraer `player_start` de forma uniforme de 0 a 63.
2. Extraer `machine_start` de forma uniforme de 0 a 63.
3. Si las posiciones son iguales, volver a extraer `machine_start` hasta que difieran.
4. Convertir un número `n` en `row = n // 8 + 1` y `column = n % 8 + 1`.

Las casillas iniciales están ocupadas, pero no se marcan como visitadas.

## 4. Quién comienza

1. Extraer `roll_player` de forma uniforme de 1 a 50.
2. Extraer `roll_machine` de forma uniforme de 1 a 50.
3. Si las tiradas son iguales, volver a extraer `roll_machine` hasta que difieran.
4. Comienza el caballo con la tirada más alta.

## 5. Orden de turnos

En cada turno, el caballo activo es aquel al que le corresponde jugar. Tras un movimiento exitoso,
el turno pasa al otro caballo.

Una captura o un caballo atrapado termina la partida de inmediato.

## 6. Prioridad de movimientos

En cada turno, antes de probar movimientos:

1. Extraer `start` de forma uniforme de 1 a 8.
2. El orden de movimientos es `start, start + 1, …, 8, 1, …, start - 1`.

El caballo activo prueba los movimientos en ese orden y toma el primer movimiento legal.

## 7. Movimientos del caballo

| Movimiento | Cambio de fila | Cambio de columna | Descripción |
|---|---|---|---|
| 1 | -1 | +2 | Una fila arriba, dos columnas a la derecha |
| 2 | -2 | +1 | Dos filas arriba, una columna a la derecha |
| 3 | -2 | -1 | Dos filas arriba, una columna a la izquierda |
| 4 | -1 | -2 | Una fila arriba, dos columnas a la izquierda |
| 5 | +1 | -2 | Una fila abajo, dos columnas a la izquierda |
| 6 | +2 | -1 | Dos filas abajo, una columna a la izquierda |
| 7 | +2 | +1 | Dos filas abajo, una columna a la derecha |
| 8 | +1 | +2 | Una fila abajo, dos columnas a la derecha |

Un movimiento es legal cuando:

- el destino está dentro del tablero;
- el destino no es una casilla ya visitada por alguno de los dos caballos.

La única excepción es la casilla actual del otro caballo: entrar en ella es una captura y es legal.

## 8. Casillas visitadas

- Una casilla pasa a estar visitada cuando un caballo entra en ella.
- Las casillas iniciales no están visitadas hasta que un caballo entra en ellas más adelante.
- Un caballo puede entrar en su propia casilla inicial si no está visitada por otra razón.
- Un caballo no puede entrar en ninguna casilla visitada, salvo para capturar al otro caballo.

## 9. Captura

Si el caballo activo entra en la casilla actual del otro caballo, el caballo activo gana de
inmediato.

## 10. Trampa

Si el caballo activo no tiene ningún movimiento legal, el caballo activo pierde y el otro caballo
gana.

## 11. Empate

Si se han jugado 200 movimientos exitosos y ningún caballo ha ganado, la partida es un empate.

## 12. Aleatoriedad

La referencia usa un generador lineal congruencial de 64 bits. El algoritmo es lo bastante pequeño
para implementarlo en cualquier lenguaje.

Estado:

```text
state = seed mod 2^64
```

Constantes:

```text
multiplier = 6364136223846793005
increment  = 1442695040888963407
modulus    = 2^64
```

Operaciones:

```text
next_value():
    state = (state * multiplier + increment) mod 2^64
    return state

random_below(bound):
    return next_value() mod bound

random_integer(minimum, maximum):
    return minimum + random_below(maximum - minimum + 1)
```

Orden de llamadas:

1. `player_start = random_below(64)`
2. `machine_start = random_below(64)`, vuelto a extraer hasta que sea distinto de `player_start`
3. `roll_player = random_integer(1, 50)`
4. `roll_machine = random_integer(1, 50)`, vuelto a extraer hasta que sea distinto de
   `roll_player`
5. Para cada turno: `start = random_integer(1, 8)`

No se permite ninguna otra llamada aleatoria entre estos pasos. La operación de módulo es parte de
la especificación; las implementaciones no deben añadir muestreo por rechazo.

## 13. Transcripción

Cada ejecución produce una transcripción con el formato definido en
[`transcript-format-es.md`](transcript-format-es.md).

La transcripción es el contrato de comportamiento. Dos implementaciones coinciden cuando producen
la misma transcripción para la misma semilla y el mismo nombre de jugador.

## 14. Implementación de referencia

La implementación de referencia es:

```text
common/harness/reference.py
```

Si la implementación y esta especificación no coinciden, prevalece la especificación y se corrige la
implementación.
