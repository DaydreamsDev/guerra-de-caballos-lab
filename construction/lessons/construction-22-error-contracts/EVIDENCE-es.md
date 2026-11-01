# construction-22 — Evidencia

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro propiedad de quien aprende. Escribe en español.

Este registro sigue una respuesta que llegaba en tres piezas y ahora llega en una. Pregunta qué
devuelve el motor, qué decide quien llama, qué registra la transcripción y qué hace el proceso.
Conserva las predicciones. La distancia entre tu predicción y la observación es el resultado, sea
cero o no.

## 1. Configuración de la ejecución

- Fecha:
- Rango de semillas: `1` a `20`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Plataforma y arquitectura:
- Línea base leída el (fecha):
- ¿Registro de `construction-21` a mano? (sí / no)
- Los números que traje de `construction-21` (líneas de código / archivos / texto de `game.o` /
  texto del binario / datos / BSS / trazas):
- Directorio temporal que usé:
- ¿Edité algún archivo versionado? (sí / no)

## 2. Inventario del contrato (antes de ejecutar)

### 2.1 Las declaraciones

Rellena esto desde el código, con números de línea, no de memoria.

| Pregunta | Respuesta |
|---|---|
| ¿Qué archivo declara `MoveOutcome`? | |
| ¿Qué archivo declara `MoveResult`? | |
| ¿Cuántos valores tiene `MoveOutcome`? | |
| Sus nombres, en orden | |
| Sus valores numéricos | |
| ¿Cuántos miembros tiene `MoveResult`? | |
| Sus nombres y tipos | |
| Qué devolvía `tryMove` en `construction-21` | |
| Qué devuelve `tryMove` aquí | |
| Qué método expone el contrato en público | |
| Qué función es el único consumidor de producción | |

### 2.2 Las ramas

| Resultado | Qué llama `playTurn` | ¿Continúa o acepta? | ¿Termina la partida? |
|---|---|---|---|
| `outsideBoard` | | | |
| `visited` | | | |
| `capture` | | | |
| `legal` | | | |

- Por qué `outsideBoard` se comprueba primero:
- Por qué `capture` se comprueba antes que `visited`:
- Qué pasa en la traza si se intercambian esos dos:

### 2.3 Recuentos

- Archivos:
- Cabeceras:
- Unidades de traducción en la compilación de consola:
- Unidades de traducción en la compilación de la sonda:
- Definiciones de clase completas en el núcleo:
- Variables globales mutables en el núcleo:
- Valores en `MoveOutcome`:
- Miembros en `MoveResult`:
- Líneas de código:
- Archivos idénticos byte a byte a `construction-21`:
- Archivos cambiados:
- Archivos nuevos:
- Llamadas a `exit` en `code/`:

Comparado con `construction-21`:

- Archivos: ______ → ______
- Cabeceras: ______ → ______
- Unidades de traducción (compilación de consola): ______ → ______
- Líneas de código: ______ → ______
- Archivos cambiados: ______
- Archivos nuevos: ______
- `sizeof(MoveOutcome)`: ______
- `sizeof(MoveResult)`: ______
- `sizeof(GameEngine<8, 8>)`: ______ → ______
- Texto de `game.o`: ______ → ______
- Texto del binario: ______ → ______
- Datos del binario: ______ → ______
- BSS del binario: ______ → ______
- Texto de `error_probe.o`: ______

## 3. Predicción (escrita antes de medir)

### 3.1 La tabla de estructura

| Pregunta | Mi predicción |
|---|---|
| Archivos en `construction-21` | |
| Archivos aquí | |
| Cabeceras aquí | |
| Unidades de traducción en la compilación de consola | |
| Líneas de código en `construction-21` | |
| Líneas de código aquí | |
| Archivos cambiados | |
| Archivos nuevos | |
| Archivos idénticos byte a byte | |
| Valores en `MoveOutcome` | |
| Miembros en `MoveResult` | |

- La fila que espero fallar, y por qué:
- Por qué el motor parece distinto pero solo cambiaron su firma y un punto de llamada:
- Qué archivo lleva el enumerado y la estructura:

### 3.2 La tabla de la sonda

| Pregunta | Mi predicción |
|---|---|
| Línea de salida de `error_probe` | |
| Estado de salida de `error_probe` | |
| Qué resultado falta en la sonda | |
| Por qué falta | |
| Estado de salida de `main` sin `--name` | |
| Estado de salida de `main` con una partida válida | |
| Trazas que coinciden, de 20 | |

