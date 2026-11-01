# construction-11 — Evidencia
> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro de quien aprende. Escribe en español.

Este registro sigue el invariante del tablero a través de la frontera de la clase: la regla de que
solo las casillas dentro del área activa de 8×8 pueden marcarse como visitadas. Conserva las
predicciones. El hueco entre tu predicción y la observación es el resultado, sea cero o no.

## 1. Preparación de la ejecución

- Fecha:
- Semilla: `1`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Línea base leída el (fecha):
- ¿Registro de `construction-10` a mano? (sí / no)
- El número que traje de `construction-10` (declaraciones públicas / definiciones / trazas):
- Directorio de trabajo que usé:

## 2. La frontera de la clase (antes de ejecutar)

### 2.1 Cabecera y declaraciones

Rellena esto desde el código fuente, con números de línea, no de memoria.

| Cabecera | Líneas | Declaraciones públicas | Declaraciones de datos privados | Clase completa | Tipo incompleto |
|---|---|---|---|---|---|
| | | | | | |

Conteos:

- Cabeceras:
- Unidades de traducción:
- Declaraciones públicas:
- Declaraciones de datos privados:
- Definiciones de clase completas:
- Tipos incompletos:
- Definiciones de función, total:
- Auxiliares internos (`game.cpp`):
- Auxiliares internos (`main.cpp`):
- Puntos de entrada del programa:

Comparado con `construction-10`:

- Declaraciones públicas: ______ → ______
- Definiciones: ______ → ______
- Tipos incompletos: ______ → ______
- Definiciones de clase completas: ______ → ______
- Llamadores modificados:
- Operaciones del tablero que desaparecieron:
- Operaciones del tablero que aparecieron:
- El único especificador de acceso que sostiene la frontera:

### 2.2 Mapa de líneas

`board.h`:

| Líneas | Contenido |
|---|---|
| | |

`board.cpp`:

| Líneas | Contenido |
|---|---|
| | |

- Línea de `public:`:
- Línea de `private:`:
- Línea de la declaración del arreglo:
- Línea del constructor:
- Línea de `is_valid`:
- Línea de la guarda en `was_visited`:
- Línea de la guarda en `mark_visited`:

### 2.3 Roles y acceso

| Rol | Operación | Acceso | Qué decide |
|---|---|---|---|
| | | | |

- Datos públicos:
- Operaciones públicas que escriben:
- Operaciones públicas que solo leen:
- Qué operaciones son `const`:
- Qué impide `const`:
- Puntos de llamada de `is_valid()` en `code/`:
- Salida de `grep -rn 'is_valid' code/`:
- Qué dejaría de compilar si se borrara `is_valid()`:
- Por qué un verificador sin usar sigue siendo útil después de la clase:

## 3. Predicción (escrita antes de medir)

### 3.1 Las operaciones públicas

El arreglo es `bool visited[16][16]`; el área activa es filas 1..8 y columnas 1..8.

| Secuencia de llamadas | `is_valid()` después | `was_visited` después | Confianza |
|---|---|---|---|
| `Board board;` | | — | |
| `board.reset()` | | — | |
| `board.mark_visited(1, 1)` | | | |
| `board.mark_visited(8, 8)` | | | |
| `board.mark_visited(0, 0)` | | | |
| `board.mark_visited(9, 9)` | | | |
| `board.mark_visited(4, 0)` | | | |
| `board.mark_visited(8, 9)` | | | |

- La fila que espero fallar, y por qué:

### 3.2 La prueba de cambio

| Pregunta | Mi predicción |
|---|---|
| ¿Qué pasa cuando se borra `private:` y compila `board.visited[0][0] = true`? | |
| `is_valid()` después de esa escritura | |
| ¿Qué pasa cuando se restaura `private:` y se compila la misma sonda? | |
| Semillas que coinciden, de 20, con la clase sin cambios | |
| Semillas que coinciden, de 20, con `visited` público | |
| ¿El juego llama a `is_valid()`? | |
| Tamaño de texto de `board.o`: ¿crece, encoge o queda igual? | |

