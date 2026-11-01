# construction-07 — Evidencia

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro propio de quien aprende. Escribe en español.

Este registro compara dos formas de alcanzar el mismo caballo y la misma casilla del tablero: un
nombre vinculado y una ubicación. Conserva las dos columnas. Un número de un solo archivo no
responde a la pregunta, y un empate es un resultado, no una diferencia que falte.

## 1. Preparación de la ejecución

- Fecha:
- Semilla: `1`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Línea base con punteros leída el (fecha):
- Línea base con referencias leída el (fecha):
- ¿Tengo el registro de `construction-06` a mano? (sí / no)
- El conteo que traje de `construction-06` (parámetros de caballo mutables / de solo lectura / por
  valor):

## 2. Inventario de punteros (antes de la ejecución)

### 2.1 Funciones que nombran un caballo

Complétalo desde el código, con números de línea, no de memoria.

| Función | Línea | Parámetros `Knight` | Forma | Lee | Escribe |
|---|---|---|---|---|---|
| | | | | | |

Recuentos:

- Funciones con un parámetro `Knight *` mutable:
- Funciones con un parámetro `const Knight *`:
- Funciones con un parámetro `Knight` por valor:
- Funciones que devuelven un puntero a caballo:
- Funciones que devuelven un puntero a casilla:
- Parámetros de caballo mutables, total:
- Parámetros de caballo de solo lectura, total:
- Parámetros de caballo por valor, total:
- Parámetros de referencia que quedan, total:

### 2.2 Las cinco formas

| Forma | Ejemplo en este archivo | Qué permite | Qué prohíbe |
|---|---|---|---|
| parámetro por valor | | | |
| parámetro de referencia | | | |
| parámetro de referencia `const` | | | |
| parámetro de puntero | | | |
| retorno de puntero | | | |

- Qué es un puntero a nivel de lenguaje:
- Qué no es un puntero:
- Qué muestra el sitio de llamada sobre la mutación:
- Dónde aparece el `&`, y dónde no:

### 2.3 Acceso al tablero

| Pregunta | Respuesta |
|---|---|
| Declaración del tablero en `main` | |
| Filas y columnas reservadas / usadas | |
| Firma y línea de `getBoardCell` | |
| Qué devuelve `getBoardCell` | |
| Sitios de llamada de `getBoardCell` | |
| Qué codifica la segunda dimensión del tipo del parámetro | |
| Qué conserva y qué descarta el tipo devuelto | |

### 2.4 El diff

- Líneas añadidas:
- Líneas eliminadas:
- Líneas netas:
- Definiciones de función antes / después:
- Parámetros antes / después:
- Firmas que cambian un tipo de caballo:
- Sitios de llamada que ganan un `&`:
- Expresiones del tablero que pasan a llamar al accesor:
- Parámetros de referencia que quedan:
- Trazas que difieren, de 20:

## 3. Predicción (escrita antes de medir)

### 3.1 El diff estructural

El cambio: sustituir cada referencia de caballo por un puntero, añadir `getBoardCell` y conservar el
juego.

| Pregunta | Mi predicción | Confianza | Razón |
|---|---|---|---|
| Líneas añadidas | | | |
| Líneas eliminadas | | | |
| Definiciones de función después | | | |
| Parámetros después | | | |
| Firmas que cambian un tipo de caballo | | | |
| Sitios de llamada que ganan un `&` | | | |
| Expresiones del tablero que pasan a llamar al accesor | | | |
| Parámetros de referencia que quedan | | | |
| Semillas que difieren, de 20 | | | |

- El resultado que espero fallar, y por qué:

### 3.2 Las direcciones del tablero

| Pregunta | Mi predicción |
|---|---|
| Desplazamiento en elementos de (2,3) desde (0,0), ancho 9 | |
| Desplazamiento en elementos de (2,3) desde (0,0), ancho 11 | |
| Qué es `boardCells[2]`, y a qué decae | |
| ¿`boardCells[2] + 3` es igual a `&boardCells[2][3]`? | |
| Qué suma `+ 3`: bytes o elementos | |
| A qué apunta `getBoardCell(...) + 9`, ancho 9 | |
| A qué apunta `getBoardCell(...) + 9`, ancho 11 | |
| Cuál de estas cosas comprueba el compilador | |

- Por qué la unidad de la aritmética de punteros es el elemento, no el byte:
- El hecho de la plataforma que podría hacer que elementos y bytes parezcan iguales:

### 3.3 Los límites

