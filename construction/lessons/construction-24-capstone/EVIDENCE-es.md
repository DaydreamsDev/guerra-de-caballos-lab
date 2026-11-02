# construction-24 — Evidencia

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro del aprendiz. Escribe en español.

Este registro sigue un cambio a través de dos escalones. Mantiene separadas cuatro cosas: la
predicción, la ruta, la medición y el veredicto. Conserva las predicciones. La diferencia entre tu
predicción y la observación es el resultado, sea cero o no.

## 1. Preparación de la ejecución

- Fecha:
- Raíz del repositorio:
- Commit copiado, o fecha si no hay commit (`git rev-parse HEAD`):
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Indicadores de optimización:
- Plataforma y arquitectura:
- Par elegido (A y B):
- Directorio de trabajo de A:
- Directorio de trabajo de B:
- Presupuesto de tiempo para el cambio:
- ¿Registro de construction-12 a mano? (sí / no)
- ¿Registro de construction-15 a mano? (sí / no)
- ¿Edité algún archivo versionado? (sí / no)

## 2. El par y las líneas base

### 2.1 Por qué este par

Completa la frase: "Elegí ______ y ______ porque mantienen constante ______ y varían ______".

- La variable que afirmo aislar:
- Las variables que no mantengo constantes, y por qué es aceptable:
- Si comparé 12 y 15, las tres variables que cambian a la vez:
- El par que rechacé, y por qué:

El cambio, escrito antes de cualquier edición:

- Nombre de la tercera pieza:
- Operaciones de la tercera pieza:
- Movimiento de la tercera pieza:
- Cómo demostraré que viaja por el mismo bucle:
- Cómo aporta dimensiones el punto de entrada de 10×10:
- El punto de entrada por defecto de 8×8:
- Qué no debe moverse:

### 2.2 Medidas de la línea base

Medidas antes de la primera edición, con `./measure.sh`:

| Escalón | Directorio de lección | Salida de `measure.sh` | Archivos | Líneas | Trazas |
|---|---|---|---:|---:|---|
| A | | | | | |
| B | | | | | |

- Bytes de texto del binario por defecto, A:
- Bytes de texto del binario por defecto, B:
- Tiempo de compilación, mediana de 3, A:
- Tiempo de compilación, mediana de 3, B:
- ¿Borré `build/` de ambas copias antes de medir? (sí / no)

### 2.3 Los dos diseños antes del cambio

| Pregunta | Escalón A | Escalón B |
|---|---|---|
| Cómo nombra el bucle al tipo de pieza | | |
| Dónde vive la geometría del movimiento | | |
| Cómo guarda el tablero sus dimensiones | | |
| Bytes de almacenamiento del tablero | | |
| Dónde está escrita la constante de 8×8 | | |
| Qué archivos de construcción listan las fuentes | | |
| ¿`run.sh` reconstruye al cambiar un encabezado? | | |
| Qué cuesta una tercera pieza en este diseño, en una frase | | |

## 3. Predicción (escrita antes de medir)

### 3.1 La predicción de coste

| Métrica | Escalón A | Escalón B | Confianza |
|---|---:|---:|---|
| Archivos tocados | | | |
| Líneas añadidas | | | |
| Líneas eliminadas | | | |
| Conceptos nuevos necesarios | | | |
| Trazas rotas en la primera construcción verde | | | |
| Tiempo de compilación, mediana de 3 (segundos) | | | |
| Bytes de texto del binario | | | |
| Trazas de referencia superadas al final | | | |

- La fila que marqué `alta` y aun así espero fallar:
- La fila que marqué `baja` y por qué:
- El número que espero que decida el veredicto:
- El número que espero que esté más cerca de invertirlo:

### 3.2 La predicción de ruta

| Pregunta | Mi predicción |
|---|---|
| Ruta de la tercera pieza en A | |
| Ruta de la tercera pieza en B | |
| Ruta de 10×10 en A | |
| Ruta de 10×10 en B | |
| Archivos que espero tocar en A | |
| Archivos que espero tocar en B | |
| Qué mitad domina en A: ¿pieza o tablero? | |
| Qué mitad domina en B: ¿pieza o tablero? | |
| Archivos de construcción que espero editar | |

