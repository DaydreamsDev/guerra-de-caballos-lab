# construction-05 — Evidencia

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro propio de quien aprende. Escribe en español.

Este registro compara dos tipos de elemento para un mismo tablero. Conserva las dos columnas. Un
número de un solo archivo no responde a la pregunta, y un empate es un resultado, no una diferencia
que falte.

## 1. Preparación de la ejecución

- Fecha:
- Semilla: `1`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Línea base con arreglo de estructuras leída el (fecha):
- Línea base con registros leída el (fecha):
- ¿Tengo el registro de construction-04 a mano? (sí / no)
- Los dos conteos que traje de `construction-04` (cambios de firma / sitios de llamada):

## 2. Inventario del tablero (antes de la ejecución)

### 2.1 Funciones que nombran `BoardCell`

Complétalo desde el código, con números de línea, no de memoria.

| Función | Línea | Recibe | Campos que lee | Campos que escribe |
|---|---|---|---|---|
| | | | | |

Recuentos:

- Funciones que reciben el tablero:
- De ellas, funciones que leen un campo:
- De ellas, funciones que escriben un campo:
- Funciones que reciben el tablero y no leen ningún campo:
- Sitios de llamada que pasan el tablero:
- Accesos a campo (`.visited`) en todo el archivo:

### 2.2 Qué dice el tipo de elemento

| Pregunta | construction-04 | construction-05 |
|---|---|---|
| Tipo de elemento | | |
| Qué guarda un elemento | | |
| Subíndices para leer el hecho de visitada | | |
| Subíndices para leer un segundo hecho por casilla | | |
| Dónde se declara un hecho nuevo por casilla | | |
| Firmas que obliga a cambiar un hecho nuevo por casilla | | |
| Sobre qué itera el recorrido | | |
| Qué mantiene dos hechos en la misma casilla | | |
| Qué sabe el tablero de su propio tamaño | | |
| Qué impide `boardCells[0][0].visited = true` | | |

### 2.3 Parámetros

| Métrica | construction-04 | construction-05 |
|---|---|---|
| Definiciones de función (cuenta `main`) | | |
| Parámetros en todas las definiciones | | |
| Parámetros de `tryMove` | | |
| Parámetros de `playTurn` | | |
| Parámetros de `playGame` | | |
| Parámetros del tablero | | |
| Parámetros añadidos o quitados por la conversión | | |

- Qué cambió en cada firma del tablero:
- Qué no cambió en cada firma del tablero:
- Elementos del arreglo / elementos que son casillas del tablero:

## 3. Predicción (escrita antes de medir)

### 3.1 El costo del campo

R-LASTVISIT, en mis palabras:

- Qué se registra:
- Cuándo se registra:
- Quién lo lee en este punto:
- Por qué la salida no cambia:

Mis números comprometidos para el campo:

| Métrica | Mi número | Confianza | Razón |
|---|---|---|---|
| Sitios de declaración del campo | | | |
| Sitios de asignación | | | |
| Ramas de `playTurn` que escriben | | | |
| Firmas que cambian | | | |
| Sitios de llamada que cambian | | | |
| Arreglos nuevos | | | |
| Globales nuevas | | | |
| Líneas añadidas | | | |
| Trazas de referencia que difieren (de 20) | | | |

- Qué hace `BoardCell boardCells[9][9] = {}` con el campo nuevo:
- El escritor que espero olvidar, y por qué:

### 3.2 El recorrido

| Métrica | Mi número | Confianza | Razón |
|---|---|---|---|
| Líneas de la función de recorrido | | | |
| Semillas donde el valor coincide con `moves=` (de 20) | | | |
| Semillas donde las coordenadas coinciden con el último destino `move=` (de 20) | | | |
| Semillas donde una escritura de captura olvidada se ve en el valor | | | |
| Semillas donde una escritura transpuesta se ve en el valor | | | |
| Semillas donde una escritura transpuesta se ve en las coordenadas | | | |

- El primer límite de bucle que espero usar:
- El último límite de bucle que espero usar:
- Qué devuelve un tablero sin tocar:

### 3.3 Las alternativas

