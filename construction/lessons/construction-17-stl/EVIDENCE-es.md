# construction-17 — Evidencia

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro del aprendiz. Escribe en español.

Este registro compara recorridos escritos a mano con algoritmos con nombre. Pregunta qué promete
cada recorrido, qué enuncia el punto de llamada después del cambio, qué paga el compilador y qué
puede ver una ejecución. Conserva las predicciones. La diferencia entre tu predicción y la
observación es el resultado, sea cero o no.

## 1. Preparación de la ejecución

- Fecha:
- Semilla: `1`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Plataforma y arquitectura:
- Línea base leída el (fecha):
- ¿Registro de `construction-16` a mano? (sí / no)
- Los números que traje de `construction-16` (líneas / encabezados de `for` / bytes de texto /
  trazas):
- Directorio temporal que usé:
- ¿Edité algún archivo versionado? (sí / no)

## 2. Los recorridos (antes de ejecutar)

### 2.1 Inventario

Completa esto desde el código, con números de línea, no de memoria.

| Recorrido | Archivo:líneas | Antes | Después | Contrato |
|---|---|---|---|---|
| Reinicio del tablero | | `for` anidado | | |
| Generación de movimientos | | `for` con índice | | |
| Orden de movimientos | | `for` con índice | | |
| Bucle de intentos | | `for` con `continue` | | |
| Encabezado del turno | | `for` con separador | | |
| Validez del tablero | | `for` anidado con salida temprana | | |
| Análisis de argumentos | | `for` sobre `argv` | | |

### 2.2 Llamadas a algoritmos

| Llamada | Archivo:línea | Algoritmo | Rango de origen | Destino |
|---|---|---|---|---|
| Reinicio del tablero | | | | |
| Generación de movimientos | | | | |
| Orden de movimientos | | | | |

- Encabezado que aporta `fill`:
- Encabezado que aporta `transform`:
- Encabezado que aporta `views::iota`:
- ¿`views::iota` almacena los índices? (sí / no)
- ¿Reserva memoria? (sí / no)

### 2.3 Recuentos

- Encabezados:
- Unidades de traducción:
- Definiciones de clase completas:
- Cuerpos de función con nombre escritos a mano:
- Lambdas:
- Encabezados de `for`:
- Puntos de llamada a algoritmos:
- Líneas de código:
- Archivos que difieren de `construction-16`:

Comparado con `construction-16`:

- Encabezados: ______ → ______
- Unidades de traducción: ______ → ______
- Definiciones de clase completas: ______ → ______
- Cuerpos de función con nombre: ______ → ______
- Lambdas: ______ → ______
- Encabezados de `for`: ______ → ______
- Puntos de llamada a algoritmos: ______ → ______
- Líneas de código: ______ → ______
- Archivos: ______ → ______
- Líneas de `board.h`: ______ → ______
- Líneas de `move_generator.h`: ______ → ______
- Líneas de `game_engine.h`: ______ → ______
- Archivos que difieren: ______

### 2.4 Preguntas de contrato

| Pregunta | Respuesta |
|---|---|
| ¿Qué promete `fill`? | |
| ¿Qué promete `transform`? | |
| ¿Qué promete `views::iota`? | |
| ¿Qué encabezados de bucle desaparecieron? | |
| ¿Qué encabezados de bucle quedan? | |
| ¿Qué bucle restante podría pasar a `any_of`? | |
| ¿Qué bucle restante no puede pasar a un algoritmo sin esconder efectos? | |
| ¿Qué imprime el bucle de intentos antes de decidir? | |
| ¿Cuántos resultados maneja el bucle de intentos? | |
| ¿Cuántas formas tiene el bucle de intentos de parar? | |
| ¿Qué limpia ahora el reinicio que el bucle anterior no limpiaba? | |
| ¿Qué archivo creció más, y cuántas líneas? | |

- Los tres recorridos que cambiaron, en palabras:
- Los cuatro encabezados de `for` que desaparecieron, en palabras:
- El único recorrido que no cambiaría, y por qué:
- Qué cambió `std::array` en el tablero, y qué no:

## 3. Predicción (escrita antes de medir)

### 3.1 La tabla de estructura

