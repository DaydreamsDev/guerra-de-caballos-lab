# archaeology-14 — Pruebas y determinismo

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** pruebas y determinismo · **Prerrequisito:** archaeology-03 ·
> **Tiempo estimado:** 90 minutos
> **Lectura:** ~19 minutos
> **Rol de la lección:** tronco

## Por qué existe esta lección

El archivo tiene cuatro variantes publicadas con código fuente. Cada una siembra con
`srand(time(NULL))` y luego extrae cinco valores con `rand()`. Ninguna acepta una semilla. Ninguna
incluye una prueba. El propio `05-known-issues.md` del archivo lo dice en una línea: "No automated
tests and no deterministic seed".

Ese es el `failure-016`, un fallo de práctica profesional. No cambia nada de las reglas del juego y
lo cambia todo en tu capacidad de demostrar que las reglas siguen en pie. Sin semilla no puedes
repetir un tablero. Sin traza no puedes distinguir un fallo corregido de uno que se movió de sitio.
Sin arnés, "funciona en mi máquina" es todo el plan de pruebas.

El catálogo también lista el `failure-003`: `srand(time(NULL))` dentro del bucle de reintentos.
`archaeology-04` lo diagnosticó como defecto de corrección. Esta lección mira la misma llamada desde
el lado de las pruebas, porque es exactamente el lugar donde se entregó la reproducibilidad.

El banco de pruebas es deliberadamente pequeño: un programa de 44 líneas, un archivo de referencia
de 6 líneas y un arnés de 27 líneas. El programa acepta `--seed` y usa el LCG de 64 bits del
laboratorio. El archivo de referencia es la transcripción exacta para la semilla 1. El arnés ejecuta
el programa dos veces con la misma semilla, compara ambas ejecuciones, luego compara la primera con
el archivo de referencia y termina con código distinto de cero cuando alguna comprobación falla.

La lección no es "añadir un framework de pruebas". Es "hacer el programa repetible, congelar una
transcripción y decir con precisión qué puede y qué no puede prometer esa transcripción".

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar por qué una prueba necesita una semilla, no solo un generador de números aleatorios;
- separar las cuatro entradas de una ejecución determinista: semilla, generador, lógica y entorno;
- leer `srand(time(NULL))` dentro de un bucle de reintentos y nombrar los dos fallos que provoca;
- ejecutar un programa con semilla dos veces y demostrar que las transcripciones son idénticas;
- congelar una traza de referencia y comparar una ejecución byte a byte contra ella;
- ejecutar otra semilla y explicar qué campos cambian y cuáles no pueden cambiar;
- afirmar por qué `rand()` por sí solo no es una traza de referencia portátil;
- escribir una nota de plataforma para una traza heredada capturada;
- distinguir un oráculo de una prueba, y una demostración de una comprobación de regresión;
- precificar la no determinación por coste de incidente, no por gusto;
- mantener el archivo de solo lectura mientras trabajas.

La lección está hecha cuando
otra persona puede reproducir tu ejecución y seguir tus motivos.

## El concepto: una prueba es una promesa que puedes repetir

### El determinismo es una propiedad de todo el proceso

Una ejecución es reproducible cuando toda entrada que pueda cambiar la salida está fijada o
registrada. En este programa hay cuatro:

| Entrada | Pregunta | Este banco |
|---|---|---|
| Semilla | ¿Dónde empieza el flujo? | `--seed N`, por defecto 1. |
| Generador | ¿Qué algoritmo avanza el flujo? | El LCG de 64 bits del laboratorio, escrito en el archivo. |
| Lógica | ¿Qué hace el programa con cada extracción? | `position = (position + draw + 1) % 8`, tres movimientos. |
| Entorno | ¿Qué más puede cambiar los bytes? | Formatos de `printf`, salto de línea, locale, ancho entero. |

Fija tres y deja una libre, y la ejecución no es reproducible. El archivo fija la lógica y el
entorno, y deja libre la semilla al llamar a `time(NULL)`. Esa única entrada libre basta para que
cualquier captura deje de ser evidencia.

El banco cierra la brecha convirtiendo la semilla en un argumento. No elimina la aleatoriedad: la
convierte en una entrada en lugar de una condición ambiental. Esa es toda la idea. Una prueba no
necesita un programa sin aleatoriedad. Necesita un programa cuya aleatoriedad puedas fijar.

### `rand()` es una interfaz, no un algoritmo

El estándar de C++ fija la firma de `rand()` y la garantía de que devuelve un valor entre `0` y
`RAND_MAX`. No fija la secuencia. Cada implementación elige su propio generador. Dos habituales
discrepan en la secuencia y en el rango:

| Implementación | `RAND_MAX` | Familia de generador | ¿Misma semilla, mismos números? |
|---|---:|---|---|
| glibc (Linux) | 2147483647 | Retroalimentación aditiva, estado de 31 bits | Sí, en glibc. |
| CRT de Microsoft (MSVC 6.0 / moderno) | 32767 | Congruencial lineal, estado de 15 bits | Sí, en MSVC. |
| glibc frente a MSVC | Distinto | Distinto | No. |

Así que `srand(1); rand()` no es un valor portátil. Es un valor en una plataforma. Una traza de
referencia construida con `rand()` es una traza del runtime de C, no del juego. Puede seguir siendo
útil, pero solo con una nota de plataforma que diga qué runtime la produjo.

Hay una segunda trampa: `rand()` es estado global. Cada llamada avanza un único flujo compartido.
Añade una impresión de depuración que extraiga un número, reordena dos llamadas o salta una
extracción en una ruta de error, y todos los valores posteriores se desplazan. El archivo lo muestra
en miniatura: cinco extracciones, y la quinta es `opcion=(rand() % 8)+1` dentro de `vecjug()` en
`variant-01:180`. Cambia el código de colocación y también cambia el generador de movimientos, sin
tocar `vecjug()` en absoluto.

El banco evita ambas trampas: el generador está escrito en el archivo, su estado es una variable
local y la función de extracción recibe el límite como argumento. Mismo código, misma semilla,
mismos bytes, en toda plataforma que tenga un tipo entero sin signo de 64 bits.

### Una traza de referencia es un oráculo, no una prueba

Una traza de referencia es un archivo con la salida esperada. Responde una pregunta: "¿qué debería
imprimir el programa?". No ejecuta nada, no compara nada y no puede fallar. La prueba es la
combinación de tres cosas:

1. un comando que produce salida;
2. un oráculo que dice cuál debería ser esa salida;
3. una comparación que devuelve un código de salida distinto de cero cuando difieren.

`code/tests/golden-seed-1.txt` es el oráculo. `run.sh` es la prueba. Si borras las líneas `exit 1`,
todavía tienes una demostración: imprime `same=no` y continúa, y un trabajo de CI la llamaría verde.
Esa distinción es la diferencia entre evidencia y decoración.

### `srand(time(NULL))` dentro del bucle de reintentos

Esta es la función de colocación del archivo, `variant-01:69-87`:

```cpp
void cabubicacion()
{
    int i=1,tirocab1,tirocab2;

    while  (i==1)
    {
        srand(time(NULL));
        tirocab1= rand() % 51;
        tirocab2= rand() % 51;
        cab1= rand() % 64;
        cab2= rand() % 64;
        if(cab1 != cab2 && tirocab1 != tirocab2)
        {
            i++;
        }
    }
    ...
}
```

El bucle reintenta hasta que los dos caballos caen en celdas distintas y obtienen números de salida
distintos. La semilla se recalcula en cada intento. `time(NULL)` tiene granularidad de un segundo,
así que cada reintento dentro del mismo segundo recibe la misma semilla y, por tanto, las mismas
cuatro extracciones. Si esas extracciones colisionan, el siguiente intento colisiona exactamente
igual. El bucle gira hasta que el reloj avance.

Se siguen dos consecuencias:

- **La reproducibilidad desaparece.** El tablero depende del reloj de pared, que no es una entrada
  que puedas escribir y repetir.
- **Los reintentos son falsos.** Un reintento debería muestrear un valor nuevo. Aquí vuelve a
  muestrear el mismo valor, así que el bucle es una espera activa hasta el siguiente segundo, no un
  muestreo por rechazo.

Este es el punto didáctico de toda la lección: una semilla solo sirve cuando se elige una vez, se
registra y se pasa como argumento. Una semilla recalculada desde el reloj es entropía disfrazada de
semilla.

## Antes de empezar

Lee esto completo:

- [`README-es.md`](README-es.md) — la ficha de la lección, el comando de compilación y el criterio
  de aceptación.
- [archaeology-03](../archaeology-03-reverse-spec/lesson-es.md) — el prerrequisito: una
  especificación de comportamiento y las primeras trazas, tomadas del código.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, la
  taxonomía de fallos; §5, la escala de severidad; §6, tono y ética.

También necesitas:

- un clon local del código fuente del archivo;
- un compilador de C++20 (`c++`, `g++` o `clang++`);
- `bash`, `diff` y `cmp`;
- un directorio temporal fuera de `original/`.

> **Regla:** el archivo es de solo lectura. Copia cualquier fichero que quieras cambiar a un
> directorio temporal; nunca edites `original/`. El banco de esta lección también es evidencia:
> cópialo antes de editarlo.

## El recorrido de 90 minutos

