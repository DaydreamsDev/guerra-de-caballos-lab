# construction-18 — Evidencia

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro del aprendiz. Escribe en español.

Este registro compara un tablero mutable con instantáneas inmutables. Pregunta qué es un valor, qué
se sigue mutando, qué compra el historial y qué cuesta. Conserva las predicciones. La diferencia
entre tu predicción y la observación es el resultado, sea cero o no.

## 1. Preparación de la ejecución

- Fecha:
- Semilla: `1`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Plataforma y arquitectura:
- Línea base leída el (fecha):
- ¿Registro de construction-17 a mano? (sí / no)
- Los números que traje de `construction-17` (líneas de código / encabezados de `for` / tamaño del
  motor / bytes de texto / trazas):
- Directorio temporal que usé:
- ¿Edité algún archivo versionado? (sí / no)

## 2. El inventario del estado (antes de ejecutar)

### 2.1 Funciones y estado

Rellena esto desde el código, con números de línea, no de memoria.

| Función o miembro | Archivo:líneas | ¿Nuevo aquí? | Lee | Escribe |
|---|---|---|---|---|
| `generate_move_candidates` | | | | |
| Almacenamiento de `Board` | | | | |
| `history` | | | | |
| `historyIndex` | | | | |
| `history_size` | | | | |
| `undo_last_move` | | | | |
| `redo_last_move` | | | | |
| `record_history` | | | | |
| `tryMove` | | | | |
| `playTurn` | | | | |

### 2.2 La frontera del historial

| Pregunta | Respuesta |
|---|---|
| Tipo que guarda `history` | |
| `sizeof` de un elemento | |
| Qué le hace `undo_last_move` al índice | |
| Qué asigna | |
| Qué borra `record_history` | |
| Dónde se llama a `record_history` | |
| Qué pasa con la rama de rehacer tras un movimiento nuevo | |
| Qué no guarda el historial | |

### 2.3 Recuentos

- Encabezados:
- Unidades de traducción:
- Definiciones de clase completas:
- Cuerpos de función con nombre escritos a mano:
- Declaraciones públicas:
- Lambdas:
- Encabezados de `for`:
- Puntos de llamada a algoritmos:
- Líneas de código:
- Archivos que difieren de `construction-17`:

Comparado con `construction-17`:

- Encabezados: ______ → ______
- Unidades de traducción: ______ → ______
- Definiciones de clase completas: ______ → ______
- Cuerpos de función con nombre: ______ → ______
- Declaraciones públicas: ______ → ______
- Lambdas: ______ → ______
- Encabezados de `for`: ______ → ______
- Puntos de llamada a algoritmos: ______ → ______
- Líneas de código: ______ → ______
- Líneas de `game_engine.h`: ______ → ______
- Archivos que difieren: ______
- Líneas añadidas y eliminadas en el diff: ______ / ______

### 2.4 Preguntas de contrato

| Pregunta | Respuesta |
|---|---|
| ¿Qué función de esta línea base es pura? | |
| ¿Qué cuatro propiedades lo demuestran? | |
| ¿`tryMove` es pura? ¿Por qué sí o por qué no? | |
| ¿Qué hace de `Board` un tipo de valor? | |
| ¿Por qué una instantánea no puede ser un puntero? | |
| ¿Qué decide deshacer? | |
| ¿Qué decide rehacer? | |
| ¿Qué decide `record_history`? | |
| ¿Qué estado se sigue mutando en el sitio? | |
| ¿Qué tres partes del estado quedan fuera del historial? | |
| ¿Qué dos archivos no cambiaron, y por qué? | |

- La pureza, en mis palabras:
- La inmutabilidad, en mis palabras:
- La diferencia entre ambas, en una frase:
- Lo único que el historial no puede restaurar:

## 3. Predicción (escrita antes de medir)

### 3.1 La tabla de estructura

| Pregunta | Mi predicción |
|---|---|
| Archivos que difieren de `construction-17` | |
| Líneas de código en `construction-17` | |
| Líneas de código aquí | |
| Líneas de `game_engine.h`, antes y después | |
| Líneas añadidas y eliminadas en el diff | |
| Cuerpos de función con nombre, antes y después | |
| Declaraciones públicas, antes y después | |
| Encabezados de `for`, antes y después | |
| Puntos de llamada a algoritmos, antes y después | |
| Lambdas, antes y después | |

- La fila que espero equivocar, y por qué:
- Por qué espero que el recuento de `for` siga en 5:
- Por qué espero que el recuento de algoritmos siga en 3:

### 3.2 La tabla de tamaños y memoria

