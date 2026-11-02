# archaeology-13 — Documentación técnica para mantenedores

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** documentación · **Prerrequisito:** archaeology-03 ·
> **Tiempo estimado:** 60 minutos
> **Lectura:** ~19 minutos
> **Rol de la lección:** opcional · puedes omitirla

## Por qué existe esta lección

`archaeology-03` reconstruyó una especificación de comportamiento a partir del código.
`archaeology-04` a `archaeology-12` encontraron fallos y midieron su coste. Nada de ese trabajo
llega a un mantenedor hasta que está escrito donde el mantenedor mira: un README, una
especificación de comportamiento y comandos que se ejecutan.

El catálogo resume este fallo como `failure-017`: al artefacto le faltan los documentos que un
mantenedor necesita. Medido contra el archivo fijado, el resumen es demasiado amplio. El archivo
tiene un `README.md` raíz, un README por variante y un `04-build-status.md` que nombra la cadena de
herramientas histórica. Lo que no tiene es más estrecho y más útil: ninguna especificación de
comportamiento, ningún comando exacto de compilación, ningún comando de ejecución y ningún comando
de prueba. Un mantenedor puede leer para qué sirve el programa y aun así no puede compilarlo,
ejecutarlo ni saber si un cambio lo rompió.

Ese hueco es la lección. La documentación no es una descripción del código. Es una interfaz con
tres lectores:

- el mantenedor nuevo, que necesita compilar y ejecutar el artefacto hoy;
- tu yo futuro, que necesita recordar por qué un comando tiene tres banderas;
- la persona que revisa, que necesita saber qué afirmaciones se verificaron y cuáles se asumieron.

Un README que dice "compílalo con el compilador original" no es una instrucción de compilación. Es
un recuerdo. Una especificación que dice "el caballo se mueve normalmente" no es una
especificación. Es una esperanza. Esta lección convierte ambos en artefactos que una persona ajena
puede ejecutar y comprobar.

## Qué podrás hacer

Al terminar esta lección puedes:

- inventariar la documentación que existe y nombrar el hueco que importa;
- escribir un README que responda qué es el artefacto, cómo compilarlo, cómo ejecutarlo, cómo
  probarlo y qué sigue desconocido;
- escribir una especificación de comportamiento con una cita por cláusula y una marca de confianza
  por cláusula;
- escribir comandos que una persona ajena puede pegar, incluido el entorno que asumen;
- auditar cada comando ejecutándolo desde un directorio limpio;
- auditar cada afirmación contra una línea, un comando o un documento nombrado;
- declarar los desconocidos sin esconderlos detrás de una prosa segura;
- mantener el documento centrado en el artefacto y no en su contexto.

La lección está hecha cuando otra persona puede compilar, ejecutar y probar la fuente documentada
sin hacerte una pregunta.

## El concepto: documentación para mantenedores

### Tres lectores, tres preguntas

La documentación falla cuando responde la pregunta de quien escribe en lugar de la de quien lee.
Quien escribe conoce la historia, la máquina y los atajos. Quien lee no conoce ninguno.

| Lector | Pregunta | Qué necesita |
|---|---|---|
| Mantenedor nuevo | ¿Cómo hago que esto funcione? | Comandos exactos, dependencias, salida esperada. |
| Tu yo futuro | ¿Por qué el comando tiene esta forma? | El motivo de cada bandera y cada adaptación. |
| Revisor | ¿Cómo sé que la afirmación es cierta? | Una línea fuente, una salida de comando o un documento nombrado. |

El README es para el primer lector. La especificación es para el tercero. El registro de evidencias
es para el segundo. Un solo documento no puede servir bien a los tres, y por eso esta lección
produce dos artefactos y un registro.

### README, especificación, prueba y comentario son contratos distintos

| Artefacto | Pregunta que responde | Modo de fallo |
|---|---|---|
| README | ¿Cómo compilo, ejecuto y pruebo esto? | Comandos que nunca se ejecutaron. |
| Especificación | ¿Qué debe ocurrir? | Reglas que describen el código en vez del comportamiento. |
| Prueba | ¿Cómo sé que sigue ocurriendo? | Una prueba que comprueba la implementación, no el contrato. |
| Comentario | ¿Por qué esta línea tiene esta forma? | Un comentario que narra o miente. |

El README puede enlazar a la especificación. La especificación puede enlazar a la prueba. Ninguno
puede pretender ser el otro. Un README que contiene una especificación completa se queda obsoleto
en dos sitios a la vez. Una especificación que contiene comandos de compilación no se puede leer
como un contrato.

### Un comando exacto es un contrato

Un comando es exacto cuando quien lee puede pegarlo y obtener el mismo resultado. Eso exige seis
cosas:

| Elemento | Pregunta | Ejemplo |
|---|---|---|
| Herramienta | ¿Qué programa se ejecuta? | `c++`, no "el compilador". |
| Versión | ¿Qué dialecto? | `-std=c++20`. |
| Banderas | ¿Qué significados cambian? | `-Dmain=legacy_main`. |
| Rutas | ¿Desde dónde? | `$ARCHIVE/...`, desde la raíz del laboratorio. |
| Entrada | ¿Qué entra? | `< /dev/null`. |
| Resultado esperado | ¿Qué sale? | Estado 0, dos ficheros, una línea. |

"Lo compilamos en Linux" es un recuerdo. Los seis elementos son una instrucción.

La misma regla se aplica a la ejecución y a la prueba. Un comando de ejecución sin su redirección
de entrada está incompleto: el programa puede esperar una tecla que nunca llega. Un comando de
prueba sin su resultado esperado es una sugerencia.

### Una afirmación sin fuente es una opinión

Cada frase del README y de la especificación es de uno de tres tipos:

| Tipo | Fuente | Ejemplo |
|---|---|---|
| Verificada en una línea | Fichero y número de línea | "El tablero tiene 64 casillas (`v01:17`)." |
| Observada en un comando | Comando y su salida | "La fuente tiene 930 líneas (`wc -l`)." |
| Documentada en otro sitio | Documento nombrado | "La cadena de herramientas histórica se nombra en `04-build-status.md`." |

