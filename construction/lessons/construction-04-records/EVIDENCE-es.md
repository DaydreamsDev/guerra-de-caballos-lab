# construction-04 — Evidencia

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro propio de quien aprende. Escribe en español.

Este registro compara dos representaciones de un mismo hecho. Conserva las dos columnas. Un número
de un solo archivo no responde a la pregunta, y un empate es un resultado, no una diferencia que
falte.

## 1. Preparación de la ejecución

- Fecha:
- Semilla: `1`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Línea base con registros leída el (fecha):
- Línea base con funciones leída el (fecha):
- ¿Tengo el registro de construction-03 a mano? (sí / no)
- Llamadores cambiados y sitios de edición que conté en construction-03:

## 2. Inventario de firmas (antes de la ejecución)

### 2.1 Funciones que reciben un `Knight`

Complétalo desde el código, con números de línea, no de memoria.

| Función | Línea | Recibe | Campos que lee | Campos que escribe |
|---|---|---|---|---|
| | | | | |

Recuentos:

- Funciones que reciben un `Knight`:
- De ellas, `const Knight &`:
- De ellas, `Knight &`:
- Funciones que reciben un caballo y no leen ningún campo:
- Accesos a campo (`.row` / `.column`) en todo el archivo:

### 2.2 Parámetros

| Métrica | construction-03 | construction-04 |
|---|---|---|
| Definiciones de función (cuenta `main`) | | |
| Parámetros en todas las definiciones | | |
| Parámetros de `tryMove` | | |
| Parámetros de `playTurn` | | |
| Parámetros de `playGame` | | |
| Parámetros de `tryMove` que son de salida | | |

- Parámetros que desaparecieron:
- Funciones cuyo número de parámetros cambió:
- Funciones cuyo número de parámetros no cambió, y por qué:

### 2.3 Lado a lado

| Pregunta | construction-03 | construction-04 |
|---|---|---|
| Cómo aprende una función cuál es el caballo activo | | |
| Cuántos nombres por caballo cruzan una llamada | | |
| Dónde se fija por primera vez la fila de un caballo | | |
| Qué dice una firma sobre los campos que lee el cuerpo | | |
| Dónde se declara un hecho nuevo por caballo | | |
| Qué cuesta un hecho nuevo por caballo en firmas | | |
| Qué impide `row = 0` | | |

## 3. Predicción (escrita antes de medir)

### 3.1 El coste del registro

R-CAPTURED, en mis palabras:

- Qué se marca:
- Cuándo se activa la marca:
- Quién la lee:
- Por qué la salida no cambia:

Mis números comprometidos para el registro:

| Métrica | Mi número | Confianza | Razón |
|---|---|---|---|
| Firmas que cambian | | | |
| Llamadas que cambian | | | |
| Funciones cuyo cuerpo cambia | | | |
| Líneas añadidas | | | |
| Trazas de referencia que difieren (de 20) | | | |

- La referencia que hace posible la asignación:
- Qué espero que `Knight playerKnight = {};` haga con el campo nuevo:

### 3.2 El coste con variables separadas

Reglas de conteo que usé (una línea cada una):

- Sitio de edición:
- Cambio de firma:
- Llamada:
- Llamador:
- Global nueva:

Mis números comprometidos:

| Métrica | Banderas en `playGame` | Banderas en `main` | Confianza |
|---|---|---|---|
| Sitios de edición | | | |
| Cambios de firma | | | |
| Llamadas cambiadas | | | |
| Llamadores de `playTurn` cambiados | | | |
| Llamadores de `playGame` cambiados | | | |
| Globales nuevas | | | |
| Líneas añadidas | | | |
| Trazas de referencia rotas (de 20) | | | |

### 3.3 Qué ubicación es la comparación justa

- Ámbito que posee el hecho en la versión con registro, y la variable exacta:
- Ámbito que posee el hecho en la versión con banderas, y por qué:
- Si el final de `playGame` imprimiera qué caballo fue capturado, qué ubicación seguiría compilando:
- La comparación que convierte a `main` en el propietario honesto:

## 4. Ejecución — las dos líneas base

- Comando de compilación de registros:
- Comando de compilación de funciones:
- Estados de salida:
- Objetivo de la diferencia: `../../../common/traces/seed-0001.txt`

Anota `vacía` o `difiere` en cada celda de resultado.

| Comprobación | Comando | Resultado |
|---|---|---|
| Registros contra la traza | | |
| Funciones contra la traza | | |
| Registros contra funciones | | |

```text
<salida de la diferencia, o "sin diferencias">
```

Semillas que coincidieron, de 20:

Si alguna diferencia no está vacía, detente aquí y completa §10.

## 5. Añadir `captured` al registro

- Copia de trabajo:
- La línea de declaración del campo:
- Las líneas de inicialización:
- La línea de asignación:
- No edité la línea base versionada de registros: (sí / no)

```text
<diferencia de la copia contra la línea base versionada>
```

Salida de `grep -n 'captured' <copia de trabajo>`:

```text
<salida>
```

Semillas que coincidieron, de 20:

- Firmas que cambiaron:
- Llamadas que cambiaron:
- Funciones cuyo cuerpo cambió:
- Líneas añadidas / eliminadas:

### 5.1 Las cuatro observaciones

| Observación | Comando o edición | Resultado |
|---|---|---|
| Ninguna firma se movió | | |
| La traza no puede ver el campo | | |
| El valor por defecto ya era correcto | | |
| Ningún dato nuevo en el ámbito de espacio de nombres | | |

- Qué hace `= {}` con un campo añadido después de escribir esa línea:
- Qué demuestra la traza y qué no:
- `sizeof(Knight)` antes / después:
- Nombres mutables en el ámbito de espacio de nombres, antes / después (`nm`):

### 5.2 Comprobación de la predicción

- Firmas predichas / observadas:
- Líneas predichas / observadas:
- Predicción más acertada, y por qué:
- Predicción más desviada, y qué se me escapó:

## 6. El mismo hecho con variables separadas

- Copia de trabajo:
- Ubicación que apliqué primero:
- Por qué esa ubicación es la propietaria honesta:
- No edité la línea base versionada de funciones: (sí / no)

### 6.1 Ubicación

| Dónde podrían vivir las banderas | Propietario del hecho | Firmas obligadas a cambiar | ¿Elegida? |
|---|---|---|---|
| local en `playTurn` | | | |
| local en `playGame`, pasada a `playTurn` | | | |
| local en `main`, pasada por `playGame` | | | |
| global en el ámbito de espacio de nombres | | | |

### 6.2 Sitios de edición, enumerados

| Sitio | Línea | Qué cambia | ¿Firma, llamada o cuerpo? |
|---|---|---|---|
| | | | |

- Total de sitios de edición:
- Cambios de firma:
- Llamadas cambiadas:
- Líneas añadidas / eliminadas:

### 6.3 Llamadores

| Función | Línea de definición | Llamadas | Llamadores | Llamadores que cambian |
|---|---|---|---|---|
| `playTurn` | | | | |
| `playGame` | | | | |

- Por qué el propietario de la bandera decide si `main` cambia:
- Qué compra la firma extra, en términos del compilador:
- Qué cuesta la firma extra, en términos de quien lee:

### 6.4 El condicional

- Qué tiene `playTurn` sobre el otro caballo, y en qué forma:
- Por qué la versión con banderas necesita un `if` / `else`:
- Por qué la versión con registro necesita una sola asignación:
- El dato que `playTurn` ya conocía y tuvo que repetir:

### 6.5 Tabla de costes

| Métrica | Registro | Banderas en `main` | Banderas en `playGame` | Qué mide |
|---|---|---|---|---|
| Sitios de edición | | | | escritura |
| Cambios de firma | | | | acoplamiento visible |
| Llamadas cambiadas | | | | alcance en el grafo de llamadas |
| Llamadores cambiados | | | | alcance en el grafo de llamadas |
| Globales nuevas | | | | estado oculto |
| Líneas añadidas / eliminadas | | | | radio de impacto |
| Conceptos nuevos del lenguaje | | | | coste de aprendizaje |
| Trazas rotas (de 20) | | | | cambio de contrato |
| Trazas reproducidas sin modificar (de 20) | | | | contrato |
| Funciones que pueden leer el hecho | | | | precisión de la lista de parámetros |

