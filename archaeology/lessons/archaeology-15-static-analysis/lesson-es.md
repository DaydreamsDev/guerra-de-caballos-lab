# archaeology-15 — Análisis estático y advertencias del compilador

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** análisis estático · **Prerrequisito:** archaeology-04 ·
> **Tiempo estimado:** 60 minutos
> **Lectura:** ~22 minutos

## Por qué existe esta lección

Una advertencia del compilador es la evidencia más barata que un código puede darte. Llega sin una
prueba, sin un depurador y sin una persona usuaria. Nombra un archivo, una línea, una columna y el
análisis que la produjo. Cuesta una compilación.

También es fácil de ignorar. Una advertencia no es un error. El programa sigue ejecutándose y la
compilación sigue teniendo éxito. Por eso precisamente se pudren las advertencias: cada una es lo
bastante pequeña para posponerla, y juntas se convierten en ruido de fondo que esconde la que
importaba.

El banco de pruebas es deliberadamente pequeño. `code/warnings.cpp` tiene nueve líneas y produce
exactamente un diagnóstico: una variable local sin usar. `code/fixed.cpp` es el mismo programa con
esa declaración eliminada. `run.sh` compila ambos, cuenta la advertencia y compara la salida:

```text
warnings=1
fixed=ok
match=yes
```

Tres líneas, una advertencia, una eliminación. La lección no es la eliminación. La lección es el
razonamiento que hace segura la eliminación y la evidencia que lo demuestra.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar qué es el análisis estático y dónde se sitúan las advertencias del compilador entre sus
  herramientas;
- leer un diagnóstico como cuatro hechos: ubicación, severidad, mensaje y opción;
- reproducir una advertencia fuera del arnés, con y sin `-Werror`;
- distinguir un defecto real, código muerto, un falso positivo y una excepción intencional;
- hacer el triaje de cada advertencia de una unidad de traducción con una decisión escrita por
  diagnóstico;
- verificar la afirmación de una advertencia en vez de creerla o descartarla;
- elegir entre eliminar, usar de verdad, `[[maybe_unused]]` y suprimir, y explicar por qué;
- corregir una advertencia sin cambiar el comportamiento observable y demostrar que no cambió;
- explicar qué demuestra una compilación limpia con `-Werror` y qué no demuestra;
- leer un arnés con criterio: `match=yes` no es automáticamente igualdad de bytes;
- escribir un registro de advertencias que otra persona pueda reproducir.

## El concepto: una advertencia es una afirmación sobre el código, no un veredicto

### El análisis estático en una frase

El **análisis estático** es razonar sobre un programa sin ejecutarlo. El compilador es el analizador
estático que ya ejecutas. Cada compilación analiza el código fuente, resuelve nombres, comprueba
tipos y aplica un conjunto de reglas; algunas de esas reglas emiten diagnósticos.

| Técnica | ¿Ejecuta el programa? | Encuentra | No encuentra |
|---|---|---|---|
| Advertencias del compilador | No | Patrones que el compilador puede probar desde el código. | Nada que dependa de valores en tiempo de ejecución. |
| Comprobador de tipos | No | Incompatibilidades de tipo, conversiones ausentes. | Errores lógicos que pasan el tipo. |
| `clang-tidy`, `cppcheck` | No | Estilo, patrones de error, flujo local. | Hechos interprocedurales profundos; algunos falsos positivos. |
| GCC `-fanalyzer` | No | Caminos, fugas, desreferencias nulas. | Escala, precisión y código que no puede modelar. |
| Pruebas | Sí | Comportamientos que alguien decidió afirmar. | Todo lo que no se afirma. |
| Sanitizadores | Sí | Errores de memoria y comportamiento indefinido en ejecución. | Caminos que nunca se ejecutan. |
| Fuzzing | Sí | Caídas con entradas generadas. | Incorrección semántica que no provoca una caída. |

La tabla es el punto: ninguna técnica cubre todo el espacio. Las advertencias son baratas,
repetibles y están disponibles en cada compilación, así que van primero. No son suficientes.

### Qué hizo realmente el compilador

El proceso de compilación es, a grandes rasgos: preprocesar, analizar sintácticamente, analizar
semánticamente, optimizar y generar código. Una advertencia de variable sin usar se produce en la
fase inicial del compilador, antes de la optimización. Puedes verificar esa afirmación: la
advertencia aparece con `-O0` y con `-O2`, sin cambios. La optimización no puede eliminar un
diagnóstico que la fase inicial (front end) ya emitió.

El compilador no adivina. Tiene un modelo preciso del lenguaje: sabe que `unusedValue` es un objeto
local, que su inicializador es un literal entero, que ninguna expresión lo lee y que nunca se toma
su dirección. Dentro de ese modelo, «sin usar» es un hecho.

El límite es el modelo, no la competencia del compilador. Un nombre puede leerlo solo una
herramienta externa, ensamblador en línea o una macro que el compilador no expande en esta
configuración. En esos casos la advertencia es un falso positivo o un hallazgo verdadero sobre una
excepción intencional. Por eso existe el triaje.

### Anatomía de un diagnóstico

Este es el diagnóstico del banco, producido por GCC 16.2.1:

```text
code/warnings.cpp:5:9: warning: unused variable ‘unusedValue’ [-Wunused-variable]
    5 |     int unusedValue = 0;
      |         ^~~~~~~~~~~
```

Léelo en cuatro partes:

| Parte | En este diagnóstico | Qué te dice |
|---|---|---|
| Ubicación | `code/warnings.cpp:5:9` | Archivo, línea y columna. La columna 9 es el nombre, no el tipo. |
| Severidad | `warning` | La compilación continúa salvo que `-Werror` esté activo. |
| Mensaje | `unused variable ‘unusedValue’` | La afirmación, en palabras del compilador. |
| Opción | `[-Wunused-variable]` | La regla que se activó. Es tu asa. |

