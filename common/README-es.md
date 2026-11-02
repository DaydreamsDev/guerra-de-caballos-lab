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
common-05 → common-00 → common-01 → common-02 → common-03 → common-04
```

## Lecciones

| ID | Lección | Concepto | Estado |
|---|---|---|---|
| `common-05` | [Cómo aprender](lessons/common-05-how-to-learn/lesson-es.md) | Metacognición | Borrador |
| `common-00` | [El juego como invariante](lessons/common-00-game-invariant/lesson-es.md) | Especificación | Borrador |
| `common-01` | [Arnés determinista](lessons/common-01-deterministic-harness/lesson-es.md) | Reproducibilidad | Borrador |
| `common-02` | [Glosario](lessons/common-02-glossary/lesson-es.md) | Vocabulario compartido | Borrador |
| `common-03` | [Compilar y ejecutar](lessons/common-03-build-and-run/lesson-es.md) | Reproducibilidad de la compilación | Borrador |
| `common-04` | [Trazas de referencia](lessons/common-04-golden-traces/lesson-es.md) | Contrato de comportamiento | Borrador |

`common-00` es el documento más importante del proyecto. Si las reglas son ambiguas, toda
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
