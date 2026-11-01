# construction-10 — Evidencia
> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro de quien aprende. Escribe en español.

Este registro sigue un invariante desde la declaración hasta una sonda: la regla de que solo las
casillas dentro del área activa de 8×8 pueden marcarse como visitadas. Conserva las predicciones. El
hueco entre tu predicción y la observación es el resultado, sea cero o no.

## 1. Preparación de la ejecución

- Fecha:
- Semilla: `1`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Línea base leída el (fecha):
- ¿Registro de `construction-09` a mano? (sí / no)
- El número que traje de `construction-09` (declaraciones públicas / definiciones / trazas):
- Directorio de trabajo que usé:

## 2. Las tres comprobaciones (antes de ejecutar)

### 2.1 Cabecera y declaraciones

Rellena esto desde el código fuente, con números de línea, no de memoria.

| Cabecera | Líneas | Declaraciones | Tipo incompleto | Definición en |
|---|---|---|---|---|
| | | | | |

Conteos:

- Cabeceras:
- Unidades de traducción:
- Declaraciones públicas:
- Tipos incompletos:
- Definiciones de función, total:
- Auxiliares internos (`game.cpp`):
- Auxiliares internos (`main.cpp`):
- Puntos de entrada del programa:

Comparado con `construction-09`:

- Declaraciones añadidas:
- Definiciones añadidas:
- Llamadores modificados:
- La única declaración nueva, completa:

### 2.2 Mapa de líneas de `board.cpp`

| Líneas | Contenido |
|---|---|
| | |

- Línea de la definición de `board_is_valid`:
- Línea de la guarda en `board_was_visited`:
- Línea de la guarda en `board_mark_visited`:
- Línea del predicado `outsideActiveArea`:
- Línea de la comprobación de nulo dentro del verificador:

### 2.3 Roles

| Rol | Función | Qué decide |
|---|---|---|
| Cumplimiento | | |
| Cumplimiento | | |
| Observación | | |

- Puntos de llamada de `board_is_valid` en `code/`:
- Salida de `grep -rn 'board_is_valid' code/`:
- Qué dejaría de compilar si se borrara el verificador:
- Por qué una comprobación sin usar es una costura y no una garantía:

## 3. Predicción (escrita antes de medir)

### 3.1 Las guardas

El arreglo es `bool visited[16][16]`; el área activa es filas 1..8 y columnas 1..8.

| Secuencia de llamadas | `board_was_visited` después | `board_is_valid` después | Confianza |
|---|---|---|---|
| `board_mark_visited(board, 8, 8)` | | | |
| `board_mark_visited(board, 0, 0)` | | | |
| `board_mark_visited(board, 9, 9)` | | | |
| `board_mark_visited(board, 4, 0)` | | | |
| `board_mark_visited(board, 8, 9)` | | | |

- La fila que espero fallar, y por qué:

### 3.2 El verificador

| Pregunta | Mi predicción |
|---|---|
| `board_is_valid` en un tablero limpio | |
| `board_is_valid` después de una marca legal | |
| `board_is_valid` después de una marca fuera de rango con guarda | |
| Semillas que coinciden, de 20 | |
| ¿El juego llama al verificador? | |
| `board_is_valid` después de quitar la guarda y marcar `(0, 0)` | |
| Tamaño de texto de `board.o`: ¿crece, encoge o queda igual? | |

- La predicción de la que estoy menos seguro, y por qué:

### 3.3 La demostración

- La guarda que necesito quitar para que aparezca el estado inválido:
- Qué línea del predicado clasifica `(0, 0)` como fuera:
- Por qué el barrido llega a `(0, 0)`:
- Mi razón:

## 4. Ejecución — línea base y sonda

- Comando de compilación de la línea base:
- Estado de salida de la línea base:
- Diff contra `../../../common/traces/seed-0001.txt`:
- Semillas que coincidieron, de 20, en la línea base sin modificar:
- Ruta del código de la sonda (solo en el directorio de trabajo):
- Comando de compilación de la sonda:
- Estado de salida de la sonda:

Pega la salida de la sonda:

```text
<salida de la sonda>
```

| Observación | Predicción | Observado | Hueco |
|---|---|---|---|
| Tablero limpio válido | | | |
| `mark(8,8)` y luego `was_visited(8,8)` | | | |
| `mark(0,0)` y luego `was_visited(0,0)` | | | |
| `mark(9,9)` y luego `was_visited(9,9)` | | | |

- La línea que me sorprendió:
- Qué hice después en lugar de ajustar la predicción:

## 5. Comparar — qué prueba la sonda

| Llamada | Decisión de la guarda | ¿Escribe el arreglo? | `board_was_visited` | `board_is_valid` |
|---|---|---|---|---|
| ninguna, tablero limpio | — | no | — | |
| `mark(8, 8)` | | | | |
| `mark(0, 0)` | | | | |
| `mark(9, 9)` | | | | |

- La dimensión del arreglo y la condición del bucle, una al lado de la otra:
- Qué índice lee la última iteración:
- Qué casillas cubre el barrido, y cuántas:
- Qué calcula `outsideActiveArea` para `(0, 0)`:
- Qué calculan las guardas para `(0, 0)`:
- Por qué el predicado y las guardas no pueden discrepar:
- Dónde corre la comprobación de nulo, y qué devuelve:
- Por qué el barrido nunca lee `(16, 16)`:
- Por qué 20 de 20 trazas no pueden detectar una llamada ausente al verificador:
- El único número que separa "implementado" de "usado":

## 6. La prueba de cambio en una copia de trabajo

- Ruta de la copia de trabajo:
- Archivos editados:
- Líneas editadas:
- Archivos versionados editados: (sí / no)

### 6.1 Guarda intacta

- Comando de compilación:
- Estado de salida:

Pega la salida de la sonda del cambio:

```text
<salida de la sonda con guarda>
```

- Validez antes de la llamada ilegal:
- Validez después de `mark(0, 0)`:
- ¿Se escribió el arreglo? (sí / no)
- Por qué el estado sigue siendo legal:

### 6.2 Guarda quitada

- Qué borré de `board_mark_visited`:
- Comando de compilación:
- Estado de salida:

Pega la salida de la sonda del cambio:

```text
<salida de la sonda sin guarda>
```

- Validez antes de la llamada ilegal:
- Validez después de `mark(0, 0)`:
- A qué casilla llegó la escritura:
- Qué línea del predicado lo reporta ahora:
- El estado que ahora es construible:
- La función que ahora lo observa:

### 6.3 Guarda restaurada y trazas

- Guarda restaurada: (sí / no)
- Comando de compilación completa:
- Estado de salida:
- Semillas que coincidieron, de 20, con la guarda:
- Semillas que coincidieron, de 20, sin la guarda:
- ¿Quitar la guarda cambió alguna transcripción? (sí / no)
- Por qué el conteo de trazas es el mismo en los dos casos:
- Qué confirman las trazas:
- Qué no confirman las trazas:

## 7. Ciclo de vida

| Pregunta | Respuesta |
|---|---|
| ¿Quién puede romper el invariante? | |
| ¿Qué función puede detectarlo? | |
| ¿Qué función lo repara? | |
| ¿`board_reset` repara un tablero roto? | |
| ¿Qué pasa con el invariante en `board_destroy`? | |
| ¿Quién es responsable de llamar al verificador? | |

- Casillas que inicializa `board_create`:
- Casillas que limpia `board_reset`:
- Casillas que un tablero correcto nunca escribe:
- Qué deja un `mark(9,9)` sin guarda después de `board_reset`:
- Por qué `board_was_visited(9, 9)` no puede reportar el daño:
- Qué función sí puede:
- Qué lección posterior elimina la posibilidad en lugar de comprobarla:
- Qué lección posterior posee el otro extremo del ciclo de vida:

## 8. Explicar

