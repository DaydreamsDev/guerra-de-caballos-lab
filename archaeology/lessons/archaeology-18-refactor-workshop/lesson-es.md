# archaeology-18 — Taller de refactorización

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** todas las clases de fallo · **Prerrequisitos:**
> archaeology-04, archaeology-05, archaeology-08, archaeology-09 · **Tiempo estimado:** 180 minutos
> **Lectura:** ~17 minutos

## Por qué existe esta lección

Todas las lecciones de Track B terminan igual: un arreglo estructural y una línea que dice "Enlace a
Track A: `construction-08`" o "`construction-09`". Esa línea es una promesa. Nadie la ha pagado aún.

Un diagnóstico es barato. Una refactorización propuesta es una frase. Una refactorización ejecutada
es una diferencia, una compilación, una traza que coincide o no coincide, y una factura. Este taller
es el único lugar de Track B donde debes entregar la factura.

La habilidad que construye es la que separa a un revisor de un consultor: **puedes mover código que
funciona de una forma mala a una forma buena sin romperlo, y puedes decir cuánto costó el
movimiento.** Ese es todo el trabajo. Todo lo demás en este laboratorio es preparación para eso.

El taller es deliberadamente abierto. Tú eliges el fallo y tú eliges el escalón, porque elegir es
parte de la competencia. La lección fija solo cuatro cosas: el método, la evidencia, el criterio de
aceptación y las reglas de honestidad.

## Qué podrás hacer

Al terminar este taller puedes:

- convertir un defecto en un banco ejecutable cuya salida es función de sus entradas;
- registrar una traza de línea base que sobrevive a un cambio, y demostrarlo byte a byte;
- elegir un escalón objetivo y decir qué clase de fallo elimina, no qué sintaxis añade;
- planificar una refactorización como una lista de cambios y una lista de lo que no debe cambiar;
- refactorizar en pasos que compilan y que mantienen la traza idéntica;
- separar una refactorización que preserva comportamiento de un arreglo que lo cambia, y mostrar
  ambas diferencias;
- medir el coste de un movimiento de diseño en archivos, líneas, nombres públicos y pruebas;
- enunciar qué prueba un conjunto de trazas que pasa y qué no prueba;
- mantener el archivo de solo lectura mientras trabajas.

El taller termina cuando el informe tiene números
medidos y la traza no cambió.

## El concepto: una refactorización es una obligación de prueba

### Tres afirmaciones antes de la primera edición

Una refactorización no es "limpiar". Es una afirmación, y necesita tres enunciados antes de la
primera edición:

| Afirmación | La pretensión | Cómo se refuta |
|---|---|---|
| Línea base | Esto es lo que el programa hace ahora, de forma reproducible. | Una segunda ejecución difiere. |
| Objetivo | Este escalón elimina esta clase de fallo. | La prueba de cambio del escalón sigue necesitando la edición vieja. |
| Invariante | Estos comportamientos observables no deben cambiar. | La traza difiere en un byte. |

Si no puedes escribir la primera afirmación, no tienes nada que preservar. Si no puedes escribir la
segunda, estás renombrando. Si no puedes escribir la tercera, estás reescribiendo y llamándolo
refactorización.

### El problema de la línea base

Aquí está la trampa que atrapa a casi todos en el primer intento: **el archivo no es una línea base
utilizable.**

Tres hechos, todos de lecciones anteriores:

- No compila hoy. `#include<conio.h>`, `#include<iostream.h>` y `void main()` son artefactos de
  Microsoft Visual C++ 6.0 (`archaeology-16`, failure-002).
- No es determinista. `srand(time(NULL))` está dentro del bucle de reintentos de colocación
  (`variant-01:76`, failure-003), así que la colocación cambia entre ejecuciones.
- Espera una tecla. `getch()` aparece cinco veces (`variant-01:781, 806, 835, 861`), así que su
  salida no es función de sus entradas.

Un programa que no puedes ejecutar dos veces no puede tener línea base. Entonces la línea base no
es el archivo. La línea base es un **banco**: el programa compilable más pequeño que todavía
contiene el defecto y escribe una traza que depende solo de sus entradas.

Ese es el primer movimiento profesional del taller, y no es un tecnicismo:

> No puedes preservar un comportamiento que no puedes observar.

### El banco tiene tres partes

| Parte | Qué es | Qué se rompe si la omites |
|---|---|---|
| El código defectuoso | La función más pequeña del archivo que carga el fallo, transcrita | Refactorizas un programa que no tiene el defecto |
| Una entrada controlada | Una semilla, un nombre, una lista de jugadas guionizada, un reloj guionizado | La traza cambia por razones ajenas a tu edición |
| Una traza | Una línea por evento observable, en la salida estándar | No tienes invariante que comprobar |

Tres reglas mantienen honesto el banco:

1. **Transcribe, no repares.** Cualquier arreglo antes de la traza A destruye la línea base. Si
   arreglas el off-by-one mientras copias, ya no tienes el fallo que elegiste.
