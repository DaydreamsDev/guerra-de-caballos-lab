# archaeology-08 — Evidencia: frontera de módulo ausente

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
| 1 | Líneas del archivo que mencionan `vectab` | | |
| 2 | Cabeceras del archivo (`.h` / `.hpp`) | | |
| 3 | Funciones que deben cambiar al mover el tablero detrás de una cabecera | | |
| 4 | Qué rechaza el compilador en `main.cpp` | | |
| 5 | Si cambia la salida | | |
| 6 | Qué sigue revelando la cabecera | | |

Una frase falsable sobre el conjunto:

>

## 3. Inspección del original

Comandos:

```bash
grep -c "vectab" original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
find original -name "*.h" -o -name "*.hpp" | wc -l
wc -l original/*/source/*.cpp
```

Observado:

- Menciones de `vectab`:
- Cabeceras en el archivo:
- Líneas de `variant-01`:
- Funciones de `variant-01` (sin contar `main`):
- Líneas de prototipos:
- Dónde se declara el tablero (archivo y línea):

Regiones que alcanzan el arreglo directamente:

| Región | Líneas | Lecturas | Escrituras |
|---|---|---|---|
| `main` | | | |
| `inicializacion` | | | |
| `impresion0` | | | |
| `impresion` | | | |
| `movcab` | | | |
| `ordenjuego` | | | |

## 4. El módulo que escribí

- Ruta de la cabecera:
- Ruta de la implementación:
- Líneas de la cabecera:
- Líneas de la implementación:
- Ediciones en `main.cpp`: (deben ser cero)
- Ayudantes movidos fuera de la clase: (sí / no)
- Dónde vive ahora la fórmula del índice:

Superficie pública que elegí:

| Declaración | Por qué la necesita un llamador |
|---|---|
| | |
| | |
| | |
| | |

## 5. Evidencia de compilación

### La mitad del compilador

Comando:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 board.cpp main.cpp -o board-split
```

La línea que añadí a `main.cpp`:

```cpp
```

El error, literal:

```text
```

- Código o identificador del error:
- Línea que señaló el compilador:
- Línea que señaló la nota:

### La mitad del enlazador

La declaración que dejé sin definición:

El error, literal:

```text
```

- Qué prueba esto que no prueba el error del compilador:
- Por qué el enlazador solo informó la función que usaba el llamador:

### La prueba de la frontera

```bash
c++ -std=c++20 -Wall -Wextra -Werror -c board.cpp -o board.o
c++ -std=c++20 -Wall -Wextra -Werror -c main.cpp -o main.o
c++ board.o main.o -o board-split
grep -n "visited\[" main.cpp && exit 1
```

- Resultado de la compilación separada:
- Resultado de `grep`:
- En el archivo, el mismo `grep` habría coincidido en cuántas funciones:

## 6. Conocimiento del llamador

| Pregunta | ¿Lo sabe el llamador? | Cómo lo aprende |
|---|---|---|
| Qué operaciones existen | | |
| Tipos de parámetros y de retorno | | |
| Qué operaciones mutan | | |
| Que las filas y columnas son base 1 | | |
| Que la entrada fuera de rango se ignora | | |
| Que el tablero es de 8×8 | | |
| Que el tablero es un arreglo de 64 `bool` | | |
| La fórmula del índice | | |
| Que `reset` es redundante tras construir | | |

La fila que más me sorprendió, y por qué:

>

## 7. Prueba de cambio

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

- Qué verifiqué ejecutando y qué solo razoné:
- La distinción entre recompilar y reescribir, en una frase:

>

## 8. Frontera de evidencia

- Verificado en la fuente:
- Verificado ejecutando el compilador:
- Inferido de la cabecera:
- Desconocido:

## 9. Qué prueba y qué no prueba la frontera

| Afirmación | ¿Respaldada? | Por qué |
|---|---|---|
| Un llamador no puede nombrar el arreglo visitado. | | |
| Toda función declarada tiene cuerpo. | | |
| La salida no cambia con la separación. | | |
| El llamador ya no necesita la fórmula del índice. | | |
| La representación está escondida del compilador. | | |
| Los llamadores sobreviven a un cambio de representación sin recompilar. | | |
| El módulo documenta su sistema de coordenadas. | | |
| El módulo rechaza entrada inválida. | | |

## 10. Severidad y coste

Un párrafo. Nombra el coste en trabajo real, no la fealdad del código. Di si el coste es medio o
alto en tu contexto y por qué. No propongas una reescritura en este párrafo.

>

## 11. Integridad del archivo

- `git status` en el archivo:
- ¿Edité `original/`? (Debe ser no.)
- ¿Apliqué algún parche dentro del archivo? (Debe ser no.)
- ¿Dónde guardé mi copia temporal?
- ¿Algún comando escribió en el archivo por accidente? (Compruébalo, no lo supongas.)

## 12. Paso siguiente

- Continúa a `archaeology-09` — tipo abstracto de datos ausente. La cabecera que escribiste sigue
  mostrando la representación; esa lección la elimina.
- Relee la fila de la tabla de conocimiento del llamador que la cabecera no pudo responder. Escribe
  la frase de contrato que le falta.
- Si tu cabecera pasó de 20 líneas, lista cada declaración y el llamador que la necesita. Lo que no
  tenga llamador es una fuga.
- Si no pudiste reproducir el error del compilador, comprueba que compilaste el `main.cpp`
  modificado y no un archivo objeto antiguo.
