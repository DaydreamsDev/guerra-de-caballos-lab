# archaeology-04 — Fallos de corrección

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** fallos de corrección · **Prerrequisito:** archaeology-03 ·
> **Tiempo estimado:** 100 minutos
> **Lectura:** ~16 minutos

## Por qué existe esta lección

`archaeology-03` reconstruyó el comportamiento del programa archivado y capturó una traza. Describió
lo que el código hace. Esta lección plantea la pregunta siguiente: cuáles de esos comportamientos
son defectos, cuánto cuestan y qué prueba los habría detectado.

El archivo conserva seis fallos de corrección. Son evidencia, no una lista de tareas. El producto
de esta lección son seis tarjetas y una ejecución de pruebas, no un archivo parcheado. Esa
distinción es el centro de la lección: el archivo registra el defecto, la línea y la consecuencia;
una reparación pertenece a una adaptación etiquetada o al laboratorio.

Los seis fallos son lo bastante pequeños para leerse de una sentada y lo bastante distintos para
enseñar seis tipos de razonamiento: un error de límite, un punto de entrada no estándar, una semilla
dentro de un bucle de reintento, un reinicio de puntero, una lectura sin límite y una condición de
victoria invertida. Cada uno tiene un síntoma fácil de ver y una causa raíz que no lo es.

## Qué podrás hacer

Al terminar esta lección puedes:

- definir un fallo de corrección y distinguirlo de un fallo de diseño, de robustez o de práctica
  profesional;
- separar un síntoma de una causa raíz;
- escribir una tarjeta de fallo con evidencia, consecuencia, prueba, arreglo mínimo y arreglo
  estructural;
- ejecutar el arnés de fallos contra el archivo y leer las seis líneas `detected`;
- explicar qué comprueba cada prueba y qué no comprueba;
- elegir un arreglo mínimo antes de un arreglo estructural;
- vincular un arreglo estructural a un peldaño de la pista A sin convertir la lección en una
  reescritura;
- ordenar seis fallos por coste en un código real;
- mantener el archivo de solo lectura mientras demuestras una reparación en una copia de trabajo.

## El concepto: fallo de corrección

Un **fallo de corrección** es un comportamiento que contradice la afirmación del programa o un
contrato declarado. El programa puede compilar, ejecutarse e imprimir un tablero. El resultado sigue
siendo incorrecto, indefinido o no reproducible. La corrección se trata del resultado, no de la
elegancia del código.

La pista B separa cinco clases. La clase determina la lección, no el archivo.

| Clase | Pregunta | Ejemplo en este archivo |
|---|---|---|
| **Corrección** | ¿Hace el programa lo que afirma? | `i <= 64` sobre un arreglo de 64 elementos. |
| **Diseño** | ¿La estructura encarece el cambio? | Estado global mutable; ocho cuerpos `case` copiados. |
| **Robustez** | ¿Funciona fuera de un entorno? | `conio.h`, `system("color 1f")`. |
| **Práctica profesional** | ¿Otra persona puede compilarlo, usarlo o confiar en él? | Sin README, sin especificación, sin pruebas. |
| **Artefacto pedagógico** | ¿La lección enseña lo que afirma? | `variant-01` no contiene punteros. |

Las seis tarjetas de esta lección son fallos de corrección. Varias tienen también consecuencias de
robustez: un desbordamiento de búfer es un problema de seguridad, y `void main` es un problema de
portabilidad. Clasifica por la pregunta principal y anota el coste secundario.

### Síntoma vs causa raíz

El síntoma es lo que ve un observador. La causa raíz es la decisión que lo produjo. Una tarjeta que
se detiene en el síntoma es un informe de error; una tarjeta que llega a la causa raíz es un
diagnóstico.

