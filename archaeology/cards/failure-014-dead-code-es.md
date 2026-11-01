# failure-014 — La etiqueta de código muerto no coincide con la guarda `enter` viva

> [English](failure-014-dead-code.md) | [Español](failure-014-dead-code-es.md)

- **Clase:** diseño
- **Severidad:** baja
- **Concepto violado:** el código que no puede afectar el resultado no debe enviarse; el código
  vivo no debe esconderse detrás de una etiqueta de código muerto
- **Archivo:** etiqueta `v1.0.0`,
  `original/variant-04-pointers-final-2008/source/guerra-de-caballos-pointers-final.cpp`

## Evidencia

- **Variantes:** `variant-04` para `enter`; `variant-02` para la línea de depuración;
  `variant-01` para la rama `default`.
- **Líneas:** `variant-04:31`, `variant-04:71`, `variant-04:880`; `variant-02:1020`;
  `variant-01:696–719`.
- **Código:**

```cpp
char enter=13;                              // variant-04, línea 31
while (jugada!=0&&enter==13)                // línea 71
enter=getch();                              // línea 880, también 905, 934, 961

cout<<pisadas<<"aqui";                      // variant-02, línea 1020
```

`enter` no está muerta. La lee la condición del bucle en la línea 71 y se asigna desde `getch()`
en cuatro sitios. Es un código de tecla global que también actúa como condición de bucle,
inicializado con el valor mágico `13`.

La línea de depuración de `variant-02:1020` es la última sentencia de `impresion` (la función
empieza en la línea 920). Se ejecuta en cada impresión del tablero y añade el carácter de dibujo
del jugador y la palabra `aqui` a la salida.

La rama `default:` de `variant-01:696–719` tampoco está muerta. Es el destino del fall-through
después de un caso fallido, y es el único camino que avanza `i` y detecta un caballo encerrado.
`archaeology-02` la etiqueta como inalcanzable; el recorrido del bucle no respalda esa etiqueta.

## Síntoma

La salida del tablero lleva una marca de depuración. La partida termina cuando el jugador pulsa
cualquier tecla que no sea Enter, porque `enter` es a la vez un código de tecla y una condición
de bucle. Un lector que confía en el catálogo busca código muerto y encuentra código vivo.

El compilador calla. Una compilación de trabajo de las cuatro variantes con
`-Wall -Wextra -Wunused` (tras normalizar `void main` en la copia) no produce ningún diagnóstico
`-Wunused-variable`: los candidatos son nombres de ámbito de archivo con enlace externo, y el
compilador no los reporta. No se encontró ninguna variable local sin usar en las cuatro
variantes.

## Causa raíz

Tres problemas distintos se agrupan bajo una etiqueta:

- **Residuo de depuración.** `cout<<pisadas<<"aqui";` quedó en la impresora del tablero. Se
  ejecuta, así que no es código muerto; es salida no deseada.
- **Una guarda viva con mala forma.** `char enter=13` es un código de tecla global usado como
  condición de bucle. Está viva, pero su significado es un número mágico y su alcance es todo
  el archivo.
- **Una rama mal etiquetada.** La rama `default` es el manejador de trampa, no código muerto. La
  etiqueta vino de un argumento de alcanzabilidad que ignoró el fall-through y el `i++`.

La causa común es analizar por nombre y no por flujo de control. Un `grep` de un nombre sin uso
no puede probar código muerto cuando el nombre tiene enlace externo, y una rama que parece un
`default` no está muerta si el bucle depende de ella.

## Consecuencia

La salida es ruidosa y el comportamiento de salida es sorprendente. Un revisor dedica tiempo a
un no-defecto y puede borrar el manejador de trampa, que es la única forma en que la partida
termina por un caballo encerrado. El defecto real —un código de tecla usado como condición de
bucle— sobrevive a la revisión.

El programa no falla. El costo es ruido, falsa confianza y un borrado riesgoso.

## Prueba que lo detecta

`archaeology/lessons/archaeology-15-static-analysis/run.sh` muestra el método de triaje en un
banco con una variable local muerta:

```text
warnings=1
fixed=ok
match=yes
```

La lección compila con `-Wall -Wextra -Werror`, lee el diagnóstico, verifica la afirmación, la
clasifica y elimina la declaración. Aplicadas al archivo, las mismas banderas producen cero
diagnósticos `-Wunused-variable`. Esa es la frontera de evidencia: el compilador no ve nombres
de ámbito de archivo con enlace externo.

La línea de depuración tiene una comprobación de una línea: `grep -n 'cout<<pisadas<<"aqui"'`
sobre `variant-02`. El uso de `enter` tiene otra: `grep -n '\benter\b'` sobre `variant-04`, que
devuelve la declaración, la condición del bucle y cuatro asignaciones.

## Arreglo mínimo

Elimina la línea de depuración. Sustituye el código de tecla global por un resultado de pausa
explícito:

```cpp
bool pause_for_key();          // devuelve false cuando el jugador pide salir
while (jugada != 0 && pause_for_key())
{
    // ...
}
```

No borres la rama `default`. Conserva el manejador de trampa y valida `i` antes de indexar
`vecjugadas`. Si la rama es realmente inalcanzable en una refactorización posterior, elimínala
con una prueba que demuestre que el camino de trampa se conserva.

## Arreglo estructural

Envía los diagnósticos por un puerto de registro que se compila fuera en las versiones de
producción. Haz que la pausa sea un adaptador de entrada que devuelva un valor, no un carácter
global. Usa un índice comprobado para `vecjugadas` (`std::array::at` o una guarda explícita),
para que el camino de trampa sea una precondición declarada y no un fall-through implícito.

Peldaños de la pista A: [`construction-03-functions`][c03] para entradas y valores de retorno
explícitos, y [`construction-17-stl`][c17] para el acceso comprobado a una secuencia de tamaño
fijo.

[c03]: ../../construction/lessons/construction-03-functions/lesson.md
[c17]: ../../construction/lessons/construction-17-stl/lesson.md

## Confianza

Alta en que `enter` está viva en `variant-04` y en que la línea de depuración está en
`variant-02`. Alta en que el archivo no produce diagnósticos `-Wunused-variable` con
`-Wall -Wextra`. Media en la intención del catálogo: la etiqueta puede describir otra revisión,
pero la fuente fijada no la respalda.

## Nota sobre el archivo

Esta tarjeta es evidencia, no una reparación del archivo. El archivo permanece de solo
lectura; la eliminación de la depuración y el adaptador de pausa van en una copia de trabajo o
en `reference/`.
