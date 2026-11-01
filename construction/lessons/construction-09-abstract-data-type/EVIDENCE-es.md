# construction-09 — Evidencia
> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro de quien aprende. Escribe en español.

Este registro compara un tablero detrás de cinco funciones libres con un tablero detrás de siete
operaciones sobre un manejador opaco, y mide un cambio de representación. Conserva los dos lados. Un
empate en el número de archivos sigue siendo un resultado si difieren las cosas que pasan a ser
posibles.

## 1. Preparación de la ejecución

- Fecha:
- Semilla: `1`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Línea base con TAD leída el (fecha):
- Línea base modular leída el (fecha):
- ¿Registro de `construction-08` a mano? (sí / no)
- El número que traje de `construction-08` (líneas / trazas / unidades de traducción):

## 2. El tipo incompleto (antes de ejecutar)

### 2.1 Cabeceras y declaraciones

Rellena esto desde el código fuente, con números de línea, no de memoria.

| Cabecera | Líneas | Declaraciones | Tipo incompleto | Definición en |
|---|---|---|---|---|
| | | | | |

Conteos:

- Cabeceras:
- Unidades de traducción:
- Declaraciones públicas:
- Tipos incompletos:
- Definiciones de función, total:
- Auxiliares internos (`game.cpp`):
- Auxiliares internos (`main.cpp`):
- Puntos de entrada del programa:

### 2.2 Dónde vive la definición

- Línea de la declaración adelantada en `board.h`:
- Línea de la definición en `board.cpp`:
- Qué hay dentro de `struct Board`:
- El único `#include` que le falta a `game.cpp`, y qué revelaría:

### 2.3 Grafo de inclusión

Pega la salida de `g++ -std=c++20 -Icode -MM` para cada `.cpp`, o rellena la tabla.

| Objeto | Inclusiones directas |
|---|---|
| `main.o` | |
| `game.o` | |
| `board.o` | |
| `random_sequence.o` | |

- Qué unidad de traducción incluye más cabeceras del proyecto:
- Qué cabecera del proyecto incluye `game.cpp` para el tablero:
- Qué cabecera del proyecto incluye `main.cpp`:

### 2.4 Qué no puede expresar la cabecera

Marca `sí` o `no` desde el código fuente, antes de compilar.

| Expresión | ¿Legal en `game.cpp`? | Por qué |
|---|---|---|
| `Board *board;` | | |
| `Board board;` | | |
| `sizeof(Board)` | | |
| `board->visited` | | |
| `delete board;` | | |
| `Board *copy = board;` | | |

- La operación que me sorprendió, y por qué:
- El nombre que esperaba ver y no vi:

## 3. Predicción (escrita antes de medir)

### 3.1 Las trazas

El cambio: ninguno. Las dos líneas base implementan el mismo juego.

| Pregunta | Mi predicción | Confianza | Razón |
|---|---|---|---|
| TAD vs `seed-0001.txt` | | | |
| Modular vs `seed-0001.txt` | | | |
| TAD vs modular, semilla 1 | | | |
| Semillas que coinciden, de 20, en cada línea base | | | |
| Primera línea que podría diferir | | | |

- El resultado que espero fallar, y por qué:

### 3.2 La superficie del cambio

El cambio: reemplazar el arreglo fijo de `board.cpp` por un vector plano y una función de índice.

| Pregunta | Mi predicción |
|---|---|
| Archivos fuente editados | |
| Líneas editadas | |
| Cabeceras editadas | |
| Archivos `.cpp` editados fuera de `board.cpp` | |
| Objetos recompilados, de 4 | |
| ¿Compila sin otras ediciones? | |
| Semillas que coinciden, de 20 | |
| ¿El archivo objeto crece, encoge o queda igual? | |

- El número que espero fallar, y por qué:

### 3.3 Visibilidad de la cabecera

Copia tu predicción de §2.4 aquí, o escríbela aquí primero y compárala después.

| Expresión | `main.cpp` | `game.cpp` | `board.cpp` |
|---|---|---|---|
| `Board *board = board_create();` | | | |
| `sizeof(Board)` | | | |
| `board->visited` | | | |
| `Board board;` | | | |
| `delete board;` | | | |
| `board_was_visited(board, 1, 1)` | | | |
| `Board *copy = board;` | | | |

- La unidad que conoce más nombres, y por qué:
- La fila que es `no` por una razón distinta a las demás, y por qué:

## 4. Ejecución — ambas líneas base

- Comando de compilación de la línea base con TAD:
- Comando de compilación de la línea base modular:
- Estados de salida:
- Objetivo de la diferencia: `../../../common/traces/seed-0001.txt`

Registra `vacía` o `diferencia` en cada celda de resultado.

| Comprobación | Comando | Resultado |
|---|---|---|
| TAD vs referencia | | |
| Modular vs referencia | | |
| TAD vs modular | | |

