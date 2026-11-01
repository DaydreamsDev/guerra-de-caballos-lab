# Pista A — Construcción (construction)

> [English](construction-track.md) | [Español](construction-track-es.md)

> Construye el mismo juego mediante una secuencia de decisiones de diseño. El juego nunca cambia; el
> coste del siguiente cambio es la lección.

## Objetivo y audiencia

**Objetivo:** la persona que aprende puede elegir un diseño y justificarlo por el coste del
siguiente cambio.

**Verificación:** criterios de aceptación, un artefacto ejecutable y un registro de evidencia.

**Audiencia:** desde principiante motivado hasta desarrollador intermedio. No se requiere
experiencia en C++; cada lección enseña la sintaxis mínima que su concepto necesita.

**Requisito previo:** variables, condicionales y bucles. Si la persona que aprende no puede leer un
`for`, empieza con `construction-00`.

El catálogo es la fuente autoritativa de IDs, prerrequisitos, tiempo y estado:
[catálogo de lecciones](lesson-catalog-es.md).

---

## Cómo funciona la pista

Cada lección cambia una decisión de diseño y mantiene fijo el comportamiento. La persona que
aprende:

1. predice el efecto del cambio antes de ejecutar;
2. ejecuta ambas versiones con la misma semilla;
3. compara la predicción con la observación;
4. explica la brecha en lenguaje llano;
5. registra la evidencia y el siguiente paso.

El ciclo es el mismo en cada lección: **predecir → ejecutar → comparar → explicar → registrar**.

## La escalera

Las lecciones se agrupan por el tipo de decisión de diseño que enseñan. El orden dentro de un grupo
es el recomendado; el catálogo tiene los prerrequisitos exactos.

### Escalera central

| ID | Concepto | Prueba de cambio |
|---|---|---|
| construction-00 | Especificación | Predecir el siguiente movimiento de una partida con semilla |
| construction-01 | Flujo de control arbitrario | Añadir «saltar turno»; contar los saltos recableados |
| construction-02 | Flujo de control estructurado | El mismo cambio, localizado; sin `goto` |
| construction-03 | Ámbito y descomposición | Extraer un movimiento sin variables globales |
| construction-04 | Agregación | Añadir `captured` a un caballo |
| construction-05 | Colección y recorrido | Recorrer el tablero sin 64 variables |
| construction-06 | Alias sin direcciones | Mutar un registro a través de un parámetro |
| construction-07 | Indirección y ciclo de vida | Acceder a una casilla por puntero; explicar el paso de fila |
| construction-08 | Ocultamiento de información | Tablero 8×8 → 10×10; solo cambia el módulo del tablero |
| construction-09 | Independencia de la representación | Cambiar la representación; los llamadores compilan |

### Escalera de clases y genéricos

| ID | Concepto | Prueba de cambio |
|---|---|---|
| construction-10 | Validez y ciclo de vida | Escrituras fuera de rango rechazadas; quitar la guarda expone el estado inválido |
| construction-11 | Clase con invariante | El estado privado preserva el invariante; el público lo expone |
| construction-12 | Ligadura dinámica | Añadir una subclase de Piece; cero ediciones en el bucle |
| construction-13 | Composición sobre herencia | Sustituir herencia por delegación; trazas sin cambios |
| construction-14 | Parámetros de tipo | Un generador de movimientos para dos tipos de pieza |
| construction-15 | Restricciones | El mismo motor instancia 8×8 y 10×10; las trazas de 8×8 pasan |

### Lentes

| ID | Concepto | Prueba de cambio |
|---|---|---|
| construction-16 | Propiedad y ciclo de vida | Propiedad con `unique_ptr`; copia eliminada; movimiento disponible |
| construction-17 | Abstracción de la iteración | La misma transcripción tras reemplazar tres recorridos |
| construction-18 | Pureza e inmutabilidad | Deshacer/rehacer gratis mediante el historial del tablero |
| construction-19 | Localidad | 10.000 partidas; perfil medido |
| construction-20 | Puertos y adaptadores | El mismo núcleo, adaptadores de consola y de prueba |
| construction-21 | Estado compartido frente a mensajes | Carrera detectada y eliminada; determinismo conservado |
| construction-22 | Recuperación | Movimiento inválido gestionado sin `exit` |