La línea de código y el caret son contexto. El caret apunta a la declaración porque eso es lo que el
compilador considera sin usar. No apunta a un uso ausente porque no hay uso al que apuntar.

La opción es la parte más útil. Nombra el análisis, es lo bastante estable para buscarla en el
manual del compilador y es el token exacto que usarías para suprimir el diagnóstico. La supresión es
el último recurso, pero no puedes decidir en su contra hasta conocer su nombre.

### La advertencia no es el defecto

«Variable sin usar» significa que el valor nunca se lee. No dice por qué. El mismo mensaje puede
describir cuatro situaciones con cuatro costes distintos:

| Situación | Clase | Coste típico | Cómo saberlo |
|---|---|---|---|
| Una local que quedó de un experimento. | Código muerto | Bajo: ruido, sin cambio de comportamiento. | Sin comentario, sin referencia, inicializador trivial. |
| Un resultado que debía leerse. | Defecto real | Alto: el programa está incompleto. | El nombre sugiere un resultado; el código cercano lo espera. |
| Un parámetro exigido por una interfaz. | Intencional | Ninguno, pero indocumentado. | La firma la fija una retrollamada o una clase base. |
| Un uso que el compilador no puede ver. | Falso positivo | Ninguno, pero exige evidencia. | El nombre lo lee asm, una macro o una herramienta externa. |

El banco es la primera fila. `unusedValue` se inicializa con `0`, nunca se lee, nunca se pasa a
ninguna parte y nunca se toma su dirección. Eliminarla quita código muerto. No repara un error vivo,
porque no hay un error vivo en esa línea.

Dilo con precisión en tu registro. «La advertencia era un defecto» es falso aquí. «La advertencia
era verdadera y el código estaba muerto» es exacto. La precisión sobre la severidad es parte de la
habilidad.

### Triaje: cuatro preguntas por advertencia

El **triaje** es el proceso de decisión entre leer un diagnóstico y cambiar el código. Para cada
advertencia, responde cuatro preguntas por escrito:

1. **¿Es verdadera la afirmación?** Verifícala. Para una local sin usar, busca el nombre y revisa su
   ámbito. Para una posible desreferencia nula, sigue el camino.
2. **¿Qué tipo de hallazgo es?** Defecto real, código muerto, falso positivo o excepción
   intencional.
3. **¿Cuál es la corrección correcta más barata?** Eliminar, usar, marcar, reestructurar o suprimir.
   Barata no es lo mismo que corta: una supresión es corta y cara más adelante.
4. **¿Cómo demostraré que el comportamiento no cambió?** Captura salida y estado de salida antes y
   después, compara el código y ejecuta la compilación estricta.

Una fila de triaje es una advertencia. Si una compilación emite cuarenta advertencias, hay cuarenta
filas, aunque treinta compartan una causa. Agrupar está permitido después de que existan las filas;
saltarse filas no.

### La advertencia del banco, con triaje

| Campo | Valor |
|---|---|
| Diagnóstico | `unused variable ‘unusedValue’` |
| Opción | `-Wunused-variable` |
| Ubicación | `code/warnings.cpp:5:9` |
| ¿Afirmación verificada? | Sí: el nombre aparece una vez, en su propia declaración. |
| Ámbito | Local a `main`; ninguna otra unidad de traducción puede leerla. |
| Clase | Código muerto. |
| Severidad | Baja: ningún comportamiento observable depende de ella. |
| Decisión | Eliminar la declaración. |
| Riesgo de comportamiento | Ninguno: el inicializador es un literal y el objeto no tiene efectos. |

Esa última fila merece cuidado. Eliminar una declaración no siempre preserva el comportamiento. Si
la declaración fuera `std::string scratch;`, el constructor y el destructor podrían reservar memoria
o lanzar una excepción; un tipo propio también podría registrar algo. Si fuera `Lock guard(mutex);`,
eliminarla quitaría un bloqueo. El argumento de seguridad no es «no se usaba»; es «no se usaba y su
construcción y destrucción no tienen ningún efecto observable». Para `int unusedValue = 0;` ese
argumento es fácil. Para un tipo de clase, tienes que comprobarlo.

### La corrección no es la única opción

Hay cinco respuestas posibles. Solo una es correcta para este banco, y las razones importan más que
la elección.

| Respuesta | Qué hace | Coste | Veredicto aquí |
|---|---|---|---|
| Eliminar la declaración. | Quita el código muerto y la advertencia. | Ninguno en este caso. | **Elegida.** |
| `(void)unusedValue;` | Crea una lectura falsa que silencia al compilador. | Conserva código muerto; esconde la intención. | Rechazada. |
| `[[maybe_unused]]` | Documenta que el objeto se deja sin usar a propósito. | Ninguno, pero debe ser verdad. | Rechazada: nada la exige. |
| `-Wno-unused-variable` | Desactiva la regla para toda la compilación. | Esconde hallazgos reales futuros. | Rechazada. |
| Imprimir el valor. | Hace que la variable se use. | Cambia la salida; falla la aceptación. | Rechazada. |

`[[maybe_unused]]` no es un truco sucio. Es la herramienta correcta cuando la declaración debe
existir: un parámetro fijado por una interfaz, una variable que solo lee el depurador, un binding
estructurado con un nombre sin usar. El atributo es documentación de una restricción real. Usarlo
en código muerto convierte un hallazgo en una mentira permanente.

La supresión tiene la misma forma a mayor escala. Un pragma por línea, una opción por archivo o un
`-Wno-...` por proyecto cambian evidencia por silencio. A veces ese intercambio es correcto, por
ejemplo cuando una cabecera de terceros emite advertencias que no puedes corregir. Entonces la
supresión necesita tres cosas: una razón, el ámbito más estrecho posible y una nota que diga cuándo
se puede quitar. «Hacía ruido» no es una razón.

### Qué demuestra una compilación limpia

Una compilación limpia con `-Wall -Wextra -Werror` demuestra un hecho estrecho y útil:

> Con este compilador, esta versión, estas opciones y estas unidades de traducción, no se activó
> ningún diagnóstico habilitado.

No demuestra que el programa sea correcto. No demuestra que no exista ningún error. No demuestra que
las pruebas sean buenas, que el código sea legible ni que un compilador futuro vaya a callar.
`code/fixed.cpp` está limpio y sigue teniendo un número mágico, ninguna prueba y ninguna validación.

La afirmación sigue valiendo la pena porque es reproducible y falsable. El error es inflarla.
Escribe el ámbito junto al resultado.

### Las advertencias son un presupuesto, no una puntuación

Un código tiene un presupuesto de advertencias. En cero, cada advertencia nueva es visible de
inmediato. En trescientas, una nueva es invisible y el equipo aprende a pasar la vista por encima de
la salida de compilación.

`-Werror` es una compuerta, no una herramienta de limpieza. Activarlo en un árbol con trescientas
advertencias no las corrige; bloquea cada compilación y enseña a la gente a saltarse la compuerta.
El orden es:

1. hacer el triaje y corregir las advertencias existentes, o registrar un conteo de referencia;
2. activar `-Werror` en el ámbito limpio;
3. mantener la compuerta y tratar cada advertencia nueva como un fallo de compilación;
4. revisar el conjunto de opciones cuando cambie el compilador o el estándar.

Este banco tiene una advertencia, así que la compuerta es honesta de inmediato. Eso es un lujo, y la
lección es notarlo.

## Antes de empezar

Lee estos documentos completos:

- [`README.md`](README.md) — la ficha de la lección y el criterio de aceptación.
- [archaeology-04](../archaeology-04-correctness-failures/lesson.md) — el prerrequisito: síntoma,
  causa raíz y la disciplina de escribir un hallazgo.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — las
  clases de fallo y la escala de severidad que usa esta lección.

También necesitas:

- un compilador de C++20 (`c++`, `g++` o `clang++`);
- `bash`, `grep`, `diff`, `cmp` y `od`;
- un directorio de trabajo bajo `build/`.

> **Regla:** no edites `code/`. Copia el banco a `build/scratch/` antes de cambiar nada. `code/` es
> la base que ejecutará la siguiente persona.

## El recorrido de 60 minutos

| Min | Paso | Qué haces | Evidencia |
|---:|---|---|---|
| 0–6 | Predecir | Te comprometes con respuestas sobre el conteo, la ubicación y la salida | Tabla de predicciones |
| 6–16 | Compilar | Ejecutas el arnés y luego reproduces el diagnóstico a mano | Diagnóstico exacto, códigos de salida |
| 16–26 | Leer | Lees el banco y el arnés línea por línea | Conteos de líneas, diff del código |
| 26–40 | Triar | Verificas la afirmación, la clasificas y eliges una corrección | Una fila de triaje, alternativas |
| 40–50 | Corregir | Eliminas la declaración muerta en una copia de trabajo | Diff de la copia, compilación limpia |
| 50–56 | Verificar | Comparas bytes, estado de salida y compilaciones estrictas | `cmp`, `-Werror`, control negativo |
| 56–60 | Registrar | Rellenas `EVIDENCE-es.md` | Entrada de portafolio |

Si demostrar el límite del arnés te lleva diez minutos, esa investigación es la lección.

### La comprobación de ingeniería de esta lección

Los seis verbos son **compilar → leer → triar → corregir → verificar → registrar**. El paso 1 añade
una predicción antes del primer verbo; es un hábito, no un séptimo verbo.

| Verbo | Dónde ocurre | Qué produce |
|---|---|---|
| **Compilar** | Paso 2 | El diagnóstico real y los códigos de salida. |
| **Leer** | Paso 3 | Los conteos de líneas, el diff y el límite del arnés. |
| **Triar** | Paso 4 | Una decisión escrita por advertencia. |
| **Corregir** | Paso 5 | Un cambio mínimo en la copia, con una razón declarada. |
| **Verificar** | Paso 6 | Bytes idénticos, estado idéntico, compuerta limpia. |
| **Registrar** | Paso 7 | `EVIDENCE-es.md` con comandos y salidas. |

## Paso 1 — Predecir (6 min)

Antes de compilar, responde estas cinco preguntas por escrito. El objetivo es comprometerte con
respuestas que puedan estar equivocadas.

1. ¿Cuántos diagnósticos produce `-Wall -Wextra` sobre `code/warnings.cpp`?
2. ¿A qué línea y columna apunta el caret: a la declaración o a un uso?
3. ¿Sobrevive la advertencia a `-O0`? ¿Sobrevive a `-O2`?
4. ¿La produce `-Wall` sola? ¿La produce `-Wextra` sola?
5. Si eliminas la declaración, ¿cambia `stdout`? ¿Cambia el código de salida?

Una predicción útil es falsable. «Las advertencias son malas» no lo es. «Una advertencia en la línea
5, columna 9, apuntando a la declaración; aparece en ambos niveles de optimización; `-Wall` sola
basta; `stdout` sigue siendo `value=10` y el código de salida sigue siendo 0» sí lo es.

### Una predicción razonada

Antes de ejecutar nada, yo escribiría:

- una advertencia, porque solo una declaración no tiene lector;
- línea 5, columna 9, bajo `unusedValue`;
- sobrevive a `-O0` y a `-O2`, porque es un hallazgo semántico;
- `-Wall` sola la produce y `-Wextra` sola no, porque pertenece a `-Wunused-variable`, que `-Wall`
  habilita;
- `stdout` sigue siendo `value=10\n` y el código de salida sigue siendo 0, porque el objeto
  eliminado nunca se lee.

No estoy tan seguro del texto exacto con `-Werror`. Mi conjetura es que la severidad pasa a `error`,
la opción pasa a `[-Werror=unused-variable]` y la compilación termina con código 1. El paso 2
comprueba esa conjetura.