2. **Controla la entrada, no la borres.** Un banco que elimina el defecto no es una línea base. Si
   el defecto es `srand(time(NULL))`, no borres la llamada; reemplaza `time(NULL)` por un valor
   guionizado que avanza en cada llamada e imprime la semilla usada. El defecto sigue visible y la
   ejecución se vuelve reproducible.
3. **Ejecútalo dos veces antes de confiar en él.** Dos ejecuciones, misma entrada, `cmp`. Si
   difieren, la entrada todavía no está controlada.

La regla 2 es la que separa un banco de una demostración. Borrar el no determinismo estabiliza la
traza y destruye la evidencia. Controlarlo estabiliza la traza y conserva la evidencia.

### La bifurcación de corrección

Algunos fallos son defectos de corrección. Para esos, dos objetivos tiran en direcciones opuestas:

- una refactorización debe preservar el comportamiento;
- un arreglo debe cambiarlo.

No puedes hacer ambas cosas en un paso, y no deberías intentarlo. La secuencia profesional son tres
pasos, cada uno con su propia diferencia:

| Paso | Qué hace | Evidencia | Mensaje de commit |
|---|---|---|---|
| 1 | Lleva el defecto a un banco | Traza A | `test: record trace A` |
| 2 | Refactoriza al escalón, con el defecto incluido | Traza A sigue idéntica byte a byte | `refactor: ...` |
| 3 | Repara el defecto | Traza B; la diferencia A→B es la justificación | `fix: ...` |

El paso 2 es el criterio de aceptación del taller. El paso 3 es opcional, y es la parte donde ganas
el derecho a decir cuánto costó el defecto.

Casi todo el mundo fusiona 2 y 3. El resultado es una diferencia donde una línea movida y un
operador cambiado conviven, y nadie, ni tú el mes que viene, puede decir qué cambio eliminó el
defecto y qué
cambio movió el código. Mantenlos separados. La diferencia es el argumento.

### Dos pruebas, no una

Una refactorización debe dos pruebas independientes:

| Prueba | Pregunta | Falla cuando |
|---|---|---|
| Preservación del comportamiento | ¿El programa sigue haciendo exactamente lo mismo? | La traza difiere |
| Ganancia de capacidad | ¿Puedo ahora hacer barato el cambio que este escalón promete? | La prueba de cambio del escalón sigue tocando los mismos archivos |

La primera prueba es una comparación de trazas. La segunda es la prueba de cambio impresa en
[`lesson-catalog.md`](../../../docs/guide/lesson-catalog-es.md). Un movimiento que pasa la primera y
falla la segunda es un renombrado con archivos extra. Un movimiento que pasa la segunda y falla la
primera es una reescritura. El taller acepta solo la intersección.

## Antes de empezar

Lee esto completo:

- [`README-es.md`](README-es.md) — la ficha del taller y el criterio de aceptación.
- [`WORKSHOP-REPORT-es.md`](WORKSHOP-REPORT-es.md) — la entrega que vas a rellenar.
- [archaeology-04](../archaeology-04-correctness-failures/lesson-es.md) — fallos de corrección y las
  seis fichas de fallo.
- [archaeology-05](../archaeology-05-global-state/lesson-es.md) — estado global y acoplamiento
  oculto.
- [archaeology-08](../archaeology-08-module-boundary/lesson-es.md) — la frontera de módulo.
- [archaeology-09](../archaeology-09-abstract-data-type/lesson-es.md) — el tipo abstracto de datos.
- [`../../../docs/guide/archaeology-track-es.md`](../../../docs/guide/archaeology-track-es.md) — §2,
  el protocolo de autopsia; §9, el mapeo de fallos a escalones.
- [`../../../docs/guide/lesson-catalog-es.md`](../../../docs/guide/lesson-catalog-es.md) — las filas
  de Track A y sus pruebas de cambio.
- [`../../../docs/archive-es.md`](../../../docs/archive-es.md) — el archivo fijado, etiqueta y
  commit.

También necesitas:

- un clon local del archivo en la etiqueta `v1.0.0`;
- un compilador de C++20 (`c++`, `g++` o `clang++`);
- `git`, para que tu refactorización tenga una diferencia y tu tabla de costes tenga una fuente de
  verdad;
- opcionalmente `cmake` y un compilador con sanitizadores;
- un directorio de trabajo fuera del archivo y fuera de los archivos versionados de este
  repositorio.

> **Regla:** el archivo es de solo lectura. Copia lo que necesites a un directorio de trabajo; nunca
> edites `original/`.

## El camino de 180 minutos

| Min | Paso | Qué haces | Evidencia |
| ---: | --- | --- | --- |
| 0–45 | **Inspeccionar** | Eliges el fallo y el escalón; construyes el banco; registras la traza A; cuentas el acoplamiento | `trace-a.txt`, cuatro conteos |
| 45–75 | **Planificar** | Escribes la lista de cambios, la lista de lo intacto y la tabla de coste previsto | Informe §3 |
| 75–140 | **Refactorizar** | Llegas al escalón en pasos, cada uno compilando y cada uno con trazas iguales | Bitácora de pasos, `git log` |
| 140–165 | **Verificar** | Compilas con `-Werror`; trazas; determinismo; sanitizador | Informe §5 |
| 165–172 | **Explicar** | Rellenas la tabla de costes medidos y el veredicto | Informe §6–7 |
| 172–180 | **Registrar** | Rellenas `EVIDENCE-es.md`; confirmas que el archivo está limpio | Entrada de portafolio |