| Fallo | Síntoma | Causa raíz |
|---|---|---|
| 001 | El bucle escribe `vectab[64]` en un arreglo de 64 elementos. | `<` se convirtió en `<=`; el límite es un segundo literal. |
| 002 | El punto de entrada devuelve `void`. | Una extensión del compilador se trató como la regla del lenguaje. |
| 003 | La colocación cambia entre ejecuciones; una semilla fija puede colgarse. | La semilla se reinicia dentro del bucle de reintento. |
| 004 | Solo se limpia la primera casilla del tablero. | El puntero se reinicia al elemento 0 en cada iteración. |
| 005 | Un nombre largo sobrescribe memoria después de `nombre`. | `cin >>` no tiene ancho y el tamaño del arreglo no está en la llamada. |
| 006 | La máquina gana cuando el caballo come los cuatro peones. | La condición de final y la etiqueta del ganador son independientes. |

La distinción importa porque los arreglos son distintos. Un parche en el síntoma, en la capa
equivocada, esconde el defecto; un arreglo en la causa raíz elimina la clase.

### El protocolo de la tarjeta

Cada fallo se documenta como una tarjeta. Las tarjetas de `cards/` usan los mismos nueve campos. Los
campos son obligatorios; una tarjeta sin evidencia es una opinión.

1. **Evidencia.** La variante, la línea y un fragmento del código. Cita el código, no un recuerdo
   del código. Si el mismo patrón aparece en varias variantes, lístalas.
2. **Síntoma.** Lo que ve un observador, en una o dos frases. Todavía sin diagnóstico.
3. **Causa raíz.** La decisión que produjo el síntoma. Es la frase que un revisor debería recordar.
4. **Consecuencia.** El coste en el trabajo real: comportamiento indefinido, un bloqueo, un
   resultado incorrecto, un hallazgo de seguridad, una prueba inestable, un puerto bloqueado. La
   severidad se trata de la consecuencia, no de lo feo que se ve el código.
5. **Prueba que lo detecta.** Nombra la prueba y di qué comprueba. Una prueba que ejecuta el patrón
   es más fuerte que una que busca texto; ambas sirven si declaras la diferencia.
6. **Arreglo mínimo.** El cambio más pequeño que elimina el daño. Un carácter, una llamada movida,
   una lectura acotada. No tiene que ser bonito.
7. **Arreglo estructural.** El cambio de diseño que previene la clase de fallo. Vincúlalo a un
   peldaño de la pista A para que se vea el concepto detrás del parche.
8. **Confianza.** Alta, media o baja, con la razón. El código puede ser seguro y la intención
   original no.
9. **Nota sobre la prueba.** Di que la prueba es una reproducción aislada, no una prueba del
   original.

El orden es un protocolo, no una preferencia. **Arreglo mínimo antes del arreglo estructural.** La
pista es explícita: nunca saltes del catálogo de fallos a la reescritura. El arreglo mínimo
demuestra que entiendes el daño y que se puede eliminar sin cambiar el diseño. El arreglo
estructural aborda después la clase, con un precio que puedes nombrar.

### Sin reparación en el archivo

El archivo está congelado en la etiqueta `v1.0.0`. `original/` es evidencia. No lo edites, no
apliques un parche dentro y no lo "arregles de paso".

- Una reparación va en una copia de trabajo, en una adaptación etiquetada o en el laboratorio.
- El arnés de pruebas no incluye el archivo; reproduce el patrón de forma aislada.
- Si editas el archivo, la evidencia desaparece y la siguiente persona no puede reproducir tu
  hallazgo.

No es burocracia. El defecto es el artefacto. Un archivo reparado documentaría la reparación, no el
programa de 2008.

## Antes de empezar

Lee esto completo:

- [`README.md`](README.md) — la ficha de la lección, el comando de pruebas y el criterio de
  aceptación.
- [`../archaeology-03-reverse-spec/lesson.md`](../archaeology-03-reverse-spec/lesson.md) — el
  prerrequisito: la especificación de comportamiento y la traza.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, la
  taxonomía de fallos, y §4, la plantilla de la tarjeta de fallo.
- [`../../../docs/archive.md`](../../../docs/archive.md) — el archivo fijado y su alcance.
- El `05-known-issues.md` del archivo — las seis filas que esta lección convierte en tarjetas.

También necesitas:

- un compilador de C++20 (`c++`, `g++` o `clang++`) y su versión;
- `bash` y `grep`;
- un clon local del archivo en la etiqueta `v1.0.0`;
- [`tests/failure_tests.cpp`](tests/failure_tests.cpp) y
  [`tests/run_failure_tests.sh`](tests/run_failure_tests.sh), que vienen con esta lección.