### 6.6 Ejecución y respuestas

```bash
<comandos>
```

| Comprobación | Resultado |
|---|---|
| Versión con banderas contra la traza (20 semillas) | |
| Versión estrecha contra la traza (20 semillas) | |
| Primera divergencia, si la hay | |

- Por qué compila la ubicación estrecha: la condición exacta de la que depende:
- Qué le ocurre a la ubicación estrecha si `playGame` imprime el caballo capturado:
- Qué hace la versión con registro en ese caso:
- En `construction-03`, la edición más pequeña que hace que las banderas pertenezcan a los caballos:
- En qué fila la ubicación estrecha gana al registro, y por qué no es el mismo cambio:

## 7. Explicar

Una frase, sin la palabra *struct* y sin *mejor*, *peor*, *limpio*, *más limpio*, *bonito*,
*elegante*, *feo* ni *modular*:

>

Un párrafo:

- A qué nombres tuvo que llegar el campo nuevo en cada versión:
- Qué dejó de decir la lista de parámetros más corta:
- Una función donde perdí información:
- Una función donde no perdí nada:
- Qué localiza el registro:
- Qué no localiza el registro:
- Qué debe sostener en la memoria de trabajo quien lee para saber qué campos usa una función, en
  cada archivo:
- Qué puede y qué no puede demostrar una traza de referencia sobre este cambio:
- Qué lección posterior corrige representación, invariantes y acceso:

## 8. Comprobación de aceptación

- `captured` añadido al registro: (sí / no)
- Inicializado para los dos caballos: (sí / no)
- Firmas que cambiaron:
- Llamadas que cambiaron:
- Las dos llamadas que **no** cambiaron, y por qué:
- Qué localiza el registro, en una línea:
- Qué no localiza el registro, en una línea:
- Ubicación con variables separadas que es la comparación honesta:
- Sitios de edición, registro / banderas en `main`:
- Cambios de firma, registro / banderas en `main`:
- Trazas reproducidas después del cambio en el registro, de 20:
- La explicación evita *struct* y las ocho palabras de veredicto: (sí / no)
- No edité las líneas base versionadas: (sí / no)

Si cambié una línea base versionada, qué y por qué:

## 9. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué no puedo explicar todavía:
- Una cosa que volvería a leer:
- El número que me llevo a `construction-05`:
- La fila donde gana el registro, y la fila donde pierde:
- El cambio que haría caro el registro, y el cambio que lo haría barato:
- Siguiente paso:

## 10. Si falta una línea base o no es fiel

Complétalo solo si faltó un archivo, falló una compilación o una diferencia no estaba vacía.

- Archivo ausente, error de compilación o diferencia:
- Salida exacta del comando:
- Qué hice en lugar de inventar un conteo:
- Cuándo lo reintentaré:

## 11. Autocomprobación

- [ ] Inventarié qué función lee qué campo, con números de línea.
- [ ] Escribí la predicción del registro y las dos de banderas antes de medir.
- [ ] Compilé las dos líneas base y obtuve tres diferencias vacías, más 20 semillas
      coincidentes.
- [ ] Añadí `captured` a una copia y mantuve las 20 trazas pasando.
- [ ] Mostré que `= {}` cubre el campo nuevo sin inicializadores explícitos.
- [ ] Mostré que un valor por defecto equivocado pasa las trazas, porque nada lo lee.
- [ ] Apliqué el mismo hecho a la línea base con funciones usando banderas separadas.
- [ ] Conté las dos ubicaciones de banderas y nombré la condición de la que depende la estrecha.
- [ ] Separé el coste de edición de la precisión que perdió la lista de parámetros.
- [ ] Mi frase de explicación evita *struct* y las ocho palabras de veredicto.
- [ ] No edité las líneas base versionadas.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §9.
