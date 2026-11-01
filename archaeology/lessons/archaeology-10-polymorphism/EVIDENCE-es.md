# archaeology-10 — Evidencia: polimorfismo ausente

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
| 1 | Funciones virtuales que declara `Piece` | | |
| 2 | Líneas de `delta_with_switch` y ramas que contiene | | |
| 3 | Coste de una llamada virtual y llamadas por ejecución | | |
| 4 | Declaraciones existentes que una tercera pieza debe cambiar, por camino | | |
| 5 | Qué switch contiene el archivo: número de movimiento o tipo de pieza | | |

Una frase falsable sobre el despacho en esta línea base:

>

## 3. Inspección de la interfaz y de la línea base

Archivo fuente:

```text
code/polymorphism.cpp
```

Partes que verifiqué:

| Parte | Líneas | ¿Verificado? |
|---|---:|---|
| `Position`, `Delta` | 5–17 | |
| `PieceKind` | 19–23 | |
| `knightDeltas`, `mirrorDeltas` | 25–33 | |
| `Piece` (tres declaraciones virtuales) | 35–41 | |
| `Knight`, `MirrorKnight` (dos sobreescrituras cada uno) | 43–71 | |
| `delta_with_switch` | 73–123 | |
| `move_with_switch` | 125–129 | |
| `main` (400 comparaciones) | 131–163 | |

Las declaraciones virtuales que encontré:

- Destructor:
- `move`:
- `role_name`:

Las sobreescrituras por implementación:

| Implementación | `move` | `role_name` |
|---|---|---|
| `Knight` | | |
| `MirrorKnight` | | |

Switches internos en `delta_with_switch` y sus casos:

- Switch externo sobre:
- Switches internos:
- Casos por switch interno:
- Ramas `default`:

## 4. Inspección del archivo

Anclajes que verifiqué en
`original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`:

| Elemento | Línea o conteo | ¿Verificado? |
|---|---:|---|
| `switch(vecjugadas[i])` | 239 | |
| `int cf1,cc1,cf2,cc2,cab;` | 29 | |
| `int cab1,cab2,jugada=1,...` | 18 | |
| Sitios con `if (cab==1)` | 9 | |
| `impresionorden` | 753 | |
| Bloques de turno `cab=1;` / `cab=2;` | 765, 790, 820, 844 | |
| `movcab(int &cf, int &cc)` | 234 | |

Sobre qué despacha el switch:

Qué selecciona el entero `cab`:

Las otras variantes:

| Variante | Línea del `switch` | Sitios con `cab==1` |
|---|---:|---:|
| `variant-01-baseline-2008` | | |
| `variant-02-records-2008` | | |
| `variant-03-pointers-dev-2008` | | |
| `variant-04-pointers-final-2008` | | |

Qué esperaba que contuviera el archivo, y qué contiene realmente:

>

## 5. Ejecución de la línea base

Comando:

```bash
cd archaeology/lessons/archaeology-10-polymorphism
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

Qué son los 400 casos:

- Piezas:
- Movimientos:
- Posiciones:
- Producto:

## 6. Captura del oráculo

El oráculo debe congelarse antes de borrar el switch. Registra los comandos exactos.

```bash
mkdir -p build/scratch
cd build/scratch
cp ../../code/polymorphism.cpp polymorphism.cpp
# reemplazar main por el bucle temporal de emisión
c++ -std=c++20 -Wall -Wextra -Werror -O2 polymorphism.cpp -o polymorphism-golden
./polymorphism-golden > golden.txt
wc -l golden.txt
```

- Salida de `wc -l golden.txt`:
- Primera línea de `golden.txt`:
- Última línea de `golden.txt`:
- Formato de cada línea (campos, en orden):
- ¿Confirmé que el oráculo vino de `move_with_switch`, no de las clases?

## 7. Reemplazo

Qué borré:

- `delta_with_switch`:
- `move_with_switch`:
- `PieceKind`:
- Cuerpo anterior de `main`:

Qué conservé:

- `Piece`:
- `Knight`:
- `MirrorKnight`:
- Tablas de deltas:

Qué añadí o reescribí:

- Lector de `main`:
- Includes o cabeceras:
- Línea de salida:
- Código de salida:

Resumen del diff (o una descripción corta si el diff es largo):

```text

