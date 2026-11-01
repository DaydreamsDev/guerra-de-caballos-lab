# construction-02 — Evidencia

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro propio de quien aprende. Escribe en español.

Este registro compara dos formas del mismo juego. Conserva las dos columnas. Un número de una sola
forma no responde a la pregunta, y un empate es un resultado, no una diferencia que falte.

## 1. Preparación de la ejecución

- Fecha:
- Semilla: `1`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Línea base estructurada leída el (fecha):
- Línea base no estructurada leída el (fecha):
- ¿Tengo el registro de construction-01 a mano? (sí / no)
- Saltos recableados que conté en construction-01:

## 2. Inventario de flujo de control (antes de la ejecución)

### 2.1 Línea base no estructurada

Complétalo desde el archivo, no de memoria. Una fila por etiqueta.

| Etiqueta | Línea | Aristas entrantes (línea y tipo) | ¿Entrada de turno? |
|---|---|---|---|
| | | | |

Recuentos:

- Etiquetas:
- Saltos:
- Entrada de turno:
- Región de inicio de turno (rango de líneas):
- Aristas entrantes a la entrada de turno, enumeradas:
- Asignaciones de `playerIsActive` (líneas):
- Incrementos de `successfulMoveCount` (líneas):

### 2.2 Línea base estructurada

Una fila por bucle y por transferencia.

| Construcción | Línea | Región que la contiene | ¿Inicia un turno? |
|---|---|---|---|
| | | | |

Recuentos:

- Bucles:
- Sitios con `continue`:
- Sitios con `break`:
- Región de inicio de turno (rango de líneas):
- Aristas entrantes a la entrada de turno, enumeradas:
- Asignaciones de `playerIsActive` (líneas):
- Incrementos de `successfulMoveCount` (líneas):

### 2.3 Lado a lado

| Pregunta | No estructurada | Estructurada |
|---|---|---|
| La frontera del turno se enuncia como | | |
| Región de inicio de turno | | |
| Aristas entrantes a la entrada de turno | | |
| Cómo empieza el siguiente turno | | |
| Etiquetas disponibles para el cambio | | |
| Dónde se invierte el lado activo | | |

## 3. Predicción (escrita antes de medir)

R-SKIP, con mis propias palabras:

- Cuándo se salta el turno:
- Qué registra igualmente el turno:
- Qué número imprime un turno saltado en `turn=`, y si es aceptable:
- Qué no cuenta el turno saltado:
- Por qué esto no necesita ningún sorteo nuevo:
- Dónde va la comprobación respecto a la cabecera:

Mis números comprometidos:

| Métrica | No estructurada | Estructurada | Confianza | Razón |
|---|---|---|---|---|
| Sitios de edición | | | | |
| Aristas entrantes a la entrada de turno | | | | |
| Aristas nuevas de flujo de control | | | | |
| Etiquetas nuevas | | | | |
| Archivos tocados | | | | |
| Conceptos nuevos de lenguaje | | | | |
| Primera línea de la traza que espero que difiera | | | | |

La hipótesis de localización, en una frase (antes de medir):

>

Qué espero que empate:

Qué espero que difiera:

## 4. Ejecución — las dos líneas base

- Comando de compilación, estructurada:
- Comando de compilación, no estructurada:
- Estados de salida:
- Destino del diff: `../../../common/traces/seed-0001.txt`

Registra `vacío` o `diff` en cada celda de resultado.

| Comprobación | Comando | Resultado |
|---|---|---|
| Estructurada contra la traza | | |
| No estructurada contra la traza | | |
| Estructurada contra no estructurada | | |

```text
<salida del diff, o «sin diferencias»>
```

Si algún diff no está vacío, detente aquí y completa §10.

## 5. Aplicar R-SKIP en copias de trabajo

- Directorio de trabajo:
- Copia estructurada:
- Copia no estructurada:
- No edité las líneas base versionadas: (sí / no)

Lista de comprobación de la implementación, aplicada a las dos:

- [ ] El contador de turnos iniciados está declarado y empieza en 0.
- [ ] Se incrementa una vez por turno, después de la comprobación de terminación y antes del sorteo.
- [ ] `turn=` imprime el contador de turnos iniciados, no `successfulMoveCount + 1`.
- [ ] La comprobación va después de la cabecera `turn=`/`order=` y antes de cualquier `try=`.
- [ ] Un salto de turno invierte el lado activo.
- [ ] Un salto de turno no registra movimiento y no incrementa `successfulMoveCount`.
- [ ] El límite de 200 movimientos y `moves=` siguen usando `successfulMoveCount`.
- [ ] Un turno saltado sigue sorteando e imprimiendo su orden.
- [ ] El salto no añade ningún sorteo.
- [ ] No añadí ninguna etiqueta nueva, o la razón de la etiqueta nueva está registrada abajo.

Comandos de compilación y ejecución:

```bash
<comandos>
```

| Comprobación | Comando | Resultado |
|---|---|---|
| Estructurada modificada contra no estructurada modificada | | |
| Estructurada modificada contra la traza | | |
| No estructurada modificada contra la traza | | |

- Primera divergencia respecto a la traza (línea y texto):
- Por qué las dos versiones modificadas deben coincidir:
- Si añadí una etiqueta, por qué:

