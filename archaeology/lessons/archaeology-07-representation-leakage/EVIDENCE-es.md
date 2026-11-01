# archaeology-07 — Evidencia: fuga de representación y arreglos paralelos

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
| 1 | Índices del tablero calculados a mano en `parallel_arrays.cpp` | | |
| 2 | Variables que deben cambiar juntas para mover un caballo | | |
| 3 | Líneas que cambia un tablero de 10×10, versión paralela | | |
| 4 | Líneas que cambia un tablero de 10×10, versión con registro | | |
| 5 | Qué esconde `Knight` y qué no esconde | | |

Una frase falsable sobre la representación:

>

## 3. Inspección del archivo

Archivo fuente:

```text
original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

Anclajes que verifiqué:

| Elemento | Línea o conteo | ¿Verificado? |
|---|---:|---|
| `int vectab[64],vecjugadas[8];` | 17 | |
| `int cab1,cab2,jugada=1,orden,ganador=1;` | 18 | |
| `int cf1,cc1,cf2,cc2,cab;` | 29 | |
| `for (i=0;i<=64;i++) vectab[i]=0;` | 61–63 | |
| `cab1=rand()%64; cab2=rand()%64;` | 81–82 | |
| Llamada `conversionvm();` | 90 | |
| Cuerpo de `conversionvm` | 104–112 | |
| Cuerpo de `conversionmv` | 226–232 | |
| `int movcab(int &cf,int &cc)` | 234 | |
| `vectab[ccab]=jugada;` | 269 | |
| `vectab[cab1]=jugada;` | 732 | |
| `cab1=movcab(cf1,cc1);` | 766 | |
| `cab2=movcab(cf2,cc2);` | 791 | |
| `cab2=movcab(cf2,cc2);` | 821 | |
| `cab1=movcab(cf1,cc1);` | 846 | |
| Líneas que contienen `vectab[` | 80 | |
| Líneas que contienen `conversionmv(` | 26 | |
| Llamadas a `conversionmv` sin prototipo ni definición | 24 | |
| Sitios de llamada a `movcab` | 4 | |

Las dos representaciones que encontré:

| Representación | Nombres | Rango | La produce | La consume |
|---|---|---|---|---|
| Índice plano del tablero | | | | |
| Fila y columna | | | | |

La línea que actualiza las dos representaciones a la vez:

```cpp

```

Qué creo que puede salir mal con esa línea:

>

Otros lugares del archivo donde el almacenamiento del tablero es público:

- `ordenjuego`:
- `impresion`:
- `movcab`:
- Otro:

## 4. Ejecución de la línea base

Comando:

```bash
cd archaeology/lessons/archaeology-07-representation-leakage
./run.sh
```

Salida observada:

```text
parallel=
record=
match=
```

- Código de salida:
- Compilador usado:
- Banderas observadas en `run.sh`:
- ¿La compilación pasó con `-Werror`? Si no, ¿qué diagnóstico apareció?

Estructura de la línea base que verifiqué:

| Parte | Líneas de `parallel_arrays.cpp` | Líneas de `record.cpp` | ¿Presente? |
|---|---:|---:|---|
| `Summary` | 4–11 | 35–42 | |
| `Knight` | ausente | 4–8 | |
| `Board` | ausente | 10–33 | |
| `mark` | ausente | 13–16 | |
| `visited_count` | ausente | 18–29 | |
| Arreglo privado | ausente | 31–32 | |
| `run_scenario` | 13–38 | 44–58 | |
| `main` | 40–47 | 60–67 | |

Qué no reproduce la línea base:

- El índice plano y la caché fila/columna guardados juntos en el archivo:
- La carga del tablero del archivo:
- La ubicación aleatoria:
- La selección de turno:
- La captura:
- La impresión:
- El bucle del juego:
- Otras omisiones:

## 5. Refactor

Congelé la salida antes de refactorizar:

```bash
mkdir -p build/scratch
./build/parallel-arrays > build/scratch/golden.txt
cat build/scratch/golden.txt
```

- Contenido de `golden.txt`:
- ¿La capturé antes de editar algo? (Debe ser sí.)

Copia temporal:

```bash
cp code/parallel_arrays.cpp build/scratch/record_attempt.cpp
```

El registro `Knight` que introduje:

```cpp

```

La interfaz `Board` que introduje:

```cpp

```

Qué se mudó dentro de `Board::mark`:

- Fórmula del índice:
- Arreglo:
- Conteo:

Qué quedó fuera de `Board`:

- Convención de coordenadas:
- Forma de salida:
- Otro:

Comando de compilación:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 \
    build/scratch/record_attempt.cpp -o build/scratch/record_attempt
```

- Resultado de la compilación:
- Diagnóstico, si hubo:

## 6. Llamadores actualizados

El llamador de esta línea base es `run_scenario`. Completa una fila por sitio.

| Sitio | `parallel_arrays.cpp` | `record.cpp` | Líneas tocadas | Notas |
|---|---|---|---:|---|
| Declaraciones de caballo | | | | |
| Marca inicial del jugador | | | | |
| Mover al jugador | | | | |
| Marca del jugador tras mover | | | | |
| Mover a la máquina | | | | |
| Marca de la máquina | | | | |
| Contar visitadas | | | | |
| Construir `Summary` | | | | |

Conteos:

- Índices calculados a mano eliminados:
- Valores `Knight` introducidos:
- Llamadas a `Board` introducidas:
- Líneas netas agregadas o eliminadas:
- Llamadores actualizados:

En el archivo, los lugares análogos son `ordenjuego`, `impresion`, `movcab` y los cuatro sitios
de llamada a `movcab` en `impresionorden`. Verifiqué:

- [ ] `ordenjuego` escribe `vectab[cab1]` y `vectab[cab2]`.
- [ ] `impresion` lee `vectab[k]`, `vectab[cab1]` y `vectab[cab2]`.
- [ ] `movcab` muta `cf`/`cc` y devuelve un índice plano.
- [ ] `impresionorden` llama a `movcab` en 766, 791, 821 y 846.

## 7. Resultado de la comparación

Salida observada después del refactor:

```text
player=2,3 machine=3,4 visited=3
```

Diferencia contra la línea congelada:

```bash
diff <(./build/scratch/record_attempt) build/scratch/golden.txt
```

- Salida del diff:
- Código de salida:

Diferencia contra la versión con registro incluida:

```bash
diff <(./build/scratch/record_attempt) <(./build/record)
```

- Salida del diff:
- Código de salida:

- ¿El resumen siguió igual?
- ¿Apareció `match=yes`?
- Si apareció una diferencia, ¿qué campo difirió y por qué?

Dirección del oráculo:

- ¿Qué versión produjo `golden.txt`?
- ¿Qué versión es la candidata?
- ¿Por qué generar la línea dorada desde la versión con registro sería circular?

## 8. Explicación

La prueba de cambio, en un párrafo. Nombra el cambio, los lugares tocados en la versión paralela y
los lugares tocados en la versión con registro. No afirmes una mejora de rendimiento.

>

La clase, no la instancia. Nombra al menos otros dos lugares del archivo donde la representación es
pública.

- Instancia en esta lección:
- Otra instancia 1:
- Otra instancia 2:

Dos arreglos, dos tamaños:

- Arreglo mínimo (solo registro):
- Arreglo estructural (registro más dueño):
- Arreglo más fuerte (invariante y comprobación de límites):
- Vínculos con la pista A:
- Por qué la línea base no alcanza del todo `construction-09`:

Qué no arregla el refactor:

- Arreglo privado todavía plano:
- Sin comprobación de límites:
- Convención de base uno:
- `Summary` todavía paralelo:
- Doble almacenamiento del archivo:
- Globales e impresión:
- Testabilidad de extremo a extremo:

## 9. Frontera de evidencia

- Verificado leyendo la fuente:
- Verificado ejecutando la línea base:
- Verificado ejecutando el refactor:
- Inferido de la prueba de cambio:
- No medido:
- Desconocido:

## 10. Qué prueba y qué no prueba el refactor

| Afirmación | ¿Respaldada? | Por qué |
|---|---|---|
| La versión con registro produce el mismo resumen en este escenario. | | |
| La versión con registro es equivalente para todo tablero y posición. | | |
| El llamador ya no calcula el índice del tablero. | | |
| El comportamiento del archivo se preserva. | | |
| `Board` esconde su almacenamiento de los llamadores. | | |
| `Board` elimina la representación plana. | | |
| `Board::mark` valida las coordenadas. | | |
| La versión con registro es más corta. | | |
| La versión con registro es más rápida. | | |
| La prueba de cambio es más barata con el registro. | | |

## 11. Severidad y coste

Un párrafo. Nombra el coste en trabajo real, no la fealdad del código. Di si el coste es medio o
alto en tu contexto y por qué. No propongas una reescritura en este párrafo.

>

## 12. Integridad del archivo

- `git status` en el archivo:
- ¿Edité `original/`? (Debe ser no.)
- ¿Apliqué algún parche dentro del archivo? (Debe ser no.)
- ¿Dónde guardé mi copia temporal?
- ¿Edité `code/parallel_arrays.cpp`? (Debe ser no.)
- ¿Edité `code/record.cpp`? (Debe ser no.)

## 13. Lista de aceptación

- [ ] Escribí las predicciones antes de inspeccionar la fuente.
- [ ] Verifiqué las declaraciones en las líneas 17, 18 y 29.
- [ ] Expliqué `conversionvm` y `conversionmv`.
- [ ] Conté 80 menciones de `vectab[` y 26 de `conversionmv(`, 24 de ellas llamadas.
- [ ] Nombré los cuatro sitios de llamada a `movcab`: 766, 791, 821, 846.
- [ ] La línea base imprimió `match=yes`.
- [ ] Congelé `golden.txt` antes de refactorizar.
- [ ] Introduje `Knight` en una copia temporal.
- [ ] Introduje `Board` con un arreglo privado y `mark`.
- [ ] Actualicé el llamador y conté los sitios.
- [ ] El refactor imprimió el mismo resumen.
- [ ] Comparé el refactor con `code/record.cpp`.
- [ ] Enuncié la frontera de aislamiento: solo el cambio de registro/tablero.
- [ ] Noté que `Board` sigue guardando `std::array<bool, 64>`.
- [ ] Noté que `Board::mark` no comprueba límites.
- [ ] Vinculé el arreglo con `construction-04`, `construction-05`, `construction-09` y
      `construction-11`.
- [ ] No afirmé una mejora de rendimiento.
- [ ] El archivo está sin modificar.

## 14. Siguiente paso

- Continúa con `archaeology-08` — frontera de módulo ausente.
- Si agrupar los datos fue más fácil que esconderlos, escribe una frase sobre la diferencia entre un
  registro y un dueño.
- Si hiciste la prueba de cambio a 10×10, conserva los dos conteos de líneas. Son el argumento de
  diseño.
- Si quieres la lección del invariante, ve a `archaeology-09`. Mantén separados el arreglo privado y
  el tipo validado en tu registro.
