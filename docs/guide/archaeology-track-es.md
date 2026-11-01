# Pista B — Arqueología (archaeology)

> [English](archaeology-track.md) | [Español](archaeology-track-es.md)

> Toma el código heredado tal como está. Diagnostícalo como un código real. Explica cada fallo por
> su coste en el mundo real. Después muestra cómo debería verse.

## Objetivo y audiencia

**Objetivo:** la persona que aprende puede leer código desconocido, separar síntoma de causa raíz,
priorizar por coste y proponer una mejora respaldada por evidencia.

**Verificación:** fichas de fallo, pruebas y criterios de aceptación.

**Audiencia:** personas junior, revisoras, mantenedoras y profesionales. No se requiere conocimiento
previo de este código base.

**Encuadre:** un postmortem centrado en causas y costes. La pregunta es «¿qué hizo que este código
funcionara, cuánto cuesta ahora y qué cambiaríamos?».

---

## 1. Por qué una autopsia y no una lista de «buenas prácticas»

Una lista de reglas se olvida. Un artefacto real con consecuencias reales, no.

El código heredado nos da cosas que un libro de texto no puede dar:

- **Fallos reales:** error de límite por uno (off-by-one), no determinismo, inicialización rota,
  nombres engañosos.
- **Restricciones reales:** un compilador antiguo, una consola de Windows, sin control de versiones,
  sin pruebas.
- **Deuda de diseño real:** estado global, duplicación, acoplamiento oculto, filtración de
  representación.
- **Problemas reales de compilación y portabilidad:** cabeceras obsoletas, comandos de plataforma,
  comportamiento indefinido.

La persona que aprende practica el trabajo real: leer, diagnosticar, priorizar, corregir, probar,
explicar.

---

## 2. El protocolo de autopsia

Cada lección de la Pista B usa la misma columna de evidencia: predecir → inspeccionar →
verificar → explicar → registrar.

| # | Paso | Salida |
|---|---|---|
| 1 | **Orientación:** leer el punto de entrada, las funciones y el flujo de datos | Mapa de código |
| 2 | **Predecir, compilar y ejecutar:** escribir qué esperas que haga el programa heredado y luego reproducirlo | Predicción + artefacto ejecutable o bloqueo documentado |
| 3 | **Inventario de código:** archivos, funciones, globales y grafo de llamadas | Inventario y grafo de llamadas |
| 4 | **Especificación de comportamiento:** hacer ingeniería inversa de las reglas | Especificación + trazas de referencia |
| 5 | **Catálogo de fallos:** clasificar por causa raíz | Fichas de fallo |
| 6 | **Severidad y coste:** impacto en el trabajo real | Lista priorizada |
| 7 | **Arreglo mínimo:** el cambio más pequeño que elimina el daño | Parche + prueba |
| 8 | **Arreglo estructural:** el cambio de diseño que previene la clase de fallo | Refactorización + enlace al peldaño de la Pista A |
| 9 | **Prueba que lo habría detectado** | Caso de prueba |
| 10 | **Retrospectiva:** qué haríamos hoy y por qué | Nota de reflexión |

**Regla:** nunca saltes del paso 5 al paso 8. Primero el arreglo mínimo; la reescritura es un último
recurso con un precio.

**La comprobación de ingeniería también se aplica aquí.** Antes de ejecutar el programa heredado,
predice qué hará: la salida, la condición de victoria, el fallo. Después de ejecutarlo, explica la
brecha. El mismo hábito de predecir → ejecutar → comparar → explicar, aplicado al código de otra
persona, es cómo las personas revisoras construyen un modelo mental sin adivinar.

---

## 3. Taxonomía de fallos

Cinco clases. La clase determina la lección, no el archivo.

### Corrección

El programa no hace lo que afirma.

Ejemplos en el original:

- `for (i = 0; i <= 64; i++)` sobre un arreglo de 64 elementos: escribe uno más allá del final.
- `void main()`: no es C++ estándar; no es portable.
- `srand(time(NULL))` dentro del bucle de reintentos: colocación no determinista y siembra repetida.
- Inicialización rota en
  `original/variant-03-pointers-dev-2008/source/guerra-de-caballos-pointers-dev.cpp:84-86`:
  `pvectab` se reinicia a `&vectab[0]` en cada iteración, así que solo se limpia el elemento 0.
- Condición de victoria invertida en el precursor: «TRIUNFO PARA LA MIQUINA» cuando el caballo
  come todos los peones.
- `cin >> nombre.vectorNombre` en `char[250]`: lectura sin límite.

### Diseño

El programa funciona, pero la estructura encarece el cambio.

- Estado mutable global (`tablero`, `ordenJugadas`, `jugada`, `ganador`).
- Acoplamiento temporal oculto: `ordenjuego` muta `jugada` e imprime.
- Entrada y salida mezcladas con la lógica: `cout` dentro de `moverJugador`.
- Ocho cuerpos `case` de ~48 líneas, copiados a mano.
- Obsesión por los primitivos / arreglos paralelos: el tablero es `int[64]`, el caballo son cuatro
  variables sueltas.
- Filtración de representación: cada función indexa `tablero[i]`.
- Sin frontera de módulo: una sola unidad de traducción, sin cabeceras.
- Sin tipo abstracto de datos: `struct stablero { int mvectab; }` es un envoltorio transparente
  alrededor de un int (variant-03 y variant-04); variant-02 no tiene tipo de tablero.
- Sin polimorfismo: un `switch` sobre el tipo de movimiento en lugar de despacho.
- Números mágicos: 64, 8, 21, 156, 219.
- Código muerto: `char enter = 13` nunca se reasigna; `cout` de depuración
  `cout << pisadas << "aqui"`.
- Abstracciones mal nombradas: `struct movimiento { char nombre[250]; }` guarda un nombre.

### Robustez

El programa solo funciona en un entorno.

- `conio.h`, `iostream.h`, `system("color 1f")`: dependencia de Windows/Microsoft Visual C++ 6.0.
- Sin validación de entrada.
- Sin gestión de errores; las rutas de fallo hacen `exit` o imprimen y continúan.
- Sin comprobaciones de límites en el búfer de nombre.
- `rand() % 64`: sesgo de módulo (menor, pero es un momento didáctico).

### Práctica profesional

El código funciona, pero el artefacto no es compartible ni mantenible.

- Sin README, sin especificación, sin instrucciones de compilación.
- Sin pruebas, sin arnés, sin determinismo.
- Artefactos binarios presentes en el archivo crudo privado (no publicados): `.exe`, `.obj`, `.pdb`,
  `.ncb`, `.opt`, `.ilk`, `.pch`.
- Archivos de proyecto y de espacio de trabajo del IDE en las carpetas originales (conservados en el
  almacén crudo privado, no publicados); duplicados de dos variantes en el archivo crudo.
- Comentarios que narran lo obvio, mienten o están corruptos por la codificación (CP1252).

---

## 4. Plantilla de ficha de fallo

Cada fallo se documenta como una ficha; el catálogo de la §7 es el índice. Una ficha tiene esta
forma:

```markdown
### failure-001 — Error de límite por uno en la inicialización del tablero

- **Clase:** corrección
- **Concepto violado:** el límite es parte del tipo
- **Paradigma que lo previene:** bucles estructurados + iteración basada en rangos
- **Severidad:** alta (comportamiento indefinido; puede corromper memoria adyacente)
- **Evidencia:** `original/variant-02-records-2008/source/guerra-de-caballos-records.cpp:72`
- **Síntoma:** el bucle escribe `vectab[64]` sobre un arreglo de 64 elementos.
- **Causa raíz:** se escribió `<` como `<=`; el límite se copió de un modelo mental basado en 1.
- **Por qué importa en el trabajo real:** las escrituras fuera de límites son una fuente
  clásica de caídas y errores de seguridad; son silenciosas hasta que dejan de serlo.
- **Arreglo mínimo:** `i < 64`.
- **Arreglo estructural:** usar un bucle basado en rangos o `std::array::fill` para que el
  límite no pueda escribirse mal.
- **Prueba que lo habría detectado:** una compilación con sanitizador
  (`-fsanitize=address`) o una prueba unitaria que compruebe el último elemento.
- **Enlace a la Pista A:** `construction-02` (bucles estructurados) y `construction-05`
  (arreglo de estructuras).
- **Lección:** el límite es parte del tipo; exprésalo una vez.
```

