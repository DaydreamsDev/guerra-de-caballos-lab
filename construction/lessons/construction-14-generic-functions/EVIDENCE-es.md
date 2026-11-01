# construction-14 — Evidencia

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro del aprendiz. Escribe en español.

Este registro sigue los ocho destinos de movimiento desde una fórmula en línea hasta una plantilla
de función: qué acepta el generador, qué sigue nombrando el bucle, cuántas funciones produjo el
compilador y cuánto cuesta el diseño. Conserva las predicciones. La diferencia entre tu predicción y
la observación es el resultado, sea cero o no.

## 1. Preparación de la ejecución

- Fecha:
- Semilla: `1`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Línea base leída el (fecha):
- ¿Registro de `construction-13` a mano? (sí / no)
- El número que traje de `construction-13` (líneas de código / archivos / trazas):
- Directorio temporal que usé:

## 2. El generador (antes de ejecutar)

### 2.1 Encabezado y declaraciones

Rellena esto desde el código, con números de línea, no de memoria.

| Encabezado | Líneas | Declaraciones públicas | Datos privados | Clase completa | Tipo incompleto |
|---|---|---|---|---|---|
| | | | | | |

Recuentos:

- Encabezados:
- Unidades de traducción:
- Declaraciones públicas:
- Declaraciones de datos privados:
- Definiciones de clase completas:
- Tipos incompletos:
- Definiciones de función, total:
- Ayudas internas (`game.cpp`):
- Ayudas internas (`main.cpp`):
- Puntos de entrada del programa:

Comparado con `construction-13`:

- Encabezados: ______ → ______
- Unidades de traducción: ______ → ______
- Declaraciones públicas: ______ → ______
- Declaraciones de datos privados: ______ → ______
- Definiciones de clase completas: ______ → ______
- Definiciones de función: ______ → ______
- Líneas del encabezado nuevo:
- Líneas añadidas a `game.cpp`:
- Líneas eliminadas de `game.cpp`:
- Tramos del diff de `game.cpp`:
- Archivos idénticos byte a byte a `construction-13`:

### 2.2 Mapa de líneas

`move_generator.h`:

| Líneas | Contenido |
|---|---|
| | |

`game.cpp`, alrededor de `tryMove` y `playTurn`:

| Líneas | Contenido |
|---|---|
| | |

- Línea de la declaración de la plantilla:
- Línea de la llave de apertura del cuerpo:
- Línea de `piece.row()`:
- Línea de `piece.column()`:
- Línea del registro `MoveCandidate`:
- Línea de `moveCandidateCount`:
- Línea del primer arreglo de desplazamientos:
- Línea del segundo arreglo de desplazamientos:
- Línea que incluye `move_generator.h` en `game.cpp`:
- Línea que llama a `generate_move_candidates`:
- Línea que indexa `moveCandidates`:
- Línea que pasa el candidato a `tryMove`:
- Líneas de `tryMove`:
- Líneas de `playTurn`:
- Líneas de `run_game`:
- Líneas que construyen un `Knight` en `run_game`:

### 2.3 El contrato y el bucle

| Pregunta | Respuesta |
|---|---|
| El parámetro de tipo de la plantilla | |
| Operaciones que exige el cuerpo | |
| ¿Se exigen sobre un objeto `const`? | |
| ¿El encabezado incluye `knight.h`? | |
| ¿El encabezado menciona `Knight`? | |
| ¿Cuántas veces llama `game.cpp` al generador? | |
| ¿Cuántos lugares de llamada a accesores quedan en `game.cpp`? | |
| ¿Cuántas firmas de ayuda en `game.cpp` nombran `Knight`? | |
| ¿Qué recibe `tryMove` ahora? | |
| ¿Qué recibía `tryMove` en `construction-13`? | |
| ¿Dónde están los desplazamientos ahora? | |
| ¿Dónde estaban en `construction-13`? | |

- Número de lecturas de `row` en `game.cpp`:
- Número de lecturas de `column` en `game.cpp`:
- Número de llamadas a `set_position` en `game.cpp`:
- Número de lecturas de `role_name` en `game.cpp`:
- La suma de esos cuatro recuentos:
- Las funciones que las contienen:

## 3. Predicción (escrita antes de medir)

### 3.1 La tabla de candidatos