### 3.3 La predicción del veredicto

| Pregunta | Mi predicción |
|---|---|
| Qué escalón espero que sea más barato | |
| El número que espero que lo decida | |
| El número que podría invertirlo | |
| El coste que espero que la tabla no muestre | |
| La predicción de la que estoy menos seguro | |
| Qué espero que me sorprenda | |

- La condición en la que se invierte mi veredicto:
- Por qué no escribo "depende":

## 4. El cambio aplicado

### 4.1 Registro de ruta del escalón A

| Punto de control | Comando | Resultado | Archivos tocados hasta ahora | Trazas |
|---|---|---|---|---|
| Línea base | `./measure.sh` | | 0 | |
| A1 la tercera pieza compila | | | | |
| A2 trazas tras la pieza | | | | |
| A3 10×10 ejecuta | | | | |
| A4 trazas tras el tablero | | | | |

- Tiempo hasta la primera construcción verde, A:
- Número de intentos de construcción antes de la primera verde, A:
- Cambios de ruta en A:
- Qué dijo la primera construcción fallida:

### 4.2 Registro de ruta del escalón B

| Punto de control | Comando | Resultado | Archivos tocados hasta ahora | Trazas |
|---|---|---|---|---|
| Línea base | `./measure.sh` | | 0 | |
| B1 la tercera pieza compila | | | | |
| B2 trazas tras la pieza | | | | |
| B3 10×10 ejecuta | | | | |
| B4 trazas tras el tablero | | | | |

- Tiempo hasta la primera construcción verde, B:
- Número de intentos de construcción antes de la primera verde, B:
- Cambios de ruta en B:
- Qué dijo la primera construcción fallida:
- Si B no llegó a una construcción verde: el último error y la ruta que intenté:

### 4.3 La tercera pieza

- Nombre del tipo en A:
- Nombre del tipo en B:
- Las cuatro operaciones, y dónde se declara cada una:
- Cómo se construye:
- Cómo demostré que viaja por el mismo bucle:
- ¿La transcripción de 8×8 quedó idéntica byte a byte? (sí / no)
- ¿Mantuve la sustitución o restauré el original?
- Qué necesitó el archivo de construcción:
- Qué emitió el compilador para el tipo nuevo (`nm -C`, una línea):

### 4.4 El punto de entrada de 10×10

- Archivo en A:
- Archivo en B:
- Cómo llegan las dimensiones al tablero:
- Cómo se reutiliza el mismo bucle de turnos:
- Semilla 1, A, cabeza y cola:

```text
<A: primeras seis líneas y últimas dos>
```

- Semilla 1, B, cabeza y cola:

```text
<B: primeras seis líneas y últimas dos>
```

- ¿Las coordenadas iniciales están en 1..10? (sí / no)
- ¿El formato coincide con `transcript-format.md`? (sí / no)
- ¿Hay traza de referencia para 10×10? (sí / no)

### 4.5 Qué no cambió

- Las 20 trazas en el camino de 8×8, A:
- Las 20 trazas en el camino de 8×8, B:
- El formato de transcripción:
- Las reglas que no toqué:
- Los archivos versionados que no toqué:

## 5. Medición

### 5.1 Archivos y líneas

Comando usado:

```bash
<el comando numstat>
```

Salida cruda de A:

```text
<salida cruda de A>
```

Salida cruda de B:

```text
<salida cruda de B>
```

| Métrica | A | B |
|---|---:|---:|
| Archivos de código editados | | |
| Archivos de código añadidos | | |
| Archivos de construcción editados | | |
| Archivos tocados, total | | |
| Líneas añadidas | | |
| Líneas eliminadas | | |

- ¿Los totales de `--numstat` incluyen comentarios y líneas en blanco? (sí / no)
- ¿`measure.sh` cuenta el punto de entrada de 10×10? (sí / no)
- Por qué el tamaño de la copia no es lo mismo que el delta que causé:

### 5.2 Conceptos nuevos

Enumera cada concepto, el escalón que lo necesitó y la línea que lo usa. Un recuento sin lista no se
acepta.

| Concepto | A o B | Archivo y línea | Por qué es nuevo para este escalón |
|---|---|---|---|
| | | | |
| | | | |
| | | | |

