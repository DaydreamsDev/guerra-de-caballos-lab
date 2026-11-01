# archaeology-06 — Evidencia: duplicación y el switch de ocho casos

> Este es tu registro. Escríbelo para tu yo futuro.
> Registra lo que observaste, no lo que esperabas que la lección pidiera.

## 1. Configuración

- Fecha:
- Ruta del archivo:
- Etiqueta descargada:
- Hash del commit (`git rev-parse HEAD`):
- Compilador y versión (`c++ --version`):
- Shell y versión:
- Kernel y distribución (`uname -sr`):
- Directorio de trabajo:
- Directorio de trabajo temporal usado:
- Algo inesperado en la configuración:

## 2. Predicción — escribir antes de inspeccionar la fuente

No edites esta tabla después del paso 2. La diferencia entre predicción y observación es la señal de
aprendizaje; reparar la predicción la destruye.

| # | Pregunta | Mi predicción | Cómo sabría que me equivoco |
|---:|---|---|---|
| 1 | Líneas por cuerpo `case`, incluidos comentario y llaves | | |
| 2 | Líneas dentro de un cuerpo que cambian entre casos | | |
| 3 | Líneas que deben moverse juntas al corregir un delta | | |
| 4 | Qué compra la tabla y qué cuesta | | |

Una frase falsable sobre el switch:

>

## 3. Inspección del switch original

Archivo fuente:

```text
original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

Anclajes que verifiqué:

| Elemento | Línea o conteo | ¿Verificado? |
|---|---:|---|
| `switch(vecjugadas[i])` | 239 | |
| `case 1` | 241 | |
| Comentario: los demás casos tienen la misma estructura que el caso 1 | 301 | |
| `case 2` | 302 | |
| `case 3` | 359 | |
| `case 4` | 415 | |
| `case 5` | 471 | |
| `case 6` | 527 | |
| `case 7` | 584 | |
| `case 8` | 640 | |
| `default` | 696 | |
| Líneas totales de los ocho cuerpos de caso | 455 | |
| Cuerpos de caso | 8 | |
| Enteros que realmente varían | 16 | |

Tamaños medidos de caso (líneas totales / líneas sin blanco):

| Caso | Total | Sin blanco |
|---:|---:|---:|
| 1 | | |
| 2 | | |
| 3 | | |
| 4 | | |
| 5 | | |
| 6 | | |
| 7 | | |
| 8 | | |

Los ocho deltas que leí de la fuente:

| Caso | Delta de fila | Delta de columna |
|---:|---:|---:|
| 1 | | |
| 2 | | |
| 3 | | |
| 4 | | |
| 5 | | |
| 6 | | |
| 7 | | |
| 8 | | |

Los pasos del esqueleto repetido que identifiqué (números de línea del caso 1):

- Comprobación de límites:
- Primera conversión:
- Rama de turno:
- Comprobación de visitada:
- Comprobación de captura:
- Aplicar el delta:
- Registrar la jugada:
- Imprimir y salir del recorrido:

Comprobación de normalización (opcional): ¿quité comentarios, espacios y dígitos y comparé los
cuerpos?

- Resultado:
- Qué difirió:

## 4. Ejecución de la línea base

Comando:

```bash
cd archaeology/lessons/archaeology-06-duplication-switch
./run.sh
```

Salida observada:

```text
comparisons=
mismatches=
```

- Código de salida:
- Compilador usado:
- Banderas observadas en `run.sh`:
- ¿La compilación pasó con `-Werror`? Si no, ¿qué diagnóstico apareció?

Estructura de la línea base que verifiqué:

| Parte | Líneas | ¿Presente? |
|---|---:|---|
| `Position` | 4–10 | |
| `Delta` | 12–16 | |
| `moveDeltas` (8 entradas) | 18–27 | |
| `apply_switch` (8 casos) | 29–52 | |
| `apply_table` (1 búsqueda) | 54–58 | |
| `main` (200 comparaciones) | 60–83 | |

Lo que la línea base no contiene:

- Comprobaciones de límites:
- Comprobaciones de visitadas:
- Lógica de captura:
- Selección de turno:
- Impresión:
- Terminación:

## 5. Captura del oráculo

El oráculo debe congelarse antes de borrar el switch. Registra los comandos exactos.

```bash
mkdir -p build/scratch
cd build/scratch
cp ../../code/movement.cpp movement.cpp
# sustituir main por el bucle temporal de emisión
c++ -std=c++20 -Wall -Wextra -Werror -O2 movement.cpp -o movement-golden
./movement-golden > golden.txt
wc -l golden.txt
```

- Salida de `wc -l golden.txt`:
- Primera línea de `golden.txt`:
- Última línea de `golden.txt`:
- Formato de cada línea (campos, en orden):
- ¿Confirmé que el oráculo vino de `apply_switch` y no de `apply_table`?

## 6. Extracción

Lo que eliminé:

- Función:
- Líneas eliminadas:
- Casos eliminados:

Lo que conservé:

- Tabla:
- Búsqueda:
- Entradas:

Lo que añadí o reescribí:

- Lector de `main`:
- Includes:
- Línea de salida:
- Código de salida:

Resumen del diff (o una descripción corta si el diff es largo):

```text

