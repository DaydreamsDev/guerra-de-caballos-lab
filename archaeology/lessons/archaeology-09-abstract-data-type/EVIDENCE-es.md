# archaeology-09 — Evidencia: tipo abstracto de datos ausente

> Este es tu registro. Escríbelo para tu yo futuro.
> Registra lo que observaste, no lo que esperabas que la lección pidiera.

## 1. Configuración

- Fecha:
- Ruta del archivo:
- Etiqueta en uso:
- Hash del commit (`git rev-parse HEAD`):
- Compilador y versión (`c++ --version`):
- Shell y versión:
- Kernel y distribución (`uname -sr`):
- Directorio de trabajo temporal:
- Algo inesperado en la configuración:

## 2. Predicción — escribir antes de leer `code/board.h`

No edites esta tabla después del paso 2. La diferencia entre predicción y observación es la señal de
aprendizaje; reparar la predicción la destruye.

| # | Pregunta | Mi predicción | Cómo sabría que me equivoco |
|---:|---|---|---|
| 1 | Líneas de `variant-03` que mencionan `mvectab` | | |
| 2 | Líneas de `main.cpp` que deben cambiar al volver opaco `Board` | | |
| 3 | Qué rechaza el compilador que la versión con clase permitía | | |
| 4 | Si las dos representaciones imprimen la misma línea | | |
| 5 | De qué hace responsable al llamador la API create/destroy | | |

Una frase falsable sobre el conjunto:

>

## 3. Inspección del original

Comandos:

```bash
grep -c "mvectab" original/variant-03-pointers-dev-2008/source/*.cpp
grep -c "tablero" original/variant-04-pointers-final-2008/source/*.cpp
grep -n "struct" original/variant-02-records-2008/source/*.cpp
grep -n "mvectab=0" original/variant-03-pointers-dev-2008/source/*.cpp
```

Observado:

- Líneas que mencionan `mvectab` en `variant-03`:
- Líneas que mencionan `tablero` en `variant-04`:
- La única estructura de `variant-02`:
- Las dos declaraciones de envoltorio (archivo y línea):
- Los dos sitios de escritura que reemplazan `vectab[i]=0;`:
- Cabeceras en el archivo:

Los envoltorios, lado a lado:

| Variante | Declaración del tipo | Campo | Declaración del arreglo |
|---|---|---|---|
| `variant-03` | | | |
| `variant-04` | | | |

La prueba que apliqué: renombrar el campo y después cambiar solo su tipo. Conté las líneas que
dejan de compilar en cada caso.

- Qué cambié en una copia temporal:
- Cuántas líneas dejaron de compilar:
- Qué prueba eso sobre el envoltorio:

## 4. El tipo opaco que escribí

- Ruta de la cabecera:
- Ruta de la implementación:
- Líneas de la cabecera:
- Líneas de la implementación:
- Ediciones en `main.cpp`: (deben ser cero)
- Dónde vive la definición de `struct Board`:
- Dónde viven los ayudantes:
- ¿Están los ayudantes en un espacio de nombres anónimo? (sí / no)
- Salida de `nm` para los ayudantes:

Superficie pública que elegí:

| Declaración | Por qué la necesita un llamador |
|---|---|
| | |
| | |
| | |
| | |
| | |
| | |

## 5. Evidencia de compilación — el tipo incompleto

Para cada operación prohibida, la línea que añadí y el diagnóstico que obtuve.

### Tamaño

```cpp
```

```text
```

### Reserva en pila

```cpp
```

```text
```

### Acceso a miembro

```cpp
```

```text
```

### Delete

```cpp
```

```text
```

- Cuáles de los cuatro fueron errores y cuál fue un aviso:
- Qué hizo `-Werror` con el aviso:
- Qué significa el aviso cuando no se promueve a error:

## 6. El intercambio