- Los tres números del enumerado que espero:
- Por qué los números no son el contrato:
- Qué imprimiría la sonda si el motor devolviera solo un enumerado:

### 3.3 La tabla de tamaños

| Pregunta | Mi predicción |
|---|---|
| `sizeof(MoveOutcome)` | |
| `sizeof(MoveResult)` | |
| `sizeof(GameEngine<8, 8>)`, antes y después | |
| Bytes de texto de `game.o`, antes y después | |
| Bytes de texto del binario, antes y después | |
| Bytes de datos del binario, antes y después | |
| Bytes de BSS del binario, antes y después | |
| Bytes de texto de `error_probe.o` | |

- Por qué espero `MoveResult` en 12 bytes:
- Por qué espero que el motor siga en 40:
- Por qué espero datos y BSS planos:
- El número que espero que me sorprenda:

### 3.4 La tabla de decisión

| Pregunta | Mi predicción |
|---|---|
| ¿Quién decide continuar tras un movimiento inválido? | |
| ¿Quién decide informarlo? | |
| ¿Quién decide detenerse? | |
| Qué sabe el núcleo de la transcripción | |
| Qué sabe el núcleo del proceso | |
| Qué hace `return 2` desde `main` | |

- Por qué `return` desde `main` no es `exit`:
- Qué puede hacer quien llama que `exit` impide:
- La recuperación que espero escribir en la prueba de cambio:

## 4. Ejecución — la línea base

### 4.1 Trazas

- Comando de compilación:
- Estado de salida:
- Diff contra `../../../common/traces/seed-0001.txt`:
- Trazas que coincidieron, de 20, en la línea base sin modificar:

### 4.2 La sonda de errores

- Comando de compilación:
- Estado de salida:
- Línea de salida:
- Resultado `outside`, nombre y número:
- Resultado `capture`, nombre y número:
- Resultado `legal`, nombre y número:
- Qué resultado no apareció, y por qué:

### 4.3 El camino de argumentos

- Comando sin `--name`:
- Mensaje:
- Estado de salida:
- Comando con una partida válida:
- Estado de salida:
- Qué línea de `main.cpp` produce cada estado:

### 4.4 Tamaños

- Salida de la sonda de `sizeof`:
- `sizeof(MoveOutcome)`:
- `sizeof(MoveResult)`:
- `sizeof(GameEngine<8, 8>)`:

### 4.5 Objeto y binario

- Texto de `game.o` antes y después:
- Texto del binario enlazado antes y después:
- Datos del binario enlazado antes y después:
- BSS del binario enlazado antes y después:
- Texto de `error_probe.o`:
- Qué números se movieron y cuáles no:

### 4.6 Opcional: el cuarto resultado

- ¿Escribí la sonda? (sí / no)
- Línea de salida:
- Semilla que usé:
- Recuentos de cada resultado:
- Por qué un motor nuevo no puede producir `visited`:
- Qué prueba esta sonda sobre `evaluate_move` como consulta:

### 4.7 Predicción y observación

| Observación | Predicho | Observado | Distancia |
|---|---|---|---|
| Archivos | | | |
| Cabeceras | | | |
| Líneas de código | | | |
| `sizeof(MoveOutcome)` | | | |
| `sizeof(MoveResult)` | | | |
| Texto de `game.o` | | | |
| Texto del binario | | | |
| Datos del binario | | | |
| BSS del binario | | | |
| Texto de `error_probe.o` | | | |
| Salida de la sonda | | | |
| Trazas que coinciden | | | |

- El número que me sorprendió:
- Qué hice después en lugar de ajustar la predicción:

## 5. Comparar — qué lleva el valor

- Comando para la comprobación de declaraciones:
- Qué devuelve `tryMove` aquí:
- Qué devolvía en `construction-21`:
- Quién inicializa el destino ahora:
- ¿Puede olvidarlo quien llama? (sí / no)
- ¿Puede ignorar el resultado? (sí / no)
- Cuántas rutas de retorno tiene `tryMove`:
- Qué añade `evaluate_move`:
- Qué prueban los tamaños:
- Qué prueban las 20 trazas:
- Qué no prueban las trazas:
- Qué prueba la sonda:
- Qué no prueba la sonda:

### 5.1 El diff exacto

