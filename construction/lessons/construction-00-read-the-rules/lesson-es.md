# Leer las reglas, ejecutar la línea base de referencia

> [English](lesson.md) | [Español](lesson-es.md)

**Concepto:** especificación (specification) · **Tiempo:** unos 50 minutos · **Estado:** borrador
**Lectura:** ~7 minutos

## Por qué existe esta lección

Una especificación es la parte del trabajo que sobrevive a un cambio de implementación. Esta
lección no te pide escribir código. Te pide leer el contrato, comprometerte con una predicción y
dejar que una transcripción te muestre qué parte de tu modelo mental era en realidad una suposición.

Es la primera lección de la Pista A. Todas las lecciones posteriores comparan una implementación
con el contrato que estableces aquí.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar la especificación y el invariante del juego;
- predecir el siguiente movimiento de una partida con semilla y compararlo con la traza.

## El concepto: una especificación es una restricción, no una descripción

Las reglas no te dicen qué ocurre en la semilla 1. Te dicen qué puede ocurrir para cualquier
semilla. La semilla elige los valores; la especificación restringe la forma.

Por ejemplo, las reglas obligan a que el orden de movimientos de un turno sea una rotación de
`1..8`. No obligan a que la rotación empiece en 3. `start` se sortea, así que su valor pertenece a
la semilla.

Esa distinción es toda la lección. Cuando dos implementaciones no coincidan, preguntarás: ¿cambió
la regla o solo el valor? Una especificación que puedes separar de su salida es una especificación
que puedes probar.

## Antes de empezar

- `common-00` — el juego como invariante.
- `common-01` — el arnés determinista.

No necesitas C++ para esta lección. Necesitas Python 3 y un editor de texto.

### Qué necesitas

- Las reglas: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md).
- El formato de la transcripción:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- La línea base de referencia:
  [`common/harness/reference.py`](../../../common/harness/reference.py).
- Esta plantilla para quien aprende: [`EVIDENCE-es.md`](EVIDENCE-es.md).

Comprueba que las reglas y la línea base existen antes de empezar:

```bash
test -f common/spec/game-rules.md && test -f common/harness/reference.py
```

Si falta `reference.py`, la línea base no está presente en esta copia del repositorio. No inventes
una transcripción y no modifiques el arnés para que coincida con tu predicción. Registra el
artefacto ausente en `EVIDENCE-es.md` §9 y vuelve cuando el archivo esté disponible. Una lección sin
su artefacto todavía no tiene evidencia.

## Paso 1 — Leer las reglas (~10 min)

Lee [`game-rules.md`](../../../common/spec/game-rules.md) una vez, de principio a fin. Después
responde estas preguntas con tus propias palabras, sin copiar frases del documento:

1. ¿Cuándo es legal un movimiento y cuál es la única excepción a la regla de celdas visitadas?
2. ¿Quién empieza y qué ocurre si las tiradas son iguales?
3. Después del primer movimiento exitoso, ¿qué celdas están visitadas exactamente?
4. Un caballo puede entrar en su propia celda inicial. ¿Por qué es coherente con la regla de
   celdas visitadas?
5. ¿Qué termina la partida además de una captura?
6. ¿Qué sección decide el ganador cuando el caballo activo no tiene ningún movimiento legal?

Escribe las respuestas en `EVIDENCE-es.md` §2. Si una respuesta necesita una suposición, márcala
como suposición.

Ahora lee las secciones **Header**, **Turn**, **Attempts**, **Successful move**, **Trap** y
**Result** de [`transcript-format.md`](../../../common/spec/transcript-format.md).

La sección **Example** usa la semilla 17, no la semilla 1. Puedes leerla ahora; no revelará tu
objetivo. El ejemplo es ilustrativo; las reglas son normativas.

## Paso 2 — Predecir antes de ejecutar (~10 min)

Escribe la predicción en `EVIDENCE-es.md` §3 **antes de ejecutar cualquier cosa**. Una vez que has
visto la transcripción, la predicción ya no es evidencia.

Predice el primer movimiento de la semilla 1 para el nombre de jugador `Learner`. Una predicción
completa tiene dos capas.

### 2.1 Lo que las reglas obligan

Completa esta tabla a partir de las reglas, no de la memoria:

| Pregunta | Regla que lo decide | Mi respuesta |
|---|---|---|
| ¿Qué caballo mueve primero? | §4 | empieza el de mayor tirada |
| ¿Dónde está ese caballo? | §3 | una de las dos celdas iniciales |
| ¿Qué forma tiene el orden del turno? | §6 | una rotación de `1..8` |
| ¿Qué movimiento se intenta primero? | §6 | el primer número de esa rotación |
| ¿Cuándo es legal el primer intento? | §7, §8 | dentro del tablero y no visitada, o captura |
| ¿Qué pasa si ningún intento es legal? | §10 | el caballo activo queda atrapado y pierde |

La tabla es la parte que puedes defender sin ejecutar nada. «Una rotación de `1..8`» es una
respuesta obligada. «El orden empieza en 3» no lo es: es un valor elegido por la semilla.

### 2.2 Lo que decide la semilla

Las celdas iniciales y las tiradas no están en las reglas. Comprométete con una suposición de todos
modos. Una predicción con un valor supuesto sigue siendo una predicción, y la comparación te
mostrará qué campos estaban obligados y cuáles fueron supuestos.

Escribe una línea esperada completa:

```text
move=1 <player|machine> <fila-origen>,<columna-origen>-><fila-destino>,<columna-destino>
```

Marca cada campo como `[obligado]` o `[supuesto]`, y añade una confianza de 0 a 100 %. Predice
también:

- qué caballo empieza;
- el orden del turno con sus ocho números;
- la primera línea `try=`;
- el resultado de ese primer intento (`legal`, `illegal=outside`, `illegal=visited` o `capture`).

Una suposición equivocada es un dato. Una predicción escrita después de ejecutar no lo es.

## Paso 3 — Ejecutar (~5 min)

Desde la raíz del repositorio, ejecuta exactamente:

```bash
python3 common/harness/reference.py --seed 1 --name Learner
```

Captura la salida completa. No recortes las líneas de intentos; allí es donde las reglas se vuelven
observables.

Si el comando falla, registra el error exacto en `EVIDENCE-es.md` §9. No parchees el arnés y no
sustituyas su salida por el ejemplo del documento de formato.

## Paso 4 — Comparar (~5 min)

Busca `move=1` en la transcripción. Pon tu predicción y la observación una al lado de la otra y
compara campo por campo:

- ¿Empezó el caballo que predijiste?
- ¿Fue el orden del turno la rotación que esperabas?
- ¿Qué intentos aparecieron antes del movimiento exitoso?
- ¿Cada intento tuvo el resultado que predijiste?
- ¿Coincidió la línea `move=` en número de movimiento, caballo, origen y destino?

Escribe las líneas exactas que difieren, no «fue diferente». Si nada difiere, di qué campos
predijiste por la razón correcta y cuáles acertaste por suerte.

Solo ahora lee el **Example** de
[`transcript-format.md`](../../../common/spec/transcript-format.md). Es ilustrativo, no normativo,
y no tiene garantía de coincidir con la tabla de movimientos. Si contradice las reglas, ganan las
reglas (game-rules §14). Registra la contradicción; notarla forma parte de leer una especificación
de forma crítica.

## Paso 5 — Explicar la brecha (~10 min)

Escribe un párrafo en `EVIDENCE-es.md` §6. Responde:

- ¿En qué campo me equivoqué?
- ¿Fue una regla que leí mal o un valor que eligió la semilla?
- ¿Qué asumí que la especificación nunca dijo?

No uses palabras de lenguajes de programación en la explicación. «La rotación empezó en otro número»
es una explicación. «`random_integer` devolvió otro valor» nombra un
mecanismo sin explicar el modelo.

Si tu predicción coincidió, explica por qué cada campo tenía que coincidir. Un acierto sin razón
es suerte, y la suerte no se transfiere a la siguiente semilla.

## Paso 6 — Criterio de aceptación: predecir el siguiente movimiento (~5 min)

El criterio de aceptación de esta lección es: **predecir el siguiente movimiento de una partida con
semilla.**

1. Elige un turno de tu transcripción que tenga otro turno después. El turno 2 es un buen valor por
   defecto.
2. Copia su línea `turn=`. Tapa todas las líneas posteriores.
3. Usando solo el encabezado, los movimientos anteriores, las celdas visitadas y las reglas §6–§8,
   escribe cada línea `try=` y la línea `move=` que esperas.
4. Descubre las líneas tapadas y compara.

Aquí no estás adivinando el sorteo. El orden del turno ya está impreso, el estado del tablero se
conoce y las reglas deciden el resto. Si tu línea difiere, explica la diferencia antes de continuar;
esa explicación es la evidencia de aceptación.

## Paso 7 — Registrar (~5 min)

Completa `EVIDENCE-es.md` §1–§8. Conserva el comando, la predicción, la diferencia exacta y la
explicación. El archivo es tu registro.

Si faltaba la línea base, completa §9 y detente. Todavía no hay nada que comparar.

## Autocomprobación (binaria)

- [ ] Leí las reglas y las secciones del formato de transcripción, no solo el ejemplo.
- [ ] Escribí mi predicción antes de ejecutar la línea base.
- [ ] Ejecuté `python3 common/harness/reference.py --seed 1 --name Learner` y capturé la salida
      completa.
- [ ] Nombré los campos exactos que difirieron, o expliqué por qué ninguno difirió.
- [ ] Predije un movimiento posterior a partir del estado impreso y las reglas, y lo comprobé.
- [ ] Completé `EVIDENCE-es.md`, incluido el siguiente paso.

Son criterios de aceptación. Si queda una casilla sin
marcar, escribe la siguiente acción más pequeña en `EVIDENCE-es.md` §8.

## Misconcepciones que esta lección refuta

| Concepción errónea | Por qué falla | Qué hacer en su lugar |
|---|---|---|
| «El ejemplo es la especificación.» | Un ejemplo muestra una partida; las reglas definen todas. | Contrasta el ejemplo con las reglas y di cuál gana. |
| «Una predicción equivocada significa que fracasé.» | La brecha localiza el supuesto que hay que reparar. | Nombra el supuesto erróneo; conserva la evidencia. |
| «El orden siempre es `1..8`.» | Es una rotación que empieza en un valor sorteado. | Separa la forma obligada del inicio que decide la semilla. |
| «Las celdas iniciales están visitadas.» | Están ocupadas, pero no visitadas hasta que alguien entra. | Relee §3 y §8 antes de predecir la legalidad. |
| «Si la salida no coincide, la salida está mal.» | La especificación es el contrato; la implementación se corrige. | Registra la discrepancia y deja que decidan las reglas. |

## Siguiente paso

`construction-01` — la línea base no estructurada. Ya tienes el contrato que toda variante
posterior debe satisfacer. Consulta el
[catálogo de lecciones](../../../docs/guide/lesson-catalog.md) para ver la ruta completa.

Si faltaba `reference.py`, no empieces `construction-01`. Restaura primero la línea base; sin ella,
no se puede comprobar «el mismo comportamiento».