```

¿La copia de trabajo compiló con las mismas banderas?

- Resultado:
- Diagnóstico, si hubo:

## 8. Prueba de cambio

El cambio: añadir una tercera pieza con su propia regla de movimiento.

| Medida | Camino con switch | Camino con despacho |
|---|---:|---:|
| Líneas añadidas a la regla | | |
| Declaraciones de regla existentes editadas | | |
| Tipos nuevos | | |
| Líneas de arnés para registrarla | | |
| Olvidar un movimiento: ¿compila limpio? | | |
| ¿Puede responder "cómo te llamas"? | | |
| Conflicto de fusión con la regla de otra pieza | | |

Dónde cayó cada edición:

- Camino con switch:
- Camino con despacho:
- Qué camino obliga a releer código compartido:
- Qué camino se puede probar solo:

## 9. Resultado de la comparación

Salida observada después del reemplazo:

```text
comparisons=
mismatches=
```

- Código de salida:
- ¿El conteo se mantuvo en 400?
- ¿El conteo de discrepancias se mantuvo en 0?
- Si apareció una discrepancia, ¿qué tupla falló y por qué?

Dirección del oráculo:

- ¿Qué implementación produjo `golden.txt`?
- ¿Qué implementación es el candidato?
- ¿Por qué generar ambos desde la misma tabla sería circular?

## 10. Explicación

El coste de una llamada virtual, en un párrafo. Nombra el vptr, la vtable, la llamada indirecta y
qué pasa con el inlining. Di si lo mediste.

>

Mi medición, o la frase "no medido":

- Repeticiones:
- Compilador y banderas:
- CPU:
- Resultado:
- ¿La diferencia está por encima del ruido?

Los dos switches, en un párrafo. Di cuál tiene el archivo, cuál reconstruye la línea base y por qué
las reparaciones difieren.

>

La clase, no la instancia. Nombra al menos otros dos lugares del archivo donde el programa decide
algo por el tipo de pieza o por un entero de tipo.

- Instancia en esta lección:
- Otra instancia 1:
- Otra instancia 2:

Dos arreglos, dos tamaños:

- Arreglo mínimo:
- Arreglo estructural:
- Enlaces a Track A:
- Qué añade `construction-13` a la pregunta:

Qué no arregla el reemplazo:

- Índice sin comprobar:
- Comportamiento del `default`:
- Globales:
- Impresión y bucle de turnos:
- Pruebas:
- Sitios con `cab==1` en el archivo:

## 11. Frontera de evidencia

- Verificado leyendo la fuente:
- Verificado ejecutando la línea base:
- Verificado ejecutando el reemplazo:
- Verificado con la prueba de cambio:
- Inferido:
- No medido:
- Desconocido:

## 12. Qué prueba y qué no prueba el reemplazo

| Afirmación | ¿Respaldada? | Por qué |
|---|---|---|
| El despacho reproduce el switch para 400 entradas. | | |
| El despacho es equivalente para todas las entradas. | | |
| El comportamiento de `movcab` quedó preservado. | | |
| El reemplazo elimina el switch de tipo. | | |
| El reemplazo elimina el switch de número de movimiento. | | |
| Una tercera pieza necesita cero ediciones en el bucle del banco. | | |
| Una tercera pieza necesita cero ediciones en el archivo. | | |
| El despacho es más rápido. | | |
| El despacho elimina el índice sin comprobar. | | |
| El despacho elimina las globales. | | |

## 13. Severidad y coste

Un párrafo. Nombra el coste en el trabajo real, no la fealdad del código. Di si el coste es medio o
alto en tu contexto y por qué. No propongas una reescritura en este párrafo.

>

## 14. Integridad del archivo

- `git status` en el archivo:
- ¿Edité `original/`? (Debe ser no.)
- ¿Apliqué algún parche dentro del archivo? (Debe ser no.)
- ¿Dónde guardé mi copia de trabajo?
- ¿Edité `code/polymorphism.cpp`? (Debe ser no.)

## 15. Lista de aceptación

- [ ] Escribí las cinco predicciones antes de inspeccionar la fuente.
- [ ] Nombré las tres declaraciones virtuales y las dos sobreescrituras por clase.
- [ ] Encontré el switch del archivo en la línea 239 y confirmé que es sobre el número de
      movimiento.
- [ ] Conté los sitios con `cab==1`: 9 en `variant-01`, 10 en las demás.
- [ ] La línea base imprimió `comparisons=400 mismatches=0`.
- [ ] Capturé una traza de 400 líneas desde el switch antes de borrarlo.
- [ ] Borré `delta_with_switch`, `move_with_switch` y `PieceKind`.
- [ ] La versión con despacho imprimió `comparisons=400 mismatches=0`.
- [ ] Registré la prueba de cambio: 23 líneas en la función compartida, 20 en un tipo nuevo, y los
      lugares tocados.
- [ ] Declaré el coste de la llamada con una medición o con "no medido".
- [ ] Separé el switch sobre el tipo de pieza del switch sobre el número de movimiento.
- [ ] Indiqué que la línea base reconstruye el switch de tipo y no es código de 2008 literal.
- [ ] Noté que los dos caballos del archivo se mueven igual.
- [ ] Noté que el `default` interno oculta un movimiento ausente.
- [ ] Enlacé el arreglo estructural a `construction-11`, `construction-12` y `construction-13`.
- [ ] El archivo está sin modificar.

## 16. Siguiente paso

- Continúa con `archaeology-11` — entrada, salida y testabilidad.
- Si la interfaz te pareció demasiado pequeña para una pieza real, escribe una frase sobre el
  límite: el movimiento en la interfaz basta para un delta, no para un juego.
- Si hiciste la medición, guarda los números. Son la única respuesta honesta a "¿es lento?".
- Si quieres la prueba de cambio difícil, añade una pieza cuyo movimiento dependa del tablero. Anota
  dónde falla la tabla.
