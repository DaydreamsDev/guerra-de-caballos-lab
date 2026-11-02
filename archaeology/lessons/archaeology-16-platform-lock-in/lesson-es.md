# archaeology-16 — Dependencia de plataforma

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** dependencia de plataforma y adaptadores ·
> **Prerrequisito:** archaeology-01 · **Tiempo estimado:** 60 minutos
> **Lectura:** ~24 minutos

## Por qué existe esta lección

El archivo no compila en Linux. No compila en macOS. Tampoco compila con un compilador moderno de
Microsoft, porque `iostream.h` se eliminó del estándar. El primer error no es sutil:

```text
fatal error: conio.h: No such file or directory
```

Seis construcciones cargan con la dependencia. En `variant-01`, etiqueta `v1.0.0`, la línea 11
incluye `conio.h`, la línea 12 incluye `iostream.h`, la línea 32 declara `void main()`, la línea 38
ejecuta `system("color 1f")`, y las líneas 42 y 43 llaman a `getch()` y a `system("cls")`. La misma
forma se repite en cada variante de 2008. El catálogo lo llama `failure-015`, un fallo de robustez:
**el programa solo funciona en un entorno**.

`archaeology-11` separó la entrada y la salida *dentro* del programa. Esta lección trata del
dispositivo que está debajo: color, limpieza y tecla. El banco tiene 34 líneas en tres archivos, y
aquí compila:

```text
adapter=portable
```

Esa frase es la trampa. "Portable" describe la compilación. No describe el comportamiento. El
llamador compila contra tres funciones en lugar de `conio.h`, y eso es una mejora real. No significa
que el programa se vea, espere o limpie igual en otra terminal. La lección es la brecha entre esas
dos afirmaciones.

El adaptador es pequeño a propósito. Oculta lo suficiente para que el llamador sea portable, y
oculta demasiado poco para que el comportamiento sea idéntico. Aprender a decir cuál de las dos
cosas compraste es la habilidad.

## Qué podrás hacer

Al terminar esta lección puedes:

- nombrar las seis construcciones del archivo que atan el programa a Windows y Visual C++ 6.0;
- separar la dependencia de compilación de la dependencia de ejecución, y decir cuál oculta una
  cabecera;
- explicar qué oculta un adaptador y qué solo traslada de sitio;
- distinguir la portabilidad de la interfaz de la identidad del comportamiento, con un ejemplo
  medido;
- demostrar que un llamador no tiene cabeceras de plataforma leyendo includes y símbolos del objeto,
  no un comentario;
- cambiar un adaptador por otro y mostrar que el llamador compila sin cambios;
- medir los bytes que escribe el adaptador y explicar por qué una tubería los recibe todos;
- afirmar por qué `std::getchar` no es `getch`, en eco, búfer y bloqueo;
- listar las capacidades de terminal que el adaptador asume sin preguntar;
- precificar la dependencia por el fallo de compilación, no por gusto;
- mantener el archivo de solo lectura mientras trabajas.

La lección está hecha cuando
otra persona puede reproducir ambas compilaciones y seguir tus motivos.

## El concepto: un nombre portable no es un comportamiento portable

### La dependencia es una dependencia del entorno

El código está **atado a una plataforma** cuando una compilación o una ejecución correcta depende de
un entorno que no forma parte del contrato declarado del artefacto. El archivo depende de tres
entornos a la vez:

| Capa | Ejemplo | Qué asume |
|---|---|---|
| Dialecto del lenguaje | `#include<iostream.h>`, `void main()` | Un compilador anterior al estándar. |
| API de plataforma | `#include<conio.h>`, `getch()` | Una consola de DOS/Windows. |
| Comportamiento de terminal | `system("color 1f")`, `system("cls")` | Un shell de Windows y un búfer de consola. |

Las dos primeras fallan en la compilación. La tercera puede sobrevivir a la compilación y fallar en
la ejecución, en silencio. Esa diferencia importa: borrar una cabecera puede hacer que un programa
compile sin hacer que funcione.

### El adaptador en una imagen

```text
main.cpp ──► platform.h ──► platform.cpp ──► terminal
             (puerto)       (adaptador)
```

`platform.h` es el **puerto**: la forma de la conversación que el llamador tiene permitido mantener
con una consola. `platform.cpp` es un **adaptador**: una implementación de esa conversación para un
entorno. `main.cpp` es el llamador, y no sabe si el adaptador escribe secuencias ANSI, llama a una
función de Win32 o no hace nada.

El puerto son tres funciones:

```cpp
void set_console_color();
void clear_console();
int read_console_key();
```

Esa es toda la interfaz. Fíjate en lo que no contiene: ningún código de color, ningún manejador de
terminal, ningún resultado de error, ninguna consulta de capacidades. La interfaz es estrecha, y esa
estrechez es a la vez su fuerza y su límite.

### Qué oculta el adaptador

El adaptador oculta **dónde vive el conocimiento de plataforma**, no el conocimiento en sí.

| Elemento | Dónde quedó | Qué ganó el llamador |
|---|---|---|
| `#include<conio.h>` | eliminado | El llamador compila en un sistema POSIX. |
| `#include<iostream.h>` | no se aborda | Nada; es un problema de dialecto, no de consola. |
| `system("color 1f")` | `set_console_color()` | El código del tema está dentro del adaptador. |
| `system("cls")` | `clear_console()` | La semántica de limpieza es ahora una promesa del adaptador. |
| `getch()` | `read_console_key()` | La semántica de tecla es ahora un nombre, no una garantía. |
| `void main()` | `int main()` en el banco | Un punto de entrada estándar, solo en el banco. |

Lee la segunda fila otra vez. El adaptador no toca `iostream.h`, porque el banco está escrito en C++
moderno. El archivo todavía lo incluye. Un adaptador de consola no es una migración de lenguaje, y
esta lección no va a fingir lo contrario.

La última fila es la misma advertencia. El banco usa `int main()` porque eso es lo que acepta un
compilador estándar. El `void main()` del archivo es un defecto separado, diagnosticado en
`archaeology-04`. El adaptador no puede arreglar una firma de función que nunca ve.