Si la refactorización tarda 120 minutos en lugar de 65, es un
hallazgo sobre tu plan, y pertenece al informe §4, "Qué se rompió".

### La comprobación de ingeniería en esta lección

Track B usa el mismo hábito que Track A, con el verbo más exigente del track:

| Paso | Dónde ocurre | Qué produce |
| --- | --- | --- |
| **Inspeccionar** | Paso 1 | Un banco, la traza A y cuatro conteos de acoplamiento |
| **Planificar** | Paso 2 | Una lista de cambios y una tabla de coste previsto |
| **Refactorizar** | Paso 3 | El escalón, un paso verde a la vez |
| **Verificar** | Paso 4 | Trazas, determinismo y advertencias |
| **Explicar** | Paso 5 | El coste medido y el veredicto |
| **Registrar** | Paso 6 | El informe y el registro de evidencias |

## Paso 1 — Inspeccionar (45 min)

### 1.1 Elige el fallo y el escalón

Elige un fallo de una lección prerrequisito y un escalón. Estos emparejamientos son puntos de
partida, no asignaciones; debes justificar el tuyo en una frase por cada uno.

| Fallo de | Qué reproduce el banco | Escalón | Qué compra el escalón |
| --- | --- | --- | --- |
| `archaeology-04`, failure-001 | `i <= 64` escribe un elemento más allá del arreglo | `construction-02` | El límite vive en un solo lugar, así que un segundo bucle no puede repetirlo |
| `archaeology-04`, failure-003 | `srand(time(NULL))` dentro del bucle de reintentos | `construction-08` | La semilla es una entrada de un módulo, no una llamada enterrada en el bucle |
| `archaeology-04`, failure-004 | `pvectab` se reinicia a `&vectab[0]` en cada iteración | `construction-09` | El tablero posee su propio reinicio; ningún llamador puede reiniciarlo mal |
| `archaeology-04`, failure-006 | `cin >> nombre.vectorNombre` sobre `char[250]` | `construction-11` | El nombre es un tipo con un límite que el compilador y la clase imponen |
| `archaeology-05`, failure-007 | Variables globales mutadas por muchas funciones | `construction-08` | El estado vive detrás de un módulo; el llamador no puede nombrarlo |
| `archaeology-08` | Una sola unidad de traducción, sin cabecera | `construction-09` | La representación sale de la compilación del llamador |
| `archaeology-09`, failure-010 | `struct stablero { int mvectab; }` | `construction-11` | La clase sostiene el invariante; el campo es privado |

Dos notas sobre la tabla:

- `construction-12` aplica cuando el banco contiene el despacho por tipo de pieza y los ocho cuerpos
  `case` copiados a mano. `construction-13` aplica cuando llegaste a un escalón que usa herencia y
  ahora quieres reemplazarla por delegación. Ambos son objetivos legítimos; ambos son más caros de
  lo que la tabla sugiere.
- El archivo no incluye una versión con clases (`variant-05` está vacía por diseño), así que
  `construction-11`, `construction-12` y `construction-13` son extensiones curriculares, no
  artefactos históricos. Dilo en tu informe. Es la misma nota de alcance que
  [`archaeology-track-es.md`](../../../docs/guide/archaeology-track-es.md) §3.

Escribe dos frases en el informe §1 y §2:

> "Elegí failure-NNN porque es el defecto que mi banco puede reproducir en menos de 50 líneas."
> "Elegí `construction-NN` porque su prueba de cambio es exactamente la edición que el código
> heredado hace cara."

Si alguna frase es vaga, elige otra vez. Un emparejamiento vago produce un taller vago.

### 1.2 Construye el banco

El archivo no va a compilar. No pelees con él. Transcribe la región más pequeña que carga el
defecto.

```bash
mkdir -p /tmp/gdc-workshop/bench
cd /tmp/gdc-workshop/bench
git init
```

Abre el archivo en la línea que cita tu ficha de fallo. Para `variant-01`:

```bash
sed -n '55,90p' \
  <archive>/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

Esa ventana contiene el bucle de inicialización, la siembra y el reintento de colocación. Copia la
función, no el archivo. Después:

1. reemplaza `iostream.h` / `conio.h` por `<cstdio>`;
2. reemplaza `void main()` por `int main(int argc, char **argv)`;
3. reemplaza las esperas de `getch()` por una entrada guionizada, o elimina la pausa por completo;
4. reemplaza `time(NULL)` por un valor de reloj guionizado, si y solo si el defecto no es la
   siembra;
5. imprime la traza con `std::printf`, una línea por evento.

Un esqueleto de banco que sirve para casi toda la tabla:

```cpp
#include <cstdio>
#include <cstdlib>

