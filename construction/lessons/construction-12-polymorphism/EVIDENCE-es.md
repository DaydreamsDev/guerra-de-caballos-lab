# construction-12 — Evidencia

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro de quien aprende. Escribe en español.

Este registro sigue la pieza a través de la interfaz: las cuatro preguntas que el bucle del juego
puede formular, la clase que las responde y el tipo concreto que sigue nombrado una vez en la raíz.
Conserva las predicciones. El hueco entre tu predicción y la observación es el resultado, sea cero o
no.

## 1. Preparación de la ejecución

- Fecha:
- Semilla: `1`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Línea base leída el (fecha):
- ¿Registro de `construction-11` a mano? (sí / no)
- El número que traje de `construction-11` (declaraciones públicas / definiciones / trazas):
- Directorio de trabajo que usé:

## 2. La interfaz (antes de ejecutar)

### 2.1 Cabeceras y declaraciones

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

Comparado con `construction-11`:

- Declaraciones públicas: ______ → ______
- Definiciones: ______ → ______
- Definiciones de clase completas: ______ → ______
- Cabeceras: ______ → ______
- Unidades de traducción: ______ → ______
- Declaraciones de datos privados: ______ → ______
- Declaraciones del tablero que cambiaron:
- Operaciones nuevas de la interfaz:
- Implementaciones nuevas de esas operaciones:
- El tipo concreto que sigue nombrado en la raíz:

### 2.2 Mapa de líneas

`piece.h`:

| Líneas | Contenido |
|---|---|
| | |

`knight_piece.h`:

| Líneas | Contenido |
|---|---|
| | |

`knight_piece.cpp`:

| Líneas | Contenido |
|---|---|
| | |

- Línea del destructor virtual:
- Línea de `row`:
- Línea de `column`:
- Línea de `set_position`:
- Línea de `role_name`:
- Línea de la etiqueta `public:`:
- Línea de la etiqueta `private:`:
- Líneas que construyen un `KnightPiece` en `run_game`:
- Línea que incluye `knight_piece.h` en `game.cpp`:

### 2.3 Roles y despacho

| Rol | Operación | La implementa | Qué pregunta el bucle |
|---|---|---|---|
| | | | |

- Número de operaciones virtuales puras en `Piece`:
- Operaciones que son `const`:
- Qué puede llamar un `const Piece *`:
- La única operación que escribe:
- Datos públicos en `KnightPiece`:
- Número de puntos de llamada virtuales en `game.cpp`:
- Las cuatro operaciones que preguntan esos puntos:
- Pruebas de tipo en el bucle (`switch`, etiqueta, `dynamic_cast`):
- Qué impide `= 0`:
- Qué cambiaría si `row()` perdiera `= 0`:

## 3. Predicción (escrita antes de medir)

### 3.1 La tabla de despacho

Cada expresión tiene tipo estático `Piece *` o `const Piece *` y apunta a un `KnightPiece`.

| Expresión | Qué función se ejecuta | Salida prevista | Confianza |
|---|---|---|---|
| `playerPiece.set_position(3, 4)` a través de `KnightPiece` | | | |
| `piece->row()` a través de `const Piece &` | | | |
| `piece->column()` a través de `const Piece &` | | | |
| `piece->role_name()` a través de `const Piece &` | | | |
| `otherPiece->row()` a través de `const Piece *` | | | |
| `fromPiece->role_name()` en `printMove` | | | |

- La fila que espero fallar, y por qué:

### 3.2 La prueba de cambio

| Pregunta | Mi predicción |
|---|---|
| Archivos que añadiré para un segundo tipo `Piece` | |
| Líneas que cambiaré en `game.cpp` | |
| Ediciones dentro de `playTurn`, `tryMove`, `printTurnHeader`, `printMove`, `playGame` | |
| Semillas que coinciden, de 20, con el sustituto en la raíz | |
| Semillas que coinciden, de 20, tras restaurar `KnightPiece` | |
| Conceptos nuevos que exige el cambio | |

- La predicción de la que estoy menos seguro, y por qué:

### 3.3 La sonda de ligadura

| Pregunta | Mi predicción |
|---|---|
| `piece.row()` a través de `KnightPiece` tras `set_position(3, 4)` | |
| `piece.row()` a través de `const Piece &` con `row` virtual | |
| La misma llamada con `row` no virtual y un cuerpo base que devuelve 0 | |
| `sizeof(Piece)` | |
| `sizeof(KnightPiece)` | |
| Tamaño de texto de `game.o`: ¿crece, encoge o queda igual? | |