- La predicción de la que estoy menos seguro, y por qué:

### 3.3 El compilador

- La línea a la que apunta el compilador cuando `visited` es privado:
- El texto exacto del error que espero:
- Por qué el compilador puede rechazar la escritura aunque `board.h` esté incluido:
- Por qué el compilador no puede rechazar un índice malo dentro de `mark_visited`:
- Mi razón:

## 4. Ejecución — línea base y sonda de frontera

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

| Observación | Predicho | Observado | Hueco |
|---|---|---|---|
| Tablero recién construido válido | | | |
| `reset()` lo deja válido | | | |
| `mark(1,1)` y luego `was_visited(1,1)` | | | |
| `mark(8,8)` y luego `was_visited(8,8)` | | | |
| `mark(0,0)` y luego `was_visited(0,0)` | | | |
| `mark(9,9)` y luego `was_visited(9,9)` | | | |
| `mark(4,0)` y luego `was_visited(4,0)` | | | |
| `mark(8,9)` y luego `was_visited(8,9)` | | | |

- La línea que me sorprendió:
- Qué hice después en lugar de ajustar la predicción:

## 5. Comparar — qué prueba la frontera

| Llamada | Decisión de la guarda | ¿Se escribió el arreglo? | `was_visited` | `is_valid` |
|---|---|---|---|---|
| ninguna, recién construido | — | no | — | |
| `reset()` | — | limpia 1..8 | — | |
| `mark(1, 1)` | | | | |
| `mark(8, 8)` | | | | |
| `mark(0, 0)` | | | | |
| `mark(9, 9)` | | | | |
| `mark(4, 0)` | | | | |
| `mark(8, 9)` | | | | |

- La interfaz pública tiene algún dato: (sí / no)
- Los únicos escritores públicos:
- Qué limpia `reset()`:
- Qué casillas no puede marcar como visitadas ninguna operación pública:
- Por qué esas casillas siguen en `false`:
- Qué recorre `is_valid()`:
- Por qué las operaciones públicas no pueden construir un estado inválido:
- Qué no prueba la sonda:
- El número que separa "la clase se usa" de "el verificador se llama":

## 6. La prueba de cambio en una copia de trabajo

- Ruta de la copia de trabajo:
- Archivos editados:
- Líneas editadas:
- Archivos versionados editados: (sí / no)

### 6.1 Estado público

- Qué borré de `board.h`:
- Comando de compilación:
- Estado de salida:

Pega la salida de la sonda de cambio:

```text
<salida de la sonda con el estado público>
```

- Validez antes de la escritura:
- Validez después de `visited[0][0] = true`:
- Qué casilla alcanzó la escritura:
- Qué línea del predicado lo reporta:
- El estado que ahora es construible:
- La operación que lo observa:

### 6.2 Estado privado de nuevo

- Qué restauré:
- Comando de compilación:
- Estado de salida:

Pega la salida del compilador:

```text
<salida del compilador>
```

- La línea que rechaza el compilador:
- El texto del error:
- Por qué la misma línea compiló en 6.1:
- Qué prueba esto sobre el especificador de acceso:

### 6.3 Trazas

- Semillas que coincidieron, de 20, con `visited` público:
- Semillas que coincidieron, de 20, con `visited` privado:
- ¿El cambio de visibilidad alteró alguna transcripción? (sí / no)
- Por qué el conteo de trazas es el mismo en ambos casos:
- Qué confirman las trazas:
- Qué no confirman las trazas:

## 7. El invariante y sus escritores

| Pregunta | Respuesta |
|---|---|
| ¿Quién puede escribir `visited`? | |
| ¿Quién puede leer `visited`? | |
| ¿Qué operación observa el invariante? | |
| ¿Qué operaciones lo imponen? | |
| ¿La clase hace correctas a las funciones miembro? | |
| ¿Qué puede seguir rompiendo el invariante? | |
| ¿`private` es una frontera de seguridad? | |
| ¿Quién es responsable de llamar a `is_valid()`? | |