```

¿La copia de trabajo compiló con las mismas banderas?

- Resultado:
- Diagnóstico, si hubo:

## 7. Coste de la extracción

| Medida | Valor |
|---|---:|
| Líneas eliminadas con `apply_switch` | |
| Cuerpos `case` eliminados | |
| Entradas de tabla conservadas | |
| Líneas añadidas para el lector de `golden.txt` | |
| Comparaciones después de la extracción | |
| Discrepancias después de la extracción | |
| Líneas de cuerpos de caso del original reemplazadas por la tabla | |

Prueba de cambio opcional: añadir un noveno delta y cambiar el límite del bucle.

| Ruta | Líneas tocadas | Notas |
|---|---:|---|
| Tabla | | |
| Switch | | |

## 8. Resultado de la comparación

Salida observada después de la extracción:

```text
comparisons=
mismatches=
```

- Código de salida:
- ¿El conteo se mantuvo en 200?
- ¿El conteo de discrepancias se mantuvo en 0?
- Si apareció una discrepancia, ¿qué tupla falló y por qué?

Dirección del oráculo:

- ¿Qué implementación produjo `golden.txt`?
- ¿Qué implementación es la candidata?
- ¿Por qué generar ambas desde la misma tabla sería circular?

## 9. Explicación

La prueba de cambio, en un párrafo. Nombra el cambio, los lugares tocados en el switch y los lugares
tocados en la tabla. No declares una mejora de rendimiento.

>

La clase, no la instancia. Nombra al menos otros dos lugares del archivo donde una regla está
guardada como múltiples copias.

- Instancia en esta lección:
- Otra instancia 1:
- Otra instancia 2:

Dos arreglos, dos tamaños:

- Arreglo mínimo:
- Arreglo estructural:
- Vínculos con la pista A:
- Por qué `construction-12` no es este arreglo:

Lo que la extracción no arregla:

- Guarda fuera de rango:
- Duplicación de ramas de turno:
- Globales e impresión:
- Testabilidad de extremo a extremo:

## 10. Frontera de evidencia

- Verificado leyendo la fuente:
- Verificado ejecutando la línea base:
- Verificado ejecutando la versión extraída:
- Inferido a partir de la prueba de cambio:
- No medido:
- Desconocido:

## 11. Qué prueba y qué no prueba la extracción

| Afirmación | ¿Respaldada? | Por qué |
|---|---|---|
| La tabla reproduce el switch para 200 entradas. | | |
| La tabla es equivalente para todas las entradas. | | |
| El comportamiento del `movcab` original se preserva. | | |
| La extracción elimina 24 líneas y 8 casos de la línea base. | | |
| La extracción elimina el despacho. | | |
| La extracción elimina la duplicación del esqueleto. | | |
| La tabla preserva el comportamiento de `default`. | | |
| La prueba de cambio es más barata con la tabla. | | |
| La tabla es más rápida en tiempo de ejecución. | | |

## 12. Severidad y coste

Un párrafo. Nombra el coste en trabajo real, no la fealdad del código. Di si el coste es medio o
alto en tu contexto y por qué. No propongas una reescritura en este párrafo.

>

## 13. Integridad del archivo

- `git status` en el archivo:
- ¿Edité `original/`? (Debe ser no.)
- ¿Apliqué algún parche dentro del archivo? (Debe ser no.)
- ¿Dónde guardé mi copia de trabajo?
- ¿Edité `code/movement.cpp`? (Debe ser no.)

## 14. Lista de aceptación

- [ ] Escribí las predicciones antes de inspeccionar la fuente.
- [ ] Verifiqué los ocho anclajes de caso y el comentario de la línea 301.
- [ ] Conté 8 cuerpos, 455 líneas y 16 enteros variables.
- [ ] La línea base imprimió `comparisons=200 mismatches=0`.
- [ ] Capturé una traza de referencia de 200 líneas desde `apply_switch` antes de borrarlo.
- [ ] Eliminé `apply_switch` y conservé `moveDeltas` y `apply_table`.
- [ ] La versión extraída imprimió `comparisons=200 mismatches=0`.
- [ ] Registré el coste: 24 líneas, 8 casos, 8 entradas, 200/0.
- [ ] Expliqué la dirección del oráculo y el riesgo de circularidad.
- [ ] Enuncié la frontera de aislamiento: solo el cálculo del delta.
- [ ] Anoté la guarda `default` perdida.
- [ ] Vinculé el arreglo estructural a `construction-03`, `construction-05` y `construction-19`.
- [ ] El archivo está sin modificar.

## 15. Siguiente paso

- Continúa a `archaeology-07` — filtración de representación y arreglos paralelos.
- Si el switch fue fácil de quitar pero el esqueleto no, escribe una frase sobre la frontera entre
  el arreglo mínimo y el estructural.
- Si hiciste la prueba de cambio opcional, conserva los dos conteos de líneas. Son el argumento de
  diseño.
- Si quieres la lección de despacho, ve a `archaeology-10`. Mantén las dos reparaciones separadas.