| Pregunta | Mi predicción |
|---|---|
| `sizeof(Board<8, 8>)` | |
| `sizeof(GameEngine<8, 8>)`, antes y después | |
| `sizeof(std::vector<Board<8, 8>>)` | |
| Asignaciones de montón por partida, antes y después | |
| Bytes asignados, semilla 1, antes y después | |
| `history_size()` después de la semilla 1 | |
| `history_size()` de la semilla más larga de las 20 | |
| Bytes de texto de `game.o`, antes y después | |
| Bytes de texto del binario, antes y después | |
| Bytes de datos del binario, antes y después | |
| Semillas que coinciden, de 20 | |

- Por qué espero que `history_size()` sea `moves + 1`:
- Por qué espero que el tamaño del tablero siga en 256:
- Por qué espero que el número de asignaciones crezca pero siga siendo pequeño:
- El número que espero que me sorprenda:

### 3.3 La prueba de cambio

| Pregunta | Mi predicción |
|---|---|
| Qué añadiré a la copia temporal | |
| Qué archivo editaré | |
| `sizeof(GameEngine<8, 8>)` después del cambio | |
| ¿El tablero cambia al deshacer? | |
| ¿El tablero es idéntico tras deshacer y rehacer? | |
| Semillas que coinciden, de 20, después del cambio | |
| Deshacer exitosos desde el final de la semilla 1 | |
| Rehacer exitosos desde el principio | |

- Por qué un accesor de solo lectura es una forma justa de observar la instantánea:
- Por qué una referencia `const` basta para demostrar que el tablero cambió:
- Qué me diría que rompí la línea base por error:

## 4. Ejecución — la línea base

### 4.1 Trazas

- Comando de compilación:
- Estado de salida:
- Diff contra `../../../common/traces/seed-0001.txt`:
- Semillas que coincidieron, de 20, en la línea base sin modificar:
- Las dos últimas líneas de la transcripción:

### 4.2 Tamaños

- Salida de la sonda de `sizeof`:
- `sizeof(Board<8, 8>)`:
- `sizeof(GameEngine<8, 8>)`:
- `sizeof(Knight)`:
- `sizeof(Position)`:
- `sizeof(MoveCandidate)`:
- `sizeof(std::vector<Board<8, 8>>)`:

### 4.3 La sonda de deshacer y rehacer

- `history_size()` después de la semilla 1:
- Resultado de un `undo_last_move()`:
- Resultado de un `redo_last_move()`:
- Deshacer exitosos hasta `false`:
- Rehacer exitosos hasta `false`:
- Qué significan los dos números de frontera:
- ¿La sonda imprimió el tablero? (sí / no / con un accesor):

### 4.4 Asignaciones

| Semilla | `history_size()` | Asignaciones | Bytes asignados |
|---|---|---|---|
| 1 | | | |
| 5 | | | |
| 7 | | | |
| 11 | | | |
| 20 | | | |

- Las filas de la tabla son una muestra; la salida completa de las 20 semillas está en la sonda:
- La misma sonda contra `construction-17`:
- Instantáneas vivas al final de la semilla 1, y bytes retenidos:
- La regla que explica los bytes:
- Peor caso en `maximumMoveCount`:

### 4.5 Código objeto y coste de compilación

- Texto de `game.o` antes y después:
- Texto del binario enlazado antes y después:
- Datos del binario enlazado antes y después:
- Líneas preprocesadas de `game.cpp` antes y después:
- Tiempo de compilación de `game.cpp` antes y después:
- Número de ejecuciones cronometradas:
- Banderas del compilador usadas:

| Observación | Predicho | Observado | Brecha |
|---|---|---|---|
| Archivos que difieren | | | |
| Líneas de código | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| `history_size()`, semilla 1 | | | |
| Bytes asignados, semilla 1 | | | |
| Texto de `game.o` | | | |
| Texto del binario | | | |
| Líneas preprocesadas | | | |
| Tiempo de compilación | | | |
| Trazas que coinciden | | | |

- El número que me sorprendió:
- Qué hice después en lugar de ajustar la predicción:

## 5. Comparar — qué demuestra el historial

- El comando del diff:
- Archivos que cambiaron:
- Líneas añadidas y eliminadas:
- Qué demuestran los recuentos de `for` y de algoritmos:
- Qué demuestran los tamaños:
- Qué demuestran los números de asignación:
- Qué demuestran las 20 trazas:
- Qué no demuestran las trazas:
- Qué demuestra la sonda:
- Qué no demuestra la sonda:

### 5.1 Lo que no cambió

- La transcripción:
- Las reglas:
- El bucle de turno:
- `generate_move_candidates`:
- `board.h`:
- Los recuentos de recorridos:
- Los tres miembros `unique_ptr`:

### 5.2 Lo que cuesta

- Bytes por movimiento aceptado:
- Instantáneas vivas y bytes, semilla 1:
- Bytes asignados, semilla más pequeña y más grande:
- Tamaño añadido al motor:
- Texto añadido a `game.o`:
- Texto y datos añadidos al binario:
- Líneas preprocesadas añadidas:
- Tiempo de compilación añadido:
- La segunda copia del estado que puede separarse:

### 5.3 La frontera

| Parte del estado | ¿Está en el historial? | Dónde vive |
|---|---|---|
| Casillas visitadas | | |
| Posición del caballo del jugador | | |
| Posición del caballo de la máquina | | |
| Bandera de jugador activo | | |
| Estado de la secuencia aleatoria | | |

- Qué restaura, por lo tanto, `undo_last_move()`:
- Qué tendría que guardar un registro `GameState`:
- El tamaño de ese registro, antes del relleno:
- Por qué esto es un límite y no un defecto:

### 5.4 Deshacer por consulta frente a deshacer por repetición

| Pregunta | Repetición | Instantánea |
|---|---|---|
| Trabajo para deshacer el último movimiento de la semilla 1 | | |
| Trabajo para deshacer el primer movimiento de la semilla 1 | | |
| Código que debe ser correcto | | |
| Memoria retenida, semilla 1 | | |
| Memoria retenida, peor caso | | |
| Visible en la transcripción | | |

- La fila que decidió el diseño por mí:
- La fila que tendría que medir antes de afirmar una ganancia de velocidad:

## 6. La prueba de cambio en una copia temporal

- Ruta de la copia temporal:
- Archivos copiados:
- Archivo editado:
- Función añadida:
- Archivos versionados editados: (sí / no)
- Comando de compilación:
- Estado de compilación con `-Wall -Wextra -Werror`:
- `history=`, `undo=`, `redo=`:
- `undo_changed_board=`:
- `state_restored=`:
- `sizeof(GameEngine<8, 8>)` después del cambio:
- Semillas que coincidieron, de 20, después del cambio:

### 6.1 El accesor

- Su firma:
- Por qué devuelve una referencia `const`:
- Por qué no añade ningún miembro de datos:
- Qué cubre la huella:
- Qué no cubre la huella:

### 6.2 Qué demuestra la prueba de cambio, y qué no

- Demuestra:
- No demuestra sobre los caballos:
- No demuestra sobre la bandera de turno:
- No demuestra sobre la rama de rehacer:
- Por qué la transcripción no puede ver nada de esto:

### 6.3 El modelo de la rama de rehacer (opcional)

- ¿Modelé `record_history` sobre una secuencia? (sí / no)
- La secuencia antes de deshacer:
- El índice tras dos deshaceres:
- La secuencia después de `erase` y `push_back`:
- Qué se descartó:
- Por qué el modelo no es una ejecución del motor:

### 6.4 El experimento de estado completo (opcional)

- ¿Probé un historial de `GameState`? (sí / no)
- Qué tuve que añadir al registro:
- El nuevo `sizeof` por instantánea:
- Semillas que coincidieron:
- Qué costó que el historial de solo tablero no costaba:

### 6.5 Recuentos

| Métrica | construction-17 | construction-18 | Diferencia |
|---|---|---|---|
| Líneas de código | | | |
| Cuerpos de función con nombre | | | |
| Declaraciones públicas | | | |
| Encabezados de `for` | | | |
| Puntos de llamada a algoritmos | | | |
| `sizeof(Board<8, 8>)` | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| Asignaciones por partida | | | |
| Bytes asignados por partida | | | |
| Texto de `game.o` | | | |
| Texto / datos del binario | | | |
| Líneas preprocesadas | | | |
| Tiempo de compilación | | | |
| Trazas que coinciden | | | |

## 7. El modelo de pureza e inmutabilidad y sus límites

| Pregunta | Respuesta |
|---|---|
| ¿Qué hace pura una función? | |
| ¿Qué función de aquí es pura? | |
| ¿Qué promete `const` en una función miembro? | |
| ¿Qué no promete `const` sobre los parámetros? | |
| ¿Qué hace de `Board` un tipo de valor? | |
| ¿Qué comparte una copia de `Board` con su origen? | |
| ¿Por qué un historial de `Board *` no registraría nada? | |
| ¿Qué lee deshacer? | |
| ¿Qué no lee deshacer? | |
| ¿Qué descarta `record_history`? | |
| ¿Qué se sigue mutando en el sitio? | |
| ¿Qué no guarda el historial? | |
| ¿Qué no observa la transcripción? | |
| ¿Qué muestra la medición de memoria? | |