El caballo está en `(4, 5)`. El tipo de prueba está en `(1, 1)` a propósito: algunos candidatos
salen del tablero.

| Movimiento | `Knight` en (4, 5) | `ScoutingPawn` en (1, 1) | Confianza |
|---|---|---|---|
| 1 | | | |
| 2 | | | |
| 3 | | | |
| 4 | | | |
| 5 | | | |
| 6 | | | |
| 7 | | | |
| 8 | | | |

- Las filas que espero equivocar, y por qué:
- Qué espero que haga el generador con los destinos fuera del tablero:
- Dónde espero que ocurra el rechazo:

### 3.2 La prueba de cambio

| Pregunta | Mi predicción |
|---|---|
| Archivos que añadiré | |
| Archivos que editaré | |
| Líneas del encabezado nuevo | |
| Líneas que añadiré en `game.cpp` | |
| Líneas que quitaré en `game.cpp` | |
| Tramos del diff | |
| Funciones cuya firma cambia | |
| Líneas de lógica que cambiaré (no nombres) | |
| Semillas que coinciden, de 20, tras el cambio | |
| Conceptos nuevos que exige el cambio | |

- La predicción de la que estoy menos seguro, y por qué:

### 3.3 Instanciación y generación de código

| Pregunta | Mi predicción |
|---|---|
| Instanciaciones del generador en el juego | |
| Símbolos de `generate_move_candidates` en `game14.o`, con `-O2` | |
| El mismo recuento con `-O0` | |
| Texto de `game.o`: ¿crece, se reduce o igual? | |
| Llamadas indirectas en `game14.o` | |
| `sizeof(MoveCandidate)` | |
| `sizeof(std::array<MoveCandidate, 8>)` | |
| Candidatos calculados en la semilla 1 | |
| Pares de destino calculados por `construction-13` en la semilla 1 | |

- Qué espero que la plantilla cambie en el código objeto, en una frase:
- Qué espero que no cambie:

## 4. Ejecución — línea base y sonda de instanciación

- Comando de compilación de la línea base:
- Estado de salida de la línea base:
- Diff contra `../../../common/traces/seed-0001.txt`:
- Semillas que coincidieron, de 20, en la línea base sin modificar:
- Ruta del código de la sonda (solo temporal):
- Comando de compilación de la sonda:
- Estado de salida de la sonda:

Pega la salida de la sonda:

```text
<salida de la sonda>
```

| Observación | Predicho | Observado | Diferencia |
|---|---|---|---|
| Candidato 1 de `Knight` | | | |
| Candidato 4 de `Knight` | | | |
| Candidato 8 de `Knight` | | | |
| Candidato 1 de `ScoutingPawn` | | | |
| Candidato 4 de `ScoutingPawn` | | | |
| Candidato 8 de `ScoutingPawn` | | | |
| `sizeof(MoveCandidate)` | | | |
| `sizeof(array)` | | | |

- La línea que me sorprendió:
- Qué hice después en lugar de ajustar la predicción:

## 5. Comparación — qué demuestra la plantilla

- Salida de `grep -n 'Knight' code/move_generator.h`:
- Salida de `grep -n 'generate_move_candidates' code/game.cpp`:
- Número de llamadas al generador en `game.cpp`:
- Número de firmas de ayuda en `game.cpp` que nombran `Knight`:
- Cuáles están dentro del espacio de nombres anónimo:

| Líneas | Función | Llamadas a operaciones de `Knight` |
|---|---|---|
| | | |

- Total de lugares de llamada a accesores:
- Total de líneas que contienen un lugar de llamada a accesor:
- La operación que el generador exige pero no declara:
- Qué decide el generador:
- Qué decide `tryMove`:
- Qué demuestra la sonda sobre el generador:
- Qué no demuestra la sonda:

### 5.1 Las dos instanciaciones

- Comando que produjo los símbolos:
- Número de símbolos de `generate_move_candidates` con `-O0`:
- Los dos nombres desmangados:
- Número de símbolos con `-O2`:
- Por qué cambia el recuento:
- Qué significa `W` en la salida de `nm`:
- Si las dos instanciaciones comparten un objeto en tiempo de ejecución:

### 5.2 Generación de código

