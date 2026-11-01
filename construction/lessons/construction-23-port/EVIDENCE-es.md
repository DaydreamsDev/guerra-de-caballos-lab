# construction-23 — Evidencia

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro propiedad de quien aprende. Escribe en español.

Este registro sigue una partida desde un sistema de tipos de C++ hasta un módulo de Python y
pregunta qué sobrevivió. Mantiene cuatro cosas separadas: las reglas, la transcripción, la
representación y el proceso. Conserva las predicciones. La distancia entre tu predicción y la
observación es el resultado, sea cero o no.

## 1. Configuración de la ejecución

- Fecha:
- Rango de semillas: `1` a `20`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Versión de Python (`python3 --version`):
- Compilador de C++ y versión (`c++ --version`):
- Plataforma y arquitectura:
- Línea base leída el (fecha):
- ¿Registro de `construction-22` a mano? (sí / no)
- Los números que traje de `construction-22` (líneas de código / archivos / trazas):
- Directorio temporal que usé:
- ¿Edité algún archivo versionado? (sí / no)

## 2. Inventario del port (antes de ejecutar)

Rellena esto desde el código, con números de línea, no de memoria.

### 2.1 Tamaño y forma

| Pregunta | Respuesta |
|---|---|
| Archivos en el port | |
| Líneas en `code/game.py` | |
| Líneas en `code/main.py` | |
| Total de líneas de código | |
| Cabeceras | |
| Unidades de traducción | |
| Pasos de compilación | |
| Dependencias de terceros | |
| Definiciones de nivel superior en `game.py` | |
| Definiciones de nivel superior en `main.py` | |
| Líneas en `common/harness/reference.py` | |

### 2.2 Qué reemplazó a qué

| Símbolo o archivo de C++ | Equivalente en Python | Qué pasó con el concepto |
|---|---|---|
| `Board` | | |
| `Position` | | |
| `Knight` | | |
| `MoveGenerator` | | |
| `RandomSequence` | | |
| `TranscriptPort` y adaptadores | | |
| `MoveOutcome` | | |
| `MoveResult` | | |
| `GameEngine::playTurn` | | |
| `parallel_runner.cpp` | | |
| `error_probe.cpp` | | |

### 2.3 Las reglas que no deben moverse

| Regla | Dónde en C++ | Dónde en Python |
|---|---|---|
| Multiplicador del LCG | | |
| Incremento del LCG | | |
| Máscara de 64 bits | | |
| Ocho desplazamientos, en orden | | |
| Movimiento inicial y rotación | | |
| Límites primero | | |
| Captura antes que visitada | | |
| Ganador de la trampa | | |
| Empate a 200 movimientos | | |
| Líneas de cabecera de la transcripción | | |

- Las tres constantes que encontré en ambos archivos:
- El primer y el último desplazamiento en ambos archivos:
- Qué archivo decide el estado de salida del proceso y mediante qué llamada:

## 3. Predicción (escrita antes de medir)

### 3.1 La tabla de estructura

| Pregunta | Mi predicción |
|---|---|
| Archivos en la línea base de C++ | |
| Archivos en el port | |
| Líneas de código en la línea base de C++ | |
| Líneas de código en el port | |
| Cabeceras en el port | |
| Pasos de compilación en el port | |
| Definiciones de nivel superior en `game.py` | |
| Trazas de referencia que coinciden | |

- La fila que espero fallar, y por qué:
- Por qué quitar clases no quita funciones:
- Qué hace `run.sh`, en una frase:

### 3.2 La tabla de comportamiento

| Pregunta | Mi predicción |
|---|---|
| Semillas que coinciden, de 20 | |
| Trazas que terminan en captura | |
| Trazas que terminan en trampa | |
| Trazas que terminan en empate | |
| Partida más larga, en movimientos | |
| Trazas con al menos una línea `illegal=visited` | |

- Cómo puede terminar una partida, según las reglas:
- Por qué un empate necesita una partida larga:
- Mi conjetura para la partida más larga, y la razón:

### 3.3 La tabla de tiempos

| Pregunta | Mi predicción |
|---|---|
| 20 partidas con `run.sh` | |
| 20 partidas en un solo proceso de Python | |
| 20 procesos de C++ | |
| 20 arranques vacíos de `python3 -c 'pass'` | |

- Cuál de estos números espero que domine la cifra de `run.sh`:
- Por qué el número en proceso es la comparación justa del trabajo del juego:
- Qué necesitaría un banco de pruebas que esta lección no ejecuta:

### 3.4 La tabla de la prueba de cambio

| Pregunta | Mi predicción |
|---|---|
| Trazas que fallan si la captura se comprueba después de visitada | |
| Trazas que fallan si se invierte el ganador de la trampa | |
| Trazas que fallan si el límite de empate es 199 en vez de 200 | |

- Por qué una regla solo puede romper una traza que la alcanza:
- Las semillas que espero que fallen con el intercambio, y por qué:
- Las semillas que espero que fallen con la inversión de la trampa, y por qué:

## 4. Ejecución — el port

### 4.1 Trazas

- Comando:
- Semillas que coincidieron, de 20, con el port sin modificar:
- Primer desplazamiento de byte de cualquier diff:
- La traza que leí primero, y qué contiene:

### 4.2 El lado de C++

- Comando para la semilla de captura:
- Comando para la semilla de trampa:
- Salida del diff:
- Qué comparten los dos programas, según el diff:
- Qué no puede mostrar el diff:

### 4.3 El oráculo

- Comando:
- Salida del diff:
- Por qué `reference.py` es un oráculo:
- Por qué no es un diseño independiente:
- Los nombres que cambiaron en `diff -u`:
- Qué prueba esta coincidencia y qué no:

### 4.4 Tiempos

- 20 partidas con `run.sh`:
- 20 procesos de C++:
- 20 arranques vacíos de Python:
- 20 partidas en un proceso de Python:
- Mi predicción y la distancia:
- La afirmación que puedo defender: ¿domina el arranque o el trabajo?
- Qué no voy a afirmar con estos cuatro números:

### 4.5 La ruta de argumentos

- Comando con un `--name` ausente:
- Mensaje:
- Estado de salida:
- Comando con una partida válida:
- Estado de salida:
- Qué llamada produce el estado:
- En qué se diferencia de que `main` devuelva 2 en C++:

### 4.6 Predicción y observación

| Observación | Predicho | Observado | Distancia |
|---|---|---|---|
| Archivos | | | |
| Líneas de código | | | |
| Definiciones de nivel superior | | | |
| Trazas que coinciden | | | |
| Trazas que terminan en captura | | | |
| Trazas que terminan en trampa | | | |
| Trazas que terminan en empate | | | |
| Partida más larga | | | |
| 20 partidas con `run.sh` | | | |
| 20 partidas en un proceso | | | |
| 20 procesos de C++ | | | |
| 20 arranques de Python | | | |

- El número que me sorprendió:
- Qué hice después en lugar de ajustar la predicción:

## 5. Comparación — qué sobrevivió

- Cómo se ven las reglas en ambos archivos:
- Cómo se ve la representación en el port:
- Qué pasó con los cuatro resultados:
- Qué pasó con la decisión de quien llama en `construction-22`:
- Qué comprobaba el compilador, y qué lo comprueba ahora:
- Qué prueban las trazas:
- Qué no prueban las trazas:

### 5.1 Tres cosas que cambiaron

1. 2. 3.

- Evidencia de cada una:
- Cuál es el cambio más grande, y por qué:
- Cuál es solo consecuencia de otro:

### 5.2 Tres cosas que no cambiaron

1. 2. 3.

- Evidencia de cada una:
- Por qué el LCG es la evidencia más fuerte de un port:
- Por qué el orden de comprobación es la segunda más fuerte:
- Qué más no cambió:

### 5.3 Lo que el port quitó

| Quitado | Qué aportaba | Qué lo reemplaza |
|---|---|---|
| `parallel_runner.cpp` | | |
| `error_probe.cpp` | | |
| `adapter_probe.cpp` | | |
| `RecordingTranscript` | | |
| `--max-moves` | | |

- Qué pieza quitada debilita más la evidencia del port:
- Qué no puede preguntar el port que la línea base de C++ sí podía:
- Qué conserva el oráculo que el port no:

### 5.4 El diff de comportamiento

- Comando:
- Salida:
- Semillas comparadas:
- Por qué un diff estructural no es la herramienta correcta aquí:

## 6. La prueba de cambio — una regla, solo en el port

- Ruta de la copia temporal:
- Archivos copiados:
- Archivos creados:
- Archivos versionados editados: (sí / no)
- Comando que ejecuta la copia:
- Comando que compara con las trazas:

### 6.1 Cambio A: captura después de visitada

- Líneas movidas:
- Semillas que fallaron:
- Recuento, de 20:
- Semillas que terminan en captura:
- ¿Son las mismas dos listas? (sí / no)
- Por qué una partida de captura puede ver el cambio:
- Por qué una partida de trampa no puede:
- Qué muestra la transcripción en la primera divergencia:

### 6.2 Cambio B: ganador de la trampa invertido

- Línea cambiada:
- Semillas que fallaron:
- Recuento, de 20:
- Semillas con `no_legal_move`:
- ¿Son las mismas dos listas? (sí / no)
- Por qué la inversión es invisible para todas las demás trazas:

### 6.3 Cambio C: límite de empate en 199

- Semillas que fallaron:
- Partida más larga en las trazas:
- Por qué el límite es código y no evidencia:

### 6.4 Restauración

- Comando:
- Semillas que coincidieron tras restaurar, de 20:
- ¿Tuve que revertir un archivo versionado? (sí / no)

### 6.5 Qué prueba la prueba de cambio y qué no