```text
<salida de diff, o "sin diferencias">
```

Semillas que coincidieron, de 20, en la línea base con TAD:

Semillas que coincidieron, de 20, en la línea base modular:

Si alguna diferencia no está vacía, detente aquí y rellena §11.

## 5. Demostrar que el tipo es incompleto

### 5.1 La sonda

- Ruta del código de la sonda (solo en el directorio de trabajo):
- Comando de compilación:
- Estado de salida de la compilación:
- Número de diagnósticos:

| Operación prohibida | Clase de diagnóstico | Nota del compilador sobre la declaración |
|---|---|---|
| `sizeof(Board)` | | |
| `board->visited` | | |
| `Board board;` | | |
| `delete board;` | | |

- Qué se negó a calcular el compilador, en una frase:
- Por qué la negativa es la abstracción y no una limitación:

### 5.2 Símbolos del objeto

- Comando de compilación del objeto:
- Comando `nm`:
- Nota de plataforma (algún símbolo que el comando no pudo mostrar):

| Cabecera | Declaraciones | Objeto | Símbolos externos | ¿Diseño en el nombre del símbolo? |
|---|---|---|---|---|
| `board.h` | 7 | `board.o` | | |
| `game.h` | 1 | `game.o` | | |
| `random_sequence.h` | 3 | `random_sequence.o` | | |

- Los siete símbolos del tablero, completos:
- Qué codifica `P5Board`:
- Qué codifica `K`:
- Qué codificaba `v` en los símbolos de `construction-08`:

### 5.3 Nombres de símbolo contra diseño

- ¿Los nombres decorados cambian cuando cambia la representación? (sí / no)
- Por qué eso importa para el enlazador:
- La frase que conservaré sobre un nombre y un tamaño:

## 6. Cambiar la representación

- Copia de trabajo (plana):
- Copia de trabajo (anidada):
- Archivos editados:
- Líneas editadas, plana:
- Líneas editadas, anidada:
- Cabeceras editadas:
- No edité ningún archivo de código versionado: (sí / no)

### 6.1 Vector plano con función de índice

- Definición del struct:
- Función de índice:
- Qué reemplazó `assign`:
- Comando de compilación:
- Estado de salida:
- Semillas que coincidieron, de 20:

### 6.2 Vector de vectores

- Definición del struct:
- Qué cambió en `board_reset`:
- Comando de compilación:
- Estado de salida:
- Semillas que coincidieron, de 20:

### 6.3 Comportamiento y coste

- La primera línea que difiere, si hay alguna:
- Por qué no debería diferir ninguna línea:
- Tamaño de texto, arreglo:
- Tamaño de texto, vector plano:
- Tamaño de texto, vector de vectores:
- ¿Los símbolos exportados son idénticos en los tres? (sí / no)
- Qué cuesta la abstracción en tiempo de ejecución:
- Qué no le oculta la abstracción al programa:

### 6.4 Archivo por archivo

| Archivo | ¿Editado? | ¿Recompilado? |
|---|---|---|
| `code/board.cpp` | | |
| `code/board.h` | | |
| `code/game.cpp` | | |
| `code/game.h` | | |
| `code/main.cpp` | | |
| `code/random_sequence.cpp` | | |
| `code/random_sequence.h` | | |

- El conteo de archivos fuente editados:
- El conteo de objetos recompilados:
- El conteo de archivos que no necesitaron edición:
- Por qué `game.cpp` no necesitó edición:

### 6.5 La inicialización que las trazas no pueden comprobar

- Qué quité de `board_create`:
- Semillas que siguieron coincidiendo, de 20:
- El orden de llamadas en `run_game` que lo esconde:
- Qué hacía distinto la versión con arreglo:
- Por qué esto es comportamiento indefinido y no una traza fallida:
- Qué lección posterior cierra este hueco:

## 7. Comparar la superficie del cambio

| Métrica | Línea base modular | Línea base con TAD |
|---|---|---|
| Archivos tocados | | |
| Líneas cambiadas | | |
| Firmas cambiadas | | |
| Conceptos nuevos necesarios | | |
| Unidades de traducción recompiladas | | |
| Trazas de 8×8 rotas | | |
| Tableros vivos a la vez | | |
| Ciclo de vida del tablero | | |
| Operaciones que reciben un tablero | | |
| Una prueba puede crear un tablero limpio | | |

- Qué filas empatan:
- Qué filas separan los dos diseños:
- Qué ya podía hacer la línea base modular, y qué no:
- Qué compró el TAD:
- Qué costó el TAD:
- El conteo de líneas de la migración misma (`diff ../construction-08-modular/code/game.cpp`):
- Qué cambio posterior separaría los dos diseños con más claridad, y por qué:

## 8. Explicar