**Los campos son obligatorios.** Una ficha sin evidencia es una opinión.

---

## 5. Escala de severidad

| Nivel | Significado | Acción |
|---|---|---|
| **Crítica** | Comportamiento indefinido, pérdida de datos, seguridad, bloqueo legal | Corregir antes de publicar |
| **Alta** | Comportamiento incorrecto, resultados no reproducibles, incidente difícil | Corregir en el parche mínimo |
| **Media** | Cambio costoso, estructura frágil | Arreglo estructural, planificado |
| **Baja** | Legibilidad, nombres, comentarios | Mejorar al tocar el archivo |

La severidad trata de la **consecuencia**, no de lo feo que se vea el código.

---

## 6. Tono y ética

- **Sin culpa.** Este código se escribió para un compilador antiguo bajo las restricciones de su
  época. Juzga la decisión, no a la persona.
- **Primero el contexto.** Explica la restricción antes de juzgar la decisión.

---

## 7. El catálogo de fallos

Estos son los fallos de mayor valor para el trabajo real. Cada uno se convierte en una lección o en
parte de una.

| ID | Fallo | Clase | Por qué importa |
|---|---|---|---|
| failure-001 | `i <= 64` sobre un arreglo de 64 elementos | corrección | Corrupción de memoria silenciosa |
| failure-002 | `void main()` | corrección | Portabilidad; comportamiento indefinido |
| failure-003 | `srand(time(NULL))` dentro de un bucle de reintentos | corrección | No determinismo; pruebas inestables |
| failure-004 | Inicialización de punteros rota en `_dev` | corrección | Comportamiento incorrecto que parece plausible |
| failure-005 | Condición de victoria invertida en el precursor | corrección | La especificación y el código no coinciden |
| failure-006 | Entrada de nombre sin límite | corrección | Desbordamiento de búfer |
| failure-007 | Estado mutable global | diseño | Los incidentes son difíciles de reproducir; pruebas imposibles |
| failure-008 | Ocho cuerpos `case` copiados a mano | diseño | Cada cambio de regla toca ocho lugares |
| failure-009 | Arreglos paralelos / variables sueltas del caballo | diseño | Los datos pueden desincronizarse; invariantes no aplicables |
| failure-010 | `struct stablero { int mvectab; }` no es un tipo abstracto de datos | diseño | La representación se filtra; no es posible sustituirla |
| failure-011 | `switch` sobre el número de jugada e `if (cab == 1)` | diseño | Una jugada nueva edita el bucle |
| failure-012 | Entrada y salida dentro de la lógica de movimiento | diseño | Núcleo no testeable; adaptador imposible |
| failure-013 | Números mágicos | diseño | El significado se pierde; los cambios son propensos a error |
| failure-014 | `cout` de depuración y guarda `enter` viva | diseño | Ruido; la etiqueta del catálogo es incorrecta |
| failure-015 | `conio.h` / `system("color")` | robustez | Dependencia de plataforma |
| failure-016 | Sin pruebas, sin arnés, sin semilla | práctica profesional | Sin red de seguridad para regresiones; sin comparación |
| failure-017 | Sin README, especificación ni instrucciones de compilación | práctica profesional | Nadie puede compilarlo ni confiar en él |
| failure-018 | Binarios y archivos de IDE en las carpetas de trabajo originales | práctica profesional | Artefactos obsoletos; no reproducible desde el código fuente |
| failure-019 | Comentarios que narran o mienten | práctica profesional | Deuda de mantenimiento; documentación falsa |