Si una frase no tiene ninguna de las tres, es una opinión. Muévela a los desconocidos o bórrala.

La marca de confianza es la otra mitad de la disciplina. Una cláusula está **verificada** cuando la
línea fuente o la salida del comando lo muestra. Está **inferida** cuando la evidencia la apoya pero
no la demuestra. Está **desconocida** cuando falta la evidencia. Las tres marcas no son adorno;
dicen al siguiente mantenedor cuánto debe confiar en la frase.

### Los desconocidos son contenido

Un documento que esconde un bloqueo es peor que uno que lo nombra. Quien lee descubrirá el bloqueo
de todos modos, después de perder una tarde. Nombrarlo cuesta una línea y ahorra la tarde.

Un desconocido útil tiene tres partes:

1. la pregunta;
2. por qué le importa a un mantenedor;
3. qué resolvería la pregunta.

"La salida es rara" no es un desconocido. "El número de líneas de salida varía entre ejecuciones
porque la fuente reinicia la semilla desde el reloj (`v01:76`); una semilla fija lo resolvería, pero
la fuente no tiene opción de semilla" sí lo es.

### La prueba de alcance

Un documento para mantenedores responde cuatro preguntas: cómo lo compilo, cómo lo ejecuto, cómo lo
pruebo y qué debe ser el comportamiento. Si una frase no ayuda a responder una de esas cuatro,
pertenece a otro registro.

Esta prueba es más estricta de lo que parece. Elimina la historia, la motivación, la propiedad, las
notas editoriales y todo lo que trata del contexto del artefacto en lugar de su uso. Esos hechos
pueden importar en otro sitio, pero no ayudan a un mantenedor a compilar un fichero. Dejarlos fuera
no es censura; es disciplina de audiencia.

### Qué contiene realmente el archivo fijado

El archivo no está indocumentado. Está subdocumentado de una forma concreta.

| Documento | Pregunta que responde | Qué no responde |
|---|---|---|
| `README.md` | Qué es el archivo y qué variantes existen. | Cómo compilar ninguna variante. |
| `original/variant-01-baseline-2008/README.md` | Dónde está la fuente y qué mecanismo usa. | Cuáles son las reglas ni cómo ejecutarla. |
| `04-build-status.md` | Qué cadena de herramientas histórica se usó y por qué falla una compilación moderna. | Un comando moderno exacto. |
| `documentation/en/01-technical-notes.md` | Qué hace cada función, en prosa. | Un contrato de comportamiento con citas. |
| `05-known-issues.md` | Qué defectos se conocen. | Un plan de prueba o una condición de éxito. |
| el fichero fuente | La implementación. | La especificación. |

El hueco no es "falta prosa". El hueco es "faltan instrucciones ejecutables y falta un
contrato". Ese es el hueco que llena esta lección.

## Antes de empezar

Lee estos documentos completos:

- [`README-es.md`](README-es.md) — la ficha de la lección y el criterio de aceptación.
- [`README-TEMPLATE-es.md`](README-TEMPLATE-es.md) — el README para mantenedores que vas a
  completar.
- [`SPEC-TEMPLATE-es.md`](SPEC-TEMPLATE-es.md) — la especificación de comportamiento que vas a
  completar.
- [archaeology-03](../archaeology-03-reverse-spec/lesson-es.md) — el prerrequisito: cómo reconstruir
  una especificación a partir de líneas fuente.
- [archaeology-01](../archaeology-01-build-today/lesson-es.md) — la sonda de compatibilidad cuyas
  cabeceras y envoltorio reutiliza esta lección.
- [`../../../docs/archive-es.md`](../../../docs/archive-es.md) — la versión fijada del archivo.
- [`../../../common/spec/game-rules-es.md`](../../../common/spec/game-rules-es.md) — la
  especificación moderna, usada solo como objetivo de comparación, nunca como evidencia de la
  fuente heredada.

También necesitas:

- un clon local del archivo en la versión fijada;
- un compilador de C++20 compatible con GCC (`c++` o `g++`);
- `bash`, `timeout`, `wc`, `grep` y `cp`;
- un directorio de trabajo fuera del archivo y fuera de las plantillas de la lección.

> **Regla:** el archivo es de solo lectura. Copia la fuente a un directorio de trabajo; nunca edites
> `original/`. Las plantillas de la lección también son entradas: cópialas a tu directorio de
> documentación antes de completarlas.

Define dos variables una vez por shell:

```bash
export ARCHIVE=/path/to/guerra-de-caballos-archive
export DOCBENCH=/tmp/guerra-doc-bench
```

`ARCHIVE` apunta al clon fijado. `DOCBENCH` es el directorio de trabajo para la copia de la fuente,
las cabeceras de compatibilidad y los artefactos de compilación. Todos los comandos siguientes usan
esos dos nombres.

## El camino de 60 minutos

| Min | Paso | Qué haces | Evidencia |
|---|---|---|---|
| 0–8 | Inspeccionar | Inventariar los documentos existentes y leer los anclajes de la fuente | Tabla de inventario |
| 8–23 | Escribir | Completar la plantilla del README, incluidos los comandos exactos | Borrador del README |
| 23–35 | Escribir | Completar la especificación con citas y marcas | Borrador de la especificación |
| 35–43 | Ejecutar | Ejecutar cada comando documentado desde un directorio limpio | Registro de ejecución |
| 43–52 | Verificar | Auditar cada comando y cada afirmación | Tablas de auditoría |
| 52–57 | Explicar | Declarar el coste y los límites de la documentación | Explicación |
| 57–60 | Registrar | Completar `EVIDENCE-es.md` y comprobar el archivo | Entrada de portafolio |