- Qué elimina la clase del llamador:
- Qué mueve la clase dentro del tipo:
- Qué no elimina la clase:
- Qué lección posterior se apoya en la clase:
- Qué lección posterior se ocupa del ciclo de vida:

## 8. Explicar

Una frase, sin usar las palabras *clase*, *privado*, *público*, *acceso*, *compilador*, *palabra
clave*, *cabecera*, *archivo*, *módulo*, *encapsulación*, *mejor*, *peor*, *limpio*, *más limpio*,
*bonito*, *elegante*, *feo* ni *modular*:

>

Un párrafo:

- Qué fija la clase:
- Qué lo impone:
- Qué hace distinta a esta frontera de la de `construction-10`:
- Qué hacen las operaciones públicas:
- Qué prueban las 20 trazas:
- Qué no prueban las trazas:
- Qué prueba la prueba de cambio:
- Qué cuesta la prueba de cambio:
- Qué no arregla la clase:
- Qué lección posterior se apoya en la clase:

Las cuatro cosas que mantuve separadas:

## 9. Comprobación de aceptación

- Identifiqué el estado privado, las operaciones públicas y el especificador de acceso: (sí / no)
- Encontré que no hay ningún dato público: (sí / no)
- Predije `valid=1` después de la construcción y observé `valid=1`: (sí / no)
- Mostré que todas las operaciones públicas preservan el invariante: (sí / no)
- Encontré que `is_valid()` sigue teniendo cero puntos de llamada: (sí / no)
- Borré `private:` en una copia de trabajo y la escritura compiló: (sí / no)
- Observé que `is_valid()` se vuelve falso después de `visited[0][0] = true`: (sí / no)
- Restauré `private:` y la misma escritura dejó de compilar: (sí / no)
- Las 20 trazas pasan con `visited` público y con `visited` privado: (sí / no)
- Registré que las trazas no pueden ver la frontera: (sí / no)
- Expliqué qué impide el control de acceso y qué no impide: (sí / no)
- No edité ningún archivo de código versionado: (sí / no)

Si edité un archivo de código versionado, qué y por qué:

## 10. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué no puedo explicar todavía:
- Una cosa que volvería a leer:
- El número que llevaré a `construction-12`:
- La frontera en la que más confío: el especificador de acceso, las guardas o el verificador:
- La frontera que me sorprendió:
- Qué añade esta línea base que `construction-10` no podía ofrecer:
- Qué sigue dejando a la implementación de la clase:
- Qué probó la prueba de cambio que las trazas no podían:
- El cambio que haría después para volver a probar la frontera:
- Siguiente paso:

## 11. Si falta una línea base o no es fiel

Rellena esto solo si faltaba un archivo, una compilación falló por una razón del entorno, un diff no
quedó vacío, o la sonda contradijo el comportamiento documentado de una forma que no pudiste
explicar.

- Archivo ausente, error de compilación, diff o contradicción:
- Salida exacta del comando:
- Qué hice en lugar de inventar un conteo:
- Cuándo volveré a intentarlo:

## 12. Autocomprobación

- [ ] Inventarié cada cabecera y cada definición con su archivo.
- [ ] Localicé el estado privado, la interfaz pública y el especificador de acceso por línea.
- [ ] Comparé la clase con `construction-10` y nombré qué oculta cada una.
- [ ] Predije el comportamiento de las operaciones públicas y la prueba de cambio antes de medir.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Compilé una sonda de frontera y vi que un tablero recién construido es válido.
- [ ] Mostré que todas las operaciones públicas preservan el invariante.
- [ ] Borré `private:` y vi que el estado inválido se vuelve construible.
- [ ] Restauré `private:` y vi la misma escritura rechazada en tiempo de compilación.
- [ ] Reproduje 20 de 20 trazas con `visited` público y con `visited` privado.
- [ ] Expliqué la diferencia entre control de acceso y límites del arreglo.
- [ ] Separé qué dice la regla, quién puede escribir, qué comprueba el compilador y qué puede ver
      una ejecución.
- [ ] Mi explicación de una frase evita las palabras de sintaxis y las palabras de veredicto.
- [ ] No edité ningún archivo de código versionado.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §10.