- Prueba sobre las trazas como detector:
- Prueba sobre la guardia del port:
- Prueba sobre las reglas inalcanzables:
- Prueba sobre las dos implementaciones:
- No prueba sobre semillas no vistas:
- No prueba sobre errores compartidos:
- No prueba sobre rendimiento:

## 7. El modelo del port y sus límites

| Pregunta | Respuesta |
|---|---|
| Qué debe coincidir exactamente | |
| Qué es libre de cambiar | |
| Qué es la transcripción | |
| Qué son las reglas | |
| Qué eligió la representación | |
| Qué promete el proceso | |
| Por qué 245 líneas no son una medida de calidad | |
| Por qué el oráculo es un primo | |
| Por qué el empate es inalcanzable | |
| Qué no puede sondear el port | |
| Qué le hace el arranque a los tiempos | |
| Qué lección posterior tiene el proyecto final | |

- Los cuatro límites que puedo nombrar:
- El límite que más importa para la corrección:
- El límite que más importa para la medición:
- Qué añadiría al port para cerrar un hueco, sin cambiar su comportamiento:

## 8. Explicar

Una frase, sin las palabras *sintaxis*, *lenguaje*, *traducir*, *mejor*, *peor*, *más limpio* ni
*más rápido*:


Un párrafo:

- Qué cambió:
- Qué quedó:
- Qué mostró la prueba de cambio:
- Qué quitó el port:
- Qué significan los números:
- Qué prueban las trazas:
- Qué no prueban:
- Qué lección posterior tiene el proyecto final:

Las cuatro cosas que mantuve separadas:

## 9. Criterio de aceptación

- Leí `game.py`, `main.py` y `run.sh`: (sí / no)
- Conté 245 líneas en 2 archivos y 1.071 en 20: (sí / no)
- Encontré las tres constantes del LCG y los ocho desplazamientos en ambos: (sí / no)
- Predije las cuatro tablas antes de medir: (sí / no)
- Reproduje las 20 trazas, byte a byte: (sí / no)
- Comparé el port con C++ en una semilla de captura y una de trampa: (sí / no)
- Ejecuté `reference.py` y expliqué el problema de los primos: (sí / no)
- Medí los cuatro tiempos: (sí / no)
- Afirmé que el arranque domina la cifra de Python: (sí / no)
- Ejecuté la ruta de argumentos inválidos y registré el estado 2: (sí / no)
- Nombré tres cosas que cambiaron y tres que no: (sí / no)
- Intercambié captura y visitada y vi fallar 11 semillas: (sí / no)
- Invertí el ganador de la trampa y vi fallar 9 semillas: (sí / no)
- Expliqué por qué las otras semillas no pueden ver cada cambio: (sí / no)
- Registré que ninguna traza de referencia alcanza el empate a 200 movimientos: (sí / no)
- Restauré la copia y confirmé 20 de 20: (sí / no)
- No edité ningún archivo de código versionado: (sí / no)

Si edité un archivo de código versionado, cuál y por qué:

## 10. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué no puedo explicar todavía:
- Una cosa que volvería a leer:
- El número que llevaré a `construction-24`:
- La predicción más lejana de la observación:
- Qué esperaba que Python hiciera más fácil y no lo hizo:
- Qué esperaba que Python hiciera más difícil y no lo hizo:
- Qué me sigue diciendo la línea base de C++ que el port no puede:
- Qué sonda quitada reconstruiría primero, y por qué:
- Qué probaría un tercer lenguaje que no probó el segundo:
- Siguiente paso:

## 11. Si algo falta o no es fiel

Rellena esto solo si faltaba un archivo, una ejecución falló por una razón del entorno, un diff no
estaba vacío o una medición contradijo el comportamiento documentado.

- Archivo ausente, error de ejecución, diff o contradicción:
- Salida exacta del comando:
- Qué hice en lugar de inventar un recuento:
- Cuándo lo reintentaré:

## 12. Autocomprobación

- [ ] Inventarié el port: archivos, líneas, definiciones y las reglas que no deben moverse.
- [ ] Nombré las cuatro capas y cuál debe coincidir exactamente.
- [ ] Predije las tablas de estructura, comportamiento, tiempos y prueba de cambio.
- [ ] Reproduje las 20 trazas con el port sin modificar.
- [ ] Comparé el port con la línea base de C++ en dos semillas.
- [ ] Ejecuté el oráculo y expliqué el problema de los primos.
- [ ] Medí los cuatro tiempos y separé arranque de trabajo.
- [ ] Ejecuté la ruta de argumentos inválidos y registré el dueño del estado.
- [ ] Nombré tres cosas que cambiaron y tres que no.
- [ ] Rompí una regla en una copia y localicé las semillas que fallan.
- [ ] Expliqué por qué las otras semillas no pueden ver el cambio.
- [ ] Nombré el empate inalcanzable y las sondas quitadas.
- [ ] Restauré la copia y confirmé 20 de 20.
- [ ] Mi explicación de una frase evita las palabras prohibidas.
- [ ] No edité ningún archivo de código versionado.

Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §10.