int main(int argc, char **argv)
{
    const int seed = (argc > 1) ? std::atoi(argv[1]) : 17;
    std::printf("seed=%d\n", seed);
    // ... la función heredada transcrita, con sus propias líneas de traza ...
    return 0;
}
```

Después ejecútalo dos veces:

```bash
c++ -std=c++20 -Wall -Wextra -O2 bench.cpp -o bench
./bench 17 > run-1.txt
./bench 17 > run-2.txt
cmp run-1.txt run-2.txt && echo "bench is deterministic"
```

Si `cmp` falla, detente. Nada de lo que sigue significa algo hasta que el banco sea función de sus
entradas.

### 1.3 Registra la traza A

```bash
./bench 17 > trace-a.txt
sha256sum trace-a.txt
cp trace-a.txt trace-a.expected.txt
```

Mantén `trace-a.expected.txt` en solo lectura durante el resto del taller. Es el invariante. No lo
edites, no lo regeneres y no lo "arregles" cuando un paso se ponga en rojo. Si cambia, la
refactorización cambió el comportamiento, y ese es el hallazgo.

Añade un pequeño ejecutor para que la comprobación sea un comando y no cuatro:

```bash
#!/bin/sh
set -eu
./bench 17 > /tmp/out.txt
cmp /tmp/out.txt trace-a.expected.txt
echo "trace-a: equal"
```

Confirma el banco y la traza A:

```bash
git add -A
git commit -m "test: record trace A for the legacy bench"
```

### 1.4 Cuenta el acoplamiento

Estos cuatro números se convierten en la columna `antes` de la tabla de costes. Tómalos ahora, del
banco y del archivo, y escribe cuál es cuál.

```bash
# 1. archivos del artefacto
ls *.cpp *.h 2>/dev/null | wc -l

# 2. líneas del artefacto
cat *.cpp *.h 2>/dev/null | wc -l

# 3. nombres públicos que un llamador debe conocer
grep -cE '^[A-Za-z_].*\);' header.h 2>/dev/null || echo "no header"

