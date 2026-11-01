# construction-15 — Evidencia

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro del aprendiz. Escribe en español.

Este registro sigue el tablero y el motor desde una clase concreta de 8×8 hasta dos plantillas de
clase, y añade un contrato nombrado para la pieza. Conserva las predicciones. La diferencia entre tu
predicción y la observación es el resultado, sea cero o no.

## 1. Preparación de la ejecución

- Fecha:
- Semilla: `1`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Línea base leída el (fecha):
- ¿Registro de `construction-14` a mano? (sí / no)
- Los números que traje de `construction-14` (líneas de código / archivos / trazas):
- Directorio temporal que usé:

## 2. Las plantillas (antes de ejecutar)

### 2.1 Encabezado e inventario de declaraciones

Rellena esto desde el código, con números de línea, no de memoria.

| Encabezado | Líneas | Declaraciones públicas | Datos privados | Clase completa | Tipo incompleto |
|---|---|---|---|---|---|
| | | | | | |
| | | | | | |
| | | | | | |

Recuentos:

- Encabezados:
- Unidades de traducción:
- Declaraciones públicas:
- Declaraciones de datos privados:
- Definiciones de clase completas:
- Tipos incompletos:
- Definiciones de función, total:
- Ayudas internas (`game_engine.h`):
- Ayuda interna (`main.cpp`):
- Puntos de entrada del programa:

Comparado con `construction-14`:

- Encabezados: ______ → ______
- Unidades de traducción: ______ → ______
- Declaraciones públicas: ______ → ______
- Declaraciones de datos privados: ______ → ______
- Definiciones de clase completas: ______ → ______
- Definiciones de función: ______ → ______
- Líneas de código: ______ → ______
- Archivos: ______ → ______
- Líneas de `board.cpp`: ______ → ______
- Líneas de `board.h`: ______ → ______
- Líneas de `game.cpp`: ______ → ______
- Líneas de `game_engine.h`: ______ → ______

### 2.2 Mapa de líneas

`board.h`:

| Líneas | Contenido |
|---|---|
| | |
| | |
| | |

`game_engine.h`:

| Líneas | Contenido |
|---|---|
| | |
| | |
| | |

Líneas concretas:

- Línea de la cabecera de plantilla de `Board`:
- Línea de `class Board`:
- Línea de `row_count()`:
- Línea de `column_count()`:
- Línea del primer `static_assert` de dimensión:
- Línea del segundo `static_assert` de dimensión:
- Línea del arreglo `visited`:
- Línea de la cabecera de plantilla de `GameEngine`:
- Línea de `class GameEngine`:
- Línea del concepto `PositionedPiece`:
- Línea de `static_assert(PositionedPiece<Knight>)`:
- Línea del miembro `Board<RowCount, ColumnCount>`:
- Líneas de los dos miembros `Knight`:
- Línea de `GameEngine<8, 8>`:
- Línea del constructor de `GameEngine`:
- Línea de `run()`:
- Línea de `playTurn()`:

### 2.3 El contrato y el bucle

| Pregunta | Respuesta |
|---|---|
| Parámetros de plantilla de `Board` | |
| Parámetros de plantilla de `GameEngine` | |
| Clase de parámetros (de tipo o no de tipo) | |
| Operaciones que exige `PositionedPiece` | |
| ¿El concepto restringe un parámetro de plantilla? | |
| ¿Dónde se comprueba el concepto? | |
| ¿Cuántas firmas de miembro nombran `Knight`? | |
| ¿Cuáles? | |
| ¿Qué posee el motor? | |
| ¿Cuál es el almacenamiento del tablero? | |
| ¿Cuál es el área activa? | |
| ¿Qué pasó con `board.cpp`? | |
| ¿Qué pasó con `game.cpp`? | |

- Las dos restricciones de dimensión, en palabras:
- Los cuatro requisitos del concepto, en palabras:
- El tipo de pieza que usa el motor:
- El tipo contra el que se comprueba el concepto:
- La línea donde ocurre la comprobación:
- Qué rechazaría la comprobación:

## 3. Predicción (escrita antes de medir)

### 3.1 La tabla de dimensiones

Semilla 1, nombre `Learner`. Escribe las columnas de 8×8 y 10×10 antes de ejecutar.