## 6. Comparación — dónde aterriza el cambio

### 6.1 Tabla de coste

| Métrica | No estructurada | Estructurada | Diferencia | Qué mide |
|---|---|---|---|---|
| Sitios de edición | | | | tecleo |
| Archivos tocados | | | | acoplamiento |
| Aristas nuevas de flujo de control | | | | flujo añadido |
| Etiquetas nuevas | | | | posiciones nombradas |
| Aristas entrantes a la entrada de turno (antes) | | | | superficie de verificación |
| Asignaciones del lado activo (antes / después) | | | | acoplamiento de estado |
| Conceptos nuevos de lenguaje | | | | coste de aprendizaje |
| Trazas de referencia rotas por R-SKIP (de 20) | | | | cambio de contrato |
| Trazas reproducidas sin modificar (semillas comprobadas) | | | | contrato |
| Transcripciones modificadas que coinciden entre sí | | | | fidelidad |

### 6.2 Sitios de edición, enumerados

No estructurada:

| Sitio | Línea | Qué cambia | Dónde se sitúa (antes / dentro / después de la región) |
|---|---|---|---|
| | | | |

Estructurada:

| Sitio | Línea | Qué cambia | Dónde se sitúa (antes / dentro / después de la región) |
|---|---|---|---|
| | | | |

- Total de sitios de edición, no estructurada:
- Total de sitios de edición, estructurada:
- Líneas añadidas / eliminadas, no estructurada:
- Líneas añadidas / eliminadas, estructurada:

### 6.3 Aristas entrantes antes del cambio, enumeradas

No estructurada:

| Arista | Línea | Tipo (salto / caída directa) | ¿Pasa la comprobación? |
|---|---|---|---|
| | | | |

Estructurada:

| Arista | Línea | Tipo (arista de bucle / salto / caída directa) | ¿Pasa la comprobación? |
|---|---|---|---|
| | | | |

- Total de aristas entrantes, no estructurada:
- Total de aristas entrantes, estructurada:

### 6.4 Respuestas

- ¿Necesitó menos sitios de edición la versión estructurada? Si no, ¿qué localizó?
- ¿Qué recuento se movió entre las dos formas y cuál quedó igual?
- ¿Qué arista entrante del archivo no estructurado era la más fácil de pasar por alto?
- Si se añadiera un quinto `goto gameLoopStart`, ¿la comprobación lo cubriría? ¿Qué leería para
  saberlo?
- Si se añadiera un segundo bucle que inicia un turno al archivo estructurado, ¿la comprobación lo
  cubriría? ¿Qué leería para saberlo?
- ¿Qué hace visible el archivo estructurado en el punto del cambio que el no estructurado deja
  implícito?
- Predicción más cercana, y por qué:
- Predicción más lejana, y qué no vi:

## 7. Explicación

Una frase, sin *malo*, *limpio*, *legible*, *elegante*, *feo*, *espagueti*, *ilegible* ni *goto*:

>

Un párrafo:

- Dónde empieza un turno en cada archivo, como hecho de flujo de control:
- A qué apunta la arista del salto de turno en cada archivo:
- Por qué la entrada de turno estructurada tiene el número de aristas entrantes que tiene:
- Por qué recuentos de edición iguales pueden diferir en coste de verificación, o qué compró el
  sitio extra:
- Qué debe sostener un lector en la memoria de trabajo en cada caso:
- Qué haría incompleto un cambio futuro en cada archivo, y cómo lo notaría:

## 8. Criterio de aceptación

- Requisito reformulado sin añadir ningún sorteo:
- Regla de conteo usada:
- Sitios de edición contados (no estructurada / estructurada):
- Aristas entrantes contadas (no estructurada / estructurada):
- Qué recuento empata:
- Qué localiza la versión estructurada:
- Qué no localiza:
- La explicación evita las ocho palabras prohibidas: (sí / no)
- No edité las líneas base versionadas: (sí / no)

Si cambié una línea base versionada, qué y por qué:

## 9. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué no puedo explicar todavía:
- Una cosa que volvería a leer:
- Los dos números que llevaré a `construction-03`:
- Siguiente paso:

## 10. Si falta una línea base o no es fiel

Completa esto solo si faltaba un archivo, falló una compilación o un diff no estaba vacío.

- Archivo ausente, error de compilación o diff:
- Salida exacta de los comandos:
- Qué hice en lugar de inventar un recuento:
- Cuándo volveré a intentarlo:

## 11. Autoverificación

- [ ] Inventarié las dos formas desde los archivos, con números de línea.
- [ ] Escribí los recuentos previstos antes de contar.
- [ ] Compilé las dos líneas base y obtuve tres diffs vacíos.
- [ ] Apliqué la misma especificación R-SKIP a las dos copias de trabajo.
- [ ] Las dos versiones modificadas coincidieron entre sí.
- [ ] Enumeré cada sitio de edición y cada arista entrante.
- [ ] Separé el coste de edición del coste de verificación.
- [ ] Mi explicación de una frase evita las ocho palabras prohibidas.
- [ ] Mi párrafo nombra la causa de flujo de control, no el estilo.
- [ ] No edité las líneas base versionadas.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §9.
