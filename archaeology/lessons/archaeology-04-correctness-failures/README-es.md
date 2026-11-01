# archaeology-04 — Fallos de corrección

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** fallos de corrección · **Prerrequisito:** archaeology-03 ·
> **Tiempo estimado:** 90 minutos

Esta lección diagnostica seis fallos de corrección en el código archivado. Cada fallo se convierte
en una tarjeta con evidencia, síntoma, causa raíz, consecuencia, la prueba que lo detecta, un
arreglo mínimo y un arreglo estructural vinculado a un peldaño de la pista A. El archivo es de solo
lectura; las pruebas son reproducciones aisladas, no pruebas del original.

## Qué hace la persona que aprende

- Ejecuta `tests/run_failure_tests.sh` contra el archivo.
- Lee las seis ubicaciones del código y separa el síntoma de la causa raíz.
- Escribe las seis tarjetas de fallo en `cards/`.
- Reproduce cada fallo con la prueba que lo detecta.
- Propone un arreglo mínimo y un arreglo estructural para cada uno.
- Ordena los seis fallos por coste.
- Registra la evidencia en `EVIDENCE-es.md`.

## El archivo

```text
https://github.com/DaydreamsDev/DaydreamsDev-guerra-de-caballos-archive
tag v1.0.0
```

El archivo está congelado. La lección lo lee; no lo edita.

## Los seis fallos

| Tarjeta | Fallo | Variante:línea | Severidad |
|---|---|---|---|
| failure-001 | El límite del bucle escribe un elemento fuera del tablero | `variant-01:61`, `variant-02:72`, `variant-03:82`, `variant-04:88` | Crítica |
| failure-002 | `void main()` | `variant-01:32`, `variant-02:37`, `variant-04:52`; contraste `variant-03:47` | Alta |
| failure-003 | `srand(time(NULL))` dentro del bucle de reintento de colocación | `variant-01:76`, `variant-02:87`, `variant-03:99`, `variant-04:105` | Alta |
| failure-004 | Inicialización con punteros rota | `variant-03:84-86` | Alta |
| failure-006 | Lectura sin límite del nombre | `variant-02:46`, `variant-03:56`, `variant-04:62` | Crítica |
| failure-005 | Condición de victoria invertida | `variant-00:299` | Alta |

## Ejecutar las pruebas

Desde este directorio de la lección:

```bash
bash tests/run_failure_tests.sh /ruta/al/archivo
```

Una ejecución de referencia en esta máquina imprime:

```text
failure-001=detected
failure-003=detected
failure-004=detected
failure-005=detected
failure-002=detected
failure-006=detected
```

El arnés de C++ imprime las primeras cuatro líneas; el script de shell imprime las dos últimas. El
orden es fijo. `detected` significa que el patrón se reprodujo, no que el programa original se haya
ejecutado. El script devuelve `0` cuando las seis líneas dicen `detected` y un valor distinto de
cero en caso contrario.

## Archivos

| Archivo | Propósito |
|---|---|
| `README.md` | Este archivo. |
| `README-es.md` | Edición en español. |
| `lesson.md` | La lección guiada. |
| `lesson-es.md` | Edición en español de la lección guiada. |
| `meta.yml` | Metadatos de la lección. |
| `EVIDENCE.md` | Registro de evidencia en inglés. |
| `EVIDENCE-es.md` | Registro de evidencia en español. |
| `tests/failure_tests.cpp` | Cuatro reproducciones aisladas: 001, 003, 004, 006. |
| `tests/run_failure_tests.sh` | Ejecuta el arnés y las dos comprobaciones de código: 002, 005. |
| `../../cards/failure-001-loop-bound.md` | Tarjeta 001, inglés. |
| `../../cards/failure-001-loop-bound-es.md` | Tarjeta 001, español. |
| `../../cards/failure-002-void-main.md` | Tarjeta 002, inglés. |
| `../../cards/failure-002-void-main-es.md` | Tarjeta 002, español. |
| `../../cards/failure-003-seed-in-loop.md` | Tarjeta 003, inglés. |
| `../../cards/failure-003-seed-in-loop-es.md` | Tarjeta 003, español. |
| `../../cards/failure-004-broken-initialization.md` | Tarjeta 004, inglés. |
| `../../cards/failure-004-broken-initialization-es.md` | Tarjeta 004, español. |
| `../../cards/failure-006-unbounded-name.md` | Tarjeta 006, inglés. |
| `../../cards/failure-006-unbounded-name-es.md` | Tarjeta 006, español. |
| `../../cards/failure-005-inverted-win.md` | Tarjeta 005, inglés. |
| `../../cards/failure-005-inverted-win-es.md` | Tarjeta 005, español. |

## Criterio de aceptación

La **Autocomprobación (binaria)** de la lección es la lista autoritativa. `meta.yml` la refleja para
las herramientas.
