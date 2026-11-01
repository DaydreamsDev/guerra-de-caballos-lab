# construction-19 — Evidencia

> [Inglés](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro del aprendiz. Escribe en español.

Este registro compara dos layouts para el mismo tablero. Pregunta qué guarda cada uno, dónde viven
los bytes, cuántas peticiones hace el programa al heap y qué puede ver una ejecución. La propiedad y
el estilo de métodos se mantienen fijos en la comparación; solo cambia la representación del
tablero. Conserva las predicciones. La diferencia entre tu predicción y la observación es el
resultado, sea cero o no.

## 1. Preparación de la ejecución

- Fecha:
- Rango de semillas: `1` a `10000`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Plataforma y arquitectura:
- Líneas base leídas el (fecha):
- ¿Registro de `construction-17` a mano? (sí / no)
- Los números que traje de `construction-17` (líneas / bytes del tablero / bytes del motor /
  reservas / bytes de texto / trazas):
- Directorio temporal que usé:
- ¿Edité algún archivo versionado? (sí / no)

## 2. Los dos layouts (antes de ejecutar)

### 2.1 Inventario

Completa esto desde el código, con números de línea, no de memoria.

| Pregunta | construction-17 | construction-19 |
|---|---|---|
| Tipo del tablero | | |
| Almacenamiento de visitadas | | |
| Bits por casilla visitada | | |
| Bytes de visitadas | | |
| Almacenamiento de ocupantes | | |
| Bytes de ocupantes | | |
| Bytes del tablero | | |
| Miembros del motor | | |
| Bytes del motor | | |
| Reservas por partida | | |
| Operaciones del tablero | | |
| Copia / movimiento del motor | | |

- Archivo y línea de la declaración del tablero en 17:
- Archivo y línea de la declaración del tablero en 19:
- Archivo y línea de los miembros del motor en 17:
- Archivo y línea de los miembros del motor en 19:
- Los dos archivos que difieren entre las líneas base:
- Los diez archivos idénticos byte a byte:
- Qué prueban los diez archivos idénticos:

### 2.2 El lado fijo

- Las tres declaraciones de miembros del motor en 17:
- Las tres declaraciones de miembros del motor en 19:
- Las llamadas a `make_unique` en 17:
- Las llamadas a `make_unique` en 19:
- Los métodos del tablero en 17:
- Los métodos del tablero en 19:
- ¿`Board` es una clase en ambas? (sí / no)
- ¿El tablero sigue siendo propiedad de un `unique_ptr` en 19? (sí / no)
- Qué líneas prueban que la propiedad no cambió:

### 2.3 Conteos

- Encabezados:
- Unidades de traducción (juego):
- Unidades de traducción (con banco de pruebas):
- Definiciones de clase completas:
- Cuerpos de función con nombre escritos a mano:
- Líneas de código (juego):
- Líneas de código (total):
- Líneas de `board.h`:
- Líneas de `game_engine.h`:
- Líneas del banco de pruebas:
- Archivos que difieren de `construction-17`:
- Archivos idénticos byte a byte a `construction-17`:

Comparado con `construction-17`:

- Encabezados: ______ → ______
- Unidades de traducción (juego): ______ → ______
- Definiciones de clase completas: ______ → ______
- Cuerpos de función con nombre: ______ → ______
- Líneas de código (juego): ______ → ______
- Líneas de código (total): ______ → ______
- Líneas de `board.h`: ______ → ______
- Líneas de `game_engine.h`: ______ → ______
- Archivos que difieren: ______

### 2.4 Preguntas de costo de diseño

| Pregunta | Respuesta |
|---|---|
| ¿Qué ahorra el bitboard? | |
| ¿Qué agrega el arreglo de ocupantes? | |
| ¿Por qué el tablero de 19 es más grande que el de 17? | |
| ¿Por qué el motor de 19 mide lo mismo que el de 17? | |
| ¿Qué sigue poseyendo el motor, y cómo? | |
| ¿Qué invariante agrega el arreglo de ocupantes? | |
| ¿Qué clase mantiene privado ese invariante? | |
| ¿Qué debe saber quien lee `was_visited` en 19? | |
| ¿Qué no puede observar la transcripción? | |

- El invariante que el tablero debe mantener en cada movimiento que continúa la partida:
- Los métodos que lo mantienen:
- Qué gana el bitboard frente al arreglo anidado:
- Qué pierde el bitboard frente al arreglo anidado:

## 3. Predicción (escrita antes de medir)

### 3.1 La tabla del layout

| Pregunta | Mi predicción |
|---|---|
| `sizeof(Board<8, 8>)` en 17 | |
| `sizeof(Board<8, 8>)` en 19 | |
| `sizeof(GameEngine<8, 8>)` en 17 | |
| `sizeof(GameEngine<8, 8>)` en 19 | |
| Reservas por partida en 17 | |
| Reservas por partida en 19 | |
| Reservas totales para 10,000 partidas en 17 | |
| Reservas totales para 10,000 partidas en 19 | |
| Archivos que difieren | |
| Trazas que coinciden, de 20, en cada una | |

- La fila que espero equivocar, y por qué:
- Qué espero que incluya `sizeof(GameEngine<8, 8>)` en cada layout:
- Por qué espero que el tablero crezca o se achique:
- Por qué espero que el tamaño del motor y el número de reservas se muevan, o no:

### 3.2 La tabla del perfil

| Pregunta | Mi predicción |
|---|---|
| Tiempo de 10,000 partidas en 17 | |
| Tiempo de 10,000 partidas en 19 | |
| Cuál es más rápida | |
| Diferencia en milisegundos | |
| Texto de `game.o` en 17 y 19 | |
| Texto del binario en 17 y 19 | |
| Datos del binario en 17 y 19 | |
| Líneas de código en 17 y 19 | |

- Por qué espero que el tiempo de reloj se mueva, o no:
- El número que espero que me sorprenda:
- Qué espero que haga el texto objeto, y por qué:
- Qué espero que haga el número de reservas, y por qué:

### 3.3 La prueba de explicación

| Pregunta | Mi predicción |
|---|---|
| ¿Cambiará el tamaño del tablero? | |
| ¿Cambiará el tamaño del motor? | |
| ¿Cambiará el número de reservas? | |
| ¿Cambiará el tiempo de reloj? | |
| ¿Por qué? | |
| ¿Qué ve la transcripción? | |
| ¿Qué incluye `sizeof(GameEngine<8, 8>)`? | |
| ¿Qué no incluye? | |

- Qué espero que mida el tablero, y por qué:
- Qué espero que mida el tiempo, y por qué:
- Qué me diría que confundí "representación distinta" con "más rápido":
- Qué me diría que confundí "tablero más grande" con "más memoria total":

## 4. Ejecución — trazas y tamaños

### 4.1 Trazas

- Comando de compilación para 17:
- Comando de compilación para 19:
- Estado de salida para 17:
- Estado de salida para 19:
- Diferencia contra `../../../common/traces/seed-0001.txt` para 17:
- Diferencia contra `../../../common/traces/seed-0001.txt` para 19:
- Semillas que coincidieron, de 20, en 17:
- Semillas que coincidieron, de 20, en 19:

### 4.2 Tamaños

- Salida de la sonda `sizeof` para 17:
- Salida de la sonda `sizeof` para 19:
- `sizeof(Board<8, 8>)` en 17:
- `sizeof(Board<8, 8>)` en 19:
- `sizeof(GameEngine<8, 8>)` en 17:
- `sizeof(GameEngine<8, 8>)` en 19:
- `sizeof(Knight)`:
- `sizeof(Position)`:
- `sizeof(MoveCandidate)`:
- `sizeof(std::array<MoveCandidate, 8>)`:

### 4.3 Objeto y binario

- Texto de `game.o` en 17 y 19:
- Texto del binario enlazado en 17 y 19:
- Datos del binario enlazado en 17 y 19:
- BSS del binario enlazado en 17 y 19:
- Qué prueba la diferencia de texto:
- Qué prueba la diferencia de datos:
- Qué no prueba la diferencia de texto:

## 5. Perfil — 10,000 partidas

### 5.1 Tiempo de reloj

- Comando del banco de pruebas para 17:
- Comando del banco de pruebas para 19:
- Opciones del compilador:
- Número de ejecuciones por lado:
- Tiempos de 10,000 partidas para 17:
- Tiempos de 10,000 partidas para 19:
- Rango para 17:
- Rango para 19:
- Qué lado es más rápido:
- ¿La diferencia está fuera del ruido entre ejecuciones? (sí / no)

### 5.2 Reservas

- Comando de la sonda de reservas:
- Reservas para 10,000 partidas en 17:
- Reservas para 10,000 partidas en 19:
- Reservas por partida en 17:
- Reservas por partida en 19:
- A qué corresponde el conteo de 17:
- A qué corresponde el conteo de 19:
- Por qué el conteo no cambió:
- ¿`std::string` reservó para los nombres de rol? (sí / no)

### 5.3 Observaciones

| Observación | Predicho | Observado | Diferencia |
|---|---|---|---|
| `sizeof(Board<8, 8>)` en 17 | | | |
| `sizeof(Board<8, 8>)` en 19 | | | |
| `sizeof(GameEngine<8, 8>)` en 17 | | | |
| `sizeof(GameEngine<8, 8>)` en 19 | | | |
| Reservas en 17 | | | |
| Reservas en 19 | | | |
| Tiempo de 10,000 partidas en 17 | | | |
| Tiempo de 10,000 partidas en 19 | | | |
| Texto de `game.o` en 17 | | | |
| Texto de `game.o` en 19 | | | |
| Trazas coincidentes en 17 | | | |
| Trazas coincidentes en 19 | | | |

- El número que me sorprendió:
- Qué hice después en lugar de ajustar la predicción:

## 6. Comparar

### 6.1 La tabla

| Métrica | construction-17 | construction-19 | Diferencia |
|---|---|---|---|
| Líneas de código (juego) | | | |
| Líneas de código (total) | | | |
| Líneas de `board.h` | | | |
| Líneas de `game_engine.h` | | | |
| `sizeof(Board<8, 8>)` | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| `sizeof(Knight)` | | | |
| Reservas por partida | | | |
| Reservas por 10,000 partidas | | | |
| Tiempo de 10,000 partidas | | | |
| Texto de `game.o` | | | |
| Texto del binario | | | |
| Datos del binario | | | |
| Trazas | | | |

### 6.2 Qué se movió

- La representación del tablero:
- El tamaño del tablero:
- El texto objeto:
- El código fuente:
- Por qué el tablero creció 32 bytes:
- Qué ahorró el bitboard:
- Qué agregó el arreglo de ocupantes:

### 6.3 Qué no se movió

- El tamaño del motor:
- El número de reservas:
- El contrato de propiedad:
- El estilo de métodos:
- La transcripción:
- Las reglas:
- El generador aleatorio:
- Los diez archivos idénticos byte a byte:
- El tiempo de reloj:
- Qué se niega a afirmar el resultado plano:

### 6.4 Qué cuesta

- Tablero más grande:
- Más instrucciones:
- Acceso a casillas menos legible:
- Invariante de ocupantes que mantener:
- Más código fuente:
- Ninguna ganancia en reservas ni en velocidad:
- Una cosa más que debe saber quien lee:

## 7. Explicar

Una frase, sin usar las palabras *caché*, *struct-of-arrays*, *array-of-structs*, *orientado a
datos*, *mejor*, *más rápido*, *más lento* u *optimización*:

>

Un párrafo:

- Qué decide el layout:
- Qué decide el bitboard:
- Qué decide el arreglo de ocupantes:
- Qué quedó fijo:
- Qué prueban los tamaños:
- Qué prueba el número de reservas:
- Qué prueba el tiempo de reloj:
- Qué prueba el código objeto:
- Qué prueba la transcripción:
- Qué no prueba la lección:
- Qué lección posterior se ocupa de puertos y adaptadores:

Las cuatro cosas que mantuve separadas:

## 8. Comprobación de aceptación

- Comparé los dos layouts campo por campo: (sí / no)
- Nombré los dos archivos que difieren y los diez que no: (sí / no)
- Confirmé que la propiedad y el estilo de métodos no cambian: (sí / no)
- Predije las tablas de layout y de perfil antes de medir: (sí / no)
- Medí `sizeof(Board<8, 8>)` en 256 y 288: (sí / no)
- Medí `sizeof(GameEngine<8, 8>)` en 24 en ambas: (sí / no)
- Medí 30,000 reservas en ambas: (sí / no)
- Ejecuté 10,000 partidas en ambos lados y registré los milisegundos: (sí / no)
- Ejecuté cada lado al menos tres veces: (sí / no)
- Reproduje las 20 trazas en ambas líneas base: (sí / no)
- Medí el texto de `game.o` en 4,656 y 4,961: (sí / no)
- Medí el texto del binario en 9,668 y 9,972: (sí / no)
- Expliqué por qué el tablero creció 32 bytes: (sí / no)
- Expliqué por qué el tamaño del motor y el número de reservas no se movieron: (sí / no)
- Expliqué por qué el tiempo de reloj no se movió: (sí / no)
- Nombré el invariante de ocupantes y el costo de legibilidad del bitboard: (sí / no)
- No edité ningún archivo de código versionado: (sí / no)

Si edité un archivo de código versionado, qué y por qué:

## 9. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué todavía no puedo explicar:
- Una cosa que volvería a leer:
- El número que llevaré a `construction-20`:
- La predicción que quedó más lejos de la observación:
- Qué esperaba que el compilador optimizara y no lo hizo:
- Qué esperaba que comprara el cambio de layout y no lo compró:
- Qué no me pudo decir la transcripción:
- Qué me dijo el tamaño del tablero que el tiempo no me dijo:
- Qué me dijo el tiempo que el tamaño del tablero no me dijo:
- El cambio de layout que probaría después, y por qué:
- El cambio de layout que rechazaría, y por qué:
- El siguiente cambio que haría para volver a probar este diseño:
- Siguiente paso:

## 10. Si falta una línea base o no es fiel

Completa esto solo si faltó un archivo, si una compilación falló por una razón del entorno, si una
diferencia no quedó vacía o si una medición contradijo el comportamiento documentado.

- Archivo faltante, error de compilación, diferencia o contradicción:
- Salida exacta del comando:
- Qué hice en lugar de inventar un conteo:
- Cuándo volveré a intentarlo:

## 11. Autoevaluación

- [ ] Inventarié los dos layouts con referencias de archivo y línea.
- [ ] Confirmé que los miembros `unique_ptr` y las llamadas a `make_unique` no cambian.
- [ ] Confirmé que `Board` es una clase con métodos en ambas líneas base.
- [ ] Predije las tablas de layout y de perfil antes de medir.
- [ ] Reproduje las 20 trazas en ambas líneas base.
- [ ] Medí el tablero, el motor, el caballo y la posición.
- [ ] Conté las reservas: 30,000 en ambas.
- [ ] Ejecuté 10,000 partidas en ambos lados y registré el rango.
- [ ] Medí `game.o` y el binario enlazado.
- [ ] Separé el tamaño del tablero del tamaño del motor.
- [ ] Separé el cambio de representación del número de reservas.
- [ ] Expliqué por qué el tablero creció y por qué el motor no.
- [ ] Expliqué por qué el tiempo de reloj quedó plano.
- [ ] Mi explicación de una frase evita las palabras de mecanismo y de veredicto.
- [ ] No edité ningún archivo de código versionado.

Sin puntaje. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §9.
