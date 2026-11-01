# archaeology-01 — Compilar y ejecutar código heredado

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** compilar y ejecutar código heredado ·
> **Prerrequisito:** common-03 · **Tiempo estimado:** 60 minutos
> **Lectura:** ~15 minutos

## Por qué existe esta lección

Toda autopsia de código heredado empieza con la misma pregunta incómoda: ¿esto siquiera se ejecuta?
Puedes leer un archivo de 930 líneas durante una hora y construir una teoría preciosa sobre sus
defectos, y luego descubrir que ese código nunca compiló en la máquina que tienes delante. Leer sin
ejecutar es adivinar con pasos extra.

Este es el paso 2 del protocolo de Track B: **predecir, compilar y ejecutar**. El producto es una de
dos cosas, y las dos son aceptables: una compilación ejecutable, o un bloqueo documentado. Un
bloqueo no es un fracaso. Un bloqueo que puedes nombrar, citar y reproducir es evidencia. "No
funciona" no lo es.

La lección enseña también una disciplina más difícil. Cuando el programa ya corre, la tentación es
decir *"funciona"*. No funciona. Compila a través de una capa de compatibilidad, se ejecuta hasta el
final e imprime algo. Si ese algo es correcto, portable o el comportamiento que el archivo describe
es otra pregunta, con otra respuesta. Separar "se ejecuta" de "es correcto" es toda la habilidad.

## Qué podrás hacer

Al terminar esta lección puedes:

- escribir una predicción falsable sobre un programa que aún no has ejecutado;
- reproducir los errores exactos del compilador en una compilación moderna ingenua, citados
  literalmente;
- explicar qué hace cada cambio de compatibilidad y qué riesgo introduce;
- compilar el código del archivo sin modificarlo, mediante una sonda que vive fuera del archivo;
- ejecutarlo con un límite de tiempo y registrar el estado de salida, el volumen de salida y la
  primera línea de error;
- explicar por qué la salida no es el contrato de transcripción moderna;
- enunciar en un párrafo qué prueba la sonda y qué no prueba;
- registrar la predicción, los errores, los cambios, la salida y el límite en `EVIDENCE-es.md`.

## El concepto: compilar y ejecutar código heredado

Una **compilación** es una transformación de código fuente a un artefacto ejecutable. En una máquina
moderna tiene cuatro entradas: el código, el compilador y su versión, los encabezados que resuelve y
las banderas. Una compilación heredada añade una quinta: el entorno que el código asumía pero nunca
declaró.

Este programa asumía una consola Windows y una biblioteca de C++ anterior al estándar. Ninguna de
las dos está presente. Por eso la compilación se divide en dos preguntas:

1. **¿Qué rechaza el compilador?** Son errores, y son texto exacto y citable.
2. **¿Qué asumía el código sobre el mundo?** No son errores. Son suposiciones silenciosas que fallan
   en tiempo de ejecución o cambian en silencio lo que el programa hace.

| Idea | Significado en esta lección |
|---|---|
| **Compilación** | Código, compilador, encabezados y banderas produciendo un artefacto ejecutable. |
| **Cuña de compatibilidad** | Un encabezado que añades para satisfacer un `include` que la biblioteca moderna ya no trae. |
| **Bandera como cambio** | Una bandera que altera el significado del código (`-fpermissive`, `-Dmain=...`). |
| **Bloqueo** | Un fallo reproducible y citable: comando, error, entorno. |
| **Límite de tiempo** | Una parada dura para que un bucle heredado no cuelgue tu sesión. |
| **Límite de la evidencia** | La línea entre "compiló y terminó" y "se comporta correctamente". |

### Por qué la compilación ingenua no es una pérdida de tiempo

Podrías ir directo a la sonda. No lo hagas. La compilación ingenua es el experimento más barato
posible, y sus errores son las frases más valiosas de la lección: son el compilador diciéndote, con
sus propias palabras, cuáles supuestos de la herramienta antigua ya no existen. Quien se salta la
compilación ingenua tiene que confiar en el resumen de la lección. Quien la ejecuta
tiene una fuente primaria.

El orden también importa. El compilador se detiene en el primer error fatal, así que arreglas un
supuesto a la vez y ves aparecer el siguiente. Esa escalera —`conio.h`, luego `iostream.h`, luego
`void main`— es la forma del port, y la descubres en lugar de que te la cuenten.