Una frase, sin usar las palabras *cabecera*, *archivo*, *módulo*, *compilar*, *puntero*, *struct*,
*clase*, *opaco*, *encapsulación*, *mejor*, *peor*, *limpio*, *más limpio*, *bonito*, *elegante*,
*feo* ni *modular*:

>

Un párrafo:

- Qué fija la declaración:
- Qué oculta:
- Qué impone el compilador:
- Qué ve el enlazador:
- Qué prueba una ejecución:
- Qué no prueba una ejecución:
- Qué añade la multiplicidad:
- Qué cuesta:
- Qué lección posterior añade invariantes:
- Qué lección posterior añade encapsulación:
- Qué lección posterior posee el ciclo de vida:
- Qué lección posterior posee puertos y adaptadores:

Las cuatro cosas que mantuve separadas:

## 9. Comprobación de aceptación

- Ejecuté ambas líneas base sin modificar: (sí / no)
- Obtuve tres diferencias vacías y 20 semillas coincidentes en cada una: (sí / no)
- Inventarié 11 declaraciones públicas y encontré el único tipo incompleto: (sí / no)
- Compilé una sonda que falla en `sizeof`, acceso a miembros y `delete`: (sí / no)
- Listé los siete símbolos del tablero y leí sus nombres decorados: (sí / no)
- Expliqué por qué ningún nombre de símbolo codifica el diseño: (sí / no)
- Predije las trazas y la superficie del cambio antes de medir: (sí / no)
- Reemplacé el arreglo por un vector plano: (sí / no)
- Lo reemplacé otra vez por un vector de vectores: (sí / no)
- `board.h`, `game.cpp` y `main.cpp` no necesitaron edición: (sí / no)
- Solo se recompiló `board.o`, y después el enlace: (sí / no)
- Obtuve 20 de 20 antes del cambio y 20 de 20 después: (sí / no)
- Registré el tamaño del objeto para las tres representaciones: (sí / no)
- Quité la inicialización de `board_create` y expliqué el resultado: (sí / no)
- Medí la superficie del cambio en ambas líneas base: (sí / no)
- Nombré las filas que empatan y las que separan: (sí / no)
- Expliqué qué permite la declaración y qué oculta: (sí / no)
- Mi frase evita las seis palabras de sintaxis y las ocho de veredicto: (sí / no)
- No edité ningún archivo de código versionado: (sí / no)

Si edité un archivo de código versionado, cuál y por qué:

## 10. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué no puedo explicar todavía:
- Una cosa que volvería a leer:
- El número que llevaré a `construction-10`:
- La comprobación en la que más confío: la traza, los símbolos del objeto o los diagnósticos de la
  sonda:
- La comprobación que me sorprendió:
- Qué ocultó la cabecera que `construction-08` todavía mostraba:
- Qué mantuvo junto la línea base modular que el TAD separa:
- Qué probó el cambio, y qué no:
- El cambio que haría después para volver a probar la frontera:
- Siguiente paso:

## 11. Si falta una línea base o no es fiel

Rellena esto solo si faltó un archivo, una compilación falló por una razón del entorno, o una
diferencia no estaba vacía.

- Archivo ausente, error de compilación o diferencia:
- Salida exacta del comando:
- Qué hice en lugar de inventar un conteo:
- Cuándo volveré a intentarlo:

## 12. Autocomprobación

- [ ] Inventarié cada cabecera y cada definición con su archivo.
- [ ] Localicé la declaración adelantada y la definición de `Board` en archivos distintos.
- [ ] Escribí el grafo de inclusión desde el compilador, no de memoria.
- [ ] Predije las trazas, la superficie del cambio y la tabla de visibilidad antes de medir.
- [ ] Compilé y ejecuté ambas líneas base y obtuve tres diferencias vacías y 20 semillas
      coincidentes en cada una.
- [ ] Compilé una sonda que falla en las cuatro operaciones prohibidas.
- [ ] Listé los siete símbolos exportados y leí sus nombres decorados.
- [ ] Cambié la representación dos veces, solo dentro de `board.cpp`.
- [ ] Registré un archivo editado, un objeto recompilado y un reenlace.
- [ ] Vi 20 de 20 trazas coincidir después de los dos cambios.
- [ ] Registré el tamaño del objeto para el arreglo y para los dos vectores.
- [ ] Quité la inicialización de `board_create` y expliqué por qué las trazas siguen pasando.
- [ ] Comparé la superficie del cambio y nombré las filas que empatan.
- [ ] Separé qué permite la declaración, qué oculta la definición, qué comprueba la cadena de
      herramientas y qué muestra una ejecución o una medición de tamaño.
- [ ] Mi frase de una sola oración evita las seis palabras de sintaxis y las ocho de veredicto.
- [ ] No edité ningún archivo de código versionado.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §10.
