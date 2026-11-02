# archaeology-16 — Evidencia: dependencia de plataforma

> Este es tu registro. Escríbelo para tu yo futuro.
> Registra lo que observaste, no lo que la lección parecía querer.

## 1. Configuración

- Fecha:
- Ruta del archivo:
- Etiqueta en uso:
- Hash del commit (`git rev-parse HEAD`):
- Compilador y versión (`c++ --version`):
- Biblioteca de C (`ldd --version` primera línea, o `getconf GNU_LIBC_VERSION`):
- Shell y versión:
- Núcleo y distribución (`uname -sr`):
- Terminal y `TERM`:
- Directorio de trabajo:
- Directorio temporal usado:
- Algo inesperado en la configuración:

## 2. Predicción — escribir antes de inspeccionar la fuente

No edites esta tabla después del paso 2. La brecha entre predicción y observación es la señal de
aprendizaje; reparar la predicción la destruye.

| # | Pregunta | Mi predicción | Cómo sabría que me equivoco |
|---:|---|---|---|
| 1 | ¿`main.cpp` incluye una cabecera de plataforma? | | |
| 2 | ¿La salida de `./run.sh` contiene bytes de escape? | | |
| 3 | ¿Un adaptador vacío enlaza el mismo llamador? | | |
| 4 | ¿`std::getchar()` se comporta como `getch()`? | | |
| 5 | ¿Cuántos bytes escribe una ejecución redirigida? | | |

Una frase falsable sobre la portabilidad en este banco:

>

## 3. Inspección de la línea base

Ficheros:

```text
code/platform.h
code/platform.cpp
code/main.cpp
run.sh
```

Recuento de líneas:

| Fichero | Líneas | ¿Verificado? |
|---|---:|---|
| `code/platform.h` | 5 | |
| `code/platform.cpp` | 18 | |
| `code/main.cpp` | 11 | |
| `run.sh` | 19 | |

El puerto:

| Función | Declarada en | Implementada en | ¿La llama `main.cpp`? |
|---|---|---|---|
| `set_console_color` | `platform.h:3` | `platform.cpp:5-8` | |
| `clear_console` | `platform.h:4` | `platform.cpp:10-13` | |
| `read_console_key` | `platform.h:5` | `platform.cpp:15-18` | |

Includes:

| Fichero | Includes | ¿Cabecera de plataforma? |
|---|---|---|
| `code/main.cpp` | | |
| `code/platform.h` | | |
| `code/platform.cpp` | | |

Qué llama `main.cpp`:

Qué no llama `main.cpp`:

Cómo captura stdout `run.sh`:

Qué tolera la comprobación de subcadena:

Qué no prueba el arnés:

## 4. Inspección del archivo

Anclajes verificados en
`original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`:

| Elemento | Línea o recuento | ¿Verificado? |
|---|---:|---|
| `#include<conio.h>` | 11 | |
| `#include<iostream.h>` | 12 | |
| `void  main()` | 32 | |
| `system("color 1f")` | 38 | |
| `getch()` en `main` | 42 | |
| `system("cls")` en `main` | 43 | |
| `getch()` en el bucle de turnos | 781, 806, 835, 861 | |
| `system("cls")` en el bucle de turnos | 784, 809, 838, 864 | |
| Llamadas a `getch` en el fichero | 5 | |
| Llamadas a `system("cls")` en el fichero | 5 | |
| Llamadas a `system("color")` en el fichero | 1 | |
| Líneas del fichero | 930 | |

Las otras variantes:

| Variante | Punto de entrada | `getch` | `system("cls")` | `system("color")` |
|---|---|---:|---:|---:|
| `variant-00-precursor-2007` | `int main()` en 359 | | | |
| `variant-01-baseline-2008` | `void main()` en 32 | 5 | 5 | 1 |
| `variant-02-records-2008` | `void main()` en 37 | 5 | 5 | 1 |
| `variant-03-pointers-dev-2008` | `int main()` en 47 | 5 | 5 | 1 |
| `variant-04-pointers-final-2008` | `void main()` en 52 | 5 | 5 | 1 |

Qué hace mejor `variant-00` que `variant-01`:

Qué sigue compartiendo `variant-00` con `variant-01`:

Qué dice el `05-known-issues.md` del archivo sobre la consola y las cabeceras:

>

Dependencia de compilación frente a dependencia de ejecución:

| Construcción | ¿Falla al compilar o al ejecutar? | Cómo se ve el fallo |
|---|---|---|
| `#include<conio.h>` | | |
| `#include<iostream.h>` | | |
| `void main()` | | |
| `getch()` | | |
| `system("color 1f")` | | |
| `system("cls")` | | |

La prueba de shell:

```bash
sh -c 'cls'; printf 'exit=%s\n' "$?"
```

Salida observada:

- Código de salida:
- Qué hace el archivo con ese código de salida:

## 5. Ejecución de la base

Comando:

```bash
cd archaeology/lessons/archaeology-16-platform-lock-in
./run.sh
```

Salida observada:

```text
adapter=portable
```

- Código de salida:
- Compilador usado:
- Flags observados en `run.sh`:
- ¿La compilación pasó con `-Werror`? Si no, ¿qué diagnóstico apareció?

Los bytes:

```bash
./build/platform-adapter | cat -v
./build/platform-adapter > build/out.txt
od -An -tx1c build/out.txt
wc -c < build/out.txt
```

- Salida de `cat -v`:
- Salida de `od`:
- Total de bytes:
- Bytes que son control de terminal:
- Bytes que son el mensaje:

| Parte | Bytes | Qué es |
|---|---:|---|
| `\033[1;37;44m` | 10 | |
| `\033[2J` | 4 | |
| `\033[H` | 3 | |
| `portable=yes\n` | 13 | |
| **Total** | **30** | |

- ¿`run.sh` mostró los escapes? ¿Por qué sí o por qué no?
- Qué concluiría erróneamente quien solo lee la salida de `run.sh`:

## 6. El segundo adaptador

Ruta temporal:

```text
build/scratch/
```

Ficheros copiados:

El adaptador simple que escribí:

```cpp

```

Comando de compilación:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 \
    build/scratch/platform-plain.cpp build/scratch/main.cpp \
    -o build/scratch/app-plain
```

- Resultado de la compilación:
- Salida observada:
- Código de salida:
- Bytes de la salida:
- ¿Cambió `main.cpp`? (Debe ser no.)

La comparación:

```bash
diff <(./build/platform-adapter) <(./build/scratch/app-plain)
```

- Salida de `diff`:
- Código de salida de `diff`:
- Qué cambió:
- Qué no cambió:
- ¿Qué afirmación demuestra esto: portabilidad de la interfaz o identidad del comportamiento?

Puerto ensanchado opcional:

- ¿Añadí un parámetro de color? ¿Por qué sí o por qué no?
- Ficheros tocados:
- Qué oculta el puerto estrecho sobre el tema:

Lo que no hice:

- El archivo sigue sin compilar porque:
- No escribí un adaptador de Windows porque:
- No probé una terminal real porque:
- No arreglé la codificación porque:

## 7. Prueba de símbolos e includes

Includes:

```bash
grep -n '#include' code/main.cpp code/platform.h code/platform.cpp
```

Observado:

Símbolos del objeto:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 -c code/platform.cpp -o build/platform.o
nm -C -u build/platform.o
```

Observado con `-O2`:

Observado con `-O0`:

Por qué difieren los nombres:

El filtro:

```bash
nm -C -u build/platform.o | grep -E 'conio|getch|system|clrscr'
```

- Salida:
- Código de salida:
- Qué demuestra:

El puerto no ejercitado:

```bash
grep -rn 'read_console_key' code/
```

- Coincidencias:
- Llamadas:
- Qué no demuestra por tanto la ejecución de aceptación:

## 8. Matriz de capacidades de terminal

Completa esto con tu entorno y con lo que el código asume. Marca `medido` cuando lo ejecutaste,
`asumido` cuando lo inferiste, y `desconocido` cuando no lo comprobaste.

| Entorno | Escape | Color | Limpieza | Tecla | Cómo lo sé |
|---|---|---|---|---|---|
| Mi terminal | | | | | |
| Tubería o fichero | | | | | |
| `TERM=dumb` | | | | | |
| Registro de CI | | | | | |
| tmux o screen | | | | | |
| Terminal de Windows 10+ | | | | | |
| `cmd.exe` antiguo | | | | | |

La prueba `TERM=dumb`:

```bash
TERM=dumb ./build/platform-adapter | cat -v
```

- Salida:
- ¿Cambiaron los bytes?
- ¿El adaptador leyó `TERM`?
- ¿El adaptador llamó a `isatty`?

El contrato de tecla:

| Propiedad | `getch()` | `std::getchar()` |
|---|---|---|
| Eco | | |
| Espera | | |
| Búfer | | |
| Fin de entrada | | |
| Redirección | | |

La prueba de codificación (opcional):

- Bytes impresos:
- Qué mostró la terminal:
- Por qué el adaptador no lo arregla:

## 9. Portabilidad de la interfaz frente a identidad del comportamiento

| Afirmación | Evidencia que tengo | ¿Respaldada? |
|---|---|---|
| El mismo llamador compila contra dos adaptadores. | | |
| Los dos adaptadores producen los mismos bytes. | | |
| Los dos adaptadores limpian la pantalla igual. | | |
| Los dos adaptadores leen una tecla igual. | | |
| El llamador está libre de cabeceras de plataforma. | | |
| El archivo ya es portable. | | |

- Portabilidad de la interfaz, en una frase:
- Identidad del comportamiento, en una frase:
- La medición que las separa:
- Cuál compra el adaptador:
- Cuál no compra:

## 10. Explicación