Si la compilación tarda diez minutos porque las cabeceras de compatibilidad te sorprenden, esa
investigación es la lección. No te saltes la ejecución para ahorrar tiempo; un comando no ejecutado
es una suposición.

### La comprobación de ingeniería en esta lección

| Paso | Dónde ocurre | Qué produce |
|---|---|---|
| **Inspeccionar** | Paso 1 | El inventario de documentación y los anclajes de la fuente. |
| **Escribir** | Pasos 2–3 | Un README y una especificación de comportamiento. |
| **Ejecutar** | Paso 4 | Un registro de comandos con estado de salida y salida observada. |
| **Verificar** | Paso 5 | Un veredicto por comando y por afirmación. |
| **Explicar** | Paso 6 | El coste del hueco y qué no arreglan los documentos. |
| **Registrar** | Paso 7 | `EVIDENCE-es.md` con comandos, fuentes y desconocidos. |

## Paso 1 — Inspeccionar (8 min)

### Inventariar lo que existe

Desde la raíz del laboratorio, con `$ARCHIVE` definido:

```bash
wc -l "$ARCHIVE/README.md" \
    "$ARCHIVE/original/variant-01-baseline-2008/README.md" \
    "$ARCHIVE/04-build-status.md"
grep -n "build\|compile\|g++\|cl.exe\|main" \
    "$ARCHIVE/README.md" "$ARCHIVE/04-build-status.md" | head -20
```

El primer comando da el tamaño de cada documento. El segundo muestra qué palabras de compilación
aparecen. Lee el resultado como mantenedor: ¿alguna línea contiene un comando que puedas pegar? En
el archivo fijado, la respuesta es no. El documento de compilación nombra una cadena de
herramientas histórica y enumera bloqueos modernos; no da un comando.

Completa la tabla de inventario en `EVIDENCE-es.md` con una fila por documento: ruta, pregunta que
responde, pregunta que deja abierta.

### Leer los anclajes de la fuente

La fuente tiene 930 líneas. No necesitas leerlas todas para documentarla; necesitas los anclajes que
la especificación va a citar.

```bash
wc -l "$ARCHIVE/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp"
file "$ARCHIVE/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp"
grep -c '//' \
    "$ARCHIVE/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp"
grep -n "void  main\|#include\|srand\|rand()\|while (jugada\|ganador=0\|quedo encerrado" \
    "$ARCHIVE/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp"
```

Registra la salida exacta. En la fuente fijada:

- `wc -l` informa `930`;
- `file` informa `C source, ASCII text`;
- `grep -c '//'` informa `119`;
- los anclajes aparecen en las líneas usadas a lo largo de esta lección.

No confíes en estos números porque los diga la lección. Ejecuta los comandos y escribe lo que
imprimió tu máquina. Un número copiado de una lección no es evidencia; es una cita.

### Elegir el alcance

Escribe una frase antes de escribir el README:

> Este documento cubre `<ruta de la fuente>` en `<versión>`, compilada con `<compilador>` en
> `<plataforma>`, para un mantenedor que necesita compilarla, ejecutarla y probarla con una prueba
> de humo.

Si la frase necesita dos fuentes, dos compiladores o dos plataformas, recórtala. Un documento que
cubre todo no cubre nada con exactitud.

## Paso 2 — Escribir el README (15 min)

Desde la raíz del laboratorio, copia la plantilla al directorio de documentación:

```bash
mkdir -p "$DOCBENCH/docs"
cp archaeology/lessons/archaeology-13-documentation/README-TEMPLATE-es.md \
    "$DOCBENCH/docs/README.md"
```

Completa cada sección. La plantilla es una lista de comprobación, no una sugerencia.

### Qué es

Tres viñetas, sin historia:

- **Propósito:** qué hace el artefacto, en una frase.
- **Alcance:** qué fuente, qué versión, qué plataforma.
- **Qué no es:** la suposición equivocada más cercana. "No es un port", "no es determinista", "no
  es el formato de transcripción moderno".

La línea "qué no es" es la más valiosa. Evita que quien lee espere una propiedad que el artefacto
nunca tuvo.

### Compilar

La sección de compilación tiene cuatro partes: dependencias, comandos, banderas y resultado
esperado.

Empieza por la capa de compatibilidad. La fuente incluye `conio.h` e `iostream.h` (`v01:11-12`), y
define `void main()` (`v01:32`). Las cabeceras y el envoltorio vienen de `archaeology-01`; cópialos
al directorio de trabajo para que la compilación no dependa del árbol de la lección en tiempo de
compilación. Ejecuta estos comandos desde la raíz del laboratorio.

```bash
mkdir -p "$DOCBENCH/compat"
cp "$ARCHIVE/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp" \
    "$DOCBENCH/legacy.cpp"
cp archaeology/lessons/archaeology-01-build-today/code/compat/conio.h \
   archaeology/lessons/archaeology-01-build-today/code/compat/iostream.h \
   "$DOCBENCH/compat/"
cp archaeology/lessons/archaeology-01-build-today/code/legacy_wrapper.cpp \
   "$DOCBENCH/legacy_wrapper.cpp"
```

Después, los dos comandos de compilación:

```bash
c++ -std=c++20 -fpermissive -Dmain=legacy_main -I"$DOCBENCH/compat" \
    -c "$DOCBENCH/legacy.cpp" -o "$DOCBENCH/legacy.o"
c++ -std=c++20 "$DOCBENCH/legacy_wrapper.cpp" "$DOCBENCH/legacy.o" \
    -o "$DOCBENCH/legacy-probe"
```

Cada bandera tiene un motivo, y el README debe darlo:

| Bandera | Motivo | Riesgo |
|---|---|---|
| `-std=c++20` | El dialecto del compilador usado en la compilación registrada. | La fuente es más antigua; el dialecto es una elección, no una afirmación. |
| `-fpermissive` | La sonda la pasa por el dialecto histórico. | Puede rebajar diagnósticos; no es una garantía de corrección. |
| `-Dmain=legacy_main` | Renombra el punto de entrada heredado para que el envoltorio lo llame. | La fuente se compila con una macro que cambia un nombre. |
| `-I"$DOCBENCH/compat"` | Resuelve `conio.h` e `iostream.h` a las adaptaciones. | Las adaptaciones no son las cabeceras históricas. |
| `-c` | Produce un fichero objeto; el envoltorio aporta `main`. | Ninguno; es una etapa de compilación. |

