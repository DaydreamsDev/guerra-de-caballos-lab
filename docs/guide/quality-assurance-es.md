# Control de calidad (QA)

> [English](quality-assurance.md) | [Español](quality-assurance-es.md)

Esta guía es para mantenedores y revisores. Quien aprende no la necesita para usar el laboratorio.

## Alcance

La QA cubre el repositorio, las lecciones, la pedagogía y el código. Una compilación verde no basta:
una lección puede compilar y aun así fallar a quien aprende.

## Tres capas

| Capa | Pregunta | Quién la ejecuta |
|---|---|---|
| **QA estructural** | ¿El repositorio se sostiene? | Comprobaciones automáticas y una persona mantenedora |
| **QA pedagógica** | ¿La lección enseña el concepto con poca fricción? | Una persona revisora independiente |
| **Validación con quien aprende** | ¿Una persona real puede terminarla y explicarla? | Alguien que no sea el autor |

## QA estructural

- Todos los enlaces internos resuelven.
- Cada archivo en inglés tiene su par en español, y al revés.
- `meta.yml`, la cabecera de la lección y el catálogo coinciden en el estado.
- El código compila; la referencia coincide con 20/20 trazas; las trazas de cada lección pasan.
- No se commitea un directorio de compilación, `__pycache__`, archivo temporal ni copia de editor.
- El código cabe en 100 columnas; Markdown cabe en 100 columnas fuera de las tablas.
- La política de evaluación vive solo en las guías canónicas, no en cada lección.
- Cada lección tiene un **Siguiente paso** y un archivo de evidencia.

## Rúbrica de QA pedagógica

Revisa cada dimensión como **pasa / todavía no**. Un pase necesita evidencia, no impresión.

| Dimensión | Pregunta | Evidencia de pase |
|---|---|---|
| **Un concepto** | ¿La lección enseña un solo concepto? | El concepto se nombra una vez y la prueba de cambio lo aísla. |
| **Ajuste al prerrequisito** | ¿Quien aprende puede empezar con lo que enseñó el prerrequisito? | La lección usa solo conceptos anteriores o define el término nuevo antes de usarlo. |
| **Valor auténtico** | ¿La persona produce algo que una ingeniera reconocería? | Un artefacto ejecutable, una traza, un diagnóstico o una revisión, no un cuestionario. |
| **Predicción** | ¿La persona predice antes de ejecutar? | Una predicción escrita y una brecha registrada. |
| **Contraste** | ¿El concepto se ve mediante una diferencia? | Un par con/sin o una prueba de cambio antes/después. |
| **Carga cognitiva** | ¿El camino es corto para la memoria de trabajo? | Un concepto, pasos señalizados, sin repetición redundante. |
| **Explicación** | ¿Puede explicar sin palabras de sintaxis? | Una consigna que pide el mecanismo en lenguaje llano. |
| **Reflexión** | ¿Decide qué sigue? | Una reflexión y un siguiente paso nombrado. |
| **Coherencia** | ¿La lección encaja en su pista y en el catálogo? | Prerrequisitos, vocabulario y enlaces coinciden con la guía de la pista. |
| **Accesibilidad** | ¿El lenguaje es llano y la estructura predecible? | Frases cortas, términos definidos, encabezados consistentes, sin significado solo por color. |

## Validación con quien aprende

Una lección llega a `stable` solo después de que una persona que no es el autor la completa. El
registro debe nombrar:

- qué construyó o diagnosticó;
- qué criterio costó entender;
- dónde necesitó ayuda;
- qué puede explicar después de la lección;
- qué siguiente paso eligió.

Si la persona no puede terminar sin el autor, la lección no es estable.

## Correspondencia con los estados

La política autoritativa está en el [catálogo de lecciones](lesson-catalog-es.md). En resumen:
`drafted` significa que el contenido existe; `in review` significa que la QA estructural pasó y la
revisión pedagógica independiente está abierta; `stable` significa que la revisión y la validación
con quien aprende están cerradas.
