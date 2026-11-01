# Entorno y compatibilidad

> [English](environment.md) | [Español](environment-es.md)

Este es el único lugar para los requisitos de entorno del laboratorio. El archivo heredado tiene su
propia historia de plataforma; ese es otro tema y está cubierto por `archaeology-16`.

## Entorno principal

- Linux o macOS con shell POSIX.
- Bash 4 o posterior.
- Un compilador C++20: `g++` o `clang++`.
- Python 3.10 o posterior.
- CMake opcional; `run.sh` usa una compilación directa cuando CMake no está.
- Herramientas POSIX estándar: `find`, `grep`, `sed`, `awk`, `cmp`, `mktemp`, `timeout`,
  `sha256sum`.

## Qué es portable

- El código C++20 en `construction/`, `reference/` y `archaeology/` usa la biblioteca estándar.
- El código Python usa solo la biblioteca estándar.
- La lógica del juego, el LCG y el contrato de transcripción son independientes del lenguaje y del
  sistema operativo.
- `construction-21` usa `std::thread` y `std::mutex`; CMake enlaza `Threads::Threads`.

## Qué está atado a POSIX

La automatización es Bash:

- `run.sh`, `build_probe.sh`, `capture_trace.sh`, `measure.sh` y el ejecutor de pruebas de fallo.
- Usan `mktemp`, `timeout`, `sed -i`, `cmp`, `sha256sum`, `/dev/null`, `xargs -0` y sustitución de
  procesos.
- `construction-19/code/benchmark.cpp` codifica `/dev/null`.
- Los archivos CMake pasan `-Wall -Wextra -Werror`, que son banderas de GCC/Clang.

## Por qué Linux para ingenieros de software

Linux es el entorno principal del laboratorio porque da acceso directo a las herramientas que usa la
ingeniería de software:

- **Shell POSIX y tuberías.** La automatización se compone: `grep`, `sed`, `awk`, `find`, `xargs`
  y la redirección funcionan igual en scripts, CI y sesiones remotas.
- **Cadenas de herramientas nativas.** Compiladores, depuradores, perfiladores, enlazadores y
  gestores de paquetes están disponibles sin una capa de compatibilidad.
- **Paridad con producción.** La mayoría de servidores y runners de CI son Linux, así que el
  comportamiento local se parece más al de despliegue.
- **Contenedores y aislamiento.** Docker, Podman y herramientas similares corren de forma nativa; la
  misma imagen funciona localmente y en CI.
- **Modelo de sistema transparente.** Archivos, permisos, procesos, señales, sockets y variables de
  entorno son visibles y automatizables.
- **Trabajo remoto.** SSH, tmux y las herramientas de terminal hacen que las máquinas remotas y los
  servidores sean de primera clase.
- **Ecosistema abierto.** La documentación, el código fuente y los repositorios de paquetes están
  disponibles directamente.

WSL2 lleva este entorno a Windows sin arranque dual. Es la única ruta compatible en Windows porque
conserva el shell POSIX, la cadena de herramientas y la semántica de rutas de las que depende el
repositorio.

## Leer los archivos Markdown

Las lecciones están en Markdown. Un editor de texto plano muestra el código fuente; un visor de
Markdown renderiza encabezados, tablas, enlaces y bloques de código.

### Linux

Okular es el visor de escritorio recomendado:

```bash
okular README.md
```

Instálalo con tu gestor de paquetes, por ejemplo `sudo apt install okular` en Debian/Ubuntu,
`sudo dnf install okular` en Fedora, o `sudo pacman -S okular` en Arch.

Alternativas de terminal: `glow README.md` o `mdcat README.md`. Para verlo en el navegador:
`pandoc README.md -o README.html`.

### Otros entornos

- **Windows:** Visual Studio Code con vista previa de Markdown, Obsidian o Typora. El laboratorio
  sigue ejecutándose solo dentro de WSL2; estas herramientas son para leer la documentación.
- **macOS:** Visual Studio Code, Obsidian, MacDown o Typora.
- **Cualquier entorno:** GitHub renderiza los archivos del repositorio; `pandoc` convierte Markdown
  a HTML o PDF.

No hace falta una vista renderizada para ejecutar el código, pero facilita mucho la lectura de
tablas, enlaces y bloques de código.

## Windows

WSL2 es la única opción compatible en Windows. El repositorio funciona tal como está dentro de WSL2.
Las shells y herramientas nativas de Windows no son flujos de trabajo soportados.

## Excepciones de plataforma conocidas

- `construction-19`: el benchmark redirige a `/dev/null`; es un banco solo para POSIX.
- `construction-16`: el adaptador usa secuencias de escape ANSI; la terminal debe soportarlas.
- `construction-21`: los hilos son portables mediante `Threads::Threads`, pero el comando manual
  `-pthread` es específico de POSIX.
- `archaeology-01` y `archaeology-03`: las sondas de compilación heredada usan `g++`,
  `-fpermissive`, `timeout` y `/dev/null`.

## Archivo heredado

El código original apuntaba a un compilador y una consola de Windows de 2007. El laboratorio no lo
compila de forma nativa; las sondas de compatibilidad compilan una copia de trabajo con un
compilador moderno y una pequeña capa de compatibilidad. El archivo sigue siendo la evidencia.

## Contenedores y CI

No se incluye Dockerfile, devcontainer ni flujo de CI. WSL2 es la ruta práctica en Windows; un
contenedor sería un artefacto adicional, no parte del repositorio actual.