> **Regla:** el archivo es de solo lectura. El arnés lo lee con `grep`; nunca escribe en él.

## El camino de 90 minutos

| Min | Paso | Qué haces | Evidencia |
|---|---|---|---|
| 0–10 | Predecir | Escribe una frase por fallo antes de leer las tarjetas | Tabla de predicciones |
| 10–25 | Reproducir | Ejecuta el arnés; lee las seis líneas `detected` | Salida de pruebas |
| 25–55 | Diagnosticar | Lee las seis ubicaciones; separa síntoma de causa raíz | Seis tarjetas |
| 55–70 | Arreglar mínimo | Escribe el parche más pequeño para cada fallo | Tabla de arreglos mínimos |
| 70–80 | Arreglar estructural | Vincula cada clase a un peldaño; ordena por coste | Tabla estructural y orden |
| 80–90 | Registrar | Completa `EVIDENCE-es.md`; comprueba que el archivo está limpio | Entrada de portafolio |

Si un fallo tarda veinte minutos porque el flujo de control está
enredado, eso es información sobre el fallo, no sobre ti.

### La comprobación de ingeniería en esta lección

La pista B usa el mismo hábito que la pista A, con otro verbo: el programa ya está construido, así
que **reproduces** en lugar de construir.

| Paso | Dónde ocurre | Qué produce |
|---|---|---|
| **Predecir** | Paso 1 | Seis afirmaciones falsables sobre cómo se ve cada fallo. |
| **Reproducir** | Paso 2 | Las seis líneas `detected` y el comando que las produjo. |
| **Diagnosticar** | Paso 3 | Seis tarjetas con síntoma, causa raíz y consecuencia. |
| **Arreglar** | Paso 4 | Un parche mínimo por tarjeta, en una copia de trabajo. |
| **Probar** | Paso 4 | La prueba que detecta cada patrón, nombrada y explicada. |
| **Registrar** | Paso 5 | Las tarjetas, el orden y `EVIDENCE-es.md`. |

## Paso 1 — Predecir (10 min)

Antes de abrir las tarjetas, escribe una frase por fallo. Usa las ubicaciones del README y la tabla
de problemas conocidos, pero no leas todavía el diagnóstico.

Para cada uno, responde tres preguntas:

1. ¿Cómo se vería el código si este fallo estuviera presente?
2. ¿Qué vería un observador cuando se ejecute?
3. ¿Qué prueba lo detectaría sin necesitar el binario original?

Una predicción útil es falsable. "El bucle está mal" no lo es. "El bucle escribe un elemento después
del arreglo, así que un sanitizador reporta un desbordamiento de búfer global" sí lo es.

### Una predicción trabajada

Toma failure-003, la semilla en el bucle de reintento:

- código esperado: `srand(time(NULL))` dentro del `while` que reintenta una colisión;
- síntoma esperado: dos ejecuciones difieren; una semilla fija puede repetir la misma colisión;
- prueba esperada: sembrar dos veces el mismo valor y observar que la secuencia se reinicia.

Esa predicción puede fallar de una forma interesante. Si la semilla estuviera fuera del bucle, la
segunda ejecución sería distinta pero el reintento avanzaría. Escribe la predicción antes de mirar.

## Paso 2 — Ejecutar las pruebas (15 min)

Desde este directorio de la lección:

```bash
bash tests/run_failure_tests.sh /ruta/al/archivo
```

Una ejecución de referencia en esta máquina imprime:

```text
failure-001=detected
failure-003=detected
failure-004=detected
failure-005=detected
failure-002=detected
failure-006=detected
```

El orden es fijo. El arnés de C++ imprime las primeras cuatro líneas; el script de shell imprime las
dos últimas. El script devuelve `0` cuando las seis dicen `detected`.

### Qué significa `detected`

`detected` significa que la prueba reprodujo el patrón. No significa que el programa archivado se
haya compilado, ejecutado o demostrado roto en tiempo de ejecución.