---

## 8. Secuencia de lecciones

| ID | Lección | Enfoque |
|---|---|---|
| archaeology-00 | Orientación: leer un código heredado | Mapa |
| archaeology-01 | Compilar y ejecutar código heredado | Compilar |
| archaeology-02 | Inventario de código y grafo de llamadas | Inventario |
| archaeology-03 | Ingeniería inversa de la especificación a partir del código | Especificar |
| archaeology-04 | Fallos de corrección (failure-001…failure-006) | Diagnosticar |
| archaeology-05 | Estado global y acoplamiento oculto (failure-007) | Diagnosticar |
| archaeology-06 | Duplicación y el switch de ocho casos (failure-008) | Diagnosticar |
| archaeology-07 | Filtración de representación y arreglos paralelos (failure-009, failure-010) | Diagnosticar |
| archaeology-08 | Frontera de módulo ausente | Diagnosticar |
| archaeology-09 | Tipo abstracto de datos ausente | Diagnosticar |
| archaeology-10 | Polimorfismo ausente (failure-011) | Diagnosticar |
| archaeology-11 | Entrada y salida y testeabilidad (failure-012) | Diagnosticar |
| archaeology-12 | Nombres y comentarios (failure-019) | Diagnosticar |
| archaeology-13 | Documentación técnica para mantenedores | Diagnosticar |
| archaeology-14 | Pruebas y determinismo (failure-016) | Diagnosticar |
| archaeology-15 | Análisis estático y advertencias del compilador | Diagnosticar |
| archaeology-16 | Dependencia de plataforma (failure-015) | Diagnosticar |
| archaeology-17 | Revisión de código de una función heredada | Diagnosticar |
| archaeology-18 | Taller de refactorización: heredado → peldaño de la Pista A | Refactorizar |
| archaeology-19 | Lista de verificación de revisión y retrospectiva final | Revisar |

---

## 9. Correspondencia de fallos con la Pista A

| Clase de fallo | Peldaño de la Pista A que lo previene |
|---|---|
| Error de límite por uno, bucles | `construction-02` bucles estructurados |
| Estado global, acoplamiento | `construction-08` modular, `construction-09` tipo abstracto de datos |
| Arreglos paralelos, variables sueltas | `construction-04` registros, `construction-05` arreglo de estructuras |
| Filtración de representación | `construction-09` tipo abstracto de datos |
| Switch sobre el tipo | `construction-12` polimorfismo, `construction-14` genérico |
| Entrada y salida mezcladas con la lógica | `construction-20` (hexagonal) |
| Sin determinismo / pruebas | `common-01` arnés |
| Dependencia de plataforma | `construction-08` modular + `construction-20` hexagonal |
| Artefacto mal etiquetado | `common-02` glosario + cada par de contraste |

La correspondencia es bidireccional: la Pista B dice *qué duele*; la Pista A dice *qué elimina el
dolor*.

---

## 10. Triaje sistémico

No corrijas los fallos uno por uno. Agrúpalos por causa raíz y corrige la clase.

Ejemplo: `i <= 64`, `j <= 7`, el 64 mágico y los bucles de tablero duplicados no son cuatro errores.
Son una sola causa raíz: **el tamaño del tablero no es un tipo**. Un solo cambio
(`std::array<Cell, N>` o una constante con nombre y un bucle por rangos) elimina la clase.

La persona que aprende practica el movimiento profesional: **encontrar la clase, no la instancia**.

---

## 11. Lo que la Pista B deliberadamente no hace

- No reescribe todo a «C++ moderno».
- No trata el estilo como un fallo sin consecuencia.
- No separa el código de su contexto.
- No publica material privado.

---

## 12. Producto mínimo viable (alcance)

`archaeology-00` hasta `archaeology-10` + las primeras 15 fichas de fallo. Eso basta para probar el
formato y cubrir corrección, diseño y las principales brechas profesionales. La pista completa está
ahora en borrador hasta `archaeology-19`; las quince fichas están en borrador.