# 4. archivos que debes leer para responder: ¿cómo se almacena una celda?
grep -rln 'vectab\|tablero\|mvectab' . | wc -l
```

| Métrica | De dónde sale | Por qué importa |
| --- | --- | --- |
| Archivos | `ls` sobre el artefacto | La unidad de revisión y de recompilación |
| Líneas | `wc -l` sobre las fuentes | Un coste, nunca un veredicto |
| Nombres públicos | Declaraciones en las cabeceras | Lo que un llamador debe aprender para usar el módulo |
| Archivos de representación | `grep -rln` por el nombre del almacenamiento | Hasta dónde llega un cambio de representación |

El cuarto número es el que más importa y el que nadie mide. En `variant-03` la respuesta es un
archivo, porque todo es un archivo. En un módulo sano la respuesta también es un archivo, pero es la
implementación, no el llamador. El mismo número, el significado opuesto. Di cuál tienes.

### 1.5 Escribe la frase del defecto

Una frase, con esta forma:

> El programa hace **X** porque **Y** no está en **Z**.

Dos ejemplos:

> "El programa limpia solo la primera celda porque el puntero de reinicio no forma parte del tipo
> del tablero."
> "El programa coloca a los caballos de forma distinta en cada ejecución porque la semilla no es una
> entrada de la función de colocación."

Esta frase es lo que el escalón objetivo debe volver imposible. Si el escalón no toca `Y` o `Z`,
elegiste el escalón equivocado. Reescribe la frase en el informe §1 y compárala con §2.

## Paso 2 — Planificar (30 min)

### La lista de cambios

Rellena el informe §3 antes de editar nada. Cuatro listas:

| Lista | Qué contiene | Por qué está separada |
| --- | --- | --- |
| Archivos a copiar | Lo que tomas del archivo o de un artefacto de Track A | Copiar no es cambiar |
| Archivos a cambiar | Lo que vas a editar, por nombre | Una lista que puedes ir tachando |
| Archivos a crear | Cabeceras, implementaciones, pruebas | Los archivos nuevos son la huella del escalón |
| Intacto | La traza A, la entrada del banco, el archivo | El invariante, expresado como archivos |

La cuarta lista es la que la gente omite. Escríbela igual. "Intacto: `trace-a.expected.txt`, el
manejo de entrada de `bench.cpp`, `original/`" es una afirmación que puedes verificar al final.

### La predicción

Rellena la tabla de costes del informe §6 con valores previstos **antes** de la refactorización. No
edites la predicción después. La brecha entre predicción y medición es la señal de aprendizaje;
reparar la predicción la destruye.

Una predicción es una frase más un número, no una sensación:

| Métrica | Previsto | Por qué ese número |
| --- | ---: | --- |
| Archivos | | |
| Líneas | | |
| Nombres públicos | | |
| Pruebas | | |
| Tiempo de compilación | | |

Casi todos predicen demasiado pocos archivos y demasiadas líneas. El conteo de archivos crece porque
una frontera necesita una cabecera; el conteo de líneas a menudo baja porque la duplicación
desaparece. Si tu predicción falla por un factor de dos, ese es el párrafo más útil de tu informe.

### La prueba de cambio del escalón

Copia la prueba de cambio del catálogo en tu plan. Estos son los criterios de aceptación de los
escalones de Track A que este taller acepta:

| Escalón | Prueba de cambio |
| --- | --- |
| `construction-08` | Tablero 8×8 → 10×10; solo cambia el módulo del tablero |
| `construction-09` | Intercambia la representación del tablero; los llamadores compilan; las trazas pasan |
| `construction-11` | El estado privado y las operaciones preservan el invariante; el estado público expone un tablero inválido |
| `construction-12` | Añade una subclase de `Piece`; cero ediciones en el bucle; las trazas originales no cambian |
| `construction-13` | Reemplaza herencia por delegación; comportamiento y 20 trazas sin cambios |

Ejecutarás esta prueba en el paso 4. Es la segunda prueba, y es la razón por la que el taller no es
solo "haz que compile".

## Paso 3 — Refactorizar (65 min)

### Las cinco reglas

1. **Un paso mecánico a la vez.** Un paso es una idea: extraer la cabecera, mover las
   definiciones, separar la unidad de traducción, ocultar la representación. Cada paso compila y
   cada paso mantiene la traza A igual.
2. **Nunca edites la traza.** Si un paso se pone en rojo, el código está mal, no la expectativa.
3. **Nunca mezcles un cambio de comportamiento con un paso estructural.** Esa es la bifurcación de
   §"La bifurcación de corrección", y no es negociable.
4. **Revierte el paso, no la refactorización.** `git checkout -- .` te devuelve al último commit
   verde, que es un estado conocido como bueno. Revertir toda la refactorización tira información.
5. **Confirma cada paso verde.** El historial de commits es la fuente de verdad de la tabla de
   costes, y es la única respuesta honesta a "qué tan grande fue el cambio".

El bucle, completo:

```bash
# 1. haz un cambio
# 2. compila y comprueba
c++ -std=c++20 -Wall -Wextra -Werror -O2 *.cpp -o bench-new
./bench-new 17 | cmp - trace-a.expected.txt
# 3. si está verde, confirma
git add -A && git commit -m "refactor: extract the board header"
```

Cuatro líneas, sesenta y cinco minutos, repetidos hasta llegar al escalón. La disciplina es la
lección.

### Una separación modular, paso a paso

El camino más común es `archaeology-05` o `archaeology-08` hacia `construction-08`. Cinco pasos,
cada uno verde:

| Paso | Cambio | Qué prueba | Qué puede romperse |
| ---: | --- | --- | --- |
| 1 | Extrae las declaraciones a `board.h`; deja las definiciones donde están | Los nombres son separables del código | Una declaración que no coincide con la definición |
| 2 | Mueve las definiciones a `board.cpp`; sigue siendo una unidad de traducción | La separación es textual antes de ser física | Un ayudante `static` que el llamador usaba |
| 3 | Compila dos unidades de traducción y enlaza | La frontera es real | Un include faltante, un símbolo duplicado |
| 4 | Vuelve privada la representación: sin campo público, sin arreglo público | El llamador no puede nombrar el almacenamiento | Un llamador que indexa el arreglo directamente |
| 5 | Borra las globales que el módulo reemplazó; pasa el estado como parámetros | El acoplamiento oculto desapareció | Una función que leía una global por un canal lateral |

Después del paso 5, ejecuta la prueba de cambio: cambia las dimensiones del tablero en el módulo del
tablero y cuenta los archivos que tocaste. Si el conteo es uno, compraste `construction-08`. Si es
cinco, moviste el código y conservaste el acoplamiento.

### Cuando un paso se pone en rojo

| Síntoma | Causa más probable | Movimiento |
| --- | --- | --- |
| La traza difiere en una línea | Cambiaste un comportamiento observable | Revierte; encuentra la línea; pregunta si alguna vez estuvo en el invariante |
| La traza difiere en tiempo u orden | Introdujiste una segunda ruta de salida | Revierte; deja un solo escritor |
| Error de enlace: definición múltiple | Una cabecera define en lugar de declarar | Marca la definición `inline` o muévela |
| Error de enlace: referencia indefinida | Un ayudante quedó `static` y el llamador lo necesita | Ponlo en la cabecera, o haz que el llamador deje de necesitarlo |
| Compila, pero la prueba de cambio falla | Moviste código sin ocultarlo | El escalón no está alcanzado; sigue |
| Compila, pero el sanitizador ahora reporta | La refactorización expuso el defecto, o lo movió | Lee el reporte; es un hallazgo sobre la línea base |

La última fila merece su propio párrafo.

### La trampa del sanitizador

Una refactorización que preserva comportamiento conserva el defecto. Entonces, si ejecutas
AddressSanitizer antes y después, y reporta la misma escritura fuera de límites las dos veces, la
refactorización es correcta: movió el código y dejó el comportamiento solo.

Si el sanitizador **deja** de reportar después de una refactorización que debía preservar
comportamiento, ocurrió una de dos cosas:

- arreglaste el defecto dentro de la refactorización, lo que significa que tu diferencia contiene
  dos cambios y tu tabla de costes está mal;
- moviste la escritura a un lugar que el sanitizador no puede ver, lo que es peor que el defecto
  original.

Ambas son hallazgos. Ninguna es un éxito. Escribe cuál tienes.

## Paso 4 — Verificar (25 min)

### Compilar

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 *.cpp -o bench-new
```

