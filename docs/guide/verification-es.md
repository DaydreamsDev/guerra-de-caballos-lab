# Verificación y evidencia (verification and evidence)

> [English](verification.md) | [Español](verification-es.md)

> Nada se califica. Nada se clasifica. La única pregunta es: **¿puedes demostrar el concepto con un
> artefacto que funciona y cumple sus criterios de aceptación?**

El método de aprendizaje y sus motivos están en [Cómo se aprende](how-learning-works-es.md). Esta
guía cubre solo la verificación.

No hay exámenes, ni cuestionarios, ni puntos, ni niveles, ni crédito parcial, y no se requiere
docente.

---

## 1. La postura

| Lo que no hacemos | Lo que hacemos en su lugar |
|---|---|
| Exámenes | Construir el artefacto y ejecutarlo |
| Cuestionarios | Predecir, ejecutar, comparar con la traza |
| Notas | Criterios de aceptación binarios |
| Ranking | Comparación con tu propio trabajo anterior (ipsativo) |
| Rúbricas con niveles | Una lista de criterios observables |
| Trabajo grupal obligatorio | Ruta independiente; la colaboración es opcional y queda fuera del método |
| Fechas límite | Tiempo sugerido; la persona que aprende decide |

**El único propósito de la verificación es el conocimiento de la propia persona que aprende:**
*¿aprendí esto y qué puedo hacer ahora exactamente?*

---

## 2. Qué cuenta como evidencia (what counts as evidence)

La evidencia es algo que se ejecuta o se puede inspeccionar, no una opinión.

| Evidencia | Forma | Cuándo se requiere |
|---|---|---|
| **Artefacto que funciona** | Código/proyecto que compila y se ejecuta sin conexión | Toda lección de artefacto |
| **Nota de predicción** | Lo que esperaba, escrito *antes* de ejecutar | Toda lección de artefacto |
| **Diferencia predicción/realidad** | Las líneas o el comportamiento exactos que difieren | Toda lección de artefacto |
| **Informe de aceptación** | Criterio → pasa/no pasa → salida observada | Toda lección de artefacto |
| **Comparación de trazas de referencia** | Diferencia de transcripción contra la referencia | Toda variante |
| **Tabla de coste del cambio** | Archivos / líneas / conceptos / pruebas tocadas | Lecciones de diseño |
| **Nota de reflexión** | Qué aprendí, qué no puedo explicar aún, siguiente paso | Toda lección |
| **Explicación grabada** (opcional) | 2–5 min, sin público requerido | Cuando la persona que aprende quiere probar la explicación |

Una captura de pantalla, un párrafo de prosa o un «lo entendí» no son evidencia.

---

## 3. Criterios de aceptación (acceptance criteria)

### 3.1 Reglas

1. **Derivados de la especificación**, no de la implementación.
2. **Observables:** un comando, una diferencia, un comportamiento.
3. **Binarios:** pasa / todavía no. Sin porcentajes.
4. **Públicos:** escritos antes de empezar el trabajo.
5. **Un concepto por criterio.**
6. **Sin criterios ocultos.** Si no está escrito, no se exige.

### 3.2 Plantilla

```markdown
## Acceptance criteria — construction-09 (abstract data type)

- [ ] acceptance-criterion-1: With seed 42, the transcript matches `traces/seed-42.txt`.
- [ ] acceptance-criterion-2: Replacing the array with a linked list changes only `board.cpp`.
- [ ] acceptance-criterion-3: Every existing caller compiles without edits.
- [ ] acceptance-criterion-4: No caller indexes the board directly (grep returns zero hits).
- [ ] acceptance-criterion-5: I can explain the difference between specification and
  representation without using the word "struct".
```

### 3.3 Ejemplos por pista

| ID | Criterios de aceptación (resumen) |
|---|---|
| construction-02 | La traza coincide; sin `goto`; una entrada/salida por función |
| construction-05 | Añadir un campo; traza sin cambios; enumerar las firmas tocadas |
| construction-07 | El mismo recorrido sobre dos disposiciones; aritmética de punteros explicada |
| construction-09 | El cambio de representación toca un archivo; los llamadores compilan |
| construction-12 | Añadir un tipo de pieza; cero ediciones en el bucle del juego |
| construction-15 | El mismo motor en 8×8 y 10×10; sin algoritmo duplicado |
| archaeology-04 | Una prueba que falla reproduce el fallo; el arreglo pasa; trazas sin cambios |
| archaeology-09 | Tipo opaco; cambio de representación; llamadores sin cambios |
| archaeology-18 | La refactorización conserva todas las trazas de referencia |