| Pregunta | Mi predicción |
|---|---|
| Encabezados de `for` en `construction-16` | |
| Encabezados de `for` aquí | |
| Puntos de llamada a algoritmos aquí | |
| Líneas de código en `construction-16` | |
| Líneas de código aquí | |
| Archivos que difieren | |
| Qué archivo se reduce | |
| Qué archivo crece más | |
| Lambdas añadidas | |

- La fila que espero equivocar, y por qué:
- Por qué cuatro encabezados de bucle se convierten en tres llamadas a algoritmos:
- Qué espero que imprima `grep 'for ('` después de la prueba de cambio:

### 3.2 La tabla de tamaños y texto

| Pregunta | Mi predicción |
|---|---|
| `sizeof(Board<8, 8>)` | |
| `sizeof(GameEngine<8, 8>)` | |
| `sizeof(std::array<MoveCandidate, 8>)` | |
| Bytes de texto de `game.o`, antes y después | |
| Bytes de texto del binario, antes y después | |
| Bytes de datos del binario, antes y después | |
| Reservas de montón por motor, antes y después | |
| Líneas preprocesadas de `game.cpp`, antes y después | |
| Tiempo de compilación de `game.cpp`, antes y después | |
| Semillas que coinciden, de 20, después del cambio | |

- Por qué espero que la cuenta de reservas no cambie:
- Por qué espero que el tamaño del tablero no cambie:
- El número que espero que me sorprenda:
- Qué espero del tiempo de compilación, y por qué:

### 3.3 La prueba de cambio

| Pregunta | Mi predicción |
|---|---|
| El recorrido que voy a reemplazar | |
| Su rango de líneas | |
| El algoritmo que voy a usar | |
| Encabezados de `for` después del cambio | |
| ¿Cambia la transcripción? | |
| Semillas que coinciden, de 20, después del cambio | |
| El bucle de intentos después del cambio | |

- Por qué `printTurnHeader` es un objetivo justo:
- Por qué `Board::is_valid` sería un objetivo más débil:
- Qué me diría que elegí el contraejemplo por error:

## 4. Ejecución — la línea base

### 4.1 Trazas

- Comando de compilación:
- Estado de salida:
- Diff contra `../../../common/traces/seed-0001.txt`:
- Semillas que coincidieron, de 20, en la línea base sin modificar:

### 4.2 Tamaños

- Salida de la sonda de `sizeof`:
- `sizeof(Board<8, 8>)`:
- `sizeof(GameEngine<8, 8>)`:
- `sizeof(Knight)`:
- `sizeof(Position)`:
- `sizeof(MoveCandidate)`:
- `sizeof(std::array<MoveCandidate, 8>)`:

### 4.3 Objeto y binario

- Texto de `game.o` antes y después:
- Texto del binario enlazado antes y después:
- Datos del binario enlazado antes y después:
- BSS del binario enlazado antes y después:
- Llamadas indirectas en `game.o` antes y después:

### 4.4 Coste de compilación

- Líneas preprocesadas de `game.cpp` antes y después:
- Tiempo de compilación de `game.cpp` antes y después:
- Número de ejecuciones de medición:
- Banderas del compilador usadas:
- Qué incluyen los números, y qué no:

| Observación | Predicho | Observado | Diferencia |
|---|---|---|---|
| Encabezados de `for` | | | |
| Puntos de llamada a algoritmos | | | |
| Líneas de código | | | |
| `sizeof(Board<8, 8>)` | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| Texto de `game.o` | | | |
| Texto del binario | | | |
| Líneas preprocesadas | | | |
| Tiempo de compilación | | | |
| Trazas que coinciden | | | |

- El número que me sorprendió:
- Qué hice después en lugar de ajustar la predicción:

## 5. Comparar — qué demuestran los algoritmos

- Comando `grep` de los recorridos:
- Encabezados de `for` antes y después:
- Comando `grep` de las llamadas a algoritmos:
- Puntos de llamada a algoritmos, y qué algoritmos son:
- Bytes de texto de `game.o` antes y después:
- Bytes de texto del binario enlazado antes y después:
- Qué demuestran los tamaños:
- Qué demuestran las 20 trazas:
- Qué no demuestran las trazas:
- Qué demuestran los números de compilación:
- Qué no demuestran los números de compilación:

### 5.1 El diff exacto