Reglas de conteo que usé (en una línea cada una):

- Sitio de edición:
- Cambio de firma:
- Sitio de llamada:
- Arreglo nuevo:
- Global nueva:

Mis números comprometidos:

| Métrica | Campo en `BoardCell` | Arreglo `int` paralelo | 64 nombres `int` | Confianza |
|---|---|---|---|---|
| Sitios de edición | | | | |
| Cambios de firma | | | | |
| Sitios de llamada cambiados | | | | |
| Arreglos nuevos | | | | |
| Globales nuevas | | | | |
| Líneas añadidas | | | | |
| Trazas de referencia rotas (de 20) | | | | |
| Cómo se identifica una casilla | | | | |

### 3.4 Qué comparación es justa

- Pares de subíndices que identifican los dos hechos, versión con registro:
- Pares de subíndices que identifican los dos hechos, versión paralela:
- Qué hace que dos pares de subíndices paralelos sean la misma casilla:
- Dónde aparece primero una escritura olvidada en el segundo arreglo:
- Qué dice el compilador sobre un subíndice intercambiado:
- La comparación que trataré como la real, y por qué:

## 4. Ejecución — las dos líneas base

- Comando de compilación del arreglo de estructuras:
- Comando de compilación de registros:
- Estados de salida:
- Objetivo del diff: `../../../common/traces/seed-0001.txt`

Registra `vacío` o `diff` en cada celda de resultado.

| Comprobación | Comando | Resultado |
|---|---|---|
| Arreglo de estructuras contra la traza | | |
| Registros contra la traza | | |
| Arreglo de estructuras contra registros | | |

```text
<salida del diff, o "sin diferencias">
```

Semillas que coincidieron, de 20:

Si algún diff no está vacío, detente aquí y completa §11.

## 5. Añadir `moveNumber` y registrarlo

- Copia de trabajo:
- La línea de declaración del campo:
- La asignación de la rama de captura:
- La asignación de la rama legal:
- No edité la línea base versionada del arreglo de estructuras: (sí / no)

```text
<diff de la copia de trabajo contra la línea base versionada>
```

Salida de `grep -n 'moveNumber' <copia de trabajo>`:

```text
<salida>
```

Semillas que coincidieron, de 20:

- Firmas que cambiaron:
- Sitios de llamada que cambiaron:
- Funciones cuyo cuerpo cambió:
- Líneas añadidas / eliminadas:

### 5.1 Las cuatro observaciones

| Observación | Comando o edición | Resultado |
|---|---|---|
| No se movió ninguna firma | | |
| La traza no puede ver el campo | | |
| El campo tiene dos escritores | | |
| Ningún dato nuevo en el ámbito de espacio de nombres | | |

- Qué hace `= {}` con el tablero y con un campo añadido después:
- Qué prueba la traza y qué no:
- `sizeof(BoardCell)` antes / después:
- Bytes del tablero de 9×9 antes / después:
- Nombres mutables en el ámbito de espacio de nombres, antes / después (`nm`):
- Qué significa ya `moveNumber` en otras partes de este archivo:

### 5.2 Comprobación de la predicción

- Sitios de asignación predichos / observados:
- Líneas predichas / observadas:
- Predicción más cercana, y por qué:
- Predicción más lejana, y qué se me escapó:

## 6. El recorrido

- Nombre de la función:
- Archivo de trabajo:
- Los dos límites de bucle:
- El valor inicial del máximo:
- Imprimí el resultado por la salida de error, así que la salida estándar no cambió: (sí / no)

```text
<fuente de la función>
```

```bash
<comandos>
```

Resultado de la semilla 1:

```text
<last_recorded=... moves=...>
```

### 6.1 Los 20 resultados

| Semilla | `last_recorded` | `moves=` | Coordenadas | Último destino `move=` | ¿Coincide? |
|---|---|---|---|---|---|
| 1 | | | | | |
| 2 | | | | | |
| 3 | | | | | |
| 4 | | | | | |
| 5 | | | | | |
| 6 | | | | | |
| 7 | | | | | |
| 8 | | | | | |
| 9 | | | | | |
| 10 | | | | | |
| 11 | | | | | |
| 12 | | | | | |
| 13 | | | | | |
| 14 | | | | | |
| 15 | | | | | |
| 16 | | | | | |
| 17 | | | | | |
| 18 | | | | | |
| 19 | | | | | |
| 20 | | | | | |