- Comando:
- Archivos cambiados:
- Archivos nuevos:
- Líneas añadidas / eliminadas / saldo:
- `game_engine.h`: añadidas / eliminadas / saldo:
- Líneas de `move_result.h`:
- Líneas de `error_probe.cpp`:
- Archivos idénticos byte a byte:
- Qué salió de `game_engine.h`:
- Qué quedó igual dentro de `playTurn`:

### 5.2 Lo que no cambió

- Las reglas:
- La transcripción:
- El tablero, el generador, el caballo, la posición:
- El puerto y los adaptadores:
- La interfaz pública del juego:
- El tamaño del motor:
- El número de trazas:

### 5.3 Lo que cuesta

- Archivos y líneas nuevas:
- Texto añadido al binario:
- Datos añadidos al binario:
- BSS añadido al binario:
- Cabeceras añadidas al grafo de inclusión:
- El segundo punto de entrada público:
- La carga de nombres:
- El riesgo de ignorar el valor:

### 5.4 La tabla del contrato

| Pregunta | `construction-21` | `construction-22` |
|---|---|---|
| Qué devuelve `tryMove` | | |
| Quién inicializa el destino | | |
| Puede olvidarlo quien llama | | |
| Puede ignorar el resultado | | |
| Número de rutas de retorno | | |
| Quién decide continuar | | |
| Quién decide detener el proceso | | |
| Cómo ve el contrato una sonda | | |
| Tamaño de la respuesta | | |

- La fila que es un intercambio real, y por qué:
- La fila que no cambia, y por qué:

## 6. La prueba de cambio — que decida quien llama

- Ruta de la copia temporal:
- Archivos copiados:
- Archivos creados:
- Archivos versionados editados: (sí / no)
- Modos de política que ejecuté:
- Comando de compilación:
- Estado de la compilación con `-Wall -Wextra -Werror`:
- Salida en modo informar:
- Salida en modo detener:
- Estado de salida en modo informar:
- Estado de salida en modo detener:
- Valores de `Recovery` que usé:
- Trazas que coincidieron, de 20, después del cambio:

### 6.1 La política

- La función que traduce resultado a recuperación:
- Por qué no tiene etiqueta `default`:
- Qué hace el compilador si se añade un quinto resultado:
- Cómo el parámetro de quien llama cambia la recuperación:
- Qué línea detiene el programa:
- Por qué esa línea no es `exit`:

### 6.2 Qué prueba la prueba de cambio, y qué no

- Prueba sobre el movimiento inválido:
- Prueba sobre quien llama:
- Prueba sobre el núcleo:
- Prueba sobre detenerse:
- Prueba sobre la transcripción:
- No prueba sobre `visited` desde un motor nuevo:
- No prueba sobre valores ignorados:
- No prueba sobre la estabilidad del enumerado:

### 6.3 Recuentos

| Métrica | `construction-21` | `construction-22` | Diferencia |
|---|---:|---:|---:|
| Líneas de código | | | |
| Archivos | | | |
| Cabeceras | | | |
| Valores en `MoveOutcome` | | | |
| Miembros en `MoveResult` | | | |
| `sizeof(MoveResult)` | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| Texto de `game.o` | | | |
| Texto del binario | | | |
| Datos del binario | | | |
| BSS del binario | | | |
| Trazas que coinciden | | | |
| Llamadas a `exit` en `code/` | | | |

## 7. El modelo del contrato y sus límites

| Pregunta | Respuesta |
|---|---|
| Qué devuelve el motor | |
| Qué decide quien llama | |
| Qué registra la transcripción | |
| Qué hace el proceso | |
| Por qué un movimiento inválido no es excepcional | |
| Por qué el enumerado tiene cuatro valores y no un booleano | |
| Por qué importa el orden de las comprobaciones | |
| Qué lee `evaluate_move` | |
| Por qué un motor nuevo no puede devolver `visited` | |
| Qué puede ignorar todavía quien llama | |
| Qué añadiría `[[nodiscard]]` | |
| Por qué `std::expected` no está en esta línea base | |
| Qué significa `return 2` desde `main` | |
| Qué lección posterior es dueña del port | |

- Los cinco acoplamientos que quedan:
- Qué acoplamiento es sobre la consulta:
- Qué acoplamiento es sobre el valor ignorado:
- Qué acoplamiento es sobre la política:
- Qué acoplamiento es sobre los números:
- Qué acoplamiento es sobre el proceso:

## 8. Explicar

Una frase, sin las palabras *struct*, *enumerado*, *valor de retorno*, *parámetro de salida*,
*excepción*, *expected*, *mejor*, *peor*, *limpio* ni *desacoplado*:

>

Un párrafo:

- Qué cambió:
- Qué se quedó:
- Qué añade la sonda:
- Qué omite la sonda:
- Qué decide quien llama:
- Qué hace el proceso:
- Qué prueban los números:
- Qué prueban las trazas:
- Qué lección posterior es dueña del port:

Las cuatro cosas que mantuve separadas:

## 9. Comprobación de aceptación

- Leí `move_result.h`, `game_engine.h`, `error_probe.cpp` y `main.cpp`: (sí / no)
- Nombré los cuatro resultados y su orden dentro de `tryMove`: (sí / no)
- Expliqué por qué `capture` se comprueba antes que `visited`: (sí / no)
- Conté 20 archivos, 11 cabeceras y 1.071 líneas de código: (sí / no)
- Nombré el archivo cambiado y los dos archivos nuevos: (sí / no)
- Predije las tablas de estructura, sonda, tamaño y decisión antes de medir: (sí / no)
- Reproduje las 20 trazas en la línea base de consola sin modificar: (sí / no)
- Ejecuté `error_probe` y vi `outside=0 capture=2 legal=3` con estado 0: (sí / no)
- Ejecuté `main` con un argumento incorrecto y vi estado 2: (sí / no)
- Medí `sizeof(MoveOutcome)` en 4 y `sizeof(MoveResult)` en 12: (sí / no)
- Medí el texto de `game.o` en 4.312 y el binario en 10.901 de texto, 876 de datos y 16 de BSS:
  (sí / no)
- Medí `error_probe.o` en 2.633 bytes de texto: (sí / no)
- Vi el cuarto resultado con un tablero con historia, o anoté por qué no: (sí / no)
- Escribí una política de quien llama en una copia temporal y la ejecuté en dos modos: (sí / no)
- Ambos modos terminaron con estado 0 y sin `exit`: (sí / no)
- Las 20 trazas coincidieron después de la prueba de cambio: (sí / no)
- La compilación temporal pasó con `-Wall -Wextra -Werror`: (sí / no)
- Nombré qué no garantiza el valor: (sí / no)
- No edité ningún archivo de código versionado: (sí / no)

Si edité algún archivo de código versionado, qué y por qué:

## 10. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué todavía no puedo explicar:
- Una cosa que volvería a leer:
- El número que llevaré a `construction-23`:
- La predicción más lejana de la observación:
- Qué esperaba que el compilador optimizara y no lo hizo:
- Qué esperaba que mostrara la sonda y no mostró:
- Qué me dijeron las trazas que la sonda no podía:
- Qué me dijo la sonda que las trazas no podían:
- Qué recuperación elegiría para un programa más grande, y por qué:
- Qué recuperación no elegiría, y por qué:
- El cambio que haría después para probar este diseño otra vez:
- Siguiente paso:

## 11. Si falta una línea base o no es fiel

Rellena esto solo si faltaba un archivo, una compilación falló por una razón del entorno, un diff no
estaba vacío, una sonda no pudo producir un resultado, o una medición contradijo el comportamiento
documentado.

- Archivo ausente, error de compilación, diff o contradicción:
- Salida exacta del comando:
- Qué hice en lugar de inventar un recuento:
- Cuándo volveré a intentarlo:

## 12. Autocomprobación

- [ ] Inventarié el contrato: el enumerado, la estructura, el punto de entrada público y el único
      consumidor de producción.
- [ ] Nombré los cuatro resultados, sus valores y su orden.
- [ ] Expliqué por qué `capture` va antes que `visited`.
- [ ] Predije las cuatro tablas antes de medir.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Ejecuté la sonda y el camino de argumentos y registré ambos estados.
- [ ] Medí `sizeof(MoveOutcome)`, `sizeof(MoveResult)` y el motor.
- [ ] Medí `game.o`, el texto del binario, los datos, el BSS y el objeto de la sonda.
- [ ] Mostré el cuarto resultado, o nombré por qué no pude.
- [ ] Escribí una política de quien llama y la ejecuté en dos modos.
- [ ] Ninguna ruta de `code/` llama a `exit`.
- [ ] Las 20 trazas coincidieron después de la prueba de cambio.
- [ ] Separé qué devuelve el motor de qué decide quien llama.
- [ ] Mi explicación de una frase evita las palabras de mecanismo y las de veredicto.
- [ ] No edité ningún archivo de código versionado.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §10.