### Por qué "se ejecutó" no es "funciona"

Tres afirmaciones distintas se esconden en esa frase:

- **Compila.** Existe un binario. Es demostrable: el compilador salió con 0.
- **Se ejecuta.** El proceso llegó a su propio final. Es demostrable: estado de salida 0.
- **Es correcto.** El comportamiento coincide con la especificación. Esto **no** es demostrable con
  una ejecución cuya salida no puedes comparar contra nada.

La tercera afirmación necesita un contrato. Este proyecto tiene uno —la transcripción moderna— y el
binario heredado no lo produce. Eso no es un defecto del programa heredado; es otro formato de
salida para otro consumidor. Confundir los dos es cómo una lección de compilación se convierte en
una afirmación falsa.

## Antes de empezar

Lee esto completo:

- [`README-es.md`](README-es.md) — la ficha de la lección, la tabla de la sonda y la salida medida.
- [`docs/guide/archaeology-track-es.md`](../../../docs/guide/archaeology-track-es.md) — §2, el
  protocolo de autopsia, en especial el paso 2.
- [`docs/archive-es.md`](../../../docs/archive-es.md) — el archivo fijado y lo que excluye.
- [`docs/guide/lesson-catalog-es.md`](../../../docs/guide/lesson-catalog-es.md) — el prerrequisito
  `common-03` y el criterio de aceptación de `archaeology-01`.

También necesitas:

- un compilador de C++ (`c++`, `g++` o `clang++`) y su versión;
- `bash`, `timeout`, `wc` y `head` — todos estándar;
- una copia local del archivo en la etiqueta `v1.0.0`;
- la sonda en [`code/`](code/), que viene con esta lección.

**No** necesitas una máquina Windows, ni un compilador viejo, ni una máquina virtual. Necesitas ser
honesto sobre lo que una máquina moderna puede y no puede probar.

> **Regla:** el archivo es de solo lectura. Todo cambio de esta lección ocurre en un directorio
> temporal o en los archivos de la propia sonda. Si te descubres editando
> `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`, detente.

## El camino de 60 minutos

| Min | Paso | Qué haces | Evidencia |
|---|---|---|---|
| 0–10 | Predecir | Escribes qué esperas del programa, antes de ejecutarlo | Nota de predicción |
| 10–20 | Compilación ingenua | Intentas una compilación moderna; citas cada error en orden | Escalera de errores |
| 20–35 | Compilación con sonda | Lees las cuñas y compilas con la sonda | Tabla de compatibilidad |
| 35–45 | Ejecutar y observar | Ejecutas con límite de tiempo; registras estado, volúmenes y primer error | Registro de ejecución |
| 45–52 | Comparar | Predicción contra observación, afirmación por afirmación | Tabla de comparación |
| 52–60 | Explicar y registrar | El párrafo del límite y `EVIDENCE-es.md` | Entrada de portafolio |

Si la escalera de errores te lleva veinte minutos porque quieres
entender cada `include`, eso es información sobre la escalera, no sobre ti.

### La comprobación de ingeniería en esta lección

| Paso | Dónde ocurre | Qué produce |
|---|---|---|
| **Predecir** | Paso 1 | Comportamiento, condición de victoria y modo de fallo, por escrito. |
| **Compilar y ejecutar** | Pasos 2–4 | Errores exactos, tabla de compatibilidad, registro de ejecución. |
| **Comparar** | Paso 5 | Un veredicto por cada comportamiento predicho. |
| **Explicar** | Paso 6 | Por qué existe la diferencia, con evidencia. |
| **Registrar** | Paso 7 | `EVIDENCE-es.md`, con el límite declarado. |

## Paso 1 — Predecir antes de compilar (10 min)

Tienes el código y no lo has ejecutado. Escribe, con tus palabras:

1. **La interfaz.** ¿Qué ve quien juega? ¿Una pantalla, o un registro que se desplaza?
2. **La interacción.** ¿El programa espera entrada, y en qué puntos?
3. **El azar.** ¿Qué es aleatorio, y el juego es reproducible si lo ejecutas dos veces?
4. **La condición de fin.** ¿Cómo termina la partida y qué imprime cuando termina?
5. **El modo de fallo.** ¿Qué esperas que se rompa primero en Linux?

Después escribe una frase falsable sobre el conjunto. Por ejemplo:

> Espero que el programa dibuje un tablero, espere una tecla entre turnos, coloque los caballos al
> azar y termine cuando un caballo caiga sobre el otro; en Linux espero que las llamadas a la
> consola de Windows fallen y que los símbolos del tablero salgan como basura.

Esa frase puede estar mal en al menos tres puntos. Bien. Una predicción que no puedes falsar no
enseña nada.

### Una predicción trabajada

Toma la afirmación 2, la interacción, solo desde el código:

- respuesta esperada: sí, el programa hace pausas entre turnos;
- evidencia esperada: `getch()` en la línea 42 y otra vez en las líneas 781, 806, 835 y 861;
- veredicto esperado en Linux: la pausa desaparece, porque `getch()` no está disponible.

Lee los `include` antes de decidir: `conio.h`, en la línea 11, es el encabezado que proporciona
`getch()`, y es lo primero que un compilador moderno va a rechazar. La predicción y la escalera de
errores están a punto de encontrarse.

## Paso 2 — Ejecutar la compilación ingenua y registrar la escalera (10 min)

Copia el código fuera del archivo primero. Nunca compiles en el lugar si puedes evitarlo, y nunca
dejes que una compilación escriba dentro del archivo:

```bash
mkdir -p /tmp/legacy-probe && cd /tmp/legacy-probe
cp /ruta/al/archivo/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp .
```

Luego el intento ingenuo, con la configuración regional fijada para que los mensajes sean
reproducibles:

```bash
LC_ALL=C c++ -std=c++20 -fsyntax-only guerra-de-caballos-baseline.cpp
```

Primer error esperado, literal:

```text
guerra-de-caballos-baseline.cpp:11:9: fatal error: conio.h: No such file or directory
   11 | #include<conio.h>
      |         ^~~~~~~~~
compilation terminated.
```

Fíjate en el número de línea: 11. El `include` de `conio.h` viene **antes** que el de `iostream.h`
en la línea 12, así que el compilador se detiene ahí y nunca ves el segundo encabezado ausente. Por
eso se arregla una cosa a la vez.

Ahora satisface solo `conio.h` e `iostream.h` con cuñas mínimas y compila otra vez:

```bash
mkdir -p compat
printf '#pragma once\ninline int getch(){return 0;}\n' > compat/conio.h
printf '#pragma once\n#include <iostream>\nusing namespace std;\n' > compat/iostream.h
LC_ALL=C c++ -std=c++20 -fsyntax-only -Icompat guerra-de-caballos-baseline.cpp
```

Segundo error esperado, literal:

```text
guerra-de-caballos-baseline.cpp:32:1: error: '::main' must return 'int'
   32 | void  main()//funcion principal
      | ^~~~
```

Dos errores, dos clases distintas de problema:

| Error | Clase | Qué te dice |
|---|---|---|
| `conio.h` no encontrado | encabezado ausente | El código apunta a una biblioteca de plataforma que ya no existe. |
| `'::main' must return 'int'` | regla del lenguaje | `void main()` se toleraba con el compilador antiguo y no es C++ ahora. |

El segundo es el interesante. `-fpermissive` lo degradará, y eso es exactamente lo que significa
`-fpermissive`: *"sé que esto no es válido; compílalo igual"*. Registra que lo usaste y registra por
qué. Una bandera que suprime una regla del lenguaje es un cambio de compatibilidad, no una
comodidad.

### Si tu escalera es distinta

Probablemente lo será, un poco. Registra lo que viste, no lo que dice esta página:

- otro compilador puede redactar los mensajes distinto, o reportar `iostream.h` primero;
- una configuración regional que no sea inglés traducirá el texto —por eso los comandos fijan
  `LC_ALL=C`;
- un compilador con `-std=gnu++20` por defecto puede aceptar más antes de quejarse.

La regla: la escalera es tu escalera. Cita tu salida.

## Paso 3 — Compilar con la sonda (15 min)

La sonda en [`code/`](code/) hace cuatro cosas: copia el código a un directorio temporal, lo compila
con los encabezados de compatibilidad, enlaza un envoltorio con `int main` estándar y ejecuta el
resultado con un límite de tiempo. Lee los cuatro archivos antes de ejecutarla.