| Min | Paso | Qué haces | Evidencia |
|---:|---|---|---|
| 0–10 | Predecir | Te comprometes con respuestas sobre semilla, repetición y portabilidad | Tabla de predicción |
| 10–25 | Sembrar | Lees el banco, encuentras las tres costuras, ejecutas la semilla 1 | Transcripción `seed=1` |
| 25–35 | Ejecutar dos veces | Ejecutas el arnés y dos ejecuciones manuales | `same=yes` |
| 35–45 | Comparar | Comparas con el archivo de referencia, byte a byte | `golden=yes`, `cmp` con salida 0 |
| 45–70 | Explicar | Ejecutas otras semillas, escribes la nota de plataforma y el coste | Tabla de semillas y nota |
| 70–90 | Registrar | Rellenas `EVIDENCE-es.md`; confirmas que el archivo está limpio | Entrada de portafolio |

Si la pregunta de portabilidad te lleva veinte minutos, esa
investigación es la lección.

### La comprobación de ingeniería en esta lección

El Track B usa el mismo hábito que el Track A, con otro verbo: el comportamiento ya existe, así que
lo **congelas** antes de cambiar nada.

| Paso | Dónde ocurre | Qué produce |
|---|---|---|
| **Predecir** | Paso 1 | Una conjetura falsable sobre semillas, repetición y portabilidad. |
| **Sembrar** | Paso 2 | Una entrada explícita y una ruta determinista. |
| **Ejecutar dos veces** | Paso 3 | Dos transcripciones y una comparación con código de salida. |
| **Comparar** | Paso 4 | Una coincidencia byte a byte contra un oráculo congelado. |
| **Explicar** | Paso 5 | Por qué importa la semilla y qué no puede prometer la traza. |
| **Registrar** | Paso 6 | `EVIDENCE-es.md` con comandos, entorno y límites. |

## Paso 1 — Predecir (10 min)

Antes de abrir el código, responde por escrito estas cinco preguntas. El objetivo es comprometerte
con respuestas que puedan estar equivocadas.

1. ¿El archivo acepta una semilla en la línea de comandos? Si no, ¿qué decide el estado inicial?
2. Si ejecutas una variante heredada dos veces dentro del mismo segundo, ¿obtienes el mismo tablero?
   ¿Por qué?
3. Si ejecutas el banco dos veces con `--seed 1`, ¿la salida es idéntica?
4. Si ejecutas el banco con `--seed 2`, ¿cambia el ganador?
5. ¿Se puede repetir con el compilador de Microsoft una traza capturada con `rand()` en
   Linux?

Una predicción útil es falsable. "El código es aleatorio" no lo es. "El archivo no tiene argumento
de semilla, así que el tablero lo decide el reloj, y dos ejecuciones en el mismo segundo producen el
mismo tablero" sí lo es.

### Una predicción trabajada

Antes de leer, yo escribiría:

- el `void main()` del archivo no toma argumentos, así que no hay entrada de semilla; `time(NULL)`
  la decide;
- dos ejecuciones en el mismo segundo comparten semilla y, por tanto, las cuatro extracciones de
  colocación;
- el banco imprime las mismas seis líneas dos veces con `--seed 1`, porque la semilla es la única
  variable;
- `--seed 2` cambia las posiciones y deja `winner` y `moves` iguales, porque el banco fija el final
  después de tres movimientos;
- una traza de glibc no se puede repetir en MSVC, porque `rand()` está definido por la
  implementación.

Cada afirmación se puede comprobar en los cuatro pasos siguientes. Si tu predicción dice "las
pruebas son buenas", has hecho un juicio, no una estimación. Escribe el mecanismo que esperas y
luego ponlo a prueba.

## Paso 2 — Sembrar (15 min)

### Leer el banco

```bash
cd archaeology/lessons/archaeology-14-tests-determinism
nl -ba code/seed_control.cpp
```

El programa tiene 44 líneas y tres costuras que conviene nombrar:

| Costura | Líneas | Qué hace |
|---|---:|---|
| Lectura de argumentos | 25–31 | Lee `--seed N`; vuelve a 1 cuando la forma no coincide. |
| `set_seed` | 13–16 | Escribe la semilla en el estado del generador. Nada más toca el estado. |
| `below` | 18–22 | Avanza el LCG y devuelve `state % bound`. |

El generador es un congruencial lineal de 64 bits con las constantes MMIX:

```cpp
randomState = randomState * 6364136223846793005ULL + 1442695040888963407ULL;
```

`below(3)` devuelve `0`, `1` o `2`. El paso del movimiento suma uno, así que cada movimiento avanza
la posición entre `1` y `3` celdas. La posición da la vuelta con `% 8`. Tres movimientos, una línea
cada uno, y después un final fijo.