El README también declara el resultado esperado: existen `legacy.o` y `legacy-probe`, y ambos
comandos salen con 0. Declara el bloqueo conocido: una compilación ingenua falla en `conio.h` porque
la cadena de herramientas moderna no lo incluye.

No escribas "instala las dependencias" sin nombrarlas. Aquí las dependencias son un fichero fuente,
dos cabeceras y un envoltorio, y los comandos copian los cuatro.

### Ejecutar

El comando de ejecución debe incluir la redirección de entrada:

```bash
timeout 3s "$DOCBENCH/legacy-probe" < /dev/null \
    > "$DOCBENCH/stdout.txt" 2> "$DOCBENCH/stderr.txt"
printf 'run_status=%s\n' "$?"
wc -l "$DOCBENCH/stdout.txt" "$DOCBENCH/stderr.txt"
```

Por qué existe cada parte:

- `timeout 3s` detiene un programa que espera una tecla. El `getch()` de compatibilidad no hace
  nada, pero el timeout es la protección que hace seguro el comando en cualquier máquina.
- `< /dev/null` hace explícita la entrada. Quien lee no debería adivinar si el programa lee stdin.
- las dos redirecciones separan la transcripción de los comandos de consola fallidos.
- `printf` registra el estado de salida. Un estado `124` significa que el timeout se disparó; es
  dato, no una caída.
- `wc -l` registra el volumen. El volumen varía porque la fuente reinicia la semilla desde el reloj
  (`v01:76`).

El README declara el control de semilla: ninguno. La fuente llama a `srand(time(NULL))` dentro del
bucle de reintento de colocación, así que dos ejecuciones no son comparables. Es un hecho que un
mantenedor debe conocer antes de intentar reproducir un fallo.

### Probar

No hay trazas doradas para esta fuente. La sección de prueba lo dice y luego da la prueba más fuerte
que existe:

```bash
grep -q 'Juega primero el caballo' "$DOCBENCH/stdout.txt" \
  && echo 'smoke=pass' || echo 'smoke=fail'
```

La prueba comprueba que el programa llegó al anuncio del orden de inicio. No comprueba las reglas,
el ganador, el tablero ni la codificación. El README declara ese límite en una línea: "Qué no
cubre: determinismo, corrección de reglas, caminos de captura y trampa, codificación."

Una prueba débil documentada es mejor que una prueba fuerte que no existe. Quien lee sabe
exactamente qué demuestra el comando.

### Especificación y evidencia

El README apunta a `SPEC.md` y declara qué cubre y qué no cubre la especificación. La sección de
evidencia registra la ruta de la fuente, los comandos ejecutados, los resultados y los desconocidos.
No repite la especificación.

### Frases débiles y frases fuertes

| Débil | Fuerte | Por qué |
|---|---|---|
| "Compila con el compilador original." | Los dos comandos `c++` de arriba, con banderas y ficheros esperados. | Quien lee puede ejecutar el segundo. |
| "El programa imprime el tablero." | "Ocho filas de ocho casillas, con los bytes `0x15`, `0x9C`, `0xDB` (`v01:16`, `v01:870-928`)." | La afirmación es observable y está citada. |
| "El juego es aleatorio." | "`srand(time(NULL))` en `v01:76`; no hay opción de semilla; dos ejecuciones difieren." | El mecanismo está nombrado. |
| "Las pruebas pasan." | El comando de humo, su salida y su límite de cobertura. | Quien lee puede reproducir el veredicto. |
| "Mira los documentos." | Un enlace relativo a `SPEC.md` y el nombre de la sección. | Quien lee llega a la afirmación. |

## Paso 3 — Escribir la especificación de comportamiento (12 min)

Desde la raíz del laboratorio, copia la plantilla:

```bash
cp archaeology/lessons/archaeology-13-documentation/SPEC-TEMPLATE-es.md \
    "$DOCBENCH/docs/SPEC.md"
```

La especificación no es un recorrido por las funciones. Es un contrato expresado en términos
observables. Cada cláusula tiene cuatro partes: la regla, las líneas fuente, la marca de confianza y
el límite.

### Una tabla de cláusulas resuelta

| Cláusula | Texto de la especificación | Fuente | Marca |
|---|---|---|---|
| Tablero | 8 filas por 8 columnas, guardado como 64 enteros. | `v01:17`, `v01:226-229` | verificada |
| Piezas | Dos caballos, uno por jugador; ninguna otra pieza. | `v01:18`, `v01:81-82` | verificada |
| Casillas iniciales | Dos casillas distintas, cada una `rand() % 64`; se redibuja mientras sean iguales. | `v01:81-83` | verificada |
| Orden de inicio | Cada caballo tira `rand() % 51`; se redibuja mientras sean iguales; empieza el mayor. | `v01:78-83`, `v01:730-748` | verificada |
| Orden de turnos | Quien empieza mueve primero; los turnos alternan hasta que termina la partida. | `v01:753-868` | verificada |
| Orden de movimientos | Cada turno sortea `rand() % 8 + 1` y evalúa los ocho movimientos en L en esa rotación. | `v01:176-205` | verificada |
| Movimiento legal | El destino está dentro del tablero y vacío u ocupado por el otro caballo. | `v01:245-252` | verificada |
| Captura | Moverse sobre el otro caballo pone `ganador=0`; gana quien mueve y termina la partida. | `v01:254-257`, `v01:768-772` | verificada |
| Trampa | Si fallan los ocho movimientos, `jugada=0` y termina la partida. | `v01:702-717` | verificada |
| Empate | No existe una cláusula de empate en la fuente. | `v01:44`; `grep -n empate` no devuelve nada | inferida |
| Salida | Texto de consola con bytes `0x15`, `0x9C`, `0xDB`; sin formato de transcripción. | `v01:16`, `v01:870-928` | verificada |
| Aleatoriedad | `srand(time(NULL))`; `rand()` para colocación, tiradas y orden de movimientos. | `v01:76`, `v01:78-82`, `v01:180` | verificada |

