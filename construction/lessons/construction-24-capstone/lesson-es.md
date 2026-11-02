# Proyecto final: informe de costes (cost report)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** justificación de compromisos · **Tiempo:** unos 175 minutos ·
**Prerrequisitos:** construction-12, construction-15 · **Estado:** borrador
**Lectura:** ~21 minutos

## Por qué existe esta lección

Track A termina aquí. No hay una línea base nueva ni sintaxis nueva. Este directorio no tiene
`code/` a propósito: las dos copias que hagas son las líneas base. Tomas dos escalones que ya
construiste, aplicas el mismo cambio a ambos, mides lo que cuesta y escribes una página que defiende
un veredicto con números.

El cambio está fijado:

- añadir un tercer tipo de pieza;
- añadir un tablero de 10×10;
- conservar el contrato de transcripción;
- conservar las 20 trazas de referencia pasando en el camino de 8×8 sin cambios.

La frase que hay que retener:

> Un diseño no es barato ni caro en abstracto. Es barato o caro para un cambio concreto, y el coste
> es el conjunto de archivos, líneas, conceptos y pruebas que ese cambio toca.

Tres notas honestas antes de empezar.

1. El artefacto es un informe, no un programa. Las dos copias de trabajo son desechables. El informe
   de una página y el registro de evidencias son lo que conservas. Si las copias compilan pero el
   informe no tiene predicción, el proyecto final no está terminado.
2. La comparación es tan justa como el cambio. Ambas copias reciben la misma tercera pieza, el mismo
   punto de entrada de 10×10, la misma aceptación y el mismo compilador. La ruta puede diferir,
   porque los escalones fuerzan rutas distintas; cuando difiera, la ruta es parte del coste y la
   registras.
3. Las 20 trazas son un control, no una prueba. Cubren solo el juego de 8×8. No hay traza de
   referencia para 10×10 ni para la tercera pieza. El informe debe decir qué está demostrado y qué
   solo se ha probado a humo.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar el coste como propiedad de un cambio nombrado;
- defender una decisión de diseño con números medidos y un informe de una página.

## El concepto: el coste es una propiedad del próximo cambio

Cada lección anterior comparó dos diseños con un cambio. Este proyecto final hace lo mismo, pero no
te dice cuál es "el" cambio. Fija uno y te pide aplicarlo dos veces.

La unidad de medida es una **prueba de cambio**: un requisito pequeño aplicado a dos variantes. La
salida no es un ganador. La salida es una factura. La comprobación que ejecutas es
**predecir → medir → comparar → explicar → registrar**.

| Métrica | Qué mide | Por qué está en la tabla |
|---|---|---|
| Archivos tocados | Acoplamiento | Un cambio que llega al archivo de construcción está más acoplado que uno que no. |
| Líneas añadidas y eliminadas | Radio de impacto | El tamaño no es calidad, pero predice el tiempo de revisión. |
| Conceptos nuevos necesarios | Coste de aprendizaje | Una ruta que necesita una idea nueva cuesta más que una que reutiliza una vieja. |
| Pruebas rotas | Red de seguridad | Un cambio que rompe el contrato por el camino es más arriesgado que uno que no. |
| Tiempo de compilación | Bucle de retroalimentación | Una construcción más lenta es un aprendizaje más lento. |
| Bytes de texto del binario | Crecimiento del código | Es el único número de tamaño que el compilador te da gratis. |
| Trazas de referencia superadas | Contrato | El control que dice que el juego de 8×8 no se movió. |

Dos de estas señales son débiles y debes decirlo. El tiempo de compilación en un programa de 600
líneas está dominado por encabezados y arranque de proceso, no por tu diseño. El texto del binario
se mueve con la inserción de código y con el optimizador. Están en la tabla porque la plantilla los
pide, no porque decidan el veredicto. El veredicto lo deciden los números que puedas defender.

### Qué significa "más barato" aquí

Más barato significa: para este cambio, en esta máquina, con estas rutas, un escalón costó menos. No
significa que el escalón sea mejor en general. El próximo cambio puede invertir la factura, y la
última sección del informe te pide nombrar el cambio que la invertiría.

Esa es toda la disciplina de Track A: un diseño es una apuesta sobre qué cambio viene después. El
proyecto final es donde muestras tu apuesta y su precio.

### Por qué el par importa más que los escalones

Una comparación solo se puede leer si la mayoría de las variables se mantienen constantes. Los dos
pares recomendados se eligen por eso:

| Par | Qué mantiene aproximadamente constante | Qué varía | Qué aísla |
|---|---|---|---|
| 12 y 13 | El `Board` concreto de 8×8; la forma del bucle | El diseño de la pieza: interfaz frente a miembro concreto | El coste de una tercera pieza |
| 14 y 15 | El generador de movimientos; el tipo de pieza, todavía `Knight` | La forma del motor: funciones libres frente a plantillas de clase | El coste de un tablero de 10×10 |

