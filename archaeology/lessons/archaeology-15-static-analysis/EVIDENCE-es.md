# archaeology-15 — Evidencia: análisis estático y advertencias del compilador

> Este es tu registro. Escríbelo para tu yo futuro.
> Registra lo que observaste, no lo que la lección esperaba que respondieras.

## 1. Entorno

- Compilador y versión (`c++ --version | head -1`):
- Estándar de C++ usado:
- Biblioteca estándar:
- Intérprete de comandos y versión:
- Núcleo (`uname -sr`):
- Directorio de trabajo:
- Directorio de la copia:
- Opciones exactas usadas:
- Algo inesperado en el entorno:

## 2. Predicción — escribe antes de compilar

No edites esta tabla después del paso 2. La brecha entre predicción y observación es la señal de
aprendizaje; reparar la predicción la destruye.

| # | Pregunta | Mi predicción | Cómo sabría que me equivoco |
|---:|---|---|---|
| 1 | ¿Cuántos diagnósticos produce `-Wall -Wextra`? | | |
| 2 | ¿A qué línea y columna apunta el caret? | | |
| 3 | ¿Sobrevive la advertencia a `-O0` y `-O2`? | | |
| 4 | ¿La produce `-Wall` sola? ¿Y `-Wextra` sola? | | |
| 5 | ¿Eliminar la declaración cambia `stdout` o el código de salida? | | |

Una frase falsable sobre la advertencia de este banco:

>

## 3. Compilar

Salida del arnés y código de salida:

```text

```

El diagnóstico, copiado literalmente:

```text

```

Comando usado para reproducirlo:

```bash

```

| Medición | Resultado |
|---|---|
| Código de salida sin `-Werror` | |
| Código de salida con `-Werror` | |
| Severidad sin `-Werror` | |
| Severidad con `-Werror` | |
| Etiqueta de la opción sin `-Werror` | |
| Etiqueta de la opción con `-Werror` | |

Conteos de advertencias:

| Opciones | Conteo de advertencias |
|---|---:|
| `-Wall` | |
| `-Wextra` | |
| `-Wall -Wextra` | |
| `-Wunused-variable` | |
| `-Wall -Wextra -Wno-unused-variable` | |

Qué prueba `-Wextra` sola sobre los conjuntos de opciones:

Qué prueba `-Wno-unused-variable`, y por qué no es la corrección:

Qué informó `-fanalyzer` en este banco:

## 4. Leer

Conteos de líneas:

| Archivo | Líneas | ¿Verificado? |
|---|---:|---|
| `code/warnings.cpp` | 9 | |
| `code/fixed.cpp` | 8 | |
| `run.sh` | 27 | |

Qué hace cada línea de `warnings.cpp`:

| Línea | Contenido | ¿Lee, escribe o ninguna? |
|---:|---|---|
| 5 | `int unusedValue = 0;` | |
| 6 | `int value = 10;` | |
| 7 | `std::printf("value=%d\n", value);` | |
| 8 | `return 0;` | |

Todas las apariciones del nombre:

```text

```

Argumento de ámbito — por qué esta búsqueda es completa para este nombre:

>

El diff entre la base y la corrección de referencia:

```text

```

Qué hace `run.sh`, por rango de líneas:

| Líneas | Qué hacen |
|---:|---|
| 2 | |
| 4–7 | |
| 9–11 | |
| 13 | |
| 15–16 | |
| 18–19 | |
| 22–25 | |
| 26–27 | |

Qué cuenta realmente esa cifra:

Qué prueba `fixed=ok` sobre la compilación estricta:

Qué compara realmente `match=yes`:

## 5. Triaje

| # | Diagnóstico | Opción | Ubicación | ¿Afirmación verdadera? | Clase | Decisión | Riesgo |
|---:|---|---|---|---|---|---|---|
| 1 | | | | | | | |

Alternativas consideradas:

| Respuesta | Por qué la acepté o la rechacé |
|---|---|
| Eliminar la declaración | |
| `(void)unusedValue;` | |
| `[[maybe_unused]]` | |
| `-Wno-unused-variable` | |
| Imprimir el valor | |

¿Es un defecto real, código muerto, un falso positivo o una excepción intencional? ¿Por qué?

>

Qué cambiaría mi clasificación:

>

## 6. Corregir

Ruta de la copia:

El diff de mi corrección contra la base:

```text

```

Comando de compilación y resultado:

```text

```

¿Mi archivo de trabajo coincide con `code/fixed.cpp`? (salida de `diff`):

```text

```

Qué decidí no cambiar:

## 7. Verificar

| Comprobación | Comando | Resultado |
|---|---|---|
| Bytes de `stdout` | `cmp build/before.txt build/after.txt` | |
| Estado de salida | `cmp build/before.exit build/after.exit` | |
| Tamaño de `stdout` | `wc -c < build/before.txt` | |
| Compilación estricta | `-Wall -Wextra -Werror` | |
| Compilación con `-Wpedantic` | `-Wall -Wextra -Werror -Wpedantic` | |
| `-O0` contra `-O2` | `cmp` de ambas salidas | |
| Control negativo | `cmp` de la salida original y la cambiada | |

La salida de 9 bytes, decodificada:

```text

```

El control negativo:

- Qué cambié:
- Qué informó la comparación:
- Código de salida de `cmp`:

El arnés después de la corrección:

```text

```

Por qué `warnings=1` sigue siendo correcto:

>

## 8. Qué demuestra la ejecución

| Afirmación | ¿Respaldada? | Evidencia |
|---|---|---|
| La base emite exactamente una advertencia. | | |
| La afirmación de la advertencia es verdadera. | | |
| El hallazgo es código muerto, no un defecto vivo. | | |
| La corrección preserva el comportamiento. | | |
| El banco corregido está limpio con `-Werror`. | | |
| La comparación de bytes detecta un cambio de comportamiento. | | |
| El programa es correcto. | | |
| No hay otros errores. | | |
| `match=yes` prueba igualdad de bytes. | | |
| Las advertencias sustituyen a las pruebas. | | |

Mi conclusión en una frase, con el compilador, las opciones y el ámbito:

>

## 9. Límite de la evidencia

Qué demuestra la compilación limpia:

>

Qué no demuestra la compilación limpia:

>

Qué no compara el arnés:

>

Qué no probé:

>

## 10. Lista de comprobación

- [ ] Escribí las cinco predicciones antes de compilar.
- [ ] Ejecuté `./run.sh` y registré las tres líneas y el código de salida.
- [ ] Reproduje el diagnóstico a mano y lo copié literalmente.
- [ ] Registré el compilador y la versión.
- [ ] Registré los códigos de salida con y sin `-Werror`.
- [ ] Registré los conteos de advertencias de cada conjunto de opciones.
- [ ] Leí los tres archivos y registré los conteos 9/8/27.
- [ ] Verifiqué la afirmación con `grep` y expuse el argumento de ámbito.
- [ ] Clasifiqué el hallazgo como código muerto.
- [ ] Rellené la fila de triaje y la tabla de alternativas.
- [ ] Copié el banco a `build/scratch/` antes de editar.
- [ ] Mi diff quita una declaración y no añade nada.
- [ ] Las compilaciones estricta y con `-Wpedantic` salen con 0 y sin salida.
- [ ] Mi archivo de trabajo coincide con `code/fixed.cpp`.
- [ ] `cmp` informa `stdout` idéntico y estado de salida idéntico.
- [ ] Registré la salida de 9 bytes.
- [ ] El control negativo hace fallar la comparación.
- [ ] Declaré el límite del arnés: igualdad de texto, no de bytes.
- [ ] Declaré qué demuestra la compilación limpia y qué no.
- [ ] `code/` no está modificado.
