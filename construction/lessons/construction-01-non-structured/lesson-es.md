# Programación no estructurada (non-structured programming)

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** flujo de control arbitrario · **Tiempo:** unos 70 minutos · **Estado:** borrador
**Lectura:** ~11 minutos

## Por qué existe esta lección

Un programa que puede saltar a cualquier parte no tiene fronteras, solo posiciones. Eso suena a
libertad. Esta lección muestra cuánto cuesta.

No vas a escribir un programa. Vas a leer uno que ya funciona, a poner precio a un cambio de regla
con las manos fuera del teclado, y a descubrir que el precio se paga en *lugares*, no en líneas.

## Qué podrás hacer

Al terminar esta lección puedes:

- identificar los saltos y etiquetas que hacen arbitrario el flujo de control;
- añadir un cambio de saltar turno y contar los saltos recableados.

## El concepto: un salto nombra una posición, no una frontera

Una función nombra una entrada y una salida. Un bucle nombra un cuerpo y una condición. Ambos dan al
lector una frontera: algo que empieza aquí, termina allá, y se puede entender sin el resto del
archivo.

Un salto nombra una **posición**. Dice «continúa en la línea 184». No dice nada sobre qué debe ser
cierto al llegar, qué puedes suponer, ni quién tiene permiso para salir. La posición es real. La
frontera es una convención que vivía solo en la cabeza de quien programaba.

De ahí se siguen dos consecuencias, y son toda la lección:

1. **Se puede entrar en una posición desde cualquier parte, así que sus lectores están en todas
   partes.** Un turno puede empezar en una sola etiqueta, pero el código que decide ir allí está
   dentro de cada rama que termina otro trabajo. La regla vive en una posición; los caminos para
   llegar a ella viven en muchas.
2. **El lector tiene que simular.** Con fronteras puedes leer una unidad aislada. Sin ellas, trazas
   el control desde arriba sosteniendo en la memoria de trabajo todos los destinos posibles a la
   vez.

Por eso el coste del siguiente cambio es alto. No porque las líneas sean feas, sino porque los
lugares que deben coincidir no están reunidos en ninguna parte.

## Antes de empezar

- `construction-00` — ya tienes el contrato y ya has visto una transcripción.
- Un compilador de C++20 (`g++` o `clang++`). `cmake` es opcional.

No necesitas saber C++ a fondo. Necesitas poder seguir un salto de una etiqueta a otra. Esa es toda
la destreza que ejercita esta lección.

### Qué necesitas

- La línea base: [`code/non-structured-baseline.cpp`](code/non-structured-baseline.cpp).
- Su script de compilación y sus notas: [`run.sh`](run.sh) y [`README.md`](README.md).
- Las reglas: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md).
- El formato de la transcripción:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- Una traza de referencia: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- Esta plantilla para quien aprende: [`EVIDENCE-es.md`](EVIDENCE-es.md).

Comprueba que los artefactos existen antes de empezar, desde este directorio de la lección:

```bash
test -f code/non-structured-baseline.cpp && test -f ../../../common/traces/seed-0001.txt
```

Si falta la línea base, el artefacto no está en esta copia del repositorio. No inventes un recuento
de saltos y no describas un programa que no has leído. Registra el archivo ausente en
`EVIDENCE-es.md` §9 y vuelve cuando esté disponible. Un coste medido sobre un programa imaginario no
es evidencia.

Si la línea base existe pero sus opciones de línea de comandos no son las de aquí abajo, usa las
suyas y registra la invocación exacta. No la parchees para que coincida con esta página.

## Paso 1 — Inventariar los saltos (~15 min)

Lee la línea base una vez, de principio a fin, sin juzgarla. Después construye un inventario.

Enumera cada etiqueta que encuentres, con su número de línea. Enumera cada salto, con su número de
línea y su destino. Es trabajo mecánico; hazlo con el archivo abierto, no de memoria.

Dos comandos te dan un punto de partida, no la respuesta completa:

```bash
grep -nE '^[[:space:]]*[A-Za-z_][A-Za-z0-9_]*:' code/non-structured-baseline.cpp
grep -n 'goto ' code/non-structured-baseline.cpp
```

