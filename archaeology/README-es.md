# Pista B — Arqueología

> [English](README.md) | [Español](README-es.md)

**Pregunta:** *¿Qué está mal en este código real, por qué importa y cómo debería verse?*

Esta pista usa el código archivado heredado como un sistema real. La persona diagnostica defectos,
explica sus consecuencias, escribe una prueba que falla, aplica la reparación mínima segura y
verifica que el comportamiento se conserve.

La autopsia es sin culpa: primero el contexto, después el juicio. No es una broma ni una burla;
el objetivo es el diagnóstico profesional.

## Contexto

La fuente fue escrita por un estudiante de segundo semestre de ingeniería de sistemas en la
Universidad Nacional de Colombia, Bogotá, para un curso de programación orientada a objetos que
seguía a programación básica. Ese es el único contexto que esta pista necesita; las lecciones se
centran en la ingeniería.

## Qué hace quien aprende

- Lee el código real y su documentación.
- Reproduce un fallo.
- Escribe una prueba que lo capture.
- Aplica la reparación mínima.
- Confirma que las trazas de referencia siguen pasando.
- Registra la evidencia.

## Alcance

El alcance mínimo de la primera entrega es `archaeology-00` a `archaeology-10`, más las primeras 15
tarjetas de fallo. La pista completa está en borrador hasta `archaeology-19`.

## Estado

`archaeology-00` a `archaeology-19` están en borrador. Las lecciones de la pista B están completas.
Las fichas de fallo de la primera entrega son 15 de 15. Ver [Fichas de fallo](cards/README-es.md).

- [Orientación: leer un código heredado](lessons/archaeology-00-orientation/lesson-es.md)
- [Compilar y ejecutar código heredado](lessons/archaeology-01-build-today/lesson-es.md)
- [Inventario de código y grafo de llamadas](lessons/archaeology-02-code-inventory/lesson-es.md)
- [Reconstruir la especificación](lessons/archaeology-03-reverse-spec/lesson-es.md)
- [Fallos de corrección](lessons/archaeology-04-correctness-failures/lesson-es.md)
- [Estado global y acoplamiento oculto](lessons/archaeology-05-global-state/lesson-es.md)
- [Duplicación y el switch de ocho casos](lessons/archaeology-06-duplication-switch/lesson-es.md)
- [Fuga de representación](lessons/archaeology-07-representation-leakage/lesson-es.md)
- [Frontera de módulo ausente](lessons/archaeology-08-module-boundary/lesson-es.md)
- [Tipo abstracto de datos ausente](lessons/archaeology-09-abstract-data-type/lesson-es.md)
- [Polimorfismo ausente](lessons/archaeology-10-polymorphism/lesson-es.md)
- [Entrada, salida y capacidad de prueba](lessons/archaeology-11-input-output/lesson-es.md)
- [Nombres y comentarios](lessons/archaeology-12-naming-comments/lesson-es.md)
- [Documentación técnica para mantenedores](lessons/archaeology-13-documentation/lesson-es.md)
- [Pruebas y determinismo](lessons/archaeology-14-tests-determinism/lesson-es.md)
- **Análisis estático y advertencias del compilador**
  [Abrir](lessons/archaeology-15-static-analysis/lesson-es.md)
- [Dependencia de plataforma](lessons/archaeology-16-platform-lock-in/lesson-es.md)
- [Revisión de código de una función heredada](lessons/archaeology-17-code-review/lesson-es.md)
- [Taller de refactorización](lessons/archaeology-18-refactor-workshop/lesson-es.md)
- [Lista de revisión y retrospectiva](lessons/archaeology-19-review-retrospective/lesson-es.md)

Ver [`../docs/guide/archaeology-track-es.md`](../docs/guide/archaeology-track-es.md) y
[`../docs/guide/lesson-catalog-es.md`](../docs/guide/lesson-catalog-es.md).