Lee dos veces la fila del empate. La ausencia de una rama es evidencia, pero "no hay empate" es una
inferencia del flujo de control completo. Márcala como inferida y di qué cambiaría la respuesta: un
camino que termine la partida sin captura ni trampa.

La fila de la salida necesita el mismo cuidado. La fuente nombra tres valores enteros (`v01:16`)
pero no una página de código. CP437 es la tabla de códigos que convierte `0x15`, `0x9C` y `0xDB` en
los glifos del tablero; una comprobación de bytes de la salida archivada registra la página de
código. Si no puedes reproducir esa comprobación, marca la página de código como inferida y cita
los valores de byte como verificados.

### Separar las tres capas

La especificación mantiene separadas las reglas, la aleatoriedad y la presentación. Quien copia la
implementación en la especificación ha escrito una descripción, no un contrato.

- **Reglas:** tablero, inicio, turnos, movimiento legal, captura, trampa.
- **Aleatoriedad:** generador, semilla, condiciones de redibujo, no reproducibilidad.
- **Presentación:** texto de consola, glifos de consola, disposición del tablero, cadenas en
  español.

La separación importa porque un cambio de presentación no debe parecer un cambio de reglas. Si el
tablero se redibuja en UTF-8, las reglas no cambian; la especificación debe hacerlo obvio.

### El límite de la especificación

La especificación cubre la fuente tal como está escrita. No cubre:

- qué hizo el compilador histórico con el comportamiento indefinido;
- qué imprimió el binario histórico privado;
- con qué frecuencia termina una partida por captura o por trampa;
- cómo renderiza los bytes una terminal CP437 en la máquina del mantenedor.

Esos son desconocidos, no huecos de redacción. Ponlos en la sección de desconocidos de la
especificación con un camino de resolución.

## Paso 4 — Ejecutar cada comando (8 min)

### Usar un directorio limpio

Abre un shell nuevo y define las dos variables otra vez. No confíes en tu historial, tu directorio
actual ni un fichero que creaste antes.

```bash
export ARCHIVE=/path/to/guerra-de-caballos-archive
export DOCBENCH=/tmp/guerra-doc-bench
rm -rf "$DOCBENCH/compat" "$DOCBENCH/legacy.cpp" "$DOCBENCH/legacy.o" \
       "$DOCBENCH/legacy-probe" "$DOCBENCH/legacy_wrapper.cpp" \
       "$DOCBENCH/stdout.txt" "$DOCBENCH/stderr.txt"
mkdir -p "$DOCBENCH/compat"
```

Copia la fuente y las cabeceras como dice el README, y pega los bloques del README en orden. El
objetivo no es demostrar que sabes escribir; es demostrar que el documento es autosuficiente.

### Registrar los cuatro resultados

Para cada bloque de comandos, registra:

| Campo | Por qué |
|---|---|
| Comando | El texto exacto, o una forma corta estable. |
| Directorio | Los comandos que dependen del directorio de trabajo no son portables. |
| Estado de salida | `0` es el camino esperado; cualquier otro es evidencia. |
| Salida observada | Las líneas que imprimió el comando, o los tamaños de fichero que produjo. |
| Salida esperada | Lo que prometió el README. |
| ¿Coincide? | Sí o no. Un "no" significa que el README está mal, no la máquina. |

En esta máquina, el bloque de compilación sale con `0` y produce `legacy.o` y `legacy-probe`. El
bloque de ejecución sale con `0`; stdout tiene cientos de líneas y el número varía entre
ejecuciones. La primera línea de stderr es el comando de consola de Windows fallido, citado
literalmente en el registro. La prueba de humo imprime `smoke=pass`.

Tus números serán distintos. Registra los tuyos. Una lección que envía un número y lo llama
universal hace lo mismo que esta lección advierte.

### Cuando un comando falla

No edites la transcripción. Edita el README. Las causas habituales son:

- falta el directorio `$DOCBENCH`;
- una ruta relativa que asumía la raíz del laboratorio;
- un compilador que no acepta `-fpermissive`;
- un timeout que se disparó antes de que el programa terminara;
- una configuración regional que cambia el texto de stderr.

Cada causa es una frase que falta en el README. Añade la frase, vuelve a ejecutar desde un
directorio limpio y registra ambos intentos.

## Paso 5 — Verificar cada afirmación (9 min)

### Auditoría de comandos

Ejecuta el README de arriba abajo en un shell nuevo. Marca cada bloque.

| Comprobación | Condición de éxito |
|---|---|
| Cada bloque `bash` se ejecuta | Ningún comando necesita una variable o un fichero no documentado. |
| Cada comando tiene un resultado esperado | El README dice cómo se ve el éxito. |
| Cada ruta está definida | `$ARCHIVE` y `$DOCBENCH` se definen antes del primer uso. |
| Cada entrada es explícita | stdin, stdout y stderr están nombrados. |
| Cada fallo está documentado | El bloqueo de la compilación ingenua y el estado del timeout están declarados. |

Un comando que solo funciona por un fichero que dejaste en tu directorio personal no está
documentado. Borra el fichero y vuelve a ejecutar.

### Probar el bloqueo documentado

El README dice que una compilación ingenua falla en `conio.h`. Demuéstralo:

```bash
c++ -std=c++20 -c "$DOCBENCH/legacy.cpp" -o "$DOCBENCH/naive.o" \
    2> "$DOCBENCH/naive.err" \
  && echo 'naive=unexpected-pass' || echo 'naive=fail-as-documented'
head -n 1 "$DOCBENCH/naive.err"
```

