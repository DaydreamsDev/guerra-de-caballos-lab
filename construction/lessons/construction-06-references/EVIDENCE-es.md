# construction-06 — Evidencia

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro propio de quien aprende. Escribe en español.

Este registro compara dos formas de nombrar el mismo registro de caballo: una copia y un alias.
Conserva las dos columnas. Un número de un solo archivo no responde a la pregunta, y un empate es un
resultado, no una diferencia que falte.

## 1. Preparación de la ejecución

- Fecha:
- Semilla: `1`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Línea base con referencias leída el (fecha):
- Línea base con arreglo de estructuras leída el (fecha):
- ¿Tengo el registro de `construction-05` a mano? (sí / no)
- Los dos conteos que traje de `construction-05` (cambios de firma / sitios de llamada):

## 2. Inventario de alias (antes de la ejecución)

### 2.1 Funciones que nombran un caballo

Complétalo desde el código, con números de línea, no de memoria.

| Función | Línea | Parámetros `Knight` | Forma | Lee | Escribe |
|---|---|---|---|---|---|
| | | | | | |

Recuentos:

- Funciones con un parámetro `Knight &` mutable:
- Funciones con un parámetro `const Knight &`:
- Funciones con un parámetro `Knight` por valor:
- Funciones que devuelven un alias de caballo:
- Parámetros de caballo mutables, total:
- Parámetros de caballo de solo lectura, total:
- Parámetros de caballo por valor, total:

### 2.2 Las cinco formas

| Forma | Ejemplo en este archivo | Qué permite | Qué prohíbe |
|---|---|---|---|
| parámetro por valor | | | |
| parámetro de referencia | | | |
| parámetro de referencia `const` | | | |
| retorno de referencia | | | |
| alias local | | | |

- Qué es una referencia a nivel de lenguaje:
- Qué no es una referencia:
- Qué dice el sitio de llamada sobre la mutación:

### 2.3 Lo que ya estaba

Enumera cada alias de caballo que existe en `construction-05` antes de esta lección. Usa números de
línea.

| Función | Línea | Forma | Qué hace |
|---|---|---|---|
| | | | |

- Las dos líneas que desaparecieron de `playTurn`, y qué las sustituyó:
- La línea que pasó de mutable a `const`, y por qué:
- Qué añade esta lección más allá de la sintaxis:
- Qué no añade:

### 2.4 El diff

- Líneas añadidas:
- Líneas eliminadas:
- Líneas netas:
- Definiciones de función antes / después:
- Parámetros antes / después:
- Firmas existentes que cambiaron de tipo:
- Sitios de llamada existentes que cambiaron un argumento:
- Sitios de llamada nuevos:
- Trazas que difieren, de 20:

## 3. Predicción (escrita antes de medir)

### 3.1 El cambio a por valor

El cambio: eliminar el `&` después de `Knight` en la firma de `moveKnight`.

| Pregunta | Mi predicción | Confianza | Razón |
|---|---|---|---|
| ¿Compila con `-Wall -Wextra -Werror`? | | | |
| Clase de diagnóstico, si aparece | | | |
| Primera línea distinta de la semilla 1 | | | |
| ¿Sigue apareciendo la línea `move=`? | | | |
| Siguiente encabezado `turn=` de ese caballo | | | |
| Movimientos en la semilla 1 (oro: 21) | | | |
| Semillas que difieren, de 20 | | | |
| Cambios de firma | | | |
| Sitios de llamada editados | | | |
| Caracteres eliminados | | | |
| ¿El juego termina? | | | |

- Por qué la línea del movimiento puede sobrevivir a la mutación fallida:
- Por qué el siguiente encabezado no puede:
- El resultado que espero fallar, y por qué:

### 3.2 Tipos de retorno del selector

| Cambio | ¿Compila? | Qué dice el compilador | Qué haría el programa |
|---|---|---|---|
| `selectActiveKnight` devuelve `Knight` | | | |
| `selectActiveKnight` devuelve `const Knight &` | | | |
| `selectOtherKnight` devuelve `Knight &` | | | |

- La línea receptora que usé para razonar sobre el vínculo:
- Qué cambio falla en el retorno y qué cambio falla en el alias:

### 3.3 Los límites

- ¿Puede una referencia ser nula?
- ¿Puede una referencia reasignarse después de inicializarse?
- ¿Qué hace `activeKnight = machineKnight;`?
- ¿El sitio de llamada muestra que el registro puede cambiar?
- ¿Qué pasa si una referencia sobrevive a su registro?
- ¿Cuál de estas cosas detecta el compilador?

## 4. Ejecución — ambas líneas base

- Comando de compilación de referencias:
- Comando de compilación del arreglo de estructuras:
- Estados de salida:
- Objetivo del diff: `../../../common/traces/seed-0001.txt`

Registra `vacío` o `diff` en cada celda de resultado.

