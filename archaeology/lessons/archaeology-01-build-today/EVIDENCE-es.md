# archaeology-01 — Evidencia: compilar y ejecutar

> Este es tu registro. Escríbelo para tu yo futuro. Registra
> lo que observaste, no lo que creías que la lección quería.

## 1. Entorno

- Fecha:
- Ruta del archivo:
- Etiqueta descargada:
- Hash del commit (`git rev-parse HEAD`):
- Archivo fuente y número de líneas (`wc -l`):
- Compilador y versión (`c++ --version`):
- Shell y versión:
- Primera línea de `timeout --version`:
- Núcleo y distribución (`uname -sr`):
- Configuración regional usada para los mensajes del compilador (`echo $LC_ALL`):
- Directorio de trabajo:
- Algo inesperado en el entorno:

## 2. Predicción — escribir antes de la primera compilación

No edites esta tabla después del paso 5. La diferencia entre predicción y observación es la señal de
aprendizaje; reparar la predicción la destruye.

| # | Pregunta | Mi predicción | Cómo sabría que está mal |
|---|---|---|---|
| 1 | Interfaz: ¿una pantalla o un registro? | | |
| 2 | Interacción: ¿espera entrada? | | |
| 3 | Azar: ¿reproducible dos veces? | | |
| 4 | Condición de fin y mensaje final | | |
| 5 | Primera cosa que se rompe en Linux | | |

Una frase falsable sobre el conjunto:

>

## 3. Compilación ingenua — la escalera de errores

Copia el código fuera del archivo antes de compilar. Registra el comando de copia.

- Comando de copia:
- Comando de compilación 1 (sin cuñas):
- Estado de salida:

Pega la salida literal:

```text

```

- Comando de compilación 2 (encabezados satisfechos):
- Estado de salida:

Pega la salida literal:

```text

```

- Comando de compilación 3 (cualquier etapa adicional que necesité):

Pega la salida literal:

```text

```

### 3.1 Tabla de errores

| # | Texto del error (solo la primera línea) | Archivo:línea | Clase: encabezado ausente / regla del lenguaje / otra |
|---|---|---|---|
| 1 | | | |
| 2 | | | |
| 3 | | | |

- ¿El orden de los errores coincidió con el orden de los `include` del archivo? sí / no
- Si no, ¿por qué no?:

### 3.2 Advertencias

- Comando usado para recoger advertencias (por ejemplo `-Wall -Wextra`):
- Cantidad de advertencias:
- Clases de advertencia y cantidades:
- Qué advertencia entrego a `archaeology-04` en lugar de diagnosticarla ahora:

## 4. Cambios de compatibilidad

Cada cambio es una adaptación en el nivel de compilación. Ninguno edita el archivo. Una fila por
cambio.

| Cambio | Dónde vive | Qué hace | Riesgo que introduce |
|---|---|---|---|
| `code/compat/iostream.h` | archivo de la lección | | |
| `code/compat/conio.h` | archivo de la lección | | |
| `code/legacy_wrapper.cpp` | archivo de la lección | | |
| `-fpermissive` | bandera de compilación | | |
| `-Dmain=legacy_main` | bandera de compilación | | |
| `-Icode/compat` | bandera de compilación | | |

- Qué cambio altera más el comportamiento del programa, y cómo:
- Qué cambio quitaría primero si quisiera un port real, y por qué:

## 5. Compilación y ejecución con la sonda

- Comando:
- Estado de salida del script:

Pega las seis líneas de salida literales:

```text
source=
compile=ok
run_status=
stdout_lines=
stderr_lines=
stderr_first=
```

- Qué significa `run_status` aquí:
- Qué habría significado `124`:
- La primera línea de `stderr` en mi configuración regional:
- La primera línea de `stderr` con `LC_ALL=C` (si lo comprobé):

### 5.1 Registro de ejecuciones