Lo que el adaptador realmente hace es convertir una **dependencia de compilación** en una
**dependencia de ejecución**:

| Construcción | Falla cuando | Modo de fallo |
|---|---|---|
| `#include<conio.h>` | POSIX, macOS, MSVC moderno | Compilación: archivo no encontrado. |
| `#include<iostream.h>` | Biblioteca estándar moderna | Compilación: archivo no encontrado. |
| `void main()` | C++ estándar | Aviso de compilación, o error con `-Werror`. |
| `getch()` | Fuera de Windows | Compilación: identificador no declarado. |
| `system("color 1f")` | Shell que no es Windows | Ejecución: comando no encontrado; estado ignorado. |
| `system("cls")` | Shell que no es Windows | Ejecución: comando no encontrado; estado ignorado. |

Las cuatro primeras son visibles en el momento de compilar. Las dos últimas no. `system()` es C++
estándar; la *cadena* es la dependencia de plataforma. En un shell POSIX, `system("cls")` arranca un
shell, no encuentra un comando llamado `cls`, imprime un error en stderr, y el shell sale con 127;
`system()` devuelve un estado de espera, y el archivo lo ignora. El programa continúa como si la
pantalla se hubiera limpiado.

Puedes ver el fallo sin el archivo:

```bash
sh -c 'cls'; printf 'exit=%s\n' "$?"
```

```text
sh: 1: cls: not found
exit=127
```

Ese es el argumento más contundente a favor del adaptador. No solo hace que el llamador compile;
mueve la *cadena de comando* específica de plataforma a un archivo donde un revisor puede
encontrarla.

### Qué no garantiza el adaptador

El adaptador garantiza que el llamador compila. No garantiza que la terminal se comporte. Estas son
las suposiciones que hace sin preguntar.

#### 1. Soporte de secuencias de escape

`platform.cpp` escribe secuencias de escape ANSI:

```cpp
std::printf("\033[1;37;44m");   // set_console_color
std::printf("\033[2J\033[H");   // clear_console
```

ANSI funciona en la mayoría de terminales POSIX. En Windows depende de la consola y de que el
procesamiento de terminal virtual esté habilitado. Una ventana `cmd.exe` antigua que no procesa
escapes muestra los bytes como texto: `←[1;37;44m←[2J←[H`. El adaptador no tiene una rama para ese
caso, ni forma de detectarlo.

#### 2. Capacidad de color

`\033[1;37;44m` es una secuencia SGR: negrita, primer plano 37 (blanco), fondo 44 (azul). Es una
representación fiel del `color 1f` del archivo (blanco brillante sobre azul) en una terminal que
soporta 16 colores. Una terminal monocroma la ignora o la aproxima. Una terminal con otra paleta
muestra otro azul. El adaptador no pregunta cuántos colores existen.

#### 3. Semántica de limpieza

`\033[2J\033[H` limpia la pantalla visible y lleva el cursor al inicio. `cls` en Windows limpia el
búfer de consola. No son la misma operación: el comportamiento del scrollback difiere, y algunas
terminales limpian solo el área visible. Si el programa depende de lo que queda arriba, los dos
adaptadores discrepan.

#### 4. Semántica de tecla

Esta es la brecha más grande, y la que un nombre oculta mejor.

| Propiedad | `getch()` (conio) | `std::getchar()` (este adaptador) |
|---|---|---|
| Cabecera | `conio.h` | `<cstdio>` |
| Eco | No | Sí, la terminal muestra la tecla. |
| Espera | Devuelve con una tecla | Espera a Enter en modo canónico. |
| Búfer | Lectura de consola sin búfer | Búfer de E/S estándar. |
| Fin de entrada | No aplica a una consola | Devuelve `EOF`. |
| Redirección | Lee la consola | Lee lo que sea que sea `stdin`. |

`read_console_key()` es un **nombre** portable. No es una **interacción** portable. Un llamador que
espera "pulsa una tecla y continúa" recibe "escribe una línea y pulsa Enter" en una terminal POSIX.
El banco nunca llama a la función, así que esta diferencia es invisible en la ejecución de
aceptación.

#### 5. Redirección

El adaptador escribe escapes en `stdout` sin condiciones. Redirige el programa a un archivo y el
archivo contiene 17 bytes de control antes del marcador:

```text
1b 5b 31 3b 33 37 3b 34 34 6d   ESC [ 1 ; 3 7 ; 4 4 m
1b 5b 32 4a                     ESC [ 2 J
1b 5b 48                        ESC [ H
70 6f 72 74 61 62 6c 65 3d 79 65 73 0a   portable=yes\n
```

Una tubería, un archivo de registro o una transcripción de CI también los reciben. El programa nunca
pregunta si `stdout` es una terminal. `system("cls")` en Windows escribe en la consola, y bajo
redirección puede no hacer nada o fallar; los comportamientos también difieren en la otra dirección.

#### 6. Codificación

El archivo dibuja el tablero con bytes CP437: `simcab1=21`, `simcab2=156`, `sep=219`. Una terminal
moderna suele interpretar los bytes como UTF-8. El byte 219 por sí solo no es UTF-8 válido, así que
el tablero puede mostrarse como caracteres de reemplazo o mojibake. El adaptador no toca la
codificación. Cambiar `conio.h` por ANSI no convierte un programa CP437 en un programa UTF-8.

#### 7. Propiedad de la salida

`main.cpp` todavía llama a `std::printf("portable=yes\n")`. El adaptador posee color, limpieza y
tecla; no posee la transcripción. Esa es una frontera deliberada en este banco, y es una brecha si
quieres que un adaptador de prueba capture la salida. `archaeology-11` es la lección para ese
puerto.

#### 8. El dialecto del lenguaje

`iostream.h` y `void main` no son problemas de consola. Son problemas de compilador. El adaptador no
los arregla, y quien diga "el archivo ya es portable" después de añadir este adaptador ha exagerado.

