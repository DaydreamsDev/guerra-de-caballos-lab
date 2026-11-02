# Núcleo común

> [English](README.md) | [Español](README-es.md)

El núcleo común es el contrato que comparten ambas pistas. Se construye antes de la primera lección.

Dos órdenes, no uno:

**Orden de construcción** — los artefactos, en orden de dependencia:

```text
especificación → arnés → trazas de referencia → primera lección
```

**Orden de aprendizaje** — las lecciones, en el orden en que se toman:

```text
common-00 → common-01 → common-02 → common-03 → common-04 → common-05
```

Si sigues la [ruta inicial](../docs/guide/starter-route-es.md), este núcleo más
`construction-00` es la ruta completa. Las seis lecciones de este núcleo son `core`.

## Lecciones

| ID | Lección | Concepto | Estado |
|---|---|---|---|
| `common-00` | [Cómo aprender](lessons/common-00-how-to-learn/lesson-es.md) | Metacognición | Borrador |
| `common-01` | [El juego como invariante](lessons/common-01-game-invariant/lesson-es.md) | Especificación | Borrador |
| `common-02` | [Arnés determinista](lessons/common-02-deterministic-harness/lesson-es.md) | Reproducibilidad | Borrador |
| `common-03` | [Glosario](lessons/common-03-glossary/lesson-es.md) | Vocabulario compartido | Borrador |
| `common-04` | [Compilar y ejecutar](lessons/common-04-build-and-run/lesson-es.md) | Reproducibilidad de la compilación | Borrador |
| `common-05` | [Trazas de referencia](lessons/common-05-golden-traces/lesson-es.md) | Contrato de comportamiento | Borrador |

`common-01` es el documento más importante del proyecto. Si las reglas son ambiguas, toda
comparación es inválida.

## Plan de directorios

| Ruta | Propósito |
|---|---|
| `common/spec/` | Las reglas, escritas una vez, independientes del código. |
| `common/harness/` | Ejecuta una variante con una semilla y captura la transcripción. |
| `common/traces/` | Salidas de referencia para el conjunto de trazas. |
| [`docs/guide/glossary-es.md`](../docs/guide/glossary-es.md) | Términos canónicos en inglés y español. |

## Verificación de ingeniería

Cada lección con artefacto usa el mismo ciclo:

```text
predecir → ejecutar → comparar → explicar → registrar
```

La brecha entre predicción y observación es la señal de aprendizaje. Nunca se califica.

## Criterios de salida

- Dos compilaciones distintas producen trazas idénticas.
- Una persona nueva puede ejecutar el núcleo común en menos de diez minutos.
- Las reglas, el glosario y el conjunto de trazas alcanzan para sostener la primera lección.

Ver [`../docs/guide/lesson-catalog-es.md`](../docs/guide/lesson-catalog-es.md).