`-Werror` no es decoración. Una advertencia que compila es un error con recibo, y todo el argumento
del taller es que los recibos importan.

### Trazas

```bash
./bench-new 17 | cmp - trace-a.expected.txt && echo "traces equal"
```

Después la diferencia a nivel de byte, que necesitarás para el informe:

```bash
diff -u trace-a.expected.txt <(./bench-new 17) || true
```

Usa `cmp` para decidir y `diff` para explicar. `cmp` responde "igual o no"; `diff` responde "dónde y
por qué". No uses `diff` solo: una diferencia vacía y una diferencia que no leíste se ven igual en
una captura de pantalla.

Si el artefacto es el juego completo en lugar de un banco local, ejecuta el conjunto dorado:

```bash
TRACES=/ruta/al/lab/common/traces
for trace in "$TRACES"/seed-*.txt; do
    name="$(basename "${trace}" .txt)"
    seed="${name#seed-}"
    ./bench-new "$((10#${seed}))" | cmp - "${trace}"
done
```

Veinte trazas, veinte llamadas silenciosas a `cmp`. Cualquier salida es un fallo.

### Determinismo

```bash
./bench-new 17 > run-a.txt
./bench-new 17 > run-b.txt
cmp run-a.txt run-b.txt && echo "deterministic"
```

La igualdad de trazas entre dos compilaciones no prueba nada si una compilación no es estable
consigo misma. Esta comprobación tarda dos segundos y atrapa cualquier `rand()` accidental que
hayas dejado.

### Sanitizadores

```bash
c++ -std=c++20 -g -O1 -fsanitize=address,undefined *.cpp -o bench-asan
./bench-asan 17 > asan-out.txt 2> asan-err.txt
```

Registra tres cosas: si `asan-out.txt` coincide con la traza A, qué dice `asan-err.txt`, y si el
reporte es el mismo que antes de la refactorización. La tercera es la que atrapa un arreglo oculto.

### La bifurcación de corrección, verificada

Si tomaste la bifurcación, ahora tienes dos trazas:

| Traza | Producida por | Qué prueba |
| --- | --- | --- |
| Traza A | El banco, y el banco refactorizado | Comportamiento preservado a través de un cambio estructural |
| Traza B | El banco refactorizado más el arreglo | El arreglo cambió exactamente lo que decía cambiar |

```bash
diff -u trace-a.expected.txt trace-b.txt
```

Cada línea de esa diferencia debe estar nombrada en el informe §5 y atribuida al arreglo, no a la
refactorización. Si una línea cambia y no puedes decir qué cambio la causó, la bifurcación falló y
la evidencia se perdió.

## Paso 5 — Explicar (7 min)

### La tabla de costes

Rellena el informe §6 con valores medidos. Las definiciones importan más que los números:

| Métrica | Cómo medirla | De qué es proxy |
| --- | --- | --- |
| Archivos | `git diff --stat` para tocados; `ls` para el total | Superficie de revisión y de recompilación |
| Líneas | `git diff --numstat` para añadidas/borradas; `wc -l` para el total | Coste de lectura, no calidad |
| Nombres públicos | Cuenta las declaraciones en las cabeceras que incluye un llamador | Lo que un recién llegado debe aprender |
| Pruebas | Cuenta las comprobaciones automáticas y las clases de fallo que cubren | Seguridad ante regresiones |
| Tiempo de compilación | Mediana de cinco compilaciones limpias, `time` en cada una | Velocidad de la máquina a este tamaño |

Dos notas honestas que pertenecen a tu informe:

- **El conteo de líneas es un coste, no un veredicto.** En este repositorio la escalera va de 507,
  523, 546, 532, 587 y 606 líneas para `construction-08` hasta `construction-13`.
  `construction-11` es *más pequeño* que `construction-10` porque la encapsulación eliminó una capa
  de funciones libres. Un escalón que añade líneas todavía puede eliminar acoplamiento.
- **El tiempo de compilación en un programa de 500 líneas mide tu portátil.** Regístralo y luego
  ignóralo. Se vuelve una métrica real en `construction-19`, donde la unidad de trabajo son 10.000
  partidas.

### El párrafo del veredicto

Tres frases, en el informe §7, en este orden:

1. ¿El escalón preservó el comportamiento, y cuál es la evidencia?
2. ¿Qué compró, en la prueba de cambio del propio escalón?
3. ¿Qué costó, y qué no arregló?

La tercera frase es la que separa un informe de un discurso de ventas. Todo escalón deja algo roto.
`construction-08` te da una frontera y ninguna independencia de representación. `construction-09` te
da el intercambio y un ciclo de vida manual. `construction-11` te da un invariante y ningún
polimorfismo. Nombra el siguiente defecto.

## Paso 6 — Registrar (8 min)

### Informe versus evidencia

Dos documentos, dos trabajos. No los dupliques.

| Documento | Trabajo | Forma |
| --- | --- | --- |
| `WORKSHOP-REPORT-es.md` | Argumenta el caso | Siete secciones, prosa y una tabla de costes |
| `EVIDENCE-es.md` | Demuestra el caso | Comandos, salida cruda, conteos, digests, diferencias |