### Portabilidad de la interfaz frente a identidad del comportamiento

Son dos afirmaciones distintas, y se confunden con rutina.

| Afirmación | Significado | Cómo se demuestra |
|---|---|---|
| **Portabilidad de la interfaz** | El mismo código del llamador compila y enlaza contra una implementación en cada destino. | Compila el mismo `main.cpp` con dos adaptadores. |
| **Identidad del comportamiento** | La misma interacción observable: bytes, tiempos, eco, bloqueo, estado de pantalla. | Mide ambas compilaciones y compáralas. |

El adaptador compra la primera por construcción. No compra la segunda, y ningún nombre la hace
aparecer. Un adaptador de enchufe permite que el enchufe encaje; no garantiza el voltaje. Aquí, la
forma son las tres declaraciones de función, y el voltaje es la terminal.

El ejemplo medido está en el paso 3. El adaptador ANSI escribe 30 bytes. Un adaptador simple escribe
13. El llamador es idéntico byte a byte en ambas compilaciones. Misma interfaz, distinto
comportamiento. Esa es la lección en una comparación.

Esto no es un argumento contra los adaptadores. Es un argumento para declarar el contrato con
precisión:

- "El llamador compila sin `conio.h`" es verdadero y útil.
- "El comportamiento de consola es portable" es falso salvo que lo hayas medido en cada destino.

Escribe la primera frase en el README. Escribe la segunda solo después de la matriz del paso 5.

### La restricción heredada

El código apuntaba a un compilador, un sistema operativo y una consola. `conio.h` era el lector de
teclas disponible, `system("color 1f")` el camino más corto al color y `system("cls")` la gestión
de pantalla que ofrecía el destino. Eran decisiones locales para un programa local.

El requisito cambió cuando el artefacto se volvió público. Un artefacto publicado se compila en
otras plataformas, y una compilación que solo funciona en una máquina no es portable. El defecto es
que el artefacto nunca nombró su plataforma, así que la frontera faltó hasta que el código salió de
la máquina.

Lo mismo pasa hoy cuando un prototipo se convierte en producto.

## Antes de empezar

Lee estos documentos completos:

- [`README-es.md`](README-es.md) — la ficha de la lección, el comando de compilación y el criterio
  de aceptación.
- [archaeology-01](../archaeology-01-build-today/lesson-es.md) — el prerrequisito: qué hace falta
  para compilar código heredado hoy, y cómo se ve un bloqueo documentado.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, la
  taxonomía de fallos; §5, la escala de severidad; §9, el mapeo a la pista A.
- [`../../../docs/archive.md`](../../../docs/archive.md) — el archivo fijado, la etiqueta y el
  commit.

También necesitas:

- un clon local del archivo en la etiqueta `v1.0.0` (para los anclajes; el banco funciona sin él);
- un compilador de C++20 (`c++`, `g++` o `clang++`);
- `bash`, `grep`, `od` y `cat -v`;
- un directorio temporal fuera de `original/`.

> **Regla:** el archivo es de solo lectura. Copia cualquier archivo que quieras cambiar a un
> directorio temporal; nunca edites `original/`. El banco de esta lección también es evidencia:
> cópialo antes de editarlo.

## El recorrido de 60 minutos

| Min | Paso | Qué haces | Evidencia |
|---:|---|---|---|
| 0–8 | Predecir | Te comprometes con respuestas sobre cabeceras, bytes y tecla | Tabla de predicciones |
| 8–20 | Inspeccionar | Lees los tres archivos y los anclajes del archivo; ejecutas la base | Recuentos y tabla de anclajes |
| 20–32 | Aislar | Copias el banco, nombras la costura, escribes un adaptador simple | `platform-plain.cpp` |
| 32–42 | Compilar | Compilas ambos llamadores, inspeccionas bytes y símbolos | 30 frente a 13 bytes, salida de `nm` |
| 42–55 | Explicar | Completas la matriz de capacidades y precificas la dependencia | Matriz y nota de coste |
| 55–60 | Registrar | Rellenas `EVIDENCE-es.md`; confirmas que el archivo está limpio | Entrada de portafolio |

Si la diferencia entre `getchar` y `getch` tarda quince minutos en
demostrarse, esa investigación es la lección.

### La comprobación de ingeniería en esta lección

La pista B usa el mismo hábito que la pista A, con otro verbo: el comportamiento ya existe, así que
**lo congelas** antes de moverlo.

| Paso | Dónde ocurre | Qué produce |
|---|---|---|
| **Predecir** | Paso 1 | Una conjetura falsable sobre la compilación, los bytes y la tecla. |
| **Inspeccionar** | Paso 2 | Los tres archivos, los anclajes del archivo y la ejecución base. |
| **Aislar** | Paso 3 | La costura nombrada y un segundo adaptador tras la misma cabecera. |
| **Compilar** | Paso 4 | Dos compilaciones, un llamador, bytes y símbolos medidos. |
| **Explicar** | Paso 5 | La matriz de capacidades, las dos afirmaciones y el coste. |
| **Registrar** | Paso 6 | `EVIDENCE-es.md` con comandos, bytes y fronteras. |

## Paso 1 — Predecir (8 min)

Antes de abrir el código, responde estas cinco preguntas por escrito. El objetivo es comprometerte
con respuestas que puedan estar equivocadas.

1. ¿`code/main.cpp` incluye alguna cabecera de plataforma? ¿Qué includes esperas?
2. ¿La salida de `./run.sh` contiene bytes de escape? ¿Y la salida del programa en sí?
3. Si reemplazas `platform.cpp` por un adaptador que no hace nada, ¿`main.cpp` sigue compilando?
4. ¿`std::getchar()` se comporta como `getch()`? Nombra una diferencia.
5. Si rediriges el programa a un archivo, ¿cuántos bytes contiene el archivo?

Una predicción útil es falsable. "El adaptador es portable" no lo es. "`main.cpp` incluye solo
`platform.h` y `<cstdio>`, el binario escribe 17 bytes de escape antes del marcador, y el arnés los
captura pero imprime solo el marcador" sí lo es.

