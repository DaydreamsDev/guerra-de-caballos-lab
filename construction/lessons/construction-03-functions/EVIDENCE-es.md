# construction-03 — Evidencia

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro propio de quien aprende. Escribe en español.

Este registro compara dos ámbitos del mismo juego. Conserva las dos columnas. Un número de un solo
archivo no responde a la pregunta, y un empate es un resultado, no una diferencia que falte.

## 1. Preparación de la ejecución

- Fecha:
- Semilla: `1`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Línea base con funciones leída el (fecha):
- Línea base estructurada leída el (fecha):
- ¿Tengo el registro de construction-02 a mano? (sí / no)
- Sitios de edición y aristas entrantes que conté en construction-02:

## 2. Inventario de ámbitos (antes de la ejecución)

### 2.1 Nombres mutables en el ámbito de espacio de nombres

Complétalo desde el archivo objeto y el código, no de memoria. Una fila por nombre.

| Nombre | Línea | Ámbito en construction-02 | Dónde vive en construction-03 | Cómo llega ahí |
|---|---|---|---|---|
| | | | | |

Recuentos:

- Nombres mutables en el ámbito de espacio de nombres, construction-02:
- Nombres mutables en el ámbito de espacio de nombres, construction-03:
- Nombres iguales antes y después:
- Nombres que pasaron a ser locales de `main`:
- Nombres que pasaron a ser locales de otra función (nombra la función):
- Nombres que pasaron a ser parámetros por valor:
- Nombres que pasaron a ser parámetros por referencia:
- Nombres que desaparecieron:

### 2.2 Firmas

Una fila por función que lee o escribe estado del juego.

| Función | Línea | Parámetros | Retorno | ¿Lee una global? |
|---|---|---|---|---|
| | | | | |

Recuentos:

- Definiciones de función, construction-02:
- Definiciones de función, construction-03:
- Parámetros de `tryMove`:
- Parámetros de `tryMove` que son de salida:
- Funciones que dependen de `randomState` sin nombrarla en su lista de parámetros:
- Función cuya línea de definición responde completa a «¿de qué depende esto?»:

### 2.3 Lado a lado

| Pregunta | construction-02 | construction-03 |
|---|---|---|
| Dónde vive el contador de movimientos | | |
| Cómo aprende una función cuál es el caballo activo | | |
| Cómo aprende una función el tablero de visitadas | | |
| Qué enuncia el sitio de llamada sobre las dependencias | | |
| Dónde se declararía un estado nuevo del turno | | |
| Qué se puede probar antes de que empiece el juego | | |

## 3. Predicción (escrita antes de medir)

### 3.1 Una llamada

La llamada que hay que predecir:
`tryMove(2, 1, 8, 8, 8, visitedCells, destinationRow, destinationColumn)`, con solo la casilla
(3,3) marcada y el otro caballo en (8,8).

- `MoveOutcome` que espero:
- Fila y columna de destino que espero:
- Por qué (el orden de comprobaciones que supuse):
- Confianza, de 0 a 100 %:

### 3.2 Coste de R-SKIP

R-SKIP, con mis propias palabras:

- Cuándo se salta el turno:
- Qué registra igualmente el turno:
- Dónde va la comprobación respecto a la cabecera:
- Dónde debe vivir el contador de turnos iniciados, y por qué no un nivel arriba ni abajo:

Mis números comprometidos:

| Métrica | construction-02 | construction-03 | Confianza | Razón |
|---|---|---|---|---|
| Sitios de edición | | | | |
| Cambios de firma | | | | |
| Sitios de llamada cambiados | | | | |
| Llamadores de `playTurn` cambiados | | | | |
| Llamadores de `playGame` cambiados | | | | |
| Globales nuevas | | | | |
| Aristas nuevas de flujo de control | | | | |
| Archivos tocados | | | | |
| Trazas de referencia rotas (de 20) | | | | |

La hipótesis de ámbito, en una frase (antes de medir):

>

Qué espero que empate:

Qué espero que difiera:

## 4. Ejecución — las dos líneas base

- Comando de compilación, funciones:
- Comando de compilación, estructurada:
- Estados de salida:
- Destino del diff: `../../../common/traces/seed-0001.txt`

Registra `vacío` o `diff` en cada celda de resultado.

| Comprobación | Comando | Resultado |
|---|---|---|
| Funciones contra la traza | | |
| Estructurada contra la traza | | |
| Funciones contra estructurada | | |

```text
<salida del diff, o «sin diferencias»>
```

Si algún diff no está vacío, detente aquí y completa §10.

## 5. La prueba de la decisión de movimiento

- Directorio de trabajo:
- Código de la prueba (copia de la línea base con funciones):
- La única edición a la copia (renombrar `main`):
- No edité la línea base versionada: (sí / no)

Comando:

```bash
<compilar y ejecutar>
```

Salida:

```text
<salida>
```

- Resultado previsto (de §3.1):
- Resultado observado:
- ¿Predicción correcta? (sí / no). Si no, qué comprobación me salté:

Los cuatro casos, con el tablero que solo contiene (3,3):

| Activo | Movimiento | Otro | Esperado | Observado |
|---|---|---|---|---|
| 2,1 | 8 | 8,8 | `visited` | |
| 2,1 | 1 | 8,8 | `legal` | |
| 2,1 | 8 | 3,3 | `capture` | |
| 1,1 | 2 | 8,8 | `outside` | |

Qué prueba la prueba:

- Estado del juego que inicializa la prueba (enumera cada valor que asigna):
- Funciones que la prueba nunca llama:
- Por qué la llamada es posible aquí y no en `construction-02`:

Qué no prueba la prueba:

- Qué sigue sin enunciar la firma sobre `visitedCells`:
- Qué muestra la fila de «fuera» sobre los parámetros de salida:
- Si llamara a `playTurn` en su lugar, qué tendría que asignar primero:

## 6. Aplicar R-SKIP en una copia de trabajo

- Copia de trabajo:
- Dueño del contador (función):
- Por qué ese ámbito:
- No edité la línea base versionada: (sí / no)

### 6.1 Colocación

| Dónde podría vivir el contador | ¿Sobrevive a una llamada? | ¿Visible sin parámetro? | ¿Elegido? |
|---|---|---|---|
| local en `playTurn` | | | |
| `static` en `playTurn` | | | |
| local en `playGame`, por referencia | | | |
| local en `main`, por referencia | | | |

### 6.2 Sitios de edición, enumerados

| Sitio | Línea | Qué cambia | ¿Firma o sitio de llamada? |
|---|---|---|---|
| | | | |

- Total de sitios de edición:
- Cambios de firma:
- Sitios de llamada cambiados:
- Líneas añadidas / eliminadas:

### 6.3 Llamadores

| Función | Línea de definición | Sitios de llamada | Llamadores | Llamadores que cambian |
|---|---|---|---|---|
| `playTurn` | | | | |
| `playGame` | | | | |

- Por qué el dueño del contador decide si `main` cambia:
- La comprobación del compilador que compra la firma extra:

### 6.4 Tabla de coste

| Métrica | construction-02 | construction-03 | Diferencia | Qué mide |
|---|---|---|---|---|
| Sitios de edición | | | | tecleo |
| Cambios de firma | | | | acoplamiento visible |
| Sitios de llamada cambiados | | | | alcance en el grafo de llamadas |
| Globales nuevas | | | | estado escondido |
| Aristas nuevas de flujo de control | | | | flujo añadido |
| Archivos tocados | | | | acoplamiento |
| Conceptos nuevos de lenguaje | | | | coste de aprendizaje |
| Trazas de referencia rotas por R-SKIP (de 20) | | | | cambio de contrato |
| Trazas reproducidas sin modificar (semillas comprobadas) | | | | contrato |
| Transcripciones modificadas que coinciden entre sí | | | | fidelidad |

### 6.5 Ejecutar y comparar

```bash
<comandos>
```

| Comprobación | Resultado |
|---|---|
| Funciones modificada contra la traza | |
| Funciones modificada contra estructurada modificada (si la conservé) | |
| Semillas que difieren de la traza (de 20) | |

- Primera divergencia respecto a la traza (línea y texto):
- Por qué las dos versiones modificadas deben coincidir:
- Primera divergencia que esperaba, y si coincidió:

### 6.6 Respuestas

- ¿Cuántos llamadores tiene `playTurn`? ¿Cuántos cambian?
- ¿Cuántos llamadores tiene `playGame`? ¿Cuántos cambian?
- ¿Qué recuento empata con `construction-02` y cuál no?
- ¿Qué compró la firma extra, en términos del compilador?
- ¿Dónde podría filtrarse estado nuevo del turno en cada archivo, y cómo lo notaría un lector?
- Predicción más cercana, y por qué:
- Predicción más lejana, y qué no vi:

## 7. Explicación

Una frase, sin *mejor*, *peor*, *limpio*, *más limpio*, *bonito*, *elegante*, *feo* ni *modular*:

>

Un párrafo:

- Qué nombres puede leer `tryMove` que su llamador no le haya pasado:
- Qué nombres puede leer `playTurn` que su llamador no le haya pasado:
- Qué le hace esa diferencia a la prueba:
- Qué debe sostener un lector en la memoria de trabajo para conocer las entradas de una función en
  cada archivo:
- Dónde debe declararse un estado nuevo del turno y qué obliga a pasarlo:
- Qué sigue sin decir la firma, y qué lección posterior lo arregla:

## 8. Criterio de aceptación

- Requisito reformulado sin añadir ningún sorteo:
- Regla de conteo usada:
- La decisión de movimiento que ejercité:
- Los valores locales que usó la prueba:
- Estado global del juego que necesitó la prueba: (ninguno / enuméralo)
- Sitios de edición contados (construction-02 / construction-03):
- Llamadores de `playTurn` contados:
- Qué recuento empata:
- Qué hizo visible la lista de parámetros:
- Cuánto cobró por ello:
- La explicación evita las ocho palabras prohibidas: (sí / no)
- No edité las líneas base versionadas: (sí / no)

Si cambié una línea base versionada, qué y por qué:

## 9. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué no puedo explicar todavía:
- Una cosa que volvería a leer:
- El número que llevaré a `construction-04`:
- El resultado que hace fácil de probar `tryMove`, y el que no:
- Siguiente paso:

## 10. Si falta una línea base o no es fiel

Completa esto solo si faltaba un archivo, falló una compilación o un diff no estaba vacío.

- Archivo ausente, error de compilación o diff:
- Salida exacta de los comandos:
- Qué hice en lugar de inventar un recuento:
- Cuándo volveré a intentarlo:

## 11. Autoverificación

- [ ] Inventarié los dos ámbitos desde los archivos objeto y el código.
- [ ] Escribí la predicción de la prueba y la del coste antes de medir.
- [ ] Compilé las dos líneas base y obtuve tres diffs vacíos.
- [ ] La prueba llama al `tryMove` versionado y no inicializa estado del juego.
- [ ] Registré el comando de la prueba y su salida exacta.
- [ ] Apliqué R-SKIP con el contador en el ámbito dueño más estrecho.
- [ ] Conté los llamadores con `grep`, no por suposición.
- [ ] Separé el coste de edición de lo que la firma hace visible.
- [ ] Mi explicación de una frase evita las ocho palabras prohibidas.
- [ ] No edité las líneas base versionadas.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §9.
