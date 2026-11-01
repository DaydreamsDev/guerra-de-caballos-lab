# Arnés determinista

> [English](README.md) | [Español](README-es.md)

El arnés convierte una semilla en una transcripción. Es la forma ejecutable de
[`../spec/game-rules.md`](../spec/game-rules.md) y el productor de las trazas de referencia de
[`../traces/`](../traces/).

## Requisitos

- Python 3.8 o superior.
- Ningún paquete de terceros. La implementación usa solo la biblioteca estándar.

No hay paso de compilación.

## Línea de comandos

```text
reference.py --seed INTEGER --name TEXT [--max-moves INTEGER]
```

| Opción | Obligatoria | Valor por defecto | Significado |
|---|---|---|---|
| `--seed` | sí | — | Entero usado como estado inicial del generador determinista. |
| `--name` | sí | — | Nombre del jugador escrito en la línea `player=`. |
| `--max-moves` | no | `200` | Movimientos exitosos permitidos antes de declarar tablas. |

Ejemplos:

```bash
python3 common/harness/reference.py --seed 1 --name Learner
common/harness/run.sh --seed 1 --name Learner
```

`run.sh` es un envoltorio mínimo que localiza `reference.py` respecto de sí mismo, así que
funciona desde cualquier directorio de trabajo. Reenvía cada argumento sin cambios.

El estado de salida es `0` cuando se escribe una transcripción. Los argumentos inválidos salen
con el estado estándar `2` de `argparse`.

## Determinismo

La misma semilla y el mismo nombre de jugador producen siempre los mismos bytes. El generador es el
generador lineal congruencial de 64 bits definido en la especificación, y el orden de llamadas está
fijado:

1. dos celdas iniciales, redibujando la celda de la máquina mientras sea igual a la del jugador;
2. dos valores de tirada, redibujando la tirada de la máquina mientras sea igual a la del jugador;
3. un valor de orden de movimientos por turno, inmediatamente antes de los intentos de ese turno.

No se hace ninguna otra llamada aleatoria. La iteración de conjuntos nunca se usa para construir
la salida, así que la transcripción no depende de la semilla de hash del intérprete.

## Transcripción

La salida coincide exactamente con [`../spec/transcript-format.md`](../spec/transcript-format.md):

- un registro UTF-8 por línea, terminado en salto de línea;
- cabecera, una línea `turn=` por turno, líneas de intento con dos espacios iniciales, líneas
  `move=` exitosas, una línea opcional `no_legal_move` y las dos líneas de resultado;
- `capture=<caballo>` en una línea de intento nombra al caballo que sería capturado;
- una captura exitosa agrega ` capture` a su línea `move=`.

La salida va a la salida estándar. Redirígela para crear una traza:

```bash
common/harness/run.sh --seed 7 --name Learner > common/traces/seed-0007.txt
```

## Comprobar un conjunto de trazas

Compara una ejecución con su traza de referencia:

```bash
diff -u common/traces/seed-0007.txt \
  <(common/harness/run.sh --seed 7 --name Learner)
```

Verifica que cada traza registrada sea reproducible:

```bash
cd common/traces
sha256sum -c checksums.txt
```

El archivo de sumas cubre `seed-0001.txt` hasta `seed-0020.txt`. Consulta
[`../traces/README-es.md`](../traces/README-es.md) para el procedimiento de regeneración.

## Comportamiento en los límites

- Una captura termina la partida de inmediato, antes de pasar el turno.
- Un caballo atrapado pierde de inmediato; no se registra ningún movimiento exitoso en ese turno.
- Cuando el número de movimientos exitosos alcanza `--max-moves` sin ganador, el resultado es
  `winner=draw`. El valor por defecto reproduce la regla de 200 movimientos de la especificación.
- `--max-moves 0` es válido y produce tablas con cero movimientos.

## Relación con la especificación

La especificación es la autoridad. Si esta implementación y la especificación discrepan, se
corrige la implementación. Las trazas son autoridad para sus semillas y actúan como contrato de
comportamiento para cualquier otra implementación.
