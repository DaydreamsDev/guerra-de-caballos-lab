# Guerra de Caballos — Laboratorio de aprendizaje

> [English](README.md) | [Español](README-es.md)

> **Empieza aquí:** [START-HERE-es.md](START-HERE-es.md)

> Un juego. Muchas formas de pensar. El mismo comportamiento, otro diseño.

Este es un laboratorio de aprendizaje construido alrededor de un invariante: el
juego de consola original *Guerra de Caballos*. Cada variante implementa las mismas reglas; solo
cambian las decisiones de diseño. El juego es el vehículo, nunca el tema.

El método de aprendizaje se describe en [Cómo se aprende](docs/guide/how-learning-works-es.md).

## El archivo es la evidencia

El código histórico, sus defectos y su vacío documentado sobre clases viven en el archivo congelado:

```text
https://github.com/DaydreamsDev/DaydreamsDev-guerra-de-caballos-archive
```

Versión fijada:

```text
v1.0.0
```

El archivo es la evidencia. Este laboratorio es la capa moderna de aprendizaje. El laboratorio no
altera el archivo.

Ver [`docs/archive-es.md`](docs/archive-es.md).

## Dos caminos

| Pista | Nombre | Pregunta que responde | Audiencia |
|---|---|---|---|
| **A** | **Construcción** (Construction) | *¿Cómo construyo lo mismo con otra forma de pensar?* | Cualquier persona que quiera conceptos, desde cero hasta avanzado |
| **B** | **Arqueología** (Archaeology) | *¿Qué está mal en este código real, por qué importa y cómo debería verse?* | Juniors, revisores, mantenedores y profesionales |

Las pistas son pedagógicamente independientes pero técnicamente acopladas por el núcleo común: la
especificación, el arnés determinista, las trazas de referencia y el glosario.

## Antes de las pistas

Si tienes poca o ninguna experiencia en ingeniería de software, empieza por la
[Ruta inicial](docs/guide/starter-route-es.md): siete lecciones, sin compilador, unas cuatro horas y
media. Después puedes entrar en la pista A o en la pista B.

## Cómo empezar

1. Sigue [`START-HERE-es.md`](START-HERE-es.md) para el orden guiado.
2. Lee [`docs/guide/conceptual-framework-es.md`](docs/guide/conceptual-framework-es.md) y
   [`docs/guide/how-learning-works-es.md`](docs/guide/how-learning-works-es.md).
3. Empieza el núcleo común con [`common-00`](common/lessons/common-00-how-to-learn/lesson-es.md) —
   cómo aprender — y sigue el núcleo en orden.
4. Elige la pista A o la pista B después del núcleo común. Se puede empezar
   por cualquiera de las dos.

## El invariante

Todas las variantes deben producir la misma transcripción del juego para la misma semilla. Eso es lo
que hace válida la comparación. Sin determinismo y trazas de referencia, las variantes son programas
distintos.

El núcleo común es:

- `common/spec/` — las reglas, escritas una vez, independientes del código.
- `common/harness/` — ejecuta una variante con una semilla y captura la transcripción.
- `common/traces/` — salidas de referencia.
- [`docs/guide/glossary-es.md`](docs/guide/glossary-es.md) — términos canónicos
  en inglés y español.

## Principios de diseño

1. **Una variable a la vez.** Cada lección cambia exactamente un aspecto crítico.
2. **El mismo comportamiento, otro diseño.** El juego no cambia; sí cambia el costo del siguiente
   cambio.
3. **El concepto antes que la sintaxis.** La sintaxis es el dialecto que obliga a enfrentar el
   concepto, no el concepto mismo.
4. **La evidencia antes que la opinión.** Trazas de referencia, criterios de aceptación y costos de
   cambio medibles.
5. **El fallo es dato.** La pista B diagnostica causas y costes.
6. **Artefactos pequeños.** 150–300 líneas por variante didáctica, no el archivo original de 900
   líneas.
7. **Bilingüe por diseño.** Cada documento para quien aprende se publica en inglés y español; ambas
   ediciones son de primera clase.
8. **Predecir antes de ejecutar.** La brecha entre expectativa y observación es la señal de
   aprendizaje.

## Política de idiomas

- Cada documento para quien aprende se publica en inglés y español.
- El inglés usa el nombre de archivo canónico; el español usa el sufijo `-es.md`.
- Ninguno de los dos idiomas es requisito para el otro.
- Los conceptos clave conservan su término en inglés entre paréntesis en la edición española, y su
  término en español entre paréntesis en la edición inglesa.
- Los identificadores de código se mantienen en inglés en ambas ediciones.
- El texto de la licencia MIT permanece en inglés; el README español lo explica.

## Mapa del repositorio

| Ruta | Propósito |
|---|---|
| [`docs/guide/`](docs/guide/) | Guía para quien aprende: conceptos, pedagogía, pistas, catálogo, verificación y glosario. |
| [`docs/archive-es.md`](docs/archive-es.md) | La versión congelada del archivo que referencia el laboratorio. |
| [`common/`](common/) | Especificación, arnés, trazas, glosario y compilación compartidos. |
| [`construction/`](construction/) | Pista A: la escalera de construcción. |
| [`archaeology/`](archaeology/) | Pista B: la autopsia del código heredado y las tarjetas de fallo. |
| [`reference/`](reference/) | La implementación «debería ser» producida por la pista A. |

## Estado actual

- **Guía para quien aprende:** disponible.
- **Núcleo común:** seis lecciones en borrador; arnés de referencia y veinte trazas disponibles.
- **Lecciones de la pista A:** `construction-00` a `construction-24` en borrador; pista A completa.
- **Lecciones de la pista B:** `archaeology-00` a `archaeology-19` en borrador; las lecciones de la
  pista B están completas.
- **Fichas de fallo de la pista B:** 15 de las 15 fichas de la primera entrega en borrador.
- **Versión moderna con clases:** cubierta por `construction-11`, `construction-12` y
  `construction-13`.
- **Implementación de referencia:** implementada; coincide con 20/20 trazas de referencia.
- **Ruta inicial:** disponible; siete lecciones, sin compilador.

El orden que no se debe invertir:

```text
especificación → arnés → trazas de referencia → primera lección → primera persona → más lecciones
```

## Licencia

El proyecto se publica bajo la licencia MIT. Ver [`LICENSE-es.md`](LICENSE-es.md) y
[`CITATION-es.md`](CITATION-es.md).