| Línea | La produce | Qué comprueba |
|---|---|---|
| `failure-001=detected` | `loop_bound_test()` | Un bucle `<= 64` borra un centinela en el índice 64. |
| `failure-003=detected` | `seed_in_loop_test()` | Volver a sembrar el mismo valor reinicia la secuencia. |
| `failure-004=detected` | `broken_initialization_test()` | Reiniciar el puntero limpia solo el índice 0. |
| `failure-005=detected` | `inverted_win_test()` | El estado de todos los peones comidos se asigna a `machine`. |
| `failure-002=detected` | `grep` de shell | `void main` en variant-01 e `int main` en variant-03. |
| `failure-006=detected` | `grep` de shell | `cin>>` hacia el búfer del nombre en variant-02. |

El arnés compila `tests/failure_tests.cpp` con `-std=c++20 -Wall -Wextra -Werror -O2`. Es un
programa pequeño con cuatro funciones; no incluye el archivo. Las comprobaciones de shell leen el
texto del archivo con `grep`.

### Si una línea dice `not-detected`

No cambies la prueba primero. Comprueba, en este orden:

1. la ruta del archivo — el script espera `original/variant-.../source/...`;
2. la etiqueta — los números de línea y el texto pertenecen a `v1.0.0`;
3. el compilador — el arnés necesita C++20 y `-Werror`;
4. el patrón — `grep` es suficientemente literal para `void main`, `int main` y `cin>>`.

Una comprobación que falla es evidencia sobre la comprobación antes que sobre el código.

### El orden esconde una dependencia

`set -euo pipefail` está activo. El arnés de C++ se ejecuta primero; si devuelve un valor distinto
de cero, el script se detiene antes de las dos comprobaciones de shell. Un `not-detected` en las
primeras cuatro líneas esconde 002 y 005. Corrige el arnés primero y vuelve a ejecutar.

## Paso 3 — Diagnosticar los seis fallos (30 min)

Las tarjetas son el producto. Este paso es un ejemplo trabajado seguido de cinco diagnósticos
breves. Lee la tarjeta completa de cada fallo; la lección te da el hilo que hay que tirar.

### failure-001 — la tarjeta trabajada

Abre [`../../cards/failure-001-loop-bound-es.md`](../../cards/failure-001-loop-bound-es.md). Léela
una vez y luego asigna cada campo al código.

**Evidencia.** `variant-01:61`, con el mismo patrón en `variant-02:72`, `variant-03:82` y
`variant-04:88`:

```cpp
int vectab[64];          // línea 17
for (i=0;i<=64;i++)      // línea 61
{
    vectab[i]=0;
}
```

**Síntoma.** El bucle escribe `vectab[64]`, un elemento después del final. El programa suele
continuar.

**Causa raíz.** El tamaño del arreglo y el límite del bucle son dos literales. `<` se convirtió en
`<=`.

**Consecuencia.** Comportamiento indefinido. Un arreglo global puede sobrescribir un global
adyacente; una copia local puede corromper la pila. El fallo es silencioso hasta que deja de serlo.

**Prueba.** `loop_bound_test()` usa un arreglo de 65 elementos, escribe un centinela en el índice 64
y ejecuta el mismo bucle `<= 64`. Devuelve `true` cuando el centinela fue borrado. El elemento 65
hace que la reproducción esté definida; la escritura 65 del original no lo está.

**Arreglo mínimo.** `i < 64`.

**Arreglo estructural.** `std::array<int, 64>` más `fill` o un bucle basado en rango. El límite
viene del tipo. Pista A: `construction-02` (bucles estructurados) y `construction-17` (STL).

**Confianza.** Alta. La declaración, el bucle y la escritura son visibles.

**Nota.** La prueba es una reproducción aislada, no una prueba del original.

Eso es toda la tarjeta. Las demás tienen la misma forma. El trabajo no es rellenar la plantilla; es
encontrar la frase que pertenece a **causa raíz**.

### failure-002 — el punto de entrada

`variant-01:32`, `variant-02:37` y `variant-04:52` declaran `void main()`. `variant-03:47` declara
`int main()`, lo que te da el par de contraste.

