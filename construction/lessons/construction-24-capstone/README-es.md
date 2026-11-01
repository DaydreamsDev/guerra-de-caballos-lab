# construction-24 — Proyecto final: informe de costes

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** justificación de compromisos · **Prerrequisito:**
> construction-12 y construction-15 · **Tiempo estimado:** 175 minutos

Este es el proyecto final de Track A. No tiene una línea base de código propia. La persona toma dos
escalones existentes, aplica el mismo cambio a ambos, mide el coste y escribe un informe de una
página que defiende una decisión de diseño con números.

## El cambio

Añadir ambas cosas a dos escalones distintos:

- un tercer tipo de pieza;
- un tablero de 10×10.

Conservar el contrato de transcripción. Las 20 trazas de referencia deben seguir pasando en el
camino de 8×8.

## Qué hace la persona que aprende

1. Elegir dos escalones. Pares recomendados: `construction-12` y `construction-13`, o
   `construction-14` y `construction-15`.
2. Copiar cada escalón a un directorio de trabajo fuera del directorio de la lección. No editar la
   línea base de la lección.
3. Predecir el coste antes de tocar el código.
4. Aplicar el mismo cambio a ambas copias.
5. Medir archivos tocados, líneas cambiadas, conceptos nuevos, pruebas rotas, tiempo de compilación,
   tamaño del binario y las 20 trazas.
6. Escribir el informe con [`REPORT-TEMPLATE-es.md`](REPORT-TEMPLATE-es.md).
7. Defender el veredicto: qué escalón fue más barato para este cambio, y por qué.

## Medir una copia

El script auxiliar cuenta archivos y líneas y ejecuta las 20 trazas de referencia:

```bash
./measure.sh ../construction-12-polymorphism
./measure.sh /tmp/capstone/rung-a
```

Imprime:

```text
files=10 lines=540 traces=20/20
```

Los conteos incluyen todos los archivos `.h` y `.cpp` directamente bajo `code/`. Un archivo de
benchmark o de sonda también se cuenta; decirlo en el informe.

## Archivos

| Archivo | Propósito |
|---|---|
| `README.md` | Este archivo. |
| `README-es.md` | Edición en español. |
| `lesson.md` | El proyecto final guiado. |
| `lesson-es.md` | Edición en español del proyecto final guiado. |
| `REPORT-TEMPLATE.md` | La plantilla del informe de una página. |
| `REPORT-TEMPLATE-es.md` | Edición en español de la plantilla. |
| `measure.sh` | Cuenta archivos y líneas y comprueba las 20 trazas. |
| `meta.yml` | Metadatos de la lección. |
| `EVIDENCE.md` | Registro de evidencias en inglés. |
| `EVIDENCE-es.md` | Registro de evidencias en español. |

## Criterio de aceptación

La persona entrega un informe de una página que declara una predicción, reporta números medidos para
dos escalones, explica qué número decidió el veredicto y nombra el próximo cambio que haría. Ambas
copias de trabajo pasan las 20 trazas de referencia en el camino de 8×8 sin cambios.
