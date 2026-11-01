# archaeology-14 — Evidencia: pruebas y determinismo

> Este es tu registro. Escríbelo para tu yo futuro.
> Registra lo que observaste, no lo que la lección parecía querer.

## 1. Configuración

- Fecha:
- Ruta del archivo:
- Compilador y versión (`c++ --version`):
- Runtime de C (`ldd --version` primera línea, o `getconf GNU_LIBC_VERSION`):
- `RAND_MAX` en esta plataforma:
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
| 1 | ¿El archivo acepta un argumento de semilla? | | |
| 2 | Dos ejecuciones en el mismo segundo: ¿mismo tablero? | | |
| 3 | Dos ejecuciones con `--seed 1`: ¿salida idéntica? | | |
| 4 | `--seed 2`: ¿cambia el ganador? | | |
| 5 | ¿Se puede repetir en MSVC una traza de `rand()` de glibc? | | |

Una frase falsable sobre el determinismo en este banco:

>

## 3. Inspección de la línea base

Ficheros:

```text
code/seed_control.cpp
code/tests/golden-seed-1.txt
run.sh
```

Recuento de líneas:

| Fichero | Líneas | ¿Verificado? |
|---|---:|---|
| `code/seed_control.cpp` | 44 | |
| `code/tests/golden-seed-1.txt` | 6 | |
| `run.sh` | 27 | |

Las tres costuras:

| Costura | Líneas | Qué hace | ¿Verificado? |
|---|---:|---|---|
| Lectura de argumentos | 25–31 | | |
| `set_seed` | 13–16 | | |
| `below` | 18–22 | | |

El generador:

- Multiplicador:
- Incremento:
- Ancho del estado:
- Qué devuelve `below(3)`:
- Cómo el paso se convierte en posición:

La semilla por defecto cuando no hay argumento válido:

El comportamiento del lector que no esperaba (`--seed abc`, `--seed 1 extra`):

## 4. Inspección del archivo

Anclajes verificados en
`original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`:

| Elemento | Línea o recuento | ¿Verificado? |
|---|---:|---|
| `void main()` | 32 | |
| `cabubicacion()` | 69–87 | |
| Bucle de reintentos `while (i==1)` | 73 | |
| `srand(time(NULL))` | 76 | |
| `rand() % 51` para el orden de salida | 78–79 | |
| `rand() % 64` para las celdas | 81–82 | |
| Comprobación de distinción | 84 | |
| `opcion=(rand() % 8)+1` | 180 | |
| Llamadas a `srand` en el fichero | 1 | |
| Llamadas a `rand()` en el fichero | 5 | |
| `argc` o `argv` en cualquier variante | 0 | |
| Ficheros de prueba o arnés en el archivo | 0 | |

Las otras variantes:

| Variante | Línea de `srand` | Línea de la quinta `rand()` | Líneas |
|---|---:|---:|---:|
| `variant-00-precursor-2007` | | | |
| `variant-01-baseline-2008` | 76 | 180 | 930 |
| `variant-02-records-2008` | 87 | 231 | 1021 |
| `variant-03-pointers-dev-2008` | 99 | 245 | 1058 |
| `variant-04-pointers-final-2008` | 105 | 257 | 1079 |

Qué dice el `05-known-issues.md` del archivo sobre pruebas y semillas:

>

Por qué el bucle de reintentos repite las mismas extracciones dentro de un segundo:

>

## 5. Ejecución de la línea base

Comando:

```bash
cd archaeology/lessons/archaeology-14-tests-determinism
./run.sh
```

Salida observada:

```text
first=seed=1
move=1 position=2
move=2 position=5
move=3 position=0
winner=player
moves=3
same=yes
golden=yes
```

- Código de salida:
- Compilador usado:
- Opciones observadas en `run.sh`:
- ¿La compilación pasó con `-Werror`? Si no, ¿qué diagnóstico apareció?

Qué significa cada etiqueta:

- `first=`:
- `same=`:
- `golden=`:
- código de salida:

La aritmética para la semilla 1:

| Movimiento | Estado tras la extracción | `state % 3` | Paso | Posición |
|---:|---:|---:|---:|---:|
| 1 | 7806831264735756412 | 1 | 2 | 2 |
| 2 | 9396908728118811419 | 2 | 3 | 5 |
| 3 | 11960119808228829710 | 2 | 3 | 0 |

- ¿Mi cálculo a mano coincidió con las posiciones impresas?
- Si no, ¿dónde divergió?

## 6. Dos ejecuciones con la misma semilla

Comandos:

```bash
./build/seed-control --seed 1 > build/run-a.txt
./build/seed-control --seed 1 > build/run-b.txt
diff build/run-a.txt build/run-b.txt
```

- Salida de `diff`:
- Código de salida de `diff`:
- ¿Son idénticas las transcripciones?
- Qué afirma `same=yes` en `run.sh`:
- Qué no afirma `same=yes`:

Entradas ambientales que comprobé:

| Entrada | ¿La lee el programa? | Evidencia |
|---|---|---|
| Reloj | | |
| Variables de entorno | | |
| Ficheros | | |
| Locale | | |
| Estado de generador compartido | | |

## 7. Comparación con la traza de referencia

Comandos:

```bash
cat code/tests/golden-seed-1.txt
./build/seed-control --seed 1 | diff - code/tests/golden-seed-1.txt
./build/seed-control --seed 1 | cmp - code/tests/golden-seed-1.txt
```

- Contenido del archivo de referencia (cópialo exacto, incluido el salto final):

```text

```

- Salida de `diff`:
- Código de salida de `diff`:
- Salida de `cmp`:
- Código de salida de `cmp`:
- ¿Informó `run.sh` de `golden=yes`?
- Por qué `cmp` es más estricto que `run.sh`:
- Cuál es el oráculo:
- Cuál es la prueba:
- Qué le pasa a la prueba si se borran las líneas `exit 1`:

Una discrepancia deliberada que produje (opcional):

- Cambio realizado:
- Resultado de `golden=`:
- Código de salida:
- ¿Edité el archivo de referencia? (Debe ser no.)

## 8. Otra semilla

Comandos:

```bash
./build/seed-control --seed 2
./build/seed-control --seed 7
```

Observado:

| Semilla | Movimiento 1 | Movimiento 2 | Movimiento 3 | Ganador | Movimientos |
|---:|---:|---:|---:|---|---:|
| 1 | 2 | 5 | 0 | player | 3 |
| 2 | 2 | 3 | 4 | player | 3 |
| 7 | 3 | 4 | 7 | player | 3 |

- ¿Qué campos cambiaron?
- ¿Qué campos no cambiaron?
- Por qué el ganador y el número de movimientos no pueden cambiar en este banco:
- Qué haría distinto un juego real:
- Por qué las semillas 1 y 2 comparten la primera posición:

## 9. Nota de plataforma

Qué garantiza `rand()` y qué no:

- Garantizado por el estándar:
- No garantizado:
- `RAND_MAX` en esta plataforma:
- `RAND_MAX` en MSVC:
- Por qué una extracción `% 51` usa bits distintos en cada plataforma:
- Por qué una llamada nueva a `rand()` en cualquier sitio desplaza las posteriores:

Una nota de plataforma para una traza heredada capturada:

| Campo | Valor de mi captura |
|---|---|
| Compilador y versión | |
| Runtime de C / libc | |
| Arquitectura y ancho de palabra | |
| Origen de la semilla | |
| Valor exacto de la semilla | |
| Línea de comandos | |
| Bytes de salida (saltos, codificación) | |
| Declaración de alcance | |

- Qué no puede aportar el archivo para esta nota:
- Por qué capturar una traza exige parchear una copia temporal:
- De quién es entonces la traza:

## 10. Explicación

El coste, en un párrafo. Nombra el bucle de reintentos, la granularidad del reloj y la
ausencia de argumento de semilla. Di qué impide cada uno.

>

Dos arreglos, dos tamaños:

- Arreglo mínimo:
- Arreglo estructural:
- Enlaces al Track A:
- Qué añade `common-01` a la pregunta:
- Qué añade `common-04` a la pregunta:

Qué no arregla el control de semilla:

- Ganador:
- Sesgo de módulo:
- `void main()`:
- Estado global:
- Validación de entrada:
- Nota de plataforma:
- El resto del juego:

La prueba de cambio:

| Cambio | Línea base | Banco con semilla |
|---|---|---|
| Ejecutar dos veces, misma semilla | | |
| Ejecutar con otra semilla | | |
| Añadir una extracción antes del bucle | | |
| Cambiar una cadena de formato | | |
| Ejecutar en otra libc | | |
| Informar un fallo con un tablero | | |

## 11. Límite de evidencia

- Verificado leyendo el código:
- Verificado compilando:
- Verificado ejecutando el arnés:
- Verificado con `cmp`:
- Inferido:
- No medido:
- Desconocido:

## 12. Qué demuestra el arnés y qué no

| Afirmación | ¿Respaldada? | Por qué |
|---|---|---|
| La semilla 1 produce la transcripción de referencia. | | |
| Dos ejecuciones con semilla 1 son idénticas. | | |
| Otra semilla cambia las posiciones. | | |
| El programa es portátil. | | |
| El generador no tiene sesgo. | | |
| La lógica del juego es correcta. | | |
| El archivo es testeable tal como se publica. | | |
| El arnés detecta toda regresión. | | |
| El archivo de referencia es una prueba por sí solo. | | |
| La traza es válida en otra plataforma. | | |

## 13. Severidad y coste

Un párrafo. Nombra el coste en trabajo real, no la fealdad del código. Di si el coste es alto o
crítico en tu contexto y por qué. No propongas una reescritura en este párrafo.

>

## 14. Integridad del archivo

- `git status` en el archivo:
- ¿Edité `original/`? (Debe ser no.)
- ¿Apliqué algún parche dentro del archivo? (Debe ser no.)
- ¿Dónde guardé mi copia temporal?
- ¿Edité `code/`? (Debe ser no.)

## 15. Lista de aceptación

- [ ] Escribí las cinco predicciones antes de inspeccionar la fuente.
- [ ] Nombré las tres costuras y las constantes del LCG.
- [ ] Compilé con las opciones del arnés y registré el compilador.
- [ ] Ejecuté `--seed 1` y vi la transcripción de seis líneas.
- [ ] Recalculé al menos una posición desde el estado.
- [ ] Ejecuté `./run.sh` y vi `same=yes` y `golden=yes`.
- [ ] Ejecuté el programa dos veces a mano y `diff` quedó vacío.
- [ ] Comparé con `cmp` y registré su código de salida.
- [ ] Ejecuté las semillas 2 y 7 y registré las posiciones.
- [ ] Expliqué por qué el ganador y el número de movimientos no cambiaron.
- [ ] Registré los anclajes del archivo: un `srand`, cinco `rand()`, sin `argv`, sin prueba.
- [ ] Expliqué por qué `srand(time(NULL))` dentro del bucle repite extracciones en un segundo.
- [ ] Afirmé por qué `rand()` está definido por la implementación y por qué importa `RAND_MAX`.
- [ ] Escribí una nota de plataforma para una traza heredada capturada.
- [ ] Distinguí el oráculo de la prueba.
- [ ] Nombré qué no arregla el control de semilla.
- [ ] Declaré el límite de evidencia: una semilla, un banco, un entorno.
- [ ] Enlacé el arreglo estructural con `common-01` y `common-04`.
- [ ] El archivo está sin modificar.

## 16. Siguiente paso

- Continúa con `archaeology-15` — análisis estático y advertencias del compilador.
- Si el bucle de reintentos te sorprendió, registra las cuatro extracciones de cada intento con una
  semilla fija.
- Si la pregunta de portabilidad sigue abierta, captura la salida del archivo con dos compiladores.
- Si no estás de acuerdo con la severidad, escribe el contexto que la cambia.