Si comparas 12 y 15, cambian tres cosas a la vez: el diseño de la pieza, el del tablero y la forma
del motor. Todavía puedes escribir un informe, pero no puedes atribuir la diferencia a una causa. Si
lo haces igual, dilo en el veredicto y no afirmes más de lo que muestran los números.

## Antes de empezar

- [`construction-12`](../construction-12-polymorphism/lesson-es.md) — el escalón de la interfaz. Es
  el prerrequisito del catálogo y un lado del primer par recomendado.
- [`construction-15`](../construction-15-generic-classes/lesson-es.md) — el escalón genérico. Es el
  otro prerrequisito del catálogo. Su `Board<RowCount, ColumnCount>` y su `GameEngine<8, 8>` son la
  ruta de 10×10 más barata de la escalera.
- [`construction-13`](../construction-13-composition/lesson-es.md) y
  [`construction-14`](../construction-14-generic-functions/lesson-es.md) — los escalones
  intermedios. Los pares recomendados los usan. Si te saltaste uno, hazlo primero o acepta una
  comparación que cambia dos variables a la vez y dilo en el informe.
- [`construction-00`](../construction-00-read-the-rules/lesson-es.md) a
  [`construction-11`](../construction-11-encapsulation/lesson-es.md) — sabes leer una clase, un
  encabezado, un script de construcción y una traza.
- Las reglas: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §1 fija el tablero
  y su numeración desde 1, §4 fija el orden de movimientos del caballo, §8 fija cuándo una celda
  pasa a visitada.
- El formato de transcripción:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- Un compilador de C++20 (`g++` o `clang++`). `cmake` es opcional; cada `run.sh` recurre a una
  compilación directa cuando no hay CMake.

No necesitas saber cómo se convierte un bucle en plantilla ni cómo se dispone una tabla virtual.
Necesitas copiar un directorio, editarlo, construirlo, contar lo que cambiaste y escribir qué número
decidió tu respuesta.

### Qué necesitas

- Dos de estos cuatro escalones, cada uno con `code/`, `run.sh` y `CMakeLists.txt`:
  [`construction-12`](../construction-12-polymorphism/),
  [`construction-13`](../construction-13-composition/),
  [`construction-14`](../construction-14-generic-functions/),
  [`construction-15`](../construction-15-generic-classes/).
