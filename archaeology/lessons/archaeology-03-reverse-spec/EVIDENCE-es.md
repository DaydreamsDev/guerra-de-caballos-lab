# archaeology-03 — Evidencia: reconstruir la especificación

> Este es tu registro. Escríbelo para tu yo futuro.
> Registra lo que observaste, no lo que esperabas que la lección pidiera.

## 1. Entorno

- Fecha:
- Ruta del archivo:
- Etiqueta descargada:
- Hash del commit (`git rev-parse HEAD`):
- Archivo fuente y cantidad de líneas (`wc -l`):
- Compilador y versión (`c++ --version`):
- Biblioteca C y versión (primera línea de `ldd --version`):
- Shell y versión:
- Primera línea de `timeout --version`:
- Kernel y distribución (`uname -sr`):
- Directorio de trabajo:
- Algo inesperado en el entorno:

## 2. Predicción — escribir antes de abrir el código

No edites esta tabla después del paso 4. La distancia entre la predicción y la observación es la
señal de aprendizaje; reparar la predicción la destruye.

| # | Pregunta | Mi predicción | Cómo sabría que está mal |
|---|---|---|---|
| 1 | Tablero y piezas: tamaño, cantidad, visualización | | |
| 2 | Inicio: quién empieza y dónde arrancan los caballos | | |
| 3 | Prioridad de movimiento: cómo se ordenan los ocho movimientos | | |
| 4 | Aleatoriedad: generador, semilla y reproducibilidad | | |
| 5 | Final: cómo termina la partida y qué imprime | | |

Una frase falsable sobre el conjunto:

>

## 3. Inspección del código — registro de reglas

Lee el código en orden de ejecución. Añade una fila por comportamiento. Usa las marcas
**verificado**, **inferido** o **desconocido**.

| # | Regla | Líneas | Evidencia | Marca |
|---:|---|---:|---|---|
| 1 | Dimensiones del tablero | | | |
| 2 | Numeración de casillas y coordenadas mostradas | | | |
| 3 | Cantidad de piezas y símbolos | | | |
| 4 | Posiciones iniciales | | | |
| 5 | Quién empieza | | | |
| 6 | Alternancia de turnos | | | |
| 7 | Generación del orden de movimientos | | | |
| 8 | Los ocho desplazamientos del caballo | | | |
| 9 | Movimiento legal: dentro del tablero | | | |
| 10 | Movimiento legal: casilla visitada | | | |
| 11 | Captura | | | |
| 12 | Trampa | | | |
| 13 | Empate | | | |
| 14 | Generador aleatorio y semilla | | | |
| 15 | Formato de salida | | | |

### 3.1 Las tres capas

| Capa | Qué encontré | Líneas |
|---|---|---|
| Reglas del juego | | |
| Aleatoriedad | | |
| Presentación | | |

### 3.2 Hallazgos que cambiaron mi lectura

Para cada uno, escribe la línea y la sorpresa.

- Casillas iniciales marcadas como visitadas:
- Orden de movimientos extraído por par de turnos:
- Rango de las tiradas:
- Semilla reiniciada dentro del bucle de reintento:
- Ausencia de regla de empate:
- Otro:

### 3.3 Ambigüedades

| Ambigüedad | Qué hace el código | Qué afirma el comentario | Cuál usé y por qué |
|---|---|---|---|
| | | | |
| | | | |

## 4. Registro de captura

- Comando:
- Semilla:
- Archivo de salida:
- Estado de salida del script:

Pega las cinco líneas de salida literalmente:

```text
seed=
output=
run_status=
stdout_lines=
stderr_lines=
```

- Qué significa `run_status` aquí:
- Qué habría significado `124`:
- La primera línea de error en mi idioma:
- La primera línea de error con `LC_ALL=C` (si la revisé):
- Cantidad de fallos de `color`:
- Cantidad de fallos de `cls`:
- ¿Coinciden con la cantidad de turnos? sí / no

### 4.1 Comprobación de determinismo

| Ejecución | Semilla | Archivo de salida | Líneas de salida | `sha256sum` |
|---|---:|---|---:|---|
| 1 | | | | |
| 2 (misma semilla) | | | | |

- ¿Coinciden los dos hashes? sí / no
- Qué prueba esto:
- Qué no prueba esto:

### 4.2 Segunda semilla

| Semilla | `run_status` | Líneas de salida | Líneas de error | Final: captura / trampa / tiempo agotado |
|---:|---:|---:|---:|---|
| | | | | |

- Qué cambió entre las dos semillas:
- Qué se mantuvo igual:
- Qué probaría una tercera semilla:

### 4.3 Semilla con colisión

- Semilla que probé y devolvió `124` (si hubo alguna):
- Por qué devolvió `124`:
- ¿Es una propiedad del original o de la sonda? Explica:

## 5. Traza observada

### 5.1 Inicio

- Tirada del caballo 1:
- Tirada del caballo 2:
- Primer caballo:
- Posición inicial del caballo 1 (índice y fila,columna):
- Posición inicial del caballo 2 (índice y fila,columna):
- Qué casilla inicial tiene el número de jugada 1:
- Qué casilla inicial tiene el número de jugada 2:

### 5.2 Primeros movimientos

Lee el tablero y las líneas `evaluo los casos`. Reconstruye los tres primeros movimientos.

| Turno | Pieza activa | Desde (fila,col) | Número | Destino (fila,col) | Resultado |
|---:|---|---:|---:|---|---|
| 1 | | | | | |
| 2 | | | | | |
| 3 | | | | | |

- El vector de orden del turno 1:
- El vector de orden del turno 2:
- ¿Son el mismo? sí / no
- Qué muestra eso sobre la frecuencia de los sorteos:

### 5.3 Final

- Ganador o caballo encerrado:
- Último número de jugada en el tablero:
- Cantidad de movimientos reales de caballo (último número menos 2):
- Motivo del final:
- Evidencia cruda (cita la línea):

### 5.4 Hechos de la salida

- Codificación de los símbolos del tablero:
- Bytes del caballo 1, del caballo 2 y de la pared:
- ¿La salida es UTF-8 válida? sí / no
- ¿La salida contiene números de turno? sí / no
- ¿La salida contiene coordenadas en forma `fila,columna`? sí / no
- ¿La salida contiene una semilla? sí / no
- Qué necesitaría una transcripción que esta salida no tiene:

## 6. Comparación con la especificación moderna

Abre `common/spec/game-rules.md` solo después de completar la sección 5.

### 6.1 Qué coincide

| Regla | Evidencia heredada | Cláusula moderna |
|---|---|---|
| | | |
| | | |
| | | |

### 6.2 Qué difiere

Clasifica cada fila como **codificación**, **regla** o **no observable**.

| # | Especificación moderna | Código heredado | Líneas | Clase | Por qué importa |
|---:|---|---|---:|---|---|
| 1 | | | | | |
| 2 | | | | | |
| 3 | | | | | |
| 4 | | | | | |
| 5 | | | | | |

### 6.3 El límite de la aleatoriedad

Un párrafo: por qué la misma semilla no produce la misma partida en ambas implementaciones.

>

- Generador heredado:
- Generador moderno:
- Orden de sorteos en el programa heredado:
- Orden de sorteos en la especificación moderna:
- Consecuencia para comparar trazas:

### 6.4 ¿La traza heredada es una traza de referencia?

- Sí / no:
- Por qué:
- Qué haría falta para una comparación justa:

## 7. Límites de la evidencia

### 7.1 Las cuatro categorías

| Categoría | Afirmaciones que puse aquí | Por qué |
|---|---|---|
| Verificado en el código | | |
| Observado en una traza | | |
| Inferido de la estructura | | |
| Desconocido | | |

### 7.2 Las cuatro afirmaciones que la traza no sostiene

| Afirmación | Por qué esta traza no puede sostenerla |
|---|---|
| Esto es lo que imprimía el programa de 2008 | |
| El motor moderno puede reproducir esta traza | |
| El juego es determinista | |
| Las reglas son correctas | |

### 7.3 La frase honesta

Escribe una frase que defenderías en una revisión de código.

>

## 8. Resumen de confianza

| Afirmación | Confianza: alta / media / baja | Qué cambiaría mi opinión |
|---|---|---|
| | | |
| | | |
| | | |

- La afirmación de la que estoy más seguro, y por qué:
- La afirmación de la que estoy menos seguro, y por qué:
- La pregunta que no puedo responder con este archivo:

## 9. Autocomprobación

- [ ] Escribí cinco predicciones antes de abrir el código.
- [ ] Leí el código en orden de ejecución y construí un registro de reglas con citas de línea.
- [ ] Separé reglas del juego, aleatoriedad y presentación.
- [ ] Registré que las casillas iniciales se marcan como visitadas.
- [ ] Registré que el orden de movimientos se extrae una vez por par de turnos.
- [ ] Registré que las tiradas son 0..50, no 1..50.
- [ ] Expliqué por qué `srand(time(NULL))` está dentro del bucle de reintento.
- [ ] Capturé al menos una traza con `capture_trace.sh` y registré sus cinco líneas.
- [ ] Ejecuté la misma semilla dos veces y confirmé bytes idénticos.
- [ ] Ejecuté una segunda semilla y registré cómo cambiaron los conteos de líneas.
- [ ] Expliqué la semilla con colisión como artefacto de la sonda.
- [ ] Leí el tablero CP437 y reconstruí los primeros movimientos.
- [ ] Comparé la especificación reconstruida con `common/spec/game-rules.md`.
- [ ] Clasifiqué cada diferencia como codificación, regla o no observable.
- [ ] Enuncié el límite de la aleatoriedad.
- [ ] Completé `SPEC-TEMPLATE-es.md` y `TRACE-TEMPLATE-es.md`.
- [ ] El archivo está sin modificar (`git status` en el archivo está limpio).

## 10. Siguiente paso

- Qué puedo explicar ahora:
- Qué no puedo explicar todavía:
- Siguiente lección o repetición:
- El defecto que noté y decidí no perseguir:
- La semilla que quiero probar después, y por qué:

## 11. Nota opcional

- Cualquier otra cosa:

---

> Recordatorio: este archivo es evidencia para ti, no una nota para nadie más.
