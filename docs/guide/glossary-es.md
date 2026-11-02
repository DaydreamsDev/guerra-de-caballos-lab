# Glosario (glossary)

> [English](glossary.md) | [Español](glossary-es.md)

Fuente única de verdad para la terminología y las traducciones. Para el marco de cuatro capas, la
fuente canónica es [`conceptual-framework.md`](conceptual-framework-es.md); las entradas
siguientes son resúmenes, no redefiniciones.

Formato: **Inglés** — *Español* — definición — concepción errónea habitual (si la hay).

---

## Fundamentos (foundations)

| Inglés | Español | Definición | Concepción errónea |
|---|---|---|---|
| Concept | Concepto | Una decisión más su consecuencia; atómico, portable, independiente del lenguaje | “Un concepto es una palabra clave.” |
| Conceptual framework | Marco conceptual | El modelo de cuatro capas: concepto, paradigma, sintaxis, implementación | “Es solo un glosario.” |
| Layer | Capa | Uno de los niveles del marco; cada uno tiene su propio ciclo de vida y su propia prueba | “Una capa es una sección.” |
| Paradigm | Paradigma | Un conjunto de conceptos con un orden preferido y un sistema de valores | “Un paradigma es un estilo de sintaxis.” |
| Syntax | Sintaxis | El dialecto que te obliga a enfrentarte al concepto | “La sintaxis es el concepto.” |
| Implementation | Implementación | El artefacto ejecutable que encarna una combinación | “El código es el diseño.” |
| Abstraction | Abstracción | Ocultar detalle detrás de una interfaz estable | “Abstracción significa más capas.” |
| Complexity | Complejidad | El coste de entender y cambiar un sistema | “La complejidad son líneas de código.” |

## Paradigmas (paradigms)

| Inglés | Español | Definición | Concepción errónea |
|---|---|---|---|
| Non-structured programming | Programación no estructurada | Flujo de control sin restricciones (p. ej. `goto`), un único ámbito global; también llamada programación monolítica (*monolithic*) | “Solo significa que no hay funciones.” |
| Procedural programming | Programación procedimental | Descomposición en procedimientos o funciones con parámetros | “Es lo mismo que la programación estructurada.” |
| Structured programming | Programación estructurada | Solo secuencia, selección e iteración; una única entrada y una única salida | “Significa usar funciones.” |
| Modular programming | Programación modular | Módulos con separación entre interfaz e implementación y ocultamiento de información | “Significa dividir archivos.” |
| Abstract data type (ADT) | Tipo abstracto de datos (TAD; también TDA) | Una especificación de operaciones independiente de la representación | “Un TAD es una estructura (`struct`).” |
| Object-oriented programming (OOP) | Programación orientada a objetos (POO) | Objetos con identidad, estado, comportamiento y despacho dinámico | “La POO significa herencia.” |
| Generic programming | Programación genérica | Algoritmos parametrizados sobre tipos | “Genérico significa `void*`.” |
| Functional programming | Programación funcional | Computación como funciones puras y valores inmutables | “Funcional significa usar lambdas.” |
| Data-oriented design | Diseño orientado a datos | La disposición en memoria y los patrones de acceso guían el diseño | “Es solo optimización.” |

## Representación de datos (data representation)

| Inglés | Español | Definición | Concepción errónea |
|---|---|---|---|
| Record | Registro | Un valor compuesto con campos con nombre (`struct`) | “Un registro es solo un conjunto de variables.” |
| Array of records (AoS) | Arreglo de estructuras | Una colección almacenada registro a registro | “AoS siempre es más lento.” |
| Struct of arrays (SoA) | Estructura de arreglos | Una colección almacenada campo a campo | “SoA siempre es más rápido.” |
| Parallel arrays | Arreglos paralelos | Varios arreglos indexados por la misma posición | “Es más simple que un registro.” |
| Indirection | Indirección | Acceder a un valor a través de una referencia o una dirección | “La indirección es un puntero.” |
| Reference | Referencia | Un alias a un objeto existente | “Una referencia es un puntero.” |
| Pointer | Puntero | Un valor tipado que contiene una dirección de memoria | “Un puntero es un entero.” |
| Ownership | Propiedad | Responsabilidad sobre el ciclo de vida de un objeto | “La propiedad es la recolección de basura.” |
| Lifetime | Ciclo de vida | El intervalo durante el cual un objeto es válido | “El ciclo de vida es el ámbito.” |
| RAII | RAII (adquisición de recursos en la inicialización) | El ciclo de vida de un recurso atado al ciclo de vida del objeto | “RAII es solo para memoria.” |