### Una predicción trabajada

Antes de leer, yo escribiría:

- `main.cpp` incluye `platform.h` y `<cstdio>`; ni `conio.h` ni `windows.h`;
- `./run.sh` imprime una línea limpia, porque captura stdout e imprime un marcador; el programa en
  sí escribe los escapes, así que la captura los contiene;
- un adaptador vacío enlaza, porque el llamador usa solo la interfaz;
- `getchar` hace eco y espera a Enter; `getch` no hace ninguna de las dos cosas;
- el archivo contiene 30 bytes: 17 de escape, `portable=yes` y un salto de línea.

Tengo menos seguridad sobre `TERM=dumb`. Mi conjetura es que el adaptador lo ignora, porque el
código nunca lee `TERM` ni llama a `isatty`. Los siguientes pasos comprueban esa conjetura.

Toda afirmación se puede comprobar en los siguientes cuatro pasos. Si tu predicción dice "los
adaptadores son buenos", has hecho un juicio, no una estimación. Escribe el mecanismo que esperas y
luego pruébalo.

## Paso 2 — Inspeccionar (12 min)

### Lee los tres archivos

```bash
cd archaeology/lessons/archaeology-16-platform-lock-in
wc -l code/platform.h code/platform.cpp code/main.cpp run.sh
nl -ba code/platform.h
nl -ba code/platform.cpp
nl -ba code/main.cpp
nl -ba run.sh
```

El banco tiene 34 líneas de C++ más un arnés de 19 líneas:

| Archivo | Líneas | Qué es |
|---|---:|---|
| `code/platform.h` | 5 | El puerto: tres declaraciones. |
| `code/platform.cpp` | 18 | El adaptador: escapes ANSI y `std::getchar`. |
| `code/main.cpp` | 11 | El llamador: dos llamadas y un marcador. |
| `run.sh` | 19 | Compila, captura, comprueba una subcadena, devuelve código. |

Lee primero `platform.h`. Son cinco líneas, y el dato importante es lo que falta: ningún código de
color, ningún manejador, ninguna consulta de capacidades, ningún retorno de error. El puerto dice
"pon un color", no "pon el color 1f".

Lee `platform.cpp`. Incluye `platform.h` y `<cstdio>`, y nada más. `set_console_color` escribe una
secuencia SGR, `clear_console` escribe dos secuencias de escape, y `read_console_key` devuelve
`std::getchar()`. No hay `#ifdef`, ni `conio.h`, ni `system`.

Lee `main.cpp`. Incluye `platform.h` y `<cstdio>`, llama a `set_console_color()` y a
`clear_console()`, imprime `portable=yes` y devuelve 0. No llama a `read_console_key`.

Esa última observación es un hecho del banco, no un accidente. La ejecución de aceptación ejercita
dos de las tres funciones. Guárdala para la frontera de evidencia.

Lee `run.sh`. Compila ambos archivos con:

```text
-std=c++20 -Wall -Wextra -Werror -O2
```

Luego captura stdout en `output` y comprueba una subcadena:

```bash
output="$("${binaryPath}")"
if [[ "${output}" != *"portable=yes"* ]]; then
    printf 'adapter=no\n'
    exit 1
fi
printf 'adapter=portable\n'
```

La comprobación de subcadena es deliberada. La captura empieza con bytes de escape, así que una
comparación de igualdad contra `portable=yes` fallaría. El arnés tolera los escapes y reporta un
marcador limpio. Es un arnés razonable para una comprobación de compilación, y no es una prueba de
terminal.

### Los anclajes del archivo

En el archivo fijado en la etiqueta `v1.0.0`:

```bash
grep -nE 'conio|iostream\.h|void +main|system\("color|system\("cls"|getch\(\)' \
    original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

La salida, con los recuentos que importan:

| Ubicación | Qué hay |
|---:|---|
| 11 | `#include<conio.h>` |
| 12 | `#include<iostream.h>` |
| 32 | `void  main()` |
| 38 | `system("color 1f")` |
| 42 | `getch()` |
| 43 | `system("cls")` |
| 781, 806, 835, 861 | `getch()` en el bucle de turnos |
| 784, 809, 838, 864 | `system("cls")` en el bucle de turnos |

Cuéntalos tú mismo:

```bash
grep -c 'getch' original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
grep -c 'system("cls")' original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
grep -c 'system("color' original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

Las respuestas son 5, 5 y 1. Un `conio.h`, un `iostream.h`, un `void main`, un comando de color,
cinco lecturas de tecla y cinco limpiezas. Esa es toda la superficie de plataforma de un programa de
930 líneas.

Las otras variantes llevan la misma forma, con una excepción instructiva:

| Variante | Punto de entrada | `getch` | `system("cls")` | `system("color")` |
|---|---|---:|---:|---:|
| `variant-00-precursor-2007` | `int main()` en 359 | 0 | 2 | 1 |
| `variant-01-baseline-2008` | `void main()` en 32 | 5 | 5 | 1 |
| `variant-02-records-2008` | `void main()` en 37 | 5 | 5 | 1 |
| `variant-03-pointers-dev-2008` | `int main()` en 47 | 5 | 5 | 1 |
| `variant-04-pointers-final-2008` | `void main()` en 52 | 5 | 5 | 1 |

El precursor lee `cin>>caracter` en la línea 370 en lugar de `getch()`. Eso es *más* portable en la
dimensión de entrada, y sigue atado al color y a la limpieza. Un arreglo parcial no es un puerto.
También muestra la diferencia entre los dos fallos: `cin` compila en todas partes y espera a Enter;
`getch` no compila fuera de Windows.

### Lo que el archivo dice de sí mismo

`05-known-issues.md` registra ambas filas con sus propias palabras:

> | Windows console command | `variant-00:363`, `variant-01:38`, ... | Platform lock-in. |
> | Pre-standard headers | All variants, lines 11–12 | `iostream.h` and `conio.h` are not modern
> standard headers. |

El laboratorio no inventa el diagnóstico. Cita el archivo y luego pregunta qué puede y qué no puede
hacer un adaptador al respecto.

### Comprueba tu predicción

- `main.cpp` incluye `platform.h` y `<cstdio>`; ninguna cabecera de plataforma.
- `./run.sh` imprime una línea limpia; el programa escribe 17 bytes de escape antes del marcador.
- Un adaptador vacío enlaza, porque el llamador usa solo la interfaz.
- `getchar` hace eco y espera a Enter; `getch` no hace ninguna de las dos cosas.
- El archivo contiene 30 bytes, no 13.
- `TERM=dumb` no cambia nada, porque el adaptador nunca lo lee.

Si tu predicción decía que la salida del arnés contendría escapes, se te escapó la captura. Si decía
que el adaptador detecta la terminal, asumiste una comprobación de capacidades que el código no
tiene. Vale la pena escribir ambas brechas.

## Paso 3 — Aislar (12 min)

### Copia, no edites

Crea un directorio temporal fuera de `original/`. `build/` está ignorado por este repositorio:

```bash
mkdir -p build/scratch
cp code/platform.h code/platform.cpp code/main.cpp build/scratch/
```

De aquí en adelante, edita los archivos en `build/scratch/`. No edites `code/`; es la base que
ejecutará la siguiente persona.

### Nombra la costura

La costura es `platform.h`. Todo lo que está por encima es C++ portable que no sabe nada de una
terminal. Todo lo que está por debajo es conocimiento de terminal. Cuenta la superficie:

| Lado de la costura | Líneas | Qué sabe |
|---|---:|---|
| `main.cpp` | 11 | Dos llamadas y un marcador. |
| `platform.h` | 5 | Tres nombres, ningún mecanismo. |
| `platform.cpp` | 18 | Bytes ANSI y `stdin`. |

La costura tiene tres funciones de ancho. Es suficientemente pequeña para cambiarla en un paso, que
es el punto.

### Escribe un segundo adaptador

En `build/scratch/platform-plain.cpp`:

```cpp
#include "platform.h"

#include <cstdio>

void set_console_color()
{
}

void clear_console()
{
}

int read_console_key()
{
    return std::getchar();
}
```

Compílalo con el *mismo* llamador:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 \
    build/scratch/platform-plain.cpp build/scratch/main.cpp \
    -o build/scratch/app-plain
./build/scratch/app-plain
```

Esperado:

```text
portable=yes
```

El llamador no cambió. Ni un byte. Eso es **portabilidad de la interfaz**, demostrada con una
compilación.

Ahora mide la diferencia:

```bash
./build/scratch/app-plain > build/scratch/plain.txt
wc -c < build/scratch/plain.txt
```

El archivo tiene 13 bytes: `portable=yes` y un salto de línea. La compilación ANSI escribió 30.
Mismo llamador, distinto comportamiento. Esa es toda la lección en una comparación.

### Opcional: ensanchar el puerto

Si el tema debe ser seleccionable, el puerto necesita un parámetro. Añade:

```cpp
void set_console_color(int code);
```

Luego actualiza ambos adaptadores y el llamador. Cuenta los archivos tocados: cuatro — la cabecera,
dos adaptadores y `main.cpp`. El puerto actual oculta `system("color 1f")` metiendo el argumento
dentro del adaptador. Eso es una **fuga de política**: la elección del tema se movió a la
implementación, donde el llamador no puede verla. No es una victoria de portabilidad, y vale la pena
nombrarla en el registro.

Si no ensanchas el puerto, di por qué. "Un tema basta para este banco" es una declaración de alcance
válida. Una suposición no declarada no lo es.

### Lo que no hiciste

No hiciste que el archivo compile. No escribiste un adaptador de Windows. No probaste una terminal
real. No arreglaste `iostream.h`, `void main` ni los glifos CP437. Escribe esas cuatro frases en la
evidencia. Un puerto que afirma más de lo que hizo es peor que un puerto que nombra su alcance.

## Paso 4 — Compilar (10 min)

### Compila la base

```bash
./run.sh
```

Esperado:

```text
adapter=portable
```

El código de salida es 0. Lee los flags de `run.sh` antes de confiar en el resultado:

- `-std=c++20` fija el dialecto;
- `-Wall -Wextra` pide los avisos;
- `-Werror` convierte un aviso en fallo de compilación;
- `-O2` optimiza, lo que importa para la comprobación de símbolos de abajo.

`-Werror` importa. Un banco que solo compila con los avisos silenciados no es una compilación
portable; es una compilación que no se ha leído.

### Mira los bytes

```bash
./build/platform-adapter | cat -v
```

Esperado:

```text
^[[1;37;44m^[[2J^[[Hportable=yes
```

`cat -v` hace visibles los escapes. Sin él, la terminal los interpreta y ves una pantalla azul con
una línea, que es exactamente la ilusión que vende el adaptador.

Ahora escribe los bytes a un archivo:

```bash
./build/platform-adapter > build/out.txt
od -An -tx1c build/out.txt
wc -c < build/out.txt
```

Decodificado a mano a partir de la salida de `od`:

| Parte | Bytes | Qué es |
|---|---:|---|
| `\033[1;37;44m` | 10 | SGR: negrita, blanco sobre azul. |
| `\033[2J` | 4 | Borra la pantalla. |
| `\033[H` | 3 | Lleva el cursor al inicio. |
| `portable=yes\n` | 13 | El marcador y un salto de línea. |
| **Total** | **30** | |

Diecisiete de los treinta bytes son control de terminal. El mensaje real del programa son trece. Si
pasas la salida por `grep`, `wc` o un archivo de registro, llegan los treinta. El adaptador nunca
pregunta si el destino es una terminal.

### Demuestra que el llamador no tiene cabeceras de plataforma

```bash
grep -n '#include' code/main.cpp code/platform.h code/platform.cpp
```

Esperado:

| Archivo | Includes |
|---|---|
| `code/main.cpp` | `"platform.h"`, `<cstdio>` |
| `code/platform.h` | ninguno |
| `code/platform.cpp` | `"platform.h"`, `<cstdio>` |

Ni `conio.h`. Ni `windows.h`. Ni `system`. Ni `getch`. La única dependencia del llamador es el
puerto y la biblioteca estándar. Esa es la afirmación mecánica que la lección puede defender.

### Demuéstralo a nivel de objeto

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 -c code/platform.cpp -o build/platform.o
nm -C -u build/platform.o
```

En esta plataforma, con `-O2`:

```text
                 U getc
                 U printf
                 U stdin
```

Con `-O0`, el mismo código reporta `getchar` y `printf`. El compilador puede reescribir `getchar` a
`getc` y un `printf` constante a `puts`. No busques el nombre del código en el archivo objeto y
concluyas que la llamada desapareció. El código es el contrato; `nm` es corroboración.

Filtra los símbolos de plataforma:

```bash
nm -C -u build/platform.o | grep -E 'conio|getch|system|clrscr'
```

El comando no imprime nada y sale con 1. Ese es el resultado que quieres: el archivo objeto no
tiene ningún símbolo indefinido de la API de consola de Windows. `getc` y `stdin` son C estándar; no
son una atadura de plataforma.

### Ejercita el puerto no ejercitado

```bash
grep -rn 'read_console_key' code/
```

Esperado: dos coincidencias, la declaración en `platform.h` y la definición en `platform.cpp`. Cero
llamadas. La compilación portable nunca lee una tecla.

Así que el criterio de aceptación "compilación portable" demuestra que el llamador compila y que
dos funciones se ejecutan. No demuestra que `read_console_key` funcione, que espere como esperaba
`getch`, ni que la terminal haga eco o no lo haga. Dilo en el registro. No es un defecto del banco;
es el borde de la evidencia.

### Pregunta al entorno

```bash
TERM=dumb ./build/platform-adapter | cat -v
```

Los bytes son idénticos. El adaptador nunca lee `TERM`, nunca llama a `isatty` y nunca pregunta si
la terminal soporta color. Esa es la diferencia entre "se ejecuta aquí" y "se adapta aquí". Un
adaptador real para un entorno hostil necesita una comprobación de capacidades o una biblioteca de
terminal; este no tiene ninguna de las dos.

Si quieres la versión contundente, compara las dos compilaciones:

```bash
diff <(./build/platform-adapter) <(./build/scratch/app-plain)
```

`diff` sale con 1 y muestra los bytes de escape en un lado y el marcador en el otro. Misma interfaz,
distinto comportamiento. Guarda esa salida.

## Paso 5 — Explicar (13 min)

### La factura en 2008

| Coste | Dónde | Consecuencia |
|---|---|---|
| `#include<conio.h>` | `variant-01:11` | La compilación falla en todo destino que no sea Windows. |
| `#include<iostream.h>` | `variant-01:12` | La compilación falla en toda biblioteca estándar moderna. |
| `void main()` | `variant-01:32` | No es estándar; algunos compiladores lo aceptan, `-Werror` lo rechaza. |
| `system("color 1f")` | `variant-01:38` | Comando de ejecución; falla en silencio donde no existe `color`. |
| `getch()` | `variant-01:42`, 781, 806, 835, 861 | Atadura de compilación; el tiempo de tecla difiere. |
| `system("cls")` | `variant-01:43`, 784, 809, 838, 864 | Comando de ejecución; estado de salida ignorado. |

Las tres primeras filas detienen la compilación. Las tres últimas dejan que la compilación tenga
éxito y cambian el comportamiento. Por eso "ahora compila" no es el final del diagnóstico.

El coste lo paga la audiencia para la que se publicó el archivo. Quien clona el repositorio en Linux
no puede compilar el artefacto en absoluto. Para un archivo publicado, eso es un defecto de
severidad alta: no un píxel equivocado, una compilación fallida. Para una demostración de una sola
máquina, el mismo código es defendible. La severidad tiene que ver con la consecuencia en un
contexto, y el contexto es el que tú escribes.

### Portabilidad de la interfaz, medida

| Pregunta | Portabilidad de la interfaz | Identidad del comportamiento |
|---|---|---|
| ¿Compila el mismo `main.cpp`? | Sí, por construcción. | No está implícito. |
| ¿Son iguales los bytes de salida? | No. | Sí, si mides ambos. |
| ¿Es igual el tiempo de tecla? | No. | No: Enter frente a una tecla. |
| ¿Es igual el estado de pantalla? | No. | Depende de la terminal. |
| ¿Es igual el color? | No. | Depende de la paleta. |
| ¿Es igual el código de salida? | Sí, para este banco. | Sí, para este banco. |

La tabla es la respuesta a la pregunta central de la lección. El adaptador convierte una dependencia
de compilación en una dependencia de ejecución. Eso es progreso: un fallo de compilación es total,
y una diferencia de ejecución se puede detectar, documentar y, si hace falta, adaptar. No es
identidad.

### Dos arreglos, dos tamaños

**Arreglo mínimo — tres funciones en una unidad de traducción.** Reemplaza las llamadas de consola
del llamador por `set_console_color`, `clear_console` y `read_console_key`; pon el código de
plataforma en `platform.cpp`; mantén el llamador libre de `conio.h`. Eso es el banco. Elimina la
atadura de compilación y le da a un revisor un solo archivo que auditar.

**Arreglo estructural — un puerto de consola con contrato.** Declara las capacidades que asume el
puerto, añade un adaptador sin consola para pruebas y tuberías, y deja que el llamador elija el
adaptador. Si el programa necesita negociación real de capacidades, usa una biblioteca de terminal
(`ncurses`, `PDCurses`) en lugar de escapes escritos a mano. Mantén el tema y la política de pausa
en el llamador o en una capa de presentación, no dentro del adaptador. Entonces una prueba puede
enlazar el adaptador vacío y afirmar el comportamiento del llamador sin una terminal.