El patrón también coincide con `std::` dentro de declaraciones, y un salto puede compartir línea con
una condición. Lee el archivo para confirmar lo que reportan los comandos. El inventario es el
archivo, no el grep.

Después responde en `EVIDENCE-es.md` §2:

1. ¿Cuántas etiquetas existen?
2. ¿Cuántos saltos existen?
3. ¿A qué etiquetas se llega desde más de un salto?
4. ¿A qué etiquetas se llega tanto por un camino de jugador como por uno de máquina?
5. ¿Qué etiquetas son entradas de turno, es decir, se alcanzan cuando el programa está a punto de
   sortear un orden de movimientos?

La pregunta 5 es la que importa. Respóndela siguiendo el código hasta el lugar donde se sortea el
orden de movimientos y retrocediendo después hasta cada posición que puede llegar allí.

## Paso 2 — Predecir el coste antes de medirlo (~10 min)

La prueba de cambio de esta lección es un solo requisito. Léelo dos veces y después escribe tu
predicción.

> **R-SKIP — saltar turno.** Cuenta los turnos cuando empiezan, produzcan o no un movimiento. Ese
> contador es independiente del valor `moves=` de la transcripción. Cuando el contador es múltiplo
> de 5, el caballo activo no mueve: el turno se anuncia igual con sus campos `turn=` y `order=`, no
> le sigue ninguna línea `try=` ni ninguna línea `move=`, y el turno pasa al otro caballo. Un turno
> saltado no cuenta para los 200 movimientos exitosos.

R-SKIP es determinista a propósito: no añade ningún sorteo, así que no altera el orden de llamadas
de §12 de las reglas. Una regla que sorteara un número sería un cambio distinto y más grande.

Antes de contar nada, escribe en `EVIDENCE-es.md` §3:

1. ¿Cuántas etiquetas inician un turno?
2. ¿Cuántos saltos hay que recablear para que R-SKIP se cumpla en todas partes?
3. ¿Cuántas etiquetas nuevas hacen falta?
4. ¿Cuántos saltos nuevos hacen falta?
5. ¿En cuántos lugares se decide quién juega después?
6. ¿Cuál es el lugar único más barato para poner la comprobación, y qué no puede hacer todavía
   desde allí?
7. La cabecera del turno imprime un número derivado del recuento de movimientos. ¿Qué número imprime
   un turno saltado, y es aceptable?
8. Confianza, de 0 a 100 %, para la pregunta 2.

Una predicción de «muchos» no es una predicción. Comprométete con un número, aunque esperes
equivocarte. La brecha entre tu número y el número contado es la señal de calibración.

### 2.1 La regla de conteo

Un recuento solo es comprobable si la regla para contar se fija antes de contar. Usa esta.

- Una **entrada de turno** es cualquier posición a la que llega el programa cuando está a punto de
  sortear un orden de movimientos.
- Una **llegada** es un salto cuyo destino es una entrada de turno.
- Un **salto recableado** es una llegada cuyo destino pasa a ejecutar la comprobación de R-SKIP.
  Cuenta todas las llegadas, incluidas las que no necesitan ningún cambio de texto: no puedes saber
  que una no lo necesita sin leerla.
- Una **etiqueta nueva** es una etiqueta que no existe antes del cambio y debe existir después.
- Un **salto nuevo** es un salto que no existe antes del cambio y debe existir después; por ejemplo,
  el que abandona el camino del turno saltado.
- **No** cuentes comentarios, formato, líneas en blanco ni un `return` simple.

Cuenta saltos y etiquetas por separado. Son unidades distintas, y mezclarlas oculta cuál de las dos
es la cara.

### 2.2 Un ejemplo resuelto del método

Esto es una demostración de conteo, no la línea base. Sus números no son tu respuesta.

```text
turn_player:              <- se llega por 2 saltos
    ...
    salto turn_machine
turn_machine:             <- se llega por 2 saltos
    ...
    salto turn_player
```

