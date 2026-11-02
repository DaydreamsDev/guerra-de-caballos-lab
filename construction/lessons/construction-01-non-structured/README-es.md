# construction-01 — Línea base de programación no estructurada

> [English](README.md) | [Español](README-es.md)

> **Estado:** borrador · **Concepto:** flujo de control arbitrario ·
> **Prerrequisito:** construction-00 · **Tiempo estimado:** 70 minutos

Este directorio contiene el artefacto **anterior** de `construction-01`. Es intencionalmente no
estructurado: el estado del juego vive en variables globales y el bucle del juego y los intentos de
movimiento se gobiernan con etiquetas y `goto`. No es una recomendación de estilo. Su propósito es
hacer observable el flujo de control arbitrario antes de que `construction-02` lo sustituya por
secuencia, selección e iteración.

## Qué implementa el programa

- El generador lineal congruencial de 64 bits exacto de `common/spec/game-rules.md`.
- El tablero de 8×8, ambos caballos, las posiciones iniciales y las tiradas iniciales.
- La prioridad de movimientos, los movimientos legales e ilegales, las celdas visitadas, la captura,
  la trampa y el empate a los 200 movimientos.
- El formato de transcripción de `common/spec/transcript-format.md`, byte por byte.

El programa acepta `--seed INTEGER --name TEXT` en cualquier orden y escribe la transcripción en la
salida estándar.

## Archivos

| Archivo | Propósito |
|---|---|
| `code/non-structured-baseline.cpp` | El juego: un `main`, globales, etiquetas y `goto`. |
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
    -o build/non-structured-baseline code/non-structured-baseline.cpp
```

El binario se escribe en `build/non-structured-baseline`.

## Verificar las trazas doradas

Desde este directorio:

```bash
for trace in ../../../common/traces/seed-*.txt; do
    traceName="$(basename "${trace}" .txt)"
    seedText="${traceName#seed-}"
    seedNumber="$((10#${seedText}))"
    ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

Las 20 trazas, desde `seed-0001.txt` hasta `seed-0020.txt`, coinciden byte por byte con GCC 16.2.1.
`cmp` no imprime nada cuando la comparación es correcta.

## Por qué esta línea base no es estructurada

El comportamiento observable es correcto. El flujo de control no es legible sin un mapa:

- `gameLoopStart` comprueba la condición terminal, extrae el orden de movimientos e imprime la
  cabecera del turno.
- `attemptStart` intenta un movimiento y salta a `attemptOutside`, `attemptCapture`,
  `attemptVisited` o `attemptLegal`.
- `attemptAdvance` incrementa el índice de intento y salta de vuelta a `attemptStart`.
- `noLegalMove` asigna el ganador y salta de vuelta a `gameLoopStart`.
- Un movimiento exitoso modifica el caballo activo en el sitio y salta de vuelta a `gameLoopStart`.

Hay una entrada y muchos puntos de reentrada: la rama del movimiento legal, la rama de la captura y
la rama de la trampa vuelven al bucle desde puntos distintos. El lector debe reconstruir el estado
global y ambas posiciones de caballo a partir de la etiqueta alcanzada, no de la firma de una
función.

El archivo no tiene comentarios por norma del proyecto, no usa abreviaturas, no usa nombres de una
sola letra y no tiene líneas de más de 100 columnas. Esas reglas hacen legible el código a nivel de
token. No reparan el flujo de control, que es el objetivo de la lección.

## Prueba de cambio

Aplicar **R-SKIP**: al inicio de un turno, si el número de turnos iniciados es un múltiplo positivo
de cinco, el caballo activo no se mueve. El turno se anuncia igualmente con sus campos `turn=` y
`order=`; no sigue ninguna línea `try=` ni `move=`. El turno pasa al otro caballo y el turno saltado
no cuenta para los 200 movimientos exitosos.

Predecir el coste antes de editar. Contar las etiquetas y los saltos que hay que recablear. El turno
se cambia actualmente dentro de `attemptLegal`, mientras que las transiciones terminales ocurren en
`attemptCapture` y `noLegalMove`. Una ruta de salto necesita una transición nueva que concuerde con
las tres. Registrar los archivos tocados, las líneas cambiadas, los conceptos nuevos necesarios y
las trazas rotas, y comparar los números con la variante estructurada de `construction-02`.

## Criterio de aceptación

El estudiante puede explicar por qué este flujo es ilegible **sin** usar la palabra «malo»,
nombrando los saltos concretos que dificultan la reconstrucción del estado.