- Qué espero que cambie la palabra clave `virtual`, en una frase:

## 4. Ejecución — línea base y sonda de despacho

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
| `role` a través de `KnightPiece` | | | |
| `role` a través de `const Piece &` | | | |
| `row` a través de `const Piece &` | | | |
| `sizeof(Piece)` | | | |
| `sizeof(KnightPiece)` | | | |
| Texto de `game.o` | | | |

- La línea que me sorprendió:
- Qué hice después en lugar de ajustar la predicción:

## 5. Comparar — qué prueba el despacho

- Salida de `grep -n 'KnightPiece' code/game.cpp`:
- Número de líneas que mencionan el tipo concreto:
- Cuáles están dentro del bucle:

| Líneas | Función | Llamadas virtuales |
|---|---|---|
| | | |

- Total de puntos de llamada virtuales:
- Líneas de `nm -C knight_piece.o` que mencionan `vtable` o `typeinfo`:
- `sizeof(Piece)`:
- `sizeof(KnightPiece)`:
- Qué guarda el objeto:
- Qué guarda la clase:
- Qué prueba la sonda sobre la llamada:
- Qué no prueba la sonda:

### 5.1 La sonda de renombrado

- Archivos cambiados en la copia de trabajo:
- Nombres de rol antiguos:
- Nombres de rol nuevos:
- Líneas que difieren, de 89:
- Qué clases de línea difieren (`who=`, `move=`, `trapped=`, `winner=`):
- Qué líneas son idénticas:
- Qué prueba esto sobre `role_name()`:

### 5.2 Desvirtualización

- Salida de `objdump` o `nm` que muestra una llamada directa:
- La función donde aparece:
- Por qué el compilador pudo probar el tipo allí:
- Por qué el bucle sigue compilando contra la interfaz:
- Qué no cambia esto sobre el diseño:

## 6. La prueba de cambio en una copia de trabajo

- Ruta de la copia de trabajo:
- Archivos añadidos:
- Líneas añadidas:
- Archivos editados:
- Líneas editadas:
- Archivos versionados editados: (sí / no)

### 6.1 Añadir la subclase

- Nombre de la clase:
- Ruta de la cabecera:
- Ruta de la implementación:
- Líneas en la cabecera:
- Líneas en la implementación:
- Las cuatro operaciones implementadas:
- Qué conserva deliberadamente igual la subclase:

### 6.2 Ejercitarla a través de la interfaz

- Ruta del código de la sonda:
- Comando de compilación de la sonda:
- Estado de salida de la sonda:

Pega la salida de la sonda:

```text
<salida de la sonda>
```

- El tipo del parámetro de la función que la usó:
- Qué sabía esa función de la clase concreta:
- Qué prueba esto:

### 6.3 Sustituir en la raíz

Pega el diff contra el `code/game.cpp` versionado:

```text
<diff>
```

- Líneas añadidas:
- Líneas eliminadas:
- Líneas dentro del bucle:
- Comando de compilación:
- Semillas que coincidieron, de 20:

### 6.4 Restaurar el original

- Qué restauré:
- Comando de compilación:
- Semillas que coincidieron, de 20, con el `KnightPiece` original:
- ¿Añadir un archivo alteró la línea base? (sí / no)

### 6.5 El hueco: el movimiento

- Dónde se declaran las tablas de movimiento:
- Dónde se indexan:
- La operación virtual que el bucle no llama:
- Qué exige una regla de movimiento distinta:
- Líneas que añadiría a la interfaz:
- Líneas que cambiaría en `tryMove`:
- ¿Una traza de referencia cubre la regla de movimiento nueva? (sí / no)
- Qué prueba el juego del caballo tras la extensión:
- Qué no prueba la extensión:
- La frase precisa de aceptación de esta lección:

## 7. La interfaz y sus límites

| Pregunta | Respuesta |
|---|---|
| ¿Qué guarda el bucle? | |
| ¿Qué pregunta para una posición? | |
| ¿Qué pregunta para un nombre? | |
| ¿Qué pregunta para mover una pieza? | |
| ¿Dónde sigue nombrado el tipo concreto? | |
| ¿Qué no cubre la interfaz? | |
| ¿Qué cuesta un tipo nuevo con el mismo comportamiento? | |
| ¿Qué cuesta una regla de movimiento nueva? | |
| ¿Qué cuesta `virtual` por objeto? | |
| ¿Qué cuesta `virtual` por clase? | |
| ¿Qué cuesta una llamada virtual en ejecución? | |
| ¿Puede el compilador eliminar la indirección? | |