| Ejecución | Segundo de reloj | Líneas de stdout | Líneas de stderr | Estado de salida | Resumen (`sha256sum`) |
|---|---|---|---|---|---|
| 1 | | | | | |
| 2 (mismo segundo que 1) | | | | | |
| 3 (tras esperar 2 s) | | | | | |

## 6. Observaciones

Responde cada punto con el comando que usaste, no con una impresión.

### 6.1 Comportamiento de salida

- ¿El programa llega a su propio final sin entrada? sí / no
- Evidencia:
- Cuánto tardó (`time`):

### 6.2 Varianza

- Dos causas de la variación en el conteo de líneas que puedo nombrar:
- Cuál domina, y cómo lo sé:

### 6.3 Reloj de la semilla

- Dos ejecuciones en el mismo segundo: idénticas / distintas
- Dos ejecuciones en segundos distintos: idénticas / distintas
- La línea que lo explica, citada:

### 6.4 Codificación

- Comando usado para inspeccionar los bytes:
- Los bytes que encontré para la pared, el caballo y el separador:
- Qué muestra en su lugar una terminal UTF-8:
- ¿Estos bytes son UTF-8 válido? sí / no

### 6.5 Pantalla frente a registro

- Cantidad de fallos de `cls` en `stderr`:
- Cantidad de turnos de la partida:
- ¿Coinciden? sí / no
- Cómo se vería la salida si `cls` funcionara:

## 7. Comparación — veredicto por predicción

El veredicto es exactamente uno de: confirmada, contradicha, sin resolver.

| # | Predicho | Observado | Veredicto |
|---|---|---|---|
| 1 | | | |
| 2 | | | |
| 3 | | | |
| 4 | | | |
| 5 | | | |

- Qué fila fue la sorpresa más útil, y por qué:
- Qué fila todavía no puedo decidir, y qué necesitaría observar:

## 8. Límite de la evidencia

Un párrafo, sin palabras de sintaxis: qué probó la compilación, qué no probó, dónde se detiene la
evidencia.

>

### 8.1 Las cuatro afirmaciones que la sonda no sostiene

Para cada una, escribe en una línea por qué esta ejecución no puede sostenerla.

| Afirmación | Por qué esta ejecución no puede sostenerla |
|---|---|
| El código es portable | |
| La salida es correcta | |
| La herramienta histórica era Visual C++ 6.0 | |
| Esto reproduce la transcripción moderna | |

### 8.2 La frase honesta

Escribe una frase que defenderías en una revisión de código.

>

## 9. Incertidumbres

| Pregunta abierta | Confianza | Qué evidencia me haría cambiar de opinión |
|---|---|---|
| | | |
| | | |
| | | |

## 10. Autoevaluación

- [ ] Escribí cinco predicciones antes de ejecutar nada.
- [ ] Cité los errores de la compilación ingenua literalmente, en orden, con números de línea.
- [ ] Enumeré cada cambio de compatibilidad con su riesgo.
- [ ] Ejecuté `build_probe.sh` y registré sus seis líneas.
- [ ] Registré `run_status` y sé qué habría significado `124`.
- [ ] Nombré dos causas de la variación en el conteo de líneas.
- [ ] Demostré que dos ejecuciones en el mismo segundo son idénticas, y que eso no es determinismo.
- [ ] Confirmé que el tablero son bytes CP437 crudos, no UTF-8.
- [ ] Comparé mi predicción con la observación, con un veredicto por fila.
- [ ] Escribí el párrafo del límite y las cuatro afirmaciones no sostenidas.
- [ ] El archivo está sin modificar (`git status` en el archivo está limpio).

## 11. Paso siguiente

- Qué puedo explicar ahora:
- Qué no puedo explicar todavía:
- Lección siguiente o repetición:
- El defecto que noté y decidí no perseguir:

## 12. Nota opcional

- Cualquier otra cosa:

---

> Recordatorio: este archivo es evidencia para ti, no una nota para nadie más.