Esperado: `naive=fail-as-documented`, y la primera línea de stderr nombra `conio.h`. Si la
compilación tiene éxito, el bloqueo del README es falso para tu compilador; actualiza la frase y
registra el compilador.

### Auditoría de afirmaciones

Toma cada frase que afirme un hecho y dale una fuente.

| Afirmación | Fuente | Tipo | Veredicto |
|---|---|---|---|
| La fuente tiene 930 líneas. | salida de `wc -l` | comando | verificada |
| El tablero es de 8 por 8. | `v01:17`, `v01:226-229` | línea | verificada |
| El juego no tiene empate. | `v01:44`; `grep -n empate` no devuelve nada | inferencia | inferida |
| Los bytes de salida son `0x15`, `0x9C`, `0xDB`. | `v01:16` | línea | verificada |
| La página de código es CP437. | una comprobación de bytes de la salida archivada | inferencia | inferida |
| La cadena de herramientas histórica era MSVC 6.0. | `04-build-status.md` | documento | documentada, no verificada aquí |
| La prueba de humo cubre las reglas. | el comando de prueba | comando | falsa; cubre una subcadena |

La última fila es la útil. Una afirmación puede ser falsa aunque el comando pase. La prueba
comprueba que el programa llegó al anuncio de inicio; no comprueba las reglas.

### La comprobación de alcance

Lee cada frase y haz las cuatro preguntas de la prueba de alcance. Una forma rápida de detectar
desvíos es listar los encabezados y comprobar que cada uno sirve a compilación, ejecución, prueba o
comportamiento:

```bash
grep -n '^#' "$DOCBENCH/docs/README.md" "$DOCBENCH/docs/SPEC.md"
```

La salida es el esquema. Si un encabezado no sirve a una de las cuatro preguntas, muévelo a otro
registro. La misma prueba se aplica a una frase: un año en el nombre de una carpeta es un
identificador, pero un párrafo sobre cuándo ocurrió algo es contexto.

### La comprobación de reproducibilidad

Ejecuta la compilación y la prueba de humo dos veces. La compilación debe producir los mismos
artefactos. La prueba de humo debe pasar ambas veces. El número de líneas de stdout no debe ser
idéntico, porque la fuente no es determinista. Si el número es idéntico dos veces, no concluyas que
el programa es determinista; ejecuta una tercera vez y registra qué ocurrió.

## Paso 6 — Explicar el coste (5 min)

### Por qué falla la documentación

La mayoría de la documentación ausente no es pereza. Se escribe de memoria, después del trabajo,
por la persona que ya no la necesita. Eso produce tres fallos predecibles:

- el comando omite una bandera que el shell de quien escribe ya tenía;
- la especificación describe la estructura del código en vez de su comportamiento;
- el desconocido se omite porque quien escribe conoce la respuesta.

La solución es procesal, no moral: ejecuta los comandos desde un directorio limpio, cita cada
cláusula y escribe el desconocido antes de que se vuelva invisible.

### Qué cuesta el hueco

Un mantenedor que no puede compilar pierde la primera hora. Un mantenedor que compila con las
banderas equivocadas pierde el primer día. Un mantenedor que confía en una especificación sin citas
pierde el primer incidente.

El coste es bajo por lector y alto por relevo. Por eso la documentación es un fallo de práctica
profesional y no de corrección: nada se cae, y la organización paga en incorporaciones repetidas.

### Dos arreglos, dos tamaños

**Arreglo mínimo — este README y esta especificación.** Una fuente, una plataforma, un compilador,
comandos exactos, cláusulas citadas, desconocidos nombrados. Elimina el bloqueo inmediato y da al
siguiente lector un punto de partida.

**Arreglo estructural — hacer ejecutable la documentación.** Pon los comandos en un script que la
integración continua ejecute en un contenedor limpio. Haz que la especificación sea la autoridad y
enlázala desde la fuente. Añade una prueba que compare la salida documentada con la observada.
Enlaces a Track A: `common-04` (compilar y ejecutar), `common-05` (trazas doradas) y
`construction-20` (el límite donde se adapta la presentación).

### Qué no arregla esta documentación

- No hace determinista el programa. `srand(time(NULL))` sigue ahí.
- No añade una prueba de regresión. La prueba de humo comprueba una subcadena.
- No hace portable la fuente. Las cabeceras de compatibilidad son una adaptación.
- No demuestra que las reglas sean correctas. Registra lo que hace la fuente.
- No reemplaza el código. Un README no arregla un off-by-one.

La documentación abarata el siguiente cambio. No hace correcto el código actual.

## Paso 7 — Registrar (3 min)

Completa [`EVIDENCE-es.md`](EVIDENCE-es.md):

- entorno: ruta del archivo, compilador, shell, directorios de trabajo;
- el inventario de documentación;
- los anclajes de la fuente con números de línea;
- las decisiones del README, incluidos los comandos y las banderas;
- las cláusulas de la especificación con citas y marcas de confianza;
- el registro de comandos con estado de salida y salida observada;
- la auditoría de afirmaciones;
- los desconocidos y sus caminos de resolución;
- la comprobación de integridad del archivo.

Una frase como "el README es claro" es una opinión. "Dos comandos, ambos salen con 0, existen
`legacy.o` y `legacy-probe`, la prueba de humo imprime `smoke=pass`, la especificación tiene 12
cláusulas y dos cláusulas inferidas (empate y página de código)" es un registro.

## El original en detalle

Hechos que puedes citar, con números de línea de `variant-01` en la versión fijada:

| Ubicación | Qué hay |
|---:|---|
| 10–14 | Las cinco líneas `#include`, incluidas `conio.h` e `iostream.h`. |
| 16 | `simcab1=21`, `simcab2=156`, `sep=219`: valores enteros; la fuente no nombra la página de código. |
| 17 | `vectab[64]`, `vecjugadas[8]`: el tablero y el orden de movimientos. |
| 18 | `cab1`, `cab2`, `jugada`, `orden`, `ganador`: el estado mutable. |
| 32 | `void main()`: no es C++ estándar. |
| 38, 43 | `system("color 1f")`, `system("cls")`: llamadas a la consola de Windows. |
| 42 | `getch()`: la única entrada. |
| 44 | `while (jugada!=0)`: el bucle de juego y el centinela de fin. |
| 61 | `for (i=0;i<=64;i++)`: una escritura más allá del tablero de 64 elementos. |
| 76 | `srand(time(NULL))`: semilla reiniciada dentro del bucle de reintento de colocación. |
| 78–83 | Tiradas `rand() % 51` y casillas `rand() % 64`; redibujo en colisión. |
| 104–112 | `conversionvm`: índice de casilla a fila y columna. |
| 176–205 | `vecjug`: una rotación aleatoria de los ocho casos de movimiento. |
| 226–229 | `conversionmv`: `cf * 8 + cc`. |
| 234–724 | `movcab`: ocho casos copiados a mano y la comprobación de trampa. |
| 254–257 | La captura pone `ganador=0`. |
| 702–717 | Fallan los ocho casos: `jugada=0`, mensaje de caballo encerrado. |
| 726–751 | `ordenjuego`: empieza quien saca la tirada mayor. |
| 753–868 | `impresionorden`: turnos, anuncio de captura, redibujo del tablero. |
| 870–928 | `impresion`: el tablero de 8 por 8, con los tres valores enteros. |
| 930 | La llave de cierre; el fichero termina aquí; sin pruebas, sin fichero de compilación. |

La fuente tiene 930 líneas y 119 contienen `//`. Eso es una superficie de documentación, no un
contrato de comportamiento: como documenta `archaeology-12`, los comentarios narran, explican o
mienten, y ninguno se ejecuta.

## Un README resuelto

Esta es la forma que pide la plantilla, completada para `variant-01`. Es un ejemplo, no un
reemplazo de tu ejecución: tu compilador, tus rutas y tu volumen de salida van en tu copia.

````markdown
# Guerra de Caballos — baseline

> Compila, ejecuta y prueba con humo la línea base archivada mediante un envoltorio de
> compatibilidad.

## Qué es

- Propósito: estudiar el comportamiento del programa de consola de dos caballos archivado en un
  anfitrión Linux moderno.
- Alcance: un fichero fuente, un compilador, una consola; la fuente no se edita.
- Qué no es: no es una adaptación a otra plataforma, no es una rama de corrección de errores, no es
  determinista, no es el formato de transcripción moderno.

## Compilar

Ejecuta desde la raíz del laboratorio. Define la ruta del archivo y un directorio de trabajo:

```bash
export ARCHIVE=/path/to/guerra-de-caballos-archive
export DOCBENCH=/tmp/guerra-doc-bench
```

Prepara la capa de compatibilidad y la copia de la fuente:

```bash
mkdir -p "$DOCBENCH/compat"
cp "$ARCHIVE/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp" \
    "$DOCBENCH/legacy.cpp"
cp archaeology/lessons/archaeology-01-build-today/code/compat/conio.h \
   archaeology/lessons/archaeology-01-build-today/code/compat/iostream.h \
   "$DOCBENCH/compat/"
cp archaeology/lessons/archaeology-01-build-today/code/legacy_wrapper.cpp \
   "$DOCBENCH/legacy_wrapper.cpp"
```

Compila:

```bash
c++ -std=c++20 -fpermissive -Dmain=legacy_main -I"$DOCBENCH/compat" \
    -c "$DOCBENCH/legacy.cpp" -o "$DOCBENCH/legacy.o"
c++ -std=c++20 "$DOCBENCH/legacy_wrapper.cpp" "$DOCBENCH/legacy.o" \
    -o "$DOCBENCH/legacy-probe"
```

- Cadena de herramientas: `c++`; registra `c++ --version`.
- Dependencias: dos cabeceras de compatibilidad y un envoltorio, copiados arriba.
- Resultado esperado: existen `legacy.o` y `legacy-probe`; ambos comandos salen con 0.
- Bloqueos conocidos: una compilación ingenua falla en `conio.h`; la fuente usa `void main`,
  `iostream.h` y llamadas a la consola de Windows (`v01:12`, `v01:32`, `v01:38`, `v01:43`).

## Ejecutar

```bash
timeout 3s "$DOCBENCH/legacy-probe" < /dev/null \
    > "$DOCBENCH/stdout.txt" 2> "$DOCBENCH/stderr.txt"
printf 'run_status=%s\n' "$?"
wc -l "$DOCBENCH/stdout.txt" "$DOCBENCH/stderr.txt"
```

- Entrada: ninguna; stdin es `/dev/null`, y `getch()` no hace nada en la cabecera de
  compatibilidad.
- Salida: stdout es la transcripción de consola; stderr contiene los comandos de consola fallidos.
- Control de semilla: ninguno. `srand(time(NULL))` en `v01:76` reinicia la semilla en cada intento
  de colocación.
- Resultado esperado: `run_status=0`; stdout tiene al menos una línea; el número de líneas varía
  entre ejecuciones.

## Probar

```bash
grep -q 'Juega primero el caballo' "$DOCBENCH/stdout.txt" \
  && echo 'smoke=pass' || echo 'smoke=fail'
```

- Trazas doradas: ninguna.
- Método de comparación: una prueba de humo por subcadena para la línea del orden de inicio.
- Qué no cubre: determinismo, corrección de reglas, caminos de captura y trampa, codificación.

## Especificación de comportamiento

- Fichero de especificación: `SPEC.md`.
- Qué cubre la especificación: tablero, inicio, orden de turnos, movimiento legal, captura,
  trampa, aleatoriedad, salida.
- Qué no cubre: el compilador histórico, el comportamiento indefinido, el volumen exacto de salida.

## Evidencia