El síntoma es una declaración sin tipo de retorno. La causa raíz es una extensión del compilador
tratada como la regla del lenguaje: Visual C++ 6.0 aceptaba `void main`, así que el código
compilaba. La consecuencia es portabilidad: un compilador conforme o una compilación con `-Werror`
lo rechaza, y el estado de salida del proceso no está definido por el código.

La prueba es el bloque de shell que busca `void main` en variant-01 e `int main` en variant-03.
Comprueba el texto, no el veredicto del compilador. El arreglo mínimo es `int main()` con
`return 0;`. El arreglo estructural mantiene `main` como un adaptador delgado alrededor de
`int runGame()`. Pista A: `construction-03`.

### failure-003 — la semilla en el bucle de reintento

`variant-01:76`, `variant-02:87`, `variant-03:99` y `variant-04:105` llaman a `srand(time(NULL))`
dentro del `while` que reintenta una colisión de colocación.

El síntoma es una colocación no reproducible. La causa raíz es que la semilla se reinicia en cada
intento, así que el generador se reinicia en lugar de avanzar. El bucle solo puede progresar cuando
el reloj cambia. Con un parche de semilla fija, una colisión se repite para siempre:
`archaeology-03` registra la semilla 9999 como `run_status=124`.

La prueba es `seed_in_loop_test()`. Usa un generador congruencial lineal pequeño, lo siembra con
`1`, guarda el primer valor, vuelve a sembrar con `1` tres veces y comprueba si el siguiente valor
se repite. Detecta el sembrado repetido, no `rand()`. El arreglo mínimo mueve la semilla antes del
bucle, o la recibe como parámetro. El arreglo estructural inyecta una fuente aleatoria. Pista A:
`construction-03` y `construction-20`.

### failure-004 — el reinicio del puntero

`variant-03:84-86`:

```cpp
for (i=0;i<=64;i++)
{
    pvectab=&vectab[0];         // línea 84
    *pvectab=*(pvectab+i);      // línea 85
    pvectab->mvectab=0;         // línea 86
}
```

El síntoma es que solo se limpia `vectab[0]`. La causa raíz es el reinicio dentro del cuerpo: cada
escritura apunta al elemento 0, y la copia de la línea 85 se sobrescribe de inmediato. La
consecuencia es un tablero que parece inicializado solo porque los globales empiezan en cero; una
segunda partida o un tablero reutilizado conserva jugadas viejas.

La prueba es `broken_initialization_test()`: ocho unos, un reinicio del puntero en cada iteración y
una comprobación de que el índice 0 es cero mientras el índice 1 sigue siendo uno. El arreglo
mínimo indexa el arreglo directamente. El arreglo estructural usa `std::array<stablero, 64>` y
`fill`. Pista A: `construction-05` y `construction-07`.

Esta tarjeta se solapa con failure-001 en `variant-03:82`. El bucle tiene el límite `<=` y el
reinicio del puntero. Mantén los dos arreglos separados en tu registro; un parche combinado
escondería qué razonamiento corrigió qué daño.

### failure-006 — el nombre sin límite

`variant-02:46`, `variant-03:56` y `variant-04:62` leen `cin>>nom.nombre` en `char nombre[250]`. La
extracción no tiene ancho. Un nombre de más de 249 caracteres escribe después del arreglo.

El síntoma es corrupción de memoria después del nombre. La causa raíz es el arreglo C más la
extracción formateada por defecto: la llamada no puede conocer el tamaño del arreglo. La
consecuencia es un desbordamiento de búfer, con comportamiento indefinido para toda entrada larga y
un coste de seguridad que depende del diseño.

La prueba es el bloque de shell que busca `cin>>` en variant-02. Detecta el patrón, no un
desbordamiento real. El arreglo mínimo es
`cin >> std::setw(sizeof(nom.nombre)) >> nom.nombre;` o
`cin.getline(nom.nombre, sizeof nom.nombre);`. El arreglo estructural es `std::string` más
`std::getline` y validación. Pista A: `construction-17` y `construction-22`.

### failure-005 — la victoria invertida

