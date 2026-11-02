# archaeology-11 — Evidencia: entrada, salida y capacidad de prueba

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
| 1 | Fuentes que enlaza el binario de consola y el de prueba | | |
| 2 | Símbolos de stdio o consola indefinidos en `core.o` | | |
| 3 | Líneas que imprime la consola, y de dónde sale el conteo | | |
| 4 | Qué afirma `test.cpp` y qué omite | | |
| 5 | Qué se rompe al añadir una línea a la transcripción dentro del núcleo | | |

Una frase falsable sobre la frontera en esta línea base:

>

## 3. Inspección de la línea base

Archivos:

```text
code/core.h
code/core.cpp
code/console.h
code/console.cpp
code/test.cpp
code/main.cpp
```

Partes que verifiqué:

| Parte | Líneas | ¿Verificado? |
|---|---:|---|
| `core.h` | 13 | |
| `core.cpp` | 26 | |
| `console.h` | 5 | |
| `console.cpp` | 11 | |
| `test.cpp` | 20 | |
| `main.cpp` | 9 | |

Includes por archivo:

| Archivo | Incluye | ¿Imprime? |
|---|---|---|
| `core.h` | | |
| `core.cpp` | | |
| `console.h` | | |
| `console.cpp` | | |
| `test.cpp` | | |
| `main.cpp` | | |

Los campos de `GameResult`, clasificados:

| Campo | ¿Hecho del dominio o presentación? | Por qué |
|---|---|---|
| `lines` | | |
| `winner` | | |
| `moveCount` | | |

Qué afirma `test.cpp`:

- cantidad de líneas:
- ganador:
- número de movimientos:

Qué no afirma:

- texto de las líneas:
- orden de las líneas:
- determinismo:
- coincidencia con la consola:
- validez de la entrada:

## 4. Inspección del archivo

Anclajes que verifiqué en
`original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`:

| Elemento | Línea o conteo | ¿Verificado? |
|---|---:|---|
| `system("color 1f")` | 38 | |
| `getch()` en `main` | 42 | |
| `system("cls")` en `main` | 43 | |
| `while (jugada!=0)` | 45 | |
| `srand(time(NULL))` | 76 | |
| `movcab` | 234–724 | |
| Líneas con `cout` dentro de `movcab` | 19 | |
| Líneas con `cout` en todo el archivo | 62 | |
| Mensajes de caballo encerrado | 704, 712 | |
| `impresionorden` | 753–868 | |
| `getch()` en el bucle de turnos | 781, 806, 835, 861 | |
| `system("cls")` en el bucle de turnos | 784, 809, 838, 864 | |

Las otras variantes:

| Variante | Líneas | Líneas con `cout` | `getch` | `system("cls")` |
|---|---:|---:|---:|---:|
| `variant-01-baseline-2008` | | | | |
| `variant-02-records-2008` | | | | |
| `variant-03-pointers-dev-2008` | | | | |
| `variant-04-pointers-final-2008` | | | | |

El lado de la entrada en `variant-02`:

| Elemento | Línea | ¿Verificado? |
|---|---:|---|
| `char nombre[250];` | 32 | |
| `cin>>nom.nombre;` | 46 | |
| `movcab(..., char &pisadas)` | 285 | |
| `ordenjuego(..., char &pisadas)` | 775 | |
| `impresionorden(char pisadas)` | 802 | |

Qué esperaba que contuviera el archivo, y qué contiene realmente:

>

## 5. Ejecución de la línea base

Comando:

```bash
cd archaeology/lessons/archaeology-11-input-output
./run.sh
```

Salida observada:

```text
console_lines=
test_output=
test=ok
match=yes
```

- Código de salida:
- Compilador usado:
- Banderas observadas en `run.sh`:
- ¿La compilación pasó con `-Werror`? Si no, ¿qué diagnóstico apareció?

Qué significan las cuatro líneas:

- `console_lines`:
- `test_output`:
- `test=ok`:
- `match=yes`:

## 6. Prueba de la frontera

Comandos:

```bash
mkdir -p build
c++ -std=c++20 -Wall -Wextra -Werror -O2 -c code/core.cpp -o build/core.o
nm -C -u build/core.o
nm -C -u build/core.o | grep -E 'printf|cout|getch|fwrite|puts'
```

Símbolos indefinidos observados:

```text

```

El comando filtrado produjo:

- Salida:
- Código de salida:
- Qué prueba ese resultado:

Enlaza la prueba sin la consola:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 code/core.cpp code/test.cpp -o build/core-test
./build/core-test
```

- Resultado:
- ¿El enlace pasó sin `console.cpp`? (Debe pasar.)
- Qué imprimió la ejecución:

Resumen de dependencias:

| Binario | Fuentes | ¿Enlaza `console.cpp`? |
|---|---|---|
| `console-app` | | |
| `core-test` | | |

## 7. Separación en la copia de trabajo

Comandos:

```bash
mkdir -p build/scratch
cd build/scratch
cp ../../code/core.h ../../code/core.cpp ../../code/console.h \
   ../../code/console.cpp ../../code/test.cpp ../../code/main.cpp .
```

Qué se movió del núcleo al adaptador:

| Antes | Después |
|---|---|
| `lines` | |
| `moveCount` | |
| texto de la transcripción | |
| orden de la transcripción | |

Qué quedó en el núcleo:

- semilla:
- nombre del jugador:
- jugador inicial:
- movimientos:
- ganador:

Resumen del diff (o una descripción corta si el diff es largo):

```text

