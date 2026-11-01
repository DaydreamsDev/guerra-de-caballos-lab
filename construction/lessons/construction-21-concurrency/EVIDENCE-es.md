# construction-21 — Evidencia

> [Inglés](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro del aprendiz. Escribe en español.

Este registro compara un núcleo con una secuencia aleatoria global con un núcleo con una secuencia
por motor, y sigue un ejecutor que pierde escrituras cuando comparte un índice de resultados.
Pregunta quién posee el estado del juego, quién posee el estado de resultados, cuál es el contrato
de orden y qué puede ver una ejecución. Conserva las predicciones. La diferencia entre tu predicción
y la observación es el resultado, sea cero o no.

## 1. Preparación de la ejecución

- Fecha:
- Rango de semillas: `1` a `16000`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Plataforma y arquitectura:
- Línea base leída el (fecha):
- ¿Registro de `construction-20` a mano? (sí / no)
- Los números que traje de `construction-20` (líneas / archivos / texto de `game.o` / texto del
  binario / datos / BSS / trazas):
- Directorio temporal que usé:
- ¿Edité algún archivo versionado? (sí / no)

## 2. Inventario del estado (antes de ejecutar)

### 2.1 Dónde vive el estado

Completa esto desde el código, con números de línea, no de memoria.

| Estado | Dueño en `construction-20` | Dueño en `construction-21` | ¿Compartido entre partidas? | Sincronización |
|---|---|---|---|---|
| Estado del generador | | | | |
| Tablero | | | | |
| Transcripción | | | | |
| Vector de resultados | | | | |
| `nextIndex` | | | | |

### 2.2 Conteos

- Archivos:
- Cabeceras:
- Unidades de traducción en la compilación de consola:
- Unidades de traducción en la compilación del ejecutor:
- Unidades de traducción en la compilación de la sonda:
- Definiciones de clase completas en el núcleo:
- Variables globales mutables en el núcleo:
- Objetos mutables compartidos en el ejecutor (race / mutex / merge):
- Líneas de código:
- Archivos idénticos byte a byte a `construction-20`:
- Archivos cambiados:
- Archivos nuevos:

Comparado con `construction-20`:

- Archivos: ______ → ______
- Cabeceras: ______ → ______
- Unidades de traducción (compilación de consola): ______ → ______
- Líneas de código: ______ → ______
- Archivos cambiados: ______
- Archivos nuevos: ______
- Generador global mutable: ______ → ______
- `sizeof(GameEngine<8, 8>)`: ______ → ______
- `sizeof(RandomSequence)`: ______
- Texto de `game.o`: ______ → ______
- Texto del binario: ______ → ______
- Datos del binario: ______ → ______
- BSS del binario: ______ → ______

### 2.3 Preguntas de contrato

| Pregunta | Respuesta |
|---|---|
| ¿Dónde estaba el estado del generador en `construction-20`? | |
| ¿Dónde está en `construction-21`? | |
| ¿Cuántos bytes ocupa `RandomSequence`? | |
| ¿Qué miembro explica ese tamaño? | |
| ¿Cuántos puntos de llamada `randomSequence.*` hay en `game_engine.h`? | |
| ¿Qué funciones libres reemplazaron? | |
| ¿Qué tres archivos cambiaron? | |
| ¿Qué archivo es nuevo? | |
| ¿Qué catorce archivos son idénticos byte a byte? | |
| ¿Cuántos hilos inicia el ejecutor? | |
| ¿Cuántas partidas por hilo? | |
| ¿Cuántas partidas en total? | |
| ¿Qué modos comparten un vector de resultados? | |
| ¿Qué modo da un vector propio a cada hilo? | |
| ¿Qué modo no tiene sincronización? | |
| ¿Qué dos objetos se comparten en `race`? | |
| ¿Qué modos ordenan por semilla antes de la suma? | |
| ¿Por qué la suma necesita ese orden? | |
| ¿Qué centinela usa el escaneo de la carrera? | |
| ¿Por qué ese centinela es seguro para estas semillas? | |
| ¿Qué estado salió de BSS? | |
| ¿Cuántos bytes perdió BSS? | |

### 2.4 Los cuatro modos

| Modo | Estado de resultados compartido | Sincronización | ¿Ordena por semilla? | Qué prueba |
|---|---|---|---|---|
| `sequential` | | | | |
| `race` | | | | |
| `mutex` | | | | |
| `merge` | | | | |

- El modo que es la referencia:
- El modo que es el fallo:
- El modo que es estado compartido con bloqueo:
- El modo que es propiedad más entrega:
- Las dos condiciones que debe cumplir un modo corregido:

## 3. Predicción (escrita antes de medir)

### 3.1 La tabla de estructura

| Pregunta | Mi predicción |
|---|---|
| Archivos en `construction-20` | |
| Archivos aquí | |
| Cabeceras aquí | |
| Unidades de traducción en la compilación de consola | |
| Unidades de traducción en la compilación del ejecutor | |
| Líneas de código en `construction-20` | |
| Líneas de código aquí | |
| Archivos cambiados | |
| Archivos nuevos | |
| Archivos idénticos byte a byte | |
| Variables globales mutables en el núcleo, antes y después | |
| Objetos mutables compartidos en el ejecutor | |

- La fila que espero equivocar, y por qué:
- Por qué el motor parece distinto pero solo cambiaron sus puntos de llamada:
- Qué archivo concentra la mayoría de las líneas nuevas:

### 3.2 La tabla de la carrera

| Pregunta | Mi predicción |
|---|---|
| `written` de `race`, de 16.000 | |
| `next_index` de `race` | |
| ¿`written` es igual a `next_index`? | |
| ¿La suma de `race` es estable entre ejecuciones? | |
| Suma de `sequential` | |
| Suma de `mutex` | |
| Suma de `merge` | |
| ¿Qué modo no ordena por semilla? | |
| ¿Qué modos coinciden con la suma secuencial? | |

- Por qué espero `written` por debajo de 16.000:
- Por qué la suma puede diferir aunque `written` fuera 16.000:
- Qué número es la evidencia directa de una escritura perdida:

### 3.3 La tabla de tamaños y texto

| Pregunta | Mi predicción |
|---|---|
| `sizeof(GameEngine<8, 8>)`, antes y después | |
| `sizeof(RandomSequence)` | |
| Texto de `game.o`, antes y después | |
| Texto del binario, antes y después | |
| Datos del binario, antes y después | |
| BSS del binario, antes y después | |
| Semillas que coinciden, de 20 | |

- Por qué espero que el motor crezca 8 bytes:
- Por qué espero que BSS se reduzca:
- Por qué espero que los datos no se muevan:
- El número que espero que me sorprenda:

### 3.4 La tabla de eliminación

| Pregunta | Mi predicción |
|---|---|
| La corrección que aplicaré | |
| Archivos que editaré | |
| Líneas que añadiré | |
| `written` después de la corrección | |
| Suma después de la corrección | |
| Semillas que coinciden, de 20, después de la corrección | |
| Resultado de compilación con `-Wall -Wextra -Werror` | |

- Por qué la corrección debería tocar solo `parallel_runner.cpp`:
- Por qué el orden forma parte de la corrección:
- Qué me diría que la carrera sigue presente:

## 4. Ejecución — la línea base

### 4.1 Trazas

- Comando de compilación:
- Estado de salida:
- Diff contra `../../../common/traces/seed-0001.txt`:
- Semillas que coincidieron, de 20, en la línea base sin modificar:

### 4.2 Los cuatro modos

- Comando de compilación:
- Estado de salida:
- Línea de `sequential`:
- Línea de `race`:
- Línea de `mutex`:
- Línea de `merge`:

| Modo | Escritas | `next_index` | Suma de verificación |
|---|---:|---:|---:|
| `sequential` | | | |
| `race` | | | |
| `mutex` | | | |
| `merge` | | | |

- Qué modos coinciden con la suma secuencial:
- Qué modo pierde escrituras:
- Qué modo tiene una suma inestable:

### 4.3 Carrera repetida

| Ejecución | `next_index` | `written` | Suma de verificación |
|---|---:|---:|---:|
| 1 | | | |
| 2 | | | |
| 3 | | | |
| 4 | | | |

- ¿`written` fue igual a `next_index` en todas las ejecuciones? (sí / no)
- Por qué aparece esa igualdad y por qué no es una garantía:
- Las dos razones por las que la suma difiere:
- Qué prueba el número de escrituras que la suma no prueba:

### 4.4 Tamaños

- Salida de la sonda de `sizeof`:
- `sizeof(GameEngine<8, 8>)`:
- `sizeof(RandomSequence)`:

### 4.5 Objeto y binario

- Texto de `game.o` antes y después:
- Texto del binario enlazado antes y después:
- Datos del binario enlazado antes y después:
- BSS del binario enlazado antes y después:
- Qué estado explica el cambio de BSS:

### 4.6 Opcional: ThreadSanitizer

- ¿TSan disponible? (sí / no)
- Comando:
- ¿Se encontró un informe? (sí / no)
- Las dos ubicaciones del informe:
- Qué prueba el informe que el número no prueba:
- Qué no prueba el informe:

### 4.7 Predicción y observación

| Observación | Predicho | Observado | Brecha |
|---|---|---|---|
| Archivos | | | |
| Líneas de código | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| `sizeof(RandomSequence)` | | | |
| Texto de `game.o` | | | |
| Texto del binario | | | |
| Datos del binario | | | |
| BSS del binario | | | |
| `written` de la carrera | | | |
| Suma de la carrera | | | |
| Trazas coincidentes | | | |

- El número que me sorprendió:
- Qué hice después en lugar de ajustar la predicción:

## 5. Comparar — qué prueba la propiedad

- Comando para la comprobación del estado del generador:
- Dónde estaba el estado antes:
- Dónde está el estado ahora:
- Comando para la comprobación de puntos de llamada:
- Puntos de llamada `randomSequence.*`:
- Qué funciones libres desaparecieron:
- Texto de `game.o` antes y después:
- Texto del binario enlazado antes y después:
- Datos del binario enlazado antes y después:
- BSS del binario enlazado antes y después:
- Qué prueban los tamaños:
- Qué prueban las 20 trazas:
- Qué no prueban las trazas:
- Qué prueba la suma:
- Qué no prueba la suma:

### 5.1 El diff exacto

- Comando:
- Archivos cambiados:
- Archivos nuevos:
- Líneas añadidas / eliminadas / netas:
- `game_engine.h`: añadidas / eliminadas / netas:
- `random_sequence.h`: añadidas / eliminadas / netas:
- `random_sequence.cpp`: añadidas / eliminadas / netas:
- Líneas de `parallel_runner.cpp`:
- Archivos idénticos byte a byte:

### 5.2 Qué no cambió

- El LCG:
- La transcripción:
- Las reglas:
- El diseño del tablero:
- El generador de movimientos:
- El bucle de intentos:
- La interfaz pública del juego:
- El número de trazas:
- Qué no movió fuera del motor el cambio de propiedad:

### 5.3 Qué cuesta

- Archivos y líneas nuevas:
- Bytes añadidos por motor:
- Texto añadido al binario:
- Datos añadidos al binario:
- BSS eliminada del binario:
- La regla de vida dentro del motor:
- La carga de lectura nueva:
- El segundo programa:
- El contrato de orden:

### 5.4 La tabla de estado

| Estado | Dueño en `construction-20` | Dueño en `construction-21` | ¿Compartido entre partidas? | Sincronización |
|---|---|---|---|---|
| Estado del generador | | | | |
| Tablero | | | | |
| Transcripción | | | | |
| Vector de resultados | | | | |
| `nextIndex` | | | | |

- Qué filas son poseídas:
- Qué filas son el problema del ejecutor:
- Por qué el estado poseído no necesita protocolo:
- Por qué el estado compartido necesita uno:

### 5.5 Por qué coinciden los modos corregidos

- Qué construye `run_one_game` por llamada:
- Qué hace `run_mutex` alrededor del añadido:
- Qué hace `run_merge` después del join:
- Qué pliega `result_checksum` y en qué orden:
- Por qué `sequential` ya está en orden de semillas:
- Por qué `mutex` necesita el orden:
- Por qué `merge` ordena aunque su concatenación ya esté ordenada por semilla:
- Las tres condiciones que hacen iguales las sumas:

## 6. La prueba de cambio — eliminar la carrera

- Ruta de la copia temporal:
- Archivos copiados:
- Archivos editados:
- Archivos versionados editados: (sí / no)
- ¿Corrección A (mutex) aplicada? (sí / no)
- ¿Corrección B (resultados por hilo) aplicada? (sí / no)
- Comando de compilación:
- Estado de compilación con `-Wall -Wextra -Werror`:
- Salida después de la corrección A:
- Salida después de la corrección B:
- ¿Estable en al menos dos ejecuciones? (sí / no)
- Semillas que coincidieron, de 20, después de la corrección:

### 6.1 Corrección A: el parche

- Líneas eliminadas de `run_race`:
- Líneas añadidas para el mutex:
- Cambio de captura de la lambda:
- El añadido protegido:
- La llamada de orden:
- El cambio del `printf`:
- Por qué se borra el escaneo `writtenResults`:

### 6.2 Corrección B: el parche

- El vector por hilo:
- La captura de la lambda:
- El añadido dentro del hilo:
- La fusión después del join:
- La llamada de orden:
- Por qué no aparece ningún bloqueo:
- En qué difiere el coste de la corrección B frente a la A:

### 6.3 Qué prueba la prueba de cambio y qué no

- Prueba sobre las escrituras perdidas:
- Prueba sobre el generador del motor:
- Prueba sobre el orden:
- Prueba sobre el radio de impacto:
- Prueba sobre las dos correcciones:
- No prueba sobre un motor compartido:
- No prueba sobre rendimiento:
- No prueba sobre la reproducibilidad de la carrera:
- Qué corrige la variante atómica y qué deja roto:

### 6.4 Conteos

| Métrica | `construction-20` | `construction-21` | Diferencia |
|---|---:|---:|---:|
| Líneas de código | | | |
| Archivos | | | |
| Generador global mutable | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| `sizeof(RandomSequence)` | | | |
| Texto de `game.o` | | | |
| Texto del binario | | | |
| Datos del binario | | | |
| BSS del binario | | | |
| Trazas coincidentes | | | |
| Modos paralelos | | | |

## 7. El modelo de concurrencia y sus límites

| Pregunta | Respuesta |
|---|---|
| ¿Qué posee el motor? | |
| ¿Qué posee el ejecutor? | |
| ¿Qué se comparte en `race`? | |
| ¿Qué se comparte en `mutex`? | |
| ¿Qué se comparte en `merge`? | |
| ¿Cuál es el «mensaje» en `merge`? | |
| ¿Por qué `race` pierde escrituras? | |
| ¿Por qué `written` es igual a `next_index` en las ejecuciones observadas? | |
| ¿Por qué esa igualdad no es una garantía? | |
| ¿Por qué la suma necesita un orden? | |
| ¿Qué serializa el mutex? | |
| ¿Qué evita la fusión? | |
| ¿Qué no garantiza el motor? | |
| ¿Qué no garantiza el adaptador de transcripción? | |
| ¿Qué no prueban las 20 trazas? | |
| ¿Qué no prueba la suma? | |
| ¿Qué necesitaría un planificador real que el ejecutor no tiene? | |
| ¿Qué lección posterior se ocupa de los contratos de error? | |

- Los cinco acoplamientos que quedan:
- Qué acoplamiento es sobre el motor:
- Qué acoplamiento es sobre el adaptador:
- Qué acoplamiento es sobre la recolección de resultados:
- Qué acoplamiento es sobre el orden:
- Qué acoplamiento es sobre la evidencia:

## 8. Explicar

Una frase, sin las palabras *hilo*, *mutex*, *bloqueo*, *carrera*, *concurrencia*, *concurrente*,
*paralelo*, *atómico*, *compartido*, *mejor*, *peor*, *limpio* ni *desacoplado*:

>

Un párrafo:

- Qué cambió:
- Qué quedó igual:
- Qué añade el ejecutor:
- Qué es la carrera:
- Qué son las correcciones:
- Qué requiere el determinismo:
- Qué prueban los números:
- Qué prueba la suma:
- Qué prueban las trazas:
- Qué lección posterior se ocupa de los contratos de error:

Las cuatro cosas que mantuve separadas:

## 9. Comprobación de aceptación

- Localicé el `randomState` global y el miembro `state` privado: (sí / no)
- Conté los ocho puntos de llamada `randomSequence.*`: (sí / no)
- Nombré los tres archivos cambiados, el ejecutor nuevo y los catorce archivos idénticos:
  (sí / no)
- Predije las tablas de estructura, carrera, tamaños y eliminación antes de medir: (sí / no)
- Reproduje las 20 trazas en la línea base de consola sin modificar: (sí / no)
- Ejecuté los cuatro modos y registré escritas y suma: (sí / no)
- Ejecuté `race` al menos cuatro veces y vi moverse el resultado: (sí / no)
- Expliqué por qué `written` es igual a `next_index`, con la advertencia de comportamiento
  indefinido: (sí / no)
- Expliqué las dos razones por las que la suma de la carrera difiere: (sí / no)
- Medí `sizeof(GameEngine<8, 8>)` en 40 y `sizeof(RandomSequence)` en 8: (sí / no)
- Medí el texto de `game.o` en 4.280 y el binario en 10.869 de texto, 876 de datos y 16 de BSS:
  (sí / no)
- Eliminé la carrera en una copia temporal con un mutex o con resultados por hilo: (sí / no)
- La ejecución corregida imprimió `written=16000` y `checksum=2826026307342797653`: (sí / no)
- La ejecución corregida fue estable en al menos dos ejecuciones: (sí / no)
- Las 20 trazas coincidieron después de la corrección: (sí / no)
- La compilación temporal pasó con `-Wall -Wextra -Werror`: (sí / no)
- Nombré qué no garantiza el motor: (sí / no)
- Nombré el contrato de orden: (sí / no)
- No edité ningún archivo de código versionado: (sí / no)

Si edité un archivo de código versionado, cuál y por qué:

## 10. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué no puedo explicar todavía:
- Una cosa que volvería a leer:
- El número que llevaré a `construction-22`:
- La predicción que más se alejó de la observación:
- Qué esperaba que el compilador optimizara y no lo hizo:
- Qué esperaba que hiciera la carrera y no hizo:
- Qué me dijo el número de escrituras que la suma no pudo decirme:
- Qué me dijo la suma que el número de escrituras no pudo decirme:
- Qué corrección elegiría para un conjunto de resultados más grande, y por qué:
- Qué corrección no elegiría, y por qué:
- El cambio que haría después para volver a probar este diseño:
- Siguiente paso:

## 11. Si falta una línea base o no es fiel

Completa esto solo si faltó un archivo, una compilación falló por una razón ambiental, un diff no
estaba vacío, TSan no estaba disponible o una medición contradijo el comportamiento documentado.

- Archivo faltante, error de compilación, diff o contradicción:
- Salida exacta del comando:
- Qué hice en lugar de inventar un conteo:
- Cuándo volveré a intentarlo:

## 12. Autocomprobación

- [ ] Inventarié los dos estados: el generador del juego y la recolección de resultados del
      ejecutor.
- [ ] Conté los ocho puntos de llamada `randomSequence.*`.
- [ ] Nombré los tres archivos cambiados y el ejecutor nuevo.
- [ ] Predije las tablas de estructura, carrera, tamaños y eliminación antes de medir.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Ejecuté `sequential`, `race`, `mutex` y `merge`.
- [ ] Ejecuté `race` más de una vez y expliqué la variación.
- [ ] Medí el motor en 40 bytes y `RandomSequence` en 8.
- [ ] Medí `game.o`, el texto del binario, los datos del binario y la BSS del binario.
- [ ] Eliminé la carrera con un mutex y, si fue posible, con resultados por hilo.
- [ ] La suma corregida coincidió con la secuencial y se mantuvo estable.
- [ ] Las 20 trazas coincidieron después de la corrección.
- [ ] Separé el estado poseído del estado compartido.
- [ ] Nombré el contrato de orden y la no reentrancia del motor.
- [ ] Mi explicación de una frase evita las palabras de mecanismo y de veredicto.
- [ ] No edité ningún archivo de código versionado.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §10.