## Paso 2 — Compilar (10 min)

### Ejecuta primero el arnés

```bash
cd archaeology/lessons/archaeology-15-static-analysis
./run.sh
printf 'exit=%s\n' "$?"
```

Esperado:

```text
warnings=1
fixed=ok
match=yes
exit=0
```

El arnés comprueba la parte mecánica de la aceptación, no la lección. Te dice que la base tiene una
advertencia, que el banco corregido compila con `-Werror` y que los dos programas imprimen el mismo
texto. No te muestra el diagnóstico. Para eso, compila a mano.

### Reproduce el diagnóstico

```bash
mkdir -p build
c++ -std=c++20 -Wall -Wextra -O2 code/warnings.cpp -o build/warnings
printf 'exit=%s\n' "$?"
```

Esperado, con GCC 16.2.1:

```text
code/warnings.cpp: In function ‘int main()’:
code/warnings.cpp:5:9: warning: unused variable ‘unusedValue’ [-Wunused-variable]
    5 |     int unusedValue = 0;
      |         ^~~~~~~~~~~
exit=0
```

Dos hechos que debes notar. La compilación termina con código 0: una advertencia no es un fallo por
defecto. Y el diagnóstico va a `stderr`, por eso `run.sh` lo redirige a `build/warnings.txt` antes
de contarlo.

### Convierte la advertencia en un fallo

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 code/warnings.cpp -o build/warnings-strict
printf 'exit=%s\n' "$?"
```

Esperado:

```text
code/warnings.cpp: In function ‘int main()’:
code/warnings.cpp:5:9: error: unused variable ‘unusedValue’ [-Werror=unused-variable]
    5 |     int unusedValue = 0;
      |         ^~~~~~~~~~~
cc1plus: all warnings being treated as errors
exit=1
```

La ubicación y el mensaje son idénticos. Cambiaron la severidad, la etiqueta de la opción y el
código de salida. Eso es todo lo que hace `-Werror`: promueve un diagnóstico a fallo de compilación.
No analiza nada nuevo.

La última línea es específica de GCC. Otro compilador lo dirá de otra forma, y esa es una razón para
registrar el compilador y la versión en tu evidencia.

### Pregunta qué conjunto de opciones lo produjo

Ejecuta el mismo código con opciones distintas. Captura el conteo, no el texto completo.

```bash
for flags in "-Wall" "-Wextra" "-Wall -Wextra" "-Wunused-variable" \
             "-Wall -Wextra -Wno-unused-variable"; do
    count="$(c++ -std=c++20 ${flags} -O2 code/warnings.cpp -o build/probe 2>&1 \
             | grep -c 'warning:' || true)"
    printf '%-40s warnings=%s\n' "${flags}" "${count}"
done
```

Esperado con GCC 16.2.1:

| Opciones | Advertencias | Qué muestra |
|---|---:|---|
| `-Wall` | 1 | La regla está en `-Wall`. |
| `-Wextra` | 0 | La regla no está en `-Wextra`. |
| `-Wall -Wextra` | 1 | La unión, no una suma. |
| `-Wunused-variable` | 1 | La regla específica basta. |
| `-Wall -Wextra -Wno-unused-variable` | 0 | Un `-Wno-` posterior gana. |

`-Wall` es un nombre engañoso. Es un conjunto curado, no todas las advertencias que conoce el
compilador. `-Wextra` añade otro conjunto curado. Ninguno es «todo», y ninguno es una garantía de
calidad. La opción específica es la que nombra el análisis, y el orden importa porque la última
gana.

Esa última fila es una demostración, no una recomendación. Acabas de probar que la supresión
funciona. No probaste que sea una buena idea.

### Mira un analizador más profundo

GCC puede ejecutar un análisis sensible a caminos con `-fanalyzer`:

```bash
c++ -std=c++20 -Wall -Wextra -fanalyzer -O2 code/warnings.cpp -o build/analyzer
```

En este banco informa la misma variable sin usar y nada más. Es lo esperado: no hay caminos,
reservas de memoria ni punteros que analizar. El comando está aquí para situar la herramienta, no
para venderla. `-fanalyzer` es una opción de GCC; si usas otro compilador, omite este comando y
dilo en el registro. Es más lento, tiene sus propios falsos positivos y sigue sin ver valores en
tiempo de ejecución.

### Registra el entorno

Escribe el compilador y la versión ahora, mientras los tienes:

```bash
c++ --version | head -1
```

Necesitarás esa línea en `EVIDENCE-es.md`. Un registro de advertencias sin versión del compilador no
es reproducible.

## Paso 3 — Leer (10 min)

### Lee los tres archivos

```bash
wc -l code/warnings.cpp code/fixed.cpp run.sh
nl -ba code/warnings.cpp
nl -ba code/fixed.cpp
nl -ba run.sh
```

Los conteos:

| Archivo | Líneas | Qué es |
|---|---:|---|
| `code/warnings.cpp` | 9 | El banco con una local muerta. |
| `code/fixed.cpp` | 8 | El mismo banco sin esa local. |
| `run.sh` | 27 | Compila, cuenta, aplica la compuerta, compara e imprime. |

`warnings.cpp` es lo bastante pequeño para leerlo completo:

```cpp
#include <cstdio>

int main()
{
    int unusedValue = 0;
    int value = 10;
    std::printf("value=%d\n", value);
    return 0;
}
```

La línea 5 declara e inicializa `unusedValue`. La línea 6 declara `value`. La línea 7 lee `value` y
lo escribe en `stdout`. La línea 8 devuelve 0. Nada lee `unusedValue`, nada toma su dirección y nada
la pasa a una función. La afirmación del compilador es verdadera.

`fixed.cpp` es idéntico salvo por la línea 5 ausente. No lo des por hecho:

```bash
diff -u code/warnings.cpp code/fixed.cpp
```

Esperado:

```text
--- code/warnings.cpp
+++ code/fixed.cpp
@@ -2,7 +2,6 @@

 int main()
 {
-    int unusedValue = 0;
     int value = 10;
     std::printf("value=%d\n", value);
     return 0;
 }
