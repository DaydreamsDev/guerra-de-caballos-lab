# construction-23 — Línea base del port

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** la sintaxis es desechable · **Prerrequisito:**
> construction-09 o construction-12 · **Tiempo estimado:** 195 minutos

Este directorio contiene el artefacto **después** de `construction-23`: un port del juego a Python.
Produce la misma transcripción que la línea base de C++, byte a byte, desde las mismas veinte
semillas.

## Qué implementa el port

- El generador lineal congruencial de 64 bits de `common/spec/game-rules.md`.
- El tablero de 8×8, los dos caballos, las posiciones iniciales y las tiradas iniciales.
- La prioridad de movimientos, los movimientos legales e ilegales, las casillas visitadas, la
  captura, la trampa y el empate a los 200 movimientos.
- El formato de transcripción de `common/spec/transcript-format.md`, byte a byte.

## Archivos

| Archivo | Propósito |
|---|---|
| `code/game.py` | El port: secuencia aleatoria, reglas del tablero, bucle y transcripción. |
| `code/main.py` | Análisis de argumentos y punto de entrada del proceso. |
| `run.sh` | Ejecuta `main.py` con el intérprete de Python del sistema. |
| `README.md` | Este archivo. |
| `README-es.md` | Edición en español. |

## El port

La línea base de C++ tiene una clase `Board`, objetos `Knight`, un `TranscriptPort`, dos
adaptadores y un `MoveResult`. El port de Python conserva las reglas y elimina la ceremonia:

- `RandomSequence` es una clase pequeña con un entero `state`.
- Las posiciones son `tuple[int, int]`.
- Las casillas visitadas son un `set[tuple[int, int]]`.
- La transcripción es una lista de cadenas que se unen al final.
- El bucle del juego es una función, `run_game`.

Las constantes del LCG, el orden de movimientos, la regla de captura, la regla de casillas
visitadas, la trampa y el empate a 200 movimientos son los mismos. La transcripción es la misma.
Los tipos, la propiedad y las comprobaciones en tiempo de compilación no lo son.

## Compilar y ejecutar

Se requiere Python 3.10 o posterior. El port usa `tuple[int, int]` y `str | None`.

```bash
./run.sh --seed 23 --name Learner
```

No hay paso de compilación ni paquete de terceros.

## Verificar las trazas de referencia

Desde este directorio:

```bash
for trace in ../../../common/traces/seed-*.txt; do
    traceName="$(basename "${trace}" .txt)"
    seedText="${traceName#seed-}"
    seedNumber="$((10#${seedText}))"
    ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

Las 20 trazas, de `seed-0001.txt` a `seed-0020.txt`, coinciden byte a byte.

## Prueba de cambio

Ejecutar el port de Python con las mismas 20 semillas que la línea base de C++. Comparar cada
transcripción. Después cambiar una regla solo en el port, observar qué trazas fallan y restaurarla.
La sintaxis es desechable; las reglas y la transcripción son el contrato.

## Criterio de aceptación

La persona que aprende puede ejecutar el port de Python, hacer coincidir las 20 trazas de referencia
y nombrar al menos tres cosas que cambiaron en el port y tres que no.