- Ruta de la fuente:
  `$ARCHIVE/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`.
- Comandos ejecutados: los bloques de compilación, ejecución y prueba de arriba, en un shell limpio.
- Resultados: ambos comandos de compilación salen con 0; la ejecución sale con 0; la prueba de humo
  imprime `smoke=pass`.
- Qué sigue desconocido: la salida exacta del binario histórico, la distribución de duración de las
  partidas, el renderizado CP437 en otras terminales.
````

## Qué demuestra la documentación y qué no

| Afirmación | ¿Soportada? | Por qué |
|---|---|---|
| Los comandos se ejecutan en la máquina registrada. | Sí | El registro tiene estado de salida y salida. |
| Los comandos se ejecutan en cualquier máquina. | No | Solo se probó un compilador y una plataforma. |
| La especificación coincide con las líneas citadas. | Sí | Cada cláusula nombra sus líneas. |
| La especificación está completa. | No | La sección de desconocidos nombra lo que falta. |
| La prueba de humo detecta regresiones. | No | Comprueba una subcadena. |
| El programa es determinista. | No | `srand(time(NULL))` en `v01:76`. |
| El comportamiento es correcto. | No se aborda | La especificación registra comportamiento, no corrección. |
| La cadena de herramientas histórica era MSVC 6.0. | Solo documentada | `04-build-status.md` lo dice; esta lección no lo verificó. |
| La documentación está lista para mantenedores. | En parte | Quien revisa aún debe juzgar la prosa. |

Esta tabla es la comprobación de honestidad. Un README que afirma "totalmente documentado" después
de una prueba de humo se excede. Di "para esta fuente, este compilador y esta prueba" cuando eso es
lo que ejecutaste.

## Autocomprobación (binaria)

- [ ] Inventarié los documentos existentes y nombré el hueco.
- [ ] Escribí la frase de alcance antes de redactar el README.
- [ ] Completé cada sección de `README-TEMPLATE-es.md`.
- [ ] La sección de compilación nombra la cadena de herramientas, las banderas, las dependencias y
      los ficheros esperados.
- [ ] La sección de ejecución nombra la entrada, la salida, el control de semilla y el estado
      esperado.
- [ ] La sección de prueba nombra el comando y qué no cubre.
- [ ] Completé `SPEC-TEMPLATE-es.md` con una cláusula por regla.
- [ ] Cada cláusula de la especificación cita líneas fuente y lleva una marca de confianza.
- [ ] Las reglas, la aleatoriedad y la presentación están separadas.
- [ ] Ejecuté cada comando del README desde un directorio limpio.
- [ ] Registré el estado de salida y la salida observada de cada bloque.
- [ ] Audité cada afirmación contra una línea, un comando o un documento.
- [ ] Declaré los desconocidos con un camino de resolución.
- [ ] La comprobación de alcance pasa.
- [ ] El archivo está sin modificar (`git status` limpio).
- [ ] `EVIDENCE-es.md` está completo.

Si una casilla no está marcada, la casilla sin marcar nombra el siguiente paso. No hay penalización
por repetir uno.

## Misconcepciones que esta lección refuta

| Concepto erróneo | Por qué es falso | Qué hacer en su lugar |
|---|---|---|
| "La documentación es prosa sobre el código." | La prosa que no se puede ejecutar ni comprobar es un recuerdo. | Escribe comandos y cita cláusulas. |
| "El README debe explicar la historia." | Un mantenedor necesita compilar, ejecutar y probar primero. | Mantén el contexto del artefacto fuera del camino de compilación. |
| "Un comando es exacto si funcionó una vez." | Puede depender de una variable, un directorio o un fichero. | Vuelve a ejecutarlo desde un directorio limpio y nombra cada entrada. |
| "La especificación puede copiar los nombres de funciones." | Los nombres son implementación; el contrato es comportamiento. | Declara lo que ve un observador y cita las líneas. |
| "Si la fuente no tiene rama de empate, no hay empate." | Es una inferencia del flujo de control. | Márcala como inferida y di qué cambiaría la respuesta. |
| "Una prueba de humo que pasa cubre las reglas." | La prueba comprueba una subcadena. | Declara el límite de cobertura junto al comando. |
| "Los desconocidos hacen ver débil el documento." | Los desconocidos escondidos hacen perder una tarde. | Nombra el desconocido y el camino de resolución. |
| "Un README sirve para mantenedores e historiadores." | Las audiencias hacen preguntas distintas. | Escribe el README para mantenedores; deja el contexto en otro sitio. |
| "El README del archivo ya documenta la compilación." | Nombra la cadena de herramientas y los bloqueos; no da un comando. | Añade el comando que ejecutaste. |
| "Una adaptación de compatibilidad es un port." | La fuente sigue asumiendo el dialecto y la consola antiguos. | Di "adaptación" y enumera qué cambia. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| README para mantenedores | El documento que dice cómo compilar, ejecutar y probar. |
| Especificación de comportamiento | Un contrato expresado en términos observables, con citas. |
| Marca de confianza | Verificada, inferida o desconocida, adjunta a una cláusula. |
| Contrato de comando | Herramienta, versión, banderas, rutas, entrada y resultado esperado. |
| Ejecución en directorio limpio | Ejecutar el documento sin depender del estado previo del shell. |
| Prueba de humo | Una comprobación pequeña de que el artefacto arranca y produce una marca conocida. |
| Traza dorada | Una salida registrada usada como referencia para ejecuciones futuras. |
| Adaptación de compatibilidad | Una cabecera o envoltorio que aporta lo que el entorno antiguo asumía. |
| Comportamiento indefinido | Una construcción cuyo resultado el lenguaje no define. |
| Frase de alcance | Una frase que nombra la fuente, la versión, el compilador y la audiencia. |
| Límite de evidencia | La línea entre lo que se ejecutó y lo que se asumió. |
| Deuda de documentación | El coste recurrente de un documento que no se puede ejecutar ni confiar. |

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
