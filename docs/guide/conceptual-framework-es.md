# Marco conceptual (conceptual framework)

> [English](conceptual-framework.md) | [Español](conceptual-framework-es.md)

> **Fuente canónica.** Si otro documento parafrasea esto, este documento prevalece. No redefinas las
> cuatro capas en ningún otro lugar; enlaza aquí.

Este es el diferenciador de todo el proyecto. Separa cuatro cosas que la mayoría de los cursos, los
tutoriales y los debates colapsan en una sola.

---

## 1. Las dos máximas (the two maxims)

### 1.1 La máxima epistémica (epistemic maxim)

> **El código es la evidencia, el concepto es la decisión, el paradigma es el mapa,**
> **la sintaxis es el dialecto.**

> **The code is the evidence, the concept is the decision, the paradigm is the map,**
> **the syntax is the dialect.**

Responde: *¿qué es cada capa?*

### 1.2 La máxima pedagógica (pedagogical maxim)

> **La forma de pensar es el objetivo, el paradigma es el mapa, la sintaxis es el gimnasio — y
> ninguno funciona aislado.**

> **The way of thinking is the goal, the paradigm is the map, the syntax is the gym — and none works
> in isolation.**

Responde: *¿para qué sirve cada capa?*

Las dos son complementarias, no redundantes: la primera es descriptiva, la segunda es instructiva.

---

## 2. Las cuatro capas (the four layers)

| Capa | Qué es | Ejemplo | Ciclo de vida | Prueba de identidad | Fallo cuando se confunden |
|---|---|---|---|---|---|
| **Concepto** (concept) | Una decisión más su consecuencia. Atómico, portable, independiente del lenguaje | «Indirección: acceder mediante una dirección para que la disposición pueda cambiar» | **Permanente** — sobrevive a los cambios de lenguaje | ¿Puedes explicarlo sin nombrar un lenguaje? | **Sintaxis como concepto:** «el capítulo de punteros» enseña `*`, no la indirección |
| **Paradigma** (paradigm) | Un paquete de conceptos con un orden preferido y un sistema de valores | La programación orientada a objetos (POO) prefiere la delegación y el despacho dinámico; la modular prefiere el ocultamiento de información | **Décadas** — sobrevive a los ciclos tecnológicos | ¿Puedes nombrar qué optimiza y qué sacrifica? | **Tribalismo de paradigma:** «la programación orientada a objetos es mejor» sin nombrar los conceptos ni el compromiso |
| **Sintaxis** (syntax) | El dialecto que te obliga a enfrentar el concepto | `->`, `virtual`, `#include`, `template` | **Por lenguaje** — sobrevive a una versión, no a una traducción | ¿Desaparece cuando cambias de lenguaje? | **Culto al cargo:** `private:` y la creencia de que la programación orientada a objetos ocurrió |
| **Implementación** (implementation) | El artefacto ejecutable que encarna una combinación | `construction-09-abstract-data-type/` | **Por proyecto / commit** — sobrevive a un commit, no a un rediseño | ¿Pasa las trazas de referencia? | **Código como diseño:** «funciona» usado como argumento completo |

El **ciclo de vida** es la columna clave: te dice qué sobrevive a un cambio. Un concepto sobrevive a
un cambio de lenguaje; un paradigma sobrevive a una década; una sintaxis sobrevive a una versión de
compilador; una implementación sobrevive a un commit. Si tu argumento muere cuando cambia la
sintaxis, nunca se trató del concepto.

---

## 3. Cómo se relacionan (how they relate)

```
Concept  (what decision?)
   └── Paradigm  (what family and values?)
          └── Syntax  (what notation forces it?)
                 └── Implementation  (what evidence?)
```

Cada capa responde a una pregunta distinta:

| Capa | Pregunta | Resultado |
|---|---|---|
| Concepto | ¿Qué decisión, y qué aporta? | Una frase sin lenguaje |
| Paradigma | ¿Qué familia, y qué optimiza? | Un compromiso |
| Sintaxis | ¿Qué notación hace la decisión inevitable? | Una característica |
| Implementación | ¿Qué evidencia lo demuestra? | Un artefacto en ejecución + trazas |

**Aprende de arriba hacia abajo; verifica de abajo hacia arriba.** La persona que aprende parte del
concepto y desciende a la implementación. Quien revisa parte de la evidencia y asciende al concepto.
Las mismas cuatro capas, en dirección opuesta.

### Ninguno funciona aislado (none works in isolation)

| Capa faltante | Qué queda | Nombre |
|---|---|---|
| Concepto sin paradigma | Una buena idea sin familia, sin orden, sin compromiso | Filosofía |
| Paradigma sin sintaxis | Una ideología sin función que obligue | Secta |
| Sintaxis sin concepto | Un dialecto sin decisión detrás | Culto al cargo |
| Implementación sin concepto | Un accidente que da la casualidad de compilar | Coincidencia |

La frase «ninguno funciona aislado» no es retórica: cada capa sin las otras produce un modo de fallo
reconocible.

---

## 4. El diferenciador (the differentiator)

La mayor parte de la enseñanza colapsa las capas:

- «Este es el capítulo de punteros» → **sintaxis presentada como concepto.**
- «Programación orientada a objetos frente a programación funcional» → **debate de paradigmas sin
  nombrar los conceptos.**
- «Código limpio» → **prescripciones de sintaxis sin una decisión.**
- «Funciona en mi máquina» → **implementación usada como evidencia de diseño.**

Este marco las separa, de modo que la persona que aprende puede:

1. **Transferir** un concepto a otro lenguaje y ver qué sobrevive.
2. **Elegir** un paradigma por compromiso, no por moda.
3. **Leer** la sintaxis como evidencia de una decisión, no como la decisión.
4. **Juzgar** una implementación por su comportamiento, no por su familiaridad.

### La prueba de las cuatro preguntas (the four-question test)

Cualquier afirmación sobre código debería sobrevivir estas cuatro preguntas:

1. **Concepto:** ¿qué decisión toma esto, y qué aporta?
2. **Paradigma:** ¿a qué familia pertenece, y qué optimiza esa familia?
3. **Sintaxis:** ¿qué notación obliga a tomar la decisión?
4. **Implementación:** ¿qué evidencia muestra que funciona?

Si una afirmación no puede responder las cuatro, está incompleta. Si solo responde la tercera,
es una opinión de sintaxis.

---

## 5. Ejemplo resuelto: el tablero (worked example: the board)

| Capa | En *Guerra de Caballos* | Forma concreta |
|---|---|---|
| **Concepto** | Independencia de representación: la lógica del juego no debería saber cómo se almacena el tablero | «Un movimiento actualiza una celda; que el tablero sea un arreglo, una lista o un bitboard no es asunto del juego.» |
| **Paradigma** | Tipo abstracto de datos / modular: ocultar la representación detrás de operaciones | Tipo abstracto de datos (TAD; abstract data type) |
| **Sintaxis** | Tipo opaco + funciones libres | `struct Tablero;` + `crear`, `marcar`, `vecinas` |
| **Implementación** | `construction-09-abstract-data-type/` | Pasa las trazas; cambiar la representación modifica solo `board.cpp` |

Segundo ejemplo, polimorfismo:

| Capa | En *Guerra de Caballos* | Forma concreta |
|---|---|---|
| **Concepto** | Enlace tardío: elegir el comportamiento en tiempo de ejecución | «Añadir una pieza sin editar el bucle.» |
| **Paradigma** | Programación orientada a objetos (POO; object-oriented programming) | Interfaz + despacho dinámico (dynamic dispatch) |
| **Sintaxis** | `virtual` / `override` | the `Piece` interface |
| **Implementación** | `construction-12-polymorphism/` | Nueva pieza, cero ediciones en `game` |

El mismo concepto puede aparecer bajo paradigmas y sintaxis distintas. Ese es el punto:
**el concepto es la parte portable.**

---

## 6. Reglas de uso canónicas (canonical usage rules)

1. **Cita las máximas literalmente.** No las parafrasees en las lecciones; enlaza aquí.
2. **Cada ficha de lección de diseño declara las cuatro capas:** `concept`, `paradigm`, `syntax`,
   `artifact`.
3. **Cada ficha de fallo completa nombra el concepto violado** y el paradigma que lo previene — no
   solo la sintaxis que parece incorrecta.
4. **Sin redefiniciones en otro lugar.** Otros documentos pueden resumir, pero enlazan aquí.
5. **La propagación de cambios es unidireccional:** edita este documento primero y luego propaga.
6. **Las traducciones son fijas.** Las versiones en español de arriba también son canónicas; no
   improvises otras nuevas.

---

## 7. Dónde aparece (where it appears)

| Documento | Cómo usa el marco |
|---|---|
| `README.md` | Principios 2–3: el concepto antes que la sintaxis; la evidencia por encima de la opinión |
| `how-learning-works.md` | Por qué la lección se construye sobre el contraste, no sobre la exposición |
| `construction-track.md` | Cada peldaño: concepto, paradigma, sintaxis que obliga, prueba de cambio |
| `archaeology-track.md` | Cada ficha de fallo completa: concepto violado, paradigma que lo previene |
| `lesson-catalog.md` | La ficha de lección de diseño declara las cuatro capas |
| `glossary.md` | El vocabulario de las cuatro capas |

---

## 8. Confusiones frecuentes (common confusions)

| Esto no es aquello | Aclaración |
|---|---|
| Concepto ≠ paradigma | Un concepto es atómico; un paradigma es un paquete con valores y un orden. |
| Paradigma ≠ sintaxis | Un paradigma puede expresarse en muchas sintaxis; una sintaxis puede servir a muchos paradigmas. |
| Sintaxis ≠ implementación | La sintaxis es notación; la implementación es un artefacto en ejecución. |
| Implementación ≠ diseño | «Funciona» es evidencia de comportamiento, no prueba de una decisión. |
| Abstracción ≠ indirección | La abstracción oculta detalle; la indirección accede mediante una referencia o una dirección. A menudo cooperan, pero son conceptos distintos. |
| Modular ≠ varios archivos | La modularidad es ocultamiento de información; la cantidad de archivos es un detalle de empaquetado. |
| Tipo abstracto de datos ≠ struct | Un tipo abstracto de datos es una especificación; un struct es una representación posible. |
| Programación orientada a objetos ≠ herencia | La programación orientada a objetos es identidad, estado, comportamiento y despacho dinámico; la herencia es un mecanismo de reutilización. |
| Genérico ≠ `void*` | Los genéricos preservan la seguridad de tipos y la intención; `void*` borra ambas. |
| Sintaxis familiar ≠ concepto comprendido | Reconocer no es transferir. La prueba es otro lenguaje y un requisito nuevo. |

---

## 9. La prueba de una línea (the one-line test)

> **Nombra la decisión, la familia, el dialecto y la evidencia — o solo estás hablando de código.**

> **Name the decision, the family, the dialect and the evidence — or you are only talking about
> code.**