| Comprobación | Comando | Resultado |
|---|---|---|
| Referencias vs oro | | |
| Arreglo de estructuras vs oro | | |
| Referencias vs arreglo de estructuras | | |

```text
<salida del diff, o "sin diferencias">
```

Semillas que coincidieron, de 20:

Si algún diff no está vacío, detente aquí y completa §11.

## 5. Romper la frontera

- Copia de trabajo:
- Firma antes:
- Firma después:
- No edité la línea base versionada con referencias: (sí / no)

### 5.1 Compilación con las opciones del proyecto

- Comando:
- Estado de salida:
- Clase de diagnóstico:
- Mensaje completo:

```text
<salida de la compilación>
```

- Qué dice el compilador que hace el parámetro:
- Por qué el compilador no puede rechazar el cambio por principio:
- Qué significaría la advertencia sin `-Werror`:

### 5.2 Compilar y ejecutar la observación

- Comando usado para degradar la advertencia:
- Estado de salida:
- Semillas que compilan:
- Comando usado para ejecutar la semilla 1:

```text
<primeras 20 líneas del diff, incluida la línea move= y el siguiente encabezado turn=>
```

- La línea `move=` que sigue apareciendo:
- El encabezado `turn=` que expone la copia:
- Por qué la línea del movimiento no es evidencia de mutación:
- Por qué el siguiente encabezado sí lo es:
- Movimientos en la semilla 1, referencia / por valor:
- Qué le hace el caballo obsoleto al resto del juego:
- La frase que conservaré sobre intención y prueba:

### 5.3 Los 20 resultados

| Semilla | Movimientos referencia | Movimientos por valor | Primera línea distinta | ¿Difiere? |
|---|---|---|---|---|
| 1 | | | | |
| 2 | | | | |
| 3 | | | | |
| 4 | | | | |
| 5 | | | | |
| 6 | | | | |
| 7 | | | | |
| 8 | | | | |
| 9 | | | | |
| 10 | | | | |
| 11 | | | | |
| 12 | | | | |
| 13 | | | | |
| 14 | | | | |
| 15 | | | | |
| 16 | | | | |
| 17 | | | | |
| 18 | | | | |
| 19 | | | | |
| 20 | | | | |

- Semillas que difieren:
- Semillas que coinciden:
- Qué dice el conteo sobre el radio de impacto de un carácter:

### 5.4 Restaurar

- Cómo restauré la referencia:
- Estado de salida de la compilación:
- Semillas que coincidieron después de restaurar, de 20:

### 5.5 Comprobación de la predicción

- Resultado de compilación predicho / observado:
- Primera línea predicha / observada:
- Conteo de semillas predicho / observado:
- Predicción más cercana, y por qué:
- Predicción más lejana, y qué se me escapó:

## 6. `const` es una vía de un solo sentido

- Copia de trabajo:
- No edité la línea base versionada con referencias: (sí / no)

### 6.1 Una escritura directa

- Línea añadida:
- Estado de salida de la compilación:
- Mensaje del compilador:

```text
<salida del error>
```

- Qué dice el mensaje sobre el alias:
- Dónde ocurre la comprobación:

### 6.2 La llamada equivocada

- Línea añadida:
- Estado de salida de la compilación:
- Mensaje del compilador:

```text
<salida del error>
```

- Por qué falla la llamada:
- A qué caballo protegió el compilador:
- Qué habría pasado en ejecución sin `const`:

### 6.3 Quitar `const`

- Tipo de retorno antes / después:
- Tipos de parámetro antes / después:
- Estado de salida de la compilación:
- ¿Compilan las dos líneas? (sí / no)
- Por qué no ejecuté esta variante:

### 6.4 Qué prueba el compilador

| Comprobación | Qué prueba el compilador | Qué solo prueba una ejecución |
|---|---|---|
| Escritura directa por un alias `const` | | |
| Pasar un alias `const` a `Knight &` | | |
| Escribir por una referencia mutable | | |
| Una copia que recibe una escritura | | |

- Cantidad de líneas que fallaron con `const`, y cantidad que compilaron sin él:
- Qué puede impedir un tipo que una traza no puede:
- Qué puede mostrar una traza que un tipo no puede:

## 7. Tipos de retorno del selector

Un cambio por copia. Registra el resultado exacto.

### 7.1 `selectActiveKnight` devuelve `Knight`

- Estado de salida de la compilación:
- Mensaje del compilador:

```text
<salida del error>
```

- Por qué el alias receptor lo rechaza:
- Qué se perdería incluso si compilara:

### 7.2 `selectActiveKnight` devuelve `const Knight &`

- Estado de salida de la compilación:
- Mensaje del compilador:

```text
<salida del error>
```

- Por qué el alias lo rechaza:
- Qué calificador se descarta:

### 7.3 `selectOtherKnight` devuelve `Knight &`