| Archivo | Qué hace | Riesgo que introduce |
|---|---|---|
| `code/compat/iostream.h` | Lleva el encabezado preestándar a `<iostream>` e inyecta `using namespace std;` | La inyección global de espacio de nombres puede cambiar la resolución de sobrecargas. |
| `code/compat/conio.h` | Proporciona un `getch()` en línea que devuelve `0` de inmediato | El programa ya no espera una tecla; la interacción desaparece. |
| `code/legacy_wrapper.cpp` | Declara `legacy_main` y la llama desde `int main` | El punto de entrada se renombra, no se repara. |
| `code/build_probe.sh` | Copia, compila, enlaza, ejecuta con `timeout 3s`, imprime seis líneas | El límite puede truncar una ejecución lenta y reportar un bloqueo falso. |

La línea de compilación es la parte que hay que entender:

```bash
"${compiler}" -std=c++20 -fpermissive -Dmain=legacy_main \
    -I"${scriptDirectory}/compat" -c "${scratchDirectory}/legacy.cpp" \
    -o "${scratchDirectory}/legacy.o"
```

Tres decisiones viven en esas dos líneas:

- `-fpermissive` — acepta `void main()` y otras construcciones preestándar como advertencias en
  lugar de errores.
- `-Dmain=legacy_main` — renombra el punto de entrada en tiempo de preprocesado, para que el
  envoltorio sea dueño de `int main` sin editar el código. El archivo conserva sus bytes originales.
- `-Icode/compat` — pone las cuñas delante de los encabezados del sistema. `iostream.h` y `conio.h`
  se resuelven en los archivos de la lección, no en algo que el compilador traiga.

Ninguna de las tres es un cambio en el código. Esa distinción es el objetivo del ejercicio: **la
sonda prueba que el código se puede adaptar, no que el código fue adaptado.**

Ahora ejecútala desde el directorio de la lección:

```bash
./code/build_probe.sh /ruta/al/archivo
```

Una ejecución registrada en la máquina de referencia, seis líneas:

```text
source=<archivo>/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
compile=ok
run_status=0
stdout_lines=614
stderr_lines=26
stderr_first=sh: línea 1: color: orden no encontrada
```

No esperes que tus dos conteos de líneas coincidan con esos. Solo `compile=ok` y `run_status=0` son
estables; los volúmenes dependen de cuántos turnos duró la partida. El paso 4 explica por qué, y
cómo demostrarlo.

Lee la sexta línea con cuidado. No es un error del compilador y no es el error de esta lección. Es
el sistema operativo informando que el comando de shell `color` no existe. El programa llamó a
`system("color 1f")` en la línea 38 y a `system("cls")` en la línea 43, y en Linux esas son
invocaciones al shell de comandos que nunca se instalaron. El mensaje está localizado, así que tu
máquina puede imprimir `orden no encontrada` o `command not found`.

## Paso 4 — Ejecutar, observar y medir la varianza (10 min)

Las seis líneas anteriores son un resumen. Ahora mira los artefactos que la sonda borra, ejecutando
los mismos pasos a mano una vez:

```bash
cd /tmp/legacy-probe
c++ -std=c++20 -fpermissive -Dmain=legacy_main -Icompat -c guerra-de-caballos-baseline.cpp
c++ -std=c++20 /ruta/a/la/leccion/code/legacy_wrapper.cpp guerra-de-caballos-baseline.o -o probe
timeout 3s ./probe < /dev/null > out.txt 2> err.txt; echo "status=$?"
wc -l out.txt err.txt
head -1 err.txt
```

Cuatro observaciones que registrar, en este orden.

**1. Termina por sí solo.** `status=0` significa que el proceso llegó al final de `main` y retornó.
En una máquina donde el programa itera sin fin, `timeout` devuelve `124`. Esa diferencia —`0` frente
a `124`— es la diferencia entre una compilación ejecutable y un bloqueo, y va en tu registro.

**2. El volumen de salida no es estable.** Los conteos de líneas se mueven entre ejecuciones: 614 y
26 en una sesión, 223 y 9 en otra, 936 y 40 en una tercera. El conteo sigue a cuántos turnos duró la
partida, y el número de turnos depende de dónde arrancaron los caballos. Un solo número no es un
contrato; es una muestra.

**3. Mismo segundo, mismos bytes.** Ejecuta el binario dos veces dentro del mismo segundo de reloj y
las dos salidas son idénticas byte a byte. Espera dos segundos y ejecuta otra vez, y difieren. La
causa está en la línea 76: `srand(time(NULL))` siembra desde un reloj con resolución de un segundo.
Esto no es determinismo. Es no determinismo con un reloj grueso, y confundir uno con otro es
exactamente el fallo que `common-01` existe para evitar.

