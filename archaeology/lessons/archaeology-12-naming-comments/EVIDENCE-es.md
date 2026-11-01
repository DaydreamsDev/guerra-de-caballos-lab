# archaeology-12 — Evidencia: nombres y comentarios

> Este es tu registro. Escríbelo para tu yo futuro.
> Registra lo que observaste, no lo que la lección parecía querer.

## 1. Configuración

- Fecha:
- Ruta del archivo:
- Etiqueta en uso:
- Hash del commit (`git rev-parse HEAD`):
- Compilador y versión (`c++ --version`):
- Shell y versión:
- Núcleo y distribución (`uname -sr`):
- Directorio de trabajo:
- Directorio temporal usado:
- Algo inesperado en la configuración:

## 2. Predicción — escribir antes de inspeccionar la fuente

No edites esta tabla después del paso 2. La brecha entre predicción y observación es la señal de
aprendizaje; reparar la predicción la destruye.

| # | Pregunta | Mi predicción | Cómo sabría que me equivoco |
|---:|---|---|---|
| 1 | ¿Qué calcula `calc`? | | |
| 2 | ¿Qué significan `f` y `c`, y en qué orden? | | |
| 3 | ¿Un renombrado cambia la salida? | | |
| 4 | ¿Cuántos comentarios del archivo explican un motivo? | | |

Una frase falsable sobre el renombrado:

>

## 3. Inspección del banco de pruebas

Ficheros:

```text
code/before.cpp
code/after.cpp
```

| Elemento | Antes | Después |
|---|---:|---:|
| Líneas del fichero | | |
| Nombre de la función | | |
| Nombres de los parámetros | | |
| Comentarios | | |
| Expresión | | |
| Salida | | |

El diff, pegado o resumido:

```text

```

Qué cambió:

- Nombre de la función:
- Nombres de los parámetros:
- Comentario:
- Salida:

Qué no cambió:

- Constante:
- Operador:
- Sitio de llamada:
- Cadena de formato:
- Pruebas:

## 4. Inspección del archivo

Fichero fuente:

```text
original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

Anclajes de nombres que verifiqué:

| Elemento | Línea o conteo | ¿Verificado? |
|---|---:|---|
| `simcab1=21,simcab2=156,sep=219` | 16 | |
| `vectab[64],vecjugadas[8]` | 17 | |
| `cab1,cab2,jugada=1,orden,ganador=1` | 18 | |
| `conversionmv(int cf,int cc)` | 226 | |
| Líneas con `vectab` | 80 | |
| Líneas con `jugada` | 76 | |
| Líneas de comentario con `//` | 119 | |

Comentarios que audité:

| Línea | Texto (cita breve) | Clase | Verbo | Por qué |
|---:|---|---|---|---|
| 1–8 | Descripción del programa | Descripción | Conservar | El código no dice para qué sirve el programa. |
| 9 | Bibliotecas necesarias | Narración | Borrar | Las líneas `#include` lo dicen. |
| 16 | Símbolos del tablero | Constantes sin nombre | Reescribir | Nombrar los tres símbolos y qué dibujan. |
| 18 | Posiciones y jugada actual | Incompleto | Reescribir | Omite `orden` y `ganador`. |
| 19 | Inicializa el vector | Narración y error | Borrar | Un nombre mejor lo reemplaza. |
| 76 | Cambia la semilla | Narración | Borrar | `srand(time(NULL))` lo dice. |
| 78 | Tirada para el caballo 2 | Mentira | Reparar o borrar | La variable es `tirocab1`. |
| 310 | El caballo no estuvo ahí | Mentira | Borrar | El `if` comprueba de quién es el turno. |
| 702 | Regla del caballo encerrado | Motivo | Conservar o acortar | La regla es real. |

Los dos comentarios que repararía primero, y por qué:

>

## 5. Ejecución de la línea base

Comando:

```bash
cd archaeology/lessons/archaeology-12-naming-comments
./run.sh
```

Salida observada:

```text
before=
after=
match=
```

- Código de salida:
- Compilador usado:
- Banderas observadas en `run.sh`:
- ¿La compilación pasó con `-Werror`? Si no, ¿qué diagnóstico apareció?

Qué compara la ejecución:

- `before.cpp`:
- `after.cpp`:
- La única entrada:
- El índice esperado:

## 6. Registro del renombrado

La copia de trabajo:

```bash
mkdir -p build/scratch
cp code/before.cpp build/scratch/renamed.cpp
```

| Nombre anterior | Nombre nuevo | Motivo | Alcance |
|---|---|---|---|
| `calc` | | | |
| `f` | | | |
| `c` | | | |

¿Renombré algo más? (Debe ser no para el arreglo mínimo.)

- Renombrado extra:
- Por qué me detuve ahí:

El fichero renombrado, pegado:

```cpp

```