| Pregunta | Mi predicción para 8×8 | Mi predicción para 10×10 | Confianza |
|---|---|---|---|
| Posición inicial del jugador | | | |
| Posición inicial de la máquina | | | |
| Tiradas | | | |
| Turnos | | | |
| Intentos | | | |
| Ganador | | | |
| Movimientos | | | |
| Líneas de transcripción | | | |
| `sizeof(Board)` | | | |
| `sizeof(GameEngine)` | | | |
| ¿Compila? | | | |
| ¿Misma salida que 8×8? | | | |

- Las filas que espero equivocar, y por qué:
- Qué espero que cambie cuando crece el tablero:
- Qué espero que siga igual cuando crece el tablero:

### 3.2 La prueba de cambio

| Pregunta | Mi predicción |
|---|---|
| Archivos que añadiré | |
| Archivos que editaré | |
| Líneas del archivo nuevo | |
| Conceptos nuevos que exige el cambio | |
| Semillas que coinciden, de 20, tras el cambio | |
| ¿Compila 10×10? | |
| ¿La salida de 10×10 es idéntica a 8×8? | |
| Archivos rastreados que tocaré | |

- La predicción de la que estoy menos seguro, y por qué:

### 3.3 Instanciación y tamaños

| Pregunta | Mi predicción |
|---|---|
| Instanciaciones de `GameEngine` en el juego de 8×8 | |
| Instanciaciones de `GameEngine` en una unidad con ambos tamaños | |
| Símbolos `nm -C both.o` para `GameEngine<8, 8>`, en `-O0` | |
| El mismo recuento para `GameEngine<10, 10>` | |
| Símbolos de `GameEngine<8, 8>` en `-O2` | |
| `sizeof(Board<8, 8>)` | |
| `sizeof(Board<10, 10>)` | |
| `sizeof(GameEngine<8, 8>)` | |
| `sizeof(GameEngine<10, 10>)` | |
| Llamadas indirectas en `both.o` | |

- Qué espero que la plantilla de clase cambie en el código objeto, en una frase:
- Qué espero que no cambie:

## 4. Ejecución — línea base y segunda instanciación

- Comando de compilación de la línea base:
- Estado de salida de la línea base:
- Diff contra `../../../common/traces/seed-0001.txt`:
- Semillas que coincidieron, de 20, en la línea base sin modificar:
- Ruta del código de 10×10 (solo temporal):
- Comando de compilación de 10×10:
- Estado de salida de 10×10:
- Líneas de la transcripción de 8×8:
- Líneas de la transcripción de 10×10:

Copia la cabecera y el final de 10×10:

```text
<cabecera y final de 10×10>
```

| Observación | Predicho | Observado | Diferencia |
|---|---|---|---|
| Jugador inicial 8×8 | | | |
| Máquina inicial 8×8 | | | |
| Jugador inicial 10×10 | | | |
| Máquina inicial 10×10 | | | |
| Turnos 8×8 | | | |
| Turnos 10×10 | | | |
| Movimientos 8×8 | | | |
| Movimientos 10×10 | | | |
| Líneas de transcripción 8×8 | | | |
| Líneas de transcripción 10×10 | | | |
| `sizeof(Board<8, 8>)` | | | |
| `sizeof(Board<10, 10>)` | | | |

- La línea que me sorprendió:
- Qué hice después en lugar de ajustar la predicción:

## 5. Comparación — qué demuestran las plantillas de clase

- Salida del `grep` de plantillas:
- Salida de `grep -n 'Knight' code/game_engine.h`:
- Número de líneas que mencionan `Knight` en `game_engine.h`:
- Número de firmas de miembro que nombran `Knight`:
- Cuáles:
- Número de menciones de `Knight` en `game.cpp`:
- Qué posee el motor:
- Qué posee el tablero:
- Qué comprueba el concepto:
- Qué no comprueba el concepto:
- Qué demuestran las 20 trazas:
- Qué no demuestran las trazas:
- Qué demuestra la ejecución de 10×10:
- Qué no demuestra la ejecución de 10×10:

### 5.1 Las dos instanciaciones