`variant-00:299` imprime `TRIUNFO PARA LA MIQUINA` cuando las cuatro casillas de los peones ya no
valen `-1`, que es el estado después de que el caballo haya comido los cuatro peones. La etiqueta
del ganador está fijada a la máquina; la condición son cuatro negaciones; no hay un valor
`Outcome` ni un modelo de jugador.

El síntoma es un resultado incorrecto. La causa raíz es que la condición de final y el ganador se
escribieron de forma independiente. La consecuencia es un marcador, una transcripción o una prueba
incorrectos. La prueba es `inverted_win_test()`: asigna `allPawnsEaten = true` a `"machine"` y
devuelve verdadero cuando el resultado no es `"player"`. El arreglo mínimo nombra el predicado y
declara el ganador de forma explícita. El arreglo estructural es un enum `Outcome` y una función
`evaluateOutcome()`. Pista A: `construction-10`.

La intención original es lo único que el código no puede resolver. La tarjeta la marca como
inferencia.

## Paso 4 — Elegir el arreglo mínimo (15 min)

Para cada tarjeta, escribe el parche más pequeño que elimina el daño. Hazlo en una copia de trabajo,
no en el archivo. El objetivo no es producir un diff bonito; es demostrar que el daño se puede
eliminar sin cambiar el diseño.

| Tarjeta | Parche mínimo | Qué no corrige |
|---|---|---|
| 001 | `i < 64` | El siguiente bucle puede repetir el literal. |
| 002 | `int main()` y `return 0;` | El juego sigue atado al proceso. |
| 003 | Mover `srand` antes del bucle, o pasar una semilla | La secuencia sigue dependiendo de la plataforma. |
| 004 | Indexar `vectab[i]` directamente | El tablero sigue teniendo un literal de longitud separado. |
| 005 | `setw(sizeof(nom.nombre))` o `getline` | Validación del contenido y espacios. |
| 006 | Nombrar el predicado; declarar el ganador correcto | No hay un modelo completo de resultado. |

Dos reglas para este paso:

1. **Un daño, un parche.** No corrijas el error de límite y el reinicio del puntero en un solo
   cambio si las tarjetas son separadas. Pierdes la evidencia de qué cambio hizo qué.
2. **Sin reescritura.** Si el parche crece más allá de unas pocas líneas, pasaste al paso 5. Está
   permitido, pero etiquétalo como arreglo estructural y conserva el mínimo en el registro.

### Qué no es un arreglo mínimo

Un arreglo mínimo no es un truco temporal del que avergonzarse. Es el primer arreglo: el que
detiene el daño y compra tiempo para el cambio de diseño. La regla de la pista es "primero el
arreglo mínimo; la reescritura es el último recurso, con un precio". Un arreglo mínimo correcto y
probado es un resultado profesional.

## Paso 5 — Elegir el arreglo estructural (10 min)

El arreglo estructural aborda la clase, no la instancia. Vincúlalo a un peldaño de la pista A para
que el concepto sea visible.

| Tarjeta | Cambio estructural | Peldaño de la pista A | Por qué previene la clase |
|---|---|---|---|
| 001 | `std::array` + `fill` o rango | `construction-02`, `construction-17` | El límite viene del tipo. |
| 002 | `main` delgado alrededor de `int runGame()` | `construction-03` | El punto de entrada es un adaptador con estado. |
| 003 | Parámetro de semilla o `RandomSource` | `construction-03`, `construction-20` | La aleatoriedad se inyecta, no se esconde en el bucle. |
| 004 | Arreglo de registros + índice/iterador | `construction-05`, `construction-07` | El cursor no se puede reiniciar por accidente. |
| 005 | `std::string` + validación | `construction-17`, `construction-22` | El búfer es dueño de su longitud y la entrada tiene contrato. |
| 006 | `Outcome` + `evaluateOutcome()` | `construction-10` | Una decisión, una fuente de verdad, probable sin `cout`. |

El peldaño es un vínculo, no un mandato. No necesitas completar `construction-17` para diagnosticar
failure-001. Necesitas nombrar el concepto que hace desaparecer la clase de fallo.

## Paso 6 — Ordenar por coste (10 min)

La severidad se trata de la consecuencia. Ordena los seis en tu contexto y escribe la razón. Un
orden de referencia para un código que acepta entrada no confiable:

| Puesto | Tarjeta | Coste | Por qué |
|---:|---|---|---|
| 1 | failure-006 | Crítico | Desbordamiento en la primera entrada; seguridad y corrupción. |
| 2 | failure-001 | Crítico | Comportamiento indefinido en cada inicialización; silencioso. |
| 3 | failure-003 | Alto | Ejecuciones no reproducibles; una semilla fija puede colgarse. |
| 4 | failure-004 | Alto | Tablero incorrecto al reutilizar; invisible en una demo nueva. |
| 5 | failure-005 | Alto | Ganador incorrecto; corrompe todo resultado posterior. |
| 6 | failure-002 | Alto | Portabilidad; visible al compilar, barato de corregir. |

El orden no es universal. Si el programa es una demo de una sola ejecución sin entrada de usuario,
failure-006 puede bajar. Si el programa es un oráculo de pruebas, failure-003 puede ser el primero.
La habilidad es el razonamiento, no el puesto.

### Coste no es lo mismo que dificultad

Failure-002 es de severidad alta y un arreglo de dos líneas. Failure-005 es crítico y un arreglo de
una línea. Failure-003 es alto y necesita una decisión de diseño sobre la semilla. Ordena por lo que
ocurre si no lo corriges, y usa el esfuerzo como desempate.

## Paso 7 — Registrar (10 min)

Completa [`EVIDENCE-es.md`](EVIDENCE-es.md):

- preparación y entorno;
- la tabla de predicciones, escrita antes de las tarjetas;
- el comando de pruebas y las seis líneas;
- el registro de tarjetas: variante, línea, síntoma, causa raíz, prueba, arreglos, confianza;
- el orden por severidad con razones;
- los límites de la evidencia;
- la comprobación de integridad del archivo.

Las tarjetas ya contienen el diagnóstico completo. El registro de evidencia es donde demuestras que
ejecutaste las pruebas, que sabes qué comprueban y que no editaste el archivo.

## El original en detalle

Datos que citarás repetidamente, con números de línea del archivo fijado:

| Variante | Líneas | Qué hay allí |
|---|---|---|
| `variant-00` | 296–299 | `pruebatriunfo()`: cuatro comprobaciones `!= -1` y el mensaje de victoria de la máquina. |
| `variant-01` | 17, 32, 61, 76 | `vectab[64]`, `void main`, `<= 64`, `srand` en el bucle de reintento. |
| `variant-02` | 32, 37, 46, 72, 87 | `char nombre[250]`, `void main`, `cin>>`, `<= 64`, `srand`. |
| `variant-03` | 31, 47, 56, 82, 84–86, 99 | `char nombre[250]`, `int main`, `cin>>`, `<= 64`, reinicio de puntero, `srand`. |
| `variant-04` | 33, 52, 62, 88, 105 | `char nombre[250]`, `void main`, `cin>>`, `<= 64`, `srand`. |

El `05-known-issues.md` del archivo es el índice. Las tarjetas son el diagnóstico. Los números de
línea son el terreno común entre ambos.

## Qué demuestran y qué no demuestran las pruebas

| Afirmación | ¿Sostenida? | Por qué |
|---|---|---|
| El patrón existe en la línea citada. | Sí | El fragmento y el número de línea. |
| El programa original se bloquea o corrompe memoria. | No | El original no se compila ni se ejecuta. |
| El programa original es explotable. | No | No se introduce entrada; no se prueba el diseño. |
| El arreglo mínimo elimina el daño. | Solo en la reproducción | El parche no se aplica al archivo. |
| El arreglo estructural previene la clase. | Por argumento de diseño | El peldaño es un concepto, no un resultado medido. |
| `void main` falla en todos los compiladores. | No | Algunos compiladores lo aceptan como extensión. |
| `cin >>` desborda con toda entrada larga. | El patrón sí; la ejecución no | La comprobación es un `grep`, no una prueba de fuzzing. |

Esta tabla es el control de honestidad de toda la lección. Una tarjeta que afirme más de lo que esta
tabla permite está exagerando.

## Autocomprobación (binaria)

