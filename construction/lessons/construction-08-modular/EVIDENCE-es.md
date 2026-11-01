# construction-08 — Evidencia
> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro propio de quien aprende. Escribe en español.

Este registro compara una unidad de traducción con cuatro, y mide un cambio en el tamaño activo del
tablero. Conserva las dos columnas. Un empate en el conteo de líneas sigue siendo un resultado si
las unidades recompiladas son distintas.

## 1. Preparación de la ejecución

- Fecha:
- Semilla: `1`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Línea base modular leída el (fecha):
- Línea base con punteros leída el (fecha):
- ¿Tengo el registro de `construction-07` a mano? (sí / no)
- El número que traje de `construction-07` (líneas / trazas / unidades de traducción):

## 2. Inventario de la interfaz (antes de la ejecución)

### 2.1 Cabeceras y declaraciones

Complétalo desde el código, con números de línea, no de memoria.

| Módulo | Cabecera | Líneas de la cabecera | Declaraciones | Definiciones en | Datos ocultos |
|---|---|---|---|---|---|
| | | | | | |

Recuentos:

- Cabeceras:
- Unidades de traducción:
- Declaraciones públicas:
- Definiciones de función, total:
- Auxiliares internos (`game.cpp`):
- Auxiliares internos (`main.cpp`):
- Puntos de entrada del programa:

### 2.2 Grafo de inclusiones

Pega la salida de `g++ -std=c++20 -Icode -MM` para cada `.cpp`, o completa la tabla.

| Objeto | Inclusiones directas |
|---|---|
| `main.o` | |
| `game.o` | |
| `board.o` | |
| `random_sequence.o` | |

- Qué unidad de traducción incluye más cabeceras del proyecto:
- Qué cabeceras del proyecto incluye `game.cpp`:
- Qué cabecera del proyecto incluye `main.cpp`:

### 2.3 Nombres ocultos

Marca `sí` o `no` desde el código, antes de compilar.

| Nombre | ¿Declarado en una cabecera? | Definido en | ¿Se puede usar desde `main.cpp`? |
|---|---|---|---|
| `run_game` | | | |
| `board_column_count` | | | |
| `BoardCell` | | | |
| `boardCells` | | | |
| `Knight` | | | |
| `randomState` | | | |
| `moveOrderMaximum` | | | |
| `parseArguments` | | | |

- La fila donde la frontera se ve más clara, y por qué:
- El nombre que esperaba visible y no lo era:

### 2.4 El único `8`

`game.cpp` contiene un literal `8`. Su línea y su nombre:

- Línea:
- Identificador:
- ¿Es una dimensión del tablero? (sí / no)
- Por qué:

## 3. Predicción (escrita antes de medir)

### 3.1 Las trazas

El cambio: ninguno. Las dos líneas base implementan el mismo juego.

| Pregunta | Mi predicción | Confianza | Razón |
|---|---|---|---|
| Modular vs `seed-0001.txt` | | | |
| Punteros vs `seed-0001.txt` | | | |
| Modular vs punteros, semilla 1 | | | |
| Semillas que coinciden, de 20 | | | |
| Primera línea que podría diferir | | | |

- El resultado que espero fallar, y por qué:

### 3.2 La superficie de cambio

El cambio: `activeBoardRowCount` y `activeBoardColumnCount` en `board.cpp` de 8 a 10.

| Pregunta | Mi predicción |
|---|---|
| Archivos fuente editados | |
| Líneas editadas | |
| Cabeceras editadas | |
| Archivos `.cpp` editados fuera de `board.cpp` | |
| Objetos recompilados, de 4 | |
| ¿Compila sin otras ediciones? | |
| Semillas que coinciden con las trazas de 8×8, de 20 | |
| ¿El juego sigue llegando a un final? | |

- El número que espero fallar, y por qué:

### 3.3 Visibilidad de las cabeceras

Copia tu predicción de §2.3 aquí, o escríbela aquí primero y compárala después.

| Nombre | `main.cpp` | `game.cpp` | `board.cpp` |
|---|---|---|---|
| `run_game` | | | |
| `board_column_count` | | | |
| `BoardCell` | | | |
| `boardCells` | | | |
| `Knight` | | | |
| `randomState` | | | |
| `moveOrderMaximum` | | | |

- La unidad que conoce más nombres, y por qué:
- La unidad que conoce menos, y por qué:

## 4. Ejecución — ambas líneas base

- Comando de compilación modular:
- Comando de compilación con punteros:
- Estados de salida:
- Objetivo del diff: `../../../common/traces/seed-0001.txt`

Registra `vacío` o `diff` en cada celda de resultado.

| Comprobación | Comando | Resultado |
|---|---|---|
| Modular vs oro | | |
| Punteros vs oro | | |
| Modular vs punteros | | |

```text
<salida del diff, o "sin diferencias">
```

Semillas que coincidieron, de 20:

Si algún diff no está vacío, detente aquí y completa §11.

## 5. Mapear cada cabecera a su implementación

### 5.1 Objetos y símbolos externos

- Comando de compilación de objetos:
- Comando de `nm`:
- Nota de plataforma (algún símbolo que el comando no pudo mostrar):

| Cabecera | Declaraciones | Objeto | Símbolos externos | Nombres ocultos |
|---|---|---|---|---|
| `board.h` | 5 | `board.o` | | |
| `game.h` | 1 | `game.o` | | |
| `random_sequence.h` | 3 | `random_sequence.o` | | |

- Qué exporta el objeto del juego, completo:
- Qué exporta el objeto del tablero, completo:
- Qué exporta el objeto aleatorio, completo:
- Dónde quedaron los 17 auxiliares del juego con `-O2`:
- La frase que conservaré sobre una cabecera y un archivo objeto:

### 5.2 Grafo de inclusiones

- Pega la salida de `g++ -MM` para las cuatro unidades:

```text
<salida>
```

- Qué objeto depende de las tres cabeceras del proyecto:
- Qué objeto depende solo de `game.h`:
- Qué cambio de cabecera recompilaría `main.o`:

### 5.3 La comprobación del enlazador

- Copia de trabajo:
- Declaración conservada:
- Definición eliminada:
- Estados de salida de la compilación:
- Estado de salida del enlace:
- Clase de diagnóstico:
- Mensaje:

```text
<salida del enlazador>
```

- Qué comprobó el compilador:
- Qué comprobó el enlazador:
- Qué no comprobó ninguno de los dos:

## 6. Cambiar el tablero a 10×10

- Copia de trabajo:
- Constante cambiada, antes / después:
- Segunda constante cambiada, antes / después:
- Archivos editados:
- Líneas editadas:
- Cabeceras editadas:
- No edité ningún archivo de código versionado: (sí / no)

### 6.1 Compilar y recompilar

- Primera compilación: objetos y marcas de tiempo:
- Después de la edición, objetos recompilados:
- Comando que decidió qué estaba obsoleto:
- Comando de enlace:
- Si la compilación recompiló los cuatro, por qué:
- Qué prueba el registro de compilación sobre `game.cpp` y `main.cpp`:

### 6.2 Comportamiento

- Comando de ejecución:
- Estado de salida:
- Semillas que coincidieron con las trazas de 8×8, de 20:
- La primera línea que difiere:
- Por qué cambian las posiciones iniciales:
- Por qué las tiradas y cada sorteo posterior vienen de un estado distinto:
- Por qué 0 de 20 es esperado y no una regresión:
- ¿El juego llegó a `winner=` y `moves=`? (sí / no)

### 6.3 Archivo por archivo

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

### 6.4 Cambio de cabecera (el contraste)

- Segunda copia de trabajo:
- Cabecera tocada:
- Objetos recompilados:
- Objetos que se quedaron:
- Por qué se quedó `main.o`:
- Por qué no se quedó `game.o`:
- La regla que conservaré sobre cambios de implementación y cambios de interfaz:

## 7. Comparar la superficie de cambio

| Métrica | Línea base con punteros | Línea base modular |
|---|---|---|
| Archivos tocados | | |
| Líneas cambiadas | | |
| Firmas cuyo tipo de parámetro cambia | | |
| Conceptos nuevos necesarios | | |
| Unidades de traducción recompiladas | | |
| Trazas de 8×8 rotas | | |

- Qué números empatan:
- Qué números separan los dos diseños:
- Qué hay dentro de la unidad recompilada en cada caso:
- Qué compró la modularidad:
- Qué costó la modularidad:
- Qué cambio posterior separaría mejor los dos diseños, y por qué:

## 8. Explicar

