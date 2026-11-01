# construction-16 — Evidencia

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro del aprendiz. Escribe en español.

Este registro sigue el tablero y los dos caballos desde miembros por valor hasta recursos poseídos.
Pregunta quién reserva, quién libera y qué pasa cuando se copia al dueño. Conserva las predicciones.
La diferencia entre tu predicción y la observación es el resultado, sea cero o no.

## 1. Preparación de la ejecución

- Fecha:
- Semilla: `1`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Línea base leída el (fecha):
- ¿Registro de `construction-15` a mano? (sí / no)
- Los números que traje de `construction-15` (tamaño del motor / reservas / trazas):
- ¿Detector de fugas disponible? (valgrind / ASan / ninguno)
- Si no hay ninguno, la razón exacta:
- Directorio temporal que usé:

## 2. Los dueños (antes de ejecutar)

### 2.1 Inventario de propiedad

Rellena esto desde el código, con números de línea, no de memoria.

| Miembro | Línea | Tipo | Qué posee | Dónde se reserva |
|---|---|---|---|---|
| | | | | |
| | | | | |
| | | | | |

Funciones especiales:

| Función | Línea | `= delete` o `= default` | Qué decide |
|---|---|---|---|
| Constructor de copia | | | |
| Asignación de copia | | | |
| Constructor de movimiento | | | |
| Asignación de movimiento | | | |

Recuentos:

- Encabezados:
- Unidades de traducción:
- Definiciones de clase completas:
- Cuerpos de función escritos a mano:
- Funciones especiales declaradas:
- Declaraciones de datos privados en encabezados:
- Declaraciones públicas:
- Líneas de código:

Comparado con `construction-15`:

- Encabezados: ______ → ______
- Unidades de traducción: ______ → ______
- Definiciones de clase completas: ______ → ______
- Cuerpos de función: ______ → ______
- Funciones especiales declaradas: ______ → ______
- Declaraciones públicas: ______ → ______
- Líneas de código: ______ → ______
- Archivos: ______ → ______
- Líneas de `game_engine.h`: ______ → ______
- Archivos que difieren: ______

### 2.2 Mapa de líneas

`game_engine.h`:

| Líneas | Contenido |
|---|---|
| | |
| | |
| | |
| | |

Líneas concretas:

- Línea de `#include <memory>`:
- Línea de la cabecera de plantilla de `GameEngine`:
- Línea de `class GameEngine`:
- Líneas de las tres llamadas a `make_unique`:
- Línea del constructor de copia, eliminado:
- Línea de la asignación de copia, eliminada:
- Línea del constructor de movimiento, por defecto:
- Línea de la asignación de movimiento, por defecto:
- Línea del miembro `board`:
- Líneas de los dos miembros `Knight`:
- Línea de `board->reset()` en `run()`:
- Línea de `GameEngine<8, 8>` en `game.cpp`:
- Línea de `playTurn()`:
- Línea del estado del generador de ámbito de archivo en `random_sequence.cpp`:

### 2.3 El contrato de propiedad

| Pregunta | Respuesta |
|---|---|
| ¿Quién reserva el tablero? | |
| ¿Quién reserva cada caballo? | |
| ¿Quién los libera? | |
| ¿Dónde está escrito el destructor? | |
| ¿Cuántos dueños puede tener un tablero? | |
| ¿Qué hace el compilador ante un intento de copia? | |
| ¿Qué deja un movimiento en el objeto de origen? | |
| ¿El motor sigue llamando a `operator new`? | |
| ¿El código del motor contiene un `new` crudo? | |
| ¿El código del motor contiene un `delete` crudo? | |
| ¿Qué hacen realmente las dos coincidencias de `delete`? | |
| ¿El orden de inicialización coincide con el de declaración? | |
| ¿Por qué importa el orden? | |
| ¿Qué transfiere `.get()`? | |

- Los tres dueños, en palabras:
- Las dos operaciones eliminadas, en palabras:
- Las dos operaciones por defecto, en palabras:
- Qué garantiza `unique_ptr`, en una frase:
- Qué no garantiza `unique_ptr`, en una frase:

## 3. Predicción (escrita antes de medir)

### 3.1 La tabla de tamaños

