# construction-20 — Evidencia

> [Inglés](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro del aprendiz. Escribe en español.

Este registro compara un núcleo que imprime con un núcleo que anuncia eventos. Pregunta quién
decide qué pasó, quién decide cómo se ve, qué cuesta la costura y qué puede ver una ejecución.
Conserva las predicciones. La diferencia entre tu predicción y la observación es el resultado, sea
cero o no.

## 1. Preparación de la ejecución

- Fecha:
- Semilla: `1`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Plataforma y arquitectura:
- Línea base leída el (fecha):
- ¿Registro de `construction-17` a mano? (sí / no)
- Los números que traje de `construction-17` (líneas / archivos / texto de `game.o` / texto del
  binario / trazas):
- Directorio temporal que usé:
- ¿Edité algún archivo versionado? (sí / no)

## 2. La frontera (antes de ejecutar)

### 2.1 Inventario del puerto

Completa esto desde el código, con números de línea, no de memoria.

| Método del puerto | Línea de declaración | Punto de llamada | Representación en consola | Campo de registro |
|---|---|---|---|---|
| `game_header` | | | | |
| `turn_header` | | | | |
| `illegal_attempt` | | | | |
| `capture_attempt` | | | | |
| `legal_attempt` | | | | |
| `move` | | | | |
| `no_legal_move` | | | | |
| `winner` | | | | |

### 2.2 Tabla de adaptadores

| Pregunta | Adaptador de consola | Adaptador de registro |
|---|---|---|
| Archivo de encabezado | | |
| Unidad de traducción | | |
| Métodos implementados | | |
| Líneas `printf`/`fprintf` | | |
| Estado que guarda | | |
| Salida que produce | | |
| ¿Se enlaza en el binario de consola? | | |
| ¿Se enlaza en la sonda? | | |

### 2.3 Recuentos

- Archivos:
- Encabezados:
- Unidades de traducción en la compilación base:
- Definiciones de clase completas:
- Métodos virtuales puros:
- Sobrescrituras:
- Líneas `printf`/`fprintf`:
- Inclusiones de `<cstdio>`:
- Puntos de llamada `port.*`:
- Líneas de código:
- Archivos idénticos byte a byte a `construction-17`:
- Archivos cambiados:
- Archivos nuevos:

Comparado con `construction-17`:

- Archivos: ______ → ______
- Encabezados: ______ → ______
- Unidades de traducción: ______ → ______
- Clases completas: ______ → ______
- Métodos virtuales puros: ______ → ______
- Sobrescrituras: ______ → ______
- Métodos de impresión en el núcleo: ______ → ______
- Líneas `printf`/`fprintf`: ______ → ______
- Inclusiones de `<cstdio>`: ______ → ______
- Líneas de código: ______ → ______
- Líneas de `game_engine.h`: ______ → ______
- Líneas de `game.cpp`: ______ → ______

### 2.4 Preguntas de contrato

| Pregunta | Respuesta |
|---|---|
| ¿Qué archivos incluyen `<cstdio>`? | |
| ¿Cuál de ellos es el núcleo? | |
| ¿Qué dos programas incluyen `game_engine.h`? | |
| ¿Qué adaptador usa cada programa? | |
| ¿Por qué hay más puntos de llamada que métodos? | |
| ¿Qué seis métodos de impresión salieron de `GameEngine`? | |
| ¿A dónde fueron sus cuerpos? | |
| ¿Qué archivo es la raíz de composición? | |
| ¿Qué conecta la raíz de composición? | |
| ¿Qué diez archivos son idénticos byte a byte? | |
| ¿Qué evento renderiza la consola y el adaptador de registro solo cuenta? | |
| ¿Qué parámetros son valores y no texto? | |
| ¿Qué dos objetos deben declararse en un orden específico? | |
| ¿Por qué importa ese orden? | |

- La dependencia del núcleo, en una frase:
- La dependencia del adaptador de consola, en una frase:
- La dependencia del adaptador de registro, en una frase:
- Qué posee el puerto:
- Qué posee el adaptador:
- Qué sigue decidiendo el núcleo:

## 3. Predicción (escrita antes de medir)

### 3.1 La tabla de estructura

| Pregunta | Mi predicción |
|---|---|
| Archivos en `construction-17` | |
| Archivos aquí | |
| Encabezados aquí | |
| Unidades de traducción en la compilación base | |
| Líneas de código en `construction-17` | |
| Líneas de código aquí | |
| Líneas de `game_engine.h`, antes y después | |
| Métodos de impresión en el núcleo, antes y después | |
| Inclusiones de `<cstdio>`, antes y después | |
| Archivos idénticos byte a byte | |
| Archivos nuevos | |
| Archivos cambiados | |

- La fila que espero equivocar, y por qué:
- Por qué el neto de líneas no es el tamaño de los archivos nuevos:
- Qué archivo espero que se reduzca, y cuánto:

### 3.2 La tabla de adaptadores

| Pregunta | Mi predicción |
|---|---|
| Métodos en el puerto | |
| Métodos del adaptador de consola | |
| Métodos del adaptador de registro | |
| Líneas `printf` del adaptador de consola | |
| Líneas de salida del adaptador de registro | |
| `sizeof` del adaptador de consola | |
| `sizeof` del adaptador de registro | |
| Qué guarda el adaptador de registro sobre el ganador | |
| Qué adaptador se enlaza en el binario de consola | |
| Qué adaptador se enlaza en la sonda | |

- Por qué espero que `ConsoleTranscript` sea un puntero:
- Por qué espero que `RecordingTranscript` sea mayor:
- Qué adaptador espero que ignore el otro binario:

### 3.3 La tabla de tamaños y texto

| Pregunta | Mi predicción |
|---|---|
| `sizeof(GameEngine<8, 8>)` | |
| `sizeof(Board<8, 8>)` | |
| Texto de `game.o`, antes y después | |
| Texto del objeto del adaptador de consola | |
| Texto del binario, antes y después | |
| Datos del binario, antes y después | |
| BSS del binario, antes y después | |
| Llamadas indirectas en `game.o`, antes y después | |
| Semillas que coinciden, de 20 | |

- Por qué espero que el motor crezca 8 bytes:
- Por qué espero que el tablero siga en 256:
- Por qué espero que el binario crezca aunque `game.o` se reduzca:
- El número que espero que me sorprenda:

### 3.4 La prueba de cambio

| Pregunta | Mi predicción |
|---|---|
| El evento que voy a añadir | |
| Archivos que voy a editar | |
| Líneas que voy a añadir al núcleo | |
| Líneas que voy a añadir al bucle | |
| ¿Cambia la transcripción de consola? | |
| Semillas que coinciden, de 20, después del cambio | |
| Nuevo conteo en la sonda | |
| Resultado de compilación con `-Wall -Wextra -Werror` | |

- Por qué `game_started` es un objetivo justo:
- Qué me diría que elegí un evento que vive dentro del bucle:
- Qué debe hacer el adaptador de consola para mantener la transcripción sin cambios:

## 4. Ejecución — la línea base

### 4.1 Trazas

- Comando de compilación:
- Estado de salida:
- Diff contra `../../../common/traces/seed-0001.txt`:
- Semillas que coincidieron, de 20, en la línea base sin modificar:

### 4.2 La sonda de adaptadores

- Comando de compilación:
- Estado de salida:
- `headers`:
- `turns`:
- `illegal`:
- `capture`:
- `legal`:
- `moves`:
- `no_legal`:
- `winners`:
- `winner`:
- `final_moves`:
- ¿Cierra la aritmética? (`illegal + legal = intentos`; `moves + no_legal = turnos`):

### 4.3 Tamaños

- Salida de la sonda de `sizeof`:
- `sizeof(Board<8, 8>)`:
- `sizeof(GameEngine<8, 8>)`:
- `sizeof(Knight)`:
- `sizeof(Position)`:
- `sizeof(MoveCandidate)`:
- `sizeof(TranscriptPort)`:
- `sizeof(ConsoleTranscript)`:
- `sizeof(RecordingTranscript)`:

### 4.4 Objeto y binario

- Texto de `game.o` antes y después:
- Texto del objeto del adaptador de consola:
- Texto del binario enlazado antes y después:
- Datos del binario enlazado antes y después:
- BSS del binario enlazado antes y después:
- Llamadas indirectas en `game.o` antes y después:

### 4.5 Predicción y observación

| Observación | Predicho | Observado | Diferencia |
|---|---|---|---|
| Archivos | | | |
| Líneas de código | | | |
| Líneas de `game_engine.h` | | | |
| Líneas `printf`/`fprintf` | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| `sizeof(Board<8, 8>)` | | | |
| Texto de `game.o` | | | |
| Texto del objeto del adaptador de consola | | | |
| Texto del binario | | | |
| Datos del binario | | | |
| Llamadas indirectas | | | |
| Trazas coincidentes | | | |

- El número que me sorprendió:
- Qué hice después en vez de ajustar la predicción:

## 5. Comparar — qué demuestra la frontera

- Comando para comprobar el encabezado de salida:
- Archivos que incluyen `<cstdio>`:
- Archivos que contienen `printf`/`fprintf`:
- Comando para comprobar el puerto:
- Métodos del puerto:
- Sobrescrituras por adaptador:
- Puntos de llamada `port.*`:
- Bytes de texto de `game.o` antes y después:
- Bytes de texto del objeto del adaptador de consola:
- Bytes de texto del binario enlazado antes y después:
- Bytes de datos del binario enlazado antes y después:
- Llamadas indirectas antes y después:
- Qué prueban los tamaños:
- Qué prueban las 20 trazas:
- Qué no prueban las trazas:
- Qué prueba la sonda:
- Qué no prueba la sonda:

### 5.1 El diff exacto

- Comando:
- Archivos cambiados:
- Archivos nuevos:
- Líneas añadidas / eliminadas / netas:
- `game_engine.h`: añadidas / eliminadas / netas:
- `game.cpp`: añadidas / eliminadas / netas:
- Líneas de `transcript_port.h`:
- Líneas de `console_transcript.h`:
- Líneas de `console_transcript.cpp`:
- Líneas de `recording_transcript.h`:
- Líneas de `adapter_probe.cpp`:
- Archivos idénticos byte a byte:

### 5.2 Lo que no cambió

- La transcripción:
- Las reglas:
- El diseño del tablero:
- El generador de movimientos:
- El bucle de intentos:
- La interfaz pública del juego:
- El conteo de trazas:
- Qué no sacó el puerto del núcleo:

### 5.3 Lo que cuesta

- Archivos y líneas nuevos:
- Llamadas indirectas añadidas:
- Bytes añadidos por motor:
- Texto añadido al binario:
- Datos añadidos al binario:
- La regla de ciclo de vida:
- La regla de cambio:
- Una cosa más que debe saber quien lee:

### 5.4 La tabla de eventos

| Evento | Punto de llamada | Representación en consola | Campo de registro |
|---|---|---|---|
| `game_header` | | | |
| `turn_header` | | | |
| `illegal_attempt` | | | |
| `capture_attempt` | | | |
| `legal_attempt` | | | |
| `move` | | | |
| `no_legal_move` | | | |
| `winner` | | | |

- Qué evento renderiza la consola y solo cuenta el adaptador de registro:
- Qué evento cuentan ambos:
- Qué evento lleva el nombre del ganador:
- Qué evento lleva el arreglo de orden de movimientos:
- Qué evento se emite fuera del bucle de turno:

## 6. La prueba de cambio en una copia temporal

- Ruta de la copia temporal:
- Archivos copiados:
- Archivos editados:
- ¿Archivos versionados editados? (sí / no)
- Evento añadido:
- Línea añadida al núcleo:
- Línea añadida al bucle:
- Implementación en el adaptador de consola:
- Implementación en el adaptador de registro:
- Comando de compilación:
- Estado de compilación con `-Wall -Wextra -Werror`:
- Semillas que coincidieron, de 20, después del cambio:
- Nuevo conteo de la sonda:
- ¿Cambió `playTurn`? (sí / no)

### 6.1 El parche

- Declaración en el puerto:
- Punto de emisión en el núcleo:
- Declaración en consola:
- Cuerpo en consola:
- Contador en registro:
- Cuerpo en registro:
- Cambio de formato en la sonda:
- Por qué se necesita `(void)seed;`:

### 6.2 Qué demuestra la prueba de cambio, y qué no

- Demuestra:
- Demuestra sobre el adaptador de consola:
- Demuestra sobre el adaptador de registro:
- Demuestra sobre el bucle:
- No demuestra sobre eventos dentro del bucle:
- No demuestra sobre el valor del evento:
- Qué cambiaría la transcripción, y por qué eso es otra prueba:

### 6.3 Recuentos

| Métrica | construction-17 | construction-20 | Diferencia |
|---|---|---|---|
| Líneas de código | | | |
| Archivos | | | |
| Métodos del puerto | | | |
| Sobrescrituras | | | |
| Puntos de llamada `port.*` | | | |
| Llamadas indirectas en `game.o` | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| Texto de `game.o` | | | |
| Texto del objeto del adaptador de consola | | | |
| Texto / datos del binario | | | |
| Trazas coincidentes | | | |

## 7. El modelo del puerto y sus límites

| Pregunta | Respuesta |
|---|---|
| ¿Qué decide el núcleo? | |
| ¿Qué decide el adaptador? | |
| ¿Qué es un puerto? | |
| ¿Qué es un adaptador? | |
| ¿Por qué los métodos se llaman como eventos y no como comandos de salida? | |
| ¿Por qué los parámetros son valores y no texto? | |
| ¿Dónde está la costura? | |
| ¿Qué exigiría al adaptador una costura más fina? | |
| ¿Qué dejaría en el núcleo una costura más gruesa? | |
| ¿De qué sigue dependiendo el núcleo? | |
| ¿De qué sigue dependiendo el puerto? | |
| ¿Qué parámetros son prestados, y por cuánto tiempo? | |
| ¿Por qué el adaptador debe vivir más que el motor? | |
| ¿Qué hace cumplir el compilador cuando el puerto crece? | |
| ¿Qué no observa la transcripción? | |
| ¿Qué observa la sonda que la transcripción no? | |
| ¿Qué paga el binario por la costura? | |

- Los tres acoplamientos que quedan:
- Qué acoplamiento es una dependencia de encabezado:
- Qué acoplamiento es una regla de ciclo de vida:
- Qué acoplamiento es una decisión de diseño:
- Qué lección posterior posee la concurrencia:

## 8. Explicar

Una frase, sin usar las palabras *puerto*, *adaptador*, *interfaz*, *virtual*, *hexagonal*,
*dependencia*, *mejor*, *peor*, *limpio* ni *desacoplado*:

>

Un párrafo:

- Qué decide el núcleo:
- Qué decide el adaptador:
- Qué es el puerto:
- Qué hace el adaptador de consola:
- Qué hace el adaptador de registro:
- Qué hace `game.cpp`:
- Qué hace `adapter_probe.cpp`:
- Qué prueba el diff:
- Qué prueban los tamaños:
- Qué prueban las llamadas indirectas:
- Qué prueba la prueba de cambio:
- Qué prueba el bucle:
- Qué lección posterior posee la concurrencia:

Las cuatro cosas que mantuve separadas:

## 9. Verificación de aceptación

- Localicé los ocho métodos del puerto y las dieciséis sobrescrituras: (sí / no)
- Localicé los diez puntos de llamada `port.*` y expliqué por qué hay más de ocho: (sí / no)
- Mostré que `game_engine.h` no incluye `<cstdio>` ni contiene `printf`: (sí / no)
- Nombré `game.cpp` como raíz de composición y `adapter_probe.cpp` como segundo programa:
  (sí / no)
- Predije las tablas de estructura, adaptadores y tamaños antes de medir: (sí / no)
- Reproduje las 20 trazas en la línea base sin modificar: (sí / no)
- Ejecuté la sonda de adaptadores y leí los conteos: (sí / no)
- Medí `sizeof(GameEngine<8, 8>)` en 32 y `sizeof(Board<8, 8>)` en 256: (sí / no)
- Medí el texto de `game.o` en 4.138 y el objeto del adaptador de consola en 1.341: (sí / no)
- Medí el binario en 10.748 bytes de texto y 876 de datos: (sí / no)
- Conté 8 llamadas indirectas en `game.o`, frente a 0 en `construction-17`: (sí / no)
- Añadí `game_started` en una copia temporal y lo implementé en ambos adaptadores: (sí / no)
- La compilación temporal pasó con `-Wall -Wextra -Werror`: (sí / no)
- Las 20 trazas coincidieron después del cambio: (sí / no)
- La sonda imprimió `started=1` y todos los demás conteos sin cambios: (sí / no)
- `playTurn` era idéntico byte a byte en la copia temporal: (sí / no)
- Expliqué por qué un adaptador puede ignorar un evento: (sí / no)
- Nombré los tres acoplamientos que quedan: (sí / no)
- No edité ningún archivo de código versionado: (sí / no)

Si edité algún archivo de código versionado, qué y por qué:

## 10. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué no puedo explicar todavía:
- Una cosa que volvería a leer:
- El número que llevaré a `construction-21`:
- La predicción que estuvo más lejos de la observación:
- Qué esperaba que el compilador optimizara y no lo hizo:
- Qué esperaba que costara la costura y no costó:
- Qué no me pudo decir la transcripción:
- Qué me dijo la sonda que la transcripción no:
- Qué evento añadiría después, y por qué:
- Qué evento no añadiría, y por qué:
- El cambio que haría después para volver a probar este diseño:
- Siguiente paso:

## 11. Si una línea base falta o no es fiel

Completa esto solo si faltó un archivo, una compilación falló por una razón ambiental, un diff no
estaba vacío o una medición contradijo el comportamiento documentado.

- Archivo ausente, error de compilación, diff o contradicción:
- Salida exacta del comando:
- Qué hice en vez de inventar un conteo:
- Cuándo volveré a intentarlo:

## 12. Autocomprobación

- [ ] Inventarié los ocho métodos del puerto con sus líneas.
- [ ] Inventarié los dos adaptadores y su estado.
- [ ] Conté los puntos de llamada `port.*`: diez.
- [ ] Mostré que el núcleo no tiene `<cstdio>` ni `printf`.
- [ ] Predije las tablas de estructura, adaptadores y tamaños antes de medir.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Ejecuté la sonda y comprobé su aritmética.
- [ ] Medí el motor, el tablero, `game.o`, el objeto del adaptador de consola y el binario.
- [ ] Conté las llamadas indirectas: 8 frente a 0.
- [ ] Añadí un evento al puerto en una copia temporal y lo implementé en ambos adaptadores.
- [ ] Las 20 trazas coincidieron después del cambio.
- [ ] `playTurn` quedó sin tocar.
- [ ] Separé lo que decide el núcleo de lo que decide el adaptador.
- [ ] Nombré la regla de ciclo de vida y la regla de cambio.
- [ ] Nombré los tres acoplamientos que quedan.
- [ ] Mi explicación de una frase evita las palabras del mecanismo y las del veredicto.
- [ ] No edité ningún archivo de código versionado.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §10.
