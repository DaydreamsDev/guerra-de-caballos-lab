# construction-02 — Línea base estructurada

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** secuencia, selección e iteración · **Prerrequisito:**
> construction-01 · **Tiempo estimado:** 65 minutos

Este directorio contiene el artefacto **después** de `construction-02`. Tiene el mismo
comportamiento observable que `construction-01`, pero el bucle del juego y los intentos de
movimiento usan secuencia, selección e iteración en lugar de etiquetas y `goto`.

## Qué implementa el programa

- El generador lineal congruencial de 64 bits de `common/spec/game-rules.md`.
- El tablero de 8×8, los dos caballos, las posiciones iniciales y las tiradas iniciales.
- La prioridad de movimientos, los movimientos legales e ilegales, las casillas visitadas, la
  captura, la trampa y el empate a los 200 movimientos.
- El formato de transcripción de `common/spec/transcript-format.md`, byte a byte.

El programa acepta `--seed INTEGER --name TEXT` en cualquier orden y escribe la transcripción en la
salida estándar.

## Archivos

| Archivo | Propósito |
|---|---|
| `code/structured-baseline.cpp` | El juego con bucles, condicionales y sin `goto`. |
| `CMakeLists.txt` | Compilación C++20 con `-Wall -Wextra -Werror`. |
| `run.sh` | Compila cuando hace falta y ejecuta el binario. |

## Compilar y ejecutar

```bash
./run.sh --seed 17 --name Learner
```

`run.sh` usa CMake cuando está instalado. Cuando CMake no está disponible, compila directamente con
el compilador indicado por `CXX`, o `c++` por defecto:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
    -o build/structured-baseline code/structured-baseline.cpp
```

El binario se escribe en `build/structured-baseline`.

## Verificar las trazas de referencia

Desde este directorio:

```bash
for trace in ../../../common/traces/seed-*.txt; do
    traceName="$(basename "${trace}" .txt)"
    seedText="${traceName#seed-}"
    seedNumber="$((10#${seedText}))"
    ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

Las 20 trazas, de `seed-0001.txt` a `seed-0020.txt`, coinciden byte a byte.

## Por qué esta línea base es estructurada

- El bucle del juego es un `while` con una sola condición terminal.
- Los intentos de movimiento son un `for` con una sola condición de salida.
- Cada rama continúa el bucle de intentos o termina el bucle del juego.
- No hay etiquetas ni `goto`.
- El estado que controla la siguiente iteración es visible en la condición del bucle y en las
  asignaciones inmediatamente anteriores.

Las funciones auxiliares del generador y de conversión de celdas permanecen. `construction-03` trata
de extraer la lógica del juego a funciones con parámetros; esta lección aísla el flujo de control.

## Prueba de cambio

Aplicar **R-SKIP**: al inicio de un turno, si el número de turnos iniciados es un múltiplo positivo
de cinco, el caballo activo no se mueve. El turno se anuncia igualmente con sus campos `turn=` y
`order=`; no sigue ninguna línea `try=` ni `move=`. El turno pasa al otro caballo y el turno saltado
no cuenta para los 200 movimientos exitosos.

Predecir el coste antes de editar. Contar los lugares que deben cambiar y compararlos con el mismo
cambio en `construction-01`. Registrar los archivos tocados, las líneas cambiadas, los conceptos
nuevos necesarios y las trazas rotas.

## Criterio de aceptación

La persona que aprende puede aplicar el mismo requisito R-SKIP a ambas líneas base y explicar por
qué la versión estructurada localiza el cambio mientras la no estructurada lo dispersa.