## Diseño (design)

| Inglés | Español | Definición | Concepción errónea |
|---|---|---|---|
| Encapsulation | Encapsulación | Proteger un invariante detrás de una interfaz | “La encapsulación es `private`.” |
| Information hiding | Ocultamiento de información | Los clientes no pueden depender de la representación interna | “Ocultar es lo mismo que encapsular.” |
| Inheritance | Herencia | Derivar un tipo de otro | “La herencia es reutilización.” |
| Polymorphism | Polimorfismo | Una interfaz, muchas implementaciones | “El polimorfismo es sobrecarga.” |
| Dynamic dispatch | Despacho dinámico | Elegir la implementación en tiempo de ejecución | “El despacho es un `switch`.” |
| Composition | Composición | Construir comportamiento combinando objetos | “La composición es herencia con pasos extra.” |
| Coupling | Acoplamiento | Grado de dependencia entre las partes | “El acoplamiento siempre es malo.” |
| Cohesion | Cohesión | Grado en que una parte tiene una sola responsabilidad | “La cohesión es lo mismo que el acoplamiento.” |
| Invariant | Invariante | Una propiedad que siempre debe cumplirse | “Un invariante es un comentario.” |
| Contract | Contrato | Precondiciones, postcondiciones, invariantes | “Un contrato es documentación.” |
| Specification | Especificación | El comportamiento que un programa debe exhibir | “El código es la especificación.” |

## Calidad y legado (quality and legacy)

| Inglés | Español | Definición | Concepción errónea |
|---|---|---|---|
| Legacy code | Código heredado | Código difícil de cambiar y sin pruebas | “Heredado significa antiguo.” |
| Symptom | Síntoma | El efecto visible de un fallo | “El síntoma es la causa.” |
| Root cause | Causa raíz | La condición que produce una clase de fallos | “Cada error tiene una sola causa.” |
| Technical debt | Deuda técnica | Una decisión de diseño que encarece los cambios futuros | “La deuda siempre es mala.” |
| Self-review | Autopsia / autorrevisión | Una revisión del trabajo propio pasado, centrada en los sistemas y no en la culpa | “La autorrevisión es autocrítica.” |
| Failure card | Ficha de fallo | Un registro estructurado: evidencia, clase, severidad, causa raíz, arreglo mínimo y estructural, prueba, concepto violado | “Una ficha es una queja.” |
| Refactoring | Refactorización | Un cambio en la estructura que preserva el comportamiento | “Refactorizar es reescribir.” |
| Minimal fix | Arreglo mínimo | El cambio más pequeño que elimina el daño | “Un arreglo mínimo es un parche.” |
| Structural fix | Arreglo estructural | Un cambio que previene la clase de fallo | “Un arreglo estructural es una reescritura.” |
| Code review | Revisión de código | Revisión de un cambio basada en evidencia | “La revisión es control de estilo.” |

## Verificación (verification)