```

Una eliminación, cero adiciones. Esa es toda la corrección. Si tu diff tiene cualquier otra cosa,
cambiaste más de lo que pedía la advertencia.

### Lee el arnés

`run.sh` tiene 27 líneas, y cada una importa para el límite de la evidencia.

| Líneas | Qué hacen |
|---:|---|
| 2 | `set -euo pipefail`: detente ante un error, una variable sin definir o una tubería fallida. |
| 4–7 | Resuelven el directorio del script, eligen el compilador y crean `build/`. |
| 9–11 | Compilan `warnings.cpp` con `-Wall -Wextra -O2`; envían `stderr` a `warnings.txt`. |
| 13 | Cuenta las líneas que contienen `warning:` en ese archivo. |
| 15–16 | Compilan `fixed.cpp` con `-Werror`; una advertencia aborta el script. |
| 18–19 | Capturan el `stdout` de ambos programas en variables del intérprete de comandos. |
| 21 | Imprime el conteo de advertencias. |
| 22–25 | Comparan las dos cadenas; imprimen `match=no` y salen con 1 si difieren. |
| 26–27 | Imprimen `fixed=ok` y `match=yes`. |

De esa lectura se siguen tres propiedades:

1. **El conteo es un conteo de líneas.** `grep -c 'warning:'` cuenta líneas coincidentes, no
   diagnósticos. En este banco son lo mismo. Si la palabra aparece en más de una línea, o en un
   nombre de archivo, el conteo se desvía. Sabe qué estás contando.
2. **La compuerta es real.** Por `set -e`, una advertencia en `fixed.cpp` con `-Werror` detiene el
   script antes de imprimir cualquier línea de éxito. Por lo tanto, `fixed=ok` significa que la
   compilación estricta pasó.
3. **La comparación es igualdad de texto, no de bytes.** `$(...)` elimina los saltos de línea
   finales de ambas salidas. Dos programas que difieren solo en saltos de línea finales comparan
   igual. `match=yes` es evidencia fuerte, no prueba de bytes idénticos.

Esa tercera propiedad no es un defecto del arnés; es un límite documentado que debes conocer antes
de citarlo. El paso 6 cierra la brecha con `cmp`.

### Comprueba tu predicción

- Una advertencia, en la línea 5, columna 9, apuntando a la declaración.
- Sobrevive a `-O0` y a `-O2`.
- `-Wall` sola la produce; `-Wextra` sola no.
- Con `-Werror` la severidad pasa a `error`, la etiqueta a `-Werror=unused-variable` y el código de
  salida a 1.
- Eliminar la declaración deja `stdout` en `value=10\n` y el código de salida en 0.

Si predijiste que `-Wextra` sola la detectaría, asumiste que los conjuntos de opciones se solapan
más de lo que lo hacen. Si predijiste que la optimización quitaría la advertencia, situaste el
análisis después del optimizador. Vale la pena escribir ambos desajustes.

## Paso 4 — Triar (14 min)

### Verifica la afirmación antes de decidir

```bash
grep -n 'unusedValue' code/warnings.cpp
```

Esperado:

```text
5:    int unusedValue = 0;
```

Una coincidencia, la declaración. Esa es la evidencia de «ningún uso en esta unidad de traducción».
Luego haz explícito el argumento de ámbito: `unusedValue` se declara dentro de `main`, tiene
almacenamiento automático, no es `static`, no la captura una lambda y nunca se toma su dirección.
Ninguna otra unidad de traducción puede nombrarla. La ausencia de lectura es completa, no solo local
a este archivo.

Para una global o un miembro de clase, el mismo `grep` no bastaría. Tendrías que buscar en todo el
programa, incluido el código generado por macros y el código que toma la dirección. El ámbito del
nombre determina el ámbito de la búsqueda.

### Rellena una fila de triaje

Usa esta forma en `EVIDENCE-es.md`:

| # | Diagnóstico | Opción | Ubicación | ¿Afirmación verdadera? | Clase | Decisión | Riesgo |
|---:|---|---|---|---|---|---|---|
| 1 | `unused variable ‘unusedValue’` | `-Wunused-variable` | `warnings.cpp:5:9` | Sí: una coincidencia, la declaración | Código muerto | Eliminar | Ninguno: literal, sin efectos |

Cada columna es una afirmación que puedes defender. «Clase: código muerto» no es una sensación; se
sigue de la verificación y del ámbito. «Riesgo: ninguno» no es optimismo; se sigue del inicializador
y de la ausencia de un destructor con efectos.

### Considera las alternativas por escrito

Ya viste la tabla en la sección del concepto. Aplícala:

- **Eliminar.** Quita la advertencia y el código muerto. El banco no tiene otra referencia. Elegida.
- **`(void)unusedValue;`** Añade una lectura que solo existe para silenciar al compilador. La
  variable sigue muerta; quien lea después tiene que entender un uso falso. Rechazada.
- **`[[maybe_unused]]`** Documenta una excepción intencional. No hay interfaz, contrato de depurador
  ni comentario que pida la variable. Usarlo aquí documentaría una restricción que no existe.
  Rechazada.
- **Suprimir la opción.** Esconde toda la clase de hallazgo para cada línea de la compilación. Una
  variable muerta no justifica eso. Rechazada.
- **Usar el valor en la salida.** Cambia `stdout`, que el criterio de aceptación prohíbe. Rechazada.

La decisión no es «eliminar porque el compilador lo dijo». Es «eliminar porque la afirmación es
verdadera, el código está muerto, eliminarlo no tiene efecto observable y toda alternativa cuesta
más».

### Cuando la advertencia es un falso positivo

Un falso positivo es un diagnóstico cuya afirmación no es verdadera para este programa. Es más raro
que un hallazgo verdadero sobre código que existe por una razón que el compilador no puede ver.
Antes de clasificarlo de una u otra forma, demuestra el mecanismo: un nombre que lee ensamblador en
línea, una variable que solo inspecciona el depurador o una declaración que se conserva para una
compilación de depuración. Si el uso es real, la advertencia no es falsa; es un límite del modelo o
una excepción intencional.

El procedimiento es el mismo:

1. demuestra que el uso existe, nombrando el mecanismo;
2. si el uso es real pero invisible para el compilador, márcalo (`[[maybe_unused]]`) o reestructura
   para que el uso sea visible;
3. si el analizador se equivoca sobre un hecho que debería conocer, registra la versión del
   compilador y la reproducción mínima;
4. suprime solo con una razón y el ámbito más estrecho.

No empieces por el paso 4. Una supresión que sobrevive a su razón es peor que la advertencia que
escondió.

### Qué no es el triaje

El triaje no es una votación. Una persona puede hacerlo. No es un concurso de severidad y no es una
razón para reescribir el programa. La salida es una decisión por diagnóstico, con evidencia. Para
este banco el triaje completo es una fila. Está bien. Una tabla de una fila correcta supera a una
tabla de diez filas copiada de un blog.

## Paso 5 — Corregir (10 min)

### Copia, no edites

```bash
mkdir -p build/scratch
cp code/warnings.cpp build/scratch/warnings.cpp
```

De aquí en adelante, edita solo `build/scratch/warnings.cpp`. `code/warnings.cpp` se queda como
base.

### Elimina la declaración

Quita la línea 5. Cualquier editor sirve; el resultado debe ser exactamente:

```cpp
#include <cstdio>