---

## 4. La comprobación de ingeniería (engineering check)

Este es el ciclo de la vida real para toda lección que produce o cambia un artefacto, no un ritual
escolar. Es a libro abierto, sin tiempo límite, sin nota, y el artefacto es la respuesta.

> **Predecir → Ejecutar → Comparar → Explicar → Registrar.**

| Paso | Qué hace la persona que aprende | Evidencia |
|---|---|---|
| **Predecir** | Escribe la salida o el comportamiento esperados **antes** de ejecutar | Nota de predicción |
| **Ejecutar** | Ejecuta con una semilla fija y captura la salida | Comando + transcripción |
| **Comparar** | Compara la predicción con la observación | Líneas exactas que difieren |
| **Explicar** | Dice *por qué* existe la brecha y nombra el supuesto equivocado | Un párrafo, sin palabras de sintaxis |
| **Registrar** | Guarda predicción, diferencia y explicación en `EVIDENCE.md` | Entrada del portafolio |

**Calibración** (calibration): el progreso es la brecha que se encoge entre expectativa y
observación. Una predicción correcta indica un modelo mental que funciona; una incorrecta señala
exactamente lo siguiente que hay que estudiar.

**Mal uso:** predecir después de ver el resultado, o copiar la explicación. La comprobación solo
funciona si la predicción se escribe primero.

### 4.1 Protocolo de verificación

1. **Leer los criterios** antes de escribir código.
2. **Predecir** el resultado y anotarlo.
3. **Compilar.**
4. **Ejecutar** el script de aceptación.
5. **Comparar** el resultado con la predicción.
6. **Diagnosticar** si falla: ¿qué criterio, qué salida, qué causa?
7. **Corregir y volver a ejecutar.**
8. **Registrar** la evidencia, incluida la diferencia de la predicción.
9. **Reflexionar:** ¿qué aprendí? ¿qué no puedo explicar aún?
10. **Decidir** el siguiente paso.

Ningún paso requiere otra persona.

---

## 5. Autodiagnóstico (self-diagnosis)

Cuatro preguntas, sin niveles, sin puntuación:

1. **Explicar:** ¿puedo describir el concepto sin notas y sin palabras de sintaxis?
2. **Cambiar:** ¿puedo aplicar la prueba de cambio a demanda y predecir su coste?
3. **Extender:** ¿puedo usar el concepto en un requisito que no he visto?
4. **Calibrar:** ¿coincidió mi predicción? Si no, ¿qué supuesto era erróneo?

Si alguna respuesta es «todavía no», el siguiente paso es explícito:

- repetir el par de contraste,
- volver a leer el ejemplo resuelto,
- regresar al prerrequisito,
- o preguntarle al material mediante un issue (opcional).

No hay penalización por repetir. Repetir es el método.

---

## 6. Portafolio de evidencia (evidence portfolio)

Un `EVIDENCE.md` por lección, conservado por la persona que aprende:

```markdown
# construction-09 — abstract data type

## Goal
Swap the board representation without touching callers.

## Artifact
`construction/lessons/construction-09-abstract-data-type/code/`

## Prediction
Expected: only `board.cpp` changes; the trace stays identical.
Observed: `board.cpp` and `game.h` changed; trace identical.

## Acceptance
- acceptance-criterion-1 pass — trace matches
- acceptance-criterion-2 pass — only `board.cpp` changed
- acceptance-criterion-3 pass — callers compile
- acceptance-criterion-4 pass — grep returns 0
- acceptance-criterion-5 not yet — I still confuse specification with representation

## Change cost
| Files | Lines | New concepts | Tests broken |
|---|---|---|---|
| 1 | 74 | 2 | 0 |

## Reflection
I understood the swap, but I still explain it with the word "struct".
Next: rewrite the explanation without syntax words, then construction-10.

## Next step
construction-10 — invariants.
```

El portafolio no se entrega. Es el registro de la persona que aprende y, más adelante, una muestra
profesional.

---

## 7. Retroalimentación sin calificador (feedback without a grader)

La retroalimentación es información sobre el artefacto, no un juicio sobre la persona.

### 7.1 Autorretroalimentación

Tres preguntas:

- **Orientar (feed up):** ¿el artefacto cumple el objetivo declarado?
- **Retroalimentar (feed back):** ¿qué criterio falla y con qué salida observada?
- **Proyectar (feed forward):** ¿cuál es el siguiente cambio más pequeño?