Enlaces a la pista A: `construction-08` (frontera de módulo) es el prerrequisito;
`construction-20` (hexagonal, puertos y adaptadores) es el peldaño directo; `common-05` (trazas de
referencia) es cómo demostrarías que la salida de consola no cambió al cambiar el adaptador.

### Lo que el adaptador no arregla

- **El dialecto del lenguaje.** `iostream.h` y `void main` siguen en el archivo.
- **La codificación.** Los bytes CP437 21, 156 y 219 siguen sin ser UTF-8.
- **El puerto de salida.** `main.cpp` todavía imprime el marcador; no hay adaptador de prueba para
  el texto.
- **El contrato de tecla.** `std::getchar` no es `getch`; el nombre no lo convierte en eso.
- **La detección de capacidades.** El adaptador nunca pregunta qué puede hacer la terminal.
- **El archivo.** Nada de esto toca `original/`.
- **Las pruebas y la semilla.** `archaeology-14` es dueña de esa brecha.
- **La validación y el manejo de errores.** El puerto no devuelve error y el llamador no comprueba
  nada.

### El intercambio, dicho con justicia

Un adaptador cuesta una cabecera, una unidad de traducción y una indirección. Una demostración de
consola para un solo destino no lo necesita: `system("color 1f")` es más corto y no hay un segundo
destino que justifique el archivo. El coste aparece en el segundo destino — un ejecutor de CI, un
lector en macOS, un arnés de pruebas, una tubería. En ese momento la llamada directa cobra el precio
completo: la compilación falla, y el fallo está en el llamador, mezclado con el juego.

Elige con la prueba de cambio, no con gusto: añade un destino, cuenta los archivos que deben
cambiar. Si la respuesta es "un adaptador", el puerto se pagó solo. Si la respuesta es "todos los
archivos que imprimen", no.

## Paso 6 — Registrar (5 min)

Rellena [`EVIDENCE-es.md`](EVIDENCE-es.md). Como mínimo:

- configuración: compilador, libc, núcleo, etiqueta y commit del archivo;
- las cinco predicciones, escritas antes de inspeccionar;
- los recuentos de líneas y el puerto de tres funciones;
- los anclajes del archivo: líneas 11–12, 32, 38, 42–43, y los recuentos 5 `getch` / 5 `cls`;
- la ejecución base, su código de salida y los 30 bytes con los 17 de escape nombrados;
- la compilación del adaptador simple: mismo llamador, 13 bytes, sin escapes;
- la comprobación de símbolos del objeto y la nota sobre la optimización `getc` / `getchar`;
- el recuento de `read_console_key`: dos coincidencias, cero llamadas;
- la prueba `TERM=dumb` y lo que no cambia;
- la matriz de capacidades: escape, color, limpieza, tecla, redirección, codificación;
- la distinción entre portabilidad de la interfaz e identidad del comportamiento, con la medición
  30/13;
- la frontera de evidencia: el marcador demuestra la compilación y la ruta de llamada, no la
  terminal;
- la comprobación de integridad del archivo.

Una frase como "el adaptador hace portable la consola" es una opinión. "El mismo `main.cpp` enlaza
contra `platform.cpp` y `platform-plain.cpp`; el primero escribe 30 bytes, el segundo 13; el código
del llamador no cambia; `nm -C -u platform.o` no tiene `conio`, `getch` ni `system`" es un registro.

El criterio de aceptación de esta lección son dos hechos:

```text
adapter=portable
```

y una segunda compilación donde el mismo llamador enlaza un adaptador distinto. Si no puedes
producir ambos, la línea sin marcar nombra el siguiente paso. No hay penalización por repetir uno.

## El original en detalle

Hechos que puedes citar, con números de línea de `variant-01` en la etiqueta `v1.0.0`:

| Ubicación | Qué hay |
|---:|---|
| 11 | `#include<conio.h>` — la API de consola. |
| 12 | `#include<iostream.h>` — el iostream anterior al estándar. |
| 32 | `void  main()` — sin tipo de retorno estándar. |
| 38 | `system("color 1f")` — el tema, mediante un comando de shell. |
| 42 | `getch()` — la primera pausa. |
| 43 | `system("cls")` — la primera limpieza. |
| 781, 806, 835, 861 | `getch()` — cada turno espera una tecla. |
| 784, 809, 838, 864 | `system("cls")` — cada turno limpia la pantalla. |
| 930 | Fin del archivo; sin archivo de compilación, sin prueba, sin nota de plataforma. |

Las otras variantes repiten el patrón. `variant-00` es el contraste interesante: usa `int main()`
en 359 y `cin>>caracter` en 370, así que es más portable en el punto de entrada y en la entrada, y
sigue llamando a `system("color 1f")` en 363 y a `system("cls")` en 373. Las variantes de punteros
cambiaron el contenedor; no cambiaron la consola. Vale la pena notarlo: un cambio de sintaxis no es
un puerto de plataforma.

El `05-known-issues.md` del archivo registra las dos filas que cita esta lección: el comando de
consola de Windows en `variant-00:363`, `variant-01:38`, `variant-02:43`, `variant-03:53` y
`variant-04:59`, y las cabeceras anteriores al estándar en las líneas 11–12 de cada variante.

## Qué demuestra y qué no demuestra este banco

| Afirmación | ¿Respaldada? | Por qué |
|---|---|---|
| El llamador compila sin `conio.h`. | Sí | Solo incluye `platform.h` y `<cstdio>`. |
| El llamador enlaza contra dos adaptadores distintos. | Sí | `platform.cpp` y `platform-plain.cpp`. |
| El objeto del adaptador no tiene símbolos de la API de consola. | Sí | `nm -C -u` no encuentra `conio`, `getch` ni `system`. |
| Los bytes de salida son idénticos entre adaptadores. | No | 30 frente a 13 bytes. |
| `read_console_key` funciona como `getch`. | No | Nunca se llama, y `getchar` difiere. |
| El adaptador detecta capacidades de terminal. | No | Nunca lee `TERM` ni llama a `isatty`. |
| El archivo compila ahora. | No | El banco es un programa separado de 34 líneas. |
| La codificación del archivo está arreglada. | No | Los bytes CP437 están intactos. |
| El `void main` del archivo está arreglado. | No | El banco usa `int main`; el archivo no. |
| El arnés prueba la terminal. | No | Comprueba una subcadena en stdout capturado. |
| El adaptador es gratis. | No | Cuesta una cabecera, una unidad y una indirección. |