- ¿Puede un `Knight *` ser nulo?
- ¿Qué pasa cuando se desreferencia un puntero nulo?
- ¿Puede un puntero reasignarse después de inicializarse?
- ¿Qué hace ahora `activeKnight = machineKnight;`, y qué hacía en `construction-06`?
- ¿Qué pasa si un puntero sobrevive a su objeto apuntado?
- ¿El sitio de llamada muestra que el objeto apuntado puede cambiar?
- ¿Cuál de estas cosas detecta el compilador?

## 4. Ejecución — ambas líneas base

- Comando de compilación de punteros:
- Comando de compilación de referencias:
- Estados de salida:
- Objetivo del diff: `../../../common/traces/seed-0001.txt`

Registra `vacío` o `diff` en cada celda de resultado.

| Comprobación | Comando | Resultado |
|---|---|---|
| Punteros vs oro | | |
| Referencias vs oro | | |
| Punteros vs referencias | | |

```text
<salida del diff, o "sin diferencias">
```

Semillas que coincidieron, de 20:

Si algún diff no está vacío, detente aquí y completa §11.

## 5. Reescribir un acceso al tablero

- Copia de trabajo:
- Acceso sustituido:
- Sustituto:

```cpp
BoardCell *rowStart = boardCells[destinationRow];
BoardCell *destinationCell = rowStart + destinationColumn;
```

- No edité la línea base versionada con punteros: (sí / no)

### 5.1 Compilar y ejecutar

- Comando de compilación:
- Estado de salida:
- Semillas que coincidieron, de 20:
- Por qué las direcciones son idénticas:

### 5.2 Los números del paso de fila

- Archivo de la sonda:
- `rowStart + 3 == &board[2][3]`:
- `flat == &board[2][3]`, ancho 9:
- Desplazamiento en elementos, ancho 9:
- Desplazamiento en bytes, ancho 9:
- `sizeof(BoardCell)`:
- Desplazamiento en elementos, ancho 11:
- `flat == &board[2][3]`, ancho 11:
- Qué cambió cuando el literal `9` sustituyó a la constante:
- La frase que conservaré sobre elementos y bytes:

### 5.3 Comprobación de la predicción

- Resultado predicho de la reescritura / observado:
- Desplazamiento predicho / observado:
- Predicción más cercana, y por qué:
- Predicción más lejana, y qué se me escapó:

## 6. Cambiar el ancho de fila

- Copia de trabajo:
- Constante cambiada:
- Estado de salida de la compilación:
- Trazas que difieren, de 20:
- Por qué las trazas distintas son esperadas aquí y no una regresión:

### 6.1 Qué se adapta y qué lee mal

- La forma bidimensional después del cambio:
- La forma aplanada con `boardColumnCount + 1`:
- La forma aplanada con el literal `9`:
- Cuál de las tres comprueba el compilador:
- Cuál de las tres falla en silencio:
- Qué haría fallar ruidosamente a un paso equivocado:

### 6.2 Contar dónde vive el diseño

| Línea base | Expresiones de dos índices que dependen del paso | Declaraciones con `[][boardColumnCount + 1]` |
|---|---|---|
| `construction-06` | | |
| `construction-07` | | |

- El total en cada línea base:
- Qué se movió y qué se quedó:
- El lugar donde el ancho sigue siendo inevitable:
- El lugar donde desapareció:

## 7. Nulo, reasignación y ciclo de vida

- Copias de trabajo:
- No edité la línea base versionada con punteros: (sí / no)

### 7.1 Nulo

- Líneas añadidas:
- Estado de salida de la compilación:
- Resultado de la ejecución:
- Qué no podía saber el compilador:
- Qué no podía representar una referencia:
- Qué promete ahora cada desreferencia:

### 7.2 Reasignación

- Líneas usadas:
- A qué apunta `activeKnight` antes / después:
- A qué apunta `before`:
- Qué hacía la misma asignación en `construction-06`:
- Por qué no ejecuté esto dentro del juego:

### 7.3 Ciclo de vida

- Función que devuelve una dirección local:
- Estado de salida de la compilación:
- Clase de diagnóstico:
- Mensaje:

```text
<salida del error>
```

- Por qué el compilador detectó este caso:
- Qué errores de ciclo de vida no puede detectar:
- Por qué `getBoardCell` es válido durante todo el juego:
- Qué lección es dueña de la propiedad y el ciclo de vida:

### 7.4 Qué prueba el compilador