### Dos compilaciones, un llamador

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 main.cpp board_array.cpp -o board-array
c++ -std=c++20 -Wall -Wextra -Werror -O2 main.cpp board_bitmask.cpp -o board-bitmask
./board-array
./board-bitmask
```

- Salida con arreglo:
- Salida con máscara de bits:
- Resultado de `run.sh`:
- Hash de `main.cpp` antes y después:
- Hash de `board.h` antes y después:

### El intercambio es excluyente

Comando:

```bash
c++ -std=c++20 -O2 main.cpp board_array.cpp board_bitmask.cpp -o both
```

El error, literal:

```text
```

- Qué prueba esto sobre la elección de representación:

### Tamaños medidos

```cpp
// probe.cpp — una sonda, no un llamador
#include "board_array.cpp"
#include <cstdio>
int main() { std::printf("array_sizeof=%zu\n", sizeof(Board)); }
```

- Tamaño con arreglo:
- Tamaño con máscara de bits:
- Cómo medí el segundo:
- Por qué el llamador no puede imprimir ninguno de los dos números:

### Qué significó "sin recompilar" en mi compilación

- Forma de compilación que usé (binario estático / biblioteca compartida):
- Archivos que se recompilaron:
- Archivos que se reenlazaron:
- Archivos que no cambiaron:
- Ediciones en la fuente del llamador:

## 7. Conocimiento del llamador

| Pregunta | ¿Lo sabe el llamador? | Cómo lo aprende |
|---|---|---|
| Qué operaciones existen | | |
| Tipos de parámetros y de retorno | | |
| Qué operaciones mutan | | |
| Cuánto mide un tablero | | |
| Cómo se almacena un tablero | | |
| Si `board_create` puede fallar | | |
| Quién es dueño del manejador | | |
| Que las filas y columnas son base 1 | | |
| Que la entrada fuera de rango se ignora | | |
| Que `board_reset` es redundante tras crear | | |

La fila que más me sorprendió, y por qué:

>

## 8. Prueba de cambio y coste

Cambios que quedan dentro del módulo:

| Cambio | Fuente del llamador | Recompilación | Salida |
|---|---|---|---|
| | | | |
| | | | |
| | | | |

Cambios que alcanzan al llamador:

| Cambio | Por qué alcanza al llamador |
|---|---|
| | |
| | |
| | |

Fallos de ciclo de vida que tendría que evitar a mano:

| Fallo | Qué ocurre | Quién lo detecta |
|---|---|---|
| | | |
| | | |
| | | |

La comparación con el pimpl, con mis palabras:

- Qué añade el pimpl:
- Qué cuesta el pimpl:
- Cuál elegiría para este llamador, y por qué:

## 9. Frontera de evidencia

- Verificado en la fuente:
- Verificado ejecutando el compilador:
- Verificado ejecutando el programa:
- Inferido de la cabecera:
- Desconocido:

## 10. Qué prueba y qué no prueba el intercambio

| Afirmación | ¿Respaldada? | Por qué |
|---|---|---|
| El llamador no puede medir ni reservar un `Board`. | | |
| El llamador no puede nombrar un miembro. | | |
| El mismo llamador compila contra dos representaciones. | | |
| La salida es idéntica para el escenario. | | |
| La fuente del llamador sobrevive a un cambio de representación. | | |
| El objeto del llamador sobrevive a un cambio de representación. | | |
| Ambas implementaciones coinciden para toda entrada. | | |
| El manejador está libre de fugas. | | |
| El tablero rechaza coordenadas inválidas. | | |
| El tipo documenta su contrato. | | |

## 11. Severidad y coste

Un párrafo. Nombra el coste en trabajo real, no la fealdad del código. Di si el coste es medio o
alto en tu contexto y por qué. No propongas una reescritura en este párrafo.

>

## 12. Integridad del archivo

- `git status` en el archivo:
- ¿Edité `original/`? (Debe ser no.)
- ¿Apliqué algún parche dentro del archivo? (Debe ser no.)
- ¿Dónde guardé mi copia temporal?
- ¿Algún comando escribió en el archivo por accidente? (Compruébalo, no lo supongas.)

## 13. Paso siguiente

- Continúa a `archaeology-10` — polimorfismo ausente. El `switch` sobre el tipo de movimiento sigue
  ahí, y un tipo opaco no lo elimina.
- Relee la fila de conocimiento del llamador que la cabecera no pudo responder. Escribe la frase de
  contrato que le falta.
- Si tu cabecera pasó de 12 líneas, lista cada declaración y el llamador que la necesita. Lo que no
  tenga llamador es una fuga.
- Si quieres la versión más segura, escribe el pimpl y compara el conteo de líneas. Después decide
  qué coste puede pagar tu llamador.
- Si las dos salidas coincidieron con demasiada facilidad, añade un tercer escenario: marca dos
  veces la misma celda, marca una esquina, marca fuera de rango. Ambas implementaciones deben seguir
  coincidiendo.