- El script de medición: [`measure.sh`](measure.sh).
- La plantilla del informe: [`REPORT-TEMPLATE-es.md`](REPORT-TEMPLATE-es.md).
- Este registro: [`EVIDENCE-es.md`](EVIDENCE-es.md).
- Las reglas y el formato de transcripción, citados arriba.
- Una traza de referencia, para el primer diff:
  [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- Espacio de trabajo fuera del repositorio. Esta lección usa `/tmp/opencode/capstone`; cualquier
  ruta escribible sirve, pero no crees las copias dentro del directorio de la lección.

Comprueba que los artefactos existen antes de empezar, desde este directorio de lección:

```bash
# common artifacts
test -f ../../../common/traces/seed-0001.txt &&
test -f measure.sh &&
test -f REPORT-TEMPLATE-es.md

# the two rungs you chose, for example 12 and 13
test -f ../construction-12-polymorphism/code/piece.h &&
test -f ../construction-13-composition/code/knight.cpp
```

Si falta un escalón, elige otro par. Un informe de costes contra un escalón que no has leído no es
evidencia. Registra la sustitución en `EVIDENCE-es.md` §11.

## El cambio, definido una vez

Ambas copias reciben el mismo cambio. Escribe estas definiciones en `EVIDENCE-es.md` §2.1 antes de
editar nada, y úsalas como aceptación para las dos.

### La tercera pieza

Un tipo nuevo que:

- responde a las mismas cuatro operaciones que el bucle del escalón ya pregunta: `row()`,
  `column()`, `set_position(row, column)` y `role_name()`;
- se puede construir a partir de un nombre de rol;
- se mueve con los ocho desplazamientos del caballo, para que las trazas sigan siendo un control
  válido;
- viaja por el mismo bucle de turnos, no por una copia del bucle;
- deja la transcripción de 8×8 idéntica byte a byte.

La prueba más barata de que viaja por el bucle es la sustitución: construye el tipo nuevo donde el
escalón construye sus dos piezas, reconstruye y compara las 20 trazas. Como las respuestas son
idénticas, la transcripción es idéntica, y las 20 trazas demuestran que el bucle aceptó un tipo
distinto. Conserva los nombres de rol `player` y `machine`: el nombre de rol es parte de la
transcripción, así que renombrarlo cambia el control. Puedes mantener la sustitución o restaurar el
original; di cuál hiciste.

Una segunda prueba es un punto de entrada temporal que ejecuta el bucle con el tipo nuevo. Cuesta
más líneas y demuestra lo mismo. Si la eliges, cuéntala.

No le des a la tercera pieza una regla de movimiento distinta en este proyecto final. Eso es otro
cambio, y cambia la interfaz, no solo el tipo. `construction-12` §5.5 lo mide; el proyecto final lo
menciona en el veredicto como el próximo cambio, no como este.

### El tablero de 10×10

Un segundo punto de entrada en la copia de trabajo que:

- ejecuta el mismo bucle de turnos sobre un área activa de 10×10;
- acepta los mismos argumentos `--seed` y `--name`, o llama a la misma función con ellos;
- imprime el mismo formato de transcripción;
- no cambia la salida del punto de entrada por defecto de 8×8.

Aceptación del ejecutable de 10×10: arranca, imprime `initial_player` e `initial_machine` con ambas
coordenadas en 1..10, y termina con `winner=` y `moves=`. Es una prueba de humo, no una traza de
referencia. No hay contrato de 10×10 en `common/traces/`.

Tres rutas honestas hacia 10×10, cada una con su precio:

| Ruta | Qué cambia | Cuándo es la barata |
|---|---|---|
| Dimensiones en tiempo de ejecución | `Board` recibe filas y columnas; `run_game` las pasa | El tablero es una clase concreta (12, 13, 14) |
| Dimensiones como plantilla | `Board<Rows, Columns>` y un motor sobre ellas | El escalón ya tiene plantillas de clase (15) |
| Un segundo tipo de tablero | Una copia del tablero con constantes de 10×10 | Nunca en este proyecto final; duplica las reglas |

La tercera ruta está en la lista para que puedas rechazarla con una razón: duplica la lógica de
límites, y el informe mediría una copia, no un diseño.

### Qué no debe moverse

- Las 20 trazas de referencia, byte a byte, en el camino por defecto de 8×8.
- El formato de transcripción: el mismo orden de líneas, las mismas claves, los mismos valores.
- Las reglas del juego: límites, captura, visitada, trampa, tablas.
- Los archivos versionados del repositorio. Toda edición ocurre en una copia de trabajo.

## Paso 1 — Elegir el par y copiar (~15 min)

### 1.1 Elegir el par

Usa 12 y 13 si quieres medir el precio de una tercera pieza. Usa 14 y 15 si quieres medir el precio
de un tablero de 10×10. No elijas el par que esperas que gane; elige el par cuya diferencia puedas
explicar.

Escribe una frase en `EVIDENCE-es.md` §2.1: "Elegí A y B porque mantienen constante ______ y varían
______". Si no puedes terminar la frase, elige el otro par.

### 1.2 Copiar el directorio completo de la lección

`measure.sh` llama a `run.sh` dentro de la copia, así que la copia necesita `code/`, `run.sh` y
`CMakeLists.txt`. Copia el directorio, no solo `code/`:

```bash
mkdir -p /tmp/opencode/capstone
rm -rf /tmp/opencode/capstone/rung-a /tmp/opencode/capstone/rung-b
cp -r ../construction-12-polymorphism /tmp/opencode/capstone/rung-a
cp -r ../construction-13-composition /tmp/opencode/capstone/rung-b
rm -rf /tmp/opencode/capstone/rung-a/build /tmp/opencode/capstone/rung-b/build
```

La última línea importa. Un `build/` copiado contiene un binario cuya fecha es más reciente que la
de los fuentes, así que `run.sh` no reconstruirá tras tu primera edición y medirás el programa
viejo.

Registra el commit del que copiaste, o la fecha si este checkout todavía no tiene commit, para que
los dos lados sean la misma línea base:

```bash
git rev-parse HEAD 2>/dev/null || date -u +%Y-%m-%dT%H:%M:%SZ
```

### 1.3 Medir ambas líneas base antes de tocar nada

```bash
./measure.sh /tmp/opencode/capstone/rung-a
./measure.sh /tmp/opencode/capstone/rung-b
```

La salida en este repositorio, para los cuatro escalones candidatos:

```text
construction-12  files=10 lines=540 traces=20/20
construction-13  files=11 lines=556 traces=20/20
construction-14  files=12 lines=587 traces=20/20
construction-15  files=12 lines=598 traces=20/20
```

Registra tus dos líneas en `EVIDENCE-es.md` §2.2. Estos son los números "antes". `measure.sh` cuenta
todos los `.h` y `.cpp` directamente bajo `code/`, así que un punto de entrada o una sonda nuevos
bajo `code/` aumentarán el recuento. Por eso el informe distingue el tamaño de la copia del delta
que causaste.

## Paso 2 — Predecir antes de tocar código (~20 min)

Este es el paso sobre el que trata realmente el proyecto final. Escribe la predicción en
`EVIDENCE-es.md` §3 y no la edites después. Si la observación no coincide, la brecha es el
resultado.

### 2.1 La predicción de coste

| Métrica | Escalón A | Escalón B | Confianza |
|---|---:|---:|---|
| Archivos tocados | | | |
| Líneas añadidas | | | |
| Líneas eliminadas | | | |
| Conceptos nuevos necesarios | | | |
| Trazas rotas en la primera construcción verde | | | |
| Tiempo de compilación, mediana de 3 (segundos) | | | |
| Bytes de texto del binario | | | |
| Trazas de referencia superadas al final | | | |

La confianza es `alta`, `media` o `baja`. Una fila que marcas `baja` y fallas vale más que una que
marcas `alta` y aciertas por suerte.

### 2.2 La predicción de ruta

| Pregunta | Mi predicción |
|---|---|
| Ruta de la tercera pieza en A | |
| Ruta de la tercera pieza en B | |
| Ruta de 10×10 en A | |
| Ruta de 10×10 en B | |
| Archivos que espero tocar en A | |
| Archivos que espero tocar en B | |
| Qué mitad domina en A: ¿pieza o tablero? | |
| Qué mitad domina en B: ¿pieza o tablero? | |

La ruta es parte del coste. Si A necesita una unidad de traducción nueva y B no, eso es un archivo y
una edición del archivo de construcción, y pertenece a la tabla.

### 2.3 La predicción del veredicto

| Pregunta | Mi predicción |
|---|---|
| Qué escalón espero que sea más barato | |
| El número que espero que lo decida | |
| El número que podría invertirlo | |
| El coste que espero que la tabla no muestre | |
| La predicción de la que estoy menos seguro | |

No escribas "depende". Escribe el número que esperas y la condición en la que se invierte.

## Paso 3 — Aplicar el cambio al escalón A (~40 min)

Trabaja en `/tmp/opencode/capstone/rung-a`. El orden de abajo es una sugerencia, no una regla; lo
que importa es que registres los puntos de control.

### 3.1 Primero la tercera pieza

Es la unidad más pequeña y te dice si el bucle está abierto. La ruta depende del escalón:

- **construction-12:** el bucle guarda `Piece *`. Añade el tipo nuevo en su propio encabezado y
  fuente, añade la fuente a `CMakeLists.txt` y a la lista de respaldo de `run.sh`, y sustitúyelo en
  las dos construcciones de `run_game`. Ese es todo el cambio; el bucle no se toca.
- **construction-13:** el bucle nombra `Knight` en diez firmas de ayudantes. Debes convertir esos
  ayudantes en plantillas sobre el tipo de pieza, o duplicar el bucle. Registra qué ruta tomaste. La
  primera reutiliza la idea de `construction-14`; la segunda duplica la lógica de turnos.
- **construction-14:** el generador ya acepta cualquier tipo con `row()` y `column()`, pero el bucle
  sigue nombrando `Knight`. Aplican las mismas dos rutas que en `construction-13`.
- **construction-15:** `GameEngine` declara dos miembros `Knight` y seis firmas `Knight`. Añade un
  parámetro de tipo de pieza restringido por `PositionedPiece`, o duplica el motor. El concepto ya
  está; usarlo como restricción es el trabajo.

Punto de control A1: el tipo nuevo compila y responde a las cuatro operaciones. Una sonda basta:

```cpp
// /tmp/opencode/capstone/probe.cpp — scratch only
#include "piece.h"          // or knight.h, depending on the rung
#include "spare_knight_piece.h"

#include <cstdio>

int main()
{
    SpareKnightPiece piece("player");
    piece.set_position(3, 4);
    std::printf("role=%s at=%d,%d\n", piece.role_name(), piece.row(), piece.column());
    return 0;
}
```

Punto de control A2: las 20 trazas siguen pasando. Si fallan, detente y lee el primer diff. Una
tercera pieza con las mismas respuestas no puede cambiar la transcripción; un fallo significa que el
camino por defecto cambió.

### 3.2 El punto de entrada de 10×10

Añade un segundo punto de entrada bajo `code/`, por ejemplo `main_ten.cpp`, que llame al mismo bucle
con dimensiones de 10×10. Deja intacto el punto de entrada por defecto.

Punto de control A3: el ejecutable de 10×10 imprime la cabecera y termina con `winner=` y `moves=`.
Comprueba que las coordenadas iniciales están en 1..10.

Punto de control A4: las 20 trazas siguen pasando. El punto de entrada de 10×10 tiene un segundo
`main`, así que no se puede enlazar con el binario por defecto; el objetivo de construcción debe
listar solo las fuentes por defecto.

### 3.3 Tres trampas que distorsionan la medición

1. **El archivo de construcción es un archivo.** `CMakeLists.txt` y la lista de fuentes de respaldo
   en `run.sh` fijan las unidades de traducción. Un `.cpp` nuevo es invisible hasta que edites
   ambos. Cuenta los dos como archivos tocados. No es papeleo; es el acoplamiento que mide la tabla.
2. **`run.sh` en 12 y 13 no comprueba encabezados.** Tras editar `board.h` o `piece.h`, borra
   `build/` o toca un `.cpp`, o ejecutarás el binario viejo. `run.sh` en 14 y 15 sí comprueba
   encabezados.
3. **Nunca uses `code/*.cpp` para la construcción por defecto.** El punto de entrada de 10×10 tiene
   su propio `main`, así que el comodín produce un símbolo duplicado al enlazar. Lista las fuentes
   explícitamente.

### 3.4 Registrar mientras avanzas

Mantén un registro de puntos de control en `EVIDENCE-es.md` §4.1:

| Punto de control | Comando | Resultado | Archivos tocados hasta ahora | Trazas |
|---|---|---|---|---|
| Línea base | `./measure.sh` | | 0 | 20/20 |
| A1 la tercera pieza compila | | | | |
| A2 trazas tras la pieza | | | | |
| A3 10×10 ejecuta | | | | |
| A4 trazas tras el tablero | | | | |

El registro es cómo respondes después "tiempo hasta la primera construcción verde". La tabla del
informe no tiene una fila para eso, pero §5 pide el coste que la tabla no muestra.

## Paso 4 — Aplicar el mismo cambio al escalón B (~40 min)

Mismas definiciones, misma aceptación, mismo orden si encaja. La regla es: no mejores B mientras
estás ahí. Un renombrado, una limpieza o una prueba extra que A no recibió hacen ilegible la
comparación.

Los puntos de control B1 a B4 reflejan A1 a A4. Regístralos en `EVIDENCE-es.md` §4.2.

Si B no se puede llevar a compilar dentro de tu presupuesto de tiempo, eso es una medición, no un
fracaso. Registra el tiempo de reloj, el último error y la ruta que intentaste. No inventes números
y no elimines B del informe en silencio. "B no llegó a una construcción verde en 40 minutos; A lo
hizo en 12" es un resultado de coste legítimo.

## Paso 5 — Medir (~20 min)

Ejecuta cada comando desde el directorio del proyecto final. Las rutas de trabajo son variables para
que los comandos sean los mismos en ambos escalones.

### 5.1 Archivos y líneas

`measure.sh` da el tamaño de la copia. El delta viene de `git diff --no-index`:

```bash
original=../construction-12-polymorphism
scratch=/tmp/opencode/capstone/rung-a

git diff --no-index --numstat "$original/code" "$scratch/code" || true
git diff --no-index --numstat "$original/CMakeLists.txt" "$scratch/CMakeLists.txt" || true
git diff --no-index --numstat "$original/run.sh" "$scratch/run.sh" || true
```

Cada línea es `añadidas<TAB>eliminadas<TAB>ruta`. Suma las columnas:

```bash
{
  git diff --no-index --numstat "$original/code" "$scratch/code" || true
  git diff --no-index --numstat "$original/CMakeLists.txt" "$scratch/CMakeLists.txt" || true
  git diff --no-index --numstat "$original/run.sh" "$scratch/run.sh" || true
} | awk '{added += $1; removed += $2; files += 1}
       END {printf "files=%d added=%d removed=%d\n", files, added, removed}'
```

Cuenta un archivo una vez, aunque aparezca en dos diffs. `--numstat` informa líneas añadidas y
eliminadas incluyendo comentarios y líneas en blanco; dilo en el informe. `git diff --no-index` sale
con estado 1 cuando hay diferencias, por eso cada llamada lleva `|| true`.

### 5.2 Conceptos nuevos

Un concepto es nuevo si la lección propia del escalón no lo demostraba ya. Nombra cada uno y señala
la línea que lo usa. Ejemplos que puedes contar legítimamente:

- una dimensión en tiempo de ejecución: un parámetro de constructor y miembros que sustituyen a
  `static constexpr`;
- un segundo punto de entrada y un segundo `main`;
- una unidad de traducción nueva añadida a la construcción;
- un parámetro de plantilla en el bucle de turnos;
- un concepto usado como restricción en vez de como `static_assert`;
- un bucle duplicado, si elegiste esa ruta.

No cuentes "leer el código", "usar el compilador" ni "añadir un archivo" dos veces. El número es un
juicio; la lista es la evidencia. Un recuento sin lista no se acepta.

### 5.3 Pruebas rotas y trazas superadas

Son dos números, no uno.

- **Pruebas rotas:** cuántas de las 20 trazas fallan en la primera construcción que compila después
  de tu cambio. Es el momento en que el contrato estuvo en riesgo.
- **Trazas de referencia superadas:** cuántas pasan al final. La aceptación es 20.

Cuenta fallos sin abortar en el primero:

```bash
passed=0
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  if "$scratch/run.sh" --seed "${seedNumber}" --name Learner | cmp -s - "${trace}"; then
    passed=$((passed + 1))
  else
    printf 'failed seed %s\n' "${seedNumber}"
  fi
done
printf 'passed=%d/20\n' "${passed}"
```

Después ejecuta el script para el registro final:

```bash
./measure.sh "$scratch"
```

Si una traza falló, lee la primera línea distinta con `diff -u` y escribe qué te dijo. Un fallo en
`initial_player` significa que las dimensiones del tablero llegaron al camino por defecto. Un fallo
en `who=` o `move=` significa que la tercera pieza cambió los roles o el orden. Un fallo en
`winner=` o `moves=` significa que se movió una regla.

### 5.4 Tiempo de compilación

Usa el mismo compilador, los mismos indicadores y la misma lista de fuentes en ambos escalones.
Construye el programa por defecto, no el punto de entrada de 10×10:

```bash
cd "$scratch"
sources="code/main.cpp code/game.cpp code/board.cpp code/knight_piece.cpp \
code/spare_knight_piece.cpp code/random_sequence.cpp"
rm -rf build
for run in 1 2 3; do
  /usr/bin/time -f "run=${run} seconds=%e" g++ -std=c++20 -Wall -Wextra -Werror -O2 \
    -Icode -o "/tmp/opencode/capstone/default-${run}" ${sources}
done
```

Registra la mediana de las tres, no la mejor. Registra `g++ --version` una vez. Si un escalón usa
CMake y el otro no, la comparación incluye el paso de configuración y debes decirlo.

La lista de fuentes de arriba es un ejemplo para `construction-12`. Sustitúyela por la lista de tu
escalón y mantenla idéntica en las tres ejecuciones. La variable se expande sin comillas a
propósito: el shell debe dividirla en argumentos de archivo separados. Si `/usr/bin/time` no es GNU
time en tu plataforma, usa la palabra clave `time` del shell y toma la línea `real`.

### 5.5 Bytes de texto del binario

```bash
size /tmp/opencode/capstone/default-1 | awk 'NR==2 {print "text=" $1 " data=" $2}'
```

El texto es código y datos de solo lectura; los datos son datos inicializados escribibles. Informa
del texto, como pide la plantilla. Si el número se mueve en dirección contraria al código fuente,
di por qué: la inserción de código, una tabla virtual o una instanciación de plantilla pueden
hacerlo.

### 5.6 Rellenar la comparación

Pon los números en `EVIDENCE-es.md` §6 y en `REPORT-TEMPLATE-es.md` §3. No copies la predicción en
la columna de observación. La brecha es el punto.

| Métrica | Escalón A | Escalón B |
|---|---:|---:|
| Archivos tocados | | |
| Líneas añadidas | | |
| Líneas eliminadas | | |
| Conceptos nuevos necesarios | | |
| Pruebas rotas | | |
| Tiempo de compilación (segundos) | | |
| Bytes de texto del binario | | |
| Trazas de referencia superadas | | |

## Paso 6 — Escribir el informe de una página (~20 min)

Abre [`REPORT-TEMPLATE-es.md`](REPORT-TEMPLATE-es.md) y rellena sus seis secciones. El límite de una
página es real: la salida cruda de los comandos va en `EVIDENCE-es.md`, no en el informe.

### 6.1 La regla del veredicto

El veredicto es una frase con tres partes:

1. qué escalón fue más barato **para este cambio**;
2. qué número medido lo decidió;
3. qué número estuvo más cerca de invertirlo.

Forma de ejemplo, no respuesta: "Para la tercera pieza y el tablero de 10×10, el escalón A fue más
barato; el número que lo decidió fue archivos tocados, y el tiempo de compilación fue lo que estuvo
más cerca de invertirlo".

Si tu frase no tiene un número, es una opinión. Si no tiene alcance, es un eslogan.

### 6.2 El coste que la tabla no muestra

Responde esto en §5 del informe. Candidatos, en orden de utilidad:

- tiempo hasta la primera construcción verde, según tu registro de puntos de control;
- número de intentos de construcción antes de la primera verde;
- cambios de ruta: ¿empezaste con una ruta y la abandonaste?
- carga conceptual: ¿qué idea nueva tardaste más en entender?
- riesgo: ¿qué tendría que volver a comprobar a mano una persona revisora?
- qué no mediste: tiempo de ejecución, memoria, tiempo de revisión, mantenimiento.

Elige el que realmente cambió tu veredicto o tu confianza. Una lista de seis no es una respuesta.

### 6.3 El próximo cambio

Nombra un cambio que harías después y predice su coste en la misma tabla. Es la prueba hacia
adelante: si no puedes predecirlo, no has entendido el acoplamiento que acabas de medir.

Buenos candidatos:

- una tercera pieza con una regla de movimiento distinta;
- un tamaño de tablero elegido en tiempo de ejecución desde `argv`;
- un segundo tipo de pieza en el bucle sin plantilla;
- un cambio en el formato de transcripción.

### 6.4 Calibración

Compara la predicción con la observación, fila por fila. Nombra la fila con la brecha más grande y
explícala mecánicamente, no moralmente. "Predije dos archivos y toqué nueve porque el archivo de
construcción lista cada fuente" es calibración. "Subestimé la complejidad" no lo es.

## Paso 7 — Defender el veredicto (~15 min)

Una persona revisora que no ha visto tus copias hace tres preguntas. Integra las respuestas en §5
del informe: la justicia en la viñeta del número decisivo, la inversión en la del próximo cambio, y
los puntos ciegos en la del coste que la tabla no muestra. Una frase cada una.

1. **¿Por qué la comparación es justa?** Mismo cambio, misma aceptación, mismo compilador e
   indicadores, misma máquina, mismas trazas. Si las rutas difieren, di cómo y por qué la diferencia
   sigue siendo atribuible.
2. **¿Qué invertiría el veredicto?** Nombra un cambio concreto y la métrica que movería. "Si el
   próximo cambio fuera una regla de movimiento distinta, el escalón de la interfaz pagaría una
   operación nueva en la interfaz y el genérico pagaría una restricción nueva".
3. **¿Qué no mediste?** Tiempo de ejecución, memoria, tiempo de revisión, coste de equipo. Di cuál
   importa para tu próximo cambio.

Después escribe la explicación de una frase sin las palabras *mejor*, *peor*, *más limpio*, *más
flexible*, *más reutilizable*, *más elegante*, *moderno* ni *correcto*. Las palabras de mecanismo sí
se permiten aquí, porque el tema es un diseño; las de veredicto no, porque esconden el número.

## Paso 8 — Comprobación de aceptación y registro (~5 min)

El criterio de aceptación es: **un informe de una página con predicción, números medidos para dos
escalones, veredicto y próximo cambio; ambas copias pasan las 20 trazas en el camino de 8×8 sin
cambios.**

- [ ] Elegí un par y puedo terminar la frase "mantienen constante ______ y varían ______".
- [ ] Copié dos escalones a directorios de trabajo fuera del repositorio y borré `build/`.
- [ ] Registré el commit y las dos salidas de `measure.sh` antes de editar.
- [ ] Escribí la predicción, con confianza, antes de la primera edición.
- [ ] Apliqué la misma tercera pieza y el mismo punto de entrada de 10×10 a ambas copias.
- [ ] La tercera pieza viaja por el mismo bucle de turnos, y dije cómo lo demostré.
- [ ] El punto de entrada de 10×10 ejecuta e imprime el formato de transcripción.
- [ ] Ambas copias pasan 20/20 en el camino de 8×8.
- [ ] Medí archivos y líneas con `--numstat`, incluidos los archivos de construcción.
- [ ] Medí el tiempo de compilación como mediana de tres construcciones limpias con los mismos
      indicadores.
- [ ] Registré los bytes de texto del binario del programa por defecto.
- [ ] Registré las pruebas rotas en la primera construcción verde por separado del 20/20 final.
- [ ] El informe tiene sus seis secciones y cabe en una página.
- [ ] El veredicto nombra el número que decidió y el que casi lo invierte.
- [ ] El próximo cambio está nombrado y predicho en la misma tabla.
- [ ] No edité ningún archivo versionado.

Estos son criterios de aceptación. Si una casilla queda sin
marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §10.

## Autocomprobación (binaria)

- [ ] Puedo decir cuánto costó el cambio en cada escalón sin abrir el informe.
- [ ] Puedo nombrar la métrica que decidió el veredicto y la que casi lo invirtió.
- [ ] Puedo explicar por qué el par que elegí aísla la variable que afirmé.
- [ ] Puedo decir qué demuestran las 20 trazas y qué no.
- [ ] Puedo nombrar el próximo cambio y predecir su coste.
- [ ] La predicción y la observación están en `EVIDENCE-es.md`, sin editar.
- [ ] El informe cabe en una página y una persona ajena podría comprobar cada número desde §5.
- [ ] Completé `EVIDENCE-es.md` §1–§12.

## Misconcepciones que esta lección refuta

| Error de concepto | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| "El escalón más barato es el mejor diseño." | Barato está acotado a un cambio y una máquina. | Di "más barato para este cambio" y nombra el próximo. |
| "El coste son las líneas de código." | Es una de siete métricas, y la más débil para el acoplamiento. | Cuenta también archivos y conceptos. |
| "20 de 20 demuestra que el cambio es correcto." | Las trazas cubren solo el juego de 8×8. | Llama pruebas de humo a 10×10 y a la tercera pieza. |
| "El tablero de 10×10 son dos constantes." | Cambiar las constantes mueve el camino por defecto y rompe las 20 trazas. | Conserva el camino de 8×8 y añade un segundo punto de entrada. |
| "La tercera pieza es gratis en el escalón de la interfaz." | Cuesta dos archivos, una edición del archivo de construcción y una tabla virtual. | Mide archivos y bytes de texto. |
| "El tiempo de compilación mide la calidad del diseño." | En 600 líneas está dominado por encabezados y arranque. | Infórmalo, pero no dejes que decida solo. |
| "Si ambos pasan, el cambio costó lo mismo." | Archivos, líneas y conceptos pueden diferir en un orden de magnitud. | Rellena todas las filas de la tabla. |
| "Puedo comparar 12 y 15 directamente." | Cambian tres variables a la vez. | Usa 12/13 o 14/15, o acota la afirmación. |
| "Una ruta con plantillas siempre es más corta." | Puede añadir un encabezado, una instanciación y un concepto. | Cuenta conceptos, no solo líneas. |
| "Falló una traza, entonces el diseño está mal." | Falló porque el camino por defecto se movió. | Es una señal de coste; restaura el control. |
| "La predicción es un trámite." | La calibración es la mitad del entregable. | Conserva la predicción original e informa la brecha. |
| "Debería limpiar el escalón ya que estoy." | Las ediciones extra hacen incomparables los dos lados. | Aplica solo el mismo cambio. |
| "Un tablero duplicado es una ruta barata a 10×10." | Duplica las reglas de límites y de visitadas. | Cambia la dimensión, no las reglas. |
| "El informe puede ocupar dos páginas si la tabla es larga." | La restricción es el argumento, no el papel. | Mueve la salida cruda a `EVIDENCE-es.md`. |

## Una ruta verificada, solo para calibrar

Lee esto después de haber escrito tu predicción. Es una ruta para `construction-12`, medida en este
repositorio con `g++`, `-O2`, mediana de tres construcciones. Es un suelo, no la respuesta esperada;
una ruta con plantillas u otra tercera pieza aterrizará en otro sitio.

```text
archivos tocados      9  (4 archivos de código editados, 3 nuevos, 2 de construcción)
líneas añadidas      77
líneas eliminadas     5
conceptos nuevos      2  (dimensión en tiempo de ejecución, segundo punto de entrada)
pruebas rotas         0 en la primera construcción verde
tiempo de compilación 1.03 s -> 1.28 s
texto del binario     9,583 -> 10,609 bytes
trazas de referencia  20/20 antes y después
```

La ruta: dar a `Board` un constructor que recibe filas y columnas, mantener 8×8 como valor por
defecto, añadir `SpareKnightPiece` como tercer tipo, sustituirlo en las dos construcciones y añadir
`main_ten.cpp` como segundo punto de entrada. La ejecución de 10×10 empieza en `2,3` y `2,10` y
termina con `winner=machine` y `moves=27` para la semilla 1, que es el mismo juego de 10×10 que
documenta `construction-15`.

Los números no son la lección. La brecha entre tu predicción y tu observación sí lo es.

## Siguiente paso

No hay `construction-25`. Track A termina con este informe.

Lo que viene después depende de tu veredicto:

- si el tablero fue la mitad cara, el próximo cambio es una pieza con una regla de movimiento
  distinta, y la tabla de próximo cambio del informe es donde lo predices;
- si la pieza fue la mitad cara, el próximo cambio es un tamaño de tablero elegido en tiempo de
  ejecución, y aplica la misma tabla;
- si las dos mitades discreparon, has encontrado el compromiso que el proyecto final existe para
  mostrar: el escalón barato para un cambio es caro para el otro.

Después del proyecto final, las lentes son `construction-16` (propiedad), `construction-18`
(funcional) y `construction-20` (hexagonal). Track B, arqueología, empieza en `archaeology-00` en el
catálogo.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog.md) para la ruta completa.

Conserva las dos copias de trabajo y el informe. El informe es el artefacto; las copias son la
evidencia.