Compílalo tú, con las mismas opciones que usa el arnés:

```bash
mkdir -p build
c++ -std=c++20 -Wall -Wextra -Werror -O2 code/seed_control.cpp -o build/seed-control
./build/seed-control --seed 1
```

Salida esperada:

```text
seed=1
move=1 position=2
move=2 position=5
move=3 position=0
winner=player
moves=3
```

### La aritmética detrás de la traza

La transcripción no es magia. Puedes recalcular cada posición desde la semilla. El estado aparece
aquí en decimal, recortado a los valores que importan:

| Movimiento | Estado tras la extracción | `state % 3` | Paso | Posición |
|---:|---:|---:|---:|---:|
| 1 | 7806831264735756412 | 1 | 2 | 2 |
| 2 | 9396908728118811419 | 2 | 3 | 5 |
| 3 | 11960119808228829710 | 2 | 3 | 0 |

El movimiento 1 empieza en la posición 0, suma 2 y cae en 2. El movimiento 2 suma 3 y cae en 5. El
movimiento 3 suma 3, pasa de 8 y cae en 0. Si tu ejecución no coincide con esta tabla, cambiaron el
generador o las constantes, no el juego.

Esto es lo que compra el determinismo: la traza se deriva, no solo se observa. Puedes discutirla,
recalcularla y comprobar un port contra ella.

### Leer la siembra del archivo

En el código fuente del archivo:

```bash
grep -n 'srand\|rand()' \
    original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
grep -c 'srand' original/variant-0*/source/*.cpp
grep -rn 'argv\|argc' original --include=*.cpp
```

El primer comando informa de un `srand` y cinco llamadas a `rand()`: líneas 76, 78, 79, 81, 82 y
180. El segundo informa de una llamada de siembra por variante con código fuente. El tercero no
imprime nada: ninguna variante lee `argc` ni `argv`. No hay argumento de semilla que pasar.

Después lee el bucle alrededor de la línea 73:

```text
73: while  (i==1)
76:     srand(time(NULL));
78:     tirocab1= rand() % 51;
79:     tirocab2= rand() % 51;
81:     cab1= rand() % 64;
82:     cab2= rand() % 64;
84:     if(cab1 != cab2 && tirocab1 != tirocab2)
```

La semilla se recalcula en cada reintento. Ese es el defecto, y se ve en cuatro líneas.

Las otras variantes repiten el patrón en `variant-02:87`, `variant-03:99` y `variant-04:105`, con
las mismas cinco extracciones. El precursor, `variant-00:36`, siembra una vez fuera de su bucle: la
versión menor del mismo error, porque sigue usando el reloj.

### Qué significa control de semilla aquí

El control de semilla son tres propiedades, y el banco tiene las tres:

1. **Una entrada.** La semilla es un argumento, no una condición ambiental.
2. **Una ruta.** Nada más en el programa lee el reloj, el entorno ni un archivo.
3. **Un dueño.** Una sola función escribe el estado del generador, así que un revisor puede ver
   todas las escrituras.

Cuando un programa tiene estas tres cosas, una prueba puede nombrar su entrada. Cuando tiene una
sola, la prueba nombra una esperanza.

## Paso 3 — Ejecutar dos veces (10 min)

### Ejecutar el arnés

```bash
./run.sh
```

Salida esperada:

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

Lee `run.sh` antes de confiar en él. Tiene 27 líneas y cada una importa:

- `set -euo pipefail` se detiene en el primer error, incluida una variable sin definir;
- `-std=c++20 -Wall -Wextra -Werror -O2` convierte una advertencia en un fallo de compilación;
- `firstRun="$(...)"` captura la salida estándar, y la sustitución de comandos recorta los saltos
  de línea finales;
- los bloques `if [[ ... != ... ]]` comparan transcripciones completas, no campos;
- `exit 1` convierte una discrepancia en una prueba fallida.

Los dos últimos puntos son la diferencia entre una demostración y una comprobación. Un script que
imprime `same=no` y termina con 0 no es una prueba. Un script que termina con 1 sí lo es.

### Hazlo a mano

```bash
./build/seed-control --seed 1 > build/run-a.txt
./build/seed-control --seed 1 > build/run-b.txt
diff build/run-a.txt build/run-b.txt
```

`diff` no imprime nada y termina con 0. Dos ejecuciones, una semilla, cero diferencias. Eso es
`same=yes` como hecho, no como etiqueta.

### Qué demuestra `same=yes`

- La semilla se lee y se aplica.
- El estado del generador no se comparte con nada que cambie entre ejecuciones.
- El formato es estable entre dos procesos.
- Nada en el programa lee el reloj, el entorno ni un archivo.

### Qué no demuestra `same=yes`

