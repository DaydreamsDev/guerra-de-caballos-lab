# archaeology-16 — Dependencia de plataforma

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** dependencia de plataforma y adaptadores ·
> **Prerrequisito:** archaeology-01 · **Tiempo estimado:** 60 minutos

Esta lección aísla las llamadas de consola que atan el archivo a Windows. Un pequeño adaptador
`platform` ofrece color, limpieza y entrada de teclas mediante una interfaz portable; el llamador ya
no incluye `conio.h` ni llama a `system("color")` directamente.

## Qué hace la persona que aprende

- Lee `conio.h`, `iostream.h`, `system("color 1f")`, `system("cls")` y `getch()` del archivo.
- Lee la interfaz `platform.h` y la implementación POSIX.
- Ejecuta el adaptador y confirma que el llamador compila sin encabezados de plataforma.
- Registra qué oculta el adaptador y qué no garantiza entre terminales.

## El adaptador

```text
code/platform.h
code/platform.cpp
code/main.cpp
```

`platform.h` expone `set_console_color`, `clear_console` y `read_console_key`. La implementación usa
secuencias de escape ANSI y `std::getchar`; no incluye `conio.h`.

## Compilar y ejecutar

```bash
./run.sh
```

Salida esperada:

```text
adapter=portable
```

El programa escribe secuencias ANSI antes de `portable=yes`; el ejecutor comprueba el marcador.

## Archivos

| Archivo | Propósito |
|---|---|
| `README.md` | Este archivo. |
| `README-es.md` | Edición en español. |
| `lesson.md` | La lección guiada. |
| `lesson-es.md` | Edición en español de la lección guiada. |
| `code/platform.h` | Interfaz de consola portable. |
| `code/platform.cpp` | Implementación POSIX/ANSI. |
| `code/main.cpp` | Llamador que usa la interfaz. |
| `run.sh` | Compila y ejecuta el adaptador. |
| `meta.yml` | Metadatos de la lección. |
| `EVIDENCE.md` | Registro de evidencias en inglés. |
| `EVIDENCE-es.md` | Registro de evidencias en español. |

## Criterio de aceptación

La **Autocomprobación (binaria)** de la lección es la lista autoritativa. `meta.yml` la refleja para
las herramientas.