int main()
{
    int value = 10;
    std::printf("value=%d\n", value);
    return 0;
}
```

Demuestra que el cambio es mínimo:

```bash
diff -u code/warnings.cpp build/scratch/warnings.cpp
```

Esperado: una línea eliminada, la declaración, y ningún otro bloque.

Si tu diff muestra líneas reordenadas, una cadena de formato cambiada o un comentario nuevo, hiciste
más de lo que pedía la advertencia. Vuelve atrás y hazlo otra vez. El cambio más pequeño que quita
la causa es el más fácil de revisar y el más fácil de demostrar seguro.

### Compila con la compuerta

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 \
    build/scratch/warnings.cpp -o build/scratch/warnings
printf 'exit=%s\n' "$?"
```

Esperado: sin salida, `exit=0`.

### Compara con la corrección de referencia

```bash
diff -u code/fixed.cpp build/scratch/warnings.cpp
```

Esperado: sin salida. Tu corrección y la corrección de referencia son el mismo archivo.

Si difieren, lee la diferencia antes de «corregirla». `fixed.cpp` es la clave de respuestas, pero el
ejercicio es el razonamiento. Copiar `fixed.cpp` sobre tu archivo de trabajo al principio produciría
una compilación limpia y no te enseñaría nada. Copiarlo al final como comprobación cruzada está
bien.

### Qué no hacer

- No añadas `(void)unusedValue;` y lo llames corrección. Silenciaste un hallazgo y conservaste el
  código muerto.
- No añadas `[[maybe_unused]]` sin una restricción que exija la variable.
- No añadas `-Wno-unused-variable` a `run.sh`. El arnés es evidencia; debilitarlo debilita la
  lección.
- No cambies la cadena de formato, el valor ni el `return`. Ninguno forma parte del hallazgo.

## Paso 6 — Verificar (6 min)

### Compara comportamiento, no intención

Compila el original y la copia corregida, captura `stdout` y el estado de salida por separado, y
compara bytes:

```bash
c++ -std=c++20 -Wall -Wextra -O2 code/warnings.cpp -o build/warnings
./build/warnings > build/before.txt
printf '%s\n' "$?" > build/before.exit

./build/scratch/warnings > build/after.txt
printf '%s\n' "$?" > build/after.exit

cmp build/before.txt build/after.txt && printf 'stdout: same bytes\n'
cmp build/before.exit build/after.exit && printf 'status: same\n'
wc -c < build/before.txt
od -c build/before.txt
```

Esperado:

```text
stdout: same bytes
status: same
9
0000000   v   a   l   u   e   =   1   0  \n
0000011
```

Nueve bytes: ocho caracteres de `value=10` y un salto de línea. Ambos programas escriben exactamente
esos bytes y salen con 0.

¿Por qué `cmp` y no `diff`? `diff` informa diferencias por líneas y puede ignorar un salto de línea
final ausente según cómo lo llames. `cmp` compara bytes. Para una afirmación de comportamiento, los
bytes son la unidad honesta.

### Comprueba otra vez la compilación estricta

```bash
c++ -std=c++20 -Wall -Wextra -Werror -Wpedantic -O2 \
    build/scratch/warnings.cpp -o build/scratch/warnings-pedantic
printf 'exit=%s\n' "$?"
```

Esperado: sin salida, `exit=0`. `-Wpedantic` añade diagnósticos de conformidad; aquí no hay ninguno
que añadir. Si se queja, lee el mensaje antes de suponer que el banco está mal.

### Comprueba ambos niveles de optimización

```bash
for level in -O0 -O2; do
    c++ -std=c++20 -Wall -Wextra -Werror "${level}" \
        build/scratch/warnings.cpp -o "build/scratch/warnings${level}"
    "build/scratch/warnings${level}" > "build/scratch/out${level}.txt"
done
cmp build/scratch/out-O0.txt build/scratch/out-O2.txt && printf 'levels: same bytes\n'
```

Esperado: `levels: same bytes`. La advertencia era un hallazgo de la fase inicial (front end) y la
salida no depende de la optimización. Vale la pena registrar ambos hechos.

### Ejecuta el control negativo

Una prueba que nunca has visto fallar no es evidencia. Copia el banco corregido una vez más, cambia
el valor impreso y confirma que el arnés lo nota. Hazlo en la copia de trabajo, nunca en `code/`.