| Afirmación | ¿Demostrada? | Por qué |
|---|---|---|
| El programa es determinista para toda semilla. | No. | Se ejecutó una semilla dos veces. |
| El programa es determinista en otra plataforma. | No. | Las dos ejecuciones usaron el mismo binario y la misma libc. |
| La lógica del juego es correcta. | No. | El ganador y el número de movimientos están fijados. |
| La salida es idéntica byte a byte. | No con `run.sh`. | La sustitución de comandos recorta los saltos finales. |
| La prueba falla cuando el programa cambia. | Todavía no. | Eso es el paso 4, con una discrepancia. |

## Paso 4 — Comparar (10 min)

### Leer el oráculo

```bash
cat code/tests/golden-seed-1.txt
```

```text
seed=1
move=1 position=2
move=2 position=5
move=3 position=0
winner=player
moves=3
```

Seis líneas, sin comentarios. El archivo es un oráculo: dice cuáles deberían ser los bytes. No puede
ejecutarse y no puede fallar.

### Comparar bytes, no impresiones

```bash
./build/seed-control --seed 1 | diff - code/tests/golden-seed-1.txt
./build/seed-control --seed 1 | cmp - code/tests/golden-seed-1.txt
```

`diff` no imprime nada y termina con 0. `cmp` no imprime nada y termina con 0, lo que significa que
los flujos son idénticos byte a byte, incluido el salto de línea final. Esto es más estricto que
`run.sh`, que compara cadenas después de recortar los saltos finales. Ambos son válidos; responden
preguntas distintas. `run.sh` pregunta "¿es la misma transcripción?", `cmp` pregunta "¿son los
mismos bytes?".

Ejecútalo otra vez:

```bash
./run.sh
```

`golden=yes` aparece solo después de que pasen las dos comprobaciones: las dos ejecuciones coinciden
y la primera coincide con el archivo. El orden importa. Una coincidencia con el archivo de
referencia en un programa que cambia entre ejecuciones es casualidad, no resultado.

### Qué demuestra la igualdad

- El lector de semilla produce la misma semilla con la que se capturó el archivo de referencia.
- Las constantes del generador y el orden de extracción no cambiaron.
- La fórmula del movimiento y el `% 8` no cambiaron.
- Las cadenas de formato y el orden de las líneas no cambiaron.
- La ejecución es repetible en esta máquina con este compilador.

### Qué no demuestra la igualdad

| Afirmación | ¿Demostrada? | Por qué |
|---|---|---|
| El juego es correcto. | No. | La transcripción son tres movimientos de un banco. |
| La traza es portátil. | No. | Se capturó con un compilador y una libc. |
| La semilla es la única entrada. | En parte. | No hay reloj ni archivos; el locale sigue siendo ambiental. |
| El archivo de referencia está completo. | No. | Cubre una semilla y tres movimientos. |
| El archivo se comporta igual. | No. | El archivo no tiene argumento de semilla. |
| La prueba detecta toda regresión. | No. | Un cambio fuera de estas seis líneas es invisible. |

### Cuando aparece una discrepancia

Una discrepancia con la traza es información, no una emergencia. Las causas habituales, por orden de
frecuencia:

1. **La semilla no se leyó.** `--seed 2` o una forma de argumento distinta cambia la transcripción.
2. **El generador cambió.** Otro multiplicador, incremento o ancho de palabra cambia cada
   extracción.
3. **El orden de extracción cambió.** Una extracción nueva antes del bucle desplaza las tres
   posiciones.
4. **La fórmula cambió.** Se quitó el `+ 1`, o `% 8` pasó a `% 64`.
5. **El formato cambió.** Un espacio, una etiqueta o un salto de línea bastan.
6. **El entorno cambió.** Un formato dependiente del locale, o una plataforma donde `int` no tiene
   32 bits.

Diagnostica imprimiendo primero la línea de semilla. Si `seed=` difiere, el problema está antes del
generador. Si `seed=` coincide y la primera posición difiere, el problema está en la extracción o en
la fórmula. Si solo difiere la última línea, el problema está en el formato.

No edites el archivo de referencia para que la ejecución pase. Eso convierte una prueba fallida en
una prueba falsa.

## Paso 5 — Explicar (25 min)

### Otra semilla

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

La semilla 2 empieza como la semilla 1 y diverge en el movimiento 2, porque la primera extracción
da el mismo paso por casualidad. Es normal: semillas distintas pueden compartir un prefijo. El flujo
es distinto, no la forma.

El ganador y el número de movimientos no cambian, y esa es la lectura honesta de este banco: el
final está fijado después del bucle. La semilla cambia la ruta aleatoria, no el guion. Un juego real
terminaría porque un caballo captura al otro, y entonces la semilla también cambiaría el final. Este
banco no puede mostrar eso, y el registro debe decirlo.