- Semillas donde el valor y `moves=` coinciden:
- Semillas donde las coordenadas y el último movimiento coinciden:
- Por qué el máximo coincide con `moves=`:
- Por qué el máximo es único:
- Qué devuelve un tablero sin tocar, y qué significa ese valor:
- Por qué el recorrido no puede detenerse antes:
- Qué añade el recorrido que la transcripción no imprimía:

### 6.2 El escritor olvidado

- Asignación que quité:
- Semillas donde todas las trazas siguieron pasando, de 20:
- Semillas donde el valor no coincidió con `moves=`:
- Semillas donde el valor siguió coincidiendo:
- Partidas que terminan en captura, de 20:
- Partidas que terminan en trampa, de 20:
- Por qué el reparto es exactamente ese:

### 6.3 El límite del valor solo

- Subíndices que intercambié:
- Semillas donde todas las trazas siguieron pasando, de 20:
- Semillas donde el recorrido del valor no coincidió, de 20:
- Semillas donde el recorrido de coordenadas no coincidió, de 20:
- Semillas que sobrevivieron a la transpuesta, y por qué:
- La frase que conservaré sobre el máximo y la permutación:

### 6.4 Qué prueba cada comprobación

| Comprobación | Qué prueba | Qué no puede ver |
|---|---|---|
| Traza de referencia | | |
| Recorrido del valor | | |
| Recorrido de coordenadas | | |
| Un invariante (no construido aquí) | | |

- La afirmación que hoy puedo hacer sin excederme:
- La afirmación que todavía no puedo hacer, y qué lección la aporta:

## 7. El mismo hecho sin el registro

- Copia de trabajo:
- Colocación que apliqué primero:
- No edité la línea base versionada de registros: (sí / no)

```text
<diff de la copia con arreglo paralelo contra la línea base de registros>
```

Semillas que coincidieron, de 20:

### 7.1 Arreglo paralelo

| Métrica | Mi predicción | Observado |
|---|---|---|
| Sitios de edición | | |
| Cambios de firma | | |
| Sitios de llamada cambiados | | |
| Arreglos nuevos | | |
| Globales nuevas | | |
| Líneas añadidas / eliminadas | | |
| Trazas de referencia rotas (de 20) | | |

- Por qué `tryMove` no cambió:
- Por qué el arreglo nuevo tuvo que llegar a `playTurn` y `playGame`:
- Qué comprueba el compilador sobre los dos pares de subíndices:
- Qué no comprueba:
- Qué costaría un tercer hecho por casilla:

### 7.2 La versión con 64 nombres

- Declaraciones necesarias para el segundo hecho, conservando el tablero booleano:
- Forma de seleccionar un nombre con una fila y una columna:
- Líneas de esa selección en cada sitio de escritura:
- Comparaciones del recorrido:
- Forma de pasar el conjunto a una función:
- Declaraciones necesarias si también se sustituye el tablero booleano por nombres:
- Qué muestra el ejercicio sobre el arreglo:
- Qué muestra sobre el arreglo de estructuras:

### 7.3 La colocación global

- Sitios de edición con el arreglo en el ámbito de espacio de nombres:
- Cambios de firma:
- Sitios de llamada cambiados:
- Globales mutables nuevas:
- Qué salió de las firmas:
- Dónde reaparece el costo:

### 7.4 Tabla de costos

| Métrica | Campo en `BoardCell` | Arreglo `int` paralelo | 64 nombres `int` | Qué mide |
|---|---|---|---|---|
| Sitios de edición | | | | escritura |
| Cambios de firma | | | | acoplamiento visible |
| Sitios de llamada cambiados | | | | alcance del grafo de llamadas |
| Arreglos nuevos | | | | segunda colección |
| Globales nuevas | | | | estado oculto |
| Líneas añadidas / eliminadas | | | | radio de impacto |
| Trazas de referencia rotas (de 20) | | | | cambio de contrato |
| Qué identifica una casilla | | | | unión |
| Costo de un tercer hecho | | | | costo marginal |