```bash
cp build/scratch/warnings.cpp build/scratch/broken.cpp
sed -i 's/int value = 10;/int value = 11;/' build/scratch/broken.cpp
c++ -std=c++20 -Wall -Wextra -Werror -O2 \
    build/scratch/broken.cpp -o build/scratch/broken
```

Ahora compara su salida con la del original:

```bash
./build/warnings > build/original.txt
./build/scratch/broken > build/broken.txt
cmp build/original.txt build/broken.txt
printf 'cmp exit=%s\n' "$?"
```

Esperado: `cmp` sale con 1 e informa que los archivos difieren. Es el camino `match=no` del arnés en
miniatura. Acabas de ver que la comprobación rechaza un cambio de comportamiento, que es lo que hace
significativa su aceptación.

### Vuelve a ejecutar el arnés

```bash
./run.sh
```

Esperado, sin cambios:

```text
warnings=1
fixed=ok
match=yes
```

`warnings=1` es correcto y no es una regresión. El arnés compila deliberadamente el `warnings.cpp`
original para informar el conteo de advertencias de la base; en este banco es 1. Tu compilación
corregida es un artefacto aparte bajo `build/scratch/`, y está limpia.

### Declara el límite de la evidencia

Has probado que:

- el banco original emite un diagnóstico y su afirmación es verdadera;
- el banco corregido no emite ninguno con el conjunto de opciones entregado más `-Wpedantic`;
- el cambio de código es una eliminación;
- `stdout` y el estado de salida son idénticos byte a byte;
- la comparación puede fallar cuando cambia el comportamiento.

No has probado que el programa sea correcto, que no exista otro error ni que el compilador vaya a
callar en otra versión. Dilo en el registro.

## Paso 7 — Registrar (4 min)

Rellena [`EVIDENCE-es.md`](EVIDENCE-es.md). Como mínimo:

- compilador, versión, estándar y opciones exactas;
- las cinco predicciones, escritas antes de la primera compilación;
- la salida del arnés y el código de salida;
- el diagnóstico literal, con archivo, línea, columna, mensaje y opción;
- los códigos de salida con y sin `-Werror`;
- los conteos de advertencias para `-Wall`, `-Wextra` y la opción específica;
- los conteos de líneas: 9, 8 y 27;
- el diff del código: una eliminación, cero adiciones;
- la fila de triaje y las alternativas rechazadas, con razones;
- el diff de la copia y el resultado de la compilación estricta;
- los resultados de `cmp` para `stdout` y estado de salida, y el tamaño de 9 bytes;
- el control negativo y su resultado;
- el límite del arnés: igualdad de texto, saltos finales eliminados, `cmp` cierra la brecha;
- una frase que declare qué demuestra la compilación limpia y qué no.

Una frase como «la advertencia está corregida» es un estado. «GCC 16.2.1 con
`-Wall -Wextra -Werror` acepta el archivo de trabajo; el diff contra la base quita una declaración y
no añade nada; `cmp` informa un `stdout` idéntico de 9 bytes y un estado de salida idéntico; el
control negativo cambia un dígito y `cmp` falla» es un registro.

## El banco en detalle

| Ubicación | Qué hay |
|---:|---|
| `warnings.cpp:1` | `#include <cstdio>`, para `std::printf`. |
| `warnings.cpp:3` | `int main()`, el punto de entrada estándar. |
| `warnings.cpp:5` | `int unusedValue = 0;`, la declaración muerta. |
| `warnings.cpp:6` | `int value = 10;`, el valor vivo. |
| `warnings.cpp:7` | `std::printf("value=%d\n", value);`, la única salida. |
| `warnings.cpp:8` | `return 0;`, el único camino de salida. |
| `fixed.cpp:5` | `int value = 10;`; la declaración muerta ya no está. |
| `run.sh:9–11` | Primera compilación, advertencias a `stderr`, capturadas en un archivo. |
| `run.sh:13` | `grep -c 'warning:'`, el conteo. |
| `run.sh:15–16` | Segunda compilación con `-Werror`, la compuerta. |
| `run.sh:18–19` | Ambas salidas capturadas en variables. |
| `run.sh:22–25` | Comparación de texto y camino de fallo. |
| `run.sh:21,26–27` | Las tres líneas impresas. |

Los dos archivos de código difieren en exactamente una línea. El arnés difiere de una comparación
estricta de bytes en tres aspectos: la sustitución de comandos elimina los saltos de línea finales,
compara solo `stdout` y no compara los dos estados de salida; se apoya en `set -e` para detenerse
ante un estado distinto de cero. Ambos hechos son lo bastante pequeños para verificarlos a mano, que
es el objetivo de un banco.

## Qué demuestra este banco y qué no

| Afirmación | ¿Respaldada? | Por qué |
|---|---|---|
| La base emite exactamente una advertencia. | Sí | `run.sh` imprime `warnings=1`; la compilación manual muestra un diagnóstico. |
| La afirmación de la advertencia es verdadera. | Sí | `grep` encuentra una aparición, la declaración; el nombre es local. |
| El hallazgo es código muerto, no un defecto vivo. | Sí | Nada lee el valor; el inicializador no tiene efectos. |
| La corrección preserva el comportamiento. | Sí | `cmp` informa `stdout` y estado idénticos; el diff es una eliminación. |
| El banco corregido está limpio con `-Werror`. | Sí | La compilación estricta sale con 0 y sin salida. |
| La comparación de bytes detecta un cambio de comportamiento. | Sí | El control negativo hace fallar a `cmp`. |
| El programa es correcto. | No | Una compilación limpia no dice nada sobre lógica ni requisitos. |
| No hay otros errores. | No | Solo se comprobaron los diagnósticos habilitados, en una unidad. |
| `-Werror` seguirá en verde para siempre. | No | Una actualización del compilador puede añadir o cambiar diagnósticos. |
| `match=yes` prueba igualdad de bytes. | No | La sustitución de comandos quita saltos finales; usa `cmp`. |
| Las advertencias sustituyen a las pruebas. | No | Ven patrones del código, no comportamiento en ejecución. |
| Suprimir es corregir. | No | Quita el mensaje y conserva la causa. |

