# Trazas de referencia

> [English](README.md) | [Español](README-es.md)

Este directorio es el contrato de comportamiento del laboratorio. Cada archivo es la transcripción
completa de una ejecución, producida por [`../harness/reference.py`](../harness/reference.py) a
partir de las reglas de [`../spec/game-rules.md`](../spec/game-rules.md) y del formato de
[`../spec/transcript-format.md`](../spec/transcript-format.md).

## El conjunto de trazas

| Archivos | Semilla | Nombre del jugador | Formato |
|---|---|---|---|
| `seed-0001.txt` … `seed-0020.txt` | `1` … `20` | `Learner` | Ceros en archivo; semilla sin ceros |

Cada archivo contiene la transcripción completa: cabecera, turnos, movimientos intentados,
movimientos exitosos y resultado. El conjunto cubre ambos ganadores y ambas condiciones de fin:

| Resultado | Trazas |
|---|---|
| Gana el jugador | 5 |
| Gana la máquina | 15 |
| Victoria por captura | 11 |
| Victoria por trampa | 9 |
| Tablas | 0 |

La ausencia de tablas es una propiedad de estas semillas, no una regla. Una partida llega al
límite de 200 movimientos solo cuando ningún caballo puede capturar ni atrapar al otro antes.

## Autoridad

Estas trazas son autoridad para sus semillas. Si otra implementación produce una salida diferente
para la misma semilla y el mismo nombre de jugador, la implementación está mal, salvo que la traza
contradiga la especificación; en ese caso la traza se regenera desde el arnés corregido.

Nunca edites una traza a mano. Cambia la implementación o la especificación, regenera el conjunto
completo y revisa el diff.

## Verificar un conjunto de trazas

Desde este directorio:

```bash
sha256sum -c checksums.txt
```

Cada línea debe informar `OK` (o su equivalente localizado). `checksums.txt` contiene un resumen
SHA-256 por cada archivo `seed-*.txt`.

Reproduce una sola traza desde la raíz del repositorio:

```bash
diff -u common/traces/seed-0001.txt \
  <(common/harness/run.sh --seed 1 --name Learner)
```

Un diff vacío significa que la ejecución coincide con el comportamiento registrado.

## Regenerar el conjunto

Desde este directorio:

```bash
for seed in $(seq 1 20); do
  printf -v paddedSeed '%04d' "$seed"
  ../harness/run.sh --seed "$seed" --name Learner > "seed-${paddedSeed}.txt"
done
sha256sum seed-*.txt > checksums.txt
```

Ejecuta la regeneración dos veces y compara los resultados byte por byte antes de aceptar un
conjunto nuevo. La documentación del arnés describe las garantías de determinismo en
[`../harness/README-es.md`](../harness/README-es.md).

## Usar las trazas como evidencia de aceptación

1. Predice la transcripción, o al menos su resultado, antes de ejecutar la implementación candidata.
2. Ejecuta la candidata con la misma semilla y el mismo nombre de jugador.
3. Compara la salida de la candidata con el archivo mediante un diff.
4. Explica cada línea diferente.
5. Registra el comando, el diff y la explicación en tu archivo de evidencia.

La traza es evidencia solo cuando el comando y la salida observada se registran junto a ella.