**4. Los símbolos son bytes CP437 crudos.** El tablero se dibuja con `simcab1 = 21`, `simcab2 = 156`
y `sep = 219` (línea 16). Son bytes sueltos: `0x15`, `0x9C`, `0xDB`. No son UTF-8, así que una
terminal UTF-8 muestra caracteres de reemplazo o nada. Haz un volcado hexadecimal de una línea del
tablero y verás `dbdb dbdb 2020 ...` —la pared es `0xDB`, repetida. Esto es un problema de
codificación de texto, no un fallo de representación de tu terminal.

**5. La pantalla es un registro acumulado.** El programa pretende repintar una sola pantalla:
`system("cls")` antes de cada turno. En Linux `cls` falla, no se borra nada y cada turno se añade.
El número de fallos de `cls` en `stderr` es igual al número de turnos. Estás leyendo un registro de
todas las pantallas que el programa quería mostrar, concatenadas.

## Paso 5 — Comparar predicción y observación (7 min)

Una tabla, un veredicto por predicción. Usa exactamente tres veredictos; no inventes un cuarto.

| Veredicto | Prueba | Qué registrar |
|---|---|---|
| **Confirmada** | La observación coincide con lo que escribiste. | La línea de salida o el estado que la confirma. |
| **Contradicha** | La observación es distinta, y puedes decir en qué. | Ambas versiones, lado a lado. |
| **Sin resolver** | No puedes saberlo con esta ejecución. | Qué necesitarías observar. |

### Comparación trabajada

| # | Predicho | Observado | Veredicto |
|---|---|---|---|
| 1 | Una pantalla repintada | Registro acumulado, `cls` falla | Contradicha |
| 2 | Espera una tecla entre turnos | Sin pausa; `getch()` devuelve 0 | Contradicha |
| 3 | Colocación aleatoria, no reproducible | Idéntica dentro del mismo segundo, distinta después | Contradicha (en el sentido interesante) |
| 4 | Termina cuando un caballo cae sobre el otro | Imprime una línea de ganador, sale con 0 | Confirmada |
| 5 | Las llamadas a consola fallan en Linux | 26 líneas en `stderr`, la primera es `color` | Confirmada |

La fila 3 es la que hay que pensar despacio. Tu predicción era "no reproducible". La observación fue
"reproducible a veces". Los dos tenías razón en parte, y la causa es un reloj de un segundo, no una
semilla. Ese es un modelo mental mejor que la predicción y que la conclusión ingenua.

## Paso 6 — Explicar el límite (5 min)

Escribe un párrafo, sin palabras de sintaxis, que responda: ¿qué probó esta compilación, qué no
probó y dónde se detiene exactamente la evidencia?

Usa esta forma si necesitas una:

> La sonda prueba **\_\_\_**. No prueba **\_\_\_**, porque **\_\_\_**. La evidencia se detiene en
> **\_\_\_**.

### Las cuatro afirmaciones que esta sonda no sostiene

| Afirmación | Por qué la sonda no puede sostenerla |
|---|---|
| "El código es portable." | Dos cuñas y una bandera permisiva son lo contrario de la portabilidad. |
| "La salida es correcta." | Nada la compara con una especificación. El juego termina; eso es todo. |
| "La herramienta histórica era Visual C++ 6.0." | Esa afirmación vive en la bóveda privada. El archivo público no puede probarla. |
| "Esto reproduce la transcripción moderna." | La transcripción moderna es un registro `clave=valor`; esto imprime bytes CP437. |

La última fila es la que más importa para el resto de Track B. El contrato moderno en
[`common/traces/`](../../../common/traces/) se ve así:

```text
seed=1
player=Learner
roll_player=11
roll_machine=20
start=machine
turn=1 who=machine at=4,4 order=1,2,3,4,5,6,7,8
move=1 machine 4,4->3,6
```

El binario heredado no puede producir eso. No porque esté roto, sino porque nunca se lo pidieron:
estaba escrito para una persona mirando una pantalla de color, no para un arnés de pruebas leyendo
líneas. Cualquier comparación futura entre los dos necesita un paso de reconstrucción, y esa
reconstrucción es una afirmación aparte, con su propia evidencia.

### La frase honesta

