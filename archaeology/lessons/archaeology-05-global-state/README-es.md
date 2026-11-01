# archaeology-05 — Estado global y acoplamiento oculto

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** estado global y acoplamiento oculto · **Prerrequisito:**
> archaeology-04 · **Tiempo estimado:** 60 minutos

Esta lección mapea el estado global mutable del código archivado de `variant-01` y el acoplamiento
oculto que produce. El resultado es una tabla de mutaciones por global y un mapa por función de
lecturas, escrituras y efectos de impresión.

## Qué hace la persona que aprende

- Ejecuta `scan_globals.sh` para listar las declaraciones y los conteos de menciones.
- Lee las funciones y registra qué globales lee, escribe e imprime cada una.
- Completa `GLOBAL-STATE-MAP-TEMPLATE-es.md`.
- Nombra la ruta de mutación más difícil de probar y el cambio mínimo que la hace testeable.

## La fuente

```text
<archivo>/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

La fuente declara el tablero, el orden de movimientos, las posiciones de los caballos, el turno, el
orden y el ganador como variables de archivo. Funciones como `cabubicacion`, `vecjug`, `movcab` y
`ordenjuego` las leen y escriben, e imprimen desde dentro de las mismas funciones.

## Ejecutar el escáner

Desde la raíz del laboratorio:

```bash
./archaeology/lessons/archaeology-05-global-state/scan_globals.sh /ruta/al/archivo
```

El script imprime las declaraciones globales, los encabezados de función que reconoce y un conteo de
menciones por global. El conteo es un punto de partida, no el mapa.

## Archivos

| Archivo | Propósito |
|---|---|
| `README.md` | Este archivo. |
| `README-es.md` | Edición en español. |
| `lesson.md` | La lección guiada. |
| `lesson-es.md` | Edición en español de la lección guiada. |
| `GLOBAL-STATE-MAP-TEMPLATE.md` | El mapa que completa la persona. |
| `GLOBAL-STATE-MAP-TEMPLATE-es.md` | Edición en español de la plantilla. |
| `scan_globals.sh` | Lista declaraciones y conteos de menciones. |
| `meta.yml` | Metadatos de la lección. |
| `EVIDENCE.md` | Registro de evidencias en inglés. |
| `EVIDENCE-es.md` | Registro de evidencias en español. |

## Criterio de aceptación

La **Autocomprobación (binaria)** de la lección es la lista autoritativa. `meta.yml` la refleja para
las herramientas.