### Por qué `rand()` por sí solo no es portátil

`rand()` te da una interfaz y un rango, no una secuencia. Se siguen tres hechos:

- **La secuencia está definida por la implementación.** glibc y MSVC producen números distintos con
  la misma semilla, y ninguno promete mantener su secuencia entre versiones.
- **`RAND_MAX` es distinto.** En esta máquina es 2147483647; en MSVC es 32767. Una extracción
  `% 51` usa bits distintos del estado en cada plataforma.
- **El estado es global.** Cualquier llamada nueva, en cualquier unidad de traducción, desplaza
  todas las extracciones posteriores.

Así que una traza de referencia capturada con `rand()` es una traza de un runtime, no de un
programa. Sigue siendo útil como prueba de regresión *en ese runtime*. No es una especificación, y
no se puede entregar a otro equipo como "la salida esperada" sin una nota.

### La nota de plataforma que necesita una traza heredada

Si capturas una transcripción de un binario heredado, registra al menos estos campos. Una traza sin
ellos es una anécdota:

| Campo | Por qué hace falta | Ejemplo |
|---|---|---|
| Compilador y versión | Otro código generado y otro CRT. | Microsoft Visual C++ 6.0. |
| Runtime de C / libc | Es dueño de `rand()` y de `RAND_MAX`. | MSVCRT, `RAND_MAX` 32767. |
| Arquitectura y ancho de palabra | Afecta al ancho entero y al desbordamiento. | x86, `int` de 32 bits. |
| Origen de la semilla | Sin él no hay nada que repetir. | `time(NULL)`: no repetible. |
| Valor exacto de la semilla | La única entrada reproducible. | Capturado instrumentando una copia temporal. |
| Línea de comandos | Los argumentos cambian la ruta. | `guerra-de-caballos.exe`. |
| Bytes de salida | Los saltos de línea y la codificación son parte de la traza. | CRLF, salida CP437. |
| Declaración de alcance | Dice para qué es válida la traza. | "Este binario, este runtime, esta compilación." |

El archivo falla en la cuarta fila: `time(NULL)` no es una entrada que puedas escribir. Para
capturar una traza siquiera, tienes que parchear una copia temporal para sembrar con un valor fijo,
y entonces la traza pertenece a la copia parcheada, no al artefacto publicado. Dilo en el registro.
Una traza de un binario modificado sigue siendo evidencia, pero es evidencia sobre la modificación.

### Severidad y coste

La escala de severidad del track pone los "resultados no reproducibles" en **alta**: corregir en el
parche mínimo. Es correcto, y la razón es económica, no estética.

Sin semilla:

- un informe de fallo dice "el caballo apareció encima del otro" y nadie puede reproducirlo;
- una corrección no se puede demostrar, porque los tableros de antes y después difieren por razones
  ajenas a la corrección;
- una refactorización no se puede comprobar, porque cada ejecución es un experimento nuevo;
- un port no se puede validar, porque no hay transcripción esperada contra la que comparar;
- una prueba inestable se culpa a la prueba, cuando la causa real es un generador sin semilla.

El coste se paga en el peor momento: durante un incidente, cuando el reloj corre y la única pregunta
es "¿este cambio lo rompió?". Una semilla cuesta un argumento. Su ausencia cuesta la capacidad de
responder esa pregunta.

### Dos arreglos, dos tamaños

**Arreglo mínimo — añadir la semilla y congelar una traza.** Acepta `--seed N`, siembra una vez
antes de la primera extracción y guarda en el repositorio un archivo de referencia con un arnés que
termine con código distinto de cero si no coincide. Eso es lo que hace este banco en 77 líneas
repartidas en tres archivos. Quita el reloj de la ruta y te da una comprobación de regresión para la
transcripción.

**Arreglo estructural — ser dueño del generador y de la traza.** Mueve el generador detrás de una
interfaz pequeña, inyéctalo en el código que necesita aleatoriedad y especifica su algoritmo y su
ancho en la documentación. Mantén las trazas de referencia en control de versiones, ejecútalas en CI
y registra la nota de plataforma para toda traza que aún dependa del entorno. En el archivo, eso
significa reemplazar `rand()` por un generador con nombre y pasar una semilla desde `main`, lo que
también exige arreglar `void main()`.

Enlaces al Track A: `common-02` (arnés determinista), `common-05` (trazas de referencia),
`construction-08` (frontera de módulo) y `construction-20` (hexagonal, donde el adaptador de consola
es dueño del entorno).

### Qué no arregla el control de semilla

- El ganador fijado del banco. Una traza con semilla no hace correcta la lógica del juego.
- El sesgo de módulo en `% bound`. Es pequeño aquí y real en general; usa un método de rechazo
  cuando importe.