Una frase, sin las palabras *cabecera*, *archivo*, *módulo*, *compilar*, *mejor*, *peor*, *limpio*,
*limpia*, *bonito*, *elegante*, *feo* ni *modular*:

>

Un párrafo:

- Qué decide una declaración pública:
- Qué oculta el archivo de implementación:
- Qué comprueba el compilador:
- Qué comprueba el enlazador:
- Qué muestra el grafo de compilación:
- Qué sigue exponiendo la interfaz:
- Qué no impone:
- Qué solo prueba una ejecución:
- Qué costó la modularidad:
- Qué lección posterior hace explícito el cambio de representación:
- Qué lecciones posteriores añaden invariantes:
- Qué lección posterior es dueña de la propiedad:
- Qué lección posterior es dueña de puertos y adaptadores:

Las cuatro cosas que mantuve separadas:

## 9. Comprobación de aceptación

- Ejecuté las dos líneas base sin modificar: (sí / no)
- Obtuve tres diffs vacíos y 20 semillas coincidentes: (sí / no)
- Inventarié 9 declaraciones públicas, 28 definiciones y 18 auxiliares internos: (sí / no)
- Mapeé cada cabecera a su objeto con `nm`: (sí / no)
- Mostré una definición ausente compilando y fallando en el enlace: (sí / no)
- Predije las trazas y la superficie de cambio antes de medir: (sí / no)
- Cambié dos constantes en `board.cpp` a 10 y 10: (sí / no)
- `game.cpp` y `main.cpp` no requirieron ediciones: (sí / no)
- Solo se recompiló `board.o`, y después se enlazó: (sí / no)
- La compilación de 10×10 llegó a un ganador o a un empate: (sí / no)
- Obtuve 20 de 20 coincidencias antes y 0 de 20 después: (sí / no)
- Expliqué por qué 0 de 20 es esperado: (sí / no)
- Un cambio de cabecera recompiló a sus inclusores, uno de `.cpp` no: (sí / no)
- Medí el costo de la superficie de cambio en las dos líneas base: (sí / no)
- Expliqué qué ocultan las cabeceras y qué exponen: (sí / no)
- Mi frase evita las cuatro palabras de diseño y las ocho de veredicto: (sí / no)
- No edité ningún archivo de código versionado: (sí / no)

Si edité un archivo de código versionado, qué y por qué:

## 10. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué no puedo explicar todavía:
- Una cosa que volvería a leer:
- El número que llevaré a `construction-09`:
- La comprobación en la que más confío: ¿la traza, los símbolos del objeto o el registro de
  compilación?
- La comprobación que me sorprendió:
- Qué ocultó la cabecera que el archivo único mostraba:
- Qué mantenía junto el archivo único que la línea base modular separa:
- El cambio que haría después para volver a probar la frontera:
- Siguiente paso:

## 11. Si una línea base falta o no es fiel

Complétalo solo si faltó un archivo, una compilación falló por una razón del entorno, o un diff no
estaba vacío.

- Archivo ausente, error de compilación o diff:
- Salida exacta del comando:
- Qué hice en lugar de inventar un conteo:
- Cuándo volveré a intentarlo:

## 12. Autocomprobación

- [ ] Inventarié cada cabecera y cada definición con su archivo.
- [ ] Escribí el grafo de inclusiones desde el compilador, no de memoria.
- [ ] Predije las trazas, la superficie de edición y la de recompilación antes de medir.
- [ ] Compilé ambas líneas base y obtuve tres diffs vacíos, más 20 semillas coincidentes.
- [ ] Compilé cuatro objetos y listé sus símbolos externos.
- [ ] Mostré la división compilación-enlace con una definición ausente.
- [ ] Cambié el tablero activo a 10×10 sin editar ningún otro archivo fuente.
- [ ] Conté un archivo editado, dos líneas, un objeto recompilado y un enlace.
- [ ] Vi 0 de 20 trazas coincidir y expliqué por qué es esperado.
- [ ] Contrasté un cambio de `.cpp` con un cambio de cabecera y sus radios de impacto.
- [ ] Comparé el costo de la superficie de cambio en las dos líneas base.
- [ ] Separé qué permite la interfaz, qué oculta la implementación, qué comprueba la cadena de
      herramientas y qué muestra un registro de compilación o una ejecución.
- [ ] Mi frase de una oración evita las cuatro palabras de diseño y las ocho de veredicto.
- [ ] No edité ningún archivo de código versionado.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §10.