| Pregunta | Mi predicción |
|---|---|
| `sizeof(GameEngine<8, 8>)` en `construction-15` | |
| `sizeof(GameEngine<8, 8>)` aquí | |
| `sizeof(Board<8, 8>)` | |
| `sizeof(std::unique_ptr<Board<8, 8>>)` | |
| `sizeof(Knight)` | |
| Bytes poseídos totales: motor más tablero más dos caballos | |
| Reservas de montón por motor | |
| Liberaciones por motor, tras la destrucción | |
| Bytes de texto del binario, antes y después | |
| Bytes de datos del binario, antes y después | |

- La fila que espero equivocar, y por qué:
- Por qué el motor puede reducirse mientras el programa no:
- Qué espero que pase con `data` y `bss`:

### 3.2 La tabla de reservas

| Pregunta | Mi predicción |
|---|---|
| Llamadas a `make_unique` en el constructor | |
| Reservas tras construir un motor | |
| Liberaciones tras morir ese motor | |
| Reservas para veinte motores | |
| Liberaciones tras morir esos veinte | |
| ¿Depende el recuento de la semilla? | |
| ¿Depende el recuento del número de turnos? | |
| ¿Depende el recuento del tamaño del tablero? | |

- Dónde espero que ocurran las reservas, y por qué:
- Qué espero si el juego queda atrapado en el turno uno:

### 3.3 La prueba de cambio

| Pregunta | Mi predicción |
|---|---|
| `std::is_copy_constructible_v<GameEngine<8, 8>>` | |
| `std::is_copy_assignable_v<GameEngine<8, 8>>` | |
| `std::is_move_constructible_v<GameEngine<8, 8>>` | |
| `std::is_move_assignable_v<GameEngine<8, 8>>` | |
| ¿Compila un programa que copia un motor? | |
| ¿Qué línea nombra el error? | |
| ¿Un motor de destino reproduce la semilla 1? | |
| ¿Un motor movido desde que llama a `run()` cómo termina? | |
| Semillas que coinciden, de 20, tras el cambio | |

- La predicción de la que estoy menos seguro, y por qué:
- Qué le hace un movimiento al objeto de origen, en mis propias palabras:

## 4. Ejecución — la línea base

- Comando de compilación:
- Estado de salida:
- Diff contra `../../../common/traces/seed-0001.txt`:
- Semillas que coincidieron, de 20, en la línea base sin modificar:
- Salida de la sonda de `sizeof`:
- Salida de la sonda de reservas para un motor:
- Salida de la sonda de reservas para veinte motores:
- Recuento de `nm` para `GameEngine<8, 8>` en `-O0`:
- Recuento de `nm` para `GameEngine<8, 8>` en `-O2`:
- Símbolos de constructor por nombre:
- ¿Hay un símbolo llamado `GameEngine(GameEngine&&)`?
- ¿Hay símbolo de destructor?

| Observación | Predicho | Observado | Diferencia |
|---|---|---|---|
| `sizeof(GameEngine<8, 8>)` | | | |
| `sizeof(Board<8, 8>)` | | | |
| Reservas por motor | | | |
| Liberaciones por motor | | | |
| Reservas para veinte | | | |
| Liberaciones para veinte | | | |
| Trazas coincidentes | | | |

- El número que me sorprendió:
- Qué hice después en lugar de ajustar la predicción:

## 5. Comparación — qué demuestra el `unique_ptr`

- Comando `grep` de los dueños:
- Comando `grep` de `new`/`delete` crudos:
- Número de coincidencias, y cuáles son:
- `operator new` / `operator delete` indefinidos en `game.o`: (sí / no)
- Bytes de texto de `game.o` antes y después:
- Bytes de texto del binario enlazado antes y después:
- Bytes de datos antes y después:
- Llamadas indirectas en `game.o` antes y después:
- Qué demuestran las 20 trazas:
- Qué no demuestran las trazas:
- Qué demuestra la sonda de reservas:
- Qué no demuestra la sonda de reservas:

### 5.1 La reubicación

- Comando que produjo los símbolos indefinidos:
- Salida de `nm -C game16.o | grep 'operator new\|operator delete'`:
- Por qué el código no tiene `new` y el archivo objeto sí:
- Qué tipo de biblioteca llama a `operator new`:
- Qué tipo de biblioteca llama a `operator delete`:

### 5.2 Lo que no cambió

- La transcripción:
- Las reglas:
- La interfaz del tablero:
- El contrato de la pieza:
- Las restricciones de dimensión:
- Las llamadas indirectas:
- Por qué los punteros inteligentes no implican una vtable:

### 5.3 Los fallos de copia

- Comando de la sonda de construcción de copia:
- Primera línea de error y mensaje:
- Comando de la sonda de asignación de copia:
- Primera línea de error y mensaje:
- Qué nombra el error, y por qué importa:
- Qué pasa al comentar las líneas 46–47 en una copia temporal:
- Por qué la regla no cambió, solo su dirección:

### 5.4 El movimiento

- Comando de la sonda de movimiento:
- Resultado de `cmp` contra la semilla 1:
- Qué le hizo el movimiento a los tres punteros:
- Comando de la sonda del movido desde:
- Estado de salida:
- Qué contiene el objeto movido desde:
- Qué hace `run()` con él:
- Qué dice la regla de C++ sobre un objeto movido desde:
- Las tres reparaciones que puedo nombrar:
- Qué reparación usa esta línea base, y si es deliberada:

### 5.5 La evidencia de fugas

- Detector que intenté:
- Salida exacta del intento:
- Evidencia que usé en su lugar:
- Tres hechos estructurales:
- Dos hechos empíricos:
- Qué no demuestra esa evidencia:
- La frase que escribiré en lugar de «sin fugas, verificado con ASan»:
- Si un detector sí corrió, su salida exacta:

## 6. La prueba de cambio en una copia temporal

- Ruta de la copia temporal:
- Archivos añadidos:
- Archivos editados:
- Líneas del archivo nuevo:
- Conceptos nuevos necesarios:
- Archivos rastreados editados: (sí / no)
- Estado de compilación de la sonda:
- Estado de la construcción de copia:
- Estado de la asignación de copia:
- Estado del movimiento:
- ¿El motor de destino coincide con la semilla 1? (sí / no)
- Resultado del motor movido desde:
- Trazas de 8×8 tras el cambio, de 20:

### 6.1 La sonda

- Encabezado incluido:
- Encabezado estándar incluido:
- Las tres afirmaciones que nombra la aceptación:
- Las dos afirmaciones extra que añadí, y por qué:
- Por qué `-c` basta:
- Por qué no se enlaza ningún `.cpp` compañero:

### 6.2 Recuentos

| Métrica | construction-15 | construction-16 | Diferencia |
|---|---|---|---|
| Líneas de código | | | |
| Archivos que difieren | | | |
| Funciones especiales declaradas | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| Reservas por motor | | | |
| Bytes poseídos, total | | | |
| Texto de `game.o` | | | |
| Texto / datos del binario | | | |
| Trazas coincidentes | | | |

### 6.3 El límite de la aceptación

- Qué afirma la aceptación:
- Qué no afirma:
- Por qué «se puede mover» no es «se puede usar después de mover»:
- Qué tiene que ver el estado compartido del generador con la propiedad:
- Qué lección posterior se ocupa del estado del generador:
- Qué lección posterior se ocupa del contrato de error:
- La frase de aceptación precisa de esta lección:

## 7. El modelo de propiedad y sus límites

| Pregunta | Respuesta |
|---|---|
| ¿Quién posee el tablero? | |
| ¿Quién posee cada caballo? | |
| ¿Quién posee el estado del generador? | |
| ¿Qué rechaza el compilador? | |
| ¿Qué permite el compilador? | |
| ¿Qué significa una copia del motor? | |
| ¿Qué significa un movimiento del motor? | |
| ¿Qué contiene un motor movido desde? | |
| ¿Pueden dos motores compartir un tablero? | |
| ¿Pueden dos motores compartir el generador? | |
| ¿Qué compra `unique_ptr`? | |
| ¿Qué cuesta en el código? | |
| ¿Qué cuesta en el código objeto? | |
| ¿Qué cuesta en ejecución? | |
| ¿Qué tiene ya la versión con miembros por valor? | |

- Qué elimina el cambio de propiedad del motor:
- Qué traslada a un tipo de biblioteca:
- Qué no elimina:
- Qué lección posterior se ocupa del estado compartido:
- Qué lección posterior se ocupa del error del movido desde:

## 8. Explicar

Una frase, sin las palabras *puntero inteligente*, *propiedad*, *ciclo de vida*, *RAII*,
*destructor*, *mejor*, *peor*, *limpio*, *seguro* o *moderno*:

>

Un párrafo:

- Qué deciden los tres miembros:
- Qué deciden las líneas 46–47:
- Qué deciden las líneas 48–49:
- Qué demuestra el `grep` de `new`/`delete`:
- Qué demuestra la salida de `nm`:
- Qué demuestra la sonda contadora:
- Qué demuestran las trazas:
- Qué demuestra la caída del movido desde:
- Cuál es el coste:
- Qué tenía ya la versión con miembros por valor:
- Qué lección posterior se ocupa de la abstracción algorítmica:
- Qué lección posterior se ocupa del generador compartido:

Las cuatro cosas que mantuve separadas:

## 9. Comprobación de aceptación

- Localicé los tres miembros que poseen y sus líneas: (sí / no)
- Localicé las dos operaciones de copia eliminadas y las dos de movimiento por defecto: (sí / no)
- Confirmé que el `grep` de `new`/`delete` crudos devuelve solo las dos líneas `= delete`: (sí / no)
- Confirmé que `game.o` sigue referenciando `operator new` y `operator delete`: (sí / no)
- Compilé la sonda con los tres `static_assert`: (sí / no)
- Reproduje el error de construcción de copia y leí su línea: (sí / no)
- Reproduje el error de asignación de copia y leí su línea: (sí / no)
- Moví un motor y el motor de destino reprodujo la semilla 1: (sí / no)
- Ejecuté el motor movido desde y registré el resultado: (sí / no)
- Conté tres reservas y tres liberaciones para un motor: (sí / no)
- Conté sesenta y sesenta para veinte motores: (sí / no)
- Medí el motor en 24 bytes y el tablero en 256: (sí / no)
- Reproduje las 20 trazas en la línea base sin modificar: (sí / no)
- Registré que no había detector de fugas y nombré mi evidencia: (sí / no)
- Nombré el generador compartido y el estado movido desde como huecos: (sí / no)
- No edité ningún archivo de código rastreado: (sí / no)

Si edité un archivo de código rastreado, qué y por qué:

## 10. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué no puedo explicar todavía:
- Una cosa que volvería a leer:
- El número que llevaré a `construction-17`:
- La predicción más lejana de la observación:
- Qué esperaba que el compilador rechazara y permitió:
- Qué esperaba que el compilador permitiera y rechazó:
- Qué decide el cambio de propiedad que yo esperaba que decidiera el motor:
- Qué sigue decidiendo el motor que yo esperaba que decidiera el tipo:
- Qué compra el cambio que los miembros por valor no podían ofrecer:
- Qué cede a cambio:
- Qué demostró la sonda contadora que las trazas no podían:
- El cambio que haría después para volver a probar este diseño:
- Siguiente paso:

## 11. Si falta una línea base o no es fiel

Rellena esto solo si faltaba un archivo, una compilación falló por una razón del entorno, un diff no
estaba vacío, una sonda contradijo el comportamiento documentado, o se pidió un detector de fugas y
no pudo ejecutarse.

- Archivo faltante, error de compilación, diff, contradicción o detector no disponible:
- Salida exacta del comando:
- Qué hice en lugar de inventar un recuento:
- Cuándo lo reintentaré:

## 12. Autocomprobación

- [ ] Inventarié los tres dueños y las cuatro funciones especiales con sus líneas.
- [ ] Comparé la línea base con propiedad con `construction-15` y nombré las ocho líneas añadidas.
- [ ] Predije la tabla de tamaños, la tabla de reservas y la prueba de cambio antes de medir.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Compilé la sonda con los tres `static_assert`.
- [ ] Reproduje ambos errores de copia y leí la línea que nombra cada uno.
- [ ] Moví un motor y coincidí con la semilla 1 usando el objeto de destino.
- [ ] Ejecuté el motor movido desde y registré el resultado.
- [ ] Conté reservas y liberaciones para un motor y para veinte.
- [ ] Medí el motor en 24 bytes y el tablero en 256.
- [ ] Separé la evidencia estructural de la evidencia empírica.
- [ ] Registré que no había detector de fugas, sin afirmar que corrió alguno.
- [ ] Nombré el generador compartido y el estado movido desde como huecos.
- [ ] Separé quién posee, qué rechaza el compilador, qué permite y qué puede ver una ejecución.
- [ ] Mi explicación de una frase evita las palabras de mecanismo y las palabras de veredicto.
- [ ] No edité ningún archivo de código rastreado.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §10.
