# construction-13 — Evidencia

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro de quien aprende. Escribe en español.

Este registro sigue la casilla desde la clase base hasta una parte: qué posee `Knight`, qué reenvía,
qué tipo puede nombrar el bucle, y qué ahorra y qué cede el objeto. Conserva las predicciones. El
hueco entre tu predicción y la observación es el resultado, sea cero o no.

## 1. Preparación de la ejecución

- Fecha:
- Semilla: `1`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Línea base leída el (fecha):
- ¿Registro de `construction-12` a mano? (sí / no)
- El número que traje de `construction-12` (declaraciones públicas / definiciones / trazas):
- Directorio de trabajo que usé:

## 2. La composición (antes de ejecutar)

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

Comparado con `construction-12`:

- Declaraciones públicas: ______ → ______
- Definiciones: ______ → ______
- Definiciones de clase completas: ______ → ______
- Cabeceras: ______ → ______
- Unidades de traducción: ______ → ______
- Declaraciones de datos privados: ______ → ______
- Declaraciones del tablero que cambiaron:
- Declaraciones virtuales eliminadas:
- `override` eliminados:
- Métodos de reenvío añadidos:
- El tipo que el bucle puede nombrar:

### 2.2 Mapa de líneas

`position.h`:

| Líneas | Contenido |
|---|---|
| | |

`knight.h`:

| Líneas | Contenido |
|---|---|
| | |

`knight.cpp`:

| Líneas | Contenido |
|---|---|
| | |

- Línea del miembro `Position`:
- Línea del miembro `roleName`:
- Línea de `row`:
- Línea de `column`:
- Línea de `set_position`:
- Línea de `role_name`:
- Línea de la etiqueta `public:` en `knight.h`:
- Línea de la etiqueta `private:` en `knight.h`:
- Líneas que construyen un `Knight` en `run_game`:
- Línea que incluye `knight.h` en `game.cpp`:

### 2.3 Roles y delegación

| Rol | Operación | La responde | Qué pregunta el bucle |
|---|---|---|---|
| | | | |

- Número de operaciones públicas en `Knight`:
- Operaciones que reenvían a `Position`:
- La operación que no reenvía, y por qué:
- Operaciones que son `const`:
- Qué puede llamar un `const Knight *`:
- La única operación que escribe:
- Datos públicos en `Knight` o `Position`:
- Número de puntos de llamada en `game.cpp`:
- Las cuatro operaciones que preguntan esos puntos:
- Número de firmas auxiliares de `game.cpp` que nombran `Knight`:
- Cabeceras con `virtual`, `override` o `: public`:
- Qué se rompería si `Position::rowValue` fuera público:

## 3. Predicción (escrita antes de medir)

### 3.1 La tabla de delegación

Cada expresión nombra un `Knight`; no hay tipo base detrás del cual esconderse.

| Expresión | Qué función se ejecuta | Salida prevista | Confianza |
|---|---|---|---|
| `playerKnight.set_position(3, 4)` | | | |
| `playerKnight.row()` | | | |
| `playerKnight.column()` | | | |
| `playerKnight.role_name()` | | | |
| `asKnight.row()` a través de `const Knight &` | | | |
| `copy.set_position(5, 6)` después de `Knight copy = playerKnight` | | | |

- La fila que espero equivocar, y por qué:
- Qué espero que comparta la copia con el original:

### 3.2 La prueba de cambio

| Pregunta | Mi predicción |
|---|---|
| Archivos que voy a eliminar | |
| Archivos que voy a añadir | |
| Líneas que voy a cambiar en `game.cpp` | |
| Funciones cuya firma cambia | |
| Líneas de lógica que voy a cambiar (no nombres) | |
| Semillas que coinciden, de 20, después del cambio | |
| Conceptos nuevos que exige el cambio | |

- La predicción de la que estoy menos seguro, y por qué:

### 3.3 La sonda de tamaño e indirección

| Pregunta | Mi predicción |
|---|---|
| `sizeof(Position)` | |
| `sizeof(Knight)` | |
| `sizeof(KnightPiece)` | |
| Bytes ahorrados por objeto | |
| Líneas de `nm -C knight.o` con `vtable` o `typeinfo` | |
| Llamadas indirectas en `game12.o` | |
| Llamadas indirectas en `game13.o` | |
| Texto de `game.o`: ¿crece, se reduce o queda igual? | |

- Qué espero que cambie el miembro en el código objeto, en una frase:

## 4. Ejecución — línea base y sonda de delegación

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

| Observación | Previsto | Observado | Hueco |
|---|---|---|---|
| `role` a través de `Knight` | | | |
| `role` a través de `const Knight &` | | | |
| Original después de mover la copia | | | |
| `sizeof(Position)` | | | |
| `sizeof(Knight)` | | | |
| Texto de `game.o` | | | |