### 7.2 Retroalimentación externa opcional

Una persona mentora o un par puede comentar, pero:

- no se requiere,
- no se califica,
- debe citar evidencia (`file:line`, salida, traza),
- la persona que aprende decide si actúa en consecuencia.

### 7.3 Alfabetización de retroalimentación (feedback literacy)

La persona que aprende practica:

- juzgar el trabajo contra los criterios, no contra otra persona,
- separar el síntoma de la causa,
- decidir qué ignorar,
- pedir evidencia cuando la retroalimentación es vaga.

Esta es la habilidad profesional; el material solo aporta el andamiaje.

---

## 8. Progresión (progression)

- **Basada en criterios:** avanza cuando pasan los criterios de aceptación.
- **Sin calendario fijo:** solo tiempo sugerido.
- **Sin penalización por repetir.**
- **Los prerrequisitos son explícitos**, pero la persona que aprende puede intentar cualquier
  peldaño y descubrir el prerrequisito al fallar.
- **Terminado es local:** una lección está terminada para *ti* cuando puedes demostrarla, no cuando
  lo dice un programa de estudios.

---

## 9. Qué se eliminó y por qué (what was removed and why)

| Eliminado | Motivo |
|---|---|
| Exámenes | Miden el recuerdo bajo restricciones artificiales y no se parecen a la aceptación en ingeniería |
| Cuestionarios | Lo mismo; predecir contra una traza ya aporta recuperación con un propósito |
| Notas | Desplazan la atención a la puntuación, fomentan trabajar por la nota e invitan a comparar |
| Puntos, curvas, insignias | Mecanismos de clasificación sin valor de aprendizaje |
| Rúbricas con niveles | Se convierten en notas disfrazadas; los criterios binarios son más claros y menos arbitrarios |
| Puntos de control con compuertas pasa/no pasa | Sustituidos por criterios de aceptación que ejecuta la persona que aprende |
| Trabajo grupal obligatorio | La persona que aprende puede estudiar de forma independiente; la colaboración es opcional y queda fuera del método |
| Fechas límite | Castigan la vida y crean presión artificial |

**Lo que conservamos:** predicción, autoexplicación, contraste, reflexión y práctica repetida. Son
mecanismos de aprendizaje, no ceremonias de evaluación.

---

## 10. Lista de la persona que aprende de forma independiente — artefactos

- [ ] Puedo ejecutar el artefacto sin conexión con un solo comando.
- [ ] Escribí una predicción antes de ejecutar y la comparé con la observación.
- [ ] Todos los criterios de aceptación pasan y tengo la salida.
- [ ] Puedo explicar el concepto sin notas y sin palabras de sintaxis.
- [ ] Puedo aplicar la prueba de cambio y predecir su coste.
- [ ] Escribí la reflexión y el siguiente paso.
- [ ] Sé qué lección viene después (o por qué repito esta).

Si todas las casillas están marcadas, la lección está completa **para ti**. Nadie más necesita
aprobarla.

---

## 11. Para docentes, si hay alguno (opcional)

El material se puede usar en un curso, pero no está diseñado en torno a uno.

- Usa los criterios de aceptación como única evaluación.
- No califiques; da retroalimentación basada en evidencia.
- Pide la predicción antes de la ejecución; la brecha es el momento de enseñanza.
- No exijas trabajo grupal; si lo usas, queda fuera del método.
- Deja que las personas que aprenden elijan la ruta; enseña los prerrequisitos a demanda.
- El docente es un facilitador y un modelo de razonamiento experto, no un calificador.

---

## 12. Comprobación de validez (validity check)

Los criterios de aceptación se pueden sortear. Cuatro salvaguardas:

1. **Predicción:** la persona que aprende escribe el resultado esperado antes de ejecutar; la brecha
   entre predicción y observación es la señal de calibración.
2. **Prueba de cambio:** pasar los criterios no basta; la persona que aprende debe aplicar un cambio
   y medir el coste.
3. **Explicación sin sintaxis:** si solo puede describir el código, el concepto no se ha aprendido.
4. **Transferencia:** aplicar el concepto a un requisito nuevo (la etapa «extender»).

Un criterio que se puede pasar copiando es un mal criterio. Reescríbelo hasta que exija comprensión.

---

## 13. El modelo en una frase

> Predecirlo, construirlo, ejecutar los criterios, comparar con la predicción, explicar la brecha
> sin sintaxis, cambiarlo y decidir qué sigue — a tu propio ritmo, sin nota.
