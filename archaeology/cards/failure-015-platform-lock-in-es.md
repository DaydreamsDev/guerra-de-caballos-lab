# failure-015 — `conio.h`, `iostream.h` y `system("color")` atan la compilación a Windows

> [English](failure-015-platform-lock-in.md) | [Español](failure-015-platform-lock-in-es.md)

- **Clase:** robustez
- **Severidad:** alta
- **Concepto violado:** una compilación depende de su entorno declarado, no de un accidente
- **Archivo:** etiqueta `v1.0.0`,
  `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`

## Evidencia

- **Variante:** `variant-01-baseline-2008`. La misma forma se repite en cada variante de 2008.
- **Líneas:** 11, 12, 32, 38, 42–43, 781–864.
- **Código:**

```cpp
#include<conio.h>                          // línea 11
#include<iostream.h>                       // línea 12

void  main()                                // línea 32
{
    system("color 1f");                     // línea 38
    getch();                                // línea 42
    system("cls");                          // línea 43
    // ...
    getch();                                // líneas 781, 806, 835, 861
    system("cls");                          // líneas 784, 809, 838, 864
}
```

`conio.h` es un encabezado de consola de DOS/Windows. `iostream.h` es el encabezado de iostream
anterior al estándar. `void main()` no es un punto de entrada estándar.
`system("color 1f")` y `system("cls")` son comandos del shell de Windows; `getch()` lee una
tecla sin eco y no es C++ estándar.

## Síntoma

El archivo no compila en Linux ni en macOS. El primer error es:

```text
fatal error: conio.h: No such file or directory
```

Un compilador moderno de Microsoft también rechaza `iostream.h`, porque el encabezado fue
eliminado del estándar. En un shell POSIX, `system("cls")` inicia un shell, no encuentra `cls`,
imprime un error y sale con 127; el archivo ignora el estado y continúa como si la pantalla se
hubiera limpiado.

## Causa raíz

El código apunta a un compilador, un sistema operativo y una consola. `conio.h` era el lector de
teclas disponible, `system("color 1f")` la vía más corta al color, y `system("cls")` la gestión
de pantalla que ofrecía el objetivo. Eran elecciones locales para un programa local.

El artefacto nunca nombró su plataforma, así que la frontera faltó hasta que el código salió de
la máquina. El bloqueo tiene tres capas: el dialecto del lenguaje (`iostream.h`, `void main`),
la API de plataforma (`conio.h`, `getch`) y el comportamiento de la terminal (`color`, `cls`).

## Consecuencia

Un lector que clona el repositorio en Linux no puede compilar el artefacto. El fallo es visible
en compilación, lo que lo hace más barato que un error de memoria, pero bloquea cualquier otro
experimento. Los comandos de ejecución fallan en silencio donde no existen, así que un puerto
parcial puede compilar y aun así comportarse distinto.

Para un archivo publicado, una compilación fallida es un defecto de severidad alta. Para una
demostración en una sola máquina, el mismo código es defendible. La severidad depende del
contexto declarado.

## Prueba que lo detecta

`archaeology/lessons/archaeology-16-platform-lock-in/run.sh` compila un llamador contra un
puerto de consola de tres funciones:

```text
adapter=portable
```

La lección escribe después un segundo adaptador y enlaza el **mismo** `main.cpp` contra él:

```text
portable=yes
```

La fuente del llamador no cambia. El adaptador ANSI escribe 30 bytes; el adaptador simple
escribe 13. La lección demuestra que el llamador no tiene encabezados de plataforma con `grep`
y `nm -C -u`, y muestra que `read_console_key` está declarada, definida y nunca se llama.

Eso es portabilidad de interfaz, no identidad de comportamiento. El contrato de teclas queda
sin verificar.

## Arreglo mínimo

Sustituye las llamadas de consola del llamador por un puerto pequeño:

```cpp
void set_console_color();
void clear_console();
int read_console_key();
```

Pon el código de plataforma en `platform.cpp`. El llamador incluye solo `platform.h` y la
biblioteca estándar. Esto elimina el bloqueo de compilación y da al revisor un solo archivo que
auditar.

No corrige `iostream.h`, `void main` ni los glifos CP437. Un adaptador de consola no es una
migración de lenguaje.

## Arreglo estructural

Declara las capacidades que asume el puerto, añade un adaptador nulo para pruebas y tuberías, y
deja que el llamador elija el adaptador. Si el programa necesita negociación real de
capacidades, usa una biblioteca de terminal (`ncurses`, `PDCurses`) en lugar de escapes
escritos a mano. Mantén el tema y la política de pausa en el llamador o en una capa de
presentación, no dentro del adaptador.

Así una prueba puede enlazar el adaptador nulo y afirmar el comportamiento del llamador sin una
terminal.

Peldaños de la pista A: [`construction-08-modular`][c08] para la frontera de módulo y
[`construction-20-hexagonal`][c20] para puertos y adaptadores. `common-04` es la contraparte de
traza dorada: congela la salida de consola antes de cambiar el adaptador.

[c08]: ../../construction/lessons/construction-08-modular/lesson.md
[c20]: ../../construction/lessons/construction-20-hexagonal/lesson.md

## Confianza

Alta en que las seis construcciones están en el archivo y en que bloquean una compilación
moderna. Alta en que el adaptador hace que el llamador compile sin `conio.h`. Media en la
identidad de comportamiento: la lección mide 30 frente a 13 bytes y nunca llama a la función de
teclas.

## Nota sobre el archivo

Esta tarjeta es evidencia, no una reparación del archivo. El archivo permanece de solo
lectura; el puerto y los adaptadores van en una copia de trabajo o en `reference/`.