| Comprobación | Qué prueba el compilador | Qué solo prueba una ejecución |
|---|---|---|
| Un puntero del tipo apuntado correcto | | |
| `const` sobre el objeto apuntado | | |
| Una dirección local devuelta | | |
| Una desreferencia nula | | |
| Aritmética de punteros dentro de una fila | | |

- Qué puede impedir un tipo que una traza no puede:
- Qué puede mostrar una traza que un tipo no puede:

## 8. Explicar

Una frase, sin las palabras *puntero*, *dirección*, *desreferencia*, *mejor*, *peor*, *limpio*,
*limpia*, *bonito*, *elegante*, *feo* ni *modular*:

>

Un párrafo:

- Qué decide un tipo puntero:
- Qué decide un retorno de puntero:
- Qué decide `const` sobre el objeto apuntado, y dónde se comprueba:
- Qué muestra el sitio de llamada, y qué oculta después de tener la dirección:
- Qué localiza el accesor del tablero:
- Qué no localiza:
- Qué puede probar el compilador:
- Qué solo prueba una ejecución:
- Qué lección posterior corrige límites y representación:
- Qué lecciones posteriores corrigen invariantes y acceso:
- Qué lección posterior corrige propiedad y ciclo de vida:

Las cuatro cosas que mantuve separadas:

## 9. Comprobación de aceptación

- Ejecuté las dos líneas base sin modificar: (sí / no)
- Obtuve tres diffs vacíos y 20 semillas coincidentes: (sí / no)
- Inventarié 17 parámetros de caballo: 9 mutables, 8 de solo lectura, 0 por valor: (sí / no)
- Identifiqué los dos retornos de puntero a caballo y el parámetro que muta: (sí / no)
- Identifiqué `getBoardCell` devolviendo `BoardCell *` y sus tres sitios de llamada: (sí / no)
- Predije el diff y los desplazamientos del tablero antes de medir: (sí / no)
- Reescribí un acceso al tablero como `rowStart + column`: (sí / no)
- La reescritura siguió coincidiendo con las 20 trazas: (sí / no)
- Medí el desplazamiento en 21 con ancho 9 y en 25 con ancho 11: (sí / no)
- Expliqué que `+ column` suma elementos, no bytes: (sí / no)
- Cambié el ancho y vi adaptarse la forma 2D y leer mal al literal `9`: (sí / no)
- Conté dónde vive el ancho de fila en las dos líneas base: (sí / no)
- Un puntero nulo compiló y cayó: (sí / no)
- Una dirección local devuelta falló la compilación: (sí / no)
- Mostré que un puntero puede reasignarse y una referencia no: (sí / no)
- Puedo explicar qué hace explícito un puntero y qué oculta: (sí / no)
- Mi frase evita las tres palabras de sintaxis y las ocho de veredicto: (sí / no)
- No edité ninguna de las dos líneas base versionadas: (sí / no)

Si cambié una línea base versionada, qué y por qué:

## 10. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué no puedo explicar todavía:
- Una cosa que volvería a leer:
- El número que llevaré a `construction-08`:
- La comprobación en la que más confío: ¿la compilación, el diff o la dirección?
- La comprobación que me sorprendió:
- Qué ocultó el accesor que mostraba el índice bidimensional:
- Qué añadió el puntero que la referencia no tenía:
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
- [ ] Inventarié el accesor del tablero y sus sitios de llamada.
- [ ] Escribí la predicción del diff, la de las direcciones y la de los límites antes de medir.
- [ ] Compilé ambas líneas base y obtuve tres diffs vacíos, más 20 semillas coincidentes.
- [ ] Reescribí un acceso al tablero como `rowStart + column` y mantuve 20 coincidencias.
- [ ] Medí el desplazamiento en elementos con ancho 9 y con ancho 11.
- [ ] Cambié el ancho de fila y separé el fallo ruidoso del silencioso.
- [ ] Conté dónde vive el ancho de fila en las dos líneas base.
- [ ] Mostré un puntero nulo compilando y cayendo.
- [ ] Mostré una dirección local devuelta fallando la compilación.
- [ ] Mostré la diferencia entre reasignar un puntero y asignar a través de una referencia.
- [ ] Separé qué permite el tipo, qué muestra el sitio de llamada, qué comprueba el compilador y
      qué solo prueba una ejecución.
- [ ] Mi frase de una oración evita las tres palabras de sintaxis y las ocho de veredicto.
- [ ] No edité las líneas base versionadas.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §10.