Aplicando la regla: dos entradas de turno, cuatro llegadas, cuatro saltos recableados y cero
etiquetas nuevas, siempre que la comprobación se escriba en ambas entradas. Si se escribe en una
sola, dos llegadas aterrizan en una entrada que nunca comprueba, y esas dos son erróneas en
silencio. El recuento no cambia, porque igualmente tuviste que leer las cuatro llegadas para saber a
qué entrada llega cada una.

La lección del ejemplo: el coste no es el tamaño de la comprobación. Es el número de llegadas. El
ejemplo tiene dos entradas; un programa real puede tener una, y una entrada a la que se llega desde
varios lugares sigue contando todas las llegadas.

## Paso 3 — Ejecutar la línea base (~10 min)

Constrúyela y reproduce una traza de referencia. Este paso establece que el programa que estás
leyendo es el juego, no un primo del juego.

```bash
./run.sh --seed 1 --name Learner > /tmp/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/baseline-0001.txt
```

`run.sh` compila con CMake cuando está disponible y, si no lo está, recurre a `c++ -std=c++20`. Si
ambos caminos fallan, compila una vez a mano y registra el comando exacto:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o build/non-structured-baseline code/non-structured-baseline.cpp
```

Un `diff` vacío es el punto. El analizador, el generador, la tabla de movimientos y el bucle de
turnos coinciden con la referencia para esta semilla. El diseño no cambió nada observable. Esa es la
precondición para medir un cambio: no puedes poner precio a un cambio en un programa cuyo
comportamiento actual no has confirmado.

Si `diff` imprime diferencias, detente. La línea base no es una edición no estructurada fiel, y cada
recuento que produzcas describiría otro programa. Registra el diff en `EVIDENCE-es.md` §9 y
repórtalo; no ajustes la traza para que coincida con el código.

Si la compilación falla, registra la salida del compilador. Una línea base que no compila no tiene
comportamiento que preservar.

## Paso 4 — Comparar: medir el coste (~10 min)

Ahora cuenta. Aplica la regla de §2.1 al archivo que tienes y completa la tabla de
`EVIDENCE-es.md` §5.

| Elemento | Previsto | Contado | Diferencia |
|---|---|---|---|
| Etiquetas que inician un turno | | | |
| Saltos recableados (llegadas a una entrada de turno) | | | |
| Etiquetas nuevas necesarias | | | |
| Saltos nuevos necesarios | | | |
| Lugares que deciden quién juega después | | | |
| Archivos tocados | 1 | 1 | 0 |

Para cada salto contado, escribe la etiqueta a la que apunta y la línea en la que está. Un número
sin lista es una opinión. La lista es lo que hace que otra persona pueda reproducir el recuento.

Después responde:

- ¿Qué número previsto fue el más cercano, y por qué?
- ¿Qué número previsto fue el más lejano, y qué no viste cuando lo escribiste?
- ¿Qué no podía hacer el punto de inserción más barato, y dónde tuvo que ir esa parte?
- ¿Mostraría la transcripción la diferencia si faltara la segunda parte? ¿Cómo?

Si un número difiere, ese es el resultado. No revises la predicción; escribe la diferencia al lado.

## Paso 5 — Explicar el coste (~10 min)

Escribe la explicación en `EVIDENCE-es.md` §6. En dos partes.

**Primero, una frase.** Di por qué este cambio es caro sin usar las palabras *malo*, *goto*,
*espagueti* o *ilegible*. Esas palabras nombran un veredicto, no una causa. Si tu frase necesita una
de ellas, has descrito tu reacción, no la estructura.

**Segundo, un párrafo.** Responde:

- ¿Qué parte de R-SKIP tiene un hogar evidente, y qué parte no tiene ninguno?
- El programa ya contiene el comportamiento «pasar el turno al otro caballo». ¿Dónde está, y por qué
  un camino nuevo no puede reutilizarlo como unidad?
- ¿Qué debe sostener un lector en la memoria de trabajo para saber si un camino llega a la
  comprobación?
- ¿Qué tiene que hacer el lector para estar seguro de que un cambio está completo?

Una buena respuesta nombra el mecanismo: el requisito se parte en una comprobación que tiene hogar y
una transición que no lo tiene, así que la segunda mitad debe copiarse de una rama. Una respuesta
débil dice que el código está desordenado. Desordenado es lo que ves; una transición única que ahora
necesita un gemelo es lo que lo causa.

## Paso 6 — Criterio de aceptación (~10 min)

El criterio de aceptación de esta lección es: **añadir un requisito de «saltar turno»; contar los
saltos recableados y explicar el coste. No se exige corregir código.**

Comprueba tu registro contra él:

- [ ] Reformulé R-SKIP con mis propias palabras y no añade ningún sorteo.
- [ ] La regla de conteo está enunciada una vez y aplicada igual a cada salto.
- [ ] El recuento es un número, y cada salto contado está listado con su destino y su línea.
- [ ] Comparé mi recuento previsto con el contado, incluidas las diferencias.
- [ ] Mi explicación de una frase evita *malo*, *goto*, *espagueti* e *ilegible*.
- [ ] Mi párrafo nombra la causa estructural, no el síntoma.
- [ ] No necesité cambiar el código para llegar a esta conclusión.

Implementar R-SKIP es opcional y queda fuera del criterio. Si lo implementas, dilo, y espera que la
transcripción cambie: R-SKIP cambia el comportamiento, así que es un juego nuevo, no un diseño
nuevo. Mídelo por separado y no lo compares contra las trazas de referencia.

## Paso 7 — Registrar (~5 min)

Completa `EVIDENCE-es.md` §1–§8. Conserva el inventario, la predicción, la salida del comando, la
tabla de recuento y la explicación. Este archivo es tu registro.

Si faltaba la línea base, completa §9 y detente. Todavía no hay nada que contar.

## Autocomprobación (binaria)

- [ ] Leí la línea base e inventarié sus etiquetas y sus saltos.
- [ ] Escribí el recuento previsto antes de contar.
- [ ] Compilé la línea base y reproduje `seed-0001.txt` con un diff vacío.
- [ ] Conté los saltos recableados con la regla enunciada y listé cada uno.
- [ ] Comparé la predicción con el recuento y nombré las diferencias.
- [ ] Expliqué el coste en una frase sin las cuatro palabras prohibidas, más un párrafo.
- [ ] Completé `EVIDENCE-es.md`.

Son criterios de aceptación. Si queda una casilla
sin marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §8.

## Misconcepciones que esta lección refuta

| Concepción errónea | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| «Funciona, así que el diseño está bien.» | Que el comportamiento funcione no dice nada del coste de la siguiente regla. | Pon precio a un cambio y compáralo con el diseño alternativo. |
| «El coste es el número de líneas que añado.» | La comprobación es corta; las llegadas son muchas. | Cuenta lugares que deben coincidir, no líneas que hay que teclear. |
| «Una comprobación arriba cubre todos los caminos.» | Los caminos que no pasan por arriba son justo los que se rompen. | Enumera cada llegada a una entrada de turno antes de editar. |
| «No estructurado significa sin estructura alguna.» | Tiene estructura: un grafo de posiciones. Le faltan fronteras. | Dibuja el grafo y lee las fronteras que faltan. |
| «Contar es objetivo, así que no hace falta una regla.» | Dos personas cuentan cosas distintas y ambas tienen «razón». | Fija la regla de conteo antes de contar. |
| «Puedo predecir después de contar.» | La predicción es la señal de calibración; debe ir primero. | Escribe el número y después mide. |
| «Si el diff no está vacío, la traza está mal.» | Las trazas son el contrato; la implementación se corrige. | Registra el diff y detente hasta que la línea base sea fiel. |

## Siguiente paso

`construction-02` — programación estructurada. Aplicarás el mismo requisito R-SKIP a una versión
construida con secuencia, selección e iteración, y contarás los mismos números. Trae este recuento
contigo. La comparación entre los dos recuentos es la lección; ningún número significa nada solo.

Consulta el [catálogo de lecciones](../../../docs/guide/lesson-catalog.md) para ver la ruta
completa.

Si faltaba la línea base, no empieces `construction-02`. El contraste necesita los dos lados.