- Comando:
- Archivos que cambiaron:
- Líneas añadidas / eliminadas / netas:
- `board.h`: añadidas / eliminadas / netas:
- `move_generator.h`: añadidas / eliminadas / netas:
- `game_engine.h`: añadidas / eliminadas / netas:
- Archivos idénticos byte a byte:

### 5.2 Lo que no cambió

- La transcripción:
- Las reglas:
- Las interfaces:
- El modelo de propiedad:
- Los tamaños:
- La cuenta de reservas:
- Las llamadas indirectas:
- Qué compró `std::array` sin cambiar el tamaño:

### 5.3 Lo que cuesta

- Líneas de `include` añadidas:
- Líneas preprocesadas añadidas:
- Tiempo de compilación añadido:
- Texto añadido en `game.o`:
- Texto añadido en el binario:
- Escrituras por reinicio, antes y después:
- Una cosa más que quien lee debe saber:

### 5.4 La semántica del reinicio

- Tipo de almacenamiento en `construction-16`:
- Tipo de almacenamiento aquí:
- Celdas que limpiaba el bucle anterior:
- Celdas que limpia `fill`:
- Por qué la transcripción no puede ver la diferencia:
- Por qué la diferencia merece registrarse igual:
- Qué puede ensanchar un algoritmo sin cambiar su nombre:

## 6. La prueba de cambio en una copia temporal

- Ruta de la copia temporal:
- Archivos copiados:
- Archivo editado:
- Líneas reemplazadas:
- Algoritmo nuevo:
- ¿Archivos versionados editados? (sí / no)
- Comando de compilación:
- Estado de compilación con `-Wall -Wextra -Werror`:
- Semillas que coincidieron, de 20, después del cambio:
- Encabezados de `for` antes y después:
- ¿Se cambió el bucle de intentos? (sí / no)

### 6.1 El reemplazo

- El encabezado de bucle que desapareció:
- El rango de origen:
- El destino:
- La operación:
- Qué sigue calculando la lambda:
- Por qué la regla del separador necesita el índice:

### 6.2 Qué demuestra la prueba de cambio, y qué no

- Demuestra:
- No demuestra sobre velocidad:
- No demuestra sobre claridad:
- Por qué no autoriza cambiar el bucle de intentos:

### 6.3 El experimento del bucle de intentos (opcional)

- ¿Intenté un algoritmo en el bucle de intentos? (sí / no)
- Qué algoritmo:
- Dónde tuvo que imprimir el predicado:
- Dónde tuvo que mutar el predicado:
- Qué información de resultado se perdió:
- Qué aceptó el compilador:
- Por qué el resultado es peor que el bucle:

### 6.4 Recuentos

| Métrica | construction-16 | construction-17 | Diferencia |
|---|---|---|---|
| Líneas de código | | | |
| Encabezados de `for` | | | |
| Puntos de llamada a algoritmos | | | |
| Lambdas | | | |
| Archivos que difieren | | | |
| `sizeof(Board<8, 8>)` | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| Texto de `game.o` | | | |
| Texto / datos del binario | | | |
| Líneas preprocesadas | | | |
| Tiempo de compilación | | | |
| Trazas que coinciden | | | |

## 7. El modelo de recorrido y sus límites

| Pregunta | Respuesta |
|---|---|
| ¿Qué enuncia un encabezado de `for`? | |
| ¿Qué enuncia una llamada a algoritmo? | |
| ¿Qué esconde `fill`? | |
| ¿Qué esconde `transform`? | |
| ¿Qué evita almacenar `views::iota`? | |
| ¿Qué no elimina la STL? | |
| ¿Cuándo es el algoritmo la forma honesta? | |
| ¿Cuándo es el bucle la forma honesta? | |
| ¿Qué recorrido tiene una salida temprana pura? | |
| ¿Qué recorrido tiene efectos alrededor de su salida temprana? | |
| ¿Qué bucle es análisis y no recorrido? | |
| ¿Qué no puede observar la transcripción? | |
| ¿Qué paga el compilador por la abstracción? | |
| ¿Qué gana el código a cambio de ese precio? | |

- Qué elimina el cambio del código:
- Qué muda a un encabezado:
- Qué muda a una lambda:
- Qué no elimina:
- Qué lección posterior se ocupa de la pureza y la inmutabilidad:
- Qué lección posterior se ocupa de la disposición de datos:

## 8. Explicar