- El `void main()` del archivo. Un argumento de semilla necesita un `main(int, char**)` estándar.
- El estado global del archivo. Sembrar un generador global sigue siendo estado global.
- La validación de entrada ausente. `--seed abc` se convierte en semilla 0 por `atoi`.
- La nota de plataforma. Un generador portátil elimina la dependencia del runtime; no elimina la
  necesidad de registrar el entorno para capturas heredadas.
- El resto del juego. Tres movimientos no son las 930 líneas del original.

## Paso 6 — Registrar (20 min)

Rellena [`EVIDENCE-es.md`](EVIDENCE-es.md). Como mínimo:

- configuración: compilador, libc, núcleo y shell;
- las cinco predicciones, escritas antes de inspeccionar;
- las tres costuras y las constantes del generador;
- los anclajes del archivo: un `srand` y cinco `rand()` por variante, sin `argv`, con líneas;
- la tabla aritmética de la semilla 1 y la transcripción observada;
- dos ejecuciones con la misma semilla y el resultado de `diff`;
- la comparación byte a byte con `cmp` y su código de salida;
- las semillas 2 y 7, con los campos que cambiaron y los que no;
- la nota de plataforma para una traza heredada capturada;
- el límite de evidencia: una semilla, un banco, un entorno;
- la comprobación de integridad del archivo.

Una frase como "el programa ahora es determinista" es una opinión. "La semilla 1 produce las
posiciones 2, 5, 0 dos veces, `cmp` termina con 0 contra el archivo de referencia, las semillas 2 y
7 producen posiciones distintas y el archivo no tiene `argv`" es un registro.

El criterio de aceptación de esta lección son exactamente dos hechos, ambos producidos por el arnés:

```text
same=yes
golden=yes
```

Si no puedes producir los dos, la línea sin marcar nombra el siguiente paso. No hay penalización por
repetir uno.

## El original en detalle

Hechos que puedes citar, con líneas de `variant-01`:

| Ubicación | Qué hay |
|---:|---|
| 32 | `void main()` — sin `argc`, sin `argv`, sin entrada de semilla. |
| 69–87 | `cabubicacion()` — colocación y orden de salida. |
| 73 | `while (i==1)` — reintenta hasta que las extracciones son distintas. |
| 76 | `srand(time(NULL));` — resembrado en cada reintento. |
| 78–79 | Dos extracciones `rand() % 51` para el orden de salida. |
| 81–82 | Dos extracciones `rand() % 64` para las celdas. |
| 84 | La comprobación de distinción que impulsa el reintento. |
| 180 | `opcion=(rand() % 8)+1` — la quinta extracción, dentro de `vecjug()`. |
| 930 | Fin del archivo; sin prueba, sin archivo de compilación, sin semilla. |

Las otras variantes repiten el patrón: `variant-02:87`, `variant-03:99`, `variant-04:105` para la
semilla, y `variant-02:231`, `variant-03:245`, `variant-04:257` para la quinta extracción. El
precursor, `variant-00:36`, siembra una vez fuera de su bucle, lo cual es mejor y sigue dependiendo
del reloj.

El `05-known-issues.md` del archivo registra ambos defectos con sus propias palabras:
`srand(time(NULL))` dentro del bucle de reintentos es "non-reproducible placement; repeated
seeding", y la sección de práctica de ingeniería dice "No automated tests and no deterministic
seed". El laboratorio no inventa el diagnóstico.

## Qué demuestra este arnés y qué no

| Afirmación | ¿Respaldada? | Por qué |
|---|---|---|
| La semilla 1 produce la transcripción de referencia. | Sí | `golden=yes`, `cmp` con salida 0. |
| Dos ejecuciones con semilla 1 son idénticas. | Sí | `same=yes`, `diff` vacío. |
| Otra semilla cambia las posiciones. | Sí | Las semillas 2 y 7 difieren de la 1. |
| El programa es portátil. | No | Un compilador, una libc. |
| El generador no tiene sesgo. | No | `% bound` tiene sesgo en general. |
| La lógica del juego es correcta. | No | El final está fijado. |
| El archivo ahora es testeable. | No | No tiene argumento de semilla ni `main` estándar. |
| El arnés detecta toda regresión. | No | Cubre seis líneas y una semilla. |
| El archivo de referencia es una prueba por sí solo. | No | Es un oráculo; `run.sh` es la prueba. |
| La traza es válida en otra plataforma. | No | Necesita la nota de plataforma. |

Esta tabla es la comprobación de honestidad. Un registro que dice "el código está probado" sin
nombrar la semilla, la plataforma y el alcance está exagerando. Di "para la semilla 1, en este
compilador" cuando eso es lo que ejecutaste.