```

Comparación byte a byte:

```bash
./build/console-app > /tmp/console-baseline.txt
cd build/scratch
./console-app > /tmp/console-separated.txt
diff /tmp/console-baseline.txt /tmp/console-separated.txt
```

- Salida de `diff`:
- ¿Cambió la transcripción? (No debe cambiar.)
- ¿Edité `code/`? (Debe ser no.)

## 8. Prueba

Qué afirma la prueba incluida:

| Aserción | Detecta | No detecta |
|---|---|---|
| `lines.size() == 8` | | |
| `winner == "player"` | | |
| `moveCount == 3` | | |

La aserción frágil:

- Cambio que hice:
- Salida observada:
- Código de salida:
- Por qué el comportamiento del juego no cambió:
- Por qué la prueba falló igual:

Determinismo:

| Ejecución | Semilla | Mov. 1 | Mov. 2 | Mov. 3 | Ganador | Movimientos |
|---|---:|---:|---:|---:|---|---:|
| 1 | 11 | | | | | |
| 2 | 11 | | | | | |
| 3 | 12 | | | | | |

- ¿Las ejecuciones 1 y 2 son idénticas?
- ¿La ejecución 3 difirió en posiciones?
- ¿La ejecución 3 difirió en ganador o conteo? ¿Por qué?

La reparación del arnés:

- Comportamiento anterior de `match=yes`:
- Comparación nueva:
- Qué pasa cuando la consola imprime `moves=4`:
- Código de salida en ese caso:

Salida observada después de la reparación:

```text
console_lines=
test_output=
test=ok
match=yes
```

## 9. Resultado de la coincidencia

- `winner` de la consola:
- `winner` de la prueba:
- `moves` de la consola:
- `moves` de la prueba:
- ¿Coinciden los campos?
- ¿El arnés salió con 0?
- Si discreparon, ¿qué campo difirió y por qué?

La comprobación filosa (opcional):

- Comando:
- Líneas observadas en la consola:
- Líneas observadas en la prueba:
- Qué imprimiría el arnés incluido:
- Por qué discrepan los dos caminos:

## 10. Explicación

El coste de 2008, en un párrafo. Nombra las impresiones dentro de `movcab`, las teclas, los borrados
de pantalla y el carácter de dibujo en las firmas del dominio. Di qué impide cada uno.

>

Dos arreglos, dos tamaños:

- Arreglo mínimo:
- Arreglo estructural:
- Enlaces a Track A:
- Qué añade `construction-20` a la pregunta:
- Qué añade `common-02` a la pregunta:

Qué no arregla la separación:

- Validación:
- Números mágicos:
- Ganador:
- Adaptador de entrada:
- Archivo:
- Pruebas:
- Transcripción exacta:

La prueba de cambio:

| Cambio | Línea base | Separado |
|---|---|---|
| Añadir una línea a la transcripción | | |
| Cambiar la redacción del ganador | | |
| Añadir un adaptador silencioso | | |
| Afirmar la posición del primer movimiento | | |
| Ejecutar sin terminal | | |

Dónde cae cada edición:

- Línea base:
- Separado:
- Qué versión falla una prueba del dominio ante un cambio de presentación:
- Qué versión mantiene la prueba en verde:

## 11. Frontera de evidencia

- Verificado leyendo la fuente:
- Verificado compilando y enlazando:
- Verificado ejecutando la línea base:
- Verificado ejecutando la versión separada:
- Verificado con la comparación del arnés:
- Inferido:
- No medido:
- Desconocido:

## 12. Qué prueba y qué no prueba la separación

| Afirmación | ¿Respaldada? | Por qué |
|---|---|---|
| `core.o` no tiene dependencia de consola. | | |
| El binario de prueba corre sin `console.cpp`. | | |
| El núcleo es determinista para una semilla fija. | | |
| El núcleo no es dueño de presentación. | | |
| La consola y la prueba coinciden en el arnés incluido. | | |
| El arnés reparado comprueba la coincidencia. | | |
| La salida de consola separada no cambió. | | |
| El núcleo valida su entrada. | | |
| El `movcab` del archivo ya es comprobable. | | |
| El banco es el juego completo de caballos. | | |
| La separación es gratuita. | | |

## 13. Severidad y coste

Un párrafo. Nombra el coste en el trabajo real, no la fealdad del código. Di si el coste es medio o
alto en tu contexto y por qué. No propongas una reescritura en este párrafo.

>

## 14. Integridad del archivo

- `git status` en el archivo:
- ¿Edité `original/`? (Debe ser no.)
- ¿Apliqué algún parche dentro del archivo? (Debe ser no.)
- ¿Dónde guardé mi copia de trabajo?
- ¿Edité `code/`? (Debe ser no.)

## 15. Lista de aceptación

- [ ] Escribí las cinco predicciones antes de inspeccionar la fuente.
- [ ] Nombré las tres fuentes del binario de consola y las dos del binario de prueba.
- [ ] Compilé `core.cpp` solo e inspeccioné `nm -C -u`.
- [ ] Confirmé que no hay `printf`, `cout` ni `getch` en `core.o`.
- [ ] Ejecuté `./run.sh` y vi las cuatro líneas esperadas.
- [ ] Encontré el `match=yes` incondicional en `run.sh`.
- [ ] Conté 62 líneas con `cout` en `variant-01` y 19 dentro de `movcab`.
- [ ] Conté 5 `getch` y 5 `system("cls")`.
- [ ] Encontré los anclajes de entrada de `variant-02`: `char[250]`, `cin`, `pisadas`.
- [ ] Copié la línea base a `build/scratch/` antes de editar.
- [ ] Moví el formato de la transcripción del núcleo a `console.cpp`.
- [ ] La transcripción separada de consola es idéntica byte a byte con `diff`.
- [ ] Reproduje la aserción frágil: una línea extra hace fallar la prueba vieja.
- [ ] Comprobé el determinismo con dos ejecuciones de la misma semilla y dos semillas distintas.
- [ ] Reparé el arnés para que `match=yes` siga a una comparación real.
- [ ] Indiqué que la línea base todavía es dueña del formato de la transcripción.
- [ ] Indiqué que no hay adaptador de entrada.
- [ ] Indiqué que el banco no es el juego completo de caballos.
- [ ] Enlacé el arreglo estructural a `construction-20`, `construction-08` y `common-02`.
- [ ] El archivo está sin modificar.

## 16. Siguiente paso

- Continúa con `archaeology-12` — nombres y comentarios.
- Si el lado de la entrada quedó incompleto, añade un adaptador de `argv` y cuenta qué cambia.
- Si la transcripción importa byte a byte, congélala como archivo de referencia (`common-05`).
- Si no estás de acuerdo con la severidad, escribe el contexto que la cambia.