- Comando que produjo los símbolos:
- Número de símbolos de `GameEngine<8, 8>` en `-O0`:
- Número de símbolos de `GameEngine<10, 10>` en `-O0`:
- Número de símbolos de `GameEngine<8, 8>` en `-O2`:
- Número de símbolos de `GameEngine<10, 10>` en `-O2`:
- Número de símbolos de `Board<8, 8>` en `-O2`:
- Por qué cambian los recuentos:
- Qué significa `W` en la salida de `nm`:
- Si las dos instanciaciones comparten un objeto en ejecución:
- Si las dos instanciaciones comparten código fuente:

### 5.2 Generación de código

- Texto de `game14.o`:
- Texto de `game15.o`:
- Diferencia:
- Texto / datos enlazados de `construction-14`:
- Texto / datos enlazados de `construction-15`:
- Llamadas indirectas en `game14.o`:
- Llamadas indirectas en `game15.o`:
- Símbolos de `GameEngine<8, 8>` en `game15.o` en `-O2`:
- Símbolos de `GameEngine<8, 8>` en `game15.o` en `-O0`:
- Por qué la ausencia de símbolos no es una propiedad de las plantillas de clase:
- Qué unidad debo usar para describir el coste, y por qué:

### 5.3 La sonda del concepto y los modos de fallo

- Resultado de `GoodPawn`:
- Resultado de `MissingSetter`:
- Resultado de `Knight`:
- Tipo sin las operaciones: primera línea de error y mensaje:
- Qué elimina el concepto de la cascada anterior:
- Qué no comprueba el concepto:
- `GameEngine<17, 8>`: primera línea de error y mensaje:
- `GameEngine<0, 8>`: primera línea de error y mensaje:
- Por qué la comprobación de dimensión no es un concepto:

## 6. La prueba de cambio en una copia temporal

- Ruta de la copia temporal:
- Archivos añadidos:
- Archivos editados:
- Líneas del archivo nuevo:
- Conceptos nuevos necesarios:
- Archivos rastreados editados: (sí / no)
- Estado de compilación de 10×10:
- Estado de salida de 10×10:
- ¿La salida de 10×10 difiere de 8×8? (sí / no)
- Trazas de 8×8 tras el cambio, de 20:

### 6.1 Añadir la segunda instanciación

- Encabezado incluido:
- Tipo de motor:
- Semilla y nombre:
- Por qué no se enlaza `game.cpp`:
- Por qué se enlazan `knight.cpp`, `position.cpp` y `random_sequence.cpp`:

### 6.2 Recuentos

| Métrica | 8×8 | 10×10 | Diferencia |
|---|---|---|---|
| Líneas de código temporales | | | |
| Archivos rastreados editados | | | |
| Instanciaciones de `GameEngine` | | | |
| Casillas activas | | | |
| Bytes de almacenamiento | | | |
| Turnos (semilla 1) | | | |
| Intentos (semilla 1) | | | |
| Movimientos (semilla 1) | | | |
| Líneas de transcripción (semilla 1) | | | |
| Trazas coincidentes | | | |

### 6.3 El límite de la aceptación

- Qué afirma la aceptación:
- Qué no afirma:
- Las seis firmas que todavía nombran `Knight`:
- Qué tocaría una parameterización completa de la pieza:
- Qué trabajo posterior se ocupa de ello:
- La frase de aceptación precisa de esta lección:

## 7. Las plantillas y sus límites

| Pregunta | Respuesta |
|---|---|
| ¿Qué es genérico? | |
| ¿Qué está restringido? | |
| ¿Qué cambia con las dimensiones? | |
| ¿Qué no cambia con las dimensiones? | |
| ¿Qué sigue siendo concreto? | |
| ¿Qué compra la plantilla de clase? | |
| ¿Qué cuesta en el código? | |
| ¿Qué cuesta en el código objeto? | |
| ¿Qué compra el concepto? | |
| ¿Qué cuesta el concepto? | |
| ¿Qué no comprueba el concepto? | |
| ¿Qué no cubre el motor? | |
| ¿Puede el motor mover un segundo tipo de pieza sin ediciones? | |

- Qué elimina la plantilla de clase de `game.cpp`:
- Qué mueve a `game_engine.h`:
- Qué no elimina:
- Qué lección posterior se ocupa del ciclo de vida:
- Qué trabajo posterior se ocupa de la parameterización completa de la pieza:

## 8. Explicar

Una frase, sin las palabras *plantilla*, *genérico*, *parámetro de tipo*, *instanciación*,
*compilador*, *mejor*, *peor*, *limpio*, *flexible*, *reutilizable* o *elegante*:

>

Un párrafo:

- Qué fija la plantilla de clase:
- Qué impone las dimensiones:
- Qué impone el contrato de la pieza:
- Qué cambia con las dimensiones:
- Qué no cambia con las dimensiones:
- Qué demuestran las 20 trazas:
- Qué no demuestran las trazas:
- Qué demuestra la ejecución de 10×10:
- Qué no cubre el concepto:
- Qué no cubre el motor:
- Qué cuesta la prueba de cambio:
- Qué lección posterior construye sobre esta:

Las cuatro cosas que mantuve separadas:

## 9. Comprobación de aceptación

- Localicé ambas plantillas de clase y nombré sus parámetros que no son de tipo: (sí / no)
- Nombré las cuatro operaciones del concepto y encontré el `static_assert`: (sí / no)
- Confirmé que `GameEngine` no recibe un tipo de pieza como parámetro: (sí / no)
- Reproduje las 20 trazas en la línea base de 8×8 sin modificar: (sí / no)
- Compilé `GameEngine<10, 10>` desde el mismo encabezado y lo ejecuté: (sí / no)
- Vi que la salida de 10×10 difiere de la de 8×8: (sí / no)
- Medí el mismo `sizeof(Board)` y `sizeof(GameEngine)` para ambos tamaños: (sí / no)
- Vi las dos instanciaciones del motor por nombre en `-O0`: (sí / no)
- Reproduje el fallo de `static_assert` de dimensión para 17×8: (sí / no)
- Reproduje el fallo del concepto para un tipo sin las operaciones: (sí / no)
- Nombré qué no comprueba el concepto: (sí / no)
- Nombré el límite: el tipo de pieza sigue siendo `Knight`: (sí / no)
- No edité ningún archivo de código rastreado: (sí / no)

Si edité un archivo de código rastreado, qué y por qué:

## 10. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué no puedo explicar todavía:
- Una cosa que volvería a leer:
- El número que llevaré a `construction-16`:
- La fila de la tabla de dimensiones que más me sorprendió:
- Qué decide la plantilla de clase que yo esperaba que decidiera el motor:
- Qué sigue decidiendo el motor que yo esperaba que decidiera la plantilla de clase:
- Qué compra la plantilla de clase que la clase concreta de 8×8 no podía ofrecer:
- Qué cede a cambio:
- Qué demostró la prueba de cambio que las 20 trazas no podían:
- El cambio que haría después para volver a probar este diseño:
- Siguiente paso:

## 11. Si falta una línea base o no es fiel

Rellena esto solo si faltaba un archivo, una compilación falló por una razón del entorno, un diff no
estaba vacío, o la segunda instanciación contradijo el comportamiento documentado de una forma que
no pudiste explicar.

- Archivo faltante, error de compilación, diff o contradicción:
- Salida exacta del comando:
- Qué hice en lugar de inventar un recuento:
- Cuándo lo reintentaré:

## 12. Autocomprobación

- [ ] Inventarié cada encabezado y cada definición con su archivo.
- [ ] Localicé ambas plantillas de clase, el concepto y las comprobaciones de dimensión por línea.
- [ ] Comparé la función genérica con las plantillas de clase y nombré qué posee cada una.
- [ ] Predije la tabla de dimensiones, la prueba de cambio y los recuentos de instanciación antes
      de medir.
- [ ] Reproduje las 20 trazas en la línea base de 8×8 sin modificar.
- [ ] Compilé y ejecuté `GameEngine<10, 10>` desde el mismo encabezado.
- [ ] Confirmé que la salida de 10×10 difiere de la de 8×8.
- [ ] Medí que ambos tipos de tablero miden 256 bytes y ambos motores 336 bytes.
- [ ] Vi las dos instanciaciones por nombre en `-O0`.
- [ ] Reproduje los fallos de dimensión y de concepto.
- [ ] Nombré qué no comprueba el concepto.
- [ ] Nombré el límite: el motor es genérico sobre dimensiones, no sobre piezas.
- [ ] Separé qué es genérico, qué está restringido, qué emite el compilador y qué puede ver una
      ejecución.
- [ ] Mi explicación de una frase evita las palabras de mecanismo y las palabras de veredicto.
- [ ] No edité ningún archivo de código rastreado.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §10.