## Autocomprobación (binaria)

- [ ] Escribí las cinco predicciones antes de abrir el código.
- [ ] Leí `code/seed_control.cpp` y nombré las tres costuras.
- [ ] Compilé con `-std=c++20 -Wall -Wextra -Werror -O2` y registré el compilador.
- [ ] Ejecuté `--seed 1` y vi la transcripción de seis líneas.
- [ ] Recalculé al menos una posición desde el estado del LCG.
- [ ] Ejecuté `./run.sh` y vi `same=yes` y `golden=yes`.
- [ ] Ejecuté el programa dos veces a mano y `diff` quedó vacío.
- [ ] Comparé con `cmp` y registré su código de salida.
- [ ] Ejecuté las semillas 2 y 7 y registré las posiciones.
- [ ] Expliqué por qué el ganador y el número de movimientos no cambiaron.
- [ ] Registré los anclajes del archivo: un `srand`, cinco `rand()`, sin `argv`.
- [ ] Expliqué por qué `srand(time(NULL))` dentro del bucle es peor que no tener semilla.
- [ ] Afirmé por qué `rand()` está definido por la implementación y por qué importa `RAND_MAX`.
- [ ] Escribí una nota de plataforma para una traza heredada capturada.
- [ ] Distinguí el oráculo de la prueba.
- [ ] Nombré qué no arregla el control de semilla: ganador, sesgo, `void main`, globales,
      validación.
- [ ] Precifiqué el defecto por coste de incidente, no por gusto.
- [ ] Enlacé el arreglo estructural con `common-02` y `common-05`.
- [ ] El archivo está sin modificar (`git status` en el archivo está limpio).

Si una casilla no está marcada, la casilla sin marcar nombra el siguiente paso. No hay penalización
por repetir uno.

## Misconcepciones que esta lección refuta

| Concepto erróneo | Por qué es falso | Qué hacer en su lugar |
|---|---|---|
| "Un programa aleatorio no se puede probar." | La aleatoriedad puede ser una entrada; una ejecución con semilla es repetible. | Convierte la semilla en argumento y congela una traza. |
| "La misma semilla da la misma salida en todas partes." | Las secuencias de `rand()` están definidas por la implementación. | Sé dueño del generador o registra la plataforma. |
| "`rand()` es estándar, así que es portátil." | La interfaz es estándar; la secuencia no. | Especifica el algoritmo y el ancho. |
| "Un archivo de referencia es una prueba." | Es un oráculo; no puede ejecutarse ni fallar. | Acompáñalo con un comando, una comparación y un código de salida. |
| "Ejecutar dos veces demuestra determinismo." | Demuestra una semilla en un entorno. | Declara la semilla, la plataforma y el alcance. |
| "`srand(time(NULL))` es una semilla." | Es entropía; no se puede escribir y repetir. | Siembra una vez con un valor explícito. |
| "Un bucle de reintentos con `srand` dentro muestrea de nuevo." | Dentro de un segundo vuelve a muestrear los mismos valores. | Siembra una vez, fuera del bucle. |
| "Las pruebas inestables son un problema de la prueba." | Normalmente el programa no tiene semilla o comparte estado. | Arregla la entrada, no la aserción. |
| "Las pruebas necesitan un framework." | Un script con código de salida es una prueba. | Usa el arnés más pequeño que pueda fallar. |
| "Capturar la salida a mano es una traza." | Una traza necesita un comando, un entorno y una comparación. | Registra los tres y compara bytes. |
| "Una ejecución verde significa que el juego es correcto." | El banco fija el final. | Di qué cubre la traza. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Determinismo | Las mismas entradas producen la misma salida. |
| Semilla | El valor explícito que inicia un flujo pseudoaleatorio. |
| Entropía | Un valor tomado del entorno, como el reloj. |
| Generador pseudoaleatorio | Un algoritmo determinista que parece aleatorio. |
| LCG | Generador congruencial lineal: `state = state * a + c`. |
| `RAND_MAX` | El mayor valor que `rand()` puede devolver en este runtime. |
| Definido por la implementación | El estándar permite que cada implementación elija. |
| Traza de referencia | Un archivo versionado con la salida esperada. |
| Oráculo | La fuente de verdad contra la que compara una prueba. |
| Arnés | El comando que ejecuta el programa y compara la salida. |
| Código de salida | El número que devuelve un programa; distinto de cero es fallo. |
| Regresión | Un comportamiento que funcionaba y dejó de hacerlo. |
| Nota de plataforma | El registro de entorno que hace interpretable una traza. |
| Sesgo de módulo | Probabilidades desiguales causadas por `% bound`. |
| Límite de evidencia | La línea entre lo que demuestra la ejecución y lo que infieres. |

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