Compilar y ejecutar:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 build/scratch/renamed.cpp \
    -o build/scratch/renamed
./build/scratch/renamed
```

- Resultado de compilación:
- Salida:
- Código de salida:
- Aviso o error, si hubo:

## 7. Registro de comentarios

| Ubicación | Antes | Después | Verbo | Motivo |
|---|---|---|---|---|
| `before.cpp:3` | | | | |
| `v01:16` | | | | |
| `v01:18` | | | | |
| `v01:78` | | | | |
| `v01:310` | | | | |
| `v01:702` | | | | |

El comentario que conservé, y qué no puede decir el código sin él:

>

El comentario que borré, y el nombre que lo reemplazó:

>

El comentario que estaba mal, y cómo lo probé:

>

## 8. Resultado de la comparación

Salida de la línea base:

```text
before=
after=
match=
```

Salida renombrada:

```text
index=
```

- ¿Coincidieron los tres valores?
- Código de salida:
- Si apareció una discrepancia, ¿qué expresión estaba mal y por qué?

Dirección del oráculo:

- ¿Qué fichero produjo la salida de referencia?
- ¿Qué fichero es el candidato?
- ¿Por qué generar ambos desde los nombres nuevos sería circular?

## 9. Explicación

Qué escondían los nombres anteriores, en un párrafo. Nombra la decisión y el lector que pagaba por
ella.

>

Qué hacían los comentarios, en un párrafo. Separa motivos, narración y mentiras.

>

La severidad y el coste en mi contexto:

- Severidad por incidente:
- Coste por año:
- Cuándo se vuelve alto:
- Qué arreglaría primero:

Dos arreglos, dos tamaños:

- Arreglo mínimo:
- Arreglo estructural:
- Enlaces a Track A:
- Qué añade el arreglo estructural que el renombrado no hace:

Qué no arregla el renombrado:

- Número mágico:
- Comprobación de límites:
- `jugada`:
- `ganador`:
- Globales:
- Pruebas:
- Comentarios del archivo:

## 10. Frontera de evidencia

- Verificado leyendo la fuente:
- Verificado ejecutando la línea base:
- Verificado ejecutando la copia renombrada:
- Inferido:
- No medido:
- Desconocido:

## 11. Qué prueba y qué no prueba el renombrado

| Afirmación | ¿Respaldada? | Por qué |
|---|---|---|
| La copia renombrada imprime la misma salida. | | |
| El renombrado preserva el comportamiento para toda entrada. | | |
| Los nombres nuevos eliminan la ambigüedad local. | | |
| Los nombres nuevos son los mejores posibles. | | |
| El borrado del comentario es seguro. | | |
| Los comentarios del archivo están todos reparados. | | |
| Las globales del archivo se renombraron. | | |
| La función es correcta para un tablero real. | | |
| El código ahora tiene pruebas. | | |

## 12. Severidad y coste

Un párrafo. Nombra el coste en trabajo real, no la fealdad del código. Di si el coste es bajo, medio
o alto en tu contexto y por qué. No propongas una reescritura en este párrafo.

>

La clase, no la instancia. Nombra otros dos lugares del archivo donde un nombre esconde una
decisión.

- Instancia en esta lección:
- Otra instancia 1:
- Otra instancia 2:

## 13. Integridad del archivo

- `git status` en el archivo:
- ¿Edité `original/`? (Debe ser no.)
- ¿Apliqué algún parche dentro del archivo? (Debe ser no.)
- ¿Dónde guardé mi copia de trabajo?
- ¿Edité `code/before.cpp` o `code/after.cpp`? (Debe ser no.)

## 14. Lista de aceptación

- [ ] Escribí las cuatro predicciones antes de inspeccionar la fuente.
- [ ] Comparé `before.cpp` y `after.cpp` con `diff` y registré los únicos cambios.
- [ ] La línea base imprimió `before=index=19`, `after=index=19`, `match=yes`.
- [ ] Registré el compilador y las banderas.
- [ ] La tabla de renombrado tiene tres filas con un motivo y un alcance cada una.
- [ ] La copia renombrada compila con `-Werror` e imprime `index=19`.
- [ ] La tabla de comentarios tiene un verbo y un motivo por cada fila que toqué.
- [ ] Cité el comentario del caballo equivocado en la línea 78 y el de la sentencia equivocada en
  310.
- [ ] Declaré qué no arregla el renombrado.
- [ ] Precifiqué el defecto por tiempo de lectura y coste de incidente.
- [ ] Enlacé el arreglo estructural a `common-02` y `construction-03`.
- [ ] El archivo está sin modificar.

## 15. Siguiente paso

- Continúa con `archaeology-13` — documentación técnica para mantenedores.
- Si quiero el renombrado difícil, llevo `vectab` por el archivo en una copia de trabajo y cuento
  las referencias.
- Una frase que quiero que mi yo futuro lea antes de renombrar cualquier cosa:

>