- Qué quita el cambio del código:
- Qué mueve a un contenedor:
- Qué no quita:
- Qué lección posterior se ocupa de la disposición de datos:
- Qué lección posterior se ocupa de concurrencia y estado compartido:

## 8. Explicar

Una frase, sin las palabras *funcional*, *puro*, *inmutable*, *instantánea*, *historial*, *mejor*,
*peor*, *limpio* ni *moderno*:

>

Un párrafo:

- Qué decide `generate_move_candidates`:
- Qué decide `Board`:
- Qué decide `history`:
- Qué decide `undo_last_move`:
- Qué decide `record_history`:
- Qué demuestra el diff:
- Qué demuestran los tamaños:
- Qué demuestran los números de asignación:
- Qué demuestran las trazas:
- Qué demuestra la sonda:
- Qué no cubre la frontera:
- Qué lección posterior se ocupa de la disposición de datos:

Las cuatro cosas que mantuve separadas:

## 9. Verificación de aceptación

- Localicé las cuatro funciones nuevas y las tres declaraciones públicas nuevas por línea: (sí / no)
- Expliqué por qué `board.h` y `move_generator.h` no cambiaron: (sí / no)
- Expliqué por qué una instantánea debe ser un valor y no un puntero: (sí / no)
- Expliqué qué borra `record_history` y por qué: (sí / no)
- Predije las tablas de estructura, tamaños y memoria antes de medir: (sí / no)
- Reproduje las 20 trazas en la línea base sin modificar: (sí / no)
- Ejecuté la sonda y leí `history=22`, `undo=1`, `redo=1`: (sí / no)
- Encontré las fronteras: 21 deshaceres y 21 rehaceres para la semilla 1: (sí / no)
- Medí el coste de asignación en las 20 semillas: (sí / no)
- Añadí un accesor de solo lectura en una copia temporal y tomé la huella del tablero: (sí / no)
- Obtuve `undo_changed_board=1` y `state_restored=1` en la compilación temporal: (sí / no)
- Las 20 trazas coincidieron con el accesor presente: (sí / no)
- `sizeof(GameEngine<8, 8>)` fue 56 antes y después del accesor: (sí / no)
- Nombré las tres partes del estado que el historial no guarda: (sí / no)
- Expliqué que deshacer es una copia de valor, no una repetición: (sí / no)
- No edité ningún archivo de código versionado: (sí / no)

Si edité algún archivo versionado, qué y por qué:

## 10. Reflexión y siguiente paso

- Lo que ahora puedo explicar sin notas:
- Lo que todavía no puedo explicar:
- Una cosa que volvería a leer:
- El número que llevaré a `construction-19`:
- La predicción que más se alejó de la observación:
- Lo que esperaba que el compilador optimizara y no optimizó:
- Lo que esperaba que costara el historial y no costó:
- Lo que la transcripción no pudo decirme:
- Lo que la sonda me dijo y la transcripción no:
- El estado que pondría primero en un registro `GameState`, y por qué:
- La parte del estado que dejaría fuera, y por qué:
- El cambio que haría después para volver a probar este diseño:
- Siguiente paso:

## 11. Si falta una línea base o no es fiel

Rellena esto solo si faltó un archivo, si una compilación falló por una razón del entorno, si un
diff no quedó vacío o si una medición contradijo el comportamiento documentado.

- Archivo que falta, error de compilación, diff o contradicción:
- Salida exacta del comando:
- Qué hice en lugar de inventar un recuento:
- Cuándo volveré a intentarlo:

## 12. Autocomprobación

- [ ] Inventarié las diez funciones y miembros con sus líneas.
- [ ] Conté los cuerpos de función y las declaraciones públicas antes y después.
- [ ] Expliqué por qué `board.h` y `move_generator.h` no cambiaron.
- [ ] Expliqué por qué el historial guarda valores y no punteros.
- [ ] Predije las tablas de estructura, tamaños y memoria antes de medir.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Ejecuté la sonda de deshacer y rehacer y leí ambas fronteras.
- [ ] Medí las asignaciones y los bytes en las 20 semillas.
- [ ] Calculé el peor caso a partir de `maximumMoveCount`.
- [ ] Observé un tablero restaurado desde fuera del motor en una copia temporal.
- [ ] Las 20 trazas coincidieron después del cambio.
- [ ] Nombré las tres partes del estado fuera del historial.
- [ ] Separé «la transcripción no cambia» de «el cambio es gratis».
- [ ] Mi explicación de una frase evita las palabras de mecanismo y de veredicto.
- [ ] No edité ningún archivo de código versionado.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §10.