Esta tabla es la comprobación de honestidad. Un registro que dice «cero advertencias, por lo tanto
limpio» sin nombrar el compilador, las opciones y el ámbito está exagerando.

## Autocomprobación (binaria)

- [ ] Escribí las cinco predicciones antes de compilar.
- [ ] Ejecuté `./run.sh` y vi `warnings=1`, `fixed=ok`, `match=yes`, salida 0.
- [ ] Reproduje el diagnóstico a mano y lo copié literalmente.
- [ ] Registré el compilador y la versión.
- [ ] Vi el código de salida cambiar de 0 a 1 al añadir `-Werror`.
- [ ] Medí los conteos para `-Wall`, `-Wextra`, la opción específica y la supresión.
- [ ] Leí `warnings.cpp`, `fixed.cpp` y `run.sh` línea por línea.
- [ ] Registré los conteos de líneas: 9, 8 y 27.
- [ ] Verifiqué la afirmación con `grep` y expuse el argumento de ámbito.
- [ ] Clasifiqué el hallazgo como código muerto, no como defecto vivo.
- [ ] Rellené una fila de triaje con evidencia en cada columna.
- [ ] Rechacé `(void)`, `[[maybe_unused]]`, la supresión y el cambio de comportamiento, con razones.
- [ ] Copié el banco a `build/scratch/` antes de editar.
- [ ] Mi diff quita una declaración y no añade nada.
- [ ] La compilación estricta sale con 0 y sin salida.
- [ ] Mi archivo de trabajo coincide exactamente con `fixed.cpp`.
- [ ] `cmp` informa `stdout` idéntico y estado de salida idéntico.
- [ ] Registré la salida de 9 bytes.
- [ ] Ejecuté el control negativo y vi fallar la comparación.
- [ ] Declaré el límite del arnés: igualdad de texto, no de bytes.
- [ ] Declaré qué demuestra la compilación limpia y qué no.
- [ ] No edité `code/`.

Si una casilla no está marcada, la casilla sin marcar nombra el siguiente paso. No hay penalización
por repetir uno.

## Misconcepciones que esta lección refuta

| Idea equivocada | Por qué está mal | Qué hacer en su lugar |
|---|---|---|
| «Las advertencias son ruido.» | El ruido es lo que esconde la advertencia que importa. | Haz triaje de cada una; mantén el presupuesto en cero. |
| «Cero advertencias significa que no hay errores.» | Una compilación limpia comprueba patrones, no requisitos ni ejecución. | Acompaña la compuerta con pruebas y revisión. |
| «`-Wall` habilita todas las advertencias.» | Es un conjunto curado; `-Wextra` añade más y ambos omiten reglas. | Lee la lista de opciones; habilita lo que necesites. |
| «`-Werror` corrige el código.» | Solo promueve diagnósticos a fallos de compilación. | Corrige o tria el hallazgo primero. |
| «Eliminar una variable sin usar siempre es seguro.» | Un constructor o destructor puede tener efectos. | Comprueba el tipo y el inicializador antes de eliminar. |
| «Suprimir una advertencia es lo mismo que corregirla.» | El mensaje desaparece; la causa permanece. | Corrige la causa o documenta una excepción estrecha. |
| «El compilador siempre tiene razón.» | Su modelo puede perder usos en asm, macros o herramientas externas. | Verifica la afirmación; marca las excepciones reales. |
| «El compilador nunca acierta con el código sin usar.» | Normalmente acierta; sin usar significa no leído dentro de su modelo. | Demuestra que el uso existe antes de descartarlo. |
| «`match=yes` prueba que la salida es idéntica.» | La captura del intérprete de comandos quita saltos de línea finales. | Compara archivos con `cmp`. |
| «Una advertencia que sobrevive a `-O2` es un error del optimizador.» | Esta viene del análisis semántico, antes de optimizar. | Sitúa el hallazgo en el proceso. |
| «Las advertencias dependen del compilador, así que se ignoran.» | Las diferencias son reales, pero los hallazgos siguen siendo evidencia. | Registra el compilador y reproduce en otro si importa. |
| «El arnés es la lección.» | El arnés comprueba tres líneas; la lección es el razonamiento. | Usa el arnés como evidencia, no como sustituto. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Análisis estático | Razonar sobre un programa sin ejecutarlo. |
| Diagnóstico | Mensaje de una herramienta sobre el código: error, advertencia o nota. |
| Opción de advertencia | La regla nombrada que produjo un diagnóstico, como `-Wunused-variable`. |
| `-Wall` | Un conjunto curado de advertencias comunes, no literalmente todas. |
| `-Wextra` | Un conjunto curado adicional con reglas que `-Wall` no habilita. |
| `-Werror` | Tratar cada advertencia habilitada como un error. |
| Front end | Las etapas de análisis sintáctico y semántico, antes de optimizar. |
| Unidad de traducción | Un archivo fuente tras el preprocesado, compilado como unidad. |
| Código muerto | Código cuyo resultado no puede afectar al comportamiento observable. |
| Triaje | Decidir qué es un hallazgo y qué hacer con él, con evidencia. |
| Falso positivo | Diagnóstico cuya afirmación no es verdadera para este programa. |
| Supresión | Desactivar un diagnóstico sin cambiar la causa. |
| `[[maybe_unused]]` | Atributo de C++17 que documenta un no uso intencional. |
| Preserva el comportamiento | La salida y el estado observables no cambian. |
| Control negativo | Un cambio deliberado que debe hacer fallar una comprobación. |
| Presupuesto de advertencias | El número tolerado de advertencias; cero es el valor útil por defecto. |

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