- Estado de salida de la compilación:
- Mensaje del compilador:

```text
<salida del error, si la hay>
```

- Qué impiden ya los parámetros:
- Qué cambiaría si los parámetros también perdieran `const`:
- Qué permitiría el programa entonces:

### 7.4 Qué permite cada forma

| Tipo de retorno | El llamador puede leer | El llamador puede escribir | Se vincula a un temporal | Puede ser nulo |
|---|---|---|---|---|
| `Knight` | | | | |
| `Knight &` | | | | |
| `const Knight &` | | | | |

- La forma que debe tener `selectActiveKnight`, y por qué:
- La forma que debe tener `selectOtherKnight`, y por qué:
- La forma que puede tener una función de impresión, y por qué:

## 8. Explicar

Una frase, sin las palabras *pointer*, *address*, *mejor*, *peor*, *limpio*, *limpia*, *bonito*,
*elegante*, *feo* ni *modular*:

>

Un párrafo:

- Qué decide un tipo de parámetro:
- Qué decide un tipo de retorno:
- Qué decide `const`, y dónde se comprueba:
- Qué dice el sitio de llamada sobre la mutación, y qué oculta:
- Qué localizan las referencias:
- Qué no localizan:
- Qué puede probar el compilador:
- Qué solo prueba una ejecución:
- Qué lección posterior corrige direcciones, nulos y reasignación:
- Qué lección posterior corrige límites y representación:
- Qué lecciones posteriores corrigen invariantes y acceso:
- Qué lección posterior corrige propiedad y ciclo de vida:

La distinción que mantuve separada:

## 9. Comprobación de aceptación

- Ejecuté las dos líneas base sin modificar: (sí / no)
- Obtuve tres diffs vacíos y 20 semillas coincidentes: (sí / no)
- Inventarié 17 parámetros de caballo: 9 mutables, 8 de solo lectura, 0 por valor: (sí / no)
- Identifiqué los dos retornos de referencia y el parámetro que muta: (sí / no)
- Predije el resultado de la compilación por valor antes de compilar: (sí / no)
- Cambié `moveKnight` para recibir `Knight` por valor: (sí / no)
- Vi que el registro del llamador dejaba de cambiar: (sí / no)
- Expliqué la línea `move=` que sobrevive y el encabezado `turn=` obsoleto: (sí / no)
- Las 20 ejecuciones por valor difirieron: (sí / no)
- Restauré la referencia y obtuve 20 coincidencias: (sí / no)
- `const` rechazó la escritura directa y la llamada equivocada: (sí / no)
- Quitar `const` hizo compilar ambas: (sí / no)
- Puedo explicar qué hace explícito una referencia y qué oculta: (sí / no)
- Mi frase evita las dos palabras de sintaxis y las ocho de veredicto: (sí / no)
- No edité ninguna de las dos líneas base versionadas: (sí / no)

Si cambié una línea base versionada, qué y por qué:

## 10. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué no puedo explicar todavía:
- Una cosa que volvería a leer:
- El número que llevaré a `construction-07`:
- La comprobación en la que más confío: ¿la compilación, el diff o el encabezado?
- La comprobación que me sorprendió:
- Qué haría visible un puntero que una referencia oculta:
- Qué añadiría un puntero que una referencia no tiene:
- Siguiente paso:

## 11. Si una línea base falta o no es fiel

Complétalo solo si faltó un archivo, una compilación falló por una razón del entorno, o un diff no
estaba vacío.

- Archivo ausente, error de compilación o diff:
- Salida exacta del comando:
- Qué hice en lugar de inventar un conteo:
- Cuándo volveré a intentarlo:

## 12. Autocomprobación

- [ ] Inventarié cada parámetro de caballo con su línea y su forma.
- [ ] Enumeré los alias que ya existían en `construction-05`.
- [ ] Escribí la predicción por valor, la del selector y la de los límites antes de medir.
- [ ] Compilé ambas líneas base y obtuve tres diffs vacíos, más 20 semillas coincidentes.
- [ ] Cambié `moveKnight` a por valor y registré el diagnóstico de compilación.
- [ ] Expliqué por qué la línea `move=` sobrevive y el siguiente encabezado `turn=` no.
- [ ] Ejecuté las 20 semillas por valor y conté las diferencias.
- [ ] Restauré la referencia y obtuve 20 coincidencias.
- [ ] Usé `const` para rechazar una escritura directa y una llamada equivocada.
- [ ] Quité `const` y vi compilar ambas sin ejecutar el resultado.
- [ ] Probé los tres tipos de retorno del selector.
- [ ] Separé qué permite la firma, qué muestra el sitio de llamada y qué comprueba el compilador.
- [ ] Mi frase de una oración evita las dos palabras de sintaxis y las ocho de veredicto.
- [ ] No edité las líneas base versionadas.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §10.