- [ ] Escribí seis predicciones antes de leer las tarjetas.
- [ ] Ejecuté `tests/run_failure_tests.sh` y vi seis líneas `detected`.
- [ ] Puedo nombrar las cuatro pruebas del arnés de C++ y las dos comprobaciones de shell.
- [ ] Puedo explicar por qué `detected` no es prueba de que el original esté roto.
- [ ] Escribí seis tarjetas con los nueve campos.
- [ ] Cada tarjeta cita la variante, la línea y un fragmento.
- [ ] Cada tarjeta separa síntoma de causa raíz.
- [ ] Cada tarjeta declara la consecuencia en el trabajo real.
- [ ] Cada tarjeta nombra la prueba y qué comprueba.
- [ ] Cada tarjeta da un arreglo mínimo antes de un arreglo estructural.
- [ ] Cada arreglo estructural se vincula a un peldaño de la pista A.
- [ ] Ordené los seis por coste y escribí la razón.
- [ ] Registré los límites de la evidencia, incluida la inferencia sobre failure-005.
- [ ] El archivo no está modificado (`git status` en el archivo está limpio).

Si una casilla no está marcada, la casilla sin marcar nombra el siguiente paso. No hay penalización
por repetir uno.

## Misconcepciones que esta lección refuta

| Idea equivocada | Por qué es incorrecta | Qué hacer en su lugar |
|---|---|---|
| "Una tarjeta de fallo es un informe de error." | Un informe describe un síntoma; una tarjeta diagnostica una causa y prueba un arreglo. | Llega a la causa raíz y nombra la prueba. |
| "La prueba demuestra que el original está roto." | Las pruebas reproducen patrones de forma aislada; no ejecutan el archivo. | Declara qué comprueba la prueba y qué no. |
| "Arregla el archivo de paso." | El archivo es evidencia; una reparación destruye el artefacto. | Parchea una copia de trabajo o una adaptación etiquetada. |
| "`void main` siempre falla." | Algunos compiladores lo aceptan como extensión. | Di "no es estándar, no es portable", no "nunca compila". |
| "El bucle escribe en `vecjugadas`." | La escritura es indefinida; el destino depende del diseño. | Di "puede corromper memoria adyacente" y cita el comportamiento indefinido. |
| "`srand` en el bucle solo afecta la aleatoriedad." | También puede hacer que un reintento de colisión no termine. | Conecta la semilla con el cuelgue de archaeology-03. |
| "El error de punteros no limpia nada." | Limpia el elemento 0; las otras 63 casillas conservan sus valores. | Di "solo se limpia la primera casilla". |
| "La condición de victoria está bien porque el comentario coincide." | Los comentarios de este archivo narran o mienten; el código y el mensaje no coinciden. | Confía en el camino ejecutado, no en el comentario. |
| "Un arreglo mínimo es un truco temporal." | Es el primer arreglo: elimina el daño y compra tiempo para el cambio de diseño. | Conserva el mínimo y luego argumenta el estructural. |
| "El arreglo estructural es el arreglo real, así que salta el mínimo." | La pista prohíbe saltar del catálogo a la reescritura. | Primero el mínimo; después el estructural, con un precio. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Fallo de corrección | Un resultado que contradice la afirmación o el contrato del programa. |
| Síntoma | Lo que ve un observador cuando ocurre el fallo. |
| Causa raíz | La decisión que produjo el síntoma. |
| Consecuencia | El coste del fallo en el trabajo real. |
| Arreglo mínimo | El cambio más pequeño que elimina el daño. |
| Arreglo estructural | El cambio de diseño que previene la clase de fallo. |
| Peldaño de la pista A | Una lección de construcción que enseña el concepto detrás del arreglo estructural. |
| Reproducción aislada | Un programa pequeño que reproduce un patrón sin el original. |
| Comportamiento indefinido | El lenguaje no impone ningún requisito sobre el resultado. |
| Desbordamiento de búfer | Una escritura después del final de un búfer de tamaño fijo. |
| No reproducible | Las mismas entradas no producen el mismo resultado. |
| Centinela | Un valor especial que marca un estado, como `-1` para un peón. |

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
