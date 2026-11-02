# archaeology-01 — Compilar y ejecutar código heredado

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** compilar y ejecutar código heredado ·
> **Prerrequisito:** common-04 · **Tiempo estimado:** 62 minutos

Esta lección toma el código archivado de `variant-01` e intenta compilarlo y ejecutarlo en una
máquina Linux moderna. El archivo no se edita. Se construye una pequeña sonda de compatibilidad
en un directorio temporal, y el resultado se registra como una compilación ejecutable o como un
bloqueo documentado.

## Qué hace la persona que aprende

- Predice qué hará el programa de consola heredado.
- Intenta una compilación moderna y registra los errores.
- Usa la sonda de compatibilidad en `code/` para compilar el código sin cambios.
- Ejecuta la sonda con un límite de tiempo y registra la salida, los errores y las diferencias
  con la predicción.
- Escribe la evidencia, incluido lo que la sonda no prueba.

## El original

```text
<archivo>/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

El código tiene 930 líneas, usa `#include<iostream.h>`, `#include<conio.h>`, `void main()`,
`system("color 1f")`, `system("cls")`, `getch()`, `srand(time(NULL))` y `rand()`.

## La sonda

| Archivo | Propósito |
|---|---|
| `code/compat/iostream.h` | Lleva el encabezado preestándar a `<iostream>` y `using namespace std`. |
| `code/compat/conio.h` | Proporciona un `getch()` sin efecto. |
| `code/legacy_wrapper.cpp` | Llama al punto de entrada heredado renombrado desde un `int main` estándar. |
| `code/build_probe.sh` | Copia el código del archivo a un directorio temporal, compila, enlaza y ejecuta. |

La sonda no edita el archivo. Renombra `main` a `legacy_main` con `-Dmain=legacy_main` y proporciona
un envoltorio. Usa `-fpermissive` porque el código histórico no es C++ moderno.

## Ejecutar la sonda

Desde este directorio:

```bash
./code/build_probe.sh /ruta/al/archivo
```

En esta máquina la salida es:

```text
source=<archivo>/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
compile=ok
run_status=0
stdout_lines=614
stderr_lines=26
stderr_first=sh: línea 1: color: orden no encontrada
```

Los conteos de líneas varían porque el programa usa `time(NULL)` y `rand()`. Las líneas de error son
los comandos Windows `color` y `cls` fallando en Linux. La salida estándar es un tablero CP437
representado con caracteres de reemplazo, no la transcripción moderna.

## Qué prueba la sonda

- El código se puede compilar con una capa de compatibilidad.
- El programa se puede ejecutar hasta el final con `getch()` sin efecto.
- Las llamadas a la consola de Windows fallan en Linux.
- La salida no es determinista y no es el contrato de transcripción moderno.

## Qué no prueba la sonda

- Que el código histórico sea portable.
- Que la salida sea correcta.
- Que la herramienta histórica fuera Microsoft Visual C++ 6.0; esa afirmación está documentada en
  la bóveda privada y no es verificable de forma independiente desde el archivo público.
- Que la transcripción moderna se pueda reproducir desde este código sin una reconstrucción
  separada.

## Criterio de aceptación

La **Autocomprobación (binaria)** de la lección es la lista autoritativa. `meta.yml` la refleja para
las herramientas.