Una frase, sin las palabras *STL*, *rangos*, *algoritmo*, *bucle*, *iterador*, *mejor*, *peor*,
*limpio* ni *moderno*:

>

Un párrafo:

- Qué decide `fill`:
- Qué decide `transform`:
- Qué decide `views::iota`:
- Qué demuestra la cuenta de `for`:
- Qué deciden los bucles restantes:
- Qué demuestra el diff:
- Qué demuestran los tamaños:
- Qué demuestra el código objeto:
- Qué demuestran los números de compilación:
- Qué demuestra la transcripción:
- Qué demuestra el bucle de intentos:
- Qué lección posterior se ocupa de la pureza y la inmutabilidad:

Las cuatro cosas que mantuve separadas:

## 9. Verificación de aceptación

- Localicé los tres puntos de llamada a algoritmos y sus líneas: (sí / no)
- Localicé los cuatro encabezados de `for` que desaparecieron y los cinco que quedan: (sí / no)
- Expliqué por qué `Board::reset` ahora limpia 256 celdas en lugar de 64: (sí / no)
- Expliqué por qué `views::iota` no reserva: (sí / no)
- Predije las tablas de estructura, tamaños y texto antes de medir: (sí / no)
- Reproduje las 20 trazas en la línea base sin modificar: (sí / no)
- Reemplacé el bucle de `printTurnHeader` en una copia temporal por `for_each` sobre `iota`:
  (sí / no)
- La compilación temporal pasó con `-Wall -Wextra -Werror`: (sí / no)
- Las 20 trazas coincidieron después del cambio: (sí / no)
- La cuenta de `for` en la copia temporal bajó de 5 a 4: (sí / no)
- Medí el tablero en 256 bytes y el motor en 24: (sí / no)
- Medí el texto de `game.o` en 4.656 y el del binario en 9.668: (sí / no)
- Medí la entrada preprocesada y el tiempo de compilación: (sí / no)
- Expliqué por qué se queda el bucle de intentos: (sí / no)
- Nombré `Board::is_valid` como un recorrido que podría ser `any_of`: (sí / no)
- No edité ningún archivo de código versionado: (sí / no)

Si edité un archivo versionado, qué y por qué:

## 10. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué no puedo explicar todavía:
- Una cosa que volvería a leer:
- El número que llevaré a `construction-18`:
- La predicción que más se alejó de la observación:
- Qué esperaba que el compilador optimizara y no lo hizo:
- Qué esperaba que costara el algoritmo y no costó:
- Qué no me pudo decir la transcripción:
- Qué me dijeron los números de compilación que la transcripción no:
- El recorrido que cambiaría después, y por qué:
- El recorrido que dejaría tranquilo, y por qué:
- El cambio que haría después para volver a probar este diseño:
- Siguiente paso:

## 11. Si falta una línea base o no es fiel

Completa esto solo si faltó un archivo, una compilación falló por una razón del entorno, un diff no
quedó vacío o una medición contradijo el comportamiento documentado.

- Archivo faltante, error de compilación, diff o contradicción:
- Salida exacta del comando:
- Qué hice en lugar de inventar un recuento:
- Cuándo lo reintentaré:

## 12. Autocomprobación

- [ ] Inventarié los siete recorridos con sus líneas.
- [ ] Conté los encabezados de `for` antes y después: 9 y 5.
- [ ] Conté los puntos de llamada a algoritmos y nombré los algoritmos.
- [ ] Expliqué el ensanchamiento del reinicio de 64 celdas a 256.
- [ ] Predije las tablas de estructura, tamaños y texto antes de medir.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Reemplacé un recorrido visible en la transcripción en una copia temporal.
- [ ] Las 20 trazas coincidieron después del cambio.
- [ ] Medí el tablero, el motor, `game.o` y el binario.
- [ ] Medí la entrada preprocesada y el tiempo de compilación.
- [ ] Separé qué promete el recorrido de qué paga el compilador.
- [ ] Nombré el bucle de intentos como contraejemplo y di cuatro razones.
- [ ] Nombré `Board::is_valid` como un resto que podría ser `any_of`.
- [ ] Separé «la transcripción no cambia» de «el cambio es gratis».
- [ ] Mi frase de explicación evita las palabras del mecanismo y del veredicto.
- [ ] No edité ningún archivo de código versionado.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §10.