- La línea que me sorprendió:
- Qué hice después en lugar de ajustar la predicción:

## 5. Comparar — qué prueba la delegación

- Salida de `grep -rn 'virtual\|override\|: public' code/`:
- Número de líneas que mencionan sintaxis de herencia:
- Salida de `nm -C knight.o | grep -E 'vtable|typeinfo'`:
- Número de firmas auxiliares de `game.cpp` que nombran `Knight`:
- Cuáles están dentro del espacio de nombres anónimo:

| Líneas | Función | Llamadas |
|---|---|---|
| | | |

- Total de puntos de llamada:
- Total de líneas que contienen un punto de llamada:
- `sizeof(Position)`:
- `sizeof(Knight)`:
- `sizeof(KnightPiece)`:
- Qué guarda el objeto:
- Qué guarda la clase:
- Qué prueba la sonda sobre la llamada reenviada:
- Qué no prueba la sonda:

### 5.1 La sonda de renombrado

- Archivos cambiados en la copia temporal:
- Nombres de rol antiguos:
- Nombres de rol nuevos:
- Líneas que difieren, de 89:
- Qué clases de línea difieren (`who=`, `move=`, `trapped=`, `winner=`):
- Qué líneas son idénticas:
- Qué prueba esto sobre `role_name()`:
- Qué prueba esto sobre el refactor:

### 5.2 Generación de código

- Llamadas indirectas en `game12.o`:
- Llamadas indirectas en `game13.o`:
- Reubicaciones de llamada directa a `Knight::` en `game13.o`:
- Por qué el conteo directo es mayor que el de puntos de llamada:
- Qué no prueba la ausencia de tabla sobre la velocidad:

## 6. La prueba de cambio en una copia temporal

- Ruta de la copia temporal:
- Archivos eliminados:
- Archivos añadidos:
- Archivos editados:
- Líneas añadidas en `game.cpp`:
- Líneas eliminadas en `game.cpp`:
- Bloques del diff:
- Líneas de lógica cambiadas:
- Archivos versionados editados: (sí / no)

### 6.1 Eliminar la interfaz

- Archivos eliminados:
- Líneas eliminadas:
- Declaraciones virtuales eliminadas:
- `override` eliminados:
- Qué seguía incluyendo `piece.h`:

### 6.2 Añadir la parte

- Ruta de la cabecera:
- Ruta de la implementación:
- Líneas en la cabecera:
- Líneas en la implementación:
- Las operaciones que expone:
- Su estado privado:
- Su estado por defecto:

### 6.3 Añadir el todo

- Ruta de la cabecera:
- Ruta de la implementación:
- Líneas en la cabecera:
- Líneas en la implementación:
- Operaciones que reenvían:
- Operación que no reenvía:
- Qué inicializa el constructor:

### 6.4 Renombrar el bucle

- Comando o script que usé:
- Salida de `grep -n 'Piece' game.cpp`:
- Líneas añadidas:
- Líneas eliminadas:
- Bloques:
- Clases de línea que cambiaron:
- Reglas que cambiaron:

### 6.5 Compilar y trazas

- Comando de compilación:
- Semillas que coincidieron, de 20:
- Diff contra la transcripción de la línea base sin modificar:

### 6.6 El hueco: la sustituibilidad

- La sustitución de `construction-12` que ya no compila:
- Por qué no tiene dónde engancharse:
- Opción 1 (duplicar el bucle): funciones que se duplicarían:
- Opción 2 (restaurar una interfaz): qué lección es:
- Opción 3 (parametrizar el bucle): qué lección es:
- La frase de aceptación precisa de esta lección:

## 7. La composición y sus límites

| Pregunta | Respuesta |
|---|---|
| ¿Qué posee `Knight`? | |
| ¿Qué reenvía? | |
| ¿Qué responde solo? | |
| ¿Dónde vive la lógica de posición? | |
| ¿Qué tipo guarda el bucle? | |
| ¿Cuántas firmas nombran ese tipo? | |
| ¿Qué cuesta un tipo nuevo con el mismo comportamiento? | |
| ¿Qué cuesta una regla de movimiento nueva? | |
| ¿Qué cuesta el miembro por objeto? | |
| ¿Qué ahorra por objeto? | |
| ¿Qué ahorra en el binario? | |
| ¿Qué cuesta en el código? | |
| ¿Puede el bucle aceptar un segundo tipo sin ediciones? | |

- Qué elimina la composición del bucle:
- Qué traslada al bucle:
- Qué no elimina:
- Qué lección posterior recupera un bucle para dos tipos:
- Qué lección posterior posee el ciclo de vida:

## 8. Explicar

Una frase, sin las palabras *composición*, *herencia*, *delegación*, *clase*, *objeto*, *virtual*,
*puntero*, *compilador*, *mejor*, *peor*, *limpio*, *flexible*, *reutilizable* ni *elegante*:

>

Un párrafo:

- Qué fija la composición:
- Qué lo impone:
- Qué la hace distinta de `construction-12`:
- Qué hacen los 19 puntos de llamada:
- Qué prueban las 20 trazas:
- Qué no prueban las trazas:
- Qué cuesta la prueba de cambio:
- Qué no cubre la composición:
- Qué ahorra:
- Qué cuesta:
- Qué lección posterior se apoya en esta:

Las cuatro cosas que mantuve separadas:

## 9. Comprobación de aceptación

- Identifiqué el miembro `Position` y los tres métodos de reenvío: (sí / no)
- Conté los 19 puntos de llamada y los asigné a funciones: (sí / no)
- Comprobé que ninguna cabecera contiene `virtual`, `override` ni `: public`: (sí / no)
- Nombré las diez firmas auxiliares que mencionan `Knight`: (sí / no)
- Predije la tabla de delegación y observé los valores reenviados: (sí / no)
- Reproduje las 20 trazas en la línea base sin modificar: (sí / no)
- Compilé la sonda de delegación y vi los valores reenviados: (sí / no)
- Medí `sizeof(Position)` y `sizeof(Knight)`: (sí / no)
- Confirmé que copiar un caballo copia su posición: (sí / no)
- Eliminé los archivos de interfaz y añadí `Position` y `Knight` en una copia temporal: (sí / no)
- Renombré el bucle con cero cambios de reglas: (sí / no)
- Obtuve 20 de 20 trazas después del refactor: (sí / no)
- Conté 13 llamadas indirectas en `game12.o` y 0 en `game13.o`: (sí / no)
- Confirmé que `nm` no muestra tabla virtual ni información de tipo para `Knight`: (sí / no)
- Nombré la sustituibilidad como límite de este diseño: (sí / no)
- Expliqué por qué no hace falta un destructor virtual: (sí / no)
- No edité ningún archivo de código versionado: (sí / no)

Si edité un archivo de código versionado, cuál y por qué:

## 10. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué no puedo explicar todavía:
- Una cosa que volvería a leer:
- El número que llevaré a `construction-14`:
- El punto de llamada que más me sorprendió:
- El tipo que esperaba que siguiera oculto, y dónde aparece en su lugar:
- Qué compra el miembro que la clase base no podía ofrecer:
- Qué cede:
- Qué probó la prueba de cambio que las trazas no podían:
- El cambio que haría después para volver a probar este diseño:
- Siguiente paso:

## 11. Si falta una línea base o no es fiel

Rellena esto solo si faltaba un archivo, una compilación falló por una razón del entorno, un diff no
estaba vacío, o la sonda contradijo el comportamiento documentado de una forma que no pudiste
explicar.

- Archivo ausente, error de compilación, diff o contradicción:
- Salida exacta del comando:
- Qué hice en lugar de inventar un conteo:
- Cuándo volveré a intentarlo:

## 12. Autocomprobación

- [ ] Inventarié cada cabecera y cada definición con su archivo.
- [ ] Localicé el miembro, los reenvíos y el estado privado por línea.
- [ ] Comparé `KnightPiece` con `Knight` y `Position` y nombré qué expone cada uno.
- [ ] Predije la tabla de delegación, la prueba de cambio y la sonda de tamaño antes de medir.
- [ ] Reproduje las 20 trazas en la línea base sin modificar.
- [ ] Compilé la sonda de delegación y vi los valores reenviados a través de `const Knight &`.
- [ ] Medí el coste del miembro en el objeto y en el binario.
- [ ] Confirmé que copiar un caballo copia su posición.
- [ ] Sustituí la herencia por un miembro y un reenvío en una copia temporal.
- [ ] Obtuve 20 de 20 trazas después del refactor y cambié cero reglas.
- [ ] Conté 13 llamadas indirectas en `game12.o` y 0 en `game13.o`.
- [ ] Confirmé que `nm` no muestra tabla virtual ni información de tipo para `Knight`.
- [ ] Nombré la sustituibilidad como límite de este diseño.
- [ ] Separé qué puede nombrar el bucle, qué decide la parte, qué emite el compilador y qué puede
      ver una ejecución.
- [ ] Mi frase de explicación evita las palabras del mecanismo y las del veredicto.
- [ ] No edité ningún archivo de código versionado.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §10.