El coste de 2008, en un párrafo. Nombra las cabeceras, el punto de entrada, el comando de color y
las llamadas de tecla. Di qué impide cada uno, y para quién.

>

Dos arreglos, dos tamaños:

- Arreglo mínimo:
- Arreglo estructural:
- Enlaces a la pista A:
- Qué añade `construction-08` a la pregunta:
- Qué añade `construction-20` a la pregunta:
- Qué añade `common-05` a la pregunta:

Lo que el adaptador no arregla:

- El dialecto del lenguaje:
- La codificación:
- El puerto de salida:
- El contrato de tecla:
- La detección de capacidades:
- El archivo:
- Las pruebas y la semilla:
- La validación y el manejo de errores:

La prueba de cambio:

| Cambio | Llamadas directas | Adaptador |
|---|---|---|
| Añadir un segundo destino | | |
| Añadir una compilación sin consola | | |
| Cambiar el tema | | |
| Añadir una prueba que capture salida | | |
| Ejecutar bajo una tubería | | |

## 11. Frontera de evidencia

- Verificado leyendo el código:
- Verificado compilando:
- Verificado ejecutando el arnés:
- Verificado midiendo bytes:
- Verificado con `nm`:
- Inferido:
- No medido:
- Desconocido:

## 12. Qué demuestra y qué no demuestra el banco

| Afirmación | ¿Respaldada? | Por qué |
|---|---|---|
| El llamador compila sin `conio.h`. | | |
| El llamador enlaza contra dos adaptadores. | | |
| El objeto del adaptador no tiene símbolos de la API de consola. | | |
| Los bytes de salida son idénticos entre adaptadores. | | |
| `read_console_key` funciona como `getch`. | | |
| El adaptador detecta capacidades de terminal. | | |
| El archivo compila ahora. | | |
| La codificación del archivo está arreglada. | | |
| El `void main` del archivo está arreglado. | | |
| El arnés prueba la terminal. | | |
| El adaptador es gratis. | | |

## 13. Severidad y coste

Un párrafo. Nombra el coste en el trabajo real, no la fealdad del código. Di si el coste es alto o
crítico en tu contexto y por qué. No propongas una reescritura en este párrafo.

>

## 14. Integridad del archivo

- `git status` en el archivo:
- ¿Edité `original/`? (Debe ser no.)
- ¿Apliqué algún parche dentro del archivo? (Debe ser no.)
- ¿Dónde guardé mi copia temporal?
- ¿Edité `code/`? (Debe ser no.)

## 15. Lista de aceptación

- [ ] Escribí las cinco predicciones antes de inspeccionar la fuente.
- [ ] Leí los tres ficheros y `run.sh`, y registré los recuentos 5/18/11/19.
- [ ] Compilé con los flags del banco y registré el compilador.
- [ ] Ejecuté `./run.sh` y vi `adapter=portable` con código de salida 0.
- [ ] Demostré que el llamador solo incluye `platform.h` y `<cstdio>`.
- [ ] Copié el banco a `build/scratch/` antes de editar.
- [ ] Escribí `platform-plain.cpp` y enlacé el `main.cpp` sin cambios contra él.
- [ ] Medí 30 bytes para el adaptador ANSI y 13 para el simple.
- [ ] Usé `cat -v` u `od` para mostrar los 17 bytes de escape.
- [ ] Inspeccioné `nm -C -u` y no encontré `conio`, `getch` ni `system`.
- [ ] Expliqué la optimización `getc` / `getchar`.
- [ ] Registré que `read_console_key` tiene dos coincidencias y cero llamadas.
- [ ] Ejecuté `TERM=dumb` y registré que los bytes no cambiaron.
- [ ] Completé la matriz de capacidades de terminal.
- [ ] Expuse las diferencias entre `getch` y `getchar` en eco, búfer y bloqueo.
- [ ] Distinguí portabilidad de la interfaz de identidad del comportamiento, con el ejemplo 30/13.
- [ ] Registré los anclajes del archivo: líneas 11–12, 32, 38, 42–43, 5 `getch`, 5 `cls`.
- [ ] Nombré lo que el adaptador no arregla.
- [ ] Precifiqué el defecto por el fallo de compilación y declaré mi contexto.
- [ ] Enlacé el arreglo estructural con `construction-08`, `construction-20` y `common-05`.
- [ ] El archivo está sin modificar.

## 16. Siguiente paso

- Continúa con `archaeology-17` — revisión de código de una función heredada.
- Si el contrato de tecla te sorprendió, ejecuta `getchar` con y sin tubería y registra cuándo
  devuelve `EOF`.
- Si quieres el lado de Windows, añade un `platform-win32.cpp` detrás de `#ifdef _WIN32` en una
  copia temporal y cuenta los ficheros tocados.
- Si te interesa la codificación, imprime los bytes 21, 156 y 219 como UTF-8 y compara los glifos.
- Si no estás de acuerdo con la severidad, escribe el contexto que la cambia.