Esta tabla es la comprobación de honestidad. Un registro que dice "la consola es portable" sin
nombrar la terminal, el adaptador y los bytes está exagerando. Di "el llamador compila contra un
puerto de consola; el adaptador ANSI escribe 30 bytes y el simple escribe 13; el comportamiento de
tecla no está verificado" cuando eso es lo que ejecutaste.

## Autocomprobación (binaria)

- [ ] Escribí las cinco predicciones antes de abrir el código.
- [ ] Leí `platform.h`, `platform.cpp`, `main.cpp` y `run.sh`.
- [ ] Registré los recuentos: 5, 18, 11 y 19.
- [ ] Ejecuté `./run.sh` y vi `adapter=portable` con código de salida 0.
- [ ] Encontré las seis construcciones del archivo: `conio.h`, `iostream.h`, `void main`,
  `system("color")`, `system("cls")` y `getch()`.
- [ ] Conté 5 `getch` y 5 `system("cls")` en `variant-01`.
- [ ] Comparé `variant-00` y nombré qué hace mejor y qué sigue compartiendo.
- [ ] Copié el banco a `build/scratch/` antes de editar.
- [ ] Escribí `platform-plain.cpp` y enlacé el `main.cpp` sin cambios contra él.
- [ ] Medí 30 bytes para el adaptador ANSI y 13 para el simple.
- [ ] Usé `cat -v` u `od` para mostrar los 17 bytes de escape.
- [ ] Demostré que el llamador no tiene cabeceras de plataforma con `grep` y `nm`.
- [ ] Expliqué la optimización `getc` / `getchar` y no confié en un grep de nombres.
- [ ] Encontré que `read_console_key` tiene dos coincidencias y cero llamadas.
- [ ] Ejecuté `TERM=dumb` y registré que los bytes no cambiaron.
- [ ] Distinguí portabilidad de la interfaz de identidad del comportamiento, con el ejemplo 30/13.
- [ ] Completé la matriz de capacidades: escape, color, limpieza, tecla, redirección, codificación.
- [ ] Nombré lo que el adaptador no arregla: dialecto, codificación, salida, tecla, detección,
  archivo.
- [ ] Precifiqué la dependencia por el fallo de compilación y declaré mi contexto.
- [ ] Enlacé el arreglo estructural con `construction-08`, `construction-20` y `common-05`.
- [ ] El archivo está sin modificar (`git status` en el archivo está limpio).

Si una casilla no está marcada, la casilla sin marcar nombra el siguiente paso. No hay penalización
por repetir uno.

## Misconcepciones que esta lección refuta

| Concepto erróneo | Por qué es falso | Qué hacer en su lugar |
|---|---|---|
| "Compila en Linux, así que es portable." | La compilación es portable; el comportamiento puede no serlo. | Mide los bytes y la interacción de tecla. |
| "El adaptador eliminó la dependencia de plataforma." | Movió la dependencia detrás de un nombre. | Di dónde vive ahora el código de plataforma. |
| "Las secuencias ANSI funcionan en todas partes." | Consolas antiguas de Windows y `TERM=dumb` muestran los bytes como texto. | Detecta la terminal o usa una biblioteca. |
| "`getchar` es un `getch` portable." | El eco, el búfer y el bloqueo difieren. | Declara el contrato de tecla; pruébalo. |
| "Los escapes son inofensivos en una tubería." | Un archivo recibe 17 bytes de control. | Pregunta si `stdout` es una terminal. |
| "`system("cls")` y `ESC[2J` son lo mismo." | El búfer de consola, el área visible y el scrollback difieren. | Define el contrato de limpieza. |
| "Si `run.sh` pasa, la consola funciona." | Comprueba una subcadena y nunca lee una tecla. | Prueba el puerto, no solo la compilación. |
| "El adaptador arregla `void main` e `iostream.h`." | Es un puerto de consola, no una migración de lenguaje. | Arregla el dialecto por separado. |
| "El adaptador arregla los glifos CP437." | La codificación es una capa separada. | Convierte los glifos explícitamente. |
| "Un puerto es una reescritura." | Este puerto tiene tres funciones de ancho. | Empieza por la costura, no por el programa. |
| "La portabilidad es gratis." | Cuesta una cabecera, una unidad y una indirección. | Precifícala con la prueba de cambio. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Dependencia de plataforma | Una compilación o ejecución correcta depende de un entorno. |
| Puerto | La forma de la conversación que el llamador tiene permitido mantener. |
| Adaptador | Una implementación de un puerto para un entorno. |
| Portabilidad de la interfaz | El mismo código del llamador compila contra cada adaptador. |
| Identidad del comportamiento | La misma interacción observable en cada destino. |
| Capacidad | Lo que una terminal puede hacer: color, limpieza, tiempo de tecla. |
| Secuencia de escape ANSI | Una secuencia de control que empieza con `ESC` (`0x1b`). |
| SGR | Select Graphic Rendition: la secuencia de color `ESC[...m`. |
| Modo canónico | Entrada por líneas que espera a Enter. |
| Eco | La terminal mostrando la tecla que pulsaste. |
| Redirección | Enviar `stdout` a un archivo o tubería en lugar de una terminal. |
| CP437 | La página de códigos IBM de 1981 usada por los glifos del archivo. |
| Terminal virtual | Soporte de Windows para procesar escapes ANSI. |
| Adaptador vacío | Una implementación que satisface el puerto y no hace nada. |
| Frontera de evidencia | La línea entre lo que la ejecución demuestra y lo que infieres. |

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