### Port y proyecto final

| ID | Concepto | Prueba de cambio |
|---|---|---|
| construction-23 | La sintaxis es desechable | Las mismas trazas desde el port de Python |
| construction-24 | Justificación de compromisos | Defender una decisión de diseño con números |

## Ruta recomendada

`construction-00 → construction-01 → construction-02 → construction-03 → construction-04 →
construction-05 → construction-06 → construction-07 → construction-08 → construction-09 →
construction-10 → construction-11 → construction-12 → construction-14 → construction-15`

Después de la columna vertebral, elige dos lentes. `construction-20` es la primera por defecto; si
eliges `construction-18`, lee antes `construction-17`. Luego toma el proyecto final:
`construction-24`.

La ruta es una recomendación, no una compuerta. La columna `Prereq` del catálogo es la autoritativa.

---

## Reglas

1. **Una variable.** Cambia exactamente un aspecto crítico por lección. Todo lo demás se mantiene
   fijo.
2. **El mismo comportamiento.** Todas las variantes pasan las mismas trazas de referencia. Si el
   comportamiento cambia, la lección no es válida.
3. **Medir el coste.** Registra archivos tocados, líneas cambiadas, conceptos introducidos y pruebas
   rotas. El tamaño del artefacto se mide, no se prescribe: las lecciones posteriores son más
   grandes porque el diseño es más grande.
4. **Forzado por la sintaxis.** Si la variante compila sin el concepto, el concepto es decoración.
5. **Predecir antes de ejecutar.** La brecha entre predicción y observación es la lección.

---

## El protocolo del par de contraste

Cada lección de concepto entrega dos versiones ejecutables. Las lecciones de preparación
(`common-00`…`common-05`), el port (`construction-23`) y el proyecto final (`construction-24`) usan
criterios de aceptación en su lugar.

La persona que aprende hace seis cosas:

1. **Predecir** el comportamiento de ambas versiones sin ejecutarlas; escribirlo.
2. **Ejecutar** ambas con la misma semilla; comparar los guiones.
3. **Comparar** la predicción con la observación; nombrar la diferencia exacta.
4. **Diferenciar** la estructura: ¿qué archivos, funciones y datos cambiaron?
5. **Explicar** la diferencia en una frase, sin nombrar la sintaxis.
6. **Medir** la prueba de cambio en ambas.

La lección se completa cuando la persona que aprende puede decir *qué decisión* cambió y *qué
compró*, no *qué palabra clave* apareció.

---

## La prueba de cambio

Una prueba de cambio es un requisito pequeño aplicado a dos versiones. Mide **coste**, no
corrección.

Predice el coste primero y después mídelo. La diferencia entre ambos es la señal de calibración:
quien no puede estimar el coste todavía no ve el acoplamiento.

Registra cuatro números:

| Métrica | Por qué |
|---|---|
| Archivos tocados | Acoplamiento |
| Líneas cambiadas | Radio de impacto |
| Conceptos nuevos requeridos | Coste de aprendizaje |
| Pruebas rotas | Red de seguridad |

Ejemplo: añadir un tercer tipo de pieza.

| Versión | Archivos | Líneas | Conceptos nuevos | Pruebas rotas |
|---|---|---|---|---|
| Un peldaño con switch | 3 | ~120 | 0 | 2 |
| Un peldaño con despacho | 1 nuevo + 1 edición | ~40 | 2 | 0 |

La lección no es «el despacho es mejor». La lección es «este cambio costó 40 líneas en lugar de 120
y compró un concepto nuevo». El siguiente cambio puede invertir el veredicto.

---

## Dónde empezar

- Persona nueva: [START-HERE-es.md](../../START-HERE-es.md).
- Índice de lecciones: [catálogo de lecciones](lesson-catalog-es.md).
- Reglas de evidencia: [verificación](verification-es.md).
- Pedagogía: [cómo se aprende](how-learning-works-es.md).