- La fila donde gana el registro:
- La fila donde gana el arreglo paralelo, si gana alguna:
- Por qué esa fila no es toda la comparación:
- Qué costó la conversión desde `construction-04`, y si este cambio la compensa:

## 8. Explicar

Una frase, sin la palabra *struct* y sin *mejor*, *peor*, *limpio*, *más limpio*, *elegante*,
*feo*, *modular*:

>

Un párrafo:

- Qué decide el tipo de elemento:
- Qué decide el recorrido:
- Qué localiza el arreglo de estructuras:
- Qué no localiza:
- Qué debe sostener en la memoria de trabajo quien lee para saber qué campos usa una función, en
  cada archivo:
- Qué puede probar cada comprobación: traza, recorrido del valor y recorrido de coordenadas:
- Qué lección posterior corrige nombrar la casilla una vez:
- Qué lección posterior corrige representación y límites:
- Qué lecciones posteriores corrigen invariantes y acceso:
- Qué lección posterior corrige el bucle mismo:

## 9. Comprobación de aceptación

- Recorrí el tablero con dos bucles: (sí / no)
- El recorrido encontró el último número de movimiento: (sí / no)
- El resultado coincidió con `moves=` en las 20 semillas: (sí / no)
- Añadí `moveNumber` a `BoardCell`: (sí / no)
- Registré el número de movimiento exitoso en los dos sitios de escritura: (sí / no)
- Las 20 trazas de referencia siguieron pasando: (sí / no)
- Puedo explicar por qué no hace falta un arreglo paralelo: (sí / no)
- Puedo explicar por qué 64 variables no se seleccionan con una coordenada: (sí / no)
- Puedo nombrar qué localiza el registro y qué no: (sí / no)
- La explicación evita *struct* y las ocho palabras de veredicto: (sí / no)
- No edité ninguna de las dos líneas base versionadas: (sí / no)

Si cambié una línea base versionada, qué y por qué:

## 10. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué no puedo explicar todavía:
- Una cosa que volvería a leer:
- El número que llevaré a `construction-06`:
- La fila donde gana el registro y la fila donde pierde:
- El cambio que haría caro el arreglo paralelo la próxima vez:
- El cambio que haría caro el registro la próxima vez:
- Siguiente paso:

## 11. Si falta una línea base o no es fiel

Completa esto solo si faltó un archivo, falló una compilación o un diff no estaba vacío.

- Archivo ausente, error de compilación o diff:
- Salida exacta del comando:
- Qué hice en lugar de inventar un conteo:
- Cuándo volveré a intentarlo:

## 12. Autocomprobación

- [ ] Inventarié qué función recibe el tablero y qué campo toca, con números de línea.
- [ ] Escribí la predicción del campo, la del recorrido y la de las dos alternativas antes de
      medir.
- [ ] Compilé las dos líneas base y obtuve tres diffs vacíos, más 20 semillas coincidentes.
- [ ] Añadí `moveNumber` a una copia de trabajo y lo registré en los dos sitios de escritura.
- [ ] Mantuve las 20 trazas pasando después de añadir el campo.
- [ ] Escribí un recorrido de dos bucles y comprobé su valor contra `moves=`.
- [ ] Comprobé sus coordenadas contra el último destino `move=`.
- [ ] Mostré que una escritura de captura olvidada es invisible en la traza y visible en 11 de 20.
- [ ] Mostré que una escritura transpuesta es invisible en el valor y visible en 14 de 20
      coordenadas.
- [ ] Compilé la variante con arreglo paralelo y conté sus firmas y sitios de llamada.
- [ ] Expliqué por qué 64 nombres no se pueden seleccionar con una coordenada calculada.
- [ ] Separé el costo marginal del costo de conversión.
- [ ] Mi frase de explicación evita *struct* y las ocho palabras de veredicto.
- [ ] No edité las líneas base versionadas.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §10.