Termina con una frase que estarías dispuesto a defender en una revisión de código:

> *"El código del archivo compila en un compilador moderno mediante una cuña de compatibilidad de
> dos encabezados y una bandera permisiva, se ejecuta hasta el final con las llamadas a consola
> fallando, e imprime un tablero CP437; no produce la transcripción moderna, y ninguna ejecución
> suya puede probar la herramienta histórica."*

Si tu frase es más corta y más fuerte que esa, revisa qué cláusula eliminaste.

## Paso 7 — Registrar (5 min)

Llena [`EVIDENCE-es.md`](EVIDENCE-es.md). Pide el entorno, la predicción, la escalera de errores, la
tabla de compatibilidad, el registro de ejecución, la comparación, el límite y el paso siguiente.

Dos hábitos que vale la pena conservar:

- **Pega, no parafrasees.** `compile=ok` es evidencia. "Compiló bien" es un recuerdo.
- **Registra la versión de todo.** Compilador, `bash`, `timeout`, el commit del archivo. Un
  resultado de compilación sin versiones es un rumor.

## El original en detalle

Datos que citarás muchas veces, con números de línea del archivo fijado:

| Línea | Qué hay | Por qué importa para la compilación |
|---|---|---|
| 10–14 | `stdio.h`, `conio.h`, `iostream.h`, `stdlib.h`, `time.h` | Dos de los cinco encabezados no existen en C++ moderno. |
| 16 | `simcab1=21, simcab2=156, sep=219` | Puntos de código CP437 como `char`, no UTF-8. |
| 17–18 | `vectab[64]`, `vecjugadas[8]`, variables sueltas de caballo | Estado global; el programa entero es una sola unidad. |
| 32 | `void  main()` | No es C++ estándar; el segundo error de la escalera. |
| 38 | `system("color 1f")` | Un comando de shell de Windows; falla en Linux. |
| 42 | `getch()` | Entrada de consola desde `conio.h`; la cuña elimina la pausa. |
| 43 | `system("cls")` | Un comando de shell de Windows; falla en Linux, una vez por turno. |
| 76 | `srand(time(NULL))` | Resolución de semilla de un segundo; explica el patrón de varianza. |
| 180 | `(rand() % 8)+1` | Aleatorización del orden de jugadas; otra fuente de diferencia entre ejecuciones. |
| 930 | fin del archivo | Sin pruebas, sin archivo de compilación, sin `main` que devuelva `int`. |

El archivo no tiene ningún archivo de compilación: ni `Makefile`, ni `CMakeLists.txt`, ni un archivo
de proyecto en el conjunto público. Esa ausencia es en sí misma un hallazgo, y pertenece a
`archaeology-02`, no a esta lección. Aquí solo explica por qué esta lección trae una sonda en lugar
de un comando del archivo.

## Qué cuenta como bloqueo

El criterio de aceptación es "una compilación ejecutable **o** un bloqueo documentado". Un bloqueo
es aceptable cuando tiene las cuatro partes:

1. **El comando exacto**, copiable, con banderas y rutas.
2. **La salida exacta**, citada, no resumida.
3. **El entorno**: compilador y versión, sistema operativo, shell, commit del archivo.
4. **El siguiente experimento** que probarías, y qué resultado te haría cambiar de opinión.

Un bloqueo sin la parte 4 es un callejón sin salida. Un bloqueo con la parte 4 es una nota de
investigación.

Bloqueos típicos en esta lección, y cómo se ven:

| Síntoma | Causa probable | Qué registrar |
|---|---|---|
| `run_status=124` | El programa no llega al final en 3 s | Que se ejecutó, que no terminó, y cuánto lo dejaste correr |
| `compile=failed` | Compilador demasiado viejo para `-std=c++20`, o conflicto de cuñas | La versión del compilador y el primer error |
| Error de enlazado en `legacy_main` | `-Dmain=legacy_main` no se aplicó en la compilación | Los dos comandos, lado a lado |
| `stdout` vacío | El binario escribió en otro flujo, o salió antes | `run_status` y `stderr` |

## Qué no hace esta lección

- No diagnostica un solo defecto. `for (i=0;i<=64;i++)`, en la línea 61, es visible y no es el tema
  de hoy. Anótalo y sigue.