| Inglés | Español | Definición | Concepción errónea |
|---|---|---|---|
| Acceptance criteria | Criterios de aceptación | Condiciones observables y binarias que definen si el artefacto cumple su objetivo | “Los criterios de aceptación son una rúbrica.” |
| Definition of done | Definición de terminado | La lista de comprobación que hace que una lección esté completa para la persona que aprende | “Terminado significa que compila.” |
| Engineering check | Comprobación de ingeniería | Predecir → ejecutar → comparar → explicar → registrar; una comprobación contra la realidad, sin tiempo límite y sin calificación | “Es un cuestionario.” |
| Prediction note | Nota de predicción | El comportamiento esperado, escrito antes de ejecutar el artefacto | “Predecir es adivinar.” |
| Calibration | Calibración | La brecha entre lo que esperabas y lo que ocurrió; una señal de progreso | “La calibración es una nota.” |
| Evidence | Evidencia | Un artefacto ejecutable, una salida, una traza o un informe | “La evidencia es una opinión.” |
| Determinism | Determinismo | La misma entrada y la misma semilla producen la misma salida | “Determinismo significa ausencia de aleatoriedad.” |
| Seed | Semilla | El valor que inicializa una secuencia pseudoaleatoria | “La semilla es una contraseña.” |
| Harness | Arnés (banco de pruebas) | Un ejecutor que corre una variante y captura su comportamiento | “Un arnés es un framework de pruebas.” |
| Golden trace | Traza de referencia | Una salida esperada registrada que se usa para comparar | “Una traza es un registro de log.” |
| Change test | Prueba de cambio | Un requisito que se usa para medir el coste de un diseño | “Una prueba de cambio es una prueba unitaria.” |
| Contrast pair | Par de contraste | Dos versiones ejecutables que difieren en un concepto | “Un par son dos ejemplos.” |
| Evidence portfolio | Portafolio de evidencia | El registro privado de la persona que aprende: artefactos, criterios y reflexiones | “Un portafolio es una entrega.” |

## Reproducibilidad y arnés (reproducibility and harness)

| Inglés | Español | Definición | Concepción errónea |
|---|---|---|---|
| Transcript | Transcripción | El registro completo de una ejecución, un registro por línea | «Una transcripción es un registro de eventos.» |
| Trace | Traza | Una transcripción registrada que se usa como evidencia | «Una traza es una prueba.» |
| Digest | Resumen criptográfico | Una huella de longitud fija de una secuencia de bytes | «Un resumen es cifrado.» |
| Build step | Paso de compilación | Un comando que convierte el código en un artefacto ejecutable | «Todo proyecto necesita uno.» |
| Interpreter | Intérprete | Un programa que ejecuta el código directamente | «Un intérprete es un compilador.» |
| Entry point | Punto de entrada | El archivo o comando que inicia un programa | «Un punto de entrada es una función.» |
| Working directory | Directorio de trabajo | El directorio desde el que se ejecuta un comando | «El directorio de trabajo es la raíz del repositorio.» |
| Location independence | Independencia de ubicación | Un comando funciona desde cualquier directorio | «La independencia de ubicación significa que no hay rutas.» |
| Build reproducibility | Reproducibilidad de la compilación | El mismo código y las mismas herramientas producen el mismo comportamiento | «Significa que no hay compilación.» |
| Reference implementation | Implementación de referencia | Una implementación completa que se usa como estándar | «Es la única implementación.» |

## Diseño de aprendizaje (learning design)