- Recuento de A:
- Recuento de B:
- El concepto que tardé más en entender:
- El concepto que esperaba y no necesité:

### 5.3 Pruebas rotas y trazas superadas

- Primera construcción verde, A: semillas fallidas, de 20:
- Primera construcción verde, B: semillas fallidas, de 20:
- Recuento final, A: superadas, de 20:
- Recuento final, B: superadas, de 20:
- La primera semilla fallida en A, y la primera línea distinta:
- La primera semilla fallida en B, y la primera línea distinta:
- Qué me dijo el fallo sobre qué camino se movió:
- El comando que produjo el recuento final:

### 5.4 Tiempo de compilación

- Compilador y versión:
- Indicadores:
- Lista de fuentes usada (solo el programa por defecto):
- A, ejecución 1 / 2 / 3:
- A, mediana:
- B, ejecución 1 / 2 / 3:
- B, mediana:
- ¿Ambos lados usaron la misma ruta de construcción? (sí / no)
- Si no, qué incluye la diferencia:
- Por qué el tiempo de compilación es una señal débil en un programa de este tamaño:

### 5.5 Texto del binario

Comando usado:

```bash
<el comando size>
```

- A, texto / datos:
- B, texto / datos:
- Dirección del cambio, y por qué:
- Qué binario medí (el de por defecto, no el de 10×10):
- Qué no incluye el número:

### 5.6 Predicción frente a observación

| Métrica | A predicho | A observado | Brecha | B predicho | B observado | Brecha |
|---|---:|---:|---:|---:|---:|---:|
| Archivos tocados | | | | | | |
| Líneas añadidas | | | | | | |
| Líneas eliminadas | | | | | | |
| Conceptos nuevos | | | | | | |
| Pruebas rotas | | | | | | |
| Tiempo de compilación | | | | | | |
| Texto del binario | | | | | | |
| Trazas de referencia | | | | | | |

- La brecha más grande, y su causa mecánica:
- La brecha que no esperaba:
- Qué hice después en vez de ajustar la predicción:

## 6. La comparación

| Métrica | Escalón A | Escalón B |
|---|---:|---:|
| Archivos tocados | | |
| Líneas añadidas | | |
| Líneas eliminadas | | |
| Conceptos nuevos necesarios | | |
| Pruebas rotas | | |
| Tiempo de compilación (segundos) | | |
| Bytes de texto del binario | | |
| Trazas de referencia superadas | | |

### 6.1 Qué mitad dominó

- En A, la pieza costó:
- En A, el tablero costó:
- En B, la pieza costó:
- En B, el tablero costó:
- La mitad que decidió la comparación:
- La mitad que no la decidió, y por qué sigue perteneciendo a la tabla:

### 6.2 Qué métrica decidió

- La métrica decisiva:
- Su valor en A y en B:
- Por qué esta métrica y no las líneas de código:
- La métrica que estuvo más cerca de invertir el resultado:
- Cuánto se acercó:

### 6.3 Qué no muestra la tabla

- Tiempo hasta la primera construcción verde, A y B:
- Intentos de construcción, A y B:
- Cambios de ruta:
- Carga conceptual:
- Riesgo que una persona revisora tendría que comprobar a mano:
- Qué no medí:
- Cuál de estos cambió mi confianza:

## 7. Veredicto y defensa

- Veredicto, una frase, con el número decisivo y el alcance:
- El número que lo decidió:
- El número que casi lo invirtió:
- Qué invertiría el veredicto, como próximo cambio concreto:
- Qué escalón elegiría para ese próximo cambio, y por qué:

### 7.1 Las tres preguntas de defensa

1. ¿Por qué la comparación es justa?
2. ¿Qué invertiría el veredicto?
3. ¿Qué no mediste?

### 7.2 El próximo cambio, predicho

| Métrica | Cambio actual, ganador | Próximo cambio, predicho |
|---|---:|---:|
| Archivos tocados | | |
| Líneas añadidas | | |
| Líneas eliminadas | | |
| Conceptos nuevos necesarios | | |
| Pruebas rotas | | |
| Trazas de referencia superadas | | |

- El próximo cambio:
- Por qué es la prueba correcta de este veredicto:
- Qué resultado falsificaría mi veredicto:

## 8. Explicar

Una frase, sin las palabras *mejor*, *peor*, *más limpio*, *más flexible*, *más reutilizable*, *más
elegante*, *moderno* ni *correcto*:

>

Un párrafo:

- Cuál fue el cambio:
- Qué mantuvo constante el par:
- Qué varió:
- Cuál fue el número decisivo:
- Qué demuestran las 20 trazas:
- Qué no demuestran las trazas:
- Qué demuestra la ejecución de 10×10:
- Qué demuestra la tercera pieza:
- Qué no muestra la tabla:
- Qué elegiría después, y por qué:

Las cuatro cosas que mantuve separadas:

## 9. Comprobación de aceptación

- Elegí un par y terminé la frase "mantiene constante / varía": (sí / no)
- Copié dos escalones a directorios de trabajo y borré `build/`: (sí / no)
- Registré el commit y las dos salidas de `measure.sh` de la línea base: (sí / no)
- Escribí la predicción, con confianza, antes de la primera edición: (sí / no)
- Apliqué la misma tercera pieza y el mismo punto de entrada de 10×10 a ambos: (sí / no)
- La tercera pieza viaja por el mismo bucle de turnos: (sí / no)
- El punto de entrada de 10×10 ejecuta e imprime el formato de transcripción: (sí / no)
- Ambas copias pasan 20/20 en el camino de 8×8: (sí / no)
- Medí archivos y líneas con `--numstat`, incluidos los archivos de construcción: (sí / no)
- Medí el tiempo de compilación como mediana de tres construcciones limpias: (sí / no)
- Registré los bytes de texto del binario del programa por defecto: (sí / no)
- Registré las pruebas rotas por separado del 20/20 final: (sí / no)
- El informe tiene sus seis secciones y cabe en una página: (sí / no)
- El veredicto nombra el número decisivo y el casi inverso: (sí / no)
- El próximo cambio está nombrado y predicho en la misma tabla: (sí / no)
- No edité ningún archivo versionado: (sí / no)

Si edité un archivo versionado, qué y por qué:

## 10. Reflexión y próximo paso

- Qué puedo explicar ahora sin notas:
- Qué no puedo explicar todavía:
- Una cosa que volvería a leer:
- El número que llevaré a la próxima lección:
- La predicción que estuvo más lejos de la observación:
- Qué me enseñó el par que un solo escalón no podía:
- Qué mediría de otra forma la próxima vez:
- El cambio que haría después, y su coste predicho:
- Próximo paso:

## 11. Si algo falta o no es fiel

Rellena esto solo si faltó un escalón, una construcción falló por una razón del entorno, un diff no
quedó vacío, una traza falló de una forma que no pudiste explicar, o una medición contradijo el
comportamiento documentado.

- Archivo faltante, error de construcción, diff o contradicción:
- Salida exacta del comando:
- Qué hice en vez de inventar un recuento:
- Cuándo volveré a intentarlo:

## 12. Autocomprobación

- [ ] Elegí un par cuya diferencia puedo atribuir a una variable.
- [ ] Copié dos directorios completos de lección y borré `build/`.
- [ ] Medí ambas líneas base antes de editar.
- [ ] Escribí las predicciones de coste, ruta y veredicto antes de la primera edición.
- [ ] Apliqué el mismo cambio a ambas copias.
- [ ] Registré la ruta, no solo el resultado.
- [ ] Demostré que la tercera pieza viaja por el mismo bucle.
- [ ] Ejecuté el punto de entrada de 10×10 y comprobé el formato.
- [ ] Confirmé 20/20 en el camino de 8×8 en ambas copias.
- [ ] Medí archivos, líneas, conceptos, pruebas rotas, tiempo de compilación y bytes de texto.
- [ ] Mantuve "pruebas rotas" y "trazas superadas" como dos números.
- [ ] Escribí el informe de una página con sus seis secciones.
- [ ] El veredicto nombra el número decisivo y el casi inverso.
- [ ] El próximo cambio está predicho en la misma tabla.
- [ ] Separé la predicción, la ruta, la medición y el veredicto.
- [ ] Mi explicación de una frase evita las palabras de veredicto.
- [ ] No edité ningún archivo de código versionado.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §10.