Una frase como "la refactorización preservó el comportamiento" pertenece al informe. El comando que
lo muestra pertenece a la evidencia. Una afirmación sin comando es una opinión, y una opinión no es
una entrega del taller.

Rellena en este orden: §1 línea base, §2 objetivo, §3 plan (ya escrito), §4 refactorización, §5
verificación, §6 coste, §7 veredicto. Después el registro de evidencias: configuración, predicción,
conteos, digest de la traza A, la bitácora de pasos, la diferencia final, la frontera de evidencia y
la comprobación de integridad del archivo.

## Un ejemplo resuelto: construction-08 → construction-09

El movimiento de escalón útil más barato de este repositorio, medido. Está aquí para que puedas
calibrar tu propia predicción.

Ambos artefactos son el mismo juego, el mismo tablero de 8×8, las mismas reglas, el mismo formato de
traza. El movimiento cambia quién posee la disposición del tablero.

| Medición | `construction-08` | `construction-09` | Delta |
| --- | ---: | ---: | ---: |
| Archivos fuente | 7 | 7 | 0 |
| Líneas totales (código + build) | 507 | 523 | +16 |
| Líneas de `board.h` | 7 | 11 | +4 |
| Líneas de `board.cpp` | 57 | 65 | +8 |
| Líneas de `game.cpp` | 284 | 288 | +4 |
| Trazas doradas que coinciden | 20 de 20 | 20 de 20 | 0 |

La cabecera antes y después:

```cpp
// construction-08/board.h — la representación está en la compilación del llamador
int board_row_count();
int board_column_count();
void board_reset();
bool board_was_visited(int row, int column);
void board_mark_visited(int row, int column);
```

```cpp
// construction-09/board.h — la representación está en board.cpp
struct Board;
Board *board_create();
void board_destroy(Board *board);
int board_row_count(const Board *board);
int board_column_count(const Board *board);
void board_reset(Board *board);
bool board_was_visited(const Board *board, int row, int column);
void board_mark_visited(Board *board, int row, int column);
```

Dieciséis líneas, cuatro de ellas en la cabecera, y el llamador ya no puede medir, reservar ni
indexar un tablero. Eso es lo que cuesta un movimiento de escalón **cuando la frontera ya existe**.

### Por qué fue barato

`construction-08` ya tenía una cabecera y un `board.cpp` separado. El movimiento a TAD solo cambió
lo que la cabecera revelaba. La parte cara de `construction-09` se había pagado un escalón antes.

Este es el hecho más útil del taller: **los escalones son baratos en el orden en que se enseñan y
caros fuera de orden.** Saltar de `variant-03` a `construction-09` no es el mismo movimiento que ir
de `construction-08` a `construction-09`, aunque el objetivo sea idéntico.

### El caso caro

El mismo objetivo, desde el archivo:

| Hecho | Valor | Fuente |
| --- | --- | --- |
| Líneas en `variant-03` | 1058 | `wc -l` sobre el archivo fijado |
| Líneas que mencionan `mvectab` | 102 | `grep -c` |
| Cabeceras en el archivo | 0 | `find` de `*.h` y `*.hpp` |
| Ejecuciones deterministas disponibles | 0 | `srand(time(NULL))`, `getch()` |

Mover ese archivo a `construction-09` en un solo paso no es una refactorización; es una
reescritura con una refactorización como coartada. La separación en cinco pasos del paso 3 es lo que
la convierte en refactorización, y la razón de que la separación exista es que cada paso tiene su
propia
comparación de trazas.

## Qué prueba y qué no prueba "trazas iguales"

| Afirmación | ¿Sostenida? | Por qué |
| --- | --- | --- |
| Para la entrada 17, la salida es idéntica byte a byte. | Sí | `cmp` no imprimió nada; registraste el digest. |
| La refactorización no cambió ningún comportamiento observable para las entradas ejecutadas. | Sí | La traza es la observación. |
| Los dos programas son equivalentes para toda entrada. | No | Ejecutaste un conjunto finito. |
| La refactorización no introdujo comportamiento indefinido. | No | Solo un sanitizador sobre las rutas ejecutadas puede hablar de eso. |
| La refactorización eliminó el defecto. | No | Preservó el defecto a propósito. Esa es la bifurcación. |
| La capacidad del escalón se ganó. | Solo con su prueba de cambio | Las trazas prueban preservación, no capacidad. |
| El diseño es mejor. | No | "Mejor" necesita la tabla de costes y un contexto. |
| El archivo está arreglado. | No | Nunca lo editaste. |

La afirmación más fuerte del taller es un `cmp` que no imprimió nada. La más débil es "el diseño es
mejor". Mantenlas separadas en cada frase que escribas.

## Autocomprobación (binaria)

