# Tarjetas de fallos

> [English](README.md) | [Español](README-es.md)

Una tarjeta por cada fallo diagnosticado en el archivo fijado en la etiqueta `v1.0.0`. Cada
tarjeta registra la evidencia, el síntoma, la causa raíz, la consecuencia, una prueba que
detecta el patrón, un arreglo mínimo y un arreglo estructural enlazado a un peldaño de la pista A.

Las tarjetas son evidencia, no reparaciones. El archivo permanece de solo lectura; los arreglos
van en una copia de trabajo o en `reference/`.

## Índice

| # | Tarjeta | Clase | Valor |
|---:|---|---|---|
| 001 | [Límite del bucle](failure-001-loop-bound-es.md) · [EN](failure-001-loop-bound.md) | corrección | El bucle del tablero escribe una casilla después de 64; el límite debe venir del tipo. |
| 002 | [`void main`](failure-002-void-main-es.md) · [EN](failure-002-void-main.md) | corrección | `void main()` no es un punto de entrada estándar; el tipo de retorno es un contrato. |
| 003 | [Semilla en el bucle](failure-003-seed-in-loop-es.md) · [EN](failure-003-seed-in-loop.md) | corrección | Volver a sembrar dentro del bucle de colocación destruye el determinismo. |
| 004 | [Inicialización rota](failure-004-broken-initialization-es.md) · [EN](failure-004-broken-initialization.md) | corrección | Reiniciar el puntero dentro del bucle limpia solo la primera casilla. |
| 005 | [Victoria invertida](failure-005-inverted-win-es.md) · [EN](failure-005-inverted-win.md) | corrección | La condición de victoria está invertida entre la especificación y el código. |
| 006 | [Nombre sin límite](failure-006-unbounded-name-es.md) · [EN](failure-006-unbounded-name.md) | corrección | La lectura `cin >> char[250]` sin límite desborda el búfer del nombre. |
| 007 | [Estado global](failure-007-global-state-es.md) · [EN](failure-007-global-state.md) | diseño | Doce globales mutables (más tres constantes) convierten cada cambio de regla en una edición de varias funciones y cada prueba en una preparación de todo el programa. |
| 008 | [Switch de ocho casos](failure-008-eight-case-switch-es.md) · [EN](failure-008-eight-case-switch.md) | diseño | Una regla de delta se copia en ocho cuerpos de 56–61 líneas; una tabla guarda los 16 enteros una vez. |
| 009 | [Arreglos paralelos](failure-009-parallel-arrays-es.md) · [EN](failure-009-parallel-arrays.md) | diseño | La posición de un caballo se guarda dos veces, como índice plano y como fila/columna, así que las copias pueden desviarse. |
| 010 | [Estructura transparente](failure-010-transparent-struct-es.md) · [EN](failure-010-transparent-struct.md) | diseño | `struct stablero { int mvectab; }` renombra el almacenamiento pero no oculta nada; los llamadores siguen indexándolo. |
| 011 | [Switch por número de jugada](failure-011-switch-on-move-number-es.md) · [EN](failure-011-switch-on-move-number.md) | diseño | El movimiento se selecciona por número de jugada y `if (cab==1)`, no por un tipo de pieza; una pieza nueva edita el bucle. |
| 012 | [E/S dentro de la lógica](failure-012-io-inside-logic-es.md) · [EN](failure-012-io-inside-logic.md) | diseño | `movcab` imprime y espera teclas, así que el núcleo no puede ejecutarse sin consola. |
| 013 | [Números mágicos](failure-013-magic-numbers-es.md) · [EN](failure-013-magic-numbers.md) | diseño | Tamaño, ancho, glifos y límite de tablas son literales sin nombre repartidos por el código. |
| 014 | [Código muerto](failure-014-dead-code-es.md) · [EN](failure-014-dead-code.md) | diseño | Un `cout` de depuración y una guarda `enter` viva añaden ruido; la etiqueta de código muerto del catálogo es incorrecta. |
| 015 | [Dependencia de plataforma](failure-015-platform-lock-in-es.md) · [EN](failure-015-platform-lock-in.md) | robustez | `conio.h`, `iostream.h`, `system("color")`, `system("cls")` y `getch()` atan la compilación a Windows. |

## Orden de lectura

Las primeras seis tarjetas son fallos de corrección: producen un resultado incorrecto o
comportamiento indefinido. Las tarjetas 007–014 son fallos de diseño: el programa funciona,
pero el costo de cambio y de prueba es alto. La tarjeta 015 es un fallo de robustez: la
compilación depende de un solo entorno.

## Correcciones al catálogo

Dos etiquetas del catálogo no coinciden con la fuente fijada. Las tarjetas citan la fuente, no
la abreviatura:

- **failure-011.** El archivo no tiene un switch sobre el tipo de pieza. Tiene un switch sobre
  el número de jugada en `variant-01:239` y nueve sitios `if (cab==1)`.
- **failure-014.** `char enter=13` está viva en `variant-04` (se lee en la línea 71 y se asigna
  desde `getch()` en cuatro sitios). La línea de depuración `cout<<pisadas<<"aqui"` en
  `variant-02:1020` es el residuo real.

La tarjeta 013 también registra que `200` no está en el archivo: es el límite de tablas de la
especificación y el arnés modernos.

## Lecciones

Cada tarjeta apunta a la lección que aporta su prueba o su análisis. La secuencia completa está
en [`../README-es.md`](../README-es.md) y
[`../../docs/guide/archaeology-track-es.md`](../../docs/guide/archaeology-track-es.md).
