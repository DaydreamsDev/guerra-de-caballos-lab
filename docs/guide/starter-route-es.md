# Ruta inicial

> [English](starter-route.md) | [Español](starter-route-es.md)

> **Para:** personas con poca o ninguna experiencia en ingeniería de software.
> **Tiempo:** 270 minutos (~4 h 30), siete lecciones.
> **Necesitas:** Python 3.10+, Bash 4+, `grep`, `diff`, `cmp`, `sha256sum` y un editor de texto.

## Qué es esta ruta

La ruta inicial es una primera pasada acotada por el laboratorio: siete lecciones en un orden fijo,
cada una con un registro escrito al final. Enseña las tres ideas de las que depende el resto del
laboratorio —una especificación, un arnés determinista y trazas de referencia— sin pedirte que
construyas un programa desde cero ni que leas una base de código grande.

## Qué necesitas

- Python 3.10 o posterior, y Bash 4 o posterior.
- `grep`, `diff`, `cmp` y `sha256sum` (estándar en Linux y macOS).
- Un editor de texto plano.

Sin compilador, sin CMake, sin paso de compilación.

## La ruta

Siete lecciones, en este orden. Los tiempos son estimaciones.

| # | Lección | Tiempo | Concepto | Evidencia |
|---|---|---|---|---|
| 1 | [common-00 — Cómo aprender](../../common/lessons/common-00-how-to-learn/lesson-es.md) | 15 min | Metacognición | [EVIDENCE-es.md](../../common/lessons/common-00-how-to-learn/EVIDENCE-es.md) |
| 2 | [common-01 — El juego como invariante](../../common/lessons/common-01-game-invariant/lesson-es.md) | 50 min | Especificación | [EVIDENCE-es.md](../../common/lessons/common-01-game-invariant/EVIDENCE-es.md) |
| 3 | [common-02 — Arnés determinista](../../common/lessons/common-02-deterministic-harness/lesson-es.md) | 60 min | Arnés | [EVIDENCE-es.md](../../common/lessons/common-02-deterministic-harness/EVIDENCE-es.md) |
| 4 | [common-03 — Glosario](../../common/lessons/common-03-glossary/lesson-es.md) | 20 min | Glosario | [EVIDENCE-es.md](../../common/lessons/common-03-glossary/EVIDENCE-es.md) |
| 5 | [common-04 — Punto de entrada y directorio de trabajo](../../common/lessons/common-04-build-and-run/lesson-es.md) | 30 min | Punto de entrada | [EVIDENCE-es.md](../../common/lessons/common-04-build-and-run/EVIDENCE-es.md) |
| 6 | [common-05 — Trazas de referencia](../../common/lessons/common-05-golden-traces/lesson-es.md) | 45 min | Trazas de referencia | [EVIDENCE-es.md](../../common/lessons/common-05-golden-traces/EVIDENCE-es.md) |
| 7 | [construction-00 — Leer las reglas](../../construction/lessons/construction-00-read-the-rules/lesson-es.md) | 50 min | Especificación | [EVIDENCE-es.md](../../construction/lessons/construction-00-read-the-rules/EVIDENCE-es.md) |

## El flujo

```text
START-HERE-es.md
├─ Laboratorio completo ── secciones 0–2
│                          └─ Pista A o Pista B
└─ Ruta inicial ── 7 lecciones, en el orden de la tabla
                  ├─ salida: siete archivos EVIDENCE-es.md
                  └─ después: Pista A o Pista B
```

## Cómo trabajar cada lección

Todas las lecciones siguen los mismos cinco pasos.

1. **Predice.** Escribe qué esperas que ocurra antes de tocar el teclado.
2. **Ejecuta.** Lanza el comando que da la lección, exactamente como está escrito.
3. **Compara.** Pon predicción y salida una al lado de la otra; usa `diff` si la salida es larga.
4. **Explica.** Di con palabras sencillas por qué difieren, nombrando la línea o el valor que te
   sorprendió.
5. **Registra.** Escribe la predicción, el comando y la comparación en el `EVIDENCE-es.md` de la
   lección.

No escribas la predicción después de ver el resultado.

## Ritmo

Dos sesiones de unas 2 h 15 cada una, o cuatro de aproximadamente 1 h.

## Criterios de salida

- [ ] Siete archivos `EVIDENCE-es.md` están completados, uno por lección.
- [ ] `sha256sum -c checksums.txt` informa los veinte archivos como OK desde `common/traces/`.
- [ ] Una traza se reproduce byte a byte con un `diff` vacío.
- [ ] La misma semilla da la misma transcripción desde dos directorios de trabajo.
- [ ] Una predicción sobre el orden de las jugadas se compara con la transcripción real.
- [ ] Puedes explicar por qué `run.sh` funciona desde cualquier directorio.

## Qué podrás hacer

- Leer una especificación y separar una regla de un valor.
- Ejecutar un programa determinista y demostrar que es reproducible.
- Verificar un artefacto con sumas de comprobación y diffs.
- Leer un programa pequeño y explicar su punto de entrada y el flujo de sus datos.
- Construir un portafolio de evidencia para cada lección.

## Después de la ruta

- La **Pista A** continúa en `construction-01` y requiere C++20.
- La **Pista B** empieza en `archaeology-00`; su comprobación de compilación requiere C++20.

## Preguntas frecuentes

**¿Necesito C++?** No para esta ruta. La Pista A y la Pista B lo requieren al continuar.

**¿Puedo saltarme `common-03`?** Léela cuando un término te bloquee; las lecciones posteriores
asumen su vocabulario.

**¿Es todo el laboratorio?** No. El laboratorio tiene dos pistas completas más allá de esta ruta.