- Texto de `game13.o`:
- Texto de `game14.o`:
- Diferencia:
- Texto / datos enlazados de `construction-13`:
- Texto / datos enlazados de `construction-14`:
- Llamadas indirectas en `game13.o`:
- Llamadas indirectas en `game14.o`:
- Símbolos de `generate_move_candidates` en `game14.o` con `-O2`:
- Símbolos de `generate_move_candidates` en `game14.o` con `-O0`:
- Por qué la ausencia de símbolo no es una propiedad de las plantillas:
- Qué no demuestra la ausencia de tabla sobre la velocidad:

### 5.3 Modos de fallo

- Tipo sin `row()`: primera línea de error y mensaje:
- Tipo con `row()` no constante: primera línea de error y mensaje:
- Cuerpo de la plantilla en un `.cpp`: mensaje del enlazador:
- Por qué el cuerpo debe ser visible:
- Qué eliminaría un concepto del primer error:

## 6. La prueba de cambio en una copia temporal

- Ruta de la copia temporal:
- Archivos añadidos:
- Archivos editados:
- Líneas añadidas en `game.cpp`:
- Líneas eliminadas en `game.cpp`:
- Tramos del diff:
- Líneas de lógica cambiadas:
- Archivos versionados editados: (sí / no)

### 6.1 Añadir el generador

- Ruta del encabezado:
- Líneas del encabezado:
- El parámetro de tipo:
- Las dos operaciones que exige:
- El registro que devuelve:
- El tamaño de un registro:
- El tamaño del arreglo:
- Por qué el cuerpo está en el encabezado:

### 6.2 Quitar la geometría en línea

- Líneas eliminadas de `game.cpp`:
- Qué las reemplazó en `tryMove`:
- Qué ya no recibe `tryMove`:
- Qué construye `playTurn`:
- Dónde se indexa el candidato:
- La inclusión añadida:

### 6.3 Compilar y trazas

- Comando de compilación:
- Semillas que coincidieron, de 20:
- Diff contra la transcripción de la línea base sin modificar:

### 6.4 Recuentos

| Métrica | construction-13 | construction-14 | Diferencia |
|---|---|---|---|
| Archivos | | | |
| Líneas de código | | | |
| Líneas de `game.cpp` | | | |
| Declaraciones públicas | | | |
| Clases completas | | | |
| Definiciones de función | | | |
| Unidades de traducción | | | |
| Texto de `game.o` | | | |
| Texto / datos del binario | | | |
| Llamadas indirectas | | | |
| Trazas | | | |

### 6.5 El hueco: solo el generador es genérico

- El segundo tipo de pieza que acepta el generador:
- Las funciones que siguen nombrando `Knight`:
- Opción 1 (duplicar el bucle): funciones que se duplicarían:
- Opción 2 (restaurar una interfaz): qué lección es:
- Opción 3 (parametrizar el bucle): qué lección es:
- La frase de aceptación precisa de esta lección:
- Qué no afirma la aceptación:

## 7. El generador y sus límites

| Pregunta | Respuesta |
|---|---|
| ¿Qué acepta el generador? | |
| ¿Qué exige? | |
| ¿Qué devuelve? | |
| ¿Qué no decide? | |
| ¿Dónde viven los desplazamientos? | |
| ¿Cuántas instanciaciones crea el juego? | |
| ¿Qué sigue nombrando el bucle? | |
| ¿Cuántas firmas lo nombran? | |
| ¿Qué cuesta un segundo tipo en el generador? | |
| ¿Qué cuesta un segundo tipo en el bucle? | |
| ¿Qué cuesta el arreglo por turno? | |
| ¿Qué cuesta el cambio en el código? | |
| ¿Qué cuesta en el código objeto? | |
| ¿Puede el bucle mover un segundo tipo sin ediciones? | |

- Qué quita la plantilla de `tryMove`:
- Qué mueve a `move_generator.h`:
- Qué no quita:
- Qué lección posterior parametriza el bucle:
- Qué lección posterior posee el ciclo de vida:

## 8. Explicar

Una frase, sin usar las palabras *plantilla*, *genérico*, *parámetro de tipo*, *instanciación*,
*compilador*, *mejor*, *peor*, *limpio*, *flexible*, *reutilizable* ni *elegante*:

>

Un párrafo:

- Qué fija el generador:
- Qué lo impone:
- Qué lo diferencia de `construction-13`:
- Qué hacen los 17 lugares de llamada a accesores:
- Qué prueban las 20 trazas:
- Qué no prueban las trazas:
- Qué cuesta la prueba de cambio:
- Qué no cubre la plantilla:
- Qué ahorra:
- Qué cuesta:
- Qué lección posterior se apoya en esta:

Las cuatro cosas que mantuve separadas:

## 9. Comprobación de aceptación

- Localicé la plantilla y nombré su parámetro de tipo: (sí / no)
- Comprobé que el encabezado no incluye `knight.h` ni menciona `Knight`: (sí / no)
- Conté los 17 lugares de llamada a accesores y la única llamada al generador: (sí / no)
- Nombré las dos operaciones que exige el generador: (sí / no)
- Predije la tabla de candidatos y observé ambas columnas: (sí / no)
- Reproduje las 20 trazas en la línea base sin modificar: (sí / no)
- Compilé la sonda y vi los mismos ocho destinos para ambos tipos: (sí / no)
- Vi los dos símbolos débiles con `-O0`: (sí / no)
- Medí `sizeof(MoveCandidate)` y el arreglo: (sí / no)
- Reproduje los errores de operación faltante y de acceso no constante: (sí / no)
- Reproduje la referencia indefinida de una definición separada: (sí / no)
- Añadí el encabezado y edité `game.cpp` en una copia temporal: (sí / no)
- Obtuve 10 líneas añadidas y 8 eliminadas y cero cambios de regla: (sí / no)
- Obtuve 20 de 20 trazas tras la refactorización: (sí / no)
- Medí los recuentos de candidatos frente a los pares de destino: (sí / no)
- Nombré el límite de este diseño: el bucle sigue nombrando `Knight`: (sí / no)
- No edité ningún archivo de código versionado: (sí / no)

Si edité un archivo de código versionado, qué y por qué:

## 10. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué todavía no puedo explicar:
- Una cosa que volvería a leer:
- El número que llevaré a `construction-15`:
- La fila de candidatos que más me sorprendió:
- Qué decide el generador que yo esperaba que decidiera el bucle:
- Qué sigue decidiendo el bucle que yo esperaba que decidiera el generador:
- Qué compra la plantilla que la fórmula en línea no podía ofrecer:
- Qué cede:
- Qué demostró la prueba de cambio que las trazas no podían:
- El cambio que haría después para volver a probar este diseño:
- Siguiente paso:

## 11. Si falta una línea base o no es fiel

Rellena esto solo si faltó un archivo, si una compilación falló por una razón del entorno,
si un diff no quedó vacío o si la sonda contradijo el comportamiento documentado de una forma
que no pudiste explicar.

- Archivo faltante, error de compilación, diff o contradicción:
- Salida exacta del comando:
- Qué hice en lugar de inventar un recuento:
- Cuándo volveré a intentarlo:

## 12. Autocomprobación

- [ ] Inventarié cada encabezado y cada definición con su archivo.
- [ ] Localicé la plantilla, el registro y los desplazamientos por línea.
- [ ] Comparé la fórmula en línea con el generador y nombré qué posee cada uno.
- [ ] Predije la tabla de candidatos, la prueba de cambio y la sonda antes de medir.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Compilé la sonda y vi los mismos ocho destinos para ambos tipos.
- [ ] Confirmé que el generador produce candidatos fuera del tablero.
- [ ] Vi las dos instanciaciones por nombre con `-O0`.
- [ ] Medí el registro, el arreglo y el crecimiento del código objeto.
- [ ] Reproduje los fallos de operación faltante, acceso no constante y definición separada.
- [ ] Moví la geometría al encabezado en una copia temporal.
- [ ] Obtuve 20 de 20 trazas tras la refactorización y cambié cero reglas.
- [ ] Conté los candidatos frente a los pares de destino.
- [ ] Nombré el límite de este diseño: solo el generador es genérico.
- [ ] Separé qué acepta el generador, qué nombra el bucle, qué emite el compilador y qué puede
      ver una ejecución.
- [ ] Mi frase de explicación evita las palabras de mecanismo y las palabras de veredicto.
- [ ] No edité ningún archivo de código versionado.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §10.