| Inglés | Español | Definición | Concepción errónea |
|---|---|---|---|
| Constructivism | Constructivismo | El conocimiento se construye a partir de la experiencia | “El constructivismo significa ausencia de guía.” |
| Constructionism | Construccionismo | Aprender haciendo artefactos que se pueden compartir | “El construccionismo son solo proyectos.” |
| Heutagogy | Heutagogía | Aprendizaje autodeterminado: la persona que aprende decide qué, cómo y cuándo | “La heutagogía es solo autoestudio.” |
| Self-regulated learning | Aprendizaje autorregulado | Previsión, ejecución y autorreflexión | “La autorregulación es fuerza de voluntad.” |
| Self-determination | Autodeterminación | Motivación mediante autonomía, competencia y relación | “La motivación es una recompensa.” |
| Flow | Flujo | Implicación cuando el desafío supera ligeramente la habilidad | “El flujo es relajación.” |
| Authentic assessment | Evaluación auténtica | Juzgar el desempeño en tareas reales | “Auténtica significa que parece realista.” |
| Feedback literacy | Alfabetización de retroalimentación | La capacidad de juzgar la calidad y actuar sobre la evidencia | “La retroalimentación es una nota.” |
| Ungrading | Sin notas | Quitar las notas y sustituirlas por retroalimentación y evidencia | “Sin notas significa sin estándares.” |
| Criterion-referenced | Referenciado a criterios | Juzgar contra criterios explícitos, no contra otras personas que aprenden | “Criterio significa norma.” |
| Ipsative | Ipsativo | Comparar con el trabajo previo propio | “Ipsativo significa subjetivo.” |
| Self-diagnosis | Autodiagnóstico | Cuatro preguntas: ¿puedo explicarlo, cambiarlo y ampliarlo, y coincidió mi predicción? | “El autodiagnóstico es autocalificarse.” |
| Worked example | Ejemplo resuelto | Un caso resuelto por completo que se estudia antes de resolver | “Los ejemplos son solo para principiantes.” |
| Fading | Desvanecimiento | Retirar el apoyo de forma gradual | “El desvanecimiento es borrar la solución.” |
| Subgoal label | Etiqueta de subobjetivo | Un nombre para una parte significativa de un ejemplo | “Las etiquetas son comentarios.” |
| Cognitive load | Carga cognitiva | La demanda de memoria de trabajo de una tarea | “La carga es dificultad.” |
| Mental model | Modelo mental | La explicación interna que una persona que aprende usa para predecir el comportamiento | “Un modelo mental es una opinión.” |
| Threshold concept | Concepto umbral | Un concepto que transforma la comprensión y es difícil de desaprender | “Umbral significa avanzado.” |
| Misconception | Concepción errónea | Un modelo equivocado estable que resiste a la explicación | “Una concepción errónea es un error.” |
| Formative feedback | Retroalimentación formativa | Información usada para decidir el siguiente paso; nunca calificada | “Formativa significa sin calificación.” |

## Términos escolares rechazados (rejected schooling terms)

| Inglés | Español | Por qué se rechaza |
|---|---|---|
| Exam | Examen | Recuerdo cronometrado bajo restricciones artificiales; no es como se acepta el trabajo de ingeniería. |
| Quiz | Cuestionario | Lo mismo; la predicción contra una traza conserva el mecanismo de aprendizaje sin la ceremonia. |
| Grade | Nota | Desplaza la atención a la puntuación y la comparación; socava la motivación intrínseca. |
| Rubric with levels | Rúbrica con niveles | Se convierte en una nota disfrazada; se sustituye por criterios de aceptación binarios. |
| Checkpoint gate | Punto de control | Un control implica permiso; se sustituye por criterios de aceptación que la persona que aprende ejecuta. |
| Ranking | Ranking / clasificación | Compara personas que aprenden en lugar de evidencia. |
| Forced group work | Trabajo grupal obligatorio | La persona que aprende puede estudiar de forma independiente; la colaboración es opcional y queda fuera del método. |

## Vocabulario del proyecto (project vocabulary)

| Inglés | Español | Definición | Concepción errónea |
|---|---|---|---|
| Invariant (project sense) | Invariante (del proyecto) | El comportamiento del juego que nunca cambia entre variantes | “El código es el invariante.” |
| Rung | Peldaño | Un paso de la escalera de conceptos | “Un peldaño es una carpeta.” |
| Core lesson | Lección del tronco | Una lección obligatoria de la columna vertebral | «Una lección del tronco es opcional.» |
| Optional lesson | Lección opcional | Una lección fuera de la columna vertebral que se puede omitir | «Una lección opcional es obligatoria para el proyecto final.» |
| Capstone | Proyecto final | La lección final que pide defender una decisión de diseño con evidencia | «El proyecto final es un resumen.» |
| Track | Pista | Una ruta de aprendizaje: construcción o arqueología | «Una pista es un nivel de dificultad.» |
| Provenance | Procedencia | El origen, la autoría y la licencia de un artefacto | “La procedencia es un comentario.” |

---

## Regla de mantenimiento

- Añade un término solo cuando una lección lo necesite.
- Una definición por término; sin sinónimos en el campo de definición.
- Si dos términos se confunden a menudo, añade una fila en la columna “Concepción errónea”, no un
  segundo término.
- Las traducciones al español se contrastan con este glosario antes de publicar la edición `es`.
