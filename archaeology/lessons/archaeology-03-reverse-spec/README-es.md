# archaeology-03 — Reconstruir la especificación

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** especificación de comportamiento · **Prerrequisito:**
> archaeology-02 · **Tiempo estimado:** 90 minutos

Esta lección reconstruye el comportamiento del código archivado de `variant-01`. El resultado es una
especificación de comportamiento y al menos una traza heredada capturada. La especificación moderna
del laboratorio es un objetivo de comparación, no evidencia de lo que hacía el programa de 2008.

## Qué hace la persona que aprende

- Lee el código y registra las reglas que realmente implementa.
- Separa las reglas del juego del generador aleatorio y de la presentación en consola.
- Captura una traza heredada con una semilla fija usando `capture_trace.sh`.
- Completa `SPEC-TEMPLATE-es.md` y `TRACE-TEMPLATE-es.md`.
- Compara la especificación reconstruida con `common/spec/game-rules.md` y registra las diferencias.

## El original

```text
<archivo>/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

El original llama a `srand(time(NULL))` y `rand()`. No tiene argumento de semilla, ni formato de
transcripción, ni arnés de pruebas. La sonda cambia la semilla en una copia de trabajo; el archivo
no se edita.

## Capturar una traza

Desde este directorio:

```bash
./code/capture_trace.sh /ruta/al/archivo 12345 /tmp/legacy-trace-12345.txt
```

La salida informa la semilla, la ruta de salida, el estado de ejecución, las líneas de salida
estándar y las líneas de error. La traza es la salida cruda del original: un tablero CP437 y
mensajes de consola en español, no la transcripción moderna.

## Archivos

| Archivo | Propósito |
|---|---|
| `README.md` | Este archivo. |
| `README-es.md` | Edición en español. |
| `lesson.md` | La lección guiada. |
| `lesson-es.md` | Edición en español de la lección guiada. |
| `SPEC-TEMPLATE.md` | Plantilla de especificación de comportamiento. |
| `SPEC-TEMPLATE-es.md` | Edición en español de la plantilla de especificación. |
| `TRACE-TEMPLATE.md` | Plantilla de traza heredada. |
| `TRACE-TEMPLATE-es.md` | Edición en español de la plantilla de traza. |
| `code/capture_trace.sh` | Copia el código, fija la semilla, compila y ejecuta. |
| `code/compat/iostream.h` | Encabezado de compatibilidad para el include preestándar. |
| `code/compat/conio.h` | `getch` sin efecto. |
| `code/legacy_wrapper.cpp` | `int main` estándar alrededor del punto de entrada heredado. |
| `meta.yml` | Metadatos de la lección. |
| `EVIDENCE.md` | Registro de evidencias en inglés. |
| `EVIDENCE-es.md` | Registro de evidencias en español. |

## Criterio de aceptación

La **Autocomprobación (binaria)** de la lección es la lista autoritativa. `meta.yml` la refleja para
las herramientas.