- No arregla nada. El archivo es de solo lectura y la sonda es un adaptador de compilación.
- No afirma portabilidad. Dos cuñas no son un port.
- No valida la salida contra una especificación. Eso es `archaeology-03`.
- No resuelve la pregunta de la herramienta. Ese límite se fijó en `archaeology-00`.
- No produce la transcripción moderna. Muestra por qué hace falta una reconstrucción.

Si te descubres explicando *por qué* el tablero está mal, detente y escríbelo como pregunta para
`archaeology-04`.

## Autocomprobación (binaria)

- [ ] Escribí cinco predicciones antes de ejecutar nada.
- [ ] Ejecuté la compilación ingenua y cité el primer error literal, con el número de línea.
- [ ] Satisfice los encabezados uno a uno y cité el segundo error.
- [ ] Puedo explicar qué acepta `-fpermissive` y por qué eso es un cambio de compatibilidad.
- [ ] Puedo explicar qué hace `-Dmain=legacy_main` y por qué el archivo queda intacto.
- [ ] Ejecuté `build_probe.sh` y registré sus seis líneas de salida.
- [ ] Registré `run_status` y sé qué habría significado `124`.
- [ ] Observé que los conteos de líneas varían, y puedo nombrar dos causas.
- [ ] Observé que dos ejecuciones en el mismo segundo son idénticas, y sé por qué eso no es
      determinismo.
- [ ] Confirmé que el tablero son bytes CP437 crudos, no UTF-8.
- [ ] Comparé mi predicción con la observación, con un veredicto por fila.
- [ ] Escribí el párrafo del límite, incluidas las cuatro afirmaciones que la sonda no sostiene.
- [ ] Llené `EVIDENCE-es.md`.
- [ ] El archivo está sin modificar (`git status` en el archivo está limpio).

Si una casilla no está marcada, esa casilla nombra el paso siguiente. No hay penalización por
repetir uno.

## Misconcepciones que esta lección refuta

| Concepto erróneo | Por qué es incorrecto | Qué hacer en su lugar |
|---|---|---|
| "Si compila, el port está hecho." | Dos cuñas y `-fpermissive` significan que el código se tolera, no que se portó. | Llámalo compilación adaptada y enumera las adaptaciones. |
| "`-fpermissive` es solo una bandera." | Degrada reglas del lenguaje; el programa compila en un estado que el estándar prohíbe. | Regístrala como cambio semántico, junto a las cuñas. |
| "Estado de salida 0 significa comportamiento correcto." | Significa que el proceso llegó a su final. La corrección necesita un contrato. | Compara con una especificación o una traza. |
| "La salida es basura, así que el programa está roto." | Los bytes son CP437 válidos; tu terminal habla UTF-8. | Haz un volcado hexadecimal antes de juzgar. |
| "Los conteos varían, así que el programa no es determinista." | También coinciden dentro del mismo segundo. La semilla tiene resolución de un segundo. | Nombra el reloj, no los dados. |
| "Un bloqueo significa que la lección fracasó." | El criterio acepta un bloqueo con comando, salida, entorno y paso siguiente. | Escribe las cuatro partes. |
| "La sonda modifica el archivo." | Copia a un directorio temporal y renombra `main` en el preprocesado. | Verifícalo con `git status` en el archivo. |
| "La compilación prueba la herramienta original." | La sonda prueba que el compilador actual puede compilar la fuente; no prueba la herramienta original. | Registra ambas afirmaciones por separado. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Compilación | Código, compilador, encabezados y banderas produciendo un artefacto ejecutable. |
| Compilación ingenua | El primer intento, sin ayuda de compatibilidad, cuyos errores son la evidencia. |
| Escalera de errores | La secuencia de errores que obtienes al arreglar un supuesto a la vez. |
| Cuña de compatibilidad | Un encabezado que suples para satisfacer un `include` que la biblioteca moderna eliminó. |
| `-fpermissive` | Una bandera que degrada algunos errores del lenguaje a advertencias. |
| Punto de entrada renombrado | `main` reescrito a `legacy_main` en el preprocesado, código intacto. |
| Límite de tiempo | Una parada dura para que un bucle heredado no cuelgue la sesión. |
| CP437 | La página de códigos IBM de la que salen los símbolos del tablero; no es UTF-8. |
| Bloqueo | Un fallo reproducible con comando, salida, entorno y siguiente experimento. |
| Límite de la evidencia | La línea entre lo que una ejecución prueba y lo que solo sugiere. |

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