- Qué elimina la interfaz del bucle:
- Qué traslada a la raíz:
- Qué no elimina:
- Qué lección posterior se apoya en la interfaz:
- Qué lección posterior se ocupa del ciclo de vida:

## 8. Explicar

Una frase, sin usar las palabras *virtual*, *polimorfismo*, *interfaz*, *clase*, *puntero*,
*compilador*, *despacho*, *ligadura dinámica*, *mejor*, *peor*, *limpio*, *flexible*, *extensible*,
*elegante* ni *feo*:

>

Un párrafo:

- Qué fija la interfaz:
- Qué lo impone:
- Qué la hace distinta de `construction-11`:
- Qué hacen los 19 puntos de llamada:
- Qué prueban las 20 trazas:
- Qué no prueban las trazas:
- Qué prueba la prueba de cambio:
- Qué cuesta la prueba de cambio:
- Qué no cubre la interfaz:
- Qué sigue costando el tipo concreto:
- Qué cuesta la ligadura dinámica en ejecución:
- Qué lección posterior se apoya en la interfaz:

Las cuatro cosas que mantuve separadas:

## 9. Comprobación de aceptación

- Identifiqué las cinco declaraciones virtuales y las cuatro sobreescrituras: (sí / no)
- Conté los 19 puntos de llamada virtuales y los asigné a funciones: (sí / no)
- Encontré que `KnightPiece` solo se nombra en las dos construcciones, y su cabecera se incluye una
  vez: (sí / no)
- Predije la tabla de despacho y observé las funciones derivadas: (sí / no)
- Reproduje las 20 trazas en la línea base sin modificar: (sí / no)
- Compilé la sonda de ligadura y vi la función derivada a través de `const Piece &`: (sí / no)
- Medí `sizeof(Piece)` y `sizeof(KnightPiece)`: (sí / no)
- Añadí una segunda implementación de `Piece` en un archivo de trabajo: (sí / no)
- La ejercité a través de `Piece &`: (sí / no)
- La sustituí en la raíz y obtuve 20 de 20 trazas: (sí / no)
- Restauré `KnightPiece` y obtuve 20 de 20 trazas: (sí / no)
- Confirmé cero ediciones en el bucle: (sí / no)
- Quité `virtual` en una copia de trabajo y expliqué el resultado: (sí / no)
- Expliqué el error de `override` y el error de tipo abstracto: (sí / no)
- Nombré la geometría del movimiento como límite de la interfaz: (sí / no)
- No edité ningún archivo de código versionado: (sí / no)

Si edité un archivo de código versionado, qué y por qué:

## 10. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué no puedo explicar todavía:
- Una cosa que volvería a leer:
- El número que llevaré a `construction-13`:
- El punto de llamada que más me sorprendió:
- El tipo concreto que esperaba que desapareciera, y dónde sigue apareciendo:
- Qué compra la interfaz que `construction-11` no podía ofrecer:
- Qué sigue dejando en el bucle:
- Qué probó la prueba de cambio que las trazas no podían:
- El cambio que haría después para volver a probar la interfaz:
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
- [ ] Localicé la interfaz, las sobreescrituras y el estado privado por línea.
- [ ] Comparé `struct Knight` con `Piece` y `KnightPiece` y nombré qué expone cada uno.
- [ ] Predije la tabla de despacho, la prueba de cambio y la sonda de ligadura antes de medir.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Compilé la sonda de despacho y vi las funciones derivadas a través de `const Piece &`.
- [ ] Medí el coste en objeto y binario de la ligadura dinámica.
- [ ] Añadí una segunda implementación en un archivo de trabajo y la ejercité a través de
      `Piece &`.
- [ ] La sustituí en la raíz con cero ediciones en el bucle y obtuve 20 de 20 trazas.
- [ ] Restauré el original y obtuve 20 de 20 trazas.
- [ ] Quité `virtual` y vi la misma llamada devolver el valor base.
- [ ] Expliqué `override` y el destructor virtual.
- [ ] Nombré qué no cubre la interfaz: la geometría del movimiento.
- [ ] Separé qué sabe el punto de llamada, qué decide el objeto, qué emite el compilador y qué
      puede ver una ejecución.
- [ ] Mi explicación de una frase evita las palabras del mecanismo y las del veredicto.
- [ ] No edité ningún archivo de código versionado.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §10.