- [ ] Elegí un fallo y un escalón, y justifiqué ambos en una frase por cada uno.
- [ ] Mi banco transcribe el defecto y no lo repara.
- [ ] Mi banco controla su entrada en lugar de borrar el defecto.
- [ ] Dos ejecuciones del banco con la misma entrada produjeron los mismos bytes.
- [ ] Registré la traza A, su comando y su digest SHA-256 antes de la primera edición.
- [ ] Registré los cuatro conteos de acoplamiento, y dije cuál es el de representación.
- [ ] Escribí la lista de cambios, incluida la lista de lo intacto.
- [ ] Escribí la tabla de coste previsto antes de la refactorización y no la edité después.
- [ ] Copié la prueba de cambio del escalón desde el catálogo a mi plan.
- [ ] Cada paso confirmado compiló con `-Wall -Wextra -Werror`.
- [ ] Cada paso confirmado mantuvo la traza A idéntica byte a byte.
- [ ] Revertí los pasos fallidos individualmente, no toda la refactorización.
- [ ] Ejecuté la prueba de cambio del escalón y registré cuántos archivos tocó.
- [ ] Ejecuté la comprobación de determinismo sobre el artefacto refactorizado.
- [ ] Ejecuté un sanitizador y expliqué si su reporte cambió.
- [ ] Si tomé la bifurcación de corrección, la traza B existe y cada línea cambiada está atribuida.
- [ ] La tabla de costes contiene valores medidos, no adjetivos.
- [ ] El veredicto nombra qué compró el escalón, qué costó y qué no arregló.
- [ ] `WORKSHOP-REPORT-es.md` está completo.
- [ ] `EVIDENCE-es.md` está completo.
- [ ] El archivo está sin modificar (`git status` en el archivo está limpio).

Si una casilla no está marcada, la casilla sin marcar nombra el siguiente paso. No hay penalización
por repetir uno.

## Ideas erróneas que esta lección refuta

| Idea errónea | Por qué está mal | Qué hacer en su lugar |
| --- | --- | --- |
| "Una refactorización es una reescritura con mejores nombres." | Una reescritura cambia el comportamiento; una refactorización no. | Mantén la traza A verde en cada paso. |
| "Si las pruebas pasan, el comportamiento está preservado." | Las pruebas cubren las entradas que ejecutan, y nada más. | Enuncia el conjunto de entradas y después enuncia el límite. |
| "El archivo es la línea base." | No compila, no es determinista y espera una tecla. | Construye un banco con entrada controlada. |
| "Preservar comportamiento significa conservar el defecto para siempre." | La refactorización lo preserva; un arreglo separado lo elimina. | Toma la bifurcación de tres pasos. |
| "Borrar `srand(time(NULL))` hace determinista el banco." | También borra el defecto que elegiste. | Reemplaza el reloj, conserva la llamada. |
| "Más líneas es peor diseño." | `construction-11` es más pequeño que `construction-10`. | Valora el acoplamiento, no el conteo de líneas. |
| "La prueba de cambio y las trazas prueban lo mismo." | Una prueba capacidad, la otra prueba preservación. | Ejecuta ambas, reporta ambas. |
| "Un sanitizador limpio significa que no hay comportamiento indefinido." | Solo cubre las rutas que se ejecutaron. | Di "sin reporte en estas rutas". |
| "El tiempo de compilación dice algo del diseño." | A 500 líneas mide la máquina. | Regístralo y luego ignóralo. |
| "Un commit grande es igual que cinco pequeños." | La diferencia es la evidencia, y una diferencia grande no tiene ninguna. | Confirma cada paso verde. |
| "Si la traza A cambió, la refactorización estaba mal." | Quizá la traza A nunca fue función de la entrada. | Comprueba el determinismo antes de culpar al código. |
| "Copiar `construction-09` y renombrarlo es una refactorización." | El banco tiene forma heredada; el escalón debe aplicarse a él. | Aplica el escalón a tu banco, paso a paso. |
| "Un movimiento de escalón siempre es una mejora." | Un escalón que no puedes pagar es una deuda. | Valóralo y después decide. |

## Términos usados aquí

| Término | Significado en una línea |
| --- | --- |
| Línea base | Lo que el programa hace ahora, de forma reproducible. |
| Banco | El programa ejecutable más pequeño que contiene el defecto y escribe una traza. |
| Traza | La salida registrada de una ejecución, byte a byte. |
| Traza A | La traza de línea base que la refactorización no debe cambiar. |
| Traza B | La traza después de un arreglo etiquetado, con su diferencia explicada. |
| Prueba de caracterización | Una prueba que fija el comportamiento actual, correcto o no. |
| Preservación del comportamiento | Ninguna salida observable cambia para las entradas registradas. |
| Refactorización | Un cambio de estructura que preserva el comportamiento. |
| Arreglo | Un cambio de comportamiento, etiquetado y justificado por separado. |
| Escalón | Una lección de Track A, definida por su prueba de cambio. |
| Prueba de cambio | La edición que un escalón promete abaratar. |
| Conteo de acoplamiento | Cuántos archivos deben cambiar cuando cambia un concepto. |
| Identidad de bytes | Dos salidas iguales a nivel de byte, decidida por `cmp`. |
| Traza dorada | Una traza registrada como contrato de comportamiento. |
| Sanitizador | Un verificador en tiempo de ejecución de errores de memoria y comportamiento indefinido. |
| Copia de trabajo | Una copia fuera del archivo y fuera del árbol versionado. |
| Revertir | Volver al último commit verde, no al principio. |

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