Una frase, sin las palabras *cabecera*, *archivo*, *módulo*, *compilar*, *puntero*, *struct*,
*clase*, *opaco*, *encapsulación*, *mejor*, *peor*, *limpio*, *más limpio*, *bonito*, *elegante*,
*feo* ni *modular*:

>

Un párrafo:

- Qué fija el invariante:
- Qué lo hace cumplir:
- Qué hace correcto al barrido:
- Qué no puede hacer el barrido:
- Qué prueban las 20 trazas:
- Qué no prueban las trazas:
- Qué prueba la prueba de cambio:
- Qué cuesta la prueba de cambio:
- Qué no hace `board_reset`:
- Qué lección posterior elimina la posibilidad:
- Qué lección posterior posee el ciclo de vida:

Las cuatro cosas que mantuve separadas:

## 9. Comprobación de aceptación

- Identifiqué el verificador, las dos guardas y la regla del área activa: (sí / no)
- Encontré que el verificador tiene cero puntos de llamada: (sí / no)
- Predije `true` para un tablero limpio y observé `true`: (sí / no)
- Expliqué el dominio del barrido como `0..15` sobre un arreglo de 16×16: (sí / no)
- Expliqué por qué `(0, 0)` es un índice válido y una casilla inválida: (sí / no)
- Mostré que el predicado y las dos guardas usan las mismas cuatro comparaciones: (sí / no)
- Con la guarda intacta, `(0, 0)` y `(9, 9)` se rechazan: (sí / no)
- Con la guarda quitada, `(0, 0)` vuelve inválido el tablero: (sí / no)
- Restauré la guarda y el estado inválido se rechaza otra vez: (sí / no)
- Las 20 trazas pasan con la guarda y sin ella: (sí / no)
- Registré que las trazas no pueden ver el verificador: (sí / no)
- Mostré que `board_reset` no repara un tablero roto: (sí / no)
- No edité ningún archivo de código versionado: (sí / no)

Si edité un archivo de código versionado, cuál y por qué:

## 10. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué no puedo explicar todavía:
- Una cosa que volvería a leer:
- El número que llevaré a `construction-11`:
- La comprobación en la que más confío: la traza, las guardas o el verificador:
- La comprobación que me sorprendió:
- Qué añade esta línea base que `construction-09` no podía ofrecer:
- Qué sigue dejando a su llamador:
- Qué probó la sonda que las trazas no podían:
- El cambio que haría después para volver a probar la frontera:
- Siguiente paso:

## 11. Si falta una línea base o no es fiel

Rellena esto solo si faltó un archivo, si una compilación falló por una razón del entorno, si un
diff no quedó vacío, o si la sonda contradijo el comportamiento documentado de una forma que no
pudiste explicar.

- Archivo ausente, error de compilación, diff o contradicción:
- Salida exacta del comando:
- Qué hice en lugar de inventar un conteo:
- Cuándo volveré a intentarlo:

## 12. Autocomprobación

- [ ] Inventarié cada cabecera y cada definición con su archivo.
- [ ] Localicé el verificador, las dos guardas y el predicado por línea.
- [ ] Comparé la cabecera con `construction-09` y encontré la única declaración nueva.
- [ ] Predije el comportamiento de las guardas y el resultado del verificador antes de medir.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Compilé una sonda y vi un tablero limpio reportado como válido.
- [ ] Expliqué el dominio del barrido y por qué `(0, 0)` está fuera del área activa.
- [ ] Mostré que el predicado coincide con las guardas.
- [ ] Quité la guarda y vi que `(0, 0)` volvía inválido el tablero.
- [ ] Restauré la guarda y vi rechazarse el estado inválido.
- [ ] Reproduje 20 de 20 trazas con la guarda y sin ella.
- [ ] Mostré que `board_reset` no repara un tablero roto.
- [ ] Separé lo que dice la regla, lo que impone el código, lo que observa el verificador y lo que
      una ejecución puede ver.
- [ ] Mi frase evita las seis palabras de sintaxis y las ocho palabras de veredicto.
- [ ] No edité ningún archivo de código versionado.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §10.
